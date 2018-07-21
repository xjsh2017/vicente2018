// PTDigital.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTDigital.h"
#include "DataEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT PTDigitalLoad(LPVOID pParam)
{
	CPTDigital* ptDigital = (CPTDigital*)pParam;
	if(ptDigital == NULL)
		return 1;
	int nQueryNo = ptDigital->GetCurrentQueryNo();
	
	if(ptDigital->IsCurrentQueryNo(nQueryNo) && !ptDigital->m_bExitThread)
	{
		BOOL bLoad = ptDigital->LoadPTDigitalConfig(nQueryNo);
		if(bLoad)
		{
			bLoad = ptDigital->LoadPTHistoryDigitalData(nQueryNo);
		}
		if(bLoad)
			ptDigital->PostMessage(THREAD_FILL_DATA, nQueryNo, 1);
		else
			ptDigital->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
	}

	ptDigital->RemoveThreadNode(nQueryNo);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTDigital

IMPLEMENT_DYNCREATE(CPTDigital, CViewBase)

//##ModelId=49B87B920179
CPTDigital::CPTDigital()
	: CViewBase(CPTDigital::IDD)
{
	//{{AFX_DATA_INIT(CPTDigital)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pObj = NULL;
	m_sCPU = -1;
	m_arrDigital.RemoveAll();
	m_nNewValueIndex = 3;
	m_nCurrentStatus = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_arrColum.RemoveAll();
	m_nQueryNo = 0;
	m_listThread.RemoveAll();

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B9203AD
CPTDigital::~CPTDigital()
{
	ClearDigitalConfig();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B92037A
void CPTDigital::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTDigital)
	DDX_Control(pDX, IDC_GIF_PTDIGITAL, m_gif);
	DDX_Control(pDX, IDC_BTN_PTDI_STYLE, m_btnStyle);
	DDX_Control(pDX, IDC_BTN_PTDI_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTDI_CALL, m_btnCall);
	DDX_Control(pDX, IDC_LIST_PTDI, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTDigital, CViewBase)
	//{{AFX_MSG_MAP(CPTDigital)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTDI_CALL, OnBtnPtdiCall)
	ON_CBN_SELCHANGE(IDC_CMB_PTDI_CPU, OnSelchangeCmbPtdiCpu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTDI, OnCustomDraw)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTDI_STYLE, OnBtnPtdiStyle)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_ROUTINEPRINT_NEW1, OnRoutineprintNew1)
	ON_COMMAND(ID_ROUTINEPRINT_NEW2, OnRoutineprintNew2)
	ON_COMMAND(ID_ROUTINEPRINT_NEW3, OnRoutineprintNew3)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTDI_CALL, OnDoubleclickedBtnPtdiCall)
	ON_BN_CLICKED(IDC_BTN_PTDI_EXCEL, OnBtnPtdiExcel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20012, OnSTTP20012)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(REFRESH_PTDI, OnRefreshPTDI)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTDigital diagnostics

#ifdef _DEBUG
//##ModelId=49B87B9203C8
void CPTDigital::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B930000
void CPTDigital::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTDigital message handlers

