// PTHistory.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTHistory.h"
#include "GlobalFunc.h"

#include "DlgSelFault.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: PTParse20116()
 功能概要: 解析20116报文线程
 返 回 值: 
 参    数: param1 录波下载窗口对象
		   Param2 
**************************************************************/
UINT PTParse20116(LPVOID pParam)
{
	CPTHistory* ptHistory = (CPTHistory*)pParam;
	if(ptHistory == NULL)
		return -1;

	//循环分发报文
	while(1)
	{
		if(ptHistory->m_bExitParse)
		{
			//退出标志为TRUE
			return 0;
		}

		if(ptHistory->m_bDataEnd && (ptHistory->m_listFullData.GetCount() == 0))
		{
			//通知线程工作完毕
			WriteLog("PTParse20116, 工作完成,正常退出",XJ_LOG_LV3);
			ptHistory->PostMessage(THREAD_WORKEND);
			return 0; //再没有报文加入且现有报文处理完成，退出
		}
		
		//休眠
		int nSize = ptHistory->m_listFullData.GetCount();
		if(nSize == 0)
			Sleep(50);
		
		//取得队列的第一个数据
		POSITION pos = ptHistory->m_listFullData.GetHeadPosition();
		if(pos == NULL)
			continue;
		STTP_FULL_DATA sttpData = (STTP_FULL_DATA)ptHistory->m_listFullData.GetAt(pos);
		
		
		CString strlog;
		strlog.Format("总帧数:%d,当前帧数:%d",sttpData.sttp_head.uMsgType,sttpData.sttp_head.uMsgLengthType);
		WriteLog(strlog, XJ_LOG_LV3);
		//读取报文成功
		try
		{
			CString str;
			str.Format("PTParse20116, 处理报文ing...,剩余:%d", nSize);
			WriteLog(str, XJ_LOG_LV3);
			int nType = sttpData.sttp_body.nEventType;
			int nEventCount = sttpData.sttp_body.variant_member.dataflat_data.record_list.size();
			if(nType == 1 || nType == 5)
			{
				//动作
				for(int i = 0; i < nEventCount; i++)
				{
					if(ptHistory->m_bExitParse)
					{
						//退出标志为TRUE
						return 0;
					}
					ptHistory->TranslateHistoryAction(&sttpData, i);
				}
				
			}
			else if(nType == 2 || nType == 6)
			{
				//告警
				for(int i = 0; i < nEventCount; i++)
				{
					if(ptHistory->m_bExitParse)
					{
						//退出标志为TRUE
						return 0;
					}
					ptHistory->TranslateHistoryAlarm(&sttpData, i);
				}
			}
			else if(nType == 3)
			{
				//开关量
				for(int i = 0; i < nEventCount; i++)
				{
					if(ptHistory->m_bExitParse)
					{
						//退出标志为TRUE
						return 0;
					}
					ptHistory->TranslateHistoryDI(&sttpData, i);
				}
			}
		}
		catch (...)
		{
			WriteLog("PTParse20116, 处理报文异常", XJ_LOG_LV3);
		}
		
		zero_sttp_full_data(sttpData);
		//删除数据
		ptHistory->RemoveHead20116SttpData();
	}

	return 0;
}

/** @brief           升降序作用位*/
int g_iPTHistoryAsc;
/////////////////////////////////////////////////////////////////////////////
// CPTHistory

IMPLEMENT_DYNCREATE(CPTHistory, CViewBase)

//##ModelId=49B87B9001A7
CPTHistory::CPTHistory()
	: CViewBase(CPTHistory::IDD)
{
	//{{AFX_DATA_INIT(CPTHistory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_nSelSrc = 1;
	m_nEventType = 0;
	m_nCurrentStatus = 0;
	m_pObj = NULL;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_EventList.RemoveAll();
	m_arrDigital.RemoveAll();
	m_nNum = 1;
	m_nOldSortCol = -1;
	m_nActionIndex = 0;

	m_listFullData.RemoveAll();
	m_bExitParse = FALSE;
	m_bDataEnd = FALSE;
	m_pParse20116Thread = NULL;
	InitializeCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B9001C5
CPTHistory::~CPTHistory()
{
	ClearAllDI();
	ClearAllEvent();
	//清除残留报文
	m_listFullData.RemoveAll();
	DeleteCriticalSection(&m_CriticalSection);
	WriteLog("CPTHistory::~CPTHistory", XJ_LOG_LV3);
}

//##ModelId=49B87B90032C
void CPTHistory::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTHistory)
	DDX_Control(pDX, IDC_BTN_PTHISTORY_SEL_COPY, m_btnCopy);
	DDX_Control(pDX, IDC_BTN_PTHISTORY_SEL_NULL, m_btnSelNull);
	DDX_Control(pDX, IDC_BTN_PTHISTORY_SEL_ALL, m_btnSelAll);
	DDX_Control(pDX, IDC_BTN_PTHISTORY_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_LIST_PTHISTORY, m_List);
	DDX_Control(pDX, IDC_GIF_PTHISTORY, m_gif);
	DDX_Control(pDX, IDC_BTN_PTHISTORY_CALL, m_btnCall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTHistory, CViewBase)
	//{{AFX_MSG_MAP(CPTHistory)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_CALL, OnBtnPthistoryCall)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_CMB_PTHISTORY_SRC, OnSelchangeCmbPthistorySrc)
	ON_CBN_SELCHANGE(IDC_CMB_PTHISTORY_TYPE, OnSelchangeCmbPthistoryType)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_PTHISTORY, OnColumnclickListPthistory)
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_SAVE, OnBtnPthistorySave)
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_SEL_NULL, OnBtnPthistorySelNull)
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_SEL_ALL, OnBtnPthistorySelAll)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTHISTORY, OnCustomDraw)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_SEL_COPY, OnBtnPthistorySelCopy)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTHISTORY_CALL, OnDoubleclickedBtnPthistoryCall)
	ON_BN_CLICKED(IDC_BTN_PTHISTORY_FAULT, OnBtnPthistoryFault)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20116, OnSTTP20116)
	ON_MESSAGE(WM_STTP_00901, OnSTTP00901)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(THREAD_WORKEND, OnThreadWorkEnd)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTHistory diagnostics

#ifdef _DEBUG
//##ModelId=49B87B90034B
void CPTHistory::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B90035B
void CPTHistory::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTHistory message handlers

