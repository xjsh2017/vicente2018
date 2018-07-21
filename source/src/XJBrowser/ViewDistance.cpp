// ViewDistance.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewDistance.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           升降序作用位*/
int g_iDistanceViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewDistance

IMPLEMENT_DYNCREATE(CViewDistance, CFormView)

CViewDistance::CViewDistance()
	: CFormView(CViewDistance::IDD)
{
	//{{AFX_DATA_INIT(CViewDistance)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_nOldSortCol = -1;
}

CViewDistance::~CViewDistance()
{
}

void CViewDistance::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDistance)
	DDX_Control(pDX, IDC_LIST_DISTANCE, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDistance, CFormView)
	//{{AFX_MSG_MAP(CViewDistance)
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_ACK, OnEventAck)
	ON_COMMAND(IDC_EVENT_ALL, OnEventAll)
	ON_COMMAND(IDC_EVENT_COPY, OnEventCopy)
	ON_COMMAND(IDC_EVENT_PROP, OnEventProp)
	ON_COMMAND(IDC_EVENT_Refresh, OnEVENTRefresh)
	ON_COMMAND(IDC_EVENT_HIDE, OnEventHide)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DISTANCE, OnCustomDraw)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DISTANCE, OnDblclkListDistance)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_DISTANCE, OnColumnclickListDistance)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTPT_ACK, OnEventPTAck)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20168, OnSTTP20168)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDistance diagnostics

#ifdef _DEBUG
void CViewDistance::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewDistance::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}



