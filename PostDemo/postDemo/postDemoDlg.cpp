
// postDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "postDemo.h"
#include "postDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CpostDemoDlg 对话框




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


// CpostDemoDlg 消息处理程序

BOOL CpostDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CpostDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CpostDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CpostDemoDlg::OnBnClickedButtonSendSms()
{
	// TODO: 在此添加控件通知处理程序代码
	//发送验证码-测试数据
	{
		CString strTempData = TEXT("data=h7NzBVJE9wQsb-pZn-s59XqQi6BDCKLHoHP6zeSEsM3th1508KyC6-jYNwdyulS2&token=e0d22110431d669167db041834f773c7&userAuth=895e60a6c8c94d6f8c66b44f984185bb_1578586766525_13302815&appCode=lite_app&deviceUUID=-7001508399625319898");
		//输出参数
		CString strOutData;
		//修改数据
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
	// TODO: 在此添加控件通知处理程序代码
	//发送获取课程列表-测试数据
	{
		CString strTempData = TEXT("");
		//输出参数
		CString strOutData;
		//修改数据
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

//一次性读取文件中的全部内容
std::string readfile(char * filename)
{
	std::string content_str;
	char * str_tmp;
	FILE * f;
	long length;
	//以二进制形式打开文件
	f = fopen(filename, "rb");
	if (NULL == f)
	{
		printf("Open file fail\n");
		return NULL;
	}
	//把文件的位置指针移到文件尾
	fseek(f, 0, SEEK_END);
	//获取文件长度;
	length = ftell(f);
	//把文件的位置指针移到文件开头
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
	//输出参数
	CString strOutData;
	//读取文本内容
	string strTextContent = readfile("C:\\Users\\User\\Desktop\\数据解密\\帮助文件\\C++程序设计\\2.txt");
	//修改数据
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
	//输出参数
	CString strOutData;
	//读取文本内容
	string strTextContent = readfile("C:\\Users\\User\\Desktop\\数据解密\\数据包\\登录\\0发送测试数据.txt");
	MessageBoxA(NULL,strTextContent.c_str(),"",0);
}