//##ModelId=49B87B90031D
void CPTHistory::OnInitialUpdate() 
{
	//记录位置
	//数据来源
	RecordRate(IDC_STATIC_PTHISTORY_SRC, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTHISTORY_SRC, 0, left_client, top_client);
	//事件类型
	RecordRate(IDC_STATIC_PTHISTORY_TYPE, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTHISTORY_TYPE, 0, left_client, top_client);
	//开始时间
	RecordRate(IDC_STATIC_PTHISTORY_START, 0, left_client, top_client);
	RecordRate(IDC_DP_PTHISTORY_START, 0, left_client, top_client);
	//结束时间
	RecordRate(IDC_STATIC_PTHISTORY_END, 0, left_client, top_client);
	RecordRate(IDC_DP_PTHISTORY_END, 0, left_client, top_client);
	//召唤按钮
	RecordRate(IDC_BTN_PTHISTORY_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTHISTORY_SAVE, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTHISTORY_SEL_ALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTHISTORY_SEL_NULL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTHISTORY_SEL_COPY, 0, left_client, top_client);
	//gif
	RecordRate(IDC_GIF_PTHISTORY, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTHIS, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTHIS_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTHIS_P2, 0, left_client, top_client);
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);

	//初始化数据来源下拉框
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTHISTORY_SRC);
	if(pCmb == NULL)
		return;

	pCmb->AddString(StringFromID(IDS_MODEL_RELAY));
	pCmb->AddString(StringFromID(IDS_MODEL_SUBSTATION));
	pCmb->SetCurSel(1);

	//pCmb->EnableWindow(FALSE);
	
	//初始化事件类型下拉框
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTHISTORY_TYPE);
	if(pCmb == NULL)
		return;
	pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	pCmb->AddString(StringFromID(IDS_EVENTTYPE_ACTION));
	pCmb->AddString(StringFromID(IDS_EVENTTYPE_ALARM));
	pCmb->AddString(StringFromID(IDS_EVENTTYPE_DI));
	pCmb->SetCurSel(0);

	//设置时间框
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_START);
	if(pTime == NULL)
		return;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_END);
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

	//初始化按钮控件
	InitControls();
	//初始化列表框
	InitListCtrl();
}

//##ModelId=49B87B90037A
void CPTHistory::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTHISTORY, left_client, IDC_DP_PTHISTORY_END, right_client, bottom_client);
}

