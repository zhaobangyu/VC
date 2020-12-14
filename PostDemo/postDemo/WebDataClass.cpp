#include "StdAfx.h"
#include "WebDataClass.h"
#include "StringUtil.h"

TCHAR CWebDataClass::m_szAccess_token[33] = TEXT("");
TCHAR CWebDataClass::m_szSecret[33] = TEXT("");
int CWebDataClass::m_nCurTimer = 0;
CWebDataClass::CWebDataClass(void)
{
	m_nCurTimer = 0;
	ZeroMemory(m_szAccess_token,sizeof(m_szAccess_token));
	ZeroMemory(m_szSecret,sizeof(m_szSecret));
}


CWebDataClass::~CWebDataClass(void)
{
}

//获取Coookie
bool CWebDataClass::GetCookie(LPCTSTR pszUrl,TCHAR szCookie[LEN_COOKIE])
{
	DWORD dwSize=0;
	InternetGetCookieEx(pszUrl,NULL,szCookie,&dwSize,INTERNET_COOKIE_HTTPONLY,NULL);
	if(InternetGetCookieEx(pszUrl,NULL,szCookie,&dwSize,INTERNET_COOKIE_HTTPONLY,NULL))
	{
		return true;
	}
	return false;
}


//设置Cookie
bool CWebDataClass::SetCookie(LPCTSTR pszUrl,LPCTSTR pszCookie)
{
	//拷贝数据
	CString strCookies = pszCookie;
	//查询_csrf-frontend
	int nTopPos = lstrlen(TEXT("front_sessionid="));
	int nEndPos = strCookies.Find(TEXT("front_sessionid="));

	CString strcsrf_frontend = strCookies.Mid(nTopPos,nEndPos-nTopPos);
	//查询advanced-frontend
	nTopPos = nEndPos+lstrlen(TEXT("front_sessionid="));
	nEndPos = strCookies.GetLength();
	CString strfront_sessionid = strCookies.Mid(nTopPos,nEndPos-nTopPos);

	//清理当前Cookie
	InternetSetCookieEx(pszUrl, NULL,TEXT(" _csrf-frontend=; expires=Thu, 01-Jan-1900 00:00:01 GMT"), INTERNET_COOKIE_HTTPONLY, NULL);
	InternetSetCookieEx(pszUrl, NULL,TEXT(" front_sessionid=; expires=Thu, 01-Jan-1900 00:00:01 GMT"), INTERNET_COOKIE_HTTPONLY, NULL);

	//设置Cookie
	BOOL bReturn = false;
	bReturn = InternetSetCookieEx(pszUrl,TEXT("_csrf-frontend"),strcsrf_frontend,INTERNET_COOKIE_HTTPONLY,NULL);
	bReturn = InternetSetCookieEx(pszUrl,TEXT("front_sessionid"),strfront_sessionid,INTERNET_COOKIE_HTTPONLY,NULL);
	return bReturn;
}

//获取Cookei
bool CWebDataClass::GetCookieData(TCHAR szInCookie[LEN_COOKIE],TCHAR szOutCookie[LEN_COOKIE],LPCTSTR pszCookieName)
{
	//临时变量
	CString strCookieName = szInCookie;
	//当前下标
	int  nCurDex = 0;
	int nHead = 0;
	int nEnd = 0;
	//查询数据
	nHead = strCookieName.Find(pszCookieName,nCurDex);
	if(nHead != -1)
	{
		nCurDex = nHead;
		//查询结尾
		nEnd  = strCookieName.Find(TEXT(";"),nCurDex);
		if(nEnd == -1)
		{
			nEnd = strCookieName.GetLength();
		}
		CString strDesc = strCookieName.Mid(nHead/*+sizeof("front_sessionid")*/,(nEnd-nHead));
		lstrcpyn(szOutCookie,strDesc.GetBuffer(0),LEN_COOKIE);
		return true;
	}
	return false;
}
//获取签名链接
bool CWebDataClass::GetSignatureUrl(TCHAR szSignatureUrl[LEN_MD5])
{
	int nCurTimer = time(NULL);
	if(abs(nCurTimer-m_nCurTimer) > 1)
	{
		m_nCurTimer = time(NULL); 
	}
	//签名数据
	TCHAR szSignatureData[255];
	//用户信息请求连接
	_sntprintf(szSignatureData,CountArray(szSignatureData),TEXT("REQUEST_URI=%s&content=%s&request_method=%s&timestamp=%d&secret_key=%s"),
		       TEXT("/jsAPI/user/me"),"",RESPONSE_LIVE_METHOD_GET,m_nCurTimer,m_szSecret);
	//生成MD5值
	bool bMd5 = CWHEncrypt::MD5Encrypt(szSignatureUrl,szSignatureUrl);
	return bMd5;
}

