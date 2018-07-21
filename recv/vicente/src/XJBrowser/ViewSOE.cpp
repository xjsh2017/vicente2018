// ViewSOE.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewSOE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           升降序作用位*/
int g_iSOEViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewSOE

IMPLEMENT_DYNCREATE(CViewSOE, CFormView)

CViewSOE::CViewSOE()
	: CFormView(CViewSOE::IDD)
{
	//{{AFX_DATA_INIT(CViewSOE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMaxRow = 200;
	m_bShowAcked = TRUE;
	m_EventManager.ClearAllEvents();
	m_arrColum.RemoveAll();
	m_nType = 0;
	g_iSOEViewAsc = -1;
	m_nOldSortCol  = -1;
	m_bShowDebug = TRUE;
	m_bShowReturn = TRUE;
}

CViewSOE::~CViewSOE()
{
}

void CViewSOE::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSOE)
	DDX_Control(pDX, IDC_LIST_SOE, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewSOE, CFormView)
	//{{AFX_MSG_MAP(CViewSOE)
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SOE, OnCustomDraw)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_DEVICE, OnEventDevice)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SOE, OnColumnclickListSoe)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SOE, OnClickListSoe)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SOE, OnItemchangedListSoe)
	ON_COMMAND(IDC_EVENT_FILTER_DEBUG, OnEventFilterDebug)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SOE, OnDblclkListSoe)
	ON_COMMAND(IDC_EVENT_FILTER_RETURN, OnEventFilterReturn)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20022, OnSTTP20022)
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(WM_STTP_20010, OnSTTP20010)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSOE diagnostics

