#ifndef QueryPlan_h
#define QueryPlan_h


#include <string>

class QueryPlan{
	public:
		QueryPlan :: QueryPlan();
		void addProjection(string columnName);
		void addSelection(string tableName);
		void addCondition(conditionList);
	private:
		// pointer to projection
		// pointer to selection
		// cost
};
#endif