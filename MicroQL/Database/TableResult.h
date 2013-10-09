#pragma once
#include <vector>
#include <string>
#include "MQLColumn.h"
#include "MQLCondition.h"
#include "MQLTupleManager.h"

using namespace std;
class TableResult {
public:
	void loadResult(string tableName, vector<MQLColumn> column, vector<MQLCondition> condition);// dummy method method parameter subject to change	
	void merge(TableResult *tb); // dummy method 
private:
	TupleManager tm;

};