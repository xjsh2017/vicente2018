// ViewOSC.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewOSC.h"
#include "MainFrm.h"
#include "GlobalFunc.h"
/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

int g_iOSCViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewOSC

IMPLEMENT_DYNCREATE(CViewOSC, CFormView)

//##ModelId=49B87B83006F
CViewOSC::CViewOSC()
	: CFormView(CViewOSC::IDD)
{
	//{{AFX_DATA_INIT(CViewOSC)
	m_bCBoxShowDebug = TRUE;
	m_bCBoxShowReady = TRUE;
	m_bCBoxShowFault = TRUE;
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_OSC;
	m_EventManager.ClearAllEvents();
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_nOldSortCol = -1;
	g_iOSCViewAsc = -1;
	m_bShowDebug = TRUE;
	m_bShowFault = TRUE;
	m_sChooseStationID = "";
}

//##ModelId=49B87B830138
CViewOSC::~CViewOSC()
{
	m_EventManager.ClearAllEvents();
}

//##ModelId=49B87B830119
void CViewOSC::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewOSC)
	DDX_Control(pDX, IDC_CHECK_FAULT, m_cboxShowFault);
	DDX_Control(pDX, IDC_STATIC_CHOOSESTATION, m_staticChooseStation);
	DDX_Control(pDX, IDC_COMBO_CHOOSESTATION, m_cmbChooseStation);
	DDX_Control(pDX, IDC_CHECK_SHOWREADY, m_cboxShowReady);
	DDX_Control(pDX, IDC_CHECK_SHOWDEBUG, m_cboxShowDebug);
	DDX_Control(pDX, IDC_LIST_OSC, m_List);
	DDX_Check(pDX, IDC_CHECK_SHOWDEBUG, m_bCBoxShowDebug);
	DDX_Check(pDX, IDC_CHECK_SHOWREADY, m_bCBoxShowReady);
	DDX_Check(pDX, IDC_CHECK_FAULT, m_bCBoxShowFault);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewOSC, CFormView)
	//{{AFX_MSG_MAP(CViewOSC)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_OSC, OnClickListOsc)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_OSC, OnColumnclickListOsc)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OSC, OnCustomDraw)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_OSC, OnDblclkListOsc)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	ON_COMMAND(IDC_EVENT_QUERYEVENT, OnEventQueryevent)
	ON_COMMAND(IDC_EVENT_FILTER_DEBUG, OnEventFilterDebug)
	ON_COMMAND(IDC_EVENT_FILTER_FAULT, OnEventFilterFault)
	ON_COMMAND(IDC_EVENT_OPENOSC, OnEventOpenosc)
	ON_COMMAND(IDC_EVENT_DOWNOSC, OnEventDownosc)
	ON_BN_CLICKED(IDC_CHECK_SHOWDEBUG, OnCheckShowdebug)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSESTATION, OnSelchangeCOMBOChooseStation)
	ON_BN_CLICKED(IDC_CHECK_SHOWREADY, OnCheckShowready)
	ON_BN_CLICKED(IDC_CHECK_FAULT, OnCheckShowFault)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_STTP_20132, OnSTTP20132)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewOSC diagnostics

#ifdef _DEBUG
//##ModelId=49B87B830148
void CViewOSC::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B830157
void CViewOSC::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewOSC message handlers

//##ModelId=49B87B830128
void CViewOSC::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_OSC));
	//GetParentFrame()->SetWindowText(" 录波 ");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B83010A
void CViewOSC::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
		CSize sizeTotal(100, 100);
		SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//读取配置
	LoadConfig();

	//初始化控件
	InitComBox();


	//初始化列表
	InitListStyle();
	InitListCtrl();
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B83009E
int CViewOSC::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;

	RefreshData();

	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B8300AB
