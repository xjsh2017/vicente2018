// SysSettingPageGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "SysSettingPageGeneral.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysSettingPageGeneral dialog

CStringArray	arrFontName;
/*   
回调函数：枚举系统字体   
*/   
BOOL   CALLBACK   AFX_EXPORT   EnumFamScreenCallBackEx(ENUMLOGFONTEX   *pelf,   NEWTEXTMETRICEX   *,   int   FontType,   LPVOID   pThis)   
{   
	if(FontType   &   RASTER_FONTTYPE)   
		return 1;   
	CString   ss;   
	if(FontType   &   TRUETYPE_FONTTYPE)   
		ss=_T(pelf->elfLogFont.lfFaceName);
	if(ss.Find('@', 0) >= 0)
		return 1;
	arrFontName.Add(ss);     
	return   1;     
}

void EnumerateFonts()
{
	HDC   hDC;   
	hDC   =   ::GetWindowDC(NULL);   
	LOGFONT   lf;   
	ZeroMemory(&lf,sizeof(lf));   
	lf.lfCharSet   =   GB2312_CHARSET;   
	if(!EnumFontFamiliesEx(   
		hDC,	//   设备描述表句柄   
		&lf,	//   逻辑字体信息指针   
		(FONTENUMPROC)EnumFamScreenCallBackEx,	//   回调函数指针   
		(LPARAM)   0,	   
		(DWORD)   0))   
		return;   
    
	::ReleaseDC(NULL,hDC);	   	
}


//##ModelId=49B87B890002
CSysSettingPageGeneral::CSysSettingPageGeneral()
	: CPropertyPage(CSysSettingPageGeneral::IDD)
{
	//{{AFX_DATA_INIT(CSysSettingPageGeneral)
	m_bActionDetail = FALSE;
	m_bDeviceTree = FALSE;
	m_bOutPut = FALSE;
	m_bSysAlarmDetail = FALSE;
	m_sStyleName = _T("");
	m_iSel = -1;
	//}}AFX_DATA_INIT
	m_sFont = "";
	m_nFontSize = 18;
}


//##ModelId=49B87B890012
void CSysSettingPageGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSettingPageGeneral)
	DDX_Control(pDX, IDC_CMB_COMMMAP_FONTSIZE, m_cmbFontSize);
	DDX_Control(pDX, IDC_CMB_COMMMAP_FONT, m_cmbFont);
	DDX_Control(pDX, IDC_BTN_COMMMAP_FONTCOLOR, m_btnFontColor);
	DDX_Control(pDX, IDC_BTN_COMMMAP_BK, m_btnBkColor);
	DDX_Control(pDX, IDC_COMBO_STYLE_CHOSE, m_comboStyle);
	DDX_Check(pDX, IDC_CHECK_ACTIONDETAIL, m_bActionDetail);
	DDX_Check(pDX, IDC_CHECK_DEVICETREE, m_bDeviceTree);
	DDX_Check(pDX, IDC_CHECK_OUTPUT, m_bOutPut);
	DDX_Check(pDX, IDC_CHECK_SYSALARMDETAIL, m_bSysAlarmDetail);
	DDX_Text(pDX, IDC_EDIT_STYLE_SETTING_NAME, m_sStyleName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysSettingPageGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CSysSettingPageGeneral)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE_CHOSE, OnSelchangeComboStyleChose)
	ON_BN_CLICKED(IDC_BTN_COMMMAP_BK, OnBtnCommmapBk)
	ON_BN_CLICKED(IDC_BTN_COMMMAP_FONTCOLOR, OnBtnCommmapFontcolor)
	ON_CBN_SELCHANGE(IDC_CMB_COMMMAP_FONT, OnSelchangeCmbCommmapFont)
	ON_CBN_SELCHANGE(IDC_CMB_COMMMAP_FONTSIZE, OnSelchangeCmbCommmapFontsize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSettingPageGeneral message handlers

BOOL CSysSettingPageGeneral::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_comboStyle.AddString(StringFromID(IDS_STYLE_COMMON));
	m_comboStyle.AddString(StringFromID(IDS_STYLE_SOUTHNET));
	m_comboStyle.AddString(StringFromID(IDS_STYLE_COUNTRYNET));
	m_comboStyle.AddString(StringFromID(IDS_STYLE_CUSTOMIZE));

	if (g_style < 4 && g_style > 0)
	{
		m_comboStyle.SetCurSel(g_style);
	}
	else
	{
		m_comboStyle.SetCurSel(0);
	}

	m_sStyleName = g_strStyleName;

// 	arrFontName.RemoveAll();
// 	EnumerateFonts();
// 	for( int i =0; i < arrFontName.GetSize(); i++)
// 	{
// 		m_cmbFont.AddString( arrFontName.GetAt(i));
// 	}
	m_cmbFont.SelectString(0, g_CommMapFont);
	m_sFont = g_CommMapFont;
	CString strSize;
	strSize.Format("%d", g_CommMapFontSize);
	m_cmbFontSize.SelectString(0, strSize);
	m_nFontSize = g_CommMapFontSize;


	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSysSettingPageGeneral::OnSelchangeComboStyleChose() 
{
	// TODO: Add your control notification handler code here
	m_iSel = m_comboStyle.GetCurSel();
}

void CSysSettingPageGeneral::OnBtnCommmapBk() 
{
	// TODO: Add your control notification handler code here
	m_btnBkColor.ShowColorDialog();
}

void CSysSettingPageGeneral::OnBtnCommmapFontcolor() 
{
	// TODO: Add your control notification handler code here
	m_btnFontColor.ShowColorDialog();
}

void CSysSettingPageGeneral::OnSelchangeCmbCommmapFont() 
{
	// TODO: Add your control notification handler code here
	int nsel = m_cmbFont.GetCurSel();
	m_cmbFont.GetLBText(nsel, m_sFont);
}

void CSysSettingPageGeneral::OnSelchangeCmbCommmapFontsize() 
{
	// TODO: Add your control notification handler code here
	int nsel = m_cmbFontSize.GetCurSel();
	CString str;
	m_cmbFontSize.GetLBText(nsel, str);
	m_nFontSize = atoi(str);
}
