#pragma once
#include <vector>
#include <list>
#include "Database\MQLTuple.h";
#include "Database\MQLColumn.h"

typedef vector<MQLTuple> TupleList;

class MQLTupleManager {
public :
	void addColumnList(MQLColumn column) {
		columnList.push_back(column);
		columnOrder[column.getColumnName()] = columnList.size() - 1;
	}

	int getColumnNo(string columnName) {
		
	}
private:
	vector<MQLColumn> columnList;
	map<string, int> columnOrder;
	TupleList tupleList;
};