int CViewOSC::InitListStyle()
{
	//先查找UIConfig配置文件中是否有此列表的列信息
	//如果有,按配置文件的信息来设置列
	//如果没有,按默认来设置列,并保存到配置文件

	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

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
		//char* arColumn[10]={"序号","故障类型", "二次设备","厂站名","起始时间","录波数据文件", "跳闸相别", "重合闸时间", "故障测距", "已确认"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 10; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; // 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 100; // 故障类型
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			case 2://
				lvCol.cx = 300; // 录波器名称 
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 3://
				lvCol.cx = 100; // 厂站名
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 4:
				lvCol.cx = 220; // 起始时间
				colName = StringFromID(IDS_EVENT_STARTTIME);
				break;
			case 5:
				lvCol.cx = 240; // 录波数据文件
				colName = StringFromID(IDS_COMMON_FILENAME);
				break;
			case 6:
				lvCol.cx = 100; // 跳闸相别
				colName = StringFromID(IDS_TRIP_PHASE);
				break;
			case 7:
				lvCol.cx = 100; //重合闸时间
				colName = StringFromID(IDS_FAULT_RECLOSE_TIME);
				break;
			case 8:
				lvCol.cx = 100; //故障测距
				colName = StringFromID(IDS_FAULT_LOCATION);
				break;
			case 9:
				lvCol.cx = 80; //已确认
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=80;//列宽100象素
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(9, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8300BB
BOOL CViewOSC::LoadListConfig()
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
			WriteLog("ViewOSC::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("ViewOSC::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewOSCConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewOSCConfig", XJ_LOG_LV3);
				WriteLog("ViewOSC::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewOSCConfig下的配置", XJ_LOG_LV3);
					WriteLog("ViewOSC::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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

//##ModelId=49B87B8300CB
BOOL CViewOSC::SaveListConfig()
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
			WriteLog("ViewOSC::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewOSC::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewOSCConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewCommConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewOSCConfig");
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
						WriteLog("未找到ViewOSCConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
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
						WriteLog("找到了ViewOSCConfig下的配置, 先删除新建所有子节点", XJ_LOG_LV3);
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
//##ModelId=49B87B8300AC
int CViewOSC::FillListData()
{
/*	int nCount = m_List.GetHeaderCtrl()->GetItemCount();
	
	for(int i = 0; i < 10; i++)
	{
		CString temp;
		temp.Format("%d", i);
		m_List.InsertItem(LVIF_TEXT, i, temp, 0, 0, 0, 0);
		for(int j = 1; j < nCount; j++)
		{
			CString str;
			str.Format("%d%d", i, j);
			m_List.SetItemText(i, j, str);
		}
	}*/

	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	CXJEventOSC * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventOSC*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	//设置checkbox的状态

	m_cboxShowReady.SetCheck(m_bCBoxShowReady);
	m_cboxShowDebug.SetCheck(m_bCBoxShowDebug);
	m_cboxShowFault.SetCheck(m_bCBoxShowFault);
	//恢复刷新
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B8300CC
BOOL CViewOSC::NeedSave()
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

/*************************************************************
 函 数 名：RegulateControlSize()
 功能概要：调整控件位置
 返 回 值: 返回值说明
**************************************************************/
//##ModelId=49B87B8300DA
void CViewOSC::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//客户区大小
	CRect rc;
	GetClientRect(&rc);
	CRect rcList;
	
	if ( g_role == MODE_SUB )             //运行与监视角色 0:主站 1:子站)
	{
		//设置按钮隐藏
		m_staticChooseStation.ShowWindow(FALSE);
		m_cmbChooseStation.ShowWindow(FALSE);
		m_cboxShowDebug.ShowWindow(FALSE);
		m_cboxShowReady.ShowWindow(FALSE);
		m_cboxShowFault.ShowWindow(FALSE);
		rcList=rc;
	}
	else
	{
		rcList=CRect(rc.left,rc.top+45,rc.right,rc.bottom);
	}
	

	m_List.MoveWindow(&rcList, TRUE);
}

//##ModelId=49B87B830167
void CViewOSC::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

//##ModelId=49B87B83016A
void CViewOSC::OnDestroy() 
{
	//保存配置
	SaveConfig();
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：LoadDataFromDB()
 功能概要：从数据库中读取最新事件, 数量由最大显示行数决定, 
			每一条信息新建一个事件对象保存在事件管理对象中
 返 回 值: 读取成功返回TRUE, 失败返回FLASE
**************************************************************/
//##ModelId=49B87B8300DB
BOOL CViewOSC::LoadDataFromDB(int num, BOOL IsQueryAcked)
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	m_EventManager.ClearAllEvents();
	m_List.DeleteAllItems();
	//与数据平台访问DLL的连接可用
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//TOP num
	Field Field0;
	bzero(&Field0, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field0.FieldName, str, str.GetLength());
	Field0.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field0);


	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "filename", EX_STTP_DATA_TYPE_STRING);
	
	Field Field31;
	pApp->m_DBEngine.SetField(sql, Field31, "trigger_time", EX_STTP_DATA_TYPE_TIME);
	
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "ms", EX_STTP_DATA_TYPE_INT);
	
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "savepath", EX_STTP_DATA_TYPE_STRING);
	
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "Bfaultfile", EX_STTP_DATA_TYPE_INT);
	
	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "fault_type", EX_STTP_DATA_TYPE_STRING);
	
	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "trigger_type", EX_STTP_DATA_TYPE_STRING);
	
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "reclose_time", EX_STTP_DATA_TYPE_INT);
	
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "Fault_distance", EX_STTP_DATA_TYPE_STRING);

	Field Field13;
	pApp->m_DBEngine.SetField(sql, Field13, "Is_Ack", EX_STTP_DATA_TYPE_INT);

	Field Field14;
	pApp->m_DBEngine.SetField(sql, Field14, "Reverse2", EX_STTP_DATA_TYPE_STRING);

	//条件
	//只查询未确认事件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("IS_ACK = 0");
	strncpy(condition1.ConditionContent, str, str.GetLength());
	if(IsQueryAcked)
	{
		//要查询已确认事件, 则此条件不加
		//	sql.Conditionlist.push_back(condition1);
	}
	else
	{
		//不查询已确认事件, 则加上只查询未确认事件的条件
		sql.Conditionlist.push_back(condition1);
	}

	//检查显示过滤条件
	if(!m_bShowDebug)
	{
		Condition condition4;
		CString strFilter;
		strFilter.Format("Reverse2='%d'", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}

	//选择显示厂站的信息
	if (!m_sChooseStationID.IsEmpty() )
	{
		Condition condition5;
		CString strFilter;
		strFilter.Format("(pt_id IN (select pt_id from tb_secdev_base where station_id='%s'))",m_sChooseStationID);
		pApp->m_DBEngine.SetCondition(sql,condition5,strFilter);
	}
	else if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}

	//检查故障信息事件
	if (m_bShowFault==FALSE)
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("fault_type<>'NULL'");
		pApp->m_DBEngine.SetCondition(sql, condition6, strFilter);

		bzero(&condition6, sizeof(Condition));
		strFilter.Format("fault_type<>'NON'");
		pApp->m_DBEngine.SetCondition(sql, condition6, strFilter);
	}

	//按动作启动时间大小排序
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by ID DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition2);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	LONGLONG startTime = StartCount();
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OSC_LIST, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewOSC::LoadDataFromDB, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	EndCount("查询录波事件表时间",startTime);

	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewOSC::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventOSC * pEvent = new CXJEventOSC;
			if(pEvent->LoadFromDB(pMemset))
			{
				//读取数据成功,加入到链表
				m_EventManager.AddEvent(pEvent);
			}
			else
			{
				//读取数据失败, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CViewOSC::LoadDataFromDB,查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return bReturn;
}

