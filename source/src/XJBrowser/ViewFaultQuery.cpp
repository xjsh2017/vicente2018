// ViewFaultQuery.cpp : implementation file
//
#include "ExcelOp.h"
#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewFaultQuery.h"
#include <afxdtctl.h>
#include "DlgFaultReport.h"
#include "ViewFault.h"
#include "ManualFaultShorten.h"
#include "FaultFrame.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/** @brief           升降序作用位*/
int g_iFaultQueryViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewFaultQuery

IMPLEMENT_DYNCREATE(CViewFaultQuery, CViewBase)

//##ModelId=49B87B830244
CViewFaultQuery::CViewFaultQuery()
	: CViewBase(CViewFaultQuery::IDD)
{
	//{{AFX_DATA_INIT(CViewFaultQuery)
	m_sQueryCondition = _T("");
	m_sQueryResult = _T("");
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nSelFaultType = 99;
	m_nChooseFaultType = 2;
	m_sSelPriID = "";
	m_sSelStationID = "";
	CTime tm(1972, 1, 1, 1, 1, 1);
	m_tmSelStart = tm;
	m_tmSelEnd = tm;
    m_nIsSaved = 0;
	m_nIsTypicalcase = 0;
	m_nOverhaul = 0;
	m_listFaultReport.RemoveAll();
	InitVoltageMap();
}

//##ModelId=49B87B8303D8
CViewFaultQuery::~CViewFaultQuery()
{
	Clear();
	TRACE("CViewFaultQuery::~CViewFaultQuery \n");
}

//##ModelId=49B87B830399
void CViewFaultQuery::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewFaultQuery)
	DDX_Control(pDX, IDC_CMB_FAULT_ONE, m_cmbPriDev);
	DDX_Control(pDX, IDC_CMB_FAULT_SECOND, m_cmbSecDev);
	DDX_Control(pDX, IDC_CMB_FAULT_STATION, m_cmbStation);
	DDX_Control(pDX, IDC_BTN_FAULT_CONTINUE, m_btnContinue);
	DDX_Control(pDX, IDC_BTN_FAULT_QUERY, m_btnQuery);
	DDX_Control(pDX, IDC_LIST_FAULT_QUERY, m_List);
	DDX_Text(pDX, IDC_STATIC_FAULT_RESULT, m_sQueryCondition);
	DDX_Text(pDX, IDC_STATIC_FAULT_RESULT2, m_sQueryResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewFaultQuery, CViewBase)
	//{{AFX_MSG_MAP(CViewFaultQuery)
	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_BTN_FAULT_QUERY, OnBtnFaultQuery)
	ON_BN_CLICKED(IDC_BTN_FAULT_CONTINUE, OnBtnFaultContinue)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_FAULT_QUERY, OnClickListFaultQuery)
	ON_CBN_SELCHANGE(IDC_CMB_FAULT_STATION, OnSelchangeCmbFaultStation)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FAULT_QUERY, OnItemchangedListFaultQuery)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FAULT_QUERY, OnColumnclickListFaultQuery)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_FAULT_PRINT, OnFaultPrint)
	ON_COMMAND(IDC_FAULT_EDIT, OnFaultEdit)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_FAULT_WEB, OnFaultWeb)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_FAULT_QUERY, OnCustomDraw)
	ON_CBN_SELCHANGE(IDC_CMB_FAULT_NET, OnSelchangeCmbFaultNet)
	ON_CBN_SELCHANGE(IDC_CMB_FAULT_ONE, OnSelchangeCmbFaultOne)
	ON_CBN_SELCHANGE(IDC_CMB_FAULT_DEVICE_TYPE, OnSelchangeCmbFaultDeviceType)
	ON_CBN_SELCHANGE(IDC_CMB_FAULT_VOLTAGE, OnSelchangeCmbFaultVoltage)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDC_FAULT_EXPORT, OnFaultExport)
	ON_COMMAND(ID_MANUAL_FAULTSHORTENN, OnManualFaultShorten)
	ON_COMMAND(IDC_FAULT_DELETE, OnFaultDelete)
	ON_CBN_SELCHANGE(IDC_CMB_FAULT_TYPE, OnSelchangeCmbFaultType)
	ON_CBN_SELCHANGE(IDC_CMB_FAULT_TYPE2, OnSelchangeCmbFaultType2)
	//}}AFX_MSG_MAP
	ON_MESSAGE(FAULTREPORT_EDITED, OnFaultReportEdited)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewFaultQuery diagnostics

#ifdef _DEBUG
//##ModelId=49B87B84000F
void CViewFaultQuery::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B84001F
void CViewFaultQuery::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewFaultQuery message handlers
//##ModelId=49B87B83038B
void CViewFaultQuery::OnInitialUpdate() 
{
	//地区
	RecordRate(IDC_STATIC_FAULT_NET, 0, left_client, top_client);
	RecordRate(IDC_CMB_FAULT_NET, 1, left_client, top_client);
	//厂站
	RecordRate(IDC_STATIC_FAULT_STATION, 0, mid_client, top_client);
	RecordRate(IDC_CMB_FAULT_STATION, 1, mid_client, top_client);
	//电压等级
	RecordRate(IDC_STATIC_FAULT_VOLTAGE, 0, left_client, top_client);
	RecordRate(IDC_CMB_FAULT_VOLTAGE, 1, left_client, top_client);
	//设备类型
	RecordRate(IDC_STATIC_FAULT_DEVICE_TYPE, 0, mid_client, top_client);
	RecordRate(IDC_CMB_FAULT_DEVICE_TYPE, 1, mid_client, top_client);
	//一次设备
	RecordRate(IDC_STATIC_FAULT_ONE, 0, left_client, top_client);
	RecordRate(IDC_CMB_FAULT_ONE, 1, left_client, top_client);
	//二次设备
	RecordRate(IDC_STATIC_FAULT_SECOND, 0, mid_client, top_client);
	RecordRate(IDC_CMB_FAULT_SECOND, 1, mid_client, top_client);
	//开始时间
	RecordRate(IDC_STATIC_FAULT_TIME1, 0, left_client, top_client);
	RecordRate(IDC_DP_FAULT_TIME1, 1, left_client, top_client);
	//结束时间
	RecordRate(IDC_STATIC_FAULT_TIME2, 0, mid_client, top_client);
	RecordRate(IDC_DP_FAULT_TIME2, 1, mid_client, top_client);
	//故障相别
	RecordRate(IDC_STATIC_FAULT_TYPE, 0, mid_client, top_client);
	RecordRate(IDC_CMB_FAULT_TYPE, 1, mid_client, top_client);
	//故障类型
	RecordRate(IDC_STATIC_FAULT_TYPE2, 0, left_client, top_client);
	RecordRate(IDC_CMB_FAULT_TYPE2, 1, left_client, top_client);
	//是否已经存档
	RecordRate(IDC_STATIC_FAULT_SAVE, 0, left_client, top_client);
	RecordRate(IDC_CMB_FAULT_SAVE, 1, left_client, top_client);
	//是否检修
	RecordRate(IDC_STATIC_FAULT_OVERHAUL, 0, left_client, top_client);
	RecordRate(IDC_CMB_FAULT_OVERHAUL, 1, left_client, top_client);
	//是否是典型事故
	RecordRate(IDC_STATIC_TYPICALCASE, 0, mid_client, top_client);
	RecordRate(IDC_CMB_TYPICALCASE, 1, mid_client, top_client);
	//查询按钮
	RecordRate(IDC_BTN_FAULT_QUERY, 0, null_effect, top_client, right_client, null_effect);
	//结果
	RecordRate(IDC_STATIC_FAULT_RESULT, 1, left_client, top_client);
	RecordRate(IDC_STATIC_FAULT_RESULT2, 1, left_client, top_client);
	//继续查询按钮
	RecordRate(IDC_BTN_FAULT_CONTINUE, 0, null_effect, top_client, right_client);
	//故障类型的关联框架
	RecordRate(IDC_STATIC_FAULT_TYPE3, 1, left_client, top_client);
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	//初始化列表
 	InitListCtrl();
 	//初始化其它控件
 	InitControls();

	ShowResult(FALSE);
	//填充下拉框
	FillNet(); //地区
	FillStation(); //厂站
	FillVoltage(NULL); //电压等级
	FillDeviceType(); //设备类型
	FillPriDevice(NULL, NULL, 0);//一次设备
	FillSecDevice(NULL, NULL, NULL, 0);  //二次设备
	FillFaultType();//故障相别
	FillFaultType2();//故障类型
	FillStartTime();//起始时间

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_SAVE);
	//清空所有数据
	pCmb->ResetContent();
	pCmb->AddString(StringFromID(IDS_COMMON_YES));
	pCmb->AddString(StringFromID(IDS_COMMON_NO));
	int index = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
    pCmb->SetCurSel(index);
	//加入"所有"选项

	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_TYPICALCASE);
	//清空所有数据
	pCmb->ResetContent();
	pCmb->AddString(StringFromID(IDS_COMMON_YES));
	pCmb->AddString(StringFromID(IDS_COMMON_NO));
	index = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
    pCmb->SetCurSel(index);

	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_OVERHAUL);
	//清空所有数据
	pCmb->ResetContent();
	pCmb->AddString(StringFromID(IDS_OVERHUAL_NO));
	pCmb->AddString(StringFromID(IDS_OVERHUAL_YES));
	index = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
    pCmb->SetCurSel(index);

	if (g_Temp)
	{
		GetDlgItem(IDC_STATIC_FAULT_SAVE)->SetWindowText(StringFromID(IDS_FAULTREPORT_ISTRUEFALUT)+":");
	}

}

