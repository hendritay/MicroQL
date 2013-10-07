#include "MQLColumn.h"

class MQLCondition {
public:
	MQLCondition(MQLColumn column1, string &oper,  MQLColumn column2);
	MQLCondition(MQLColumn column1, string &oper,  string &result);
};