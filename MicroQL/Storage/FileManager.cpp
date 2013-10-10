#include "stdafx.h"
#include "FileManager.h"
#include <iostream>
#include <fstream>
#include "CommonUtility.h"

string FileManager::FileHeader = "MicroQL 1.0";


bool FileManager::createAFile(string path) {
	// initialize file. 
	 ofstream myfile(path);
	//ifstream myfile (path);
	if (myfile.is_open()) {
		// precreate a header file
		myfile << FileManager::FileHeader;
		myfile << "Page Size : ";
		myfile << PageSize;
		myfile << "bytes ";
		int remaining = PageSize;
		string end =	"FileHeader End";
		myfile.seekp(remaining -end.size());
		myfile.write(end.c_str(), end.size());
		// precreate a storage manager page
		myfile << "11"; // storage manager page and table dicitonary page 
		myfile.seekp(PageSize *2);
		myfile << CommonUtility::convertShortTo2Bytes(0);
		
		
	} else 
		return false;
	
	return true;
}

void FileManager::writeAt(string payload, int pageNo, int offset) {
	ofstream myfile(path, ios_base::in);
	if (myfile.is_open())  {
		myfile.seekp(pageNo * PageSize + offset);		
		myfile.write(payload.c_str(), payload.size());
	}
	myfile.close();
}

string FileManager::readPage(int pageNo) {

	ifstream myfile(path);
	myfile.seekg(pageNo * PageSize); 
	char * memblock;
	memblock = new char [PageSize];
	myfile.read(memblock, PageSize);
	return string(memblock, PageSize);
}