//##ModelId=49B87B84004E
void CViewFaultQuery::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
}

//##ModelId=49B87B8303B9
void CViewFaultQuery::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_FAULTREPORT));
//	GetParentFrame()->SetWindowText("事故报告");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B8302A2
int CViewFaultQuery::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B8302FD
int CViewFaultQuery::InitListStyle()
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
		lvCol.fmt=LVCFMT_CENTER;
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
		//char* arColumn[14]={"序号", "事故包名称","创建时间","故障时间","故障元件","厂站1","厂站2","故障类型","录波测距","是否检修", "是否已存档", "典型故障", "详细原因",""};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 14 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 60; //序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 240; // 事故包名称
				colName = StringFromID(IDS_FAULTREPORT_NAME);
				break;
			case 2:
				lvCol.cx = 180; //创建时间
				colName = StringFromID(IDS_COMMON_CREATETIME);
				break;
			case 3://
				lvCol.cx = 180; // 故障时间
				colName = StringFromID(IDS_FAULT_TIME);
				break;
			case 4://
				lvCol.cx = 150; // 故障元件
				colName = StringFromID(IDS_FAULT_DEVICE);
				break;
			case 5://
				lvCol.cx = 100; // 厂站1
				colName = StringFromID(IDS_FAULT_STATION1);
				break;
			case 6:
				lvCol.cx = 100; //厂站2
				colName = StringFromID(IDS_FAULT_STATION2);
				break;
			case 7:
				lvCol.cx = 100; //故障类型
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			case 8:
				lvCol.cx = 80; //故障测距
				colName = StringFromID(IDS_FAULT_LOCATION);
				break;
			case 9:
				lvCol.cx = 90; //是否检修
				colName = StringFromID(IDS_FAULTREPORT_ISOVERHAUL);
				break;
			case 10:
				lvCol.cx = 90; //是否已存档
				colName = StringFromID(IDS_FAULTREPORT_ISSAVED);
				break;
			case 11:
				lvCol.cx = 90; //典型故障
				colName = StringFromID(IDS_FAULTREPORT_ISTYPICAL);
				break;
			case 12:
				lvCol.cx = 90; //详细原因
				colName = StringFromID(IDS_COMMON_DETAILREASON);
				break;
			case 13:
				lvCol.cx = 90; //""
				colName = "";
				break;
			default:
				lvCol.cx=90;//列宽100象素
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(13, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 函 数 名：InitControls()
 功能概要：初始化其它控件
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B8302AF
int CViewFaultQuery::InitControls()
{
	/*
	m_btnQuery.SetThemeHelper(&m_ThemeHelper);
//	m_btnQuery.SetIcon(IDI_SEARCH);
	m_btnQuery.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnQuery.SetTooltipText(_T("查询事故报告"));
	*/

	return 0;
}

/*************************************************************
 函 数 名：RegulateControlSize()
 功能概要：调整控件位置
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8302BF
void CViewFaultQuery::RegulateControlSize()
{
	
}

/*************************************************************
 函 数 名：ShowResult()
 功能概要：显示还是隐藏查询结果
 返 回 值: 
 参    数：param1	显示或隐藏
		   Param2
**************************************************************/
//##ModelId=49B87B8302CE
void CViewFaultQuery::ShowResult( BOOL bShow )
{
	UINT nShowResult;
	if(bShow)
		nShowResult = SW_SHOW;
	else
		nShowResult = SW_HIDE;
	UINT nShowCondition;
	if(bShow)
		nShowCondition = SW_HIDE;
	else
		nShowCondition = SW_SHOW;
	//查询条件
	CWnd* pWnd = GetDlgItem(IDC_STATIC_FAULT_NET);
	if(pWnd != NULL){
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_FAULT_NET);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_STATION);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_FAULT_STATION);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_VOLTAGE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_FAULT_VOLTAGE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_DEVICE_TYPE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_ONE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_FAULT_ONE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_SECOND);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_FAULT_SECOND);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_TIME1);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_DP_FAULT_TIME1);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_TIME2);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_DP_FAULT_TIME2);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_TYPE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_TYPE2);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_TYPE3);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_FAULT_TYPE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_FAULT_TYPE2);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_SAVE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_FAULT_SAVE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_TYPICALCASE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_TYPICALCASE);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_OVERHAUL);
		pWnd->ShowWindow(nShowCondition);
		pWnd = GetDlgItem(IDC_CMB_FAULT_OVERHAUL);
		pWnd->ShowWindow(nShowCondition);
		
		m_btnQuery.ShowWindow(nShowCondition);
		//查询结果
		pWnd = GetDlgItem(IDC_STATIC_FAULT_RESULT);
		pWnd->ShowWindow(nShowResult);
		pWnd = GetDlgItem(IDC_STATIC_FAULT_RESULT2);
		pWnd->ShowWindow(nShowResult);
		m_btnContinue.ShowWindow(nShowResult);
		pWnd = GetDlgItem(IDC_LIST_FAULT_QUERY);
		pWnd->ShowWindow(nShowResult);
	}
}

//##ModelId=49B87B84006D
void CViewFaultQuery::OnBtnFaultQuery() 
{
	// TODO: Add your control notification handler code here
	try
	{
		CWnd * pWnd = GetParent()->GetParent()->GetParent();
		if(pWnd != NULL)
		{
			//向父窗口发送消息, 选择了空行,发送空指针
			pWnd ->PostMessage(FAULTREPORT_SEL_CHANGED, 0, (LPARAM)NULL);
		}
		
		
		g_wndWait.Show();
		if(!QueryFaultReport())
		{
			g_wndWait.Hide();
			return;
		}
		
	}
	catch(...)
	{
		WriteLog("CViewFaultQuery::OnBtnFaultQuery(), 查询故障包出错");
		return;
	}
	
	try
	{
		//填充列表
		FillListData();
	}
	catch (...)
	{
		WriteLog("CViewFaultQuery::OnBtnFaultQuery(), 填充列表数据出错");
		return;
	}
	
	try
	{
		//显示结果
		ShowResult(TRUE);
		//
		UpdateData(FALSE);
	}
	catch(...)
	{
		WriteLog("CViewFaultQuery::OnBtnFaultQuery(), 更新数据显示出错");
		return;
	}
	g_wndWait.Hide();
}

//##ModelId=49B87B84007D
void CViewFaultQuery::OnBtnFaultContinue() 
{
	// TODO: Add your control notification handler code here
	ShowResult(FALSE);
}

//##ModelId=49B87B84007F
void CViewFaultQuery::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_FAULT_QUERY, left_client, IDC_BTN_FAULT_CONTINUE, right_client, bottom_client);
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8302DE
BOOL CViewFaultQuery::LoadListConfig()
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
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!");
			WriteLog("ViewFault::读取UIConfig失败,将使用默认列风格");
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
				WriteLog(str);
				WriteLog("ViewFaultQuery::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewFaultQueryConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewFaultQueryConfig");
				WriteLog("ViewFaultQuery::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewFaultQueryConfig下的配置");
					WriteLog("ViewFaultQuery::读取UIConfig失败,将使用默认列风格");
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

