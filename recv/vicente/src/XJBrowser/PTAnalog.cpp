// PTAnalog.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTAnalog.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

UINT PTAnalogLoad(LPVOID pParam)
{
	CPTAnalog* ptAnalog = (CPTAnalog*)pParam;
	if(ptAnalog == NULL)
		return 1;
	int nQueryNo = ptAnalog->GetCurrentQueryNo();
	
	//碰到退出线程,退出
	if(!ptAnalog->m_bExitThread && ptAnalog->IsCurrentQueryNo(nQueryNo))
	{
		if(ptAnalog->LoadPTAnalogConfig(nQueryNo))
		{
			ptAnalog->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
		}
		else
		{
			ptAnalog->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
		}
	}
	
	ptAnalog->RemoveThreadNode(nQueryNo);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTAnalog

IMPLEMENT_DYNCREATE(CPTAnalog, CViewBase)

//##ModelId=49B87B930159
CPTAnalog::CPTAnalog()
	: CViewBase(CPTAnalog::IDD)
{
	//{{AFX_DATA_INIT(CPTAnalog)
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_sCPU = "";
	m_nRealtimeValueIndex = 4;
	m_nNewValueIndex = 5;
	m_nRealtimeValueTimer = 0;
	m_nTimer = 0;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_arrColum.RemoveAll();
	m_nCurrentStatus = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_nQueryNo = 0;
	m_listThread.RemoveAll();

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B930341
CPTAnalog::~CPTAnalog()
{
	ClearAnalogConfig();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B93031C
void CPTAnalog::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTAnalog)
	DDX_Control(pDX, IDC_BTN_PTANALOG_EXCEL, m_btnExcel);
	DDX_Control(pDX, IDC_GIF_PTANALOG, m_gif);
	DDX_Control(pDX, IDC_BTN_PTANALOG_STYLE, m_btnStyle);
	DDX_Control(pDX, IDC_BTN_PTANALOG_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTANALOG_CALL, m_btnCall);
	DDX_Control(pDX, IDC_LIST_PTANALOG, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTAnalog, CViewBase)
	//{{AFX_MSG_MAP(CPTAnalog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTANALOG_CALL, OnBtnPtanalogCall)
	ON_CBN_SELCHANGE(IDC_CMB_PTANALOG_CPU, OnSelchangeCmbPtanalogCpu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTANALOG, OnCustomDraw)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTANALOG_STYLE, OnBtnPtanalogStyle)
	ON_COMMAND(ID_ROUTINEPRINT_NEW1, OnRoutineprintNew1)
	ON_COMMAND(ID_ROUTINEPRINT_NEW2, OnRoutineprintNew2)
	ON_COMMAND(ID_ROUTINEPRINT_NEW3, OnRoutineprintNew3)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTANALOG_CALL, OnDoubleclickedBtnPtanalogCall)
	ON_BN_CLICKED(IDC_BTN_PTANALOG_EXCEL, OnBtnPtanalogExcel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20008, OnSTTP20008)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTAnalog diagnostics

#ifdef _DEBUG
//##ModelId=49B87B93035B
void CPTAnalog::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B93036B
void CPTAnalog::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTAnalog message handlers

