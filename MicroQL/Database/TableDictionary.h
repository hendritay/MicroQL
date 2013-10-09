#pragma once
#include "../Storage/FileManager.h"
#include "../Storage/StorageManager.h"
#include "TableDefinition.h"
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