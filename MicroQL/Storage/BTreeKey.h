#pragma once
#include "Database\MQLColumn.h"
#include <string>
#include "CommonUtility.h"
#include "Storage\FileManager.h"
#include <iostream>
/*
  
  ---------------------------
  |
  |[chars]Key[rightchildpage]
  |--------------------------

*/
class BTreeKey {
public:
	BTreeKey() {
				value = ""; 
		rightChildPage = 0;
		payloadOffSet =0;
		leftChildPage = 0;
		mOverFlowPage  = 0;
		deleted = false;
	}

	void setValue(string &pValue) {
		value = pValue;
	}
	BTreeKey( string &pValue) {
	//	column = pColumn;
		value = pValue; 
		rightChildPage = 0;
		payloadOffSet =0;
		leftChildPage = 0;
		mOverFlowPage  = 0;
		deleted = false;
	}

	void setRightPage(int rightPageNo) {
		rightChildPage  = rightPageNo;
	}

	void setLeftChildPage(int left) {
		leftChildPage = left;
	}

	
	void setRightChildPageAndWrite(FileManager *fm, int pageNo, int right) {
		if (myKeyOffSet < 0 || myKeyOffSet > FileManager::PageSize) {
			throw new exception("Not valid keyoffset");
		}
		// skip the no of chars and chars itself
		fm->writeAt(CommonUtility::convertShortTo2Bytes(right) , pageNo, myKeyOffSet+ value.size() + 5);		
		setRightPage(right);		
	}

	void setLeftChildPageAndWrite(FileManager *fm, int pageNo, int left) {
		if (myKeyOffSet < 0 || myKeyOffSet > FileManager::PageSize) {
			throw new exception("Not valid keyoffset");
		}
		// skip the no of chars and chars itself
		fm->writeAt(CommonUtility::convertShortTo2Bytes(left), pageNo, myKeyOffSet+ value.size() + 3);		
		setLeftChildPage(left);		
	}

	void setPayload(string &ppayLoad) {
		payLoad = ppayLoad; // hardwork does pay off
	}

	int getTotalSpaceRequired() {
		return serialize().size() + serializePayload().size();
	}

	ListChar serializePayload() {
		ListChar result;
		CommonUtility::appendIntToList(result, payLoad.size());

		CommonUtility::convertStringToList(payLoad, result);
		return result;

	}
	ListChar serialize() {
		ListChar result;

		
		if (deleted){
			result.push_back('1');			
		} else 
			result.push_back('0');

		CommonUtility::appendIntToList(result, value.size());
		CommonUtility::convertStringToList(value, result);
		CommonUtility::appendIntToList(result, leftChildPage);
		CommonUtility::appendIntToList(result, rightChildPage);
		CommonUtility::appendIntToList(result, payloadOffSet);

		
		
		return result;
	}

	static BTreeKey deSerialize(string &content, int &toSkipChar) {

		int currPosition = 0;
		
		bool deleted = content.at(currPosition) == '1';
			
		currPosition++;

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
		btreek.setDeleted(deleted);
		return btreek;
	}

	string getPayload() {
		return payLoad;
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
		if (offset > FileManager::PageSize)
			throw new exception("invalid offset");
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

	bool isDeleted() {
		return deleted;
	}

	void setDeleted(bool value) {
		deleted = value;
	}

	int getKeyOffset() {
		return myKeyOffSet;
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
	bool deleted; 
};