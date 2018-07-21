// PTAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTAlarm.h"
#include "MainFrm.h"
#include "DlgEventSignal.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/** @brief           升降序作用位*/
int g_iPTAlarmAsc;

UINT PTAlarmLoad(LPVOID pParam)
{
	CPTAlarm* ptAlarm = (CPTAlarm*)pParam;
	if(ptAlarm == NULL)
		return 0;
	int nQueryNo = ptAlarm->GetCurrentQueryNo();
	//读取内容
	if(ptAlarm->IsCurrentQueryNo(nQueryNo) && !ptAlarm->m_bExitThread)
	{
		if(ptAlarm->ReLoadData(nQueryNo))
			ptAlarm->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
		else
			ptAlarm->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
	}
	ptAlarm->RemoveThreadNode(nQueryNo);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CPTAlarm

IMPLEMENT_DYNCREATE(CPTAlarm, CViewBase)

//##ModelId=49B87B9400AC
CPTAlarm::CPTAlarm()
	: CViewBase(CPTAlarm::IDD)
{
	//{{AFX_DATA_INIT(CPTAlarm)
	m_bQueryAcked = TRUE;
	m_bShowDebug = TRUE;
	m_bShowReturn = TRUE;
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nMaxRow = g_PTAlarmNum;
	m_bShowAcked = TRUE;
	m_EventManager.ClearAllEvents();
	m_pObj = NULL;
	m_nOldSortCol = -1;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_nQueryNo = 0;
	m_listThread.RemoveAll();

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B940261
CPTAlarm::~CPTAlarm()
{
	m_arrColum.RemoveAll();
	m_EventManager.ClearAllEvents();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B940232
void CPTAlarm::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTAlarm)
	DDX_Control(pDX, IDC_BTN_PTALARM_EXCEL, m_btnExcel);
	DDX_Control(pDX, IDC_BTN_PTALARM_SIGN, m_btnSign);
	DDX_Control(pDX, IDC_BTN_PTALARM_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTALARM_DETAIL, m_btnDetail);
	DDX_Control(pDX, IDC_BTN_PTALARM_ACKALL, m_btnAckAll);
	DDX_Control(pDX, IDC_BTN_PTALARM_ACK, m_btnAck);
	DDX_Control(pDX, IDC_LIST_PTALARM, m_List);
	DDX_Check(pDX, IDC_CHECK_PTALARM_HIDE, m_bQueryAcked);
	DDX_Check(pDX, IDC_CHECK_PTALARM_DEBUG, m_bShowDebug);
	DDX_Check(pDX, IDC_CHECK_PTALARM_RETURN, m_bShowReturn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTAlarm, CViewBase)
	//{{AFX_MSG_MAP(CPTAlarm)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTALARM, OnCustomDraw)
	ON_BN_CLICKED(IDC_BTN_PTALARM_ACK, OnBtnPtalarmAck)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTALARM, OnColumnclickListPtalarm)
	ON_BN_CLICKED(IDC_BTN_PTALARM_DETAIL, OnBtnPtalarmDetail)
	ON_BN_CLICKED(IDC_BTN_PTALARM_SIGN, OnBtnPtalarmSign)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTALARM_ACKALL, OnBtnPtalarmAckall)
	ON_BN_CLICKED(IDC_BTN_PTALARM_PRINT, OnBtnPtalarmPrint)
	ON_BN_CLICKED(IDC_CHECK_PTALARM_HIDE, OnCheckPtalarmHide)
	ON_BN_CLICKED(IDC_CHECK_PTALARM_DEBUG, OnCheckPtalarmDebug)
	ON_BN_CLICKED(IDC_CHECK_PTALARM_RETURN, OnCheckPtalarmReturn)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTALARM, OnDblclkListAction)
	ON_BN_CLICKED(IDC_BTN_PTALARM_EXCEL, OnBtnPtalarmExport)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTAlarm diagnostics

#ifdef _DEBUG
//##ModelId=49B87B940271
void CPTAlarm::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B940280
void CPTAlarm::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTAlarm message handlers