#ifdef _DEBUG
void CViewSOE::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewSOE::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewSOE message handlers
/*************************************************************
 函 数 名: InitListCtrl()
 功能概要: 初始化列表
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewSOE::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	RefreshData();

	return 0;
}

/*************************************************************
 函 数 名: InitListStyle()
 功能概要: 初始化列表样式
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewSOE::InitListStyle()
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
		//char* arColumn[8]={"序号","信息类别","二次设备","发生时间", "消息内容","厂站名","一次设备","已确认"};
		for (int nCol=0; nCol < 8; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName="";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; //序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 80; //类别
				colName = StringFromID(IDS_COMMON_CLASS);
				break;
			case 2://
				lvCol.cx = 300; //二次设备
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 3://
				lvCol.cx = 180; //发生时间
				colName = StringFromID(IDS_COMMON_OCCUR_TIME);
				break;
			case 4:
				lvCol.cx = 200; //消息内容
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 5:
				lvCol.cx = 140; //"厂站
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 6:
				lvCol.cx = 160; //一次设备
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			case 7:
				lvCol.cx = 80;//已确认
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(7, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 函 数 名: FillListData()
 功能概要: 填充列表数据
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewSOE::FillListData()
{
	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	CXJEvent * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEvent*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}

	g_iSOEViewAsc = -1;
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, 3);
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
	
	return 0;
}

/*************************************************************
 函 数 名: LoadListConfig()
 功能概要: 载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewSOE::LoadListConfig()
{
		//读取UI配置文件
	BOOL bResult = TRUE;
	CString strNodeName = "ViewSOEIConfig";
	if(m_nType == 1)
		strNodeName = "ViewSOEIIConfig";
	else if(m_nType == 2)
		strNodeName = "ViewSOEIIIConfig";
	else if(m_nType == 3)
		strNodeName = "ViewSOEDebugConfig";
	
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!", XJ_LOG_LV3);
			WriteLog("ViewAlarm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("ViewAlarm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			CString str;
			str.Format("//%s", strNodeName);
			pSelected = pDoc->selectSingleNode((_bstr_t)str);

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
// 				WriteLog("未找到ViewAlarmConfig");
// 				WriteLog("ViewAlarm::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
// 					WriteLog("未找到ViewAlarmConfig下的配置");
// 					WriteLog("ViewAlarm::读取UIConfig失败,将使用默认列风格");
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

/*************************************************************
 函 数 名: SaveListConfig()
 功能概要: 保存列信息配置
 返 回 值: 保存成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewSOE::SaveListConfig()
{
		//保存UI配置文件
	BOOL bResult = TRUE;
	CString strNodeName = "ViewSOEIConfig";
	if(m_nType == 1)
		strNodeName = "ViewSOEIIConfig";
	else if(m_nType == 2)
		strNodeName = "ViewSOEIIIConfig";
	else if(m_nType == 3)
		strNodeName = "ViewSOEDebugConfig";
	
	CoInitialize(NULL); //初始化COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
// 			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!");
// 			WriteLog("ViewAlarm::保存UIConfig失败", XJ_LOG_LV3);
			bResult = FALSE;
		} 	

		//加载UIConfig文件 
		if(bResult)
		{
			if(!pDoc->load((_variant_t)g_UIConfig))
			{
				//加载失败
				MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
// 				CString str;
// 				str.Format("when load UIConfig, error in line %d, char %d\n%s", errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
// 				WriteLog(str);
// 				WriteLog("ViewAlarm::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				CString str;
				str.Format("//%s", strNodeName);
				pSelected = pDoc->selectSingleNode((_bstr_t)str);
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
				//	WriteLog("未找到ViewAlarmConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement((_bstr_t)strNodeName);
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
					//	WriteLog("未找到ViewAlarmConfig下的配置, 新建所有子节点");
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
					//	WriteLog("找到了ViewAlarmConfig下的配置, 先删除新建所有子节点");
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
 函 数 名: NeedSave()
 功能概要: 判断是否需要保存列表的列信息
 返 回 值: 需要保存返回TRUE,不需要保存返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewSOE::NeedSave()
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
 函 数 名: AddEventToList()
 功能概要: 将指定事件加入到列表中显示
 返 回 值: 成功返回TRUE，失败返回FALSE
 参    数: param1 指定事件
		   Param2 指定在列表中的位置
**************************************************************/
BOOL CViewSOE::AddEventToList( CXJEvent * pEvent, int i /*= 0*/ )
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;
	
	//0:序号
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:信息类型
	if(pEvent->m_nEventType == XJ_EVENT_ACTION)
	{
		//动作
		m_List.SetItemText(i, 1, StringFromID(IDS_EVENTTYPE_ACTION));
	}
	else if(pEvent->m_nEventType == XJ_EVENT_ALARM)
	{
		//告警
		m_List.SetItemText(i, 1, StringFromID(IDS_EVENTTYPE_ALARM));
	}
	else if(pEvent->m_nEventType == XJ_EVENT_DI)
	{
		//开关量
		m_List.SetItemText(i, 1, StringFromID(IDS_EVENTTYPE_DI));
	}
	else
	{
		//未知
		m_List.SetItemText(i, 1, StringFromID(IDS_COMMON_UNKNOWN));
	}
	
	//2:保护名称
	CString str;
	str = pEvent->m_pSec->m_sName;
	m_List.SetItemText(i, 2, str);
	//3:发生时间
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 3, sTime);
	//4:消息内容
	if(pEvent->m_nEventType == XJ_EVENT_DI)
	{
		CXJEventDI* pDI = (CXJEventDI*)pEvent;
		str.Format("%s %s", pDI->m_sDIName, DisplayValue(pDI->m_nValue));
		m_List.SetItemText(i, 4, str);
	}
	else
		m_List.SetItemText(i, 4, pEvent->m_sMsg);
	//5:厂站名
	m_List.SetItemText(i, 5, pEvent->m_sStationName);
	//6:一次设备名
	m_List.SetItemText(i, 6, pEvent->m_sDeviceName);
	//7:已确认
	m_List.SetItemText(i, 7, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//设置数据
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/*************************************************************
 函 数 名: RegulateControlSize()
 功能概要: 调整控件大小,位置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewSOE::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//客户区大小
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	
	//调整列表框大小,占整个高度
	m_List.MoveWindow(&rc, TRUE);
}

void CViewSOE::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

void CViewSOE::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//设置可滚动视图的必要属性
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//读取配置
	LoadConfig();
	//初始化列表控件
	InitListCtrl();
}

void CViewSOE::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strTitle = "";
	if(m_nType == 0)
		strTitle = StringFromID(IDS_EVENTCLASS_I);
	else if(m_nType == 1)
		strTitle = StringFromID(IDS_EVENTCLASS_II);
	else if(m_nType == 2)
		strTitle = StringFromID(IDS_EVENTCLASS_III);
	else if(m_nType == 3)
		strTitle = StringFromID(IDS_EVENTCLASS_DEBUG);
	pApp->SetAppTile(strTitle);
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 函 数 名: LoadActionFromDB()
 功能概要: 从数据库读取动作数据
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 要查找的个数
		   Param2 是否要查询已确认的事件
**************************************************************/
BOOL CViewSOE::LoadActionFromDB( int num, BOOL IsQueryAcked /*= TRUE*/ )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//因为返回信息需要先解析才知道是否事件是否返回信息，先读取num*5的事件
	num *= 5;
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//TOP num
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field1);
		
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

	//检查过滤条件,查几类事件
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	if(m_nType == 3)
	{
		//调试信息视图
		str.Format("ISDEBUG=%d", XJ_EVENTINFO_DEBUG);
	}
	else
		str.Format("property=%d", m_nType); //分类信息视图
	pApp->m_DBEngine.SetCondition(sql, condition3, str);

	//检查显示过滤条件
	if(!m_bShowDebug && m_nType != 3)
	{
		//非调试信息视图
		Condition condition4;
		CString strFilter;
		strFilter.Format("IsDebug=%d", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}

	//按动作启动时间大小排序
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by RECVTIME DESC,MS2 DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition2);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ACTION_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewSOE::LoadActionFromDB, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewSOE::LoadActionFromDB, 读取到%d条数据", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		//符合条件数
		int nChecked = 0;
		for(int i = 0; i < nCount; i++)
		{
			//判断已符合条件的个数
			if(nChecked == m_nMaxRow)
				break;
			//创建事件
			CXJEventAction * pEvent = new CXJEventAction;
			if(pEvent->LoadFromDB(pMemset))
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
						pMemset->MoveNext();
						continue;
					}
				}
				//加入到链表
				m_EventManager.AddEvent(pEvent);
				nChecked++;
			}
			else
			{
				//读取数据失败, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset->MoveNext();
		}
		if(nChecked < m_nMaxRow)
		{
			CString str;
			str.Format("CViewSOE::LoadActionFromDB, 要显示%d条数据,只找到%d条数据", m_nMaxRow, nChecked);
			WriteLog(str, XJ_LOG_LV3);
		}
	}
	else
	{
		CString str;
		str.Format("CViewSOE::LoadActionFromDB,查询失败,原因为%s", sError);
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
 函 数 名: LoadAlarmFromDB()
 功能概要: 从数据库读取告警数据
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 要查找的个数
		   Param2 是否要查询已确认的事件
**************************************************************/
BOOL CViewSOE::LoadAlarmFromDB( int num, BOOL IsQueryAcked /*= TRUE*/ )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//因为返回信息需要先解析才知道是否事件是否返回信息，先读取num*5的事件
	num *= 5;
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//TOP num
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field1);
		
	//条件
	//只查询未确认事件
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("is_ack = 0");
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

	//检查过滤条件,查几类事件
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	if(m_nType == 3)
	{
		//调试信息视图
		str.Format("ISDEBUG=%d", XJ_EVENTINFO_DEBUG);
	}
	else
		str.Format("property=%d", m_nType); //分类信息视图
	pApp->m_DBEngine.SetCondition(sql, condition3, str);

	//检查显示过滤条件
	if(!m_bShowDebug && m_nType != 3)
	{
		//非调试信息视图
		Condition condition4;
		CString strFilter;
		strFilter.Format("IsDebug=%d", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}
		
	//按动作启动时间大小排序
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by RECVTIME DESC,MS2 DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition2);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ALARM_DATA, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewSOE::LoadAlarmFromDB, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewSOE::LoadAlarmFromDB, 读取到%d条数据", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		//符合条件数
		int nChecked = 0;
		for(int i = 0; i < nCount; i++)
		{
			//判断已符合条件的个数
			if(nChecked == m_nMaxRow)
				break;
			//创建事件
			CXJEventAlarm * pEvent = new CXJEventAlarm;
			if(pEvent->LoadFromDB(pMemset))
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
						pMemset->MoveNext();
						continue;
					}
				}
				//加入到链表
				m_EventManager.AddEvent(pEvent);
				nChecked++;
			}
			else
			{
				//读取数据失败, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset->MoveNext();
		}
		if(nChecked < m_nMaxRow)
		{
			CString str;
			str.Format("CViewSOE::LoadAlarmFromDB, 要显示%d条数据,只找到%d条数据", m_nMaxRow, nChecked);
			WriteLog(str, XJ_LOG_LV3);
		}
	}
	else
	{
		CString str;
		str.Format("CViewSOE::LoadAlarmFromDB,查询失败,原因为%s", sError);
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
 函 数 名: LoadDataFromDB()
 功能概要: 从数据库读取数据
 返 回 值: 
 参    数: param1 要查找的个数
		   Param2 是否要查询已确认的事件
**************************************************************/
void CViewSOE::LoadDataFromDB( int num, BOOL IsQueryAcked /*= TRUE*/ )
{
	//清除旧数据
	m_EventManager.ClearAllEvents();
	int nRowNum = num/2;
	if(m_nType == 3)
	{
		//调试信息视图
		nRowNum = num/3;
	}
	//载入动作
	LoadActionFromDB(nRowNum, IsQueryAcked);
	//载入告警
	LoadAlarmFromDB(nRowNum, IsQueryAcked);
	if(m_nType == 3)
	{
		//调试信息视图,载入开关量
		LoadDIFromDB(nRowNum, IsQueryAcked);
	}
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置未确认事件文本颜色
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CViewSOE::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

void CViewSOE::OnDestroy() 
{
	//保存配置
	SaveConfig();
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CViewSOE::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			//	pMenu->EnableMenuItem(IDC_EVENT_SIGN, MF_ENABLED);
			}
			if(nCount == 1)
			{
				//只选择了一个
				pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_PROP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
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
			if(m_nType != 3)
			{
				//非调试信息视图
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
			}
			else
			{
				//调试信息视图
				pMenu->CheckMenuItem(IDC_EVENT_FILTER_DEBUG, MF_CHECKED);
			}

			//是否显示返回
			pMenu->EnableMenuItem(IDC_EVENT_FILTER_RETURN, MF_ENABLED);
			if(m_bShowReturn)
			{
				//显示返回
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

void CViewSOE::OnEventAck() 
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
			CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				if(pEvent->Ack())
				{
					//刷新界面显示, 通知其它窗口
 					RefreshAck(pEvent, nIndex);
 					TellOtherWndAck(pEvent);
				}
			}
		}
	}
}

