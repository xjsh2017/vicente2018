// SysSettingSheet.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "SysSettingSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysSettingSheet

IMPLEMENT_DYNAMIC(CSysSettingSheet, CPropertySheet)

//##ModelId=49B87B8802DE
CSysSettingSheet::CSysSettingSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	AddPage(&m_PageGeneral);
	AddPage(&m_PageInfoCase);
	AddPage(&m_PageStatus);
	AddPage(&m_PagePTProp);
	AddPage(&m_PageStyle);
	AddPage(&m_PageBrowser);
}

//##ModelId=49B87B8802E2
CSysSettingSheet::CSysSettingSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;    // Lose the Apply Now button
	AddPage(&m_PageGeneral);
	AddPage(&m_PageInfoCase);
	AddPage(&m_PageStatus);
	AddPage(&m_PagePTProp);
	AddPage(&m_PageStyle);
	AddPage(&m_PageBrowser);
}

//##ModelId=49B87B8802FA
CSysSettingSheet::~CSysSettingSheet()
{
}


BEGIN_MESSAGE_MAP(CSysSettingSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSysSettingSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSettingSheet message handlers