//##ModelId=49B87B930301
void CPTAnalog::OnInitialUpdate() 
{
	//记录控件位置
	RecordRate(IDC_BTN_PTANALOG_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTANALOG_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTANALOG_EXCEL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTANALOG_STYLE, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTANALOG, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTANALOG_CPU, 0, left_client, top_client);
	RecordRate(IDC_GIF_PTANALOG, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTAI, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTAI_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTAI_P2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTANALOG_LOADING, 0, mid_client, top_client);
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
//	ResizeParentToFit();

	//初始化列表
	InitListCtrl();
	//初始化其它控件
	InitControls();
	//加载动画
	if( m_gif.Load(MAKEINTRESOURCE(IDR_GIF_WAIT), _T("GIF")))
	{
		m_gif.Draw();
		::Sleep(50);
		m_gif.Stop();
	}
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B930208
int CPTAnalog::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
/*	RefreshControls();
	LoadPTAnalogConfig();
	LoadPTHistoryAnalogData();*/

	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B930222
int CPTAnalog::InitListStyle()
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
		lvCol.fmt=LVCFMT_LEFT;
	if(LoadListConfig() && m_arrColum.GetSize() >= 5)
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
		//char* arColumn[5]={"序号","模拟量名称", "模拟量代码", "单位", "实时值"};
	
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 5 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 50; // 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 140; // 模拟量名称
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 140; // 模拟量代码
				colName = StringFromID(IDS_COMMON_CODE);
				break;
			case 3:
				lvCol.cx = 80;
				colName = StringFromID(IDS_COMMON_UNIT);
				break;
			case 4:
				lvCol.cx = 160;
				colName = StringFromID(IDS_COMMON_REALTIME_VALUE);
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText= colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		//默认隐藏第3列(模拟量代码)
		m_List.SetColumnHide(2, TRUE);
	}
	if( !g_PTAIRealtimeValue )
	{
		//实时值
		m_List.SetColumnHide(m_nRealtimeValueIndex, TRUE);
	}
	else
	{
		m_List.SetColumnHide(m_nRealtimeValueIndex, FALSE);
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
//##ModelId=49B87B930242
BOOL CPTAnalog::LoadListConfig()
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
			WriteLog("PTAnalog::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("PTAnalog::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTAnalogConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到PTAnalogConfig", XJ_LOG_LV3);
				WriteLog("PTAnalog::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到PTAnalogConfig下的配置", XJ_LOG_LV3);
					WriteLog("PTAnalog::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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

//##ModelId=49B87B930251
BOOL CPTAnalog::SaveListConfig()
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
			WriteLog("PTAnalog::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("PTAnalog::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTAnalogConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到PTAnalogConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTAnalogConfig");
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
						WriteLog("未找到PTAnalogConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
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
						WriteLog("找到了PTAnalogConfig下的配置, 先删除新建所有子节点", XJ_LOG_LV3);
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
//##ModelId=49B87B930232
int CPTAnalog::FillListData(int nQueryNo)
{
	WriteLog("CPTAnalog::FillListData,开始填充数据", XJ_LOG_LV3);

	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	PT_ANALOG *pts = NULL;
	EnterCriticalSection(&m_CriticalOper);
	int nValueCount = m_HisValueMgr.GetCount();
	for(int i = 0; i < m_arrAnalog.GetSize(); i ++)
	{
		if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo)))
			break;
		pts = m_arrAnalog.GetAt(i);
		CString sID = pts->ID;
		if(1 == g_PTIndexType)
		{
			sID.Format("%d", i+1);
		}
		m_List.InsertItem(i, sID);
		m_List.SetItemText(i, 1, pts->Name);
		m_List.SetItemText(i, 2, pts->CodeName); //
		m_List.SetItemText(i, 3, pts->Unit); //单位
		m_List.SetItemText(i, 4, SetFloatValue(pts, pts->Value));
		
		if(g_PTAIValues)
		{
			for(int k = 0; k < nValueCount; k++)
			{
				CString sHisValue = m_HisValueMgr.GetValue(k, atoi(pts->ID));
				m_List.SetItemText(i, m_nNewValueIndex+k, SetFloatValue(pts, sHisValue));
			}
		}
		m_List.SetItemData(i, (DWORD)pts);
	}
	LeaveCriticalSection(&m_CriticalOper);
	//恢复刷新
	m_List.SetRedraw(TRUE);
	
	WriteLog("CPTAnalog::FillListData,填充数据完毕", XJ_LOG_LV3);

	return 0;
}

//##ModelId=49B87B930261
BOOL CPTAnalog::NeedSave()
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
		for(int i = 0; i < m_nNewValueIndex; i++)
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
//##ModelId=49B87B930271
int CPTAnalog::InitControls()
{
	/*
	//召唤按钮
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
//	m_btnCall.SetIcon(IDI_SEARCH);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("召唤模拟量"));

	//显示设置按钮
	m_btnStyle.SetThemeHelper(&m_ThemeHelper);
//	m_btnStyle.SetIcon(IDI_SEARCH);
	m_btnStyle.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnStyle.SetTooltipText(_T("设置显示风格"));
*/
	//打印按钮
	m_btnPrint.SetThemeHelper(&m_ThemeHelper);
//	m_btnPrint.SetIcon(IDI_PRINT);
	m_btnPrint.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnPrint.SetTooltipText( StringFromID(IDS_COMMON_PRINT));
	m_btnPrint.SetMenu(IDR_MENU_ROUTINE_PRINT, GetSafeHwnd());
	return 0;
}

//##ModelId=49B87B93038A
void CPTAnalog::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTANALOG, left_client, IDC_BTN_PTANALOG_CALL, right_client, bottom_client);
}

//##ModelId=49B87B9303A9
void CPTAnalog::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名：LoadPTAnalogConfig()
 功能概要：从数据库读取模拟量配置信息,模拟量历史值只保存一次,
			并且保存在配置表中,所以直接读出值
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B930280
BOOL CPTAnalog::LoadPTAnalogConfig(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	BOOL bReturn = TRUE;

	stopRealtimeValueTimer();

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据PT_ID, cpu_code查找DI_ID,CODE_NAME,NAME,按DI_ID从小到大排列
		
	CString str;
		
	//字段
	//AI_ID
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "AI_ID";
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

	//VALUE
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "VALUE";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_FLOAT;
	sql.Fieldlist.push_back(Field4);

	//curtime
	Field Field7;
	bzero(&Field7, sizeof(Field));
	str = "CURTIME";
	strncpy(Field7.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_TIME;
	sql.Fieldlist.push_back(Field7);

	//unit
	Field Field5;
	bzero(&Field5, sizeof(Field));
	str = "UNIT";
	strncpy(Field5.FieldName, str, str.GetLength());
	Field5.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field5);

	//ai_precision
	Field Field6;
	bzero(&Field6, sizeof(Field));
	str = "AI_PRECISION";
	strncpy(Field6.FieldName, str, str.GetLength());
	Field6.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Field6);
	
	//条件
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %s", m_sCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	//按AI_ID大小排序
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by AI_ID");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition3);
			
	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_AI_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTAnalog::LoadPTAnalogConfig, 查询失败");
		return FALSE;
	}

	CString strCurTime = "";
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTAnalog::LoadPTAnalogConfig,读取到%d条数据", nCount);
		WriteLogEx(str);

		EnterCriticalSection(&m_CriticalOper);
		int nValueNo = m_HisValueMgr.CreateValue("");
		BOOL bHasTime = FALSE;
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(g_role != MODE_SUB && i%20 == 0)
				Sleep(1);
			//创建开关量对象
			PT_ANALOG * analog = new PT_ANALOG;
			analog->ID = pMemset.GetValue((UINT)0); //DI_ID
			analog->Name = pMemset.GetValue(1); //NAME
			analog->CodeName = pMemset.GetValue(2); //Code_Name
			analog->Value = pMemset.GetValue(3); //value
			float fValue = atof(analog->Value);
			analog->Value.Format("%.3f", fValue);
			m_HisValueMgr.AddValue(nValueNo, atoi(analog->ID), analog->Value);
			if(!bHasTime)
			{
				if(analog->Value != "")
				{
					bHasTime = TRUE;
					m_HisValueMgr.SetTime(nValueNo, pMemset.GetValue(4));
				}
			}
			analog->Unit = "";
			analog->Unit = pMemset.GetValue(5); //unit
			analog->Precision = pMemset.GetValue(6); //ai_precision
			m_arrAnalog.Add(analog);
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTAnalog::LoadPTAnalogConfig,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	WriteLog("CPTAnalog::LoadPTAnalogConfig,查询数据完毕", XJ_LOG_LV3);

	startRealtimeValueTimer();

	return bReturn;
}