void CViewSOE::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//无权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	BOOL bAckAction = AckAllEvent(EX_STTP_INFO_PT_ACTION_DATA, m_nType);
	BOOL bAckAlarm = AckAllEvent(EX_STTP_INFO_PT_ALARM_DATA, m_nType);
	for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
	{
		CXJEvent* pEvent = (CXJEvent*)m_EventManager.m_EventList.GetAt(i);
		if(pEvent == NULL)
			continue;
		if(pEvent->m_nEventType == XJ_EVENT_ALARM && bAckAlarm)
		{
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
			TellOtherWndAck(pEvent);
		}
		else if(pEvent->m_nEventType == XJ_EVENT_ACTION && bAckAction)
		{
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
			TellOtherWndAck(pEvent);
		}
	}
}

void CViewSOE::OnEventCopy() 
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
			CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
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

void CViewSOE::OnEventDevice() 
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
	CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pEvent->m_pSec->m_pOwner == NULL)
		return;
	pApp->ShowDevice(pEvent->m_pSec->m_pOwner);
}

void CViewSOE::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	//重新填充数据
	RefreshData();
}

void CViewSOE::OnEventProp() 
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
	CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//设置当前事件
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

void CViewSOE::OnEVENTRefresh() 
{
	// TODO: Add your command handler code here
	RefreshData();
}

