// ViewComm.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewComm.h"
#include "PrintListCtrl.h"
#include "GlobalFunc.h"
#include "DlgDetailStatus.h"
#include "MainFrm.h"

Record::Record(CStationObj* pObj)
{
	m_sName = pObj->m_sName;
	m_nCommStatus = pObj->m_nCommStatus;
	sTime.Format("%s", pObj->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S"));
	m_sDurativeTime = pObj->m_sDurativeTime;
	m_sReason = pObj->m_sNewStatusReason;
	m_sReasonTime.Format("%s", pObj->m_tmReasonTime.Format("%Y-%m-%d %H:%M:%S"));
	m_nCommStatus1 = pObj->m_nCommStatus1;
	sTime1.Format("%s", pObj->m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S"));
	m_sDurativeTime1 = pObj->m_sDurativeTime1;
	m_sReason1 = pObj->m_sNewStatusReason1;
	m_sReasonTime1.Format("%s", pObj->m_tmReasonTime1.Format("%Y-%m-%d %H:%M:%S"));
	m_sManufacturer = pObj->m_sManufacturer;
	m_nIPFlag = pObj->m_nIPFlag;
}

CommStatusList::CommStatusList()
{
	hItem = NULL;
	pStationObj = NULL;
}

UINT CommViewLoadData(LPVOID pParam)
{
	if(pParam == NULL)
		return -1;
	CViewComm* pView = (CViewComm*)pParam;
	//读取数据
	pView->LoadDetailDataFromDB();
	//填入数据
	pView->PostMessage(THREAD_FILL_DATA, 0, 0);
	pView->NotifyThreadEnd();
	return 0;
}

/** @brief           升降序作用位*/
int g_iCommViewAsc;

/////////////////////////////////////////////////////////////////////////////
// CViewComm

IMPLEMENT_DYNCREATE(CViewComm, CFormView)

//##ModelId=49B87B8503CA
CViewComm::CViewComm()
	: CFormView(CViewComm::IDD)
{
	//{{AFX_DATA_INIT(CViewComm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = 100;
	m_EventManager.ClearAllEvents();
	m_nOldSortCol = -1;
	m_bExitThread = FALSE;
	m_nTimer = 0;
	g_iCommViewAsc = -1;

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;

	m_nQueryTimer = 0;
	m_arrCall.RemoveAll();
	m_sResult = "";
	m_List.Create(WS_VISIBLE|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_SHOWSELALWAYS,
		CRect(10,10,200,200), this, 65, TVOXS_COLUMNHDR|TVOXS_ROWSEL|TVOXS_ITEMTIPS|TVOXS_PICKANYWHERE);
	pMenu = new CMenu;
	pMenu->CreatePopupMenu();
	m_pLoadDataThread = NULL;
	InitializeCriticalSection(&m_CriticalSection);
}

CViewComm::~CViewComm()
{
	DeleteCriticalSection(&m_CriticalSection);
	ClearStationPosMap();
	WriteLog("CViewComm::~CViewComm");
	if(pMenu != NULL)
	{
		pMenu->DestroyMenu();
		delete pMenu;
		pMenu = NULL;
	}
}


void CViewComm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewComm)
	DDX_Control(pDX, IDC_LIST_COMM, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewComm, CFormView)
	//{{AFX_MSG_MAP(CViewComm)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_STATION_HISTORY, OnStationHistory)
	ON_COMMAND(IDC_STATION_MAP, OnStationMap)
	ON_COMMAND(IDC_STATION_TREE, OnStationTree)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_COMM, OnColumnclickListComm)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMM, OnClickListComm)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_COMMAND(IDC_COMM_ON, OnCommOn)
	ON_COMMAND(IDC_COMM_OFF, OnCommOff)
	ON_COMMAND(IDC_COMM_EXPORT, OnCommExport)
	ON_COMMAND(IDC_COMM_PRINT, OnCommPrint)
	ON_COMMAND_RANGE(ITEM_1, ITEM_13, OnCommandItem)
	ON_WM_MEASUREITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(WM_STTP_20146, OnSTTP20146)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(COMMVIEW_QUERY_END, OnEndCallStationStatus)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(TEST_STATIONSTATUS, OnTestStationStatus)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnReFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewComm diagnostics

#ifdef _DEBUG


void CViewComm::AssertValid() const
{
	CFormView::AssertValid();
}


void CViewComm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewComm message handlers


void CViewComm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_STATIONCOMMU));
	//GetParentFrame()->SetWindowText("厂站通断");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CViewComm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	
	InitListCtrl();
	
	//开始刷新厂站持续时间定时器
	m_nTimer = SetTimer(1, g_RefreshStationTime*60*1000, 0);
}

void CViewComm::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
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
int CViewComm::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
	{
		return -1;
	}
	InitListStyle();

	LoadAllDataFromDB();
	FillListData();
	RefreshStatusDetail(); 
	
	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
int CViewComm::InitListStyle()
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
		int IDX = 0;
		for(int i = 0; i < m_arrColum.GetSize(); i++)
		{
			lvCol.iSubItem = i;
			lvCol.cx = m_arrColum[i].nItemWidth;
			CString sName = m_arrColum[i].sName;
			char * name = sName.GetBuffer(sName.GetLength());
			lvCol.pszText = name;
			m_List.InsertColumn(IDX++, &lvCol);
			if (m_arrColum[i].bHide)
			{
				m_List.SetColumnWidth(i, 0);
			}
		}

	}
	else
	{
		//载入配置失败
		//char* arColumn[9]={"厂站名", "状态","状态改变时间","状态持续时间","备通道状态","备通道状态改变时间","备通道状态持续时间","断开原因","通道测试"};		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 9; nCol++)
		{
			lvCol.iSubItem= nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 130; // 厂站名
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 1://
				lvCol.cx = 80; // 状态
				colName = StringFromID(IDS_COMMU_STATUS);
				break;
			case 2://
				lvCol.cx = 160; // 状态改变时间
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 3://
				lvCol.cx = 160; // 状态持续时间
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 4:
				lvCol.cx = 80; //备通道状态
				colName = StringFromID(IDS_STANDBYCHL_COMMU_STATUS);
				break;
			case 5:
				lvCol.cx = 160; //备通道状态改变时间
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 6:
				lvCol.cx = 160; //备通道状态持续时间
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 7:
				lvCol.cx = 150; 	//断开原因
				colName = StringFromID(IDS_DISCONNECT_REASON);
				break;
			case 8:
				lvCol.cx = 100;   //通道测试
				colName = StringFromID(IDS_COMMU_TEST);
				break;
			default:
				lvCol.cx=120;//列宽100象素
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol, &lvCol);
			SColumn	col;
			col.bHide = FALSE;
			col.nItemWidth = lvCol.cx;
			col.sName = lvCol.pszText;
			m_arrColum.Add(col);
		}
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	return 0;
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
BOOL CViewComm::LoadListConfig()
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
			WriteLog("ViewComm::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("ViewComm::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewCommConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewCommConfig");
				WriteLog("ViewComm::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewCommConfig下的配置");
					WriteLog("ViewComm::读取UIConfig失败,将使用默认列风格");
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


BOOL CViewComm::SaveListConfig()
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
			WriteLog("ViewComm::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewComm::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewCommConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewCommConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewCommConfig");
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
						WriteLog("未找到ViewCommConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了ViewCommConfig下的配置, 先删除新建所有子节点");
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
int CViewComm::FillListData()
{
	WriteLog("CViewComm::FillListData, start");
	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();

	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	CString strStation = _T("");
	Record* pRecord = NULL;
	
	while (pos != NULL)
	{
		m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
		MYASSERT_CONTINUE(pList);
		if(SetStationInfo(pList))
		{

		}
	}

	//恢复刷新
	m_List.SetRedraw(TRUE);
	WriteLog("CViewComm::FillListData, end");
	return 0;
}


/*************************************************************
 函 数 名：RegulateControlSize()
 功能概要：调整控件位置
 返 回 值: 返回值说明
**************************************************************/
void CViewComm::RegulateControlSize()
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


void CViewComm::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::OnPrepareDC(pDC, pInfo);
}

BOOL CViewComm::IsSelected(const CObject* pDocItem) const
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::IsSelected(pDocItem);
}

