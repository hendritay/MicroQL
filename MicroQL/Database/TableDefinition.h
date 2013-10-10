#pragma once
#include <string>
#include "Database\MQLColumn.h"
#include <vector>
typedef vector<MQLColumn> ColumnList ;
using namespace std;

class TableDefinition {
public:
	TableDefinition() {

	}
	TableDefinition(string tbName) {
		tableName = tbName;
	}
	MQLColumn &getColumnAt(int i) {
		return columnList.at(i);
	}

	string getTableName() {
		return tableName;
	}
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
		startRecordPageNo = initialRecordPage;
	}
private:
	string tableName;
	ColumnList columnList;
	MQLColumn PrimaryKey;
	int startRecordPageNo; 

};