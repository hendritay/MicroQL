#pragma once
#include <vector>
#include <string>
#include <iostream>

#include "Storage\BTree.h"

#include "Database\MQLColumn.h"
#include "Database\MQLCondition.h"
#include "Database\MQLTupleManager.h"
#include "Database\TableDictionary.h"

using namespace std;

class TableResult {
public:
	
	TableResult() {
	}
	TableResult(TableDictionary *td, BTree *bTree) {
		tableDict = td;
		myBTree = bTree;
		tm = new MQLTupleManager();

	}
	void loadResult(string tableName, vector<MQLColumn> &column, vector<MQLCondition> &condition){// dummy method method parameter subject to change	
		TableDefinition td = tableDict->getTableDefinition(tableName);
		int size = td.getNoOfColumn();
		for (int i = 0; i < size; i++) {
			tm->addColumnList(td.getColumnAt(i));
		}		

		vector<MQLCondition>::iterator iter; 

		for (iter = condition.begin(); iter != condition.end(); iter++) {
			tm->addConditionColumn(*iter);
		}
		
		myBTree->scan(td.getRecordPage(), tm);

		int sizeOfTM = tm->getNoOfColumn();
		vector<MQLColumn>::iterator iterCol;
		bool exists;
		for (int i = 0; i < sizeOfTM; i++) {
			exists = false;
			for (iterCol = column.begin(); iterCol != column.end(); iterCol++) {
				if (iterCol->getColumnName().compare(tm->getColumnAt(i).getColumnName()) == 0)  {
					exists = true;
					break;
				}
			}

			if (!exists) {
				tm->removeColumnAt(i);
				i--;
				sizeOfTM--;
			}
		}
	}

			

	static TableResult merge(TableResult *tb, TableResult *tb2, TableDictionary *tableDict, BTree *myBTree) {
		TableResult tr(tableDict, myBTree);
		tr.addTupleManagerDef(tb->getTupleManager());
		tr.addTupleManagerDef(tb2->getTupleManager());
		return tr;
	}
	
	int getNoOfRows() {
		return tm->getNoOfRows();
	}
	string getValueAt(int rowIndex, int columnIndex) {
		return tm->getValueAt(rowIndex, columnIndex);
	}
	int getNoOfColumns() {
		return tm->getNoOfColumn();
	}
	void addMergedRow(TableResult *tb1, int rowIndex1, TableResult *tb2, int rowIndex2) {
		MQLTupleManager *tm1 = tb1->getTupleManager();
		MQLTupleManager *tm2 = tb2->getTupleManager();		

		MQLTuple td = MQLTupleManager::mergeTuple(tm1, rowIndex1, tm2, rowIndex2);

		if (td.getNoColumn() != tm->getNoOfColumn()) {
			cout <<"Column of Tuple Manager don't match with tb1 and tb2";
			cout << "Didn't add";
		}
		
		tm->addTuple(td);
	}
protected:

	MQLTupleManager *getTupleManager() {
		return tm;
	}
	
	void addTupleManagerDef(MQLTupleManager *pTM) {
		int size = pTM->getNoOfColumn();

		for (int i = 0; i < size; i++) {
			tm->addColumnList(pTM->getColumnAt(i));
		}
	}
	
private:
	MQLTupleManager *tm;
	TableDictionary *tableDict;
	BTree *myBTree ;

};