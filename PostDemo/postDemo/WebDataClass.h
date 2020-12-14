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

#define LEN_COOKIE					1024								//Cookie����
#define RESPONSE_LIVE_METHOD_GET			TEXT("GET")								//����ʽ
#define RESPONSE_LIVE_METHOD_POS			TEXT("POST")							//����ʽ
#define LOGON_LIVE_PORT						80													//��¼�˿�

class  CWebDataClass
{
public:
	CWebDataClass(void);
	~CWebDataClass(void);

	static int   m_nCurTimer;
	static TCHAR m_szAccess_token[33];
	static TCHAR m_szSecret[33];

	//��ȡCookie
	static bool GetCookie(LPCTSTR pszUrl,TCHAR szCookie[LEN_COOKIE]);
	//����Cookie
	static bool SetCookie(LPCTSTR pszUrl,LPCTSTR pszCookie);
	//��ȡCookei
	static bool GetCookieData(TCHAR szInCookie[LEN_COOKIE],TCHAR szOutCookie[LEN_COOKIE],LPCTSTR pszCookieName);
	//ǩ������
	static bool GetSignatureUrl(TCHAR szSignatureUrl[LEN_MD5]);
	//����ͷ����
	static bool GetResponseHeadUrl(TCHAR szHeadUrl[],int nSize);
	//Http����
	static BOOL HttpRequest(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszHead,LPCTSTR pszSendData,LPCWSTR RequestType,CString &strOutData);
	static BOOL HttpRequestEx(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,std::string strHead,std::string strSendData,LPCWSTR RequestType,CString &strOutData);
	//��������
	static BOOL PosData(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,CString &strOutData);
	//��������
	static BOOL PosDataEx(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,int nLength,LPCTSTR pszCookie,CString &strOutData);
	//��������
	static BOOL PosDataEx2(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,int nLength,LPCTSTR pszCookie,CString &strOutData);
	//��������
	static BOOL PostDataTargetData(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,string strSendData,CString &strOutData);
	//��������
	static BOOL GetData(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,CString &strOutData);
	//���ò���
	static void SetParameter(TCHAR szAccess_token[33],TCHAR szSecret[33]);
	//��ǰʱ��
	static void SetCurTimer(int nCurTimer);
	//�����ַ�����
	static CString GetSendString(CString strItem,CString strSendData);
};

#endif
/////////////////////////////////////////////////////////////////////////////////////////
////����ʾ������
////���ò���
//CWebDataClass::SetParameter(szAccess_token,szSecret);
////��ȡ����
//CWebDataClass::GetData(LOGIN_LIVE_DOMAIN,RESPONSE_LIVE_USER_INFO,TEXT(""),szOutData,sizeof(szOutData));
////��������
////CWebDataClass::PosData(LOGIN_LIVE_DOMAIN,RESPONSE_LIVE_USER_INFO,szHeadData,TEXT(""));
///////////////////////////////////////////////////////////////////////////////////////////