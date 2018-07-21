/********************************************************************
	created:	2008/09/23
	created:	23:9:2008   16:45
	filename: 	F:\vicente\Code\src\XJBrowser\ViewAction.cpp
	file path:	F:\vicente\Code\src\XJBrowser
	file base:	ViewAction
	file ext:	cpp
	author:		LYH
	
	purpose:	
*********************************************************************/
// ViewAction.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewAction.h"
#include "XJBrowserDoc.h"
#include "DataEngine.h"
#include "DlgEventProp.h"
#include "MainFrm.h"
#include "DlgEventSignal.h"
#include "GlobalFunc.h"

#define VIEWACTION_COL_NUM 11

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

UINT ActionViewLoadData(LPVOID pParam)
{
	if(pParam == NULL)
		return -1;
	CViewAction* pView = (CViewAction*)pParam;


	//读取数据
	if(pView->LoadDataFromDB())
	{
		pView->PostMessage(THREAD_FILL_DATA, 0, 0);
	}
	pView->NotifyThreadEnd();
	return 0;
}

/** @brief           升降序作用位*/
int g_iActionViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewAction

IMPLEMENT_DYNCREATE(CViewAction, CFormView)


CViewAction::CViewAction()
	: CFormView(CViewAction::IDD)
{
	//{{AFX_DATA_INIT(CViewAction)
	m_bCBoxShowDebug = TRUE;
	m_bCBoxShowReady = TRUE;
	m_bCBoxShowReturn = TRUE;
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_EventManager.ClearAllEvents();
	m_nOldSortCol = -1;
	g_iActionViewAsc = -1;
	m_bShowDebug = TRUE;
	m_bShowReturn = TRUE;
	m_bQuitThread = FALSE;
	m_sChooseStationID = "" ;
	m_pLoadDataThread = NULL;
	InitializeCriticalSection(&m_CriticalSection);
}


CViewAction::~CViewAction()
{
	DeleteCriticalSection(&m_CriticalSection);
	m_arrColum.RemoveAll();
	m_EventManager.ClearAllEvents();
	WriteLog("CViewAction::~CViewAction ");
}

void CViewAction::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewAction)
	DDX_Control(pDX, IDC_CHECK_SHOWRETURN, m_cboxShowReturn);
	DDX_Control(pDX, IDC_CHECK_SHOWREADY, m_cboxShowReady);
	DDX_Control(pDX, IDC_CHECK_SHOWDEBUG, m_cboxShowDebug);
	DDX_Control(pDX, IDC_STATIC_CHOOSESTATION, m_StateChooseStation);
	DDX_Control(pDX, IDC_COMBO_CHOOSESTATION, m_cmbChooseStation);
	DDX_Control(pDX, IDC_LIST_ACTION, m_List);
	DDX_Check(pDX, IDC_CHECK_SHOWDEBUG, m_bCBoxShowDebug);
	DDX_Check(pDX, IDC_CHECK_SHOWREADY, m_bCBoxShowReady);
	DDX_Check(pDX, IDC_CHECK_SHOWRETURN, m_bCBoxShowReturn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewAction, CFormView)
	//{{AFX_MSG_MAP(CViewAction)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_ACTION, OnClickListAction)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ACTION, OnItemchangedListAction)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_ACTION, OnCustomDraw)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_DEVICE, OnEventDevice)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ACTION, OnColumnclickListAction)
	ON_COMMAND(IDC_EVENT_SIGN, OnEventSign)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ACTION, OnDblclkListAction)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	ON_COMMAND(IDC_EVENT_QUERYEVENT, OnEventQueryevent)
	ON_COMMAND(IDC_EVENT_FILTER_DEBUG, OnEventFilterDebug)
	ON_COMMAND(IDC_EVENT_FILTER_RETURN, OnEventFilterReturn)
	ON_COMMAND(IDC_EXPORT_EXCEL, OnExportExcel)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSESTATION, OnSelchangeCOMBOChooseStation)
	ON_BN_CLICKED(IDC_CHECK_SHOWREADY, OnCheckShowready)
	ON_BN_CLICKED(IDC_CHECK_SHOWDEBUG, OnCheckShowdebug)
	ON_BN_CLICKED(IDC_CHECK_SHOWRETURN, OnCheckShowreturn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20022, OnSTTP20022)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(REFRESH_ACTIONVIEW, OnRefreshActionView)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnReFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewAction diagnostics

#ifdef _DEBUG

void CViewAction::AssertValid() const
{
	CFormView::AssertValid();
}


void CViewAction::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewAction message handlers

void CViewAction::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
 	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
 	pApp->SetAppTile(StringFromID(IDS_SECPROP_ACTION));
	//GetParentFrame()->SetWindowText(" 动作 ");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CViewAction::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class	
	CFormView::OnPrepareDC(pDC, pInfo);
//	RegulateControlSize();
}


