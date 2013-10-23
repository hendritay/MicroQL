#pragma once
#include <string>
#include "Database\MQLColumn.h"
#include <vector>
#include <iostream>
#include <iomanip>

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

		if (column.isPrimaryKey()) 
			PrimaryKey = column;
	}

	void create();
	ListChar serialize();
	static TableDefinition * deSerialize(string &content);

	MQLColumn getPrimaryKeyColumn() {
		return PrimaryKey;
	}

	int getNoOfColumn() {
		return columnList.size();
	}
	// page will be requested by TableDictionary under addTable
	void setRecordPage(int initialRecordPage) {
		startRecordPageNo = initialRecordPage;
	}

	int getRecordPage() {
		return startRecordPageNo;
	}

	void setTableName(string table){
		tableName = table;
	}

	bool findColumnName(string colName) {

		ColumnList::iterator iter;

		for (iter = columnList.begin(); iter != columnList.end(); iter++) {
			if (iter->getColumnName().compare(colName) == 0)
				return true;
		}

		return false;
		
	}

	void print() {
		cout << left;
		cout << '|' << setw(30) << "Column " << '|' << setw(10) << "Type " << '|' << endl;
		cout << "========================================\n";
		ColumnList::iterator iter;

		for (iter = columnList.begin(); iter != columnList.end(); iter++) {
			cout << setw(30);
			cout << iter->getColumnName(); 

			if (PrimaryKey.getColumnName().compare(iter->getColumnName()) == 0) 
				cout << "(PK)";
			cout <<"         ";
			cout << setw(10);
			cout << "VARCHAR(";
			cout << iter->getColumnSize();
			cout << ")\n";
		}
	}
private:
	string tableName;
	ColumnList columnList;
	MQLColumn PrimaryKey;
	int startRecordPageNo; 

};