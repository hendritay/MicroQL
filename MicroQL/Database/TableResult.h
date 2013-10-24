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
		this->tm  = NULL;
	}

	~TableResult() {
	/*	if (this->tm != NULL)  {
			delete this->tm ;
			this->tm  = NULL;
		}*/
	}
	TableResult(TableDictionary *td, BTree *bTree) {
		tableDict = td;
		myBTree = bTree;
		tm = new MQLTupleManager();

	}

	void setDeleteMark(FileManager *fm) {
		tm->setDeleteMark(fm);
	}

	void startUpdate(FileManager *fm, StorageManager *sm, vector<MQLCondition> &cond, string primaryKeyColumn) {
		tm->startUpdate(fm, sm, cond, primaryKeyColumn);
	}
	void loadResultStar(string tableName) {
		if (!tableDict->tableExists(tableName)){
			cout <<"Table Name not exits";
			return; 
		}
		TableDefinition td = tableDict->getTableDefinition(tableName);
		int size = td.getNoOfColumn();
		for (int i = 0; i < size; i++) {
			tm->addColumnList(td.getColumnAt(i));
		}

		myBTree->scan(td.getRecordPage(), tm);
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
			return;
		}
		
		tm->addTuple(td);
	}

	vector<MQLColumn> getColumnList() {
		return tm->getColumnList();
	}

	void print() {
			cout << left;
		
		int size = tm->getNoOfColumn();
		int columnSize = 79 / size;

		for (int i = 0; i < size; i++) {
			cout << setw(columnSize);
			cout << tm->getColumnAt(i).getColumnName();
		}
		cout << endl;
		cout << setfill('=') << setw(size * columnSize) << "";
		cout << setfill(' ');
		cout << endl;
		int noOfTuples = tm->getNoOfRows();

		for (int i = 0; i < noOfTuples; i++) {
			for (int j = 0; j < size; j++) {
				cout << setw(columnSize);
				cout << tm->getValueAt(i, j);				
			}
			cout << endl;
		}
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