//##ModelId=49B87B92036B
void CPTDigital::OnInitialUpdate() 
{
	//记录控件位置
	RecordRate(IDC_BTN_PTDI_CALL, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_PTDI_PRINT, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_PTDI_EXCEL, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_BTN_PTDI_STYLE, 0, left_client, null_effect, null_effect, bottom_client);
	RecordRate(IDC_STATIC_PTDI, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_CMB_PTDI_CPU, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_GIF_PTDIGITAL, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTDI, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTDI_P1, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_STATIC_PTDI_P2, 0, left_client, top_client, null_effect, null_effect);
	RecordRate(IDC_STATIC_PTDIGITAL_LOADING, 0, mid_client, top_client);

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
//##ModelId=49B87B920243
int CPTDigital::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;

	InitListStyle();
/*	RefreshControls();
	LoadPTDigitalConfig();
	LoadPTHistoryDigitalData();*/

	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B920261
int CPTDigital::InitListStyle()
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
		//char* arColumn[3]={"序号","开关量名称", "开关量代码"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 3 ; nCol++)
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
				lvCol.cx = 140; // 开关量名称
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 140; // 开关量代码
				colName = StringFromID(IDS_COMMON_CODE);
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
		//默认隐藏第3列(开关量代码)
		m_List.SetColumnHide(2, TRUE);
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
//##ModelId=49B87B920290
BOOL CPTDigital::LoadListConfig()
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
			WriteLog("PTDigital::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("PTDigital::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTDigitalConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到PTDigitalConfig", XJ_LOG_LV3);
				WriteLog("PTDigital::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到PTDigitalConfig下的配置", XJ_LOG_LV3);
					WriteLog("PTDigital::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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

//##ModelId=49B87B92029F
BOOL CPTDigital::SaveListConfig()
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
			WriteLog("PTDigital::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("PTDigital::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTDigitalConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewCommConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTDigitalConfig");
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
						WriteLog("未找到PTDigitalConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
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
						WriteLog("找到了PTDigitalConfig下的配置, 先删除新建所有子节点", XJ_LOG_LV3);
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
//##ModelId=49B87B920271
int CPTDigital::FillListData(int nQueryNo)
{
	WriteLog("CPTDigital::FillListData, 开始填充数据", XJ_LOG_LV3);
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	CString strLog;
	strLog.Format("CPTDigital::FillListData, 共有%d批历史值", m_HisValueMgr.GetCount());
	WriteLog(strLog, XJ_LOG_LV3);
	PT_DIGITAL *pts = NULL;
	for(int i = 0; i < m_arrDigital.GetSize(); i ++)
	{
		if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo)))
			break;

		pts = m_arrDigital.GetAt(i);
		CString sID = pts->ID;
		if(1 == g_PTIndexType)
		{
			sID.Format("%d", i+1);
		}
		m_List.InsertItem(i, sID);
		m_List.SetItemText(i, 1, pts->Name);
		m_List.SetItemText(i, 2, pts->CodeName);
		for(int k = 0; k < m_HisValueMgr.GetCount(); k++)
		{
			CString sHisValue = m_HisValueMgr.GetValue(k, atoi(pts->ID));
			m_List.SetItemText(i, m_nNewValueIndex+k, DisplayValue(atoi(sHisValue)));
		}
		m_List.SetItemData(i, (DWORD)pts);
	}
	LeaveCriticalSection(&m_CriticalOper);
	//恢复刷新
	m_List.SetRedraw(TRUE);
	WriteLog("CPTDigital::FillListData, 填充数据完毕", XJ_LOG_LV3);
	return 0;
}

//##ModelId=49B87B9202AF
BOOL CPTDigital::NeedSave()
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
//##ModelId=49B87B9202BF
int CPTDigital::InitControls()
{
	/*
	//召唤按钮
	m_btnCall.SetThemeHelper(&m_ThemeHelper);
//	m_btnCall.SetIcon(IDI_SEARCH);
	m_btnCall.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnCall.SetTooltipText(_T("召唤开关量"));

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

//##ModelId=49B87B93001F
void CPTDigital::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTDI, left_client, IDC_BTN_PTDI_CALL, right_client, bottom_client);
}

//##ModelId=49B87B93003E
void CPTDigital::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名：LoadPTDigitalConfig()
 功能概要：从数据库中读取开关量配置信息,tb_pt_di_def
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B9202DE
BOOL CPTDigital::LoadPTDigitalConfig(int nQueryNo)
{
	if(!m_pObj)
		return FALSE;
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	WriteLogEx("CPTDigital::LoadPTDigitalConfig, 开始查询");

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

	//按DI_ID大小排序
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by DI_ID");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition3);

	if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
			
	CMemSet pMemset;		
	char sError[MAXMSGLEN];
	memset(sError, 0, MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTDigital::LoadPTDigitalConfig, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CPTDigital::LoadPTDigitalConfig,查询到%d条数据", nCount);
		WriteLog(strLog, XJ_LOG_LV3);
		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			//创建开关量对象
			PT_DIGITAL * digital = new PT_DIGITAL;
			digital->ID = pMemset.GetValue((UINT)0); //DI_ID
			digital->Name = pMemset.GetValue(1); //NAME
			digital->CodeName = pMemset.GetValue(2); //Code_Name
			m_arrDigital.Add(digital);
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTDigital::LoadPTDigitalConfig,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLogEx("CPTDigital::LoadPTDigitalConfig, 查询结束");

	return bReturn;
}

/*************************************************************
 函 数 名：ClearDigitalConfig()
 功能概要：清除开关量配置信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B9202EE
BOOL CPTDigital::ClearDigitalConfig()
{
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_arrDigital.GetSize(); i++)
	{
		PT_DIGITAL * digital = (PT_DIGITAL*)m_arrDigital.GetAt(i);
		delete digital;
		digital = NULL;
	}
	m_arrDigital.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);
	return TRUE;
}

/*************************************************************
 函 数 名：AddDigitalValue()
 功能概要：加入某关开量值到列表
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1   要加入的开关量的ID
           Param2   要加入的开关量的值
		   Param3	指定要加入到第几列, 此值应该>=3
**************************************************************/
//##ModelId=49B87B9202EF
BOOL CPTDigital::AddDigitalValue(CString ID, CString VALUE, int iCol)
{
	//先找到DI_ID==ID的索引
	BOOL bReturn = TRUE;
	CString strID = ID;
	strID.TrimRight();
	
	PT_DIGITAL* pst = FindDigital(strID);
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
	bReturn = m_List.SetItemText(nItem, iCol/*第三列为最新值*/, DisplayValue(atoi(VALUE)));
	return bReturn;
}

/*************************************************************
 函 数 名：LoadPTHistoryDigitalData()
 功能概要：载入开关量历史值
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B92030D
BOOL CPTDigital::LoadPTHistoryDigitalData(int nQueryNo)
{
	if(m_pObj == NULL)
		return FALSE;
	if(g_PTDIValues <= 0)
		return TRUE;
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
	
	BOOL bReturn = TRUE;	

	WriteLogEx("CPTDigital::LoadPTHistoryDigitalData, 开始查询time");

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//取得数据库内有历史数据的Time列表
	CString str;
		
	//字段

	Field Field0;
	bzero(&Field0, sizeof(Field));
	Field0.FieldType = 97;
	sql.Fieldlist.push_back(Field0);

	//CURTIME
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "CURTIME";
	strncpy(Field1.FieldName, str, str.GetLength());
	Field1.FieldType = EX_STTP_DATA_TYPE_TIME;
	sql.Fieldlist.push_back(Field1);
		
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

	//按CURTIME大小排序
	Condition condition3;
	bzero(&condition3, sizeof(Condition));
	str.Format("order by CURTIME DESC");
	strncpy(condition3.ConditionContent, str, str.GetLength());
	condition3.ConditionType = EX_STTP_WHERE_ABNORMALITY; //非where条件
	sql.Conditionlist.push_back(condition3);

	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;
				
	CMemSet pMemset;	
	char sError[MAXMSGLEN];
	memset(sError, 0, MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTDigital::LoadPTHistoryDigitalData, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CPTDigital::LoadPTHistoryDigitalData,查询到%d条time", nCount);
		WriteLog(strLog, XJ_LOG_LV3);

		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(i >= g_PTDIValues)
				break;
			CString str;
			str = pMemset.GetValue((UINT)0);
			m_HisValueMgr.CreateValue(str);
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTDigital::LoadPTHistoryDigitalData,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	if(!bReturn)
		return FALSE;

	WriteLogEx("CPTDigital::LoadPTHistoryDigitalData, 查询time结束");
	
	WriteLogEx("CPTDigital::LoadPTHistoryDigitalData, 开始查询历史值");

	EnterCriticalSection(&m_CriticalOper);
	int nHisCount = m_HisValueMgr.GetCount();
	LeaveCriticalSection(&m_CriticalOper);

	//添加每一列
	//从数据库读出每个Time对应的定值组
	for (int i = 0; i < nHisCount; i ++)
	{
		if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
			break;
		if(MODE_SUB != g_role)
			Sleep(1);
		EnterCriticalSection(&m_CriticalOper);
		CString strTime = m_HisValueMgr.GetTime(i);
		LeaveCriticalSection(&m_CriticalOper);
		
		//从数据库读出每个Time对应的开关量组

		//查找最新的num条记录
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		//取得数据库内有历史数据的Time列表
		CString str;
			
		//字段
		//CURTIME
		Field Field1;
		bzero(&Field1, sizeof(Field));
		str = "DI_ID";
		strncpy(Field1.FieldName, str, str.GetLength());
		Field1.FieldType = EX_STTP_DATA_TYPE_INT;
		sql.Fieldlist.push_back(Field1);

		Field Field2;
		bzero(&Field2, sizeof(Field));
		str = "VALUE";
		strncpy(Field2.FieldName, str, str.GetLength());
		Field2.FieldType = EX_STTP_DATA_TYPE_INT;
		sql.Fieldlist.push_back(Field2);
			
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

		//CURTIME
		Condition condition3;
		bzero(&condition3, sizeof(Condition));
		str.Format("CURTIME = '%s'", strTime);
		strncpy(condition3.ConditionContent, str, str.GetLength());
		sql.Conditionlist.push_back(condition3);

		if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
			break;
				
		CMemSet pMemset;
			
		char sError[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
			
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_DATA, sql, sError, &pMemset);
		}
		catch (...)
		{
			WriteLog("CPTDigital::LoadPTHistoryDigitalData, 查询失败", XJ_LOG_LV3);
			return FALSE;
		}
		if(nResult == 1)
		{
			pMemset.MoveFirst();
			int nCount = pMemset.GetMemRowNum();
			CString str;
			str.Format("CPTDigital::LoadPTHistoryDigitalData,查询%s的历史数据,条数为%d", strTime, nCount);
			WriteLog(str, XJ_LOG_LV3);
			int nItem = 0;
			EnterCriticalSection(&m_CriticalOper);
			for(int k = 0; k < nCount; k++)
			{
				if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
					break;
				if(MODE_SUB != g_role && k%20 == 0)
					Sleep(1);

				CString strID, strValue;
				strID = pMemset.GetValue((UINT)0);
				strValue = pMemset.GetValue(1);

				m_HisValueMgr.AddValue(i, atoi(strID), strValue);
				pMemset.MoveNext();
			}
			LeaveCriticalSection(&m_CriticalOper);
		}
		else
		{
			CString str;
			str.Format("CPTDigital::LoadPTHistoryDigitalData,查询失败,原因为%s", sError);
			WriteLog(str, XJ_LOG_LV3);
			bReturn = FALSE;
		}
	}

	WriteLogEx("CPTDigital::LoadPTHistoryDigitalData, 查询历史值结束", XJ_LOG_LV3);
	return bReturn;
}

/*************************************************************
 函 数 名：OnSTTP20012()
 功能概要：响应20012(保护开关量采样值下载通知)报文
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B930090
void CPTDigital::OnSTTP20012( WPARAM wParam,LPARAM lParam )
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
	
	//检查是否20012报文
	if(pSttpData->sttp_head.uMsgID != 20012)
	{
		//错误报文
		CString str;
		str.Format("CPTDigital::OnSTTP20012 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTDigital::OnSTTP20012 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//检查CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//不是本CPU的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20012 收到错误CPU报文, CPU为%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	if(m_bFirstFrame)
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
		WriteLog("CPTDigital::OnSTTP20012 处理报文失败", XJ_LOG_LV3);
	}

	if( pSttpData->sttp_head.uMsgEndFlag == 1 )
	{
		//还有后续帧
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_SUCCESS, XJ_OPER_CALL_DIGTIAL, m_sCPU);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLDIGITAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DIGTIAL, OPER_SUCCESS);
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;

	//根据当前状态更新控件可用性
	UpdateControlsEnable();
	CString str;
	str.Format("保护[%s]召唤开关量成功", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV2);
}

//##ModelId=49B87B93004E
void CPTDigital::OnBtnPtdiCall() 
{
	// TODO: Add your control notification handler code here
	//响应按钮按下操作, 召唤开关量或取消召唤
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

		//组织20011报文
		if(!pApp->m_SttpEngine.BuildDataFor20011PTDigital(sttpData, m_pObj->m_sID, atoi(m_sCPU)))
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
				str.Format("保护[%s]发送召唤开关量报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送召唤开关量报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox( StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,状态改为正在召唤开关量
			m_nCurrentStatus = 1;
			m_bFirstFrame = TRUE;

			//启动定时器
			CDataEngine * pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nPTDiTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			
			//根据当前状态更新控件可用性
			UpdateControlsEnable();
			CString str;
			str.Format("保护[%s]下发开关量召唤命令成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else 
	{
		//正在召唤开关量
		//取消召唤
		CancelCall();
	}
}

/*************************************************************
 函 数 名：LoadDataFromSTTP()
 功能概要：从STTP报文读入开关量数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1 报文数据
**************************************************************/
//##ModelId=49B87B92031C
BOOL CPTDigital::LoadDataFromSTTP( STTP_FULL_DATA * sttpData )
{
	if(sttpData == NULL)
		return FALSE;
	
	//解析开关量数据
	try
	{
		
		//读取每项值,加入到列表
		int nCount = sttpData->sttp_body.variant_member.value_data.size();
		for(int i = 0; i < nCount; i++)
		{
			CString id;
			id.Format("%d", sttpData->sttp_body.variant_member.value_data[i].id);
			CString Value = XJToMS(sttpData->sttp_body.variant_member.value_data[i].str_value);
			AddDigitalValue(id, Value, m_nNewValueIndex);
		}
	}
	catch (...)
	{
		WriteLog("CPTDigital::LoadDataFromSTTP, 错误", XJ_LOG_LV3);
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
//##ModelId=49B87B93009C
void CPTDigital::OnSTTP20069( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤开关量的状态,不处理报文
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
		//不是本装置的报文
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
		str.Format("保护[%s]召唤开关量失败!", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV1);
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_DIGTIAL, m_sCPU);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLDIGITAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DIGTIAL, OPER_FAILD);

	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;

	//根据当前状态更新控件可用性
	UpdateControlsEnable();
	AfxMessageBox( StringFromID(IDS_CALL_FAIL));
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：响应20125(装置不支持功能回应报文)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B9300AB
void CPTDigital::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1)
	{
		//没有处于召唤开关量的状态,不处理报文
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
	
	//处理报文
	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;
			
	//根据当前状态更新控件可用性
	UpdateControlsEnable();

	CString str;
	str.Format("保护[%s]召唤开关量失败,原因为:装置不支持", m_pObj->m_sName);
	WriteLog(str, XJ_LOG_LV1);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_DIGTIAL, m_sCPU);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLDIGITAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DIGTIAL, OPER_FAILD);

	AfxMessageBox( StringFromID(IDS_CALLFAIL_UNSUPPORT));
}

/*************************************************************
 函 数 名：UpdateControlsEnable()
 功能概要：根据当前状态更新控件可用性
 返 回 值: void
**************************************************************/
//##ModelId=49B87B92031E
void CPTDigital::UpdateControlsEnable()
{
	//按状态分情况
	if(m_nCurrentStatus == 0)
	{	
		//召唤按钮文字改为召唤
		SetDlgItemText(IDC_BTN_PTDI_CALL, StringFromID(IDS_BTN_CALLDIGITAL));
		
		//打印和风格设置按钮
		GetDlgItem(IDC_BTN_PTDI_PRINT) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTDI_EXCEL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTDI_STYLE) ->EnableWindow(TRUE);
		
		//下拉框
		GetDlgItem(IDC_CMB_PTDI_CPU) ->EnableWindow(TRUE);
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
		SetDlgItemText(IDC_BTN_PTDI_CALL, CANCEL_CALL);
		//打印和风格设置按钮
		GetDlgItem(IDC_BTN_PTDI_PRINT) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTDI_EXCEL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTDI_STYLE) ->EnableWindow(FALSE);
		
		//下拉框
		GetDlgItem(IDC_CMB_PTDI_CPU) ->EnableWindow(FALSE);
		//开始动画
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//开始计时
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime, StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTDI, str);
	}
}

