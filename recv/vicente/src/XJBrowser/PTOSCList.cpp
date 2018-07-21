// PTOSCList.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTOSCList.h"
#include "GlobalFunc.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

#define OSCSRC_DEV	0
#define OSCSRC_SUBSTAION	1
#define OSCSRC_MAINSTATION	2

#define	COL_MAINRECEIVETIME	5
#define NUM_COL	6

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: PTParse20026()
 功能概要: 解析20026报文线程
 返 回 值: 
 参    数: param1 录波下载窗口对象
		   Param2 
**************************************************************/
UINT PTParse20026(LPVOID pParam)
{
	CPTOSCList* ptOscList = (CPTOSCList*)pParam;
	if(ptOscList == NULL)
		return -1;

	//循环分发报文
	while(1)
	{
		if(ptOscList->m_bExitParse)
		{
			//退出标志为TRUE
			return 0;
		}

		if(ptOscList->m_bDataEnd && (ptOscList->m_listFullData.GetCount() == 0))
		{
			//通知线程工作完毕
			WriteLog("PTParse20026, 工作完成,正常退出",XJ_LOG_LV3);
			ptOscList->PostMessage(THREAD_WORKEND);
			return 0; //再没有报文加入且现有报文处理完成，退出
		}
		
		//休眠
		int nSize = ptOscList->m_listFullData.GetCount();
		if(nSize == 0)
			Sleep(50);
		
		//取得队列的第一个数据
		POSITION pos = ptOscList->m_listFullData.GetHeadPosition();
		if(pos == NULL)
			continue;
		STTP_FULL_DATA sttpData = (STTP_FULL_DATA)ptOscList->m_listFullData.GetAt(pos);
		
		//读取报文成功
		try
		{
			CString str;
			str.Format("PTParse20026, 处理报文ing...,剩余:%d", nSize);
			WriteLog(str, XJ_LOG_LV3);
			int nCount = sttpData.sttp_body.variant_member.file_list.size();
			for(int i = 0; i < nCount; i++)
			{
				if(ptOscList->m_bExitParse) //退出
					return 0;
				//创建对象
				CXJEventOSC* pOsc = new CXJEventOSC;
				if(!pOsc->LoadFromSttpForPT(sttpData, i))
				{
					delete pOsc;
					pOsc = NULL;
					continue;
				}
				//设置ID
				pOsc->m_lID = ptOscList->m_nNum++;
				//加入到链表
				ptOscList->m_EventManager.AddEvent(pOsc);
			}
		}
		catch (...)
		{
			WriteLog("PTParse20026, 处理报文异常", XJ_LOG_LV3);
		}
		
		zero_sttp_full_data(sttpData);
		//删除数据
		ptOscList->RemoveHead20026SttpData();
	}

	return 0;
}

UINT QueryData(LPVOID pParam)
{
	CPTOSCList* ptOscList = (CPTOSCList*)pParam;
	if(ptOscList == NULL)
		return -1;
	ptOscList->LoadDataFromDB();
	ptOscList->NotifyThreadEnd();
	ptOscList->PostMessage(THREAD_WORKEND);
	return 0;
}

/** @brief           升降序作用位*/
int g_iPTOSCListAsc;

/////////////////////////////////////////////////////////////////////////////
// CPTOSCList

IMPLEMENT_DYNCREATE(CPTOSCList, CViewBase)

//##ModelId=49B87B8F0179
CPTOSCList::CPTOSCList()
	: CViewBase(CPTOSCList::IDD)
{
	//{{AFX_DATA_INIT(CPTOSCList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nSelSrc = 0;
	m_nCurrentStatus = 0;
	m_pObj = NULL;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_EventManager.ClearAllEvents();
	m_nFrameCount = 0;
	m_arrFrame.RemoveAll();
	m_EventDownload.RemoveAll();
	m_bUseBreakPoint = FALSE;
	m_nNum = 1;
	m_nOldSortCol = -1;
	/** @brief           下载成功数*/
	m_nSuccess = 0;
	/** @brief           下载失败数*/
	m_nFaild = 0;
	/** @brief           部分下载数*/
	m_nPart = 0;
	m_listFullData.RemoveAll();
	m_bExitParse = FALSE;
	m_bDataEnd = FALSE;
	m_pParse20026Thread = NULL;
	m_pQueryThread = NULL;
	InitializeCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B8F02BF
CPTOSCList::~CPTOSCList()
{
	m_EventManager.ClearAllEvents();
	//清除残留报文
	m_listFullData.RemoveAll();
	WriteLog("CPTOSCList::~CPTOSCList", XJ_LOG_LV3);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B8F02AF
void CPTOSCList::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTOSCList)
	DDX_Control(pDX, IDC_BTN_PTOSCLIST_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_LIST_PTOSCLIST, m_List);
	DDX_Control(pDX, IDC_GIF_PTOSCLIST, m_gif);
	DDX_Control(pDX, IDC_BTN_PTOSCLIST_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BTN_PTOSCLIST_CALL, m_btnCall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTOSCList, CViewBase)
	//{{AFX_MSG_MAP(CPTOSCList)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_CMB_PTOSCLIST_SRC, OnSelchangeCmbPtosclistSrc)
	ON_BN_CLICKED(IDC_BTN_PTOSCLIST_CALL, OnBtnPtosclistCall)
	ON_BN_CLICKED(IDC_BTN_PTOSCLIST_DOWN, OnBtnPtosclistDown)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PTOSCLIST_OPEN, OnBtnPtosclistOpen)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTOSCLIST, OnColumnclickListPtosclist)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTOSCLIST, OnDblclkListPtosclist)
	ON_BN_CLICKED(IDC_BTN_PTOSCLIST_COPY, OnBtnPtosclistCopy)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTOSCLIST, OnCustomDraw)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTOSCLIST_CALL, OnDoubleclickedBtnPtosclistCall)
	ON_BN_DOUBLECLICKED(IDC_BTN_PTOSCLIST_DOWN, OnDoubleclickedBtnPtosclistDown)
	//}}AFX_MSG_MAP
	//}}AFX_MSG_MAP
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20026, OnSTTP20026)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(PTOSCLIST_CALLFILE_END, OnCallFileEnd)
	ON_MESSAGE(WM_STTP_20043, OnSTTP20043)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_WRFRAME_OPEN, OnWRFrameOpen)
	ON_MESSAGE(THREAD_WORKEND, OnThreadWorkEnd)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(WM_WRFRAME_CLOSE, OnWRFrameClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTOSCList diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8F02DE
void CPTOSCList::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B8F02EE
void CPTOSCList::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTOSCList message handlers

//##ModelId=49B87B8F02A0
void CPTOSCList::OnInitialUpdate() 
{
	//记录位置
	//上排
	RecordRate(IDC_STATIC_PTOSCLIST_SRC, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTOSCLIST_SRC, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTOSCLIST_START, 0, left_client, top_client);
	RecordRate(IDC_DP_PTOSCLIST_START, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTOSCLIST_END, 0, left_client, top_client);
	RecordRate(IDC_DP_PTOSCLIST_END, 0, left_client, top_client);

	//按钮
	RecordRate(IDC_BTN_PTOSCLIST_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTOSCLIST_DOWN, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTOSCLIST_OPEN, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTOSCLIST_COPY, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTOL_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTOL_P2, 0, left_client, top_client);
	
	RecordRate(IDC_GIF_PTOSCLIST, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTLIST, 0, left_client, top_client);
	RecordRate(IDC_STATIC_NUM_PTLIST, 1, left_client, top_client);

	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);

	//初始化按钮控件
	InitControls();
	//初始化列表
	InitListCtrl();
	//初始化数据来源下拉框
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTOSCLIST_SRC);
	if(pCmb != NULL)
	{
		pCmb->AddString(StringFromID(IDS_MODEL_RELAY));
		if (g_role != MODE_SUB)
		{
			pCmb->AddString( StringFromID(IDS_SOURCE_SUBSTATIONDB));
			pCmb->AddString( StringFromID(IDS_SOURCE_LOCALDB));
			pCmb->SetCurSel(1);
			m_nSelSrc = 1;
		}
		else
		{
			pCmb->SetCurSel(0);
			m_nSelSrc = 0;
		}
	}
	//设置时间框
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_START);
	if(pTime == NULL)
		return;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_END);
	if(pTime2 == NULL)
		return;
	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
 	pTime2->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//设置范围
	CTime tmMin(1972, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	pTime2->SetRange(&tmMin, &tmMax);
	//设置时间
	CTime tm = CTime::GetCurrentTime();
	CTimeSpan ts(30, 0, 0, 0);
	CTime tm1 = tm - ts;
	VERIFY(pTime->SetTime(&tm1));

	//加载动画
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}
}

