#ifndef QueryPlan_h
#define QueryPlan_h

#include "stdafx.h"
#include <string>
using namespace std;
class QueryPlan{
	public:
		QueryPlan :: QueryPlan();
		void addProjection(string columnName);
		void addSelection(string tableName);
		//void addCondition(conditionList);
	private:
		// pointer to projection
		// pointer to selection
		// cost
};
#endif