
// postDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CpostDemoApp:
// �йش����ʵ�֣������ postDemo.cpp
//

class CpostDemoApp : public CWinApp
{
public:
	CpostDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CpostDemoApp theApp;