//请求头链接
bool CWebDataClass::GetResponseHeadUrl(TCHAR szHeadUrl[],int nSize)
{
	int nCurTimer = time(NULL);
	if(abs(nCurTimer-m_nCurTimer) > 1)
	{
		m_nCurTimer = time(NULL); 
	}
	//签名数据
	TCHAR szSignatureMd5[LEN_MD5];
	//获取签名
	if(GetSignatureUrl(szSignatureMd5))
	{
		//转换数据
		_sntprintf(szHeadUrl,nSize,TEXT("X-AUTHORIZATION: access_token=%s;timestamp=%d;signature=%s"),m_szAccess_token,m_nCurTimer,szSignatureMd5);
		return true;
	}
	return false;
}


//参数说明
//pHomeUrl 主网页
//pPageUrl 子网页
//sRes     输出结果
BOOL CWebDataClass::HttpRequest(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszHead,LPCTSTR pszSendData,LPCWSTR RequestType,CString &strOutData)
{
	if(!pHomeUrl || !pPageUrl)
	{
		strOutData.Format(TEXT("Parameter error"));
		return FALSE;
	}

	LONG nPort = LOGON_LIVE_PORT;
	HINTERNET hInternet; 
	DWORD nGetSize; 
	LPSTR lpszData = NULL;  // buffer for the data 
	DWORD dwSize = 0;       // size of the data available 
	DWORD dwDownloaded = 0; // size of the downloaded data

	hInternet = InternetOpen(_T("Mozilla/4.0 (compatible; Indy Library)"),INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0); 
	if( NULL == hInternet )
	{
		strOutData.Format(TEXT("Open link error. ErrCode=[%u]"),GetLastError());
		InternetCloseHandle(hInternet); 
		return FALSE; 
	} 

	// 打开http session 
	HINTERNET hSession = InternetConnect(hInternet, pHomeUrl,(INTERNET_PORT)nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

	//请求头
	CString header = pszHead;
	CString strSendData = pszSendData;
	LPSTR pResponse = new char[640*1024];
	memset(pResponse, 0, 640*1024);

	HINTERNET hRequest = HttpOpenRequest(hSession,RequestType, pPageUrl,
		_T("HTTP/1.1"), _T(""), 0,INTERNET_FLAG_NO_AUTH|
		INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_NO_CACHE_WRITE, 0);

	int timeout = 10000;
	InternetSetOption(hRequest, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout,
		sizeof(timeout));
	InternetSetOption(hRequest, INTERNET_OPTION_SEND_TIMEOUT, &timeout,
		sizeof(timeout));
	InternetSetOption(hRequest, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout,
		sizeof(timeout));
	InternetSetOption(hRequest, INTERNET_OPTION_DATA_SEND_TIMEOUT, &timeout,
		sizeof(timeout));
	InternetSetOption(hRequest, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, &timeout,
		sizeof(timeout));
	InternetSetOption(hRequest, INTERNET_OPTION_LISTEN_TIMEOUT, &timeout,
		sizeof(timeout));

	BOOL bResult = HttpSendRequest(hRequest, header.GetBuffer(), header.GetLength(), strSendData.GetBuffer(0), strSendData.GetLength());
	header.ReleaseBuffer();

	if( FALSE == bResult )
	{
		strOutData.Format(TEXT("Send request error. ErrCode=[%u]"),GetLastError());
		InternetCloseHandle(hRequest); 
		InternetCloseHandle(hSession);
		InternetCloseHandle(hInternet);
		delete []pResponse;
		return FALSE;
	}

	nGetSize=0;
	// 循环读取数据  
	while(1) 
	{
		// 检查在http response 还有多少字节可以读取
		if (!InternetQueryDataAvailable(hRequest,&dwSize,0,0)) 
		{ 
			break;
		} 
		else
		{
			// 读取数据
			if(InternetReadFile(hRequest, (LPVOID)&pResponse[nGetSize],
				dwSize, &dwDownloaded)) 
			{
				nGetSize += dwSize;
				if (dwDownloaded == 0 || nGetSize > 600 * 1024) 
				{
					// 没有剩余数据
					break; 
				}
			}
		}
	}

	pResponse[nGetSize] = 0;

#ifdef _UNICODE
	LPTSTR ptsRes = new TCHAR[2*nGetSize+2];
	memset(ptsRes, 0, 2*nGetSize+2);
	MultiByteToWideChar(CP_ACP, 0, pResponse, -1, ptsRes, 2*nGetSize);
	strOutData.Format(TEXT("%s"),ptsRes);
#else
	sRes.Format(_T("%s"), pResponse);
	strOutData.Format(TEXT("%s"),ptsRes);
#endif

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hSession);
	InternetCloseHandle(hInternet);

#ifdef _UNICODE
	strOutData.Format(TEXT("%s"),ptsRes);
	delete []ptsRes;
#else
	strOutData.Format(TEXT("%s"),ptsRes);
#endif
	delete []pResponse;


	CString strOutTemp = strOutData;
	if(strOutTemp.Find(_T("<html>"))!=-1)
	{
		strOutData.Format(TEXT("An unknown error occurred"));
		return FALSE;
	}
	return TRUE;
}