/*************************************************************
 函 数 名：ClearAnalogConfig()
 功能概要：清除模拟量配置信息
 返 回 值: 清除成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B930290
BOOL CPTAnalog::ClearAnalogConfig()
{
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_arrAnalog.GetSize(); i++)
	{
		PT_ANALOG * analog = (PT_ANALOG*)m_arrAnalog.GetAt(i);
		delete analog;
		analog = NULL;
	}
	m_arrAnalog.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);
	return TRUE;
}

/*************************************************************
 函 数 名：AddAnalogValue()
 功能概要：加入某模拟量值到列表
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1   要加入的模拟量的ID
           Param2   要加入的模拟量的值
		   Param3	指定要加入到第几列
**************************************************************/
//##ModelId=49B87B93029F
BOOL CPTAnalog::AddAnalogValue(CString ID, CString VALUE, int iCol)
{
	//先找到DI_ID==ID的索引
	BOOL bReturn = TRUE;
	CString strID = ID;
	strID.TrimRight();

	PT_ANALOG* pst = FindAnalog(strID);
	if(pst == NULL)
		return FALSE;
	LVFINDINFO  fi;
	fi.flags = LVFI_PARAM;
	fi.lParam = (LPARAM)pst;
	
	//查找此值在第几行
	int nItem = m_List.FindItem(&fi);
	if(nItem<0)
		return FALSE;
	//设置值
	pst->Value = VALUE;

	//设置ListCtrl的Value列内容
	bReturn = m_List.SetItemText(nItem, iCol, SetFloatValue(pst, pst->Value));
	return bReturn;
}

