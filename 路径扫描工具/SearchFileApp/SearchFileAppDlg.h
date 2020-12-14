
// SearchFileAppDlg.h : 头文件
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

// CSearchFileAppDlg 对话框
class CSearchFileAppDlg : public CDialogEx
{
// 构造
public:
	CSearchFileAppDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SEARCHFILEAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CLog  m_Log;
	TCHAR m_szLog[255];
	bool  m_bSymbolChinese;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearchfile();

	//功能函数
public:
	//查找过滤字符
	void find(char* lpPath,std::vector<tagFileInfo> &fileList);
	//解析过滤字符
	void ReadSymbolList(std::string strSymboBuffer,std::vector<std::string> &symboList);
	//查找过滤字符
	bool FindSymbolFileName(std::string FileName,std::vector<std::string> symboList,std::string &strRet);
	//获取工作目录
	bool GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount);
	int IncludeSpace(char *str);
	int IncludeChinese(char *str);
	afx_msg void OnBnClickedCheckChinese();
};