//##ModelId=49B87B8F02FD
void CPTOSCList::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTOSCLIST, left_client, IDC_DP_PTOSCLIST_START, right_client, bottom_client);
	
}

/*************************************************************
 函 数 名：InitControls()
 功能概要：初始化按钮控件
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F0203
int CPTOSCList::InitControls()
{
	/*
	//录波按钮
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
//	m_btnCall.SetIcon(IDI_SEARCH);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("召唤录波列表"));
	
	//下载按钮
	m_btnDown.SetThemeHelper(&m_ThemeHelper);
//	m_btnDown.SetIcon(IDI_SEARCH);
	m_btnDown.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnDown.SetTooltipText(_T("下载指定的录波文件"));

	//打开按钮
	m_btnOpen.SetThemeHelper(&m_ThemeHelper);
//	m_btnOpen.SetIcon(IDI_WAVE);
	m_btnOpen.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnOpen.SetTooltipText(_T("打开指定的录波文件"));
	*/
	return 0;
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B8F0213
int CPTOSCList::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
//	FillListData();

	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B8F0222
int CPTOSCList::InitListStyle()
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
		//char* arColumn[NUM_COL]={"序号", "名称","大小", "故障时间", "已下载", "主站接收时间"};
	
		//分配列宽,最小需求
		for (int nCol=0; nCol < NUM_COL ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 80; //序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 250; // 名称
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 100; // 大小
				colName = StringFromID(IDS_COMMON_FILESIZE);
				break;
			case 3://
				lvCol.cx = 200; // 故障时间
				colName = StringFromID(IDS_FAULT_TIME);
				break;
			case 4:
				lvCol.cx = 80; //已下载
				colName = StringFromID(IDS_DOWNLOAD_COMPLETE);
				break;
			case 5:
				lvCol.cx = 200; //主站接收时间
				colName = StringFromID(IDS_EVENT_RECIVETIME);
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
	}
	m_List.SetColumnHide(COL_MAINRECEIVETIME, TRUE);
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
//##ModelId=49B87B8F0233
BOOL CPTOSCList::LoadListConfig()
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
			WriteLog("PTOSCList::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("PTOSC::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTOSCListConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到PTOSCListConfig");
				WriteLog("PTOSCList::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到PTOSCListConfig下的配置");
					WriteLog("PTOSCList::读取UIConfig失败,将使用默认列风格");
					bResult = FALSE;
				}
				else
				{
					//找到子节点列表指针
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					if(pChild->Getlength() != NUM_COL)
					{
						WriteLog("PTOSCList::列表的列数不对,将使用默认列风格");
						bResult = FALSE;
					}
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

//##ModelId=49B87B8F0242
BOOL CPTOSCList::SaveListConfig()
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
			WriteLog("PTOSC::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("PTOSC::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTOSCListConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到PTOSCListConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTOSCListConfig");
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
						WriteLog("未找到PTOSCListConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了PTOSCListConfig下的配置, 先删除新建所有子节点");
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
//##ModelId=49B87B8F0232
int CPTOSCList::FillListData()
{
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	if(m_nSelSrc == OSCSRC_MAINSTATION)
	{
		m_List.SetColumnHide(COL_MAINRECEIVETIME, FALSE);
	}
	else
		m_List.SetColumnHide(COL_MAINRECEIVETIME, TRUE);
	//填充ListCtrl内容
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
	//恢复刷新
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B8F0251
BOOL CPTOSCList::NeedSave()
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

//##ModelId=49B87B8F031C
void CPTOSCList::OnSelchangeCmbPtosclistSrc() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTOSCLIST_SRC);
	MYASSERT(pCmb);
	int nSel = pCmb->GetCurSel();
	if(nSel >= 0)
	{
		m_nSelSrc = nSel;
	}
}

//##ModelId=49B87B8F032C
void CPTOSCList::OnBtnPtosclistCall() 
{
	// TODO: Add your control notification handler code here
	//响应按钮按下操作, 召唤开关量或取消召唤
	MYASSERT(m_pObj);
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}

	if(m_nSelSrc == OSCSRC_MAINSTATION)
	{
		if(m_nCurrentStatus == 0)
		{
			//查询
			m_nCurrentStatus = 1;
			StartQueryThread();
		}
		else 
		{
			//取消
			m_nCurrentStatus = 0;
			EndQueryThread();
		}
		//根据当前状态更新控件可用性
		UpdateControlsEnable();
		return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}
	
	if(m_nCurrentStatus == 0)
	{
		//空闲状态, 点击按钮表示要召唤开关量
		//申请报文数据容器
		CString str="";
		SetDlgItemText(IDC_STATIC_NUM_PTLIST, str);

		STTP_FULL_DATA sttpData;
		
		CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_START);
		MYASSERT(pTime);
		CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_END);
		MYASSERT(pTime2);
		CTime tmStart;
		pTime->GetTime(tmStart);
		CTime tmEnd;
		pTime2->GetTime(tmEnd);
		//组织20025报文
		
		if (g_role != MODE_SUB)
		{
			if(!pApp->m_SttpEngine.BuildDataFor20025WaveFileList(sttpData, m_pObj->m_sID, m_nSelSrc, tmStart, tmEnd))
			{
				//组织报文失败
				return;
			}
		}
		else
		{
			if(!pApp->m_SttpEngine.BuildDataFor20025WaveFileList(sttpData, m_pObj->m_sID, 0, tmStart, tmEnd))
			{
				//组织报文失败
				return;
			}
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
				str.Format("保护[%s]发送召唤录波列表报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送召唤录波列表报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,状态改为正在召唤录波列表
			m_nCurrentStatus = 1;
			
			//启动定时器
			CDataEngine * pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nWavFileListTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//删除之前的列表
			m_List.DeleteAllItems();
			m_EventManager.ClearAllEvents();
			m_nNum = 1;
			
			//根据当前状态更新控件可用性
			UpdateControlsEnable();

			CString str;
			str.Format("保护[%s]发送召唤录波列表报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);

			//启动解析20026线程
			StartParse20026Thread();
		}
	}
	else 
	{
		//正在召唤开关量
		//取消召唤
		CancelCall();
		//退出解析20026线程
		EndParse20026Thread();
	}
}

