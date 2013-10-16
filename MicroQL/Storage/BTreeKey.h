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
	BTreeKey() {
	}
	BTreeKey( string &pValue) {
	//	column = pColumn;
		value = pValue; 
		rightChildPage = 0;
		payloadOffSet =0;
		leftChildPage = 0;
		mOverFlowPage  = 0;
	}

	void setRightPage(int rightPageNo) {
		rightChildPage  = rightPageNo;
	}

	void setLeftChildPage(int left) {
		leftChildPage = left;
	}

	void setPayload(string &ppayLoad) {
		payLoad = ppayLoad; // hardwork does pay off
	}

	int getTotalSpaceRequired() {
		return serialize().size() + serializePayload().size();
	}

	string serializePayload() {
		string result;
		result.append(CommonUtility::convertShortTo2Bytes(payLoad.size()));
		result.append(payLoad);
		return result;

	}
	string serialize() {
		string result;
		result.append(CommonUtility::convertShortTo2Bytes(value.size()));
		result.append(value);
		result.append(CommonUtility::convertShortTo2Bytes(leftChildPage));
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

		int leftPageNo = CommonUtility::convert2BytesToInt(content.substr(currPosition, NoOfBytes));
		currPosition += NoOfBytes;

		int rightPageNo = CommonUtility::convert2BytesToInt(content.substr(currPosition, NoOfBytes));
		currPosition += NoOfBytes;

		int payLoadOffset = CommonUtility::convert2BytesToInt(content.substr(currPosition, NoOfBytes));
		currPosition += NoOfBytes;

		toSkipChar = currPosition;
		BTreeKey btreek(key);
		btreek.setRightPage(rightPageNo);
		btreek.setLeftChildPage(leftPageNo);
		btreek.setPayloadOffset(payLoadOffset);
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

	/*string getPayLoad() {
		return payLoad;
	}*/

	void setPayloadOffset(int offset) {
		payloadOffSet = offset;
	}

	int getLeftChildPage() {
		return leftChildPage;
	}

	void setOverFlowPage(int overflow) {
		mOverFlowPage = overflow;		
	}

	int getOverFlowPage() {
		return mOverFlowPage;
	}

	void setKeyOffset(int off) {
		myKeyOffSet	= off;
	}
private:
	
	string value;
	//BTreeKey *nextNode;
	int myKeyOffSet; // to reset left right page. 
	int rightChildPage;
	int leftChildPage; 
	int payloadOffSet; // Where the payload resides at the same page
	string payLoad; // record
	int mOverFlowPage;
};