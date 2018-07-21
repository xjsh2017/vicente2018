// ViewPTComm.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewPTComm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           升降序作用位*/
int g_iPTCommViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewPTComm

IMPLEMENT_DYNCREATE(CViewPTComm, CFormView)

//##ModelId=49B87B8201F5
CViewPTComm::CViewPTComm()
	: CFormView(CViewPTComm::IDD)
{
	//{{AFX_DATA_INIT(CViewPTComm)
	m_bShowPTCommOFF = g_ShowPTCommOFF;
	m_bShowPTCommON = g_ShowPTCommON;
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_PTCOMM;
	m_EventManager.ClearAllEvents();
	m_nMaxRow = 100;
	m_bShowAcked = TRUE;
	m_nOldSortCol = -1;
	m_nTimer = 0;
	g_iPTCommViewAsc = -1;
	m_nTimer = -1;
	m_sChooseStationID = "";

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
}

//##ModelId=49B87B82033C
CViewPTComm::~CViewPTComm()
{
	m_EventManager.ClearAllEvents();
	TRACE("CViewPTComm::~CViewPTComm \n");
}

//##ModelId=49B87B82030D
void CViewPTComm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewPTComm)
	DDX_Control(pDX, IDC_CHECK_SHOWPTCOMMON, m_cBoxShowPTCommON);
	DDX_Control(pDX, IDC_CHECK_SHOWPTCOMMOFF, m_cBoxShowPTcommOFF);
	DDX_Control(pDX, IDC_COMBO_CHOOSESTATIOIN, m_cmbChooseStation);
	DDX_Control(pDX, IDC_STATIC_CHOOSESTATION, m_staticChooseStation);
	DDX_Control(pDX, IDC_LIST_PTCOMM, m_List);
	DDX_Check(pDX, IDC_CHECK_SHOWPTCOMMOFF, m_bShowPTCommOFF);
	DDX_Check(pDX, IDC_CHECK_SHOWPTCOMMON, m_bShowPTCommON);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewPTComm, CFormView)
	//{{AFX_MSG_MAP(CViewPTComm)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTCOMM, OnCustomDraw)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_EVENT_DEVICE, OnEventDevice)
	ON_COMMAND(IDC_EVENT_SEC, OnEventSec)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTCOMM, OnClickListPtcomm)
	ON_COMMAND(IDC_EVENT_SEC_PROP, OnEventSecProp)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTCOMM, OnColumnclickListPtcomm)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTCOMM, OnDblclkListPtcomm)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_PTCOMM_ON, OnPtcommOn)
	ON_COMMAND(IDC_PTCOMM_OFF, OnPtcommOff)
	ON_COMMAND(IDC_PTCOMM_HISTORY, OnPtcommHistory)
	ON_WM_TIMER()
	ON_COMMAND(IDC_PTCOMM_EXPORT, OnPtcommExport)
	ON_COMMAND(IDC_PTCOMM_PRINT, OnPtcommPrint)
	ON_COMMAND(IDC_PTCOMM_REFRESH, OnPtcommRefresh)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOOSESTATIOIN, OnSelchangeComboChoosestatioin)
	ON_BN_CLICKED(IDC_CHECK_SHOWPTCOMMON, OnCheckShowptcommon)
	ON_BN_CLICKED(IDC_CHECK_SHOWPTCOMMOFF, OnCheckShowptcommoff)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20002, OnSTTP20002)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewPTComm diagnostics

#ifdef _DEBUG
//##ModelId=49B87B82035B
void CViewPTComm::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B82036B
void CViewPTComm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewPTComm message handlers

//##ModelId=49B87B82032C
void CViewPTComm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_DEVICECOMMU));
	//GetParentFrame()->SetWindowText("设备通断");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B820301
void CViewPTComm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
		CSize sizeTotal(100, 100);
		SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	InitComBox();
	InitListCtrl();

	//开始刷新保护通信状态持续时间定时器
	m_nTimer = SetTimer(1, g_RefreshPTCommTime*60*1000, 0);
}

//##ModelId=49B87B82037A
void CViewPTComm::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
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
//##ModelId=49B87B820254
int CViewPTComm::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
	FillListData();

	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B820271