/*************************************************************
 函 数 名：UpdateControlsEnable()
 功能概要：根据当前状态更新控件可用性
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F0261
void CPTOSCList::UpdateControlsEnable()
{
	//按状态分情况
	if(m_nCurrentStatus == 0)
	{	
		//按钮
		m_btnCall.EnableWindow(TRUE);
		SetDlgItemText(IDC_BTN_PTOSCLIST_CALL, StringFromID(IDS_FUNC_CALL_LIST));
		m_btnDown.EnableWindow(TRUE);
		SetDlgItemText(IDC_BTN_PTOSCLIST_DOWN, StringFromID(IDS_FUNC_CALL_FILE));
		m_btnOpen.EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTOSCLIST_COPY)->EnableWindow(TRUE);
		
		//下拉框
		GetDlgItem(IDC_CMB_PTOSCLIST_SRC) ->EnableWindow(TRUE);
		//时间
		GetDlgItem(IDC_DP_PTOSCLIST_START) ->EnableWindow(TRUE);
		GetDlgItem(IDC_DP_PTOSCLIST_END) ->EnableWindow(TRUE);

		//停止动画
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//停止计时
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus  > 0)
	{
		//按钮
		m_btnCall.EnableWindow(FALSE);
		m_btnDown.EnableWindow(FALSE);
		m_btnOpen.EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTOSCLIST_COPY)->EnableWindow(FALSE);
		
		//下拉框
		GetDlgItem(IDC_CMB_PTOSCLIST_SRC) ->EnableWindow(FALSE);
		//时间
		GetDlgItem(IDC_DP_PTOSCLIST_START) ->EnableWindow(FALSE);
		GetDlgItem(IDC_DP_PTOSCLIST_END) ->EnableWindow(FALSE);
		
		//开始动画
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//开始计时
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTLIST, str);

		if(m_nCurrentStatus == 1)
		{
			//召唤录波列表
			m_btnCall.EnableWindow(TRUE);
			SetDlgItemText(IDC_BTN_PTOSCLIST_CALL, CANCEL_CALL);
		}
		else if(m_nCurrentStatus == 2)
		{
			//下载录波文件
			m_btnDown.EnableWindow(TRUE);
			SetDlgItemText(IDC_BTN_PTOSCLIST_DOWN, CANCEL_CALL);
		}
	}
}

/*************************************************************
 函 数 名：CancelCall()
 功能概要：取消召唤
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F0271
void CPTOSCList::CancelCall()
{
	//当前是空闲的, 此操作无效
	if(m_nCurrentStatus == 0)
		return;
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	//停止定时器
	KillTimer(m_nTimer);
	if(m_nCurrentStatus == 1)
	{
		//正在召唤录波列表
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20025, m_pObj->m_sID, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在下载录波文件
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20042, m_pObj->m_sID, 0);
		//清空待下载列表
		m_EventDownload.RemoveAll();
		//清空结果
		m_CallFileResult = "";
	}
	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_nCurrentStatus = 0;
	//更改控件可用性
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：OnSTTP20026()
 功能概要：响应20026(录波列表下载通知)
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F035C
void CPTOSCList::OnSTTP20026( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤录波列表的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20026报文
	if(pSttpData->sttp_head.uMsgID != 20026)
	{
		//错误报文
		CString str;
		str.Format("CPTOSCList::OnSTTP20026 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTOSCList::OnSTTP20026 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str);
		return;
	}
	
	//加入到报文链表
	WriteLog("加入报文到链表");
	AddTail20026SttpData(*pSttpData);
	WriteLog("加入报文到链表完毕");

	CString strEnd = "";
	strEnd.Format("结束标志为 %d", pSttpData->sttp_head.uMsgEndFlag);
	WriteLog(strEnd);
	//判断是否还有后续帧
	if(pSttpData->sttp_head.uMsgEndFlag == 0)
	{
		//停止计时器
		KillTimer(m_nTimer);
		//通知线程报文没了
		m_bDataEnd = TRUE;

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_OSCLIST);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLOSCLIST, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCLIST, OPER_SUCCESS);
	}
}

/*************************************************************
 函 数 名：AddEventToList()
 功能概要：把一条事件的信息加入到列表
 返 回 值: 加入成功返回TRUE,失败返回FALSE
 参    数：param1	要加入的事件
		   Param2	加入的位置
**************************************************************/
//##ModelId=49B87B8F0272
BOOL CPTOSCList::AddEventToList( CXJEventOSC * pEvent, int i )
{
	//事件为空, 返回FALSE
	if(pEvent == NULL)
		return FALSE;
	
	//"序号", "名称","大小", "故障时间", "已下载"

	CString str;
	str.Format("%d", pEvent->m_lID);
	//0:序号
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;

	//1:名称
	m_List.SetItemText(i, 1, pEvent->m_sFileName);
	
	//2:大小
	str = "-";
	if(pEvent->m_nFileSize > 0)
		str.Format("%d Byte", pEvent->m_nFileSize);
	m_List.SetItemText(i, 2, str);

	//3:故障时间
	str.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 3, str);

	//4:已下载
	m_List.SetItemText(i, 4, pEvent->GetDownloadString());

	//5.主站接收时间
	m_List.SetItemText(i, 5, pEvent->m_tmMainReceiveTime.Format("%Y-%m-%d %H:%M:%S"));

	//设置数据
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

/*************************************************************
 函 数 名：TranslateMultiFrame()
 功能概要：解析多帧的定值报文
 返 回 值: 
 参    数：param1	报文数据
		   Param2
**************************************************************/
//##ModelId=49B87B8F0280
void CPTOSCList::TranslateMultiFrame( STTP_FULL_DATA* pSttpData )
{
	if(pSttpData == NULL)
		return;
	
	//解析定值数据
	try
	{
		//读取每项值,加入到新定值链表
		int nCount = pSttpData->sttp_body.variant_member.file_list.size();
		for(int i = 0; i < nCount; i++)
		{
			CXJEventOSC * pEvent = new CXJEventOSC;
			if(!pEvent->LoadFromSttpForPT(*pSttpData, i)) //指定索引
			{
				//读取数据失败
				delete pEvent;
				pEvent = NULL;
				continue;
			}
			//加入到链表
			m_EventManager.AddEvent(pEvent);
			//加入到列表,加入到第一行
			AddEventToList(pEvent, 0);
		}
		
		//判断是否已收取完,序号数组里的元素数=总帧数就认为已收取完
		if(m_arrFrame.GetSize() == m_nFrameCount)
		{
			//收取完成,关闭定时器
			KillTimer(m_nTimer);
			//加入显示
			for(int i = 0; i < m_EventManager.GetCount(); i++)
			{
				CXJEventOSC* pEvent = (CXJEventOSC*)m_EventManager.GetAtEvent(i);
				AddEventToList(pEvent, 0);
			}
			//处理完毕,改变状态和按钮文字
			m_nCurrentStatus = 0;
			
			//更改控件可用度
			UpdateControlsEnable();
		}
	}
	catch (...)
	{
		WriteLog("CPTOSCList::TranslateMultiFrame, 错误", 3);
		return;
	}
}