/*************************************************************
 函 数 名：InitControls()
 功能概要：初始化其它控件(除List外的)
 返 回 值: 失败返回-1, 成功返回>=0
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B900246
int CPTHistory::InitControls()
{
	/*
	//召唤按钮
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
//	m_btnCall.SetIcon(IDI_SEARCH);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("召唤历史事件"));

	//保存按钮
	m_btnSave.SetThemeHelper(&m_ThemeHelper);
//	m_btnSave.SetIcon(IDI_SAVE);
	m_btnSave.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnSave.SetTooltipText(_T("保存所选事件"));
	if (g_role)
	{
		m_btnSave.EnableWindow(FALSE);
	}

	//选择所有
	m_btnSelAll.SetThemeHelper(&m_ThemeHelper);
//	m_btnSelAll.SetIcon(IDI_SEARCH);
	m_btnSelAll.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnSelAll.SetTooltipText(_T("选中所有事件"));
	if (g_role)
	{
		m_btnSelAll.EnableWindow(FALSE);
	}

	//取消所有选择
	m_btnSelNull.SetThemeHelper(&m_ThemeHelper);
//	m_btnSelNull.SetIcon(IDI_SEARCH);
	m_btnSelNull.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnSelNull.SetTooltipText(_T("取消所有选中事件"));
	if (g_role)
	{
		m_btnSelNull.EnableWindow(FALSE);
	}
*/
	return 1;
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B900251
int CPTHistory::InitListCtrl()
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
//##ModelId=49B87B900261
int CPTHistory::InitListStyle()
{
	//空图形列表, 用来调整行高
// 	CImageList   m_l;   
// 	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
// 	m_List.SetImageList(&m_l,LVSIL_SMALL);
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
		//char* arColumn[7]={"序号","事件类型", "启动时间", "事件描述", "事件值", "完成时间", "信号量发生时间"};
	
		//分配列宽,最小需求
		for (int nCol=0; nCol < 7 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 80; // 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 100; // 事件类型
				colName = StringFromID(IDS_COMMON_TYPE);
				break;
			case 2://
				lvCol.cx = 200; // 启动时间
				colName = StringFromID(IDS_COMMON_OCCUR_TIME);
				break;
			case 3:
				lvCol.cx = 200; //事件描述
				colName = StringFromID(IDS_EVENT_DESC);
				break;
			case 4:
				lvCol.cx = 80; //事件值
				colName = StringFromID(IDS_EVENT_VALUE);
				break;
			case 5:
				lvCol.cx = 180; //完成时间
				colName = StringFromID(IDS_COMMON_FINISHTIME);
				break;
			case 6:
				lvCol.cx = 180; //信号量发生时间
				colName = StringFromID(IDS_EVENT_SIGNALTIME);
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
	//	m_List.SetColumnHide(0, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B900280
BOOL CPTHistory::LoadListConfig()
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
			pSelected = pDoc->selectSingleNode("//PTHistoryConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到PTHistoryConfig", XJ_LOG_LV3);
				WriteLog("PTHistoryList::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到PTHistoryConfig下的配置", XJ_LOG_LV3);
					WriteLog("PTHistoryConfig::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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

//##ModelId=49B87B900290
BOOL CPTHistory::SaveListConfig()
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
				WriteLog(str, XJ_LOG_LV3);
				WriteLog("PTOSC::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTHistoryConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到PTHistoryConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTHistoryConfig");
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
						WriteLog("未找到PTHistoryConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
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
						WriteLog("找到了PTHistoryConfig下的配置, 先删除新建所有子节点", XJ_LOG_LV3);
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
//##ModelId=49B87B900271
int CPTHistory::FillListData()
{
	CString str;
	str.Format("一共有[%d]条事件", m_nNum - 1);
	WriteLog(str, XJ_LOG_LV3);

	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//填充ListCtrl内容
	POSITION pos = m_EventList.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_EventList.GetNext(pos);
		if(pEvent == NULL)
			continue;
		//加入显示
		AddEventToList(pEvent);
	}
	//恢复刷新
	m_List.SetRedraw(TRUE);
	return 0;
}

//##ModelId=49B87B90029F
BOOL CPTHistory::NeedSave()
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

//##ModelId=49B87B90038A
void CPTHistory::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名：UpdateControlsEnable()
 功能概要：根据当前状态更新控件可用性
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9002AF
void CPTHistory::UpdateControlsEnable()
{
	//按状态分情况
	if(m_nCurrentStatus == 0)
	{	
		//召唤按钮文字改为召唤
		SetDlgItemText(IDC_BTN_PTHISTORY_CALL, StringFromID(IDS_CALLOPER_CALL));
		
		//下拉框
//		GetDlgItem(IDC_CMB_PTHISTORY_SRC) ->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_PTHISTORY_TYPE) ->EnableWindow(TRUE);
		//时间选择框
		GetDlgItem(IDC_DP_PTHISTORY_START) ->EnableWindow(TRUE);
		GetDlgItem(IDC_DP_PTHISTORY_END) ->EnableWindow(TRUE);
		//保存按钮
		if (g_role != MODE_SUB)
		{
			m_btnSave.EnableWindow(TRUE);
			m_btnCopy.EnableWindow(TRUE);
			m_btnSelAll.EnableWindow(TRUE);
			m_btnSelNull.EnableWindow(TRUE);
		}
		//停止动画
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//停止计时
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus  == 1)
	{
		//召唤按钮文字改为取消召唤
		SetDlgItemText(IDC_BTN_PTHISTORY_CALL, CANCEL_CALL);
		
		//下拉框
//		GetDlgItem(IDC_CMB_PTHISTORY_SRC) ->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_PTHISTORY_TYPE) ->EnableWindow(FALSE);
		//时间选择框
		GetDlgItem(IDC_DP_PTHISTORY_START) ->EnableWindow(FALSE);
		GetDlgItem(IDC_DP_PTHISTORY_END) ->EnableWindow(FALSE);
		//保存按钮
		if (g_role != MODE_SUB)
		{
			m_btnSave.EnableWindow(FALSE);
			m_btnCopy.EnableWindow(FALSE);
			m_btnSelAll.EnableWindow(FALSE);
			m_btnSelNull.EnableWindow(FALSE);
		}
		//开始动画
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//开始计时
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTHIS, str);
	}
}

/*************************************************************
 函 数 名：CancelCall()
 功能概要：取消召唤操作
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9002BF
void CPTHistory::CancelCall()
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
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20105, m_pObj->m_sID, 0);
	}
	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_nCurrentStatus = 0;
	//更改控件可用性
	UpdateControlsEnable();

	CString str;
	str.Format("保护[%s]召唤历史事件命令已取消", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);
}

/*************************************************************
 函 数 名：IsVaild20069()
 功能概要：判断收到的20069是否自己的
 返 回 值: 是自己的失败报文返回TRUE, 否则返回FALSE
 参    数：param1	失败的操作报文ID
		   Param2
**************************************************************/
//##ModelId=49B87B9002C0
BOOL CPTHistory::IsVaild20069( int nMsgID )
{
	if(20105 == nMsgID && 1 == m_nCurrentStatus)
	{
		//召唤历史事件
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B900399
void CPTHistory::OnBtnPthistoryCall() 
{
	// TODO: Add your control notification handler code here
	//响应按钮按下操作, 召唤或取消召唤
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
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
	
	if(m_nCurrentStatus == 0)
	{
		//空闲状态, 点击按钮表示要召唤开关量
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_START);
		if(pTime == NULL)
			return;
		CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_END);
		if(pTime2 == NULL)
			return;
		CTime tmStart;
		pTime->GetTime(tmStart);
		CTime tmEnd;
		pTime2->GetTime(tmEnd);
		//组织20105报文
		if(!pApp->m_SttpEngine.BuildDataFor20105HistoryEvent(sttpData, m_pObj->m_sID, m_nEventType, tmStart, tmEnd, m_nSelSrc))
		{
			//组织报文失败
			WriteLog("组织20105报文失败", XJ_LOG_LV3);
			return;
		}
		else
		{
			WriteLog("组织20105报文成功", XJ_LOG_LV3);
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
				str.Format("保护[%s]发送召唤历史事件报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送召唤历史事件报文失败,原因为缓存已满", m_pObj->m_sName);
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
			int nTimeOut = pData->m_nHistoryEventTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//根据当前状态更新控件可用性
			UpdateControlsEnable();
			
			//删除之前的列表
			m_List.DeleteAllItems();
			ClearAllEvent();
			m_nNum = 1;

			//启动线程
			StartParse20116Thread();

			CString str;
			str.Format("保护[%s]发送召唤历史事件报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else 
	{
		//正在召唤开关量
		//退出线程
		EndParse20116Thread();
		//取消召唤
		CancelCall();
	}
}

/*************************************************************
 函 数 名：OnPTFrameOpen()
 功能概要：响应保护属性性打开消息, 关联设备
 返 回 值: void
 参    数：param1
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B9003C0
void CPTHistory::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;
	int nIndex = (int)wParam;

	m_List.DeleteAllItems();
	//清除所有事件
	ClearAllEvent();
	//清除开关量定义
	ClearAllDI();
	m_nNum = 1;
	m_nActionIndex = 0;

	//页面还原
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	
	SetDlgItemText(IDC_STATIC_TIME_PTHIS, "");
}

/*************************************************************
 函 数 名：OnSTTP20069()
 功能概要：响应20069(操作失败)报文
 返 回 值: void
 参    数：param1
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B9003C9
void CPTHistory::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤的状态,不处理报文
		//WriteLog("CPTHistory::OnSTTP20069::没有处于召唤的状态,不处理报文", 3);
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
        WriteLog("CPTHistory::OnSTTP20069::报文为空", XJ_LOG_LV3);
		return;
	}
	
	//检查是否20069报文
	if(pSttpData->sttp_head.uMsgID != 20069)
	{
		//错误报文
        WriteLog("CPTHistory::OnSTTP20069::错误报文", XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
        WriteLog("CPTHistory::OnSTTP20069::不是本装置的报文", XJ_LOG_LV3);
		return;
	}
	
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本装置的报文
		WriteLog("CPTHistory::OnSTTP20069::IsVaild20069", XJ_LOG_LV3);
		return;
	}
	
	//关闭定时器
	KillTimer(m_nTimer);
	
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
		str.Format("保护[%s]召唤历史事件失败!", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);
	}
	
	//退出线程
	EndParse20116Thread();
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
	
	//根据当前状态更新控件可用性
	UpdateControlsEnable();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_HISTORY);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLHEVENT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_HISTORY, OPER_FAILD);

	AfxMessageBox( StringFromID(IDS_CALL_FAIL));
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：响应20125(装置不支持功能)报文
 返 回 值: void
 参    数：param1
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B9003D9
void CPTHistory::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤状态,不处理报文
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
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}
	
	//检查是否本功能的报文
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}
	
	//关闭定时器
	KillTimer(m_nTimer);
	
	//退出线程
	EndParse20116Thread();
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
	
	//根据当前状态更新控件可用性
	UpdateControlsEnable();

	CString str;
	str.Format("保护[%s]召唤历史事件失败,原因为:装置不支持", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV1);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_HISTORY);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLHEVENT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_HISTORY, OPER_FAILD);
	
	AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
}

//##ModelId=49B87B9003A9
void CPTHistory::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		KillTimer(m_nTimer);
		if(m_nCurrentStatus == 1)
		{
			//退出线程
			EndParse20116Thread();
			//改变状态
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
			CString str;
			str.Format("保护[%s]召唤历史事件失败,原因为:超时", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d", OPER_FAILD, XJ_OPER_CALL_HISTORY);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLHEVENT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_HISTORY, OPER_FAILD);
			//提示
			AfxMessageBox( StringFromID(IDS_CALLFAIL_TIMEOUT));
		}
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//改变持续时间显示
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTHIS, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：OnSTTP20116()
 功能概要：响应保护历史事件下载通知, 生成事件
 返 回 值: void
 参    数：param1
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B910000
void CPTHistory::OnSTTP20116( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CPTHistory::OnSTTP20116 start", XJ_LOG_LV3);
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20116报文
	if(pSttpData->sttp_head.uMsgID != 20116)
	{
		//错误报文
		CString str;
		str.Format("CPTHistory::OnSTTP20116 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTHistory::OnSTTP20116 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//保存
	AddTail20116SttpData(*pSttpData);
	WriteLog("CPTHistory::OnSTTP20116 neat end", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：AddEventToList()
 功能概要：把指定事件加入到列表框中显示
 返 回 值: 
 参    数：param1	指定事件
		   Param2	指定列表行索引
**************************************************************/
//##ModelId=49B87B9002CE
void CPTHistory::AddEventToList( CFaultEvent* pEvent, int nIndex /*= 0*/ )
{
	//检查数据有效性
	MYASSERT(pEvent);
	if(nIndex < 0)
		return;
	//"序号","事件类型", "发生时间", "事件描述", "事件值"
	CString str = "";
	str.Format("%d", pEvent->m_nID);
	//0:序号
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1:事件类型
	str = ""; 
	//1-动作信息 2－自检信息 3－压板信息
	if(pEvent->m_nType == 1)
	{
		str = StringFromID(IDS_ACTION_CHR);
	}
	else if(pEvent->m_nType == 2)
	{
		str = StringFromID(IDS_EVENTTYPE_ALARM);
	}
	else if(pEvent->m_nType == 3)
	{
		str = StringFromID(IDS_EVENTTYPE_DI);
	}
	else if(pEvent->m_nType == 99)
	{
		str = StringFromID(IDS_ACTION_SIGNAL);
	}
	m_List.SetItemText(nIndex, 1, str);

	//2:启动时间
	str.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
	m_List.SetItemText(nIndex, 2, str);
	//3:事件描述
	m_List.SetItemText(nIndex, 3, pEvent->m_sEventDefName);
	//4:事件值
	str = pEvent->m_sEventContent;
	if(pEvent->m_nType == 1)
	{
		//特征值
		if(pEvent->m_strUnit != "" && pEvent->m_sEventContent != "")
			str.Format("%s(%s)", pEvent->m_sEventContent, pEvent->m_strUnit);
	}
	else if(pEvent->m_nType == 3)
	{
		int nResult = atoi(pEvent->m_sEventContent);
		if(nResult == 0)
			str = "OFF";
		else
			str = "ON";
	}
	else
	{
		int nResult = atoi(pEvent->m_sEventContent);
		if(nResult == 0)
			str = StringFromID(IDS_CASE_RETURN);
		else
			str = StringFromID(IDS_CASE_ACTION);
	}
	m_List.SetItemText(nIndex, 4, str);

	if(pEvent->m_nType != 3)
	{
		//5.完成时间
		str.Format("%s.%03d", pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsReceive);
		m_List.SetItemText(nIndex, 5, str);
	}

	if(pEvent->m_nType == 99)
	{
		//6.信号量时间
		str.Format("%s.%03d", pEvent->m_tmSign.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsSign);
		m_List.SetItemText(nIndex, 6, str);
	}


	//关联设备
	m_List.SetItemData(nIndex, (DWORD)pEvent);
}

/*************************************************************
 函 数 名：TranslateHistoryAction()
 功能概要：解析历史动作信息
 返 回 值: 
 参    数：param1	报文数据
		   Param2
**************************************************************/
//##ModelId=49B87B9002DE
void CPTHistory::TranslateHistoryAction( STTP_FULL_DATA* pSttpData, int i)
{
	//检查数据有效性
	MYASSERT(pSttpData);
	MYASSERT(m_pObj);

	//保护装置ID
	CString strID = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved1);
	if(strID != m_pObj->m_sID)
		return;
	//启动时间
	CString strTime1 = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved2);
	int nms1 = 0;
	CTime tmTime1 = StringToTimeSttp15(strTime1, nms1);
	//完成时间
	CString strTime2 = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved3);
	int nms2 = 0;
	CTime tmTime2 = StringToTimeSttp15(strTime2, nms2);
	
	//循环特征值
	int nChrCount = pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list.size();
	for(int k = 0; k < nChrCount; k++)
	{
		//创建事件
		CFaultEvent* pEvent = new CFaultEvent;
		pEvent->m_nNum = m_nActionIndex;
		pEvent->m_sSecID = strID;
		pEvent->m_pSec = m_pObj;
		pEvent->m_nType = 1;
		pEvent->m_tmStart = tmTime1;
		pEvent->m_nmsStart = nms1;
		pEvent->m_tmReceiveTime = tmTime2;
		pEvent->m_nmsReceive = nms2;
		pEvent->m_sEventDef.Format("%d", pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].nDataType);
		pEvent->m_sEventContent = ComplexExchange(XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].strValue));
		//查找特征值名称
		pEvent->m_sEventDefName = GetChrName(atoi(pEvent->m_sEventDef), pEvent->m_strUnit);
		
		//加上序号
		pEvent->m_nID = m_nNum++;
		//加入到链表
		m_EventList.AddTail(pEvent);
		//加入到列表显示
		//	AddEventToList(pEvent);
	}
	
	//循环信号量
	int nSignCount = pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].condition_list.size();
	for(int m = 0; m < nSignCount; m++)
	{
		//创建事件
		CFaultEvent* pEvent = new CFaultEvent;
		pEvent->m_nNum = m_nActionIndex;
		pEvent->m_sSecID = strID;
		pEvent->m_pSec = m_pObj;
		pEvent->m_nType = 99;
		pEvent->m_tmStart = tmTime1;
		pEvent->m_nmsStart = nms1;
		pEvent->m_tmReceiveTime = tmTime2;
		pEvent->m_nmsReceive = nms2;
		pEvent->m_sEventDef.Format("%d", pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].condition_list[m].nDataType);
		pEvent->m_sEventContent = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].condition_list[m].strValue);
		pEvent->m_tmSign = StringToTimeSttp15(XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].condition_list[m].strReserved), pEvent->m_nmsSign);
		//查找信号量名称
		pEvent->m_sEventDefName = GetActionSignName(atoi(pEvent->m_sEventDef));
		
		//加上序号
		pEvent->m_nID = m_nNum++;
		//加入到链表
		m_EventList.AddTail(pEvent);
		//加入到列表显示
		//	AddEventToList(pEvent);
	}
	m_nActionIndex++;
}


