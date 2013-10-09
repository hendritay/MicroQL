// author: Kim

#ifndef Select_h
#define Select_h


#include "stdafx.h"
#include <vector>
#include <string>
#include "QueryPlan.h"
#include "Database/TableResult.h"

class Select{
	public:
		Select :: Select();
		TableResult evaluateQuery(string query);
	private:
		vector<QueryPlan> queryPlans;
		string query;
		//columnList
		//joinList
		//conditionList


		void generateQueryPlan();
		
		
		//void evaluateWhere(TableResult * tb);	// either attr = "" OR attr = attr, call populate
		//void evaluateJoin(TableResult * tb);	// inner join bet. 2 tables, call merge
		//void evaluateSelect(TableResult * tb);	// can select from diff tables

		bool parseSemantics();
		bool parseSyntaxAndType(string query, vector<string> * tokensPtr, vector<int> * clausePosPtr);

		// *** other additional functions ***
		bool isAttributeOrRelationDotAttribute(string value);
		vector<string> splitString(string toBeSplit, string delimiters);
		void stringToUpper(string &s);
};
#endif