//参数说明
//pHomeUrl 主网页
//pPageUrl 子网页
//sRes     输出结果
BOOL CWebDataClass::HttpRequestEx(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,std::string strHead,std::string strSendData,LPCWSTR RequestType,CString &strOutData)
{
	if(!pHomeUrl || !pPageUrl)
	{
		strOutData.Format(TEXT("Parameter error"));
		return FALSE;
	}

	LONG nPort = LOGON_LIVE_PORT;
	HINTERNET hInternet; 
	DWORD nGetSize; 
	LPSTR lpszData = NULL;  // buffer for the data 
	DWORD dwSize = 0;       // size of the data available 
	DWORD dwDownloaded = 0; // size of the downloaded data

	hInternet = InternetOpen(_T("Mozilla/4.0 (compatible; Indy Library)"),INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0); 
	if( NULL == hInternet )
	{
		strOutData.Format(TEXT("Open link error. ErrCode=[%u]"),GetLastError());
		InternetCloseHandle(hInternet); 
		return FALSE; 
	} 

	// 打开http session 

	HINTERNET hSession = InternetConnect(hInternet, pHomeUrl,(INTERNET_PORT)nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

	//请求头
	LPSTR pResponse = new char[640*1024];
	memset(pResponse, 0, 640*1024);

	HINTERNET hRequest = HttpOpenRequest(hSession,RequestType, pPageUrl,
		_T("HTTP/1.1"), _T(""), 0,INTERNET_FLAG_NO_AUTH|
		INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_NO_CACHE_WRITE, 0);

	int timeout = 10000;
	InternetSetOption(hRequest, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout,
		sizeof(timeout));
	InternetSetOption(hRequest, INTERNET_OPTION_SEND_TIMEOUT, &timeout,
		sizeof(timeout));
	InternetSetOption(hRequest, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout,
		sizeof(timeout));
	InternetSetOption(hRequest, INTERNET_OPTION_DATA_SEND_TIMEOUT, &timeout,
		sizeof(timeout));
	InternetSetOption(hRequest, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, &timeout,
		sizeof(timeout));
	InternetSetOption(hRequest, INTERNET_OPTION_LISTEN_TIMEOUT, &timeout,
		sizeof(timeout));

	BOOL bResult = HttpSendRequestA(hRequest,strHead.c_str(),strHead.length(),(LPVOID)strSendData.c_str(),strSendData.length()); 

	if( FALSE == bResult )
	{
		strOutData.Format(TEXT("Send request error. ErrCode=[%u]"),GetLastError());
		InternetCloseHandle(hRequest); 
		InternetCloseHandle(hSession);
		InternetCloseHandle(hInternet);
		delete []pResponse;
		return FALSE;
	}

	nGetSize=0;
	// 循环读取数据  
	while(1) 
	{
		// 检查在http response 还有多少字节可以读取
		if (!InternetQueryDataAvailable(hRequest,&dwSize,0,0)) 
		{ 
			break;
		} 
		else
		{
			// 读取数据
			if(InternetReadFile(hRequest, (LPVOID)&pResponse[nGetSize],
				dwSize, &dwDownloaded)) 
			{
				nGetSize += dwSize;
				if (dwDownloaded == 0 || nGetSize > 600 * 1024) 
				{// 没有剩余数据
					break; 
				}
			}

			break;
		}
	}

	pResponse[nGetSize] = 0;

#ifdef _UNICODE
	LPTSTR ptsRes = new TCHAR[2*nGetSize+2];
	memset(ptsRes, 0, 2*nGetSize+2);
	MultiByteToWideChar(CP_ACP, 0, pResponse, -1, ptsRes, 2*nGetSize);
	strOutData.Format(TEXT("%s"),ptsRes);
#else
	sRes.Format(_T("%s"), pResponse);
	strOutData.Format(TEXT("%s"),ptsRes);
#endif

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hSession);
	InternetCloseHandle(hInternet);

#ifdef _UNICODE
	strOutData.Format(TEXT("%s"),ptsRes);
	delete []ptsRes;
#else
	strOutData.Format(TEXT("%s"),ptsRes);
#endif
	delete []pResponse;


	CString strOutTemp = strOutData;
	if(strOutTemp.Find(_T("<html>"))!=-1)
	{
		strOutData.Format(TEXT("An unknown error occurred"));
		return FALSE;
	}
	return TRUE;
}