//##ModelId=49B87B8302EE
BOOL CViewFaultQuery::SaveListConfig()
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
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!");
			WriteLog("ViewFaultQuery::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog(str);
				WriteLog("ViewFaultQuery::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewFaultQueryConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewFaultQueryConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewFaultQueryConfig");
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
						WriteLog("未找到ViewFaultQueryConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了ViewFaultQueryConfig下的配置, 先删除新建所有子节点");
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

//##ModelId=49B87B8302EF
BOOL CViewFaultQuery::NeedSave()
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
	else if(m_arrColum.GetSize() != nCount)
	{
		//如果数量不相等,需要保存
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
		for(int i = 0; i < nCount; i++)
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

//##ModelId=49B87B84009C
void CViewFaultQuery::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();

	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here	
}

//##ModelId=49B87B8400AB
void CViewFaultQuery::OnClickListFaultQuery(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	int nSubItem = pNMListView->iSubItem;
	if(nSelected < 0)
	{
		//父窗口是分割窗口
		CWnd * pWnd = GetParent()->GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//向父窗口发送消息, 选择了空行,发送空指针
		pWnd ->PostMessage(FAULTREPORT_SEL_CHANGED, 0, (LPARAM)NULL);
	}

	if (nSubItem == 10)
	{ 
		CFaultReport* pFaultReport = (CFaultReport*)m_List.GetItemData(nSelected);
		CString str;
		if (pFaultReport == NULL)
		{
			return;
		}
		str.Format("%s[%s]?", StringFromID(IDS_TIP_FAULTREPORT_SWITCH),
			(pFaultReport->m_bIsSaved == 0 ? StringFromID(IDS_ARCHIVE_YES):StringFromID(IDS_ARCHIVE_NO)));
		if (AfxMessageBox(str, MB_YESNO) == IDYES)
		{
			CString strTemp;
			m_List.GetItemText(nSelected, nSubItem, strTemp.GetBuffer(2), 2);
           
			if (!strTemp.CompareNoCase(StringFromID(IDS_COMMON_YES)))
			{
				pFaultReport->m_bIsSaved = 0;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_COMMON_NO));
					if (IssueToWeb(pFaultReport) != 0)
					{
						AfxMessageBox(StringFromID(IDS_FAULTREPORT_SYNC_FAIL));
					}
				}
				else
				{
					pFaultReport->m_bIsSaved = 1;
					AfxMessageBox(StringFromID(IDS_COMMON_UPDATEDB_FAILED));
				}
			}
			else
			{
				pFaultReport->m_bIsSaved = 1;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_COMMON_YES));
					if (IssueToWeb(pFaultReport) != 0)
					{
						AfxMessageBox(StringFromID(IDS_FAULTREPORT_SYNC_FAIL));
					}
				}
				else
				{
					pFaultReport->m_bIsSaved = 0;
					AfxMessageBox(StringFromID(IDS_COMMON_UPDATEDB_FAILED));
				}
			}
		}
	}
	if (nSubItem == 11)
	{ 
		CFaultReport* pFaultReport = (CFaultReport*)m_List.GetItemData(nSelected);
		CString str;
		if (pFaultReport == NULL)
		{
			return;
		}
		str.Format("%s[%s]?", StringFromID(IDS_TIP_FAULTREPORT_SWITCH),
			(pFaultReport->m_nTypicalcase == 0 ? StringFromID(IDS_TYPICAL_YES):StringFromID(IDS_TYPICAL_NO)));
		if (AfxMessageBox(str, MB_YESNO) == IDYES)
		{
			CString strTemp;
			m_List.GetItemText(nSelected, nSubItem, strTemp.GetBuffer(2), 2);
			
			if (!strTemp.CompareNoCase(StringFromID(IDS_COMMON_YES)))
			{
				pFaultReport->m_nTypicalcase = 0;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_COMMON_NO));
					if (IssueToWeb(pFaultReport) != 0)
					{
						AfxMessageBox(StringFromID(IDS_FAULTREPORT_SYNC_FAIL));
					}
				}
				else
				{
					pFaultReport->m_nTypicalcase = 1;
					AfxMessageBox(StringFromID(IDS_COMMON_UPDATEDB_FAILED));
				}
			}
			else
			{
				pFaultReport->m_nTypicalcase = 1;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_COMMON_YES));
					if (IssueToWeb(pFaultReport) != 0)
					{
						AfxMessageBox(StringFromID(IDS_FAULTREPORT_SYNC_FAIL));
					}
				}
				else
				{
					pFaultReport->m_nTypicalcase = 0;
					AfxMessageBox(StringFromID(IDS_COMMON_UPDATEDB_FAILED));
				}
			}
		}
	}
	if (nSubItem == 12)
	{
		int nCount = m_List.GetSelectedCount();
		if(nCount != 1)
			return;
		int nIndex = -1;
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex == -1)
			return;
		CFaultReport* pReport = (CFaultReport*)m_List.GetItemData(nIndex);
		if(pReport == NULL)
			return;
		CDlgFaultReport dlg;
		LV_COLUMN lvCol;
		lvCol.mask   =   LVCF_TEXT;
		char temp[256];
		for(int j = 0; j < 256; j++)
		{
			temp[j] = '\0';
		}
		lvCol.cchTextMax = 256;
		lvCol.pszText = temp;
		m_List.GetColumn(9, &lvCol);
		dlg.m_sTitle = lvCol.pszText;
		dlg.m_pWnd = this;
		dlg.m_pFaultReport = pReport;
		dlg.m_nCurTabSel = 5;
		dlg.DoModal();
	}
	if(nSubItem == 13)
	{
		//故障回放
		CFaultReport* pFaultReport = (CFaultReport*)m_List.GetItemData(nSelected);
		CString sPath = pFaultReport->GetFisPath();
		if(!sPath.IsEmpty())
		{
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString sDir = pApp->GetDownDir();
			CString sFullPath = sDir + sPath;
			OpenXJFaultInversion(sFullPath, GetSafeHwnd());
			CString sLog;
			sLog.Format("执行故障回放脚本,脚本路径:%s", sFullPath);
			WriteLog(sLog);
		}
		else
			AfxMessageBox(StringFromID(IDS_TIP_FAULTREPORT_NO_PLAYBACK_SCRIPT));
	}
	*pResult = 0;
}

/*************************************************************
 函 数 名：FillNet()
 功能概要：填充地区下拉框
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CViewFaultQuery::FillNet()
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_NET);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	POSITION pos = pData->m_NetList.GetHeadPosition();
	while(pos != NULL)
	{
		CNetObj* pNet = (CNetObj*)pData->m_NetList.GetNext(pos);
		if(pNet == NULL)
			continue;
		//加入到下拉框
		int nIndex = pCmb->AddString(/*pNet->m_sID*/pNet->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pNet);
		}
	}
	//加入"所有"选项
	int nIndex = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
}


/*************************************************************
 函 数 名：FillStation()
 功能概要：填充厂站下拉框
 返 回 值: void
 参    数：param1 地区指针
		   Param2
**************************************************************/
//##ModelId=49B87B83030D
void CViewFaultQuery::FillStation(CNetObj* pNet)
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if(pStation == NULL)
			continue;
		//加入到下拉框
		int nIndex;
		if (pNet == NULL || pNet != NULL && !pNet->m_sID.CompareNoCase(pStation->m_sNetID))
		{
			nIndex = pCmb->AddString(pStation->m_sName);
			if (pNet != NULL)
			{
				FillPriDevice(pStation, NULL, 0);
				FillSecDevice(pStation, NULL, NULL, 0);	
			}
			if(nIndex >= 0)
			{
				pCmb->SetItemData(nIndex, (DWORD)pStation);
			}
		}
	}
	//加入"所有"选项
	int nIndex = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	m_cmbStation.OnDataFilled();
}

/*************************************************************
 函 数 名：FillVoltage()
 功能概要：填充电压等级
 返 回 值: 
 参    数：param1	厂站指针
		   Param2
**************************************************************/
void CViewFaultQuery::FillVoltage(CStationObj* pStationObj)
{
   //检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_VOLTAGE);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (pApp == NULL)
	{
		return;	
	}
	CStationObj* pStation = NULL;
	int nIndex = 0;
	int nKey = 0;
	CString strTemp = "";

	if (pStationObj == NULL)
	{
		CComboBox* pNetCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_NET);
		int nSelNet = pNetCmb->GetCurSel();
		CString strNet = _T(""); 
		pNetCmb->GetLBText(nSelNet, strNet);
		if (!strNet.CompareNoCase(StringFromID(IDS_COMMON_ALL)))
		{
			POSITION pos = m_VoltageMap.GetStartPosition();
			while (pos != NULL)
			{
				m_VoltageMap.GetNextAssoc(pos, nKey, (CStationObj*&)pStation);
				strTemp.Format("%dkV", nKey);
				nIndex = pCmb->AddString(strTemp);
				
			}
		}
		else
		{
			MAP_VOLTAGE VoltageMap;
			InitVoltageMap(strNet, &VoltageMap);
			POSITION pos = VoltageMap.GetStartPosition();
			while (pos != NULL)
			{
				VoltageMap.GetNextAssoc(pos, nKey, (CStationObj*&)pStation);
				strTemp.Format("%dkV", nKey);
				nIndex = pCmb->AddString(strTemp);
			}
		}
	}
	else
	{	
		for(int i = 0; i < pStationObj->m_arrVoltage.GetSize(); i++)
		{
			strTemp.Format("%dkV", pStationObj->m_arrVoltage[i]);
			nIndex = pCmb->AddString(strTemp);
		}
	}

	nIndex = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
	{
		pCmb->SetCurSel(nIndex);
	}
}

/*************************************************************
 函 数 名：FillDeviceType()
 功能概要：填充设备类型
 返 回 值: 
 参    数：param1	
		   Param2
**************************************************************/
void CViewFaultQuery::FillDeviceType()
{
  //检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	//加入到下拉框
	int nIndex = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	pCmb->AddString(StringFromID(IDS_MODEL_BUS));
    pCmb->AddString(StringFromID(IDS_MODEL_LINE));
    pCmb->AddString(StringFromID(IDS_MODEL_BREAKER));
    pCmb->AddString(StringFromID(IDS_MODEL_GEN));
	pCmb->AddString(StringFromID(IDS_MODEL_REACTOR));
    pCmb->AddString(StringFromID(IDS_MODEL_TRANS));
    pCmb->AddString(StringFromID(IDS_MODEL_SWITCH));
    pCmb->AddString(StringFromID(IDS_MODEL_CAP));
}

/*************************************************************
 函 数 名：FillPriDevice()
 功能概要：填充一次设备下拉框
 返 回 值: 
 参    数：param1	指定一次设备所属厂站, 为空时表示所有厂站
		   Param2   电压等级
**************************************************************/
//##ModelId=49B87B83030E
void CViewFaultQuery::FillPriDevice(CStationObj* pStation, int nVoltage, int nDeviceType)
{
	//检查数据有效性
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_ONE);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	
	//填充新数据
	if(pStation != NULL)
	{
		if (nDeviceType != 0)
		{
			FillPriDeviceByType(pCmb, pStation, nDeviceType, nVoltage);
		}
		else
		{
			//指定厂站
			FillAllPriDevice(pCmb, pStation, nDeviceType, nVoltage);
		}
	}
	else
	{
		CComboBox* pCmbStation = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
		if (pCmbStation == NULL)
		{
			return;
		}

		//所有厂站
		for (int i = 0; i < pCmbStation->GetCount(); i++)
		{
			CStationObj* pStation1 = (CStationObj*)pCmbStation->GetItemData(i);
			if(pStation1 == NULL)
				continue;
			if (nDeviceType != 0)
			{
				FillPriDeviceByType(pCmb, pStation1, nDeviceType, nVoltage);
			}
			else
			{
				//加入到下拉框
				FillAllPriDevice(pCmb, pStation1, nDeviceType, nVoltage);
			}
		}
	}
	
	//加入"所有"选项
	int nIndex = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);

	m_cmbPriDev.OnDataFilled();
}

