#include <string>
using namespace std;

class MQLColumn {
public:
    MQLColumn(string &name, int size) {
	    columnName = name;
		columnSize = size;
	}
private:
	string columnName;
	int columnSize; 
	string columnType; //always string 
};