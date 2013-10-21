// author: Kim

#ifndef Select_h
#define Select_h


#include "stdafx.h"
#include <vector>
#include <string>
#include <map>
#include "QueryPlan.h"
#include "Database/TableResult.h"
#include "Database/TableDictionary.h"
#include "Database/MQLColumn.h"
#include "Database/MQLCondition.h"

enum queryPlanType{
	SF, SFJ, SFW, SFJW
};
/*
SELECT FROM 
SELECT FROM INNER JOIN ON (when evaluating INNER JOIN, grab those columns in SELECT also)
SELECT FROM WHERE (when evaluating WHERE, grab those columns in SELECT also)
SELECT FROM INNER JOIN ON WHERE (when evaluating WHERE, grab those columns in SELECT also)
*/

class Select{
	public:
		Select :: Select();
		Select :: Select(TableDictionary * tdPtr);
		bool evaluateQuery(string query);	// TODO: return TableResult ptr
	private:
		map<string, TableResult> tableResultMap;
		map<string, vector<MQLColumn>> columnMap;
		map<string, vector<MQLCondition>> conditionMap;
		vector<QueryPlan> queryPlans;
		string query;
		TableResult finalResult;
		TableDictionary * tdPtr;
		//columnList
		//joinList
		//conditionList


		//void generateQueryPlan();
		void removeRedundancy();
		void evaluateQueryPlan(int * queryPlanTypePtr);
		void evaluateWhere(QueryPlan * qpPtr, int * queryPlanTypePtr);// either attr = "" OR attr = attr, call populate
		void evaluateJoin(QueryPlan * qpPtr, int *queryPlanTypePtr);	// inner join bet. 2 tables, call merge
		void evaluateSelect(QueryPlan * qpPtr);	// can select from diff tables

		bool parseSemantics();
		bool parseSyntaxAndType(string query, vector<string> * tokensPtr, int * queryPlanType);

		// *** other additional functions ***
		bool isAttributeOrRelationDotAttribute(string value);
		vector<string> splitString(string toBeSplit, string delimiters);
		void stringToUpper(string &s);
};
#endif