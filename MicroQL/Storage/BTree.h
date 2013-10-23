#pragma once
#include <string>
#include "Storage\PageManager.h"
#include "CommonUtility.h"
#include "Storage\BTreeKey.h";
#include "Storage\BTreePage.h"
#include "Storage\FileManager.h";
#include "Storage\StorageManager.h";
#include "Database\MQLTupleManager.h";

#include "Database\MQLCondition.h"

#include <map>
#include <vector>
using namespace std;
class BTree {
public:
	BTree(FileManager *fm, StorageManager *sm) { // 1003
		myFileManager = fm;
		myStorageManager  = sm;
	}

	static void InitializePage(FileManager *fm, StorageManager *sm, int pageNo) {
		BTreePage page1(fm, sm);
		page1.rewriteToNewPage(pageNo);
	}

	void scanAllKey(int rootPage, vector<BTreeKey> &bKey) {
		scan(rootPage, bKey);
	}

	void scanAllKey(int rootPage, MQLTupleManager *tm) {
		scan(rootPage, tm);
	}
	void  scan(int rootPageNo, MQLTupleManager *tm) {

		string pageContent = myFileManager->readPage(rootPageNo);
		BTreePage myBTreePage = BTreePage::deSerializePage(myFileManager, myStorageManager, rootPageNo, pageContent, true);

		if (myBTreePage.getKeySize() == 0) { // root page {
			return;
		} 

		int size = myBTreePage.getKeySize();

		for (int i = 0; i < size; i++) {
			BTreeKey &key = myBTreePage.getBKey(i);
			if (!key.isDeleted()) {
				MQLTuple tuple = MQLTuple::deSerialize(tm, key.getPayload());			
				if (tm->match(tuple)) {
					tuple.setPageLocation(rootPageNo);
					tuple.setBKeyOffset(key.getKeyOffset());

					tm->addTuple(tuple);
				}
			}

			if (key.getLeftChildPage() != 0) {
				scan(key.getLeftChildPage(), tm);
			} 

			if (i == size -1 ){
				if (key.getRightChild() != 0)
					scan(key.getRightChild(), tm);
			}
		}


	}	
	void insertRecord(int rootPage, BTreeKey bKey) {



		BTreePage targetPage(myFileManager, myStorageManager);
		int pageNo; 
		searchRecord(bKey.getValue(), rootPage, targetPage, pageNo);

		append(targetPage, bKey);

	}

	bool getBKey(string target, int rootPage, BTreeKey &bKey) {
		return searchSingleRecord(target, rootPage, bKey);
	}
private:
	StorageManager *myStorageManager;
	FileManager *myFileManager;	



	void append(BTreePage &bTreePage, BTreeKey bKey) {
		bTreePage.append(bKey);		


		if (bTreePage.mustSplit()) {
			BTreeKey danglingKey;
			BTreePage page1(myFileManager, myStorageManager);
			BTreePage page2(myFileManager, myStorageManager);
			bTreePage.splitPage(page1, page2, danglingKey);
			danglingKey.setLeftChildPage(page1.getMyPage());
			danglingKey.setRightPage (page2.getMyPage());


			if (page1.getParent() != 0){
				string page = myFileManager->readPage(page1.getParent());
				BTreePage parentPage = BTreePage::deSerializePage(myFileManager, myStorageManager,  page1.getParent(), page, true);

				append(parentPage, danglingKey);
			} else { // special case (parent) 
				BTreePage rootPage(myFileManager, myStorageManager);				

				int page1NewPage = myStorageManager->GiveMeFreePageNo();
				int page1CurrentPage = page1.getMyPage();
				page1.setParent(page1CurrentPage);
				page2.setParentAndWrite(page1CurrentPage);				
				page1.rewriteToNewPage(page1NewPage);		

				danglingKey.setLeftChildPage(page1NewPage);
				rootPage.addKey(danglingKey);	
				rootPage.rewriteToNewPage(page1CurrentPage);
			}
		} else {
			bTreePage.save();
		}
	}



	void  scan(int rootPageNo, vector<BTreeKey> &bKey) { 

		string pageContent = myFileManager->readPage(rootPageNo);
		BTreePage myBTreePage = BTreePage::deSerializePage(myFileManager, myStorageManager, rootPageNo, pageContent, true);
		if (myBTreePage.getKeySize() == 0) { // root page {
			return;
		} 

		int size = myBTreePage.getKeySize();

		for (int i = 0; i < size; i++) {
			BTreeKey &key = myBTreePage.getBKey(i);
			if (!key.isDeleted())
				bKey.push_back(key);

			if (key.getLeftChildPage() != 0) {
				scan(key.getLeftChildPage(), bKey);
			} 

			if (i == size -1 ){
				if (key.getRightChild() != 0)
					scan(key.getRightChild(), bKey);
			}
		}


	}	



	bool searchSingleRecord(string target, int rootPageNo, BTreeKey &bKey) {

		string pageContent = myFileManager->readPage(rootPageNo);
		BTreePage myBTreePage = BTreePage::deSerializePage(myFileManager, myStorageManager, rootPageNo, pageContent, false);
		if (myBTreePage.getKeySize() == 0) { // root page {
			return false;
		}

		int targetIndex = myBTreePage.findKey(target);
		if (myBTreePage.findKey(target) >= 0) {
			// found 
			bKey = myBTreePage.getBKey(targetIndex);
			return true;
		} else {

			BTreeKey bKey = myBTreePage.findIntermediate(target);
			if (bKey.getLeftChildPage() != 0 ) {
				if (bKey.getValue().compare(target) < 0)
					return searchSingleRecord(target, bKey.getRightChild(), bKey);
				else
					return searchSingleRecord(target, bKey.getLeftChildPage(), bKey);
			} else {
				// found suitable node to be inserted to. 
				return false;
			}
		}
		return false;
	}	


	void  searchRecord(string target, int rootPageNo, BTreePage &bTreePage, int &pageNo) {

		string pageContent = myFileManager->readPage(rootPageNo);
		BTreePage myBTreePage = BTreePage::deSerializePage(myFileManager, myStorageManager, rootPageNo, pageContent,true);
		if (myBTreePage.getKeySize() == 0) { // root page {
			bTreePage = myBTreePage;
			pageNo = rootPageNo;
			return;
		}

		int targetIndex = myBTreePage.findKey(target);
		if (myBTreePage.findKey(target) >= 0) {
			// found 
			pageNo = rootPageNo;
			bTreePage = myBTreePage;
			return;
		} else {

			BTreeKey bKey = myBTreePage.findIntermediate(target);
			if (bKey.getLeftChildPage() != 0 ) {
				if (bKey.getValue().compare(target) < 0)
					searchRecord(target, bKey.getRightChild(), bTreePage, pageNo);
				else
					searchRecord(target, bKey.getLeftChildPage(), bTreePage, pageNo);

			} else {
				// found suitable node to be inserted to. 
				pageNo = rootPageNo;
				bTreePage = myBTreePage;
				return;
			}
		}
	}	

};