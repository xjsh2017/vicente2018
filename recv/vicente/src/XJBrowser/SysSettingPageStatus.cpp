// SysSettingPageStatus.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "SysSettingPageStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysSettingPageStatus property page

IMPLEMENT_DYNCREATE(CSysSettingPageStatus, CPropertyPage)

//##ModelId=49B87B8802FE
CSysSettingPageStatus::CSysSettingPageStatus() : CPropertyPage(CSysSettingPageStatus::IDD)
{
	//{{AFX_DATA_INIT(CSysSettingPageStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//##ModelId=49B87B8802FF
CSysSettingPageStatus::~CSysSettingPageStatus()
{
}

//##ModelId=49B87B88033C
void CSysSettingPageStatus::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSettingPageStatus)
	DDX_Control(pDX, IDC_BTN_RUN_ON_COLOR, m_btnRunON);
	DDX_Control(pDX, IDC_BTN_RUN_OFF_COLOR, m_btnRunOFF);
	DDX_Control(pDX, IDC_BTN_RUN_DEBUG_COLOR, m_btnRunDebug);
	DDX_Control(pDX, IDC_BTN_RUN_CODE_COLOR, m_btnRunCode);
	DDX_Control(pDX, IDC_BTN_RUN_CHECK_COLOR, m_btnRunCheck);
	DDX_Control(pDX, IDC_BTN_COMM_ON_COLOR, m_btnCommON);
	DDX_Control(pDX, IDC_BTN_COMM_OFF_COLOR, m_btnCommOFF);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysSettingPageStatus, CPropertyPage)
	//{{AFX_MSG_MAP(CSysSettingPageStatus)
	ON_BN_CLICKED(IDC_BTN_COMM_ON_COLOR, OnBtnCommOnColor)
	ON_BN_CLICKED(IDC_BTN_COMM_OFF_COLOR, OnBtnCommOffColor)
	ON_BN_CLICKED(IDC_BTN_RUN_ON_COLOR, OnBtnRunOnColor)
	ON_BN_CLICKED(IDC_BTN_RUN_CHECK_COLOR, OnBtnRunCheckColor)
	ON_BN_CLICKED(IDC_BTN_RUN_OFF_COLOR, OnBtnRunOffColor)
	ON_BN_CLICKED(IDC_BTN_RUN_DEBUG_COLOR, OnBtnRunDebugColor)
	ON_BN_CLICKED(IDC_BTN_RUN_CODE_COLOR, OnBtnRunCodeColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSettingPageStatus message handlers

//##ModelId=49B87B88033F
void CSysSettingPageStatus::OnBtnCommOnColor() 
{
	// TODO: Add your control notification handler code here
	m_btnCommON.ShowColorDialog();
}

//##ModelId=49B87B880341
void CSysSettingPageStatus::OnBtnCommOffColor() 
{
	// TODO: Add your control notification handler code here
	m_btnCommOFF.ShowColorDialog();
}

//##ModelId=49B87B88034B
void CSysSettingPageStatus::OnBtnRunOnColor() 
{
	// TODO: Add your control notification handler code here
	m_btnRunON.ShowColorDialog();
}

//##ModelId=49B87B88034D
void CSysSettingPageStatus::OnBtnRunCheckColor() 
{
	// TODO: Add your control notification handler code here
	m_btnRunCheck.ShowColorDialog();
}

//##ModelId=49B87B88034F
void CSysSettingPageStatus::OnBtnRunOffColor() 
{
	// TODO: Add your control notification handler code here
	m_btnRunOFF.ShowColorDialog();
}

//##ModelId=49B87B880351
void CSysSettingPageStatus::OnBtnRunDebugColor() 
{
	// TODO: Add your control notification handler code here
	m_btnRunDebug.ShowColorDialog();
}

//##ModelId=49B87B880353
void CSysSettingPageStatus::OnBtnRunCodeColor() 
{
	// TODO: Add your control notification handler code here
	m_btnRunCode.ShowColorDialog();
}
