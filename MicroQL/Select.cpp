#include "stdafx.h"
#include "Select.h"
#include "QueryPlan.h"
#include "Database/TableResult.h"
#include "Database/TableDictionary.h"

#include <map>
#include <string>
Select :: Select(){ }

bool Select :: evaluateQuery(string query) 
{
	// *** 1. normalization ***
	// need to do what?

	// *** 2. parse the query & generate query plan ***
	// split the query
	vector<string> tokens = splitString(query, " ");
	vector<string> * tokensPtr = &tokens;
	TableResult * trPtr ;
	TableResult tr;
	//trPtr = &tr;

	// check for syntax and generate query plan
	if(!parseSyntaxAndType(query, tokensPtr))
		return false;
		//throw "Invalid Syntax or Type used";

	// check query plan for schema
	if(!parseSemantics())
		throw "Invalid Semantics";

	// *** 3. remove redundancy ***
	// need to use rules and check if it fulfils 1 rule, change it to the simplified version
	removeRedundancy();

	// *** 4. generate the other query plans ***
	//generateQueryPlan();


	// *** evaluate the query ***
	evaluateQueryPlan(trPtr);


	// *** return a copy of the resultTable values ***
	return true;
	//return *trPtr;

}
void Select :: removeRedundancy()
{
	// change thoery/concept to code
	QueryPlan * qpPtr = &queryPlans.at(0);
	// remove exact duplicated where conditions (a = b AND a = b)
	// remove duplicated where conditions (a = " " AND " " = a, a = b AND b = a)
}

void Select :: evaluateQueryPlan(TableResult *& tbPtr)
{
	map<string, TableResult> tableResults;
	map<string, TableResult> * tableResultsPtr = &tableResults;
	QueryPlan * qpPtr = &queryPlans.at(0);
	evaluateWhere(tableResultsPtr,qpPtr);
	evaluateJoin(tableResultsPtr,qpPtr);
	evaluateSelect(tableResultsPtr, qpPtr);
}
void Select :: evaluateWhere(map<string, TableResult> * tableResultsPtr, QueryPlan * qpPtr)
{	
	
	// evaluate "" = a / "" = a
	for(vector<SelectCondition> :: iterator it = qpPtr->conditions.begin(), end = qpPtr->conditions.end(); it != end;)
	{
		if(it->isLeftStr || it->isRightStr)
		{

			
			string tableName;

			// TODO: find the tableName that leftValue/rightValue exist
			if(it->isLeftStr)
			{
				//tableName
			}
			else
			{
				//tableName
			}

			// find whether exsiting tableResults exist
			map<string, TableResult > :: iterator iter = tableResultsPtr->find(tableName);
			if(iter != tableResultsPtr->end())
			{
				//iter->second.loadResult(tableName, *it);
			}
			else
			{
				TableResult tb();
				//tableResultsPtr->insert(pair<string, TableResult>(tableName, tb));
				//iter->second.loadResult(tableName, *it);
			}
			// erase
			qpPtr->conditions.erase(it++);


		}
		else
		{
			++it;
		}
		
	}	// end of for

	// evaluate a = b
	for(vector<SelectCondition> :: iterator it = qpPtr->conditions.begin(), end = qpPtr->conditions.end(); it != end;)
	{
		// TODO: find the tableName that leftValue,rightValue exist
		string leftTableName, rightTableName;


		map<string, TableResult > :: iterator iter = tableResultsPtr->find(leftTableName);
		map<string, TableResult > :: iterator iter1 = tableResultsPtr->find(rightTableName);
		if(iter != tableResultsPtr->end())
		{
			if(iter1 != tableResultsPtr->end())
			{
				// get entire col of iter & iter1
				// loop through col of iter
					// loop through col of iter1
						// if both are equal
							// save rowPtr/num of both
			}
			else
			{
				// create new tableResult, load data
				TableResult tb();
				//tableResultsPtr->insert(pair<string, TableResult>(rightTableName, tb));
				//tb.second.loadResult(rightTableName);

				// copy loop
			}
		}
		else
		{
			// create new tableResult, load data
			TableResult tb();
			//tableResultsPtr->insert(pair<string, TableResult>(leftTableName, tb));
			//tb.second.loadResult(leftTableName)

			if(iter1 != tableResultsPtr->end())
			{
				// loop
			}
			else
			{
				// create new tableResult, load data
				TableResult tb1();
				//tableResultsPtr->insert(pair<string, TableResult>(rightTableName, tb1));
				//tb1.second.loadResult(rightTableName)

				// loop
			}
		}
		

	}
}

