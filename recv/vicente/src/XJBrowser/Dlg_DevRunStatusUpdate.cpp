// Dlg_DevRunStatusUpdate.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "Dlg_DevRunStatusUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_DevRunStatusUpdate dialog


CDlg_DevRunStatusUpdate::CDlg_DevRunStatusUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_DevRunStatusUpdate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_DevRunStatusUpdate)
	m_cstrDevName = _T("");
	m_cstrTime = _T("");
	//}}AFX_DATA_INIT
}


void CDlg_DevRunStatusUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_DevRunStatusUpdate)
	DDX_Control(pDX, IDC_COMBO_DevRunUpdate_Status, m_cbPTRunStatus);
	DDX_Control(pDX, IDC_BUTTON_DevRunStatus_OK, m_btOk);
	DDX_Control(pDX, IDC_BUTTON_DevRunStatus_Exit, m_btExit);
	DDX_Text(pDX, IDC_EDIT_DevName, m_cstrDevName);
	DDX_Text(pDX, IDC_EDIT_DevRunUpdate_Time, m_cstrTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_DevRunStatusUpdate, CDialog)
	//{{AFX_MSG_MAP(CDlg_DevRunStatusUpdate)
	ON_BN_CLICKED(IDC_BUTTON_DevRunStatus_OK, OnBUTTONDevRunStatusOK)
	ON_BN_CLICKED(IDC_BUTTON_DevRunStatus_Exit, OnBUTTONDevRunStatusExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_DevRunStatusUpdate message handlers

void CDlg_DevRunStatusUpdate::OnBUTTONDevRunStatusOK() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	CString strTemp;
	try
	{
		//检查通讯情况
		if(!pApp->GetSTTPStatus())
		{
			AfxMessageBox(STTP_DISCONNECT);
			return;
		}
	
        int nRunStatus = 3;
		int result;
		CString m_strRunStatus;
		m_cbPTRunStatus.GetLBText(m_cbPTRunStatus.GetCurSel(), m_strRunStatus);
		if (!m_strRunStatus.CompareNoCase(StringFromID(IDS_RUNSTATUS_OVERHAULS)))
		{
			nRunStatus = 0;
		}
		else if (!m_strRunStatus.CompareNoCase(StringFromID(IDS_RUNSTATUS_OUTAGE)))
		{
			nRunStatus = 1;
		}
		else if (!m_strRunStatus.CompareNoCase(StringFromID(IDS_RUNSTATUS_SERVICE)))
		{
			nRunStatus = 2;
		}
		else if (!m_strRunStatus.CompareNoCase(StringFromID(IDS_RUNSTATUS_NOACCESS)))
		{
			nRunStatus = 3;
		}
		else if (!m_strRunStatus.CompareNoCase(StringFromID(IDS_RUNSTATUS_TESTING)))
		{
			nRunStatus = 4;
		}
		else
		{
			nRunStatus = 3;
		}
       
		STTP_FULL_DATA sttpData;
		if (pApp->m_SttpEngine.BuildDataFor20079UpdatePtRunStatus(sttpData, m_cstrPTID, nRunStatus))
		{
			result = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			if (result)
			{
				strTemp.Format("%s:%s", StringFromID(IDS_RUNSTATUSCHANGE_NOTIFY_SUCCESS),m_cstrPTName);
				AfxMessageBox(strTemp);
				WriteLog(strTemp);
			}
			else
			{
				strTemp.Format("%s:%s", StringFromID(IDS_RUNSTATUSCHANGE_NOTIFY_FAIL),m_cstrPTName);
				AfxMessageBox(strTemp);
				WriteLog(strTemp);
			}
		}
		else
		{
			strTemp = StringFromID(IDS_STATIONCHANGE_BUILD_FAIL);
			AfxMessageBox(strTemp);
			WriteLog(strTemp);
		}
		
		m_nStatus = nRunStatus;
		SendMessage(WM_CLOSE);
		
	}
	catch (...)
	{
		AfxMessageBox(StringFromID(IDS_RUNSTATUSCHANGE_NOTIFY_FAIL));		
	}
}

void CDlg_DevRunStatusUpdate::OnBUTTONDevRunStatusExit() 
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_CLOSE);
}

BOOL CDlg_DevRunStatusUpdate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cstrDevName = m_cstrPTName;

	m_cbPTRunStatus.AddString(StringFromID(IDS_RUNSTATUS_OVERHAULS));	//0
	m_cbPTRunStatus.AddString(StringFromID(IDS_RUNSTATUS_OUTAGE));	//1
	m_cbPTRunStatus.AddString(StringFromID(IDS_RUNSTATUS_SERVICE));	//2
	m_cbPTRunStatus.AddString(StringFromID(IDS_RUNSTATUS_NOACCESS));	//3
	m_cbPTRunStatus.AddString(StringFromID(IDS_RUNSTATUS_TESTING));//4
	
	CXJBrowserApp *pApp =(CXJBrowserApp*)AfxGetApp();
    int nRunStatus = 3;
	CString strRunTime="";
    BOOL bGet = FALSE;
	
	
	if (!pApp->QueryPtRunStatus(m_cstrPTID, nRunStatus, strRunTime))
	{
		AfxMessageBox(StringFromID(IDS_QUERY_RUNSTATUS_FAIL));
		return FALSE;
	}

	m_nStatus = nRunStatus;
	CString m_strRunStatus,strTemp;
	if(nRunStatus == 0)
		m_strRunStatus = StringFromID(IDS_RUNSTATUS_OVERHAULS);
	else if(nRunStatus == 1)
		m_strRunStatus = StringFromID(IDS_RUNSTATUS_OUTAGE);
	else if(nRunStatus == 2)
		m_strRunStatus = StringFromID(IDS_RUNSTATUS_SERVICE);
	else if(nRunStatus == 3)
		m_strRunStatus == StringFromID(IDS_RUNSTATUS_NOACCESS);
	else if(nRunStatus == 4)
		m_strRunStatus = StringFromID(IDS_RUNSTATUS_TESTING);
	else
		m_strRunStatus = StringFromID(IDS_RUNSTATUS_NOACCESS);
	
	
	int nSel=0,nCount = m_cbPTRunStatus.GetCount();
	for (int i=0;i<nCount;i++)
	{
		m_cbPTRunStatus.GetLBText(i,strTemp);
		if (!strTemp.CompareNoCase(m_strRunStatus))
		{
			m_cbPTRunStatus.SetCurSel(i);
			bGet = TRUE;
			break;
		}
	}
	
	if (!bGet)
	{
		m_cbPTRunStatus.SetCurSel(2);
	}
		
	SYSTEMTIME time;
	//GetSystemTime(&time);
	::GetLocalTime(&time);
	m_cstrTime.Format("%d/%02d/%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour,time.wMinute,
				time.wSecond);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