/*************************************************************
 函 数 名：FillSecDevice()
 功能概要：填充二次设备下拉框
 返 回 值: 
 参    数：param1	指定一次设备所属厂站, 为空时表示所有厂站
		   Param2   一次设备指针
		   Param3   电压等级
		   Param4   设备类型
**************************************************************/
void CViewFaultQuery::FillSecDevice(CStationObj* pStation, CDeviceObj* pDevice, int nVoltage, int nDeviceType)
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_SECOND);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	if (pStation != NULL)
	{
			FillSecDeviceMap(pStation, pDevice, nVoltage, nDeviceType);
	}
	else
	{
		CComboBox* pCmbStation = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
		if (pCmbStation == NULL)
		{
			return;
		}

		//所有厂站
		for (int i = 0; i < pCmbStation->GetCount(); i++)
		{
			CStationObj* pStation = (CStationObj*)pCmbStation->GetItemData(i);
			if(pStation == NULL)
				continue;
			FillSecDeviceMap(pStation, pDevice, nVoltage, nDeviceType);
		}
	}
	//加入"所有"选项
	int nIndex = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
	{
		pCmb->SetCurSel(nIndex);
	}
	m_cmbSecDev.OnDataFilled();
}


/*************************************************************
 函 数 名：FillSecDeviceMap()
 功能概要：填充指定厂站下的二次设备
 返 回 值: 
 参    数：param1  厂站指针	
		   Param2  一次设备指针
		   Param3  电压等级
		   param4  设备类型
**************************************************************/
void CViewFaultQuery::FillSecDeviceMap(CStationObj* pStation, CDeviceObj* pDevice, int nVoltage, int nDeviceType)
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_SECOND);
	if(pCmb == NULL)
		return;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_PROTECT, pStation->m_sID);

	POSITION pos = listDevice.GetHeadPosition();
	int nIndex;
	while(pos != NULL)
	{
		CSecObj* pSecObj = (CSecObj*)listDevice.GetNext(pos);
		if (pSecObj == NULL)
		{
			continue;
		}
		if(pSecObj->m_pOwner == NULL)
			continue;
		if (pDevice == NULL || (pDevice != NULL && !pSecObj->m_pOwner->m_sName.CompareNoCase(pDevice->m_sName)))
		{
			if (nVoltage == 0 || (nVoltage != 0 && pSecObj->m_pOwner->m_nVoltage == nVoltage))
			{
				if (nDeviceType == 0 || (nDeviceType != 0 && nDeviceType == pSecObj->m_pOwner->m_nType))
				{
					nIndex = pCmb->AddString(pSecObj->m_sName);
					if (nIndex >= 0)
					{
						pCmb->SetItemData(nIndex, (DWORD)pSecObj);
					}
				}
			}
		}
	}
}

/*************************************************************
 函 数 名：FillSpecificPriDevice()
 功能概要：将指定厂站下的指定类型的一次设备填充到指定下拉框 
 返 回 值: void
 参    数：param1	下拉框指针
		   Param2	厂站指针
		   param3	一次设备类型
		   param4   电压等级
**************************************************************/
//##ModelId=49B87B83031C
void CViewFaultQuery::FillSpecificPriDevice( CComboBox* pCmb, CStationObj* pStation, int nType, int nVoltage )
{
	//检查数据有效性
	if(pCmb == NULL)
		return;
	if(pStation == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, nType, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		if (nVoltage == 0 || (nVoltage != 0 && pObj->m_nVoltage == nVoltage))
		{	
			//加入到下拉框
			//if (pObj->m_bHasSecDevice)
			//{
				int nIndex = pCmb->AddString(pObj->m_sName);
				if(nIndex >= 0)
				{
					pCmb->SetItemData(nIndex, (DWORD)pObj);
				}
			//}
		}
	}
}

