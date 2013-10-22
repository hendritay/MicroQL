#include "stdafx.h"
#include "Select.h"
#include "QueryPlan.h"
#include "Database/TableResult.h"
#include "Database/TableDictionary.h"
#include "Database/MQLColumn.h"
#include "Database/MQLCondition.h"

#include <map>
#include <string>
Select :: Select(){ }

Select :: Select(TableDictionary * tdPtr1){tdPtr = tdPtr1;}

bool Select :: evaluateQuery(string query) 
{
	
	tableResultMap.clear();
	columnMap.clear();
	conditionMap.clear();
	queryPlans.clear();

	// *** 1. normalization ***
	// need to do what?

	// *** 2. parse the query & generate query plan ***
	// split the query
	vector<string> tokens = splitString(query, " ");
	vector<string> * tokensPtr = &tokens;
	TableResult * trPtr ;
	TableResult tr;
	//trPtr = &tr;
	int queryPlanType;
	int * queryPlanTypePtr = &queryPlanType;
	// check for syntax and store in a query plan
	if(!parseSyntaxAndType(query, tokensPtr, queryPlanTypePtr))
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
	evaluateQueryPlan(queryPlanTypePtr);
	

	// *** return a copy of the resultTable values ***
	return true;
	//return *trPtr;

}
void Select :: removeRedundancy()
{
	// change thoery/concept to code
	QueryPlan * qpPtr = &queryPlans.at(0);
	// remove exact duplicated where conditions (a = b AND a = b)
	// remove a = a
	// remove duplicated where conditions (a = " " AND " " = a, a = b AND b = a)
	// p and p <=> true

	// always false
	// a = "123" and a = "abc"

	// if either is false, all false a = b and b = c 

}