/*************************************************************
 函 数 名：OnPTFrameOpen()
 功能概要：响应保护属性页面打开消息, 刷新页面内容
 返 回 值: void
 参    数：param1	消息参数
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B9300CB
void CPTDigital::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新对应的二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;

	RefreshControls();
	//页面还原
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	//清除召唤计时
	SetDlgItemText(IDC_STATIC_TIME_PTDI, "");

	ReFillAll();
}

/*************************************************************
 函 数 名：RefreshControls()
 功能概要：刷新控件值, 在第一次启动和更新对应保护时调用
 返 回 值: void
**************************************************************/
//##ModelId=49B87B92032C
void CPTDigital::RefreshControls()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTDI_CPU);
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
//##ModelId=49B87B92032D
BOOL CPTDigital::SaveSelCPU( CComboBox * pCmb )
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

//##ModelId=49B87B930050
void CPTDigital::OnSelchangeCmbPtdiCpu() 
{
	// TODO: Add your control notification handler code here
	//保存当前选择的CPU
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTDI_CPU);
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
 功能概要：设置差异时list有文本颜色
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B93005D
void CPTDigital::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

//##ModelId=49B87B930061
void CPTDigital::OnTimer(UINT nIDEvent) 
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
			CString str;
			str.Format("保护[%s]召唤开关量失败,原因为:超时", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			
			CString strMsg;
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_DIGTIAL, m_sCPU);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLDIGITAL, m_pObj->m_sID, strMsg, XJ_OPER_CALL_DIGTIAL, OPER_FAILD);
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
		SetDlgItemText(IDC_STATIC_TIME_PTDI, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：ReFillAll()
 功能概要：重新读取和填充所有内容
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B92034D
BOOL CPTDigital::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();
	
	GetDlgItem(IDC_STATIC_PTDIGITAL_LOADING)->ShowWindow(SW_SHOW);
	
	//清空内容
	EnterCriticalSection(&m_CriticalOper);
	m_HisValueMgr.ResetContent();
	LeaveCriticalSection(&m_CriticalOper);
	ClearDigitalConfig();
	ClearDataCol();
	m_List.DeleteAllItems();
	
	//读取数据
	if(MODE_SUB == g_role)
	{
		//直接读取
		LoadPTDigitalConfig(0);
		LoadPTHistoryDigitalData(0);
		AddValueCol(0);
		FillListData(0);
		GetDlgItem(IDC_STATIC_PTDIGITAL_LOADING)->ShowWindow(SW_HIDE);
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
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9300CE
void CPTDigital::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
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
//##ModelId=49B87B92033C
void CPTDigital::ClearDataCol()
{
	//检查数据有效性
	if(m_List.GetSafeHwnd() == NULL)
		return;
	EnterCriticalSection(&m_CriticalOper);
	int nCount = m_List.GetHeaderCtrl()->GetItemCount();
	while(nCount > m_nNewValueIndex)
	{
		m_List.DeleteColumn(m_nNewValueIndex);
		nCount = m_List.GetHeaderCtrl()->GetItemCount();
	}
	LeaveCriticalSection(&m_CriticalOper);
}

/*************************************************************
 函 数 名：CancelCall()
 功能概要：取消召唤操作
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B92033D
void CPTDigital::CancelCall()
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
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20011, m_pObj->m_sID, atoi(m_sCPU));
	}
	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_nCurrentStatus = 0;
	//更改控件可用性
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：退出自己所起的线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B92035B
void CPTDigital::EndOwnerThread()
{
	WriteLog("开始退出保护开关量页面读取数据线程", XJ_LOG_LV3);
	m_bExitThread = TRUE;
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("退出保护开关量页面读取数据线程完毕", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：IsVaild20069()
 功能概要：判断收到的20069是否自己的
 返 回 值: 是自己的失败报文返回TRUE, 否则返回FALSE
 参    数：param1	失败的操作报文ID
		   Param2
**************************************************************/
//##ModelId=49B87B92033E
BOOL CPTDigital::IsVaild20069( int nMsgID )
{
	if(20011 == nMsgID && 1 == m_nCurrentStatus)
	{
		//召唤开关量
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B93006E
BOOL CPTDigital::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTDI);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

/*************************************************************
 函 数 名：DisplayValue()
 功能概要：得到开关量的值显示字符串
 返 回 值: 显示值
 参    数：param1	开关量值
		   Param2
**************************************************************/
//##ModelId=49B87B92034B
CString CPTDigital::DisplayValue( int nValue )
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
			return StringFromID(IDS_CASE_OPENING);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_CLOSING);
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

//##ModelId=49B87B9300DC
void CPTDigital::OnRefreshPTDI( WPARAM wParam, LPARAM lParam )
{
	//ReFillAll();
	StartThread();
}

//##ModelId=49B87B93007D
void CPTDigital::OnBtnPtdiStyle() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->DoSysSetting(SYS_SETTING_PTPROP);
}

//##ModelId=49B87B920399
void CPTDigital::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_DIGITAL));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_DIGITAL));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//##ModelId=49B87B93007F
void CPTDigital::OnShowWindow(BOOL bShow, UINT nStatus) 
{

	CViewBase::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名：TranslateSingleKey()
 功能概要：解释单个关键字的意义
 返 回 值: 关键字表示的值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B92035C
CString CPTDigital::TranslateSingleKey( CString sKey )
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
//##ModelId=49B87B92035E
BOOL CPTDigital::InitCOM()
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
//##ModelId=49B87B92035F
void CPTDigital::ReleaseCOM()
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
 功能概要：打印开关量
 返 回 值: 打印模板全路径
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B920360
void CPTDigital::DoPrintDigital( CString strPath )
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
			sLog.Format("%s\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL), strPath);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//设置回调函数
	if(pReportDoc != NULL && bOpened)
	{
		if (m_pObj->m_nType == TYPE_WAVEREC)
		{
			pReportDoc->SetQueryFunction(TranslateKeyInWRDigital);
		}
		else
			pReportDoc->SetQueryFunction(TranslateKeyInPTDigital);
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

//##ModelId=49B87B930083
void CPTDigital::OnRoutineprintNew1() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintDigital(DIGITAL_NEW1_TP);
}

//##ModelId=49B87B93008C
void CPTDigital::OnRoutineprintNew2() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintDigital(DIGITAL_NEW2_TP);
}

//##ModelId=49B87B93008E
void CPTDigital::OnRoutineprintNew3() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintDigital(DIGITAL_NEW3_TP);
}

