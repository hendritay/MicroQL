#include "MQLColumn.h"

class MQLCondition {
public:
	MQLCondition(MQLColumn column1, string &oper,  MQLColumn column2) {
		myColumn1 = column1;
		myColumn2 = column2;
	}
private:
	MQLColumn myColumn1;
	MQLColumn myColumn2; 
};