void CViewComm::OnDestroy() 
{
	//关闭定时器
	KillTimer(m_nTimer);
	
	//保存UI配置
	SaveListConfig();
	
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名：AddStationToList()
 功能概要：加入厂站状态信息到列表
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数：param1   要加入的厂站指针
**************************************************************/
BOOL CViewComm::AddStationToList( CStationObj* pStation)
{
	WriteLog("CViewComm::AddStationToList start", XJ_LOG_LV3);
	//事件为空, 返回FALSE
    MYASSERT_BOOL(pStation);
	MYASSERT_BOOL(m_List.GetSafeHwnd());
	//刷新持续时间
	pStation->RefreshDurativeTime();
	
	//"厂站名", "状态","状态改变时间","状态持续时间","状态改变原因","备通道状态","备通道状态改变时间","备通道状态持续时间","备通道状态改变原因","所属厂家信息"
	CommStatusList* pList = NULL;
    m_mapStationPos.Lookup(pStation->m_sName, (void*&)pList);

	MYASSERT_BOOL(pList);
	Record* pRecord = pList->recordList.GetTail();
	MYASSERT_BOOL(pRecord);
	
	AddRecordToList(pList);
	
	SetStationInfo(pStation, pList->hItem, FALSE);
	
	POSITION pos = pList->recordList.GetTailPosition();
	if(pList->hItem != NULL && pos != NULL)
		m_List.SetItemData(pList->hItem, (DWORD)pos);

	return TRUE;
}

BOOL CViewComm::InsertStationInfo(CommStatusList* pList, Record* pRecord, BOOL bShow, POSITION pos)
{
	MYASSERT_BOOL(pList);
	MYASSERT_BOOL(pRecord);
	if (bShow || pList->recordList.GetCount() == 1)
	{
		pList->hItem = m_List.InsertItem("");
		SetStationInfo(pRecord, pList->hItem, FALSE);
		m_List.SetItemData(pList->hItem, (DWORD)pos);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CViewComm::InsertStationInfo(CommStatusList* pList, Record* pRecord, BOOL bShow, int nLastRecordStatus, POSITION pos)
{
	MYASSERT(pList);
	MYASSERT(pRecord);
	if (bShow && GetStatus(pRecord) != nLastRecordStatus)
	{
		HTREEITEM hChildItem = m_List.InsertItem("", pList->hItem, TVI_LAST);
		SetStationInfo(pRecord, hChildItem, TRUE);
		m_List.SetItemData(hChildItem, (DWORD)pos);
	}
}

void CViewComm::SetStationInfo(CStationObj* pStation, HTREEITEM hItem, BOOL bChildItem)
{
	BOOL result = TRUE;
    CString sTime = "";
    CString sReason= "";
	int IDX = 0;
	if (!bChildItem)
	{
		
		//1:厂站名称
		CString sID;
		result = m_List.SetItemText(hItem, pStation->m_sName, IDX);
	}
	
	++IDX;
	//2:状态
	result = m_List.SetItemText(hItem, pStation->m_nCommStatus == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((pStation->m_nCommStatus==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)), IDX);
	if (pStation->m_nCommStatus == 0)
	{
		m_List.SetItemColor(hItem, RGB(255, 0, 0), IDX);
	}

	++IDX;
	//3:状态开始时间
	sTime.Format("%s", pStation->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S"));
	result = m_List.SetItemText(hItem, sTime, IDX);
	
	++IDX;
	//4:状态持续时间
	result = m_List.SetItemText(hItem, pStation->m_sDurativeTime, IDX);
	
	
	++IDX;
	//5:备通道状态
	result = m_List.SetItemText(hItem, pStation->m_nCommStatus1 == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((pStation->m_nCommStatus1==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)), IDX);
	if (pStation->m_nCommStatus1 == 0)
	{
		m_List.SetItemColor(hItem, RGB(255, 0, 0), IDX);
	}
	
	
	++IDX;
	//6:备通道状态开始时间
	if(pStation->m_nCommStatus1 != 0 && pStation->m_nCommStatus1 != 1)
		sTime = "";
	else
		sTime.Format("%s", pStation->m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S"));
	result = m_List.SetItemText(hItem, sTime, IDX);
	
	++IDX;
	//7:备通道状态持续时间
	result = m_List.SetItemText(hItem, pStation->m_sDurativeTime1, IDX);
	
	++IDX;
	result = m_List.SetItemText(hItem, GetStatus(pStation) != 3 ? StringFromID(IDS_DISCONNECT_REASON) : "" , IDX);
	m_List.SetItemColor(hItem, RGB(0, 0, 255), IDX);
	

	if (!bChildItem)
	{
		++IDX;
		//9:检测正确性
		result = m_List.SetItemText(hItem, StringFromID(IDS_COMMU_TEST), IDX);
		m_List.SetItemColor(hItem, RGB(0, 0, 255), IDX);
	}
	//设置数据
	if (g_style == 1)
	{
		m_List.SetTextBkColor(g_ListSpaceColor2);
	}
	else if(g_style == 2)
	{
		m_List.SetTextBkColor(g_ListSpaceColor3);
	}
	else if(g_style == 3)
	{
		m_List.SetTextBkColor(g_ListSpaceColor4);
	}
	else
	{
		m_List.SetTextBkColor(RGB(237, 254, 253));
	}
}

void CViewComm::SetStationInfo(Record* record, HTREEITEM hItem, BOOL bChildItem)
{
	BOOL result = TRUE;
    CString sTime = "";
    CString sReason= "";
	int IDX = 0;
	
	if (!bChildItem)
	{
		//1:厂站名称
		CString sID;
		result = m_List.SetItemText(hItem, record->m_sName, IDX);
	}
	
	++IDX;
	//2:状态
	result = m_List.SetItemText(hItem, record->m_nCommStatus == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((record->m_nCommStatus==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)), IDX);
	if (record->m_nCommStatus == 0)
	{
		m_List.SetItemColor(hItem, RGB(255, 0, 0), IDX);
	}
	
	++IDX;
	//3:状态开始时间
	result = m_List.SetItemText(hItem, record->sTime, IDX);
	
	++IDX;
	//4:状态持续时间
	result = m_List.SetItemText(hItem, record->m_sDurativeTime, IDX);
	
	++IDX;
	//5:备通道状态
	result = m_List.SetItemText(hItem, record->m_nCommStatus1 == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((record->m_nCommStatus1==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON)), IDX);
	if (record->m_nCommStatus1 == 0)
	{
		m_List.SetItemColor(hItem, RGB(255, 0, 0), IDX);
	}
	
	++IDX;
	//6:备通道状态开始时间
	result = m_List.SetItemText(hItem, record->sTime1, IDX);
	
	++IDX;
	//7:备通道状态持续时间
	result = m_List.SetItemText(hItem, record->m_sDurativeTime1, IDX);
	
	++IDX;
	//8:断开原因
	result = m_List.SetItemText(hItem, GetStatus(record) != 3 ? StringFromID(IDS_DISCONNECT_REASON) : "", IDX);
	m_List.SetItemColor(hItem, RGB(0, 0, 255), IDX);
	
	if (!bChildItem)
	{
		//9:检测正确性
		++IDX;
		result = m_List.SetItemText(hItem, StringFromID(IDS_COMMU_TEST), IDX);
		m_List.SetItemColor(hItem, RGB(0, 0, 255), IDX);
	}
	//设置数据s
	//m_List.SetTextBkColor(RGB(237, 254, 253));
	if (g_style == 1)
	{
		m_List.SetTextBkColor(g_ListSpaceColor2);
	}
	else if(g_style == 2)
	{
		m_List.SetTextBkColor(g_ListSpaceColor3);
	}
	else if(g_style == 3)
	{
		m_List.SetTextBkColor(g_ListSpaceColor4);
	}
	else
	{
		m_List.SetTextBkColor(RGB(237, 254, 253));
	}
}

BOOL CViewComm::SetStationInfo(CommStatusList* pList)
{
	MYASSERT_BOOL(pList);
	POSITION pos = pList->recordList.GetTailPosition();
	BOOL bSuccess = FALSE;
	int nTemp = 0;
	Record* pRecord = NULL;
	while (!bSuccess && pos != NULL)
	{
		pRecord = pList->recordList.GetAt(pos);
		MYASSERT_CONTINUE(pRecord);
		//第一条是最新状态，必须显示
		bSuccess = InsertStationInfo(pList, pRecord, TRUE, pos);
		nTemp = GetStatus(pRecord);
		pList->recordList.GetPrev(pos);
	}

	while (pos != NULL)
	{
		pRecord = pList->recordList.GetAt(pos);
		InsertStationInfo(pList, pRecord, (GetStatus(pRecord) == 3 ? g_ShowCommON : g_ShowCommOFF), nTemp, pos);
		GetStatus(pRecord, &nTemp);
		pList->recordList.GetPrev(pos);
	}
	if(!bSuccess)
		pList->hItem = NULL;
	return bSuccess;
}


/*************************************************************
 函 数 名：OnSTTP20144()
 功能概要：响应主子站通讯状态变化(20144)报文
 返 回 值: void 
 参    数：param1	消息参数
		   Param2	报文内容
**************************************************************/
void CViewComm::OnSTTP20144( WPARAM wParam, LPARAM lParam )
{
//	KillTimer(1);
	WriteLog("CViewComm::OnSTTP20144 start", XJ_LOG_LV3);
	
	CStationObj* pObj = (CStationObj*)lParam;
	if(!pObj)
		return;
	//查找厂站
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	if(m_pLoadDataThread)
		return;

	CommStatusList* pList = NULL;
	m_mapStationPos.Lookup(pObj->m_sName, (void*&)pList);
	//改变LIST中的值
	AddStationToList(pObj);

	//刷新显示
	FillListData();
    //SetTimer(1, g_RefreshStationTime*60*1000, 0);
	WriteLog("CViewComm::OnSTTP20144 neat end", XJ_LOG_LV3);
}


void CViewComm::OnContextMenu(CWnd* pWnd, CPoint point) 
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
				TRACE("CListCtrlEx::OnContextMenu -- 你右击了列头\n");
				if(pMenu != NULL)
				{
					//先清理刚才的菜单
					if (pMenu != NULL)
					{
						int nCount = pMenu->GetMenuItemCount();
						for(int i = 0; i < nCount; i++)
						{
							pMenu->DeleteMenu(0, MF_BYPOSITION);
						}
					}
					//加入题头还分隔
					pMenu->AppendMenu(MF_STRING | MF_DISABLED, ITEM_TITLE, StringFromID(IDS_SELECT_SHOW_COLUMN));
					pMenu->AppendMenu(MF_SEPARATOR, ITEM_SEPARATOR, "");
					//加入各个列名
					int nColCount = m_arrColum.GetSize();
					for(int i = 0; i < nColCount; i++)
					{
						SColumn  sc = m_arrColum.GetAt(i);
						if(sc.bHide)
						{//隐藏的
							pMenu->AppendMenu(MF_STRING | MF_UNCHECKED, ITEM_1 + i, sc.sName);
						}
						else
						{
							pMenu->AppendMenu(MF_STRING | MF_CHECKED, ITEM_1 + i, sc.sName);
						}
					}
					
					pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
				}
			}
		}
		else if(pChildWnd != NULL && pChildWnd->GetSafeHwnd() == m_List.GetSafeHwnd())
		{//点击的是ListCtrl的非列头部分
			//生成菜单
			CMenu Menu, *pMenu;
			if(!Menu.LoadMenu(IDR_MENU_STATION))
				return;
			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount == 1)
			{
				//只选择了一个
				pMenu->EnableMenuItem(IDC_STATION_MAP, MF_ENABLED);
				pMenu->EnableMenuItem(IDC_STATION_TREE, MF_ENABLED);
			}

			if(1 == g_ShowCommON)
			{
				//显示通信状态为正常的装置信息
				pMenu->CheckMenuItem(IDC_COMM_ON, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_COMM_ON, MF_UNCHECKED);
			}
			if(1 == g_ShowCommOFF)
			{
				//显示通信状态为断开的装置信息
				pMenu->CheckMenuItem(IDC_COMM_OFF, MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem(IDC_COMM_OFF, MF_UNCHECKED);
			}

			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

void CViewComm::OnStationHistory() 
{
	// TODO: Add your command handler code here
	//准备打开查询与统计的参数
	//主子站通讯状态查询: 1$$$厂站ID$$$通讯状态$$$自动查询(0-不自动查询, 1-自动查询)
	CString sSQL = "";
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
	{
		sSQL = "1$$$ $$$99$$$0";
	}
	else if(nCount == 1)
	{
		HTREEITEM hItem = m_List.GetSelectedItem();
		if(hItem == NULL)
			return;
		
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		DEVICE_LIST listDevice;
		listDevice.RemoveAll();
		pData->GetStationList(listDevice);
		POSITION pos = NULL;
		for(pos = listDevice.GetHeadPosition(); pos != NULL;)
		{
			CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
			if (!pStation->m_sName.CompareNoCase(m_List.GetItemText(hItem)))
			{
				//
				sSQL.Format("1$$$%s$$$99$$$1", pStation->m_sID);
				break;
			}		
		}
	}
	//打开查询与统计
	OpenXJQuery(sSQL, this->GetSafeHwnd());
}

void CViewComm::OnStationMap() 
{
	// TODO: Add your command handler code here
	//只处理选择了一条事件的情况
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	HTREEITEM hItem = m_List.GetSelectedItem();
	if(hItem == NULL)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = NULL;
	for(pos = listDevice.GetHeadPosition(); pos != NULL;)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if (!pStation->m_sName.CompareNoCase(m_List.GetItemText(hItem)))
		{
			pApp->ShowDevice(pStation);
			break;
		}		
	}
}

void CViewComm::OnStationTree() 
{
	// TODO: Add your command handler code here
	//只处理选择了一条事件的情况
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	HTREEITEM hItem = m_List.GetSelectedItem();
	if(hItem == NULL)
		return;
    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = NULL;
	for(pos = listDevice.GetHeadPosition(); pos != NULL;)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if (!pStation->m_sName.CompareNoCase(m_List.GetItemText(hItem)))
		{
			pApp->LocateInTree(pStation);
			break;
		}
		
	}
}


void CViewComm::OnColumnclickListComm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	return;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iCommViewAsc = - g_iCommViewAsc;
	}
	else
	{
		g_iCommViewAsc = 1;
		m_nOldSortCol = iCol;
	}

	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名：RefreshDuravieTime()
 功能概要：刷新LIST中指定位置的ITEM的持续时间显示
 返 回 值: void
 参    数：param1	指定位置索引
		   Param2
**************************************************************/
void CViewComm::RefreshDuravieTime(HTREEITEM hItem, CStationObj* pStationObj)
{
	//总数
	MYASSERT(hItem);
	CommStatusList* pCommStatusList = NULL;
	if (pStationObj == NULL)
	{
		//得到厂站
		CString strStationName = m_List.GetItemText(hItem);
		m_mapStationPos.Lookup(strStationName, (void*&)pCommStatusList);
		if (pCommStatusList != NULL)
		{
			pStationObj = pCommStatusList->pStationObj;
		}
		else
		{
			return;
		}
	}

	POSITION pos = NULL;
	Record* pRecord = NULL;
	MYASSERT(pStationObj);
	//刷新持续时间
	pStationObj->RefreshDurativeTime();


	if(pStationObj->m_nCommStatus != -1)
	{
		//更改显示
		m_List.SetItemText(hItem, pStationObj->m_sDurativeTime, 3);
	}
	if(pStationObj->m_nCommStatus1 != -1)
	{
		m_List.SetItemText(hItem, pStationObj->m_sDurativeTime1, 6);
	}
	
	//刷新
	int iIndex = m_List.GetItemIndex(hItem);
	m_List.Update(iIndex);
}


BOOL CViewComm::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CFormView::DestroyWindow();
}

