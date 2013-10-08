#include <string>
#include "MQLColumn.h"
#include <vector>

using namespace std;

class TableDefinition {
public:
	MQLColumn getColumnAt(int i); 
	string getTableName();
	void addColumn(MQLColumn column);

	void create();
private:
	string tableName;
	vector<MQLColumn> columnList;
	MQLColumn PrimaryKey;
}