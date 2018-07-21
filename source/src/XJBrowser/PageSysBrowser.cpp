// PageSysBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PageSysBrowser.h"
#include "DlgWork.h"
#include "DlgApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSysBrowser property page

IMPLEMENT_DYNCREATE(CSysSettingPageBrowser, CPropertyPage)

CSysSettingPageBrowser::CSysSettingPageBrowser() : CPropertyPage(CSysSettingPageBrowser::IDD)
{
	//{{AFX_DATA_INIT(CPageSysBrowser)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSysSettingPageBrowser::~CSysSettingPageBrowser()
{
}

void CSysSettingPageBrowser::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSysBrowser)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysSettingPageBrowser, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSysBrowser)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BTN_BROWSER_WORK, OnBtnWorkStatus)
	ON_BN_CLICKED(IDC_BTN_BROWSER_APP, OnBtnWorkAPP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSysSettingPageBrowser::OnBtnWorkStatus()
{
	CDlgWork dlg;
	dlg.DoModal();
}

void CSysSettingPageBrowser::OnBtnWorkAPP()
{
	CDlgApp dlg;
	dlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPageSysBrowser message handlers