void CViewComm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CFormView::OnClose();
}

void CViewComm::OnClickListComm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;
	HTREEITEM hItem = m_List.GetItemFromIndex(nItem);
	if(nItem != -1)
	{
		int IDX = m_List.GetColumnsCount();
		if(nSubItem == IDX - 3 && (m_List.GetItemText(hItem, IDX - 3) != ""))
		{
			POSITION pos = NULL;
			pos = (POSITION)m_List.GetItemData(hItem);
			CommStatusList* pList = NULL;
			CString strStationName = _T("");
		
			strStationName = m_List.GetItemText(hItem);

			if (strStationName == _T(""))
			{
				HTREEITEM hParentItem = m_List.GetParentItem(hItem);
				strStationName = m_List.GetItemText(hParentItem);
			}

			m_mapStationPos.Lookup(strStationName, (void*&)pList);
			MYASSERT(pList);
			MYASSERT(pos);
			DlgDetailStatus detailStatusDlg(pList, pos);
			detailStatusDlg.DoModal();
		}

		if(nSubItem == IDX - 2 && (m_List.GetItemText(hItem, IDX - 2) != ""))
		{
			//得到该行对应的厂站
			CommStatusList* pList = NULL;
			//得到厂站
			CString strStationName = m_List.GetItemText(hItem);
			m_mapStationPos.Lookup(strStationName, (void*&)pList);
            MYASSERT(pList);
			MYASSERT(pList->pStationObj);
			TestStationStatus(pList->pStationObj);
		}
	}
	*pResult = 0;
}