/*************************************************************
 函 数 名：LoadPTHistoryAnalogData()
 功能概要：载入模拟量历史值 
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B9302AF
BOOL CPTAnalog::LoadPTHistoryAnalogData()
{
		if(m_pObj == NULL)
		return FALSE;
	BOOL bReturn = TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据PT_ID, cpu_code查找DI_ID,CODE_NAME,NAME,按DI_ID从小到大排列
		
	CString str;
		
	//字段
	//AI_ID
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "AI_ID";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_INT;
	sql.Fieldlist.push_back(Field1);

	//VALUE
	Field Field4;
	bzero(&Field4, sizeof(Field));
	str = "VALUE";
	strncpy(Field4.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_FLOAT;
	sql.Fieldlist.push_back(Field4);

	//curtime
	Field Field7;
	bzero(&Field7, sizeof(Field));
	str = "CURTIME";
	strncpy(Field7.FieldName, str, str.GetLength());
	Field4.FieldType = EX_STTP_DATA_TYPE_TIME;
	sql.Fieldlist.push_back(Field7);
	
	//条件
	//PT_ID
	Condition condition1;
	bzero(&condition1, sizeof(Condition));
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	strncpy(condition1.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition1);

	//cpu_code
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("CPU_CODE = %s", m_sCPU);
	strncpy(condition2.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(condition2);

	CMemSet pMemset;
		
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_AI_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTAnalog::LoadPTAnalogConfig, 查询失败");
		return FALSE;
	}

	CString strCurTime = "";
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTAnalog::LoadPTHistoryAnalogData,读取到%d条数据", nCount);
		WriteLogEx(str);

		
		for(int i = 0; i < nCount; i++)
		{
			if( 0 == i )
			{
				CString strTime;
				strTime.Format("%s:%s", StringFromID(IDS_COMMON_REALTIME_VALUE),pMemset.GetValue(2));
				LVCOLUMN m_vcolumn;
				m_vcolumn.mask = LVCF_TEXT;
				m_vcolumn.pszText =strTime.GetBuffer(0);
				m_vcolumn.cchTextMax = strTime.GetLength();
				m_List.SetColumn(m_nRealtimeValueIndex, &m_vcolumn);
			}
			AddAnalogValue(pMemset.GetValue((UINT)0), pMemset.GetValue(1), m_nRealtimeValueIndex);
					
			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CPTAnalog::LoadPTHistoryAnalogData,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	WriteLog("CPTAnalog::LoadPTHistoryAnalogData,查询数据完毕", XJ_LOG_LV3);

	return bReturn;
}

//##ModelId=49B87B9303B9
void CPTAnalog::OnBtnPtanalogCall() 
{
	// TODO: Add your control notification handler code here
	//响应召唤按钮按下消息,发送召唤模拟量报文或取消召唤
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
		//空闲状态, 点击按钮表示要召唤模拟量
		//申请报文数据容器
		STTP_FULL_DATA sttpData;
		
		//组织召唤模拟量20007报文
		if(!pApp->m_SttpEngine.BuildDataFor20007PTAnalog(sttpData, m_pObj->m_sID, atoi(m_sCPU)))
		{
			//组织报文失败
			return;
		}
		
		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);

		zero_sttp_full_data(sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("保护[%s]发送召唤模拟量报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送召唤模拟量报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,状态改为正在召唤模拟量
			m_nCurrentStatus = 1;
			m_bFirstFrame = TRUE;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nPTAiTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//增加人工操作日志
			//pApp->AddManOperator(FUNC_QUY_CALLANALOG, m_pObj->m_sName);
			
			//根据当前状态更新控件可用性
			UpdateControlsEnable();

			CString str;
			str.Format("保护[%s]发送召唤模拟量报文成功,等待回应", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else
	{
		//取消召唤
		CancelCall();
	}
}


/*************************************************************
 函 数 名：OnSTTP20008()
 功能概要：响应WM_STTP_20008(模拟量下载通知)消息
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据结构指针
**************************************************************/
//##ModelId=49B87B940006
void CPTAnalog::OnSTTP20008( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤模拟量的状态,不处理报文
		return;
	}

	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}

	//检查是否20008报文
	if(pSttpData->sttp_head.uMsgID != 20008)
	{
		//错误报文
		CString str;
		str.Format("CPTAnalog::OnSTTP20008 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTAnalog::OnSTTP20008 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//检查CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//不是本CPU的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20008 收到错误CPU报文, CPU为%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	if( m_bFirstFrame )
	{
		m_bFirstFrame = FALSE;

		//时间
		CString strTime = pSttpData->sttp_body.ch_time_15_BIT1;
		//时间转换
		int nms;
		CTime tmTime = StringToTimeSttp15(strTime, nms);
		CString strTimeDisplay = tmTime.Format("%Y-%m-%d %H:%M:%S");
		//新加一列
		int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
	}

	//处理报文
	if(!LoadDataFromSTTP(pSttpData))
	{
		WriteLog("CPTAnalog::OnSTTP20008 处理报文失败", XJ_LOG_LV3);
	}

	if( pSttpData->sttp_head.uMsgEndFlag == 1 )
	{
		//还有后续帧
		return;
	}

	//收到正确报文, 停止超时计时器
	KillTimer(m_nTimer);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_SUCCESS, XJ_OPER_CALL_ANALOG, m_sCPU);
	pApp->AddNewManOperator(FUNC_QUY_CALLANALOG, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ANALOG, OPER_SUCCESS);
	//处理完毕,改变状态
	m_nCurrentStatus = 0;

	//根据当前状态更新控件可用性
	UpdateControlsEnable();

	CString str;
	str.Format("保护[%s]召唤模拟量成功", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);
}

/*************************************************************
 函 数 名：LoadDataFromSTTP()
 功能概要：从STTP报文中读取模拟量数据,本函数认为传入的报文已经是正确报文,不做判断
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
**************************************************************/
//##ModelId=49B87B9302B0
BOOL CPTAnalog::LoadDataFromSTTP( STTP_FULL_DATA* sttpData )
{
	if(sttpData == NULL)
		return FALSE;

	//解析模拟量数据
	try
	{		
		//读取每项值,加入到列表
		int nCount = sttpData->sttp_body.variant_member.value_data.size();
		for(int i = 0; i < nCount; i++)
		{
			CString id;
			id.Format("%d", sttpData->sttp_body.variant_member.value_data[i].id);
			CString Value = XJToMS(sttpData->sttp_body.variant_member.value_data[i].str_value);
			AddAnalogValue(id, Value, m_nNewValueIndex);
		}
	}
	catch (...)
	{
		WriteLog("CPTAnalog::LoadDataFromSTTP, 错误", 3);
		return FALSE;
	}
	
	return TRUE;
}

/*************************************************************
 函 数 名：OnSTTP20069()
 功能概要：响应20069(控制报文通用返回通知)报文
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B940010
void CPTAnalog::OnSTTP20069( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤模拟量的状态,不处理报文
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
		//处理完毕,改变状态和按钮文字
		CString str;
		str.Format("保护[%s]召唤模拟量失败", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);
	}

	m_nCurrentStatus = 0;

	//根据当前状态更新控件可用性
	UpdateControlsEnable();
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ANALOG, m_sCPU);
	pApp->AddNewManOperator(FUNC_QUY_CALLANALOG, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ANALOG, OPER_FAILD);
	AfxMessageBox( StringFromID(IDS_CALL_FAIL));
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：响应20125(装置不支持功能回应报文)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B94001F
void CPTAnalog::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤模拟量的状态,不处理报文
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
	if(!IsVaild20125(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
			
	//根据当前状态更新控件可用性
	UpdateControlsEnable();

	CString str;
	str.Format("保护[%s]召唤模拟量失败,原因为:装置不支持", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV1);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ANALOG, m_sCPU);
	pApp->AddNewManOperator(FUNC_QUY_CALLANALOG, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ANALOG, OPER_FAILD);
	
	AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
}

/*************************************************************
 函 数 名：UpdateControlsEnable()
 功能概要：根据当前状态更新控件可用性
 返 回 值: void
**************************************************************/
//##ModelId=49B87B9302C0
void CPTAnalog::UpdateControlsEnable()
{
	//按状态分情况
	if(m_nCurrentStatus == 0)
	{	
		//召唤按钮文字改为取消召唤
		SetDlgItemText(IDC_BTN_PTANALOG_CALL, StringFromID(IDS_BTN_CALLANALOG));

		//打印和风格设置按钮
		GetDlgItem(IDC_BTN_PTANALOG_PRINT) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTANALOG_EXCEL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTANALOG_STYLE) ->EnableWindow(TRUE);
		
		//下拉框
		GetDlgItem(IDC_CMB_PTANALOG_CPU) ->EnableWindow(TRUE);
		//动画
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//停止计时
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;
	}
	else if(m_nCurrentStatus  == 1)
	{
		//召唤按钮文字改为取消召唤
		SetDlgItemText(IDC_BTN_PTANALOG_CALL, CANCEL_CALL);

		//打印和风格设置按钮
		GetDlgItem(IDC_BTN_PTANALOG_PRINT) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTANALOG_EXCEL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTANALOG_STYLE) ->EnableWindow(FALSE);

		//下拉框
		GetDlgItem(IDC_CMB_PTANALOG_CPU) ->EnableWindow(FALSE);
		//动画
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//开始计时
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime, StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTAI, str);
	}
}