int CViewPTComm::InitListStyle()
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
		//char* arColumn[9]={"设备名称","状态", "状态改变时间", "状态持续时间", "厂站","一次设备", "调管单位", "运维单位", "检修单位"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 9; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName="";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 300; //设备名称
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 1://
				lvCol.cx = 100; // 状态
				colName = StringFromID(IDS_COMMU_STATUS);
				break;
			case 2://
				lvCol.cx = 180; // 时间
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 3://
				lvCol.cx = 200; // 持续时间
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 4://
				lvCol.cx = 140; // 厂站
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 5:
				lvCol.cx = 160; // 一次设备
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			case 6:
				lvCol.cx = 160; // 所属调管单位
				colName = StringFromID(IDS_MODEL_MANAGEMENT);
				break;
			case 7:
				lvCol.cx = 160; // 运维单位
				colName = StringFromID(IDS_MODEL_MAINTENANCE);
				break;
			case 8:
				lvCol.cx = 160; // 检修单位
				colName = StringFromID(IDS_MODEL_OVERHUALUNIT);
				break;
			default:
				lvCol.cx=160;//列宽100象素
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第7列(所属调管单位)
		m_List.SetColumnHide(6, TRUE);
		//默认隐藏第8列(运维单位)
		m_List.SetColumnHide(7, TRUE);
		m_List.SetColumnHide(8, TRUE);
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
//##ModelId=49B87B820281
BOOL CViewPTComm::LoadListConfig()
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
			WriteLog("ViewPTComm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("ViewPTComm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewPTCommConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewPTCommConfig", XJ_LOG_LV3);
				WriteLog("ViewPTComm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewCommConfig下的配置", XJ_LOG_LV3);
					WriteLog("ViewPTComm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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

//##ModelId=49B87B82029F
BOOL CViewPTComm::SaveListConfig()
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
			WriteLog("ViewPTComm::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewPTComm::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewPTCommConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewPTCommConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewPTCommConfig");
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
						WriteLog("未找到ViewPTCommConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
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
						WriteLog("找到了ViewPTCommConfig下的配置, 先删除新建所有子节点", XJ_LOG_LV3);
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
//##ModelId=49B87B820280
int CViewPTComm::FillListData()
{

	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return -1;

	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice);
	//循环所有厂站
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		if (pObj->m_pStation->m_sID == m_sChooseStationID||m_sChooseStationID.IsEmpty())
		{
			AddEventToList(pObj, 0);
		}
		
	}
	m_cBoxShowPTcommOFF.SetCheck(m_bShowPTCommOFF);
	m_cBoxShowPTCommON.SetCheck(m_bShowPTCommON);
	//恢复刷新
	m_List.SetRedraw(TRUE);

	return 0;
}

//##ModelId=49B87B8202A0
BOOL CViewPTComm::NeedSave()
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
//##ModelId=49B87B8202AF
void CViewPTComm::RegulateControlSize()
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
		m_cBoxShowPTCommON.ShowWindow(FALSE);
		m_cBoxShowPTcommOFF.ShowWindow(FALSE);
	//	m_cboxShowDebug.ShowWindow(FALSE);
	//	m_cboxShowReturn.ShowWindow(FALSE);
	//	m_cboxShowReady.ShowWindow(FALSE);
		rcList=rc;
	}
	else
	{
		rcList=CRect(rc.left,rc.top+45,rc.right,rc.bottom);
	}
	
	
	//调整列表框大小,占整个高度的(1 - 0.618)

	m_List.MoveWindow(&rcList, TRUE);
}