/*************************************************************
 函 数 名：OnSTTP20157()
 功能概要：响应超时通知报文
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B9300BB
void CPTDigital::OnSTTP20157( WPARAM wParam,LPARAM lParam )
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
		//正在召唤开关量				
		int nTimeOut = pData->m_nPTDiTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}

void CPTDigital::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTDigital::OnDoubleclickedBtnPtdiCall() 
{
	// TODO: Add your control notification handler code here
	
}

/****************************************************
Date:2012/12/6  Author:LYH
函数名:   FindDigital	
返回值:   PT_DIGITAL*	
功能概要: 
参数: CString sID	
*****************************************************/
PT_DIGITAL* CPTDigital::FindDigital( CString sID )
{
	for(int i = 0; i < m_arrDigital.GetSize(); i++)
	{
		PT_DIGITAL* pd = (PT_DIGITAL*)m_arrDigital.GetAt(i);
		if(pd == NULL)
			continue;
		if(pd->ID == sID)
			return pd;
	}
	return NULL;
}

void CPTDigital::StartThread()
{
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTDigitalLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("模拟量查询线程启动成功,查询批次:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTDigital::GetNextQueryNo()
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

void CPTDigital::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("退出保护开关量查询线程,查询号:%d", nNo);
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
	str.Format("保护开关量查询线程成功退出,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTDigital::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("保护开关量查询线程成功启动,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTDigital::GetFirstNode()
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

void CPTDigital::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//过期的批次
		return;
	}
	
	AddValueCol(wParam);
	FillListData(wParam);

	GetDlgItem(IDC_STATIC_PTDIGITAL_LOADING)->ShowWindow(SW_HIDE);
}

void CPTDigital::AddValueCol( int nQueryNo )
{
	if(g_PTDIValues <= 0)
		return;
	if(m_arrDigital.GetSize == 0)
		return;
	if(MODE_SUB != g_role && !IsCurrentQueryNo(nQueryNo))
		return;
	//加入值列
	CString strTimeDisplay;
	CString strTime = "";
	WriteLog("CPTDigital::AddValueCol, 开始加入值列", XJ_LOG_LV3);
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
	
	WriteLog("CPTDigital::AddValueCol, 加入值列完毕", XJ_LOG_LV3);
}

int CPTDigital::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

BOOL CPTDigital::LoadDefaultCpu()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTDI_CPU);
	if(pCmb == NULL)
		return TRUE;
	if( pCmb->GetCount() == 0 )
		return TRUE;
	if(m_pObj == NULL)
		return FALSE;
	
	WriteLog("CPTDigital::LoadDefaultCpu, 查询开始", XJ_LOG_LV3);
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTDigital::LoadDefaultCpu, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTDigital::LoadDefaultCpu,查询到%d条数据", nCount);
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
		str.Format("CPTDigital::LoadDefaultCpu,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLog("CPTDigital::LoadDefaultCpu, 查询结束", XJ_LOG_LV3);
	
	return bReturn;
}

void CPTDigital::OnBtnPtdiExcel() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString strName;
	strName.Format("%s(%s-CPU[%s])-%s.xls", StringFromID(IDS_SECPROP_DIGITAL),m_pObj->m_sName, m_sCPU, t.Format("%Y%m%d%H%M%S"));
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