/*************************************************************
 函 数 名：OnPTFrameOpen()
 功能概要：响应保护属性页打开消息, 刷新内容
 返 回 值: void
 参    数：param1	消息参数
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B940031
void CPTAnalog::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;

	//页面还原
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	//清除召唤计时
	SetDlgItemText(IDC_STATIC_TIME_PTAI, "");
	//刷新控件值
	RefreshControls();
	
	ReFillAll();
}

/*************************************************************
 函 数 名：RefreshControls()
 功能概要：刷新控件值, 在第一次启动或更改对应保护时才要调用
 返 回 值: void
**************************************************************/
//##ModelId=49B87B9302C1
void CPTAnalog::RefreshControls()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTANALOG_CPU);
	if(pCmb == NULL)
		return;

	//清空
	pCmb->ResetContent();

	if(m_pObj == NULL)
		return;

	//CPU下拉框, 填充内容为"CPU号-描述"
	for(int i = 0; i < m_pObj->m_arrCPU.GetSize(); i++)
	{
		//取得CPU对象
		CSecCPU * pCpu = (CSecCPU*)m_pObj->m_arrCPU.GetAt(i);
		CString str;
		str.Format("%d-%s", pCpu->code, pCpu->des); //cpu号-描述
		int nIndex = pCmb->AddString(str);
		if(nIndex != -1)
			pCmb->SetItemData(nIndex, (DWORD)pCpu);
	}
	//设置默认选择
	if(pCmb->GetCount() > 0)
	{
		if( pCmb->GetCount() == 1 )
		{
			pCmb->SetCurSel(0);
		}
		else
		{
			if( !LoadDefaultCpu())
			{			
				pCmb->SetCurSel(0);
			}
		}

		//保存当前选择
		SaveSelCPU(pCmb);
		return;
	}
}

/*************************************************************
 函 数 名：SaveSelCPU()
 功能概要：保存当前选择的CPU
 返 回 值: void
 参    数：param1	CPU下拉框
**************************************************************/
//##ModelId=49B87B9302CE
BOOL CPTAnalog::SaveSelCPU( CComboBox * pCmb )
{
	int nIndex = pCmb->GetCurSel();
	if(nIndex < 0)
		return FALSE;
	CString strCPU;
	pCmb->GetLBText(nIndex, strCPU);
	//CPU显示的格式为: CPU号-CPU描述
	int pos = strCPU.Find('-', 0);
	if(pos != -1)
	{
		strCPU = strCPU.Left(pos);
	}
	if(m_sCPU != strCPU)
	{
		m_sCPU = strCPU;
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B9303BB
void CPTAnalog::OnSelchangeCmbPtanalogCpu() 
{
	// TODO: Add your control notification handler code here
	//保存当前选择
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTANALOG_CPU);
	if(pCmb == NULL)
		return;

	if(SaveSelCPU(pCmb))
	{
		//重新载入数据
		ReFillAll();
	}	
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置List中的值有变化时文字颜色
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9303C8
void CPTAnalog::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				//值的第一列与第二列差异
				if(nSubItem == m_nNewValueIndex)
				{
					//值
					CString strValue = m_List.GetItemText(nItem, m_nNewValueIndex);
					CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex + 1);
					
					//去除两边空格
					strValue.TrimLeft();
					strValue.TrimRight();
					strOldValue.TrimLeft();
					strOldValue.TrimRight();

					//设置文本颜色
					if(strValue != strOldValue)
						lplvcd->clrText = XJ_LIST_ONE_TWO;
				}
				//第二列与第三列差异
				if(nSubItem == m_nNewValueIndex + 1)
				{
					//值
					CString strOldValue = m_List.GetItemText(nItem, m_nNewValueIndex + 1);
					CString strOlderValue = m_List.GetItemText(nItem, m_nNewValueIndex + 2);

					//去除空格
					strOldValue.TrimLeft();
					strOldValue.TrimRight();
					strOlderValue.TrimLeft();
					strOlderValue.TrimRight();

					//设置文本颜色
					if(strOldValue != strOlderValue)
						lplvcd->clrText = XJ_LIST_TWO_THREE;
				}

				//间隔
				if(nItem%2 != 0 && nSubItem < m_nNewValueIndex)
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
				
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

//##ModelId=49B87B9303CC
void CPTAnalog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//停止时间
		KillTimer(m_nTimer);
		if(m_nCurrentStatus == 1)
		{
			//改变状态
			m_nCurrentStatus = 0;
			UpdateControlsEnable();
			CString str;
			str.Format("保护[%s]召唤模拟量失败,原因为:超时", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_ANALOG, m_sCPU);
			pApp->AddNewManOperator(FUNC_QUY_CALLANALOG, m_pObj->m_sID, strMsg, XJ_OPER_CALL_ANALOG, OPER_FAILD);
			//提示
			AfxMessageBox( StringFromID(IDS_CALLFAIL_TIMEOUT));
		}
	}
	if(nIDEvent == m_nRecordTimer)
	{
		//改变持续时间显示
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime, StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTAI, str);
	}
	if( nIDEvent == m_nRealtimeValueTimer )
	{
		LoadPTHistoryAnalogData();
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：ReFillAll()
 功能概要：重新读入和填充所有内容
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B9302F0
BOOL CPTAnalog::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();

	GetDlgItem(IDC_STATIC_PTANALOG_LOADING)->ShowWindow(SW_SHOW);

	//清空内容
	ClearAnalogConfig();
	EnterCriticalSection(&m_CriticalOper);
	m_HisValueMgr.ResetContent();
	LeaveCriticalSection(&m_CriticalOper);
	ClearDataCol();
	m_List.DeleteAllItems();
	
	//读取数据
	if(g_role == MODE_SUB)
	{
		//直接读取
		LoadPTAnalogConfig(0);
		//加入历史值列
		AddValueCol(0);
		//填充数据
		FillListData(0);
		GetDlgItem(IDC_STATIC_PTANALOG_LOADING)->ShowWindow(SW_HIDE);
	}
	else
	{
		//启动线程
		StartThread();
	}
	
	
	return TRUE;
}

