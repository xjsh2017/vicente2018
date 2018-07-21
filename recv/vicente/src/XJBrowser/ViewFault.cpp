// ViewFault.cpp : implementation file
//
#include "ExcelOp.h"
#include "stdafx.h"
#include "XJBrowser.h"
#include "ViewFault.h"
#include "DlgFaultReport.h"
#include "GlobalFunc.h"
#include "ManualFaultShorten.h"
#include "RemindObj.h"

//#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           升降序作用位*/
int g_iFaultViewAsc;

/////////////////////////////////////////////////////////////////////////////
// CViewFault

IMPLEMENT_DYNCREATE(CViewFault, CFormView)

//##ModelId=49B87B84033E
CViewFault::CViewFault()
	: CFormView(CViewFault::IDD)
{
	//{{AFX_DATA_INIT(CViewFault)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_nViewType = XJ_VIEW_FAULT;
	m_listFaultReport.RemoveAll();
	m_nMaxRow = 100;
	/** @brief           之前被排序的列*/
	m_nOldSortCol = -1;

	m_bOnlyShowNoOverhual = FALSE;
	m_bOnlyShowSaved = FALSE;
	m_bOnlyShowTypical = FALSE;
}

//##ModelId=49B87B85005D
CViewFault::~CViewFault()
{
	Clear();
	TRACE("CViewFault::~CViewFault \n");
}

//##ModelId=49B87B85002F
void CViewFault::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewFault)
	DDX_Control(pDX, IDC_LIST_FAULT_NEW, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewFault, CFormView)
	//{{AFX_MSG_MAP(CViewFault)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FAULT_NEW, OnItemchangedListFaultNew)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FAULT_NEW, OnColumnclickListFaultNew)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FAULT_NEW, OnClickListFaultNew)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_FAULT_PRINT, OnFaultPrint)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_FAULT_EDIT, OnFaultEdit)
	ON_COMMAND(IDC_FAULT_WEB, OnFaultWeb)
	ON_COMMAND(IDC_FAULT_REFRESH, OnFaultRefresh)
	ON_COMMAND(IDC_FAULT_CREATE, OnFaultCreate)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_FAULT_NEW, OnCustomDraw)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDC_FAULT_DELETE, OnFaultDelete)
	ON_COMMAND(IDC_FAULT_EXPORT, OnFaultExport)
	ON_COMMAND(ID_MANUAL_FAULTSHORTENN, OnManualFaultShorten)
	ON_COMMAND(IDC_FAULT_ONLYSHOWOVERHAUL, OnFaultOnlyshowoverhaul)
	ON_COMMAND(IDC_FAULT_ONLYSHOWSAVED, OnFaultOnlyshowsaved)
	ON_COMMAND(IDC_FAULT_ONLYSHOWTYPICAL, OnFaultOnlyshowtypical)
	//}}AFX_MSG_MAP
	ON_MESSAGE(FAULTREPORT_EDITED, OnFaultReportEdited)
	ON_MESSAGE(WM_STTP_20151, OnSTTP20151)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewFault diagnostics

#ifdef _DEBUG
//##ModelId=49B87B85006D
void CViewFault::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B85007D
void CViewFault::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewFault message handlers

//##ModelId=49B87B85004E
void CViewFault::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_FAULTREPORT));
//	GetParentFrame()->SetWindowText("事故报告");
	//CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	//pWnd->SendMessage(WM_PAINTMYCAPTION, NULL, NULL);
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B850024
void CViewFault::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	LoadConfig();

	//初始化控件
	InitListCtrl();
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B84038A
int CViewFault::InitListCtrl()
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
//##ModelId=49B87B84038B
int CViewFault::InitListStyle()
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

			if ( sName == StringFromID(IDS_FAULTREPORT_ISTRUEFALUT))
			{
				g_Temp = 1;
			}

			m_List.InsertColumn(i,&lvCol, bHide);
		}
	}
	else
	{
		//载入配置失败
		//char* arColumn[14]={"序号", "事故包名称","创建时间","故障时间","故障元件","厂站1","厂站2","故障类型","录波测距","是否检修", "是否已存档","典型故障","详细原因",""};
		
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
				lvCol.cx = 120; //是否检修
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
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8403A9
BOOL CViewFault::LoadListConfig()
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
				WriteLog("ViewFault::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewFaultConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewFaultConfig");
				WriteLog("ViewFault::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewFaultConfig下的配置");
					WriteLog("ViewFault::读取UIConfig失败,将使用默认列风格");
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

//##ModelId=49B87B8403B9
BOOL CViewFault::SaveListConfig()
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
			WriteLog("ViewFault::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewFault::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewFaultConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewFaultConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewFaultConfig");
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
						WriteLog("未找到ViewFaultConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了ViewFaultConfig下的配置, 先删除新建所有子节点");
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
//##ModelId=49B87B840399
int CViewFault::FillListData()
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
		AddReportToList(pReport, nIndex);
		nIndex++;
	}
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B8403C8
BOOL CViewFault::NeedSave()
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
//##ModelId=49B87B8403C9
void CViewFault::RegulateControlSize()
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

