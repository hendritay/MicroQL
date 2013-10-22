#pragma once
#include "Database\MQLColumn.h"
#include <list>

using namespace std;
class MQLTupleManager;

class  MQLTuple {
public:

	MQLTuple(MQLTupleManager *parent) {
		myParent = parent;
	}
	void addValue(string value) {
		valueList.push_back(value);
	}

	string getValueAt(int i) {
		list<string>::iterator iter = valueList.begin();
		std::advance(iter, i);
		return *iter;
	}


	string serialize() {
		list<string>::iterator iter;
		string resultList;
		resultList.append(CommonUtility::convertShortToString(valueList.size()));

		for (iter = valueList.begin(); iter != valueList.end(); iter++) {
			resultList.append(CommonUtility::convertShortToString(iter->size()));

			resultList.append( *iter);
		}
		return resultList;
	}

	static MQLTuple deSerialize(MQLTupleManager *tm, string content) {

		MQLTuple tuple(tm); 
		int currPosition = 0;
		int NoOfBytes = 2;
		int columnLength ;


		int NoOfColumn = CommonUtility::convert2BytesToInt(content.substr(currPosition, NoOfBytes));
		currPosition += NoOfBytes;
		int noChars = 0;
		for (int i =0 ; i < NoOfColumn; i++) {
			noChars =  CommonUtility::convert2BytesToInt(content.substr(currPosition, NoOfBytes));
			currPosition += NoOfBytes;
			tuple.addValue(content.substr(currPosition, noChars));
			currPosition += noChars;
		}
		return tuple;

	}

	int getNoColumn() {
		return valueList.size();
	}
	
	void removeValueAt(int i) {
		list<string>::iterator iter =valueList.begin();
		advance(iter, i);
		valueList.erase(iter);
	}
private:
	
	list<string> valueList;
	MQLTupleManager *myParent;
};