//##ModelId=49B87B820399
void CViewPTComm::OnDestroy() 
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
//##ModelId=49B87B8202BF
BOOL CViewPTComm::LoadDataFromDB(int num, BOOL IsQueryAcked)
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
	//选择显示厂站的信息
	if ( !m_sChooseStationID.IsEmpty())
	{
		Condition condition5;
		CString strFilter;
		strFilter.Format("(pt_id IN (select pt_id from tb_secdev_base where station_id='%s'))",m_sChooseStationID);
		pApp->m_DBEngine.SetCondition(sql,condition5,strFilter);
	}
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_COMMU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewPTComm::LoadDataFromDB, 查询失败", XJ_LOG_LV3);
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
			str.Format("CViewPTComm::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventPTComm * pEvent = new CXJEventPTComm;
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
		str.Format("CViewPTComm::LoadDataFromDB,查询失败,原因为%s", sError);
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
 函 数 名：OnSTTP20002()
 功能概要：响应20002(保护通讯状态上载通知)报文
 返 回 值: void
 参    数：param1	设备指针
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B830021
void CViewPTComm::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewPTComm::OnSTTP20002 start", XJ_LOG_LV3);
	//取得设备指针
	CSecObj* pObj = (CSecObj*)wParam;
	if(pObj != NULL)
	{
		if (pObj->m_sStationID != m_sChooseStationID&& !m_sChooseStationID.IsEmpty())
		{
			CString str;
			str.Format("收到厂站ID为%s报文，并非当前选定的厂站，已过滤。",pObj->m_sStationID);
			WriteLog(str, XJ_LOG_LV3);
			return;
		}
		//改变在通信状态列表中的显示
 		ChangeStatusInList(pObj);

		CString str;
		str.Format("装置ID:%s,名称:%s,厂站ID:%s,当前状态:%d", pObj->m_sID, pObj->m_sName, pObj->m_sStationID,pObj->m_nCommStatus);
		WriteLog(str);
	}
	//闪烁
	if(pObj != NULL && g_BlinkDevice == 1)
	{
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserView * pView = pApp->GetSVGView();
		//CViewCommMap* pCommView = pApp->GetCommView();
		//++m_nCount;
		if(pView != NULL)
		{
			WriteLog("CViewPTComm::OnSTTP20002, 闪烁设备", XJ_LOG_LV3);
			//闪烁
			//pView->AddBlinkDevice(pObj);
			pView->AddToBlinkList(pObj);
		}
	}	

	WriteLog("CViewPTComm::OnSTTP20002 neat end", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置非正常状态下背景色
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B82039B
void CViewPTComm::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
					//if(nSubItem == 1)//状态
					//{
						//得到事件对象
						CSecObj * pSec = (CSecObj*)m_List.GetItemData(nItem);
						if(pSec == NULL)
							break;
						if(pSec->m_nCommStatus == 1)
						{
							//断开
						    lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					//}
				}
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

//##ModelId=49B87B8203AA
void CViewPTComm::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_PTCOMM))
				return;

			Menu.DeleteMenu(IDC_EVENT_DEVICE, MF_BYCOMMAND);
			Menu.DeleteMenu(IDC_PTCOMM_REFRESH, MF_BYCOMMAND);

			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount == 1)
			{
				//只选择了一个
				pMenu->EnableMenuItem(IDC_EVENT_DEVICE, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_EVENT_SEC_PROP, MF_ENABLED);
			}
			if(1 == g_ShowPTCommON)
			{
				//显示通信状态为正常的装置信息
				pMenu->CheckMenuItem(IDC_PTCOMM_ON, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_PTCOMM_ON, MF_UNCHECKED);
			}
			if(1 == g_ShowPTCommOFF)
			{
				//显示通信状态为断开的装置信息
				pMenu->CheckMenuItem(IDC_PTCOMM_OFF, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_PTCOMM_OFF, MF_UNCHECKED);
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}


//##ModelId=49B87B8203B9
void CViewPTComm::OnEventDevice() 
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
	//得到设备
	CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
	if(pObj == NULL)
		return;
	//显示该事件的一次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pObj->m_pOwner == NULL)
		return;
	pApp->ShowDevice(pObj->m_pOwner);
}

//##ModelId=49B87B8203BB
void CViewPTComm::OnEventSec() 
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
	//得到设备
	CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
	if(pObj == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->ShowDevice(pObj);
}

//##ModelId=49B87B8203C8
void CViewPTComm::OnClickListPtcomm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
// 	得到所选行
// 		NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
// 		int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
// 		if(nSelected > -1)
// 		{
// 			//点击到事件, 确认此事件
// 			//得到该行对应的事件
// 			CXJEventPTComm * pEvent = (CXJEventPTComm*)m_List.GetItemData(nSelected);
// 			if(pEvent == NULL)
// 				return;
// 			if(pEvent->Ack())
// 			{
// 				//刷新界面显示, 通知其它窗口更新
// 				RefreshAck(pEvent, nSelected);
// 				TellOtherWndAck(pEvent);
// 			}
// 		}
	*pResult = 0;
}

