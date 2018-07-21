// SysSettingPagePTProp.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "SysSettingPagePTProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysSettingPagePTProp property page

IMPLEMENT_DYNCREATE(CSysSettingPagePTProp, CPropertyPage)

//##ModelId=49B87B88035D
CSysSettingPagePTProp::CSysSettingPagePTProp() : CPropertyPage(CSysSettingPagePTProp::IDD)
{
	//{{AFX_DATA_INIT(CSysSettingPagePTProp)
	m_bSettingGroup = FALSE;
	m_bPTAI = FALSE;
	//}}AFX_DATA_INIT
	m_nSelSettingValues = 5;
	/** @brief           开关量项数下拉框选择的索引*/
	m_nSelDIValues = 5;
	/** @brief           开关量值显示风格下拉框选择的索引*/
	m_nSelDIStyle = 5;
	/** @brief           软压板项数下拉框选择的索引*/
	m_nSelSoftValues = 5;
	/** @brief           软压板值显示风格下拉框选择的索引*/
	m_nSelSoftStyle = 5;
}

//##ModelId=49B87B88035E
CSysSettingPagePTProp::~CSysSettingPagePTProp()
{

}

//##ModelId=49B87B880370
void CSysSettingPagePTProp::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSettingPagePTProp)
	DDX_Check(pDX, IDC_CHECK_SETTING_GROUP, m_bSettingGroup);
	DDX_Check(pDX, IDC_CHECK_PTAI, m_bPTAI);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysSettingPagePTProp, CPropertyPage)
	//{{AFX_MSG_MAP(CSysSettingPagePTProp)
	ON_CBN_SELCHANGE(IDC_CMB_DI_NUM, OnSelchangeCmbDiNum)
	ON_CBN_SELCHANGE(IDC_CMB_DI_VALUE, OnSelchangeCmbDiValue)
	ON_CBN_SELCHANGE(IDC_CMB_SETTING_NUM, OnSelchangeCmbSettingNum)
	ON_CBN_SELCHANGE(IDC_CMB_SOFT_NUM, OnSelchangeCmbSoftNum)
	ON_CBN_SELCHANGE(IDC_CMB_SOFT_VALUE, OnSelchangeCmbSoftValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSettingPagePTProp message handlers

//##ModelId=49B87B88037C
BOOL CSysSettingPagePTProp::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_SETTING_NUM);
	if(pCmb == NULL)
		return FALSE;
	//初始化定值历史值项数下拉框
	int i = 0;
	for(i = 0; i < 11; i++)
	{
		CString str;
		str.Format("%d", i);
		pCmb->AddString(str);
	}
	//预选择
	pCmb->SetCurSel(m_nSelSettingValues);

	//初始化开关量历史值项数下拉框
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DI_NUM);
	if(pCmb == NULL)
		return FALSE;
	for(i = 0; i < 11; i++)
	{
		CString str;
		str.Format("%d", i);
		pCmb->AddString(str);
	}
	pCmb->SetCurSel(m_nSelDIValues);

	//初始化开关量值显示风格下拉框
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DI_VALUE);
	if(pCmb == NULL)
		return FALSE;
	pCmb->AddString("1/0");
	pCmb->AddString("ON/OFF");
	pCmb->AddString( StringFromID(IDS_CASE_COMMISSIONING)+"/"+StringFromID(IDS_CASE_CEASING));
	pCmb->AddString( StringFromID(IDS_CASE_CLOSING)+"/"+StringFromID(IDS_CASE_OPENING));
	pCmb->AddString( StringFromID(IDS_CASE_ACTION)+"/"+StringFromID(IDS_CASE_RETURN));
	pCmb->SetCurSel(m_nSelDIStyle);

	//初始化软压板历史值项数下拉框
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_SOFT_NUM);
	if(pCmb == NULL)
		return FALSE;
	for(i = 0; i < 11; i++)
	{
		CString str;
		str.Format("%d", i);
		pCmb->AddString(str);
	}
	pCmb->SetCurSel(m_nSelSoftValues);

	//初始化软压板值显示风格下拉框
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_SOFT_VALUE);
	if(pCmb == NULL)
		return FALSE;
	pCmb->AddString("1/0");
	pCmb->AddString("ON/OFF");
	pCmb->AddString(StringFromID(IDS_CASE_COMMISSIONING)+"/"+StringFromID(IDS_CASE_CEASING));
	pCmb->SetCurSel(m_nSelSoftStyle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//##ModelId=49B87B88037E
void CSysSettingPagePTProp::OnSelchangeCmbDiNum() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DI_NUM);
	if(pCmb == NULL)
		return;
	m_nSelDIValues = pCmb->GetCurSel();
}

//##ModelId=49B87B880380
void CSysSettingPagePTProp::OnSelchangeCmbDiValue() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_DI_VALUE);
	if(pCmb == NULL)
		return;
	m_nSelDIStyle = pCmb->GetCurSel();
}

//##ModelId=49B87B880382
void CSysSettingPagePTProp::OnSelchangeCmbSettingNum() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_SETTING_NUM);
	if(pCmb == NULL)
		return;
	m_nSelSettingValues = pCmb->GetCurSel();
}

//##ModelId=49B87B880384
void CSysSettingPagePTProp::OnSelchangeCmbSoftNum() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_SOFT_NUM);
	if(pCmb == NULL)
		return;
	m_nSelSoftValues = pCmb->GetCurSel();
}

//##ModelId=49B87B88038B
void CSysSettingPagePTProp::OnSelchangeCmbSoftValue() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_SOFT_VALUE);
	if(pCmb == NULL)
		return;
	m_nSelSoftStyle = pCmb->GetCurSel();
}
