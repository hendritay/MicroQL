#pragma once
#include <string>
#include "Storage\PageManager.h"
#include "CommonUtility.h"
#include "Storage\BTreeKey.h";
#include "Storage\BTreePage.h"
#include "Storage\FileManager.h";
#include "Storage\StorageManager.h";

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
	void insertRecord(int rootPage, BTreeKey bKey) {

		

		BTreePage targetPage(myFileManager, myStorageManager);
		int pageNo; 
		searchRecord(bKey.getValue(), rootPage, targetPage, pageNo);

		append(targetPage, bKey);

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
				BTreePage parentPage = BTreePage::deSerializePage(myFileManager, myStorageManager,  page1.getParent(), page);
				append(parentPage, danglingKey);
			} else { // special case (parent) 
				BTreePage rootPage(myFileManager, myStorageManager);				
				rootPage.addKey(danglingKey);				
				int page1NewPage = myStorageManager->GiveMeFreePageNo();
				int page1CurrentPage = page1.getMyPage();
				page1.setParent(page1CurrentPage);
				page1.rewriteToNewPage(page1NewPage);
				rootPage.rewriteToNewPage(page1CurrentPage);
			}
		} else {
			bTreePage.save();
		}
	}

	

	void  searchRecord(string target, int rootPageNo, BTreePage &bTreePage, int &pageNo) {

		string pageContent = myFileManager->readPage(rootPageNo);
		BTreePage myBTreePage = BTreePage::deSerializePage(myFileManager, myStorageManager, rootPageNo, pageContent);
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
				if (bKey.getValue().compare(target) > 0)
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