//##ModelId=49B87B8203CC
void CViewPTComm::OnEventSecProp() 
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
	//得到设备
	CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
	if(pObj == NULL)
		return;
	//显示该事件的二次设备图形
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	if(pObj->m_nType == TYPE_WAVEREC)
		pDoc->ShowSecPropPage(pObj, 1);
	else
		pDoc->ShowSecPropPage(pObj, PT_PROP_GENERAL);
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,排序时被调用 
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数：param1	比较对象1
		   Param2	比较对象2
		   param3	比较参数
**************************************************************/
//##ModelId=49B87B820247
int CALLBACK CViewPTComm::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CSecObj* pObj1 = (CSecObj*)lParam1;
	CSecObj* pObj2 = (CSecObj*)lParam2;
	if(pObj2 == NULL || pObj1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"设备名称","状态", "状态改变时间", "状态持续时间", "厂站","一次设备","所属调管单位", "运维单位"
	switch(nCol)
	{
	case 0: //保护名称
		str1 = pObj1->m_sName;
		str2 = pObj2->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 1: //状态
		iResult = pObj1->m_nCommStatus - pObj2->m_nCommStatus;
		break;
	case 2: //状态改变时间
		str1.Format("%s", pObj1->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		str2.Format("%s", pObj2->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //状态持续时间
		iResult = pObj1->m_lCommDurative - pObj2->m_lCommDurative;
		break;
	case 4: //厂站名
		str1 = pObj1->m_sStationID;
		str2 = pObj2->m_sStationID;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //一次设备名
		str1 = "";
		if(pObj1->m_pOwner != NULL)
			str1 = pObj1->m_pOwner->m_sName;
		str2 = "";
		if(pObj2->m_pOwner != NULL)
			str2 = pObj2->m_pOwner->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: // 所属调管单位
		if (pObj1->m_pStation != NULL && pObj2->m_pStation != NULL)
			iResult = pObj1->m_pStation->m_nManagedepartment - pObj2->m_pStation->m_nManagedepartment;
		break;
	case 7: // 运维单位
		if (pObj1->m_pStation != NULL && pObj2->m_pStation != NULL)
			iResult = pObj1->m_pStation->m_nMaintaindepartment - pObj2->m_pStation->m_nMaintaindepartment;
		break;
	case 8: //检修单位
		if (pObj1->m_pStation != NULL && pObj2->m_pStation != NULL)
			iResult = pObj1->m_pStation->m_nServiceDepartment - pObj2->m_pStation->m_nServiceDepartment;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTCommViewAsc;
	return iResult;
}

//##ModelId=49B87B8203D8
void CViewPTComm::OnColumnclickListPtcomm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iPTCommViewAsc = - g_iPTCommViewAsc;
	}
	else
	{
		g_iPTCommViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87B8203DC
void CViewPTComm::OnDblclkListPtcomm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected > -1)
	{
		
	}
	*pResult = 0;
}

//##ModelId=49B87B830000
BOOL CViewPTComm::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTCOMM);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

