// PTSetting.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTSetting.h"

#include "DlgCheck.h"
#include "DlgCheckPro.h"
#include "DlgValidateID.h"
#include "GlobalFunc.h"
#include "DlgSettingDetail.h"
#include "ListCellEdit.h"

#include "MainFrm.h"
#include "DlgOperHis.h"

#include "XJPTSetStore.h"
#include "qptsetstatetable.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

#define CURRENT_ZONE	99

UINT PTSettingLoad(LPVOID pParam)
{
	CPTSetting* ptSetting = (CPTSetting*)pParam;
	if(ptSetting == NULL)
	{
		return 0;
	}
	int nQueryNo = ptSetting->GetCurrentQueryNo();
	int nLoadType = ptSetting->GetLoadType();
	

	if(!ptSetting->m_bExitThread && ptSetting->IsCurrentQueryNo(nQueryNo))
	{
		BOOL bLoad = ptSetting->ReLoadData(nQueryNo, nLoadType);
		
		ptSetting->PostMessage(THREAD_FILL_DATA, nQueryNo, nLoadType);
	}
		
	ptSetting->RemoveThreadNode(nQueryNo);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTSetting

IMPLEMENT_DYNCREATE(CPTSetting, CViewBase)

//##ModelId=49B87B8B0234
CPTSetting::CPTSetting()
	: CViewBase(CPTSetting::IDD)
{
	//{{AFX_DATA_INIT(CPTSetting)
	m_sCurrentZone = _T("0");
	m_sEditZone = _T("");
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_arrSetting.RemoveAll();
	m_arrGroup.RemoveAll();
	m_nNewValueIndex = 11;
	m_nCurrentStatus = 0;
	m_nCurrentDetailStatus = 0;
	m_arrModifyList.RemoveAll();
	m_nSrc = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_bCanGroup = FALSE;
	m_nNewZoneIndex = 6;
	m_nOperationNum = -1;
	m_arrNewSetting.RemoveAll();
	m_arrFrame.RemoveAll();
	m_nFrameCount = 0;

	m_arrColum.RemoveAll();

	m_bExitThread = FALSE;
	m_pLoadThread = NULL;

	m_nQueryNo = 0;

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	m_sOperUser = _T("");
	m_sMonUser = _T("");
	m_sRunUser = _T("");
	m_sOldZoneValue = _T("");
	m_bOperVerify = false;
	m_bMonVerify = false;
	m_bCallZoneSuccess = false;
	m_bOperTow = false;
	m_nModifySettingOrModifyZone = 0;
	m_bChecking = FALSE;
	m_bAlreadyShowOnce = false;

	m_listThread.RemoveAll();
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B8D03C9
CPTSetting::~CPTSetting()
{
	ClearModifyData();
	ClearSettingGroup();
	ClearSettingConfig();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B8D033C
void CPTSetting::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTSetting)
	DDX_Control(pDX, IDC_LIST_PTZONE, m_ListZone);
	DDX_Control(pDX, IDC_BTN_PTSET_VERIFY2, m_btnVerify2);
	DDX_Control(pDX, IDC_BTN_PTSET_VERIFY1, m_btnVerify1);
	DDX_Control(pDX, IDC_GIF_PTSET, m_gif);
	DDX_Control(pDX, IDC_BTN_PTSET_MODIFY_VIEWPROG, m_btnViewPTSetProg);
	DDX_Control(pDX, IDC_BTN_PTSET_MODIFY_VIEW_HIS, m_btnViewPTSetHis);
	DDX_Control(pDX, IDC_BTN_PTSET_ZONE_MODIFY, m_btnModifyZone);
	DDX_Control(pDX, IDC_BTN_PTSET_STYLE, m_btnStyle);
	DDX_Control(pDX, IDC_BTN_PTSET_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTSET_MODIFY, m_btnModifySetting);
	DDX_Control(pDX, IDC_BTN_PTSET_CALLZONE, m_btnCallZone);
	DDX_Control(pDX, IDC_BTN_PTSET_CALL, m_btnCallSetting);
	DDX_Control(pDX, IDC_LIST_PTSET, m_List);
	DDX_Control(pDX, IDC_TAB_PTSET, m_TabCtrl);
	DDX_Text(pDX, IDC_EDIT_PTSET_CURZONE, m_sCurrentZone);
	DDX_Text(pDX, IDC_EDIT_PTSET_EDITZONE, m_sEditZone);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTSetting, CViewBase)
	//{{AFX_MSG_MAP(CPTSetting)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTSET_CALL, OnBtnPtsetCall)
	ON_BN_CLICKED(IDC_BTN_PTSET_CALLZONE, OnBtnPtsetCallzone)
	ON_BN_CLICKED(IDC_BTN_PTSET_MODIFY, OnBtnPtsetModify)
	ON_BN_CLICKED(IDC_BTN_PTSET_ZONE_MODIFY, OnBtnPtsetZoneModify)
	ON_BN_CLICKED(IDC_BTN_PTSET_MODIFY_VIEWPROG, OnBtnViewProg)
	ON_BN_CLICKED(IDC_BTN_PTSET_MODIFY_VIEW_HIS, OnBtnViewHis)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PTSET, OnSelchangeTabPtset)
	ON_CBN_SELCHANGE(IDC_CMB_PTSET_CPU, OnSelchangeCmbPtsetCpu)
	ON_CBN_SELCHANGE(IDC_CMB_PTSET_SRC, OnSelchangeCmbPtsetSrc)
	ON_CBN_SELCHANGE(IDC_CMB_PTSET_SELZONE, OnSelchangeCmbPtsetSelzone)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTSET, OnCustomDraw)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTZONE, OnCustomDraw1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTSET, OnClickListPtset)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTSET_STYLE, OnBtnPtsetStyle)
	ON_BN_CLICKED(IDC_BTN_PTSET_PRINT, OnBtnPtsetPrint)
	ON_COMMAND(ID_SETTINGPRINT_NEW1, OnSettingprintNew1)
	ON_COMMAND(ID_SETTINGPRINT_NEW2, OnSettingprintNew2)
	ON_COMMAND(ID_SETTINGPRINT_NEW3, OnSettingprintNew3)
	ON_COMMAND(ID_SETTINGPRINT_ORDER_1, OnSettingprintOrder1)
	ON_COMMAND(ID_SETTINGPRINT_ORDER_2, OnSettingprintOrder2)
	ON_COMMAND(ID_SETTINGPRINT_ORDER3, OnSettingprintOrder3)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTSET_CALL, OnDoubleclickedBtnPtsetCall)
	ON_BN_DOUBLECLICKED(IDC_BTN_PTSET_CALLZONE, OnDoubleclickedBtnPtsetCallzone)
	ON_BN_CLICKED(IDC_BTN_PTSET_VERIFY1, OnBtnPtsetVerify1)
	ON_BN_CLICKED(IDC_BTN_PTSET_VERIFY2, OnBtnPtsetVerify2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTSET, OnDblclkListPtset)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTZONE, OnClickListPtzone)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTZONE, OnDblclkListPtzone)
	ON_BN_CLICKED(IDC_BTN_PTSET_EXCEL, OnBtnPtsetExcel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20016, OnSTTP20016)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20107, OnSTTP20107)
    ON_MESSAGE(WM_STTP_20174, OnSTTP20174)
	ON_MESSAGE(WM_STTP_20052, OnSTTP20052)
	ON_MESSAGE(WM_STTP_20054, OnSTTP20054)
	ON_MESSAGE(WM_STTP_20048, OnSTTP20048)
	ON_MESSAGE(WM_STTP_20170, OnSTTP20170)
	ON_MESSAGE(WM_STTP_20056, OnSTTP20056)
	ON_MESSAGE(WM_STTP_20172, OnSTTP20172)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(REFRESH_PTSETTING, OnRefreshPTSetting)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
	//ON_MESSAGE(WM_GETTIP, ShowTip)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTSetting diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8E004E
void CPTSetting::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B8E00BB
void CPTSetting::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTSetting message handlers

//##ModelId=49B87B8D02D0
void CPTSetting::OnInitialUpdate() 
{
	//记录控件位置
	//上面一排
	RecordRate(IDC_STATIC_PTSET_SRC, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTSET_SRC, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSET_CURZONE, 0, left_client, top_client);
	RecordRate(IDC_EDIT_PTSET_CURZONE, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTSET_SELZONE, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTSET_SELZONE, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSET_CPU, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTSET_CPU, 0, left_client, top_client);

	
	//TAB栏
	RecordRate(IDC_TAB_PTSET, 1, left_client, top_client);
	//下面一排按钮
	RecordRate(IDC_BTN_PTSET_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_CALLZONE, 0, left_client, top_client);
	RecordRate(IDC_STATIC_SETTING_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_SETTING_P2, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_STYLE, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_VERIFY1, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_VERIFY2, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_ZONE_MODIFY, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_MODIFY, 0, left_client, top_client);
	RecordRate(IDC_GIF_PTSET, 0, left_client, top_client);
    RecordRate(IDC_REMIND, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTSETTING, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_EXCEL, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTSETTING_LOADING, 0, mid_client, top_client);

	CViewBase::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
//	ResizeParentToFit();

	m_btnVerify1.SetIcon(IDI_KEY1);
	m_btnVerify1.SetFlat(FALSE);
	
	m_btnVerify2.SetIcon(IDI_KEY1);
	m_btnVerify2.SetFlat(FALSE);

	if(g_PTControlModel == 1)
	{
		//隐藏验证权限按钮
		m_btnVerify1.ShowWindow(SW_HIDE);
		m_btnVerify2.ShowWindow(SW_HIDE);
	}
	//初始化列表
	InitListCtrl();
	if (g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		InitZoneListCtrl();
	}
	//初始化其它控件
	InitControls();
	//判断隐藏/显示 分组TAB栏
	if(g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		//显示
		GetDlgItem(IDC_TAB_PTSET)->ShowWindow(SW_SHOW);
	}
	else
	{
		//隐藏
		GetDlgItem(IDC_TAB_PTSET)->ShowWindow(SW_HIDE);
	}

	//加载动画
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}

	if (g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		m_List.ShowWindow(false);
		m_ListZone.ShowWindow(true);
	}
	else
	{
		m_ListZone.ShowWindow(false);
	}
}

//##ModelId=49B87B8E0138
void CPTSetting::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(g_PTSettingGroup != 1 && g_PTControl != 1) //定值不分组
	{
		MoveCtrl(IDC_LIST_PTSET, left_client, IDC_BTN_PTSET_EXCEL, right_client, bottom_client);
		if(m_List.GetSafeHwnd() != NULL)
		{
			CRect rc;
			m_List.GetWindowRect(&rc);
			rc.bottom -= 16;
			ScreenToClient(&rc);
			
			m_List.MoveWindow(rc);
		}
	}
	if(g_PTSettingGroup == 1 || g_PTControl == 1) //定值分组
	{
		MoveCtrl(IDC_LIST_PTSET, left_client, IDC_TAB_PTSET, right_client, bottom_client);
		MoveCtrl(IDC_LIST_PTZONE, left_client, IDC_TAB_PTSET, right_client, bottom_client);
		if(m_List.GetSafeHwnd() != NULL)
		{
			CRect rc;
			m_List.GetWindowRect(&rc);
			rc.bottom -= 16;
			ScreenToClient(&rc);
			
			m_List.MoveWindow(rc);
		}

		if(m_ListZone.GetSafeHwnd() != NULL)
		{
			CRect rc;
			m_ListZone.GetWindowRect(&rc);
			rc.bottom -= 16;
			ScreenToClient(&rc);
			
			m_ListZone.MoveWindow(rc);
		}
	}
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B8C002E
int CPTSetting::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();

	return 0;
}

int CPTSetting::InitZoneListCtrl()
{
	if(m_ListZone.GetSafeHwnd() == NULL)
		return -1;
	InitZoneListStyle();
	
	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B8C009C
int CPTSetting::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	//先查找UIConfig配置文件中是否有此列表的列信息
	//如果有,按配置文件的信息来设置列
	//如果没有,按默认来设置列,并保存到配置文件
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvCol.fmt=LVCFMT_LEFT;
	if(LoadListConfig())
	{
		//载入配置成功
		for(int i = 0; i < m_arrColum.GetSize(); i++)
		{
			lvCol.iSubItem = i;
			lvCol.cx = m_arrColum[i].nItemWidth;
			CString sName = m_arrColum[i].sName;
			char * name = sName.GetBuffer(sName.GetLength());
			lvCol.pszText = name;
			BOOL bHide = m_arrColum[i].bHide;

			m_List.InsertColumn(i,&lvCol, bHide);
		}
	}
	else
	{
		//载入配置失败
		//char* arColumn[11]={"序号","定值名称", "定值代码", "单位", "基准值", "组号", "条目号","步长", "最小值/最大值", "精度", "数据类型"};
		//分配列宽,最小需求
		for (int nCol=0; nCol < 11 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 50; // 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 140; // 定值名称
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 140; // 定值代码
				colName = StringFromID(IDS_COMMON_CODE);
				break;
			case 3:
				lvCol.cx = 80; //单位
				colName = StringFromID(IDS_COMMON_UNIT);
				break;
			case 4:
				lvCol.cx = 80; //基准值
				colName = StringFromID(IDS_SETTING_ORDER);
				break;
			case 5:
				lvCol.cx = 60; //组号
				colName = StringFromID(IDS_POINT_GROUP);
				break;
			case 6:
				lvCol.cx = 60; //条目号
				colName = StringFromID(IDS_POINT_ITEM);
				break;
			case 7:
				lvCol.cx = 60; //步长
				colName = StringFromID(IDS_POINT_STEP);
				break;
			case 8:
				lvCol.cx = 80; //最小值/最大值
				colName = StringFromID(IDS_POINT_MAXMIN);
				break;
			case 9:
				lvCol.cx = 60; //精度
				colName = StringFromID(IDS_POINT_PRECISION);
				break;
			case 10:
				lvCol.cx = 80; //数据类型
				colName = StringFromID(IDS_POINT_DATATYPE);
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			int result = m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		//默认隐藏第3列(定值代码)
		m_List.SetColumnHide(2, TRUE);
		//默认隐藏第5列(基准值)
		m_List.SetColumnHide(4, TRUE);
		if(!g_PTControl)
		{
			m_List.SetColumnHide(5, TRUE);
			m_List.SetColumnHide(6, TRUE);
			m_List.SetColumnHide(7, TRUE);
			m_List.SetColumnHide(8, TRUE);
			m_List.SetColumnHide(9, TRUE);
		}
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}


int CPTSetting::InitZoneListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_ListZone.SetImageList(&m_l,LVSIL_SMALL);
	//先查找UIConfig配置文件中是否有此列表的列信息
	//如果有,按配置文件的信息来设置列
	//如果没有,按默认来设置列,并保存到配置文件
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_LEFT;
	
	//载入配置失败
	//char* arColumn[7]={"名称","组号", "条目号", "下限", "上限", "步长", "当前值"};
	//分配列宽,最小需求
	for (int nCol=0; nCol < 7 ; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0://
			lvCol.cx = 140; //名称
			colName = StringFromID(IDS_COMMON_NO);
			break;
		case 1://
			lvCol.cx = 80; //组号
			colName = StringFromID(IDS_POINT_GROUP);
			break;
		case 2://
			lvCol.cx = 80;  //条目号
			colName = StringFromID(IDS_POINT_ITEM);
			break;
		case 3:
			lvCol.cx = 80; //下限
			colName = StringFromID(IDS_POINT_LOWERLIMIT);
			break;
		case 4:
			lvCol.cx = 80; //上限
			colName = StringFromID(IDS_POINT_UPPERLIMIT);
			break;
		case 5:
			lvCol.cx = 80; //步长
			colName = StringFromID(IDS_POINT_STEP);
			break;
		case 6:
			lvCol.cx = 100; //当前值
			colName = StringFromID(IDS_POINT_CURRENT_VALUE);
			break;
		default:
			lvCol.cx = 100;
			break;
		}
		lvCol.pszText=colName.GetBuffer(1);
		int result = m_ListZone.InsertColumn(nCol,&lvCol);
	}
	
	//设置风格
	m_ListZone.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8C0157
BOOL CPTSetting::LoadListConfig()
{
	//读取UI配置文件
	BOOL bResult = TRUE;
	
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!", XJ_LOG_LV3);
			WriteLog("PTSetting::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
			bResult = FALSE;
		} 	
		//加载UIConfig文件 
		if(bResult)
		{
			if(!pDoc->load((_variant_t)g_UIConfig))
			{
				//加载失败
				MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
				CString str;
				str.Format("when load UIConfig, error in line %d, char %d\n%s", errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
				WriteLog(str, XJ_LOG_LV3);
				WriteLog("PTSetting::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTSettingConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到PTSettingConfig", XJ_LOG_LV3);
				WriteLog("PTSetting::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到PTSettingConfig下的配置", XJ_LOG_LV3);
					WriteLog("PTSetting::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
					bResult = FALSE;
				}
				else
				{
					//找到子节点列表指针
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					//循环读取子节点信息
					for(int i = 0; i < pChild ->Getlength(); i++)
					{
						//得到子节点
						MSXML2::IXMLDOMNodePtr pNode;
						pNode = pChild ->Getitem(i);
						//得到子节点属性列表指针
						MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap;
						pAttrMap = pNode ->Getattributes();
						
						//列属性结构
						SColumn	col;
						
						//循环读取属性
						for(int j = 0; j < pAttrMap ->Getlength(); j++)
						{
							MSXML2::IXMLDOMNodePtr			pItem;
							pItem = pAttrMap ->Getitem(j);
							CString strValue = (char *)(_bstr_t)pItem ->GetnodeValue();
							int nHide = 0;
							int nWidth = 0;
							switch(j)
							{
							case 0: //列名
								col.sName = strValue;
								break;
							case 1: //列宽
								nWidth = atoi(strValue);
								col.nItemWidth = nWidth;
								break;
							case 2:	//是否隐藏
								nHide = atoi(strValue);
								if(0 == nHide)
								{
									col.bHide = FALSE;
								}
								else
								{
									col.bHide = TRUE;
								}
								break;
							default:
								break;
							}
						}
						m_arrColum.Add(col);
					}
				}
			}
		}		
	}	
	//关闭打开的COM
	CoUninitialize();
	return bResult;
}

//##ModelId=49B87B8C01B5
BOOL CPTSetting::SaveListConfig()
{
	//保存UI配置文件
	BOOL bResult = TRUE;
	
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!", XJ_LOG_LV3);
			WriteLog("PTSetting::保存UIConfig失败", XJ_LOG_LV3);
			bResult = FALSE;
		} 	

		//加载UIConfig文件 
		if(bResult)
		{
			if(!pDoc->load((_variant_t)g_UIConfig))
			{
				//加载失败
				MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
				CString str;
				str.Format("when load UIConfig, error in line %d, char %d\n%s", errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
				WriteLog(str, XJ_LOG_LV3);
				WriteLog("PTSetting::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTSettingConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到PTSettingConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTSettingConfig");
					if(pNew != NULL)
					{
						//加入新节点
						pRoot ->appendChild(pNew);
						//新建所有子节点
						for(int i = 0; i < m_arrColum.GetSize(); i++)
						{
							MSXML2::IXMLDOMElementPtr pNewChild = NULL;
							pNewChild = pDoc ->createElement("Col");
							CString sName = m_arrColum[i].sName;
							int nWidth = m_arrColum[i].nItemWidth;
							BOOL bHide = m_arrColum[i].bHide;
							pNewChild ->setAttribute("Name", _variant_t(sName));
							CString sWidth;
							sWidth.Format("%d", nWidth);
							pNewChild ->setAttribute("Width", _variant_t(sWidth));
							CString sHide;
							if(bHide) //隐藏
							{
								sHide = "1";
							}
							else
							{
								sHide = "0";
							}
							pNewChild ->setAttribute("Hide", _variant_t(sHide));
							//加入子节点
							pNew ->appendChild(pNewChild);
						}
					}
				}
				else //找到了ViewActionConfig
				{
					//查找是否有子节点
					if(!pSelected ->hasChildNodes())
					{
						//找不到子节点
						WriteLog("未找到VPTSettingConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
						//新建所有子节点
						for(int i = 0; i < m_arrColum.GetSize(); i++)
						{
							MSXML2::IXMLDOMElementPtr pNewChild = NULL;
							pNewChild = pDoc ->createElement("Col");
							CString sName = m_arrColum[i].sName;
							int nWidth = m_arrColum[i].nItemWidth;
							BOOL bHide = m_arrColum[i].bHide;
							pNewChild ->setAttribute("Name", _variant_t(sName));
							CString sWidth;
							sWidth.Format("%d", nWidth);
							pNewChild ->setAttribute("Width", _variant_t(sWidth));
							CString sHide;
							if(bHide) //隐藏
							{
								sHide = "1";
							}
							else
							{
								sHide = "0";
							}
							pNewChild ->setAttribute("Hide", _variant_t(sHide));
							pSelected ->appendChild(pNewChild);
						}
					}
					else
					{
						//找到有子节点, 删除所有子节点,再新建
						WriteLog("找到了PTSettingConfig下的配置, 先删除新建所有子节点", XJ_LOG_LV3);
						//找到子节点列表指针
						MSXML2::IXMLDOMNodeListPtr pChild;
						pChild = pSelected ->GetchildNodes();
						
						//删除子节点
						MSXML2::IXMLDOMNodePtr pNode = NULL;
						pNode = pChild ->Getitem(0);
						while(pNode != NULL)
						{
							pSelected ->removeChild(pNode);
							pNode = pChild ->Getitem(0);
						}
						
						//新建子节点
						for(int i = 0; i < m_arrColum.GetSize(); i++)
						{
							MSXML2::IXMLDOMElementPtr pNewChild = NULL;
							pNewChild = pDoc ->createElement("Col");
							CString sName = m_arrColum[i].sName;
							int nWidth = m_arrColum[i].nItemWidth;
							BOOL bHide = m_arrColum[i].bHide;
							pNewChild ->setAttribute("Name", _variant_t(sName));
							CString sWidth;
							sWidth.Format("%d", nWidth);
							pNewChild ->setAttribute("Width", _variant_t(sWidth));
							CString sHide;
							if(bHide) //隐藏
							{
								sHide = "1";
							}
							else
							{
								sHide = "0";
							}
							pNewChild ->setAttribute("Hide", _variant_t(sHide));
							pSelected ->appendChild(pNewChild);
						}
					}
				}
				//保存配置文件
				pDoc ->save(_variant_t(g_UIConfig));
				bResult = TRUE;
			}
		}	
	}
	//关闭打开的COM
	CoUninitialize();
	return bResult;
}

