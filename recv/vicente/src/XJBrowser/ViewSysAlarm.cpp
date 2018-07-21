// ViewSysAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewSysAlarm.h"
#include "MainFrm.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/** @brief           升降序作用位*/
int g_iSysAlarmViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarm

IMPLEMENT_DYNCREATE(CViewSysAlarm, CFormView)

//##ModelId=49B87B810128
CViewSysAlarm::CViewSysAlarm()
	: CFormView(CViewSysAlarm::IDD)
{
	//{{AFX_DATA_INIT(CViewSysAlarm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_SYSALARM;
	m_EventManager.ClearAllEvents();
	//最大显示条数
	m_nMaxRow = 100;
	CString strIniFile = g_SysConfig;
	m_nMaxRow = GetPrivateProfileInt("SysAlarmViewConfig", "MaxRow", 100, strIniFile);
	m_bShowAcked = TRUE;
	m_nOldSortCol = -1;
	g_iSysAlarmViewAsc = -1;
}

//##ModelId=49B87B8101E4
CViewSysAlarm::~CViewSysAlarm()
{
	m_EventManager.ClearAllEvents();
	TRACE("CViewSysAlarm::~CViewSysAlarm \n");
}

//##ModelId=49B87B8101C5
void CViewSysAlarm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewSysAlarm)
	DDX_Control(pDX, IDC_LIST_SYSALARM, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewSysAlarm, CFormView)
	//{{AFX_MSG_MAP(CViewSysAlarm)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_DEVICE, OnEventDevice)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SYSALARM, OnClickListSysalarm)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SYSALARM, OnColumnclickListSysalarm)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SYSALARM, OnCustomDraw)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SYSALARM, OnDblclkListSysalarm)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SYSALARM, OnItemchangedListSysalarm)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_STTP_20155, OnSTTP20155)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20138, OnSTTP20138)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(WM_STTP_20176, OnSTTP20176)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarm diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8101F4
void CViewSysAlarm::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B810203
void CViewSysAlarm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewSysAlarm message handlers

//##ModelId=49B87B8101D4
void CViewSysAlarm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_SYSALARM));
	//GetParentFrame()->SetWindowText("系统告警");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B8101B7
void CViewSysAlarm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(100, 100);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	InitListCtrl();
}

//##ModelId=49B87B810213
void CViewSysAlarm::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B81014F
int CViewSysAlarm::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	RefreshData();

	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B810157