/*************************************************************
 函 数 名：AddEventToList()
 功能概要：加入事件信息到列表,如果要加入的位置已经有数据了, 原先的数据
			将会自动下移,新数据在正确位置	
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数：param1   要加入的事件指针
           Param2   要加入的位置,从0开始计数
**************************************************************/
//##ModelId=49B87B8300EC
BOOL CViewOSC::AddEventToList(CXJEventOSC * pEvent, int i)
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;

	//"序号","故障类型", "录波器名称","厂站名","起始时间","录波数据文件", "跳闸相别", "重合闸时间", "故障测距", "已确认"
	//0:序号
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:故障类型
	if(pEvent->m_nHaveFault == 1)
		m_List.SetItemText(i, 1, pEvent->GetFaultType());
	else
		m_List.SetItemText(i, 1, StringFromID(IDS_FAULT_NOFAULT));
	//2:录波器名称
	CString str;
	str = pEvent->m_pSec->m_sName;
	m_List.SetItemText(i, 2, str);
	//3:厂站名
	m_List.SetItemText(i, 3, pEvent->m_sStationName);
	//4:发生时间
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 4, sTime);
	//5:录波数据文件
	m_List.SetItemText(i, 5, pEvent->m_sFileName);
	//6:跳闸相别
	if(pEvent->m_nHaveFault == 1)
		m_List.SetItemText(i, 6, pEvent->GetTriggerType(pEvent->m_sTriggerType));
	else
		m_List.SetItemText(i, 6, "--");
	//7:重合闸时间	
	if(pEvent->m_nHaveFault == 1)
	{
		if(pEvent->m_nRecloseTime == 0)
			m_List.SetItemText(i, 7, StringFromID(IDS_RECLOSE_NOTCLOSE));
		else
		{
			sTime.Format("%d", pEvent->m_nRecloseTime);
			m_List.SetItemText(i, 7, sTime);
		}
	}
	else
		m_List.SetItemText(i, 7, "--");
	//8:故障测距
	if(pEvent->m_nHaveFault == 1)
	{
		m_List.SetItemText(i, 8, pEvent->m_sFaultDistance);
	}
	else
		m_List.SetItemText(i, 8, "--");
	//9:已确认
	m_List.SetItemText(i, 9, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//设置数据
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

//##ModelId=49B87B830177
void CViewOSC::OnClickListOsc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
// 	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
// 	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
// 	if(nSelected > -1)
// 	{
// 		//点击到事件, 确认此事件
// 		//得到该行对应的事件
// 		CXJEventOSC * pEvent = (CXJEventOSC*)m_List.GetItemData(nSelected);
// 		if(pEvent == NULL)
// 			return;
// 		if(pEvent->Ack())
// 		{
// 			//刷新界面显示, 通知其它窗口更新
// 			RefreshAck(pEvent, nSelected);
// 			TellOtherWndAck(pEvent);
// 		}
// 	}
	*pResult = 0;
}

