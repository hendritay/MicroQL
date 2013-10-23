#include "stdafx.h"
#include "InsertDefinition.h"
#include "Storage\BTreeKey.h"
#include "Database\MQLTuple.h"
#include "Storage\BTree.h"
#include "Storage\BTreeKey.h"

#include "Database\MQLTupleManager.h"
#include <iostream>


string InsertDefinition::getTableName() {
	return tableName;
}

			
void InsertDefinition::addInsert(MQLCondition cond) {
	
	InsertColumn[cond.getColumn1().getColumnName()] = cond;
}

bool InsertDefinition::verify(TableDictionary *tdict, BTree *bTree) {

	if (!tdict->tableExists(tableName)) {
		cout << "No table was found.\n";
		return false;
	}

	TableDefinition td = tdict->getTableDefinition(tableName);

	int colSize = td.getNoOfColumn();
	if (colSize != InsertColumn.size()) {
		cout << "Column count mismatch\n";
		return false;
	}

	map<string, MQLCondition>::iterator iterfound;
	for (int i =0; i < colSize; i++) {
		iterfound = InsertColumn.find(td.getColumnAt(i).getColumnName());

		if (iterfound == InsertColumn.end()) {
			cout << "Column " + td.getColumnAt(i).getColumnName() + "not found.\n";
			return false;
		} else {
			if (iterfound->second.getColumn2().getColumnName().size() > td.getColumnAt(i).getColumnSize()) {
				cout << "Column " + td.getColumnAt(i).getColumnName() + " more than ";
				cout << td.getColumnAt(i).getColumnSize();
				cout << " chars " << endl;
				return false;
			}
		}
	}

	iterfound = InsertColumn.find(td.getPrimaryKeyColumn().getColumnName());

	BTreeKey bKey;
	if (bTree->getBKey(iterfound->second.getColumn2().getColumnName(), td.getRecordPage(),  bKey)) {
		cout <<"Duplicate primary key found\n";
		return false;
	}
	 iterfound->second.getColumn2().getColumnName();

	return true;

	
}

void InsertDefinition::execute(TableDictionary *td, BTree *bTree) {
	TableDefinition tdef = td->getTableDefinition(tableName);

	MQLTupleManager *tm = new MQLTupleManager();
	MQLTuple tuple1(tm);
	BTreeKey bKey;
	int size = tdef.getNoOfColumn();
	map<string, MQLCondition>::iterator iterFound;
	for (int i = 0; i < size; i++) {
		tm->addColumnList(tdef.getColumnAt(i));
		iterFound = InsertColumn.find(tdef.getColumnAt(i).getColumnName());

		if (iterFound == InsertColumn.end()) {
			cout << "Insert defintion not valid";
			return;				
		}

		if (tdef.getColumnAt(i).isPrimaryKey()) {
			bKey.setValue(iterFound->second.getColumn2().getColumnName());
		}

		tuple1.addValue(iterFound->second.getColumn2().getColumnName());
	}
	bKey.setPayload(tuple1.serialize());
	
	bTree->insertRecord(tdef.getRecordPage(), bKey);	
}