/*************************************************************
 函 数 名：AddEventToList()
 功能概要：将指定二次设备的通信状态信息加入到列表框中
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1	指定二次设备
		   Param2	指定在列表中的位置
**************************************************************/
//##ModelId=49B87B8202CE
BOOL CViewPTComm::AddEventToList( CSecObj* pSec, int i )
{
	//判断数据有效性
	if(pSec == NULL)
		return FALSE;
	if(m_List.GetSafeHwnd() == NULL)
		return FALSE;
	if(!IsNeedShow(pSec->m_nCommStatus))
		return FALSE;
	//刷新持续时间
	pSec->RefreshCommDurativeTime();

	//"设备名称","状态", "状态改变时间", "状态持续时间", "厂站","一次设备", "所属调管单位", "运维单位"

	//0:设备名称
	CString str;
	str = pSec->m_sName;
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;
	//1:状态
	m_List.SetItemText(i, 1, GetCommStatusString(pSec->m_nCommStatus));
	//2:状态改变时间
	m_List.SetItemText(i, 2, pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
	//3:状态持续时间
	m_List.SetItemText(i, 3, pSec->m_sCommDurativeTime);
	//4:厂站名
	str = "";
	if(pSec->m_pStation != NULL)
	{
		str = pSec->m_pStation->m_sName;
	}
	m_List.SetItemText(i, 4, str);
	//一次设备
	str = "";
	if(pSec->m_pOwner != NULL)
	{
		str = pSec->m_pOwner->m_sName;
	}
	m_List.SetItemText(i, 5, str);
	
	//所属调管单位
	str = "";
	if(pSec->m_pStation != NULL)
	{
		str = pSec->m_pStation->m_sManagedepartment;
	}
	m_List.SetItemText(i, 6, str);
	
	//运维单位
	str = "";
	if(pSec->m_pStation != NULL)
	{
		str = pSec->m_pStation->m_sMaintaindepartment;
	}
	m_List.SetItemText(i, 7, str);

	str = "";
	if(pSec->m_pStation != NULL)
	{
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		if(pData != NULL)
			str = pData->GetServiceDepartmentName(pSec->m_pStation->m_nServiceDepartment);
	}
	m_List.SetItemText(i, 8, str);

	//关联数据
	m_List.SetItemData(i, (DWORD)pSec);
	return TRUE;
}

/*************************************************************
 函 数 名：IsNeedShow()
 功能概要：判断当前通信状态的保护信息是否需要显示
 返 回 值: 需要显示返回TRUE, 不需要显示返回FALSE
 参    数：param1	通信状态
		   Param2
**************************************************************/
//##ModelId=49B87B8202DE
BOOL CViewPTComm::IsNeedShow( int nStatus )
{
	BOOL bReturn = FALSE;
	switch(nStatus)
	{
	case 0: //正常
		if(1 == g_ShowPTCommON)
			bReturn = TRUE;
		else
			bReturn = FALSE;
		break;
	case 1: //断开
		if(1 == g_ShowPTCommOFF)
			bReturn = TRUE;
		else 
			bReturn = FALSE;
		break;
	default:
		bReturn = TRUE;
		break;
	}
	return bReturn;
}

//##ModelId=49B87B830003
void CViewPTComm::OnPtcommOn() 
{
	// TODO: Add your command handler code here
	//改变通信状态正常是否要显示的设置
	g_ShowPTCommON = !g_ShowPTCommON;
	m_bShowPTCommON = g_ShowPTCommON;
	//重新填充数据
	FillListData();
}

//##ModelId=49B87B830005
void CViewPTComm::OnPtcommOff() 
{
	// TODO: Add your command handler code here
	//改变通信状态断开是否要显示的设置
	g_ShowPTCommOFF = !g_ShowPTCommOFF;
	m_bShowPTCommOFF = g_ShowPTCommOFF;
	//重新填充数据
	FillListData();
}

//##ModelId=49B87B83000F
void CViewPTComm::OnPtcommHistory() 
{
	// TODO: Add your command handler code here
	//准备打开查询与统计的参数
	//保护通讯状态查询: 2$$$厂站ID$$$设备类型$$$设备ID $$$通讯状态$$$自动查询(0-不自动查询, 1-自动查询)
	CString sSQL = "";
	int nCount = m_List.GetSelectedCount();
	if(nCount < 1)
	{
		sSQL = "2$$$ $$$99$$$ $$$99$$$0";
	}
	else
	{
		int nIndex = -1;
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex == -1)
			return;
		//得到设备
		CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
		if(pObj == NULL)
			return;
		if(pObj->m_nType == TYPE_PROTECT)
			sSQL.Format("2$$$ $$$99$$$ $$$%s$$$99$$$1", pObj->m_sID);
		else
			sSQL.Format("2$$$ $$$99$$$%s$$$ $$$99$$$1", pObj->m_sID);
	}
	//打开查询与统计
	OpenXJQuery(sSQL, this->GetSafeHwnd());
}