/*************************************************************
 函 数 名：ClearAllEvent()
 功能概要：删除所有事件记录
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9002F0
void CPTHistory::ClearAllEvent()
{
	POSITION pos = m_EventList.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_EventList.GetNext(pos);
		if(pEvent == NULL)
			continue;
		delete pEvent;
	}
	m_EventList.RemoveAll();
}

/*************************************************************
 函 数 名：GetChrName()
 功能概要：查找指定特征值描述
 返 回 值: 特征值描述
 参    数：param1	特征值ID
		   Param2	单位
**************************************************************/
//##ModelId=49B87B9002F1
CString CPTHistory::GetChrName( int nID, CString& strUnit )
{
	//检查数据有效性
	if(m_pObj == NULL)
		return "";
	//载入保护配置
	m_pObj->RefreshConfig();
	//查找
	PT_ActionCharacter* pAC = (PT_ActionCharacter*)m_pObj->FindActionCharacter(nID);
	if(pAC)
	{
		strUnit = pAC->strUnits;
		return pAC->strName;
	}
	
	//没查找到, 在通用中查找
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp ->GetDataEngine();
	PT_ActionCharacter *pActChr;
	pActChr = NULL;
	for (int i = 0; i < pData->m_arrUniversalActChr.GetSize(); i ++)
	{
        pActChr = (PT_ActionCharacter *) pData->m_arrUniversalActChr[i];
		if (nID == pActChr->nID)
		{
			strUnit = pActChr->strUnits;
			return pActChr->strName;
		}
	}
	return "";
}