void CViewSOE::OnEventSec() 
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
	CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pEvent->m_pSec);
}

void CViewSOE::OnEventSecProp() 
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
	CXJEvent* pEvent = (CXJEvent*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	//找开保护属性页
	if(pEvent->m_nEventType == XJ_EVENT_ACTION)
		pDoc->ShowSecPropPage(pEvent->m_pSec, PT_PROP_ACTION);
	else if(pEvent->m_nEventType == XJ_EVENT_ALARM)
		pDoc->ShowSecPropPage(pEvent->m_pSec, PT_PROP_ALARM);
	else
	{
		if(pEvent->m_pSec->m_nType == TYPE_WAVEREC)
			pDoc->ShowSecPropPage(pEvent->m_pSec, 1);
		else
			pDoc->ShowSecPropPage(pEvent->m_pSec);
	}
}

/*************************************************************
 函 数 名: RefreshAck()
 功能概要: 确认成功后刷新界面显示
 返 回 值: 
 参    数: param1 确认的事件
		   Param2 事件位置
**************************************************************/
void CViewSOE::RefreshAck( CXJEvent* pEvent, int nIndex /*= -1*/ )
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
	m_List.SetItemText(nIndex, 7, StringFromID(IDS_ACK_YES));
	//刷新
	m_List.Update(nIndex);
}

