
// SearchFileAppDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SearchFileApp.h"
#include "SearchFileAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_PATH 1024  //最长路径长度  

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


// CSearchFileAppDlg 对话框




CSearchFileAppDlg::CSearchFileAppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchFileAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSearchFileAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSearchFileAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEARCHFILE, &CSearchFileAppDlg::OnBnClickedButtonSearchfile)
	ON_BN_CLICKED(IDC_CHECK_CHINESE, &CSearchFileAppDlg::OnBnClickedCheckChinese)
END_MESSAGE_MAP()


// CSearchFileAppDlg 消息处理程序

BOOL CSearchFileAppDlg::OnInitDialog()
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
	bool bRet = GetWorkDirectory(m_szLog,sizeof(m_szLog));
	if(bRet == false){
		CString strError;
		strError.Format(TEXT("获取日志文件路径失败,ErrorCode:%d"),GetLastError());
		AfxMessageBox(strError.GetBuffer(0));
		return false;
	}

	//先删除文件
	lstrcat(m_szLog,TEXT("\\log.txt"));
	DeleteFile(m_szLog);
	//过滤中文字符
	m_bSymbolChinese = true;
	//过滤中文字符
	((CButton*)GetDlgItem(IDC_CHECK_CHINESE))->SetCheck(BST_CHECKED);
	//设置默认过滤字符串
	GetDlgItem(IDC_EDIT_SEARCH_SYMBOL)->SetWindowText(TEXT("+, ,=,&,#,/,\\,?,%,&"));
	m_Log.InitLogFile(m_szLog);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSearchFileAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSearchFileAppDlg::OnPaint()
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
HCURSOR CSearchFileAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*----------------------------  
 * 功能 : 递归遍历文件夹，找到其中包含的所有文件 
 *----------------------------  
 * 函数 : find 
 * 访问 : public   
 *  
 * 参数 : lpPath [in]      需遍历的文件夹目录 
 * 参数 : fileList [in]    以文件名称的形式存储遍历后的文件  
 */  
void CSearchFileAppDlg::find(char* lpPath,std::vector<tagFileInfo> &fileList)  
{  
   TCHAR szFind[MAX_PATH];  
   WIN32_FIND_DATA FindFileData;  
     
   strcpy(szFind,lpPath);  
   strcat(szFind,"\\*.*");  
  
   HANDLE hFind=::FindFirstFile(szFind,&FindFileData);  
   if(INVALID_HANDLE_VALUE == hFind)    return;  
  
   while(true)  
   {  
        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)  
        {  
            if(FindFileData.cFileName[0]!='.')  
            {  
                char szFile[MAX_PATH];  
                strcpy(szFile,lpPath);  
                strcat(szFile,"\\");  
                strcat(szFile,(char* )(FindFileData.cFileName));  
                find(szFile,fileList);  
            }  
        }  
        else  
        {  
			tagFileInfo FileInfo;
			ZeroMemory(&FileInfo,sizeof(tagFileInfo));
			FileInfo.strFileName = FindFileData.cFileName;
			FileInfo.strFilePath = FindFileData.cFileName;
            //std::cout << FindFileData.cFileName << std::endl;  
            fileList.push_back(FileInfo);  
        }  
        if(!FindNextFile(hFind,&FindFileData))    break;  
   }  
   FindClose(hFind);  
}  

//解析过滤字符
void CSearchFileAppDlg::ReadSymbolList(std::string strSymboBuffer,std::vector<std::string> &symboList)
{
	stringstream sstr(strSymboBuffer);  
	string token;  
	int be=0;  
	while(getline(sstr, token, ','))  
	{  
		symboList.push_back(token);  
	}
}

//查找过滤字符
bool CSearchFileAppDlg::FindSymbolFileName(std::string FileName,std::vector<std::string> symboList,std::string &strRet)
{
	if(symboList.size() == 0){
		return false;
	}
	for(int i=0; i<symboList.size(); i++){
		//空格判断
		int nIndex =FileName.find(symboList[i].c_str());
		if(nIndex != -1){
			strRet = symboList[i].c_str();
			return true;
		}
	}
	return false;
}

//中文判断
//返回0：无中文，返回1：有中文
int CSearchFileAppDlg::IncludeChinese(char *str)
{
	char c;
	while(1)
	{
		c=*str++;
		if (c==0) break;  //如果到字符串尾则说明该字符串没有中文字符
		if (c&0x80)        //如果字符高位为1且下一字符高位也是1则有中文字符
			if (*str & 0x80) return 1;
	}
	return 0;
}

//空格判断
//返回0：无空格，返回1：有空格
int CSearchFileAppDlg::IncludeSpace(char *str)
{
	char c;
	while(1)
	{
		c=*str++;
		if (c == ' ')        //如果字符高位为1且下一字符高位也是1则有中文字符
			return 1;
	}
	return 0;
}


//进程目录
bool CSearchFileAppDlg::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
{
	//模块路径
	TCHAR szModulePath[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szModulePath,sizeof(szModulePath));

	//分析文件
	for (INT i=lstrlen(szModulePath);i>=0;i--)
	{
		if (szModulePath[i]==TEXT('\\'))
		{
			szModulePath[i]=0;
			break;
		}
	}

	//设置结果
	ASSERT(szModulePath[0]!=0);
	lstrcpyn(szWorkDirectory,szModulePath,wBufferCount);
	return true;
}

