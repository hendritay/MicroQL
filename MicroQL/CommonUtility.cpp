#include "stdafx.h"
#include "CommonUtility.h"
#include <sstream>
#include <locale> 
#include <iostream> 
#include <functional> 
#include <cctype>

#include <algorithm> 
string CommonUtility::convertShortToString(unsigned short number) {
	unsigned char front = number;
	unsigned char back = number >> 8;
	string result;
	result.push_back (front);
	result.push_back(back);
	return result;
}


void CommonUtility::convertStringToList(string from, list<char> &result) {

	string::iterator iter;

	for (iter = from.begin(); iter != from.end(); iter++) {
		result.push_back(*iter);
	}
}

void CommonUtility::appendStringToList(list<char> &result, string from) {

	string::iterator iter;

	for (iter = from.begin(); iter != from.end(); iter++) {
		result.push_back(*iter);
	}
}

list<char> CommonUtility::convertShortTo2Bytes(unsigned short number) 
{
	unsigned char front = number;
	unsigned char back = number >> 8;
	list<char> result;
	result.push_back (front);
	result.push_back(back);
	return result;
}

void CommonUtility::appendIntToList(list<char> &listOne, unsigned number) {
	unsigned char front = number;
	unsigned char back = number >> 8;
	listOne.push_back (front);
	listOne.push_back(back);
}

void CommonUtility::appendList(list<char> &listOne, list<char> &listTwo) {
	listOne.insert(listOne.end(), listTwo.begin(), listTwo.end());

}

	int CommonUtility::convert2BytesToInt(string value) {
		unsigned char front = value[0];
		unsigned char back = value[1];

		int result = back << 8 | front;
		return result;
	}
	 string CommonUtility::NumberToString ( int Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }

	 std::string &CommonUtility::ltrim(std::string &s) {
		 
        s.erase(s.begin(), find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
	}

// trim from end
 std::string &CommonUtility::rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
std::string &CommonUtility::trim(std::string &s) {
        return ltrim(rtrim(s));
}