/*************************************************************
 函 数 名：RefreshAck()
 功能概要：确认事件后界面刷新显示
 返 回 值: void
 参    数：param1	确认的事件
		   Param2	事件在列表中的位置
**************************************************************/
//##ModelId=49B87B8300FA
void CViewOSC::RefreshAck( CXJEventOSC* pEvent, int nIndex )
{
	//判断数据有效性
	if(pEvent == NULL)
		return;
	if(nIndex == -1)
	{
		//查找事件在列表中的位置
		LVFINDINFO lvFind;
		lvFind.flags = LVFI_PARAM;
		lvFind.lParam = (LPARAM)pEvent;
		nIndex = m_List.FindItem(&lvFind);
	}
	//实在找不到,退出
	if(nIndex == -1)
		return;
	//更新ID,因为新事件事先不知道ID
	CString str;
	str.Format("%d", pEvent->m_lID);
	m_List.SetItemText(nIndex, 0, str);
	//更新是否确认显示
	m_List.SetItemText(nIndex, 9, StringFromID(IDS_ACK_YES));
	//刷新
// 	CRect rc;
// 	m_List.GetItemRect(nIndex, &rc, LVIR_BOUNDS);
// 	m_List.InvalidateRect(rc);
	m_List.Update(nIndex);
}

//##ModelId=49B87B83017B
void CViewOSC::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_EVENTVIEW))
				return;

			/*
			Menu.DeleteMenu(IDC_EVENT_COPY, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_EVENT_ACK, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_EVENT_ALL, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_EVENT_PROP, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_EVENT_Refresh, MF_BYCOMMAND);
			*/
			

			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount > 0)
			{
				//有选择
				pMenu->EnableMenuItem(IDC_EVENT_COPY, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_ACK, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_DOWNOSC, MF_ENABLED);
			}
			if(nCount == 1)
			{
				//只选择了一个
			//	pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_PROP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_QUERYEVENT, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_OPENOSC, MF_ENABLED);
			}
			if(m_bShowAcked)
			{
				//隐藏已确认事件
				pMenu->CheckMenuItem(IDC_EVENT_HIDE, MF_CHECKED);
	
			}
			else
			{
				//显示
				pMenu->CheckMenuItem(IDC_EVENT_HIDE, MF_UNCHECKED);
			}

			//是否显示调试
			pMenu->EnableMenuItem(IDC_EVENT_FILTER_DEBUG, MF_ENABLED);
			if(m_bShowDebug)
			{
				//显示调试
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_DEBUG, MF_CHECKED);
		
			}
			else
			{
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_DEBUG, MF_UNCHECKED);
			}
			//是否显示无故障信息
		    pMenu->EnableMenuItem(IDC_EVENT_FILTER_FAULT, MF_ENABLED);
			if (m_bShowFault)
			{
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_FAULT,MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_FAULT,MF_UNCHECKED);
			}
			

			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87B830188
