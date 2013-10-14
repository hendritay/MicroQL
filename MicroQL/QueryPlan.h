#ifndef QueryPlan_h
#define QueryPlan_h

#include "stdafx.h"
#include "SelectCondition.h"
#include <string>
#include <vector>
using namespace std;

class QueryPlan{
	public:
		QueryPlan :: QueryPlan();
		// columnName
		vector<string> projections;	// SELECT clause
		vector<string> selections; // FROM & INNER JOIN ON clause (store table name)
		vector<SelectCondition> joins;	// INNER JOIN ON clause (store join condition only)
		vector<SelectCondition> conditions;	// WHERE clause
		//void addProjection(string tableName, string columnName);
		//void addSelection(string tableName);
		//void addCondition(SelectCondition con);
		// add list of conditions
	//private:
		
		// cost
};
#endif