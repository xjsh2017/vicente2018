// ViewDI.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewDI.h"
#include "MainFrm.h"
#include "GlobalFunc.h"
#include "DlgEventSignal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           升降序作用位*/
int g_iDIViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewDI

IMPLEMENT_DYNCREATE(CViewDI, CFormView)

//##ModelId=49B87B850139
CViewDI::CViewDI()
	: CFormView(CViewDI::IDD)
{
	//{{AFX_DATA_INIT(CViewDI)
	m_bCBoxShowDebug = TRUE;
	m_bCBoxShowReady = TRUE;
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_DI;
	m_EventManager.ClearAllEvents();
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_nOldSortCol = -1;
	g_iDIViewAsc = -1;
	m_bShowDebug = TRUE;
	m_sChooseStationID = "";
}

//##ModelId=49B87B850213
CViewDI::~CViewDI()
{
	m_EventManager.ClearAllEvents();
	TRACE("CViewDI::~CViewDI \n");
}

//##ModelId=49B87B8501E4
void CViewDI::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDI)
	DDX_Control(pDX, IDC_STATIC_CHOOSESTATION, m_staticChooseStation);
	DDX_Control(pDX, IDC_COMBO_CHOOSESTATIOIN, m_cmbChooseStation);
	DDX_Control(pDX, IDC_CHECK_SHOWREADY, m_cmbShowReady);
	DDX_Control(pDX, IDC_CHECK_SHOWDEBUG, m_cmbShowDebug);
	DDX_Control(pDX, IDC_LIST_DI, m_List);
	DDX_Check(pDX, IDC_CHECK_SHOWDEBUG, m_bCBoxShowDebug);
	DDX_Check(pDX, IDC_CHECK_SHOWREADY, m_bCBoxShowReady);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDI, CFormView)
	//{{AFX_MSG_MAP(CViewDI)
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
	ON_NOTIFY(NM_CLICK, IDC_LIST_DI, OnClickListDi)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DI, OnColumnclickListDi)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DI, OnCustomDraw)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DI, OnDblclkListDi)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	ON_COMMAND(IDC_EVENT_FILTER_DEBUG, OnEventFilterDebug)
	ON_COMMAND(IDC_EVENT_SIGN, OnEventSign)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSESTATIOIN, OnSelchangeComboChoosestatioin)
	ON_BN_CLICKED(IDC_CHECK_SHOWREADY, OnCheckShowready)
	ON_BN_CLICKED(IDC_CHECK_SHOWDEBUG, OnCheckShowdebug)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20010, OnSTTP20010)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDI diagnostics

#ifdef _DEBUG
//##ModelId=49B87B850222
void CViewDI::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B850224
void CViewDI::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewDI message handlers

//##ModelId=49B87B8501F4
void CViewDI::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_DIGITAL));
	//GetParentFrame()->SetWindowText("开关量");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B8501D8
void CViewDI::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//读取配置
	LoadConfig();

	//初始化控件信息
	InitComBox();
	InitListStyle();
	//初始化列表
	InitListCtrl();
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B850169
int CViewDI::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	
	return RefreshData();
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B850177
int CViewDI::InitListStyle()
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
		//char* arColumn[8]={"序号","二次设备","时间", "名称","值", "厂站", "一次设备", "已确认"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 8; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName="";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; // 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 300; // 保护名称
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 2://
				lvCol.cx = 180; // 时间 
				colName = StringFromID(IDS_COMMON_TIME);
				break;
			case 3://
				lvCol.cx = 180; // 名称
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 4:
				lvCol.cx = 60; // 值
				colName = StringFromID(IDS_COMMON_VALUE);
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
				lvCol.cx=80;//列宽100象素
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		//默认隐藏"确认"
		m_List.SetColumnHide(7, TRUE);
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
//##ModelId=49B87B850186
BOOL CViewDI::LoadListConfig()
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
			WriteLog("ViewDI::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("ViewDI::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewDIConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewDIConfig");
				WriteLog("ViewDI::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewDIConfig下的配置");
					WriteLog("ViewDI::读取UIConfig失败,将使用默认列风格");
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

//##ModelId=49B87B850196
BOOL CViewDI::SaveListConfig()
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
			WriteLog("ViewDI::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewDI::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewDIConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewDIConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewDIConfig");
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
						WriteLog("未找到ViewDIConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了ViewDIConfig下的配置, 先删除新建所有子节点");
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
//##ModelId=49B87B850178
int CViewDI::FillListData()
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
	CXJEventDI * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventDI*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	//设置checkbox的状态
	m_cmbShowReady.SetCheck(m_bCBoxShowReady);
	m_cmbShowDebug.SetCheck(m_bCBoxShowDebug);
	//恢复刷新
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B850197
BOOL CViewDI::NeedSave()
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
//##ModelId=49B87B8501A5
void CViewDI::RegulateControlSize()
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
		m_cmbShowDebug.ShowWindow(FALSE);
		m_cmbShowReady.ShowWindow(FALSE);
		rcList=rc;
	}
	else
	{
		rcList=CRect(rc.left,rc.top+45,rc.right,rc.bottom);
	}

	m_List.MoveWindow(&rcList, TRUE);
}

