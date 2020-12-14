#pragma once

//文件最大4M  
static const int g_nMaxSize = 1024*1024*4;  

//class CLock  
//{  
//public:  
//	CLock(void)     { ::InitializeCriticalSection(&m_cs);   }  
//	~CLock(void)    { ::DeleteCriticalSection(&m_cs);       }  
//	void Lock()     { ::EnterCriticalSection(&m_cs);        }  
//	void UnLock()   { ::LeaveCriticalSection(&m_cs);        }  
//private:  
//	CRITICAL_SECTION    m_cs;  
//};  
//
//class CScopeLock  
//{  
//public:  
//	CScopeLock(CLock& lock)  
//		:m_lock(lock)  
//	{  
//		m_lock.Lock();  
//	}  
//	~CScopeLock()  
//	{  
//		m_lock.UnLock();  
//	}  
//private:  
//	CLock& m_lock;  
//};  

class CLog  
{  
public:  
	static CLog*    Instance()  
	{  
		static CLog log;  
		return &log;  
	}  
	bool    WriteLog(const char* lpLog);  
	bool    WriteLog(const wchar_t* lpLog);  
	bool    WriteLog(const char* lpFile, long lLine, const char* lpLog);  
	bool    WriteLog(const char* lpFile, long lLine, const wchar_t* lpLog);  
	bool    WriteJoinLog(const wchar_t* lpText, const wchar_t* lpLog);  
	bool    WriteJoinLog(const char* lpFile, long lLine, const wchar_t* lpText, const wchar_t* lpLog);  

public:  
	CLog();  
	~CLog();  
	bool    InitLogFile(CString strLogFileName,bool bWriteLog=true);  
	char*   WcharToChar(const wchar_t* lpSource);  

private:  
	TCHAR m_szLog[MAX_PATH];  
	int     m_nWriteSize; 
	bool    m_bWriteLog; 
	//CLock   m_lock;  
};  