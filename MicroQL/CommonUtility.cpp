#include "stdafx.h"
#include "CommonUtility.h"

string CommonUtility::convertShortTo2Bytes(unsigned short number) 
{
	char front = number;
	char back = number >> 8;
	string result;
	result.push_back (front);
	result.push_back(back);
	return result;
}

	int CommonUtility::convert2BytesToInt(string value) {
		char front = value[0];
		char back = value[1];

		int result = back << 8 | front;
	}
