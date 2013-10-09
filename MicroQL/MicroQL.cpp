// MicroQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Storage\FileManager.h"
#include "Storage\StorageManager.h"
#include <iostream>
#include <fstream>
int _tmain(int argc, _TCHAR* argv[])
{
	/*
	string path = "c:\\study\\cs4221\\hellodb.txt";
	//FileManager::createAFile(path);
	FileManager * fm = new FileManager(path);
	StorageManager sm (fm);
	sm.FreePageNo(4);
	*/
	int a = 12561;
	int b = (a >> 8);
	char c = a;
	int e = c >> 8;
	int d = b << 8 | c;

	return 0;
}

