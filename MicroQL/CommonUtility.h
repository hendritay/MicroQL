#pragma once
#include <string>
#include <list>
#include "MainType.h"

using namespace std;
class CommonUtility {
public:
	static void appendStringToList(ListChar &result, string from);

	static void appendIntToList(ListChar &listOne, unsigned number);
	static void convertStringToList(string variable, ListChar& resultList);
	static list<char> convertShortTo2Bytes(unsigned short number);
	static string convertShortToString(unsigned short number);

	static int convert2BytesToInt(string value);

	
	static string NumberToString ( int Number );
	static void appendList(list<char> &listOne, list<char> &listTwo);
	static std::string &ltrim(std::string &s) ;

// trim from end
static std::string &rtrim(std::string &s) ;

// trim from both ends
static std::string &trim(std::string &s) ;
};