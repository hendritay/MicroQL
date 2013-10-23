#include "stdafx.h"
#include "FileManager.h"
#include <iostream>
#include <fstream>
#include "CommonUtility.h"

string FileManager::FileHeader = "MicroQL 1.0";


bool FileManager::createAFile(string path) {
	// initialize file. 
	ofstream myfile(path);
	if (myfile.is_open()) 
		myfile.close();
	else 
	   return false;

	FileManager fm(path);
	list<char> header;
	CommonUtility::convertStringToList(FileManager::FileHeader + "Page Size : 4096 bytes", header);
	list<char> header2;
	CommonUtility::convertStringToList("11", header2);
	fm.writeAt(header, 0, 0);
	fm.writeAt(header2, 1, 0); // storage page
	fm.writeAt(CommonUtility::convertShortTo2Bytes(0), 2, 0); // dictionary page
	return true;
}

 bool FileManager::writeLog(string path, string command){
	// initialize file. 
	 try {
		ofstream myfile(path, ios::app);
		myfile << command;
		myfile << endl;
		myfile.close();
	 } catch (exception ex) {
	 }
	return true;
}

void FileManager::writeAt(char payload, int pageNo, int offset) {
	ofstream myfile(path, ios_base::in | ios_base::binary);
		if (myfile.is_open())  {
		myfile.seekp(pageNo * PageSize + offset);		
		myfile.write(&payload, 1);
	}
	myfile.close();
}

void FileManager::writeAt(list<char> &payload, int pageNo, int offset) {
	ofstream myfile(path, ios_base::in | ios_base::binary);
	char * pay = new char[payload.size()];

	list<char>::iterator iter;

	int i = 0;
	for (iter = payload.begin(); iter != payload.end(); iter++) {
		pay[i] = *iter;
		i++;
	}
	
	if (myfile.is_open())  {
		myfile.seekp(pageNo * PageSize + offset);		
		myfile.write(pay, payload.size());
	}
		myfile.close();
	delete pay;
}

char * FileManager::readPageChar(int pageNo) {

	ifstream myfile(path, ios::binary);
	myfile.seekg(pageNo * PageSize); 
	char * memblock;
	memblock = new char [PageSize];
	myfile.read(memblock, PageSize);
	myfile.close();

	return memblock;
}

string FileManager::readPage(int pageNo) {

	ifstream myfile(path, ios::binary);
	myfile.seekg(pageNo * PageSize); 
	char * memblock;
	memblock = new char [PageSize];
	myfile.read(memblock, PageSize);
	myfile.close();

	return string(memblock, PageSize);
}

bool FileManager::fileExists(string path) {

	ifstream myfile(path);
	if (myfile.is_open()){
		myfile.close();
		return true;
	}

	return false;
}
