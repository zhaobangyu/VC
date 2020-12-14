
// SearchFileAppDlg.h : ͷ�ļ�
//

#pragma once
#include <stdio.h>  
#include<iostream>  
#include<vector>  
#include <Windows.h>  
#include <fstream>    
#include <iterator>  
#include <string>  
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
#include <sstream>
using namespace std;  
#include "Log.h"

struct tagFileInfo{
	std::string strFileName;
	std::string strFilePath;
	std::string desc;
};

// CSearchFileAppDlg �Ի���
class CSearchFileAppDlg : public CDialogEx
{
// ����
public:
	CSearchFileAppDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SEARCHFILEAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CLog  m_Log;
	TCHAR m_szLog[255];
	bool  m_bSymbolChinese;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearchfile();

	//���ܺ���
public:
	//���ҹ����ַ�
	void find(char* lpPath,std::vector<tagFileInfo> &fileList);
	//���������ַ�
	void ReadSymbolList(std::string strSymboBuffer,std::vector<std::string> &symboList);
	//���ҹ����ַ�
	bool FindSymbolFileName(std::string FileName,std::vector<std::string> symboList,std::string &strRet);
	//��ȡ����Ŀ¼
	bool GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount);
	int IncludeSpace(char *str);
	int IncludeChinese(char *str);
	afx_msg void OnBnClickedCheckChinese();
};