/*************************************************************
 函 数 名：GetActionSignName()
 功能概要：查找指定信号量描述
 返 回 值: 信号量描述
 参    数：param1	信号量ID
		   Param2
**************************************************************/
//##ModelId=49B87B9002FE
CString CPTHistory::GetActionSignName( int nID )
{
	//检查数据有效性
	if(m_pObj == NULL)
		return "";
	//载入保护配置
	m_pObj->RefreshConfig();
	//查找
	PT_Signal* pAC = (PT_Signal*)m_pObj->FindActionSignal(nID);
	
	if(pAC)	
		return pAC->strName;

	return "";
}

/*************************************************************
 函 数 名：TranslateHistoryAlarm()
 功能概要：解析历史告警信息
 返 回 值: 
 参    数：param1 报文数据
		   Param2
**************************************************************/
//##ModelId=49B87B9002E0
void CPTHistory::TranslateHistoryAlarm( STTP_FULL_DATA* pSttpData, int i )
{
	//检查数据有效性
	MYASSERT(pSttpData);
	MYASSERT(m_pObj);
		
	//保护装置ID
	CString strID = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved1);
	if(strID != m_pObj->m_sID)
		return;
	
	//循环信号量
	int nChrCount = pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list.size();
	for(int k = 0; k < nChrCount; k++)
	{
		//创建事件
		CFaultEvent* pEvent = new CFaultEvent;
		pEvent->m_sSecID = strID;
		pEvent->m_pSec = m_pObj;
		pEvent->m_nType = 2;
		CString strTime = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].strReserved);
		pEvent->m_tmStart = StringToTimeSttp15(strTime, pEvent->m_nmsStart);
		strTime = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved2);
		pEvent->m_tmReceiveTime = StringToTimeSttp15(strTime, pEvent->m_nmsReceive);
		pEvent->m_sEventDef.Format("%d", pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].nDataType);
		pEvent->m_sEventContent = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].strValue);
		//查找信号量名称
		pEvent->m_sEventDefName = GetAlarmSignName(atoi(pEvent->m_sEventDef));
		
		pEvent->m_nID = m_nNum++;
		//加入到链表
		m_EventList.AddTail(pEvent);
		//加入到列表显示
		//	AddEventToList(pEvent);
	}
}

/*************************************************************
 函 数 名：GetAlarmSignName()
 功能概要：查找指定告警信号量的描述
 返 回 值: 信号量描述
 参    数：param1	信号量ID
		   Param2
**************************************************************/
//##ModelId=49B87B900300
CString CPTHistory::GetAlarmSignName( int nID )
{
	//检查数据有效性
	if(m_pObj == NULL)
		return "";
	//载入保护配置
	m_pObj->RefreshConfig();
	//查找
	PT_Signal* pAC = (PT_Signal*)m_pObj->FindAlarmSignal(nID);
	if(pAC)
	{
		return pAC->strName;
	}
	
	return "";
}

