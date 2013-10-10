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
	void merge(TableResult *tb); // dummy method 
private:
	TupleManager tm;

};