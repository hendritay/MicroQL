// author: Kim

#ifndef Select_h
#define Select_h


#include "stdafx.h"
#include <vector>
#include <string>
#include <map>
#include "QueryPlan.h"
#include "Database/TableResult.h"

class Select{
	public:
		Select :: Select();
		bool evaluateQuery(string query);
	private:
		vector<QueryPlan> queryPlans;
		string query;
		//columnList
		//joinList
		//conditionList


		void generateQueryPlan();
		void removeRedundancy();
		void evaluateQueryPlan(TableResult *& tbPtr);
		void evaluateWhere(map<string, TableResult> * tableResultsPtr, QueryPlan * qpPtr);// either attr = "" OR attr = attr, call populate
		void evaluateJoin(map<string, TableResult> * tableResultsPtr, QueryPlan * qpPtr);	// inner join bet. 2 tables, call merge
		void evaluateSelect(map<string, TableResult> * tableResultsPtr,  QueryPlan * qpPtr);	// can select from diff tables

		bool parseSemantics();
		bool parseSyntaxAndType(string query, vector<string> * tokensPtr);

		// *** other additional functions ***
		bool isAttributeOrRelationDotAttribute(string value);
		vector<string> splitString(string toBeSplit, string delimiters);
		void stringToUpper(string &s);
};
#endif