//发送数据
BOOL CWebDataClass::PosData(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,CString &strOutData)
{
	TCHAR szHeadData[MAX_PATH];
	if(GetResponseHeadUrl(szHeadData,sizeof(szHeadData)))
	{
		BOOL bReques = HttpRequest(pHomeUrl,pPageUrl,szHeadData,pszSendData,RESPONSE_LIVE_METHOD_POS,strOutData);
		return bReques;
	}
	return false;
}

//发送数据
BOOL CWebDataClass::PosDataEx(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,int nLength,LPCTSTR pszCookie,CString &strOutData)
{
	SOCKET s;  
	WSADATA wsa;  
	struct sockaddr_in serv;  
	WSAStartup(MAKEWORD(2, 1), &wsa);
	if ((s = socket(AF_INET, SOCK_STREAM, 0))<0)  
	{  
		return FALSE;
	}

	memset(&serv,0,sizeof(serv));  
	serv.sin_family = AF_INET;  
	serv.sin_port = htons(LOGON_LIVE_PORT);
	//首页
	std::string strHomeUrl;
	//子页面
	std::string strPageUrl;
	//发送内容
	std::string strSendData;
#if UNICODE
	strHomeUrl = StringUtil::WstringToString(pHomeUrl);
	strPageUrl = StringUtil::WstringToString(pPageUrl);
	strSendData = StringUtil::WstringToString(pszSendData);
#else
	strHomeUrl = pHomeUrl;
	strPageUrl = pPageUrl;
	strSendData = pszSendData;
#endif
	hostent *phst=gethostbyname(strHomeUrl.c_str());
	in_addr * iddr=(in_addr*)phst->h_addr;
	unsigned long IPUL=iddr->s_addr;
	serv.sin_addr.S_un.S_addr = IPUL;  
	if ((connect(s, (struct sockaddr *)&serv, sizeof(serv)))<0)  
	{  
		return FALSE; 
	}

	//数据长度
	int nSendDataLength = 0;
#if UNICODE
	nSendDataLength = strSendData.length();
#else
	nSendDataLength = CString(pszSendData).GetLength();
#endif
	//临时数据内存
	TCHAR szBuffer[LEN_COOKIE];
	std::string strSendBuf;
	char rcvBuf[4096];
	memset(rcvBuf, 0, sizeof(rcvBuf));

	std::string strTemp = "";  
	strSendBuf = "POST ";

#if UNICODE
	strTemp = StringUtil::WstringToString(pPageUrl);
	strSendBuf = strSendBuf+strTemp;
#else
	strTemp = pPageUrl;
	strSendBuf = strSendBuf+strTemp;
#endif
	strSendBuf = strSendBuf+" HTTP/1.1\r\n";

	strSendBuf = strSendBuf+"X-Requested-With: XMLHttpRequest\r\n";
	strSendBuf = strSendBuf+"Accept-Language: zh-CN,zh\r\n";
#if UNICODE
	//拼接数据
	_sntprintf_s(szBuffer,sizeof(szBuffer),TEXT("Referer: %s%s"),pHomeUrl,pPageUrl);
	//数据转换
	strTemp = StringUtil::WstringToString(szBuffer); 
	strSendBuf = strSendBuf+strTemp;
#else
	//拼接数据
	_sntprintf_s(szBuffer,sizeof(szBuffer),TEXT("Referer: %s%s"),pHomeUrl,pPageUrl)
	strSendBuf = strSendBuf+szBuffer;
#endif
	strSendBuf = strSendBuf+"\r\n";
	strSendBuf = strSendBuf+"Accept: application/json, text/javascript, */*\r\n"; 
	strSendBuf = strSendBuf+"Content-Type: application/x-www-form-urlencoded\r\n";
	//strSendBuf = strSendBuf+"Accept-Encoding: gzip, deflate\r\n"; 
	strSendBuf = strSendBuf+"User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36 OPR/43.0.2442.1144\r\n";
#if UNICODE
	//拼接数据
	_sntprintf_s(szBuffer,sizeof(szBuffer),TEXT("Host: %s\r\n"),pHomeUrl);
	//数据转换
	strTemp = StringUtil::WstringToString(szBuffer);
	strSendBuf = strSendBuf+strTemp;
	//拼接数据
	_sntprintf_s(szBuffer,sizeof(szBuffer),TEXT("Content-Length: %d\r\n"),nSendDataLength);
	//数据转换
	strTemp = StringUtil::WstringToString(szBuffer);
	strSendBuf = strSendBuf+strTemp;
#else
	//拼接数据
	_sntprintf_s(szBuffer,sizeof(szBuffer),TEXT("Host: %s\r\n"),pHomeUrl);
	strSendBuf = strSendBuf+szBuffer;
	_sntprintf_s(szBuffer,sizeof(szBuffer),TEXT("Content-Length: %d\r\n"),nSendDataLength);
	strSendBuf = strSendBuf+szBuffer;
#endif
	strSendBuf = strSendBuf+"Connection: Keep-Alive\r\n";
	strSendBuf = strSendBuf+"Cache-Control: no-store, no-cache, must-revalidate\r\n";
#if UNICODE
	 _sntprintf_s(szBuffer,sizeof(szBuffer),TEXT("Cookie: %s\r\n"),pszCookie);
	 //数据转换
	 strTemp = StringUtil::WstringToString(szBuffer);
	 strSendBuf = strSendBuf+strTemp;
#else
	 _sntprintf_s(szBuffer,sizeof(szBuffer),TEXT("Cookie: %s\r\n"),pszCookie); 
	 strSendBuf = strSendBuf+szBuffer;
#endif
	strSendBuf = strSendBuf+"\r\n";
#if UNICODE
	_sntprintf_s(szBuffer,sizeof(szBuffer),TEXT("%s"),pszSendData);
	//数据转换
	strSendBuf = strSendBuf+strSendData;
#else 
	strSendBuf = strSendBuf+pszSendData;
#endif
   
	//std::string strUTF8 = StringUtil::GBKToUTF8(strSendBuf);
	//发送数据
	int nSendResult = send(s,strSendBuf.c_str(),strSendBuf.length(), 0);
	if (nSendResult < 0)  
	{  
		closesocket(s);
		WSACleanup();  
		return FALSE;
	}  

	//接收返回值
	int RecvResult = recv(s, rcvBuf, sizeof(rcvBuf)-1, 0);//如果rcvBuf的存储空间减小，则分几次copy  
	if (RecvResult < 0)
	{
		closesocket(s);
		WSACleanup();
	}
	rcvBuf[RecvResult + 1] = 0;

#if UNICODE
	std::wstring strRecvBuf = StringUtil::StringToWstring(rcvBuf);
	strOutData = strRecvBuf.c_str();
#else
	strOutData = rcvBuf;
#endif

	//读取json数据
	int nFindPos = strOutData.Find(TEXT("{\"code\""));
	if (nFindPos != -1)
	{
		CString strData = strOutData.Mid(nFindPos, strOutData.GetLength());
		strOutData = strData;
	}
	closesocket(s);
	WSACleanup();
	return true;
}

