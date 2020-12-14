
// SearchFileAppDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SearchFileApp.h"
#include "SearchFileAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_PATH 1024  //�·������  

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


// CSearchFileAppDlg �Ի���




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


// CSearchFileAppDlg ��Ϣ�������

BOOL CSearchFileAppDlg::OnInitDialog()
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
	bool bRet = GetWorkDirectory(m_szLog,sizeof(m_szLog));
	if(bRet == false){
		CString strError;
		strError.Format(TEXT("��ȡ��־�ļ�·��ʧ��,ErrorCode:%d"),GetLastError());
		AfxMessageBox(strError.GetBuffer(0));
		return false;
	}

	//��ɾ���ļ�
	lstrcat(m_szLog,TEXT("\\log.txt"));
	DeleteFile(m_szLog);
	//���������ַ�
	m_bSymbolChinese = true;
	//���������ַ�
	((CButton*)GetDlgItem(IDC_CHECK_CHINESE))->SetCheck(BST_CHECKED);
	//����Ĭ�Ϲ����ַ���
	GetDlgItem(IDC_EDIT_SEARCH_SYMBOL)->SetWindowText(TEXT("+, ,=,&,#,/,\\,?,%,&"));
	m_Log.InitLogFile(m_szLog);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSearchFileAppDlg::OnPaint()
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
HCURSOR CSearchFileAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*----------------------------  
 * ���� : �ݹ�����ļ��У��ҵ����а����������ļ� 
 *----------------------------  
 * ���� : find 
 * ���� : public   
 *  
 * ���� : lpPath [in]      ��������ļ���Ŀ¼ 
 * ���� : fileList [in]    ���ļ����Ƶ���ʽ�洢��������ļ�  
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

//���������ַ�
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

//���ҹ����ַ�
bool CSearchFileAppDlg::FindSymbolFileName(std::string FileName,std::vector<std::string> symboList,std::string &strRet)
{
	if(symboList.size() == 0){
		return false;
	}
	for(int i=0; i<symboList.size(); i++){
		//�ո��ж�
		int nIndex =FileName.find(symboList[i].c_str());
		if(nIndex != -1){
			strRet = symboList[i].c_str();
			return true;
		}
	}
	return false;
}

//�����ж�
//����0�������ģ�����1��������
int CSearchFileAppDlg::IncludeChinese(char *str)
{
	char c;
	while(1)
	{
		c=*str++;
		if (c==0) break;  //������ַ���β��˵�����ַ���û�������ַ�
		if (c&0x80)        //����ַ���λΪ1����һ�ַ���λҲ��1���������ַ�
			if (*str & 0x80) return 1;
	}
	return 0;
}

//�ո��ж�
//����0���޿ո񣬷���1���пո�
int CSearchFileAppDlg::IncludeSpace(char *str)
{
	char c;
	while(1)
	{
		c=*str++;
		if (c == ' ')        //����ַ���λΪ1����һ�ַ���λҲ��1���������ַ�
			return 1;
	}
	return 0;
}


//����Ŀ¼
bool CSearchFileAppDlg::GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount)
{
	//ģ��·��
	TCHAR szModulePath[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szModulePath,sizeof(szModulePath));

	//�����ļ�
	for (INT i=lstrlen(szModulePath);i>=0;i--)
	{
		if (szModulePath[i]==TEXT('\\'))
		{
			szModulePath[i]=0;
			break;
		}
	}

	//���ý��
	ASSERT(szModulePath[0]!=0);
	lstrcpyn(szWorkDirectory,szModulePath,wBufferCount);
	return true;
}

void CSearchFileAppDlg::OnBnClickedButtonSearchfile()
{
	//ѡ��·��
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

	//�ж�·���Ƿ����
	if( (_access(szSetPath, 0 )) == -1 )
	{
		AfxMessageBox(TEXT("Ŀ¼������!"));
		return;
	}
	//��ɾ���ļ�
	DeleteFile(m_szLog);
	//����Ŀ¼
	lstrcat(szSetPath,"\\");

	//��ȡ�����ַ�SYMBOL
	std::vector<std::string> symbolList;

	CString strLog;
	strLog.Format(TEXT("��ʼ����Ŀ¼:%s"),szSetPath);
	m_Log.WriteLog(strLog.GetBuffer(0));
	m_Log.WriteLog(TEXT("----start"));

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	std::vector<tagFileInfo> fileList;//����һ����Ž���ļ����Ƶ�����  \
	// �洢�Ƿ��ļ���
	std::vector<tagFileInfo> FileData;  
	//����һ�ν���������ļ�,��ȡ�ļ����б�  
	find(szSetPath,fileList);//֮��ɶ��ļ��б��е��ļ�������Ӧ�Ĳ���  
	//�Ƿ��ļ�
	CString strSymbolList;
	GetDlgItemText(IDC_EDIT_SEARCH_SYMBOL,strSymbolList);
	//�����ַ���
	std::vector<std::string> SymoboList;
	ReadSymbolList(strSymbolList.GetBuffer(0),SymoboList);
	if(SymoboList.size() == 0){
		AfxMessageBox(TEXT("�����ַ���Ϊ��."));
		return;
	}

	//����ļ����������ļ�������  
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
			//�ļ�
			char szFileName[255];
			lstrcpyn(szFileName,FileInfoTemp.strFileName.c_str(),sizeof(szFileName));
			//�����ж�
			int nret = IncludeChinese(szFileName);
			if(nret == 1){
				FileInfo.strFileName = FileInfoTemp.strFileName.c_str();
				FileInfo.strFilePath = FileInfoTemp.strFilePath;
				FileInfo.desc = "���������ַ�";
				bAdd = true;
			}
		}

		std::string strRet;
		bool bRet = FindSymbolFileName(FileInfoTemp.strFileName,SymoboList,strRet);
		if(bRet){
			FileInfo.strFileName = FileInfoTemp.strFileName.c_str();
			FileInfo.strFilePath = FileInfoTemp.strFilePath;
			FileInfo.desc += " ���� " + strRet + " �ַ�";
			bAdd = true;
		}
		if(bAdd){
			FileData.push_back(FileInfo);
		}
	}  
	m_Log.WriteLog(TEXT("----end"));

	//�����Ϣ
	strLog.Format(TEXT("�ۼ�ɨ���ļ�:%d��,�Ƿ��ļ��ۼ�:%d��\n\n\n\n"),fileList.size(),FileData.size());
	m_Log.WriteLog(strLog);
	m_Log.WriteLog(TEXT("�Ƿ��ļ��б�========="));
	for(int i=0; i<FileData.size(); i++){
		tagFileInfo FileInfo;
		ZeroMemory(&FileInfo,sizeof(tagFileInfo));
		FileInfo = FileData[i];
		strLog.Format(TEXT("�ļ���:%s    ����:%s"),
			FileInfo.strFileName.c_str(),
			FileInfo.desc.c_str(),
			FileInfo.strFilePath);
		m_Log.WriteLog(strLog);
	}

	CString strOpenLog;
	strOpenLog.Format(TEXT("notepad.exe %s"),m_szLog);
	//���ļ�
	WinExec(strOpenLog, SW_SHOW );
}


void CSearchFileAppDlg::OnBnClickedCheckChinese()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int state =((CButton *)GetDlgItem(IDC_CHECK_CHINESE))->GetCheck();
	if(state == 1){
		m_bSymbolChinese = true;
	}
	else{
		m_bSymbolChinese = false;
	}
}
