#include <string>
#include "MQLColumn.h"

using namespace std;

class TableDefinition {
public:
	MQLColumn getColumnAt(int i); 
	string getTableName();
private:
	string tableName;

}