void CSearchFileAppDlg::OnBnClickedButtonSearchfile()
{
	//选择路径
	TCHAR   szSetPath[MAX_PATH]={0};
	CString strGetPath;
	GetDlgItemText(IDC_EDIT_SEARCH_PATH,strGetPath);
	if(strGetPath.GetLength() == 0){
		LPITEMIDLIST   pitem;
		BROWSEINFO   info;
		::ZeroMemory(&info,sizeof(info));
		info.hwndOwner=this-> m_hWnd;
		info.lpszTitle=_T( "Choose   the   Folder: ");
		info.ulFlags=BIF_USENEWUI;
		info.pszDisplayName = szSetPath;
		if(pitem=::SHBrowseForFolder(&info))
		{
			::SHGetPathFromIDList(pitem,szSetPath);
			UpdateData(FALSE);
		}
		else
		{
			return;
		}
	}
	else{
		lstrcpyn(szSetPath,strGetPath.GetBuffer(0),sizeof(szSetPath));
	}

	//判断路径是否存在
	if( (_access(szSetPath, 0 )) == -1 )
	{
		AfxMessageBox(TEXT("目录不存在!"));
		return;
	}
	//先删除文件
	DeleteFile(m_szLog);
	//遍历目录
	lstrcat(szSetPath,"\\");

	//读取过滤字符SYMBOL
	std::vector<std::string> symbolList;

	CString strLog;
	strLog.Format(TEXT("开始遍历目录:%s"),szSetPath);
	m_Log.WriteLog(strLog.GetBuffer(0));
	m_Log.WriteLog(TEXT("----start"));

	// TODO: 在此添加控件通知处理程序代码
	std::vector<tagFileInfo> fileList;//定义一个存放结果文件名称的链表  \
	// 存储非法文件名
	std::vector<tagFileInfo> FileData;  
	//遍历一次结果的所有文件,获取文件名列表  
	find(szSetPath,fileList);//之后可对文件列表中的文件进行相应的操作  
	//非法文件
	CString strSymbolList;
	GetDlgItemText(IDC_EDIT_SEARCH_SYMBOL,strSymbolList);
	//过滤字符串
	std::vector<std::string> SymoboList;
	ReadSymbolList(strSymbolList.GetBuffer(0),SymoboList);
	if(SymoboList.size() == 0){
		AfxMessageBox(TEXT("过滤字符串为空."));
		return;
	}

	//输出文件夹下所有文件的名称  
	for(int i = 0; i < fileList.size(); i++)  
	{  
		tagFileInfo FileInfoTemp;
		FileInfoTemp = fileList[i];
		strLog.Format(TEXT("%d-%s"),i,FileInfoTemp.strFileName.c_str());
		m_Log.WriteLog(strLog);

		tagFileInfo FileInfo;
		ZeroMemory(&FileInfo,sizeof(tagFileInfo));
		bool bAdd = false;

		if(m_bSymbolChinese){
			//文件
			char szFileName[255];
			lstrcpyn(szFileName,FileInfoTemp.strFileName.c_str(),sizeof(szFileName));
			//中文判断
			int nret = IncludeChinese(szFileName);
			if(nret == 1){
				FileInfo.strFileName = FileInfoTemp.strFileName.c_str();
				FileInfo.strFilePath = FileInfoTemp.strFilePath;
				FileInfo.desc = "包含中文字符";
				bAdd = true;
			}
		}

		std::string strRet;
		bool bRet = FindSymbolFileName(FileInfoTemp.strFileName,SymoboList,strRet);
		if(bRet){
			FileInfo.strFileName = FileInfoTemp.strFileName.c_str();
			FileInfo.strFilePath = FileInfoTemp.strFilePath;
			FileInfo.desc += " 包含 " + strRet + " 字符";
			bAdd = true;
		}
		if(bAdd){
			FileData.push_back(FileInfo);
		}
	}  
	m_Log.WriteLog(TEXT("----end"));

	//输出信息
	strLog.Format(TEXT("累计扫描文件:%d个,非法文件累计:%d个\n\n\n\n"),fileList.size(),FileData.size());
	m_Log.WriteLog(strLog);
	m_Log.WriteLog(TEXT("非法文件列表========="));
	for(int i=0; i<FileData.size(); i++){
		tagFileInfo FileInfo;
		ZeroMemory(&FileInfo,sizeof(tagFileInfo));
		FileInfo = FileData[i];
		strLog.Format(TEXT("文件名:%s    描述:%s"),
			FileInfo.strFileName.c_str(),
			FileInfo.desc.c_str(),
			FileInfo.strFilePath);
		m_Log.WriteLog(strLog);
	}

	CString strOpenLog;
	strOpenLog.Format(TEXT("notepad.exe %s"),m_szLog);
	//打开文件
	WinExec(strOpenLog, SW_SHOW );
}


void CSearchFileAppDlg::OnBnClickedCheckChinese()
{
	// TODO: 在此添加控件通知处理程序代码
	int state =((CButton *)GetDlgItem(IDC_CHECK_CHINESE))->GetCheck();
	if(state == 1){
		m_bSymbolChinese = true;
	}
	else{
		m_bSymbolChinese = false;
	}
}
