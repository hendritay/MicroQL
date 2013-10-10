#pragma once
#include <vector>
#include <list>
#include "Database\MQLTuple.h";
#include "Database\MQLColumn.h"

typedef vector<MQLTuple> TupleList;

class TupleManager {
public :
	void addColumnList(MQLColumn);
private:
	vector<MQLColumn> columnList;
	TupleList tupleList;
};