//##ModelId=49B87B850233
void CViewDI::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

//##ModelId=49B87B850244
void CViewDI::OnDestroy() 
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
//##ModelId=49B87B8501A6
BOOL CViewDI::LoadDataFromDB(int num, BOOL IsQueryAcked)
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
	bzero(&Field1, sizeof(Field));
	str = _T("ID");
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = _T("Msg");
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field2);

	Field Field3;
	bzero(&Field3, sizeof(Field));
	str = _T("PT_ID");
	strncpy(Field3.FieldName, str, str.GetLength());
	Field3.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field3);

	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = _T("CPU");
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field4);

	Field Field5;
	bzero(&Field5, sizeof(Field));
	str = _T("Di_ID");
	strncpy(Field5.FieldName, str, str.GetLength());
	Field5.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field5);

	Field Field6;
	bzero(&Field6, sizeof(Field));
	str = _T("Is_Ack");
	strncpy(Field6.FieldName, str, str.GetLength());
	Field6.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field6);

	Field Field7;
	bzero(&Field7, sizeof(Field));
	str = _T("Value");
	strncpy(Field7.FieldName, str, str.GetLength());
	Field7.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field7);

	Field Field8;
	bzero(&Field8, sizeof(Field));
	str = _T("Time");
	strncpy(Field8.FieldName, str, str.GetLength());
	Field8.FieldType = EX_STTP_DATA_TYPE_TIME;
	sql.Fieldlist.push_back(Field8);

	Field Field9;
	bzero(&Field9, sizeof(Field));
	str = _T("MS");
	strncpy(Field9.FieldName, str, str.GetLength());
	Field9.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field9);

	Field Field10;
	bzero(&Field10, sizeof(Field));
	str = _T("IsDebug");
	strncpy(Field10.FieldName, str, str.GetLength());
	Field10.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field10);
		
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
		strFilter.Format("IsDebug=%d", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}

	//选择显示厂站的信息
	if (!m_sChooseStationID.IsEmpty())
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
		WriteLog("CViewDI::LoadDataFromDB, 查询失败");
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
			str.Format("CViewDI::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLog(str);
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
		str.Format("CViewDI::LoadDataFromDB,查询失败,原因为%s", sError);
		WriteLog(str);
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
//##ModelId=49B87B8501B7
BOOL CViewDI::AddEventToList(CXJEventDI * pEvent, int i)
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;

	//0:序号
	CString str;
	str.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;

	//1:保护名称
	str = pEvent->m_pSec==NULL?"":pEvent->m_pSec->m_sName;
	m_List.SetItemText(i, 1, str);
	//2:时间
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 2, sTime);
	//3:开关量定义
	m_List.SetItemText(i, 3, pEvent->m_sDIName);
	//4:值
	m_List.SetItemText(i, 4, DisplayValue(pEvent->m_nValue));
	//5:厂站
	m_List.SetItemText(i, 5, pEvent->m_sStationName);
	//6:一次设备
	m_List.SetItemText(i, 6, pEvent->m_sDeviceName);
	//7:已确认
	m_List.SetItemText(i, 7, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//设置数据
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/*************************************************************
 函 数 名：OnSTTP20010()
 功能概要：响应20010(保护开关量采样值上载通知)报文
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B850296
void CViewDI::OnSTTP20010( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewDI::OnSTTP20010 start");

	CXJEventDI* pParam = (CXJEventDI*)lParam;
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
		WriteLog("CViewDI::OnSTTP20010, 调试事件不显示");
		return;
	}

	//复制
	CXJEventDI* pEvent = (CXJEventDI*)pParam->Clone();
	if(pEvent == NULL)
		return;

	//加入到链表
	m_EventManager.AddEvent(pEvent);
	//加入到列表,加入到第一行
	AddEventToList(pEvent, 0);
	CheckLineCount();

	WriteLog("CViewDI::OnSTTP20010 neat end");
}

//##ModelId=49B87B850246
void CViewDI::OnContextMenu(CWnd* pWnd, CPoint point) 
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
				pMenu->EnableMenuItem(IDC_EVENT_SIGN, MF_ENABLED);
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
			
			
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87B850254
void CViewDI::OnEventAck() 
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
			CXJEventDI* pEvent = (CXJEventDI*)m_List.GetItemData(nIndex);
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

//##ModelId=49B87B850256
void CViewDI::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//无权限
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckAllEvent(EX_STTP_INFO_CHANGE_DI_CFG))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventDI* pEvent = (CXJEventDI*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

//##ModelId=49B87B850261
void CViewDI::OnEventCopy() 
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
			CXJEventDI* pEvent = (CXJEventDI*)m_List.GetItemData(nIndex);
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

//##ModelId=49B87B850263
void CViewDI::OnEventDevice() 
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
	CXJEventDI* pEvent = (CXJEventDI*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pEvent->m_pSec->m_pOwner == NULL)
		return;
	pApp->ShowDevice(pEvent->m_pSec->m_pOwner);
}

//##ModelId=49B87B850265
void CViewDI::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	m_bCBoxShowReady = m_bShowAcked;
	//重新填充数据
	RefreshData();
}

//##ModelId=49B87B850267
void CViewDI::OnEventProp() 
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
	CXJEventDI* pEvent = (CXJEventDI*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//设置当前事件
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

//##ModelId=49B87B850271
void CViewDI::OnEventSec() 
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
	CXJEventDI* pEvent = (CXJEventDI*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pEvent->m_pSec);
}

//##ModelId=49B87B850273
void CViewDI::OnClickListDi(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
// 	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
// 	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
// 	if(nSelected > -1)
// 	{
// 		//点击到事件, 确认此事件
// 		//得到该行对应的事件
// 		CXJEventDI * pEvent = (CXJEventDI*)m_List.GetItemData(nSelected);
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

//##ModelId=49B87B850277
void CViewDI::OnEventSecProp() 
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
	CXJEventDI* pEvent = (CXJEventDI*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	nIndex = PT_PROP_DIGITAL;
	if(pEvent->m_pSec != NULL)
	{
		if(pEvent->m_pSec->m_nType == TYPE_WAVEREC)
			nIndex = WR_PROP_DIGITAL;
	}
	pDoc->ShowSecPropPage(pEvent->m_pSec,PT_PROP_DIGITAL);
}

/*************************************************************
 函 数 名：RefreshAck()
 功能概要：事件确认后刷新界面显示
 返 回 值: 
 参    数：param1	确认的事件
		   Param2	事件在列表中的位置
**************************************************************/
//##ModelId=49B87B8501C6
void CViewDI::RefreshAck( CXJEventDI* pEvent, int nIndex )
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
// 	CRect rc;
// 	m_List.GetItemRect(nIndex, &rc, LVIR_BOUNDS);
// 	m_List.InvalidateRect(rc);
	m_List.Update(nIndex);
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果.1, 0, -1
 参    数：param1 比较对象1
		   Param2 比较对象2
		   param3 比较参数
**************************************************************/
//##ModelId=49B87B85015A
int CALLBACK CViewDI::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CXJEventDI* pEvent1 = (CXJEventDI*)lParam1;
	CXJEventDI* pEvent2 = (CXJEventDI*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号","保护名称","时间", "名称","值","消息内容","已确认"
	switch(nCol)
	{
	case 0: //事件ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //保护名称
		str1 = pEvent1->m_pSec->m_sName;
		str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //发生时间
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3:	//开关量定义
		str1 = pEvent1->m_sDIName;
		str2 = pEvent2->m_sDIName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //值
		iResult = pEvent1->m_nValue - pEvent2->m_nValue;
		break;
	case 5: //是否已确认
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iDIViewAsc;
	return iResult;
}

/*************************************************************
 函 数 名：OnColumnclickListDi()
 功能概要：响应点击列头事件, 对点击的列进行排序
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B850280
void CViewDI::OnColumnclickListDi(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iDIViewAsc = - g_iDIViewAsc;
	}
	else
	{
		g_iDIViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnEventPropAck()
 功能概要：响应事件属性对话框确认事件消息, 刷新界面显示
 返 回 值:  void
 参    数：param1	事件类型
		   Param2	被确认事件属性
**************************************************************/
//##ModelId=49B87B8502A0
void CViewDI::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DI)
		return;
	CXJEventDI* pEvent = (CXJEventDI*)lParam;
	if(pEvent == NULL)
		return;
	//刷新界面显示
	RefreshAck(pEvent);
}