/*************************************************************
 函 数 名：OnPTFrameClose()
 功能概要：窗口关闭时, 退出线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B940040
void CPTAnalog::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 函 数 名：ClearDataCol()
 功能概要：删除新加的数据列
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9302D0
void CPTAnalog::ClearDataCol()
{
	//检查数据有效性
	if(m_List.GetSafeHwnd() == NULL)
		return;
	
	int nCount = m_List.GetHeaderCtrl()->GetItemCount();
	while(nCount > m_nNewValueIndex)
	{
		m_List.DeleteColumn(m_nNewValueIndex);
		nCount = m_List.GetHeaderCtrl()->GetItemCount();
	}
}

/*************************************************************
 函 数 名：CancelCall()
 功能概要：取消召唤操作
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9302DE
void CPTAnalog::CancelCall()
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
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20007, m_pObj->m_sID, atoi(m_sCPU));
	}
	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_nCurrentStatus = 0;
	//更改控件可用性
	UpdateControlsEnable();
}

//##ModelId=49B87B9302DF
void CPTAnalog::EndOwnerThread()
{
	WriteLog("开始退出保护模拟量页面读取数据线程", XJ_LOG_LV3);
	
	m_bExitThread = TRUE;
	
	while( GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("退出保护模拟量页面读取数据线程完毕", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：IsVaild20069()
 功能概要：判断收到的20069是否自己的
 返 回 值: 是自己的失败报文返回TRUE, 否则返回FALSE
 参    数：param1	失败的操作报文ID
		   Param2
**************************************************************/
//##ModelId=49B87B9302E0
BOOL CPTAnalog::IsVaild20069( int nMsgID )
{
	if(20007 == nMsgID && 1 == m_nCurrentStatus)
	{
		//召唤模拟量
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B9303D8
BOOL CPTAnalog::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTANALOG);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

/*************************************************************
 函 数 名：IsVaild20125()
 功能概要：判断是否自己功能的20125报文 
 返 回 值: 是自己功能的报文返回TRUE, 否则返回FALSE
 参    数：param1	操作报文ID
		   Param2
**************************************************************/
//##ModelId=49B87B9302EE
BOOL CPTAnalog::IsVaild20125( int nMsgID )
{
	if(20007 == nMsgID && 1 == m_nCurrentStatus)
	{
		//召唤模拟量
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B9303DB
void CPTAnalog::OnBtnPtanalogStyle() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->DoSysSetting(SYS_SETTING_PTPROP);
}

//##ModelId=49B87B93033C
void CPTAnalog::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_ANALOG));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_ANALOG));


	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 函 数 名：TranslateSingleKey()
 功能概要：解释单个关键字的意义
 返 回 值: 关键字表示的值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B9302F1