/*************************************************************
 函 数 名：ClearAllDI()
 功能概要：清除所有开关量定义
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B90030E
void CPTHistory::ClearAllDI()
{
	for(int i = 0; i < m_arrDigital.GetSize(); i++)
	{
		PT_DIGITAL* ptg = (PT_DIGITAL*)m_arrDigital.GetAt(i);
		if(ptg == NULL)
			continue;
		delete ptg;
	}
	m_arrDigital.RemoveAll();
}

/*************************************************************
 函 数 名：GetPTDI()
 功能概要：从数据库载入本保护的所有开关量定义
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B90030F
BOOL CPTHistory::GetPTDI(int nCPU)
{
	BOOL bReturn = TRUE;

	if(m_pObj == NULL)
		return FALSE;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据PT_ID, cpu_code查找DI_ID,CODE_NAME,NAME,按DI_ID从小到大排列
		
	CString str;
		
	//字段
	//DI_ID
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "DI_ID";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//name
	Field Field2;
	bzero(&Field2, sizeof(Field));
	str = "NAME";
	strncpy(Field2.FieldName, str, str.GetLength());
	Field2.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field2);

	//code_name
	Field Field3;
	bzero(&Field3, sizeof(Field));
	str = "CODE_NAME";
	strncpy(Field3.FieldName, str, str.GetLength());
	Field3.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field3);

	//cpu_code
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "CPU_CODE";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field4);
		
	//条件
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	/*
	//CPU
	Condition CPU;
	bzero(&CPU, sizeof(Condition));
	str.Format("cpu_code = %d", nCPU);
	strncpy(CPU.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(CPU);
	*/

	//按DI_ID大小排序
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by DI_ID");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition3);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTHistory::GetPTDI, 查询失败");
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
			//创建开关量对象
			PT_DIGITAL * digital = new PT_DIGITAL;
			digital->ID = pMemset->GetValue((UINT)0); //DI_ID
			digital->Name = pMemset->GetValue(1); //NAME
			digital->CodeName = pMemset->GetValue(2); //Code_Name
			digital->CpuCode = atoi(pMemset->GetValue(3));
			m_arrDigital.Add(digital);
					
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTHistory::GetPTDI,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}

/*************************************************************
 函 数 名：GetDIName()
 功能概要：得到指定开关量的描述
 返 回 值: 开关量描述
 参    数：param1	开关量ID
		   Param2
**************************************************************/
//##ModelId=49B87B900310
CString CPTHistory::GetDIName( CString sID, int nCPU )
{
	//检查数据有效性
	if(m_pObj == NULL)
		return "";
	if(m_arrDigital.GetSize() == 0)
		GetPTDI(nCPU);
	for(int i = 0; i < m_arrDigital.GetSize(); i++)
	{
		PT_DIGITAL* ptg = (PT_DIGITAL*)m_arrDigital.GetAt(i);
		if(ptg == NULL)
			continue;
		if(ptg->ID == sID && ptg->CpuCode == nCPU)
			return ptg->Name;
	}
	return "";
}

/*************************************************************
 函 数 名：TranslateHistoryDI()
 功能概要：解析历史开关量信息
 返 回 值: 
 参    数：param1	报文数据
		   Param2
**************************************************************/
//##ModelId=49B87B9002EE
void CPTHistory::TranslateHistoryDI( STTP_FULL_DATA* pSttpData, int i)
{
	//检查数据有效性
	if(pSttpData == NULL)
		return;
	if(m_pObj == NULL)
		return;
	//保护装置ID
	CString strID = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved1);
	if(strID != m_pObj->m_sID)
		return;
	//启动时间
	CString strTime1 = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].strReserved2);
	int nms1 = 0;
	CTime tmTime1 = StringToTimeSttp15(strTime1, nms1);
	//CPU
	int nCPU = pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].nReserved;
	
	//循环特征值
	int nChrCount = pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list.size();
	for(int k = 0; k < nChrCount; k++)
	{
		//创建事件
		CFaultEvent* pEvent = new CFaultEvent;
		pEvent->m_sSecID = strID;
		pEvent->m_pSec = m_pObj;
		pEvent->m_nType = 3;
		pEvent->m_tmStart = tmTime1;
		pEvent->m_nmsStart = nms1;
		pEvent->m_nCPU = nCPU;
		pEvent->m_sEventDef.Format("%d", pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].nDataType);
		pEvent->m_sEventContent = XJToMS(pSttpData->sttp_body.variant_member.dataflat_data.record_list[i].field_list[k].strValue);
		//查找开关量名称
		pEvent->m_sEventDefName = GetDIName(pEvent->m_sEventDef, nCPU);
		if (pEvent->m_sEventDefName == "")
		{
			WriteLog("CPTHistory::TranslateHistoryDI::未查找到开关量定义 " + pEvent->m_sSecID + " " + pEvent->m_sEventDef, XJ_LOG_LV3);
		}
		
		//加上序号
		pEvent->m_nID = m_nNum++;
		//加入到链表
		m_EventList.AddTail(pEvent);
		//加入到列表显示
		//	AddEventToList(pEvent);
	}
}