/*************************************************************
 函 数 名：FillListData()
 功能概要：填充列表数据
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B8C00FA
int CPTSetting::FillListData(int nQueryNo)
{
	WriteLog("CPTSetting::FillListData, 开始", XJ_LOG_LV3);

	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);  
	int nGroupCount = m_arrGroup.GetSize();
	PT_SETTING *pts = NULL;
	int nIndex = 0;
	for(int i = 0; i < m_arrSetting.GetSize(); i ++)
	{
		pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		if(g_PTSettingGroup == 1 && nGroupCount > 0)
		{
			//按组显示
			if(pts->Group != m_nGroup)
			{
				//如果不等于当前所选组号
				continue;
			}
		}
		CString sID = pts->ID;
		if(1 == g_PTIndexType)
		{
			sID.Format("%d", nIndex+1);
		}
		m_List.InsertItem(nIndex, sID); //ID
		CString strName;
		int z = pts->Name.Find(",", 0);
		if (z != -1)
		{
			strName = pts->Name.Left(z);
		}
		else
		{
			strName = pts->Name;
		}
		m_List.SetItemText(nIndex, 1, strName); //名称
		m_List.SetItemText(nIndex, 2, pts->CodeName); //代码
		m_List.SetItemText(nIndex, 3, pts->Unit); //单位
		m_List.SetItemText(nIndex, 4, pts->OrderValue); //基准值
		CString str;
		str.Format("%d", pts->Group);
		m_List.SetItemText(nIndex, 5, str);
		str.Format("%d", pts->Item);
		m_List.SetItemText(nIndex,6, str);
		str.Format("%s", pts->stepValue);
		m_List.SetItemText(nIndex, 7, str);
		
		str.Format("%s/%s", pts->minValue, pts->maxValue);
		m_List.SetItemText(nIndex, 8, str);
		m_List.SetItemText(nIndex, 9, pts->Precision);
		switch (pts->DataType)
		{
       //0-浮点 1-整型 2-控制字(十六进制) 3-字符串 4-控制字(二进制)
		case 0:
			str = StringFromID(IDS_DATATYPE_FLOAT);
			break;
		case 1:
			str = StringFromID(IDS_DATATYPE_INT);
			break;
		case 2:
			str = StringFromID(IDS_DATATYPE_CONTROL16);
			break;
		case 3:
			str = StringFromID(IDS_DATATYPE_STRING);
			break;
		case 4:
			str = StringFromID(IDS_DATATYPE_STRING);
			break;
		default:
			str = _T("");
			break;
		}
		m_List.SetItemText(nIndex, 10, str);
		m_List.SetItemData(nIndex, (DWORD)pts);
		nIndex++;
	}
	//恢复刷新
	LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);

	if (g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		//填充ListCtrl内容
		m_ListZone.DeleteAllItems();
		//填充数据时禁止刷新
		m_ListZone.SetRedraw(FALSE);
		CSecCPU* pCpu = NULL;
		for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
		{
			pCpu = m_pObj->m_arrCPU[k];
			if (pCpu->code == atoi(m_sCPU))
			{
				break;
			}
		}
		MYASSERT_INT(pCpu);
		CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;
		if(m_pObj->m_bNewDevice)
		{
			for (int i = 0;  i < arrZONE.GetSize(); i++)
			{
				//	char* arColumn[7]={"名称","组号", "条目号", "下限", "上限", "步长", "当前值"};
				PT_ZONE * ptz = arrZONE[i];
				CString str;
				m_ListZone.InsertItem(i, ptz->zone_name); //ID
				str.Format("%d", ptz->group);
				m_ListZone.SetItemText(i, 1, str);
				str.Format("%d", ptz->item);
				m_ListZone.SetItemText(i, 2, str);
				str.Format("%d", ptz->minValue);
				m_ListZone.SetItemText(i, 3, str); 
				str.Format("%d", ptz->maxValue);
				m_ListZone.SetItemText(i, 4, str);
				str.Format("%d", ptz->stepValue);
				m_ListZone.SetItemText(i, 5, str);
				str.Format("%d", ptz->zoneValue);
				m_ListZone.SetItemText(i, 6, str);
				m_ListZone.SetItemData(i, (DWORD)ptz);
			}
		}
		else
		{
			int nMin = 10000;
			int nMax = -1;
			
			nMin = pCpu->from_zone<=pCpu->to_zone?pCpu->from_zone:pCpu->to_zone;
			nMax = pCpu->from_zone>=pCpu->to_zone?pCpu->from_zone:pCpu->to_zone;

			CString str;
			m_ListZone.InsertItem(0, StringFromID(IDS_SETTING_CURRENTZONE)); //ID
			m_ListZone.SetItemText(0, 1, "-");
			m_ListZone.SetItemText(0, 2, "-");
			str.Format("%d", nMin);
			m_ListZone.SetItemText(0, 3, str); 
			str.Format("%d", nMax);
			m_ListZone.SetItemText(0, 4, str);
			m_ListZone.SetItemText(0, 5, "-");
			m_ListZone.SetItemText(0, 6, m_sCurrentZone);
			//m_ListZone.SetItemData(i, (DWORD)ptz);
		}
		
		//恢复刷新
		m_ListZone.SetRedraw(TRUE);
	}

	WriteLog("CPTSetting::FillListData, 结束", XJ_LOG_LV3);
	return 0;
}

//##ModelId=49B87B8C0222
BOOL CPTSetting::NeedSave()
{
	BOOL bReturn = FALSE;

	//先保存列信息
	int nCount = m_List.m_hideHeader.m_aColum.GetSize();
	for(int j = 0; j < nCount; j++)
	{
		if(!m_List.m_hideHeader.m_aColum[j].bHide)
		{
			//只保存非隐藏列的列宽
			m_List.m_hideHeader.m_aColum[j].nItemWidth = m_List.GetColumnWidth(j);
		}
	}
	
	if(m_arrColum.GetSize() == 0)
	{
		//m_arrColum为空, 需要保存
		bReturn = TRUE;
	}
	else
	{
		//对比各项,发现一项不等就需要保存
		for(int i = 0; i < m_arrColum.GetSize(); i++)
		{
			if(m_arrColum[i].bHide != m_List.m_hideHeader.m_aColum[i].bHide)
			{
				bReturn = TRUE;
				break;
			}
			if(m_arrColum[i].nItemWidth != m_List.m_hideHeader.m_aColum[i].nItemWidth)
			{
				bReturn = TRUE;
				break;
			}
			if(m_arrColum[i].sName != m_List.m_hideHeader.m_aColum[i].sName)
			{
				bReturn = TRUE;
				break;
			}
		}
	}
	if(bReturn)
	{
		//如果需要保存, 则重写m_arrColum
		m_arrColum.RemoveAll();
		for(int i = 0; i < m_nNewValueIndex; i++)
		{
			SColumn col;
			col.sName = m_List.m_hideHeader.m_aColum[i].sName;
			col.nItemWidth = m_List.m_hideHeader.m_aColum[i].nItemWidth;
			col.bHide = m_List.m_hideHeader.m_aColum[i].bHide;
			m_arrColum.Add(col);
		}
	}
	return bReturn;
}

/*************************************************************
 函 数 名：InitControls()
 功能概要：初始化其它控件, 设置初始值, 显示风格
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B8C0271
int CPTSetting::InitControls()
{
	/*
	//召唤定值按钮
	m_btnCallSetting.SetThemeHelper(&m_ThemeHelper);
	m_btnCallSetting.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCallSetting.SetTooltipText(_T("召唤定值"));

	//召唤定值区号
	m_btnCallZone.SetThemeHelper(&m_ThemeHelper);
	m_btnCallZone.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCallZone.SetTooltipText(_T("召唤定值区号"));

	//显示设置按钮
	m_btnStyle.SetThemeHelper(&m_ThemeHelper);
	m_btnStyle.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnStyle.SetTooltipText(_T("设置显示风格"));
*/
	//打印按钮
	m_btnPrint.SetThemeHelper(&m_ThemeHelper);
	m_btnPrint.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnPrint.SetTooltipText( StringFromID(IDS_COMMON_PRINT));
	m_btnPrint.SetMenu(IDR_MENU_SETTING_PRINT, GetSafeHwnd());
/*
	//切换定值区号按钮
	m_btnModifyZone.SetThemeHelper(&m_ThemeHelper);
	m_btnModifyZone.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnModifyZone.SetTooltipText(_T("切换定值区"));

	//修改定值按钮
	m_btnModifySetting.SetThemeHelper(&m_ThemeHelper);
	m_btnModifySetting.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnModifySetting.SetTooltipText(_T("修改定值"));
*/
	return 0;
}


//##ModelId=49B87B8E01B5
void CPTSetting::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：ClearSettingConfig()
 功能概要：清除定值配置信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8C02CE
BOOL CPTSetting::ClearSettingConfig()
{
	EnterCriticalSection(&m_CriticalOper);
	//清除定值配置
	for(int i = 0; i < m_arrSetting.GetSize(); i++)
	{
		PT_SETTING * pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		delete pts;
		pts = NULL;
	}
	m_arrSetting.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);

	//清除定值组配置
// 	for(i = 0; i < m_arrGroup.GetSize(); i++)
// 	{
// 		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
// 		delete pg;
// 		pg = NULL;
// 	}
// 	m_arrGroup.RemoveAll();

	return TRUE;
}

/*************************************************************
 函 数 名：LoadSettingConfig()
 功能概要：从数据库载入保护定值定义, 保存在定值链表中
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8C031D
BOOL CPTSetting::LoadPTSettingConfig(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	WriteLog("CPTSetting::LoadPTSettingConfig,查询开始", XJ_LOG_LV3);
	BOOL bReturn = TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据PT_ID, cpu_code, zone, 103group(如果要分组显示的话)查找
	//setting_id,CODE_NAME,NAME,103item,property,vpickList,unit,s_precision,datatype
	//按setting_ID从小到大排列
		
	CString str;
		
	//字段
	//Setting_ID
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Setting_ID", EX_STTP_DATA_TYPE_INT);

	//name
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);

	//code_name
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "code_name", EX_STTP_DATA_TYPE_STRING);

	//vpickList
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "vpicklist", EX_STTP_DATA_TYPE_STRING);

	//unit
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "unit", EX_STTP_DATA_TYPE_STRING);

	//s_precision
	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "s_precision", EX_STTP_DATA_TYPE_STRING);

	//datatype
	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "datatype", EX_STTP_DATA_TYPE_INT);

	//minvalue
	Field Field9;
	pApp->m_DBEngine.SetField(sql, Field9, "minvalue", EX_STTP_DATA_TYPE_FLOAT);

	//maxvalue
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "maxvalue", EX_STTP_DATA_TYPE_FLOAT);

	//stepvalue
	Field Field11;
	pApp->m_DBEngine.SetField(sql, Field11, "stepvalue", EX_STTP_DATA_TYPE_FLOAT);

	//stepvalue
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "103group", EX_STTP_DATA_TYPE_INT);
	
	//stepvalue
	Field Field13;
	pApp->m_DBEngine.SetField(sql, Field13, "103item", EX_STTP_DATA_TYPE_INT);

	//stepvalue
	Field Field14;
	pApp->m_DBEngine.SetField(sql, Field14, "61850ref", EX_STTP_DATA_TYPE_INT);

	Field Field15;
	pApp->m_DBEngine.SetField(sql, Field15, "Reserve2", EX_STTP_DATA_TYPE_STRING);

	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

/*	int nGroupCount = 0;
	//103group
	if(g_PTSettingGroup == 1)
	{
		EnterCriticalSection(&m_CriticalOper);
		nGroupCount = m_arrGroup.GetSize();
		LeaveCriticalSection(&m_CriticalOper);
		//如果分组显示
		if(nGroupCount > 0)
		{
			Condition condition5;
			str.Format("103GROUP = %d", m_nGroup);
			pApp->m_DBEngine.SetCondition(sql, condition5, str);
		}
	}*/

	//按Setting_ID大小排序
	Condition condition4;
	str.Format("order by SETTING_ID");
	pApp->m_DBEngine.SetCondition(sql, condition4, str, EX_STTP_WHERE_ABNORMALITY); //非where条件	
		
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadPTSettingConfig, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CPTSetting::LoadPTSettingConfig,查询到%d条记录", nCount);
		WriteLog(strLog, XJ_LOG_LV3);
		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			//创建定值对象
			PT_SETTING * pts = new PT_SETTING;
			//setting_id,NAME,CODE_NAME,
			//vpickList,unit,s_precision,datatype
			CString str;
			pts->ID = pMemset.GetValue((UINT)0); //Setting_ID
			pts->Name = pMemset.GetValue(1); //NAME
			pts->CodeName = pMemset.GetValue(2); //Code_Name
			pts->VPickList = pMemset.GetValue(3); //vpicklist
			pts->Unit = "";
			pts->Unit = pMemset.GetValue(4); //unit
			pts->Precision = pMemset.GetValue(5); //s_precision
			str = pMemset.GetValue(6); //datatype
			pts->DataType = atoi(str);
			str = pMemset.GetValue(7);
			pts->minValue = str;//atof(str);
			str = pMemset.GetValue(8);
			pts->maxValue = str;//atof(str);
			str = pMemset.GetValue(9);
			pts->stepValue = str;//atof(str);
			str = pMemset.GetValue(10);
			pts->Group = atoi(str);
			str = pMemset.GetValue(11);
			pts->Item = atoi(str);
			str = pMemset.GetValue(12);
			pts->nRefType = 1;
			if(!str.IsEmpty())
			{
				str.MakeUpper();
				if(str.Find("$SP$", 0) != -1)
					pts->nRefType = 0;
			}
			else
			{
				CString sGroupName = GetGroupName(pts->Group);
				if(!sGroupName.IsEmpty())
				{
					if(sGroupName.Find( StringFromID(IDS_COMMON_PARAMETER), 0) != -1)
						pts->nRefType = 0;
				}
			}
			str = pMemset.GetValue(13);
			pts->ntimeMStoS = 0;
			if(!str.IsEmpty())
			{
				pts->ntimeMStoS = atoi(str);
			}

			//定值组
		/*	if(g_PTSettingGroup == 1 && nGroupCount > 0)
			{
				pts->Group = m_nGroup;
			}*/

			m_arrSetting.Add(pts);
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadPTSettingConfig,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}	
	
	WriteLog("CPTSetting::LoadPTSettingConfig,查询结束", XJ_LOG_LV3);

	return bReturn;
}

/*************************************************************
 函 数 名：LoadPTGroupConfig()
 功能概要：载入保护定值组组号,描述信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8C036B
BOOL CPTSetting::LoadPTGroupConfig(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	
	BOOL bReturn = TRUE;

	WriteLogEx("CPTSetting::LoadPTGroupConfig,查询开始");

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据station_id, 103addr, cpu_code, 查找103group,name
		
	CString str;	
	//字段
	//id
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "103group", EX_STTP_DATA_TYPE_INT);
	
	//name
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "name", EX_STTP_DATA_TYPE_STRING);
		
	//条件
	//station_id
	Condition condition2;
	str.Format("STATION_ID = '%s'", m_pObj->m_sStationID);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
		
	//103addr
	Condition condition3;
	str.Format("103ADDR = %d", m_pObj->m_n103Addr);
	pApp->m_DBEngine.SetCondition(sql, condition3, str);

	//cpu
	Condition condition4;
	str.Format("CPU = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition4, str);
		
	
	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_COMMU_103GROUP_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadPTGroupConfig, 查询失败");
		return FALSE;
	}
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();

		CString str;
		str.Format("CPTSetting::LoadPTGroupConfig,查询到%d条数据", nCount);
		WriteLogEx(str);
		
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			CString str = pMemset.GetValue((UINT)0); //103group
			int nGroup = atoi(str);
			str = pMemset.GetValue(1); //name
			SetGroupName(nGroup, str);

			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadPTGroupConfig,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLogEx("CPTSetting::LoadPTGroupConfig,查询正常结束");
	
	return bReturn;
}

/*************************************************************
 函 数 名：LoadOrderValue()
 功能概要：载入定值基准值信息
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数：param1	
**************************************************************/
//##ModelId=49B87B8C03B9
BOOL CPTSetting::LoadOrderValue(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	WriteLog("CPTSetting::LoadOrderValue, 查询开始", XJ_LOG_LV3);
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//先查找最新的一次基准值时间
	CString strTime = LoadNewOrderTime();
	if(strTime == "")
	{
		//没有最新时间, 表示没有基准值
		return FALSE;
	}
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	//查找最新的1条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	//根据PT_ID, CPU, Zone,Time 查找Value(基准值)
	
	//id
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "ID", EX_STTP_DATA_TYPE_INT);
	//value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "value", EX_STTP_DATA_TYPE_STRING);
		
	//条件
	CString str = "";
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
		
	//cpu
	Condition condition2;
	str.Format("CPU = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
		
	//zone
	Condition condition3;
	int nzone = atoi(m_sZone);
	if( nzone == CURRENT_ZONE )
		nzone = atoi(m_sCurrentZone);
	str.Format("ZONE = %d", nzone);
	pApp->m_DBEngine.SetCondition(sql, condition3, str);

	//时间
	Condition condition5;
	str.Format("TIME = '%s'", strTime);
	pApp->m_DBEngine.SetCondition(sql, condition5, str);
			
	
	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_ST_ORDERSETTING_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadOrderValue, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTSetting::LoadOrderValue,查询到%d条数据", nCount);
		WriteLogEx(str);
		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(g_role != MODE_SUB && i%20 == 0)
				Sleep(1);
			//取第一个
			str = pMemset.GetValue((UINT)0); //ID
			PT_SETTING* pts = FindSetting(str);
			if(pts != NULL)
			{
				pts->OrderValue = pMemset.GetValue(1);
			}
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadOrderValue,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}

	WriteLog("CPTSetting::LoadOrderValue, 查询结束", XJ_LOG_LV3);
	
	return bReturn;
}

/*************************************************************
 函 数 名：AddSettingValue()
 功能概要：插入某项定值到列表
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1   CString		定值ID
		   param2   CString		定值值
		   param3   int			要插入的列
**************************************************************/
//##ModelId=49B87B8D0021
BOOL CPTSetting::AddSettingValue( CString ID, CString VALUE, int iCol )
{
	EnterCriticalSection(&m_CriticalOper);
	//先找到Setting_ID==ID的索引
	BOOL bReturn = TRUE;
	CString strID = ID;
	strID.TrimRight();
	
	PT_SETTING* pst = (PT_SETTING*)FindSetting(strID);
	if(pst == NULL)
	{
		LeaveCriticalSection(&m_CriticalOper);
		return FALSE;
	}
	LVFINDINFO  fi;
	fi.flags = LVFI_PARAM;
	fi.lParam = (LPARAM)pst;
	
	//查找此值在第几行
	int nItem = m_List.FindItem(&fi);
	if(nItem<0)
	{
		LeaveCriticalSection(&m_CriticalOper);
		return FALSE;
	}
	
	//设置值
	pst->Value = VALUE;
	
	bReturn = m_List.SetItemText(nItem, iCol/*最新值*/, GetDisplayValue(pst, pst->Value));
	LeaveCriticalSection(&m_CriticalOper);
	return bReturn;
}

