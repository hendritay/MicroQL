
#include "stdafx.h"
#include <string>
#include "SelectCondition.h"
using namespace std;
SelectCondition :: SelectCondition(){ }
SelectCondition :: SelectCondition(string leftValue, bool isLeftStr, string oper, string rightValue, bool isRightStr)
:leftValue(leftValue), isLeftStr(isLeftStr), oper(oper), rightValue(rightValue), isRightStr(isRightStr){}