void CViewAction::OnInitialUpdate() 
{
	LONGLONG startTime = StartCount();

	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	//载入配置
	LoadConfig();
	
	//初始化combox信息
	InitComBox();
	//初始化列表
	InitListCtrl();
	

	EndCount("初始化动作页面时间", startTime);
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表控件,包括初始化列表样式和填充列表数据
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
int CViewAction::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	//设置列表风格
	InitListStyle();
	//从数据库读取数据
	RefreshData();
	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
int CViewAction::InitListStyle()
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
		//char* arColumn[VIEWACTION_COL_NUM]={"序号","类别", "故障类型","二次设备", "装置启动时间","信息接收时间","相对时间","消息内容","厂站名","一次设备","已确认"};
		for (int nCol=0; nCol < VIEWACTION_COL_NUM ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 60; //列宽50象素 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 100; //列宽60象素 类别
				colName = StringFromID(IDS_COMMON_CLASS);
				break;
			case 2://
				lvCol.cx = 100; //列宽60象素 故障类型
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			case 3://
				lvCol.cx = 300; //列宽160象素 二次设备
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 4: 
				lvCol.cx = 180; //装置启动时间
				colName = StringFromID(IDS_EVENT_STARTTIME);
				break;
			case 5:
				lvCol.cx = 180; //信息接收时间
				colName = StringFromID(IDS_EVENT_RECIVETIME);
				break;
			case 6:
				lvCol.cx = 80;//相对时间
				colName = StringFromID(IDS_EVENT_RELATIVETIME);
				break;
			case 7:
				lvCol.cx = 200; //信息内容
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 8:
				lvCol.cx = 140; //厂站名
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 9:
				lvCol.cx = 160; //一次设备
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			case 10:
				lvCol.cx = 80; //已确认
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=100;//列宽100象素
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		//默认隐藏第3列(故障类型)
		m_List.SetColumnHide(2, TRUE);
		//默认隐藏"确认"
		m_List.SetColumnHide(10, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
BOOL CViewAction::LoadListConfig()
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
			WriteLog("ViewAction::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("ViewAction::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewActionConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewActionConfig");
				WriteLog("ViewAction::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewActionConfig下的配置");
					WriteLog("ViewAction::读取UIConfig失败,将使用默认列风格");
					bResult = FALSE;
				}
				else
				{
					//找到子节点列表指针
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					if( pChild->Getlength() != VIEWACTION_COL_NUM)
					{
						WriteLog("ViewAction::列数量发生变化,不读取UIConfig,将使用默认列风格");
						bResult = FALSE;
					}
					else
					{
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
	}	
	//关闭打开的COM
	CoUninitialize();
	return bResult;
}

BOOL CViewAction::SaveListConfig()
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
			WriteLog("ViewAction::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewAction::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewActionConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewActionConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewActionConfig");
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
						WriteLog("未找到ViewActionConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了ViewActionConfig下的配置, 先删除新建所有子节点");
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
int CViewAction::FillListData()
{
	if(m_bQuitThread)
		return 0;
	EnterCriticalSection(&m_CriticalSection);
	WriteLog("CViewAction::FillListData, 取得锁控制权");
	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	CXJEventAction * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			if(m_bQuitThread)
			{
				LeaveCriticalSection(&m_CriticalSection);
				return 0;
			}
			pEvent = (CXJEventAction*)pList->GetAt(i);

			AddEventToList(pEvent, i);
		}
	}
	m_cboxShowReady.SetCheck(m_bCBoxShowReady);
	m_cboxShowDebug.SetCheck(m_bCBoxShowDebug);
	m_cboxShowReturn.SetCheck(m_bCBoxShowReturn);
	//恢复刷新
	m_List.SetRedraw(TRUE);
	LeaveCriticalSection(&m_CriticalSection);
	WriteLog("CViewAction::FillListData, 释放锁控制权");
	return 0;
}

/*************************************************************
 函 数 名：NeedSave()
 功能概要：判断是否列信息有改变需要保存
 返 回 值: 需要保存返回TRUE, 不需要保存返回FALSE
**************************************************************/
//##ModelId=49B87B8702CE
BOOL CViewAction::NeedSave()
{
	BOOL bReturn = FALSE;

	//先读取最新列信息
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
 功能概要：调整控件大小, 此处列表控件占整个客户区大小
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8702DE
void CViewAction::RegulateControlSize()
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
		m_StateChooseStation.ShowWindow(FALSE);
		m_cmbChooseStation.ShowWindow(FALSE);
		m_cboxShowDebug.ShowWindow(FALSE);
		m_cboxShowReturn.ShowWindow(FALSE);
		m_cboxShowReady.ShowWindow(FALSE);
		rcList=rc;
	}
	else
	{
		rcList=CRect(rc.left,rc.top+45,rc.right,rc.bottom);
	}
	
	
	//调整列表框大小,占整个高度
	m_List.MoveWindow(&rcList, TRUE);
}


void CViewAction::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