/*************************************************************
 函 数 名：LoadPTHistorySettingData()
 功能概要：载入保护历史值
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8D006D
BOOL CPTSetting::LoadPTHistorySettingData(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	WriteLog("CPTSetting::LoadPTHistorySettingData, 查询time开始", XJ_LOG_LV3);
	BOOL bReturn = TRUE;

	if(g_PTSettingValues <= 0)
		return TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//取得数据库内有历史数据的Time列表
	CString str = _T("");
		
	Field Field0;
	bzero(&Field0, sizeof(Field));
	Field0.FieldType = 97;
	sql.Fieldlist.push_back(Field0);
	
	//CURTIME
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "CURTIME";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TIME;
	sql.Fieldlist.push_back(Field1);
		
	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);

	int nzone = atoi(m_sZone);
	if( nzone == CURRENT_ZONE )
		nzone = atoi(m_sCurrentZone);
	Condition condition4;
	str.Format("ZONE = %d", nzone);
	pApp->m_DBEngine.SetCondition(sql, condition4, str);

	//按时间大小排序
	Condition condition5;
	str.Format("order by CURTIME DESC");
	pApp->m_DBEngine.SetCondition(sql, condition5, str, EX_STTP_WHERE_ABNORMALITY);//非where
			
	
	CMemSet pMemset;	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadPTHistorySettingData, 查询失败");
		return FALSE;
	}
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTSetting::LoadPTHistorySettingData,查询time,查询到%d条数据", nCount);
		WriteLogEx(str);
		
		for(int i = 0; i < nCount; i++)
		{
			if(i >= g_PTSettingValues)
				break;

			CString str;
			str = pMemset.GetValue((UINT)0);
			EnterCriticalSection(&m_CriticalOper);
			m_HisValueMgr.CreateValue(str);
			LeaveCriticalSection(&m_CriticalOper);
					
			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadPTHistorySettingData,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	WriteLog("CPTSetting::LoadPTHistorySettingData, 查询time结束", XJ_LOG_LV3);
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	WriteLog("CPTSetting::LoadPTHistorySettingData, 查询历史数据开始", XJ_LOG_LV3);
	//添加每一列
	//从数据库读出每个Time对应的定值组
	EnterCriticalSection(&m_CriticalOper);
	int nHisCount = m_HisValueMgr.GetCount();
	LeaveCriticalSection(&m_CriticalOper);
	for (int i = 0; i < nHisCount; i ++)
	{
		if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
			break;
		if(g_role != MODE_SUB)
			Sleep(1);

		EnterCriticalSection(&m_CriticalOper);
		CString strTime = m_HisValueMgr.GetTime(i);
		LeaveCriticalSection(&m_CriticalOper);
		
		//从数据库读出每个Time对应的历史值
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		CString str;	
		//字段
		//setting_id
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "setting_id", EX_STTP_DATA_TYPE_INT);

		//VALUE
		Field Field2;
		pApp->m_DBEngine.SetField(sql, Field2, "Value", EX_STTP_DATA_TYPE_STRING);
			
		//条件
		//PT_ID
		Condition condition1;
		str.Format("PT_ID = '%s'", m_pObj->m_sID);
		pApp->m_DBEngine.SetCondition(sql, condition1, str);

		//cpu_code
		Condition condition2;
		str.Format("CPU_CODE = %s", m_sCPU);
		pApp->m_DBEngine.SetCondition(sql, condition2, str);

		//zone
		Condition condition3;
		str.Format("ZONE = %d", nzone);
		pApp->m_DBEngine.SetCondition(sql, condition3, str);

		//CURTIME
		Condition condition4;
		str.Format("CURTIME = '%s'", strTime);
		pApp->m_DBEngine.SetCondition(sql, condition4, str);
				
		
		CMemSet pMemset;
			
		char sError[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
			
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_DATA, sql, sError, &pMemset);
		}
		catch (...)
		{
			WriteLogEx("CPTSetting::LoadPTHistorySettingData, 查询失败");
			continue;
		}
		if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
			break;
		if(nResult == 1)
		{
			pMemset.MoveFirst();
			int nCount = pMemset.GetMemRowNum();
			CString str;
			str.Format("CPTSetting::LoadPTHistorySettingData,查询%s的历史数据,条数为%d", strTime, nCount);
			WriteLogEx(str);

			EnterCriticalSection(&m_CriticalOper);
			for(int k = 0; k < nCount; k++)
			{
				if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
					break;
				if(g_role != MODE_SUB && k%20 == 0)
					Sleep(1);
				CString strID, strValue;
				strID = pMemset.GetValue((UINT)0);
				strValue = pMemset.GetValue(1);
				strValue.TrimLeft();
				strValue.TrimRight();

				m_HisValueMgr.AddValue(i, atoi(strID), strValue);

				pMemset.MoveNext();
			}
			LeaveCriticalSection(&m_CriticalOper);	
		}
		else
		{
			CString str;
			str.Format("CPTSetting::LoadPTHistorySettingData,查询失败,原因为%s", sError);
			WriteLogEx(str);
			continue;
		}
	}
	WriteLog("CPTSetting::LoadPTHistorySettingData, 查询历史数据结束", XJ_LOG_LV3);
	return TRUE;
}

/*************************************************************
 函 数 名：OnBtnPtsetCall()
 功能概要：召唤保护定值
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8E0203
void CPTSetting::OnBtnPtsetCall() 
{
	// TODO: Add your control notification handler code here
	//响应按钮按下操作, 召唤定值或取消召唤
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
	//验证权限
	if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
		return;
	}

	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}

	if (m_pObj->m_bNewDevice && m_sEditZone.IsEmpty())
	{
		AfxMessageBox( StringFromID(IDS_ZONE_UNKNOWN_EDITZONE));
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//空闲状态, 点击按钮表示要召唤定值
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		//组织20015报文

		int nSendZone = atoi(m_sZone);
		//if(nSendZone == atoi(m_sCurrentZone))
		//	nSendZone = CURRENT_ZONE;
		
		if(!pApp->m_SttpEngine.BuildDataFor20015PTSetting(sttpData, m_pObj->m_sID, atoi(m_sCPU), nSendZone, m_nSrc))
		{
			//组织报文失败
			return;
		}
		
	
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("保护[%s]发送召唤定值报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送召唤定值报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,状态改为正在召唤定值
			m_nCurrentStatus = 1;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nPTSettingTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//增加人工操作日志
			//pApp->AddManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sName);

			//更改控件可用度
			UpdateControlsEnable();

			CString str;
			str.Format("保护[%s]发送召唤定值报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//正在召唤定值
		//取消召唤
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}
}

/*************************************************************
 函 数 名：OnSTTP20016()
 功能概要：响应20016(保护定值下载通知)报文
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8E034B
void CPTSetting::OnSTTP20016( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤定值的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20016报文
	if(pSttpData->sttp_head.uMsgID != 20016)
	{
		//错误报文
		CString str;
		str.Format("CPTSetting::OnSTTP20016 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20016 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//检查CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//不是本CPU的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20016 收到错误CPU报文, CPU为%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//检查定值区号
	int nZone = pSttpData->sttp_body.nZone;
	if(nZone != atoi(m_sZone) && nZone != CURRENT_ZONE && atoi(m_sZone) != CURRENT_ZONE)
	{
		//不是指定定值区号的定值
		CString str;
		str.Format("指定的定值区号为%s, 上送报文的定值区号为%d", m_sZone, nZone);
		WriteLog(str, XJ_LOG_LV3);
	}

	//判断是单帧还是多帧
	if(pSttpData->sttp_head.uMsgType < 2)
	{
		//单帧,直接处理
		//关闭定时器
		KillTimer(m_nTimer);
		
		//处理报文
		if(!LoadDataFromSTTP(pSttpData))
		{
			WriteLog("CPTSetting::OnSTTP20016 处理报文失败", XJ_LOG_LV3);
		}
		
		//处理完毕,改变状态和按钮文字
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();

		CString sLog;
		sLog.Format("保护[%s]召唤定值成功!", m_pObj->m_sName);
		WriteLog(sLog, XJ_LOG_LV2);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d,%s,%s", OPER_SUCCESS, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_SUCCESS);

		// 保持最新一次定值到 定义表
		//CXJPTSetStore::GetInstance()->SaveRecallToDB(m_sCPU, m_pObj->m_sID, m_arrSetting);
	}
	else if(pSttpData->sttp_head.uMsgType >= 2)
	{
		//多帧
		m_nFrameCount = pSttpData->sttp_head.uMsgType; //总帧数
		//当前帧序号
		int nFrame = pSttpData->sttp_head.uMsgLengthType;
		//判断合法性
		if(nFrame > m_nFrameCount)
		{
			//处理完毕,改变状态和按钮文字
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();

			CString sLog;
			sLog.Format("保护[%s]召唤定值失败!", m_pObj->m_sName);
			WriteLog(sLog, XJ_LOG_LV1);

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_FAILD);

			return;
		}
		//判断当前帧是否已收到过
		BOOL bFound = FALSE;
		for(int i = 0; i < m_arrFrame.GetSize(); i++)
		{
			int nTempFrame = m_arrFrame.GetAt(i);
			if(nTempFrame == nFrame)
			{
				//已存在
				bFound = TRUE;
				break;
			}
		}
		if(bFound)
		{
			//已存在, 不处理此报文
			CString str;
			str.Format("收到定值多帧报文, 总帧数%d, 当前是第%d帧, 之前已收到过,不处理", m_nFrameCount, nFrame);
			WriteLog(str, XJ_LOG_LV3);
			return;
		}
		else
		{
			//之前不存在,处理
			CString str;
			str.Format("收到定值多帧报文, 总帧数%d, 当前是第%d帧", m_nFrameCount, nFrame);
			WriteLog(str, XJ_LOG_LV3);
			//保存序号
			m_arrFrame.Add(nFrame);
			//解析报文
			TranslateMultiFrame(pSttpData);
		}
	}
}

/*************************************************************
 函 数 名：LoadDataFromSTTP()
 功能概要: 从STTP报文中读取数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
**************************************************************/
//##ModelId=49B87B8D00BB
BOOL CPTSetting::LoadDataFromSTTP( STTP_FULL_DATA * sttpData )
{
	if(sttpData == NULL)
		return FALSE;
	
	//解析定值数据
	try
	{
		//时间,取当前系统时间
		CTime tmTime = CTime::GetCurrentTime();
		CString strTimeDisplay = tmTime.Format("%Y-%m-%d %H:%M:%S");
		//新加一列
		int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
		//读取每项值,加入到列表
		int nCount = sttpData->sttp_body.variant_member.value_data.size();
		for(int i = 0; i < nCount; i++)
		{
			CString id;
			id.Format("%d", sttpData->sttp_body.variant_member.value_data[i].id);
			CString Value = XJToMS(sttpData->sttp_body.variant_member.value_data[i].str_value);
			Value.TrimLeft();
			Value.TrimRight();
			Value = ComplexExchange(Value);
			AddSettingValue(id, Value, Index);
		}
	}
	catch (...)
	{
		WriteLog("CPTSetting::LoadDataFromSTTP, 错误", XJ_LOG_LV3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：OnSTTP20069()
 功能概要：响应20069(控制报文通用返回通知)报文
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8E038A
void CPTSetting::OnSTTP20069( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;

	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 3 && m_nCurrentStatus != 4)
	{
		//没有处于召唤定值状态,也没有处于召唤定值区号状态,
		//也没有处于修改定值状态, 也没有处于定值组切换状态,不处理报文
		CString str;
		str.Format("CPTSetting::OnSTTP20069 当前状态为%d", m_nCurrentStatus);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		WriteLog("CPTSetting::OnSTTP20069 报文为空", XJ_LOG_LV3);
		return;
	}
	
	//检查是否20069报文
	if(pSttpData->sttp_head.uMsgID != 20069)
	{
		//错误报文
		WriteLog("CPTSetting::OnSTTP20069 错误报文", XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		WriteLog("CPTSetting::OnSTTP20069 不是本装置的报文", XJ_LOG_LV3);
		return;
	}

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20069 当前回复操作为%d,当前状态为%d", pSttpData->sttp_body.nMsgId, m_nCurrentStatus);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	int nStatus = pSttpData->sttp_body.nStatus;
	if(0)
	{
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();
	}
	else
	{
		//操作失败
		//处理完毕,改变状态和按钮文字
		if(m_nCurrentStatus == 1)
		{
			//正在召唤定值				
			CString str;
			str.Format("保护[%s]召唤定值失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_FAILD);

			AfxMessageBox( StringFromID(IDS_CALL_FAIL));
		}
		else if(m_nCurrentStatus == 2)
		{
			//正在召唤定值区号				
			CString str;
			str.Format("保护[%s]召唤定值区号失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			m_bCallZoneSuccess = false;
			//更改控件可用度
			UpdateControlsEnable();

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ZONE, m_sCPU);			
			pApp->AddNewManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ZONE, OPER_FAILD);

			if (m_bOperTow)
			{
				AfxMessageBox( StringFromID(IDS_CALLZONG_FAIL_BEFORE_MODIFY));
				m_bOperTow = false;
				m_bOperVerify = false;
				m_bMonVerify = false;
				UpdateControlsEnable();
			}
			else
			{
				AfxMessageBox( StringFromID(IDS_CALL_FAIL));
			}
		}
		else if(m_nCurrentStatus == 3)
		{
			//正在修改定值
			m_bMonVerify = false;
			m_bOperVerify = false;
			CString str;
			str.Format("保护[%s]修改定值失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;

			//回复修改前的值
			RevertModifyValue();
			
			//更改控件可用度
			UpdateControlsEnable();


			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			if(m_bChecking)
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_PRECORRECTION,m_sCPU, m_sZone, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				
				pApp->AddNewManOperator("定值修改预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_EXECUTE,m_sCPU, m_sZone, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}

				pApp->AddNewManOperator("定值修改执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
			AfxMessageBox( StringFromID(IDS_MODIFY_SETTING_FAIL));
		}
		else if(m_nCurrentStatus == 4)
		{
			//正在切换定值区
			m_bMonVerify = false;
			m_bOperVerify = false;
			CString str;
			str.Format("保护[%s]定值区切换失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();
			AfxMessageBox( StringFromID(IDS_MODIFY_ZONE_FAIL));
			//回复修改前的值
			RevertModifyValue(2);

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			if(m_bChecking)
			{
				CString strMsg;
				if(g_OperTableType == 0)
					strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone);
				else if(g_OperTableType == 1)	
					strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone, m_sCurrentZone);
				pApp->AddNewManOperator("定值区切换预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				if(g_OperTableType == 0)
					strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone);
				else if(g_OperTableType == 1)
					strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone, m_sCurrentZone);
				pApp->AddNewManOperator("定值区切换执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
		}	
	}
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：响应20125(装置不支持功能)报文
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文内容
**************************************************************/
//##ModelId=49B87B8E03C8
void CPTSetting::OnSTTP20125( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;

	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
        WriteLog("CPTSetting::OnSTTP20125 报文为空,不处理报文", XJ_LOG_LV3);
		return;
	}
	
	//检查是否20125报文
	if(pSttpData->sttp_head.uMsgID != 20125)
	{
 		//错误报文
        WriteLog("CPTSetting::OnSTTP20125 错误报文", XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
        WriteLog("CPTSetting::OnSTTP20125 不是本装置的报文", XJ_LOG_LV3);
		return;
	}

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		CString strInfo;
		strInfo.Format("pSttpData->sttp_body.nMsgId: %d   m_nCurrentStatus: %d", pSttpData->sttp_body.nMsgId, m_nCurrentStatus);
        WriteLog("CPTSetting::OnSTTP20125 不是本功能的报文" + strInfo, XJ_LOG_LV3);
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	//处理完毕,改变状态和按钮文字
	if(m_nCurrentStatus == 1)
	{
		//正在召唤定值
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_FAILD);

		AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤定值区号
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ZONE, m_sCPU);			
		pApp->AddNewManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ZONE, OPER_FAILD);

		AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 3)
	{
		//正在修改定值
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();

		//回复修改前的值
		RevertModifyValue();

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		if(m_bChecking)
		{
			CString strMsg;
			strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_PRECORRECTION,m_sCPU, m_sZone, m_arrModifyList.GetSize());
			for(int i = 0; i < m_arrModifyList.GetSize(); i++)
			{
				STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
				CString strTemp = sttpData->str_value.c_str();
				if(g_OperTableType == 0)
					strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
				else if(g_OperTableType == 1)
				{
					CString sOldValue = FindNewValue(sttpData->id);
					strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
				}
			}
			
			pApp->AddNewManOperator("定值修改预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_PRECORRECTION, OPER_FAILD, m_nOperationNum);
		}
		else
		{
			CString strMsg;
			strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_EXECUTE,m_sCPU, m_sZone, m_arrModifyList.GetSize());
			for(int i = 0; i < m_arrModifyList.GetSize(); i++)
			{
				STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
				CString strTemp = sttpData->str_value.c_str();
				if(g_OperTableType == 0)
					strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
				else if(g_OperTableType == 1)
				{
					CString sOldValue = FindNewValue(sttpData->id);
					strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
				}
			}
			pApp->AddNewManOperator("定值修改执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_EXECUTE, OPER_FAILD, m_nOperationNum);
		}

		AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 4)
	{
		//正在切换定值区
		m_nCurrentStatus = 0;
		
		//更改控件可用度
		UpdateControlsEnable();

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		if(m_bChecking)
		{
			CString strMsg;
			if(g_OperTableType == 0)
				strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone);
			else if(g_OperTableType == 1)	
				strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone, m_sCurrentZone);
			pApp->AddNewManOperator("定值区切换预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_PRECORRECTION, OPER_FAILD, m_nOperationNum);
		}
		else
		{
			CString strMsg;
			if(g_OperTableType == 0)
				strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone);
			else if(g_OperTableType == 1)
				strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone, m_sCurrentZone);
			pApp->AddNewManOperator("定值区切换执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_EXECUTE, OPER_FAILD, m_nOperationNum);
		}

		AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
}

/*************************************************************
 函 数 名：OnBtnPtsetCallzone()
 功能概要：召唤当前定值区号
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8E0242
void CPTSetting::OnBtnPtsetCallzone() 
{
	// TODO: Add your control notification handler code here
	//响应按钮按下操作, 召唤定值区号或取消召唤
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}

	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//空闲状态, 点击按钮表示要召唤
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		if (m_pObj->m_bNewDevice)
		{
			//组织20106报文
			if(!pApp->m_SttpEngine.BuildDataFor20173PTZone(sttpData, m_pObj->m_sID, atoi(m_sCPU)))
			{
				//组织报文失败
				return;
			}
		}
		else
		{
			//组织20106报文
			if(!pApp->m_SttpEngine.BuildDataFor20106PTZone(sttpData, m_pObj->m_sID, atoi(m_sCPU)))
			{
				//组织报文失败
				return;
			}
		}
		
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("保护[%s]发送召唤定值报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送召唤定值报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,状态改为正在召唤定值区号
			m_nCurrentStatus = 2;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nPTZoneTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			//增加人工操作日志
			//pApp->AddManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sName);

			//更改控件可用度
			UpdateControlsEnable();

			CString str;
			str.Format("保护[%s]发送召唤定值区号报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤定值区号
		//取消召唤
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}

	
}

/*************************************************************
 函 数 名：OnSTTP20107()
 功能概要：响应20107(召唤定值区号通知)报文
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8F000F
void CPTSetting::OnSTTP20107( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 2)
	{
		//没有处于召唤定值区号的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20107报文
	if(pSttpData->sttp_head.uMsgID != 20107)
	{
		//错误报文
		CString str;
		str.Format("CPTSetting::OnSTTP20107 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	} 
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20107 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//检查CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//不是本CPU的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20107 收到错误CPU报文, CPU为%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	int nZone = pSttpData->sttp_body.nZone;
	m_sCurrentZone.Format("%d", nZone);

	if (m_ListZone.GetItemCount() != 0)
	{
		if(!m_pObj->m_bNewDevice)
		{
			//确认第一条就是显示当前定值区
			CString str;
			str.Format("%d", nZone);
			m_ListZone.SetItemText(0, m_nNewZoneIndex, str);
		}
	}

	//SetZoneCmbToCurrentZone();

	CString sLog;
	sLog.Format("保护[%s]召唤定值区号成功!", m_pObj->m_sName);
	WriteLog(sLog, XJ_LOG_LV2);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_SUCCESS, XJ_OPER_CALL_ZONE, m_sCPU);			
	pApp->AddNewManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ZONE, OPER_SUCCESS);
	
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;

	//刷新界面显示
	UpdateData(FALSE);

	//更改控件可用度
	UpdateControlsEnable();
}


void CPTSetting::OnSTTP20174( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 2)
	{
		WriteLog("CPTSetting::OnSTTP20174 没有处于召唤定值区号的状态,不处理报文", XJ_LOG_LV3);
		//没有处于召唤定值区号的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20174报文
	if(pSttpData->sttp_head.uMsgID != 20174)
	{
		//错误报文
		CString str;
		str.Format("CPTSetting::OnSTTP20174 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20174 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//不是本CPU的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20174 收到错误CPU报文, CPU为%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//关闭定时器
	KillTimer(m_nTimer);
	
	CSecCPU* pCpu = NULL;
	for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
	{
		pCpu = m_pObj->m_arrCPU[k];
		if (pCpu->code == nCpu)
		{
			break;
		}
	}
	MYASSERT(pCpu);

	bool bHasEditZone = false;
	CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;

	int nCount = pSttpData->sttp_body.variant_member.value_data.size();
	for(int i = 0; i < nCount; i++)
	{
		int id;
		id = pSttpData->sttp_body.variant_member.value_data[i].id;
		CString Value = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
		for (int j = 0; j < arrZONE.GetSize(); j++)
		{
			PT_ZONE * ptz = arrZONE[j];
			if (ptz->code == id)
			{
				if (ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) != -1)
				{
					m_sCurrentZone = Value;
					ptz->zoneValue = atoi(Value);
					LVFINDINFO lvFind;
					lvFind.flags = LVFI_PARAM;
					lvFind.lParam = (LPARAM)ptz;
					int nIndex = m_ListZone.FindItem(&lvFind);
					if(nIndex != -1 && m_nModifySettingOrModifyZone == 0)
					{
						m_ListZone.SetItemText(nIndex, m_nNewZoneIndex, Value);
					}
				}
				else if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
				{
					m_sEditZone = Value;
					ptz->zoneValue = atoi(Value);
					LVFINDINFO lvFind;
					lvFind.flags = LVFI_PARAM;
					lvFind.lParam = (LPARAM)ptz;
					int nIndex = m_ListZone.FindItem(&lvFind);
					if(nIndex != -1 && m_nModifySettingOrModifyZone == 0)
					{
						m_ListZone.SetItemText(nIndex, m_nNewZoneIndex, Value);
					}
					bHasEditZone = true;
				}
				break;
			}
		}
	}


	/*
	//处理报文
	int nZone = pSttpData->sttp_body.nZone;
	m_sCurrentZone.Format("%d", nZone);
	*/
	if (!bHasEditZone)
	{
		AfxMessageBox( StringFromID(IDS_CALL_EDITZONE_FAIL));
		m_bCallZoneSuccess = false;
	}
	else
	{
		m_bCallZoneSuccess = true;
	}
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
	//刷新界面显示
	UpdateData(FALSE);
	
	//更改控件可用度
	UpdateControlsEnable();
	WriteLog("CPTSetting::OnSTTP20174 处理结束", XJ_LOG_LV3);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_SUCCESS, XJ_OPER_CALL_ZONE, m_sCPU);			
	pApp->AddNewManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ZONE, OPER_SUCCESS);

	if (m_bOperTow)
	{
		if (!m_bCallZoneSuccess)
		{
			AfxMessageBox(StringFromID(IDS_CALLZONG_FAIL_BEFORE_MODIFY));
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
			return;
		}
		else
		{
			CString str;
			str.Format("%s, %s(%s), %s(%s)", StringFromID(IDS_ZONE_AUTOCALL_SUCCESS),
				StringFromID(IDS_ZONE_RUN),m_sCurrentZone, StringFromID(IDS_ZONE_EDIT),m_sEditZone);
			AfxMessageBox(str);
			if (m_nModifySettingOrModifyZone == 0)
			{
				return;
			}
			else if(m_nModifySettingOrModifyZone == 1)
			{
				OnBtnPtsetModify1();
			}
			else if(m_nModifySettingOrModifyZone == 2)
			{
				OnBtnPtsetZoneModify1();
			}
			
		}
	}
}

/*************************************************************
 函 数 名：UpdateControlsEnable()
 功能概要：根据当前状态更新各控件可用性
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8D00EA
void CPTSetting::UpdateControlsEnable()
{
	SetDlgItemText(IDC_REMIND, "");
	//按状态分情况
	if(m_nCurrentStatus == 0)
	{	
		//召唤定值
		SetDlgItemText(IDC_BTN_PTSET_CALL, StringFromID(IDS_BTN_CALLSETTING));
		//召唤定值区号
		SetDlgItemText(IDC_BTN_PTSET_CALLZONE, StringFromID(IDS_BTN_CALLZONE));
		//修改定值
		SetDlgItemText(IDC_BTN_PTSET_MODIFY, StringFromID(IDS_MODIFY_SETTING));
		//切换定值区
		SetDlgItemText(IDC_BTN_PTSET_ZONE_MODIFY, StringFromID(IDS_MODIFY_ZONE));

		//空闲状态,都可用
		GetDlgItem(IDC_BTN_PTSET_CALL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSET_CALLZONE) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSET_MODIFY) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSET_ZONE_MODIFY) ->EnableWindow(TRUE);
		if(g_PTControlModel == 2)
		{
			//广东模式
			if(!m_bOperVerify)
			{
				GetDlgItem(IDC_BTN_PTSET_VERIFY2)->EnableWindow(false);
				m_btnVerify1.SetIcon(IDI_KEY1);
				m_btnVerify2.SetIcon(IDI_KEY1);
			}
			else
			{
				m_btnVerify1.SetIcon(IDI_TICK);
				if (m_bMonVerify)
				{
					m_btnVerify2.SetIcon(IDI_TICK);
				}
				else
				{
					m_btnVerify2.SetIcon(IDI_KEY1);
				}
				GetDlgItem(IDC_BTN_PTSET_VERIFY2)->EnableWindow(true);
			}

			if (m_bOperVerify && m_bMonVerify)
			{
				m_btnVerify1.SetIcon(IDI_TICK);
				m_btnVerify2.SetIcon(IDI_TICK);
				GetDlgItem(IDC_BTN_PTSET_MODIFY) ->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_PTSET_ZONE_MODIFY) ->EnableWindow(TRUE);
			}
			else
			{
				m_btnVerify1.SetIcon(IDI_KEY1);
				m_btnVerify2.SetIcon(IDI_KEY1);
				GetDlgItem(IDC_BTN_PTSET_MODIFY) ->EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_PTSET_ZONE_MODIFY) ->EnableWindow(FALSE);
				m_sOperUser = _T("");
				m_sMonUser = _T("");
			}
		}

		//打印和风格设置按钮
		GetDlgItem(IDC_BTN_PTSET_PRINT) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSET_EXCEL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSET_STYLE) ->EnableWindow(TRUE);

		//下拉框
		GetDlgItem(IDC_CMB_PTSET_CPU) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_PTSET_SELZONE) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_PTSET_SRC) ->EnableWindow(TRUE);
		//停止动画
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//停止计时
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus > 0 && m_nCurrentStatus < 5)
	{
		//召唤或操作状态,除了自己其它的都不可用

		GetDlgItem(IDC_BTN_PTSET_CALL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSET_CALLZONE) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSET_MODIFY) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSET_ZONE_MODIFY) ->EnableWindow(FALSE);

		//打印和风格设置按钮
		GetDlgItem(IDC_BTN_PTSET_PRINT) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSET_EXCEL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSET_STYLE) ->EnableWindow(FALSE);

		//下拉框
		GetDlgItem(IDC_CMB_PTSET_CPU) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_PTSET_SELZONE) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_PTSET_SRC) ->EnableWindow(FALSE);

		//开始动画
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();

		//开始计时
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTSETTING, str);

		//让自己可用
		switch(m_nCurrentStatus)
		{
		case 1: //召唤定值
			SetDlgItemText(IDC_BTN_PTSET_CALL, CANCEL_CALL);
			GetDlgItem(IDC_BTN_PTSET_CALL) ->EnableWindow(TRUE);
			break;
		case 2: //召唤定值区号
			SetDlgItemText(IDC_BTN_PTSET_CALLZONE, CANCEL_CALL);
			GetDlgItem(IDC_BTN_PTSET_CALLZONE) ->EnableWindow(TRUE);
			break;
		case 3: //修改定值
			SetDlgItemText(IDC_BTN_PTSET_MODIFY, StringFromID(IDS_BTN_CANCEL_MODIFY));
			GetDlgItem(IDC_BTN_PTSET_MODIFY) ->EnableWindow(TRUE);
			break;
		case 4: //定值组切换
			SetDlgItemText(IDC_BTN_PTSET_ZONE_MODIFY, StringFromID(IDS_CANCEL_SWITCH));
			GetDlgItem(IDC_BTN_PTSET_ZONE_MODIFY) ->EnableWindow(TRUE);
			break;
		default:
			break;
		}
	}
}

/*************************************************************
 函 数 名：OnBtnPtsetModify()
 功能概要：定值修改
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8E0271
void CPTSetting::OnBtnPtsetModify() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_List.m_arrEdited.GetSize();
	CString str;
	str.Format("CPTSetting::OnBtnPtsetModify() 修改的值的数量为%d", nCount);
	WriteLog(str, XJ_LOG_LV3);
	if (m_pObj->m_bNewDevice && m_nCurrentStatus == 0)
	{
		if(m_List.m_arrEdited.GetSize() <= 0)
		{
			//无修改定值, 提示先修改
			AfxMessageBox( StringFromID(IDS_TIP_EDIT_SETTING));
			return;
		}
		m_nModifySettingOrModifyZone = 1;
		m_bOperTow = true;
		OnBtnPtsetCallzone();
	}
	else
	{
		//OnBtnPtsetModify1();
		OnBtnPtsetModify2();
	}
}


void CPTSetting::OnBtnPtsetModify1()
{
	m_bOperTow = false;
	m_nModifySettingOrModifyZone = 0;
	m_bChecking = FALSE;
	//响应按钮按下操作, 修改定值或取消修改
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	/*
	if (g_bVerify)
	{
		//验证权限,修改定值需要控制权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
		{
			CString strOut;
			//收集被修改的值
			CheckModifyData(m_arrModifyList, strOut);
			//回复修改前的值
			RevertModifyValue();
			return;
		}
	}
	*/

	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		CString strOut;
		//收集被修改的值
		CheckModifyData(m_arrModifyList, strOut);
		//回复修改前的值
		RevertModifyValue();
		return;
	}

	/*if (m_pObj->m_bNewDevice && m_sEditZone.IsEmpty())
	{
		AfxMessageBox("此装置编辑定值区号未知!");
		return;
	}*/
	
	/*if (m_pObj->m_bNewDevice && !m_sCurrentZone.CompareNoCase(m_sEditZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		AfxMessageBox("运行定值区号与编辑定值区号相同禁止修改定值!");
		return;
	}*/

	/*if (m_pObj->m_bNewDevice && atoi(m_sZone) == atoi(m_sCurrentZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		AfxMessageBox("禁止修改运行区定值!");
		return;
	}*/

	/*if (m_pObj->m_bNewDevice && atoi(m_sZone) != atoi(m_sEditZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		CString str;
		str.Format("选择的定值区号(%s)与编辑定值区号(%s)不一致,禁止修改!", m_sZone, m_sEditZone);
		AfxMessageBox(str);
		return;
	}*/


	if(m_nCurrentStatus == 0)
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		//正处于空闲状态, 开始修改定值
		//检查用户有否修改定值
		if(m_List.m_arrEdited.GetSize() <= 0)
		{
			//AfxMessageBox("CPTSetting::OnBtnPtsetModify1.DEBUG");
			//无修改定值, 提示先修改
			AfxMessageBox( StringFromID(IDS_TIP_EDIT_SETTING));
			return;
		}

		//组建要修改值的链表
		m_strOutPut = "";
		//记录新值
		CheckModifyData(m_arrModifyList, m_strOutPut);

		//要求用户再次确认
		/*if (AfxMessageBox(m_strOutPut, MB_OKCANCEL) != IDOK)
		{
			//回复修改前的值
			RevertModifyValue();
			UpdateControlsEnable();
			return;
		}*/

		m_nOperationNum = GetOperationNum();

		//操作员确认
		CDlgCheck dlg(this, 2);
		dlg.m_strModify = m_strOutPut;
		if(dlg.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//普通模式,要求操作员验证
				CDlgValidateID dlgID(2);
				dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
				if(dlgID.DoModal() == IDOK)
				{
					m_sOperUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以操作员身份验证成功:修改定值", m_sOperUser);
					WriteLog(str);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sOperUser, -1, OPER_SUCCESS,m_nOperationNum);
				}
				else
				{
					//无操作权限
					//回复修改前的值
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以操作员身份验证失败:修改定值", m_sOperUser);
					WriteLog(str, XJ_LOG_LV2);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sOperUser, -1, OPER_FAILD,m_nOperationNum);
					return;
				}
			}
		}
		else
		{
			//不同意修改
			//回复修改前的值
			RevertModifyValue();
			UpdateControlsEnable();
			return;
		}

		//监护员确认
		CDlgCheck dlg1(this, 1);
		dlg1.m_strModify = m_strOutPut;
		if(dlg1.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//普通模式,要求监护员验证
				CDlgValidateID dlgID(1);
				dlgID.m_strComUserID = m_sOperUser;
				dlgID.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
				if(dlgID.DoModal() == IDOK)
				{
					m_sMonUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以监护员身份验证成功:修改定值", m_sMonUser);
					WriteLog(str);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sMonUser, -1, OPER_SUCCESS,m_nOperationNum);
				}
				else
				{
					//无操作权限
					//回复修改前的值
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以监护员身份验证失败:修改定值", m_sMonUser);
					WriteLog(str, XJ_LOG_LV2);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sMonUser, -1, OPER_FAILD,m_nOperationNum);
					return;
				}
			}
		}
		else
		{
			//不同意修改
			//回复修改前的值
			RevertModifyValue();
			UpdateControlsEnable();
			return;
		}

		WriteLog("组建修改定值报文");
		//组建报文
		STTP_FULL_DATA sttpData;
		pApp->m_SttpEngine.BuildDataFor20051SettingModify(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone), 0, &m_arrModifyList);
		WriteLog("组建修改定值报文完毕");

		WriteLog("发送修改定值报文");
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("保护[%s]发送修改定值报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送修改定值报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
			//回复修改前的值
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			//m_btnModifySetting.EnableWindow(false);
			//m_btnModifyZone.EnableWindow(false);
			//m_btnVerify2.EnableWindow(false);
			m_sOperUser = _T("");
			m_sMonUser = _T("");
		}
		else
		{
			//成功,状态改为正在修改定值
			m_bChecking = TRUE;
			m_nCurrentStatus = 3;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nModifySettingTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			//增加人工操作
			//pApp->AddManOperator(FUNC_OPT_SETSETTING, m_pObj->m_sName);

			//改变控件可用性
			UpdateControlsEnable();

			CString str;
			str.Format("保护[%s]发送修改定值报文成功", m_pObj->m_sName);
			WriteLog(str);
		}
	}
	else if(m_nCurrentStatus == 3)
	{
		//正在修改定值状态
		//取消操作
		CancelCall();
	}
	else
	{
		//有其它操作正在进行
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
		CString strOut;
		//收集被修改的值
		CheckModifyData(m_arrModifyList, strOut);
		//回复修改前的值
		RevertModifyValue();
		return;
	}
}