//##ModelId=49B87B85008C
void CViewFault::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	//调整控件位置
	RegulateControlSize();
}

//##ModelId=49B87B85009C
void CViewFault::OnDestroy() 
{
	SaveConfig();
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：LoadDataFromDB()
 功能概要：从数据库中载入数据
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数：param1	要载入的记录数
		   Param2
**************************************************************/
//##ModelId=49B87B8403D8
BOOL CViewFault::LoadDataFromDB( int nNum )
{
	//清除旧数据
	Clear();

	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//查询最近的nNum个记录
	CString str;		
		
	//top
	Field Field1;
	str.Format("%d", nNum);
	pApp->m_DBEngine.SetField(sql, Field1, str, EX_STTP_DATA_TYPE_TOP);
	//条件

	if(m_bOnlyShowNoOverhual)
	{
		Condition condition2;
		CString strFiter = "overhaul_flag = 0";
		pApp->m_DBEngine.SetCondition(sql, condition2, strFiter);
	}
	if(m_bOnlyShowSaved)
	{
		Condition condition3;
		CString strFiter = "reverse2 > 0";
		pApp->m_DBEngine.SetCondition(sql, condition3, strFiter);
	}
	if(m_bOnlyShowTypical)
	{
		Condition condition4;
		CString strFiter = "typicalcase = 1";
		pApp->m_DBEngine.SetCondition(sql, condition4, strFiter);
	}

	if(!pApp->MgrAllStations())
	{
		Condition condition6;
		CString strFilter;
		strFilter.Format("((station1_id in(select station_id from tb_user_related where user_id='%s')))",pApp->m_User.m_strUSER_ID,pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql,condition6,strFilter);
	}

	Condition Condition1;
	str.Format("order by ID DESC");
	pApp->m_DBEngine.SetCondition(sql, Condition1, str, EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewFault::LoadDataFromDB, 查询失败");
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
			if(!pReport->LoadFromDB(pMemset))
			{
				delete pReport;
				pReport = NULL;
			}
			else
			{
				//加入到链表
				m_listFaultReport.AddTail(pReport);
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CViewFault::LoadDataFromDB,查询失败,原因为%s", sError);
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
 函 数 名：Clear()
 功能概要：清除之前的故障报告信息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8403DA
void CViewFault::Clear()
{
	POSITION pos = m_listFaultReport.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultReport* pReport = (CFaultReport*)m_listFaultReport.GetNext(pos);
		if(pReport != NULL)
			delete pReport;
	}
	m_listFaultReport.RemoveAll();
}

/*************************************************************
 函 数 名：AddReportToList()
 功能概要：加入指定的故障报告到列表框中显示
 返 回 值: void
 参    数：param1	指定故障报告
		   Param2	指定位置
**************************************************************/
//##ModelId=49B87B850000
void CViewFault::AddReportToList( CFaultReport* pReport, int nIndex )
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

//##ModelId=49B87B85009E
void CViewFault::OnItemchangedListFaultNew(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//AfxMessageBox("item change");
	
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
//##ModelId=49B87B84037A
int CALLBACK CViewFault::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
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
	iResult *= g_iFaultViewAsc;
	return iResult;
}

/*************************************************************
 函 数 名：OnColumnclickListDi()
 功能概要：响应点击列头事件, 对点击的列进行排序
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8500AB
void CViewFault::OnColumnclickListFaultNew(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	AfxMessageBox("响应点击列头事件, 对点击的列进行排序");
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iFaultViewAsc = - g_iFaultViewAsc;
	}
	else
	{
		g_iFaultViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnClickListFaultNew()
 功能概要：响应点击列表事件, 当选择为空时发送消息给详细窗口
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8500BB
void CViewFault::OnClickListFaultNew(NMHDR* pNMHDR, LRESULT* pResult) 
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
		return;
	}
	if (nSubItem == 9)
	{ 
		CFaultReport* pFaultReport = (CFaultReport*)m_List.GetItemData(nSelected);
		CString str;
		if (pFaultReport == NULL)
		{
			return;
		}
		str.Format("%s[%s]?",StringFromID(IDS_TIP_FAULTREPORT_SWITCH),(pFaultReport->m_nOverhaul == 0 ? StringFromID(IDS_OVERHUAL_YES):StringFromID(IDS_OVERHUAL_NO)));
		if (AfxMessageBox(str, MB_YESNO) == IDYES)
		{			
			if (pFaultReport->m_nOverhaul == 1)
			{
				pFaultReport->m_nOverhaul = 0;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_OVERHUAL_NO));
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
				pFaultReport->m_nOverhaul = 1;
				if (pFaultReport->SaveToDB())
				{
					m_List.SetItemText(nSelected, nSubItem, StringFromID(IDS_OVERHUAL_YES));
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
	if (nSubItem == 10)
	{ 
		CFaultReport* pFaultReport = (CFaultReport*)m_List.GetItemData(nSelected);
		CString str;
		if (pFaultReport == NULL)
		{
			return;
		}
		str.Format("%s[%s]?", StringFromID(IDS_TIP_FAULTREPORT_SWITCH),(pFaultReport->m_bIsSaved == 0 ? StringFromID(IDS_ARCHIVE_YES):StringFromID(IDS_ARCHIVE_NO)));
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
		str.Format("%s[%s]?",(pFaultReport->m_nTypicalcase == 0 ? StringFromID(IDS_TYPICAL_YES):StringFromID(IDS_TYPICAL_NO)));
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

//##ModelId=49B87B8500BF
void CViewFault::OnContextMenu(CWnd* pWnd, CPoint point) 
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
				pMenu->EnableMenuItem(IDC_FAULT_EXPORT, MF_ENABLED);
			}
			pMenu->EnableMenuItem(IDC_FAULT_DELETE, MF_ENABLED);
			pMenu->EnableMenuItem(IDC_FAULT_REFRESH, MF_ENABLED);
			pMenu->EnableMenuItem(IDC_FAULT_CREATE, MF_ENABLED);
			pMenu->EnableMenuItem(ID_MANUAL_FAULTSHORTENN, MF_ENABLED);

			pMenu->EnableMenuItem(IDC_FAULT_ONLYSHOWOVERHAUL, MF_ENABLED);
			pMenu->CheckMenuItem(IDC_FAULT_ONLYSHOWOVERHAUL, m_bOnlyShowNoOverhual?MF_CHECKED:MF_UNCHECKED);

			pMenu->EnableMenuItem(IDC_FAULT_ONLYSHOWSAVED, MF_ENABLED);
			pMenu->CheckMenuItem(IDC_FAULT_ONLYSHOWSAVED, m_bOnlyShowSaved?MF_CHECKED:MF_UNCHECKED);

			pMenu->EnableMenuItem(IDC_FAULT_ONLYSHOWTYPICAL, MF_ENABLED);
			pMenu->CheckMenuItem(IDC_FAULT_ONLYSHOWTYPICAL, m_bOnlyShowTypical?MF_CHECKED:MF_UNCHECKED);
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87B8500CB
void CViewFault::OnFaultPrint() 
{
	// TODO: Add your command handler code here
	//父窗口是分割窗口
	CWnd * pWnd = GetParent()->GetParent()->GetParent();
	if(pWnd == NULL)
		return;
	//向父窗口发送消息
	pWnd ->PostMessage(FAULTREPORT_PRINT, 0, 0);
}

//##ModelId=49B87B8500CD
BOOL CViewFault::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_FAULT_NEW);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CFormView::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B8500D0
void CViewFault::OnFaultEdit() 
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

/*************************************************************
 函 数 名：OnFaultReportChanged()
 功能概要：响应故障报告信息被编辑消息
 返 回 值: void
 参    数：param1
		   Param2	故障报告指针
**************************************************************/
void CViewFault::OnFaultReportEdited( WPARAM wParam, LPARAM lParam )
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
 函 数 名：ChangedValueInList()
 功能概要：改变指定故障报告在列表中的显示
 返 回 值: 
 参    数：param1	指定故障报告
		   Param2	指定索引
**************************************************************/
//##ModelId=49B87B850003
void CViewFault::ChangedValueInList( CFaultReport* pReport, int nIndex /*= -1*/ )
{
	//检查数据有效性
	MYASSERT(pReport);
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

/*************************************************************
 函 数 名：OnSTTP20151()
 功能概要：响应20151,自动归档信息包变化通知
 返 回 值: void
 参    数：param1
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8500E0
void CViewFault::OnSTTP20151( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewFault::OnSTTP20151 start");

	CFaultReport* pParam = (CFaultReport*)lParam;
	if(!pParam)
		return;

	//判断是新事件还是旧事件变更
	if(pParam->m_nRportType == 1 || (pParam->m_nRportType == 4 && !m_bOnlyShowNoOverhual))
	{
		//新信息包
		//创建新事件
		CFaultReport* pReport = pParam->Clone();
		if(!pReport)
			return;
		if(pParam->m_nRportType == 4)
			pReport->m_nOverhaul = 1;
		//加入到链表
		m_listFaultReport.AddTail(pReport);
		//填充数据时禁止刷新
		m_List.SetRedraw(FALSE);
		//加入到显示, 第一列
		AddReportToList(pReport, 0);
		//检查行数
		CheckLineCount();
		m_List.SetRedraw(TRUE);

		CString str;
		str.Format("新增信息包, 包ID为%d", pReport->m_nID);
		WriteLog(str, XJ_LOG_LV3);

		CString sContent = "";
		sContent.Format("{%s [%s][%s][%s] %s}", 
			pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S"), 
			pReport->m_pStation1 == NULL?"":pReport->m_pStation1->m_sName, pReport->m_pStation2 == NULL?"":pReport->m_pStation2->m_sName, 
			pReport->m_pPriDevice == NULL?"":pReport->m_pPriDevice->m_sName, 
			pReport->m_sName);
		CRemindContent * pRC = new CRemindContent(StringFromID(IDS_TOOLBAR_FAULTREPORT), sContent);
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->GetMainWnd()->SendMessage(ADD_REMIND, 0, (LPARAM)pRC);
	}
	else if(pParam->m_nRportType == 2 || (pParam->m_nRportType == 5 && !m_bOnlyShowNoOverhual))
	{
		//信息变更
		//先查找是否有载入在内存中
		int nID = pParam->m_nID;
		CFaultReport* pReport = FindFaultReport(nID);
		if(pReport != NULL)
		{
			//已载入在内存中
			pReport->UpdateFrom(pParam);
			if(pParam->m_nRportType == 5)
				pReport->m_nOverhaul = 1;
			//更新在列表中的显示
			//查找事件在列表中的位置
			LVFINDINFO lvFind;
			lvFind.flags = LVFI_PARAM;
			lvFind.lParam = (LPARAM)pReport;
			int nIndex = m_List.FindItem(&lvFind);
			if(nIndex > -1)
			{
				//填充数据时禁止刷新
				m_List.SetRedraw(FALSE);
				m_List.Update(nIndex);
				m_List.SetRedraw(TRUE);
			}
			
			CString str;
			str.Format("信息包变更, 包ID为%d", pReport->m_nID);
			WriteLog(str, XJ_LOG_LV3);
		}
	}
	else
	{
		WriteLog("未知故障包类型");
		return;
	}

	WriteLog("CViewFault::OnSTTP20151 neat end");
}

/*************************************************************
 函 数 名：FindFaultReport()
 功能概要：查找指定的故障报告是否有在内存中
 返 回 值: 查找成功返回指针, 失败或未找到返回NULL
 参    数：param1	指定ID
		   Param2
**************************************************************/
//##ModelId=49B87B850010
CFaultReport* CViewFault::FindFaultReport( int nID )
{
	//找到在链表中的位置
	POSITION pos = m_listFaultReport.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultReport* pTemp = (CFaultReport*)m_listFaultReport.GetNext(pos);
		if(pTemp->m_nID == nID)
			return pTemp;
	}
	return NULL;
}

void CViewFault::DeleteFaultReport( CFaultReport* pReport )
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


/*************************************************************
 函 数 名：CheckLineCount()
 功能概要：检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B850012
void CViewFault::CheckLineCount()
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
		CFaultReport* pEvent = (CFaultReport*)m_List.GetItemData(nNewCount - 1);
		//从列表中删除
		m_List.DeleteItem(nNewCount - 1);
		//从事件链表和内存中删除
		RemoveFromMem(pEvent);
	}
}

/*************************************************************
 函 数 名：RemoveFromMem()
 功能概要：将指定故障报告从内存和链表中删除
 返 回 值: void
 参    数：param1	指定故障报告
		   Param2
**************************************************************/
//##ModelId=49B87B850013
void CViewFault::RemoveFromMem( CFaultReport* pReport )
{
	//检查数据有效性
	if(pReport == NULL)
		return;
	//找到在链表中的位置
	POSITION pos = m_listFaultReport.GetHeadPosition();
	POSITION posOld = pos;
	while(pos != NULL)
	{
		posOld = pos;
		CFaultReport* pTemp = (CFaultReport*)m_listFaultReport.GetNext(pos);
		if(pTemp == pReport)
			break;
	}
	//从链表中删除
	if(posOld != NULL)
		m_listFaultReport.RemoveAt(posOld);
	//删除指针
	delete pReport;
	pReport = NULL;
}

/*************************************************************
 函 数 名：RemoveFromList()
 功能概要：将指定故障报告从列表框中删除
 返 回 值: void
 参    数：param1	指定故障报告
		   Param2
**************************************************************/
//##ModelId=49B87B850020
void CViewFault::RemoveFromList( CFaultReport* pReport )
{
	//检查有效性
	if(pReport == NULL)
		return;

	//查找事件在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pReport;
	int nIndex = m_List.FindItem(&lvFind);

	//未找到
	if(nIndex < 0)
		return;

	//移除
	m_List.DeleteItem(nIndex);
}

//##ModelId=49B87B8500DA
void CViewFault::OnFaultWeb() 
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

/*************************************************************
 函 数 名：IssueToWeb()
 功能概要：将指定的故障包发布到WEB
 返 回 值: 发布结果. -1- 无效的故障包. 0-成功. 1-通讯服务器未连接. 2-组织报文失败. 3-发送报文失败. 
 参    数：param1	指定要发布的故障包
		   Param2
**************************************************************/
//##ModelId=49B87B850022
int CViewFault::IssueToWeb( CFaultReport* pReport )
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
	
	//组织20151报文
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

void CViewFault::OnFaultRefresh() 
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
void CViewFault::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	RefreshData();
	WriteLog("CViewFault::OnViewRefresh, 强制刷新完毕", XJ_LOG_LV3);
}

void CViewFault::OnFaultCreate() 
{
	// TODO: 新建故障包
	//1.查询数据库中最新的包ID
	int nID = QueryLatsetID();
	if(nID < 0)
	{
		WriteLog("未查询到合法的ID", XJ_LOG_LV3);
		nID = 0;
		//return;
	}
	//2.新建包
	CFaultReport* pReport = CreateFaultReport(++nID);
	if(pReport == NULL)
		return;
	//4.保存到数据库
	if(!AddReportToDB(pReport))
	{
		//保存失败
		AfxMessageBox(StringFromID(IDS_PACK_SAVEPACKAGE_FAIL));
		delete pReport;
		return;
	}
	//3.添加到链表和LIST显示
	m_listFaultReport.AddTail(pReport);
	AddReportToList(pReport, 0); //第一行
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 函 数 名: QueryLatsetID()
 功能概要: 查询最新的故障包ID
 返 回 值: 最新的故障包ID
 参    数: param1 
		   Param2 
**************************************************************/
int CViewFault::QueryLatsetID()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	int nReturn = -1;
	
	//查询最近的1个记录
	CString str;		
	
	//top 1
	Field Field1;
	str.Format("%d", 1);
	pApp->m_DBEngine.SetField(sql, Field1, str, EX_STTP_DATA_TYPE_TOP);
	//条件
	Condition Condition1;
	str.Format("order by ID DESC");
	pApp->m_DBEngine.SetCondition(sql, Condition1, str, EX_STTP_WHERE_ABNORMALITY);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CViewFault::QueryLatsetID, 查询失败");
		delete[] sError;
		delete pMemset;
		return -1;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount == 1)
		{
			//只应该查询到一个
			CString strID = pMemset->GetValue(UINT(0));
			nReturn = atoi(strID);
		}
	}
	else
	{
		CString str;
		str.Format("CViewFault::QueryLatsetID,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return nReturn;
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 函 数 名: CreateFaultReport()
 功能概要: 新建故障包
 返 回 值: 新创建的故障包
 参    数: param1 包ID
		   Param2 
**************************************************************/
CFaultReport* CViewFault::CreateFaultReport(int nID)
{
	CFaultReport* pReport = new CFaultReport;
	pReport->m_nID = nID;
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("%s%s", StringFromID(IDS_FAULTREPORT_NEW),t.Format("%Y%m%d%H%M%S"));
	pReport->m_sName = strName;
	pReport->m_tmCreateTime = t;
	pReport->m_tmFaultTime = t;

	return pReport;
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 函 数 名: AddReportToDB()
 功能概要: 将故障包加入到数据库,事件不处理
 返 回 值: 加入成功返回TRUE,失败返回FALSE
 参    数: param1 指定故障包
		   Param2 
**************************************************************/
BOOL CViewFault::AddReportToDB( CFaultReport* pReport )
{
	BOOL bReturn = TRUE;

	if(pReport == NULL)
		return FALSE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//查询最近的nNum个记录
	CString str;		
	
	//top
	//字段
	//ID
	Field Field1;
	str.Format("%d", pReport->m_nID);
	pApp->m_DBEngine.SetField(sql, Field1, "ID", EX_STTP_DATA_TYPE_INT, str);

	//Name
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "CaseName", EX_STTP_DATA_TYPE_STRING, pReport->m_sName);
	
	//Name
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "FaultMsTime", EX_STTP_DATA_TYPE_INT, "0");


	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "reverse4", EX_STTP_DATA_TYPE_STRING, pReport->m_tmCreateTime.Format("%Y%m%d%H%M%S"));

	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "FaultTime", EX_STTP_DATA_TYPE_STRING, pReport->m_tmFaultTime.Format("%Y%m%d%H%M%S"));
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJInsertData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError);
	}
	catch (...)
	{
		WriteLog("CViewFault::AddReportToDB, 失败");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult != 1)
	{
		CString str;
		str.Format("CViewFault::AddReportToDB,失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}

void CViewFault::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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



void CViewFault::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnLButtonDblClk(nFlags, point);
}

void CViewFault::OnFaultDelete() 
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

void CViewFault::DeleteFaultShortenData(CList<int, int>&  itemList)
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

BOOL CViewFault::DeleteFaultShortenData(CFaultReport* pReport)
{
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//设置条件
	
	CString str;
	Condition con;
	str.Format("casename = '%s' and id = %d", pReport->m_sName, pReport->m_nID);
	pApp->m_DBEngine.SetCondition(sql, con, str);

	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError);
	}
	catch (...)
	{
		WriteLog("CViewFault::DeleteFaultShortenData, 失败");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//成功
		DeleteFaultShortenEvents(pReport);
		DeleteFaultAnalyData(pReport);
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;
	
	delete[] sError;
	
	return bReturn;
}