void CViewOSC::OnEventAck() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//无权限
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
		return;
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				if(pEvent->Ack())
				{
					//刷新界面显示, 通知其它窗口更新
					RefreshAck(pEvent, nIndex);
					TellOtherWndAck(pEvent);
				}
			}
		}
	}
}

//##ModelId=49B87B83018A
void CViewOSC::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//无权限
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckAllEvent(EX_STTP_INFO_OSC_LIST))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventOSC* pEvent = (CXJEventOSC*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

//##ModelId=49B87B83018C
void CViewOSC::OnEventCopy() 
{
	// TODO: Add your command handler code here
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
		return;
	int nIndex = -1;
	CString str = "";
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				str += pEvent->GetDescribe();
			}
		}
	}
	
	//复制str到剪贴板
	if (OpenClipboard())
	{
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GHND, str.GetLength()+1 );
		LPTSTR pszDst = (LPTSTR)GlobalLock(hMem);
		lstrcpy(pszDst,str);
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();
	}
}

//##ModelId=49B87B830197
void CViewOSC::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	m_bCBoxShowReady = m_bShowAcked;
	//重新填充数据
	RefreshData();
}

//##ModelId=49B87B830199
void CViewOSC::OnEventProp() 
{
	// TODO: Add your command handler code here
	//只处理选择了一条事件的情况
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	//得到事件
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//设置当前事件
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

//##ModelId=49B87B83019B
void CViewOSC::OnEventSec() 
{
	// TODO: Add your command handler code here
	//只处理选择了一条事件的情况
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	//得到事件
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pEvent->m_pSec);
}

