#include "stdafx.h"
#include "Select.h"
#include "QueryPlan.h"
#include "Database/TableResult.h"
#include "Database/TableDictionary.h"
#include "Database/MQLColumn.h"
#include "Database/MQLCondition.h"

#include <map>
#include <string>
#include <set>
#include <sstream>

//Select :: Select(){ }

Select :: Select(TableDictionary * tdPtr1, BTree *bTreePtr1){tdPtr = tdPtr1; bTreePtr = bTreePtr1;}

TableResult * Select :: evaluateQuery(string query) 
{
	
	tableResultMap.clear();
	columnMap.clear();
	conditionMap.clear();

	// *** 2. parse the query & generate query plan ***
	// split the query
	vector<string> tokens = splitString(query, " ");
	vector<string> * tokensPtr = &tokens;
	TableResult * trPtr;
	int queryPlanType;
	int * queryPlanTypePtr = &queryPlanType;
	isEmptyResult = false;

	// check for syntax and store in a query plan
	if(!parseSyntaxAndType(query, tokensPtr, queryPlanTypePtr))
		return NULL;
		//throw "Invalid Syntax or Type used";

	// 3. check semantics, remove redundancy
	if(!parseSemantics(queryPlanTypePtr))
		if(!isEmptyResult)
			return NULL;
		else
		{
			TableResult tr(tdPtr, bTreePtr);
			trPtr = &tr;
			return trPtr;
		}

	// *** evaluate the query ***
	evaluateQueryPlan(queryPlanTypePtr);
	
	trPtr = &finalResult;
	if(trPtr == NULL)
	{
		TableResult tr(tdPtr, bTreePtr);
		trPtr = &tr;
	}
	// *** return a copy of the resultTable values ***
	//return true;
	return trPtr;

}

