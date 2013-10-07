#include <string>
#include "MQLColumn.h"
#include <vector>

using namespace std;

class TableDefinition {
public:
	MQLColumn getColumnAt(int i); 
	string getTableName();
	void addColumn(MQLColumn column);

private:
	string tableName;
	vector<MQLColumn> columnList;

}