//##ModelId=49B87B940226
void CPTAlarm::OnInitialUpdate() 
{

	//记录控件
	RecordRate(IDC_CHECK_PTALARM_HIDE, 0, left_client, top_client);
	RecordRate(IDC_CHECK_PTALARM_DEBUG, 0, left_client, top_client);
	RecordRate(IDC_CHECK_PTALARM_RETURN, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_ACK, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_EXCEL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_ACKALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_DETAIL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTALARM_SIGN, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_P3, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTALARM_LOADING, 0, mid_client, top_client);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
//	ResizeParentToFit();

	//初始化列表
	InitListCtrl();
	//初始化其它控件
	InitControls();
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B940186
int CPTAlarm::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
/*	LoadDataFromDB(m_nMaxRow, m_bShowAcked);
	FillListData();*/

	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B940196
int CPTAlarm::InitListStyle()
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
		//char* arColumn[5]={"序号","类别","告警发生时间", "消息内容", "已确认"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 5 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 80; // 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1:
				lvCol.cx = 80; //类别
				colName = StringFromID(IDS_COMMON_CLASS);
				break;
			case 2://
				lvCol.cx = 180; // 告警发生时间
				colName = StringFromID(IDS_COMMON_OCCUR_TIME);
				break;
			case 3://
				lvCol.cx = 200; // 消息内容
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 4:
				lvCol.cx = 80;
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx = 80;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(4, TRUE);
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
//##ModelId=49B87B9401B5
BOOL CPTAlarm::LoadListConfig()
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
			WriteLog("PTAlarm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("PTAlarm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTAlarmConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到PTAlarmConfig", XJ_LOG_LV3);
				WriteLog("PTAlarm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到PTAlarmConfig下的配置", XJ_LOG_LV3);
					WriteLog("PTAlarm::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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

//##ModelId=49B87B9401C5
BOOL CPTAlarm::SaveListConfig()
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
			WriteLog("PTAlarm::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("PTAlarm::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTAlarmConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到PTAlarmConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTAlarmConfig");
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
						WriteLog("未找到PTAlarmConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
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
						WriteLog("找到了PTAlarmConfig下的配置, 先删除新建所有子节点");
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
//##ModelId=49B87B9401A5
int CPTAlarm::FillListData(int m_nQueryNo)
{
	WriteLog("CPTAlarm::FillListData 开始填充", XJ_LOG_LV3);
	
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	
	CXJEventAlarm * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventAlarm*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}
	LeaveCriticalSection(&m_CriticalOper);
	//恢复刷新
	m_List.SetRedraw(TRUE);
	
	WriteLog("CPTAlarm::FillListData 开始填充", XJ_LOG_LV3);
	
	return 0;
}

//##ModelId=49B87B9401C6
BOOL CPTAlarm::NeedSave()
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
 函 数 名：InitControls()
 功能概要：初始化其它控件, 设置初始值, 显示风格
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B9401D4
int CPTAlarm::InitControls()
{
	/*
	//确认按钮
	m_btnAck.SetThemeHelper(&m_ThemeHelper);
//	m_btnAck.SetIcon(IDI_ACK);
	m_btnAck.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnAck.SetTooltipText(_T("确认所选事件"));

	//确认所有按钮
	m_btnAckAll.SetThemeHelper(&m_ThemeHelper);
//	m_btnAckAll.SetIcon(IDI_ACK);
	m_btnAckAll.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnAckAll.SetTooltipText(_T("确认所有列出的事件"));

	//详细信息按钮
	m_btnDetail.SetThemeHelper(&m_ThemeHelper);
//	m_btnDetail.SetIcon(IDI_EVENT);
	m_btnDetail.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnDetail.SetTooltipText(_T("显示事件详细信息"));

	//信息号按钮
	m_btnSign.SetThemeHelper(&m_ThemeHelper);
//	m_btnSign.SetIcon(IDI_SEARCH);
	m_btnSign.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnSign.SetTooltipText(_T("显示事件信号量汇总"));

	//打印按钮
	m_btnPrint.SetThemeHelper(&m_ThemeHelper);
//	m_btnPrint.SetIcon(IDI_PRINT);
	m_btnPrint.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnPrint.SetTooltipText(_T("打印所选事件详细信息"));
*/
	return 0;
}

//##ModelId=49B87B940290
void CPTAlarm::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTALARM, left_client, IDC_CHECK_PTALARM_RETURN, right_client, bottom_client);
}

//##ModelId=49B87B9402AF
void CPTAlarm::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
}

/*************************************************************
 函 数 名：LoadDataFromDB()
 功能概要：从数据库中读取最新事件, 数量由最大显示行数决定, 
			每一条信息新建一个事件对象保存在事件管理对象中
 返 回 值: 读取成功返回TRUE, 失败返回FLASE
**************************************************************/
//##ModelId=49B87B9401E4
BOOL CPTAlarm::LoadDataFromDB(int num, BOOL IsQueryAcked, int nQueryNo)
{
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	if(m_pObj == NULL)
		return FALSE;

	WriteLog("CPTAlarm::LoadDataFromDB, 开始查询");
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//因为返回信息需要先解析才知道是否事件是否返回信息，先读取num*3的事件
	num *= 3;

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
	//TOP num
	Field Field111;
	bzero(&Field111, sizeof(Field));
	str.Format("%d", num);
	strncpy(Field111.FieldName, str, str.GetLength());
	Field111.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field111);


	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "msg", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field31;
	pApp->m_DBEngine.SetField(sql, Field31, "alarm_id", EX_STTP_DATA_TYPE_INT);
	
	Field Field32;
	pApp->m_DBEngine.SetField(sql, Field32, "value", EX_STTP_DATA_TYPE_INT);
	
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
	
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "isdebug", EX_STTP_DATA_TYPE_INT);
	
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "property", EX_STTP_DATA_TYPE_INT);
		
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

	//检查显示过滤条件
	if(!m_bShowDebug)
	{
		Condition condition4;
		CString strFilter;
		strFilter.Format("IsDebug=%d", XJ_EVENTINFO_NORMAL);
		pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
	}

	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);
		
	//按动作启动时间大小排序
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by RECVTIME DESC,MS2 DESC");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition3);
		
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
		
	CMemSet pMemset;

	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ALARM_DATA, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLogEx("CPTAlarm::LoadDataFromDB, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CPTAlarm::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLogEx(str);
		}
		//符合条件数
		int nChecked = 0;

		for(int i = 0; i < nCount; i++)
		{
			if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			//判断已符合条件的个数
			if(nChecked == m_nMaxRow)
				break;
			//创建事件
			CXJEventAlarm * pEvent = new CXJEventAlarm;
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
				EnterCriticalSection(&m_CriticalOper);
				m_EventManager.AddEvent(pEvent);
				LeaveCriticalSection(&m_CriticalOper);
				nChecked++;
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
			str.Format("CViewAlarm::LoadDataFromDB, 要显示%d条数据,只找到%d条数据", m_nMaxRow, nChecked);
			WriteLog(str);
		}
	}
	else
	{
		CString str;
		str.Format("CPTAlarm::LoadDataFromDB,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}

	WriteLog("CPTAlarm::LoadDataFromDB, 查询完毕");

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
//##ModelId=49B87B9401F4
BOOL CPTAlarm::AddEventToList(CXJEventAlarm * pEvent, int i)
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
	
	//2:发生时间
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 2, sTime);
	//3:消息内容
	m_List.SetItemText(i, 3, pEvent->m_sMsg);
	//4:已确认
	m_List.SetItemText(i, 4, (pEvent->m_nIsAck==1)?StringFromID(IDS_ACK_YES):"");
	//设置数据
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/*************************************************************
 函 数 名：OnSTTP20018()
 功能概要：响应20018(保护告警事件上载通知)报文
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B9402F2
void CPTAlarm::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	CXJEventAlarm* pParam = (CXJEventAlarm*)lParam;
	if(pParam == NULL)
		return;
	if(pParam->m_pSec != m_pObj)
		return;

	//不显示调试信息的情况
	if(!m_bShowDebug && pParam->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		WriteLog("CPTAlarm::OnSTTP20018, 调试事件不显示");
		return;
	}

	//不显示返回信息
	if(!m_bShowReturn && pParam->m_nIsReturn == 1)
	{
		WriteLog("CPTAlarm::OnSTTP20018, 返回事件不显示");
		return;
	}

	if(GetThreadCount() > 0)
	{
		WriteLog("CPTAlarm::OnSTTP20018, 正在载入数据,不处理报文", XJ_LOG_LV3);
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
}

/*************************************************************
函 数 名：OnPTFrameOpen()
功能概要：响应保护属性页面打开消息,刷新内容 
返 回 值: void
参    数：param1	消息参数
Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B9402FD
void CPTAlarm::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	int nIndex = (int)wParam;
	
	ReFillAll();
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置未确认事件文字颜色 
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9402B1
void CPTAlarm::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

					if (g_role == MODE_SUB)
					{
						//得到事件对象
						CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nItem);
						if(pEvent == NULL)
							break;
						if(pEvent->m_nIsAck == 0)
						{
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					}

					if(nSubItem == 1)
					{
						//得到事件对象, 要保证EventManager里的列表顺序与视图list中的顺序一样
						CXJEventAlarm * pEvent = (CXJEventAlarm*)m_List.GetItemData(nItem);
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
						else if (g_style == 3)
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

//##ModelId=49B87B9402C0
void CPTAlarm::OnBtnPtalarmAck() 
{
	// TODO: Add your control notification handler code here
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
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				if(pEvent->Ack())
				{
					RefreshAck(pEvent, nIndex);
					//发送消息通知主框架
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
					if(pFrame != NULL)
					{
						pFrame->PostMessage(WM_EVENTPT_ACK, (WPARAM)pEvent->m_nEventType, (LPARAM)pEvent);
					}
				}
			}
		}
	}
}

/*************************************************************
 函 数 名：RefreshAck()
 功能概要：事件确认后刷新界面显示
 返 回 值: void
 参    数：param1	确认的事件
		   Param2	事件位置
**************************************************************/
//##ModelId=49B87B940203
void CPTAlarm::RefreshAck( CXJEventAlarm* pEvent, int nIndex )
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
	m_List.SetItemText(nIndex, 4, StringFromID(IDS_ACK_YES));
	//刷新
// 	CRect rc;
// 	m_List.GetItemRect(nIndex, &rc, LVIR_BOUNDS);
// 	m_List.InvalidateRect(rc);
	m_List.Update(nIndex);
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果
 参    数：param1	要比较的对象1
		   Param2	要比较的对象2
		   Param3	比较参数
**************************************************************/
//##ModelId=49B87B940177
int CALLBACK CPTAlarm::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CXJEventAlarm* pEvent1 = (CXJEventAlarm*)lParam1;
	CXJEventAlarm* pEvent2 = (CXJEventAlarm*)lParam2;
	if(pEvent2 == NULL || pEvent1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号","类别","告警发生时间", "消息内容", "已确认"
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
		break;
	case 4: //是否已确认
		iResult = pEvent1->m_nIsAck - pEvent2->m_nIsAck;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTAlarmAsc;
	return iResult;
}

//##ModelId=49B87B9402CE
void CPTAlarm::OnColumnclickListPtalarm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iPTAlarmAsc = - g_iPTAlarmAsc;
	}
	else
	{
		g_iPTAlarmAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

//##ModelId=49B87B9402D2
void CPTAlarm::OnBtnPtalarmDetail() 
{
	// TODO: Add your control notification handler code here
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
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	//多个选择时只处理第一个
	if(nIndex != -1)
	{
		CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
		if(pEvent != NULL)
		{
			m_EventManager.m_pCurrentEvent = pEvent;
			pApp->ShowEventProp(&m_EventManager, TRUE);
		}
	}
}

/*************************************************************
 函 数 名：OnEventPropAckPT()
 功能概要：响应由保护属性页面打开的事件属性对话框发生事件确认的消息
 返 回 值:  void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87B94030D
void CPTAlarm::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
	if(pEvent == NULL)
		return;
	//刷新界面显示
	RefreshAck(pEvent);
}

/*************************************************************
 函 数 名：OnEventPropAck()
 功能概要：响应由事件栏打开的事件属性对话框确认事件消息
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87B94031C
void CPTAlarm::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec != m_pObj)
		return;
	//查找在本地的地址
	CXJEventAlarm* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//找到了本地地址,更新显示
		RefreshAck(pLocal);
	}
}

/*************************************************************
 函 数 名：OnEventListAck()
 功能概要：响应由事件栏发出的事件确认消息
 返 回 值: void
 参    数：param1	事件类型
		   Param2	事件指针
**************************************************************/
//##ModelId=49B87B94031F
void CPTAlarm::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	int nType = (int)wParam;
	if(nType != XJ_EVENT_ALARM)
		return;
	CXJEventAlarm* pEvent = (CXJEventAlarm*)lParam;
	if(pEvent == NULL)
		return;
	if(pEvent->m_pSec != m_pObj)
		return;
	//查找在本地的地址
	CXJEventAlarm* pLocal = FindLocalPoint(pEvent);
	if(pLocal != NULL)
	{
		//找到了本地地址,更新显示
		RefreshAck(pLocal);
		//通知事件属性对话框这里发生了被动确认
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CMainFrame* pFrame = (CMainFrame*)pApp->GetMainWnd();
		if(pFrame != NULL)
		{
			pFrame->PostMessage(WM_EVENTPT_PASSIVE_ACK, (WPARAM)pLocal->m_nEventType, (LPARAM)pLocal);
		}
	}
}

