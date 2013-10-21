#pragma once
#include <vector>
#include <string>
#include "Database\MQLColumn.h"
#include "Database\MQLCondition.h"

/* 
     Delete employee 
	  WHERE name = "youyou"

	  DeleteDefintion ud;

	  
	  //WHERE Part 
	  MQLColumn column1("name", VT_VARCHAR);
	  MQLColumn column2("youyou", CT_CONSTANT_STRING);
	  MQLCondition cond1(column1, "=", column2);

	  ud.setTableName("employee")
	  ud.addSet(set1);
	  ud.addSet(set2);
	  ud.addWhere(cond1);

	
*/ 
using namespace std;

class DeleteDefinition {
public:
	DeleteDefinition() {
		tableName = "";
	}
	void setName(string name) {
		tableName = name;
	}
	string getTableName();
	MQLColumn getColumnAt(int i); 	
	
	MQLCondition getCondition(int i);
	
	void addWhere(MQLCondition cond1) {
		whereColumn.push_back(cond1);
	}

	void execute();
private:
	string tableName;
	vector<MQLCondition> whereColumn;
};