CString CPTAnalog::TranslateSingleKey( CString sKey )
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
		if(strHead != "$PT_ROUTINE_LIST")
		{
			//如果不是保护常规值, 返回空
			return "";
		}
		//取得尾部字符
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "PT_ROUTINE_ID$")
		{
			return m_List.GetItemText(nIndex, 0); //常规值ID
		}
		if(strTail == "PT_ROUTINE_NAME$")
		{
			return m_List.GetItemText(nIndex, 1); //常规值名称
		}
		if(strTail == "PT_ROUTINE_CODE$")
		{
			return m_List.GetItemText(nIndex, 2); //常规值代码
		}
		if(strTail == "PT_ROUTINE_UNIT$")
		{
			return m_List.GetItemText(nIndex, 3); //单位
		}
		//以上都不是,可能是$PT_ROUTINE_ VALUE#N$的形式
		int nValueItem = -1;
		nValueItem = strTail.Find('#', 0);
		if(nValueItem != -1)
		{
			//是有#的形式, 判断是不是PT_ROUTINE_ VALUE#N
			CString strTemp = strTail;
			strTemp = strTemp.Left(nValueItem);
			if(strTemp != "PT_ROUTINE_VALUE")
			{
				return "";
			}
			//得到值索引
			strTemp = strTail;
			strTemp.Delete(0, nValueItem+1);
			nValueItem = atoi(strTemp);
			if(nValueItem < 1)
				nValueItem = 1;
			int nSubItem = m_nNewValueIndex + nValueItem - 1;
			return m_List.GetItemText(nIndex, nSubItem);
		}
	}
	else
	{
		//判断是否有.GetCount()方法
		nFind = sKey.Find(".GetCount()", 0);
		if(nFind > -1)
		{
			//有.GetCount()方法
			if(sKey == "$PT_ROUTINE_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_List.GetItemCount());
				return sReturn;
			}
		}
		else
		{
			//普通关键字
			if(sKey == "$PT_SEL_CPU$") //当前所选CPU
				return m_sCPU;
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
			//以上都不是, 有可能是$PT_ROUTINE_TIME#N$的情况
			int nValueItem = -1;
			nValueItem = sKey.Find('#', 0);
			if(nValueItem != -1)
			{
				//是有#的形式, 判断是不是PT_ROUTINE_TIME#N
				CString strTemp = sKey;
				strTemp = strTemp.Left(nValueItem);
				if(strTemp != "$PT_ROUTINE_TIME")
				{
					return "";
				}
				//得到值索引
				strTemp = sKey;
				strTemp.Delete(0, nValueItem+1);
				nValueItem = atoi(strTemp);
				if(nValueItem < 1)
					nValueItem = 1;
				int nSubItem = m_nNewValueIndex + nValueItem - 1;
				LVCOLUMN col;
				col.mask = LVCF_TEXT;
				char temp[256];
				for(int j = 0; j < 256; j++)
				{
					temp[j] = '\0';
				}
				col.cchTextMax = 256;
				col.pszText = temp;
				m_List.GetColumn(nSubItem, &col);
				CString strReturn = temp;
				return strReturn;
			}
		}
	}
	return "";
}

/*************************************************************
 函 数 名：InitCOM()
 功能概要：初始化打印模板COM组件
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9302FD
BOOL CPTAnalog::InitCOM()
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
//##ModelId=49B87B9302FE
void CPTAnalog::ReleaseCOM()
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
 函 数 名：DoPrintAnalog()
 功能概要：打印模拟量
 返 回 值: 
 参    数：param1	打印模板全路径
		   Param2
**************************************************************/
//##ModelId=49B87B9302FF
void CPTAnalog::DoPrintAnalog( CString strPath )
{
	InitCOM();
	//打开模板文件
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, strPath);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),strPath);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//设置回调函数
	if(pReportDoc != NULL && bOpened)
	{
		if (m_pObj->m_nType == TYPE_WAVEREC)
		{
			pReportDoc->SetQueryFunction(TranslateKeyInWRAnalog);
		}
		else
			pReportDoc->SetQueryFunction(TranslateKeyInPTAnalog);
	}
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

//##ModelId=49B87B940000
void CPTAnalog::OnRoutineprintNew1() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintAnalog(ANALOG_NEW1_TP);
}

//##ModelId=49B87B940002
void CPTAnalog::OnRoutineprintNew2() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintAnalog(ANALOG_NEW2_TP);
}

//##ModelId=49B87B940004
void CPTAnalog::OnRoutineprintNew3() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintAnalog(ANALOG_NEW3_TP);
}

/*************************************************************
 函 数 名：OnSTTP20157()
 功能概要：响应20157, 超时通知报文
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B94002E
void CPTAnalog::OnSTTP20157( WPARAM wParam,LPARAM lParam )
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
	
	//检查CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//不是本CPU的报文
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
		//正在召唤模拟量				
		int nTimeOut = pData->m_nPTAiTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}

void CPTAnalog::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTAnalog::OnDoubleclickedBtnPtanalogCall() 
{
	// TODO: Add your control notification handler code here
	
}
/****************************************************
Date:2012/12/3  Author:LY
函数名:   SetFloatValue	
返回值:   void	
功能概要: 
参数: CString strVPickList
*****************************************************/
CString CPTAnalog::SetFloatValue(PT_ANALOG *pst, CString strValue)
{
	if (pst==NULL)
	{
		return strValue;
	}
	CString sValue = strValue;
	if (sValue != "")
	{
		CString sPrecision = pst->Precision;
		
		int nDouhao = sPrecision.Find(",");
		sPrecision.Delete(0,nDouhao+1);
		int nAdd = 3;
		sPrecision.TrimLeft();
		sPrecision.TrimRight();
		if(!sPrecision.IsEmpty())
		{
			nAdd = atoi(sPrecision);			//精度
		}
		
		float fValue = atof(sValue);
		CString sFormat;
		sFormat.Format("%%.0%df", nAdd);
		sValue.Format(sFormat, fValue);
	}
	return sValue;
}