/*************************************************************
 函 数 名：FindLocalPoint()
 功能概要：查找指定事件在本地的地址, 因为保护属性页面的事件和事件栏的事件是不同的地址
 返 回 值: 查询到的本地地址,未找到返回NULL
 参    数：param1	要查找的事件
**************************************************************/
//##ModelId=49B87B940213
CXJEventAlarm* CPTAlarm::FindLocalPoint( CXJEventAlarm* pEvent )
{
	if(pEvent == NULL)
		return NULL;
	CXJEventAlarm* pReturn = NULL;
	//因为现在有ID不知道的情况,只能比较二次设备ID, 发生时间等信息
	for(int i = 0; i < m_EventManager.GetCount(); i++)
	{
		CXJEventAlarm* pTemp = (CXJEventAlarm*)m_EventManager.GetAtEvent(i);
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

//##ModelId=49B87B9402DE
void CPTAlarm::OnBtnPtalarmSign() 
{
	// TODO: Add your control notification handler code here
	CDlgEventSignal dlg;
	//循环所选事件,增加信号量
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				//解析事件
				pEvent->TranslateFData();
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
 函 数 名：ReFillAll()
 功能概要：重新填充所有内容
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B940215
BOOL CPTAlarm::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();
	
	GetDlgItem(IDC_STATIC_PTALARM_LOADING) ->ShowWindow(SW_SHOW);
	//清空内容
	EnterCriticalSection(&m_CriticalOper);
	m_EventManager.ClearAllEvents();
	LeaveCriticalSection(&m_CriticalOper);
	m_List.DeleteAllItems();
	//重新载入内容
	if(MODE_SUB == g_role)
	{
		if(LoadDataFromDB(m_nMaxRow, m_bQueryAcked, 0))
			FillListData(0);
		GetDlgItem(IDC_STATIC_PTALARM_LOADING) ->ShowWindow(SW_HIDE);
	}
	else
		StartThread();
	return TRUE;
}

/*************************************************************
 函 数 名：OnPTFrameClose()
 功能概要：窗口关闭时, 退出线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B94032D
void CPTAlarm::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：关闭自己所起的线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B940216
void CPTAlarm::EndOwnerThread()
{
	WriteLog("开始退出保护告警页面读取数据线程", XJ_LOG_LV3);
	
	m_bExitThread = TRUE;
	
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("退出保护告警页面读取数据线程完毕", XJ_LOG_LV3);
}

//##ModelId=49B87B9402E0
BOOL CPTAlarm::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTALARM);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B940251
void CPTAlarm::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!bActivate)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj != NULL)
	{
		CString strStation = StringFromID(IDS_COMMON_UNKNOWN_SUBSTATION);
		if(m_pObj->m_pStation != NULL)
			strStation = m_pObj->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pObj->m_sName);
		pApp->SetAppTile(str, StringFromID(IDS_FUN_ALARM));
	}
	else
 		pApp->SetAppTile( StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_FUN_ALARM));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B9402E3
void CPTAlarm::OnBtnPtalarmAckall() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj == NULL)
		return;
	//无权限
	if (g_bVerify)
	{
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	if(AckPTAllEvent(EX_STTP_INFO_PT_ALARM_DATA, m_pObj->m_sID))
	{
		for(int i = 0; i < m_EventManager.m_EventList.GetSize(); i++)
		{
			CXJEventAlarm* pEvent = (CXJEventAlarm*)m_EventManager.m_EventList.GetAt(i);
			if(pEvent == NULL)
				continue;
			pEvent->m_nIsAck = 1;
			RefreshAck(pEvent);
		}
	}
}

/*************************************************************
 函 数 名：InitCOM()
 功能概要：初始化打印模板COM组件
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B940222
BOOL CPTAlarm::InitCOM()
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
//##ModelId=49B87B940223
void CPTAlarm::ReleaseCOM()
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
//##ModelId=49B87B940224
CString CPTAlarm::TranslateSingleKey( CString sKey )
{
	//判断数据有效性
	if(sKey == "")
		return "";
	if(m_pObj == NULL)
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
		if(strHead != "$ALARM_EVENT_LIST")
		{
			//如果不是动作事件列表, 返回空
			return "";
		}
		CXJEventAlarm* pEvent = (CXJEventAlarm*)m_List.GetItemData(nIndex);
		if(pEvent == NULL)
			return "";
		//取得尾部字符
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "EVENT_ALARMTYPE$")
		{
			return pEvent->GetAlarmType(); //事件类别
		}
		if(strTail == "EVENT_TIME1_FULL$")
		{
			CString str;
			str.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
			return str; //启动时间完整值
		}
		if(strTail == "EVENT_TIME1$")
		{
			CString str;
			str = pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
			return str; //启动时间
		}
		if(strTail == "EVENT_TIME1_MS$")
		{
			CString str;
			str.Format("%03d", pEvent->m_nms);
			return str; //启动时间毫秒值
		}
		if(strTail == "EVENT_TIME2_FULL$")
		{
			CString str;
			str.Format("%s.%03d", pEvent->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms2);
			return str; //完成时间完整值
		}
		if(strTail == "EVENT_TIME2$")
		{
			CString str;
			str = pEvent->m_tmTime2.Format("%Y-%m-%d %H:%M:%S");
			return str; //完成时间
		}
		if(strTail == "EVENT_TIME2_MS$")
		{
			CString str;
			str.Format("%03d", pEvent->m_nms2);
			return str; //完成时间毫秒值
		}
		if(strTail == "EVENT_DEF$")
		{
			return pEvent->m_sMsg; //事件内容
		}
		if(strTail == "EVENT_ACK$")
		{
			return pEvent->m_nIsAck==1?StringFromID(IDS_ACK_YES):""; //已确认
		}
		if(strTail == "EVENT_SEC_NAME$")
		{
			if(pEvent->m_pSec != NULL)
				return pEvent->m_pSec->m_sName; //二次设备名
			return "";
		}
		if(strTail == "EVENT_DEVICE_NAME$")
		{
			if(pEvent->m_pSec != NULL)
			{
				if(pEvent->m_pSec->m_pOwner != NULL)
				{
					return pEvent->m_pSec->m_pOwner->m_sName; //一次设备名
				}
			}
		}
	}
	else
	{
		//判断是否有.GetCount()方法
		nFind = sKey.Find(".GetCount()", 0);
		if(nFind > -1)
		{
			//有.GetCount()方法
			if(sKey == "$ALARM_EVENT_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_List.GetItemCount());
				return sReturn;
			}
		}
		else
		{
			//普通关键字
			if(sKey == "$PT_NAME$") //保护名称
			{
				return m_pObj->m_sName;
			}
			if(sKey == "$PT_MODEL$") //保护型号
			{
				return m_pObj->m_sModel;
			}
			if(sKey == "$DEVICE_NAME$") //一次设备
			{
				if(m_pObj->m_pOwner != NULL)
					return m_pObj->m_pOwner->m_sName;
				return "";
			}
			if(sKey == "$STATION_NAME$") //厂站名
			{
				
				//查找厂站
				CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
				CDataEngine* pData = pApp->GetDataEngine();
				CStationObj* pStation = (CStationObj*)pData->FindDevice(m_pObj->m_sStationID, TYPE_STATION);
				if(pStation != NULL)
				{
					return pStation->m_sName;
				}
				else
				{
					return "";
				}
				
			}
		}
	}
	return "";
}

//##ModelId=49B87B9402EE
void CPTAlarm::OnBtnPtalarmPrint() 
{
	// TODO: Add your control notification handler code here
	
	//打印
	InitCOM();
	//打开模板文件
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, PTALARM_LIST);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),PTALARM_LIST);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//设置回调函数
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInPTAlarm);
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

//##ModelId=49B87B9402F0
void CPTAlarm::OnCheckPtalarmHide() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReFillAll();
}

/*************************************************************
 函 数 名: CheckLineCount()
 功能概要: 检查列表中的元素数是否多出了指定的个数,如果是,删除后面的 
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTAlarm::CheckLineCount()
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

void CPTAlarm::OnCheckPtalarmDebug() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReFillAll();
}

void CPTAlarm::OnCheckPtalarmReturn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ReFillAll();
}

void CPTAlarm::OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected >= 0)
	{
		//得到该行对应的事件
		CXJEventAlarm * pEvent = (CXJEventAlarm*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//设置当前事件
		m_EventManager.m_pCurrentEvent = pEvent;
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->ShowEventProp(&m_EventManager);
	}
	*pResult = 0;
}

void CPTAlarm::StartThread()
{
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTAlarmLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("保护告警查询线程启动成功,查询批次:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTAlarm::GetNextQueryNo()
{
	int nReturn = 0;
	if(m_nQueryNo == 1000)
	{
		nReturn = 0;
	}
	else
		nReturn = m_nQueryNo+1;
	return nReturn;
}

void CPTAlarm::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("开始退出保护告警查询线程,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);

	EnterCriticalSection(&m_CriticalSection);  
	POSITION pos = m_listThread.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION posOld = pos;
		THREADNODE* pNode = (THREADNODE*)m_listThread.GetNext(pos);
		if(pNode != NULL && pNode->nNo == nNo)
		{
			m_listThread.RemoveAt(posOld);
			delete pNode;
			break;
		}
	}
	LeaveCriticalSection(&m_CriticalSection);

	str.Format("保护告警查询线程成功退出,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTAlarm::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("保护告警查询线程成功启动,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTAlarm::GetFirstNode()
{
	THREADNODE_PTR pReturn  = NULL;
	EnterCriticalSection(&m_CriticalSection);  
	POSITION pos = m_listThread.GetHeadPosition();
	if(pos != NULL)
	{
		pReturn = (THREADNODE_PTR)m_listThread.GetNext(pos);
	}
	LeaveCriticalSection(&m_CriticalSection);
	return pReturn;
}


int CPTAlarm::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);  
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

BOOL CPTAlarm::ReLoadData( int nQueryNo )
{
	return LoadDataFromDB(m_nMaxRow, m_bQueryAcked, nQueryNo);
}

void CPTAlarm::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//过期的批次
		return;
	}
	if( lParam == 1)
		FillListData(wParam);
	GetDlgItem(IDC_STATIC_PTALARM_LOADING) ->ShowWindow(SW_HIDE);
}


void CPTAlarm::OnBtnPtalarmExport() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString strName;
	strName.Format("%s(%s)-%s.xls", StringFromID(IDS_LATEST_ALARM),m_pObj->m_sName, t.Format("%Y%m%d%H%M%S"));
	//取得用户选择文件夹
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(m_List.ExportExcel(strPath))
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