int CViewSysAlarm::InitListStyle()
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
		//char* arColumn[6]={"序号", "二次设备", "发生时间", "消息内容", "厂站", "已确认"};
		LV_COLUMN lvCol;
		lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvCol.fmt=LVCFMT_CENTER;
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 6; nCol++)
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
				lvCol.cx = 300; // 二次设备
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 2://
				lvCol.cx = 180; // 发生时间
				colName = StringFromID(IDS_COMMON_OCCUR_TIME);
				break;
			case 3://
				lvCol.cx = 240; // 消息内容
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 4:
				lvCol.cx = 100; //厂站
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 5: 
				lvCol.cx = 100; //已确认
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=50;//列宽100象素
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}

		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(5, TRUE);
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
//##ModelId=49B87B810177
BOOL CViewSysAlarm::LoadListConfig()
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
			WriteLog("ViewSysAlarm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("ViewSysAlarm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewSysAlarmConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewSysAlarmConfig", XJ_LOG_LV3);
				WriteLog("ViewSysAlarm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewSysAlarmConfig下的配置", XJ_LOG_LV3);
					WriteLog("ViewSysAlarm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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

//##ModelId=49B87B810178
BOOL CViewSysAlarm::SaveListConfig()
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
			WriteLog("ViewSysAlarm::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewSysAlarm::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewSysAlarmConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewSysAlarmConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewSysAlarmConfig");
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
						WriteLog("未找到ViewSysAlarmConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
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
						WriteLog("找到了ViewSysAlarmConfig下的配置, 先删除新建所有子节点", XJ_LOG_LV3);
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
//##ModelId=49B87B810167
int CViewSysAlarm::FillListData()
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
	CXJEventSysAlarm * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventSysAlarm*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B810186
BOOL CViewSysAlarm::NeedSave()
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
//##ModelId=49B87B810187
void CViewSysAlarm::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//客户区大小
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	
	//调整列表框大小,占整个高度的(1 - 0.618)
	float fHList = (float)rcList.Height();
	fHList = float(fHList);
	rcList.bottom = (long)fHList;
	m_List.MoveWindow(&rcList, TRUE);
}

//##ModelId=49B87B810222
void CViewSysAlarm::OnDestroy() 
{
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
//##ModelId=49B87B810196
BOOL CViewSysAlarm::LoadDataFromDB(int num, BOOL IsQueryAcked)
{
	BOOL bReturn = TRUE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	m_EventManager.ClearAllEvents();
	m_List.DeleteAllItems();
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

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("( (host_name in(select pt_id from tb_secdev_base where station_id in(select station_id from tb_user_related where user_id='%s'))) or (station_name in(select station_id from tb_user_related where user_id='%s')) )",pApp->m_User.m_strUSER_ID,pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
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
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_ALARM_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewSysAlarm::LoadDataFromDB, 查询失败", XJ_LOG_LV3);
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
			str.Format("CViewSysAlarm::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventSysAlarm * pEvent = new CXJEventSysAlarm;
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
		str.Format("CViewSysAlarm::LoadDataFromDB,查询失败,原因为%s", sError);
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
//##ModelId=49B87B810199
BOOL CViewSysAlarm::AddEventToList(CXJEventSysAlarm * pEvent, int i)
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;

	//"序号", "二次设备", 发生时间","消息内容", "厂站","已确认"
	//0:序号
	CString str;
	str.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;

	//1: 二次设备
	str = "";
	if(pEvent->m_pSec != NULL)
	{
		str =  pEvent->m_pSec->m_sName;
	}
	m_List.SetItemText(i, 1, str);

	//2:发生时间
	CString sTime;
	sTime.Format("%s", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	m_List.SetItemText(i, 2, sTime);
	//3:消息内容 
//	if (pEvent->m_nType == 99)
//	{
	m_List.SetItemText(i, 3, pEvent->m_sMsg);
//	}
//	else
//	{
//		m_List.SetItemText(i, 3, pEvent->m_sDetailMsg);
//	}

	//4:厂站
// 	if (pEvent->m_nType == 5)
// 	{
// 		m_List.SetItemText(i, 4, pEvent->m_sStationId);
// 	}
// 	else
// 	{
		m_List.SetItemText(i, 4, pEvent->m_sStationName);
	//}
	//5:已确认
	m_List.SetItemText(i, 5, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//设置数据
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}


//##ModelId=49B87B810224
void CViewSysAlarm::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			}
			if(nCount == 1)
			{
				//只选择了一个
			//	pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_PROP, MF_ENABLED);
			//	pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
			//	pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
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
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87B810234
void CViewSysAlarm::OnEventAck() 
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
			CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
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

//##ModelId=49B87B810236
void CViewSysAlarm::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//无权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckAllEvent(EX_STTP_INFO_SYS_ALARM_CFG))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

//##ModelId=49B87B810243
void CViewSysAlarm::OnEventCopy() 
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
			CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
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

//##ModelId=49B87B810245
void CViewSysAlarm::OnEventDevice() 
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
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pEvent->m_pSec->m_pOwner == NULL)
		return;
	pApp->ShowDevice(pEvent->m_pSec->m_pOwner);
}

//##ModelId=49B87B810247
void CViewSysAlarm::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	//重新填充数据
	RefreshData();
}

//##ModelId=49B87B810251
void CViewSysAlarm::OnEventProp() 
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
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//设置当前事件
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

//##ModelId=49B87B810253
void CViewSysAlarm::OnEventSec() 
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
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pEvent->m_pSec);
}

//##ModelId=49B87B810255
void CViewSysAlarm::OnClickListSysalarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
 	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
 	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
 	if(nSelected < 0)
 	{
 		//取消了所有选择
		//父窗口是分割窗口,父窗口的父窗口是SysAlarmFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//向父窗口发送SELECTCHANGED消息
		pWnd ->PostMessage(VIEWSYSALARM_SELECTCHANGED, 0, (LPARAM)NULL);
 	}
	*pResult = 0;
}

//##ModelId=49B87B810261
void CViewSysAlarm::OnEventSecProp() 
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
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	if(pEvent->m_pSec->m_nType == TYPE_WAVEREC)
		pDoc->ShowSecPropPage(pEvent->m_pSec,1);
	else
		pDoc->ShowSecPropPage(pEvent->m_pSec);
}

