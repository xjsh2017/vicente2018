// DlgStationSvg.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgStationSvg.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStationSvg dialog


//##ModelId=49B87BA3037A
CDlgStationSvg::CDlgStationSvg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStationSvg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStationSvg)
	m_sPath = _T("");
	//}}AFX_DATA_INIT
	m_nType = 1;
	m_pStation = NULL;
	m_pBay = NULL;
}


//##ModelId=49B87BA30382
void CDlgStationSvg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStationSvg)
	DDX_Text(pDX, IDC_EDIT_STATION_SVGPATH, m_sPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStationSvg, CDialog)
	//{{AFX_MSG_MAP(CDlgStationSvg)
	ON_BN_CLICKED(IDC_BTN_STATIONSVG_ADD, OnBtnStationsvgAdd)
	ON_BN_CLICKED(IDC_BTN_STATIONSVG_DELETE, OnBtnStationsvgDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStationSvg message handlers

//##ModelId=49B87BA30385
void CDlgStationSvg::OnBtnStationsvgAdd() 
{
	// TODO: Add your control notification handler code here

	//ȡ�õ�ǰϵͳ·��
	DWORD cchCurDir = MAX_PATH; 
    LPTSTR lpszCurDir; 
	TCHAR tchBuffer[MAX_PATH];
	for(int i = 0; i < MAX_PATH; i++)
	{
		tchBuffer[i] = '\0';
	}
	lpszCurDir = tchBuffer; 
    GetCurrentDirectory(cchCurDir, lpszCurDir);


	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strSTDown = pApp ->GetDownDir();
	
	//��ѡ���ļ��Ի���,���û�ѡ���ļ�
	static char szFilter[] = "SVG Files(*.svg)|*.svg|All Files(*.*)|*.*||";
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, szFilter);
	dlgFile.m_ofn.lpstrTitle =  StringFromID(IDS_STATION_SELECT_SVG); //�Ի������
	//dlgFile.m_ofn.lpstrFilter =_T("*.svg"); //����
	dlgFile.m_ofn.lpstrInitialDir = strSTDown;
	if(dlgFile.DoModal() == IDOK)
	{
		//�õ�ȫ·��
		CString strFullPath = dlgFile.GetPathName();
		//������·����ת����Сд���ж�
		CString tempFullPath = strFullPath;
		CString tempSTDown = strSTDown;
		tempFullPath.MakeLower();
		tempSTDown.MakeLower();
		if(tempFullPath.Find(tempSTDown, 0) == -1) //�ж�ѡ����ļ��Ƿ���STDownĿ¼��
		{
			CString str;
			str.Format("%s:%s", StringFromID(IDS_ADDOSC_PATHERROR),strSTDown);
			AfxMessageBox(str);
		}
		else //��STDownĿ¼��
		{
			m_sPath = strFullPath;
		}
	}

	//���õ�ǰĿ¼
	SetCurrentDirectory(lpszCurDir);
	UpdateData(FALSE);
}

//##ModelId=49B87BA3038A
BOOL CDlgStationSvg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if(m_pStation == NULL && m_nType == 1)
		return FALSE;
	if(m_pBay == NULL && m_nType == 2)
		return FALSE;
	if(m_nType != 1 && m_nType != 2)
		return FALSE;
	
	// TODO: Add extra initialization here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	//��ʼ��·��
	if(m_nType == 1)
	{
		if(m_pStation->m_sSvgPath  != "")
			m_sPath = strDownDir + m_pStation->m_sSvgPath;
		else
			m_sPath = "";
	}
	else if(m_nType == 2)
	{
		if(m_pBay->m_sSvgPath  != "")
			m_sPath = strDownDir + m_pBay->m_sSvgPath;
		else
			m_sPath = "";
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//##ModelId=49B87BA3038C
void CDlgStationSvg::OnBtnStationsvgDelete() 
{
	// TODO: Add your control notification handler code here
	m_sPath = "";
	UpdateData(FALSE);
}

//##ModelId=49B87BA3038E
void CDlgStationSvg::OnOK() 
{
	// TODO: Add extra validation here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	CString strPath = m_sPath;
	//��Сд
	strPath.MakeLower();
	strDownDir.MakeLower();
	if(strPath != "")
	{
		//���п��ܵ�/������"\\"
		strPath.Replace("/", "\\");
		if(strPath.Find(strDownDir) > -1)
		{	
			int length = strDownDir.GetLength();
			strPath.Delete(0, length);
		}
		while(strPath.Left(1) == "\\")
		{
			//ȥ��ǰ���"\\"
			strPath = strPath.Right(strPath.GetLength() - 1);
		}
	}

	CString strOldPath = "";
	if(m_nType == 1)
		strOldPath = m_pStation->m_sSvgPath;
	else if(m_nType == 2)
		strOldPath = m_pBay->m_sSvgPath;
	while(strOldPath.Left(1) == "\\")
	{
		//ȥ��ǰ���"\\"
		strOldPath = strOldPath.Right(strOldPath.GetLength() - 1);
	}
	//��Сд
	strOldPath.MakeLower();

	if(strOldPath != strPath)
	{
		//�б仯�ű���
		if(m_nType == 1)
		{
			CString sLog;
			sLog.Format("��·��Ϊ%s", strPath);
			WriteLog(sLog);

			m_pStation->m_sSvgPath = strPath;
			//���浽���ݿ�
			m_pStation->SaveSvgPath(strPath);
		}
		else if(m_nType == 2)
		{
			m_pBay->m_sSvgPath = strPath;
			//���浽���ݿ�
			m_pBay->SaveSvgPath(strPath);
		}
	}
	
	CDialog::OnOK();
}