void Select :: evaluateJoin(map<string, TableResult> * tableResultsPtr, QueryPlan * qpPtr)
{
	// inner join bet. 2 tables, call merge
	for(vector<SelectCondition> :: iterator it = qpPtr->joins.begin(), end = qpPtr->joins.end(); it != end;)
	{
		// TODO: find the tableName that leftValue,rightValue exist
		string leftTableName, rightTableName;
		// find exsiting tableResults 
		map<string, TableResult > :: iterator iter = tableResultsPtr->find(leftTableName);
		map<string, TableResult > :: iterator iter1 = tableResultsPtr->find(rightTableName);
		if(iter != tableResultsPtr->end())
		{
			if(iter1 != tableResultsPtr->end())
			{
				//iter->second.loadResult(rightTableName, iter1->second);
				//remove right tableResult
				//tableResultsPtr->erase(iter1);
			}
		}
	}
}
void Select :: evaluateSelect(map<string, TableResult> * tableResultsPtr, QueryPlan * qpPtr)
{
	// can select from diff tables
	//tupleList = tableResultsPtr->at(0).getResult(ColumnList); // must return in the order of the column list
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
	// check whether whether tree is disjoint 
	// (right now the only situation is: there is some attribute in SELECT / WHERE that does not belong to any tables in FROM/INNER JOIN)


	// check for Select a from b where c = "" whether a & c is from table b
	// check for Select a from b inner join c on d = e where f = "" . a,d,e,f must be from c OR d
	
	QueryPlan qp = queryPlans.at(0);
	
	// check SELECT clause
	for(int i = 0; i < qp.projections.size(); i++)
	{
		// TODO: find table that this column belongs to
		string tableName;
		bool found = false;

		for(int in = 0; in < qp.selections.size(); in++)
		{
			if(qp.selections.at(in).compare(tableName) == 0)
			{
				found = true;
				break;
			}
		}
		if(!found)
		{
			return false;
		}

	}	

	// check WHERE clause
	for(int i = 0; i < qp.conditions.size(); i++)
	{
		
		string tableName;
		bool found = false;
		if(!qp.conditions.at(i).isLeftStr)
		{
			// TODO: find table that this column belongs to
			//tableName = 
			for(int in = 0; in < qp.selections.size(); in++)
			{
				if(qp.selections.at(in).compare(tableName) == 0)
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
				return false;
			}
		}
		found = false;
		if(!qp.conditions.at(i).isRightStr)
		{
			// TODO: find table that this column belongs to
			//tableName = 
			for(int in = 0; in < qp.selections.size(); in++)
			{
				if(qp.selections.at(in).compare(tableName) == 0)
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
				return false;
			}
		}

	}

	// if there is inner join, check that 1 is from table A, 1 is from table B
	for(int i = 0; i < qp.joins.size(); i++)
	{
		
		string leftTableName, rightTableName;
		// TOOD: get tableName of this column
		//leftTableName = qp.joins.at(i).leftValue;
		//rightTableName = 
		// if(leftTableName.compare(rightTableName) == 0)
		// {
		//		return false;
		// }
		int leftIndex = -1, rightIndex = -1;
		for(int in = 0; in < qp.selections.size(); in++)
		{
			if(qp.selections.at(in).compare(leftTableName) == 0)
			{
				leftIndex = in;
			}
			if(qp.selections.at(in).compare(rightTableName) == 0)
			{
				rightIndex = in;
			}
		}
		if(leftIndex == rightIndex || leftIndex == -1 || rightIndex == -1)
		{
			return false;
		}
		

	}
	return true;
}

