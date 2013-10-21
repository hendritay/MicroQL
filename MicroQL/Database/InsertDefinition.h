#pragma once
#include <vector>
#include <string>
#include "Database\MQLColumn.h"
#include "Database\MQLCondition.h"
#include "Database\TableDictionary.h"
#include "Storage\BTree.h"


/* 
     Insert employee (name, age) VALUES ("john", "10")

	  InsertDefintion ud;

	  // INSERT Part 
	  MQLColumn setcolumn1("name", VT_VARCHAR);
	  MQLColumn setcolumn2("john", VT_CONSTANTSTRING);
	  MQLCondition set1(setcolumn1, "=", setcolumn2);

	  MQLColumn setcolumn3("age", VT_VARCHAR);
	  MQLColumn setcolumn4("10", VT_VARCHAR);
	  MQLCondition set2(setcolumn3, "=", setcolumn4);

	  ud.setTableName("employee")
	  ud.addInsert(set1);
	  ud.addInsert(set2);
	
*/ 
using namespace std;

class InsertDefinition {
public:
	void setName(string name) {
		tableName = name;
	}
	string getTableName();
	MQLCondition getConditionAt(int i); 	
			
	void addInsert(MQLCondition cond);

	void execute(TableDictionary *td, BTree *bTree);
private:
	string tableName;
	map<string, MQLCondition> InsertColumn;
};