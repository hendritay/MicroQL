#include <string>
#include "../MainType.h"

using namespace std;

/* USAGE 
  CREATE TABLE a (ABC, 


*/ 

class MQLColumn {
public:
    MQLColumn(string &name, ColumnType ct, int size) {
	    columnName = name;
		columnSize = size;
		columnType = ct;
	}
private:
	string columnName;
	int columnSize; 
	ColumnType columnType; //always string 
};