void CPTSetting::OnBtnPtsetModify2()
{
	m_bOperTow = false;
	m_nModifySettingOrModifyZone = 0;
	m_bChecking = FALSE;
	//响应按钮按下操作, 修改定值或取消修改
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
	/*
	if (g_bVerify)
	{
		//验证权限,修改定值需要控制权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
		{
			CString strOut;
			//收集被修改的值
			CheckModifyData(m_arrModifyList, strOut);
			//回复修改前的值
			RevertModifyValue();
			return;
		}
	}
	*/

	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		CString strOut;
		//收集被修改的值
		CheckModifyData(m_arrModifyList, strOut);
		//回复修改前的值
		RevertModifyValue();
		return;
	}

	/*if (m_pObj->m_bNewDevice && m_sEditZone.IsEmpty())
	{
		AfxMessageBox("此装置编辑定值区号未知!");
		return;
	}*/
	
	/*if (m_pObj->m_bNewDevice && !m_sCurrentZone.CompareNoCase(m_sEditZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		AfxMessageBox("运行定值区号与编辑定值区号相同禁止修改定值!");
		return;
	}*/

	/*if (m_pObj->m_bNewDevice && atoi(m_sZone) == atoi(m_sCurrentZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		AfxMessageBox("禁止修改运行区定值!");
		return;
	}*/

	/*if (m_pObj->m_bNewDevice && atoi(m_sZone) != atoi(m_sEditZone))
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		CString str;
		str.Format("选择的定值区号(%s)与编辑定值区号(%s)不一致,禁止修改!", m_sZone, m_sEditZone);
		AfxMessageBox(str);
		return;
	}*/


	if(m_nCurrentStatus == 0)
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		//正处于空闲状态, 开始修改定值
		//检查用户有否修改定值
		if(m_List.m_arrEdited.GetSize() <= 0)
		{
			//AfxMessageBox("CPTSetting::OnBtnPtsetModify1.DEBUG");
			//无修改定值, 提示先修改
			AfxMessageBox( StringFromID(IDS_TIP_EDIT_SETTING));
			return;
		}

		//组建要修改值的链表
		m_strOutPut = "";
		//记录新值
		CheckModifyData(m_arrModifyList, m_strOutPut);

		//要求用户再次确认
		/*if (AfxMessageBox(m_strOutPut, MB_OKCANCEL) != IDOK)
		{
			//回复修改前的值
			RevertModifyValue();
			UpdateControlsEnable();
			return;
		}*/

		m_nOperationNum = GetOperationNum();

		//操作员确认
		CDlgCheckPro dlg(this, 2);
		dlg.m_strModify = m_strOutPut;
		dlg.m_arrModifyList.Copy(m_arrModifyList);
		dlg.m_sZone = m_sZone;
		dlg.m_sCPU = m_sCPU;
		if(dlg.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//普通模式,要求操作员验证
				CDlgValidateID dlgID(2, 1);
				dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
				if(dlgID.DoModal() == IDOK)
				{
					m_sOperUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					
					CXJPTSetStore *pStore = CXJPTSetStore::GetInstance();
					QPTSetStateTable *pState = pStore->GetState();
					pStore->ReLoadState();
					
					int nPTSetState = pState->GetStateID();

					CString str;
					CString sRecords;
					int nState = pApp->GetPTSetModState(PT_ZONE(), sRecords);
					if (XJ_OPER_HANGOUT != nPTSetState){
						CString sCurUserID = pState->GetOperUserID().constData();
						str.Format("用户[%s]正在对该装置进行定值修改操作作业 或者 该装置已取消了挂牌，请稍后再试", sCurUserID);
						AfxMessageBox(str, MB_OK | MB_ICONWARNING);

						RevertModifyValue(1, 1);
						return;
					}

					CXJPTSetStore::GetInstance()->GetState()->Next_PTSet_State_2(atoi(m_sCPU), atoi(m_sZone)
						, m_sOperUser.GetBuffer(0), m_arrModifyList, m_arrSetting);
				}
				else
				{
					//无操作权限
					//回复修改前的值

					CString sRecords;
					int nState = pApp->GetPTSetModState(PT_ZONE(), sRecords);
					if (1 != nState){
						CString sCurUserID = pApp->GetUserIDByState(2, sRecords);
						str.Format("用户[%s]正在对该装置进行定值修改操作作业 或者 该装置已取消了挂牌，请稍后再试", sCurUserID);
						AfxMessageBox(str, MB_OK | MB_ICONWARNING);
						
						RevertModifyValue(1, 1);
						m_nCurrentDetailStatus = 0;
						return;
					}
					
					RevertModifyValue();
					str.Format("用户%s以操作员身份验证失败:修改定值", m_sOperUser);
					WriteLog(str, XJ_LOG_LV2);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sOperUser, -1, OPER_FAILD,m_nOperationNum);
					
					//pApp->RevertPTSetModState(1);
					m_nCurrentDetailStatus = 0;

					return;
				}
			}
		}
		else
		{
			//不同意修改
			//回复修改前的值
			UpdateControlsEnable();

			CString str;
			CString sRecords;
			int nState = pApp->GetPTSetModState(PT_ZONE(), sRecords);
			if (1 != nState){
				CString sCurUserID = pApp->GetUserIDByState(2, sRecords);
				str.Format("用户[%s]正在对该装置进行定值修改操作作业 或者 该装置已取消了挂牌，请稍后再试", sCurUserID);
				AfxMessageBox(str, MB_OK | MB_ICONWARNING);
				
				RevertModifyValue(1, 1);
				m_nCurrentDetailStatus = 0;
				return;
			}
			
			RevertModifyValue();
			//pApp->RevertPTSetModState(1);
			m_nCurrentDetailStatus = 0;

			return;
		}

		//监护员确认
		CDlgCheckPro dlg1(this, 1);
		dlg1.m_strModify = m_strOutPut;
		dlg1.m_arrModifyList.Copy(m_arrModifyList);
		dlg1.m_sZone = m_sZone;
		dlg1.m_sCPU = m_sCPU;
		if(dlg1.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//普通模式,要求监护员验证
				CDlgValidateID dlgID(1, 1);
				dlgID.m_strComUserID = m_sOperUser;
				dlgID.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
				if(dlgID.DoModal() == IDOK)
				{
					m_sMonUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以监护员身份验证成功:修改定值", m_sMonUser);
					WriteLog(str);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sMonUser, -1, OPER_SUCCESS,m_nOperationNum);
				
				    // 监护人员确认后修改状态机
					//pApp->ModifyDZModState(3, m_sMonUser, m_pObj);
					PT_ZONE zone;
					pApp->GetPTSetModState(zone);
					pApp->NextPTSetModState(3, zone, m_sMonUser);
				}
				else
				{
					//无操作权限
					//回复修改前的值
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以监护员身份验证失败:修改定值", m_sMonUser);
					WriteLog(str, XJ_LOG_LV2);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sMonUser, -1, OPER_FAILD,m_nOperationNum);
					
					pApp->RevertPTSetModState(1);
					m_nCurrentDetailStatus = 0;
					return;
				}
			}
		}
		else
		{
			//不同意修改
			//回复修改前的值
			RevertModifyValue();
			UpdateControlsEnable();
			pApp->RevertPTSetModState(1);
			m_nCurrentDetailStatus = 0;
			
			return;
		}
	}
	else if(m_nCurrentStatus == 3)
	{
		//正在修改定值状态
		//取消操作
		CancelCall();
	}
	else
	{
		//有其它操作正在进行
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
		CString strOut;
		//收集被修改的值
		CheckModifyData(m_arrModifyList, strOut);
		//回复修改前的值
		RevertModifyValue();
		return;
	}
}

BOOL CPTSetting::ExcutePTSet()
{
	BOOL bReturn = TRUE;

	if (NULL == m_pObj){
		m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
		m_nCurrentDetailStatus = 0;
		return FALSE;
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	WriteLog("组建修改定值报文");
	//组建报文
	STTP_FULL_DATA sttpData;
	pApp->m_SttpEngine.BuildDataFor20051SettingModify(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone), 0, &m_arrModifyList);
	WriteLog("组建修改定值报文完毕");
	
	WriteLog("发送修改定值报文");
	//发送报文
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//判断是否成功
	if(nReturn != 1)
	{
		//失败
		if(nReturn == -1)
		{
			CString str;
			str.Format("保护[%s]发送修改定值报文失败,原因为连接中断", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("保护[%s]发送修改定值报文失败,原因为缓存已满", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
		AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		//回复修改前的值
		RevertModifyValue();

		m_bMonVerify = false;
		m_bOperVerify = false;
		//m_btnModifySetting.EnableWindow(false);
		//m_btnModifyZone.EnableWindow(false);
		//m_btnVerify2.EnableWindow(false);
		m_sOperUser = _T("");
		m_sMonUser = _T("");

		pApp->RevertPTSetModState(1);
		m_nCurrentDetailStatus = 0;
		m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);

		bReturn = FALSE;
	}
	else
	{
		//成功,状态改为正在修改定值
		m_bChecking = TRUE;
		m_nCurrentStatus = 3;
		
		//启动定时器
		CDataEngine* pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nModifySettingTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//增加人工操作
		//pApp->AddManOperator(FUNC_OPT_SETSETTING, m_pObj->m_sName);
		
		//改变控件可用性
		UpdateControlsEnable();
		
		CString str;
		str.Format("保护[%s]发送修改定值报文成功", m_pObj->m_sName);
		WriteLog(str);
		
		//AfxMessageBox(str);
	}

	return bReturn;
}

/*************************************************************
 函 数 OnBtnViewProg()
 功能概要：定值修改进度查看
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8E0271
void CPTSetting::OnBtnViewProg() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	CCJTabCtrlBar &bar = pMainFrame->m_wndGlobalMsgBar;
	
	CRect rcRect;
	GetWindowRect(&rcRect);
	
	if (m_bAlreadyShowOnce){
		bar.GetWindowRect(rcRect);
		m_pointPTSetModView.x = rcRect.left - 2;
		m_pointPTSetModView.y = rcRect.top - 18;

	}else{
		LONG w = 600;
		LONG h = 220;

		m_pointPTSetModView.x = rcRect.right - w;
		m_pointPTSetModView.y = rcRect.bottom - h;
		m_bAlreadyShowOnce = true;
	}
	
	bar.ShowWindow(bar.IsVisible() ? SW_HIDE : SW_SHOW);
	pMainFrame->FloatControlBar(&bar, m_pointPTSetModView, CBRS_ALIGN_LEFT);

	
	CString str;
	str.Format("x: %d; y: %d", m_pointPTSetModView.x, m_pointPTSetModView.y);
	//AfxMessageBox(str);

}

/*************************************************************
 函 数 OnBtnViewHis()
 功能概要：定值修改历史查看
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8E0271
void CPTSetting::OnBtnViewHis() 
{
	// TODO: Add your control notification handler code here
	CDlgOperHis dlg;
	dlg.m_nType = 1;
	dlg.m_pObj = m_pObj;
	dlg.DoModal();
}

/*************************************************************
 函 数 名：CheckModifyData()
 功能概要：组建要修改的定值列表
 返 回 值: void
 参    数：param1	要修改的定值列表
		   Param2	输出文字
**************************************************************/
//##ModelId=49B87B8D0119
void CPTSetting::CheckModifyData( MODIFY_LIST& modifyList, CString& strOut )
{
	//清空修改值列表
	ClearModifyData();

	//定值值列
	CPoint ptItem;
	CString strText, strPickList;
	strOut = StringFromID(IDS_VERIFY_MODIFYSETTING_CONFIRM)+":\r\n";
	CString str;
	str.Format("%s:%s\r\n", StringFromID(IDS_SECPROP_ZONE),m_sZone);
	strOut += str;
	PT_SETTING *pts;
	for (int i = 0; i < m_List.m_arrEdited.GetSize(); i ++)
	{
		ptItem = m_List.m_arrEdited.GetAt(i);
		if (ptItem.y != m_nNewValueIndex)
		{
			continue;
		}

		STTP_DATA * sttpData = new STTP_DATA;

		strText = m_List.GetItemText(ptItem.x, ptItem.y);
		
		//去掉"*"
		strText = strText.Left(strText.Find("*", 0));
		if (strText.IsEmpty())
		{
			continue;
		}
		
		//取得该项定值的定义
		pts = (PT_SETTING *)m_arrSetting.GetAt(ptItem.x);
		strPickList = pts->VPickList;
		strPickList.TrimLeft();
		strPickList.TrimRight();
		
		//ID
		sttpData->id = atoi(pts->ID);
		// CPU_CODE
		sttpData->nCpu = atoi(m_sCPU);

		//输出字段
		strOut += pts->ID + "\t";
		CString strName = _T("");
		int z = pts->Name.Find(",", 0);
		if (z != -1)
		{
			strName = pts->Name.Left(z);
		}
		else
		{
			strName = pts->Name;
		}
		strOut += strName + " "+StringFromID(IDS_COMMON_FROM)+" ";
		strOut += GetDisplayValue(pts, pts->Value) + "\t"+StringFromID(IDS_COMMON_CHANGETO)+"-> ";
		strOut += strText + "\r\n";

		CString strTmp;
		strTmp.Format("%s@%s", strName, GetDisplayValue(pts, pts->Value)); // name@value
		sttpData->str_reserved = strTmp; // 原值
		
		if (strPickList.IsEmpty())
		{//strText可以直接写入报文
			//去除可能有的单位
			pts->Unit.TrimRight();
			if (!pts->Unit.IsEmpty())
			{
				int nSub = strText.Find(pts->Unit, 0);
				if (nSub != -1)
					strText = strText.Left(nSub);
			}
		}
		else
		{//strText必须先转换
			//定值的值
			strText = GetIDFromValue(pts->VPickList, strText);
		}
		if(pts->ntimeMStoS == 1)
		{
			double fValue = atof(strText);
			fValue *= 1000;
			//int nValue = (int)fValue;
			strText.Format("%.0f", fValue);
		}
		//定值的值
//		sttpData->str_value = MSToXJ(strText);
		sttpData->str_value = strText;

		//加入到链表
		modifyList.Add(sttpData);
	}
}

/*************************************************************
 函 数 名：GetTranslatedValue()
 功能概要：取得经过翻译VPickList后的Value
 返 回 值: 经过翻译VPickList后的Value
 参    数：param1	定值定义
		   Param2	要翻译的值
**************************************************************/
//##ModelId=49B87B8D0148
CString CPTSetting::GetTranslatedValue( PT_SETTING *pst,const CString strValue )
{
	//根据VPickList填充Value内容,VPickList不空时，Value值的解释根据VPivkList确定 
	//如VPickList="0:on;1:off" 而 Value=1 时，strValue为"off"
	//如VPickList为空，而Value=1，Unit="s" 则strValue = "1s"  
	CString strTemp;
	CString strUnit		= pst->Unit;
	CString strVPickList= pst->VPickList;
	CString strReturn	= strValue;//pst->Value;
	
	//去掉多余的空格
	strUnit.TrimLeft();
	strUnit.TrimRight();
	strReturn.TrimLeft();
	strReturn.TrimRight();
	strVPickList.TrimLeft();
	strVPickList.TrimRight();
	if (strVPickList.IsEmpty())
	{
	}
	else
	{
		if (CheckVPicklist(strVPickList))
		{
			int nFenhao =0;	//分号的位置
			int nMaohao =0; //冒号的位置
			BOOL bFound = FALSE;
			
			//只要strVPickList不空，则劈开分号之间的字符串，挨个检查冒号左边的值是否与strReturn相同
			//如果相同则stValue用冒号右边的字串取代
			while (!strVPickList.IsEmpty()) 	 
			{
				nFenhao = strVPickList.Find(';', 0);
				if(nFenhao>0)
					strTemp = strVPickList.Left(nFenhao);
				else
					strTemp = strVPickList;
				
				nMaohao = strTemp.Find(':', 0);			 
				if (nMaohao>0)
				{
					if (strTemp.Left(nMaohao) == strReturn)
					{
						strTemp.Delete(0, nMaohao+1);
						strReturn = strTemp;
						bFound = TRUE;
						break;
					}
				}
				
				if (nFenhao>0)
					strVPickList.Delete(0, nFenhao+1);
				else
					break;
			}
		}
		else
		{
			CString str;
			str.Format("CPTSetting::CheckVPicklist,查询失败,原因为数据库vpicklist为%s,格式不对",strVPickList);
			WriteLog(str, XJ_LOG_LV3);
		}
	}
	return strReturn;
}

/*************************************************************
 函 数 名：ClearModifyData()
 功能概要：清除修改值链表
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8D0167
void CPTSetting::ClearModifyData()
{
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		delete sttpData;
	}

	m_arrModifyList.RemoveAll();
}

/*************************************************************
 函 数 名：OnSTTP20052()
 功能概要：定值修改预校通知
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8F004E
void CPTSetting::OnSTTP20052( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//检查是处于修改定值状态
	if(m_nCurrentStatus != 3)
	{
		//非修改定值状态
		return;
	}

	//取得报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;

	//检查是否20052报文
	if(pSttpData->sttp_head.uMsgID != 20052)
	{
		//错误报文
		CString str;
		str.Format("CPTSetting::OnSTTP20052 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20052 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	CString strMsg;
	strMsg.Format("%d,%d,%d,%d,%d", 
		pSttpData->sttp_body.nStatus, 
		XJ_OPER_MODIFYSETTING_PRECORRECTION,
		pSttpData->sttp_body.nCpu, 
		pSttpData->sttp_body.nZone, 
		m_arrModifyList.GetSize());
	
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		CString strTemp = sttpData->str_value.c_str();
		if(g_OperTableType == 0)
			strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
		else if(g_OperTableType == 1)
		{
			CString sOldValue = FindNewValue(sttpData->id);
			strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
		}
	}
	
	pApp->AddNewManOperator("定值修改预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_PRECORRECTION, pSttpData->sttp_body.nStatus, m_nOperationNum);

	//关闭预校超时  
	KillTimer(m_nTimer);


	CString strOutput = m_strOutPut;

	//请求确认
	if(pSttpData->sttp_body.nStatus == 0)
	{
		WriteLog("定值修改预校成功,开始下发执行命令");
		//成功
		strOutput += StringFromID(IDS_EXECUTE_MODIFYSETTING_CONFIRM);
		/*CDlgCheckPro dlg(this,2);
		dlg.m_strModify = strOutput;
		dlg.m_arrModifyList.Copy(m_arrModifyList);
		dlg.m_sZone = m_sZone;
		dlg.m_sCPU = m_sCPU;
		//提示用户操作结果
		if (dlg.DoModal() == IDOK)*/
		{
			//发送执行报文
			//组建报文
			STTP_FULL_DATA sttpData;
			pApp->m_SttpEngine.BuildDataFor20053SettingModifyExe(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone), 0, &m_arrModifyList);
			
			//发送报文
			int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			//判断是否成功
			if(nReturn != 1)
			{
				//失败
				if(nReturn == -1)
				{
					CString str;
					str.Format("保护[%s]发送修改定值执行报文失败,原因为连接中断", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				else if(nReturn == -2)
				{
					CString str;
					str.Format("保护[%s]发送修改定值执行报文失败,原因为缓存已满", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}

				//修改状态
				m_nCurrentStatus = 0;
				UpdateControlsEnable();
				//回复修改前的值
				RevertModifyValue();

				AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
				
				// 重启定时
				KillTimer(m_nPTSetTimer);
				m_nCurrentDetailStatus = 0;
				m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);

				return;
			}
			
			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nModifySettingTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		}
/*		else
		{
			//放弃执行
			//修改状态为空闲
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
			//回复修改前的值
			RevertModifyValue();

			// 重启定时
			KillTimer(m_nPTSetTimer);
			m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
		}*/
	}
	else
	{
		WriteLog("定值修改预校失败");
		m_bMonVerify = false;
		m_bOperVerify = false;
		//修改状态为空闲
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//回复修改前的值
		RevertModifyValue();
		//失败
		strOutput += "\n";
		strOutput += StringFromID(IDS_VERIFY_MODIFYSETTING_FAIL);
		//提示用户操作结果
		AfxMessageBox(strOutput, MB_OK);
		
		pApp->RevertPTSetModState(1);
		m_nCurrentDetailStatus = 0;

		// 重启定时
		KillTimer(m_nPTSetTimer);
		m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
	}

	m_bChecking = FALSE;
}

/*************************************************************
 函 数 名：GetSettingObject()
 功能概要：根据定值ID找到定值对象
 返 回 值: 定值对象
 参    数：param1	定值ID
**************************************************************/
//##ModelId=49B87B8D0186
PT_SETTING * CPTSetting::GetSettingObject( CString strID )
{
	strID.TrimLeft();
	strID.TrimRight();

	PT_SETTING *p = NULL;
	for (int i = 0; i < m_arrSetting.GetSize(); i ++)
	{
		p = m_arrSetting.GetAt(i);
		
		if (p->ID == strID)
			return p;
	}
	
	return NULL; 
}

/*************************************************************
 函 数 名：GetIDFromValue()
 功能概要：根据值查找值的定义
 返 回 值: 值定义
 参    数：param1	值说明
		   Param2	值
**************************************************************/
//##ModelId=49B87B8D01A5
CString CPTSetting::GetIDFromValue( CString strPickList, CString strValue )
{
	CString strID, strTmp;
	strID = strValue;
	int nPos1 = 0, nPos2 = 0;
	
	while (1) 
	{
		nPos1 = strPickList.Find(":", nPos2);
		nPos2 = strPickList.Find(";", nPos1);
		if (nPos1 == -1)
		{
			//没找到
			return strID;
		}
		
		if (nPos2 == -1)
		{
			nPos2 = strPickList.GetLength();
		}
		
		strTmp = strPickList.Mid(nPos1 + 1, nPos2 - nPos1 - 1);
		if (strValue == strTmp)
		{//找到了
			strPickList = strPickList.Left(nPos1);
			nPos2 = strPickList.ReverseFind(';');
			if (nPos2 != -1)
				strID = strPickList.Right(strPickList.GetLength() - nPos2 - 1);
			else
				strID = strPickList.Left(nPos1);
			
			break;
		}
	}
	
	return strID;
}

/*************************************************************
 函 数 名：OnSTTP20054()
 功能概要：响应保护定值修改执行通知(20054)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8F006D
void CPTSetting::OnSTTP20054( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//检查是处于修改定值状态
	if(m_nCurrentStatus != 3)
	{
		//非修改定值状态
		return;
	}
	
	//取得报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//检查是否20054报文
	if(pSttpData->sttp_head.uMsgID != 20054)
	{
		//错误报文
		CString str;
		str.Format("CPTSetting::OnSTTP20054 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20054 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}


	CString strMsg;
	strMsg.Format("%d,%d,%d,%d,%d", pSttpData->sttp_body.nStatus, XJ_OPER_MODIFYSETTING_EXECUTE,pSttpData->sttp_body.nCpu, pSttpData->sttp_body.nZone, m_arrModifyList.GetSize());
	WriteLog(strMsg);
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		CString strTemp = sttpData->str_value.c_str();
		if(g_OperTableType == 0)
			strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
		else if(g_OperTableType == 1)
		{
			CString sOldValue = FindNewValue(sttpData->id);
			strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
		}
	}
	WriteLog(strMsg);

	pApp->AddNewManOperator("定值修改执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_EXECUTE, pSttpData->sttp_body.nStatus, m_nOperationNum);
	
	//关闭定时器
	KillTimer(m_nTimer);

	//修改状态
	m_nCurrentStatus = 0;
	UpdateControlsEnable();

	//处理报文
	if(pSttpData->sttp_body.nStatus == 0)
	{
		//成功
		//清除修改标记
		m_List.ClearEdited();

		// 修改状态机
		//pApp->ModifyDZModState(5, m_sOperUser, m_pObj);
		PT_ZONE zone;
		pApp->GetPTSetModState(zone);
		pApp->NextPTSetModState(5, zone, m_sOperUser);
		pApp->SetRevertModifyValueFlag(0);

		//提示
		AfxMessageBox( StringFromID(IDS_EXECUTE_MODIFYSETTING_SUCCESS), MB_OK|MB_ICONINFORMATION);
		m_nCurrentDetailStatus = 0;
		m_btnModifySetting.EnableWindow(FALSE);
		m_btnModifyZone.EnableWindow(FALSE);

		// 保持修改后的一次定值到 定义表
		//CXJPTSetStore::GetInstance()->SaveRecallToDB(m_sCPU, m_pObj->m_sID, m_arrSetting);
	}
	else
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		//失败
		//回复修改前的值
		RevertModifyValue();
		AfxMessageBox( StringFromID(IDS_EXECUTE_MODIFYSETTING_FAIL));

		// 修改状态机
		pApp->RevertPTSetModState(1);
		m_nCurrentDetailStatus = 0;
	}

	m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
}

/*************************************************************
 函 数 名：OnBtnPtsetZoneModify()
 功能概要：定值组切换
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8E0290
void CPTSetting::OnBtnPtsetZoneModify() 
{
		
	// TODO: Add your control notification handler code here
	int nCount = m_List.m_arrEdited.GetSize();
	CString str;
	str.Format("CPTSetting::OnBtnPtsetZoneModify() 修改的值的数量为%d", nCount);
	WriteLog(str, XJ_LOG_LV3);
	if (m_pObj->m_bNewDevice && m_nCurrentStatus == 0)
	{
		if(m_ListZone.m_arrEdited.GetSize() == 0)
		{
			AfxMessageBox(StringFromID(IDS_TIP_EDIT_ZONE));
			return;
		}
		m_nModifySettingOrModifyZone = 2;
		m_bOperTow = true;
		OnBtnPtsetCallzone();
	}
	else
	{
		OnBtnPtsetZoneModify1();
	}
}

void CPTSetting::OnBtnPtsetZoneModify1()
{
	m_bOperTow = false;
	m_nModifySettingOrModifyZone = 0;
	m_bChecking = FALSE;

	if(m_pObj == NULL)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	/*
	if (g_bVerify)
	{
		//验证权限,修改定值需要控制权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
			return;
	}
	*/
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}

	if(m_nCurrentStatus == 0)
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		//空闲状态
		//更新控件绑定的变量值
		UpdateData(TRUE);

		if(m_ListZone.m_arrEdited.GetSize() == 0)
		{
			AfxMessageBox( StringFromID(IDS_TIP_SELECT_ZONE));
			return;
		}
		CPoint ptItem = m_ListZone.m_arrEdited.GetAt(0);
		CString strText = m_ListZone.GetItemText(ptItem.x, ptItem.y);
		strText = strText.Left(strText.Find("*", 0));

		m_sModifyZone = strText;

		m_nOperationNum = GetOperationNum();

		//要求用户确认
		CString str;
		str.Format("%s:%s, %s:%s, %s %s %s %s %s?", StringFromID(IDS_MODEL_RELAY),m_pObj->m_sName, StringFromID(IDS_MODEL_CPU),m_sCPU, 
			StringFromID(IDS_SECPROP_ZONE), StringFromID(IDS_COMMON_FROM), m_sOldZoneValue, StringFromID(IDS_COMMON_CHANGETO),strText);
		//m_sOldZoneValue = _T("");
		/*if (AfxMessageBox(str, MB_OKCANCEL) != IDOK)
		{
			UpdateControlsEnable();
			return;
		}*/

		//操作员确认
		CDlgCheck dlg(this, 2);
		dlg.m_strModify = str;
		if(dlg.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//普通模式,要求操作员验证
				CDlgValidateID dlgID(2);
				dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
				if(dlgID.DoModal() == IDOK)
				{
					m_sOperUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以操作员身份验证成功:定值区切换", m_sOperUser);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sOperUser, -1, OPER_SUCCESS, m_nOperationNum);
				}
				else
				{
					//无操作权限
					//回复修改前的值
					RevertModifyValue(2);
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以操作员身份验证失败:定值区切换", m_sOperUser);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sOperUser, -1, OPER_FAILD, m_nOperationNum);
					return;
				}
			}			
		}
		else
		{
			//不同意修改
			//回复修改前的值
			RevertModifyValue(2);
			UpdateControlsEnable();
			return;
		}

		//监护员确认
		CDlgCheck dlg1(this, 1);
		dlg1.m_strModify = str;
		if(dlg1.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//普通模式,要求监护员验证
				CDlgValidateID dlgID(1);
				dlgID.m_strComUserID = m_sOperUser;
				dlgID.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
				if(dlgID.DoModal() == IDOK)
				{
					m_sMonUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以监护员身份验证成功:定值区切换", m_sMonUser);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sMonUser, -1, OPER_SUCCESS, m_nOperationNum);
				}
				else
				{
					//无操作权限
					//回复修改前的值
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以监护员身份验证失败:定值区切换", m_sMonUser);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sMonUser, -1, OPER_FAILD, m_nOperationNum);
					return;
				}
			}
		}
		else
		{
			//不同意修改
			//回复修改前的值
			RevertModifyValue(2);
			UpdateControlsEnable();
			return;
		}

		//组建预校报文
		STTP_FULL_DATA sttpData;
		if (m_pObj->m_bNewDevice)
		{
			bool bRun = false;
			bool bEdit = false;
			for (int j = 0; j < m_ListZone.GetItemCount(); j++)
			{
				CString str = m_ListZone.GetItemText(j, 6);
				if (str.Find("*") != -1)
				{
					CString strItem = m_ListZone.GetItemText(j, 0);
					if (strItem.Find(StringFromID(IDS_COMMON_RUN)) != -1)
					{
						bRun = true;
					}
					else if (strItem.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
					{
						bEdit = true;
					}
				}
			}

			int nId = -1;
			CSecCPU* pCpu = NULL;
			for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
			{
				pCpu = m_pObj->m_arrCPU[k];
				if (pCpu->code == atoi(m_sCPU))
				{
					break;
				}
			}
			MYASSERT(pCpu);
			CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;
			for (int i = 0;  i < arrZONE.GetSize(); i++)
			{
				PT_ZONE * ptz = arrZONE[i];
				if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1 && bEdit)
				{
					nId = ptz->code;
					break;
				}
				else if (ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) != -1 && bRun)
				{
					nId = ptz->code;
					break;
				}
			}

			if (nId == -1)
			{
				AfxMessageBox( StringFromID(IDS_TIP_ZONEID_ERROR));
				return;
			}
			pApp->m_SttpEngine.BuildDataFor20169SettingChange(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(strText), nId);
		}
		else
		{
			pApp->m_SttpEngine.BuildDataFor20047SettingChange(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(strText));
		}
		
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("保护[%s]发送定值组切换报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送定值组切换报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
			//回复修改前的值
			RevertModifyValue(2);
			m_bMonVerify = false;
			m_bOperVerify = false;
			//m_btnModifySetting.EnableWindow(false);
			//m_btnModifyZone.EnableWindow(false);
			//m_btnVerify2.EnableWindow(false);
			m_sOperUser = _T("");
			m_sMonUser = _T("");
		}
		else
		{
			//成功,状态改为正在修改定值组
			m_bChecking = TRUE;

			m_nCurrentStatus = 4;
			UpdateControlsEnable();

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nChangeZoneTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			//增加人工操作日志
			//pApp->AddManOperator(FUNC_OPT_CHANGEZONE, m_pObj->m_sName);

			//修改控件可用性
			UpdateControlsEnable();

			CString str;
			str.Format("保护[%s]发送定值组切换报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 4)
	{
		//正在修改定值区, 取消
		CancelCall();
	}
	else
	{
		//正在其它操作
		return;
	}
} 

/*************************************************************
 函 数 名：OnSTTP20048()
 功能概要：响应定值组切换预校通知(20048)
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F008C
void CPTSetting::OnSTTP20048( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 4)
	{
		//没有处于定值组切换的状态,不处理报文
		return;
	}
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20048报文
	if(pSttpData->sttp_head.uMsgID != 20048)
	{
		//错误报文
		CString str;
		str.Format("CPTSetting::OnSTTP20048 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20048 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//关闭预校超时  
	KillTimer(m_nTimer);
	
	CString strMsg;
	if(g_OperTableType == 0)
		strMsg.Format("%d,%d,%d,%d", pSttpData->sttp_body.nStatus, XJ_OPER_CHANGEZONE_PRECORRECTION, pSttpData->sttp_body.nCpu, pSttpData->sttp_body.nZone);
	else if(g_OperTableType == 1)	
		strMsg.Format("%d,%d,%d,,1,,%d,%s", pSttpData->sttp_body.nStatus, XJ_OPER_CHANGEZONE_PRECORRECTION, pSttpData->sttp_body.nCpu, pSttpData->sttp_body.nZone, m_sCurrentZone);
	pApp->AddNewManOperator("定值区切换预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_PRECORRECTION, pSttpData->sttp_body.nStatus, m_nOperationNum);
	
	
	//处理报文
	if(pSttpData->sttp_body.nStatus == 0)
	{
		//成功
		CString str;
		str.Format("%s! %s:%s %s:(%d) %s:(%d)", StringFromID(IDS_VERIFY_SUCCESS), StringFromID(IDS_MODEL_RELAY),strID, StringFromID(IDS_MODEL_CPU),pSttpData->sttp_body.nCpu, 
			StringFromID(IDS_ZONE_SWITCHTO),pSttpData->sttp_body.nZone);

		CDlgCheck dlg(this,3);
		dlg.m_strModify = str;
		//提示用户操作结果
		if (dlg.DoModal() == IDOK)
		{
			//组建执行报文
			STTP_FULL_DATA sttpData;
			pApp->m_SttpEngine.BuildDataFor20055SettingChangeExe(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(/*m_sZone*/m_sModifyZone));

			//发送报文
			int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			//判断是否成功
			if(nReturn != 1)
			{
				//失败
				if(nReturn == -1)
				{
					CString str;
					str.Format("保护[%s]发送定值切换执行报文失败,原因为连接中断", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				else if(nReturn == -2)
				{
					CString str;
					str.Format("保护[%s]发送定值切换执行报文失败,原因为缓存已满", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));

				//改变状态
				//回复修改前的值
				RevertModifyValue(2);
				m_nCurrentStatus = 0;
				UpdateControlsEnable();
				return;
			}
			//开启修改超时定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nChangeZoneTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		}
		else
		{
			//修改状态为空闲
			//回复修改前的值
			RevertModifyValue(2);
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
		}
	}
	else
	{
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		//失败
		//回复修改前的值
		RevertModifyValue(2);
		//修改状态为空闲
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		AfxMessageBox( StringFromID(IDS_VERIFY_FAIL));
	}

	m_bChecking = FALSE;
}


