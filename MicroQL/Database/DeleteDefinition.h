#pragma once
#include <vector>
#include <string>
#include "Database\MQLColumn.h"
#include "Database\MQLCondition.h"
#include "Storage\BTree.h"
#include "Database\TableResult.h"


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
		tableName = CommonUtility::trim(name);
	}
	string getTableName();
	MQLColumn getColumnAt(int i); 	
	
	MQLCondition getCondition(int i);
	
	void addWhere(MQLCondition cond1) {
		whereColumn.push_back(cond1);
	}


	void execute(TableDictionary *td, BTree *bTree, FileManager *fm) {

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
			// start delete
			tr.setDeleteMark(fm);
		}

		cout << noRows;
		cout << " deleted.\n";
	}

	bool verify(TableDictionary *tdict, BTree *bTree) {

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
	
	
		return true;
	
	}
private:
	string tableName;
	vector<MQLCondition> whereColumn;
};