// SysSettingPageInfoCase.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "SysSettingPageInfoCase.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CSysSettingPageInfoCase property page

IMPLEMENT_DYNCREATE(CSysSettingPageInfoCase, CPropertyPage)

//##ModelId=49B87B88039A
CSysSettingPageInfoCase::CSysSettingPageInfoCase() : CPropertyPage(CSysSettingPageInfoCase::IDD)
{
	//{{AFX_DATA_INIT(CSysSettingPageInfoCase)
	m_bBlinkDevice = FALSE;
	m_bBlinkSec = FALSE;
	m_bPopWnd = FALSE;
	m_bTopSec = FALSE;
	m_bUseSound = FALSE;
	m_bShowViewI = FALSE;
	m_bAlarmViewI = FALSE;
	m_bShowViewII = FALSE;
	m_bAlarmViewII = FALSE;
	m_bShowViewIII = FALSE;
	m_bAlarmViewIII = FALSE;
	m_nAlarmTime = 0;
	m_nFlashTime = 0;
	m_bAutoChange = FALSE;
	//}}AFX_DATA_INIT
}

//##ModelId=49B87B88039B
CSysSettingPageInfoCase::~CSysSettingPageInfoCase()
{
}

//##ModelId=49B87B8803C9
void CSysSettingPageInfoCase::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSettingPageInfoCase)
	DDX_Control(pDX, IDC_BTN_VIEWIII_COLOR, m_btnViewIIIColor);
	DDX_Control(pDX, IDC_BTN_VIEWII_COLOR, m_btnViewIIColor);
	DDX_Control(pDX, IDC_BTN_VIEWI_COLOR, m_btnViewIColor);
	DDX_Check(pDX, IDC_CHECK_BLINKDEVICE, m_bBlinkDevice);
	DDX_Check(pDX, IDC_CHECK_BLINKSEC, m_bBlinkSec);
	DDX_Check(pDX, IDC_CHECK_POP_WND, m_bPopWnd);
	DDX_Check(pDX, IDC_CHECK_TOP_STATION, m_bTopSec);
	DDX_Check(pDX, IDC_CHECK_USE_SOUND, m_bUseSound);
	DDX_Check(pDX, IDC_CHECK_VIEWI, m_bShowViewI);
	DDX_Check(pDX, IDC_CHECK_VIEWI_POP, m_bAlarmViewI);
	DDX_Check(pDX, IDC_CHECK_VIEWII, m_bShowViewII);
	DDX_Check(pDX, IDC_CHECK_VIEWII_POP, m_bAlarmViewII);
	DDX_Check(pDX, IDC_CHECK_VIEWIII, m_bShowViewIII);
	DDX_Check(pDX, IDC_CHECK_VIEWIII_POP, m_bAlarmViewIII);
	DDX_Text(pDX, IDC_EDIT_ALARMTIME, m_nAlarmTime);
	DDV_MinMaxInt(pDX, m_nAlarmTime, 0, 999);
	DDX_Text(pDX, IDC_EDIT_FLASHTIME, m_nFlashTime);
	DDX_Check(pDX, IDC_CHECK_AUTOCHANGE, m_bAutoChange);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysSettingPageInfoCase, CPropertyPage)
	//{{AFX_MSG_MAP(CSysSettingPageInfoCase)
	ON_BN_CLICKED(IDC_BTN_VIEWI_COLOR, OnBtnViewiColor)
	ON_BN_CLICKED(IDC_BTN_VIEWII_COLOR, OnBtnViewiiColor)
	ON_BN_CLICKED(IDC_BTN_VIEWIII_COLOR, OnBtnViewiiiColor)
	ON_BN_CLICKED(IDC_CHECK_BLINKSEC, OnCheckBlinksec)
	ON_BN_CLICKED(IDC_CHECK_VIEWI, OnCheckViewi)
	ON_BN_CLICKED(IDC_CHECK_VIEWII, OnCheckViewii)
	ON_BN_CLICKED(IDC_CHECK_VIEWIII, OnCheckViewiii)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSettingPageInfoCase message handlers

//##ModelId=49B87B8803CC
BOOL CSysSettingPageInfoCase::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(!m_bShowViewI)
	{
		//不显示I类事件, 则是否闪烁I类事件不可用
		GetDlgItem(IDC_CHECK_VIEWI_POP) ->EnableWindow(FALSE);
	}
	if(!m_bShowViewII)
	{
		//不显示II类事件, 则是否闪烁II类事件不可用
		GetDlgItem(IDC_CHECK_VIEWII_POP) ->EnableWindow(FALSE);
	}
	if(!m_bShowViewIII)
	{
		//不显示III类事件, 则是否闪烁III类事件不可用
		GetDlgItem(IDC_CHECK_VIEWIII_POP) ->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//##ModelId=49B87B8803CE
void CSysSettingPageInfoCase::OnBtnViewiColor() 
{
	// TODO: Add your control notification handler code here
	m_btnViewIColor.ShowColorDialog();
}

//##ModelId=49B87B8803D0
void CSysSettingPageInfoCase::OnBtnViewiiColor() 
{
	// TODO: Add your control notification handler code here
	m_btnViewIIColor.ShowColorDialog();
}

//##ModelId=49B87B8803D8
void CSysSettingPageInfoCase::OnBtnViewiiiColor() 
{
	// TODO: Add your control notification handler code here
	m_btnViewIIIColor.ShowColorDialog();
}

//##ModelId=49B87B8803DA
void CSysSettingPageInfoCase::OnCheckBlinksec() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bBlinkSec)
	{
		//选择
		GetDlgItem(IDC_CHECK_BLINKDEVICE) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_TOP_STATION) ->EnableWindow(TRUE);
	}
	else
	{
		//未选择
		m_bBlinkDevice = FALSE;
		m_bTopSec = FALSE;
		GetDlgItem(IDC_CHECK_TOP_STATION) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_BLINKDEVICE) ->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

//##ModelId=49B87B8803DC
void CSysSettingPageInfoCase::OnCheckViewi() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bShowViewI)
	{
		//选择
		//GetDlgItem(IDC_CHECK_VIEWI_POP) ->EnableWindow(TRUE);
	}
	else
	{
		//未选择
		//m_bAlarmViewI = FALSE;
		//GetDlgItem(IDC_CHECK_VIEWI_POP) ->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

//##ModelId=49B87B8803DE
void CSysSettingPageInfoCase::OnCheckViewii() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bShowViewII)
	{
		//选择
		//GetDlgItem(IDC_CHECK_VIEWII_POP) ->EnableWindow(TRUE);
	}
	else
	{
		//未选择
		//m_bAlarmViewII = FALSE;
		//GetDlgItem(IDC_CHECK_VIEWII_POP) ->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}

//##ModelId=49B87B8803E0
void CSysSettingPageInfoCase::OnCheckViewiii() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bShowViewIII)
	{
		//选择
		//GetDlgItem(IDC_CHECK_VIEWIII_POP) ->EnableWindow(TRUE);
	}
	else
	{
		//m_bAlarmViewIII = FALSE;
		//GetDlgItem(IDC_CHECK_VIEWIII_POP) ->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
