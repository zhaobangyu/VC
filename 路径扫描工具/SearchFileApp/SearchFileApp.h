
// SearchFileApp.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSearchFileAppApp:
// �йش����ʵ�֣������ SearchFileApp.cpp
//

class CSearchFileAppApp : public CWinApp
{
public:
	CSearchFileAppApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSearchFileAppApp theApp;