void CViewFault::OnFaultExport() 
{
	//父窗口是分割窗口
	CWnd * pWnd = GetParent()->GetParent()->GetParent();
	if(pWnd == NULL)
		return;
	//向父窗口发送消息
	pWnd ->PostMessage(FAULTREPORT_EXPORT_EXCEL, 0, 0);
}


void CViewFault::OnManualFaultShorten()
{
	CManualFaultShorten manDlg;
	manDlg.pView = this;
	manDlg.DoModal();
}


BOOL CViewFault::DeleteFaultShortenEvents(CFaultReport* pReport)
{
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//设置条件
	
	CString str;
	Condition con;
	str.Format("MainRecID = %d", pReport->m_nID);
	pApp->m_DBEngine.SetCondition(sql, con, str);
	
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("CViewFault::DeleteFaultShortenEvents, 失败");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//成功
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;
	
	delete[] sError;
	
	return bReturn;
}

/****************************************************
Date:2012/2/6  Author:LYH
函数名:   DeleteFaultAnalyData	
返回值:   BOOL	
功能概要: 
参数: CFaultReport * pReport	
*****************************************************/
BOOL CViewFault::DeleteFaultAnalyData( CFaultReport* pReport )
{
	BOOL bReturn = TRUE;

	if(pReport == NULL)
		return FALSE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//设置条件
	
	CString str;
	Condition con;
	str.Format("MainRecID = %d", pReport->m_nID);
	pApp->m_DBEngine.SetCondition(sql, con, str);
	
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJDeleteData(EX_STTP_INFO_ANALYFAULT_DATA, sql, sError);
	}
	catch (...)
	{
		WriteLog("CViewFault::DeleteFaultAnalyData, 失败");
		delete[] sError;
		return FALSE;
	}
	
	if(nResult == 1)
	{
		//成功
		bReturn = TRUE;
	}
	else
		bReturn = FALSE;
	
	delete[] sError;
	
	return bReturn;
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   RefreshData	
返回值:   int	
功能概要: 
*****************************************************/
int CViewFault::RefreshData()
{
	//父窗口是分割窗口
	CWnd * pWnd = GetParent()->GetParent()->GetParent();
	if(pWnd == NULL)
		return 0;
	//向父窗口发送消息, 选择了空行,发送空指针
	pWnd ->PostMessage(FAULTREPORT_SEL_CHANGED, 0, (LPARAM)NULL);
	if(!LoadDataFromDB(m_nMaxRow))
		return 0;
	FillListData();
	WriteLog("故障报告视图刷新数据完毕", XJ_LOG_LV3);
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
void CViewFault::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	if(wParam == 0)
		return;
	if(lParam == STATIONINIT_CREATE)
		return;
	CStationObj* pStation = (CStationObj*)wParam;
	BOOL bNeedReload = FALSE;
	POSITION pos = m_listFaultReport.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultReport* pReport = (CFaultReport*)m_listFaultReport.GetNext(pos);
		if(pReport == NULL)
			continue;
		if(pReport->m_sStation1ID == pStation->m_sID || pReport->m_sStation2ID == pStation->m_sID)
		{
			bNeedReload = TRUE;
			break;
		}
	}
	if(bNeedReload)
	{
		RefreshData();
		WriteLog("StationInit,故障报告页面刷新完毕", XJ_LOG_LV3);
	}
	else
	{
		WriteLog("StationInit,故障报告页面无须刷新", XJ_LOG_LV3);
	}
}

