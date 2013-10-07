#pragma once
#include <vector>
#include <string>
using namespace std;
class TableResult {
public:
	void loadResult(string tableName, vector<string> column, vector<string> condition);// dummy method method parameter subject to change
	void merge(TableResult *tb); // dummy method 
};