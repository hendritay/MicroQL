#include "FileManager.h"

class StorageManager {
public:
	StorageManager(FileManager *fm) {
		myFileManager = fm;
		
	}	
	// it will search a free page and give it to the requestor
	int GiveMeFreePageNo();
	bool FreePageNo(int pageNo);

private:
	/* The First Page always belongs to Storage manager
	noone gonna snatch away"
*/
	static const int MotherPage =1;
	FileManager *myFileManager;

};