/*************************************************************
 函 数 名: CompareFunction()
 功能概要: 比较函数,排序时被调用
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数: param1 比较对象1
		   Param2 比较对象2
		   Param3 比较参数
**************************************************************/
int CALLBACK CViewSOE::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CXJEvent* pEvent1 = (CXJEvent*)lParam1;
	CXJEvent* pEvent2 = (CXJEvent*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号","信息类别","二次设备","发生时间", "消息内容","厂站名","一次设备","已确认"
	switch(nCol)
	{
	case 0: //事件ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //信息类别
		iResult = pEvent1->m_nEventType - pEvent2->m_nEventType;
		break;
	case 2: //保护名称
		str1 = pEvent1->m_pSec->m_sName;
		str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //发生时间
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //消息内容
		str1 = pEvent1->m_sMsg;
		str2 = pEvent2->m_sMsg;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //厂站名
		str1 = pEvent1->m_sStationName;
		str2 = pEvent2->m_sStationName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //一次设备名
		str1 = pEvent1->m_sDeviceName;
		str2 = pEvent2->m_sDeviceName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 7: //是否已确认
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iSOEViewAsc;
	return iResult;
}

void CViewSOE::OnColumnclickListSoe(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iSOEViewAsc = - g_iSOEViewAsc;
	}
	else
	{
		g_iSOEViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

void CViewSOE::OnClickListSoe(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected < 0)
	{
		//父窗口是分割窗口,父窗口的父窗口是SOEFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//向父窗口发送SELECTCHANGED消息, 选择了空行,发送空指针
		pWnd ->PostMessage(VIEWACTION_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	*pResult = 0;
}

void CViewSOE::OnItemchangedListSoe(NMHDR* pNMHDR, LRESULT* pResult) 
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
		CXJEvent * pEvent = (CXJEvent*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//父窗口是分割窗口,父窗口的父窗口是SOEFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//向父窗口发送SELECTCHANGED消息
		pWnd->PostMessage(VIEWSOE_SELECTCHANGED, 0, (LPARAM)pEvent);
	}

	*pResult = 0;
}

/*************************************************************
 函 数 名: TellOtherWndAck()
 功能概要: 告诉其它窗口这里确认了事件
 返 回 值: 
 参    数: param1 被确认的事件
		   Param2 是否被动确认
**************************************************************/
void CViewSOE::TellOtherWndAck( CXJEvent* pEvent, BOOL bIsPassive /*= FALSE*/ )
{
	//父窗口是分割窗口,父窗口的父窗口是SOEFrame.
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
		pWnd ->PostMessage(VIEW_SOE_LIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
}

void CViewSOE::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	CXJEventAction* pParam = (CXJEventAction*)lParam;
	if(pParam == NULL)
		return;
	
	//不显示调试信息的情况
	if(m_nType != 3)
	{
		//非调试信息视图
		if(!m_bShowDebug && pParam->m_IsDebug == XJ_EVENTINFO_DEBUG)
		{
			WriteLog("CViewSOE::OnSTTP20022, 调试事件不显示", XJ_LOG_LV3);
			return;
		}
	}

	//不显示返回信息
	if(!m_bShowReturn && pParam->m_nIsReturn == 1)
	{
		WriteLog("CViewSOE::OnSTTP20022, 返回事件不显示", XJ_LOG_LV3);
		return;
	}
	
	//复制
	CXJEventAction* pEvent = (CXJEventAction*)pParam->Clone();
	if(pEvent == NULL)
		return;
	
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

}

/*************************************************************
 函 数 名: CheckLineCount()
 功能概要: 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewSOE::CheckLineCount()
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

void CViewSOE::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	CXJEventAlarm* pParam = (CXJEventAlarm*)lParam;
	if(pParam == NULL)
		return;

	//不显示调试信息的情况
	if(m_nType != 3)
	{
		//非调试信息视图
		if(!m_bShowDebug && pParam->m_IsDebug == XJ_EVENTINFO_DEBUG)
		{
			WriteLog("CViewSOE::OnSTTP20018, 调试事件不显示", XJ_LOG_LV3);
			return;
		}
	}

	//不显示返回信息
	if(!m_bShowReturn && pParam->m_nIsReturn == 1)
	{
		WriteLog("CViewSOE::OnSTTP20018, 返回事件不显示", XJ_LOG_LV3);
		return;
	}

	//复制
	CXJEventAlarm* pEvent = (CXJEventAlarm*)pParam->Clone();
	if(pEvent == NULL)
		return;
	
	//加入到链表
	m_EventManager.AddEvent(pEvent);
	//加入到列表,加入到第一行
	AddEventToList(pEvent, 0);
	CheckLineCount();
	
	//弹出事件属性框
	if(g_ShowEventDlg == 1)
	{
		CXJBrowserApp* pApp  = (CXJBrowserApp*)AfxGetApp();
		m_EventManager.m_pCurrentEvent = pEvent;
		pApp->ShowEventProp(&m_EventManager);
	}	
}

void CViewSOE::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//重新读入数据
	RefreshData();
}

/*************************************************************
 函 数 名: LoadDIFromDB()
 功能概要: 从数据库读取开关量数据
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数: param1 要查找的个数
		   Param2 是否要查询已确认的事件
**************************************************************/
BOOL CViewSOE::LoadDIFromDB( int num, BOOL IsQueryAcked /*= TRUE*/ )
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//与数据平台访问DLL的连接可用
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//TOP num
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field1);
		
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

	//检查过滤条件,查几类事件
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	//调试信息视图
	str.Format("ISDEBUG=%d", XJ_EVENTINFO_DEBUG);
	pApp->m_DBEngine.SetCondition(sql, condition3, str);

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("(pt_id in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}
		
	//按ID大小排序
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by TIME DESC,MS DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition2);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_CHANGE_DI_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewSOE::LoadDIFromDB, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CViewSOE::LoadDIFromDB, 读取到%d条数据", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventDI * pEvent = new CXJEventDI;
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
		str.Format("CViewSOE::LoadDIFromDB,查询失败,原因为%s", sError);
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
 函 数 名: DisplayValue()
 功能概要: 得到开关量的值显示字符串
 返 回 值: 显示值
 参    数: param1 开关量值
		   Param2 
**************************************************************/
CString CViewSOE::DisplayValue( int nValue )
{
	//开关量值显示风格.0: 1/0, 1: ON/OFF, 2: 投/退, 3: 合/开, 4:动作/返回.
	if(0 == g_PTDIStyle)
	{
		if(1 == nValue)
			return "1";
		else if(0 == nValue)
			return "0";
	}
	if(1 == g_PTDIStyle)
	{
		if(1 == nValue)
			return "ON";
		else if(0 == nValue)
			return "OFF";
	}
	if(2 == g_PTDIStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_COMMISSIONING);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_CEASING);
	}
	if(3 == g_PTDIStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_CLOSING);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_OPENING);
	}
	if(4 == g_PTDIStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_ACTION);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_RETURN);
	}
	CString str;
	str.Format("%d", nValue);
	return str;
}

