#include "stdafx.h"
#include "StorageManager.h"
#include <iostream>

int StorageManager::GiveMeFreePageNo() {
	int currentPage = StorageManager::MotherPage;

	
	string::iterator iter;		
	while (true) {
		string page = myFileManager->readPage(currentPage);	
		// reserve the last page for Storage Manager usage. 
		for (iter = page.begin(); iter != page.end() - 1; iter++) {
			if ((*iter) != '1')  {			
				int distance = std::distance(page.begin(), iter) ;
				myFileManager->writeAt("1", StorageManager::MotherPage, distance);
				return distance + 1;
			}
		}
		currentPage += FileManager::PageSize;		
	}

}

bool StorageManager::FreePageNo(int pageNo) {

	int InternalPageNo = (pageNo / FileManager::PageSize) * FileManager::PageSize + 1;
	int offset = pageNo % FileManager::PageSize;

	// multiple of PageSize is reserved for storage manager
	// shouldn't be free by requester. 
    if (offset == 0)
		return false;
	else if (offset == 1 && InternalPageNo == 1) {
		return false; 
	}

	offset = offset - 1;
	myFileManager->writeAt("0", InternalPageNo, offset);
}