BOOL CViewComm::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_COMM);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
}

void CViewComm::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//刷新厂站持续时间
		RefreshTime();
	}
	if(nIDEvent == m_nQueryTimer)
	{
		if(m_arrCall.GetSize() > 0)
		{
			StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
			if(pCall != NULL)
			{
				pCall->nRelust = -3;
			}
		}
		SendMessage(COMMVIEW_QUERY_END);
	}
	CFormView::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：RefreshTime()
 功能概要：刷新所有厂站的持续时间
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CViewComm::RefreshTime()
{
	WriteLog("CViewComm::RefreshTime,定时刷新开始", XJ_LOG_LV3);
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);

	if(m_List.GetSafeHwnd() == NULL)
		return;

	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	HTREEITEM hItem = NULL;
	CString strStationName = _T("");
	
	//遍历厂站map
	while (pos != NULL)
	{
		m_mapStationPos.GetNextAssoc(pos, strStationName, (void*&)pList);
		MYASSERT(pList);
		RefreshDuravieTime(pList->hItem, NULL);
	}
	//恢复刷新
	m_List.SetRedraw(TRUE);
	WriteLog("CViewComm::RefreshTime,定时刷新结束", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：IsNeedShow()
 功能概要：判断当前通信状态的厂站信息是否需要显示
 返 回 值: 需要显示返回TRUE, 不需要显示返回FALSE
 参    数：param1	通信状态
		   Param2
**************************************************************/
BOOL CViewComm::IsNeedShow( int nStatus )
{
	BOOL bReturn = FALSE;
	switch(nStatus)
	{
	case 0: //断开
		if(1 == g_ShowCommOFF)
			bReturn = TRUE;
		else
			bReturn = FALSE;
		break;
	case 1: //正常
		if(1 == g_ShowCommON)
			bReturn = TRUE;
		else 
			bReturn = FALSE;
		break;
	default:
		bReturn = FALSE;
		break;
	}
	return bReturn;
}

void CViewComm::OnCommOn() 
{
	// TODO: Add your command handler code here
	//改变通信状态正常是否要显示的设置
	if(g_ShowCommON == 0)
		g_ShowCommON = 1;
	else if(g_ShowCommON == 1)
		g_ShowCommON = 0;
	//重新填充数据
	SetListData();
}

void CViewComm::OnCommOff() 
{
	// TODO: Add your command handler code here
	//改变通信状态断开是否要显示的设置
	if(g_ShowCommOFF == 0)
		g_ShowCommOFF = 1;
	else if(g_ShowCommOFF == 1)
		g_ShowCommOFF = 0;
	//重新填充数据
	SetListData();
}

void CViewComm::OnCommExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("%s%s.xls", StringFromID(IDS_TOOLBAR_STATIONCOMMU),t.Format("%Y%m%d%H%M%S"));
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

void CViewComm::OnCommPrint() 
{
	// TODO: Add your command handler code here
	/*CPrintListCtrl print;
	print.SetListCtrl(m_List.GetSafeHwnd());
	
	// 调整每列的位置
	print.ResizeColumnsToFitContent(TRUE);
	
	// 是否裁减每行的长度 FALSE 否， TRUE 是
	print.TruncateColumn(TRUE);
	
	// 打印列之间的分割线
	print.PrintColumnSeparator(TRUE);
	
	// 打印行之间的分割线
	print.PrintLineSeparator(TRUE);
	
	// 打印表的名称
	print.PrintHead(TRUE);
	
	// 设置各种字体
	print.SetHeadFont("黑体", 14);
	print.SetFootFont("宋体", 12);
	print.SetListFont("宋体", 12);
	
	// 设置纸的走向 TRUE,横向；FALSE 纵向
	print.SetPrinterOrientation(FALSE);
	print.Print(GetSafeHwnd(), "主子站通讯状态", FALSE);*/

	//得到系统下载路径
	CString strDir = STATIONCOMM_LIST;
	//组建全路径
	if(g_DoubleChannel == 1)
		strDir = STATIONCOMM_LIST_DOUBLE;

	//打印
	InitCOM();
	//打开模板文件
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, strDir);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),strDir);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//设置回调函数
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInStationComm);
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
 函 数 名：WriteResultToExcel()
 功能概要：将列表中的数据导出到Excel
 返 回 值: 导出成功返回TRUE, 失败返回FALSE
 参    数：param1	Excel文件保存位置
		   Param2
**************************************************************/
BOOL CViewComm::WriteResultToExcel( CString strFullPath )
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
				//"序号", "厂站名", "状态","状态改变时间","状态持续时间","状态改变原因","备通道状态","备通道状态改变时间","备通道状态持续时间","备通道状态改变原因","所属厂家信息"
				strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT,  \"%s\" TEXT, \"s\" TEXT, \"s\" TEXT, \"s\" TEXT, \"s\" TEXT, \"s\" TEXT, \"s\" TEXT, \"s\" TEXT)",
					StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_COMMU_STATUS), StringFromID(IDS_STATUS_CHANGE_TIME),StringFromID(IDS_COMMON_DURATION),
					StringFromID(IDS_DISCONNECT_REASON), StringFromID(IDS_STANDBYCHL_COMMU_STATUS),StringFromID(IDS_STATUS_CHANGE_TIME),StringFromID(IDS_COMMON_DURATION),
					StringFromID(IDS_DISCONNECT_REASON), StringFromID(IDS_MODEL_MANUFACTURER));
				database.ExecuteSQL(strSQL);
			}

			POSITION pos = m_mapStationPos.GetStartPosition();
			CommStatusList* pList = NULL;
			CString strStation = _T("");
			
			while (pos != NULL)
			{
				m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
				MYASSERT_CONTINUE(pList);
				if(pList->pStationObj != NULL)
					AddResultToExcel(&database, pList->pStationObj);
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
void CViewComm::AddResultToExcel( CDatabase* pDatabase, CStationObj* pStation )
{
	//检查数据有效性
	MYASSERT(pDatabase);
	MYASSERT(pStation);

	CString sReason = "";
	if(pStation->m_nCommStatus == 0)
	{
		//断开
		sReason = pStation->m_sNewStatusReason;
	}

	//6:备通道状态
	CString sStatus = "";
	if(pStation->m_nCommStatus1 != 0 && pStation->m_nCommStatus1 != 1)
		sStatus = "";
	else
		sStatus = pStation->m_nCommStatus1==0?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON);

	//7:备通道状态开始时间
	CString sTime = "";
	if(pStation->m_nCommStatus1 != 0 && pStation->m_nCommStatus1 != 1)
		sTime = "";
	else
		sTime.Format("%s", pStation->m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S"));

	//9:状态改变原因
	//只在状态为断开时显示值
	CString sReason1= "";
	if(pStation->m_nCommStatus1 == 0)
	{
		sReason1 = pStation->m_sNewStatusReason1;
	}

	//"厂站名", "状态","状态改变时间","状态持续时间","状态改变原因","备通道状态","备通道状态改变时间","备通道状态持续时间","备通道状态改变原因","所属厂家信息"
	CString strSQL;
	strSQL.Format("INSERT INTO Log VALUES ('%s','%s','%s','%s','%s', '%s','%s','%s','%s', '%s')",
		pStation->m_sName, (pStation->m_nCommStatus==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON), pStation->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S"), pStation->m_sDurativeTime, sReason, sStatus, sTime, pStation->m_sDurativeTime1, sReason1,  pStation->m_sManufacturer);
	pDatabase->ExecuteSQL(strSQL);
}

/*************************************************************
 函 数 名：InitCOM()
 功能概要：初始化打印模板COM组件
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
BOOL CViewComm::InitCOM()
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
void CViewComm::ReleaseCOM()
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
 函 数 名：TranslateSingleKey()
 功能概要：解释单个关键字的意义
 返 回 值: 关键字表示的值
 参    数：param1	关键字
		   Param2
**************************************************************/
CString CViewComm::TranslateSingleKey( CString sKey )
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
		int nCount = 0;

		POSITION pos = m_mapStationPos.GetStartPosition();
		CommStatusList* pList = NULL;
		CString strStation = _T("");
		
		CStationObj* pObj = NULL;
		while (pos != NULL)
		{
			m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
			MYASSERT_CONTINUE(pList);
			if(nCount == nIndex)
			{
				pObj = pList->pStationObj;
			}
			nCount++;
		}
		
        //HTREEITEM hItem = m_List.GetItemFromIndex(nIndex);
		//CStationObj* pObj = (CStationObj*)m_List.GetItemData(hItem);
		if(pObj == NULL)
			return "";
		//取得尾部字符
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "DEVICE_NAME$")
		{
			return pObj->m_sName; //厂站名
		}
		if(strTail == "DEVICE_COMM$")
		{
			//通信状态
			return (pObj->m_nCommStatus==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON);
		}
		if(strTail == "DEVICE_COMM_TIME$")
		{
			//状态改变时间
			return pObj->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S");
		}
		if(strTail == "DEVICE_COMM_DURATIVE$")
		{
			//状态持续时间
			return pObj->m_sDurativeTime;
		}
		if(strTail == "DEVICE_COMM_REASON$")
		{
			//状态改变原因
			return pObj->m_sNewStatusReason;
		}
		if(strTail == "DEVICE_COMM_RESERVE$")
		{
			//通信状态
			return pObj->m_nCommStatus1 == -1 ? StringFromID(IDS_COMMON_UNUSED) : ((pObj->m_nCommStatus1==0)?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
		}
		if(strTail == "DEVICE_COMM_TIME_RESERVE$")
		{
			CString sTime;
			//状态改变时间
			if(pObj->m_nCommStatus1 != 0 && pObj->m_nCommStatus1 != 1)
				return "";
			else
				sTime.Format("%s", pObj->m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S"));
			return sTime;
		}
		if(strTail == "DEVICE_COMM_DURATIVE_RESERVE$")
		{
			//状态持续时间
			return pObj->m_sDurativeTime1;
		}
		if(strTail == "DEVICE_COMM_REASON_RESERVE$")
		{
			//状态改变原因
			return pObj->m_sNewStatusReason1;
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
				sReturn.Format("%d", m_mapStationPos.GetCount());
				return sReturn;
			}
		}
		else
		{
			
		}
	}
	return "";
}

