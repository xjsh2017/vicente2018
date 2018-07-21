// DlgExportProcess.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgExportProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExportProcess dialog


CDlgExportProcess::CDlgExportProcess(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExportProcess::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExportProcess)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgExportProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExportProcess)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExportProcess, CDialog)
	//{{AFX_MSG_MAP(CDlgExportProcess)
	//}}AFX_MSG_MAP
	ON_MESSAGE(EXPORT_PROCESS, OnProcessUpdate)
	ON_MESSAGE(EXPORT_RESULT, OnExportEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExportProcess message handlers

BOOL CDlgExportProcess::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CProgressCtrl* pProcess = (CProgressCtrl*)GetDlgItem(IDC_EXPORT_PROGRESS);
	if( pProcess )
	{
		pProcess->SetRange(0, 100);
		pProcess->SetPos(0);
	}
	CWnd* pBtn = GetDlgItem(IDOK);
	if(pBtn)
	{
		pBtn->EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgExportProcess::OnProcessUpdate(WPARAM wParam, LPARAM lParam)
{
	CProgressCtrl* pProcess = (CProgressCtrl*)GetDlgItem(IDC_EXPORT_PROGRESS);
	if( pProcess )
	{
		pProcess->SetRange( 0, (int)wParam );
		pProcess->SetPos( (int)lParam );
	}
	UpdateData(FALSE);
}

void CDlgExportProcess::OnExportEnd(WPARAM wParam, LPARAM lParam)
{
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_EXPORTRESULT);
	if(pStatic)
	{
		CString str;
		str.Format("%s,%s:%d,%s:%d", StringFromID(IDS_EXPORT_FINISHED),StringFromID(IDS_COMMON_SUCCESS),(int)wParam, StringFromID(IDS_COMMON_FAILED),(int)lParam);
		pStatic->SetWindowText(str);
	}
	CWnd* pBtn = GetDlgItem(IDOK);
	if(pBtn)
	{
		pBtn->EnableWindow(TRUE);
	}
}