//发送数据
BOOL CWebDataClass::PosDataEx2(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,int nLength,LPCTSTR pszCookie,CString &strOutData)
{
	SOCKET s;  
	WSADATA wsa;  
	struct sockaddr_in serv;  
	WSAStartup(MAKEWORD(2, 1), &wsa);
	if ((s = socket(AF_INET, SOCK_STREAM, 0))<0)  
	{  
		return FALSE;
	}

	memset(&serv,0,sizeof(serv));  
	serv.sin_family = AF_INET;  
	serv.sin_port = htons(LOGON_LIVE_PORT);
	//首页
	std::string strHomeUrl;
	//子页面
	std::string strPageUrl;
	//发送内容
	std::string strSendData;
#if UNICODE
	strHomeUrl = StringUtil::WstringToString(pHomeUrl);
	strPageUrl = StringUtil::WstringToString(pPageUrl);
	strSendData = StringUtil::WstringToString(pszSendData);
#else
	strHomeUrl = pHomeUrl;
	strPageUrl = pPageUrl;
	strSendData = pszSendData;
#endif
	hostent *phst=gethostbyname(strHomeUrl.c_str());
	in_addr * iddr=(in_addr*)phst->h_addr;
	unsigned long IPUL=iddr->s_addr;
	serv.sin_addr.S_un.S_addr = IPUL;  
	if ((connect(s, (struct sockaddr *)&serv, sizeof(serv)))<0)  
	{  
		return FALSE; 
	}

	//数据长度
	int nSendDataLength = 0;
#if UNICODE
	nSendDataLength = strSendData.length();
#else
	nSendDataLength = CString(pszSendData).GetLength();
#endif
	//临时数据内存
	TCHAR szBuffer[LEN_COOKIE];
	std::string strSendBuf;
	char rcvBuf[4096];
	memset(rcvBuf, 0, sizeof(rcvBuf));

	std::string strTemp = "";  
	strSendBuf = "POST /bit16/ko/getPlayUrlByMyLearnTab";
	strSendBuf = strSendBuf+" HTTP/1.1\r\n";

	strSendBuf = strSendBuf+"gateway: 1\r\n";
	strSendBuf = strSendBuf+"Host: api-prod.sunlands.com\r\n";
	strSendBuf = strSendBuf+"Connection: Keep-Alive\r\n";
	strSendBuf = strSendBuf+"Accept-Encoding: gzip\r\n";
	strSendBuf = strSendBuf+"User-Agent: okhttp/3.12.1\r\n";
	strSendBuf = strSendBuf+"Content-Type: multipart/form-data; ";
	strSendBuf = strSendBuf+"boundary=1d8d6de8-cde0-497f-9cc8-0dde80193c00\r\n";
	strSendBuf = strSendBuf+"Content-Length: 519\r\n";

	strSendBuf = strSendBuf+"digest: SHA-256=J7t6yMPr/8W9ErC8Z5v7K89PFXKupWg/X7CBPV99Spo=\r\n";
	strSendBuf = strSendBuf+"x-date: Thu, 09 Jan 2020 19:42:13 GMT\r\n";
	strSendBuf = strSendBuf+"proxy-authorization: hmac username=\"sunlands_liteapp_android\", ";
	strSendBuf = strSendBuf+"algorithm=\"hmac-sha256\", headers=\"digest x-date\", ";
	strSendBuf = strSendBuf+"signature=\"BwhULXxHnKUgZXB/UVzHw+lGkoyWvlOBY3It5Xq/OqE=\"\r\n\r\n";

	strSendBuf = strSendBuf+"--1d8d6de8-cde0-497f-9cc8-0dde80193c00\r\n";
	strSendBuf = strSendBuf+"Content-Disposition: form-data; name=\"data\"\r\n";
	strSendBuf = strSendBuf+"Content-Length: 107\r\n\r\n";

	strSendBuf = strSendBuf+"VhOVRanKClkPcVTmABZDxwb9pbwDXYNIPSB9K4_LTou8vM_5_qttYP-a_f97dJzWQpuvSlBpAKhvLztM07sjr1mWm_x8B1F9X30XqC-o4Og\r\n";
	strSendBuf = strSendBuf+"--1d8d6de8-cde0-497f-9cc8-0dde80193c00\r\n";
	strSendBuf = strSendBuf+"Content-Disposition: form-data; name=\"token\"\r\n";
	strSendBuf = strSendBuf+"Content-Length: 32\r\n\r\n";

	strSendBuf = strSendBuf+"1399d83eb53d48132528bf1c93a27de1\r\n";
	strSendBuf = strSendBuf+"--1d8d6de8-cde0-497f-9cc8-0dde80193c00\r\n";
	strSendBuf = strSendBuf+"Content-Disposition: form-data; name=\"version\"\r\n";
	strSendBuf = strSendBuf+"Content-Length: 7\r\n\r\n";

	strSendBuf = strSendBuf+"4.3.2.1\r\n";
	strSendBuf = strSendBuf+"--1d8d6de8-cde0-497f-9cc8-0dde80193c00--\r\n";
   
	//std::string strUTF8 = StringUtil::GBKToUTF8(strSendBuf);
	//发送数据
	int nSendResult = send(s,strSendBuf.c_str(),strSendBuf.length(), 0);
	if (nSendResult < 0)  
	{  
		closesocket(s);
		WSACleanup();  
		return FALSE;
	}  

	//接收返回值
	int RecvResult = recv(s, rcvBuf, sizeof(rcvBuf)-1, 0);//如果rcvBuf的存储空间减小，则分几次copy  
	if (RecvResult < 0)
	{
		closesocket(s);
		WSACleanup();
	}
	rcvBuf[RecvResult + 1] = 0;

#if UNICODE
	std::wstring strRecvBuf = StringUtil::StringToWstring(rcvBuf);
	strOutData = strRecvBuf.c_str();
#else
	strOutData = rcvBuf;
#endif

	//读取json数据
	int nFindPos = strOutData.Find(TEXT("{\"code\""));
	if (nFindPos != -1)
	{
		CString strData = strOutData.Mid(nFindPos, strOutData.GetLength());
		strOutData = strData;
	}
	closesocket(s);
	WSACleanup();
	return true;
}