/*************************************************************
 函 数 名: OnViewRefresh()
 功能概要: 响应页面强制刷新消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewComm::OnViewRefresh( WPARAM wParma, LPARAM lParam )
{
	/*CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = (CDataEngine*)pApp->GetDataEngine();
	
	pData->QueryStationStatus();
	ClearStationPosMap();
	//重新填充列表框
	FillListData();*/
	RefreshStatusDetail();
	WriteLog("CViewComm::OnViewRefresh, 强制刷新完毕", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名: QueryStationStatus()
 功能概要: 查询厂站通讯状态
 返 回 值: int 失败返回<0的整数，成功返回0 
 参    数: param1 指定厂站
		   Param2 指定通道
**************************************************************/
int CViewComm::QueryStationStatus( CStationObj* pObj, int nChannel /*= 0*/ )
{
	//检测正确性
	MYASSERT_INT(pObj);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	//组织20145报文
	if(!pApp->m_SttpEngine.BuildDataFor20145StationStatus(sttpData, pObj->m_sID, nChannel))
	{
		//组织报文失败
		if(m_arrCall.GetSize() > 0)
		{
			StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
			if(pCall != NULL)
			{
				pCall->nRelust = -2;
			}
		}
		SendMessage(COMMVIEW_QUERY_END);
		return -1;
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
			str.Format("厂站%s发送召唤状态报文失败,原因为连接中断", pObj->m_sID);
			WriteLog(str);
			if(m_arrCall.GetSize() > 0)
			{
				StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
				if(pCall != NULL)
				{
					pCall->nRelust = -2;
				}
			}
			SendMessage(COMMVIEW_QUERY_END);
			return -1;
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("厂站%s发送召唤状态报文失败,原因为缓存已满", pObj->m_sID);
			WriteLog(str);
			if(m_arrCall.GetSize() > 0)
			{
				StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
				if(pCall != NULL)
				{
					pCall->nRelust = -2;
				}
			}
			SendMessage(COMMVIEW_QUERY_END);
			return -1;
		}
	}
	else
	{
		//启动定时器
		CDataEngine * pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nDevGeneralTimeOut;
		m_nQueryTimer = SetTimer(2, nTimeOut*1000, 0);
	}
	return 0;
}

/*************************************************************
 函 数 名: OnSTTP20146()
 功能概要: 响应20146(厂站通讯状态下载通知)
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewComm::OnSTTP20146( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewComm::OnSTTP20146");
	if(m_arrCall.GetSize() <= 0)
	{
		//没有在召唤厂站通讯状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20146报文
	if(pSttpData->sttp_head.uMsgID != 20146)
	{
		//错误报文
		CString str;
		str.Format("CViewComm::OnSTTP20146 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}

	StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
	if(pCall != NULL)
	{
		pCall->nRelust = 0;
	}
	else
		return;
	MYASSERT(pCall->pStation); 
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_station_id;
	if(strID != pCall->pStation->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CViewComm::OnSTTP20146 收到其它厂站报文, 厂站ID为%s", strID);
		WriteLog(str);
		return;
	}

	int nChannel = pSttpData->sttp_body.nFlag;
	if(nChannel != pCall->nChannel)
	{
		//不是本通道的报文
		CString str;
		str.Format("CViewComm::OnSTTP20146 收到其它通道的报文, 通道为%d", nChannel);
		WriteLog(str);
		return;
	}
	
	//关闭定时器
	KillTimer(m_nQueryTimer);
	
	//处理报文
	
	int nStatus = pSttpData->sttp_body.nStatus;
	int nReason = pSttpData->sttp_body.nEventType;

	/*
	CString str;
	str.Format("nChannel:%d   nStatus:%d  pCall->pStation->m_nCommStatus:%d", nChannel, nStatus, pCall->pStation->m_nCommStatus);
    WriteLog(str);
	*/

	if ((nChannel == 0 && nStatus != pCall->pStation->m_nCommStatus) || (nChannel == 1 && nStatus != pCall->pStation->m_nCommStatus1))
	{
		CString strStationTime = pSttpData->sttp_body.ch_time_12_BIT1;
		CString strReasonTime = pSttpData->sttp_body.ch_time_12_BIT2;
		CTime tmStation;
		CTime tmReason;
		if (strStationTime == "" || strReasonTime == "")
		{
			//处理完毕
			//SendMessage(COMMVIEW_QUERY_END);
			// 如果时间为空,还需设置通讯状态，此处略
			WriteLog("CViewComm::OnSTTP20146, 收到时间为空");
			tmStation =  CTime::GetCurrentTime();
			tmReason =  CTime::GetCurrentTime();
		}
		else
		{
			tmStation = StringToTimeSttp12(strStationTime);
			tmReason = StringToTimeSttp12(strReasonTime);
		}
		pCall->pStation->SetStatus(nStatus, tmStation, nReason, nChannel, tmReason, pCall->pStation->m_nIPFlag);
		
		AddStationToList(pCall->pStation);

		//改变LIST中的值
		//刷新显示
		FillListData();
		//通知设备树和图形界面
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CMainFrame* pFrame=(CMainFrame*)pApp->GetMainWnd();
		if(pFrame != NULL)
		{
			WriteLog("CViewComm::OnSTTP20146, 状态变位，发送通知");
			pFrame->PostMessage(STATION_CHANGED, 0, (LPARAM)pCall->pStation);
		}
	}
	else
	{
		WriteLog("CViewComm::OnSTTP20146, 状态未变位，不发送通知");
	}
	
	//处理完毕
	SendMessage(COMMVIEW_QUERY_END);
}