void CViewSOE::OnSTTP20010( WPARAM wParam, LPARAM lParam )
{	
	if(m_nType != 3)
		return;

	CXJEventDI* pParam = (CXJEventDI*)lParam;
	if(pParam == NULL)
		return;
	//复制
	CXJEventDI* pEvent = (CXJEventDI*)pParam->Clone();
	if(pEvent == NULL)
		return;
	
	//加入到链表
	m_EventManager.AddEvent(pEvent);
	//加入到列表,加入到第一行
	AddEventToList(pEvent, 0);
	CheckLineCount();
}

/*************************************************************
 函 数 名: LoadConfig()
 功能概要: 读取配置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewSOE::LoadConfig()
{
	CString strIniFile = g_SysConfig;
	CString strNodeName = "SOEIViewConfig";
	if(m_nType == 1)
		strNodeName = "SOEIIViewConfig";
	else if(m_nType == 2)
		strNodeName = "SOEIIIViewConfig";
	else if(m_nType == 3)
		strNodeName = "SOEDebugViewConfig";
	
	//最大显示条数
	m_nMaxRow = GetPrivateProfileInt(strNodeName, "MaxRow", 100, strIniFile);
	//是否显示已确认事件
	m_bShowAcked = GetPrivateProfileInt(strNodeName, "ShowAcked", 1, strIniFile);
	//是否显示调试事件
	m_bShowDebug = GetPrivateProfileInt(strNodeName, "ShowDebug", 1, strIniFile);
	//是否显示返回事件
	m_bShowReturn = GetPrivateProfileInt(strNodeName, "ShowReturn", 1, strIniFile);

	if(m_nType == 3)
	{
		//调试信息视图
		m_bShowDebug = TRUE;
	}
}

/*************************************************************
 函 数 名: SaveConfig()
 功能概要: 保存配置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewSOE::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString strNodeName = "SOEIViewConfig";
	if(m_nType == 1)
		strNodeName = "SOEIIViewConfig";
	else if(m_nType == 2)
		strNodeName = "SOEIIIViewConfig";
	else if(m_nType == 3)
		strNodeName = "SOEDebugViewConfig";
	CString str;
	
	//最大显示条数
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString(strNodeName, "MaxRow", str, strIniFile);
	//是否显示已确认事件
	str.Format("%d", m_bShowAcked);
	WritePrivateProfileString(strNodeName, "ShowAcked", str, strIniFile);
	//是否显示调试事件
	str.Format("%d", m_bShowDebug);
	WritePrivateProfileString(strNodeName, "ShowDebug", str, strIniFile);
	//是否显示返回事件
	str.Format("%d", m_bShowReturn);
	WritePrivateProfileString(strNodeName, "ShowReturn", str, strIniFile);
}

void CViewSOE::OnEventFilterDebug() 
{
	// TODO: Add your command handler code here
	m_bShowDebug = !m_bShowDebug;
	//重新填充数据
	RefreshData();
}

void CViewSOE::OnDblclkListSoe(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected > -1)
	{
		//点击到事件, 确认此事件
		//得到该行对应的事件
		CXJEvent * pEvent = (CXJEvent*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//设置当前事件
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
	*pResult = 0;
}

void CViewSOE::OnEventFilterReturn() 
{
	// TODO: Add your command handler code here
	m_bShowReturn = !m_bShowReturn;
	//重新填充数据
	RefreshData();
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   RefreshData	
返回值:   int	
功能概要: 
*****************************************************/
int CViewSOE::RefreshData()
{
	//重新读入数据
	//父窗口是分割窗口,父窗口的父窗口是SOEFrame
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//向父窗口发送SELECTCHANGED消息, 选择了空行,发送空指针
		pWnd ->PostMessage(VIEWSOE_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	else
		return 0;
	LoadDataFromDB(m_nMaxRow, m_bShowAcked);
	FillListData();
	WriteLog("SOE视图刷新完毕", XJ_LOG_LV3);
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
void CViewSOE::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	if(lParam == STATIONINIT_CREATE)
		return;
	BOOL bNeedReload = FALSE;
	CStationObj* pStation = (CStationObj*)wParam;
	CXJEvent * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = pList->GetAt(i);
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
		WriteLog("StationInit,SOE事件页面刷新完毕", XJ_LOG_LV3);
	}
	else
		WriteLog("StationInit,SOE事件页面刷新完毕", XJ_LOG_LV3);
}
