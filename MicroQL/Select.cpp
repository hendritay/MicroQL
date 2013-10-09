#include "stdafx.h"
#include "Select.h"
#include "QueryPlan.h"
#include "Database/TableResult.h"

#include <string>
Select :: Select(){ }

TableResult Select :: evaluateQuery(string query) 
{
	// *** 1. normalization ***
	// need to do what?

	// *** 2. parse the query & generate query tree ***
	// split the query
	vector<string> tokens = splitString(query, " ");
	vector<string> * tokensPtr = &tokens;
	vector<int> clausePos;
	vector<int> * clausePosPtr = &clausePos;
	// check for syntax and generate query tree
	if(!parseSyntaxAndType(query, tokensPtr, clausePosPtr))
		throw "Invalid Syntax or Type used";

	// check query tree for schema
	if(!parseSemantics())
		throw "Invalid Semantics";

	// *** 3. remove redundancy ***
	// need to use rules and check if it fulfils 1 rule, change it to the simplified version


	// *** 4. generate the query plan ***
	generateQueryPlan();


	// *** evaluate the query ***
	// create empty ResultTable and have a pointer point to it
	TableResult tb; //new TableResult();
	TableResult * tbPtr = &tb;

	// if there is where clause
	//		evaluateWhere(resultTablePtr);
	// if there is join clause
	//		evaluateJoin(resultTablePtr);
	// evaluateSelect(resultTablePtr);

	// *** return a copy of the resultTable values ***
	return tb;

}




void Select :: generateQueryPlan()
{
	// create a query plan
	QueryPlan queryPlan;
	queryPlans.push_back(queryPlan);

	// loop through the Query Tree and add to Query Plan
	// addProjection(columnName);
	// addSelection(tableName);
	// addCondition(conditionList);
}

bool Select :: parseSemantics()
{
	// check whether relation/attribute doesnt exist, whether tree is disjoint
	// check for WHERE a.b (a is relation, b is attribute, is that syntax or schema?
	return true;
}