/*************************************************************
 函 数 名：FillFaultType()
 功能概要：填充故障相别下拉框
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B83032E
void CViewFaultQuery::FillFaultType()
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_TYPE);
	if(pCmb == NULL)
		return;
	//清空所有数据
	pCmb->ResetContent();
	//填充数据
	if (m_nChooseFaultType == 2)
	{
		// 	0	未知
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_UNKNOWN));
	}
	if (m_nChooseFaultType == 0 || m_nChooseFaultType == 2)
	{
		// 	1 A相接地
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_A));
		// 	2 B相接地
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_B));
		// 	3 C相接地
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_C));
	}
	if (m_nChooseFaultType == 1 || m_nChooseFaultType == 2)
	{
		// 	4 AB两相短路
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_AB));
		// 	5 BC两相短路
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_BC));
		// 	6 CA两相短路
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_CA));
		// 	7 AB两相短路接地
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_ABN));
		// 	8 BC两相短路接地
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_BCN));
		// 	9 CA两相短路接地
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_CAN));
		// 	10 ABC三相短路
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_ABC));
		// 	11 ABC三相短路接地
		pCmb->AddString(StringFromID(IDS_FAULTTYPE_ABCN));
	}

	//加入"所有"选项
	int nIndex = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
}
/*************************************************************
 函 数 名：FillFaultType2()
 功能概要：填充故障类型下拉框
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B83032E
void CViewFaultQuery::FillFaultType2()
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_TYPE2);
	if(pCmb == NULL)
		return;
	//清空所有数据
	pCmb->ResetContent();
	if (m_nChooseFaultType == 2 || m_nChooseFaultType == 0)
	{
		//填充数据
	// 	0	未知
	pCmb->AddString(StringFromID(IDS_FAULT_SINGLEPHASE));
	}
	if (m_nChooseFaultType == 2 || m_nChooseFaultType == 1)
	{	
		// 	1 A相接地
		pCmb->AddString(StringFromID(IDS_FAULT_BETWEENPHASE));
	}
	

	//加入"所有"选项
	int nIndex = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
}
//##ModelId=49B87B8400BB
void CViewFaultQuery::OnSelchangeCmbFaultStation() 
{
	// TODO: Add your control notification handler code here
	//重新填充一次设备下拉框,二次设备下拉框
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
	FillVoltage(pStation);
	//一次设备
	FillPriDevice(pStation, NULL, 0);
	//二次设备
	FillSecDevice(pStation, NULL, NULL, 0);

}

/*************************************************************
 函 数 名：FillStartTime()
 功能概要：填充起始时间选择框
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B83033C
void CViewFaultQuery::FillStartTime()
{
	WriteLog("CViewFaultQuery::FillStartTime, 开始操作");
	//检查数据有效性
 	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_FAULT_TIME1);
 	if(pTime == NULL)
 		return;
 	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_FAULT_TIME2);
 	if(pTime2 == NULL)
 		return;
 	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
 	pTime2->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//设置范围
	CTime tmMin(1971, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	pTime2->SetRange(&tmMin, &tmMax);

	//设置时间
	CTime tm = CTime::GetCurrentTime();
	CTimeSpan ts(30, 0, 0, 0);
	CTime tm1 = tm - ts;
	VERIFY(pTime->SetTime(&tm1));
}

/*************************************************************
 函 数 名：GetAllCurSel()
 功能概要：取得所有控件的当前选择值
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B83033D
void CViewFaultQuery::GetAllCurSel()
{
	UpdateData(TRUE);
	//清空原来的值
	m_sSelStationID = "";
	m_sSelPriID = "";
	m_nSelFaultType = 99;
	CTime tm(1972, 1, 1, 1, 1, 1);
	m_tmSelStart = tm;
	m_tmSelEnd = tm;
	m_sQueryCondition = StringFromID(IDS_QUERY_CONDITION)+":";
	CString strTemp;
    int nSel;
	//地区
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_NET);
	if(pCmb != NULL)
	{
		nSel = pCmb->GetCurSel();
		if(nSel >= 0)
		{
			CNetObj* pNet = (CNetObj*)pCmb->GetItemData(nSel);
			if(pNet != NULL)
				m_sSelNet = pNet->m_sNote;
		}
	}
	//厂站
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	if(pCmb != NULL)
	{
		nSel = pCmb->GetCurSel();
		if(nSel >= 0)
		{
			CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
			if(pStation != NULL)
				m_sSelStationID = pStation->m_sID;
			pCmb->GetLBText(nSel, strTemp);
			m_sQueryCondition += "[";
			m_sQueryCondition += strTemp;
			m_sQueryCondition += "],";
		}
	}
	
	//电压等级
    pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_VOLTAGE);
	if(pCmb != NULL)
	{
		nSel = pCmb->GetCurSel();
		if(nSel >= 0)
		{
			pCmb->GetLBText(nSel, strTemp);
			strTemp.TrimRight("kV");
			m_nSelVoltage = atoi((LPCSTR)strTemp);
			
		}
	}
	//设备类型
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	if(pCmb != NULL)
	{
		nSel = pCmb->GetCurSel();
		if(nSel >= 0)
		{
			m_nSelDeviceType = nSel;
		}
	}
	//一次设备
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_ONE);
	if(pCmb != NULL)
	{
		nSel = pCmb->GetCurSel();
		if(nSel >= 0)
		{
			CDeviceObj* pObj = (CDeviceObj*)pCmb->GetItemData(nSel);
			if(pObj != NULL)
				m_sSelPriID = pObj->m_sID;
			pCmb->GetLBText(nSel, strTemp);
			m_sQueryCondition += "[";
			m_sQueryCondition += strTemp;
			m_sQueryCondition += "],";
		}
	}
	//二次设备
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_SECOND);
	if(pCmb != NULL)
	{
		nSel = pCmb->GetCurSel();
		if(nSel >= 0)
		{
			pCmb->GetLBText(nSel, m_sSelSecDevice);
		}
	}
	//故障相别
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_TYPE);
	if(pCmb != NULL)
	{
		nSel = pCmb->GetCurSel();
		if(nSel >= 0 && nSel < 12)
		{
			m_nSelFaultType = nSel;
		}
		if(nSel >= 0)
		{
			pCmb->GetLBText(nSel, strTemp);
			m_sQueryCondition += "[";
			m_sQueryCondition += strTemp;
			m_sQueryCondition += "],";
		}
	}
	//故障类型
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_TYPE2);
	if (pCmb != NULL)
	{
		m_nChooseFaultType = pCmb->GetCurSel();
	}
		
	//起始时间
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_FAULT_TIME1);
	if(pTime != NULL)
	{
		pTime->GetTime(m_tmSelStart);
		strTemp = m_tmSelStart.Format("%Y%m%d%H%M%S");
		m_sQueryCondition += "[";
		m_sQueryCondition += strTemp;
		m_sQueryCondition += "],";
	}
	//结束时间
	pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_FAULT_TIME2);
	if(pTime != NULL)
	{
		pTime->GetTime(m_tmSelEnd);
		strTemp = m_tmSelEnd.Format("%Y%m%d%H%M%S");
		m_sQueryCondition += "[";
		m_sQueryCondition += strTemp;
		m_sQueryCondition += "]";
	}

	//是否存档
	 pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_SAVE);
	 if (pCmb != NULL)
	 {
		 int nSel = pCmb->GetCurSel();
		 if(nSel >= 0)
		 {
			 pCmb->GetLBText(nSel, strTemp);
			 if (!strTemp.CompareNoCase(StringFromID(IDS_COMMON_YES)))
			 {
				 m_nIsSaved = 1;
			 }
			 else if(!strTemp.CompareNoCase(StringFromID(IDS_COMMON_NO)))
			 {
				 m_nIsSaved = 0;
			 }
			 else
			 {
				 m_nIsSaved = 2;
			 }
		}
	 }
	 //是否是典型事故
	 pCmb = (CComboBox*)GetDlgItem(IDC_CMB_TYPICALCASE);
	 if (pCmb != NULL)
	 {
		 int nSel = pCmb->GetCurSel();
		 if(nSel >= 0)
		 {
			 pCmb->GetLBText(nSel, strTemp);
			 if (!strTemp.CompareNoCase(StringFromID(IDS_COMMON_YES)))
			 {
				 m_nIsTypicalcase = 1;
			 }
			 else if(!strTemp.CompareNoCase(StringFromID(IDS_COMMON_NO)))
			 {
				 m_nIsTypicalcase = 0;
			 }
			 else
			 {
				 m_nIsTypicalcase = 2;
			 }
		 }
	 }
	 pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_OVERHAUL);
	 if(pCmb != NULL)
	 {
		 int nSel = pCmb->GetCurSel();
		 if(nSel == 0 || nSel == 1)
		 {
			 m_nOverhaul = nSel;
		 }
		 else
			 m_nOverhaul = 2;
	 }
}

/*************************************************************
 函 数 名：QuerySpecificPriDevice()
 功能概要：根据一次设备查询故障归档数据
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
BOOL CViewFaultQuery::QuerySpecificPriDevice()
{

	BOOL bReturn = TRUE;
	BOOL bComparePriID = FALSE;
	//组织查询条件
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	MYASSERT_BOOL(pApp);
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	//条件
	CString str = "";
	
	CComboBox* pCmbOne = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_ONE);
//	MYASSERT_BOOL(pCmbOne);
	int nSelOne = pCmbOne->GetCurSel();
	CDeviceObj* pDeviceObj = (CDeviceObj*)pCmbOne->GetItemData(nSelOne);
//	MYASSERT_BOOL(pDeviceObj);
	
	CComboBox* pCmbSec = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_SECOND);
//	MYASSERT_BOOL(pCmbSec);

	int nSelSec = pCmbSec->GetCurSel();
	CSecObj* pSecObj = (CSecObj*)pCmbSec->GetItemData(nSelSec);
//	MYASSERT_BOOL(pSecObj);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	
	if (pDeviceObj != NULL && pSecObj == NULL)
	{
		//厂站
		Condition con1;
		if(m_sSelStationID != "")
		{
			str.Format("(STATION1_ID='%s' or STATION2_ID='%s')", m_sSelStationID, m_sSelStationID);
			pApp->m_DBEngine.SetCondition(sql, con1, str);
		}
		//一次设备
		Condition con2;
		if(m_sSelPriID != "")
		{
			str.Format("PRIDEVICEID='%s'", m_sSelPriID);
			pApp->m_DBEngine.SetCondition(sql, con2, str);
		}
	}
	else if (pSecObj != NULL)
	{
		Condition con1;
		str.Format("ID in (select MainRecID from tb_FaultShortenEvent where PTID = '%s')", pSecObj->m_sID);
		pApp->m_DBEngine.SetCondition(sql, con1, str);
	}
	else
	{
		//厂站
		Condition con1;
		if(m_sSelStationID != "")
		{
			str.Format("(STATION1_ID='%s' or STATION2_ID='%s')", m_sSelStationID, m_sSelStationID);
			pApp->m_DBEngine.SetCondition(sql, con1, str);
		}
		bComparePriID = TRUE;
		/*
		str = "PriDeviceID in (";
		for (int i = 0; i < pCmbOne->GetCount(); i++)
		{
		pDeviceObj = (CDeviceObj*)pCmbOne->GetItemData(i);
		if (pDeviceObj != NULL)
		{
		str.Format(str + "'%s',", pDeviceObj->m_sID);
		}
		}
		str.TrimRight(",");
		str += ")";
		if (str.GetLength() > 600)
		{
		AfxMessageBox(_T("查询条件过少，请重新选择"));
		return FALSE;
		}
		Condition con1;
		pApp->m_DBEngine.SetCondition(sql, con1, str);
		*/
	}
	
	//故障类型
	Condition con3;
	//获取故障类型
	int FaultType=-1;
	if (m_nChooseFaultType==0)
	{
		FaultType = m_nSelFaultType + 1;
	}
	else if (m_nChooseFaultType==1)
	{
		FaultType = m_nSelFaultType + 4;
	}
	else
	{
		FaultType = m_nSelFaultType;
	}
	if(FaultType >= 0 && FaultType <= 12)
	{
		//过滤选择相间故障或者单相故障
		if ((m_nChooseFaultType == 0 && FaultType <= 3) || m_nChooseFaultType == 1 && FaultType > 3 && FaultType < 12 || m_nChooseFaultType == 2)
		{
			str.Format("FAULTTYPE=%d", FaultType);
			pApp->m_DBEngine.SetCondition(sql, con3, str);
		}
		else if (FaultType == 4 && m_nChooseFaultType == 0)
		{
			//过滤选择相间故障或者单相故障
			str.Format("FaultType >0 AND Faulttype <= 3");
			pApp->m_DBEngine.SetCondition(sql, con3, str);
		}
		else if (m_nChooseFaultType == 1 && FaultType == 12)
		{
			str.Format("FaultType >=4 AND Faulttype <= 11");
			pApp->m_DBEngine.SetCondition(sql, con3, str);
		}
	}
	//起始时间
	Condition con4;
	str.Format("FAULTTIME > '%s'", m_tmSelStart.Format("%Y%m%d%H%M%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//结束时间
	Condition con5;
	str.Format("FAULTTIME < '%s'", m_tmSelEnd.Format("%Y%m%d%H%M%S"));
	pApp->m_DBEngine.SetCondition(sql, con5, str);
	
	
	//是否存档
	if (m_nIsSaved != 2)
	{
		Condition con6;
		str.Format("REVERSE2 = %d", m_nIsSaved);
		pApp->m_DBEngine.SetCondition(sql, con6, str);
	}
	
		//是否存档
	if (m_nIsTypicalcase != 2)
	{
		Condition con7;
		str.Format("TYPICALCASE = %d", m_nIsTypicalcase);
		pApp->m_DBEngine.SetCondition(sql, con7, str);
	}

	if(m_nOverhaul != 2)
	{
		Condition con8;
		str.Format("overhaul_flag = %d", m_nOverhaul);
		pApp->m_DBEngine.SetCondition(sql, con8, str);
	}
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewFaultQuery::QueryFaultReport, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
			CFaultReport* pReport = new CFaultReport();
			MYASSERT_CONTINUE(pReport);
			if(!pReport->LoadFromDB(pMemset))
			{
				delete pReport;
			}
			else
			{
				if(bComparePriID)
				{
					for (int i = 0; i < pCmbOne->GetCount(); i++)
					{
						pDeviceObj = (CDeviceObj*)pCmbOne->GetItemData(i);
						if (pDeviceObj != NULL && !pReport->m_sPriDeviceID.CompareNoCase(pDeviceObj->m_sID) || pDeviceObj == NULL)
						{
							//加入到链表
							m_listFaultReport.AddTail(pReport);
							break;
						}
					}		
				}
				else
				{
					m_listFaultReport.AddTail(pReport);
				}
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CViewFaultQuery::QueryFaultReport,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	int nCount = m_listFaultReport.GetCount();
	m_sQueryResult.Format("%s:%d", StringFromID(IDS_FAULTREPORT_QUERY_COUNT),nCount);
	
	return bReturn;
}


/*************************************************************
 函 数 名：QueryFaultReport()
 功能概要：根据条件查询故障报告
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B83035B
BOOL CViewFaultQuery::QueryFaultReport()
{
	//取得所有选择值
	GetAllCurSel();
	//清空原有数据
	Clear();
	//清空列表框
	m_List.DeleteAllItems();

	return QuerySpecificPriDevice();	
}

/*************************************************************
 函 数 名：Clear()
 功能概要：清空原来的数据
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B83036B
void CViewFaultQuery::Clear()
{
	//清空链表
	POSITION pos = m_listFaultReport.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultReport* pReport = (CFaultReport*)m_listFaultReport.GetNext(pos);
		if (pReport!=NULL)
		{
			delete pReport;
			pReport = NULL;
		}
		
	}
	m_listFaultReport.RemoveAll();
}

/*************************************************************
 函 数 名：FillListData()
 功能概要：填充列表数据
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B83037A
void CViewFaultQuery::FillListData()
{
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	
	POSITION pos = m_listFaultReport.GetHeadPosition();
	int nIndex = 0;
	while(pos != NULL)
	{
		CFaultReport* pReport = (CFaultReport*)m_listFaultReport.GetNext(pos);
		MYASSERT(pReport);
		AddReportToList(pReport, nIndex);
		nIndex++;
	}
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 函 数 名：AddReportToList()
 功能概要：加入指定的故障报告到列表框中显示
 返 回 值: void
 参    数：param1	指定故障报告
		   Param2	指定位置
**************************************************************/
//##ModelId=49B87B83037B
void CViewFaultQuery::AddReportToList( CFaultReport* pReport, int nIndex )
{
	//检查数据有效性
	if(pReport == NULL)
		return;
	if(nIndex < 0)
		return;
	//"序号", "事故包名称", "故障时间","故障元件","厂站1","厂站2","故障类型","故障测距"
	CString str;
	//0:序号
	str.Format("%d", pReport->m_nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1:事故包名称
	m_List.SetItemText(nIndex, 1, pReport->m_sName);
	//2:创建时间
	str = pReport->m_tmCreateTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 2, str);
	//3:故障时间
	str = pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 3, str);
	//4:故障元件
	if(pReport->m_pPriDevice != NULL)
	{
		str = pReport->m_pPriDevice->m_sName;
	}
	else
		str = "";
	m_List.SetItemText(nIndex, 4, str);
	//5:厂站1
	str = "";
	if(pReport->m_pStation1 != NULL)
	{
		str = pReport->m_pStation1->m_sName;
	}
	m_List.SetItemText(nIndex, 5, str);
	//6:厂站2
	str = "";
	if(pReport->m_pStation2 != NULL)
	{
		str = pReport->m_pStation2->m_sName;
	}
	m_List.SetItemText(nIndex, 6, str);
	//7:故障类型
	str = pReport->GetFaultType();
	m_List.SetItemText(nIndex, 7, str);
	//8:故障测距
	str = "";
	if( !pReport->m_sFaultDistance.IsEmpty() )
	{
		str.Format("%s km", pReport->m_sFaultDistance);
	}
	m_List.SetItemText(nIndex, 8, str);
	//9:是否检修
	m_List.SetItemText(nIndex, 9, pReport->m_nOverhaul == 0?StringFromID(IDS_OVERHUAL_NO):StringFromID(IDS_OVERHUAL_YES));
	//10:是否存档
	m_List.SetItemText(nIndex, 10, pReport->m_bIsSaved?StringFromID(IDS_COMMON_YES):StringFromID(IDS_COMMON_NO));
	//11:是否典型
	m_List.SetItemText(nIndex, 11, pReport->m_nTypicalcase == 0?StringFromID(IDS_COMMON_NO):StringFromID(IDS_COMMON_YES));
	
    m_List.SetItemText(nIndex, 12, StringFromID(IDS_COMMON_EDIT));
	m_List.SetItemText(nIndex, 13, StringFromID(IDS_COMMON_PLAYBACK));
	//关联到DATA
	m_List.SetItemData(nIndex, (DWORD)pReport);
}

//##ModelId=49B87B8400BD
void CViewFaultQuery::OnItemchangedListFaultQuery(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if((pNMListView->uChanged   &   LVIF_STATE)&&
		(pNMListView->uNewState   &   LVIS_SELECTED)) 
	{
		//事实上鼠标点击的选择动作, 会产生三次ITEMCHANGED消息, 
		//1是A的文字背景消失触发，2是A取消选中触发，3是B被选中触发
		
		int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
		if(nSelected < 0)
			return;
		
		//得到该行对应的事件
		CFaultReport* pReport = (CFaultReport*)m_List.GetItemData(nSelected);
		if(pReport == NULL)
			return;
		//载入详细信息
// 		if(pReport->m_bLoadDetail == FALSE)
// 			pReport->LoadDetail();
		//发送消息给详细显示窗口
		CWnd * pWnd = GetParent()->GetParent()->GetParent();
		if(pWnd != NULL)
		{
			pWnd->PostMessage(FAULTREPORT_SEL_CHANGED, 0, (LPARAM)pReport);
		}
	}
	*pResult = 0;
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果.1, 0, -1
 参    数：param1 比较对象1
		   Param2 比较对象2
		   param3 比较参数
**************************************************************/
//##ModelId=49B87B830293
int CALLBACK CViewFaultQuery::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得要比较的数据
	CFaultReport* pReport1 = (CFaultReport*)lParam1;
	CFaultReport* pReport2 = (CFaultReport*)lParam2;
	if(pReport1 == NULL || pReport2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	float fTemp1;
	float fTemp2;
	int iResult = 0;
	//"序号", "事故包名称", "故障时间","故障元件","厂站1","厂站2","故障类型","故障测距"
	switch(nCol)
	{
	case 0: //ID
		iResult = pReport1->m_nID - pReport2->m_nID;
		break;
	case 1: //事故包名称
		str1 = pReport1->m_sName;
		str2 = pReport2->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //创建时间
		str1 = pReport1->m_tmCreateTime.Format("%Y-%m-%d %H:%M:%S");
		str2 = pReport2->m_tmCreateTime.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //故障时间
		str1 = pReport1->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
		str2 = pReport2->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4:	//故障元件
		str1 = "";
		if(pReport1->m_pPriDevice != NULL)
			str1 = pReport1->m_pPriDevice->m_sName;
		str2 = "";
		if(pReport2->m_pPriDevice != NULL)
			str2 = pReport2->m_pPriDevice->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //厂站1
		str1 = "";
		if(pReport1->m_pStation1 != NULL)
			str1 = pReport1->m_pStation1->m_sName;
		str2 = "";
		if(pReport2->m_pStation1 != NULL)
			str2 = pReport2->m_pStation1->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //厂站2
		str1 = "";
		if(pReport1->m_pStation2 != NULL)
			str1 = pReport1->m_pStation2->m_sName;
		str2 = "";
		if(pReport2->m_pStation2 != NULL)
			str2 = pReport2->m_pStation2->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 7: //故障类型
		str1 = pReport1->GetFaultType();
		str2 = pReport2->GetFaultType();
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 8: //故障测距
		fTemp1 = atof(pReport1->m_sFaultDistance);
		fTemp2 = atof(pReport2->m_sFaultDistance);
		if(fTemp1 - fTemp2 < -0.00001)
			iResult = -1;
		else if(fTemp1 - fTemp2 > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 9://是否检修
		iResult = pReport1->m_nOverhaul - pReport2->m_nOverhaul;
		break;
	case 10://是否存档
		iResult = pReport1->m_bIsSaved - pReport2->m_bIsSaved;
		break;
	case 11://是否典型
		iResult = pReport1->m_nTypicalcase - pReport2->m_nTypicalcase;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iFaultQueryViewAsc;
	return iResult;
}

//##ModelId=49B87B8400CC
void CViewFaultQuery::OnColumnclickListFaultQuery(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iFaultQueryViewAsc = - g_iFaultQueryViewAsc;
	}
	else
	{
		g_iFaultQueryViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87B8400D0
void CViewFaultQuery::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	//CListCtrlEx先截取了WM_CONTEXTMENU消息, 处理完后再转完给所有者窗口
	//所以这里先得到消息参数再处理消息
	LPMSG  msg;
	msg = (LPMSG)GetCurrentMessage();
	UINT wParam = (UINT)msg ->wParam;
	if(wParam == LISTCTRLEX_MARK)
	{
		CPoint * ptTemp = (CPoint*)msg->lParam;
		point = *ptTemp;
		
		//判断是右击在列头还是列表,只处理列表,列头在CListCtrlEx中处理
		CPoint pt = point;
		m_List.ScreenToClient(&pt);
		
		CWnd* pChildWnd = m_List.ChildWindowFromPoint(pt);
		if(pChildWnd != NULL && pChildWnd->GetSafeHwnd() != m_List.GetSafeHwnd())
		{
			char szClass [50];
			
			// Verify that this window handle is indeed the header
			// control's by checking its classname.
			GetClassName(pChildWnd->GetSafeHwnd(), szClass, 50);
			if (!lstrcmp (szClass, "SysHeader32"))
			{
			}
		}
		else if(pChildWnd != NULL && pChildWnd->GetSafeHwnd() == m_List.GetSafeHwnd())
		{//点击的是ListCtrl的非列头部分
			//生成菜单
			CMenu Menu, *pMenu;
			if(!Menu.LoadMenu(IDR_MENU_FAULTREPORT))
				return;
			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount == 1)
			{
				//只选择了一个
				pMenu->EnableMenuItem(IDC_FAULT_EDIT, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_FAULT_PRINT, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_FAULT_WEB, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_FAULT_DELETE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_FAULT_EXPORT, MF_ENABLED);
			}
			//pMenu->EnableMenuItem(IDC_FAULT_REFRESH, MF_ENABLED);
			//pMenu->EnableMenuItem(IDC_FAULT_CREATE, MF_ENABLED);
			pMenu->EnableMenuItem(ID_MANUAL_FAULTSHORTENN, MF_ENABLED);

			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87B8400DC
void CViewFaultQuery::OnFaultPrint() 
{
	// TODO: Add your command handler code here
	//父窗口是分割窗口
	CWnd * pWnd = GetParent()->GetParent()->GetParent();
	if(pWnd == NULL)
		return;
	//向父窗口发送消息
	pWnd ->PostMessage(FAULTREPORT_PRINT, 0, 0);
}

//##ModelId=49B87B8400DE
void CViewFaultQuery::OnFaultEdit() 
{
	// TODO: Add your command handler code here
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	CFaultReport* pReport = (CFaultReport*)m_List.GetItemData(nIndex);
	if(pReport == NULL)
		return;
	CDlgFaultReport dlg;
	LV_COLUMN lvCol;
	lvCol.mask   =   LVCF_TEXT;
	char temp[256];
	for(int j = 0; j < 256; j++)
	{
		temp[j] = '\0';
	}
	lvCol.cchTextMax = 256;
	lvCol.pszText = temp;
	m_List.GetColumn(9, &lvCol);
	dlg.m_sTitle = lvCol.pszText;
	dlg.m_pWnd = this;
	dlg.m_pFaultReport = pReport;
	dlg.DoModal();
}

//##ModelId=49B87B8400E0
BOOL CViewFaultQuery::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_FAULT_QUERY);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

void CViewFaultQuery::OnFaultReportEdited( WPARAM wParam, LPARAM lParam )
{
	CFaultReport* pReport = (CFaultReport*)lParam;
	if(pReport != NULL)
	{
		//故障信息动作事件重新排序
		pReport->SortActionSign();
		ChangedValueInList(pReport);
		//发送消息给详细显示窗口
		CWnd * pWnd = GetParent()->GetParent()->GetParent();
		if(pWnd != NULL)
		{
			pWnd->SendMessage(FAULTDETAIL_REFRESH, 0, (LPARAM)pReport);
		}
	}	
}

/*************************************************************
 函 数 名: IssueToWeb()
 功能概要: 将指定的故障包发布到WEB
 返 回 值: 发布结果 0-成功. 1-通讯服务器未连接. 2-发送报文失败
 参    数: param1 指定要发布的故障包
		   Param2 
**************************************************************/
int CViewFaultQuery::IssueToWeb( CFaultReport* pReport )
{
	if(pReport == NULL)
		return -1;
	//发送消息给通信服务器
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		CString str;
		str.Format("%s 发送到WEB, %s", pReport->m_sName, STTP_DISCONNECT);
		WriteLog(str);
		return 1;
	}
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	
	//组织20015报文
	if(!pApp->m_SttpEngine.BuildDataFor20151FaultPacket(sttpData, 3, pReport))
	{
		//组织报文失败
		CString str;
		str.Format("%s 发送到WEB, 组织报文失败", pReport->m_sName);
		WriteLog(str);
		return 2;
	}
	
	//发送报文
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//判断是否成功
	if(nReturn != 1)
	{
		CString str;
		str.Format("%s 发送到WEB, 发送报文失败", pReport->m_sName);
		WriteLog(str);
		return 3;
	}
	
	return 0;
}

/*************************************************************
 函 数 名: ChangedValueInList()
 功能概要: 改变指定故障报告在列表中的显示
 返 回 值: 
 参    数: param1 指定故障报告
		   Param2 指定索引
**************************************************************/
void CViewFaultQuery::ChangedValueInList( CFaultReport* pReport, int nIndex /*= -1*/ )
{
	//检查数据有效性
	if(pReport == NULL)
		return;
	if(nIndex == -1)
	{
		//查找在列表中的位置
		//查找事件在列表中的位置
		LVFINDINFO lvFind;
		lvFind.flags = LVFI_PARAM;
		lvFind.lParam = (LPARAM)pReport;
		nIndex = m_List.FindItem(&lvFind);
	}
	if(nIndex < 0)
		return;
	//
	//1:事故包名称
	CString str;
	m_List.SetItemText(nIndex, 1, pReport->m_sName);
	//2:创建时间
	str = pReport->m_tmCreateTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 2, str);
	//3:故障时间
	str = pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 3, str);
	//4:故障元件
	if(pReport->m_pPriDevice != NULL)
	{
		str = pReport->m_pPriDevice->m_sName;
	}
	else
		str = "";
	m_List.SetItemText(nIndex, 4, str);
	//5:厂站1
	str = "";
	if(pReport->m_pStation1 != NULL)
	{
		str = pReport->m_pStation1->m_sName;
	}
	m_List.SetItemText(nIndex, 5, str);
	//6:厂站2
	str = "";
	if(pReport->m_pStation2 != NULL)
	{
		str = pReport->m_pStation2->m_sName;
	}
	m_List.SetItemText(nIndex, 6, str);
	//7:故障类型
	str = pReport->GetFaultType();
	m_List.SetItemText(nIndex, 7, str);
	//8:故障测距
	m_List.SetItemText(nIndex, 8, pReport->m_sFaultDistance);
	//9:是否检修
	m_List.SetItemText(nIndex, 9, pReport->m_nOverhaul == 0?StringFromID(IDS_OVERHUAL_NO):StringFromID(IDS_OVERHUAL_YES));
	//10:是否存档
	m_List.SetItemText(nIndex, 10, pReport->m_bIsSaved?StringFromID(IDS_COMMON_YES):StringFromID(IDS_COMMON_NO));
	//11:是否典型
	m_List.SetItemText(nIndex, 11, pReport->m_nTypicalcase == 0?StringFromID(IDS_COMMON_NO):StringFromID(IDS_COMMON_YES));
	
	//刷新
	m_List.Update(nIndex);
}

void CViewFaultQuery::OnFaultWeb() 
{
	// TODO: Add your command handler code here
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	CString str = "";
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CFaultReport* pReport = (CFaultReport*)m_List.GetItemData(nIndex);
			if(pReport != NULL)
			{
				int nResult = IssueToWeb(pReport);
				CString strResult;
				strResult.Format("%s %s -- %s\n", pReport->m_sName, StringFromID(IDS_PUBLICTO_WEB),
					nResult == 0?StringFromID(IDS_COMMON_SUCCESS):StringFromID(IDS_COMMON_FAILED));	
				str += strResult;
			}
		}
	}	
	AfxMessageBox(str);
}

