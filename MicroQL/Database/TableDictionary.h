#pragma once
#include "Storage/FileManager.h"
#include "Storage/StorageManager.h"
#include "Database/TableDefinition.h"
#include <map>
#include <string>
#include <iomanip>

typedef map<string, pair<int, TableDefinition *>> DictionaryList;

class TableDictionary {
public:
	TableDictionary(FileManager *file, StorageManager *sm) {
		myFile = file;
		myStorage = sm;
		populateTableNameList();	
		populatedTableList = false;
	}

	TableDefinition retrieveTable(string tableName) {
		TableDefinition td;
		return td;
	}

	void storeTableToDictionary(TableDefinition *td);
	bool tableExists(string &tableName) ;
	// columnname must be unique through the entire database. 
	bool columnExists(string columnName);

	TableDefinition getTableDefinition(string tableName) {
		if (!populatedTableList)
			populateTableList();

		pair<int, TableDefinition*> pair1 = dicitionaryList.find(tableName)->second;
		TableDefinition td = *(pair1.second);
		return td;
	}

	string getTableByColumnName(string columnName) {
	if (!populatedTableList) 
		populateTableList();

		map<string, string>::iterator iter = columnNameList.find(columnName);
		if (iter != columnNameList.end()) {
			return iter->second;
		} else 
			return "";
	}

	
	void print() {
		
		cout << "Table Name\n";
		cout << "==========\n";

		DictionaryList::iterator iter;

		for (iter = dicitionaryList.begin(); iter != dicitionaryList.end(); iter++) {
			cout << iter->first;
			cout << endl;
		}
	}

private:
	void addColumnToColumnList(TableDefinition *td);

	void populateTableNameList();
	void populateTableList();
	bool populatedTableList;
	map<string, string> columnNameList;
	DictionaryList dicitionaryList;
	FileManager *myFile;
	StorageManager * myStorage;	
};