/*************************************************************
 函 数 名：OnPTFrameOpen()
 功能概要：响应打开保护属性页消息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F036B
void CPTOSCList::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	int nIndex = (int)wParam;

	//从列表框中删除
	m_List.DeleteAllItems();
	//从内存中删除
	m_EventManager.ClearAllEvents();
	m_nNum = 0;
	//页面还原
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	//清除召唤计时
	SetDlgItemText(IDC_STATIC_TIME_PTLIST, "");
	SetDlgItemText(IDC_STATIC_NUM_PTLIST, "");
}

//##ModelId=49B87B8F032E
void CPTOSCList::OnBtnPtosclistDown() 
{
	// TODO: Add your control notification handler code here
	//响应按钮按下操作, 下载录波文件或取消下载
	MYASSERT(m_pObj);

	if( m_nSelSrc == OSCSRC_MAINSTATION )
	{
		AfxMessageBox( StringFromID(IDS_DOWNOSC_BY_LOCAL));
		return;
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
		{
			return;
		}
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}

	if(m_nCurrentStatus == 0)
	{
		/** @brief           下载成功数*/
		m_nSuccess = 0;
		/** @brief           下载失败数*/
		m_nFaild = 0;
		/** @brief           部分下载数*/
		m_nPart = 0;

		//取得当前选择的录波列表
		int nCount = m_List.GetSelectedCount();
		if(nCount < 1)
		{
			AfxMessageBox( StringFromID(IDS_DOWNOSC_SELLIST));
			return;
		}
		//把选择的录波列表加入到待召唤链表
		m_EventDownload.RemoveAll();
		m_CallFileResult = "";

		int nIndex = -1;
		for(int i = 0; i < nCount; i++)
		{
			nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
			if(nIndex != -1)
			{
				CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
				if(pEvent != NULL)
				{
					//加入到待召唤链表
					m_EventDownload.AddTail(pEvent);
				}
			}
		}
		//开始下载
		//改变状态
		m_nCurrentStatus = 2;
		//更新显示
		UpdateControlsEnable();

		//检查文件
		CheckDownload();

		//下载第一个
		if(m_EventDownload.GetCount() > 0)
		{
			CXJEventOSC* pOsc = (CXJEventOSC*)m_EventDownload.GetHead();
			if(pOsc != NULL)
			{
				DownloadOSCFile(pOsc);
			}
		}
		else
		{
			AfxMessageBox( StringFromID(IDS_DOWNLOAD_FINISHED));
			//改变状态
			m_nCurrentStatus = 0;
			//更新显示
			UpdateControlsEnable();
		}
	}
	else
	{
		//取消召唤
		CancelCall();
	}
}

/*************************************************************
 函 数 名：DownloadOSCFile()
 功能概要：下载指定录波列表的录波文件
 返 回 值: void
 参    数：param1	指定录波列表
		   Param2
**************************************************************/
void CPTOSCList::DownloadOSCFile( CXJEventOSC* pEvent )
{
	//响应按钮按下操作, 下载录波文件或取消召唤
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	//申请报文数据容器
	STTP_FULL_DATA sttpData;

	int nOffset = 0;
	if(m_bUseBreakPoint)
	{
		nOffset = pEvent->m_nOffset;
	}
	
	CString strFileName;
	if (g_role != MODE_SUB)
	{
		strFileName = pEvent->m_sFilePath + pEvent->m_sFileName;
		if(strFileName.GetLength() >= 100)
			strFileName = pEvent->m_sFileName;
	}
	else
	{
		strFileName = pEvent->m_sFileName;
	}

	//CString strFileName = pEvent->m_sFileName;
	//组织20042报文
	if(!pApp->m_SttpEngine.BuildDataFor20042DownloadWaveFile(sttpData, m_pObj->m_sID, strFileName, pEvent->m_nDataSrc, nOffset, pEvent->m_nFileNum))
	{
		//组织报文失败
		//发送召唤完成消息,结果为发送失败
		pEvent->m_nDownloaded = 0;
		this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 1);
		return;
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
			str.Format("%s发送召唤录波文件报文失败,原因为连接中断", m_pObj->m_sID);
			WriteLog(str);
			//发送召唤完成消息,结果为发送失败
			pEvent->m_nDownloaded = 0;
			this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 1);
			return;
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("%s发送召唤录波文件报文失败,原因为缓存已满", m_pObj->m_sID);
			WriteLog(str);
			//发送召唤完成消息,结果为发送失败
			pEvent->m_nDownloaded = 0;
			this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 1);
			return;
		}
	}
	else
	{
		pEvent->m_nDownloaded = 0;
		//启动定时器
		CDataEngine * pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nWavFileTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//修改值
		CString strValue;
		strValue.Format("%d", nOffset);
		ChangeDownloadedValue(pEvent, nOffset);
	}
}

/*************************************************************
 函 数 名：OnCallFileEnd()
 功能概要：召唤一个录波文件完成, 正在被召唤的是待召唤链表的第一个, 完成后在此处从链表移除
 返 回 值: void
 参    数：param1
		   Param2	召唤结果. 1-发送失败. 2-返回失败. 3-超时. 4.-装置不支持. 5-成功
**************************************************************/
//##ModelId=49B87B8F036F
void CPTOSCList::OnCallFileEnd( WPARAM wparam, LPARAM lParam )
{
	//没有处于下载录波文件的状态, 不处理消息
	if(m_nCurrentStatus != 2)
		return;
	if(m_EventDownload.GetCount() < 1)
	{
		//没有待下载文件, 当作已完成
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
	}
	CXJEventOSC* pEvent = m_EventDownload.GetHead();
	//记录完成结果
	m_CallFileResult += StringFromID(IDS_CALLOPER_CALL);
	m_CallFileResult += "[";
	m_CallFileResult += pEvent->m_sFileName;
	m_CallFileResult += "]";
	m_CallFileResult += "\t";
	int nResult = (int)lParam;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	switch(pEvent->m_nDownloaded)
	{
	case 0:
		m_CallFileResult += StringFromID(IDS_COMMON_FAILED);
		m_nFaild++;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_OSCFILE);
		pApp->AddNewManOperator(FUNC_QUY_CALLOSC, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCFILE, OPER_FAILD);
		break;
	case 1:
		m_CallFileResult += StringFromID(IDS_COMMON_SUCCESS);
		m_nSuccess++;
		strMsg.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_OSCFILE);
		pApp->AddNewManOperator(FUNC_QUY_CALLOSC, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCFILE, OPER_SUCCESS);
		break;
	case 2:
		m_CallFileResult += StringFromID(IDS_DOWNLOAD_PARTIAL);
		m_nPart++;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_OSCFILE);
		pApp->AddNewManOperator(FUNC_QUY_CALLOSC, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCFILE, OPER_FAILD);
		break;
	default:
		break;
		WriteLog("文件下载状态异常!");
	}

	ChangeValue(pEvent);
	m_CallFileResult += "\r\n";
	//移除第一个待下载列表
	m_EventDownload.RemoveHead();
	//检查是否还有待下载列表
	if(m_EventDownload.GetCount() < 1)
	{
		//已没有等下载列表,表示全部下载完成
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
	//	AfxMessageBox(m_CallFileResult);
		CString str;
		str.Format("%s: %d\n%s: %d\n%s: %d", StringFromID(IDS_COMMON_SUCCESS),m_nSuccess, 
			StringFromID(IDS_COMMON_FAILED), m_nFaild, StringFromID(IDS_DOWNLOAD_PARTIAL), m_nPart);
		AfxMessageBox(str);
	}
	else
	{
		//下载下一个
		CXJEventOSC* pOsc = (CXJEventOSC*)m_EventDownload.GetHead();
		DownloadOSCFile(pOsc);
	}
}

