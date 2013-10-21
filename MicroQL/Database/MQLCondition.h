#pragma once
#include "Database\MQLColumn.h"

class MQLCondition {
public:
	MQLCondition() {
	}
	MQLCondition(MQLColumn column1, string &oper,  MQLColumn column2) {
		myColumn1 = column1;
		myColumn2 = column2;
	}

	MQLColumn &getColumn1() {
		return myColumn1;
	}
	MQLColumn &getColumn2() {
		return myColumn2;
	}
private:
	MQLColumn myColumn1;
	MQLColumn myColumn2; 
};