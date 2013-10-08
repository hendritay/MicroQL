// author: Kim

#ifndef Select_h
#define Select_h


#include "stdafx.h"
#include <vector>
#include <string>
#include "QueryPlan.h"

class Select{
	public:
		Select :: Select();
		//ResultTable evaluateQuery(string query);
	private:
		vector<QueryPlan> queryPlans;
		string query;
		//columnList
		//joinList
		//conditionList


		QueryPlan generateQueryPlan();
		bool parseSyntax();	
		bool parseSchema();
		/*void evaluateWhere(ResultTable * resultTable);	// either attr = "" OR attr = attr, call populate
		void evaluateJoin(ResultTable * resultTable);	// inner join bet. 2 tables, call merge
		void evaluateSelect(ResultTable * resultTable);	// can select from diff tables
		*/
};
#endif