/*************************************************************
 函 数 名：OnSTTP20043()
 功能概要：响应录波文件下载完成通知
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CPTOSCList::OnSTTP20043( WPARAM wParam, LPARAM lParam )
{
	MYASSERT(m_pObj);
	if(m_nCurrentStatus != 2)
	{
		//没有处于下载录波文件的状态,不处理报文
		return;
	}

	//没有等下载, 不处理报文
	if(m_EventDownload.GetCount() < 1)
		return;
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20043报文
	if(pSttpData->sttp_head.uMsgID != 20043)
	{
		//错误报文
		CString str;
		str.Format("CPTOSCList::OnSTTP20043 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTOSCList::OnSTTP20043 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str);
		return;
	}

	//检查是否当前要下载的文件
	CString strFileName = XJToMS(pSttpData->sttp_body.variant_member.file_data.strFileName);
	//取得第一个待下载的录波列表
	CXJEventOSC* pEvent = (CXJEventOSC*)m_EventDownload.GetHead();
	MYASSERT(pEvent);
	strFileName.Replace("/", "\\");
	if(strFileName != pEvent->GetFileRelatePath())
	{
		//不是本录波列表的文件
		WriteLog("CPTOSCList::OnSTTP20043 上送的录波文件名与本录波列表文件不匹配");
		CString str;
		str.Format("上送路径为%s, 本地路径为%s", strFileName, pEvent->GetFileRelatePath());
		WriteLog(str, XJ_LOG_LV3);
		//return;
	}

	pEvent->m_nDownloaded = pSttpData->sttp_body.nStatus;
	pEvent->m_nOffset = 0;
	CString strStatus;
	strStatus.Format("%d", pEvent->m_nDownloaded);
	WriteLog("20043 报文结果为" + strStatus, 3);
	if (g_role != MODE_SUB)
	{
		if(pEvent->m_nDownloaded == 0 && pSttpData->sttp_body.nFlag > 0)
		{
			//记录端点位置
			pEvent->m_nDownloaded = 2;
			pEvent->m_nOffset = pSttpData->sttp_body.nFlag;
		}
		else if(pEvent->m_nDownloaded == 0 && pSttpData->sttp_body.nFlag <= 0)
		{
			//下载失败
			pEvent->m_nDownloaded = 0;
			WriteLog("下载文件失败!", 3);
		}
		if(pEvent->m_nDownloaded == 1 && pEvent->m_nFileSize == 0)
		{
			//下载成功，记录文件大小
			pEvent->m_nFileSize = pSttpData->sttp_body.nFlag;
			WriteLog("下载文件成功!", 3);
		}
	}
	else
	{
		pEvent->m_nDownloaded = 1;
	}

	//关闭计时器
	KillTimer(m_nTimer);
	//发送成功消息
	this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 5);
}

/*************************************************************
 函 数 名：OnSTTP20069()
 功能概要：响应20069,操作失败报文
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CPTOSCList::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//没有处于下载录波文件或召唤录波列表的状态,不处理报文
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
		CString str;
		str.Format("CPTSetting::OnSTTP20069 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTOSCList::OnSTTP20069 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str);
		return;
	}

	if(m_nCurrentStatus == 1)
	{
		//召唤录波列表
		if(pSttpData->sttp_body.nMsgId != 20025)
			return;
		//是本次召唤的失败报文
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//关闭计时器
		KillTimer(m_nTimer);
		//退出线程
		EndParse20026Thread();
		CString str;
		str.Format("保护[%s]召唤录波列表失败", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_OSCLIST);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLOSCLIST, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCLIST, OPER_FAILD);

		//提示失败
		AfxMessageBox( StringFromID(IDS_CALL_FAIL));
	}
	else if(m_nCurrentStatus == 2)
	{
		//召唤录波文件
		if(pSttpData->sttp_body.nMsgId != 20042)
			return;
		//取得第一个待下载的录波列表
		CXJEventOSC* pEvent = (CXJEventOSC*)m_EventDownload.GetHead();
		if(pEvent == NULL)
			return;
		
		//失败
		pEvent->m_nDownloaded = 0;
		pEvent->m_nOffset = 0;
		//关闭计时器
		KillTimer(m_nTimer);
		//发送消息
		this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 5);
	}
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：响应20125, 装置不支持功能报文
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
void CPTOSCList::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	MYASSERT(m_pObj);
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//没有处于下载录波文件或召唤录波列表的状态,不处理报文
		return;
	}
		
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20125报文
	if(pSttpData->sttp_head.uMsgID != 20125)
	{
		//错误报文
		CString str;
		str.Format("CPTSetting::OnSTTP20125 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20125 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str);
		return;
	}
	//关闭计时器
	KillTimer(m_nTimer);
	
	if(m_nCurrentStatus == 1)
	{
		//召唤录波列表
		if(pSttpData->sttp_body.nMsgId != 20025)
			return;
		//是本次召唤的失败报文
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		//关闭计时器
		KillTimer(m_nTimer);
		//退出线程
		EndParse20026Thread();

		CString str;
		str.Format("保护[%s]召唤录波列表失败,原因为:装置不支持", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_OSCLIST);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLOSCLIST, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCLIST, OPER_FAILD);

		//提示失败
		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	else if(m_nCurrentStatus == 2)
	{
		//召唤录波文件
		if(pSttpData->sttp_body.nMsgId != 20042)
			return;
		//取得第一个待下载的录波列表
		CXJEventOSC* pEvent = (CXJEventOSC*)m_EventDownload.GetHead();
		if(pEvent == NULL)
			return;
		
		//失败
		pEvent->m_nDownloaded = 0;
		pEvent->m_nOffset = 0;
		//关闭计时器
		KillTimer(m_nTimer);
		//发送消息
		this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 5);
	}
}

void CPTOSCList::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		KillTimer(m_nTimer);
		if(m_nCurrentStatus == 1)
		{
			//改变状态
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
			//退出解析20026线程
			EndParse20026Thread();
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_OSCLIST);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLOSCLIST, m_pObj->m_sID, strMsg, XJ_OPER_CALL_OSCLIST, OPER_FAILD);
			//提示
			AfxMessageBox( StringFromID(IDS_CALLFAIL_TIMEOUT));
		}
		else if(m_nCurrentStatus == 2)
		{
			//发送完成消息
			this->SendMessage(PTOSCLIST_CALLFILE_END, 0, 3);
		}
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//改变持续时间显示
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTLIST, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：ChangeValue()
 功能概要：改变录波列表是否已下载的显示
 返 回 值: 
 参    数：param1	指定录波列表
		   Param2
**************************************************************/
//##ModelId=49B87B8F0292
void CPTOSCList::ChangeValue( CXJEventOSC* pEvent )
{
	//检查数据有效性
	if(pEvent == NULL)
		return;
	//查找位置
	int nIndex = -1;
	//查找事件在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pEvent;
	nIndex = m_List.FindItem(&lvFind);
	//找不到位置
	if(nIndex < 0)
		return;
	//大小
	if(pEvent->m_nDownloaded == 1) //成功
	{ 
		CString str;
		str.Format("%d Byte", pEvent->m_nFileSize);
		m_List.SetItemText(nIndex, 2, str);
	}
	//改变值
	m_List.SetItemText(nIndex, 4, pEvent->GetDownloadString());
	//刷新
	m_List.Update(nIndex);
}

