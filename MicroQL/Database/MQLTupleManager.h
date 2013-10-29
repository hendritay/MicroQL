#pragma once
#include <map>

#include <vector>
#include <list>
#include "Database\MQLTuple.h";
#include "Database\MQLColumn.h"
#include "Database\MQLCondition.h"
#include <iostream>

#include "MainType.h";

typedef vector<MQLTuple> TupleList;

class MQLTupleManager {
public :
	// Column must be added first;
	// then follow by condition


	void addColumnList(MQLColumn column) {
		columnList.push_back(column);
		columnOrder[column.getColumnName()] = columnList.size() - 1;
	}

	void addConditionColumn(MQLCondition cond) {
		columnCondition.push_back(cond);
		columnNo.push_back(columnOrder.find(cond.getColumn1().getColumnName())->second);
	}

	void addTuple(MQLTuple &tp) {
		if (tp.getNoColumn() != columnList.size()) {
			cout << "Tuple's column and Tuple Manager don't match";
			throw new exception("Tuple's column and Tuple Manager don't match");
		}
		tupleList.push_back(tp);
	}

	int getNoOfColumn() {
		return columnList.size();
	}

	MQLColumn getColumnAt(int i) {
		return *(columnList.begin() + i);
	}

	int getColumnNo(string columnName) {
		return columnOrder.find(columnName)->second;		
	}

	bool match(MQLTuple &tp) {
		vector<int>::iterator iter;
		vector<MQLCondition>::iterator iterCond = columnCondition.begin();

		int index =0;

		for (iter = columnNo.begin(); iter != columnNo.end(); iter++) {

			if (iterCond->getColumn2().getColumnType() == CT_CONSTANT_STRING) {
				if (tp.getValueAt(*iter).compare(iterCond->getColumn2().getColumnName()) != 0) {
					return false;
				}
			} else {
				int columnNo = getColumnNo(iterCond->getColumn2().getColumnName());

				if (tp.getValueAt(*iter).compare(tp.getValueAt(columnNo)) != 0)
					return false;
			}

			iterCond++;
		}
		return true;
	}

	string getValueAt(int row, int col) {
		return (tupleList.begin() + row)->getValueAt(col);
	}
	int getNoOfRows() {
		return tupleList.size();
	}

	void removeColumnAt(int i) {
		TupleList::iterator iter;

		for (iter = tupleList.begin(); iter != tupleList.end(); iter++) {
			iter->removeValueAt(i);
		}

		columnList.erase(columnList.begin() + i);

	}

	static MQLTuple mergeTuple(MQLTupleManager &tm1, int index1, MQLTupleManager &tm2, int index2) {

		MQLTuple t1 = tm1.getTupleAt(index1);
		MQLTuple t2 = tm2.getTupleAt(index2);

		MQLTuple tp(t1);

		int size2 = t2.getNoColumn();

		for (int i = 0; i < size2; i++) {
			tp.addValue(t2.getValueAt(i));
		}
		return tp;
	}

	void setDeleteMark(FileManager *fm) {
		TupleList::iterator iter;

		for (iter = tupleList.begin(); iter != tupleList.end(); iter++) {
			iter->markDelete(fm);
		}
	}

	void startUpdate(FileManager *fm, StorageManager *sm, vector<MQLCondition> &updateCond, string primaryKeyColumn) {

		vector<MQLCondition>::iterator iterCond = updateCond.begin();

		
		int priColNo = getColumnNo(primaryKeyColumn);

		TupleList::iterator tupleIter;


		int colNo;
		map<int, vector<pair<string, MQLTuple*>>> mapList; // maple story
		map<int, vector<pair<string, MQLTuple*>>>::iterator iterFound;
		for (tupleIter = tupleList.begin(); tupleIter != tupleList.end(); tupleIter++) {
			for (iterCond = updateCond.begin(); iterCond != updateCond.end(); iterCond++) {

				colNo = getColumnNo(iterCond->getColumn1().getColumnName());
				// temp hack;
					tupleIter->setValueAt(colNo, iterCond->getColumn2().getColumnName());				

				/*if (iterCond->getColumn2().getColumnType() == CT_CONSTANT_STRING) {
					tupleIter->setValueAt(colNo, iterCond->getColumn2().getColumnName());				
				} else {
					int columnNo = getColumnNo(iterCond->getColumn2().getColumnName());
					tupleIter->setValueAt(colNo, tupleIter->getValueAt(columnNo));
				}*/
				
			}

			iterFound = mapList.find(tupleIter->getPageLocation());
			if (iterFound != mapList.end()) {
				pair<string, MQLTuple*> pair1(tupleIter->getValueAt(priColNo), tupleIter._Ptr);
				iterFound->second.push_back(pair1);
			} else {
				vector<pair<string, MQLTuple*>> newList;
				pair<string, MQLTuple*> pair1(tupleIter->getValueAt(priColNo), tupleIter._Ptr);
				newList.push_back(pair1);
				mapList[tupleIter->getPageLocation()]= newList;
			}			
		}

		// heavy lifting
		map<int, vector<pair<string, MQLTuple*>>>::iterator iterMap;

		vector<pair<string, MQLTuple*>>::iterator iterTuplePair;

		for (iterMap = mapList.begin(); iterMap != mapList.end(); iterMap++) {
			string page = fm->readPage(iterMap->first);

			BTreePage treePage = BTreePage::deSerializePage(fm, sm, iterMap->first, page, true);
			for (iterTuplePair = iterMap->second.begin(); iterTuplePair != iterMap->second.end(); iterTuplePair++) {
				treePage.changePayload(iterTuplePair->first, iterTuplePair->second->serialize());
			}

			treePage.rewriteToNewPage(treePage.getMyPage());
		}

		

	}
	vector<MQLColumn> getColumnList() {
		return columnList;
	}
protected:
	MQLTuple &getTupleAt(int i) {
		return *(tupleList.begin() + i);
	}

private:
	vector<MQLColumn> columnList;
	vector<MQLCondition> columnCondition;
	vector<int> columnNo;
	map<string, int> columnOrder;
	TupleList tupleList;
};