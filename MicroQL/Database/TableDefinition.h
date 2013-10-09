#pragma once
#include <string>
#include "MQLColumn.h"
#include <vector>
typedef vector<MQLColumn> ColumnList ;
using namespace std;

class TableDefinition {
public:
	TableDefinition() {

	}
	MQLColumn &getColumnAt(int i) {
		return columnList.at(i);
	}

	string getTableName();
	void addColumn(MQLColumn &column){
		columnList.push_back(column);
	}

	void create();
	string serialize();
	static TableDefinition * deSerialize(string &content);

	int getNoOfColumn() {
		return columnList.size();
	}
	// page will be requested by TableDictionary under addTable
	void setRecordPage(int initialRecordPage) {
		recordPage = initialRecordPage;
	}
private:
	string tableName;
	ColumnList columnList;
	MQLColumn PrimaryKey;
	int recordPage; 

};