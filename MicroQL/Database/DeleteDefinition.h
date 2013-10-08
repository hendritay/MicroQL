#include <vector>
#include <string>

#include "MQLColumn.h"
#include "MQLCondition.h"

using namespace std;

class DeleteDefinition {
public:
	string getTableName();
	MQLColumn getColumnAt(int i); 	
	void addColumn(MQLColumn column);
	MQLCondition getCondition(int i);
	void addCondition(MQLCondition cond1);
	void execute();
private:
	string tableName;
	vector<MQLColumn> updateColumn;
	vector<MQLCondition> whereColumn;

	
};