/*************************************************************
 函 数 名: OnSTTP20069()
 功能概要: 响应20069(召唤失败通知)
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewComm::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//当前没有在召唤
	if(m_arrCall.GetSize() <= 0)
	{
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
	if(pSttpData->sttp_head.uMsgID != 20069)
	{
		//错误报文
		return;
	}
	
	StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
	if(pCall != NULL)
	{
		pCall->nRelust = -2;
	}
	else
		return;

	if(pCall->pStation == NULL)
		return;
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != pCall->pStation->m_sID)
	{
		//不是本装置的报文
		return;
	}
	
	//关闭定时器
	KillTimer(m_nQueryTimer);
	
	//处理报文
	int nStatus = pSttpData->sttp_body.nStatus;
	if(0)
	{
		//操作成功
	}
	else
	{
		//操作失败
		CString str;
		str.Format("厂站 %s 召唤通讯状态失败!", pCall->pStation->m_sID);
		WriteLog(str);
	}
	
	//处理完毕
	SendMessage(COMMVIEW_QUERY_END);
}

/*************************************************************
 函 数 名: ClearCallList()
 功能概要: 清空召唤列表
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewComm::ClearCallList()
{
	for(int i = 0; i < m_arrCall.GetSize(); i++)
	{
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(i);
		MYASSERT_CONTINUE(pCall);
		MYDELETE(pCall);
	}
	m_arrCall.RemoveAll();
}

/*************************************************************
 函 数 名: AddToCallList()
 功能概要: 将指定厂站指定通道加入到待召唤列表
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定通道
**************************************************************/
void CViewComm::AddToCallList( CStationObj* pObj, int nChannel )
{
	MYASSERT(pObj);
	StationStauts_Call* pNew = new StationStauts_Call;
	pNew->pStation = pObj;
	pNew->nChannel = nChannel;
	m_arrCall.Add(pNew);
}

/*************************************************************
 函 数 名: StartCall()
 功能概要: 对召唤项进行召唤
 返 回 值: 
 参    数: param1 指定召唤项
		   Param2 
**************************************************************/
void CViewComm::StartCall( StationStauts_Call* pCall)
{
	MYASSERT(pCall);
	if(QueryStationStatus(pCall->pStation, pCall->nChannel) < 0)
	{
		//进行查询失败
		pCall->nRelust = -1;
	}
	else
	{
		//进行查询成功
		//查找厂站在列表中的位置
		CommStatusList* pCommStatusList = NULL;
		m_mapStationPos.Lookup(pCall->pStation->m_sName, (void*&)pCommStatusList);
		if(pCommStatusList != NULL && pCommStatusList->hItem != NULL)
		{
			//改变文字
			int IDX = m_List.GetColumnsCount();
			m_List.SetItemText(pCommStatusList->hItem, StringFromID(IDS_CALL_CALLING), IDX - 2);
			int iIndex = m_List.GetItemIndex(pCommStatusList->hItem);
			m_List.Update(iIndex);
		}
	}
}