void CViewFaultQuery::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				if(nItem  != -1)
				{
					//间隔
					if(nItem%2 != 0)
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
				}

				if (nSubItem >= 9)
				{
					lplvcd->clrText = RGB(0, 0, 255);
				}
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}		
}

void CViewFaultQuery::OnSelchangeCmbFaultNet() 
{
	// TODO: Add your control notification handler code here
		//重新填充一次设备下拉框,二次设备下拉框
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_NET);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	CNetObj* pNet = (CNetObj*)pCmb->GetItemData(nSel);


	CComboBox* pCmbStation = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	if (pCmbStation == NULL)
	{
		return;
	}
	int nSelStation = pCmbStation->GetCurSel();
	if (nSelStation < 0)
	{
		return;
	}
	CStationObj* pStation = (CStationObj*)pCmbStation->GetItemData(nSelStation);

	int nVoltage = GetVoltageCmbValue();

	CComboBox* pCmbDeviceType= (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	if (pCmbDeviceType == NULL)
	{
		return;
	}
	int nSelDeviceType = pCmbDeviceType->GetCurSel();

	CComboBox* pCmbDevice = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_ONE);
	if (pCmbDevice == NULL)
	{
		return;
	}
	int nSelDevice = pCmbDevice->GetCurSel();
	CDeviceObj* pDevice = (CDeviceObj*)pCmbDevice->GetItemData(nSelDevice);

	//厂站
	FillStation(pNet);
	FillVoltage(NULL);
	FillPriDevice(pStation, nVoltage, nSelDeviceType);
	FillSecDevice(pStation, pDevice, nVoltage, nSelDeviceType);
}