bool Select :: parseSyntaxAndType(string query, vector<string> * tokensPtr)
{
	// ** ASSUME : No renaming of relation name. Spaces bet. ,
	// ** only cater for 1 inner join

	// select * from a
	// select *, b from a
	// select * from a where b = c
	// select * from a where b = ""
	// select * from a inner join b on a.c = b.c (R.A is not allowed for now)


	QueryPlan qp;
	queryPlans.push_back(qp);

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
	bool isAlrightToEnd = false;

	for(int i = 0; i < tokensPtr->size(); i++)
	{
		string currentValue = tokensPtr->at(i);
		string upperValue = currentValue;
		stringToUpper(upperValue);
		
		if(expectClause)
		{
			if(in >= compulsoryClauses.size())
			{
				// finish checking for compulsoryClauses
				in = 0;
				checkComplusory = false;
				isFirstValue = true;
			}

			if(checkComplusory)
			{
				if(upperValue.compare(compulsoryClauses.at(in)) != 0)
				{
					return false;
				}
				in++;
			}
			else
			{
				isAlrightToEnd = false;
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
						/*
						if(tdPtr->tableExists(currentValue1))
						{
							qp.selections.push_back(currentValue1);
						}
						else
						{
							return false;
						}
						*/
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
			
			
			expectClause = false;
		}
		else
		{
			if(checkComplusory)
			{
				// check whether there is , or it is ,
				int pos = currentValue.find(',');
				vector<string> temp; 
				if(pos != -1)
				{
					// found ,
					hasColma = true;
					temp = splitString(currentValue, ",");
				}
				else if(currentValue.compare(",") == 0)
				{
					hasColma = true;
					temp = splitString(currentValue, ",");
				}
				else
				{
					hasColma = false;
				}
				// this should be a value

				// after SELECT value
				if(in == 1)
				{
					// follow by */a
					string value;
					if (hasColma)
					{
						if(temp.size() > 1)
						{
							for(int index = 0; index < temp.size(); index++)
							{
								if(temp.at(index).find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1)
								{
									return false;
								}
								/*
								if(tdPtr->columnExists(temp.at(index)))
								{
									qp.projections.push_back(temp.at(index));
								}
								else
								{
									return false;
								}
								*/
							}
							hasColma = false;
						}
						else
						{
							value = temp[0];
						}
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
						if(value.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1)
						{
							return false;
						}
						/*
						if(tdPtr->columnExists(value))
						{
							qp.projections.push_back(value);
						}
						else
						{
							return false;
						}
						*/
					}

					if(!hasColma)
					{
						expectClause = true;
					}
					isAlrightToEnd = false;

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
					else
					{
						qp.selections.push_back(value);
					}
					if(!hasColma)
					{
						expectClause = true;
						isAlrightToEnd = true;
					
					}
					else
					{
						isAlrightToEnd = false;
					}
				}
			
				
			}
			// !checkComplusory
			else
			{
				
				if(isInnerJoin)
				{
					// this must be b = d
					if(currentValue.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1)
					{
						return false;
					}
					/*
					if(!tdPtr->columnExists(currentValue))
					{
						return false;
					}
					*/

					i++;
					string currentValue1;
					if(i < tokensPtr->size())
					{
						currentValue1 = tokensPtr->at(i);
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
					string currentValue2 ;
					if(i < tokensPtr->size())
					{
						currentValue2 = tokensPtr->at(i);
						if(currentValue2.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1)
						{
							return false;
						}
						/*
						if(!tdPtr->columnExists(currentValue2))
						{
							return false;
						}
						*/
					}
					else
					{
						return false;
					}
					SelectCondition con(currentValue,false, "=", currentValue2, false);
					qp.joins.push_back(con);
					isAlrightToEnd = true;
					expectClause = true;
				}
				else
				{
					bool isLeftStr, isRightStr;
					// can be b = a/ a = "" / "" = a
					if(currentValue.compare("\"") == 0)
					{
						// find end of "
						string a;
						i++;
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
							i++;
						}
						currentValue= a;
						isLeftStr = true;
					}
					// "abc"
					else if (currentValue.find_first_of("\"") !=  currentValue.find_last_of("\""))
					{
						isLeftStr = true;
					}
					else
					{
						if(currentValue.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1)
						{
							return false;
						}
						/*
						if(!tdPtr->columnExists(currentValue))
						{
							return false;
						}
						*/
						isLeftStr = false;
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
					string currentValue2;
					if(i < tokensPtr->size())
					{
						
						currentValue2 = tokensPtr->at(i);
						if(currentValue2.compare("\"") == 0)
						{
							// find end of "
							string a;
							i++;
							while(i < tokensPtr->size())
							{
								currentValue2 = tokensPtr->at(i);
								if(currentValue2.compare("\"") != 0)
								{
									a += currentValue2;
								}
								else
								{
									break;
								}
								i++;
							}
							currentValue2= a;
							isRightStr = true;
						}
						// "abc"
						else if (currentValue2.find_first_of("\"") !=  currentValue2.find_last_of("\""))
						{
							isRightStr = true;
						}
						else
						{
							if(currentValue2.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1)
							{
								return false;
							}
							/*
							if(!tdPtr->columnExists(currentValue2))
							{
								return false;
							}
							*/
							isRightStr = false;
						}

					}
					else
					{
						return false;
					}
					SelectCondition con(currentValue,isLeftStr, "=", currentValue2, isRightStr);
					qp.conditions.push_back(con);
					isAlrightToEnd = true;
					expectClause = true;
				}
			}
		}

	}	// end of for
	if(!isAlrightToEnd)
	{
		return false;
	}
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