/*************************************************************
 函 数 名: OnEndCallStationStatus()
 功能概要: 响应召唤完成消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewComm::OnEndCallStationStatus( WPARAM wParam, LPARAM lParam )
{
	//取得列表中的第一个对象
	if(m_arrCall.GetSize() <= 0)
	{
		return;
	}
	StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
	MYASSERT(pCall);
	CStationObj* pStation = pCall->pStation;
	MYASSERT(pStation);
	//召唤结果
	CString str;
	if(pCall->nRelust == 0)
	{
		if(pCall->nChannel == 0)
			str.Format("%s:%s, %s:%d, %s:%s\n", 
			StringFromID(IDS_MODEL_SUBSTATION),pCall->pStation->m_sName, 
			StringFromID(IDS_COMMU_CHANNEL),pCall->nChannel, 
			StringFromID(IDS_COMMU_STATUS),pCall->pStation->m_nCommStatus==0?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
		else if(pCall->nChannel == 1)
			str.Format("%s:%s, %s:%d, %s:%s\n", 
			StringFromID(IDS_MODEL_SUBSTATION),pCall->pStation->m_sName, 
			StringFromID(IDS_COMMU_CHANNEL),pCall->nChannel, 
			StringFromID(IDS_COMMU_STATUS), pCall->pStation->m_nCommStatus1==0?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
	}
	else if(pCall->nRelust == -1)
	{
		str.Format("%s:%s, %s:%d, %s\n", 
			StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, 
			StringFromID(IDS_COMMU_CHANNEL), pCall->nChannel, StringFromID(IDS_CALLFAIL_SEND));
	}
	else if(pCall->nRelust == -2)
	{
		str.Format("%s:%s, %s:%d, %s\n", 
			StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, 
			StringFromID(IDS_COMMU_CHANNEL), pCall->nChannel, StringFromID(IDS_CALLFAIL_RESPONSE));
	}
	else if(pCall->nRelust == -3)
	{
		str.Format("%s:%s, %s:%d, %s\n", 
			StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, 
			StringFromID(IDS_COMMU_CHANNEL), pCall->nChannel, StringFromID(IDS_CALLFAIL_TIMEOUT));
	}
	m_sResult += str;
	m_arrCall.RemoveAt(0);
	
	//判断是否召唤全部结束
	if(m_arrCall.GetSize() > 0)
	{
		//继续召唤
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
		StartCall(pCall);
	}
	else
	{
		//召唤结束
		//进行查询成功
		//查找厂站在列表中的位置
		CommStatusList* pCommStatusList = NULL;
		m_mapStationPos.Lookup(pCall->pStation->m_sName, (void*&)pCommStatusList);
		if(pCommStatusList != NULL && pCommStatusList->hItem != NULL)
		{
			//改变文字
			int IDX = m_List.GetColumnsCount();
			m_List.SetItemText(pCommStatusList->hItem, StringFromID(IDS_COMMU_TEST), IDX - 2);
			int iIndex = m_List.GetItemIndex(pCommStatusList->hItem);
			m_List.Update(iIndex);
		}
		AfxMessageBox(m_sResult);
	}
	//删除第一项
	MYDELETE(pCall);
}


/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,排序时被调用
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数：param1	比较对象1
		   Param2	比较对象2
		   param3	比较参数
**************************************************************/
int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//取得比较数据
	HTREEITEM hItem1 = (HTREEITEM) lParam1;
	HTREEITEM hItem2 = (HTREEITEM) lParam2;
	CStationObj* pObj1 = (CStationObj*)pApp->m_pComm->m_List.GetItemData(hItem1);
    CStationObj* pObj2 = (CStationObj*)pApp->m_pComm->m_List.GetItemData(hItem2);
	if(pObj1 == NULL || pObj2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	if (nCol == 0 || nCol == 4)
	{
		return 0;
	}

	CString str1, str2;
	int iResult = 0;
	//"厂站名", "状态","状态改变时间","状态持续时间"
	switch(nCol)
	{
	case 0: //厂站名
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
		str1 = pObj1->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S");
		str2 = pObj2->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //状态持续时间
		iResult = pObj1->m_lDurative - pObj2->m_lDurative;
		break;
	case 4: //状态改变原因
		str1 = pObj1->m_sNewStatusReason;
		str2 = pObj2->m_sNewStatusReason;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iCommViewAsc;
	return iResult;
}

/*************************************************************
 函 数 名：SetListData()
 功能概要：设置列表数据
 返 回 值: 无
**************************************************************/
void CViewComm::SetListData()
{
	/*m_List.DeleteAllItems();
	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	CString strStationName = _T("");

	//遍历厂站map
	while (pos != NULL)
	{
		m_mapStationPos.GetNextAssoc(pos, strStationName, (void*&)pList);
		SetStationInfo(pList);
	}*/
	FillListData();
	//RefreshStatusDetail();
}

/*************************************************************
 函 数 名：ClearStationPosMap()
 功能概要：清空StationPosMap
 返 回 值: 无
**************************************************************/
void CViewComm::ClearStationPosMap()
{
	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	CString strStation = _T("");
	Record* pRecord = NULL;
	
	while (pos != NULL)
	{
		m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
		ClearRecord(pList);
		MYDELETE(pList);
	}
	m_mapStationPos.RemoveAll();
}


void CViewComm::OnCommandItem(UINT nID)
{
	if(nID < ITEM_1 || nID > ITEM_13)
	{
		return;
	}
	
	int nPos = nID - (int)ITEM_1; //列数,从0开始

	m_arrColum[nPos].bHide = !m_arrColum[nPos].bHide;
	//改变列的显性
	if(m_arrColum[nPos].bHide)
	{		//是隐藏
		m_List.SetColumnWidth(nPos, 0);
	}
	else 
	{		//是显示
		m_List.SetColumnWidth(nPos, m_arrColum[nPos].nItemWidth);
	}
	SetListData();
}


void CViewComm::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDCtl == IDC_LIST_COMM)
	{
		lpMeasureItemStruct->itemHeight = 25;
	}
	
	CFormView::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CViewComm::ReadStatusData(CStationObj* pStationObj, CommStatusList** pList)
{
	MYASSERT(pStationObj);

	if(m_bExitThread)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//通过Station_ID查询最新一次值(Status, Time, Reason)
	
	//设置字段
	Field Field0;
	pApp->m_DBEngine.SetField(sql, Field0, "10", EX_STTP_DATA_TYPE_TOP);
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Status", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Time", EX_STTP_DATA_TYPE_TIME);
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "Reason", EX_STTP_DATA_TYPE_STRING);
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "Reserve1", EX_STTP_DATA_TYPE_STRING);
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "Reserve2", EX_STTP_DATA_TYPE_STRING);
	
	//设置条件
	Condition Condition0;
	CString str;
	str.Format("STATION_ID = '%s'", pStationObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, Condition0, str);
	
	Condition Condition1;
	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC, id desc", EX_STTP_WHERE_ABNORMALITY);
	
	//strSQL.Format("select Status, Time, Reason, Reserve1, Reserve2  from tb_pt_station_status where station_id = '%s' and Reserve2 > '%s' order by Reserve2", pStationObj->m_sID, strTime);
	//WriteLog(strSQL);
	
	//进行查询
	
	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	if(m_bExitThread)
		return;

	//CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_STATUS_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLog("CViewComm::ReadDetailReason, 查询失败");
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CViewComm::ReadDetailReason, 厂站[%s]读取到%d条数据", pStationObj->m_sID, nCount);
		WriteLog(str);
		
		CString strStatus;
		CString strTime;
		CString strReason;
		CString strChannel;
		CString strReasonTime;
		if(nCount > 0)
		{
			for(int k = 0; k < nCount - 1; k++)
			{
				if(m_bExitThread)
					return;
				pMemset.MoveNext();
			}
			for(int i = 0; i < nCount; i++)
			{
				if(m_bExitThread)
					return;
				strStatus = pMemset.GetValue((UINT)0);
				strTime = pMemset.GetValue(1);
				strReason = pMemset.GetValue(2);
				strChannel = pMemset.GetValue(3);
				strReasonTime = pMemset.GetValue(4);
				
				//pStationObj->SetStatus(atoi(strStatus), StringToTime(strTime), atoi(strReason), atoi(strChannel), StringToTime(strReasonTime), pStationObj->m_nIPFlag);
				AddRecordToList(pStationObj, pList);
				//pMemset->MoveNext();
				pMemset.MoveForward();
			}
		}

		if (0 == nCount)
		{
			CString strLog;
			strLog.Format("CViewComm::ReadStatusData, 未找到%s通断记录", pStationObj->m_sID);
			WriteLog(strLog);
			//LoadStatusFromDB(pStationObj, pList);
			AddRecordToList(pStationObj, pList);
			//AddRecordToList(pObj, pList);
		}
	}
	else
	{
		CString str;
		str.Format("CViewComm::ReadDetailReason, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
}


void CViewComm::ClearRecord(CStationObj* pStationObj, BOOL bClearAll)
{
	MYASSERT(pStationObj);
	CommStatusList* pList = NULL;
	m_mapStationPos.Lookup(pStationObj->m_sName, (void*&)pList);
	MYASSERT(pList);
	ClearRecord(pList);
	if (bClearAll)
	{
		MYDELETE(pList);
		m_mapStationPos.RemoveKey(pStationObj->m_sName);
	}
}


void CViewComm::ClearRecord(CommStatusList* pList)
{
	MYASSERT(pList);
	Record* pRecord = NULL;
	POSITION posList = pList->recordList.GetHeadPosition();
	while(posList != NULL)
	{
		pRecord = pList->recordList.GetNext(posList);
		MYDELETE(pRecord);
	}
	pList->recordList.RemoveAll();
}

BOOL CViewComm::AddRecordToList(CStationObj* pStationObj, CommStatusList** pList)
{
	MYASSERT_BOOL(pStationObj);	
	m_mapStationPos.Lookup(pStationObj->m_sName, (void*&)(*pList));
	if (*pList == NULL)
	{
        *pList = new CommStatusList;
		(*pList)->pStationObj = pStationObj;
		m_mapStationPos.SetAt(pStationObj->m_sName, *pList);
	}
	
	return AddRecordToList(*pList);
}

BOOL CViewComm::AddRecordToList(CommStatusList* pList)
{	
	MYASSERT_BOOL(pList);
	MYASSERT_BOOL(pList->pStationObj);
	
	Record* pRecord = new Record(pList->pStationObj);
	pList->recordList.AddTail(pRecord);
	while(pList->recordList.GetCount() > 10 )
	{
		Record* pRecord = pList->recordList.GetHead();
		pList->recordList.RemoveHead();
		MYDELETE(pRecord);
	}
	return TRUE;
}


int CViewComm::GetStatus(Record* pRecord, int* nCut)
{
	MYASSERT_BOOL(nCut);
	*nCut = GetStatus(pRecord);
	return *nCut;
}


int CViewComm::GetStatus(Record* pRecord)
{
	MYASSERT_BOOL(pRecord);
	return GetStatus(pRecord->m_nCommStatus, pRecord->m_nCommStatus1, pRecord->m_nIPFlag);
}

int CViewComm::GetStatus(CStationObj* pStationObj)
{
	MYASSERT_BOOL(pStationObj);
	return GetStatus(pStationObj->m_nCommStatus, pStationObj->m_nCommStatus1, pStationObj->m_nIPFlag);
}


int CViewComm::GetStatus(int nCommStatus, int nCommStatus1, int nIPFlag)
{
	int nReturn = 0;
	if(nIPFlag == 0)
	{
		if(nCommStatus != 1)
			nReturn = 0;
		else
			nReturn = 3;
	}
	else
	{
		if(nCommStatus != 1 && nCommStatus1 != 1)
			nReturn = 0;
		if(nCommStatus == 1 && nCommStatus1 != 1)
			return 1;
		if(nCommStatus != 1 && nCommStatus1 == 1)
			return 2;
		if(nCommStatus == 1 && nCommStatus1 == 1)
			return 3;
	}
	return nReturn;
}



/*************************************************************
 函 数 名：LoadStatusFromDB()
 功能概要：从硬盘数据库读入子站与主站通讯状态,只查询最新一次
 返 回 值: void 状态
 参    数：param1	通道
**************************************************************/
//##ModelId=49B87BA6038A
void CViewComm::LoadStatusFromDB(CStationObj* pObj, CommStatusList** pList)
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//通过Station_ID查询最新一次值(Status, Time, Reason)

	//设置字段
	Field Field0;
	pApp->m_DBEngine.SetField(sql, Field0, "1", EX_STTP_DATA_TYPE_TOP);
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Status", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Time", EX_STTP_DATA_TYPE_TIME);
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "Reason", EX_STTP_DATA_TYPE_STRING);
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "Reserve1", EX_STTP_DATA_TYPE_STRING);
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "Reserve2", EX_STTP_DATA_TYPE_STRING);

	//设置条件
	Condition Condition0;
	CString str;
	str.Format("STATION_ID = '%s'", pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, Condition0, str);

	Condition Condition1;
	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC, id desc", EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet pMemset;
//	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_STATUS_CFG, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewComm::LoadStatusFromDB, 查询失败");
		delete[] sError;