/*************************************************************
 函 数 名：TellOtherWndAck()
 功能概要：当事件确认成功后, 通知其它窗口刷新界面显示
 返 回 值: void
 参    数：param1	被确认的事件
**************************************************************/
//##ModelId=49B87B8501C9
void CViewDI::TellOtherWndAck( CXJEventDI* pEvent )
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
 函 数 名：OnCustomDraw()
 功能概要：未确认事件改变文字颜色
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B850284
void CViewDI::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
					//得到事件对象
					CXJEventDI * pEvent = (CXJEventDI*)m_List.GetItemData(nItem);
					if(pEvent == NULL)
						break;
					
// 					if(pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
// 					{
// 						//调试
// 						lplvcd->clrTextBk = XJ_STATUS_RUN_CHECK;
// 					}

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
//##ModelId=49B87B8501D5
void CViewDI::CheckLineCount()
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

//##ModelId=49B87B850288
void CViewDI::OnDblclkListDi(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected > -1)
	{
		//点击到事件, 确认此事件
		//得到该行对应的事件
		CXJEventDI * pEvent = (CXJEventDI*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//设置当前事件
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}

//##ModelId=49B87B850293
BOOL CViewDI::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_DI);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

/*************************************************************
 函 数 名：DisplayValue()
 功能概要：得到开关量的值显示字符串
 返 回 值: 显示值
 参    数：param1	开关量值
		   Param2
**************************************************************/
//##ModelId=49B87B8501D6
CString CViewDI::DisplayValue( int nValue )
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

