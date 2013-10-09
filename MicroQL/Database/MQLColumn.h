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
    MQLColumn(string &name, ColumnType ct, int size) {
	    columnName = name;
		columnSize = size;
		columnType = ct;
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
		return result;
	}


private:
	string columnName;
	int columnSize; 
	ColumnType columnType; //always string 
};