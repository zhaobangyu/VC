#include "stdafx.h"
#include "StringUtil.h"
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include "iconv.h"
#include<string.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>


StringUtil::StringUtil(void)
{
}

StringUtil::~StringUtil(void)
{
}

//charתwchar
bool StringUtil::CharToWChar(const char *strSource, wchar_t* wstrDes, int iCount)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strSource, strlen(strSource), NULL, 0);
	if (len >= iCount)
	{
		return false;
	}
	MultiByteToWideChar(CP_ACP, 0, strSource, strlen(strSource), wstrDes, len);
	wstrDes[len] = '\0';
	return true;
}

//wcharתchar
bool StringUtil::WcharToChar(const wchar_t* wstrSource, char* strDes, int iCount)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wstrSource, wcslen(wstrSource), NULL, 0, NULL, NULL);
	if (len >= iCount)
	{
		return false;
	}
	WideCharToMultiByte(CP_ACP, 0, wstrSource, wcslen(wstrSource), strDes, len, NULL, NULL);
	strDes[len] = '\0';
	return true;
}

//stringתwstring
std::wstring StringUtil::StringToWstring(const string &strSource)
{
	const char* _Source = strSource.c_str();
	size_t _Dsize = MultiByteToWideChar(CP_ACP, 0, _Source, -1, NULL, 0);
	wchar_t *_Dest = new wchar_t[_Dsize];
	MultiByteToWideChar(CP_ACP, 0, _Source, -1, _Dest, _Dsize);
	wstring result = _Dest;
	delete[]_Dest;
	return result;
}

//wstringתstring
std::string StringUtil::WstringToString(const wstring &wstrSource)
{
	const wchar_t* _Source = wstrSource.c_str();
	size_t _Dsize = WideCharToMultiByte(CP_ACP, 0, _Source, -1, NULL, 0, NULL, NULL);
	char *_Dest = new char[_Dsize];
	WideCharToMultiByte(CP_ACP, 0, _Source, -1, _Dest, _Dsize, NULL, NULL);
	string result = _Dest;
	delete[]_Dest;
	return result;
}

//��strPattern�ַ����ָ�strPolicyids�ַ�
std::vector<std::string> StringUtil::Split(std::string strPolicyids, std::string strPattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	strPolicyids += strPattern;//��չ�ַ��Է������
	std::string::size_type iSize = strPolicyids.size();
	std::string::size_type iPos = 0;

	for (; iPos < iSize; iPos++)
	{
		pos = strPolicyids.find(strPattern, iPos);
		if (pos < iSize)
		{
			std::string strTemp = strPolicyids.substr(iPos, pos - iPos);
			result.push_back(strTemp);
			iPos = pos + strPattern.size() - 1;
		}
	}
	return result;
}

//��ȡ�����ַ�֮��ĵ�һ���ַ�
std::string StringUtil::GetMidString(std::string strSource, std::string strBegin, std::string strEnd)
{
	string content;
	int nPos1 = strSource.find(strBegin);
	if (nPos1 != -1)
	{
		int nPos2 = strSource.find(strEnd, nPos1 + strBegin.length());
		if (nPos2 != -1)
		{
			content = strSource.substr(nPos1 + strBegin.length(), nPos2 - nPos1 - strBegin.length());
		}
	}
	return content;
}


std::vector<std::string> StringUtil::GetAllMidString(std::string strSource, std::string strBegin, std::string strEnd)
{
	string content;
	std::vector<std::string> stringVector;
	int nPos1 = strSource.find(strBegin);
	while (nPos1 != -1)
	{
		int nPos2 = strSource.find(strEnd, nPos1 + strBegin.length());
		if (nPos2 != -1)
		{
			content = strSource.substr(nPos1 + strBegin.length(), nPos2 - nPos1 - strBegin.length());
			stringVector.push_back(content);
			nPos1 = strSource.find(strBegin, nPos2 + strEnd.length());
		}
	}
	return stringVector;
}

// ReplaceAllString(string("12212"),"12","21") ���  22211   
std::string& StringUtil::ReplaceAllString(std::string& strSource, const std::string& strOld, const std::string& strNew)
{
	std::string::size_type uiStart = 0;
	std::string::size_type   pos(0);
	while (true)
	{
		pos = strSource.find(strOld, uiStart);
		if (pos != std::string::npos)
		{
			strSource.replace(pos, strOld.length(), strNew);
			uiStart = pos + strNew.length();
		}
		else
		{
			break;
		}
	}
	return strSource;
}
string StringUtil::ReverseString(string strInput)
{
	string strOut;
	char *pInput = (char*)strInput.c_str();
	char *pContent = new char[strInput.size() + 1];
	memset(pContent, 0, strInput.size() + 1);
	int k = 0;
	for (int i = strInput.size() - 1; i >= 0; i--)
	{
		char c = pInput[i];
		pContent[k++] = c;
	}
	pContent[k] = '\0';
	strOut = pContent;
	delete[]pContent;
	return strOut;
}

string  StringUtil::WstringToUTF8(const wstring& wstrSource)
{
	const wchar_t* _Source = wstrSource.c_str();
	size_t _Dsize = WideCharToMultiByte(CP_UTF8, 0, _Source, -1, NULL, 0, NULL, NULL);
	char *_Dest = new char[_Dsize];
	WideCharToMultiByte(CP_UTF8, 0, _Source, -1, _Dest, _Dsize, NULL, NULL);
	string result = _Dest;
	delete[]_Dest;
	return result;
}

wstring StringUtil::UTF8ToWstring(const string& strSource)
{
	const char* _Source = strSource.c_str();
	size_t _Dsize = MultiByteToWideChar(CP_UTF8, 0, _Source, -1, NULL, 0);
	wchar_t *_Dest = new wchar_t[_Dsize];
	MultiByteToWideChar(CP_UTF8, 0, _Source, -1, _Dest, _Dsize);
	wstring result = _Dest;
	delete[]_Dest;
	return result;
}

string StringUtil::GBKToUTF8(const std::string& strGBK)
{
	string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

string StringUtil::UTF8ToGBK(const std::string& strUTF8)
{
	const char *p = strUTF8.c_str();
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)p, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

std::string StringUtil::WcharToString(const wchar_t* wstrSource, int iCount)
{
	std::string strOut;
	char *pszFilePath = new char[iCount];
	bool bwToChar = StringUtil::WcharToChar(wstrSource, pszFilePath, iCount);
	if (bwToChar)
	{
		strOut = pszFilePath;
	}
	return strOut;
}


//转string
string StringUtil::ToString(LPSTR str)
{
	string strBuffer = str;
	return strBuffer;
}

string StringUtil::ToString(LPWSTR str)
{
	std::string strBuffer = WstringToString(str);
	return strBuffer;
}

unsigned char StringUtil::ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

unsigned char StringUtil::FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z')
	{
		y = x - 'A' + 10;
	}
	else if (x >= 'a' && x <= 'z')
	{
		y = x - 'a' + 10;
	}
	else if (x >= '0' && x <= '9')
	{
		y = x - '0';
	}
	else
	{
		return 0;
	}
	return y;
}

std::string StringUtil::UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

std::string StringUtil::UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%')
		{
			//assert(i + 2 < length);  
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}


//错误描述 
LPSTR StringUtil::ConvertErrorCodeToString(DWORD ErrorCode)
{
	HLOCAL LocalAddress = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, ErrorCode, 0, (PTSTR)&LocalAddress, 0, NULL);
	return (LPSTR)LocalAddress;
}