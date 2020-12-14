
// postDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "postDemo.h"
#include "postDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpostDemoDlg �Ի���




CpostDemoDlg::CpostDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpostDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CpostDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CpostDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND_SMS, &CpostDemoDlg::OnBnClickedButtonSendSms)
	ON_BN_CLICKED(IDC_BUTTON_GET_KC_VIDEO_INFO, &CpostDemoDlg::OnBnClickedButtonGetKcVideoInfo)
	ON_BN_CLICKED(IDC_BUTTON_SING_KC_INFO, &CpostDemoDlg::OnBnClickedButtonSingKcInfo)
	ON_BN_CLICKED(IDC_BUTTON_READ_TXT, &CpostDemoDlg::OnBnClickedButtonReadTxt)
END_MESSAGE_MAP()


// CpostDemoDlg ��Ϣ�������

BOOL CpostDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CpostDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CpostDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CpostDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CpostDemoDlg::OnBnClickedButtonSendSms()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//������֤��-��������
	{
		CString strTempData = TEXT("data=h7NzBVJE9wQsb-pZn-s59XqQi6BDCKLHoHP6zeSEsM3th1508KyC6-jYNwdyulS2&token=e0d22110431d669167db041834f773c7&userAuth=895e60a6c8c94d6f8c66b44f984185bb_1578586766525_13302815&appCode=lite_app&deviceUUID=-7001508399625319898");
		//�������
		CString strOutData;
		//�޸�����
		bool bPosData = CWebDataClass::PosDataEx(TEXT("mobile.sunland.org.cn"),
			TEXT("/login/util/sendSMS.action"),
			strTempData,
			strTempData.GetLength(),
			TEXT(""),
			strOutData);
		CString ret = TEXT("");
		ret.Format(TEXT("bPosData=%d strOutData=%s"), bPosData, strOutData);
		MessageBox(ret);
	}
}


void CpostDemoDlg::OnBnClickedButtonGetKcVideoInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ͻ�ȡ�γ��б�-��������
	{
		CString strTempData = TEXT("");
		//�������
		CString strOutData;
		//�޸�����
		bool bPosData = CWebDataClass::PosDataEx2(TEXT("api-prod.sunlands.com"),
			TEXT("/bit16/ko/getPlayUrlByMyLearnTab"),
			strTempData,
			strTempData.GetLength(),
			TEXT(""),
			strOutData);
		CString ret = TEXT("");
		ret.Format(TEXT("bPosData=%d strOutData=%s"), bPosData, strOutData);
		MessageBox(ret);
	}
}

//һ���Զ�ȡ�ļ��е�ȫ������
std::string readfile(char * filename)
{
	std::string content_str;
	char * str_tmp;
	FILE * f;
	long length;
	//�Զ�������ʽ���ļ�
	f = fopen(filename, "rb");
	if (NULL == f)
	{
		printf("Open file fail\n");
		return NULL;
	}
	//���ļ���λ��ָ���Ƶ��ļ�β
	fseek(f, 0, SEEK_END);
	//��ȡ�ļ�����;
	length = ftell(f);
	//���ļ���λ��ָ���Ƶ��ļ���ͷ
	fseek(f, 0, SEEK_SET);
	str_tmp = (char *)malloc((length + 1) * sizeof(char));
	fread(str_tmp, 1, length, f);
	str_tmp[length] = '\0';
	fclose(f);

	if(str_tmp)
	{
		content_str = str_tmp;
		free(str_tmp);
		str_tmp = NULL;
	}
	return content_str;
}

void CpostDemoDlg::OnBnClickedButtonSingKcInfo()
{
	//�������
	CString strOutData;
	//��ȡ�ı�����
	string strTextContent = readfile("C:\\Users\\User\\Desktop\\���ݽ���\\�����ļ�\\C++�������\\2.txt");
	//�޸�����
	bool bPosData = CWebDataClass::PostDataTargetData(TEXT("api-prod.sunlands.com"),
		TEXT("/bit16/ko/getPlayUrlByMyLearnTab"),
		strTextContent,
		strOutData);
	CString ret = TEXT("");
	ret.Format(TEXT("bPosData=%d strOutData=%s"), bPosData, strOutData);
	MessageBox(ret);
}


void CpostDemoDlg::OnBnClickedButtonReadTxt()
{
	//�������
	CString strOutData;
	//��ȡ�ı�����
	string strTextContent = readfile("C:\\Users\\User\\Desktop\\���ݽ���\\���ݰ�\\��¼\\0���Ͳ�������.txt");
	MessageBoxA(NULL,strTextContent.c_str(),"",0);
}