void CViewAction::OnDestroy() 
{
	//保存配置
	SaveConfig();
	//保存UI设置
	if(NeedSave())
		SaveListConfig();

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：LoadDataFromDB()
 功能概要：从数据库中读取最新事件,
			每一条信息新建一个事件对象保存在事件管理对象中
 返 回 值: 读取成功返回TRUE, 失败返回FLASE
  参    数：param1   要查找的个数
			param2	 是否查询已确认字段, TRUE为查询,FALSE为不查询
**************************************************************/
BOOL CViewAction::LoadDataFromDB()
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	if(m_bQuitThread)
		return FALSE;

	EnterCriticalSection(&m_CriticalSection);
	WriteLog("CViewAction::LoadDataFromDB, 取得锁控制权");
	m_EventManager.ClearAllEvents();

	//因为返回信息需要先解析才知道是否事件是否返回信息，先读取num*5的事件
	
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
	str.Format("%d", m_nMaxRow*3);
	strncpy(Field0.FieldName, str, str.GetLength());
	Field0.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field0);

	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);

	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "msg", EX_STTP_DATA_TYPE_STRING);

	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "pt_id", EX_STTP_DATA_TYPE_STRING);

	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "is_ack", EX_STTP_DATA_TYPE_INT);

	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "curtime", EX_STTP_DATA_TYPE_TIME);

	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "ms", EX_STTP_DATA_TYPE_INT);

	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "ms2", EX_STTP_DATA_TYPE_INT);

	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "recvtime", EX_STTP_DATA_TYPE_TIME);

	Field Field9;
	pApp->m_DBEngine.SetField(sql, Field9, "faulttype", EX_STTP_DATA_TYPE_STRING);

	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "isdebug", EX_STTP_DATA_TYPE_INT);

	Field Field11;
	pApp->m_DBEngine.SetField(sql, Field11, "fdata", EX_STTP_DATA_TYPE_STRING);

	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "property", EX_STTP_DATA_TYPE_INT);


	//条件
	//只查询未确认事件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("IS_ACK = 0");
	strncpy(condition1.ConditionContent, str, str.GetLength());
	if(m_bShowAcked)
	{
		//要查询已确认事件, 则此条件不加
		//	sql.Conditionlist.push_back(condition1);
	}
	else
	{
		//不查询已确认事件, 则加上只查询未确认事件的条件
		sql.Conditionlist.push_back(condition1);
	}

	//检查事件类型过滤条件
	CString strFiter = BuildOrSQL();
	if(strFiter != "")
	{
		Condition condition3;
		pApp->m_DBEngine.SetCondition(sql, condition3, strFiter);
	}

	//检查显示过滤条件
	if(!m_bShowDebug)
	{
		Condition condition4;
		CString strFilter;
		strFilter.Format("IsDebug=%d", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}
	//选择显示厂站的信息
	if ( !m_sChooseStationID.IsEmpty())
	{ //选择了特定厂站
		Condition condition5;
		CString strFilter;
		strFilter.Format("(pt_id IN (select pt_id from tb_secdev_base where station_id='%s'))",m_sChooseStationID);
		pApp->m_DBEngine.SetCondition(sql,condition5,strFilter);
	}
	else if(!pApp->MgrAllStations())
	{
		Condition condition5;
		CString strFilter;
		strFilter.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition5,strFilter);
	}

	//按动作启动时间大小排序
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by RECVTIME DESC,MS2 DESC");
	//str.Format("order by recvtime desc");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition2);

			
	CMemSet pMemset;
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	if(m_bQuitThread)
	{
		LeaveCriticalSection(&m_CriticalSection);
		WriteLog("CViewAction::LoadDataFromDB, 释放锁控制权");
		return FALSE;
	}

	int nResult;
	LONGLONG startTime = StartCount();
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ACTION_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogTemp("CViewAction::LoadDataFromDB, 查询失败");
		LeaveCriticalSection(&m_CriticalSection);
		WriteLog("CViewAction::LoadDataFromDB, 释放锁控制权");
		return FALSE;
	}
	if(m_bQuitThread)
	{
		LeaveCriticalSection(&m_CriticalSection);
		WriteLog("CViewAction::LoadDataFromDB, 释放锁控制权");
		return FALSE;
	}
	EndCount("查询动作事件表时间",startTime);
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewAction::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLogTemp(str);
		}

		//符合条件数
		int nChecked = 0;
		for(int i = 0; i < nCount; i++)
		{
			if(m_bQuitThread)
			{
				LeaveCriticalSection(&m_CriticalSection);
				WriteLog("CViewAction::LoadDataFromDB, 释放锁控制权");
				return FALSE;
			}
			Sleep(g_ThreadTime);
			//判断已符合条件的个数
			if(nChecked == m_nMaxRow)
				break;
			//创建事件
			CXJEventAction * pEvent = new CXJEventAction;
			if(pEvent->LoadFromDB(&pMemset))
			{
				//读取数据成功
				if(!m_bShowReturn)
				{
					//不显示返回事件
					if(pEvent->m_nIsReturn == 1)
					{
						//删除此事件
						delete pEvent;
						pEvent = NULL;
						pMemset.MoveNext();
						continue;
					}
				}
				//加入到链表
				m_EventManager.AddEvent(pEvent);
				nChecked++;
				if(m_bQuitThread)
				{
					LeaveCriticalSection(&m_CriticalSection);
					WriteLog("CViewAction::LoadDataFromDB, 释放锁控制权");
					return FALSE;
				}
			}
			else
			{
				//读取数据失败, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset.MoveNext();
		}
		if(nChecked < m_nMaxRow)
		{
			CString str;
			str.Format("CViewAction::LoadDataFromDB, 要显示%d条数据,只找到%d条数据", m_nMaxRow, nChecked);
			WriteLogTemp(str);
		}
	}
	else
	{
		CString str;
		str.Format("CViewAction::LoadDataFromDB,查询失败,原因为%s", sError);
		WriteLogTemp(str);
		bReturn = FALSE;
	}
	LeaveCriticalSection(&m_CriticalSection);
	WriteLog("CViewAction::LoadDataFromDB, 释放锁控制权");
	WriteLog("CViewAction::LoadDataFromDB, 查询正常结束");
	return bReturn;
}