void CPTSetting::OnSTTP20170( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 4)
	{
		WriteLog("CPTSetting::OnSTTP20170 没有处于定值组切换的状态,不处理报文", XJ_LOG_LV3);
		//没有处于定值组切换的状态,不处理报文
		return;
	}

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20048报文
	if(pSttpData->sttp_head.uMsgID != 20170)
	{
		//错误报文
		CString str;
		str.Format("CPTSetting::OnSTTP20170 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20170 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//关闭预校超时  
	KillTimer(m_nTimer);
	

	CString strMsg;
	if(g_OperTableType == 0)
		strMsg.Format("%d,%d,%s,%s", OPER_SUCCESS, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone);
	else if(g_OperTableType == 1)	
		strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_SUCCESS, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone, m_sCurrentZone);
	pApp->AddNewManOperator("定值区切换预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_PRECORRECTION, OPER_SUCCESS, m_nOperationNum);
	//处理报文
	if(pSttpData->sttp_body.nStatus == 0)
	{
	
		//成功
		CPoint ptItem = m_ListZone.m_arrEdited.GetAt(0);
		CString strText = m_ListZone.GetItemText(ptItem.x, ptItem.y);
		strText = strText.Left(strText.Find("*", 0));
		CString str;
		str.Format("%s!%s:%s %s:(%d) %s:(%d)", StringFromID(IDS_VERIFY_SUCCESS), StringFromID(IDS_MODEL_RELAY),strID, StringFromID(IDS_MODEL_CPU),pSttpData->sttp_body.nCpu, 
			StringFromID(IDS_ZONE_SWITCHTO),pSttpData->sttp_body.nZone);
		if (AfxMessageBox(str, MB_OKCANCEL) == IDOK)
		{
			//组建执行报文
			STTP_FULL_DATA sttpData;
			int nId = -1;

			CSecCPU* pCpu = NULL;
			for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
			{
				pCpu = m_pObj->m_arrCPU[k];
				if (pCpu->code == pSttpData->sttp_body.nCpu)
				{
					break;
				}
			}
			MYASSERT(pCpu);
			
			CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;
			for (int i = 0;  i < arrZONE.GetSize(); i++)
			{
				PT_ZONE * ptz = arrZONE[i];
				if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
				{
					nId = ptz->code;
					break;
				}
			}
			
			if (nId == -1)
			{
				AfxMessageBox(StringFromID(IDS_TIP_ZONEID_ERROR));
				return;
			}

			pApp->m_SttpEngine.BuildDataFor20171SettingChangeExe(sttpData, m_pObj->m_sID, atoi(m_sCPU), atoi(strText), nId);

			//发送报文
			int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			//判断是否成功
			if(nReturn != 1)
			{
				//失败
				if(nReturn == -1)
				{
					CString str;
					str.Format("保护[%s]发送定值切换执行报文失败,原因为连接中断", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				else if(nReturn == -2)
				{
					CString str;
					str.Format("保护[%s]发送定值切换执行报文失败,原因为缓存已满", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));

				//改变状态
				m_nCurrentStatus = 0;
				UpdateControlsEnable();
				return;
			}
			//开启修改超时定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nChangeZoneTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		}
		else
		{
			//修改状态为空闲
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
		}
	}
	else
	{
		//失败
		//修改状态为空闲
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		AfxMessageBox(StringFromID(IDS_VERIFY_SWITCHZONE_FAIL));
	}
}

/*************************************************************
 函 数 名：OnSTTP20056()
 功能概要：响应保护定值切换执行通知(20056)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8F00AB
void CPTSetting::OnSTTP20056( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//检查是处于定值切换状态
	if(m_nCurrentStatus != 4)
	{
		//非定值切换状态
		return;
	}
	
	//取得报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//检查是否20054报文
	if(pSttpData->sttp_head.uMsgID != 20056)
	{
		//错误报文
		CString str;
		str.Format("CPTSetting::OnSTTP20056 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20056 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}


	CString strMsg;
	if(g_OperTableType == 0)
		strMsg.Format("%d,%d,%d,%d", pSttpData->sttp_body.nStatus, XJ_OPER_CHANGEZONE_EXECUTE, pSttpData->sttp_body.nCpu, pSttpData->sttp_body.nZone);
	else if(g_OperTableType == 1)
		strMsg.Format("%d,%d,%d,,1,,%d,%s", pSttpData->sttp_body.nStatus, XJ_OPER_CHANGEZONE_EXECUTE, pSttpData->sttp_body.nCpu, pSttpData->sttp_body.nZone, m_sCurrentZone);
	pApp->AddNewManOperator("定值区切换执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_EXECUTE, pSttpData->sttp_body.nStatus, m_nOperationNum);

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	if(pSttpData->sttp_body.nStatus == 0)
	{
		m_sCurrentZone.Format("%d", pSttpData->sttp_body.nZone);
		m_ListZone.SetItemText(0, 6, m_sCurrentZone);
		UpdateData(FALSE);
		m_ListZone.ClearEdited();
		//成功
		AfxMessageBox(StringFromID(IDS_EXECUTE_SWITCHZONE_SUCCESS));
	}
	else
	{
		//失败
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		//回复修改前的值
		RevertModifyValue(2);
		AfxMessageBox(StringFromID(IDS_EXECUTE_SWITCHZONE_FAIL));
	}

	//修改状态
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
}

void CPTSetting::OnSTTP20172( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//检查是处于定值切换状态
	if(m_nCurrentStatus != 4)
	{
		WriteLog("CPTSetting::OnSTTP20172 非定值切换状态", XJ_LOG_LV3);
		//非定值切换状态
		return;
	}
	
	//取得报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//检查是否20172报文
	if(pSttpData->sttp_head.uMsgID != 20172)
	{
		//错误报文
		CString str;
		str.Format("CPTSetting::OnSTTP20172 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20172 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	CSecCPU* pCpu = NULL;
	for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
	{
		pCpu = m_pObj->m_arrCPU[k];
		if (pCpu->code == pSttpData->sttp_body.nCpu)
		{
			break;
		}
	}
	MYASSERT(pCpu);
	
	CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;
	int nCount = pSttpData->sttp_body.variant_member.value_data.size();
	for(int i = 0; i < nCount; i++)
	{
		int id;
		id = pSttpData->sttp_body.variant_member.value_data[i].id;
		CString Value = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
		for (int j = 0; j < arrZONE.GetSize(); j++)
		{
			PT_ZONE * ptz = arrZONE[j];
			if (ptz->code == id)
			{
				if (ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) != -1)
				{
					m_sCurrentZone = Value;
				}
				else if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
				{
					m_sEditZone = Value;
				}
				break;
			}
		}
	}
	
	CString strMsg;
				if(g_OperTableType == 0)
					strMsg.Format("%d,%d,%s,%s", OPER_SUCCESS, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone);
				else if(g_OperTableType == 1)
					strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_SUCCESS, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone, m_sCurrentZone);
				pApp->AddNewManOperator("定值区切换执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_EXECUTE, OPER_SUCCESS, m_nOperationNum);
	//关闭定时器
	KillTimer(m_nTimer);
		
	//处理报文
	if(pSttpData->sttp_body.nStatus == 0)
	{
		//成功
		m_ListZone.ClearEdited();
		AfxMessageBox(StringFromID(IDS_EXECUTE_SWITCHZONE_SUCCESS));
	}
	else
	{
		//失败
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		AfxMessageBox(StringFromID(IDS_EXECUTE_SWITCHZONE_FAIL));
	}

	//修改状态
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：OnPTFrameOpen()
 功能概要：响应保护属性页面打开消息, 刷新页面内容
 返 回 值: void
 参    数：param1	消息参数
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B8F00DC
void CPTSetting::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	m_bOperVerify = false;
	m_bMonVerify = false;
	//更新对应的二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	if (m_pObj->m_bNewDevice)
	{
		m_sEditZone = "";
	}
	else
	{
		m_sEditZone = StringFromID(IDS_CALLOPER_NOTSUPPORT);
	}
	
	int nIndex = (int)wParam;

	m_bLoadData = FALSE;
	if(m_pObj->m_bNewDevice)
	{
		CSecCPU* pCpu = NULL;
		for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
		{
			pCpu = m_pObj->m_arrCPU[k];
			MYASSERT_CONTINUE(pCpu);
			CTypedPtrArray<CPtrArray, PT_ZONE*>& arrZONE =  pCpu->m_arrZONE;
			for (int i = 0;  i < arrZONE.GetSize(); i++)
			{
				PT_ZONE * ptz = arrZONE[i];
				MYASSERT_CONTINUE(ptz);
				GetZone(ptz->PT_ID, ptz->cpu, ptz->code, ptz);
				if (ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) != -1)
				{
					m_sCurrentZone = ptz->zoneValue;
				}
				else if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
				{
					m_sEditZone = ptz->zoneValue;
				}
			}	
		}
	}

	if(nIndex == PT_PROP_SETTING)
	{
 		//ReFillAll(TRUE);
		//StartThread();
	}

	RefreshControls();
	
	//页面还原
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	SetDlgItemText(IDC_STATIC_TIME_PTSETTING, "");
	//判断是否显示控制功能
	if(g_PTControl == 1 /*&& m_pObj->m_nType != TYPE_WAVEREC*/)
	{
		if(g_PTControlModel == 2)//广东模式
		{
			m_btnVerify1.ShowWindow(SW_SHOW);
			m_btnVerify2.ShowWindow(SW_SHOW);
		}
		if(m_pObj->m_nCanControl == 0)
		{
			m_btnModifySetting.ShowWindow(SW_SHOW);
			m_btnModifyZone.ShowWindow(SW_SHOW);
			if(g_PTModifyZone != 1)
				m_btnModifyZone.ShowWindow(SW_HIDE);
		}
	}

	m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
	m_btnModifySetting.ShowWindow(SW_HIDE);
	m_btnModifyZone.ShowWindow(SW_HIDE);
	m_btnViewPTSetProg.ShowWindow(SW_HIDE);
	
	CRect rcRect;
	m_btnVerify2.GetWindowRect(rcRect);
	ScreenToClient(&rcRect);

	m_btnViewPTSetHis.ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_PTSET_MODIFY_VIEW_HIS)->SetWindowPos(NULL
		, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height()
		,SWP_NOZORDER/* | SWP_NOSIZE*/ );

	m_btnVerify1.GetWindowRect(rcRect);
	ScreenToClient(&rcRect);
	GetDlgItem(IDC_BTN_PTSET_MODIFY_VIEWPROG)->SetWindowPos(NULL
		, rcRect.left, rcRect.top, rcRect.Width(), rcRect.Height()
		,SWP_NOZORDER/* | SWP_NOSIZE*/ );

	//m_List.ShowWindow(false);
	//m_ListZone.ShowWindow(true);
	if (g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		m_List.ShowWindow(true);
		m_ListZone.ShowWindow(false);
	}
	else
	{
		m_List.ShowWindow(true);
		m_ListZone.ShowWindow(false);
	}

	m_nLoadType = 0;
	ReFillAll();
}

/*************************************************************
 函 数 名：RefreshControls()
 功能概要：刷新控件值, 在第一次启动和更新对应保护时调用
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8D01D4
void CPTSetting::RefreshControls()
{
	//1.填充定值来源下拉框
	CComboBox * pCMB = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_SRC);
	if(pCMB == NULL)
		return;
	pCMB->ResetContent();
	pCMB->AddString(StringFromID(IDS_MODEL_RELAY));
	pCMB->AddString(StringFromID(IDS_SOURCE_SUBSTATIONDB));
	pCMB->AddString(StringFromID(IDS_COMMON_OTHER));
	pCMB->SetCurSel(0);
	m_nSrc = 0;
	//2.填充CPU下拉框
	ReFillCPU();
	//3.填充定值区号下拉框
	ReFillZone();

	UpdateData(FALSE);
}

/*************************************************************
 函 数 名：ReFillCPU()
 功能概要：重新填充CPU下拉框
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8D01E4
void CPTSetting::ReFillCPU()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_CPU);
	if(pCmb == NULL)
		return;
	//清空
	pCmb->ResetContent();
	
	if(m_pObj == NULL)
		return;
	
	//CPU下拉框, 填充内容为"CPU号-描述"
	for(int i = 0; i < m_pObj->m_arrCPU.GetSize(); i++)
	{
		//取得CPU对象
		CSecCPU * pCpu = (CSecCPU*)m_pObj->m_arrCPU.GetAt(i);
		CString str;
		str.Format("%d-%s", pCpu->code, pCpu->des); //cpu号-描述
		int nIndex = pCmb->AddString(str);
		if(nIndex != -1)
			pCmb->SetItemData(nIndex, (DWORD)pCpu);
	}
	//设置默认选择
	if(pCmb->GetCount() > 0)
	{
		if( pCmb->GetCount() == 1 )
			pCmb->SetCurSel(0);
		else
		{
			if( !LoadDefaultCpu() )
			pCmb->SetCurSel(0);
		}
		
		//保存当前选择
		SaveSelCPU(pCmb);
		return;
	}
}

/*************************************************************
 函 数 名：ReFillZone()
 功能概要：根据当前所选CPU填充定值区号下拉框
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0203
void CPTSetting::ReFillZone()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_CPU);
	if(pCmb == NULL)
		return;
	int nIndex = pCmb->GetCurSel();
	if(nIndex == -1)
		return;

	CSecCPU * pCpu = (CSecCPU*)pCmb->GetItemData(nIndex);
	if(pCpu == NULL)
		return;

	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_SELZONE);
	if(pCmb == NULL)
		return;

	//清空
	pCmb->ResetContent();

	//判断是用From-to还是用指定
	if(m_pObj->m_bNewDevice)
	{
		//使用指定
		for(int i = 0; i < pCpu->m_arrZONE.GetSize(); i++)
		{
			PT_ZONE * ptz = (PT_ZONE*)pCpu->m_arrZONE.GetAt(i);
			
			
			if(ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) != -1)
			{	
				for(int i = ptz->minValue; i <= ptz->maxValue; i++)
				{
					CString str;
					str.Format("%d", i);
					pCmb->AddString(str);
				}
				CString str1;
				str1.Format("%d", ptz->zoneValue);
				m_sCurrentZone = str1; 
			}
			
			if (ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
			{
				CString str2;
				str2.Format("%d", ptz->zoneValue);
				m_sEditZone = str2;
			}
			
			if (ptz->zone_name.Find(StringFromID(IDS_COMMON_RUN)) == -1 && ptz->zone_name.Find(StringFromID(IDS_COMMON_EDIT)) == -1)
			{
				for(int i = ptz->minValue; i <= ptz->maxValue; i++)
				{
					CString str;
					str.Format("%d", i);
					pCmb->AddString(str);
				}
				CString str1;
				str1.Format("%d", ptz->zoneValue);
				m_sCurrentZone = str1; 
			}
		}
	}
	else
	{
		//使用from-to
		if(pCpu->from_zone <= pCpu->to_zone)
		{
			for(int i = pCpu->from_zone; i <= pCpu->to_zone; i++)
			{
				CString str;
				str.Format("%d", i);
				pCmb->AddString(str);
			}
		}
		else
		{
			for(int i = pCpu->to_zone; i <= pCpu->from_zone; i++)
			{
				CString str;
				str.Format("%d", i);
				pCmb->AddString(str);
			}
		}
	}
	int nCurrent = pCmb->AddString(StringFromID(IDS_SETTING_CURRENTZONE));
	if(nCurrent >= 0)
	{
		pCmb->SetCurSel(nCurrent);
		//保存当前选择
		SaveSelZone(pCmb);
	}

	//设置默认选择
// 	if(pCmb->GetCount() > 0)
// 	{
// 		pCmb->SetCurSel(0);
// 		pCmb->GetLBText(0, m_sCurrentZone);
// 		//保存当前选择
// 		SaveSelZone(pCmb);
// 		UpdateData(FALSE);
// 		return;
// 	}
}

/*************************************************************
 函 数 名：ResetGroupTab()
 功能概要：重新设置分组TAB, 在第一次启动和重新关联保护时调用
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8D0222
void CPTSetting::ResetGroupTab()
{
	CTabCtrl * pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_PTSET);
	if(pTab == NULL)
		return;
	//清空原来的数据
	pTab->DeleteAllItems();

	if(m_pObj == NULL)
		return;

	if(g_PTSettingGroup != 1 && g_PTControl == 1 /*&& m_pObj->m_nType != TYPE_WAVEREC*/)
	{
		pTab->InsertItem(0, StringFromID(IDS_SECPROP_ZONE));
		pTab->InsertItem(1, StringFromID(IDS_SECPROP_SETTING));
	}

	if(g_PTSettingGroup == 1)
	{
		pTab->InsertItem(0, StringFromID(IDS_SECPROP_SETTING));
		for(int i = 0; i < m_arrGroup.GetSize(); i++)
		{
			PT_GROUP * ptg = (PT_GROUP*)m_arrGroup.GetAt(i);
			CString str; //格式: 组号-组名
			str.Format("%d-%s", ptg->Group, ptg->Name);
			int ii = i;
			pTab->InsertItem(++ii, str);
		}
	}
	

	//设置默认
	if(pTab->GetItemCount() > 1)
	{
		pTab->SetCurSel(1);

		//保存当前选择
		SaveSelGroup(pTab);
	}
}