/*************************************************************
 函 数 名：RefreshAck()
 功能概要：事件确认后刷新界面显示
 返 回 值: void
 参    数：param1	确认的事件
		   Param2	事件位置
**************************************************************/
//##ModelId=49B87B8101A6
void CViewSysAlarm::RefreshAck( CXJEventSysAlarm* pEvent, int nIndex )
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
	m_List.SetItemText(nIndex, 5, StringFromID(IDS_ACK_YES));
	//刷新
// 	CRect rc;
// 	m_List.GetItemRect(nIndex, &rc, LVIR_BOUNDS);
// 	m_List.InvalidateRect(rc);
	m_List.Update(nIndex);
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,排序时被调用
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数：param1	比较对象1
		   Param2	比较对象2
		   param3	比较参数
**************************************************************/
//##ModelId=49B87B81014A
int CALLBACK CViewSysAlarm::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CXJEventSysAlarm* pEvent1 = (CXJEventSysAlarm*)lParam1;
	CXJEventSysAlarm* pEvent2 = (CXJEventSysAlarm*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号", "二次设备", 发生时间","消息内容", "厂站","已确认"
	switch(nCol)
	{
	case 0: //事件ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //二次设备
		str1 = "";
		str2 = "";
		if(pEvent1->m_pSec != NULL)
			str1 = pEvent1->m_pSec->m_sName;
		if(pEvent2->m_pSec != NULL)
			str2 = pEvent2->m_pSec->m_sName;
	case 2: //发生时间
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //消息内容
		str1 = pEvent1->m_sMsg;
		str2 = pEvent2->m_sMsg;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
	case 4: //厂站名
		str1 = pEvent1->m_sStationName;
		str2 = pEvent2->m_sStationName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //是否已确认
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iSysAlarmViewAsc;
	return iResult;
}

//##ModelId=49B87B810263
void CViewSysAlarm::OnColumnclickListSysalarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iSysAlarmViewAsc = - g_iSysAlarmViewAsc;
	}
	else
	{
		g_iSysAlarmViewAsc = 1;
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
//##ModelId=49B87B81027B
void CViewSysAlarm::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_SYSALARM)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)lParam;
	if(pEvent == NULL)
		return;
	//刷新界面显示
	RefreshAck(pEvent);
}

/*************************************************************
 函 数 名：TellOtherWndAck()
 功能概要：当事件确认成功后, 通知其它窗口更新显示
 返 回 值: void
 参    数：param1	被确认的事件
**************************************************************/
//##ModelId=49B87B8101A9
void CViewSysAlarm::TellOtherWndAck( CXJEventSysAlarm* pEvent )
{
	//发送消息通知事件属性对话框
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
	if(pFrame != NULL)
	{
		//事件栏主动发生确认事件动作时, 给主框架发确认事件消息
		//此消息由主框架转发给事件属性对话框和保护属性页面
		pFrame->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
	}
	//父窗口是分割窗口,父窗口的父窗口是ActionFrame.
	CWnd* pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		pWnd->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
	}
}

/*************************************************************
 函 数 名：CheckLineCount()
 功能概要：检查列表中的元素数是否多出了指定的个数,如果是,删除后面的
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8101B6
void CViewSysAlarm::CheckLineCount()
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

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：未确认事件特殊文字颜色
 返 回 值: void
**************************************************************/
//##ModelId=49B87B810267
void CViewSysAlarm::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				//按状态给定背景色
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

