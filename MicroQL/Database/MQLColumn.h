#pragma once
#include <string>
#include "MainType.h"
#include "CommonUtility.h"
using namespace std;

/* USAGE 
  CREATE TABLE a (ABC, 


*/ 

class MQLColumn {
public:
	MQLColumn(){
	}
	MQLColumn(string &name, ColumnType ct){
		columnName = name;
		columnSize = 0;
		columnType = ct;
		primaryKey = false;
	}
    MQLColumn(string &name, ColumnType ct, int size, bool isPrimaryKey) {
	    columnName = name;
		columnSize = size;
		columnType = ct;
		primaryKey = isPrimaryKey;
	}

	string getColumnName() {
		return columnName;
	}

	int getColumnSize() {
		return columnSize;
	}

	ColumnType  getColumnType() {
		return columnType;
	}

	ListChar serialize() {
		ListChar resultList;
		CommonUtility::appendIntToList(resultList, columnName.size());
		CommonUtility::appendStringToList(resultList,columnName);
		CommonUtility::appendIntToList(resultList, columnType);
		CommonUtility::appendIntToList(resultList, columnSize);
		if (primaryKey) 
			resultList.insert(resultList.end(), '1');
		else
			resultList.insert(resultList.end(), '0');

		return resultList;
	}

	void setPrimaryKey(bool primary){
		primaryKey = primary;
	}

	bool isPrimaryKey() {
		return primaryKey;
	}


private:
	string columnName;
	int columnSize; 
	ColumnType columnType; //always string 
	bool primaryKey;
};