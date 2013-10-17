#pragma once
#include "Storage/FileManager.h"
#include "Storage/StorageManager.h"
#include "Database/TableDefinition.h"
#include <map>
#include <string>
typedef map<string, pair<int, TableDefinition *>> DictionaryList;

class TableDictionary {
public:
	TableDictionary(FileManager *file, StorageManager *sm) {
		myFile = file;
		myStorage = sm;
		populateTableNameList();	
		populatedTableList = false;
	}

	TableDefinition retrieveTable(string tableName);
	void storeTableToDictionary(TableDefinition *td);
	bool tableExists(string &tableName);
	// columnname must be unique through the entire database. 
	bool columnExists(string columnName);

	TableDefinition getTableDefinition(string tableName) {
		return *dicitionaryList.find(tableName)->second.second;
	}

	string getTableByColumnName(string columnName) {
		return "";
	}

private:
	void addColumnToColumnList(TableDefinition *td);

	void populateTableNameList();
	void populateTableList();
	bool populatedTableList;
	map<string, bool> columnNameList;
	DictionaryList dicitionaryList;
	FileManager *myFile;
	StorageManager * myStorage;	
};