bool Select :: parseSyntaxAndType(string query, vector<string> * tokensPtr, vector<int> * clausePosPtr)
{
	// ** ASSUME : No renaming of relation name. Spaces bet. ,

	// select * from a
	// select *, b from a
	// select * from a where b = c
	// select * from a where b = ""
	// select * from a inner join b on a.c = b.c
	vector<string> compulsoryClauses, optionalClauses;
	compulsoryClauses.push_back("SELECT"); // follow by */a/r.a
	compulsoryClauses.push_back("FROM"); // follow by r a
	optionalClauses.push_back("INNER JOIN"); 
	optionalClauses.push_back("WHERE");

	int in = 0;
	bool expectClause = true;
	bool canBeClause = false;
	bool hasColma = false;
	bool isFirstValue = true;
	bool checkComplusory = true;
	bool isInnerJoin = false;
	for(int i = 0; i < tokensPtr->size(); i++)
	{
		string currentValue = tokensPtr->at(i);
		string upperValue = currentValue;
		stringToUpper(upperValue);
		
		if(expectClause)
		{
			if(checkComplusory)
			{
				if(upperValue.compare(compulsoryClauses.at(in)) != 0)
				{
					return false;
				}
			}
			else
			{
				if(upperValue.compare("INNER") == 0)
				{
					i++;
					if(i < tokensPtr->size())
					{
						string currentValue1 = tokensPtr->at(i);
						string upperValue1 = currentValue1;
						stringToUpper(upperValue1);
						if(upperValue1.compare("JOIN") != 0)
						{
							return false;
						}
					}
					else
					{
						return false;
					}
					i++;
					if(i < tokensPtr->size())
					{
						string currentValue1 = tokensPtr->at(i);
						int pos = currentValue1.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
						if(pos != -1)	
						{
							return false;
						}
						
					}
					else
					{
						return false;
					}
					i++;
					if(i < tokensPtr->size())
					{
						string currentValue1 = tokensPtr->at(i);
						string upperValue1 = currentValue1;
						stringToUpper(upperValue1);
						if(upperValue1.compare("ON") == 0)
						{
							expectClause = false;
						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
					isInnerJoin = true;
					isFirstValue = false;
				}
				else if (upperValue.compare("WHERE") == 0)
				{
					isInnerJoin = false;
					isFirstValue = false;
				}
				else if (!isFirstValue && upperValue.compare("AND") == 0)
				{

				}
				else
				{
					return false;
				}
			}
			// CREATE NODE INSTEAD
			clausePosPtr->push_back(i);
			in++;
			if(in >= compulsoryClauses.size())
			{
				// finish checking for compulsoryClauses
				in = 0;
				checkComplusory = false;
				isFirstValue = true;
			}
			expectClause = false;
		}
		else
		{
			if(checkComplusory)
			{
				// check whether there is , or it is ,
				vector<string> temp = splitString(currentValue, ",");
				if(temp.size() > 0)
				{
					// found ,
					hasColma = true;
				}
				else if(currentValue.compare(",") == 0)
				{
					hasColma = true;
				}
				else
				{
					hasColma = false;
				}
				// this should be a value

				// after SELECT value
				if(in == 1)
				{
					// follow by */a/r.a
					string value;
					if (hasColma)
					{
						value = temp[0];
					}
					else
					{
						value = currentValue;
					}
					if(value.compare("*") == 0 && isFirstValue)
					{
						if(hasColma)
						{
							return false;
						}
						isFirstValue  = false;
					}
					else
					{
						if(!isAttributeOrRelationDotAttribute(value))
						{
							return false;
						}
					}
				
				}
				// after FROM value
				else if (in == 2)
				{
					// follow by r
					string value;
					if (hasColma)
					{
						value = temp[0];
					}
					else
					{
						value = currentValue;
					}

					int pos = value.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
					if(pos != -1)	
					{
						// there exist some weird values
						return false;
					}

				}
				if(!hasColma)
				{
					expectClause = true;
				}
			}
			// !checkComplusory
			else
			{
				if(isInnerJoin)
				{
					// this must be R.b = R1.d / b = d
					if(!isAttributeOrRelationDotAttribute(currentValue))
					{
						return false;
					}
					i++;
					if(i < tokensPtr->size())
					{
						string currentValue1 = tokensPtr->at(i);
						if(currentValue1.compare("=") != 0)
						{
							return false;
						}
					}
					else
					{
						return false;
					}
					i++;
					if(i < tokensPtr->size())
					{
						string currentValue1 = tokensPtr->at(i);
						if(!isAttributeOrRelationDotAttribute(currentValue1))
						{
							return false;
						}
						
					}
					else
					{
						return false;
					}
					
				}
				else
				{
					// can be R.b = R1.d/ a.b = "" / "" = a.b
					if(currentValue.compare("\"") == 0)
					{
						// find end of "
						string a;
						while(i < tokensPtr->size())
						{
							currentValue = tokensPtr->at(i);
							if(currentValue.compare("\"") != 0)
							{
								a += currentValue;
							}
							else
							{
								break;
							}
						}
					}
					else
					{
						if(!isAttributeOrRelationDotAttribute(currentValue))
						{

							return false;
						}
					}
					i++;
					if(i < tokensPtr->size())
					{
						string currentValue1 = tokensPtr->at(i);
						if(currentValue1.compare("=") != 0)
						{
							return false;
						}
					}
					else
					{
						return false;
					}
					i++;
					if(i < tokensPtr->size())
					{
						string currentValue1 = tokensPtr->at(i);
						if(currentValue.compare("\"") == 0)
						{
							// find end of "
							string a;
							while(i < tokensPtr->size())
							{
								currentValue1 = tokensPtr->at(i);
								if(currentValue1.compare("\"") != 0)
								{
									a += currentValue1;
								}
								else
								{
									break;
								}
							}
						}
						else
						{
							if(!isAttributeOrRelationDotAttribute(currentValue1))
							{

								return false;
							}
						}

					}
					else
					{
						return false;
					}
				}
			}
		}

	}	// end of for
	return true;
}


// *** other additional functions ***
bool Select :: isAttributeOrRelationDotAttribute(string value)
{
	int pos = value.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	if(pos != -1)	
	{
		// if it is R.A, no problem
		if(value.at(pos) == '.')
		{
			// check that behind 
			int secPos = value.find_last_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
			if(pos != secPos)
			{
				// there exist some other weird values
				return false;
			}
		}
	}
	return true;
}

vector<string> Select::splitString(string toBeSplit, string delimiters)
{
	vector<string> tokens;

	char * cToBeSplit = (char *)toBeSplit.c_str();
	char * cDelimiters = (char *)delimiters.c_str();

	char * pch;
	pch = strtok (cToBeSplit, cDelimiters);

	while (pch != NULL)
	{
		string token(pch);
		tokens.push_back(token);
		pch = strtok (NULL, cDelimiters);
	}

	return tokens;
}

void Select::stringToUpper(string &s)
{
	for(unsigned int l = 0; l < s.length(); l++)
	{
		s[l] = toupper(s[l]);
	}
}