/****************************************************
Date:2013/8/8  Author:LYH
函数名:   LoadConfig	
返回值:   void	
功能概要: 
*****************************************************/
void CViewFault::LoadConfig()
{
	CString strIniFile = g_SysConfig;

	m_nMaxRow = GetPrivateProfileInt("FaultViewConfig", "MaxRow", 100, strIniFile);
	m_bOnlyShowNoOverhual = GetPrivateProfileInt("FaultViewConfig", "OnlyShowOverhual", 0, strIniFile);
	m_bOnlyShowSaved = GetPrivateProfileInt("FaultViewConfig", "OnlyShowSaved", 0, strIniFile);
	m_bOnlyShowTypical = GetPrivateProfileInt("FaultViewConfig", "OnlyShowTypical", 0, strIniFile);
}

/****************************************************
Date:2013/8/8  Author:LYH
函数名:   SaveConfig	
返回值:   void	
功能概要: 
*****************************************************/
void CViewFault::SaveConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//最大显示条数
	str.Format("%d", m_nMaxRow);
	WritePrivateProfileString("FaultViewConfig", "MaxRow", str, strIniFile);
	str.Format("%d", m_bOnlyShowNoOverhual);
	WritePrivateProfileString("FaultViewConfig", "OnlyShowOverhual", str, strIniFile);
	str.Format("%d", m_bOnlyShowSaved);
	WritePrivateProfileString("FaultViewConfig", "OnlyShowSaved", str, strIniFile);
	str.Format("%d", m_bOnlyShowTypical);
	WritePrivateProfileString("FaultViewConfig", "OnlyShowTypical", str, strIniFile);
}

void CViewFault::OnFaultOnlyshowoverhaul() 
{
	// TODO: Add your command handler code here
	m_bOnlyShowNoOverhual = !m_bOnlyShowNoOverhual;
	RefreshData();
}

void CViewFault::OnFaultOnlyshowsaved() 
{
	// TODO: Add your command handler code here
	m_bOnlyShowSaved = !m_bOnlyShowSaved;
	RefreshData();
}

void CViewFault::OnFaultOnlyshowtypical() 
{
	// TODO: Add your command handler code here
	m_bOnlyShowTypical = !m_bOnlyShowTypical;
	RefreshData();
}