/****************************************************
Date:2012/12/6  Author:LYH
函数名:   FindAnalog	
返回值:   PT_ANALOG*	
功能概要: 
参数: CString sID	
*****************************************************/
PT_ANALOG* CPTAnalog::FindAnalog( CString sID )
{
	for(int i = 0; i < m_arrAnalog.GetSize(); i++)
	{
		PT_ANALOG* pa = (PT_ANALOG*)m_arrAnalog.GetAt(i);
		if(pa == NULL)
			continue;
		if(pa->ID == sID)
			return pa;
	}
	return NULL;
}

void CPTAnalog::StartThread()
{	
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTAnalogLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("模拟量查询线程启动成功,查询批次:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTAnalog::GetNextQueryNo()
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


void CPTAnalog::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("退出保护模拟量查询线程,查询号:%d", nNo);
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
	str.Format("保护模拟量查询线程成功退出,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTAnalog::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("保护模拟量查询线程成功启动,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTAnalog::GetFirstNode()
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


int CPTAnalog::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

/****************************************************
Date:2014/1/9  Author:LYH
函数名:   OnThreadFillData	
返回值:   void	
功能概要: 由另一个线程发出更新数据显示的通知
参数: WPARAM wParam	查询批次
参数: LPARAM lParam	
*****************************************************/
void CPTAnalog::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//过期的批次
		return;
	}
	if( lParam == 1)
	{
		AddValueCol(wParam);
		FillListData(wParam);
	}
	GetDlgItem(IDC_STATIC_PTANALOG_LOADING)->ShowWindow(SW_HIDE);
}

/****************************************************
Date:2014/1/9  Author:LYH
函数名:   AddValueCol	
返回值:   void	
功能概要: 为列表加上一列用来显示值
参数: int nQueryNo	
*****************************************************/
void CPTAnalog::AddValueCol( int nQueryNo )
{
	if(!g_PTAIValues)
		return;
	if(m_arrAnalog.GetSize == 0)
		return;
	if(MODE_SUB != g_role && !IsCurrentQueryNo(nQueryNo))
		return;
	//加入值列
	CString strTimeDisplay;
	CString strTime = "";
	WriteLog("CPTAnalog::AddValueCol, 开始加入值列", XJ_LOG_LV3);
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_HisValueMgr.GetCount(); i++)
	{
		strTime = m_HisValueMgr.GetTime(i);
		if(strTime.ReverseFind('.') != -1)
			strTimeDisplay = strTime.Left(strTime.ReverseFind('.'));
		else
			strTimeDisplay = strTime;
		if(IsCurrentQueryNo(nQueryNo) || MODE_SUB == g_role)
			m_List.InsertColumn(m_nNewValueIndex+i, strTimeDisplay, LVCFMT_LEFT, 150, 0);
	}
	LeaveCriticalSection(&m_CriticalOper);

	WriteLog("CPTAnalog::AddValueCol, 加入值列完毕", XJ_LOG_LV3);
}

BOOL CPTAnalog::LoadDefaultCpu()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTANALOG_CPU);
	if(pCmb == NULL)
		return TRUE;
	if( pCmb->GetCount() == 0 )
		return TRUE;
	if(m_pObj == NULL)
		return FALSE;
	
	WriteLog("CPTAnalog::LoadDefaultCpu, 查询开始", XJ_LOG_LV3);
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	Field Field0;
	bzero(&Field0, sizeof(Field));
	Field0.FieldType = 97;
	sql.Fieldlist.push_back(Field0);
	
	//id
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "cpu_code", EX_STTP_DATA_TYPE_INT);
	
	//条件
	CString str = "";
	//PT_ID
	Condition condition1;
	str.Format("PT_ID = '%s'", m_pObj->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	//按cpu排序
	Condition condition5;
	str.Format("order by cpu_code");
	pApp->m_DBEngine.SetCondition(sql, condition5, str, EX_STTP_WHERE_ABNORMALITY);//非where
	
	
	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_AI_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTAnalog::LoadDefaultCpu, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTAnalog::LoadDefaultCpu,查询到%d条数据", nCount);
		WriteLogEx(str);
		if( nCount >= 1 )
		{
			str = pMemset.GetValue((UINT)0); //ID
			int c = atoi(str);
			for( int k = 0; k < pCmb->GetCount(); k++ )
			{
				CSecCPU * cpu =  (CSecCPU *)pCmb->GetItemData(k);
				if( cpu != NULL && cpu->code == c)
				{
					pCmb->SetCurSel(k);
					bReturn = TRUE;
					break;
				}
			}
		}
	}
	else
	{
		CString str;
		str.Format("CPTAnalog::LoadDefaultCpu,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLog("CPTAnalog::LoadDefaultCpu, 查询结束", XJ_LOG_LV3);
	
	return bReturn;
}

void CPTAnalog::startRealtimeValueTimer()
{
	if( g_PTAIRealtimeValue )
		m_nRealtimeValueTimer = SetTimer(99, g_PTAIRealtimeRefresh*1000, 0);
}

void CPTAnalog::stopRealtimeValueTimer()
{
	if( g_PTAIRealtimeValue && m_nRealtimeValueTimer)
		KillTimer(m_nRealtimeValueTimer);
}

void CPTAnalog::OnBtnPtanalogExcel() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString strName;
	strName.Format("%s(%s-CPU[%s])-%s.xls", StringFromID(IDS_SECPROP_ANALOG),m_pObj->m_sName, m_sCPU, t.Format("%Y%m%d%H%M%S"));
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
