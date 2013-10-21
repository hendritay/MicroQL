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