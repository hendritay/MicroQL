#pragma once
#include <string>
#include "Storage\PageManager.h"
#include "CommonUtility.h"
#include "Storage\BTreeKey.h";
#include "Storage\BTreePage.h"
#include "Storage\FileManager.h";
#include <map>
#include <vector>
using namespace std;
class BTree {
	BTree(FileManager *fm) { // 1003
		myFileManager = fm;
	}
private:
	FileManager *myFileManager;	

	// append node
	void append(int targetPage) {

	}

	void append(BTreePage bTreePage, BTreeKey bKey) {

	}

	void insertRecord(int rootPage, BTreeKey bKey) {
		// first record, special treatment

	}

	void  searchRecord(string target, int rootPageNo, BTreePage &bTreePage, int &pageNo) {

		string pageContent = myFileManager->readPage(rootPageNo);
		BTreePage myBTreePage = BTreePage::deSerializePage(myFileManager,pageContent);

		int targetIndex = myBTreePage.findKey(target);
		if (myBTreePage.findKey(target) >= 0) {
			// found 
			pageNo = rootPageNo;
			bTreePage = myBTreePage;
			return;
		} else {
			
			BTreeKey bKey = myBTreePage.findIntermediate(target);
			if (bKey.getRightChild() != 0 ) {
				searchRecord(target, bKey.getRightChild(), bTreePage, pageNo);
			} else {
				// found suitable node to be inserted to. 
				pageNo = rootPageNo;
				bTreePage = myBTreePage;
				return;
			}
		}


	}	

};