//##ModelId=49B87B8301A5
void CViewOSC::OnEventSecProp() 
{
	// TODO: Add your command handler code here
	//只处理选择了一条事件的情况
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	//得到事件
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//显示该事件的二次设备属性页
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	nIndex = PT_PROP_OSCLIST;
	if(pEvent->m_pSec != NULL)
	{
		if(pEvent->m_pSec->m_nType == TYPE_WAVEREC)
			nIndex = WR_PROP_OSC;
	}
	pDoc->ShowSecPropPage(pEvent->m_pSec, nIndex);
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数：param1	比较对象1
		   Param2	比较对象2
		   param3	比较参数
**************************************************************/
//##ModelId=49B87B83008F
int CALLBACK CViewOSC::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CXJEventOSC* pEvent1 = (CXJEventOSC*)lParam1;
	CXJEventOSC* pEvent2 = (CXJEventOSC*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int nPos1 = -1;
	int nPos2 = -1;
	int iResult = 0;
	//"序号","故障类型", "录波器名称","厂站名","起始时间","录波数据文件", "跳闸相别", "重合闸时间", "故障测距", "已确认"
	switch(nCol)
	{
	case 0: //事件ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //故障类型
		if(pEvent1->m_nHaveFault == 1)
			str1 = pEvent1->GetFaultType();
		else
			str1 = StringFromID(IDS_FAULT_NOFAULT);
		if(pEvent2->m_nHaveFault == 1)
			str2 = pEvent2->GetFaultType();
		else
			str2 = StringFromID(IDS_FAULT_NOFAULT);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //录波器名称
		str1 = pEvent1->m_pSec->m_sName;
		str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //厂站名
		str1 = pEvent1->m_sStationName;
		str2 = pEvent2->m_sStationName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //起始时间
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //录波数据文件
		str1 = pEvent1->m_sFileName;
		str2 = pEvent2->m_sFileName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //跳闸相别
		if(pEvent1->m_nHaveFault)
			str1 = pEvent1->GetTriggerType(pEvent1->m_sTriggerType);
		else 
			str1 = "--";
		if(pEvent2->m_nHaveFault)
			str2 = pEvent2->GetTriggerType(pEvent2->m_sTriggerType);
		else
			str2 = "--";
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 7: //重合闸时间
		if(pEvent1->m_nHaveFault == 0)
			pEvent1 ->m_nRecloseTime = 0;
		if(pEvent2->m_nHaveFault == 0)
			pEvent2->m_nRecloseTime = 0;
		iResult = pEvent1->m_nRecloseTime - pEvent2->m_nRecloseTime;
	case 8: //故障测距
		if(pEvent1->m_nHaveFault == 0)
			str1 = "--";
		else
			str1 = pEvent1->m_sFaultDistance;
		if(pEvent2->m_nHaveFault == 0)
			str2 = "--";
		else
			str2 = pEvent2->m_sFaultDistance;
		nPos1 = str1.Find('+', 0);
		nPos2 = str2.Find('+', 0);
		if(nPos1 == -1 && nPos2 == -1)
		{
			//都是实数
			float dis1 = atof(str1);
			float dis2 = atof(str2);
			iResult = dis1 - dis2;
		}
		else if(nPos1 == -1 && nPos2 > -1)
		{	
			//1为实数,2为复数,认为实数比复数小
			iResult = -1;
		}
		else if(nPos1 > -1 && nPos2 == -1)
		{
			//1为复数,2为实数.认为实数比复数小
			iResult = 1;
		}
		else if(nPos1 > -1 && nPos2 > -1)
		{
			iResult = CompareComplex(str1, str2);
		}
		break;
	case 9: //是否已确认
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iOSCViewAsc;
	return iResult;
}

/*************************************************************
 函 数 名：OnColumnclickListOsc()
 功能概要：响应点击列头事件,对被点击列进行排序
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8301A7
void CViewOSC::OnColumnclickListOsc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iOSCViewAsc = - g_iOSCViewAsc;
	}
	else
	{
		g_iOSCViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnEventPropAck()
 功能概要：响应事件属性对话框确认事件消息, 刷新界面显示
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87B8301BC
void CViewOSC::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_OSC)
		return;
	CXJEventOSC* pEvent = (CXJEventOSC*)lParam;
	if(pEvent == NULL)
		return;
	//刷新界面显示
	RefreshAck(pEvent);
}

/*************************************************************
 函 数 名：TellOtherWndAck()
 功能概要：事件确认成功后, 通知其它窗口刷新界面显示
 返 回 值: void
 参    数：param1	被确认的事件
**************************************************************/
//##ModelId=49B87B8300FD
void CViewOSC::TellOtherWndAck( CXJEventOSC* pEvent )
{
	//发送消息通知事件属性对话框
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
	if(pFrame != NULL)
	{
		pFrame->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
	}
}

/*************************************************************
 函 数 名：OnSTTP20132()
 功能概要：响应20132, 录波文件自动上送通知
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8301C6
void CViewOSC::OnSTTP20132( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewOSC::OnSTTP20132 start", XJ_LOG_LV3);

	CXJEventOSC * pEvent = (CXJEventOSC *)lParam;
	if(!pEvent)
		return;
	
	if (pEvent->m_sStationId != m_sChooseStationID&& !m_sChooseStationID.IsEmpty())
	{
		CString str;
		str.Format("收到厂站ID为%s报文，并非当前选定的厂站，已过滤。",pEvent->m_sStationId);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//不显示调试信息的情况
	if(!m_bShowDebug  && pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		WriteLog("CViewDI::OnSTTP20132, 调试事件不显示", XJ_LOG_LV3);
		return;
	}
	//不显示无故障信息的情况
	if(!m_bShowFault  && pEvent->m_nHaveFault == 1)
	{
		WriteLog("CViewDI::OnSTTP20132, 无故障事件不显示", XJ_LOG_LV3);
		return;
	}

	CXJEventOSC* pNew = (CXJEventOSC*)pEvent->Clone();
	if(!pNew)
		return;
	//加入到链表
	m_EventManager.AddEvent(pNew);
	//加入到列表,加入到第一行
	AddEventToList(pNew, 0);
	CheckLineCount();
	
	WriteLog("CViewOSC::OnSTTP20132 neat end", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8301AB
void CViewOSC::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

/*************************************************************
 函 数 名：CheckLineCount()
 功能概要：检查列表中的元素数是否多出了指定的个数,如果是,删除后面的
 返 回 值: void
**************************************************************/
//##ModelId=49B87B830109
void CViewOSC::CheckLineCount()
{
	int nCount = m_List.GetItemCount();
	int nDel = nCount - m_nMaxRow;
	if(nDel <= 0)
	{
		//列表中显示数没超过最大数
		return;
	}
	//删除最后一项,直到数量符合
	for(int i = 0; i < nDel; i++)
	{
		int nNewCount = m_List.GetItemCount();
		//取得最后一项
		CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nNewCount - 1);
		//从列表中删除
		m_List.DeleteItem(nNewCount - 1);
		//从事件链表和内存中删除
		m_EventManager.RemoveAtEvent(pEvent);
	}
}

//##ModelId=49B87B8301B5
void CViewOSC::OnDblclkListOsc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected > -1)
	{
		//点击到事件, 确认此事件
		//得到该行对应的事件
		CXJEventOSC * pEvent = (CXJEventOSC*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//设置当前事件
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}

//##ModelId=49B87B8301B9
BOOL CViewOSC::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_OSC);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