/*************************************************************
 函 数 名：AddEventToList()
 功能概要：加入事件信息到列表
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数：param1   要加入的事件指针
           Param2   要加入的位置,从0开始计数
**************************************************************/
BOOL CViewAction::AddEventToList(CXJEventAction * pEvent, int i)
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;

	//0:序号
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	//1:类别
	if(pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		//调试
		m_List.SetItemText(i, 1, StringFromID(IDS_RUNSTATUS_TESTING));
	}
	else
	{
		//正常
		m_List.SetItemText(i, 1, pEvent->GetAlarmType());
	}
	//2.故障类型
	m_List.SetItemText(i, 2, pEvent->GetFaultType());
	//3:保护名称
	CString str;
	m_List.SetItemText(i, 3, pEvent->m_pSec->m_sName);
	//4:装置启动时间
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 4, sTime);
	//5:信息接收时间
	sTime.Format("%s.%03d", pEvent->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms2);
	m_List.SetItemText(i, 5, sTime);
	//6:相对时间
	str.Format("%d ms", pEvent->m_nRelativeTime);
	m_List.SetItemText(i, 6, str);
	//6:消息内容
	m_List.SetItemText(i, 7, pEvent->m_sMsg);
	//7:厂站名
	m_List.SetItemText(i, 8, pEvent->m_sStationName);
	//8:一次设备名
	m_List.SetItemText(i, 9, pEvent->m_sDeviceName);
	//9:已确认
	m_List.SetItemText(i, 10, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//设置数据s
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

void CViewAction::OnClickListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected < 0)
	{
		//父窗口是分割窗口,父窗口的父窗口是ActionFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//向父窗口发送SELECTCHANGED消息, 选择了空行,发送空指针
		pWnd ->PostMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	*pResult = 0;
}


void CViewAction::OnItemchangedListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if((pNMListView->uChanged   &   LVIF_STATE)&&
		(pNMListView->uNewState   &   LVIS_SELECTED)) 
	{
		//事实上鼠标点击的选择动作, 会产生三次ITEMCHANGED消息, 
		//1是A的文字背景消失触发，2是A取消选中触发，3是B被选中触发
		//为避免刷新太频繁, 这里只处理被选中消息, 这样处理会漏掉选择了空行的消息
		//所以把选择空行放在NM_CLICK中处理, 对于移动键盘上下键导致的选择到空行不做处理

		int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
		if(nSelected < 0)
			return;
		
		//得到该行对应的事件
		CXJEventAction * pEvent = (CXJEventAction*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//父窗口是分割窗口,父窗口的父窗口是ActionFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//向父窗口发送SELECTCHANGED消息
		pWnd ->PostMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)pEvent);
	}
	
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnSTTP20022()
 功能概要：响应20022(保护动作事件上载通知)报文
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B88005E
void CViewAction::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewAction::OnSTTP20022 start");

	CXJEventAction* pParam = (CXJEventAction*)lParam;
	if(pParam == NULL)
		return;

	if (pParam->m_sStationId != m_sChooseStationID && !m_sChooseStationID.IsEmpty())
	{
		CString str;
		str.Format("收到厂站ID为%s报文，并非当前选定的厂站，已过滤。",pParam->m_sStationId);
		WriteLog(str);
		return;
	}

	if(pParam != NULL)
	{
		CString str;
		str.Format("装置ID:%s,名称:%s,厂站ID:%s", pParam->m_sSecID,pParam->m_sSecName,pParam->m_sStationId);
		WriteLog(str);
	}
	
	
	//不显示调试信息的情况
	if(!m_bShowDebug && pParam->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		WriteLog("CViewAction::OnSTTP20022, 调试事件不显示");
		return;
	}

	//不显示返回信息
	if(!m_bShowReturn && pParam->m_nIsReturn == 1)
	{
		WriteLog("CViewAction::OnSTTP20022, 返回事件不显示");
		return;
	}

	//复制
	CXJEventAction* pEvent = (CXJEventAction*)pParam->Clone();
	if(pEvent == NULL)
		return;

	if(m_pLoadDataThread)
		return;//如果线程正在读入数据则不处理此报文

	//加入到链表
	m_EventManager.AddEvent(pEvent);
	//加入到列表
	AddEventToList(pEvent, 0); //加入到第一行
	CheckLineCount();

	//弹出事件属性框
	if(g_ShowEventDlg == 1)
	{
		CXJBrowserApp* pApp  = (CXJBrowserApp*)AfxGetApp();
		m_EventManager.m_pCurrentEvent = pEvent;
		pApp->ShowEventProp(&m_EventManager);
	}

	WriteLog("CViewAction::OnSTTP20022 neat end");
	
	
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置I,II,III类事件背景色, 未确认事件文本颜色
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CViewAction::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(pNMHDR ->code == NM_CUSTOMDRAW)
	{
		LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
		int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //行索引
		//TRACE1("CViewAction::OnCustomDraw  %d\n", nItem);
		switch(lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			{
				*pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
			//	lplvcd->clrTextBk = g_ListSpaceColor2;
				break;
			}
		case CDDS_ITEMPREPAINT:
			{
				*pResult = CDRF_NOTIFYSUBITEMDRAW;
			//	lplvcd->clrTextBk = g_ListSpaceColor2;
				break;
			}
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM :
			{
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //行索引
				//TRACE1("CViewAction::OnCustomDraw  %d\n", nItem);
				int nSubItem = lplvcd->iSubItem; //列索引
				//先给默认值, 后面再特殊处理
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				//按事件类型(I, II, III)给定背景色
				if(nItem  != -1)
				{
					if (g_role == MODE_SUB)
					{
						//得到事件对象
						CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nItem);
						if(pEvent == NULL)
							break;
						if(pEvent->m_nIsAck == 0)
						{
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					}


					//第二列，事件类型
 					if(nSubItem == 1)
 					{
						//得到事件对象, 要保证EventManager里的列表顺序与视图list中的顺序一样
						CXJEventAction * pEvent = (CXJEventAction*)m_List.GetItemData(nItem);
						if(pEvent == NULL)
							break;
						if(pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
						{
							//调试
							lplvcd->clrText = XJ_STATUS_RUN_CHECK;
						}
						else
						{
							CString strAlarm = pEvent->GetAlarmType();
							if(strAlarm == "I")
							{
								//I类事件,背景色
								lplvcd->clrText = g_colEventI;
							}
							else if(strAlarm == "II")
							{
								//II类事件,背景色s
								lplvcd->clrText = g_colEventII;
							}
							else if(strAlarm == "III")
							{
								//III类事件,背景色
								lplvcd->clrText = g_colEventIII;
							}
						}
					}

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


void CViewAction::OnContextMenu(CWnd* pWnd, CPoint point) 
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

			
			//Menu.DeleteMenu(IDC_EVENT_COPY, MF_BYCOMMAND);
			//Menu.DeleteMenu(IDC_EVENT_ACK, MF_BYCOMMAND);
			//Menu.DeleteMenu(IDC_EVENT_ALL, MF_BYCOMMAND);
			//Menu.DeleteMenu(IDC_EVENT_PROP, MF_BYCOMMAND);
			//Menu.DeleteMenu(IDC_EVENT_Refresh, MF_BYCOMMAND);
			

			pMenu = Menu.GetSubMenu(0);

			int nCount = m_List.GetSelectedCount();
			
			if(nCount > 0)
			{
				//有选择
				pMenu->EnableMenuItem(IDC_EVENT_COPY, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_ACK, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SIGN, MF_ENABLED);
			}
			if(nCount == 1)
			{
				//只选择了一个
				pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_PROP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_QUERYEVENT, MF_ENABLED);
			}
			if(m_bShowAcked)
			{
				//显示已确认事件
				pMenu->CheckMenuItem(IDC_EVENT_HIDE, MF_CHECKED);
			
			}
			else
			{
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
			
			//是否显示返回
			pMenu->EnableMenuItem(IDC_EVENT_FILTER_RETURN, MF_ENABLED);
			if(m_bShowReturn)
			{
				//显示调试
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_RETURN, MF_CHECKED);
				
			}
			else
			{
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_RETURN, MF_UNCHECKED);
			}
	
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

/*************************************************************
 函 数 名：OnEventAck()
 功能概要：确认所选事件
 返 回 值: void
**************************************************************/
void CViewAction::OnEventAck() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//无权限
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
			CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				if(pEvent->Ack())
				{
					//确认成功,更新显示
					RefreshAck(pEvent, nIndex);
					TellOtherWndAck(pEvent);
				}
			}
		}
	}
}