//发送数据
BOOL CWebDataClass::PostDataTargetData(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,string strSendData,CString &strOutData)
{
	SOCKET s;  
	WSADATA wsa;  
	struct sockaddr_in serv;  
	WSAStartup(MAKEWORD(2, 1), &wsa);
	if ((s = socket(AF_INET, SOCK_STREAM, 0))<0)  
	{  
		return FALSE;
	}

	memset(&serv,0,sizeof(serv));  
	serv.sin_family = AF_INET;  
	serv.sin_port = htons(LOGON_LIVE_PORT);
	//首页
	std::string strHomeUrl;
	//子页面
	std::string strPageUrl;
#if UNICODE
	strHomeUrl = StringUtil::WstringToString(pHomeUrl);
	strPageUrl = StringUtil::WstringToString(pPageUrl);
#else
	strHomeUrl = pHomeUrl;
	strPageUrl = pPageUrl;
#endif
	hostent *phst=gethostbyname(strHomeUrl.c_str());
	in_addr * iddr=(in_addr*)phst->h_addr;
	unsigned long IPUL=iddr->s_addr;
	serv.sin_addr.S_un.S_addr = IPUL;  
	if ((connect(s, (struct sockaddr *)&serv, sizeof(serv)))<0)  
	{  
		return FALSE; 
	}

	//临时数据内存
	TCHAR szBuffer[LEN_COOKIE];
	char rcvBuf[4096];
	memset(rcvBuf, 0, sizeof(rcvBuf));
	//发送数据
	int nSendResult = send(s,strSendData.c_str(),strSendData.length(), 0);
	if (nSendResult < 0)  
	{  
		closesocket(s);
		WSACleanup();  
		return FALSE;
	}  

	//接收返回值
	int RecvResult = recv(s, rcvBuf, sizeof(rcvBuf)-1, 0);//如果rcvBuf的存储空间减小，则分几次copy  
	if (RecvResult < 0)
	{
		closesocket(s);
		WSACleanup();
	}
	rcvBuf[RecvResult + 1] = 0;

#if UNICODE
	std::wstring strRecvBuf = StringUtil::StringToWstring(rcvBuf);
	strOutData = strRecvBuf.c_str();
#else
	strOutData = rcvBuf;
#endif

	//读取json数据
	int nFindPos = strOutData.Find(TEXT("{\"code\""));
	if (nFindPos != -1)
	{
		CString strData = strOutData.Mid(nFindPos, strOutData.GetLength());
		strOutData = strData;
	}
	closesocket(s);
	WSACleanup();
	return true;
}