//		delete pMemset;
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();

		CString strStatus;
		CString strTime;
		CString strReason;
		CString strChannel;
		CString strReasonTime;			
		for(int i = 0; i < nCount; i++)
		{
			strStatus = pMemset.GetValue((UINT)0);
			strTime = pMemset.GetValue(1);
			strReason = pMemset.GetValue(2);
			strChannel = pMemset.GetValue(3);
			strReasonTime = pMemset.GetValue(4);
			
			pObj->SetStatus(atoi(strStatus), StringToTime(strTime), atoi(strReason), atoi(strChannel), StringToTime(strReasonTime), pObj->m_nIPFlag);
			AddRecordToList(pObj, pList);
			pMemset.MoveNext();		
		}
		if (0 == nCount)
		{
			str.Format("CViewComm::LoadStatusFromDB, 未找到%s的最新通断记录", pObj->m_sID);
			WriteLog(str);
			AddRecordToList(pObj, pList);
		}
	}
	else
	{
		str.Format("CViewComm::LoadStatusFromDB, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
//	delete pMemset;
	sError = NULL;
//	pMemset = NULL;
}

void CViewComm::RefreshStatusDetail()
{
	ClearStationPosMap();

	StartThread();
}

BOOL CViewComm::LoadAllDataFromDB()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	//组建查询条件
	CString str = "";
	CString strSQL;
	strSQL.Format("SELECT A.Status, A.Time, A.Reason, A.Reserve1, A.Reserve2, A.Station_ID  FROM tb_pt_station_status A JOIN (SELECT station_id,Reserve1,MAX(Reserve2) Reserve2 FROM tb_pt_station_status  GROUP BY station_id,Reserve1) B ON A.station_id = B.station_id AND A.Reserve2 = B.Reserve2");
	//strSQL.Format("SELECT A.Status, A.Time, A.Reason, A.Reserve1, A.Reserve2, A.Station_ID  FROM tb_pt_station_status A JOIN (SELECT station_id,Reserve1,MAX(Reserve2) Reserve2 FROM tb_pt_station_status  GROUP BY station_id,Reserve1) B ON A.station_id = B.station_id AND A.Reserve2 = B.Reserve2");
	WriteLog(strSQL);
	
	//进行查询
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_FULL;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	CMemSet pMemset;
	//	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJExecuteSql(MutiSql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewComm::LoadAllDataFromDB, 查询失败");
		delete[] sError;
		//		delete pMemset;
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();

		str.Format("CViewComm::LoadAllDataFromDB, 查询到%d条数据", nCount);
		WriteLog(str);
		
		CString strStatus;
		CString strTime;
		CString strReason;
		CString strChannel;
		CString strReasonTime;			
		for(int i = 0; i < nCount; i++)
		{
			strStatus = pMemset.GetValue((UINT)0);
			strTime = pMemset.GetValue(1);
			strReason = pMemset.GetValue(2);
			strChannel = pMemset.GetValue(3);
			strReasonTime = pMemset.GetValue(4);
			CString strID = pMemset.GetValue(5);
			CStationObj* pObj = (CStationObj*)pData->FindDevice(strID, TYPE_STATION);
			if(pObj != NULL)
			{
				pObj->SetStatus(atoi(strStatus), StringToTime(strTime), atoi(strReason), atoi(strChannel), StringToTime(strReasonTime), pObj->m_nIPFlag);
				CommStatusList* pList = NULL;
				AddRecordToList(pObj, &pList);
			}
			pMemset.MoveNext();		
		}
	}
	else
	{
		str.Format("CViewComm::LoadAllDataFromDB, 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	//	delete pMemset;
	sError = NULL;
//	pMemset = NULL;
	return TRUE;
}

void CViewComm::LoadDetailDataFromDB() 
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = (CDataEngine*)pApp->GetDataEngine();
	if(pData == NULL)
		return;

	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	
	EnterCriticalSection(&m_CriticalSection);
	WriteLog("CViewComm::LoadDetailDataFromDB, 取得锁控制权");

	while(pos != NULL)
	{
		if(m_bExitThread)
		{
			LeaveCriticalSection(&m_CriticalSection);
			WriteLog("CViewComm::LoadDetailDataFromDB, 释放锁控制权");
			return;
		}
		Sleep(10);
		//取得厂站指针
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		//读取数据
		MYASSERT_CONTINUE(pObj);
		CommStatusList* pList = NULL;
		ReadStatusData(pObj, &pList);
	}
	LeaveCriticalSection(&m_CriticalSection);
	WriteLog("CViewComm::LoadDetailDataFromDB, 释放锁控制权");
}

/****************************************************
Date:2013/6/4  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewComm::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	return; //2015/3/10 for test

	ClearStationPosMap();
	LoadAllDataFromDB();
	FillListData();
	RefreshStatusDetail();
	WriteLog("StationInit,厂站通讯状态刷新完毕", XJ_LOG_LV3);
}

/****************************************************
Date:2013/10/8  Author:LYH
函数名:   TestStationStatus	
返回值:   void	
功能概要: 
参数: CStationObj * pStation	
*****************************************************/
void CViewComm::TestStationStatus( CStationObj* pStation )
{
	if(!pStation)
		return;

	int IDX = m_List.GetColumnsCount();
	//查找厂站在列表中的位置
	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	CString strStation = _T("");
				
	while (pos != NULL)
	{
		m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
		MYASSERT_CONTINUE(pList);
		if(pList->pStationObj == pStation)
			break;
		pList = NULL;
	}
	if(pList == NULL)
		return;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_arrCall.GetSize() > 0)
	{
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
		if(pCall->pStation != pStation)
		{
			AfxMessageBox(StringFromID(IDS_TESTSTATION_BUSY));
		}
		else
		{
			//取消召唤
			if(AfxMessageBox(StringFromID(IDS_TESTSTATION_CANCEL), MB_YESNOCANCEL)==IDYES)
			{
				//申请报文数据容器
				STTP_FULL_DATA sttpData;
				//停止定时器
				KillTimer(m_nQueryTimer);
				//组建取消报文
				pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20145, pStation->m_sID, 0);
				//发送报文
				pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
				//清空召唤列表
				ClearCallList();
				
				
				//改变文字
				if(pList != NULL)
				{
					int IDX = m_List.GetColumnsCount();
					m_List.SetItemText(pList->hItem, StringFromID(IDS_COMMU_TEST), IDX - 2);
					m_List.Update(m_List.GetItemIndex(pList->hItem));
				}
				
			}
		}
		return;
	}
	
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
	
	m_sResult = "";
	m_List.SetItemText(pList->hItem, StringFromID(IDS_BTN_CANCEL_CALL), IDX - 2);
	//查询厂站状态
	if(pList->pStationObj->m_nIPFlag == 0) //单通道
	{
		AddToCallList(pList->pStationObj, 0);
	}
	else //双通道
	{
		AddToCallList(pList->pStationObj, 0);
		AddToCallList(pList->pStationObj, 1);
	}
	
	//开始召唤
	if(m_arrCall.GetSize() > 0)
	{
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
		if(pCall != NULL)
		{
			StartCall(pCall);
		}
	}
}

/****************************************************
Date:2013/10/8  Author:LYH
函数名:   OnTestStationStatus	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewComm::OnTestStationStatus( WPARAM wParam, LPARAM lParam )
{
	if(lParam == NULL)
		return;
	CStationObj* pStation = (CStationObj*)lParam;
	TestStationStatus(pStation);
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   StartThread	
返回值:   void	
功能概要: 
*****************************************************/
void CViewComm::StartThread()
{
	EndThread();
	m_bExitThread = FALSE;
	m_pLoadDataThread = AfxBeginThread(CommViewLoadData,this,THREAD_PRIORITY_BELOW_NORMAL);
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   EndThread	
返回值:   void	
功能概要: 
*****************************************************/
void CViewComm::EndThread()
{
	if(!m_pLoadDataThread)
		return;
	m_bExitThread = TRUE;
	DWORD dw = WaitForSingleObject(m_pLoadDataThread->m_hThread, INFINITE);
	switch(dw)
	{
	case WAIT_TIMEOUT:
	case WAIT_FAILED:
		TerminateThread(m_pLoadDataThread->m_hThread,NULL);
		WriteLog("接收报文线程无法正常退出,强制结束", XJ_LOG_LV3);
		break;
	}
	m_pLoadDataThread = NULL;
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   NotifyThreadEnd	
返回值:   void	
功能概要: 
*****************************************************/
void CViewComm::NotifyThreadEnd()
{
	m_pLoadDataThread = NULL;
	m_bExitThread = FALSE;
	WriteLog("CViewComm::NotifyThreadEnd 线程结束", XJ_LOG_LV3);
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   OnAllClose	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewComm::OnAllClose( WPARAM wParam, LPARAM lParam )
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
void CViewComm::OnReFillData( WPARAM wParam, LPARAM lParam )
{
	FillListData_Multi();
}

/****************************************************
Date:2014/1/3  Author:LYH
函数名:   FillListData_Multi	
返回值:   int	
功能概要: 
*****************************************************/
int CViewComm::FillListData_Multi()
{
	if(m_bExitThread)
		return 0;
	WriteLog("CViewComm::FillListData_Multi, start");
	EnterCriticalSection(&m_CriticalSection);
	WriteLog("CViewComm::FillListData_Multi, 取得锁控制权");
	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	
	POSITION pos = m_mapStationPos.GetStartPosition();
	CommStatusList* pList = NULL;
	CString strStation = _T("");
	Record* pRecord = NULL;
	
	while (pos != NULL)
	{
		if(m_bExitThread)
		{
			break;
		}
		m_mapStationPos.GetNextAssoc(pos, strStation, (void*&)pList);
		MYASSERT_CONTINUE(pList);
		if(SetStationInfo(pList))
		{
			
		}
		//	AddStationToList(pList->pStationObj);
	}
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
	LeaveCriticalSection(&m_CriticalSection);
	WriteLog("CViewComm::FillListData_Multi, 释放锁控制权");
	WriteLog("CViewComm::FillListData_Multi, end");
	return 0;
}
