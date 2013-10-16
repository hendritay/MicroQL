#include <map>
#include <string>
#include "Storage\PageManager.h"
#include "Storage\BTreeKey.h"
#include "Storage\FileManager.h"
#include "Storage\StorageManager.h"
#include <vector>

// right side of me inherit my right key for his left key 
// i inherit left side's right key as my left key;
using namespace std;
typedef map<string, BTreeKey> BKeyList;
class BTreePage {
public :


	BTreePage(FileManager *fm, StorageManager *sm ) {
		myFileManager = fm;
		myStorageManager =sm;
		IsNewKey = false;
		mNextSibling = 0;
		mParent = 0;
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


	static BTreePage deSerializePage(FileManager *fm, StorageManager *sm, int pageNo, string page) {

		BTreePage myBTreePage(fm, sm);

		int currPosition = 0;
		if (page[0] != PageManager::RECORDPAGE) {
			throw new exception("this is no record page");
		}

		currPosition += 1;

		short NoOfBytes = 2;		
		int noOfKeys = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));

		currPosition += NoOfBytes;
		int parent = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));

		currPosition += NoOfBytes;
		int nextSibling = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));


		currPosition += NoOfBytes;
		int firstFreeOffset = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
		currPosition += NoOfBytes;

		int secondFreeOffset = CommonUtility::convert2BytesToInt(page.substr(currPosition, NoOfBytes));
		currPosition += NoOfBytes;

		int toSkip = 0;

		string content = page.substr(currPosition);
		map<string, BTreeKey> *listOfKey = new map<string, BTreeKey> ();

		for (int i = 0; i < noOfKeys; i++) {			
			BTreeKey bTree = BTreeKey::deSerialize(content, toSkip);
			content.erase(content.begin(), content.begin() + toSkip);
			// get or no get the payload, that's the question
			if (page[bTree.getPayloadOffSet()]  == PageManager::NORMALROW ) { //
				int NoOfChars = CommonUtility::convert2BytesToInt(page.substr(bTree.getPayloadOffSet() + 1, NoOfBytes));
				bTree.setPayload(page.substr(bTree.getPayloadOffSet() + 1 + NoOfBytes, NoOfChars));
			} else {
				int pageNo = CommonUtility::convert2BytesToInt(page.substr(bTree.getPayloadOffSet() + 1, NoOfBytes));
				string overflowPage = fm->readPage(pageNo);

				if (overflowPage[0] != PageManager::OVERFLOWPAGE) {
					throw new exception("This is no overflow page");
				} else {
					int OverNoOfChars = CommonUtility::convert2BytesToInt(overflowPage.substr(1, NoOfBytes));
					bTree.setPayload(overflowPage.substr(NoOfBytes + 1, OverNoOfChars)); // assume the entire over flow page to 
				}
			}

			myBTreePage.addKey(bTree);
		}

		myBTreePage.setNextSibling(nextSibling);
		myBTreePage.setParent(parent);
		myBTreePage.setFirstFreeOffset(firstFreeOffset);
		myBTreePage.setSecondFreeOffset(secondFreeOffset);
		myBTreePage.setMyPage(pageNo);

		return myBTreePage;
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

	

	// find intermediate will return you the page 
	// however you need to test the value to see if to use the right page of left page. 
	BTreeKey findIntermediate(string target) {
		map<string, BTreeKey>::iterator iter = listKey.begin(); 		
		map<string, BTreeKey>::iterator iterNext;

		if (listKey.size() == 1) {
			return listKey.begin()->second;
		}


		if (iter->first.compare(target) > 0) {
			return iter->second;
		}

		while (true) {
			iterNext = iter;
			iterNext++;
			if (iter->first.compare(target) < 0 &&  iterNext->first.compare(target) > 0) {
				return iterNext->second;
			} else 	if (iterNext == listKey.end()) {
				return iterNext->second;
			}
		}
	}

	int getKeySize() {
		return listKey.size();
	}

	int getFirstFreeOffset() {
		return firstFreeOffset;
	}

	int getSecondFreeOffset() {
		return secondFreeOffset;
	}

	// staging for adding into Page STring
	void append(BTreeKey bKey) {
		listKey[bKey.getValue()] = bKey; 
		newKey = &(listKey.find(bKey.getValue())->second);
		IsNewKey =true;
	}

	void addKey(BTreeKey bkey) {
		listKey[bkey.getValue()] = bkey; 

	}

	void rewriteToNewPage(int pageNo) {

		int currPosition = 0;
		string page; 
		string pageEnd;

		// Write Page Type
		page.push_back(PageManager::RECORDPAGE);

		//Write no key ;
		page.append(CommonUtility::convertShortTo2Bytes (listKey.size()));

		//Write parent;		
		page.append(CommonUtility::convertShortTo2Bytes (mParent));

		// write sibling -- MAKE SURE Sibling is properly set. 
		page.append(CommonUtility::convertShortTo2Bytes (mNextSibling));


		BKeyList::iterator iter;

		// reinitialize the freeOffset;
		// pageType + No Key + Parent + Next Sibling + FreeOffset Begin + Second Offset
		firstFreeOffset = 1 + 2 + 2 + 2 + 2 + 2; // start from second 

		secondFreeOffset =  FileManager::PageSize - 1;
		for (iter = listKey.begin(); iter != listKey.end(); iter++) {
			// update the first free offset
			string key = iter->second.serialize();
			string payload = iter->second.serializePayload ();

			firstFreeOffset += key.size();			

			if (payload.size() > MAX_CHARS_IN_SAME_PAGE) {

				// update offset
				secondFreeOffset -= 3; 
				// this order is correct;
				pageEnd.insert(0, CommonUtility::convertShortTo2Bytes(iter->second.getOverFlowPage()));
				pageEnd.insert(pageEnd.begin(), PageManager::OVERFLOWROW);

			} else {
				secondFreeOffset -= payload.size();
				secondFreeOffset--; 
				// the order is correct!
				pageEnd.insert(0, payload);
				pageEnd.insert(payload.begin(), PageManager::NORMALROW);		
			}
		}

		page.insert(FirstFreePosition, CommonUtility::convertShortTo2Bytes(firstFreeOffset));
		page.insert(SecondFreePosition, CommonUtility::convertShortTo2Bytes(secondFreeOffset));

		myFileManager->writeAt(page, pageNo, 0);
		myFileManager->writeAt(pageEnd, pageNo, secondFreeOffset);
	}

	// ASSUME the new key left and right page has been set properly 

	void splitPage(BTreePage &page1, BTreePage &page2, BTreeKey &danglingKey) {

		int middle = listKey.size() / 2; 

		map<string, BTreeKey>::iterator iter;

		int i= 0; 

		for (iter = listKey.begin(); iter != listKey.end(); iter++) {
			if ( i < middle) {
				page1.addKey(iter->second);
			} else if (i > middle) {
				page2.addKey(iter->second);
			} else {
				danglingKey = iter->second;
			}
			i++;
		}

		int Page2NewPage = myStorageManager->GiveMeFreePageNo();

		// inherit the parent
		page1.setParent(mParent);
		page2.setParent(mParent);
		// inherit the next page sibling 
		page2.setNextSibling(mNextSibling);
		// they are sibling now 
		page1.setNextSibling(Page2NewPage);

		page1.rewriteToNewPage(myPage);
		page2.rewriteToNewPage(Page2NewPage);

	}

	bool mustSplit() {

		if (!IsNewKey) 
			return false;

		if (listKey.size() > MAX_KEY)
			return true;

		if (newKey->getTotalSpaceRequired() > getFreeChars()) {
			return true;
		}		
		return false;
	}

	// MADNESS, HOPE I WON'T FORGET AFTER 2 MONTHS 

	void save() { // Save The New Key into the page.  // partial key only to save I/O

		if (!IsNewKey) 
			return;

		// Increase Key 
		myFileManager->writeAt(CommonUtility::convertShortTo2Bytes(listKey.size()), myPage, NoKeyPosition);
	
		// getPayload first
		string payload = newKey->serializePayload(); 
		if (payload.size() > MAX_CHARS_IN_SAME_PAGE) {
			// write it overflow page;
			// request new page for overflow string 
			int requestNewPage = myStorageManager->GiveMeFreePageNo();
			string payload = newKey->serializePayload();
			payload .insert(payload.begin(), PageManager::OVERFLOWPAGE);
			myFileManager->writeAt(payload, requestNewPage, 0);

			// update the record page
			secondFreeOffset -= 3; 
			string payloadOverflow;
			payloadOverflow.push_back(PageManager::OVERFLOWROW);
			payloadOverflow.append(CommonUtility::convertShortTo2Bytes(requestNewPage));			

			newKey->setPayloadOffset(secondFreeOffset);
			newKey->setOverFlowPage(requestNewPage);

			myFileManager->writeAt(payloadOverflow, myPage, secondFreeOffset);
			myFileManager->writeAt(CommonUtility::convertShortTo2Bytes(secondFreeOffset), myPage, SecondFreePosition);

		} else {
			secondFreeOffset -= payload.size();
			secondFreeOffset--; 
			newKey->setPayloadOffset(secondFreeOffset);
			payload.insert(payload.begin(), PageManager::NORMALROW);
			// write the record
			myFileManager->writeAt(payload, myPage, secondFreeOffset);
			// update the offset 
			myFileManager->writeAt(CommonUtility::convertShortTo2Bytes(secondFreeOffset), myPage, SecondFreePosition);
		}

		string key = newKey->serialize();

		// write the actual key 
		myFileManager->writeAt(key, myPage, firstFreeOffset);

		// update the first free offset
		firstFreeOffset += key.size();
		myFileManager->writeAt(CommonUtility::convertShortTo2Bytes(firstFreeOffset), myPage, FirstFreePosition);

		// write the payload 
		// complete 
	}

	int getFreeChars() {
		return secondFreeOffset - firstFreeOffset;
	}

	int getParent() {
		return mParent; 
	}

	int getMyPage() {
		return myPage;
	}
	void setMyPage(int page) {
		myPage = page;
	}
private:
	static const int MAX_CHARS_IN_SAME_PAGE = 800;
	static const int NoKeyPosition = 1; // No Of Keys stored at offset 1
	static const int FirstFreePosition = 7;
	static const int SecondFreePosition = 9;

	static const int MAX_KEY = 4;
	static const int MIN_KEY = 4; 
	int myPage;
	int mParent;
	int mNextSibling;
	BKeyList listKey;
	BTreeKey *newKey;
	bool IsNewKey; // check if there is new key added. 
	FileManager *myFileManager;
	StorageManager *myStorageManager;

	int firstFreeOffset;
	int secondFreeOffset;

};