void Select :: evaluateQueryPlan(int * queryPlanTypePtr)
{
	QueryPlan * qpPtr = &queryPlans.at(0);
	evaluateWhere(qpPtr, queryPlanTypePtr);
	evaluateJoin(qpPtr, queryPlanTypePtr);

	// only when there is no inner join & where
	if(*queryPlanTypePtr == SF)
		evaluateSelect(qpPtr);
}
void Select :: evaluateWhere(QueryPlan * qpPtr, int * queryPlanTypePtr)
{	
	string oper = "=";

	// create MQLCondition for each condition in WHERE
	for(vector<SelectCondition> :: iterator it = qpPtr->conditions.begin(), end = qpPtr->conditions.end(); it != end;)
	{
		//"" = a / "" = a
		if(it->isLeftStr || it->isRightStr)
		{
			
			string tableName;
			string leftColName = it->leftValue;
			string rightColName = it->rightValue;
			if(it->isLeftStr)
			{
				tableName = tdPtr->getTableByColumnName(rightColName);	
			}
			else
			{
				tableName = tdPtr->getTableByColumnName(leftColName);
			}
			
			// find whether exsiting tableResults exist
			map<string, vector<MQLCondition> > :: iterator iter1 = conditionMap.find(tableName);

			if(iter1 != conditionMap.end())
			{
				if(it->isLeftStr)
				{
					MQLColumn lefCol(leftColName, CT_CONSTANT_STRING);
					MQLColumn rightCol(rightColName, CT_VARCHAR);
					MQLCondition con = MQLCondition(lefCol,oper, rightCol);
					iter1->second.push_back(con);
				}
				else
				{
					MQLColumn lefCol(leftColName, CT_VARCHAR);
					MQLColumn rightCol(rightColName, CT_CONSTANT_STRING);
					MQLCondition con = MQLCondition(lefCol,oper, rightCol);
					iter1->second.push_back(con);
				}
			}
			else
			{
				if(it->isLeftStr)
				{
					MQLColumn lefCol(leftColName, CT_CONSTANT_STRING);
					MQLColumn rightCol(rightColName, CT_VARCHAR);
					MQLCondition con = MQLCondition(lefCol,oper, rightCol);
					vector<MQLCondition> v1;
					v1.push_back(con);
					conditionMap.insert(pair<string, vector<MQLCondition>> (tableName, v1));
				}
				else
				{
					MQLColumn lefCol(leftColName, CT_VARCHAR);
					MQLColumn rightCol(rightColName, CT_CONSTANT_STRING);
					MQLCondition con = MQLCondition(lefCol,oper, rightCol);
					vector<MQLCondition> v1;
					v1.push_back(con);
					conditionMap.insert(pair<string, vector<MQLCondition>> (tableName, v1));
				
				}
			
			}
		}

		// a = b (both from same table)
		else
		{
			string tableName = tdPtr->getTableByColumnName(it->leftValue);

			map<string, vector<MQLCondition> > :: iterator iter1 = conditionMap.find(tableName);

			if(iter1 != conditionMap.end())
			{

				MQLColumn lefCol(it->leftValue, CT_VARCHAR);
				MQLColumn rightCol(it->rightValue, CT_VARCHAR);
				MQLCondition con = MQLCondition(lefCol,oper, rightCol);
				iter1->second.push_back(con);
			}
			else
			{

				MQLColumn lefCol(it->leftValue, CT_VARCHAR);
				MQLColumn rightCol(it->rightValue, CT_VARCHAR);
				MQLCondition con = MQLCondition(lefCol, oper, rightCol);
				vector<MQLCondition> v1;
				v1.push_back(con);
				conditionMap.insert(pair<string, vector<MQLCondition>> (tableName, v1));
			}
		}
		
	}	// end of for


	// loop through Select, add each column to the correct table
	for(vector<string> :: iterator it = qpPtr->projections.begin(), end = qpPtr->projections.end(); it != end; ++it)
	{
		string tableName = tdPtr->getTableByColumnName(*it);	

		map<string, vector<MQLColumn> > :: iterator iter = columnMap.find(tableName);
		if(iter != columnMap.end())
		{
			MQLColumn col(*it, CT_VARCHAR);
			iter->second.push_back(col);
		}
		else
		{
			MQLColumn col(*it, CT_VARCHAR);
			vector<MQLColumn> v;
			v.push_back(col);
			columnMap.insert(pair<string, vector<MQLColumn>> (tableName, v));
		}
	}	// end of for
	
	// Select *
	if(qpPtr->projections.size() == 0)
	{
		for(int i =0; i < qpPtr->selections.size(); i++)
		{
			string tableName = qpPtr->selections.at(i);
			TableDefinition td = tdPtr->getTableDefinition(tableName);
			int noOfCol = td.getNoOfColumn();
			for(int in = 0; in < noOfCol; in++)
			{
				vector<MQLColumn> v;
				v.push_back(td.getColumnAt(in));
				columnMap.insert(pair<string, vector<MQLColumn>> (tableName, v));
			}
		}
	}
	/*
	if(*queryPlanTypePtr == SFJW)
	{
		for(vector<SelectCondition> :: iterator it = qpPtr->joins.begin(), end = qpPtr->joins.end(); it != end; ++it)
		{
			string leftTableName, rightTableName;
			//leftTableName = getTableNameByColumn(it->leftValue);	
			//rightTableName = getTableNameByColumn(it->rightValue);	
			map<string, vector<MQLColumn> > :: iterator iter = columnMap.find(leftTableName);
			if(iter != columnMap.end())
			{
				MQLColumn col(it->leftValue, CT_VARCHAR);
				iter->second.push_back(col);
			}
			else
			{
				MQLColumn col(it->leftValue, CT_VARCHAR);
				vector<MQLColumn> v;
				v.push_back(col);
				columnMap.insert(pair<string, vector<MQLColumn>> (leftTableName, v));
			}

			iter = columnMap.find(rightTableName);
			if(iter != columnMap.end())
			{
				MQLColumn col(it->rightValue, CT_VARCHAR);
				iter->second.push_back(col);
			}
			else
			{
				MQLColumn col(it->rightValue, CT_VARCHAR);
				vector<MQLColumn> v;
				v.push_back(col);
				columnMap.insert(pair<string, vector<MQLColumn>> (rightTableName, v));
			}
		}	// end of for
	}
	*/
	
	if(*queryPlanTypePtr == SFW)
	{
		for(map<string, vector<MQLColumn>> :: iterator it = columnMap.begin(), end = columnMap.end(); it != end; ++it)
		{
			map<string, vector<MQLCondition> > :: iterator iter = conditionMap.find(it->first);
			
			TableResult tr;
			tr.loadResult(it->first, it->second, iter->second);	
			finalResult = tr;

		}	// end of for

	}
	else
	{
		// load data into TableResult
		if(columnMap.size() == conditionMap.size())
		{
			for(map<string, vector<MQLColumn>> :: iterator it = columnMap.begin(), end = columnMap.end(); it != end; ++it)
			{
				map<string, vector<MQLCondition> > :: iterator iter = conditionMap.find(it->first);
			
				TableResult tr;
				tr.loadResult(it->first, it->second, iter->second);	// add 
				tableResultMap.insert(pair<string, TableResult>(it->first, tr));

			}	// end of for
		}
		else
		{
			int i =0;
		}

	}

	//  a = b (both are not in same table) does not exist in WHERE clause. Only allowed in INNER JOIN 
	/*
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
		

	}	// end of for
	*/
}