/*************************************************************
 函 数 名：OnSTTP00901()
 功能概要：收到00901报文, 召唤历史事件完毕
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B910013
void CPTHistory::OnSTTP00901( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CPTHistory::OnSTTP00901 start", XJ_LOG_LV3);
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤的状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否00901报文
	if(pSttpData->sttp_head.uMsgID != 901)
	{
		//错误报文
		return;
	}
	
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}
	
	//关闭定时器
	KillTimer(m_nTimer);
	
	//通知解析线程报文结束
	m_bDataEnd = TRUE;

	CString str;
	str.Format("保护[%s]召唤历史事件成功!", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d", OPER_SUCCESS, XJ_OPER_CALL_HISTORY);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLHEVENT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_HISTORY, OPER_SUCCESS);

	WriteLog("CPTHistory::OnSTTP00901 neat end", XJ_LOG_LV3);
}

//##ModelId=49B87B9003AC
void CPTHistory::OnSelchangeCmbPthistorySrc() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTHISTORY_SRC);
	if(pCmb == NULL)
		return;
	m_nSelSrc = pCmb->GetCurSel();
}

//##ModelId=49B87B9003BA
void CPTHistory::OnSelchangeCmbPthistoryType() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTHISTORY_TYPE);
	if(pCmb == NULL)
		return;
	m_nEventType = pCmb->GetCurSel();
}

/*************************************************************
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果
 参    数：param1	要比较的对象1
		   Param2	要比较的对象2
		   Param3	比较参数
**************************************************************/
//##ModelId=49B87B900234
int CALLBACK CPTHistory::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CFaultEvent* pEvent1 = (CFaultEvent*)lParam1;
	CFaultEvent* pEvent2 = (CFaultEvent*)lParam2;
	if(pEvent1 == NULL || pEvent2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int type1, type2;
	int iResult = 0;
	//"序号","事件类型", "发生时间", "事件描述", "事件值"
	switch(nCol)
	{
	case 0: //序号
		iResult = pEvent1->m_nID - pEvent2->m_nID;
		break;
	case 1: //事件类型
		type1 = pEvent1->m_nType;
		type2 = pEvent2->m_nType;
		if(type1 == 99)
			type1 = 1;
		if(type2 == 99)
			type2 = 1;
		iResult = type1- type2;
		break;
	case 2: //发生时间
		str1 = pEvent1->m_tmStart.Format("%Y-%m-%d %H:%M:%S");
		str2 = pEvent2->m_tmStart.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //事件描述
		str1 = pEvent1->m_sEventDefName;
		str2 = pEvent2->m_sEventDefName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4: //事件值
		str1 = pEvent1->m_sEventContent;
		str2 = pEvent2->m_sEventContent;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iPTHistoryAsc;
	return iResult;
}

//##ModelId=49B87B9003BC
void CPTHistory::OnColumnclickListPthistory(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iPTHistoryAsc = - g_iPTHistoryAsc;
	}
	else
	{
		g_iPTHistoryAsc = 1;
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
//##ModelId=49B87B91000F
void CPTHistory::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤状态,不处理报文
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
	if(pSttpData->sttp_head.uMsgID != 20157)
	{
		//错误报文
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		return;
	}
	
	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
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
		//正在召唤历史事件				
		int nTimeOut = pData->m_nHistoryEventTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}

void CPTHistory::OnBtnPthistorySave() 
{
	// TODO: Add your control notification handler code here
	//取得选择项
	HISTORYEVENT_LIST listAction; 
	HISTORYEVENT_LIST listAlarm; 
	HISTORYEVENT_LIST listDI;
	listAction.RemoveAll();
	listAlarm.RemoveAll();
	listDI.RemoveAll();

	GetSelected(&listAction, &listAlarm, &listDI);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	MYASSERT(pApp);

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

	//告警
	SendAlarm(&listAlarm);
	//开关量
	SendDI(&listDI);
	//动作
	SendAction(&listAction);

	AfxMessageBox( StringFromID(IDS_TIP_SAVE_FINISHED));
}

/*************************************************************
 函 数 名: GetSelected()
 功能概要: 取得选择项
 返 回 值: 
 参    数: param1 保存所选动作事件
		   Param2 保存所选告警事件
		   Param3 保存所选开关量事件
**************************************************************/
void CPTHistory::GetSelected( HISTORYEVENT_LIST* listAction, HISTORYEVENT_LIST* listAlarm, HISTORYEVENT_LIST* listDI )
{
	UpdateData(TRUE);
	//检查数据有效性
	MYASSERT(listAction);
	MYASSERT(listAlarm);
	MYASSERT(listDI);
	//取得选择
	for(int i = 0; i < m_List.GetItemCount(); i++)
	{
		if(m_List.GetCheck(i))
		{
			//被选择
			CFaultEvent* pEvent = (CFaultEvent*)m_List.GetItemData(i);
			MYASSERT_CONTINUE(pEvent);
			if(pEvent->m_nType == 1 || pEvent->m_nType == 99)
			{
				//动作
				listAction->AddTail(pEvent);
			}
			else if(pEvent->m_nType == 2)
			{
				//告警
				listAlarm->AddTail(pEvent);
			}
			else if(pEvent->m_nType == 3)
			{
				//开关量
				listDI->AddTail(pEvent);
			}
		}
	}
}

/*************************************************************
 函 数 名: SendAlarm()
 功能概要: 发送告警报文
 返 回 值: 
 参    数: param1 告警事件链表
		   Param2 
**************************************************************/
void CPTHistory::SendAlarm( HISTORYEVENT_LIST* listAlarm )
{
	//检查数据有效性
	MYASSERT(listAlarm);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	
	//组织报文
	if(!pApp->m_SttpEngine.BuildDataFor20116FaultAlarm(sttpData, listAlarm))
	{
		//组织报文失败
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
			str.Format("保护%s发送保存历史告警事件报文失败,原因为连接中断", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("保护%s发送保存历史告警事件报文失败,原因为缓存已满", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == 0)
		{
			CString str;
			str.Format("保护%s发送保存历史告警事件报文失败,原因为无效数据", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
}

/*************************************************************
 函 数 名: SendDI()
 功能概要: 发送开关量报文
 返 回 值: 
 参    数: param1 开关量事件链表
		   Param2 
**************************************************************/
void CPTHistory::SendDI( HISTORYEVENT_LIST* listDI )
{
	//检查数据有效性
	MYASSERT(listDI);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	
	//组织报文
	if(!pApp->m_SttpEngine.BuildDataFor20116FaultDI(sttpData, listDI))
	{
		//组织报文失败
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
			str.Format("保护%s发送保存历史开关量事件报文失败,原因为连接中断", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("保护%s发送保存历史开关量事件报文失败,原因为缓存已满", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == 0)
		{
			CString str;
			str.Format("保护%s发送保存历史开关量事件报文失败,原因为无效数据", m_pObj->m_sID);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
}

/*************************************************************
 函 数 名: SendAction()
 功能概要: 发送动作报文
 返 回 值: 
 参    数: param1 动作事件链表
		   Param2 
**************************************************************/
void CPTHistory::SendAction( HISTORYEVENT_LIST* listAction )
{
	//检查数据有效性
	MYASSERT(listAction);

	//记录已保存的事件
	CWordArray arrActionNum;
	arrActionNum.RemoveAll();
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	POSITION pos = listAction->GetHeadPosition();
	while(pos != NULL)
	{
		CFaultEvent* pEvent = (CFaultEvent*)listAction->GetNext(pos);
		MYASSERT_CONTINUE(pEvent);
		//取得动作号
		int nActionNum = pEvent->m_nNum;
		//查找是否已保存过
		BOOL bSaved = FALSE;
		for(int i = 0; i < arrActionNum.GetSize(); i++)
		{
			int nIndex = arrActionNum.GetAt(i);
			if(nIndex == nActionNum)
			{
				bSaved = TRUE;
				break;
			}
		}
		//以保存过的不再处理
		if(bSaved)
			continue;
		arrActionNum.Add(nActionNum);
		//取得所有相关的事件
		HISTORYEVENT_LIST listSave;
		listSave.RemoveAll();
		POSITION pos1 = m_EventList.GetHeadPosition();
		while(pos1 != NULL)
		{
			CFaultEvent* pAction = (CFaultEvent*)m_EventList.GetNext(pos1);
			MYASSERT_CONTINUE(pAction);
			if(pAction->m_nType != 1 && pAction->m_nType != 99)
				continue;
			if(pAction->m_nNum != nActionNum)
				continue;
			listSave.AddTail(pAction);
		}

		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		//组织报文
		if(!pApp->m_SttpEngine.BuildDataFor20116FaultAction(sttpData, &listSave))
		{
			//组织报文失败
			continue;
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
				str.Format("保护%s发送保存历史动作事件报文失败,原因为连接中断", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护%s发送保存历史动作事件报文失败,原因为缓存已满", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == 0)
			{
				CString str;
				str.Format("保护%s发送保存历史动作事件报文失败,原因为无效数据", m_pObj->m_sID);
				WriteLog(str, XJ_LOG_LV2);
			}
		}
	}

}

void CPTHistory::OnBtnPthistorySelNull() 
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < m_List.GetItemCount(); i++)
	{
		m_List.SetCheck(i, FALSE);
	}
}

void CPTHistory::OnBtnPthistorySelAll() 
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < m_List.GetItemCount(); i++)
	{
		m_List.SetCheck(i, TRUE);
	}
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置差异时list有文本颜色
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
void CPTHistory::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				if(nItem > -1)
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

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: AddTail20116SttpData()
 功能概要: 将20116报文加入到链表尾
 返 回 值: 
 参    数: param1 20116报文数据
		   Param2 
**************************************************************/
void CPTHistory::AddTail20116SttpData( STTP_FULL_DATA& sttpData )
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
 函 数 名: RemoveHead20116SttpData()
 功能概要: 删除20116报文链表的第一项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTHistory::RemoveHead20116SttpData()
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
 函 数 名: StartParse20116Thread()
 功能概要: 启动解析20026报文线程
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTHistory::StartParse20116Thread()
{
	m_bExitParse = FALSE;
	m_bDataEnd = FALSE;
	//清除残留报文
	m_listFullData.RemoveAll();
	//启动
	m_pParse20116Thread = AfxBeginThread(PTParse20116,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
	if(m_pParse20116Thread != NULL)
	{
		m_pParse20116Thread->m_bAutoDelete = FALSE;
		m_pParse20116Thread->ResumeThread();
		WriteLog("CPTHistory::StartParse20116Thread, 线程启动成功", XJ_LOG_LV3);
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: EndParse20116Thread()
 功能概要: 强制停止20026解析报文
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTHistory::EndParse20116Thread()
{
	//退出线程
	if(m_pParse20116Thread != NULL)
	{
		//线程还没退出去
		m_bExitParse = TRUE;	//设置退出标志
		WaitForSingleObject(m_pParse20116Thread->m_hThread, INFINITE);
		delete m_pParse20116Thread;
		m_pParse20116Thread = NULL;
		WriteLog("CPTHistory::EndParse20116Thread, 线程退出成功", XJ_LOG_LV3);
	}
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: OnThreadWorkEnd()
 功能概要: 线程通知界面，报文处理完毕
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTHistory::OnThreadWorkEnd( WPARAM wParam, LPARAM lParam )
{
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
	
	//加入显示
	FillListData();
	
	//根据当前状态更新控件可用性
	UpdateControlsEnable();
}

/*************************************************************
 CreateDate: 15:1:2010     Author:LYH
 函 数 名: OnPTFrameClose()
 功能概要: 响应窗口关闭消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CPTHistory::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	//退出线程
	EndParse20116Thread();
}

void CPTHistory::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (nSBCode == SB_ENDSCROLL)
	{
		this->SendMessage(WM_SIZE);
	}
	CViewBase::OnHScroll(nSBCode, nPos, pScrollBar);
	
}

BOOL CPTHistory::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CViewBase::OnScrollBy(sizeScroll, bDoScroll);
}

void CPTHistory::OnBtnPthistorySelCopy() 
{
	// TODO: Add your control notification handler code here
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
		    CFaultEvent* pEvent = (CFaultEvent*)m_List.GetItemData(nIndex);
			for (int j = 0; j < m_List.GetHeaderCtrl()->GetItemCount(); j++)
			{
				char sz[512] = {0};
				m_List.GetItemText(nIndex, j, sz, 512);
				str += sz;
				str += "   ";
			}
		}
		str += "\r\n";
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

void CPTHistory::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTHistory::OnDoubleclickedBtnPthistoryCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTHistory::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_HISTORYEVENT));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_HISTORYEVENT));
	
	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CPTHistory::OnBtnPthistoryFault() 
{
	// TODO: Add your control notification handler code here
	CDlgSelFault dlg;
	dlg.SetSecObj(m_pObj);
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_START);
	if(pTime == NULL)
		return;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTHISTORY_END);
	if(pTime2 == NULL)
		return;
	CTime tmStart;
	pTime->GetTime(tmStart);
	CTime tmEnd;
	pTime2->GetTime(tmEnd);
	dlg.SetQueryTime(tmStart, tmEnd);

	int selFault = -1;

	if( dlg.DoModal() == IDOK )
	{
		CFaultReport* report = dlg.GetFault();

		if( NULL == report )
		{
			return;
		}
		report->LoadDetail();

		HISTORYEVENT_LIST listAction; 
		HISTORYEVENT_LIST listAlarm; 
		HISTORYEVENT_LIST listDI;
		listAction.RemoveAll();
		listAlarm.RemoveAll();
		listDI.RemoveAll();
		
		GetSelected(&listAction, &listAlarm, &listDI);

		POSITION pos = listAction.GetHeadPosition();
		while(pos != NULL)
		{
			CFaultEvent* pEvent = (CFaultEvent*)listAction.GetNext(pos);
			MYASSERT_CONTINUE(pEvent);
			CFaultEvent* pClone = pEvent->Clone();
			pClone->m_nMainRecID = report->m_nID;
			if( 1 == pClone->m_nType )
			{
				//特征量
				report->m_listChr.Add(pClone);
			}
			if( pClone->m_nType ==  99 )
			{
				pClone->m_nType = 2; //动作信号量
				report->m_listSign.Add(pClone);
			}
		}
		
		pos = listAlarm.GetHeadPosition();
		while(pos != NULL)
		{
			CFaultEvent* pEvent = (CFaultEvent*)listAlarm.GetNext(pos);
			MYASSERT_CONTINUE(pEvent);
			CFaultEvent* pClone = pEvent->Clone();
			pClone->m_nMainRecID = report->m_nID;
			pClone->m_nType = 3;
			report->m_listAlarm.Add(pClone);
		}

		pos = listDI.GetHeadPosition();
		while(pos != NULL)
		{
			CFaultEvent* pEvent = (CFaultEvent*)listDI.GetNext(pos);
			MYASSERT_CONTINUE(pEvent);
			CFaultEvent* pClone = pEvent->Clone();
			pClone->m_nMainRecID = report->m_nID;
			pClone->m_nType = 8;
			report->m_listDIChange.Add(pClone);
		}

		//save
		if( 0 != report->MultiSaveEvent() )
		{
			AfxMessageBox( StringFromID(IDS_COMMON_UPDATEDB_FAILED));
		}else
		{
			AfxMessageBox( StringFromID(IDS_FAULTREPORT_UPDATE_SUCCESS));
		}

		report->ClearDetail();

	}
}