void CViewAction::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//无权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckAllEvent(EX_STTP_INFO_PT_ACTION_DATA))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventAction* pEvent = (CXJEventAction*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

/*************************************************************
 函 数 名：OnEventCopy()
 功能概要：复制所选事件的详细描述到剪切板
 返 回 值: void
**************************************************************/
void CViewAction::OnEventCopy() 
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
			CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
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

/*************************************************************
 函 数 名：OnEventDevice()
 功能概要：定位到相关联的一次设备画面
 返 回 值: void
**************************************************************/
void CViewAction::OnEventDevice() 
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
	CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pEvent->m_pSec->m_pOwner == NULL)
		return;
	pApp->ShowDevice(pEvent->m_pSec->m_pOwner);
}

/*************************************************************
 函 数 名：OnEventHide()
 功能概要：改变是否隐藏已确认事件, 重新填充事件
 返 回 值: void
**************************************************************/
void CViewAction::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	m_bCBoxShowReady = m_bShowAcked;
	//重新填充数据
	//从数据库读取数据
	RefreshData();
}

/*************************************************************
 函 数 名：OnEventProp()
 功能概要：查看事件属性
 返 回 值: void
**************************************************************/
void CViewAction::OnEventProp() 
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
	CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//设置当前事件
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

/*************************************************************
 函 数 名：OnEventSec()
 功能概要：定位到相关联的二次设备画面
 返 回 值: void
**************************************************************/
void CViewAction::OnEventSec() 
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
	CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pEvent->m_pSec);
}


void CViewAction::OnEventSecProp() 
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
	CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	//打开保护动作事件页
	pDoc->ShowSecPropPage(pEvent->m_pSec, PT_PROP_ACTION);
}

/*************************************************************
 函 数 名：RefreshAck()
 功能概要：事件确认后,更新显示
 返 回 值: void
 参    数：param1	确认的事件
		   Param2	事件位置
**************************************************************/
void CViewAction::RefreshAck( CXJEventAction* pEvent, int nIndex )
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
	m_List.SetItemText(nIndex, VIEWACTION_COL_NUM-1, StringFromID(IDS_ACK_YES));
	//刷新