//发送数据
BOOL CWebDataClass::GetData(LPCTSTR pHomeUrl, LPCTSTR pPageUrl,LPCTSTR pszSendData,CString &strOutData)
{
	TCHAR szHeadData[MAX_PATH];
	if(GetResponseHeadUrl(szHeadData,sizeof(szHeadData)))
	{
		BOOL bReques = HttpRequest(pHomeUrl,pPageUrl,szHeadData,pszSendData,RESPONSE_LIVE_METHOD_GET,strOutData);
		return bReques;
	}
	return false;
}

//设置参数
void CWebDataClass::SetParameter(TCHAR szAccess_token[33],TCHAR szSecret[33])
{
	lstrcpyn(m_szAccess_token,szAccess_token,sizeof(m_szAccess_token));
	lstrcpyn(m_szSecret,szSecret,sizeof(m_szSecret));
}

//当前时间
void CWebDataClass::SetCurTimer(int nCurTimer)
{
	m_nCurTimer = nCurTimer;
}

//发送字符处理
CString CWebDataClass::GetSendString(CString strItem,CString strSendData)
{
	CString strDataTemp;
	std::string strData = StringUtil::ToString(strSendData.GetBuffer(0));
	std::string strGBK = StringUtil::GBKToUTF8(strData);
	strData = StringUtil::UrlEncode(strGBK);
	std::string strTemp = strItem + "=";
	strData = strTemp+strData;
	strData = strData+"&";
#if UNICODE
	strDataTemp = StringUtil::StringToWstring(strData).c_str();
#else
	strDataTemp.Format(TEXT("%s=%s&"),strItem,strSendData)
#endif
	return strDataTemp;
}