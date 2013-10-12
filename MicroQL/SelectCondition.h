#ifndef SelectCondition_h
#define SelectCondition_h


#include "stdafx.h"
#include <string>
using namespace std;

class SelectCondition{
	public:
		SelectCondition :: SelectCondition();
		SelectCondition :: SelectCondition(string leftValue, bool isLeftStr, string oper, string rightValue, bool isRightStr);
		string leftValue;
		bool isLeftStr;
		string oper;
		string rightValue;
		bool isRightStr;
};
#endif