void CViewOSC::OnEVENTRefresh() 
{
	// TODO: Add your command handler code here
	RefreshData();
}

/*************************************************************
 函 数 名: OnViewRefresh()
 功能概要: 响应页面强制刷新消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewOSC::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
	WriteLog("CViewOSC::OnViewRefresh, 强制刷新完毕", XJ_LOG_LV3);
}

void CViewOSC::OnEventQueryevent() 
{
	// TODO: Add your command handler code here
	//只处理选择了一条事件的情况
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	//得到事件
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	CSecObj* pObj = pEvent->m_pSec;
	if(pObj == NULL)
		return;
	//组建查询参数
	CString sQuery = "";
	if(pObj->m_nType == TYPE_PROTECT)
		sQuery.Format("0$$$ $$$99$$$ $$$%s$$$99$$$2$$$1", pObj->m_sID);
	else
		sQuery.Format("0$$$ $$$99$$$%s$$$ $$$99$$$2$$$1", pObj->m_sID);
	OpenXJQuery(sQuery, GetSafeHwnd());
}

/*************************************************************
 函 数 名: LoadConfig()
 功能概要: 读取配置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewOSC::LoadConfig()
{
	CString strIniFile = g_SysConfig;
	
	//最大显示条数
	m_nMaxRow = GetPrivateProfileInt("OSCViewConfig", "MaxRow", 100, strIniFile);
	//是否显示已确认事件
	m_bShowAcked = GetPrivateProfileInt("OSCViewConfig", "ShowAcked", 1, strIniFile);
	//是否显示调试事件
	m_bShowDebug = GetPrivateProfileInt("OSCViewConfig", "ShowDebug", 1, strIniFile);
	//是否显示无故障事件
	m_bShowFault = GetPrivateProfileInt("OSCViewConfig", "ShowFault", 1, strIniFile);
	//设定checkbox选项
	m_bCBoxShowReady = m_bShowAcked;
	m_bCBoxShowDebug = m_bShowDebug;
	m_bCBoxShowFault = m_bShowFault;
}

/*************************************************************
 函 数 名: SaveConfig()
 功能概要: 保存配置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewOSC::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//最大显示条数
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString("OSCViewConfig", "MaxRow", str, strIniFile);
	//是否显示已确认事件
	str.Format("%d", m_bShowAcked);
	WritePrivateProfileString("OSCViewConfig", "ShowAcked", str, strIniFile);
	//是否显示调试事件
	str.Format("%d", m_bShowDebug);
	WritePrivateProfileString("OSCViewConfig", "ShowDebug", str, strIniFile);
	//是否显示无故障事件
	str.Format("%d", m_bShowFault);
	WritePrivateProfileString("OSCViewConfig", "ShowFault", str, strIniFile);
}

void CViewOSC::OnEventFilterDebug() 
{
	// TODO: Add your command handler code here
	m_bShowDebug = !m_bShowDebug;
	m_bCBoxShowDebug = m_bShowDebug;
	//重新填充数据
	RefreshData();
}
void CViewOSC::OnEventFilterFault()
{
	// TODO: Add your command handler code here
	m_bShowFault = !m_bShowFault;
	m_bCBoxShowFault = m_bShowFault;
	//重新填充数据
	RefreshData();

}
void CViewOSC::OnEventOpenosc() 
{
	// TODO: Add your command handler code here
	//只处理选择了一条事件的情况
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	//得到事件
	CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	if(pEvent->IsDownloaded() != 1)
	{
		if(AfxMessageBox(StringFromID(IDS_COMMON_NEED_DOWNLOAD), MB_OKCANCEL)==IDOK)
		{
			//增加到下载页面
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			pApp->ShowDownOsc(pEvent);
		}
		return;
	}
	CString strFilePath = pEvent->GetFilePath();
	//打开文件
	OpenWaveFile(strFilePath, GetSafeHwnd());
}

void CViewOSC::OnEventDownosc() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
		return;
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				//增加到下载页面
				pApp->ShowDownOsc(pEvent);
			}
		}
	}
}
/*************************************************************
 函 数 名: InitComBox()
 功能概要: 载入combox列表信息 默认为第一项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewOSC::InitComBox()
{

 	//用厂站中的数据填充列表
	m_cmbChooseStation.ResetContent();

	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);


 	POSITION pos = listDevice.GetHeadPosition();
	m_cmbChooseStation.InsertString(0," "+StringFromID(IDS_COMMON_ALL)+" ");
 	while(pos != NULL)
 	{
 		 CStationObj*pStation = (CStationObj*)listDevice.GetNext(pos);
 		if(pStation == NULL)
 			continue;
		//加入到下拉框
		int nIndex = m_cmbChooseStation.AddString(pStation->m_sName);
		m_cmbChooseStation.SetItemData(nIndex,(DWORD)pStation);
	}
	m_cmbChooseStation.SetCurSel(0);
	m_cmbChooseStation.OnDataFilled(TRUE, 0);
}
void CViewOSC::OnSelchangeCOMBOChooseStation() 
{
	// TODO: Add your control notification handler code here
	//获取当前选择项
	int nIndex=((CComboBox*)GetDlgItem(IDC_COMBO_CHOOSESTATION))->GetCurSel();
	
	CStationObj* pStationid=(CStationObj*)m_cmbChooseStation.GetItemData(nIndex);
	if(pStationid)	
	{
		CString sStationID=	pStationid->m_sID;
		m_sChooseStationID.Format("%s",sStationID);
	}
	else
		m_sChooseStationID = "";
	
	//刷新列表信息
	InitListCtrl();
}



void CViewOSC::OnCheckShowdebug() 
{
	// TODO: Add your control notification handler code here
	//修改选定状态
	m_bCBoxShowDebug=!m_bCBoxShowDebug;
	
	m_bShowDebug=!m_bShowDebug;
	
	//重新填充数据
	RefreshData();
}




void CViewOSC::OnCheckShowready() 
{
	// TODO: Add your control notification handler code here
	//修改选定状态
	m_bCBoxShowReady=!m_bCBoxShowReady;
	
	m_bShowAcked=!m_bShowAcked;

	//重新填充数据
	RefreshData();
	
}

void CViewOSC::OnCheckShowFault() 
{
	// TODO: Add your control notification handler code here
	//修改选定状态
	m_bCBoxShowFault=!m_bCBoxShowFault;
	
	m_bShowFault=!m_bShowFault;
	
	//重新填充数据
	RefreshData();
	
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   RefreshData	
返回值:   int	
功能概要: 
*****************************************************/
int CViewOSC::RefreshData()
{
	if(!LoadDataFromDB(m_nMaxRow,m_bShowAcked))
	{
		//读取数据失败
		WriteLog("录波事件视图读取数据失败", XJ_LOG_LV2);
		return 0;
	}
	else
	{
		//成功
		FillListData();
	}
	WriteLog("录波事件视图刷新完毕", XJ_LOG_LV3);
	return 1;
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewOSC::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	InitComBox();//重新载入厂站选择下拉框
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	BOOL bNeedReload = FALSE;
	CXJEventOSC * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventOSC*)pList->GetAt(i);
			if(pEvent == NULL)
				continue;
			if(pEvent->m_sStationId == pStation->m_sID)
			{
				bNeedReload = TRUE;
				break;
			}
		}
	}
	if(bNeedReload)
	{
		RefreshData();
		WriteLog("StationInit,录波简报页面刷新完毕", XJ_LOG_LV3);
	}
	else
		WriteLog("StationInit,录波简报页面无须刷新", XJ_LOG_LV3);
}

