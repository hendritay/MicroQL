#pragma once
#include <vector>
#include <string>
#include "MQLColumn.h"
#include "MQLCondition.h"


using namespace std;
class TableResult {
public:
	void loadResult(string tableName, vector<MQLColumnn> column, vector<MQLCondition> condition);// dummy method method parameter subject to change
	void merge(TableResult *tb); // dummy method 
};