void CViewFaultQuery::OnSelchangeCmbFaultVoltage() 
{
	// TODO: Add your control notification handler code here
	int nVoltage = GetVoltageCmbValue();

    CComboBox* pCmbStation = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	int nSelStation = pCmbStation->GetCurSel();
	if(nSelStation < 0)
		return;
    CStationObj* pStation = (CStationObj*)pCmbStation->GetItemData(nSelStation);

	CComboBox* pCmbDeviceType = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	int nSelDeviceType = pCmbDeviceType->GetCurSel();
	if (nSelDeviceType < 0)
	{
		return;
	}

	FillPriDevice(pStation, nVoltage, nSelDeviceType);
	FillSecDevice(pStation, NULL, nVoltage, nSelDeviceType);
}


void CViewFaultQuery::OnSelchangeCmbFaultDeviceType() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;

    CComboBox* pCmbStation = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	int nSelStation = pCmbStation->GetCurSel();
	if(nSelStation < 0)
		return;
    CStationObj* pStation = (CStationObj*)pCmbStation->GetItemData(nSelStation);

    int nVoltage = GetVoltageCmbValue();

	FillPriDevice(pStation, nVoltage, nSel);
	FillSecDevice(pStation, NULL, nVoltage, nSel);
}

void CViewFaultQuery::OnSelchangeCmbFaultOne() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_ONE);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	CDeviceObj* pDevice = (CDeviceObj*)pCmb->GetItemData(nSel);

    CComboBox* pCmbStation = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	int nSelStation = pCmbStation->GetCurSel();
	if(nSelStation < 0)
		return;
    CStationObj* pStation = (CStationObj*)pCmbStation->GetItemData(nSelStation);

	int nVoltage = GetVoltageCmbValue();

	CComboBox* pCmbDeviceType = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	int nSelDeviceType = pCmbDeviceType->GetCurSel();
	if (nSelDeviceType < 0)
	{
		return;
	}
	
    FillSecDevice(pStation, pDevice, nVoltage, nSelDeviceType);
}

