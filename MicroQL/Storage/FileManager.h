#pragma once
#include <string>
#include <list>

using namespace std;

class FileManager {
public:
	static const int PageSize = 4 * 1024; // 4KB

	FileManager(string &mainPath) {
		path = mainPath;
	}
	static bool createAFile(string path);
	void writeAt(list<char> &payload, int pageNo, int offset);
	void writeAt(char payload, int pageNo, int offset);

	string readPage(int pageNo);
	char * readPageChar(int pageNo);
	
private:
	static string FileHeader ;
	string path;
};