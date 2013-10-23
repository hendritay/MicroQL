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
		tableName = CommonUtility::trim(name);
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

	void execute(TableDictionary *td, BTree *bTree, FileManager *fm, StorageManager *sm) {

		TableResult tr(td, bTree);

		vector<MQLCondition>::iterator iter;


		vector<MQLColumn> columnList;

		TableDefinition tdef = td->getTableDefinition(tableName);
		int size = tdef.getNoOfColumn();
		for (int i = 0; i < size; i++) {
			columnList.push_back(tdef.getColumnAt(i));
		}		

		tr.loadResult(tableName, columnList, whereColumn);

		int noRows = tr.getNoOfRows();
		if (tr.getNoOfRows() != 0) {
			// start update
			tr.startUpdate(fm, sm, updateColumn, tdef.getPrimaryKeyColumn().getColumnName()); 
		}

		cout << noRows;
		cout << " updated.\n";
	}

	bool verify(TableDictionary *tdict) {
		if (!tdict->tableExists(tableName)) {
			cout << "No table was found.\n";
			return false;
		}

		TableDefinition td = tdict->getTableDefinition(tableName);

		
		vector<MQLCondition>::iterator iterWhere;

		for (iterWhere = whereColumn.begin(); iterWhere != whereColumn.end(); iterWhere++) {
			if (!td.findColumnName(iterWhere->getColumn1().getColumnName())) {
				cout << iterWhere->getColumn1().getColumnName();
				cout << " doesn't exists in the table definition \n";
				return false;
			} 
		}

		for (iterWhere = updateColumn.begin(); iterWhere != updateColumn.end(); iterWhere++) {
			if (!td.findColumnName(iterWhere->getColumn1().getColumnName())) {
				cout << iterWhere->getColumn1().getColumnName();
				cout << " doesn't exists in the table definition \n";
				return false;
			} 
		}

		vector<MQLCondition>::iterator iterSet;

		for (iterSet = updateColumn.begin(); iterSet != updateColumn.end(); iterSet++) {
			if (iterSet->getColumn1().getColumnName().compare(td.getPrimaryKeyColumn().getColumnName()) == 0) {
				cout << "Update of primary key value is not allowed.";
				return false;
			}
		}

	
	
		return true;
	}
private:
	string tableName;
	vector<MQLCondition> updateColumn;
	vector<MQLCondition> whereColumn;
};