void Select :: evaluateQueryPlan(int * queryPlanTypePtr)
{
	//QueryPlan * qpPtr = &queryPlans.at(0);
	if(*queryPlanTypePtr != SF)
	{
		if(*queryPlanTypePtr != SFJ)
			evaluateWhere(queryPlanTypePtr);
		if(*queryPlanTypePtr != SFW)
			evaluateJoin(queryPlanTypePtr);
	}
	// only when there is no inner join & where
	if(*queryPlanTypePtr == SF)
		evaluateSelect();
}
void Select :: evaluateWhere(int * queryPlanTypePtr)
{	
	string oper = "=";

	// create MQLCondition for each condition in WHERE
	for(vector<SelectCondition> :: iterator it = qp.conditions.begin(), end = qp.conditions.end(); it != end;++it)
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
	for(vector<string> :: iterator it = qp.projections.begin(), end = qp.projections.end(); it != end; ++it)
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
	if(qp.projections.size() == 0)
	{
		for(int i =0; i < qp.selections.size(); i++)
		{
			string tableName = qp.selections.at(i);
			TableDefinition td = tdPtr->getTableDefinition(tableName);
			int noOfCol = td.getNoOfColumn();

			vector<MQLColumn> v;
			for(int in = 0; in < noOfCol; in++)
			{
				v.push_back(td.getColumnAt(in));
			}
			columnMap.insert(pair<string, vector<MQLColumn>> (tableName, v));
		}
	}
	else
		{
			map<string, vector<MQLColumn> > :: iterator temp1;
			for(vector<SelectCondition> :: iterator it = qp.joins.begin(), end = qp.joins.end(); it != end; ++it)
			{
				string leftTableName = tdPtr->getTableByColumnName(it->leftValue);
				string rightTableName = tdPtr->getTableByColumnName(it->rightValue);	

				temp1 = columnMap.find(leftTableName);
				if(temp1 != columnMap.end())
				{
					MQLColumn col(it->leftValue, CT_VARCHAR);
					bool found = false;
					for(int i = 0; i < temp1->second.size(); i++)
					{
						if(temp1->second.at(i).getColumnName().compare(leftTableName) == 0)
						{
							found= true;
							break;
						}
					}
					if(!found)
					{
						temp1->second.push_back(col);
					}
					
				}
				else
				{
					MQLColumn col(it->leftValue, CT_VARCHAR);
					vector<MQLColumn> v;
					v.push_back(col);
					columnMap.insert(pair<string, vector<MQLColumn>> (leftTableName, v));
				}

				temp1 = columnMap.find(rightTableName);
				if(temp1 != columnMap.end())
				{
					MQLColumn col(it->rightValue, CT_VARCHAR);
					bool found = false;
					for(int i = 0; i < temp1->second.size(); i++)
					{
						if(temp1->second.at(i).getColumnName().compare(rightTableName) == 0)
						{
							found= true;
							break;
						}
					}
					if(!found)
					{
						temp1->second.push_back(col);
					}
					
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

	
	if(*queryPlanTypePtr == SFW)
	{
		for(map<string, vector<MQLColumn>> :: iterator it = columnMap.begin(), end = columnMap.end(); it != end; ++it)
		{
			map<string, vector<MQLCondition> > :: iterator iter = conditionMap.find(it->first);
			
			TableResult tr(tdPtr, bTreePtr);
			tr.loadResult(it->first, it->second, iter->second);	
			finalResult = tr;

		}	// end of for

	}
	else
	{
		vector<MQLCondition> con;
		// load data into TableResult
		for(map<string, vector<MQLColumn>> :: iterator it = columnMap.begin(), end = columnMap.end(); it != end; ++it)
		{
			map<string, vector<MQLCondition> > :: iterator iter = conditionMap.find(it->first);
			
			TableResult tr(tdPtr, bTreePtr);
			if(iter != conditionMap.end())
				tr.loadResult(it->first, it->second, iter->second);
			else
				tr.loadResult(it->first, it->second, con); 
			tableResultMap.insert(pair<string, TableResult>(it->first, tr));

		}	// end of for


	}

}

void Select :: evaluateJoin(int *queryPlanTypePtr)
{

	// a join b join c. For 1st joining there are 3 ways, which to pick? doesn't matter the order, same number of checking needs to be done
	// 1 way to optimize it. if it's PK and some column. Those columns that PK has joined with, mark them USED. if num of USED == the some column no. of rows. exit

	// merge() tr3 = merge(tr1, tr2)
	// tr3.container
	// tr3.addRows(tr row, tr2.row(3))
	// remove column
	string tableResultMapKey;
	if(*queryPlanTypePtr == SFJ)
	{
		for(vector<string> :: iterator temp = qp.projections.begin(), end = qp.projections.end(); temp != end; ++temp)
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
		if(qp.projections.size() == 0)
		{
			for(int i =0; i < qp.selections.size(); i++)
			{
				string tableName = qp.selections.at(i);
				TableDefinition td = tdPtr->getTableDefinition(tableName);
				int noOfCol = td.getNoOfColumn();
				vector<MQLColumn> v;
				for(int in = 0; in < noOfCol; in++)
				{
					v.push_back(td.getColumnAt(in));
				}
				columnMap.insert(pair<string, vector<MQLColumn>> (tableName, v));
			}
		}
		else
		{
			map<string, vector<MQLColumn> > :: iterator temp1;
			for(vector<SelectCondition> :: iterator it = qp.joins.begin(), end = qp.joins.end(); it != end; ++it)
			{
				string leftTableName = tdPtr->getTableByColumnName(it->leftValue);
				string rightTableName = tdPtr->getTableByColumnName(it->rightValue);	

				temp1 = columnMap.find(leftTableName);
				if(temp1 != columnMap.end())
				{
					MQLColumn col(it->leftValue, CT_VARCHAR);
					bool found = false;
					for(int i = 0; i < temp1->second.size(); i++)
					{
						if(temp1->second.at(i).getColumnName().compare(leftTableName) == 0)
						{
							found= true;
							break;
						}
					}
					if(!found)
					{
						temp1->second.push_back(col);
					}
					
				}
				else
				{
					MQLColumn col(it->leftValue, CT_VARCHAR);
					vector<MQLColumn> v;
					v.push_back(col);
					columnMap.insert(pair<string, vector<MQLColumn>> (leftTableName, v));
				}

				temp1 = columnMap.find(rightTableName);
				if(temp1 != columnMap.end())
				{
					MQLColumn col(it->rightValue, CT_VARCHAR);
					bool found = false;
					for(int i = 0; i < temp1->second.size(); i++)
					{
						if(temp1->second.at(i).getColumnName().compare(rightTableName) == 0)
						{
							found= true;
							break;
						}
					}
					if(!found)
					{
						temp1->second.push_back(col);
					}
					
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
		
		 vector<MQLCondition> con;
		// load data into TableResult
		for(map<string, vector<MQLColumn>> :: iterator it = columnMap.begin(), end = columnMap.end(); it != end; ++it)
		{
			map<string, vector<MQLCondition> > :: iterator iter = conditionMap.find(it->first);
			
			TableResult tr(tdPtr, bTreePtr);
			if(iter != conditionMap.end())
				tr.loadResult(it->first, it->second, iter->second);
			else
				tr.loadResult(it->first, it->second, con); 
			tableResultMap.insert(pair<string, TableResult>(it->first, tr));

		}	// end of for
	}

	vector<vector<string>> joinedTables;
	int joinedTablesIndex = 0;
	bool firstJoin = false;
	// inner join bet. 2 tables, call merge
	for(vector<SelectCondition> :: iterator it = qp.joins.begin(), end = qp.joins.end(); it != end; ++it)
	{
		
		string leftTableName, rightTableName;
		leftTableName = tdPtr->getTableByColumnName(it->leftValue);
		rightTableName = tdPtr->getTableByColumnName(it->rightValue);

		// load both sides to each tableResult
		vector<MQLCondition> cons;
		vector<MQLColumn> cols;
		int leftColNum = 0, rightColNum = 0;
		int leftColJoinedTableIndex = -1, rightColJoinedTableIndex = -1;
		TableResult tr1(tdPtr, bTreePtr), tr2(tdPtr, bTreePtr);
		
		for(int i = 0; i < joinedTables.size(); i++)
		{
			vector<string> vec = joinedTables.at(i);
			for(int in = 0; in < vec.size(); in++)
			{
				if(vec.at(in).compare(leftTableName) == 0)
				{
					leftColJoinedTableIndex = in;
				}
				if(vec.at(in).compare(rightTableName) == 0)
				{
					rightColJoinedTableIndex = in;
				}
			}
		}
		
		if(leftColJoinedTableIndex == -1)
		{
			// get existing tableResults
			map<string, TableResult > :: iterator iter = tableResultMap.find(leftTableName);
			if(iter != tableResultMap.end())
			{
				tr1 = iter->second;
				vector<MQLColumn> v = tr1.getColumnList();
				for(int i = 0; i < v.size(); i++)
				{
					MQLColumn c = v.at(i);
					if(c.getColumnName().compare(it->leftValue) == 0)
					{
						leftColNum = i;
						break;
					}
				}
			}
			else
			{
				MQLColumn leftCol(it->leftValue, CT_VARCHAR);
				cols.push_back(leftCol);
				tr1.loadResult(leftTableName, cols, cons);
			
			}
		}
		// there are existing join on 1 of the tables
		else
		{
			stringstream ss;
			ss << leftColJoinedTableIndex;
			map<string, TableResult> :: iterator itera = tableResultMap.find(ss.str());
			tr1 = itera->second;
			vector<MQLColumn> v = tr1.getColumnList();
			for(int i = 0; i < v.size(); i++)
			{
				MQLColumn c = v.at(i);
				if(c.getColumnName().compare(it->leftValue) == 0)
				{
					leftColNum = i;
					break;
				}
			}
		}

		if(rightColJoinedTableIndex == -1)
		{
			map<string, TableResult > :: iterator iter1 = tableResultMap.find(rightTableName);
			if(iter1 != tableResultMap.end())
			{
				tr2 = iter1->second;
				vector<MQLColumn> v = tr2.getColumnList();
				for(int i = 0; i < v.size(); i++)
				{
					MQLColumn c = v.at(i);
					if(c.getColumnName().compare(it->rightValue) == 0)
					{
						rightColNum = i;
						break;
					}
				}
			}
			else
			{
				cols.clear();
				MQLColumn rightCol(it->rightValue, CT_VARCHAR);
				cols.push_back(rightCol);
				tr2.loadResult(rightTableName, cols, cons);
			}
		}
		// there are existing join on 1 of the tables
		else
		{
			stringstream ss;
			ss << rightColJoinedTableIndex;
			map<string, TableResult> :: iterator itera = tableResultMap.find(ss.str());
			tr2 = itera->second;
			vector<MQLColumn> v = tr2.getColumnList();
			for(int i = 0; i < v.size(); i++)
			{
				MQLColumn c = v.at(i);
				if(c.getColumnName().compare(it->rightValue) == 0)
				{
					rightColNum = i;
					break;
				}
			}
		}
		TableResult tr3(tdPtr, bTreePtr);
		tr3 = TableResult::merge(&tr1, &tr2, tdPtr, bTreePtr);
		// loop through and check for equal values
		for(int i = 0; i < tr1.getNoOfRows(); i++)
		{
			
			string left = tr1.getValueAt(i, leftColNum);
			for(int in = 0; in <tr2.getNoOfRows(); in++)
			{
				string right = tr2.getValueAt(in, rightColNum);
				if(left.compare(tr2.getValueAt(in, rightColNum)) == 0)
				{
					tr3.addMergedRow(&tr1, i, &tr2, in);
				}
			}
		}
		stringstream ss;
		if(leftColJoinedTableIndex  != -1 ||rightColJoinedTableIndex != -1)
		{
			map<string, TableResult> :: iterator iterat;
			if(leftColJoinedTableIndex != -1)
			{
				ss<< leftColJoinedTableIndex << "";
				iterat = tableResultMap.find(ss.str());
				joinedTables.at(leftColJoinedTableIndex).push_back(rightTableName);
			}
			else
			{
				
				ss<< rightColJoinedTableIndex << "";
				iterat = tableResultMap.find(ss.str());
				joinedTables.at(rightColJoinedTableIndex).push_back(leftTableName);
				
			}
			iterat->second = tr3;
			
		}
		else
		{
			ss<< joinedTablesIndex << "";
			tableResultMap.insert(pair<string, TableResult>(ss.str(), tr3));
			tableResultMapKey = ss.str();
			joinedTablesIndex++;
		}
		
		if(joinedTables.size() ==0)
		{
			vector<string> vec;
			vec.push_back(leftTableName);
			vec.push_back(rightTableName);
			joinedTables.push_back(vec);
		}
		/*
		// loop through and check for equal values
		for(int i = 0; i < tr1Ptr->getNoOfRows(); i++)
		{
			string left = tr1Ptr->getValueAt(i, leftColNum);
			for(int in = 0; in <tr2Ptr->getNoOfRows(); in++)
			{
				if(left.compare(tr2Ptr->getValueAt(in, rightColNum)) == 0)
				{
					tr3.addMergedRow(tr1Ptr, i, tr2Ptr, in);
				}
			}
		}
		*/
	}	// end of for

	map<string, TableResult> :: iterator iter = tableResultMap.find(tableResultMapKey);
	finalResult = iter->second;
}
void Select :: evaluateSelect()
{
	// 1. Query Plan -> SELECT FROM  
	// Select from 1 table only

	// loop through Select, add each column to the correct table
	string tableName = qp.selections.at(0);
	map<string, vector<MQLColumn> > :: iterator iter = columnMap.find(tableName);

	for(vector<string> :: iterator it = qp.projections.begin(), end = qp.projections.end(); it != end; ++it)
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
			map<string, vector<MQLColumn>> :: iterator iter = columnMap.find(tableName);
			if(iter != columnMap.end())
			{
				iter->second.push_back(col);
			}
			else
			{
				vector<MQLColumn> v;
				v.push_back(col);
				columnMap.insert(pair<string, vector<MQLColumn>> (tableName, v));
			}
		}
	}	// end of for

	// Select *
	if(qp.projections.size() == 0)
	{
		for(int i =0; i < qp.selections.size(); i++)
		{
			string tableName = qp.selections.at(i);
			TableDefinition td = tdPtr->getTableDefinition(tableName);
			int noOfCol = td.getNoOfColumn();

			vector<MQLColumn> v;
			for(int in = 0; in < noOfCol; in++)
			{
				v.push_back(td.getColumnAt(in));			
			}
			columnMap.insert(pair<string, vector<MQLColumn>> (tableName, v));
		}
	}
	if(iter == columnMap.end())
	{
		iter = columnMap.find(tableName);
	}
	TableResult tr(tdPtr, bTreePtr);
	vector<MQLCondition> v;
	tr.loadResult(tableName, iter->second, v);
	finalResult = tr;

}

bool Select :: parseSemantics(int *queryPlanTypePtr)
{
	// check whether whether tree is disjoint 
	// (right now the only situation is: there is some attribute in SELECT / WHERE that does not belong to any tables in FROM/INNER JOIN)


	// check for Select a from b where c = "" whether a & c is from table b
	// check for Select a from b inner join c on d = e where f = "" . a,d,e,f must be from c OR d

	
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


	map <string, string> attrMap;
	// check WHERE clause
	for(int i = 0; i < qp.conditions.size(); i++)
	{
		SelectCondition sc = qp.conditions.at(i);
		string leftTableName, rightTableName;
		bool found = false, bothNotStr = false;

		if(!sc.isLeftStr)
		{
			leftTableName = tdPtr->getTableByColumnName(sc.leftValue);
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
			
			// a = "123" and a = "abc" (always false)
			// a = "abc" and a = "abc" (remove one)

			if(sc.isRightStr)
			{
				pair<map<string, string>::iterator,bool> ret = attrMap.insert(pair<string, string>(sc.leftValue, sc.rightValue));
				if (ret.second==false)
				{
					// duplicated condition, remove
					if(ret.first->second.compare(sc.rightValue) == 0)
					{
						qp.conditions.erase(qp.conditions.begin() + i);
						i--;
					}
					// same attribute but different string, will always be false
					else
					{
						isEmptyResult = true;
						return false;
					}
					
				}
			}
		}

		found = false;
		if(!sc.isRightStr)
		{
			rightTableName = tdPtr->getTableByColumnName(sc.rightValue);
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
			if(!sc.isLeftStr)
				bothNotStr = true;

			// always false
			// a = "123" and a = "abc"
			if(sc.isLeftStr)
			{
				pair<map<string, string>::iterator,bool> ret = attrMap.insert(pair<string, string>(sc.rightValue, sc.leftValue));
				if (ret.second==false)
				{
					// duplicated condition, remove
					if(ret.first->second.compare(sc.leftValue) == 0)
					{
						qp.conditions.erase(qp.conditions.begin() + i);
						i--;
					}
					// same attribute but different string, will always be false
					else
					{
						isEmptyResult = true;
						return false;
					}
					
				}
			}
		}

		if(bothNotStr)
		{
			//  a = b (both are not in same table) does not exist in WHERE clause
			if(leftTableName.compare(rightTableName) != 0)
			{
				return false;
			}
			// qty = qty
			if(sc.leftValue.compare(sc.rightValue) == 0)
			{
				qp.conditions.erase(qp.conditions.begin() + i);
				i--;
			}

		}
	}
	bool executeThis;
	// if there is inner join, check that 1 is from table A, 1 is from table B
	for(int i = 0; i < qp.joins.size(); i++)
	{
		executeThis = true;
		string leftTableName = tdPtr->getTableByColumnName(qp.joins.at(i).leftValue); 
		string rightTableName = tdPtr->getTableByColumnName(qp.joins.at(i).rightValue);
		if(leftTableName.compare(rightTableName) == 0)
		{
			string leftValue = qp.joins.at(i).leftValue;
			string rightValue = qp.joins.at(i).rightValue;
			if(leftValue.compare(rightValue) != 0)
			{
				return false;
			}
			// remove to become a single table-> SELECT employeeid FROM EmployeeInformation INNER JOIN EmployeeInformation ON employeeid = employeeid
			else
			{
				
				// remove joins
				qp.joins.erase(qp.joins.begin() + i);
				i--;
				if(qp.joins.size() == 0)
				{
					if(*queryPlanTypePtr == SFJW)
						*queryPlanTypePtr = SFW;
					else if(*queryPlanTypePtr == SFJ)
						*queryPlanTypePtr = SF;
				}
				// remove from selections
				for(int in = 0; in < qp.selections.size(); in++)
				{
					if(qp.selections.at(in).compare(leftTableName) == 0)
					{
						qp.selections.erase(qp.selections.begin() + in);
						in--;
						break;
					}
				}
				executeThis = false;
			}
		}
		if(executeThis)
		{
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

	}

	/*
	// check FROM clause
	for(int i = 0; i < qp.selections.size(); i++)
	{
		if(!tdPtr->tableExists(qp.selections.at(i)))
		{
			return false;
		}
	}
	*/
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

	// remove ;
	string a = tokensPtr->at(tokensPtr->size() - 1);
	if(a.at(a.size()-1) == ';')
	{
		tokensPtr->at(tokensPtr->size() - 1) = a.substr(0, a.size()-1);
	}
	else
	{
		return false;
	}

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
						
						if(tdPtr->tableExists(currentValue1))
						{
							qp.selections.push_back(currentValue1);
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
					isInnerJoin = false;
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
								
								if(tdPtr->columnExists(temp.at(index)))
								{
									qp.projections.push_back(temp.at(index));
								}
								else
								{
									return false;
								}
								
							}
							// , exist at the end of currentValue
							if(currentValue.find_last_of(",") == currentValue.size()-1)
							{
								hasColma = true;
							}
							else
							{
								hasColma = false;
							}
						}
						else
						{
							value = temp[0];
							if(tdPtr->columnExists(value))
							{
								qp.projections.push_back(value);
							}
							else
							{
								return false;
							}

						}
					}
					else
					{
						value = currentValue;
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
						
							if(tdPtr->columnExists(value))
							{
								qp.projections.push_back(value);
							}
							else
							{
								return false;
							}
						
						}

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
					
					if(!tdPtr->columnExists(currentValue))
					{
						return false;
					}
					

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
						
						if(!tdPtr->columnExists(currentValue2))
						{
							return false;
						}
						
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
					if(currentValue.compare("'") == 0)
					{
						// find end of "
						string a;
						i++;
						while(i < tokensPtr->size())
						{
							currentValue = tokensPtr->at(i);
							if(currentValue.compare("'") != 0)
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
					else if (currentValue.find_first_of("'") !=  currentValue.find_last_of("'"))
					{
						string t = currentValue.substr(currentValue.find_first_of("'")+1);
						currentValue = t.substr(0, t.find_last_of("'"));
						isLeftStr = true;
					}
					else
					{
						if(currentValue.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1)
						{
							return false;
						}
						
						if(!tdPtr->columnExists(currentValue))
						{
							return false;
						}
						
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
						if(currentValue2.compare("'") == 0)
						{
							// find end of "
							string a;
							i++;
							while(i < tokensPtr->size())
							{
								currentValue2 = tokensPtr->at(i);
								if(currentValue2.compare("'") != 0)
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
						else if (currentValue2.find_first_of("'") !=  currentValue2.find_last_of("'"))
						{
							string t = currentValue2.substr(currentValue2.find_first_of("'")+1);
							currentValue2 = t.substr(0, t.find_last_of("'"));
							isRightStr = true;
						}
						else
						{
							if(currentValue2.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") != -1)
							{
								return false;
							}
							
							if(!tdPtr->columnExists(currentValue2))
							{
								return false;
							}
							
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