#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewDistance message handlers
/****************************************************
Date:2013/7/9  Author:LYH
函数名:   InitListCtrl	
返回值:   int	失败返回<=0, 成功返回>0
功能概要: 初始化列表框
*****************************************************/
int CViewDistance::InitListCtrl()
{
	RefreshData();
	return 1;
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   InitListStyle	
返回值:   int	失败返回<=0, 成功返回>0
功能概要: 初始化列表样式
*****************************************************/
int CViewDistance::InitListStyle()
{
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
		//char* arColumn[9]={"序号","时间", "线路名称","电压等级", "线路总长度", "故障点到M端距离", "杆塔位置", "维护局名称", "已确认"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 9; nCol++)
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
				lvCol.cx = 180; // 时间
				colName = StringFromID(IDS_COMMON_TIME);
				break;
			case 2://
				lvCol.cx = 220; // 线路名称 
				colName = StringFromID(IDS_MODEL_LINE);
				break;
			case 3://
				lvCol.cx = 80; // 电压等级
				colName = StringFromID(IDS_MODEL_VOLTAGE);
				break;
			case 4:
				lvCol.cx = 100; // 线路总长度
				colName = StringFromID(IDS_LINE_TOTALLENGTH);
				break;
			case 5:
				lvCol.cx = 100; //故障点到M端距离
				colName = StringFromID(IDS_FAULTPOINT_M);
				break;
			case 6:
				lvCol.cx = 80; //杆塔位置
				colName = StringFromID(IDS_TOWER_LOCATION);
				break;
			case 7:
				lvCol.cx = 160; //维护局名称
				colName = StringFromID(IDS_MAINTAIN_DEPARTMENT);
				break;
			case 8:
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
		//默认隐藏"确认"
		m_List.SetColumnHide(8, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   LoadListConfig	
返回值:   BOOL	载入成功返回TRUE, 失败返回FALSE
功能概要: 载入列信息配置
*****************************************************/
BOOL CViewDistance::LoadListConfig()
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
			WriteLog("ViewDistance::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("ViewDistance::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewDistanceConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("ViewDistance::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewDistanceConfig下的配置");
					WriteLog("ViewDistance::读取UIConfig失败,将使用默认列风格");
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

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   SaveListConfig	
返回值:   BOOL	保存成功返回TRUE, 失败返回FALSE
功能概要: 保存列信息配置
*****************************************************/
BOOL CViewDistance::SaveListConfig()
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
			WriteLog("ViewDistance::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewDistance::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewDistanceConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewDistanceConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewDistanceConfig");
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
						WriteLog("未找到ViewDistanceConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了ViewDistanceConfig下的配置, 先删除新建所有子节点");
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

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   NeedSave	
返回值:   BOOL	需要保存返回TRUE,不需要保存返回FALSE
功能概要: 判断是否需要保存列表的列信息
*****************************************************/
BOOL CViewDistance::NeedSave()
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

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   RegulateControlSize	
返回值:   void	
功能概要: 调整控件大小,位置
*****************************************************/
void CViewDistance::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//客户区大小
	CRect rc;
	GetClientRect(&rc);
	CRect rcList;
		
	rcList=rc;
	
	m_List.MoveWindow(&rcList, TRUE);
}

void CViewDistance::OnDestroy() 
{
	SaveConfig();
	if(NeedSave())
		SaveListConfig();
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CViewDistance::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

void CViewDistance::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	//读取配置
	LoadConfig();

	InitListStyle();
	//初始化列表
	InitListCtrl();
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   LoadDataFromDB	
返回值:   BOOL	查询成功返回TRUE, 失败返回FALSE
功能概要: 从数据库读取数据
参数: int num	要查找的个数
参数: BOOL IsQueryAcked	是否要查询已确认的事件
*****************************************************/
BOOL CViewDistance::LoadDataFromDB( int num, BOOL IsQueryAcked /*= TRUE*/ )
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

	//排序
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_DISTANCE_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewDistance::LoadDataFromDB, 查询失败");
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
			str.Format("CViewDistance::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventDistance * pEvent = new CXJEventDistance;
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
		str.Format("CViewDistance::LoadDataFromDB,查询失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   AddEventToList	
返回值:   BOOL	加入成功返回TRUE,失败返回FALSE
功能概要: 把一条事件的信息加入到列表
参数: CXJEventDistance * pEvent	要加入的事件
参数: int i	加入的位置
*****************************************************/
BOOL CViewDistance::AddEventToList( CXJEventDistance * pEvent, int i )
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;
	//"序号","时间", "线路名称","电压等级", "线路总长度", "故障点到M端距离", "杆塔位置", "维护局名称", "已确认"
	//0:序号
	CString str;
	str.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:时间
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 1, sTime);
	//2:线路名称
	m_List.SetItemText(i, 2, pEvent->m_sLineName);
	//3:电压等级
	str.Format("%dkV", pEvent->m_nVoltage);
	m_List.SetItemText(i, 3, str);
	//4:线路总长度
	str.Format("%.2fkm", pEvent->m_fLineTotalLength);
	m_List.SetItemText(i, 4, str);
	//5:故障点到M端距离
	str.Format("%.2fkm", pEvent->m_fDistance);
	m_List.SetItemText(i, 5, str);
	//6:杆塔位置
	str.Format("%d", pEvent->m_nTowerPos);
	m_List.SetItemText(i, 6, str);
	//7:维护局名称
	m_List.SetItemText(i, 7, pEvent->m_sManagerUnit);
	//8:已确认
	m_List.SetItemText(i, 8, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//设置数据
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   FillListData	
返回值:   int	失败返回<=0, 成功返回>0
功能概要: 填充列表数据
*****************************************************/
int CViewDistance::FillListData()
{
	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	CXJEventDistance * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventDistance *)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	//恢复刷新
	m_List.SetRedraw(TRUE);
	return 1;
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   RefreshData	
返回值:   int	失败返回<=0, 成功返回>0
功能概要: 刷新数据，负责清空数据、从数据库读入、填充到列表框
*****************************************************/
int CViewDistance::RefreshData()
{
	if(!LoadDataFromDB(m_nMaxRow, m_bShowAcked))
	{
		//读取数据失败
		WriteLog("读取小波测距事件失败", XJ_LOG_LV1);
		return 0;
	}
	else
	{
		//成功
		FillListData();
	}
	WriteLog("刷新小波测距事件页面完毕", XJ_LOG_LV3);
	return 1;
}

void CViewDistance::OnContextMenu(CWnd* pWnd, CPoint point) 
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
				pMenu->EnableMenuItem(IDC_EVENT_PROP, MF_ENABLED);
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

void CViewDistance::OnEventAck() 
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
			CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nIndex);
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

void CViewDistance::OnEventAll() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//无权限
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckAllEvent(EX_STTP_INFO_DISTANCE_CFG))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventDistance* pEvent = (CXJEventDistance*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

void CViewDistance::OnEventCopy() 
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
			CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nIndex);
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

void CViewDistance::OnEventProp() 
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
	CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nIndex);
	if(pEvent == NULL)
		return;
	//设置当前事件
	m_EventManager.m_pCurrentEvent = pEvent;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowEventProp(&m_EventManager);
}

void CViewDistance::OnEVENTRefresh() 
{
	// TODO: Add your command handler code here
	RefreshData();
}

void CViewDistance::OnEventHide() 
{
	// TODO: Add your command handler code here
	m_bShowAcked = !m_bShowAcked;
	//重新填充数据
	RefreshData();
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   RefreshAck	
返回值:   void	
功能概要: 确认成功后刷新界面显示
参数: CXJEventDistance * pEvent	确认的事件
参数: int nIndex	事件位置
*****************************************************/
void CViewDistance::RefreshAck( CXJEventDistance* pEvent, int nIndex /*= -1*/ )
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
	m_List.SetItemText(nIndex, 8, StringFromID(IDS_ACK_YES));
	
	m_List.Update(nIndex);
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   TellOtherWndAck	
返回值:   void	
功能概要: 告诉其它窗口这里确认了事件
参数: CXJEventDistance * pEvent	被确认的事件
*****************************************************/
void CViewDistance::TellOtherWndAck( CXJEventDistance* pEvent )
{
	//发送消息通知事件属性对话框
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
	if(pFrame != NULL)
	{
		pFrame->PostMessage(WM_EVENTLIST_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
	}
}
void CViewDistance::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;
	//刷新界面显示
	RefreshAck(pEvent);
}

void CViewDistance::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
	WriteLog("CViewDistance::OnViewRefresh, 强制刷新完毕", XJ_LOG_LV3);
}

void CViewDistance::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
					CXJEventDistance * pEvent = (CXJEventDistance*)m_List.GetItemData(nItem);
					if(pEvent == NULL)
						break;
					
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

void CViewDistance::OnDblclkListDistance(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected > -1)
	{
		//点击到事件, 确认此事件
		//得到该行对应的事件
		CXJEventDistance * pEvent = (CXJEventDistance*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//设置当前事件
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}

void CViewDistance::OnColumnclickListDistance(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iDistanceViewAsc = - g_iDistanceViewAsc;
	}
	else
	{
		g_iDistanceViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   CompareFunction	
返回值:   int CALLBACK	比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
功能概要: 比较函数,排序时被调用
参数: LPARAM lParam1	比较对象1
参数: LPARAM lParam2	比较对象2
参数: LPARAM lParamData	比较参数
*****************************************************/
int CALLBACK CViewDistance::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CXJEventDistance* pEvent1 = (CXJEventDistance*)lParam1;
	CXJEventDistance* pEvent2 = (CXJEventDistance*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号","时间", "线路名称","电压等级", "线路总长度", "故障点到M端距离", "杆塔位置", "维护局名称", "已确认"
	switch(nCol)
	{
	case 0: //事件ID
		iResult = pEvent1->m_lID - pEvent2->m_lID;
		break;
	case 1: //发生时间
		str1.Format("%s.%03d", pEvent1->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nms);
		str2.Format("%s.%03d", pEvent2->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nms);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //线路名称
		str1 = pEvent1->m_sLineName;
		str2 = pEvent2->m_sLineName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3:	//电压等级
		iResult = pEvent1->m_nVoltage - pEvent2->m_nVoltage;
		break;
	case 4: //线路总长度
		if(pEvent1->m_fLineTotalLength - pEvent2->m_fLineTotalLength < -0.00001)
			iResult = -1;
		else if(pEvent1->m_fLineTotalLength - pEvent2->m_fLineTotalLength > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 5: //故障点到M端距离
		if(pEvent1->m_fDistance - pEvent2->m_fDistance < -0.00001)
			iResult = -1;
		else if(pEvent1->m_fDistance - pEvent2->m_fDistance > 0.00001)
			iResult = 1;
		else
			iResult = 0;
		break;
	case 6://杆塔位置
		iResult = pEvent1->m_nTowerPos - pEvent2->m_nTowerPos;
		break;
	case 7: //维护局名称
		str1 = pEvent1->m_sManagerUnit;
		str2 = pEvent2->m_sManagerUnit;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 8://已确认
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iDistanceViewAsc;
	return iResult;
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   CheckLineCount	
返回值:   void	
功能概要: 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的
*****************************************************/
void CViewDistance::CheckLineCount()
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
		CXJEventDistance* pEvent = (CXJEventDistance*)m_List.GetItemData(nNewCount - 1);
		//从列表中删除
		m_List.DeleteItem(nNewCount - 1);
		//从事件链表和内存中删除
		m_EventManager.RemoveAtEvent(pEvent);
	}
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   LoadConfig	
返回值:   void	
功能概要: 读取配置
*****************************************************/
void CViewDistance::LoadConfig()
{
	CString strIniFile = g_SysConfig;
	
	//最大显示条数
	m_nMaxRow = GetPrivateProfileInt("DistanceViewConfig", "MaxRow", 100, strIniFile);
	//是否显示已确认事件
	m_bShowAcked = GetPrivateProfileInt("DistanceViewConfig", "ShowAcked", 1, strIniFile);
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   SaveConfig	
返回值:   void	
功能概要: 保存配置
*****************************************************/
void CViewDistance::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//最大显示条数
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString("DistanceViewConfig", "MaxRow", str, strIniFile);
	//是否显示已确认事件
	str.Format("%d", m_bShowAcked);
	WritePrivateProfileString("DistanceViewConfig", "ShowAcked", str, strIniFile);
}

/****************************************************
Date:2013/7/9  Author:LYH
函数名:   OnSTTP20168	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewDistance::OnSTTP20168( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewDistance::OnSTTP20168 start");
	
	CXJEventDistance* pParam = (CXJEventDistance*)lParam;
	if(pParam == NULL)
		return;

	//复制
	CXJEventDistance* pEvent = (CXJEventDistance*)pParam->Clone();
	if(pEvent == NULL)
		return;
	
	//加入到链表
	m_EventManager.AddEvent(pEvent);
	//加入到列表,加入到第一行
	AddEventToList(pEvent, 0);
	CheckLineCount();
	
	WriteLog("CViewDistance::OnSTTP20168 neat end");
}

void CViewDistance::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	//GetParentFrame()->SetWindowText("小波测距");
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_LOCATION));
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CViewDistance::OnEventPTAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;
	//查找在本地的地址
	CXJEventDistance* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//找到了本地地址,更新显示
		RefreshAck(pLocal);
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
		if(pFrame != NULL)
			pFrame->PostMessage(WM_EVENTLIST_PASSIVE_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pLocal);
	}
}

void CViewDistance::OnEventPropAckPT( WPARAM wParma, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParma;
	if(nType != XJ_EVENT_DISTANCE)
		return;
	CXJEventDistance* pEvent = (CXJEventDistance*)lParam;
	if(pEvent == NULL)
		return;
	//查找在本地的地址
	CXJEventDistance* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//找到了本地地址,更新显示
		RefreshAck(pLocal);
	}
}

CXJEventDistance* CViewDistance::FindLocalPoint( CXJEventDistance* pEvent )
{
	if(pEvent == NULL)
		return NULL;
	CXJEventDistance* pReturn = NULL;
	//因为现在有ID不知道的情况,只能比较二次设备ID, 发生时间等信息
	for(int i = 0; i < m_EventManager.GetCount(); i++)
	{
		CXJEventDistance* pTemp = (CXJEventDistance*)m_EventManager.GetAtEvent(i);
		if(pTemp->m_pSec == pEvent->m_pSec)
		{
			//二次设备相同,比较发生时间,时间用字符串比
			if(pTemp->m_tmTime == pEvent->m_tmTime)
			{
				//发生时间相同,比较发生时间毫秒值
				if(pTemp->m_nms == pEvent->m_nms)
				{
					//相同,认为是同一事件
					pTemp->m_lID = pEvent->m_lID;
					pTemp->m_nIsAck = pEvent->m_nIsAck;
					pReturn = pTemp;
					break;
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