/*************************************************************
 函 数 名：ChangeStatusInList()
 功能概要：改变指定二次设备的通信状态在通信状态列表框中的显示
 返 回 值: void
 参    数：param1	指定二次设备
		   Param2
**************************************************************/
//##ModelId=49B87B8202E0
void CViewPTComm::ChangeStatusInList( CSecObj* pSec )
{
	//判断数据有效性
	if(pSec == NULL)
		return;
	if(m_List.GetSafeHwnd() == NULL)
		return;

	//查找在列表中的位置
	LVFINDINFO lvFind;
	int nIndex = -1;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pSec;
	nIndex = m_List.FindItem(&lvFind);

	//判断是此状态下是否需要显示设备信息
	if(!IsNeedShow(pSec->m_nCommStatus))
	{
		if(nIndex > -1)
		{
			//移除
			m_List.DeleteItem(nIndex);
		}
	}
	else
	{
		if(nIndex < 0)
		{
			//增加
			AddEventToList(pSec, 0);
		}
		else
		{
			//改变值
			//1:状态
			m_List.SetItemText(nIndex, 1, GetCommStatusString(pSec->m_nCommStatus));
			//2:状态改变时间
			m_List.SetItemText(nIndex, 2, pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
			//3:状态持续时间
			m_List.SetItemText(nIndex, 3, pSec->m_sCommDurativeTime);
			//刷新
			m_List.Update(nIndex);
		}
	}
}

//##ModelId=49B87B830011
void CViewPTComm::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//刷新厂站持续时间
		if(!g_bSuspend)
			RefreshTime();
	}
	CFormView::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：RefreshTime()
 功能概要：刷新所有保护的通信状态的持续时间
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8202F0
void CViewPTComm::RefreshTime()
{
	WriteLog("CViewPTComm::RefreshTime,定时刷新开始", XJ_LOG_LV3);
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	int nCount = m_List.GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		if(g_bSuspend)
			break;
		RefreshDuravieTime(i);
	}
	//恢复刷新
	m_List.SetRedraw(TRUE);
	WriteLog("CViewPTComm::RefreshTime,定时刷新结束", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：RefreshDuravieTime()
 功能概要：刷新LIST中指定位置的持续时间显示
 返 回 值: 
 参    数：param1	指定在LIST中的位置
		   Param2
**************************************************************/
//##ModelId=49B87B8202EE
void CViewPTComm::RefreshDuravieTime( int nIndex )
{
	//总数
	int nCount = m_List.GetItemCount();
	//判断索引有效性
	if(nIndex < 0)
		return;
	if(nIndex >= nCount)
		return;
	CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
	if(pObj == NULL)
		return;
	//刷新持续时间
	pObj->RefreshCommDurativeTime();
	//更改显示
	m_List.SetItemText(nIndex, 3, pObj->m_sCommDurativeTime);
	//刷新
	m_List.Update(nIndex);
}

//##ModelId=49B87B830014
void CViewPTComm::OnPtcommExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	
	CString strtype = StringFromID(IDS_TITLE_DEV_COMMUSTATUS);
	if(m_bShowPTCommON && !m_bShowPTCommOFF)
		strtype = StringFromID(IDS_TITLE_DEV_COMMUON);
	if(!m_bShowPTCommON && m_bShowPTCommOFF)
		strtype = StringFromID(IDS_TITLE_DEV_COMMUOFF);
	if(!m_bShowPTCommON && !m_bShowPTCommOFF)
		strtype = StringFromID(IDS_TITLE_DEV_COMMUUNKNOWN);
	CString sStationName = StringFromID(IDS_COMMON_ALL);
	if(!m_sChooseStationID.IsEmpty())
	{
		CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_COMBO_CHOOSESTATIOIN);
		if(pCmb)
		{
			int nIndex = pCmb->GetCurSel();
			CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nIndex);
			if(pStation)
				sStationName = pStation->m_sName;
		}
	}
	CString strName;
	strName.Format("%s-%s%s.xls", sStationName, strtype, t.Format("%Y%m%d%H%M%S"));
	//取得用户选择文件夹
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(WriteResultToExcel(strPath))
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

//##ModelId=49B87B83001F
void CViewPTComm::OnPtcommPrint() 
{
	// TODO: Add your command handler code here
	//打印
	InitCOM();
	//打开模板文件
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, PTCOMM_LIST);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),PTCOMM_LIST);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//设置回调函数
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInPTComm);
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