// 	CRect rc;
// 	m_List.GetItemRect(nIndex, &rc, LVIR_BOUNDS);
// 	m_List.InvalidateRect(rc);
	m_List.Update(nIndex);
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
int CALLBACK CViewAction::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CXJEventAction* pEvent1 = (CXJEventAction*)lParam1;
	CXJEventAction* pEvent2 = (CXJEventAction*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号","类别", "故障类型","保护名称", "装置启动时间","信息接收时间","消息内容","厂站名","一次设备","已确认"
	switch(nCol)
	{
	case 0: //事件ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //类别
		if(pEvent1->m_IsDebug == XJ_EVENTINFO_DEBUG)
			str1 = StringFromID(IDS_RUNSTATUS_TESTING);
		else
			str1 = pEvent1->GetAlarmType();
		if(pEvent2->m_IsDebug == XJ_EVENTINFO_DEBUG)
			str2 = StringFromID(IDS_RUNSTATUS_TESTING);
		else
			str2 = pEvent2->GetAlarmType();
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //故障类型
		str1 = pEvent1->GetFaultType();
		str2 = pEvent2->GetFaultType();
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //保护名称
		str1 = pEvent1->m_pSec->m_sName;
		str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //装置启动时间
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //信息接收时间
		str1.Format("%s.%03d", pEvent1->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms2);
		str2.Format("%s.%03d", pEvent2->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms2);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6:
		iResult = pEvent1->m_nRelativeTime - pEvent2->m_nRelativeTime;
		break;
	case 7: //消息内容
		str1 = pEvent1->m_sMsg;
		str2 = pEvent2->m_sMsg;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 8: //厂站名
		str1 = pEvent1->m_sStationName;
		str2 = pEvent2->m_sStationName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 9: //一次设备名
		str1 = pEvent1->m_sDeviceName;
		str2 = pEvent2->m_sDeviceName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 10: //是否已确认
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iActionViewAsc;
	return iResult;
}


void CViewAction::OnColumnclickListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iActionViewAsc = - g_iActionViewAsc;
	}
	else
	{
		g_iActionViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnEventPropAck()
 功能概要：响应事件属性对话框确认事件消息
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
void CViewAction::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ACTION)
		return;
	CXJEventAction* pEvent = (CXJEventAction*)lParam;
	if(pEvent == NULL)
		return;
	//刷新界面显示
	RefreshAck(pEvent);
}

/*************************************************************
 函 数 名：TellOtherWndAck()
 功能概要：当确认事件成功后通知其它窗口改变其界面显示
 返 回 值: void
 参    数：param1	被确认的事件
		   param2	是否被动确认
**************************************************************/
void CViewAction::TellOtherWndAck( CXJEventAction* pEvent, BOOL bIsPassive)
{
	//发送消息通知事件属性对话框
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
	if(pFrame != NULL)
	{
		if(bIsPassive)
		{
			//确认动作时由保护属性事件列表发出时, 事件栏给主框架发被动确认消息
			//此消息由主框架转发给事件属性对话框
			pFrame->PostMessage(WM_EVENTLIST_PASSIVE_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
		}
		else
		{
			//事件栏主动发生确认事件动作时, 给主框架发确认事件消息
			//此消息由主框架转发给事件属性对话框和保护属性页面
			pFrame->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
		}
	}
	//父窗口是分割窗口,父窗口的父窗口是ActionFrame.
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
		pWnd ->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
}

/*************************************************************
 函 数 名：OnEventPTAck()
 功能概要：响应保护属性页面确认事件消息, 刷新界面显示, 要先找到对应的事件,因为指针是不同的
 返 回 值: void
 参    数：param1	 事件类型
		   Param2	 事件指针
**************************************************************/
void CViewAction::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ACTION)
		return;
	CXJEventAction* pEvent = (CXJEventAction*)lParam;
	if(pEvent == NULL)
		return;
	//查找在本地的地址
	CXJEventAction* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//找到了本地地址,更新显示
		RefreshAck(pLocal);
		//发送消息,因为不知道事件属性对话框显示的是什么,也发一份
		TellOtherWndAck(pLocal, TRUE);
	}
}

/*************************************************************
 函 数 名：FindLocalPoint()
 功能概要：查找指定事件在本地的地址, 因为保护属性页面的事件和事件栏的事件是不同的地址
 返 回 值: 查询到的本地地址,未找到返回NULL
 参    数：param1	要查找的事件
**************************************************************/
CXJEventAction* CViewAction::FindLocalPoint( CXJEventAction* pEvent )
{
	if(pEvent == NULL)
		return NULL;
	CXJEventAction* pReturn = NULL;
	//因为现在有ID不知道的情况,只能比较二次设备ID, 发生时间等信息
	for(int i = 0; i < m_EventManager.GetCount(); i++)
	{
		CXJEventAction* pTemp = (CXJEventAction*)m_EventManager.GetAtEvent(i);
		if(pTemp->m_pSec == pEvent->m_pSec)
		{
			//二次设备相同,比较发生时间,时间用字符串比
			if(pTemp->m_tmTime == pEvent->m_tmTime)
			{
				//发生时间相同,比较发生时间毫秒值
				if(pTemp->m_nms == pEvent->m_nms)
				{
					//发生时间毫秒值相同,比较接收时间
					if(pTemp->m_tmTime2 == pEvent->m_tmTime2)
					{
						//接收时间相同,比较接收时间毫秒值
						if(pTemp->m_nms2 = pEvent->m_nms2)
						{
							//相同,认为是同一事件
							pTemp->m_lID = pEvent->m_lID;
							pTemp->m_nIsAck = pEvent->m_nIsAck;
							pReturn = pTemp;
							break;
						}
						else
						{
							//不同,下一个
							continue;
						}
					}
					else
					{
						//接收时间不同,下一个
						continue;
					}
				}
				else
				{
					//发生时间毫秒值不同,下一个
					continue;
				}
			}
			else
			{
				//发生时间不同,下一个
				continue;
			}
		}
		else
		{
			//二次设备不同, 下一个
			continue;
		}
	}
	return pReturn;
}

