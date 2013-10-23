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

	// table ra: attribute PK:a, b, c, d
	// table rb: attribute PK:e, f, g
	// table rc: attribute PK:h

	// valid syntax & semantics
/*
	validVec.push_back("SeLect * from ra");
	validVec.push_back("Select a from ra");
	validVec.push_back("Select a, b, c from ra");
	validVec.push_back("Select a,b,c from ra");
	validVec.push_back("Select a from ra where c = d");
	validVec.push_back("Select a from ra where c = \"123\"");
	validVec.push_back("Select a from ra where \"123\" = c");
	validVec.push_back("Select a from ra where \"123\" = c and \"abc\" = b");
	validVec.push_back("Select a from ra where \"123\" = c and b = d");
	validVec.push_back("Select a from ra where \"123\" = c and b = c");
	validVec.push_back("Select a from ra inner join rb on a = e");
	validVec.push_back("Select * from ra inner join rb on e = a");
	validVec.push_back("Select * from ra inner join rb on b = f");	
	validVec.push_back("Select a from ra inner join rb on e = d where \"123\" = d");
	validVec.push_back("Select a from ra inner join rb on e = d where f = g");
	validVec.push_back("Select a from ra inner join rb on e = d where \"ABC\" = e");
	validVec.push_back("Select a from ra inner join rb on e = d where \"ABC\" = b");
	validVec.push_back("Select a from ra inner join rb on e = d where f = g and f = \"abcd\"");
	validVec.push_back("Select a from ra inner join rb on e = d where f = g and f = \"abcd\" and b = e");
	validVec.push_back("Select a from ra inner join rb on e = d where f = g and f = \"abcd\" and d = f");

	// invalid syntax
	invalidVec.push_back(" ");
	invalidVec.push_back("Select ");
	invalidVec.push_back("a");
	invalidVec.push_back("Select a");
	invalidVec.push_back("Select r.a");
	invalidVec.push_back("Select a from");
	invalidVec.push_back("Select a from r.a");
	invalidVec.push_back("Select *,a from ra");
	invalidVec.push_back("Select *, a from ra");
	invalidVec.push_back("Select a from ra where");
	invalidVec.push_back("Select a from ra where c");
	invalidVec.push_back("Select a from ra where c = ");
	invalidVec.push_back("Select a from ra where c = b.d");
	invalidVec.push_back("Select a from ra where c = 123");
	invalidVec.push_back("Select a from ra where c = d and");
	invalidVec.push_back("Select a from ra where c = d and b");
	invalidVec.push_back("Select a from ra inner");
	invalidVec.push_back("Select a from ra inner join");
	invalidVec.push_back("Select a from ra inner join on");
	invalidVec.push_back("Select a from ra inner join rb on ");
	invalidVec.push_back("Select a from ra inner join rb on a");
	invalidVec.push_back("Select a from ra inner join rb on a = ");
	invalidVec.push_back("Select a from ra inner join rb on a = 123");
	invalidVec.push_back("Select a from ra inner join rb on a = \"123\"");
	invalidVec.push_back("Select a from ra inner join rb on \"123\" = a");
	invalidVec.push_back("Select a from ra inner join rb on d = \"abc\"");
	invalidVec.push_back("Select a from ra inner join rb on d = b.e");
*/
	// invalid semantics & type
	/*
	invalidVec.push_back("Select a from ra"); // a is not from table b
	invalidVec.push_back("Select d from b"); // a is not an atrribute
	invalidVec.push_back("Select a from c"); // c is not a table
	invalidVec.push_back("Select a, b, c from b"); // 1 or all attributes is not from table b
	invalidVec.push_back("Select * from ra where a = b");	// b is not an attribute
	invalidVec.push_back("Select * from ra where a = i");	// i doesn't exist
	invalidVec.push_back("Select * from ra where a = e");	// e doesn't belong to ra
	invalidVec.push_back("Select * from ra where e = a");	// e doesn't belong to ra
	invalidVec.push_back("Select * from ra inner join rd on a = b");	// rd doesn't exist
	invalidVec.push_back("Select * from ra inner join rb on h = a");	// h does not belong to rb
	invalidVec.push_back("Select * from ra inner join rb on a = b");	// both a,b from same table!
	invalidVec.push_back("Select * from ra inner join rb on a = i");	// i doesn't exist
	*/	