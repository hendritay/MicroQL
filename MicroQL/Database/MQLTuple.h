#pragma once
#include "MQLColumn.h"
#include <list>

using namespace std;
class MQLTupleManager;

class  MQLTuple {
public:
	MQLTuple(MQLTupleManager *parent) {
		myParent = parent;
	}
	void addValue(string value) {
		valueList.push_back(value);
	}
private:
	//vector<MQLColumn> column;
	list<string> valueList;
	MQLTupleManager *myParent;
};