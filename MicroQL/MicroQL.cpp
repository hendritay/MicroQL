// MicroQL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Storage\FileManager.h"
#include <iostream>
#include <fstream>
int _tmain(int argc, _TCHAR* argv[])
{
	string path = "c:\\study\\cs4221\\hellodb.txt";
	//FileManager::createAFile(path);
	//FileManager::writeAt(path, "Haha", 1, 0);
	string read = FileManager::readPage(path, 0);


	return 0;
}

