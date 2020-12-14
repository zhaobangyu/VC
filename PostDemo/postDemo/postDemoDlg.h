
// postDemoDlg.h : 头文件
//

#pragma once
#include "WebDataClass.h"
#include <stdio.h>
#include <stdlib.h>
#include "StringUtil.h"
// CpostDemoDlg 对话框
class CpostDemoDlg : public CDialogEx
{
// 构造
public:
	CpostDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_POSTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//功能函数
public:
	CString getTextContent();
public:
	afx_msg void OnBnClickedButtonSendSms();
	afx_msg void OnBnClickedButtonGetKcVideoInfo();
	afx_msg void OnBnClickedButtonSingKcInfo();


	afx_msg void OnBnClickedButtonReadTxt();
};
