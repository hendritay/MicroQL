#pragma once
#include <string>
using namespace std;

class FileManager {
public:
	static const int PageSize = 4 * 1024; // 4KB

	FileManager(string &mainPath) {
		path = mainPath;
	}
	static bool createAFile(string path);
	void writeAt(string payload, int pageNo, int offset);

	string readPage(int pageNo);
private:
	static string FileHeader ;
	string path;
};