/*************************************************************
 函 数 名：SaveSelGroup()
 功能概要：保存当前选择的组号
 返 回 值: void
 参    数：param1	分组TAB
**************************************************************/
//##ModelId=49B87B8D0232
BOOL CPTSetting::SaveSelGroup( CTabCtrl * pTab )
{
	int nIndex = pTab->GetCurSel();
	TCITEM tc;
	char temp[256];
	for(int j = 0; j < 256; j++)
	{
		temp[j] = '\0';
	}
	tc.mask = TCIF_TEXT;
	tc.pszText = temp;
	tc.cchTextMax = 256;
	pTab->GetItem(nIndex, &tc);
	
	CString strGroup = tc.pszText;
	int pos = strGroup.Find('-', 0);
	if(pos != -1)
	{
		strGroup = strGroup.Left(pos);
	}
	int nGroup = atoi(strGroup);
	if(nGroup != m_nGroup)
	{
		m_nGroup = atoi(strGroup);
		return TRUE;
	}
	return FALSE;
}

/*************************************************************
 函 数 名：SaveSelZone()
 功能概要：保存当前选择的定值区号
 返 回 值: void
 参    数：param1	定值区号下拉框
**************************************************************/
//##ModelId=49B87B8D0213
BOOL CPTSetting::SaveSelZone( CComboBox * pCmb )
{
	int nIndex = pCmb->GetCurSel();
	if(nIndex < 0)
		return FALSE;
	CString sZone;
	pCmb->GetLBText(nIndex, sZone);
	if( sZone == StringFromID(IDS_SETTING_CURRENTZONE) )
	{
		sZone.Format("%d", CURRENT_ZONE);
	}
	if(sZone != m_sZone)
	{
		m_sZone = sZone;
		return TRUE;
	}
	return FALSE;
}

/*************************************************************
 函 数 名：SaveSelCPU()
 功能概要：保存当前选择的CPU号
 返 回 值: void
 参    数：param1	CPU下拉框
**************************************************************/
//##ModelId=49B87B8D01F4
BOOL CPTSetting::SaveSelCPU( CComboBox * pCmb )
{
	int nIndex = pCmb->GetCurSel();
	if(nIndex < 0)
		return FALSE;
	CString strCPU;
	pCmb->GetLBText(nIndex, strCPU);
	//CPU显示的格式为: CPU号-CPU描述
	int pos = strCPU.Find('-', 0);
	if(pos != -1)
	{
		strCPU = strCPU.Left(pos);
	}
	if(m_sCPU != strCPU)
	{
		m_sCPU = strCPU;
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B8E02AF
void CPTSetting::OnSelchangeTabPtset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//保存选择值
	CTabCtrl * pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_PTSET);
	if(pTab == NULL)
		return;

	int nCurrentSel = pTab->GetCurSel();
	if (nCurrentSel == 0)
	{
		m_List.ShowWindow(false);
		m_ListZone.ShowWindow(true);		
	}
	else
	{
		m_List.ShowWindow(true);
		m_ListZone.ShowWindow(false);
		if(SaveSelGroup(pTab))
		{
			m_nLoadType = 2;
			ReFillAll();
		}
	}

	*pResult = 0;
}

//##ModelId=49B87B8E02BF
void CPTSetting::OnSelchangeCmbPtsetCpu() 
{
	// TODO: Add your control notification handler code here
	//取得当前值
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_CPU);
	if(pCmb == NULL)
		return;
//	if(g_wndWait.GetSafeHwnd() != NULL)
//		g_wndWait.Show(SPLASH_NOTIMER);
	if(SaveSelCPU(pCmb))
	{
		ReFillZone();	
		m_nLoadType = 0;
		ReFillAll();
	}
}

//##ModelId=49B87B8E02CE
void CPTSetting::OnSelchangeCmbPtsetSrc() 
{
	// TODO: Add your control notification handler code here
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_SRC);
	if(pCmb == NULL)
		return;
	int nIndex = pCmb->GetCurSel();
	
	m_nSrc = nIndex;
}

//##ModelId=49B87B8E02DE
void CPTSetting::OnSelchangeCmbPtsetSelzone() 
{
	// TODO: Add your control notification handler code here
	//取得当前值
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_SELZONE);
	if(pCmb == NULL)
		return;

	if(SaveSelZone(pCmb))
	{
		m_nLoadType = 1;
		ReFillAll();
	}
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置LIST文字显示风格
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8E02EE
void CPTSetting::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(pNMHDR ->code == NM_CUSTOMDRAW)
	{
		LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
		switch(lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			{
				*pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
				break;
			}
		case CDDS_ITEMPREPAINT:
			{
				*pResult = CDRF_NOTIFYSUBITEMDRAW;
				break;
			}
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM :
			{
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //行索引
				int nSubItem = lplvcd->iSubItem; //列索引
				//先给默认值, 后面再特殊处理
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);

				//控制字类型
				//控制字类型
				PT_SETTING *pts = (PT_SETTING *)m_List.GetItemData(nItem);
				if(pts != NULL)
				{
					if (pts->DataType == 2 || pts->DataType ==4)
					{
						lplvcd->clrTextBk = RGB(220, 220, 220);
					}
				}

				if(g_PTSettingCompare == 0)
				{
					//与前一次值比较
					//值的第一列与第二列差异
					if(nSubItem == m_nNewValueIndex)
					{
						//值
						CString strValue = m_List.GetItemText(nItem, m_nNewValueIndex);
						CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex + 1);
						
						//去除两边空格
						strValue.TrimLeft();
						strValue.TrimRight();
						strOldValue.TrimLeft();
						strOldValue.TrimRight();
						
						//设置文本颜色
						if(strValue != strOldValue)
							lplvcd->clrText = XJ_LIST_ONE_TWO;
					}
					//第二列与第三列差异
					if(nSubItem == m_nNewValueIndex + 1)
					{
						//值
						CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex + 1);
						CString strOlderValue = m_List.GetItemText(nItem, m_nNewValueIndex + 2);
						
						//去除空格
						strOldValue.TrimLeft();
						strOldValue.TrimRight();
						strOlderValue.TrimLeft();
						strOlderValue.TrimRight();
						
						//设置文本颜色
						if(strOldValue != strOlderValue)
							lplvcd->clrText = XJ_LIST_TWO_THREE;
					}
				}
				else
				{
					//与基准值比较
					//值的第一列与基准值差异
					if(nSubItem == m_nNewValueIndex)
					{
						//值
						CString strValue = m_List.GetItemText(nItem, m_nNewValueIndex);
						CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex - 1);
						
						//去除两边空格
						strValue.TrimLeft();
						strValue.TrimRight();
						strOldValue.TrimLeft();
						strOldValue.TrimRight();
						
						//设置文本颜色
						if(strValue != strOldValue)
							lplvcd->clrText = XJ_LIST_ONE_TWO;
					}
					//第二列与基准值差异
					if(nSubItem == m_nNewValueIndex + 1)
					{
						//值
						CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex + 1);
						CString strOlderValue = m_List.GetItemText(nItem, m_nNewValueIndex - 1);
						
						//去除空格
						strOldValue.TrimLeft();
						strOldValue.TrimRight();
						strOlderValue.TrimLeft();
						strOlderValue.TrimRight();
						
						//设置文本颜色
						if(strOldValue != strOlderValue)
							lplvcd->clrText = XJ_LIST_TWO_THREE;
					}
				}
				
				//最新值列背景色,表示可修改
				if(nSubItem == m_nNewValueIndex)
				{
					//用蓝色背景
					lplvcd->clrTextBk = RGB(200,255,255);
					if(m_arrSetting.GetSize() > nItem)
					{
						PT_SETTING *pts = m_arrSetting[nItem];
						if(pts != NULL)
						{
							if (pts->nRefType == 0)
							{
								lplvcd->clrTextBk = RGB(132, 132, 132);
							}
						}
					}
				}

				//间隔
				if(nItem%2 != 0 && nSubItem < m_nNewValueIndex)
				{
					if (g_style == 1)
					{
						lplvcd->clrTextBk = g_ListSpaceColor2;
					}
					else if(g_style == 2)
					{
						lplvcd->clrTextBk = g_ListSpaceColor3;
					}
					else if(g_style == 3)
					{
						lplvcd->clrTextBk = g_ListSpaceColor4;
					}
					else
					{
						lplvcd->clrTextBk = g_ListSpaceColor;
					}
				}
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置LIST文字显示风格
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8E02EE
void CPTSetting::OnCustomDraw1( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(pNMHDR ->code == NM_CUSTOMDRAW)
	{
		LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
		switch(lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			{
				*pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
				break;
			}
		case CDDS_ITEMPREPAINT:
			{
				*pResult = CDRF_NOTIFYSUBITEMDRAW;
				break;
			}
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM :
			{
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //行索引
				int nSubItem = lplvcd->iSubItem; //列索引
				//先给默认值, 后面再特殊处理
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				//最新值列背景色,表示可修改
				if(nSubItem == 6)
				{
					//用蓝色背景
					lplvcd->clrTextBk = RGB(200,255,255);
				}
				
				*pResult = CDRF_NEWFONT;
				break;
	
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

/*************************************************************
 函 数 名：OnClickListPtset()
 功能概要：响应点击事件, 最新值列可进行修改操作
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8E02FD
void CPTSetting::OnClickListPtset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;
	if (nSubItem == m_nNewValueIndex && nItem != -1 /*&& m_pObj->m_nType != TYPE_WAVEREC*/)
	{
		//SetDlgItemText(IDC_REMIND, "双击鼠标退出编辑状态");
	}
	//是否可使用控制功能
	if(g_PTControl == 1 /*&& m_pObj->m_nType != TYPE_WAVEREC*/)
	{
		if(nItem != -1)
		{
			if(nSubItem == m_nNewValueIndex)
			{
				//最新值列
				
				PT_SETTING* pts = (PT_SETTING*)m_List.GetItemData(nItem);
				MYASSERT(pts);
				int nTextLength = -1;
				if(pts->nRefType != 0)
				{
					if(pts->DataType == 2 || pts->DataType == 4)
					{
						int nValueCount = 0;
						//控制字
						CString sName = pts->Name;
						if(!sName.IsEmpty())
						{
							int nFind = sName.Find(",", 0);
							if(nFind != -1)
							{
								sName.Delete(0, nFind+1);
								//查找数量
								nFind = sName.Find(",", 0);
								if(nFind != -1)
									nValueCount = atoi(sName.Left(nFind));
							}
						}
						if(nValueCount > 0)
						{
							if(pts->DataType == 2)
							{
								//16进制
								int nTemp = nValueCount%4;
								nTextLength = (nValueCount+4-nTemp)/4;
							}
							if(pts->DataType == 4)
							{
								//2进制
								nTextLength = nValueCount;
							}
						}
					}
					m_List.EditSubItem(nItem, nSubItem, atof(pts->minValue), atof(pts->maxValue),3,pts->DataType, atof(pts->stepValue), nTextLength,g_PTSettingJudge, 1==pts->ntimeMStoS);
				}
				
				/*
				CString str = m_List.GetItemText(nItem, nSubItem);
				str = str.Left(str.Find("*", 0));
				int nValue = atoi(str);
				PT_SETTING* pts = (PT_SETTING*)m_List.GetItemData(nItem);
				MYASSERT(pts);
				if (nValue > pts->maxValue)
				{
					AfxMessageBox("修改后的定值超过最大值!");
				}
				*/
			}
		}
	}
	*pResult = 0;
}

//##ModelId=49B87B8E030D
void CPTSetting::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		KillTimer(m_nTimer);
		
		//提示
		CString str = "";
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		str = StringFromID(IDS_CALLFAIL_TIMEOUT);
		switch(m_nCurrentStatus)
		{
		case 1:
			//str = "召唤定值超时!";
			strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
			pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_FAILD);
			break;
		case 2:
			//str = "召唤定值区号超时!";
			m_nCurrentStatus = 0;
			m_bCallZoneSuccess = false;
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ZONE, m_sCPU);			
			pApp->AddNewManOperator(FUNC_QUY_CALLZONE, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ZONE, OPER_FAILD);
			break;
		case 3:
			//str = "修改定值超时!";
			if(m_bChecking)
			{
				strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_PRECORRECTION,m_sCPU, m_sZone, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				
				pApp->AddNewManOperator("定值修改预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,%s,%d", OPER_FAILD, XJ_OPER_MODIFYSETTING_EXECUTE,m_sCPU, m_sZone, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				pApp->AddNewManOperator("定值修改执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSETTING_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
			break;
		case 4:
			//str = "定值组切换超时!";
			if(m_bChecking)
			{
				if(g_OperTableType == 0)
					strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone);
				else if(g_OperTableType == 1)	
					strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_PRECORRECTION, m_sCPU, m_sModifyZone, m_sCurrentZone);
				pApp->AddNewManOperator("定值区切换预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				if(g_OperTableType == 0)
					strMsg.Format("%d,%d,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone);
				else if(g_OperTableType == 1)
					strMsg.Format("%d,%d,%s,,1,,%s,%s", OPER_FAILD, XJ_OPER_CHANGEZONE_EXECUTE, m_sCPU, m_sModifyZone, m_sCurrentZone);
				pApp->AddNewManOperator("定值区切换执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_CHANGEZONE_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
			break;
		default:
			str = "";
			break;
		}
		if(m_nCurrentStatus == 1 && m_nFrameCount > 0)
		{
			//正在接收的是多帧报文, 清空缓存
			ClearMultiFrame();
		}

		//改变状态
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//回复修改前的值
		RevertModifyValue();
		//回复修改前的值
		RevertModifyValue(2);
		AfxMessageBox(str);
	}
	
	if (nIDEvent == m_nPTSetTimer){
		KillTimer(m_nPTSetTimer);
		
		CString str;
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();

		CXJPTSetStore *pStore = CXJPTSetStore::GetInstance();
		QPTSetStateTable *pState = pStore->GetState();
		
		int nPTSetState = pState->GetStateID();
		int nHangoutType = pState->GetType();

		CString sRunnerUserID = pState->GetRunnerUserID().constData();
		CString sOperUserID = pState->GetOperUserID().constData();
		
		// 定值修改按钮是否可用： 
		if (XJ_OPER_HANGOUT == nPTSetState 
			&& XJ_OPER_PTSET == nHangoutType
			&& (sOperUserID.IsEmpty() || pApp->m_User.m_strUSER_ID == sOperUserID)){
			m_btnModifySetting.EnableWindow(TRUE);
		}else{
			m_btnModifySetting.EnableWindow(FALSE);
		}

		// 定值区切换按钮是否可用：
		if (XJ_OPER_HANGOUT == nPTSetState 
			&& XJ_OPER_PTZONESET == nHangoutType
			&& (sOperUserID.IsEmpty() || pApp->m_User.m_strUSER_ID == sOperUserID)){
			m_btnModifyZone.EnableWindow(TRUE);
		}else{
			m_btnModifyZone.EnableWindow(FALSE);
		}

		// 定值修改按钮是否可见
		if (XJ_OPER_UNHANGOUT != nPTSetState && CString(pState->GetPTID().constData()) == m_pObj->m_sID
			&& XJ_OPER_PTSET == nHangoutType
			&& (pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_OPERATOR)
			|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER)))
			m_btnModifySetting.ShowWindow(SW_SHOW);
		else
			m_btnModifySetting.ShowWindow(SW_HIDE);

		// 定值区切换按钮是否可见
		if (XJ_OPER_UNHANGOUT != nPTSetState && CString(pState->GetPTID().constData()) == m_pObj->m_sID
			&& XJ_OPER_PTZONESET == nHangoutType
			&& (pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_OPERATOR)
			|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER)))
			m_btnModifyZone.ShowWindow(SW_SHOW);
		else
			m_btnModifyZone.ShowWindow(SW_HIDE);

		// 查看挂牌进度按钮是否可见
		if (XJ_OPER_UNHANGOUT != nPTSetState && CString(pState->GetPTID().constData()) == m_pObj->m_sID
			&& (XJ_OPER_PTSET == nHangoutType || XJ_OPER_PTZONESET ==nHangoutType)){
			m_btnViewPTSetProg.ShowWindow(SW_SHOW);
			if (XJ_OPER_PTZONESET == nHangoutType)
				m_btnViewPTSetProg.SetWindowText("定值区修改进度");
			else if (XJ_OPER_PTSET == nHangoutType)
				m_btnViewPTSetProg.SetWindowText("定值修改进度");
			else
				m_btnViewPTSetProg.SetWindowText("查看修改进度");
		}
		else
			m_btnViewPTSetProg.ShowWindow(SW_HIDE);

		if (5 == m_pObj->m_nRunStatu && pApp->m_User.m_strUSER_ID == sOperUserID){
			if (4 == nPTSetState && 0 == m_nCurrentDetailStatus){	
				m_nCurrentDetailStatus = 1;

				AfxMessageBox("运行人员已验证定值单内容，定值修改内容将下发到子站，单击<确定>将执行定值修改"
					, MB_OK|MB_ICONINFORMATION);

				pStore->ReLoadState();
				nPTSetState = pState->GetStateID();
				if (0 == nPTSetState){
					AfxMessageBox("运行人员已经取消了相应装置的挂牌，此次定值修改取消！");
					RevertModifyValue();
					pStore->RevertModify();
					m_nCurrentDetailStatus = 0;
				}else{
					pApp->SetRevertModifyValueFlag(2);	// 不允许此时取消挂牌操作
					ExcutePTSet();
				}
			}
		}
		
		if (pState->GetFlags() == 1 && CString(pState->GetPTID().constData()) == m_pObj->m_sID){
			RevertModifyValue();
			pState->SetFlags(0);
			pStore->Save();
		}
		
		// 启用定时器
		m_nPTSetTimer = SetTimer(XJ_OPER_PTSET, 3*1000, NULL);
	}

	if(nIDEvent == m_nRecordTimer)
	{
		//改变持续时间显示
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTSETTING, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：ClearMultiFrame()
 功能概要：清理处理多帧报文时的缓存数据
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0242
void CPTSetting::ClearMultiFrame()
{
	//总帧数,超时是要用总帧数来判断是否正在接收多帧报文
	m_nFrameCount = 0;
	//帧
	m_arrFrame.RemoveAll();
	//定值数据
	for(int i = 0; i < m_arrNewSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)m_arrNewSetting.GetAt(i);
		delete pts;
	}
	m_arrNewSetting.RemoveAll();
}

/*************************************************************
 函 数 名：TranslateMultiFrame()
 功能概要：解析多帧的定值报文, 把数据保存在m_arrNewSetting中, 收全后统一处理
 返 回 值: void
 参    数：param1	当前帧报文数据
		   Param2
**************************************************************/
//##ModelId=49B87B8D0251
void CPTSetting::TranslateMultiFrame( STTP_FULL_DATA* pSttpData )
{
	if(pSttpData == NULL)
		return;
	
	//解析定值数据
	try
	{
		//读取每项值,加入到新定值链表
		int nCount = pSttpData->sttp_body.variant_member.value_data.size();
		for(int i = 0; i < nCount; i++)
		{
			CString id;
			id.Format("%d", pSttpData->sttp_body.variant_member.value_data[i].id);
			CString Value = XJToMS(pSttpData->sttp_body.variant_member.value_data[i].str_value);
			Value = ComplexExchange(Value);
			//创建对象
			PT_SETTING* pts = new PT_SETTING;
			pts->ID = id;
			pts->Value = Value;
			m_arrNewSetting.Add(pts);
		}

		//判断是否已收取完,序号数组里的元素数=总帧数就认为已收取完
		if(m_arrFrame.GetSize() == m_nFrameCount)
		{
			//收取完成,关闭定时器
			KillTimer(m_nTimer);
			//时间,取当前系统时间
			CTime tmTime = CTime::GetCurrentTime();
			CString strTimeDisplay = tmTime.Format("%Y-%m-%d %H:%M:%S");
			//新加一列
			int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
			//加入到list显示
			for(int j = 0; j < m_arrNewSetting.GetSize(); j++)
			{
				PT_SETTING* pts = (PT_SETTING*)m_arrNewSetting.GetAt(j);
				AddSettingValue(pts->ID, pts->Value, Index);
			}
			//清除缓存数据
			ClearMultiFrame();
			//处理完毕,改变状态和按钮文字
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();

			CString sLog;
			sLog.Format("保护[%s]召唤定值成功!", m_pObj->m_sName);
			WriteLog(sLog, XJ_LOG_LV2);

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s,%s", OPER_SUCCESS, XJ_OPER_CALL_SETTING, m_sCPU, m_sZone);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLSETTING, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SETTING, OPER_SUCCESS);
		}
	}
	catch (...)
	{
		WriteLog("CPTSetting::LoadDataFromSTTP, 错误", XJ_LOG_LV3);
		return;
	}
}

