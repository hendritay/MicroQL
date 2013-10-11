#pragma once
#include <string>
#include "Database\MQLColumn.h"
#include <vector>
typedef vector<MQLColumn> ColumnList ;
using namespace std;

/*
   CREATE TABLE employee (
       name		10, 
	   salary   30,
	   dob      40, 
	   PK (name)
   )

   TableDefinition td;
   td.setTableName("name");

   MQColumnName column1("name", VT_VARCHAR, 10, true);
   MQColumnName column2("salary", VT_VARCHAR, 30, false);
   MQColumnName column3("dob", VT_VARCHAR, 30, false);

   td.addColumn(column1);
   td.addColumn(column2);
   td.addColumn(column);

*/

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