void CViewFaultQuery::InitVoltageMap()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (pApp == NULL)
	{
		return;
	}
    CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if(pStation == NULL)
			continue;
	    for(int i = 0; i < pStation->m_arrVoltage.GetSize(); i++)
		{
			m_VoltageMap.SetAt(pStation->m_arrVoltage.GetAt(i), pStation);
		}
	}
}

void CViewFaultQuery::InitVoltageMap(CString strNetID, MAP_VOLTAGE* VoltageMap)
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (pApp == NULL)
	{
		return;
	}
    CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if(pStation == NULL)
			continue;
		if (!pStation->m_sNetID.CompareNoCase(strNetID))
		{
			for(int i = 0; i < pStation->m_arrVoltage.GetSize(); i++)
			{
				VoltageMap->SetAt(pStation->m_arrVoltage.GetAt(i), pStation);
			}
		}
	}
}


int CViewFaultQuery::GetVoltageCmbValue()
{
	CComboBox* pCmbVoltage = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_VOLTAGE);
	if(pCmbVoltage == NULL)
		return 0;
	int nSelVoltage = pCmbVoltage->GetCurSel();
	if(nSelVoltage < 0)
		return 0;
	CString strVoltage;
	pCmbVoltage->GetLBText(nSelVoltage, strVoltage);
	if (!strVoltage.CompareNoCase(StringFromID(IDS_COMMON_ALL)))
	{
		return 0;
	}
	else
	{
		strVoltage.TrimRight("kV");
		return atoi(strVoltage);	
	}
}

void CViewFaultQuery::FillAllPriDevice(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage)
{
	//线路
	FillSpecificPriDevice(pCmb, pStation, TYPE_LINE, nVoltage);
	//母线
	FillSpecificPriDevice(pCmb, pStation, TYPE_BUS, nVoltage);
	//变压器
	FillSpecificPriDevice(pCmb, pStation, TYPE_TRANS, nVoltage);
	//电容器
	FillSpecificPriDevice(pCmb, pStation, TYPE_CAP, nVoltage);
	//电抗器
	FillSpecificPriDevice(pCmb, pStation, TYPE_REACTOR, nVoltage);
	//发电机
	FillSpecificPriDevice(pCmb, pStation, TYPE_GEN, nVoltage);
	//开关
	FillSpecificPriDevice(pCmb, pStation, TYPE_BREAK, nVoltage);
	//刀闸
	FillSpecificPriDevice(pCmb, pStation, TYPE_SWITCH, nVoltage);
}


void CViewFaultQuery::FillPriDeviceByType(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage)
{
	    switch(nDeviceType)
		{
		case 1:
			FillSpecificPriDevice(pCmb, pStation, 1, nVoltage);
			break;
		case 2:
			FillSpecificPriDevice(pCmb, pStation, 2, nVoltage);
			break;
		case 3:
			FillSpecificPriDevice(pCmb, pStation, 3, nVoltage);
			break;
		case 4:
			FillSpecificPriDevice(pCmb, pStation, 4, nVoltage);
			break;
		case 5:
			FillSpecificPriDevice(pCmb, pStation, 5, nVoltage);
			break;
		case 6:
			FillSpecificPriDevice(pCmb, pStation, 6, nVoltage);
			break;
		case 7:
			FillSpecificPriDevice(pCmb, pStation, 7, nVoltage);
			break;
		case 8:
			FillSpecificPriDevice(pCmb, pStation, 8, nVoltage);
			break;
		}
}


BOOL CViewFaultQuery::IfHasSecDevice(CStationObj* pStation, CDeviceObj* pDeviceObj)
{
	CSecObj* pSecObj = NULL;
	POSITION pos = NULL;
	CString sStationID = "";
	if(pStation != NULL)
		sStationID = pStation->m_sID;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		pSecObj = (CSecObj*)listDevice.GetNext(pos);
		if (pSecObj == NULL)
		{
			continue;
		}
		if (pSecObj->m_pOwner == NULL)
		{
			continue;
		}
		if(!pSecObj->m_pOwner->m_sName.CompareNoCase(pDeviceObj->m_sName))
		{
			return TRUE;
		}
	}
	return FALSE;
}


void CViewFaultQuery::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CViewFaultQuery::OnFaultExport() 
{
	// TODO: Add your command handler code here
	//父窗口是分割窗口
	CWnd * pWnd = GetParent()->GetParent()->GetParent();
	if(pWnd == NULL)
		return;
	//向父窗口发送消息
	pWnd ->PostMessage(FAULTREPORT_EXPORT_EXCEL, 0, 0);
}


void CViewFaultQuery::OnManualFaultShorten()
{
	CManualFaultShorten manDlg;
	manDlg.pView = ((CFaultFrame*)GetParentFrame())->m_pFault;
	manDlg.DoModal();
}

void CViewFaultQuery::OnFaultDelete() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//无权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
		{
			return;
		}
	}
	CList<int, int>  itemList;
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int index = m_List.GetNextSelectedItem(pos);
		if (index < 0)
		{
			continue;
		}
		itemList.AddTail(index);
	}
	
	if (itemList.GetCount() == 0)
	{
		AfxMessageBox(StringFromID(IDS_TIP_SELECTITEM));
	}

	if(AfxMessageBox(StringFromID(IDS_TIP_DELETE_FAULTREPORT), MB_OKCANCEL) != IDOK)
	{
		return;
	}
	
	DeleteFaultShortenData(itemList);
}

/****************************************************
Date:2012/4/5  Author:LYH
函数名:   DeleteFaultShortenData	
返回值:   void	
功能概要: 
参数: CList<int	
参数: int> & itemList	
*****************************************************/
void CViewFaultQuery::DeleteFaultShortenData( CList<int, int>& itemList )
{
	POSITION pos = itemList.GetHeadPosition();
	while (pos != NULL)
	{
		int index = itemList.GetNext(pos);
		CFaultReport* pReport = (CFaultReport*)m_List.GetItemData(index);
		MYASSERT_CONTINUE(pReport);
		if (pReport->DeleteFaultShortenData())
		{
			DeleteFaultReport(pReport);
		}
	}
	FillListData();
}

/****************************************************
Date:2012/4/5  Author:LYH
函数名:   DeleteFaultReport	
返回值:   void	
功能概要: 
参数: CFaultReport * pReport	
*****************************************************/
void CViewFaultQuery::DeleteFaultReport( CFaultReport* pReport )
{
	//找到在链表中的位置
	POSITION pos = m_listFaultReport.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultReport* pTemp = (CFaultReport*)m_listFaultReport.GetAt(pos);
		if(pTemp == pReport)
		{
			MYDELETE(pTemp);
			m_listFaultReport.RemoveAt(pos);
			return;
		}
		m_listFaultReport.GetNext(pos);
	}
}

void CViewFaultQuery::OnSelchangeCmbFaultType() 
{
	// TODO: Add your control notification handler code here
	CString str;
	((CComboBox*)GetDlgItem(IDC_CMB_FAULT_TYPE))->GetWindowText(str);
	if (str == StringFromID(IDS_COMMON_ALL))
	{
		m_nChooseFaultType = 2;
	}
//	FillFaultType();
//	FillFaultType2();
}

void CViewFaultQuery::OnSelchangeCmbFaultType2() 
{
	// TODO: Add your control notification handler code here
	CString str;
//	((CComboBox*)GetDlgItem(IDC_CMB_FAULT_TYPE2))->GetWindowText(str);

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_TYPE2);
	if(pCmb == NULL)
		return;
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_TYPE2);
	if(pCmb != NULL)
	{
		m_nChooseFaultType = pCmb->GetCurSel();
	}
	FillFaultType();
//	FillFaultType2();
}

/****************************************************
Date:2013/6/4  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewFaultQuery::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	//填充下拉框
	FillNet(); //地区
	FillStation(); //厂站
	FillVoltage(NULL); //电压等级
	FillPriDevice(NULL, NULL, 0);//一次设备
	FillSecDevice(NULL, NULL, NULL, 0);  //二次设备
	WriteLog("StationInit,故障报告查询页面刷新完毕", XJ_LOG_LV3);
}