/*************************************************************
 函 数 名：OnPTFrameClose()
 功能概要：响应关闭窗口消息, 退出线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F00FA
void CPTSetting::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 函 数 名：ReFillAll()
 功能概要：重新读取和填充所有内容
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D02AF
BOOL CPTSetting::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();

	GetDlgItem(IDC_STATIC_PTSETTING_LOADING)->ShowWindow(SW_SHOW);
	WriteLog("CPTSetting::ReFillAll,开始", XJ_LOG_LV3);
	
	//清除数据
	ClearDataCol();
	if(2 != m_nLoadType)
	{
		EnterCriticalSection(&m_CriticalOper);
		m_HisValueMgr.ResetContent();
		LeaveCriticalSection(&m_CriticalOper);
	}
	if(0 == m_nLoadType)
	{
		ClearSettingGroup();
		ClearSettingConfig();
	}
	if(0 == m_nLoadType || 2 == m_nLoadType)
	{
		m_List.DeleteAllItems();
	}

	if(MODE_SUB == g_role)
	{
		//载入数据
		ReLoadData(0, m_nLoadType);
		//填充数据
		FillData(0, m_nLoadType);
	}
	else
		StartThread();

	WriteLog("CPTSetting::ReFillAll,结束", XJ_LOG_LV3);
	return TRUE;
}

/*************************************************************
 函 数 名：ClearDataCol()
 功能概要：删除新加的数据列
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0261
void CPTSetting::ClearDataCol()
{
	//检查数据有效性
	if(m_List.GetSafeHwnd() == NULL)
		return;

	int nCount = m_List.GetHeaderCtrl()->GetItemCount();
	while(nCount > m_nNewValueIndex)
	{
		m_List.DeleteColumn(m_nNewValueIndex);
		nCount = m_List.GetHeaderCtrl()->GetItemCount();
	}
}

/*************************************************************
 函 数 名：CancelCall()
 功能概要:	取消召唤操作
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0262
void CPTSetting::CancelCall()
{
	//当前是空闲的, 此操作无效
	if(m_nCurrentStatus == 0)
		return;
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	//停止定时器
	KillTimer(m_nTimer);
	if(m_nCurrentStatus == 1)
	{
		//正在召唤定值
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20015, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤定值区号
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20106, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
	}
	else if(m_nCurrentStatus == 3)
	{
		//正在修改定值
		//回复修改前的值
		RevertModifyValue();
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20051, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20053, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
	}
	else if(m_nCurrentStatus == 4)
	{
		//正在切换定值区
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20047, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20055, m_pObj->m_sID, atoi(m_sCPU), atoi(m_sZone));
	}
	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_nCurrentStatus = 0;
	//更改控件可用性
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：SetSettingValue()
 功能概要：设置指定的定值
 返 回 值: 
 参    数：param1	定值ID
		   Param2	定值值
		   Parme3	列索引
**************************************************************/
//##ModelId=49B87B8D0271
void CPTSetting::SetSettingValue( CString ID, CString VALUE, int iCol )
{
	//先找到Setting_ID==ID的索引
	CString strID = ID;
	strID.TrimLeft();
	strID.TrimRight();
	
	PT_SETTING* pst = (PT_SETTING*)FindSetting(strID);
	if(pst == NULL)
		return;
	LVFINDINFO  fi;
	fi.flags = LVFI_PARAM;
	fi.lParam = (LPARAM)pst;
	
	//查找此值在第几行
	int nItem = m_List.FindItem(&fi);
	if(nItem<0)
		return;
	
	//设置ListCtrl的Value列内容
	m_List.SetItemText(nItem, m_nNewValueIndex/*最新值*/, GetDisplayValue(pst, pst->Value));

	//刷新
	m_List.Update(nItem);
}

/*************************************************************
 函 数 名：RevertModifyValue()
 功能概要：回复修改定值时修改的值
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0280
void CPTSetting::RevertModifyValue(int nType, int nFlag)
{
	if(nType == 1)
	{
		if(m_arrModifyList.GetSize() < 0)
			return;
		for(int i = 0; i < m_arrModifyList.GetSize(); i++)
		{
			STTP_DATA * sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
			CString strID;
			strID.Format("%d", sttpData->id);
			SetSettingValue(strID, "", 0);
		}
		//清除修改标记
		m_List.ClearEdited();

		// 还原数据库位置
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
		if (0 == nFlag)
			pApp->RevertPTSetModState(1);
		m_nCurrentDetailStatus = 0;
	}
	else if(nType == 2)
	{
		if (m_ListZone.GetItemCount() != 0)
		{
			if(!m_pObj->m_bNewDevice)
			{
				m_ListZone.SetItemText(0, 6,m_sOldZoneValue);
				m_ListZone.Update(0);
			}
			else
			{
				m_ListZone.SetItemText(m_nModifyItem, 6,m_sOldZoneValue);
				m_ListZone.Update(m_nModifyItem);
			}
		}
		m_ListZone.ClearEdited();
	}

}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：退出自己所起的所有线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D02B0
void CPTSetting::EndOwnerThread()
{
	WriteLog("开始退出保护定值页面读取数据线程", XJ_LOG_LV3);
	m_bExitThread = TRUE;
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("退出保护定值页面读取数据线程完毕", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：IsVaild20069()
 功能概要：判断收到的20069是否自己的
 返 回 值: 是自己的失败报文返回TRUE, 否则返回FALSE
 参    数：param1	失败的操作报文ID
		   Param2
**************************************************************/
//##ModelId=49B87B8D0281
BOOL CPTSetting::IsVaild20069( int nMsgID )
{
	if(20015 == nMsgID && 1 == m_nCurrentStatus)
	{
		//召唤定值
		return TRUE;
	}
	if((20106 == nMsgID || 20173 == nMsgID)  && 2 == m_nCurrentStatus)
	{
		//召唤定值区号
		return TRUE;
	}
	if(20051 == nMsgID && 3 == m_nCurrentStatus)
	{
		//修改定值
		return TRUE;
	}
	if(20053 == nMsgID && 3 == m_nCurrentStatus)
	{
		//修改定值
		return TRUE;
	}
	if((20047 == nMsgID || 20169 == nMsgID) && 4 == m_nCurrentStatus)
	{
		//定值组切换
		return TRUE;
	}
	if((20055 == nMsgID || 20171 == nMsgID) && 4 == m_nCurrentStatus)
	{
		//定值组切换
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B8E031C
BOOL CPTSetting::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTSET);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTZONE);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

/*************************************************************
 函 数 名：FindSetting()
 功能概要：根据定值ID找到定值对象
 返 回 值: 定值对象
 参    数：param1	定值ID
		   Param2
**************************************************************/
//##ModelId=49B87B8D0291
PT_SETTING* CPTSetting::FindSetting( CString sID )
{
	PT_SETTING* pst = NULL;
	for(int i = 0; i < m_arrSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		if(pts->ID == sID)
		{
			pst = pts;
			break;
		}
	}
	return pst;
}

/*************************************************************
 函 数 名：LoadSettingGroup()
 功能概要：查找定值组
 返 回 值: 查找成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D0020
BOOL CPTSetting::LoadSettingGroup(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	WriteLogEx("CPTSetting::LoadSettingGroup, 查询开始");

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据PT_ID, cpu_code,查找103group
	
	CString str;
	Field Field0;
	bzero(&Field0, sizeof(Field));
	Field0.FieldType = 97;
	sql.Fieldlist.push_back(Field0);
		
	//字段
	//103group
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "103group", EX_STTP_DATA_TYPE_INT);

	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//cpu_code
	Condition condition2;
	str.Format("CPU_CODE = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
		
	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadSettingGroup, 查询失败");
		return FALSE;
	}

	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		
		CString str;
		str.Format("CPTSetting::LoadSettingGroup,查询到%d条数据", nCount);
		WriteLogEx(str);
		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			//创建定值组对象
			PT_GROUP * group = new PT_GROUP;
			if(group == NULL)
			{
				pMemset.MoveNext();
				continue;
			}
			CString str = pMemset.GetValue((UINT)0); //103group
			
			group->Group = atoi(str);
			group->Name = ""; //name
			
			m_arrGroup.Add(group);

			if(i == 0)
				m_nGroup = group->Group;
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadSettingGroup,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}	
	
	if(m_arrGroup.GetSize() > 0)
		LoadPTGroupConfig(nQueryNo);

	WriteLogEx("CPTSetting::LoadSettingGroup, 查询正常结束");

	return bReturn;
}

/*************************************************************
 函 数 名：ClearSettingGroup()
 功能概要：清除定值组配置
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8C031C
BOOL CPTSetting::ClearSettingGroup()
{
	EnterCriticalSection(&m_CriticalOper);
	//清除定值组配置
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
		delete pg;
		pg = NULL;
	}
	m_arrGroup.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);
	return TRUE;
}

/*************************************************************
 函 数 名：SetGroupName()
 功能概要：设置定值组的名称
 返 回 值: void
 参    数：param1	定值组ID
		   Param2	定值组名称
**************************************************************/
//##ModelId=49B87B8D029F
void CPTSetting::SetGroupName( int nGroupID, CString sName )
{
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
		if(pg->Group == nGroupID)
		{
			pg->Name = sName;
			break;
		}
	}
	LeaveCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B8F0109
void CPTSetting::OnRefreshPTSetting( WPARAM wParam, LPARAM lParam )
{
	//判断隐藏/显示 分组TAB栏
	if(g_PTSettingGroup == 1 || g_PTControl == 1)
	{
		//显示
		GetDlgItem(IDC_TAB_PTSET)->ShowWindow(SW_SHOW);
	}
	else
	{
		//隐藏
		GetDlgItem(IDC_TAB_PTSET)->ShowWindow(SW_HIDE);
	}
	OnSize(0, 0, 0);
	m_nLoadType = 0;
	ReFillAll();	
}

//##ModelId=49B87B8E032C
void CPTSetting::OnBtnPtsetStyle() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->DoSysSetting(SYS_SETTING_PTPROP);
}

/*************************************************************
 函 数 名：LoadNewOrderTime()
 功能概要：查询最新的基准值时间
 返 回 值: 基准值时间
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D001F
CString CPTSetting::LoadNewOrderTime()
{
	CString strReturn = "";
	if(m_pObj == NULL)
		return "";
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的1条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//根据PT_ID, CPU, Zone, 查找Time从大到小排序,取最新一个
	
	//top
	CString str;	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "1", EX_STTP_DATA_TYPE_TOP);
	//id
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "TIME", EX_STTP_DATA_TYPE_TIME);
	
	//条件
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	//cpu
	Condition condition2;
	str.Format("CPU = %s", m_sCPU);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	//zone
	Condition condition3;
	int nzone = atoi(m_sZone);
	if(nzone == CURRENT_ZONE)
		nzone = atoi(m_sCurrentZone);
	str.Format("ZONE = %s", m_sZone);
	pApp->m_DBEngine.SetCondition(sql, condition3, str);
	
	//按时间排序
	Condition condition5;
	str = "order by TIME DESC";
	pApp->m_DBEngine.SetCondition(sql, condition5, str, EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_ST_ORDERSETTING_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadNewOrderTime, 查询失败");
		delete[] sError;
		delete pMemset;
		return "";
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString str = "";
		if(nCount == 0)
		{
			CString str;
			str.Format("保护%s, CPU号%s, ZONE号%s, 没有定值基准值", m_pObj->m_sID, m_sCPU, m_sZone);
			WriteLog(str, XJ_LOG_LV3);
		}
		if(nCount == 1)
		{
			strReturn = pMemset->GetValue((UINT)0); //只能有一个
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadNewOrderTime,查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	return strReturn;
}

//##ModelId=49B87B8D03B9
void CPTSetting::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!bActivate)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj != NULL)
	{
		CString strStation = StringFromID(IDS_COMMON_UNKNOWN_SUBSTATION);
		if(m_pObj->m_pStation != NULL)
			strStation = m_pObj->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pObj->m_sName);
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_SETTING));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_SETTING));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 函 数 名：TranslateSingleKey()
 功能概要：解释单个关键字的意义
 返 回 值: 关键字表示的值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B8D02BF
CString CPTSetting::TranslateSingleKey( CString sKey )
{
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pObj == NULL)
		return "";
	if(m_List.GetSafeHwnd() == NULL)
		return "";
	CString sReturn = "";
	int nFind = -1;
	//判断是否带有[ ]运算符
	nFind = sKey.Find('[', 0);
	if(nFind > -1)
	{
		//带有[]运算符
		int nFind2 = sKey.Find(']', nFind);
		if(nFind2 == -1)
		{
			//没找到结束括号
			return "";
		}
		//取得索引号
		CString strIndex = sKey.Mid(nFind+1, nFind2- nFind - 1);
		int nIndex = atoi(strIndex);
		if(nIndex < 0)
			return "";
		if(nIndex >= m_List.GetItemCount())
			return "";
		//取得头部
		CString strHead = sKey.Left(nFind);
		if(strHead != "$PT_ROUTINE_LIST")
		{
			//如果不是保护常规值, 返回空
			return "";
		}
		//取得尾部字符
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "PT_ROUTINE_ID$")
		{
			return m_List.GetItemText(nIndex, 0); //常规值ID
		}
		if(strTail == "PT_ROUTINE_NAME$")
		{
			return m_List.GetItemText(nIndex, 1); //常规值名称
		}
		if(strTail == "PT_ROUTINE_CODE$")
		{
			return m_List.GetItemText(nIndex, 2); //常规值代码
		}
		if(strTail == "PT_ROUTINE_UNIT$")
		{
			return m_List.GetItemText(nIndex, 3); //单位
		}
		if(strTail == "PT_ROUTINE_ORDER$")
		{
			return m_List.GetItemText(nIndex, 4); //定值基准值
		}
		if(strTail == "PT_ROUTINE_DATATYPE$")
		{
			return m_List.GetItemText(nIndex, 10);//数据类型
		}
		//以上都不是,可能是$PT_ROUTINE_ VALUE#N$的形式
		int nValueItem = -1;
		nValueItem = strTail.Find('#', 0);
		if(nValueItem != -1)
		{
			//是有#的形式, 判断是不是PT_ROUTINE_ VALUE#N
			CString strTemp = strTail;
			strTemp = strTemp.Left(nValueItem);
			if(strTemp != "PT_ROUTINE_VALUE")
			{
				return "";
			}
			//得到值索引
			strTemp = strTail;
			strTemp.Delete(0, nValueItem+1);
			nValueItem = atoi(strTemp);
			if(nValueItem < 1)
				nValueItem = 1;
			int nSubItem = m_nNewValueIndex + nValueItem - 1;
			return m_List.GetItemText(nIndex, nSubItem);
		}
	}
	else
	{
		//判断是否有.GetCount()方法
		nFind = sKey.Find(".GetCount()", 0);
		if(nFind > -1)
		{
			//有.GetCount()方法
			if(sKey == "$PT_ROUTINE_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_List.GetItemCount());
				return sReturn;
			}
		}
		else
		{
			//普通关键字
			if(sKey == "$PT_SEL_CPU$") //当前所选CPU
				return m_sCPU;
			if(sKey == "$PT_SEL_ZONE$") //当前所选ZONE
				return m_sZone;
			if(sKey == "$PT_NAME$") //保护名称
			{
				return m_pObj->m_sName;
			}
			if(sKey == "$PT_MODEL$") //保护型号
			{
				return m_pObj->m_sModel;
			}
			if(sKey == "$DEVICE_NAME$") //一次设备
			{
				if(m_pObj->m_pOwner != NULL)
					return m_pObj->m_pOwner->m_sName;
				return "";
			}
			if(sKey == "$STATION_NAME$") //厂站名
			{
				
				//查找厂站
				CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
				CDataEngine* pData = pApp->GetDataEngine();
				CStationObj* pStation = (CStationObj*)pData->FindDevice(m_pObj->m_sStationID, TYPE_STATION);
				if(pStation != NULL)
				{
					return pStation->m_sName;
				}
				else
				{
					return "";
				}
				
			}
			//以上都不是, 有可能是$PT_ROUTINE_TIME#N$的情况
			int nValueItem = -1;
			nValueItem = sKey.Find('#', 0);
			if(nValueItem != -1)
			{
				//是有#的形式, 判断是不是PT_ROUTINE_TIME#N
				CString strTemp = sKey;
				strTemp = strTemp.Left(nValueItem);
				if(strTemp != "$PT_ROUTINE_TIME")
				{
					return "";
				}
				//得到值索引
				strTemp = sKey;
				strTemp.Delete(0, nValueItem+1);
				nValueItem = atoi(strTemp);
				if(nValueItem < 1)
					nValueItem = 1;
				int nSubItem = m_nNewValueIndex + nValueItem - 1;
				LVCOLUMN col;
				col.mask = LVCF_TEXT;
				char temp[256];
				for(int j = 0; j < 256; j++)
				{
					temp[j] = '\0';
				}
				col.cchTextMax = 256;
				col.pszText = temp;
				m_List.GetColumn(nSubItem, &col);
				CString strReturn = temp;
				return strReturn;
			}
		}
	}
	return "";
}

/*************************************************************
 函 数 名：OnBtnPtsetPrint()
 功能概要：打印定值单
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8E032E
void CPTSetting::OnBtnPtsetPrint() 
{
	// TODO: Add your control notification handler code here
}

/*************************************************************
 函 数 名：InitCOM()
 功能概要：初始化打印模板COM组件
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D02C1
BOOL CPTSetting::InitCOM()
{
	/*HRESULT hr;
	if((hr = ::CLSIDFromProgID(L"XJReportDoc", &clsid)) != NOERROR)
	{
		TRACE("unable to find program ID -- error= %x\n", hr);
		return FALSE;
	}
	
	if((hr = ::CoGetClassObject(clsid, CLSCTX_INPROC_SERVER, NULL, 
		IID_IClassFactory, (void **)&pClf)) != NOERROR)
	{
		TRACE("unable to find CLSID -- error = %x\n", hr);
		return FALSE;
	}
	
	pClf ->CreateInstance(NULL, IID_IUnknown, (void **)&pUnk);
	pUnk ->QueryInterface(IID_IXJReport, (void **)&pReportDoc);
	
	return TRUE;*/
	return InitReportComNoReg(pClf, pReportDoc);
}

/*************************************************************
 函 数 名：ReleaseCOM()
 功能概要：删除COM组件
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8D02C2
void CPTSetting::ReleaseCOM()
{
	if(pClf != NULL)
	{
		pClf->Release();
		pClf = NULL;
	}
	if(pUnk != NULL)
	{
		pUnk->Release();
		pUnk = NULL;
	}
	if(pReportDoc != NULL)
	{
		pReportDoc->Release();
		pReportDoc = NULL;
	}
}

/*************************************************************
 函 数 名：DoPrintSetting()
 功能概要：打印定值
 返 回 值: void
 参    数：param1	模板文件全路径
		   Param2
**************************************************************/
//##ModelId=49B87B8D02CE
void CPTSetting::DoPrintSetting( CString strPath )
{
	InitCOM();
	//打开模板文件
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, strPath);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),strPath);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//设置回调函数
	if(pReportDoc != NULL && bOpened)
	{
		if (m_pObj->m_nType == TYPE_WAVEREC)
		{
			pReportDoc->SetQueryFunction(TranslateKeyInWRSetting);
		}
		else
			pReportDoc->SetQueryFunction(TranslateKeyInPTSetting);
	}
	if(pReportDoc != NULL && bOpened)
		pReportDoc->FillValueData();
	//打印
	if(pReportDoc != NULL && bOpened)
	{
		g_hook = 1;
		pReportDoc->Print();
		g_hook = 0;
	}
	//删除COM
	ReleaseCOM();
}

//##ModelId=49B87B8E033C
void CPTSetting::OnSettingprintNew1() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintSetting(SETTING_NEW1_TP);
}

//##ModelId=49B87B8E033E
void CPTSetting::OnSettingprintNew2() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintSetting(SETTING_NEW2_TP);
}

//##ModelId=49B87B8E0340
void CPTSetting::OnSettingprintNew3() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintSetting(SETTING_NEW3_TP);
}

//##ModelId=49B87B8E0342
void CPTSetting::OnSettingprintOrder1() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintSetting(SETTING_ORDER1_TP);
}

//##ModelId=49B87B8E0344
void CPTSetting::OnSettingprintOrder2() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintSetting(SETTING_ORDER2_TP);
}

//##ModelId=49B87B8E0346
void CPTSetting::OnSettingprintOrder3() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintSetting(SETTING_ORDER3_TP);
}

/*************************************************************
 函 数 名：OnSTTP20157()
 功能概要：响应20157, 超时返回通知
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F00CB
void CPTSetting::OnSTTP20157( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2 && m_nCurrentStatus != 3 && m_nCurrentStatus != 4)
	{
		//没有处于召唤定值状态,也没有处于召唤定值区号状态,
		//也没有处于修改定值状态, 也没有处于定值组切换状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20069报文
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//错误报文
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}

	//检查CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//不是本CPU的报文
		return;
	}
	
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}

	//超时行为
	int nTimeoutStatus = pSttpData->sttp_body.nStatus;
	//1: 起始基准时间后移设定间隔
	//关闭定时器
	KillTimer(m_nTimer);
	//启动定时器
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	
	if(m_nCurrentStatus == 1)
	{
		//正在召唤定值				
		int nTimeOut = pData->m_nPTSettingTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤定值区号				
		int nTimeOut = pData->m_nPTZoneTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 3)
	{
		//正在修改定值
		
		int nTimeOut = pData->m_nModifySettingTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 4)
	{
		//正在切换定值区				
		int nTimeOut = pData->m_nChangeZoneTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}

/*************************************************************
 函 数 名: LoadCurrentZone()
 功能概要: 从数据库查询当前定值区
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTSetting::LoadCurrentZone()
{
	if(g_role != MODE_SUB && m_bExitThread)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的1条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//TOP num
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str.Format("%d", 1);
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field1);
		
	//条件
	//只查询未确认事件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("pt_id = '%s'", m_pObj->m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	Condition condition3;
	str.Format("CPU_CODE=%d", atoi(m_sCPU));
	pApp->m_DBEngine.SetCondition(sql, condition3, str);
		
	//按动作启动时间大小排序
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by CURTIME DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition2);
			
	CMemSet pMemset;

	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ZONE_CFG, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CPTSetting::LoadCurrentZone, 查询失败", XJ_LOG_LV3);
		return;
	}
	if(g_role != MODE_SUB && m_bExitThread)
		return;
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount == 1)
		{
			CString str;
			str.Format("CPTSetting::LoadCurrentZone, 读取到%d条数据", nCount);
			WriteLog(str, XJ_LOG_LV3);

			m_sCurrentZone = pMemset.GetValue((UINT)3);
			//m_sZone = m_sCurrentZone;
		}	
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadCurrentZone,查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
	}
	
	WriteLog("CPTSetting::LoadCurrentZone, 查询正常结束", XJ_LOG_LV3);
}

void CPTSetting::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTSetting::OnDoubleclickedBtnPtsetCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTSetting::OnDoubleclickedBtnPtsetCallzone() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTSetting::OnBtnPtsetVerify0() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	int nDZModState = pApp->GetPTSetModState(PT_ZONE(), CString());

	//运行人员确认
	CDlgCheckPro dlg(this, 0);
	dlg.m_strModify = m_strOutPut;
	dlg.m_sZone = m_sZone;
	dlg.m_sCPU = m_sCPU;
	if(dlg.DoModal() == IDOK)
	{
		if(g_PTControlModel == 1)
		{
			//普通模式,要求运行人员验证
			CDlgValidateID dlgID(0);
			dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
			if(dlgID.DoModal() == IDOK)
			{
				m_sRunUser = dlgID.m_strUser;
				
				CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
				CString str;
				str.Format("用户%s以运行员身份验证成功:修改定值", m_sRunUser);
				WriteLog(str);
				pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sRunUser, -1, OPER_SUCCESS,m_nOperationNum);
				
				// 运行人员核对后修改状态机
				//pApp->ModifyDZModState(4, m_sRunUser, m_pObj);
				PT_ZONE zone;
				pApp->GetPTSetModState(zone);
				pApp->NextPTSetModState(4, zone, m_sRunUser);
			}
			else
			{
				//无操作权限
				//回复修改前的值
				RevertModifyValue();
				CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
				CString str;
				str.Format("用户%s以运行员身份验证失败:修改定值", m_sRunUser);
				WriteLog(str, XJ_LOG_LV2);
				pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sRunUser, -1, OPER_FAILD,m_nOperationNum);

				return;
			}
		}
	}
	else
	{
		//不同意修改
		//回复修改前的值
		RevertModifyValue();
		UpdateControlsEnable();
		
		pApp->RevertPTSetModState(1);
		m_nCurrentDetailStatus = 0;

		return;
	}
}

void CPTSetting::OnBtnPtsetVerify1() 
{
	// TODO: Add your control notification handler code here
	CDlgValidateID dlgID(2);
	dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
	if(dlgID.DoModal() == IDOK)
	{
		m_sOperUser = dlgID.m_strUser;
		m_bOperVerify = true;
		m_btnVerify1.SetIcon(IDI_TICK);
		m_btnVerify2.EnableWindow(TRUE);
	}
	
	return;
}

void CPTSetting::OnBtnPtsetVerify2() 
{
	// TODO: Add your control notification handler code here
	CDlgValidateID dlgID(1);
	dlgID.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
	if(dlgID.DoModal() == IDOK)
	{
		m_sMonUser = dlgID.m_strUser;
		if (!m_sMonUser.CompareNoCase(m_sOperUser))
		{
			AfxMessageBox(StringFromID(IDS_VALIDATE_MULTI_IDENTITY));
			return;
		}
		m_bMonVerify = true;
		m_btnVerify2.SetIcon(IDI_TICK);
		if (m_bOperVerify)
		{
			m_btnModifySetting.EnableWindow(TRUE);
			m_btnModifyZone.EnableWindow(TRUE);
		}
	}
}

void CPTSetting::OnDblclkListPtset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

 	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;

	CString sCurZone;
	CString sSelZone;
	
	GetDlgItem(IDC_EDIT_PTSET_CURZONE)->GetWindowText(sCurZone);
	GetDlgItem(IDC_CMB_PTSET_SELZONE)->GetWindowText(sSelZone);

	
	if(nItem != -1)
	{
		PT_SETTING* pts = (PT_SETTING*)m_List.GetItemData(nItem);
		MYASSERT(pts);
		
		if(g_PTControl && nSubItem == m_nNewValueIndex /*&& m_pObj->m_nType != TYPE_WAVEREC*/)
		{
			
			if( atoi(m_sZone) == CURRENT_ZONE )
			{
				AfxMessageBox(StringFromID(IDS_MODIFYSETTING_SELZONE));
				return;
			}
			if(g_PTNotModifyCurrentZone)
			{
				if (sCurZone == sSelZone)
				{
					AfxMessageBox(StringFromID(IDS_MODIFYSETTING_NOTCURRENTZONE));
					return;
				}
			}
			if(pts->nRefType == 0)
			{
				AfxMessageBox(StringFromID(IDS_MODIFYSETTING_NOTPARAM));
				return;
			}
			SetDlgItemText(IDC_REMIND, StringFromID(IDS_TIP_QUIT_EDITMODE));
			if (CheckVPicklist(pts->VPickList))
			{
				//最新值列
				CString str;
				str.Format("%s",pts->VPickList);
				int nMax =GetTranslatedNumber(str);
				str = "";
				for (int i = -1 ;i <= nMax; i++)
				{
					CString sID;
					CString sReturn;
					sID.Format("%d",i);
					sReturn = GetTranslatedValue(pts,sID);
					if (sID == sReturn )
						continue;
					str += sReturn;
					if (i != nMax)
					{
						str += ";";
					}	
				}
				m_List.ListSubItem(nItem, nSubItem, str);
			}
			else
			{
				//最新值列
				int nTextLength = -1;
				if(pts->DataType == 2 || pts->DataType == 4)
				{
					int nValueCount = 0;
					//控制字
					CString sName = pts->Name;
					if(!sName.IsEmpty())
					{
						int nFind = sName.Find(",", 0);
						if(nFind != -1)
						{
							sName.Delete(0, nFind+1);
							//查找数量
							nFind = sName.Find(",", 0);
							if(nFind != -1)
								nValueCount = atoi(sName.Left(nFind));
						}
					}
					if(nValueCount > 0)
					{
						if(pts->DataType == 2)
						{
							//16进制
							int nTemp = nValueCount%4;
							nTextLength = (nValueCount+4-nTemp)/4;
						}
						if(pts->DataType == 4)
						{
							//2进制
							nTextLength = nValueCount;
						}
					}
				}
				m_List.EditSubItem(nItem, nSubItem, atof(pts->minValue), atof(pts->maxValue),3,pts->DataType, atof(pts->stepValue), nTextLength,g_PTSettingJudge, 1==pts->ntimeMStoS);
			}
		}
		else
		{
			pts->Value = m_List.GetItemText(nItem, m_nNewValueIndex);
			if ((pts->DataType == 2 || pts->DataType == 4)&& nSubItem < m_nNewValueIndex)
			{
				pts->Value = m_List.GetItemText(nItem, m_nNewValueIndex);
				//初始化对话框
				CDlgSettingDetail dlg;
				dlg.m_pts = pts;
				dlg.DoModal();
 			}
		}	
	}
	*pResult = 0;
}

