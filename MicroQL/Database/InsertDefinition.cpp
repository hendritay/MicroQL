#include "stdafx.h"
#include "InsertDefinition.h"
#include "Storage\BTreeKey.h"
#include "Database\MQLTuple.h"
#include "Storage\BTree.h"
#include "Database\MQLTupleManager.h"
#include <iostream>


string InsertDefinition::getTableName() {
	return tableName;
}

			
void InsertDefinition::addInsert(MQLCondition cond) {
	
	InsertColumn[cond.getColumn1().getColumnName()] = cond;
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