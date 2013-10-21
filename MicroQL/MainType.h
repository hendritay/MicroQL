#pragma once
#include <list>

typedef std::list<char> ListChar;
enum ColumnType {
  CT_VARCHAR, 
  CT_CONSTANT_STRING,
  CT_CONSTANT_INTEGER 
};