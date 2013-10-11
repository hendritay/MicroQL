#pragma once
#include "Database\MQLColumn.h"
#include <string>
#include "CommonUtility.h"

/*
  
  ---------------------------
  |
  |[chars]Key[rightchildpage]
  |--------------------------

*/
class BTreeKey {
public:
	BTreeKey( string &pValue) {
	//	column = pColumn;
		value = pValue; 
		rightChildPage = 0;
		payloadOffSet =0;
	}

	void setRightPage(int rightPageNo) {
		rightChildPage  = rightPageNo;
	}

	void setPayload(string &ppayLoad) {
		payLoad = ppayLoad; // hardwork does pay off
	}

	string serialize() {
		string result;
		result.append(CommonUtility::convertShortTo2Bytes(value.size()));
		result.append(value);
		result.append(CommonUtility::convertShortTo2Bytes(rightChildPage));
		result.append(CommonUtility::convertShortTo2Bytes(payloadOffSet));
		return result;
	}

	static BTreeKey deSerialize(string &content, int &toSkipChar) {
		int currPosition = 0;
		int NoOfBytes = 2;
		int charsNo = CommonUtility::convert2BytesToInt(content.substr(currPosition, NoOfBytes));
		currPosition += NoOfBytes;

		string key = content.substr(currPosition, charsNo);
		currPosition += charsNo;

		int rightPageNo = CommonUtility::convert2BytesToInt(content.substr(currPosition, NoOfBytes));
		currPosition += NoOfBytes;

		int payLoadOffset = CommonUtility::convert2BytesToInt(content.substr(currPosition, NoOfBytes));
		currPosition += NoOfBytes;

		toSkipChar = currPosition;
		BTreeKey btreek(key);
		btreek.setRightPage(rightPageNo);
		return btreek;

	}

	int getPayloadOffSet() {
		return payloadOffSet;
	}

	string getValue() {
		return value;
	}

	int getRightChild() {
		return rightChildPage;
	}
private:
	
	string value;
	BTreeKey *nextNode;
	int rightChildPage;
	int payloadOffSet;
	string payLoad; // record

};