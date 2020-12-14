#ifndef WEB_DATA_CLASS_H
#define WEB_DATA_CLASS_H

#pragma once

#include "stdafx.h"
#include <afxinet.h>  
#include <atlstr.h>  
#include <cstdio>  
#include <Wininet.h>
#include <iostream>
#include "WHEncrypt.h"
#pragma comment(lib, "Wininet.lib")
using namespace std;

#define LEN_COOKIE					1024								//Cookie长度
#define RESPONSE_LIVE_METHOD_GET			TEXT("GET")								//请求方式
#define RESPONSE_LIVE_METHOD_POS			TEXT("POST")							//请求方式
#define LOGON_LIVE_PORT						80													//登录端口

class  CWebDataClass
{
public:
	CWebDataClass(void);
	~CWebDataClass(void);

	static int   m_nCurTimer;
	static TCHAR m_szAccess_token[33];
	static TCHAR m_szSecret[33];

	//获取Cookie
	static bool GetCookie(LPCTSTR pszUrl,TCHAR szCookie[LEN_COOKIE]);
	//设置Cookie
	static bool SetCookie(LPCTSTR pszUrl,LPCTSTR pszCookie);
	//获取Cookei
	static bool GetCookieData(TCHAR szInCookie[LEN_COOKIE],TCHAR szOutCookie[LEN_COOKIE],LPCTSTR pszCookieName);
	//签名链接
	static bool GetSignatureUrl(TCHAR szSignatureUrl[LEN_MD5]);
	//请求头链接
	static bool GetResponseHeadUrl(TCHAR szHeadUrl[],int nSize);
	//Http请求
	static BOOL HttpRequest(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszHead,LPCTSTR pszSendData,LPCWSTR RequestType,CString &strOutData);
	static BOOL HttpRequestEx(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,std::string strHead,std::string strSendData,LPCWSTR RequestType,CString &strOutData);
	//发送数据
	static BOOL PosData(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,CString &strOutData);
	//发送数据
	static BOOL PosDataEx(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,int nLength,LPCTSTR pszCookie,CString &strOutData);
	//发送数据
	static BOOL PosDataEx2(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,int nLength,LPCTSTR pszCookie,CString &strOutData);
	//发送数据
	static BOOL PostDataTargetData(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,string strSendData,CString &strOutData);
	//发送数据
	static BOOL GetData(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,CString &strOutData);
	//设置参数
	static void SetParameter(TCHAR szAccess_token[33],TCHAR szSecret[33]);
	//当前时间
	static void SetCurTimer(int nCurTimer);
	//发送字符处理
	static CString GetSendString(CString strItem,CString strSendData);
};

#endif
/////////////////////////////////////////////////////////////////////////////////////////
////调用示例代码
////设置参数
//CWebDataClass::SetParameter(szAccess_token,szSecret);
////获取数据
//CWebDataClass::GetData(LOGIN_LIVE_DOMAIN,RESPONSE_LIVE_USER_INFO,TEXT(""),szOutData,sizeof(szOutData));
////发送数据
////CWebDataClass::PosData(LOGIN_LIVE_DOMAIN,RESPONSE_LIVE_USER_INFO,szHeadData,TEXT(""));
///////////////////////////////////////////////////////////////////////////////////////////