/*************************************************************
 函 数 名：OnEventPropAckPT()
 功能概要：响应由保护属性页面打开的事件属性对话框发生事件确认的消息
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
void CViewAction::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ACTION)
		return;
	CXJEventAction* pEvent = (CXJEventAction*)lParam;
	if(pEvent == NULL)
		return;
	//查找在本地的地址
	CXJEventAction* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//找到了本地地址,更新显示
		RefreshAck(pLocal);
		//发送消息,只发给事件详细窗口
		//父窗口是分割窗口,父窗口的父窗口是ActionFrame.
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd != NULL)
		pWnd ->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pLocal->m_nEventType, (LPARAM)pLocal);
	}
}


void CViewAction::OnEventSign() 
{
	// TODO: Add your command handler code here
	CDlgEventSignal dlg;
	//循环所选事件,增加信号量
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
	{
		AfxMessageBox(StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				for(int j = 0; j < pEvent->m_Signals.GetSize(); j++)
				{
					PT_Signal* sign = (PT_Signal*)pEvent->m_Signals.GetAt(j);
					dlg.AddSignal(sign);
				}
			}
		}
	}
	//显示对话框
	dlg.DoModal();
}

/*************************************************************
 函 数 名：CheckLineCount()
 功能概要：检查列表中的元素数是否多出了指定的个数,如果是,删除后面的
 返 回 值: void
**************************************************************/
void CViewAction::CheckLineCount()
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


void CViewAction::OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected >= 0)
	{
		//点击到事件, 确认此事件
		//得到该行对应的事件
		CXJEventAction * pEvent = (CXJEventAction*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//设置当前事件
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}


BOOL CViewAction::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_ACTION);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

/*************************************************************
 函 数 名：BuildOrSQL()
 功能概要：组建I,II,II类信息的过滤查询语句
 返 回 值: 查询语句
 参    数：param1
		   Param2
**************************************************************/
CString CViewAction::BuildOrSQL()
{
	CString sReturn = "";
	if(g_ShowEventI == 1)
	{
		//I类信息需要显示
		sReturn += "(";
		sReturn += "property=0";
	}
	if(g_ShowEventII == 1)
	{
		//II类信息需要显示
		if(sReturn == "")
		{
			sReturn += "(";
			sReturn += "property=1";
		}
		else
		{
			sReturn += " or ";
			sReturn += "property=1";
		}
	}
	if(g_ShowEventIII)
	{
		//III类信息需要显示
		if(sReturn == "")
		{
			sReturn += "(";
			sReturn += "property=2";
		}
		else
		{
			sReturn += " or ";
			sReturn += "property=2";
		}
	}

	if(sReturn != "")
		sReturn += ")";
	return sReturn;
}

/*************************************************************
 函 数 名：OnRefreshActionView()
 功能概要：响应强制刷新动作事件列表消息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CViewAction::OnRefreshActionView( WPARAM wParam, LPARAM lParam )
{
	//父窗口是分割窗口,父窗口的父窗口是ActionFrame
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//向父窗口发送SELECTCHANGED消息, 发送空指针
		pWnd ->SendMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	
	RefreshData();
}

void CViewAction::OnEVENTRefresh() 
{
	// TODO: Add your command handler code here
	//父窗口是分割窗口,父窗口的父窗口是ActionFrame
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//向父窗口发送SELECTCHANGED消息, 发送空指针
		pWnd ->SendMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	RefreshData();
}

/*************************************************************
 函 数 名: OnViewRefresh()
 功能概要: 响应页面强制刷新消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewAction::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
}

void CViewAction::OnEventQueryevent() 
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
	CXJEventAction* pEvent = (CXJEventAction*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	CSecObj* pObj = pEvent->m_pSec;
	if(pObj == NULL)
		return;
	//组建查询参数
	CString sQuery = "";
	sQuery.Format("0$$$ $$$99$$$ $$$%s$$$99$$$0$$$1", pObj->m_sID);
	OpenXJQuery(sQuery, GetSafeHwnd());
}

/*************************************************************
 函 数 名: LoadConfig()
 功能概要: 读取配置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewAction::LoadConfig()
{
	CString strIniFile = g_SysConfig;
	
	//最大显示条数
	m_nMaxRow = GetPrivateProfileInt("ActionViewConfig", "MaxRow", 100, strIniFile);
	//是否显示已确认事件
	m_bShowAcked = GetPrivateProfileInt("ActionViewConfig", "ShowAcked", 1, strIniFile);
	//是否显示调试事件
	m_bShowDebug = GetPrivateProfileInt("ActionViewConfig", "ShowDebug", 1, strIniFile);
	//是否显示返回事件
	m_bShowReturn = GetPrivateProfileInt("ActionViewConfig", "ShowReturn", 1, strIniFile);
	//设定checkbox选项
	m_bCBoxShowReady = m_bShowAcked;
	m_bCBoxShowReturn = m_bShowReturn;
	m_bCBoxShowDebug = m_bShowDebug;
}

/*************************************************************
 函 数 名: SaveConfig()
 功能概要: 保存配置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewAction::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//最大显示条数
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString("ActionViewConfig", "MaxRow", str, strIniFile);
	//是否显示已确认事件
	str.Format("%d", m_bShowAcked);
	WritePrivateProfileString("ActionViewConfig", "ShowAcked", str, strIniFile);
	//是否显示调试事件
	str.Format("%d", m_bShowDebug);
	WritePrivateProfileString("ActionViewConfig", "ShowDebug", str, strIniFile);
	//是否显示调试事件
	str.Format("%d", m_bShowReturn);
	WritePrivateProfileString("ActionViewConfig", "ShowReturn", str, strIniFile);
}

void CViewAction::OnEventFilterDebug() 
{
	// TODO: Add your command handler code here
	m_bShowDebug = !m_bShowDebug;
	m_bCBoxShowDebug = m_bShowDebug;
	//重新填充数据
	//从数据库读取数据
	RefreshData();
}

void CViewAction::OnEventFilterReturn() 
{
	// TODO: Add your command handler code here
	
	m_bShowReturn = !m_bShowReturn;
	m_bCBoxShowReturn = m_bShowReturn;
	//重新填充数据
	//从数据库读取数据
	RefreshData();
}

void CViewAction::OnExportExcel() 
{
	/*
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("动作事件%s.xls", t.Format("%Y%m%d%H%M%S"));
	//取得用户选择文件夹
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(WriteResultToExcel(strPath))
	{
		//导出成功
		if(AfxMessageBox("导出文件成功, 按[确定]打开文件目录", MB_OKCANCEL) == IDOK)
		{
			CString str = "/n,/select,";
			str += strPath;
			ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
		}
	}
	else
	{
		//导出失败
		AfxMessageBox("导出到文件失败");
	}
	*/
}