/*************************************************************
函 数 名：InitCOM()
功能概要：初始化打印模板COM组件
返 回 值: 初始化成功返回TRUE, 失败返回FALSE
参    数：param1
Param2
**************************************************************/
//##ModelId=49B87B8202F1
BOOL CViewPTComm::InitCOM()
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
//##ModelId=49B87B8202F2
void CViewPTComm::ReleaseCOM()
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
 函 数 名：WriteResultToExcel()
 功能概要：将列表中的数据导出到Excel
 返 回 值: 导出成功返回TRUE, 失败返回FALSE
 参    数：param1	Excel文件保存位置
		   Param2
**************************************************************/
//##ModelId=49B87B8202F3
BOOL CViewPTComm::WriteResultToExcel( CString strFullPath )
{
	CDatabase database;
	CString strDriver = "MICROSOFT EXCEL DRIVER (*.XLS)";
	CString strSQL,str;
	
	strSQL.Format("DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",
		strDriver, strFullPath, strFullPath);
	
	CFileFind find;
	BOOL IsFind = FALSE;
	IsFind = find.FindFile(strFullPath, 0);
	
	try
	{
		if( database.OpenEx(strSQL,CDatabase::noOdbcDialog) )
		{
			if(!IsFind)
			{
				//新建
				//"设备名称","状态", "状态改变时间", "状态持续时间", "厂站","一次设备","所属调管单位", "运维单位"
				strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT)",
					StringFromID(IDS_COMMON_NO), StringFromID(IDS_MODEL_MANAGEMENT), StringFromID(IDS_MODEL_MAINTENANCE),
					StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_MODEL_PRIMARY), StringFromID(IDS_MODEL_SECONDARY), StringFromID(IDS_COMMU_STATUS),
					StringFromID(IDS_STATUS_CHANGE_TIME), StringFromID(IDS_COMMON_DURATION));
				database.ExecuteSQL(strSQL);
			}
			for(int i = 0; i < m_List.GetItemCount(); i++)
			{
				CSecObj* pObj = (CSecObj*)m_List.GetItemData(i);
				AddResultToExcel(&database, pObj, i+1);
			}
		}
		else
			return FALSE;
		database.Close();
	}
	catch(CDBException * e)
	{
		TRACE("数据库错误: " + e->m_strError);
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：AddResultToExcel()
 功能概要：将指定的结果项写入到指定的Excel文件中
 返 回 值: void
 参    数：param1	数据库对象
		   Param2	设备对象
**************************************************************/
//##ModelId=49B87B8202FE
void CViewPTComm::AddResultToExcel( CDatabase* pDatabase, CSecObj* pSec, int index )
{
	//检查数据有效性
	if(pSec == NULL)
		return;
	if(pDatabase == NULL)
		return;

	//4:厂站名
	CString strStation = "";
	if(pSec->m_pStation != NULL)
	{
		strStation = pSec->m_pStation->m_sName;
	}
	//一次设备
	CString strDevice = "";
	if(pSec->m_pOwner != NULL)
	{
		strDevice = pSec->m_pOwner->m_sName;
	}
	// 所属调管单位 
	CString strManagedepartment = "";
	// 运维单位 
	CString strMaintaindepartment = "";
	if(pSec->m_pStation != NULL)
	{
		strManagedepartment = pSec->m_pStation->m_sManagedepartment;
		strMaintaindepartment = pSec->m_pStation->m_sMaintaindepartment;
	}
	CString strServiceDepartment = "";
	if(pSec->m_pStation != NULL)
	{
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		if(pData != NULL)
			strServiceDepartment = pData->GetServiceDepartmentName(pSec->m_pStation->m_nServiceDepartment);
	}

	//"设备名称","状态", "状态改变时间", "状态持续时间", "厂站","一次设备","所属调管单位","运维单位"
	CString strSQL;
	strSQL.Format("INSERT INTO Log VALUES (%d, '%s','%s','%s','%s','%s','%s','%s', '%s')",
		index, strManagedepartment,strMaintaindepartment, strStation, strDevice, pSec->m_sName, GetCommStatusString(pSec->m_nCommStatus), pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"), pSec->m_sCommDurativeTime);
	pDatabase->ExecuteSQL(strSQL);
}

/*************************************************************
 函 数 名：TranslateSingleKey()
 功能概要：解释单个关键字的意义
 返 回 值: 关键字表示的值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B820252
CString CViewPTComm::TranslateSingleKey( CString sKey )
{
	//判断数据有效性
	if(sKey == "")
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
		if(strHead != "$DEVICE_LIST")
		{
			//如果不是设备列表, 返回空
			return "";
		}
		CSecObj* pObj = (CSecObj*)m_List.GetItemData(nIndex);
		if(pObj == NULL)
			return "";
		//取得尾部字符
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "DEVICE_NAME$")
		{
			return pObj->m_sName; //设备名
		}
		if(strTail == "DEVICE_COMM$")
		{
			//通信状态
			return GetCommStatusString(pObj->m_nCommStatus);
		}
		if(strTail == "DEVICE_COMM_TIME$")
		{
			//通信状态改变时间
			return pObj->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S");
		}
		if(strTail == "DEVICE_RUN_TIME$")
		{
			//运行状态改变时间
			return pObj->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S");
		}
		if(strTail == "DEVICE_COMM_DURATIVE$")
		{
			//通信状态持续时间
			return pObj->m_sCommDurativeTime;
		}
		if(strTail == "DEVICE_RUN_DURATIVE$")
		{
			//运行状态持续时间
			return pObj->m_sRunDurativeTime;
		}
		if(strTail == "DEVICE_OWNER$")
		{
			//一次设备
			CString strDevice = "";
			if(pObj->m_pOwner != NULL)
			{
				strDevice = pObj->m_pOwner->m_sName;
			}
			return strDevice;
		}
		if(strTail == "DEVICE_STATION$")
		{
			//厂站名
			CString strStation = "";
			if(pObj->m_pStation != NULL)
			{
				strStation = pObj->m_pStation->m_sName;
			}
			return strStation;
		}
	}
	else
	{
		//判断是否有.GetCount()方法
		nFind = sKey.Find(".GetCount()", 0);
		if(nFind > -1)
		{
			//有.GetCount()方法
			if(sKey == "$DEVICE_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_List.GetItemCount());
				return sReturn;
			}
		}
		else
		{
			
		}
	}
	return "";
}

void CViewPTComm::OnPtcommRefresh() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = (CDataEngine*)pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//载入所有二次设备
	pData->LoadAllSec();
	//刷新所有设备的状态
	pData->RefreshDevStatus();
	//重新填充列表框
	FillListData();
}

/*************************************************************
 函 数 名: OnViewRefresh()
 功能概要: 响应页面强制刷新消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewPTComm::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	//重新载入二次设备状态统一放在外层做
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = (CDataEngine*)pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//载入所有二次设备
//	pData->LoadAllSec();
	//刷新所有设备的状态
//	pData->RefreshDevStatus();
	//重新填充列表框
	FillListData();
	WriteLog("CViewPTComm::OnViewRefresh, 强制刷新完毕", XJ_LOG_LV3);
}

CScrollBar* CViewPTComm::GetScrollBarCtrl(int nBar) const
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::GetScrollBarCtrl(nBar);
}
/*************************************************************
 函 数 名: InitComBox()
 功能概要: 载入combox列表信息 默认为第一项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewPTComm::InitComBox()
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





void CViewPTComm::OnSelchangeComboChoosestatioin() 
{
	// TODO: Add your control notification handler code here
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
	FillListData();
}

void CViewPTComm::OnCheckShowptcommon() 
{
	// TODO: Add your control notification handler code here
	g_ShowPTCommON = !g_ShowPTCommON;
	m_bShowPTCommON = !m_bShowPTCommON;
	//刷新列表信息
	FillListData();
}

void CViewPTComm::OnCheckShowptcommoff() 
{
	// TODO: Add your control notification handler code here
	g_ShowPTCommOFF = !g_ShowPTCommOFF;
	m_bShowPTCommOFF = !m_bShowPTCommOFF;
	//刷新列表信息
	FillListData();
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewPTComm::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	InitComBox();//重新载入厂站选择下拉框
	FillListData();
	WriteLog("StationInit,二次设备通讯状态页面刷新完毕", XJ_LOG_LV3);
}
