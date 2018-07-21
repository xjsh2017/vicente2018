// DlgWork.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgWork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWork dialog


CDlgWork::CDlgWork(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWork::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWork)
	m_strWork = _T("");
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
	//LoadLibrary(_T("RICHED32.DLL"));
}


void CDlgWork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWork)
	DDX_Text(pDX, IDC_RICHEDIT_WORK, m_strWork);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWork, CDialog)
	//{{AFX_MSG_MAP(CDlgWork)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWork message handlers

BOOL CDlgWork::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strFilePath = GetFilePath();
	ReadWorkFile(strFilePath);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CDlgWork::GetFilePath()
{
	//得到基址
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	//目录后带'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//组织路径
	CString strFullPath = strDownDir + g_WorkStatusFileName;
	return strFullPath;
}

void CDlgWork::ReadWorkFile(CString strFilePath)
{
	CFileFind finder; //文件搜索类
	BOOL bWorking = finder.FindFile(strFilePath);
	if(!bWorking)
		return;
	CFile mFile;
	if(mFile.Open(strFilePath,CFile::modeRead)==0)
		return;
	m_strWork = "";
	int nBuffSize = 1024;
	char pbuf[1024];
	int nFileSize = mFile.GetLength();
	int nReadSize = 0;
	while(nReadSize < nFileSize)
	{
		bzero(pbuf, 1024);
		int nBytesRead=mFile.Read(pbuf,nBuffSize);
		nReadSize += nBytesRead;
		mFile.Seek(nReadSize, 0);
		CString strTemp = pbuf;
		m_strWork += strTemp;
	}
	TRACE("the read size is %d, file size is %d\n", nReadSize, nFileSize);
	
	mFile.Close();
}