void CViewDI::OnEVENTRefresh() 
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
void CViewDI::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
	WriteLog("CViewDI::OnViewRefresh, 强制刷新完毕", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名: LoadConfig()
 功能概要: 读取配置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewDI::LoadConfig()
{
	CString strIniFile = g_SysConfig;
	
	//最大显示条数
	m_nMaxRow = GetPrivateProfileInt("DIViewConfig", "MaxRow", 100, strIniFile);
	//是否显示已确认事件
	m_bShowAcked = GetPrivateProfileInt("DIViewConfig", "ShowAcked", 1, strIniFile);
	//是否显示调试事件
	m_bShowDebug = GetPrivateProfileInt("DIViewConfig", "ShowDebug", 1, strIniFile);
	//设定checkbox选项
	m_bCBoxShowReady = m_bShowAcked;
	m_bCBoxShowDebug = m_bShowDebug;
}

/*************************************************************
 函 数 名: SaveConfig()
 功能概要: 保存配置
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewDI::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//最大显示条数
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString("DIViewConfig", "MaxRow", str, strIniFile);
	//是否显示已确认事件
	str.Format("%d", m_bShowAcked);
	WritePrivateProfileString("DIViewConfig", "ShowAcked", str, strIniFile);
	//是否显示调试事件
	str.Format("%d", m_bShowDebug);
	WritePrivateProfileString("DIViewConfig", "ShowDebug", str, strIniFile);
}

void CViewDI::OnEventFilterDebug() 
{
	// TODO: Add your command handler code here
	m_bShowDebug = !m_bShowDebug;
	m_bCBoxShowDebug = m_bShowDebug;
	//重新填充数据
	RefreshData();
}


void CViewDI::AddSignToEvent(CXJEventDI* pEvent)
{
	MYASSERT(pEvent);
	PT_Signal* pSignal = new PT_Signal;
	pSignal->strStation = pEvent->m_sStationName;
	pSignal->strPT_ID = pEvent->m_pSec->m_sID;
	pSignal->strPT_Model = pEvent->m_pSec->m_sModel;
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	pSignal->strTime = sTime;
	pSignal->strName = pEvent->m_sMsg;
	pSignal->nEventType = pEvent->m_nProperty;
	pSignal->nValue = pEvent->m_nValue;
	pSignal->strPTName = pEvent->m_pSec->m_sName;
	pEvent->m_Signals.Add(pSignal);
}

void CViewDI::OnEventSign() 
{
	// TODO: Add your command handler code here
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
			CXJEventDI* pEvent = (CXJEventDI*)m_List.GetItemData(nIndex);
			if (pEvent->m_Signals.GetSize() == 0)
			{
				AddSignToEvent(pEvent);
			}
			MYASSERT_CONTINUE(pEvent);
			for(int j = 0; j < pEvent->m_Signals.GetSize(); j++)
			{
				PT_Signal* sign = (PT_Signal*)pEvent->m_Signals.GetAt(j);
				dlg.AddSignal(sign);
			}
		}
	}
	
	//显示对话框
	dlg.DoModal();
}
/*************************************************************
 函 数 名: InitComBox()
 功能概要: 载入combox列表信息 默认为第一项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewDI::InitComBox()
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

void CViewDI::OnSelchangeComboChoosestatioin() 
{
	// TODO: Add your control notification handler code here
	//获取当前选择项
	int nIndex=((CComboBox*)GetDlgItem(IDC_COMBO_CHOOSESTATIOIN))->GetCurSel();
	CStationObj* pStationid=(CStationObj*)m_cmbChooseStation.GetItemData(nIndex);
	if(pStationid)
	{	CString sStationID=	pStationid->m_sID;
	
		m_sChooseStationID.Format("%s",sStationID);
	}
	else
		m_sChooseStationID = "";
	//刷新列表信息
	RefreshData();
}

void CViewDI::OnCheckShowready() 
{
	// TODO: Add your control notification handler code here
	//修改选定状态
	m_bCBoxShowReady=!m_bCBoxShowReady;
	
	m_bShowAcked=!m_bShowAcked;
	//重新填充数据
	RefreshData();
}

void CViewDI::OnCheckShowdebug() 
{
	// TODO: Add your control notification handler code here
	//修改选定状态
	m_bCBoxShowDebug=!m_bCBoxShowDebug;
	
	m_bShowDebug=!m_bShowDebug;
	//重新填充数据
	RefreshData();
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   RefreshData	
返回值:   int	
功能概要: 
*****************************************************/
int CViewDI::RefreshData()
{
	if(!LoadDataFromDB(m_nMaxRow, m_bShowAcked))
	{
		//读取数据失败
		WriteLog("读取开关量变位事件失败", 1);
		return 0;
	}
	else
	{
		//成功
		FillListData();
	}
	WriteLog("刷新开关量变位事件页面完毕", XJ_LOG_LV3);
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
void CViewDI::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	InitComBox();//重新载入厂站选择下拉框
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	BOOL bNeedReload = FALSE;
	CXJEventDI * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventDI*)pList->GetAt(i);
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
		WriteLog("StationInit,开关量变位事件页面刷新完毕", XJ_LOG_LV3);
	}
	else
		WriteLog("StationInit,开关量变位事件页面无须刷新", XJ_LOG_LV3);
}
