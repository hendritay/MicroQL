#pragma once
#include <vector>
#include <string>
#include "MQLColumn.h"
#include "MQLCondition.h"

/* 
     UPDATE employee SET 
	    name = age,
		age = "10"
	  WHERE name = "youyou"

	  UpdateDefintion ud;

	  // SET Part 
	  MQLColumn setcolumn1("name", VT_VARCHAR);
	  MQLColumn setcolumn2("age", VT_VARCHAR);
	  MQLCondition set1(setcolumn1, "=", setcolumn2);

	  MQLColumn setcolumn3("age", VT_VARCHAR);
	  MQLColumn setcolumn4("10", VT_VARCHAR);
	  MQLCondition set2(setcolumn3, "=", setcolumn4);

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

class UpdateDefinition {
public:
	void setName(string name) {
		tableName = name;
	}
	string getTableName();
	MQLColumn getColumnAt(int i); 	
	
	MQLCondition getCondition(int i);
	
	void addWhere(MQLCondition cond1) {
		whereColumn.push_back(cond1);
	}
	void addSet(MQLCondition cond) {
		updateColumn.push_back(cond);
	}

	void execute();
private:
	string tableName;
	vector<MQLCondition> updateColumn;
	vector<MQLCondition> whereColumn;
};