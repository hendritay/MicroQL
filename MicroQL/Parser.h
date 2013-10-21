#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include "Database\TableDefinition.h"
#include "Database\TableDictionary.h"
#include "Database\DeleteDefinition.h"
#include "Database\UpdateDefinition.h"
#include "Database\MQLCondition.h"
#include "Database\InsertDefinition.h"
using namespace std;
class Parser
{
public:
    Parser(void);
    ~Parser(void);

	TableDefinition* parseCreateCmd(string command);
    DeleteDefinition* parseDeleteCmd(string command);
    UpdateDefinition* parseUpdateCmd(string command);
    InsertDefinition* parseInsertCmd(string command);
private:
  
    //General Functions
    bool replaceSubstring(string& inputString, const string& replaceTarget, const string& replaceValue);
    string getTableName(string tableInfo, string command);
    string getPriKey(string priKeyInfo);
    bool getAndAddCol(string colInfo, string priKey, TableDefinition &td);
    bool preCheck(string command);
    string preprocessCommand(string sqlommand);
  
    //Variables
    TableDefinition  *td;
    MQLColumn mqlCol;
    DeleteDefinition *delDef;
    UpdateDefinition *upDef;
    InsertDefinition *insDef;
};