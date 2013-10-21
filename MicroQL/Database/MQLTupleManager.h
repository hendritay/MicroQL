#pragma once
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
				if (!tp.getValueAt(*iter).compare(iterCond->getColumn2().getColumnName())) {
					return false;
				}
			} else {
				int columnNo = getColumnNo(iterCond->getColumn2().getColumnName());

				if (!tp.getValueAt(*iter).compare(tp.getValueAt(columnNo)))
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
	}

	static MQLTuple mergeTuple(MQLTupleManager *tm1, int index1, MQLTupleManager *tm2, int index2) {

		MQLTuple t1 = tm1->getTupleAt(index1);
		MQLTuple t2 = tm2->getTupleAt(index2);

		MQLTuple tp(t1);

		int size2 = t2.getNoColumn();

		for (int i = 0; i < size2; i++) {
			tp.addValue(t2.getValueAt(i));
		}
		return tp;
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