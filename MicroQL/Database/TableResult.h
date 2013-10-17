#pragma once
#include <vector>
#include <string>
#include "Database\MQLColumn.h"
#include "Database\MQLCondition.h"
#include "Database\MQLTupleManager.h"

using namespace std;

class TableResult {
public:
	void loadResult(string tableName, vector<MQLColumn> column, vector<MQLCondition> condition);// dummy method method parameter subject to change	

	TableResult merge(TableResult *tb, TableResult *tb2);
	int getNoOfRows();
	string getValueAt(int rowIndex, int columnIndex);
	int getNoOfColumns();
	void addMergedRow(TableResult *tb1, int rowIndex1, TableResult *tb2, int rowIndex2);
	
private:
	TupleManager tm;

};