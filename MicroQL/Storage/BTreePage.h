#include <map>
#include <string>
#include "Storage\PageManager.h"
#include "Storage\BTreeKey.h"
#include "Storage\FileManager.h"

using namespace std;

class BTreePage {
public :
	BTreePage(FileManager *fm ) {
		myFileManager = fm;
	}
	void setParent(int PageNo) {
		mParent = PageNo;
	}

	void setNextSibling(int SiblingPageNo) {
		mNextSibling = SiblingPageNo;	
	}

	int findKey(string key) {
		map<string, BTreeKey>::iterator iterFound = listKey.find(key);

		if (iterFound != listKey.end()) 
			return distance(listKey.begin(), iterFound);
		else
			return -1;
	}

	
	static BTreePage deSerializePage(FileManager *fm, string page) {
		
		BTreePage myBTreePage(fm);
		return myBTreePage;

		//int currPosition = 0;
		//if (page[0] != PageManager::RECORDPAGE) {
		//	throw new exception("this is no record page");
		//}

		//currPosition += 1;

		//short NoOfBytes = 2;		
		//int noOfKeys = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));

		//currPosition += NoOfBytes;
		//int parent = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));

		//currPosition += NoOfBytes;
		//int nextSibling = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));


		//currPosition += NoOfBytes;
		//int firstFreeOffset = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
		//currPosition += NoOfBytes;

		//int secondFreeOffset = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
		//int toSkip = 0;

		//string content = page.substr(currPosition);
		//map<string, BTreeKey> *listOfKey = new map<string, BTreeKey> ();

		//for (int i = 0; i < noOfKeys; i++) {			
		//	BTreeKey bTree = BTreeKey::deSerialize(content, toSkip);
		//	content.erase(content.begin(), content.begin() + toSkip);
		//	// get or no get the payload, that's the question
		//	if (page[bTree.getPayloadOffSet()]  == PageManager::NORMALROW ) { //
		//		int NoOfChars = CommonUtility::convert2BytesToInt(page.substr(bTree.getPayloadOffSet() + 1, NoOfBytes));
		//		bTree.setPayload(page.substr(bTree.getPayloadOffSet() + 1 + NoOfBytes, NoOfChars));
		//	} else {
		//		int pageNo = CommonUtility::convert2BytesToInt(page.substr(bTree.getPayloadOffSet() + 1, NoOfBytes));
		//		string overflowPage = fm->readPage(pageNo);

		//		if (overflowPage[0] != PageManager::OVERFLOWPAGE) {
		//			throw new exception("This is no overflow page");
		//		} else {
		//			int OverNoOfChars = CommonUtility::convert2BytesToInt(overflowPage.substr(1, NoOfBytes));
		//			bTree.setPayload(overflowPage.substr(NoOfBytes + 1, OverNoOfChars)); // assume the entire over flow page to 
		//		}
		//	}

		//	myBTreePage.addBTreeKey(bTree);
	
		//}
		//myBTreePage.setNextSibling(nextSibling);
		//myBTreePage.setParent(parent);
		//myBTreePage.setFirstFreeOffset(firstFreeOffset);
		//myBTreePage.setSecondFreeOffset(secondFreeOffset);


		//return myBTreePage;
	}
	
//	void addBTreeKey(BTreeKey btk) {
	//	listKey[btk.getValue()] = btk;
	//}

	void setFirstFreeOffset(int first) {
		firstFreeOffset = first;
	}

	void setSecondFreeOffset(int second) {
		secondFreeOffset = second;
	}

	BTreeKey findIntermediate(string target) {
		map<string, BTreeKey>::iterator iter = listKey.begin(); 		
		map<string, BTreeKey>::iterator iterNext;

		if (listKey.size() == 1) {
			return listKey.at(0);
		}

		while (true) {
			iterNext = iter;
			iterNext++;
			if (iter->first.compare(target) < 0 &&  iterNext->first.compare(target) > 0) {
				return iter->second;
			} else 	if (iterNext == listKey.end()) {
				return iter->second;
			}
		}
	} 
private:
	int mParent;
	int mNextSibling;
	map<string, BTreeKey> listKey;
	FileManager *myFileManager;
	int firstFreeOffset;
	int secondFreeOffset;

};