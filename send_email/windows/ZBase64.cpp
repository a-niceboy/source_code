#include "ZBase64.h"

string ZBase64::Encode(const unsigned char* Data, int DataByte) const
{
	//编码表
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	//返回值
	string strEncode;
	unsigned char Tmp[4] = { 0 };
	//int LineLength=0;
	for (int i = 0; i < (int)(DataByte / 3); ++i)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
	}

	//对剩余数据进行编码
	int Mod = DataByte % 3;
	if (Mod == 1)
	{
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}

	return strEncode;
}

string& ZBase64::Encode(string& sEncode, const unsigned char* Data, int DataByte) const
{
	//TraceLog log(__FUNCTION__);
	//编码表
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int nSize = 0;
	//返回值
	int Mod = DataByte % 3;
	if (Mod == 0)
	{
		nSize = DataByte / 3 * 4;
		sEncode.resize(nSize);
	}
	else
	{
		nSize = (DataByte / 3 + 1) * 4;
		sEncode.resize(nSize);
	}

	int i = 0;
	for (; i < (int)(DataByte / 3); ++i)
	{
		sEncode[i * 4] = EncodeTable[Data[0] >> 2];
		sEncode[i * 4 + 1] = EncodeTable[((Data[0] << 4) | (Data[1] >> 4)) & 0x3F];
		sEncode[i * 4 + 2] = EncodeTable[((Data[1] << 2) | (Data[2] >> 6)) & 0x3F];
		sEncode[i * 4 + 3] = EncodeTable[Data[2] & 0x3F];

		Data += 3;
	}

	//对剩余数据进行编码

	if (Mod == 1)
	{
		sEncode[i * 4] = EncodeTable[(Data[0] & 0xFC) >> 2];
		sEncode[i * 4 + 1] = EncodeTable[((Data[0] & 0x03) << 4)];
		sEncode[i * 4 + 2] = '=';
		sEncode[i * 4 + 3] = '=';
	}
	else if (Mod == 2)
	{
		sEncode[i * 4] = EncodeTable[(Data[0] & 0xFC) >> 2];
		sEncode[i * 4 + 1] = EncodeTable[((Data[0] & 0x03) << 4) | ((Data[1] & 0xF0) >> 4)];
		sEncode[i * 4 + 2] = EncodeTable[((Data[1] & 0x0F) << 2)];
		sEncode[i * 4 + 3] = '=';
	}

	return sEncode;
}

string& ZBase64::EncodeUrlSafe(string& sEncode, const unsigned char* Data, int DataByte) const
{
	//编码表
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

	int nSize = 0;
	//返回值
	int Mod = DataByte % 3;
	if (Mod == 0)
	{
		nSize = DataByte / 3 * 4;
		sEncode.resize(nSize);
	}
	else
	{
		nSize = (DataByte / 3 + 1) * 4;
		sEncode.resize(nSize);
	}

	int i = 0;
	for (; i < (int)(DataByte / 3); ++i)
	{
		sEncode[i * 4] = EncodeTable[Data[0] >> 2];
		sEncode[i * 4 + 1] = EncodeTable[((Data[0] << 4) | (Data[1] >> 4)) & 0x3F];
		sEncode[i * 4 + 2] = EncodeTable[((Data[1] << 2) | (Data[2] >> 6)) & 0x3F];
		sEncode[i * 4 + 3] = EncodeTable[Data[2] & 0x3F];

		Data += 3;
	}

	//对剩余数据进行编码

	if (Mod == 1)
	{
		sEncode[i * 4] = EncodeTable[(Data[0] & 0xFC) >> 2];
		sEncode[i * 4 + 1] = EncodeTable[((Data[0] & 0x03) << 4)];
		sEncode[i * 4 + 2] = '=';
		sEncode[i * 4 + 3] = '=';
	}
	else if (Mod == 2)
	{
		sEncode[i * 4] = EncodeTable[(Data[0] & 0xFC) >> 2];
		sEncode[i * 4 + 1] = EncodeTable[((Data[0] & 0x03) << 4) | ((Data[1] & 0xF0) >> 4)];
		sEncode[i * 4 + 2] = EncodeTable[((Data[1] & 0x0F) << 2)];
		sEncode[i * 4 + 3] = '=';
	}

	return sEncode;
}

vector<unsigned char>& ZBase64::Decode(const char* Data, int DataByte, vector<unsigned char>& outdata) const
{
	outdata.clear();

	//解码表
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};

	//返回值
	int nValue;
	int i = 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data != '\n')
		{
			nValue = DecodeTable[(int)*Data] << 18;
			++Data;

			nValue += DecodeTable[(int)*Data] << 12;
			++Data;

			outdata.push_back((nValue & 0x00FF0000) >> 16);
			if (*Data != '=')
			{
				nValue += DecodeTable[(int)*Data] << 6;
				++Data;

				outdata.push_back((nValue & 0x0000FF00) >> 8);
				if (*Data != '=')
				{
					nValue += DecodeTable[(int)*Data];
					++Data;

					outdata.push_back(nValue & 0x000000FF);
				}
			}
			i += 4;
		}
		else// 回车换行,跳过
		{
			Data++;
			i++;
		}
	}

	return outdata;
}

vector<unsigned char>& ZBase64::DecodeUrlSafe(const char* Data, int DataByte, vector<unsigned char>& outdata) const
{
	outdata.clear();

	//解码表
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 62, // '-' 
		0,
		0, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 63, // '_' 
		0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};

	//返回值
	int nValue;
	int i = 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data != '\n')
		{
			nValue = DecodeTable[(int)*Data] << 18;
			++Data;

			nValue += DecodeTable[(int)*Data] << 12;
			++Data;

			outdata.push_back((nValue & 0x00FF0000) >> 16);
			if (*Data != '=')
			{
				nValue += DecodeTable[(int)*Data] << 6;
				++Data;

				outdata.push_back((nValue & 0x0000FF00) >> 8);
				if (*Data != '=')
				{
					nValue += DecodeTable[(int)*Data];
					++Data;

					outdata.push_back(nValue & 0x000000FF);
				}
			}
			i += 4;
		}
		else// 回车换行,跳过
		{
			Data++;
			i++;
		}
	}

	return outdata;
}