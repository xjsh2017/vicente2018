// DlgPtnote.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgPtnote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPtnote dialog


CDlgPtnote::CDlgPtnote(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPtnote::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPtnote)
	m_sContent = _T("");
	m_sName = _T("");
	//}}AFX_DATA_INIT
	m_pNote = NULL;
}


void CDlgPtnote::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPtnote)
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_sContent);
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPtnote, CDialog)
	//{{AFX_MSG_MAP(CDlgPtnote)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPtnote message handlers

BOOL CDlgPtnote::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( !m_pNote )
		return FALSE;

	m_sName = m_pNote->note_name;
	m_sContent = m_pNote->note_content;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPtnote::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_pNote->note_name = m_sName;
	m_pNote->note_content = m_sContent;

	m_pNote->update_time = CTime::GetCurrentTime();
	
	CDialog::OnOK();
}
