#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
using namespace std;
int code_convert(char *from_charset, char *to_charset, char *inbuf, int inlen, char *outbuf, int outlen);
int gbk_to_utf8(char *gbk, char *utf8, size_t n);
unsigned int ngx_utf8_decode(unsigned char **p, size_t n);
int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

class StringUtil
{
public:
	StringUtil(void);
	~StringUtil(void);

	//字符操作
	static bool CharToWChar(const char *strSource, wchar_t* wstrDes, int iCount);
	static bool WcharToChar(const wchar_t* wstrSource, char* strDes, int iCount);
	static std::string WstringToString(const wstring &wstrSource);
	static std::wstring StringToWstring(const string &strSource);
	static std::vector<std::string> Split(std::string strPolicyids, std::string strPattern);
	// ReplaceAllString(string("12212"),"12","21") 22211   
	static std::string& ReplaceAllString(std::string& strSource, const std::string& strOld, const std::string& strNew);
	static std::string GetMidString(std::string strSource, std::string strBegin, std::string strEnd);
	static std::vector<std::string> GetAllMidString(std::string strSource, std::string strBegin, std::string strEnd);
	static string ReverseString(string strInput);
	static string  WstringToUTF8(const wstring& ws);
	static wstring UTF8ToWstring(const string& s);
	static string GBKToUTF8(const std::string& strGBK);
	static string UTF8ToGBK(const std::string& strUTF8);
	static std::string WcharToString(const wchar_t* wstrSource, int iCount);
	//转string
	static string ToString(LPSTR str);
	static string ToString(LPWSTR str);
	//URL编解码
	static unsigned char ToHex(unsigned char x);
	static unsigned char FromHex(unsigned char x);
	static std::string UrlEncode(const std::string& str);
	static std::string UrlDecode(const std::string& str);

	//文件操作
public:
	//错误描述 
	static LPSTR ConvertErrorCodeToString(DWORD ErrorCode);
};