void Select :: evaluateJoin(QueryPlan * qpPtr, int *queryPlanTypePtr)
{

	// a join b join c. For 1st joining there are 3 ways, which to pick? doesn't matter the order, same number of checking needs to be done
	// 1 way to optimize it. if it's PK and some column. Those columns that PK has joined with, mark them USED. if num of USED == the some column no. of rows. exit

	// merge() tr3 = merge(tr1, tr2)
	// tr3.container
	// tr3.addRows(tr row, tr2.row(3))
	// remove column

	if(*queryPlanTypePtr == SFJ)
	{
		for(vector<string> :: iterator temp = qpPtr->projections.begin(), end = qpPtr->projections.end(); temp != end; ++temp)
		{
			string tableName = tdPtr->getTableByColumnName(*temp);	

			map<string, vector<MQLColumn> > :: iterator temp1 = columnMap.find(tableName);
			if(temp1 != columnMap.end())
			{
				MQLColumn col(*temp, CT_VARCHAR);
				temp1->second.push_back(col);
			}
			else
			{
				MQLColumn col(*temp, CT_VARCHAR);
				vector<MQLColumn> v;
				v.push_back(col);
				columnMap.insert(pair<string, vector<MQLColumn>> (tableName, v));
			}
		}	// end of for

		// Select *
		if(qpPtr->projections.size() == 0)
		{
			for(int i =0; i < qpPtr->selections.size(); i++)
			{
				string tableName = qpPtr->selections.at(i);
				TableDefinition td = tdPtr->getTableDefinition(tableName);
				int noOfCol = td.getNoOfColumn();
				for(int in = 0; in < noOfCol; in++)
				{
					vector<MQLColumn> v;
					v.push_back(td.getColumnAt(in));
					columnMap.insert(pair<string, vector<MQLColumn>> (tableName, v));
				}
			}
		}
		/*
		for(vector<SelectCondition> :: iterator it = qpPtr->joins.begin(), end = qpPtr->joins.end(); it != end; ++it)
		{
			string leftTableName, rightTableName;
			//leftTableName = getTableNameByColumn(it->leftValue);	
			//rightTableName = getTableNameByColumn(it->rightValue);	
			map<string, vector<MQLColumn> > :: iterator iter = columnMap.find(leftTableName);
			if(iter != columnMap.end())
			{
				MQLColumn col(it->leftValue, CT_VARCHAR);
				iter->second.push_back(col);
			}
			else
			{
				MQLColumn col(it->leftValue, CT_VARCHAR);
				vector<MQLColumn> v;
				v.push_back(col);
				columnMap.insert(pair<string, vector<MQLColumn>> (leftTableName, v));
			}

			iter = columnMap.find(rightTableName);
			if(iter != columnMap.end())
			{
				MQLColumn col(it->rightValue, CT_VARCHAR);
				iter->second.push_back(col);
			}
			else
			{
				MQLColumn col(it->rightValue, CT_VARCHAR);
				vector<MQLColumn> v;
				v.push_back(col);
				columnMap.insert(pair<string, vector<MQLColumn>> (rightTableName, v));
			}
		}	// end of for
		*/

		// load data into TableResult
		for(map<string, vector<MQLColumn>> :: iterator it = columnMap.begin(), end = columnMap.end(); it != end; ++it)
		{
			map<string, vector<MQLCondition> > :: iterator iter = conditionMap.find(it->first);
			
			TableResult tr;
			tr.loadResult(it->first, it->second, iter->second);
			tableResultMap.insert(pair<string, TableResult>(it->first, tr));

		}	// end of for
	}

	vector<pair<string, TableResult> > pairVec;
	bool firstJoin = false;
	// inner join bet. 2 tables, call merge
	for(vector<SelectCondition> :: iterator it = qpPtr->joins.begin(), end = qpPtr->joins.end(); it != end;)
	{
		
		string leftTableName, rightTableName;
		leftTableName = tdPtr->getTableByColumnName(it->leftValue);
		rightTableName = tdPtr->getTableByColumnName(it->rightValue);

		// load both sides to each tableResult
		MQLColumn leftCol(it->leftValue, CT_VARCHAR);
		MQLColumn rightCol(it->rightValue, CT_VARCHAR);

		vector<MQLCondition> cons;
		vector<MQLColumn> cols;
		cols.push_back(leftCol);
		TableResult tr1, tr2;
		
		tr1.loadResult(leftTableName, cols, cons);
		cols.clear();
		cols.push_back(rightCol);
		tr2.loadResult(rightTableName, cols, cons);
		
		// get existing tableResults
		map<string, TableResult > :: iterator iter = tableResultMap.find(leftTableName);
		map<string, TableResult > :: iterator iter1 = tableResultMap.find(rightTableName);

		TableResult * tr1Ptr = &iter->second, * tr2Ptr= &iter1->second;
		TableResult tr3;
		//tr3.merge(tr1Ptr, tr2Ptr);
		
		// loop through and check for equal values
		for(int i = 0; i < tr1.getNoOfRows(); i++)
		{
			string left = tr1.getValueAt(i, 0);
			for(int in = 0; in <tr2.getNoOfRows(); in++)
			{
				if(left.compare(tr2.getValueAt(i,0)) == 0)
				{
					tr3.addMergedRow(tr1Ptr, i, tr2Ptr, in);
				}
			}
		}

	}	// end of for

	for(map<string, TableResult> :: iterator it = tableResultMap.begin(), end = tableResultMap.end(); it != end; ++it)
	{
		finalResult = it->second;
	}

}
void Select :: evaluateSelect(QueryPlan * qpPtr)
{
	// 1. Query Plan -> SELECT FROM  
	// Select from 1 table only

	// loop through Select, add each column to the correct table
	string tableName;
	map<string, vector<MQLColumn> > :: iterator iter = columnMap.find(tableName);

	for(vector<string> :: iterator it = qpPtr->projections.begin(), end = qpPtr->projections.end(); it != end; ++it)
	{
		tableName = tdPtr->getTableByColumnName(*it);			
		if(iter != columnMap.end())
		{
			MQLColumn col(*it, CT_VARCHAR);
			iter->second.push_back(col);
		}
		else
		{
			MQLColumn col(*it, CT_VARCHAR);
			vector<MQLColumn> v;
			v.push_back(col);
			columnMap.insert(pair<string, vector<MQLColumn>> (tableName, v));
		}
	}	// end of for

	// Select *
	if(qpPtr->projections.size() == 0)
	{
		for(int i =0; i < qpPtr->selections.size(); i++)
		{
			string tableName = qpPtr->selections.at(i);
			TableDefinition td = tdPtr->getTableDefinition(tableName);
			int noOfCol = td.getNoOfColumn();

			for(int in = 0; in < noOfCol; in++)
			{
				vector<MQLColumn> v;
				v.push_back(td.getColumnAt(in));
				columnMap.insert(pair<string, vector<MQLColumn>> (tableName, v));
			}
		}
	}

	TableResult tr;
	vector<MQLCondition> v;
	tr.loadResult(tableName, iter->second, v);
	finalResult = tr;

}
/*
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
*/
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
		string tableName = tdPtr->getTableByColumnName(qp.projections.at(i));
		bool found = false;

		for(int in = 0; in < qp.selections.size(); in++)
		{
			if(qp.selections.at(in).compare(tableName) == 0)
			{
				found = true;
				break;
			}
		}	// end of for
		if(!found)
		{
			return false;
		}

	}	// end of for

	// check WHERE clause
	for(int i = 0; i < qp.conditions.size(); i++)
	{
		
		string leftTableName, rightTableName;
		bool found = false, bothNotStr = false;
		if(!qp.conditions.at(i).isLeftStr)
		{
			string leftTableName = tdPtr->getTableByColumnName(qp.conditions.at(i).leftValue);
			for(int in = 0; in < qp.selections.size(); in++)
			{
				if(qp.selections.at(in).compare(leftTableName) == 0)
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
			string rightTableName = tdPtr->getTableByColumnName(qp.conditions.at(i).rightValue);
			for(int in = 0; in < qp.selections.size(); in++)
			{
				if(qp.selections.at(in).compare(rightTableName) == 0)
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
				return false;
			}
			if(!qp.conditions.at(i).isLeftStr)
				bothNotStr = true;
		}

		if(bothNotStr)
		{
			//  a = b (both are not in same table) does not exist in WHERE clause
			if(leftTableName.compare(rightTableName) != 0)
			{
				return false;
			}
		}
	}

	// if there is inner join, check that 1 is from table A, 1 is from table B
	for(int i = 0; i < qp.joins.size(); i++)
	{
		string leftTableName = tdPtr->getTableByColumnName(qp.joins.at(i).leftValue); 
		string rightTableName = tdPtr->getTableByColumnName(qp.joins.at(i).rightValue);
		if(leftTableName.compare(rightTableName) == 0)
		{
				return false;
		}
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

bool Select :: parseSyntaxAndType(string query, vector<string> * tokensPtr, int * queryPlanTypePtr)
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
	bool hasJoin = false, hasWhere = false;
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
					hasJoin = true;
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
					hasWhere = true;
				}
			}
		}

	}	// end of for
	if(!isAlrightToEnd)
	{
		return false;
	}
	if(!hasJoin && !hasWhere)
		*queryPlanTypePtr = SF;
	else if (hasJoin && hasWhere)
		*queryPlanTypePtr = SFJW;
	else if (hasJoin)
		*queryPlanTypePtr = SFJ;
	else
		*queryPlanTypePtr = SFW;
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