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
		//Select :: Select();
		Select :: Select(TableDictionary * tdPtr1, BTree *bTreePtr1);
		TableResult * evaluateQuery(string query);	// TODO: return TableResult ptr
	private:
		map<string, TableResult> tableResultMap;
		map<string, vector<MQLColumn>> columnMap;
		map<string, vector<MQLCondition>> conditionMap;
		QueryPlan qp;
		//vector<QueryPlan> queryPlans;	// only have 1 queryplan
		string query;
		TableResult finalResult;
		TableDictionary * tdPtr;
		BTree *bTreePtr;
		bool isEmptyResult;

		/*
			four standard query plan's evaluation is done in the following 4 functions:
		*/
		void evaluateQueryPlan(int * queryPlanTypePtr);
		void evaluateWhere(int * queryPlanTypePtr);// either attr = "" OR attr = attr, call populate
		void evaluateJoin(int *queryPlanTypePtr);	// inner join bet. 2 tables, call merge
		void evaluateSelect();	// can select from diff tables

		/*
			-check semantics Select ...  & WHERE ...-> all attributes belongs to FROM, INNER JOIN tables.
				if WHERE contains a = b, both must be from same table!
				ON -> 1 attribute belongs to FROM table, another belongs to ON table
			-remove redundancy (a = "abc" and "abc" = a)
			-check for always false conditions and stop query evaluation
		*/
		bool parseSemantics();

		/*
			-check that syntax  & type is correct & Select ... -> all are attributes, FROM ... -> all are tables, INNER JOIN -> table, ON -> all attributes, WHERE -> all attributes
			-check that type is correct e.g 
		*/
		bool parseSyntaxAndType(string query, vector<string> * tokensPtr, int * queryPlanType);
		

		// *** other additional functions ***
		vector<string> splitString(string toBeSplit, string delimiters);
		void stringToUpper(string &s);
};
#endif