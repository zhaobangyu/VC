
// postDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "WebDataClass.h"
#include <stdio.h>
#include <stdlib.h>
#include "StringUtil.h"
// CpostDemoDlg �Ի���
class CpostDemoDlg : public CDialogEx
{
// ����
public:
	CpostDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_POSTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	//���ܺ���
public:
	CString getTextContent();
public:
	afx_msg void OnBnClickedButtonSendSms();
	afx_msg void OnBnClickedButtonGetKcVideoInfo();
	afx_msg void OnBnClickedButtonSingKcInfo();


	afx_msg void OnBnClickedButtonReadTxt();
};