//##ModelId=49B87B8F0340
void CPTOSCList::OnBtnPtosclistOpen() 
{
	// TODO: Add your control notification handler code here
	if(m_List.GetSelectedCount() != 1)
	{
		//只能选择一个录波文件
		AfxMessageBox( StringFromID(IDS_TIP_SELECT_ONEITEM));
		return;
	}
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex != -1)
	{
		CXJEventOSC* pEvent = (CXJEventOSC*)m_List.GetItemData(nIndex);
		if(pEvent == NULL)
			return;
		if(pEvent->m_nDownloaded  != 1)
		{
			AfxMessageBox( StringFromID(IDS_OPENFAIL_NOTDOWN));
			return;
		}
		CString strFilePath = pEvent->GetFilePath();
		//打开文件
		OpenWaveFile(strFilePath, GetSafeHwnd());
	}
}

//##ModelId=49B87B8F03AA
void CPTOSCList::OnWRFrameOpen( WPARAM wParam, LPARAM lParam )
{
	m_pObj = (CSecObj*)lParam;

	//从列表框中删除
	m_List.DeleteAllItems();
	//从内存中删除
	m_EventManager.ClearAllEvents();
	m_nNum = 0;
	//页面还原
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	//清除召唤计时
	SetDlgItemText(IDC_STATIC_TIME_PTLIST, "");
}

//##ModelId=49B87B8F034B
BOOL CPTOSCList::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTOSCLIST);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