//##ModelId=49B87B81026B
void CViewSysAlarm::OnDblclkListSysalarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected > -1)
	{
		//点击到事件, 确认此事件
		//得到该行对应的事件
		CXJEventSysAlarm * pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//设置当前事件
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnSTTP20155()
 功能概要：收到20115,周期召唤信息变化告警通知, 生成新事件
 返 回 值: void
 参    数：param1
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B810282
void CViewSysAlarm::OnSTTP20155( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewSysAlarm::OnSTTP20155 start", XJ_LOG_LV3);
	CXJEventSysAlarm * pParam = (CXJEventSysAlarm *)lParam;
	if(!pParam)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)pParam->Clone();
	if(!pEvent)
		return;
	//加入到链表
	m_EventManager.AddEvent(pEvent);
	//加入到列表,加入到第一行
	AddEventToList(pEvent, 0);
	CheckLineCount();
	WriteLog("CViewSysAlarm::OnSTTP20155 neat end", XJ_LOG_LV3);
}

//##ModelId=49B87B810274
void CViewSysAlarm::OnItemchangedListSysalarm(NMHDR* pNMHDR, LRESULT* pResult) 
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
		CXJEventSysAlarm * pEvent = (CXJEventSysAlarm*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//父窗口是分割窗口,父窗口的父窗口是SysAlarmFrame
		CWnd * pWnd = GetParent()->GetParent();
		if(pWnd == NULL)
			return;
		//向父窗口发送SELECTCHANGED消息
		pWnd ->PostMessage(VIEWSYSALARM_SELECTCHANGED, 0, (LPARAM)pEvent);
	}
	*pResult = 0;
}

//##ModelId=49B87B810278
BOOL CViewSysAlarm::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_SYSALARM);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

void CViewSysAlarm::OnEVENTRefresh() 
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
void CViewSysAlarm::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
	WriteLog("CViewSysAlarm::OnViewRefresh, 强制刷新完毕", XJ_LOG_LV3);
}

/****************************************************
Date:2012/5/7  Author:LYH
函数名:   OnSTTP20138	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewSysAlarm::OnSTTP20138( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewSysAlarm::OnSTTP20138 start", XJ_LOG_LV3);
	
	CXJEventSysAlarm * pParam = (CXJEventSysAlarm *)lParam;
	if(!pParam)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)pParam->Clone();
	if(!pEvent)
		return;
	//加入到链表
	m_EventManager.AddEvent(pEvent);
	//加入到列表,加入到第一行
	AddEventToList(pEvent, 0);
	CheckLineCount();
	WriteLog("CViewSysAlarm::OnSTTP20138 neat end", XJ_LOG_LV3);
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   RefreshData	
返回值:   int	
功能概要: 
*****************************************************/
int CViewSysAlarm::RefreshData()
{
	//父窗口是分割窗口,父窗口的父窗口是SysAlarmFrame
	CWnd * pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//向父窗口发送SELECTCHANGED消息
		pWnd ->PostMessage(VIEWSYSALARM_SELECTCHANGED, 0, (LPARAM)NULL);
	}
	else
		return 0;
	if(LoadDataFromDB(m_nMaxRow, m_bShowAcked))
	{
		//读取数据成功,填充列表
		FillListData();
	}
	else
		return 0;
	WriteLog("系统告警页面刷新完毕", XJ_LOG_LV3);
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
void CViewSysAlarm::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	BOOL bNeedReload = FALSE;
	CXJEventSysAlarm * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
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
		WriteLog("StationInit,系统告警事件页面刷新完毕", XJ_LOG_LV3);
	}
	else
	{
		WriteLog("StationInit,系统告警事件页面刷新完毕", XJ_LOG_LV3);
	}
}

/****************************************************
Date:2013/6/7  Author:LYH
函数名:   OnSTTP20176	
返回值:   void	
功能概要: 解析系统告警(20176)报文
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewSysAlarm::OnSTTP20176( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewSysAlarm::OnSTTP20176 start", XJ_LOG_LV3);
	CXJEventSysAlarm * pParam = (CXJEventSysAlarm *)lParam;
	if(!pParam)
		return;
	CXJEventSysAlarm* pEvent = (CXJEventSysAlarm*)pParam->Clone();
	if(!pEvent)
		return;
	//加入到链表
	m_EventManager.AddEvent(pEvent);
	//加入到列表,加入到第一行
	AddEventToList(pEvent, 0);
	CheckLineCount();
	WriteLog("CViewSysAlarm::OnSTTP20176 neat end", XJ_LOG_LV3);
}