void CPTSetting::OnClickListPtzone(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	*pResult = 0;
	return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;

	m_nModifyItem = 0;

	//是否可使用控制功能
	if(g_PTControl != 1 /*|| m_pObj->m_nType == TYPE_WAVEREC*/)
		return;
	if(g_PTModifyZone != 1)
		return;
	if(nItem != -1)
	{
		if(nSubItem == m_nNewZoneIndex)
		{
			CString str = m_ListZone.GetItemText(nItem, 0);
			//最新值列
			if(m_pObj->m_bNewDevice == 1)
			{
				if (m_pObj->m_bNewDevice /*&& str.Find("编辑") != -1*/)
				{
					for (int j = 0; j < m_ListZone.GetItemCount(); j++)
					{
						CString str = m_ListZone.GetItemText(j, 6);
						if (str.Find("*") != -1)
						{
							if (j != nItem)
							{
								AfxMessageBox(StringFromID(IDS_SWITCHZONE_ONLYONE));
								return;
							}
						}
					}

					if(str.Find(StringFromID(IDS_COMMON_EDIT)) != -1)
					{
						AfxMessageBox(StringFromID(IDS_MODIFYSETTING_EDITZONE));
						return;
					}
					PT_ZONE* ptz = (PT_ZONE*)m_ListZone.GetItemData(nItem);
					MYASSERT(ptz);
					
					m_nModifyItem = nItem;

					m_sOldZoneValue = m_ListZone.GetItemText(nItem, 6);

					m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue, 2, 1, 1);
					
					/*if (str.Find("运行") != -1)
					{
						m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue, 2);
					}
					else
					{
						m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue);	
					}*/
				}
			}
			else
			{
				for (int j = 0; j < m_ListZone.GetItemCount(); j++)
				{
					CString str = m_ListZone.GetItemText(j, 6);
					if (str.Find("*") != -1)
					{
						if (j != nItem)
						{
							AfxMessageBox(StringFromID(IDS_SWITCHZONE_ONLYONE));
							return;
						}
					}
				}

				m_sOldZoneValue = m_ListZone.GetItemText(nItem, 6);

				m_ListZone.EditSubItem(nItem, nSubItem, atoi(m_ListZone.GetItemText(nItem, 3)), atoi(m_ListZone.GetItemText(nItem, 4)), 3, 1,1);
			}
		}
	}
	*pResult = 0;
}



/*************************************************************
 函 数 名：GetZone()
 功能概要：从数据库查找对应的定值区号
 返 回 值: 查找成功返回TRUE, 失败或无数据返回FALSE
**************************************************************/
//##ModelId=49B87BA8023A
BOOL CPTSetting::GetZone(CString pt_id, int nCpu, int zone_id, PT_ZONE* pt_zone)
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "zone_value";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field2);
		
	//条件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", pt_id);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_Code = %d", nCpu);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("zone_id = %d", zone_id);
	strncpy(condition3.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition3);

	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(/*EX_STTP_INFO_ZONE_BASE_CFG*/EX_STTP_INFO_PT_ZONE_CONFIG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CPTSetting::GetZone, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 0)
		{
			bReturn = FALSE;
		}
		else
		{
			CString str = pMemset->GetValue((UINT)0);
			pt_zone->zoneValue = atoi(str);
		}
	
	}
	else
	{
				
		CString str;
		str.Format("CPTSetting::GetZone,查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		bReturn = FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

BOOL CPTSetting::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CViewBase::PreTranslateMessage(pMsg);
}

void CPTSetting::StartThread()
{
	//启动线程
	
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTSettingLoad,this,THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("保护定值查询线程启动成功,查询批次:%d", m_nQueryNo);
	WriteLog(strLog);
}

/****************************************************
Date:2012/3/13  Author:LYH
函数名:   FindNewValue	
返回值:   CString	
功能概要: 
参数: int nID	
*****************************************************/
CString CPTSetting::FindNewValue( int nID )
{
	for(int i = 0; i < m_arrSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)m_arrSetting.GetAt(i);
		if(pts == NULL)
			continue;
		if(atoi(pts->ID) == nID)
			return pts->Value;
	}
	return "";
}

/*
void CPTSetting::ShowTip()
{
	m_tool.UpdateTipText("双击退出编辑状态", this);
}
*/


void CPTSetting::OnDblclkListPtzone(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;

	m_nModifyItem = 0;

	//是否可使用控制功能
	if(g_PTControl != 1 /*|| m_pObj->m_nType == TYPE_WAVEREC*/)
		return;
	if(g_PTModifyZone != 1)
		return;
	if(nItem != -1)
	{
		if(nSubItem == m_nNewZoneIndex)
		{
			PT_ZONE *pZone = (PT_ZONE *)m_ListZone.GetItemData(nItem);
			if (pZone!=NULL)
			{
				CString str;
				//最新值列
				if (pZone->zoneValue != NULL)
				{
					
					str.Format("%d", pZone->zoneValue);
				}
				else
				{
					//str.Format("%d;%d;%d", 3, 4, 5);
					for(int i = pZone->minValue; i <= pZone->maxValue ; i++)
					{
						CString str2;
						if (i == pZone->maxValue)
						{	
							str2.Format("%d",i);
							str += str2;
						}
						else
						{
							str2.Format("%d;",i);
							str += str2;
						}
						
					}
					
				}
				CString sOld = m_ListZone.GetItemText(nItem, m_nNewZoneIndex);
				if( sOld.Find('*') == -1 )
					m_sOldZoneValue = sOld;
				m_ListZone.ListSubItem(nItem, nSubItem, str);
//				break;
			}
			else
			{
				CSecCPU* pCpu = NULL;
				for(int k = 0; k < m_pObj->m_arrCPU.GetSize(); k++)
				{
					pCpu = m_pObj->m_arrCPU[k];
					if (pCpu->code == atoi(m_sCPU))
					{
						break;
					}
				}
				CString str;
				str = "";
				if(pCpu->from_zone <= pCpu->to_zone)
				{
					for(int i = pCpu->from_zone; i <= pCpu->to_zone; i++)
					{
						CString str2;
						if (i == pCpu->to_zone)
						{	
							str2.Format("%d",i);
							str += str2;
						}
						else
						{
							str2.Format("%d;",i);
							str += str2;
						}
					}
				}
				else
				{
					for(int i = pCpu->to_zone; i <= pCpu->from_zone; i++)
					{
						CString str2;
						if (i == pCpu->from_zone)
						{	
							str2.Format("%d",i);
							str += str2;
						}
						else
						{
							str2.Format("%d;",i);
							str += str2;
						}
					}
				}
				CString sOld = m_ListZone.GetItemText(nItem, m_nNewZoneIndex);
				if( sOld.Find('*') == -1 )
					m_sOldZoneValue = sOld;
				m_ListZone.ListSubItem(nItem, nSubItem, str);
			}
		}
	}

// 	if(nItem != -1)
// 	{
// 		if(nSubItem == m_nNewZoneIndex)
// 		{
// 			CString str = m_ListZone.GetItemText(nItem, 0);
// 			//最新值列
// 			if(m_pObj->m_bNewDevice == 1)
// 			{
// 				if (m_pObj->m_bNewDevice /*&& str.Find("编辑") != -1*/)
// 				{
// 					for (int j = 0; j < m_ListZone.GetItemCount(); j++)
// 					{
// 						CString str = m_ListZone.GetItemText(j, 6);
// 						if (str.Find("*") != -1)
// 						{
// 							if (j != nItem)
// 							{
// 								AfxMessageBox("仅支持对一项定值区进行修改!");
// 								return;
// 							}
// 						}
// 					}
// 
// 					if(str.Find("编辑") != -1)
// 					{
// 						AfxMessageBox("编辑区定值不可修改!");
// 						return;
// 					}
// 					PT_ZONE* ptz = (PT_ZONE*)m_ListZone.GetItemData(nItem);
// 					MYASSERT(ptz);
// 					
// 					m_nModifyItem = nItem;
// 
// 					m_sOldZoneValue = m_ListZone.GetItemText(nItem, 6);
// 
// 					m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue, 2, 1, 1);
// 					
// 					/*if (str.Find("运行") != -1)
// 					{
// 						m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue, 2);
// 					}
// 					else
// 					{
// 						m_ListZone.EditSubItem(nItem, nSubItem, ptz->minValue, ptz->maxValue);	
// 					}*/
// 				}
// 			}
// 			else
// 			{
// 				for (int j = 0; j < m_ListZone.GetItemCount(); j++)
// 				{
// 					CString str = m_ListZone.GetItemText(j, 6);
// 					if (str.Find("*") != -1)
// 					{
// 						if (j != nItem)
// 						{
// 							AfxMessageBox("仅支持对一项定值区进行修改!");
// 							return;
// 						}
// 					}
// 				}
// 
// 				m_sOldZoneValue = m_ListZone.GetItemText(nItem, 6);
// 
// 				m_ListZone.EditSubItem(nItem, nSubItem, atoi(m_ListZone.GetItemText(nItem, 3)), atoi(m_ListZone.GetItemText(nItem, 4)), 3, 1,1);
// 			}
// 		}
// 	}
	*pResult = 0;
}
/****************************************************
Date:2012/10/22  Author:LY
函数名:   GetTranslatedNumber	
返回值:   int	
功能概要: 
参数: CString str	
*****************************************************/
int CPTSetting::GetTranslatedNumber(CString str)
{
	int nID = -1;
	CString sID = "";
	//根据VPickList填充Value内容,VPickList不空时，Value值的解释根据VPivkList确定 
	//如VPickList="0:on;1:off" 而 Value=1 时，strValue为"off"
	//如VPickList为空，而Value=1，Unit="s" 则strValue = "1s"  
	CString strTemp = "";

	CString strVPickList = str;

	
	//去掉多余的空格

	strVPickList.TrimLeft();
	strVPickList.TrimRight();
	if (!strVPickList.IsEmpty())
	{
		int nFenhao =0;	//分号的位置
		int nMaohao =0; //冒号的位置
		BOOL bFound = FALSE;
		
		//只要strVPickList不空，则劈开分号之间的字符串，挨个检查冒号左边的值是否与strReturn相同
		//如果相同则stValue用冒号右边的字串取代
		while (!strVPickList.IsEmpty()) 	 
		{
			nFenhao = strVPickList.Find(';', 0);
			if(nFenhao>0)
				strTemp = strVPickList.Left(nFenhao);
			else
				strTemp = strVPickList;
			
			nMaohao = strTemp.Find(':', 0);			 
			if (nMaohao>0)
			{
				sID = strTemp.Left(nMaohao);
			}
			
			if (nFenhao>0)
				strVPickList.Delete(0, nFenhao+1);
			else
				break;
		}
	}
	nID = atoi(sID);
	return nID;
}

/****************************************************
Date:2012/11/27  Author:LYH
函数名:   GetGroupName	
返回值:   CString	
功能概要: 
参数: int nGroupID	
*****************************************************/
CString CPTSetting::GetGroupName( int nGroupID )
{
	for(int i = 0; i < m_arrGroup.GetSize(); i++)
	{
		PT_GROUP * pg = (PT_GROUP*)m_arrGroup.GetAt(i);
		if(pg->Group == nGroupID)
		{
			return pg->Name;
		}
	}
	return "";
}
/****************************************************
Date:2012/11/29  Author:LY
函数名:   CheckVPicklist	
返回值:   BOOL	
功能概要: 
参数: CString strVPickList
*****************************************************/
BOOL CPTSetting::CheckVPicklist(CString strVPickList)
{
	BOOL bRutern = FALSE;
	if (strVPickList == "")
	{
		return bRutern;
	}
	CString sCopy = strVPickList;
	while (!sCopy.IsEmpty())
	{
		int nMaohao = sCopy.Find(':',0);
		int nFenhao = sCopy.Find(';', 0);
		if (nMaohao<0 || nFenhao<0 || nMaohao>nFenhao || nMaohao == nFenhao-1 || nMaohao == 0) //若1.找不到分号或者冒号2.冒号在分号后面3.冒号在第一个4.或者冒号和分号是挨着的 返回错误
		{	
			return bRutern;
		}
		sCopy.Delete(0, nMaohao+1);
		nMaohao = sCopy.Find(':',0);
		nFenhao = sCopy.Find(';', 0);
		if (nMaohao<nFenhao && nMaohao != -1)   //第一个分号前有两个冒号 返回错误
		{
			return bRutern; 
		}
		sCopy.Delete(0,nFenhao+1); //删除当前第一个分号前的字符
	}
	return TRUE;

}
/****************************************************
Date:2012/12/3  Author:LY
函数名:   SetFloatValue	
返回值:   void	
功能概要: 
参数: CString strVPickList
*****************************************************/
CString CPTSetting::SetFloatValue(PT_SETTING *pst, CString strValue)
{
	if (pst==NULL)
	{
		return strValue;
	}
	if (pst->DataType == 0)
	{
		CString sValue = strValue;
		if (sValue != "")
		{
			CString sPrecision = pst->Precision;
			int nDouhao = sPrecision.Find(",");
			sPrecision.Delete(0,nDouhao+1);
			int nAdd = 3;
			sPrecision.TrimLeft();
			sPrecision.TrimRight();
			if(!sPrecision.IsEmpty())
			{
				nAdd = atoi(sPrecision);			//精度
			}
			
			float fValue = atof(sValue);
			CString sFormat;
			sFormat.Format("%%.0%df", nAdd);
			sValue.Format(sFormat, fValue);
			return sValue;
		}
	}
	return strValue;
}


int CPTSetting::GetNextQueryNo()
{
	int nReturn = 0;
	if(m_nQueryNo == 1000)
	{
		nReturn = 0;
	}
	else
		nReturn = m_nQueryNo+1;
	return nReturn;
}


void CPTSetting::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("退出保护定值查询线程,查询号:%d", nNo);
	EnterCriticalSection(&m_CriticalSection);  
	POSITION pos = m_listThread.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION posOld = pos;
		THREADNODE* pNode = (THREADNODE*)m_listThread.GetNext(pos);
		if(pNode != NULL && pNode->nNo == nNo)
		{
			m_listThread.RemoveAt(posOld);
			delete pNode;
			break;
		}
	}
	LeaveCriticalSection(&m_CriticalSection);

	str.Format("保护定值查询线程成功退出,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTSetting::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("保护定值查询线程成功启动,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTSetting::GetFirstNode()
{
	THREADNODE_PTR pReturn  = NULL;
	EnterCriticalSection(&m_CriticalSection);  
	POSITION pos = m_listThread.GetHeadPosition();
	if(pos != NULL)
	{
		pReturn = (THREADNODE_PTR)m_listThread.GetNext(pos);
	}
	LeaveCriticalSection(&m_CriticalSection);
	return pReturn;
}

/****************************************************
Date:2013/12/23  Author:LYH
函数名:   SetZoneCmbToCurrentZone	
返回值:   void	
功能概要: 
*****************************************************/
void CPTSetting::SetZoneCmbToCurrentZone()
{
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_SELZONE);
	if(pCmb != NULL)
	{
		pCmb->SelectString(-1, m_sCurrentZone);
		SaveSelZone(pCmb);
	}
}

void CPTSetting::AddValueCol( int nQueryNo )
{
	if(g_PTSettingValues <= 0)
		return;
	if(m_arrSetting.GetSize() == 0)
		return;
	if(MODE_SUB != g_role && !IsCurrentQueryNo(nQueryNo))
		return;
	//加入值列
	CString strTimeDisplay;
	CString strTime = "";
	WriteLog("CPTSetting::AddValueCol, 开始加入值列", XJ_LOG_LV3);
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_HisValueMgr.GetCount(); i++)
	{
		strTime = m_HisValueMgr.GetTime(i);
		if(strTime.ReverseFind('.') != -1)
			strTimeDisplay = strTime.Left(strTime.ReverseFind('.'));
		else
			strTimeDisplay = strTime;
		if(IsCurrentQueryNo(nQueryNo) || MODE_SUB == g_role)
		{
			m_List.InsertColumn(m_nNewValueIndex+i, strTimeDisplay, LVCFMT_LEFT, 150, 0);
			CString strLog;
			strLog.Format("添加第%d列定值历史值列,时间:%s", m_nNewValueIndex+i+1, strTimeDisplay);
			WriteLog(strLog);
		}
	}
	LeaveCriticalSection(&m_CriticalOper);
	
	WriteLog("CPTSetting::AddValueCol, 加入值列完毕", XJ_LOG_LV3);
}

void CPTSetting::FillHisData( int nQueryNo )
{
	if(g_PTSettingValues <= 0)
		return;
	if(m_arrSetting.GetSize() == 0)
		return;
	if(MODE_SUB != g_role && !IsCurrentQueryNo(nQueryNo))
		return;
	WriteLog("CPTSetting::FillHisData, 开始填充历史值", XJ_LOG_LV3);
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	if(m_HisValueMgr.GetCount() > 0)
	{
		for(int i = 0; i < m_List.GetItemCount(); i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo)))
				break;
			PT_SETTING* pts = (PT_SETTING*)m_List.GetItemData(i);
			if(!pts)
				continue;
			for(int k = 0; k < m_HisValueMgr.GetCount(); k++)
			{				
				CString sHisValue = m_HisValueMgr.GetValue(k, atoi(pts->ID));
				if( 0 == k )
				{					
					pts->Value = sHisValue;
				}
				m_List.SetItemText(i, m_nNewValueIndex+k, GetDisplayValue(pts, sHisValue));
			}
		}
	}
	//恢复刷新
	LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);
	WriteLog("CPTSetting::FillHisData, 历史值填充完毕", XJ_LOG_LV3);
}

CString CPTSetting::GetDisplayValue( PT_SETTING* pst, CString strValue )
{
	CString strReturn = SetFloatValue(pst, strValue);
	strReturn = GetTranslatedValue(pst, strReturn);
	if(pst->ntimeMStoS == 1)
	{
		float fValue = atof(strReturn);
		fValue /= 1000;
		strReturn.Format("%.3f", fValue);
	}

	if( pst->DataType == 2 && !strReturn.IsEmpty() ) //16进制控制字
	{
		//将16进制控制字转为0x0000 的格式，不足4位的将补全
		CString str = strReturn;
		str.MakeLower();
		int nfind = str.Find( "0x", 0 );
		if( nfind == 0 )
		{
			//以0x开头
			str.Delete( 0, 2 );
			strReturn.Delete( 0, 2 );
		}
		int noffset = 0;
		if( str.GetLength()%4 != 0 )
			noffset = 4 - str.GetLength() % 4;
		CString strAdd = "0x";
		if( nfind < 0 && g_PTSetting160x == 0 ) //本身没有0x，并且不强制添加
			strAdd = "";
		for ( int i = 0; i < noffset; i++ )
		{
			strAdd += "0";
		}
		strReturn = strAdd + strReturn;		
	}
	return strReturn;
}

void CPTSetting::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//过期的批次
		return;
	}
	//填充数据
	FillData(wParam, lParam);
}

/****************************************************
Date:2014/1/9  Author:LYH
函数名:   FillData	
返回值:   void	
功能概要: 
参数: int nQueryNo	
参数: int nLoadType	
*****************************************************/
void CPTSetting::FillData( int nQueryNo, int nLoadType )
{
	if(0 == nLoadType)
	{
		ResetGroupTab();
		//SetZoneCmbToCurrentZone();
		UpdateData(FALSE);
	}
	if(0 == nLoadType || 2 == nLoadType)
	{
		FillListData(nQueryNo);
	}
	
	AddValueCol(nQueryNo);
	FillHisData(nQueryNo);
	GetDlgItem(IDC_STATIC_PTSETTING_LOADING)->ShowWindow(SW_HIDE);
}

BOOL CPTSetting::ReLoadData( int nQueryNo, int nLoadType )
{
	BOOL bLoad = TRUE;
	//直接载入
	if(0 == nLoadType)
	{
		LoadCurrentZone();
		LoadSettingGroup(nQueryNo);
		bLoad = LoadPTSettingConfig(nQueryNo);
		if(bLoad)
		{
			//查询基准值
			LoadOrderValue(0);
		}
	}
	if(bLoad && 2 != nLoadType)
	{
		LoadPTHistorySettingData(nQueryNo);
	}
	return bLoad;
}

/****************************************************
Date:2014/1/11  Author:LYH
函数名:   GetThreadCount	
返回值:   int	
功能概要: 
*****************************************************/
int CPTSetting::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

BOOL CPTSetting::LoadDefaultCpu()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSET_CPU);
	if(pCmb == NULL)
		return TRUE;
	if( pCmb->GetCount() == 0 )
		return TRUE;
	if(m_pObj == NULL)
		return FALSE;

	WriteLog("CPTSetting::LoadDefaultCpu, 查询开始", XJ_LOG_LV3);
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	Field Field0;
	bzero(&Field0, sizeof(Field));
	Field0.FieldType = 97;
	sql.Fieldlist.push_back(Field0);

	//id
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "cpu_code", EX_STTP_DATA_TYPE_INT);
		
	//条件
	CString str = "";
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	//按cpu排序
	Condition condition5;
	str.Format("order by cpu_code");
	pApp->m_DBEngine.SetCondition(sql, condition5, str, EX_STTP_WHERE_ABNORMALITY);//非where
	
	
	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSetting::LoadDefaultCpu, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTSetting::LoadDefaultCpu,查询到%d条数据", nCount);
		WriteLogEx(str);
		if( nCount >= 1 )
		{
			str = pMemset.GetValue((UINT)0); //ID
			int c = atoi(str);
			for( int k = 0; k < pCmb->GetCount(); k++ )
			{
				CSecCPU * cpu =  (CSecCPU *)pCmb->GetItemData(k);
				if( cpu != NULL && cpu->code == c)
				{
					pCmb->SetCurSel(k);
					bReturn = TRUE;
					break;
				}
			}
		}
	}
	else
	{
		CString str;
		str.Format("CPTSetting::LoadDefaultCpu,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}

	WriteLog("CPTSetting::LoadDefaultCpu, 查询结束", XJ_LOG_LV3);
	
	return bReturn;
}

void CPTSetting::OnBtnPtsetExcel() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString sZone = m_sZone;
	if( atoi(m_sZone) == 99 )
	{
		sZone = m_sCurrentZone;
	}
	CString strName;
	strName.Format("%s(%s-%s[%s]-%s[%s])-%s.xls", StringFromID(IDS_SECPROP_SETTING),m_pObj->m_sName, 
		StringFromID(IDS_MODEL_CPU),m_sCPU, StringFromID(IDS_SECPROP_ZONE),sZone, t.Format("%Y%m%d%H%M%S"));
	//取得用户选择文件夹
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(m_List.ExportExcel(strPath))
	{
		//导出成功
		if(AfxMessageBox(StringFromID(IDS_COMMON_EXPORT_SUCCESS), MB_OKCANCEL) == IDOK)
		{
			CString str = "/n,/select,";
			str += strPath;
			ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
		}
	}
	else
	{
		//导出失败
		AfxMessageBox(StringFromID(IDS_COMMON_EXPORT_FAIL));
	}
}