//##ModelId=49B87B8F034E
void CPTOSCList::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名：CheckDownload()
 功能概要：检查要下载的文件
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8F029F
void CPTOSCList::CheckDownload()
{
	if(m_EventDownload.GetCount() < 1)
		return;
	BOOL bHasSuccess = FALSE; //是否包含已下载的文件
	BOOL bHasHalf = FALSE; //是否包含部分下载的文件

	POSITION pos = m_EventDownload.GetHeadPosition();
	while(pos != NULL)
	{
		CXJEventOSC* pOsc = (CXJEventOSC*)m_EventDownload.GetNext(pos);
		if(pOsc == NULL)
			continue;
		if(pOsc->m_nDownloaded == 1)
		{
			bHasSuccess = TRUE;
		}
		if(pOsc->m_nDownloaded == 2)
			bHasHalf = TRUE;
	}

	if(bHasSuccess)
	{
		if(AfxMessageBox(StringFromID(IDS_DOWNLOAD_AGAIN), MB_YESNO) == IDYES)
		{
			
		}
		else
		{
			//不重新下载, 去除已下载的文件
			POSITION pos = m_EventDownload.GetHeadPosition();
			while(pos != NULL)
			{
				POSITION oldPos = pos;
				CXJEventOSC* pFile = (CXJEventOSC*)m_EventDownload.GetNext(pos);
				if(pFile == NULL)
					continue;
				if(pFile->m_nDownloaded == 1)
				{
					m_EventDownload.RemoveAt(oldPos);
				}
			}
		}
	}
	if(bHasHalf)
	{
		if(AfxMessageBox(StringFromID(IDS_DOWNLOAD_RESUME), MB_YESNO) == IDYES)
		{
			m_bUseBreakPoint = TRUE;
		}
		else
		{
			m_bUseBreakPoint = FALSE;
		}
	}
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果
 参    数：param1	要比较的对象1
		   Param2	要比较的对象2
		   Param3	比较参数
**************************************************************/
//##ModelId=49B87B8F01AA
int CALLBACK CPTOSCList::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CXJEventOSC* pOSC1 = (CXJEventOSC*)lParam1;
	CXJEventOSC* pOSC2 = (CXJEventOSC*)lParam2;
	if(pOSC1 == NULL || pOSC2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"序号", "名称","大小", "故障时间", "已下载"
	switch(nCol)
	{
	case 0: //序号
		iResult = pOSC1->m_lID - pOSC2->m_lID;
		break;
	case 1: //名称
		str1 = pOSC1->m_sFileName;
		str2 = pOSC2->m_sFileName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //大小
		iResult = pOSC1->m_nFileSize - pOSC2->m_nFileSize;
		break;
	case 3: //故障时间
		str1 = pOSC1->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		str2 = pOSC2->m_tmTime.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //已下载
		iResult = pOSC1->m_nDownloaded - pOSC2->m_nDownloaded;
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTOSCListAsc;
	return iResult;
}

//##ModelId=49B87B8F0350
void CPTOSCList::OnColumnclickListPtosclist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iPTOSCListAsc = - g_iPTOSCListAsc;
	}
	else
	{
		g_iPTOSCListAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnSTTP20157()
 功能概要：响应超时通知报文
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
void CPTOSCList::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	MYASSERT(m_pObj);
	if(m_nCurrentStatus != 2 && m_nCurrentStatus != 1)
	{
		//没有处于召唤状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		WriteLog("CPTOSCList::OnSTTP20157 报文为空", XJ_LOG_LV3);
		return;
	}
	
	//检查是否20069报文
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//错误报文
		WriteLog("CPTOSCList::OnSTTP20157 报文号错误, 非20157", XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		WriteLog("CPTOSCList::OnSTTP20157 不是本装置报文", XJ_LOG_LV3);
		return;
	}
	
	if(m_nCurrentStatus == 1)
	{
		//召唤录波列表
		if(pSttpData->sttp_body.nMsgId != 20025)
		{
			WriteLog("CPTOSCList::OnSTTP20157 报文号错误, 非20025", XJ_LOG_LV3);
			return;
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//录波文件下载要求(20042)
		if(pSttpData->sttp_body.nMsgId != 20042)
		{
			WriteLog("CPTOSCList::OnSTTP20157 报文号错误, 非20042", XJ_LOG_LV3);
			return;
		}
	}
	
	//超时行为
	int nTimeoutStatus = pSttpData->sttp_body.nStatus;
	//1: 起始基准时间后移设定间隔
	//关闭定时器
	KillTimer(m_nTimer);
	//启动定时器
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	
	if(m_nCurrentStatus == 1)
	{
		//正在召唤录波列表				
		int nTimeOut = pData->m_nWavFileListTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在召唤录波文件
		WriteLog("CPTOSCList::OnSTTP20157 刷新超时时间", XJ_LOG_LV3);
		int nTimeOut = pData->m_nWavFileTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		//更新正在下载的文件的已下载内容值
		CXJEventOSC* pEvent = (CXJEventOSC*)m_EventDownload.GetHead();
		MYASSERT(pEvent);
		CString sValue = XJToMS(pSttpData->sttp_body.strMessage);
		ChangeDownloadedValue(pEvent, sValue);
	}
}

/*************************************************************
 函 数 名：ChangeDownloadedValue()
 功能概要：修改正在下载的文件的已下载内容大小
 返 回 值: 
 参    数：param1	正在下载的文件
		   Param2	已下载内容大小
**************************************************************/
void CPTOSCList::ChangeDownloadedValue( CXJEventOSC* pEvent, CString sValue )
{
	//检查数据有效性
	MYASSERT(pEvent);
	//查找位置
	int nIndex = -1;
	//查找事件在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pEvent;
	nIndex = m_List.FindItem(&lvFind);
	//找不到位置
	if(nIndex < 0)
		return;
	CString str;
	str.Format("%s K", sValue);
	//改变值
	m_List.SetItemText(nIndex, 4, str);
	//刷新
	m_List.Update(nIndex);
}

void CPTOSCList::OnDblclkListPtosclist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected > -1)
	{
		CXJEventOSC * pEvent = (CXJEventOSC*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//打开录波
		if(pEvent->m_nDownloaded  != 1)
		{
			if(AfxMessageBox(StringFromID(IDS_COMMON_NEED_DOWNLOAD), MB_YESNOCANCEL)!=IDYES)
				return;
			//把选择的录波列表加入到待召唤链表
			/** @brief           下载成功数*/
			m_nSuccess = 0;
			/** @brief           下载失败数*/
			m_nFaild = 0;
			/** @brief           部分下载数*/
			m_nPart = 0;
			m_EventDownload.RemoveAll();
			m_CallFileResult = "";
			//加入到待召唤链表
			m_EventDownload.AddTail(pEvent);
			//改变状态
			m_nCurrentStatus = 2;
			//更新显示
			UpdateControlsEnable();
			
			//检查文件
			CheckDownload();
			//下载第一个
			if(m_EventDownload.GetCount() > 0)
			{
				CXJEventOSC* pOsc = (CXJEventOSC*)m_EventDownload.GetHead();
				if(pOsc != NULL)
					DownloadOSCFile(pOsc);
			}
			else
			{
				AfxMessageBox(StringFromID(IDS_DOWNLOAD_FINISHED));
				//改变状态
				m_nCurrentStatus = 0;
				//更新显示
				UpdateControlsEnable();
			}
			
		}
		else
		{
			CString strFilePath = pEvent->GetFilePath();
			//打开文件
			OpenWaveFile(strFilePath, GetSafeHwnd());
		}
	}
	*pResult = 0;
}

void CPTOSCList::OnBtnPtosclistCopy() 
{
	// TODO: Add your control notification handler code here
		//取得所选文件
	CStringArray arrSelectFile;
	arrSelectFile.RemoveAll();
	int nCount = m_List.GetSelectedCount();
	//无选择
	if(nCount < 1)
	{
		AfxMessageBox(StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	int nIndex = -1;
	CString str = "";
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CXJEventOSC* pOSC = (CXJEventOSC*)m_List.GetItemData(nIndex);
			if(pOSC == NULL)
				continue;
			if(pOSC->m_nDownloaded  != 1)
			{
				AfxMessageBox(StringFromID(IDS_OPENFAIL_NOTDOWN));
				return;
			}

			CString strName = pOSC->m_sFileName;
			//去后缀名
			int nFind = strName.ReverseFind('.');
			if(nFind >= 0)
			{
				strName = strName.Left(nFind);
			}
			//将文件名加入到数组
			BOOL bExist = FALSE;
			for(int k = 0; k < arrSelectFile.GetSize(); k++)
			{
				CString strValue = arrSelectFile.GetAt(k);
				if(strValue == strName)
				{
					bExist = TRUE;
					break;
				}
			}
			if(!bExist)
			{
				//添加
				arrSelectFile.Add(strName);
			}
		}
	}

	//是否有文件
	if(arrSelectFile.GetSize() <= 0)
		return;

	//取得用户选择文件夹
	CString strNewDir = GetSelectDir();
	if(strNewDir.IsEmpty())
		return;

	//复制文件
	for(i = 0; i<arrSelectFile.GetSize(); i++)
	{
		CFileFind finder; //文件搜索类
		CString strPath;
		strPath = GetOSCFileDir(FALSE);
		if(strPath.Right(1) != "\\")
			strPath += "\\";
		CString strFilter;
		strFilter.Format("%s.*", arrSelectFile.GetAt(i));
		strPath += strFilter; //指定文件名
		BOOL bWorking = finder.FindFile(strPath);

		if (!bWorking)
		{
			strPath = GetOSCFileDir(TRUE);
			if(strPath.Right(1) != "\\")
			{
				strPath += "\\";
			}
			strFilter.Format("%s.*", arrSelectFile.GetAt(i));
			strPath += strFilter; //指定文件名
			bWorking = finder.FindFile(strPath);			
		}

		while(bWorking)
		{
			bWorking = finder.FindNextFile();
			
			// skip . and .. files;
			if (finder.IsDots())
			{
				continue;
			}
			else if(finder.IsDirectory()) // 是目录
			{
				continue;
			}
			else //是文件
			{
				CString strName = finder.GetFileName(); //文件名,带后缀
				//目标路径
				CString strNewPath = strNewDir+strName;
				//目录
				CString strOldPath = finder.GetFilePath();
				//复制
				CString strLog;
				if(CopyFile(strOldPath, strNewPath, TRUE))
				{
					strLog.Format("导出文件成功,目标路径%s", strNewPath);
				}
				else
				{
					strLog.Format("导出文件失败,目标路径%s", strNewPath);
				}
			}
		}
		finder.Close();
	}

	//导出
	if(AfxMessageBox(StringFromID(IDS_COMMON_EXPORT_SUCCESS), MB_OKCANCEL) == IDOK)
	{
		ShellExecute(GetSafeHwnd(), "open", "Explorer.exe", strNewDir, NULL, SW_SHOW); 
	}
}

/*************************************************************
 函 数 名: GetOSCFileDir()
 功能概要: 得到录波文件存放目录
 返 回 值: 录波文件存放目录
 参    数: param1 
		   Param2 
**************************************************************/
CString CPTOSCList::GetOSCFileDir(BOOL bNewPath)
{
	MYASSERT_STRING(m_pObj);
	//得到基址
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strDownDir = pApp->GetDownDir();
	//目录后带'\\'
	if(strDownDir.Right(1) != "\\")
		strDownDir += "\\";
	//组织路径
	CString strFullPath = strDownDir + m_pObj->m_sStationID;
	
	if (bNewPath)
	{
		strFullPath += "\\Collection";
	}
	
	if (m_pObj->m_nType == TYPE_PROTECT)
	{
		strFullPath += "\\保护\\";
	}
	else
	{
		strFullPath += "\\录波器\\";
	}
	
	strFullPath += m_pObj->m_sName + "(" + m_pObj->m_sID + ")" + "\\";
	
	return strFullPath;
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: Add20026SttpData()
 功能概要: 将20026报文加入到链表尾
 返 回 值: 
 参    数: param1 20026报文数据
		   Param2 
**************************************************************/
void CPTOSCList::AddTail20026SttpData( STTP_FULL_DATA& sttpData )
{
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  
		
		m_listFullData.AddTail(sttpData);
		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		LeaveCriticalSection(&m_CriticalSection);
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: RemoveHead20026SttpData()
 功能概要: 删除20026报文链表的第一项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTOSCList::RemoveHead20026SttpData()
{
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  
		
		m_listFullData.RemoveHead();
		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		LeaveCriticalSection(&m_CriticalSection);
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: Parse20026()
 功能概要: 解析20026报文数据
 返 回 值: 
 参    数: param1 报文数据对象
		   Param2 
**************************************************************/
void CPTOSCList::Parse20026( STTP_FULL_DATA* pSttpData )
{
    //录波文件列表下载通知(20026)
	//判断有效性
	MYASSERT(pSttpData);
	int nCount = pSttpData->sttp_body.variant_member.file_list.size();
	for(int i = 0; i < nCount; i++)
	{
		//创建对象
		CXJEventOSC* pOsc = new CXJEventOSC;
		if(!pOsc->LoadFromSttpForPT(*pSttpData, i))
		{
			delete pOsc;
			pOsc = NULL;
			continue;
		}
		//设置ID
		pOsc->m_lID = m_nNum++;
		//加入到链表
		m_EventManager.AddEvent(pOsc);
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: OnThreadWorkEnd()
 功能概要: 线程工作正常结束通知
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTOSCList::OnThreadWorkEnd( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CPTOSCList::OnThreadWorkEnd, 开始结果界面显示");
	int nCount = m_EventManager.GetCount();
	CString str;
	str.Format("%s: %d", StringFromID(IDS_EVENTCOUNT_TOTAL),nCount);
	SetDlgItemText(IDC_STATIC_NUM_PTLIST, str);
			
	//填充List
	FillListData();

	//刷新状态
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: StartParse20026Thread()
 功能概要: 启动解析20026报文线程
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTOSCList::StartParse20026Thread()
{
	m_bExitParse = FALSE;
	m_bDataEnd = FALSE;
	//清除残留报文
	m_listFullData.RemoveAll();
	//启动
	m_pParse20026Thread = AfxBeginThread(PTParse20026,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
	if(m_pParse20026Thread != NULL)
	{
		m_pParse20026Thread->m_bAutoDelete = FALSE;
		m_pParse20026Thread->ResumeThread();
		WriteLog("CPTOSCList::StartParse20026Thread, 线程启动成功");
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: EndParse20026Thread()
 功能概要: 强制停止20026解析报文
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTOSCList::EndParse20026Thread()
{
	//退出线程
	if(m_pParse20026Thread != NULL)
	{
		//线程还没退出去
		m_bExitParse = TRUE;	//设置退出标志
		WaitForSingleObject(m_pParse20026Thread->m_hThread, INFINITE);
		delete m_pParse20026Thread;
		m_pParse20026Thread = NULL;
		WriteLog("CPTOSCList::EndParse20026Thread, 线程退出成功");
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: OnPTFrameClose()
 功能概要: 响应保护窗口退出消息，退出线程
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTOSCList::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	//退出线程
	EndParse20026Thread();
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: OnWRFrameClose()
 功能概要: 响应录波器窗口退出消息，退出线程
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTOSCList::OnWRFrameClose( WPARAM wParam, LPARAM lParam )
{
	//退出线程
	EndParse20026Thread();
}

void CPTOSCList::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

void CPTOSCList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTOSCList::OnDoubleclickedBtnPtosclistCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTOSCList::OnDoubleclickedBtnPtosclistDown() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTOSCList::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pObj != NULL)
	{
		CString strStation = StringFromID(IDS_COMMON_UNKNOWN_SUBSTATION);
		if(m_pObj->m_pStation != NULL)
			strStation = m_pObj->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pObj->m_sName);
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_REMOTEFILE));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_REMOTEFILE));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/****************************************************
Date:2014/5/20  Author:LYH
函数名:   LoadDataFromDB	
返回值:   BOOL	
功能概要: 
*****************************************************/
BOOL CPTOSCList::LoadDataFromDB()
{
	if(!m_pObj)
		return FALSE;
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_START);
	MYASSERT_BOOL(pTime);
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_END);
	MYASSERT_BOOL(pTime2);
	CTime tmStart;
	pTime->GetTime(tmStart);
	CTime tmEnd;
	pTime2->GetTime(tmEnd);

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

	Field Field15;
	pApp->m_DBEngine.SetField(sql, Field15, "download", EX_STTP_DATA_TYPE_INT);

	Field Field16;
	pApp->m_DBEngine.SetField(sql, Field16, "Reverse1", EX_STTP_DATA_TYPE_STRING);
	
	//条件
	Condition condition1;
	CString sc;
	sc.Format("pt_id='%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, sc);
	Condition con2;
	sc.Format("trigger_time>'%s' AND trigger_time<'%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"), tmEnd.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con2, sc);

	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	if( m_bExitQuery )
		return FALSE;

	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OSC_LIST, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLog("CPTOSCList::LoadDataFromDB, 查询失败", XJ_LOG_LV3);
		return FALSE;
	}

	if( m_bExitQuery )
		return FALSE;
	
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CPTOSCList::LoadDataFromDB, 读取到%d条数据", nCount);
			WriteLog(str, XJ_LOG_LV3);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventOSC * pEvent = new CXJEventOSC;
			if(pEvent->LoadFromDB(&pMemset))
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

			if( i%10 == 0)
				Sleep(1);
			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTOSCList::LoadDataFromDB,查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		return FALSE;
	}
	return TRUE;
}

/****************************************************
Date:2014/5/20  Author:LYH
函数名:   StartQueryThread	
返回值:   void	
功能概要: 
*****************************************************/
void CPTOSCList::StartQueryThread()
{
	m_bExitQuery = FALSE;
	
	//启动
	m_pQueryThread = AfxBeginThread(QueryData,this,THREAD_PRIORITY_BELOW_NORMAL);
	
	WriteLog("CPTOSCList::StartQueryThread, 线程启动成功");
}

/****************************************************
Date:2014/5/20  Author:LYH
函数名:   EndQueryThread	
返回值:   void	
功能概要: 
*****************************************************/
void CPTOSCList::EndQueryThread()
{
	//退出线程
	if(m_pQueryThread != NULL)
	{
		//线程还没退出去
		m_bExitQuery = TRUE;	//设置退出标志
		//WaitForSingleObject(m_pQueryThread->m_hThread, INFINITE);
		m_pParse20026Thread = NULL;
		WriteLog("CPTOSCList::EndQueryThread, 线程退出成功");
	}
}

/****************************************************
Date:2014/5/20  Author:LYH
函数名:   NotifyThreadEnd	
返回值:   void	
功能概要: 
*****************************************************/
void CPTOSCList::NotifyThreadEnd()
{
	m_pQueryThread = NULL;
	m_bExitQuery = FALSE;
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	WriteLog("CPTOSCList::NotifyThreadEnd, 线程退出成功");
}
