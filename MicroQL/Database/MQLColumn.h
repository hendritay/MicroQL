#pragma once
#include <string>
#include "../MainType.h"
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
		MQLColumn(name, ct,1000, false);
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

	string serialize() {
		string result;
		result.append(CommonUtility::convertShortTo2Bytes(columnName.size()));
		result.append(columnName);
		result.append(CommonUtility::convertShortTo2Bytes(columnType));
		result.append(CommonUtility::convertShortTo2Bytes(columnSize));
		if (primaryKey) 
			result.append("1");
		else
			result.append("0");

		return result;
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