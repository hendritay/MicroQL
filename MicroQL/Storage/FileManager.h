#pragma once
#include <string>
using namespace std;

class FileManager {
public:
	static bool createAFile(string path);
	static void writeAt(string path, string payload, int pageNo, int offset);
	static string readPage(string &path, int pageNo);
private:
	static const int PageSize = 4 * 1024; // 4KB
	static string FileHeader ;
};