#include "stdafx.h"
#include "FileManager.h"
#include <iostream>
#include <fstream>

string FileManager::FileHeader = "MicroQL 1.0";


bool FileManager::createAFile(string path) {
	// initialize file. 
	 ofstream myfile(path);
	//ifstream myfile (path);
	if (myfile.is_open()) {
		myfile << FileManager::FileHeader;
		myfile << "Page Size : ";
		myfile << PageSize;
		myfile << "bytes ";
		int remaining = PageSize;
		string end =	"FileHeader End";
		myfile.seekp(remaining -end.size());

		myfile.write(end.c_str(), end.size());
	} else 
		return false;
	
	return true;
}

void FileManager::writeAt(string path, string payload, int pageNo, int offset) {
	ofstream myfile(path, ios_base::in);
	if (myfile.is_open())  {
		myfile.seekp(pageNo * PageSize + offset);		
		myfile.write(payload.c_str(), payload.size());
	}
	myfile.close();
}

string FileManager::readPage(string &path, int pageNo) {

	ifstream myfile(path);
	myfile.seekg(pageNo * PageSize); 
	char * memblock;
	memblock = new char [PageSize];
	myfile.read(memblock, PageSize);
	return string(memblock, PageSize);
}