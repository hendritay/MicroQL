#include "stdafx.h"
#include "CommonUtility.h"
#include <sstream>

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