// DlgCheck.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCheck dialog


//##ModelId=49B87BA402E1
CDlgCheck::CDlgCheck(CWnd* pParent /*=NULL*/, int nType)
	: CDialog(CDlgCheck::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCheck)
	m_strModify = _T("");
	m_nType = nType;
	//}}AFX_DATA_INIT
}


//##ModelId=49B87BA402E4
void CDlgCheck::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCheck)
	DDX_Text(pDX, IDC_EDIT_CHECK, m_strModify);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCheck, CDialog)
	//{{AFX_MSG_MAP(CDlgCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCheck message handlers

BOOL CDlgCheck::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_nType ==1)
	{
		SetWindowText( StringFromID(IDS_CHECK_GUARDIAN));
	}
	else if (m_nType ==2)
	{
		SetWindowText( StringFromID(IDS_CHECK_OPERATOR));
	}
	else
	{
		SetWindowText( StringFromID(IDS_CHECK_DEFAULT));
	}
		
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
