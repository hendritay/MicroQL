#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include "Database/TableDefinition.h"
#include "Database/TableDictionary.h"
#include "Database/DeleteDefinition.h"
#include<stdafx.h>
using namespace std;
class Parser
{
public:
	Parser(void);
	~Parser(void);

private:

	//General Functions
	bool replaceSubstring(string& inputString, const string& replaceTarget, const string& replaceValue);
	string getTableName(string tableInfo, string command);
	string getPriKey(string priKeyInfo);
	bool getAndAddCol(string colInfo, string priKey);
	bool parseCreateCmd(string command);
	bool parseDeleteCmd(string command);
	bool parseUpdateCmd(string command);
	bool parseInsertCmd(string command);
	bool parse(string command);

	//Variables
	string tableName;
	TableDefinition  *td;
	MQLColumn mqlCol;
	DeleteDefinition *delDef;
}