void CViewAction::RefreshData()
{
	//父窗口是分割窗口,父窗口的父窗口是ActionFrame
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//向父窗口发送SELECTCHANGED消息, 选择了空行,发送空指针
		pWnd ->PostMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	
	//清空列表
	m_List.DeleteAllItems();
	StartThead();
	WriteLog("动作视图刷新所有数据", XJ_LOG_LV3);
}
/*************************************************************
 函 数 名: InitComBox()
 功能概要: 载入combox列表信息 默认为第一项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewAction::InitComBox()
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
	m_cmbChooseStation.InsertString(0," ("+StringFromID(IDS_COMMON_ALL)+") ");
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
/*************************************************************
 函 数 名: OnSelchangeCOMBOChooseStation()
 功能概要: 相应选择厂站的消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewAction::OnSelchangeCOMBOChooseStation() 
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
	RefreshData();
}

void CViewAction::OnCheckShowready() 
{
	// TODO: Add your control notification handler code here
	//修改选定状态
	m_bCBoxShowReady=!m_bCBoxShowReady;

	m_bShowAcked=!m_bShowAcked;
	//重新填充数据
	//从数据库读取数据
	RefreshData();
	
}

void CViewAction::OnCheckShowdebug() 
{
	// TODO: Add your control notification handler code here
	//修改选定状态
	m_bCBoxShowDebug=!m_bCBoxShowDebug;
	
	m_bShowDebug=!m_bShowDebug;
	//重新填充数据
	//从数据库读取数据
	RefreshData();
}

void CViewAction::OnCheckShowreturn() 
{
	// TODO: Add your control notification handler code here
	//修改选定状态
	m_bCBoxShowReturn=!m_bCBoxShowReturn;

	m_bShowReturn=!m_bShowReturn;
	//重新填充数据
	//从数据库读取数据
	RefreshData();
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewAction::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	
	InitComBox();//重新载入厂站选择下拉框
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	BOOL bNeedReload = FALSE;
	CXJEventAction * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventAction*)pList->GetAt(i);
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
		WriteLog("StationInit,动作事件页面刷新完毕", XJ_LOG_LV3);
	}
	else
	{
		WriteLog("StationInit,动作事件页面无须刷新", XJ_LOG_LV3);
	}
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   StartThead	
返回值:   void	
功能概要: 
*****************************************************/
void CViewAction::StartThead()
{
	EndThread();
	m_bQuitThread = FALSE;
	m_pLoadDataThread = AfxBeginThread(ActionViewLoadData,this,THREAD_PRIORITY_BELOW_NORMAL);
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   EndThread	
返回值:   void	
功能概要: 
*****************************************************/
void CViewAction::EndThread()
{
	if(!m_pLoadDataThread)
		return;
	m_bQuitThread = TRUE;
	DWORD dw = WaitForSingleObject(m_pLoadDataThread->m_hThread, INFINITE);
	switch(dw)
	{
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
		TerminateThread(m_pLoadDataThread->m_hThread,NULL);
		WriteLog("读取动作事件线程无法正常退出,强制结束", XJ_LOG_LV3);
		break;
	}
	m_pLoadDataThread = NULL;
	m_bQuitThread = FALSE;
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   NotifyThreadEnd	
返回值:   void	
功能概要: 
*****************************************************/
void CViewAction::NotifyThreadEnd()
{
	m_pLoadDataThread = NULL;
	m_bQuitThread = FALSE;
	WriteLog("CViewAction::NotifyThreadEnd 线程结束", XJ_LOG_LV3);
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   OnAllClose	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewAction::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	EndThread();
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   OnReFillData	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewAction::OnReFillData( WPARAM wParam, LPARAM lParam )
{
	FillListData();
}
