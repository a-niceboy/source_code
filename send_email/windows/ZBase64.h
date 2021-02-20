#ifndef _ZBASE64_H_
#define _ZBASE64_H_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <stdio.h>

class ZBase64
{
public:
	string Encode(const unsigned char* Data, int DataByte) const;
	string& Encode(string& sOut, const unsigned char* Data, int DataByte) const;
	vector<unsigned char>& Decode(const char* Data, int DataByte, vector<unsigned char>& outdata) const;

	string& EncodeUrlSafe(string& sEncode, const unsigned char* Data, int DataByte) const;
	vector<unsigned char>& DecodeUrlSafe(const char* Data, int DataByte, vector<unsigned char>& outdata) const;
};

#endif