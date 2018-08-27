// PTSoftBoard.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTSoftBoard.h"
#include "DlgCheck.h"
#include "DlgDataCheck.h"
#include "DlgValidateID.h"
#include "DlgValidateUser.h"
#include "GlobalFunc.h"

#include "MainFrm.h"
#include "DlgOperHis.h"

#include "XJTagOutStore.h"
#include "XJUserStore.h"
#include "XJUtilsStore.h"
#include "qptsetstatetable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int PTSETSOFT_TIMER_LEN = 2;

UINT PTSoftBoardLoad(LPVOID pParam)
{
	CPTSoftBoard* ptSoftBoard = (CPTSoftBoard*)pParam;
	if(ptSoftBoard == NULL)
		return 0;
	int nQueryNo = ptSoftBoard->GetCurrentQueryNo();
	//读取内容
	if(ptSoftBoard->IsCurrentQueryNo(nQueryNo) && !ptSoftBoard->m_bExitThread)
	{
		BOOL bLoad = ptSoftBoard->LoadPTSoftBoardConfig(nQueryNo);
		if(bLoad)
		{
			ptSoftBoard->LoadPTHistorySoftBoardData(nQueryNo);
			ptSoftBoard->PostMessage(THREAD_FILL_DATA, nQueryNo, 0);
		}
	}
	
	ptSoftBoard->RemoveThreadNode(nQueryNo);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPTSoftBoard

IMPLEMENT_DYNCREATE(CPTSoftBoard, CViewBase)

//##ModelId=49B87B89037C
CPTSoftBoard::CPTSoftBoard()
	: CViewBase(CPTSoftBoard::IDD)
{
	//{{AFX_DATA_INIT(CPTSoftBoard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrSoftBoard.RemoveAll();
	m_pObj = NULL;
	m_sCPU = "";
	m_nNewValueIndex = 3;
	m_nCurrentStatus = 0;
	m_nCurrentDetailStatus = 0;
	m_nTimer = 0;
	m_nRecordTimer = 0;
	m_nOPTime = 0;
	m_nOperationNum = -1;
	m_bExitThread = FALSE;
	m_pLoadThread = NULL;
	m_arrColum.RemoveAll();

	pReportDoc = NULL;
	pClf = NULL;
	pUnk = NULL;

	m_bOperVerify = false;
	m_bMonVerify = false;
	m_sOperUser = "";
	m_sMonUser = "";

	m_bChecking = FALSE;
	m_bAlreadyShowOnce = false;

	m_nQueryNo = 0;
	m_listThread.RemoveAll();
	InitializeCriticalSection(&m_CriticalSection);
	InitializeCriticalSection(&m_CriticalOper);
}

//##ModelId=49B87B8A02B1
CPTSoftBoard::~CPTSoftBoard()
{
	ClearSoftBoardConfig();
	ClearModifyList();
	DeleteCriticalSection(&m_CriticalOper);
	DeleteCriticalSection(&m_CriticalSection);
}

//##ModelId=49B87B8A0271
void CPTSoftBoard::DoDataExchange(CDataExchange* pDX)
{
	CViewBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPTSoftBoard)
	DDX_Control(pDX, IDC_BTN_PTSOFT_VERIFY2, m_btnVerify2);
	DDX_Control(pDX, IDC_BTN_PTSOFT_VERIFY1, m_btnVerify1);
	DDX_Control(pDX, IDC_GIF_PTSOFT, m_gif);
	DDX_Control(pDX, IDC_BTN_PTSOFTSET_VIEWPROG, m_btnViewProg);
	DDX_Control(pDX, IDC_BTN_PTSOFTSET_VIEW_HIS, m_btnViewHis);
	DDX_Control(pDX, IDC_BTN_PTSOFT_STYLE, m_btnStyle);
	DDX_Control(pDX, IDC_BTN_PTSOFT_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BTN_PTSOFT_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_BTN_PTSOFT_CALL, m_btnCall);
	DDX_Control(pDX, IDC_LIST_PTSOFT, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPTSoftBoard, CViewBase)
	//{{AFX_MSG_MAP(CPTSoftBoard)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PTSOFT_CALL, OnBtnPtsoftCall)
	ON_BN_CLICKED(IDC_BTN_PTSOFT_MODIFY, OnBtnPtsoftModify)
	ON_BN_CLICKED(IDC_BTN_PTSOFTSET_VIEWPROG, OnBtnViewProg)
	ON_BN_CLICKED(IDC_BTN_PTSOFTSET_VIEW_HIS, OnBtnViewHis)
	ON_CBN_SELCHANGE(IDC_CMB_PTSOFT_CPU, OnSelchangeCmbPtsoftCpu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PTSOFT, OnCustomDraw)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PTSOFT, OnClickListPtsoft)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PTSOFT_STYLE, OnBtnPtsoftStyle)
	ON_COMMAND(ID_ROUTINEPRINT_NEW1, OnRoutineprintNew1)
	ON_COMMAND(ID_ROUTINEPRINT_NEW2, OnRoutineprintNew2)
	ON_COMMAND(ID_ROUTINEPRINT_NEW3, OnRoutineprintNew3)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_DOUBLECLICKED(IDC_BTN_PTSOFT_CALL, OnDoubleclickedBtnPtsoftCall)
	ON_BN_CLICKED(IDC_BTN_PTSOFT_VERIFY1, OnBtnPtsoftVerify1)
	ON_BN_CLICKED(IDC_BTN_PTSOFT_VERIFY2, OnBtnPtsoftVerify2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PTSOFT, OnDblclkListPtsoft)
	ON_BN_CLICKED(IDC_BTN_PTSOFT_EXCEL, OnBtnPtsoftExcel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20109, OnSTTP20109)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20118, OnSTTP20118)
	ON_MESSAGE(WM_STTP_20114, OnSTTP20114)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_PTFRAME_CLOSE, OnPTFrameClose)
	ON_MESSAGE(REFRESH_PTSOFTBOARD, OnRefreshPTSoft)
	ON_MESSAGE(THREAD_FILL_DATA, OnThreadFillData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTSoftBoard diagnostics

#ifdef _DEBUG
//##ModelId=49B87B8A02DE
void CPTSoftBoard::AssertValid() const
{
	CViewBase::AssertValid();
}

//##ModelId=49B87B8A02FD
void CPTSoftBoard::Dump(CDumpContext& dc) const
{
	CViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPTSoftBoard message handlers

//##ModelId=49B87B8A0246
void CPTSoftBoard::OnInitialUpdate() 
{
	//记录控件位置
	RecordRate(IDC_BTN_PTSOFT_CALL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFT_PRINT, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFT_MODIFY, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFT_STYLE, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_VERIFY1, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSET_VERIFY2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSOFT, 0, left_client, top_client);
	RecordRate(IDC_CMB_PTSOFT_CPU, 0, left_client, top_client);
	RecordRate(IDC_GIF_PTSOFT, 0, left_client, top_client);
	RecordRate(IDC_STATIC_TIME_PTSB, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSB_P1, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSB_P2, 0, left_client, top_client);
	RecordRate(IDC_STATIC_PTSB_P3, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFT_EXCEL, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFTSET_VIEWPROG, 0, left_client, top_client);
	RecordRate(IDC_BTN_PTSOFTSET_VIEW_HIS, 0, left_client, top_client);

	RecordRate(IDC_STATIC_PTSOFT_LOADING, 0, mid_client, top_client);
	CViewBase::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	SetScrollSizes(MM_TEXT, szDlg);
//	ResizeParentToFit();

	m_btnVerify1.SetIcon(IDI_KEY1);
	m_btnVerify1.SetFlat(FALSE);
	
	m_btnVerify2.SetIcon(IDI_KEY1);
	m_btnVerify2.SetFlat(FALSE);

	if(g_PTControlModel == 1)//普通模式
	{
		m_btnVerify1.ShowWindow(SW_HIDE);
		m_btnVerify2.ShowWindow(SW_HIDE);
	}

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
//##ModelId=49B87B8A005D
int CPTSoftBoard::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	InitListStyle();
/*	RefreshControls();
	LoadPTSoftBoardConfig();
	LoadPTHistorySoftBoardData();*/

	return 0;
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B8A007D
int CPTSoftBoard::InitListStyle()
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
		//char* arColumn[3]={"序号","软压板名称", "软压板代码"};
		
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
				lvCol.cx = 140; // 软压板名称
				colName = StringFromID(IDS_COMMON_NAME);
				break;
			case 2://
				lvCol.cx = 140; // 软压板代码
				colName = StringFromID(IDS_COMMON_CODE);
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		//默认隐藏第3列(软压板)
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
//##ModelId=49B87B8A00CB
BOOL CPTSoftBoard::LoadListConfig()
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
			WriteLog("PTSoftBoard::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("PTSoftBoard::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//PTSoftBoardConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到PTSoftBoardConfig", XJ_LOG_LV3);
				WriteLog("PTSoftBoard::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到PTSoftBoardConfig下的配置", XJ_LOG_LV3);
					WriteLog("PTSoftBoard::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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

//##ModelId=49B87B8A00EA
BOOL CPTSoftBoard::SaveListConfig()
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
			WriteLog("PTSoftBoard::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("PTSoftBoard::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//PTSoftBoardConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewCommConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("PTSoftBoardConfig");
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
						WriteLog("未找到PTSoftBoardConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
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
						WriteLog("找到了PTSoftBoardConfig下的配置, 先删除新建所有子节点", XJ_LOG_LV3);
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
//##ModelId=49B87B8A00AB
int CPTSoftBoard::FillListData(INT nQueryNo)
{
	WriteLog("CPTSoftBoard::FillListData, 开始填充数据", XJ_LOG_LV3);

	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	EnterCriticalSection(&m_CriticalOper);
	CString strLog;
	strLog.Format("CPTSoftBoard::FillListData, 共有%d批历史值", m_HisValueMgr.GetCount());
	WriteLog(strLog, XJ_LOG_LV3);
	PT_SOFTBOARD *pts = NULL;
	for(int i = 0; i < m_arrSoftBoard.GetSize(); i ++)
	{
		if(g_role != MODE_SUB && (!IsCurrentQueryNo(nQueryNo)))
			break;

		pts = m_arrSoftBoard.GetAt(i);
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
	//恢复刷新
	m_List.SetRedraw(TRUE);
	LeaveCriticalSection(&m_CriticalOper);
	WriteLog("CPTSoftBoard::FillListData, 填充数据完毕", XJ_LOG_LV3);
	return 0;
}

//##ModelId=49B87B8A0109
BOOL CPTSoftBoard::NeedSave()
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
//##ModelId=49B87B8A0128
int CPTSoftBoard::InitControls()
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
	m_btnPrint.SetTooltipText(StringFromID(IDS_COMMON_PRINT));
	m_btnPrint.SetMenu(IDR_MENU_ROUTINE_PRINT, GetSafeHwnd());
/*
	//软压板投退
	m_btnModify.SetThemeHelper(&m_ThemeHelper);
//	m_btnModify.SetIcon(IDI_MODIFY);
	m_btnModify.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 20);
	m_btnModify.SetTooltipText(_T("投退软压板"));
*/
	return 0;
}


//##ModelId=49B87B8A032C
void CPTSoftBoard::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	MoveCtrl(IDC_LIST_PTSOFT, left_client, IDC_BTN_PTSOFT_CALL, right_client, bottom_client);
}

//##ModelId=49B87B8A035B
void CPTSoftBoard::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();
	CViewBase::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名：LoadPTSoftBoardConfig()
 功能概要：从数据库中读取软压板配置信息,tb_pt_softboard_def
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8A0148
BOOL CPTSoftBoard::LoadPTSoftBoardConfig(int nQueryNo)
{
	if(!m_pObj)
		return FALSE;
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	WriteLog("CPTSoftBoard::LoadPTSoftBoardConfig, 开始查询", XJ_LOG_LV3);
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//根据PT_ID, cpu_code查找SOFT_ID,CODE_NAME,NAME,按DI_ID从小到大排列
		
	CString str;
		
	//字段
	//SOFT_ID
	Field Field1;
	bzero(&Field1, sizeof(Field));
	str = "SOFT_ID";
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
	str.Format("order by SOFT_ID");
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SOFTBOARD_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSoftBoard::LoadPTSoftBoardConfig, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			if(MODE_SUB != g_role && i%20 == 0)
				Sleep(1);
			//创建对象
			PT_SOFTBOARD * softboard = new PT_SOFTBOARD;
			softboard->ID = pMemset.GetValue((UINT)0); //DI_ID
			softboard->Name = pMemset.GetValue(1); //NAME
			softboard->CodeName = pMemset.GetValue(2); //Code_Name
			m_arrSoftBoard.Add(softboard);
					
			pMemset.MoveNext();
		}
		LeaveCriticalSection(&m_CriticalOper);
	}
	else
	{
		CString str;
		str.Format("CPTSoftBoard::LoadPTSoftBoardConfig,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLog("CPTSoftBoard::LoadPTSoftBoardConfig, 查询结束", XJ_LOG_LV3);

	return bReturn;
}

/*************************************************************
 函 数 名：ClearSoftBoardConfig()
 功能概要：清除软压板配置信息
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8A0167
BOOL CPTSoftBoard::ClearSoftBoardConfig()
{
	EnterCriticalSection(&m_CriticalOper);
	for(int i = 0; i < m_arrSoftBoard.GetSize(); i++)
	{
		PT_SOFTBOARD * digital = (PT_SOFTBOARD*)m_arrSoftBoard.GetAt(i);
		if(digital != NULL)
		{
			delete digital;
			digital = NULL;
		}
	}
	m_arrSoftBoard.RemoveAll();
	LeaveCriticalSection(&m_CriticalOper);
	return TRUE;
}

/*************************************************************
 函 数 名：AddSoftBoardValue()
 功能概要：加入某软压板值到列表
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1   要加入的软压板的ID
           Param2   要加入的软压板的值
		   Param3	指定要加入到第几列, 此值应该>=3
**************************************************************/
//##ModelId=49B87B8A0177
BOOL CPTSoftBoard::AddSoftBoardValue(CString ID, CString VALUE, int iCol)
{
	//先找到DI_ID==ID的索引
	BOOL bReturn = TRUE;
	CString strID = ID;
	strID.TrimRight();
	
	PT_SOFTBOARD* pst = FindSoftBoard(strID);
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
	bReturn = m_List.SetItemText(nItem, iCol, DisplayValue(atoi(VALUE)));
	return bReturn;
}

/*************************************************************
 函 数 名：LoadPTHistorySoftBoardData()
 功能概要：载入软压板历史值
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8A0196
BOOL CPTSoftBoard::LoadPTHistorySoftBoardData(int nQueryNo)
{
	if(!m_pObj)
		return FALSE;
	if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
		return FALSE;

	BOOL bReturn = TRUE;

	if(g_PTSoftValues <= 0)
		return TRUE;

	WriteLog("CPTSoftBoard::LoadPTHistorySoftBoardData, 查询time开始", XJ_LOG_LV3);
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
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SOFTBOARD_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSoftBoard::LoadPTHistorySoftBoardData, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString strLog;
		strLog.Format("CPTSoftBoard::LoadPTHistorySoftBoardData,查询到%d条time", nCount);
		WriteLog(strLog, XJ_LOG_LV3);

		EnterCriticalSection(&m_CriticalOper);
		for(int i = 0; i < nCount; i++)
		{
			if(MODE_SUB != g_role && (!IsCurrentQueryNo(nQueryNo) || m_bExitThread))
				break;
			
			if(i >= g_PTSoftValues)
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
		str.Format("CPTSoftBoard::LoadPTHistorySoftBoardData,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	

	WriteLog("CPTSoftBoard::LoadPTHistorySoftBoardData, 查询time结束", XJ_LOG_LV3);

	WriteLog("CPTSoftBoard::LoadPTHistorySoftBoardData, 查询历史值开始", XJ_LOG_LV3);
	
	EnterCriticalSection(&m_CriticalOper);
	int nHisCount = m_HisValueMgr.GetCount();
	LeaveCriticalSection(&m_CriticalOper);
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
		str = "SOFT_ID";
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
				
		CMemSet pMemset;

		char sError[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
			
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SOFTBOARD_DATA, sql, sError, &pMemset);
		}
		catch (...)
		{
			WriteLogEx("CPTSoftBoard::LoadPTHistorySoftBoardData, 查询失败");
			return FALSE;
		}
		if(nResult == 1)
		{
			pMemset.MoveFirst();
			int nCount = pMemset.GetMemRowNum();
			CString str;
			str.Format("CPTSoftBoard::LoadPTHistorySoftBoardData,查询%s的历史数据,条数为%d", strTime, nCount);
			WriteLog(str, XJ_LOG_LV3);

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
			str.Format("CPTSoftBoard::LoadPTHistorySoftBoardData,查询失败,原因为%s", sError);
			WriteLogEx(str);
			bReturn = FALSE;
		}
	}

	WriteLog("CPTSoftBoard::LoadPTHistorySoftBoardData, 查询历史值结束", XJ_LOG_LV3);
	return TRUE;
}

//##ModelId=49B87B8A037A
void CPTSoftBoard::OnBtnPtsoftCall() 
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
		
		//组织20108报文
		if(!pApp->m_SttpEngine.BuildDataFor20108PTSoftBoard(sttpData, m_pObj->m_sID, atoi(m_sCPU)))
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
				str.Format("保护[%s]发送召唤软压板报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送召唤软压板报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
		}
		else
		{
			//成功,状态改为正在召唤软压板
			m_nCurrentStatus = 1;
			m_bFirstFrame = TRUE;
			UpdateControlsEnable();

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nPTSoftBoardTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
			//增加人工操作日志
			//pApp->AddManOperator(FUNC_QUY_CALLSOFT, m_pObj->m_sName);
			CString str;
			str.Format("保护[%s]发送召唤软压板报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 1)
	{
		//正在召唤软压板
		//取消召唤
		CancelCall();
	}
	else
	{
		AfxMessageBox(StringFromID(IDS_CALL_BUSY));
	}
}

/*************************************************************
 函 数 名：OnSTTP20109()
 功能概要：响应20109(召唤软压板通知)报文
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8A03DE
void CPTSoftBoard::OnSTTP20109( WPARAM wParam,LPARAM lParam )
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
	
	//检查是否20109报文
	if(pSttpData->sttp_head.uMsgID != 20109)
	{
		//错误报文
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20109 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20109 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	//检查CPU
	int nCpu = pSttpData->sttp_body.nCpu;
	if(nCpu != atoi(m_sCPU))
	{
		//不是本CPU的报文
		CString str;
		str.Format("CPTSetting::OnSTTP20109 收到错误CPU报文, CPU为%d", nCpu);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}

	if(m_bFirstFrame)
	{
		m_bFirstFrame = FALSE;
		//时间,取当前系统时间
		CTime tmTime = CTime::GetCurrentTime();
		CString strTimeDisplay = tmTime.Format("%Y-%m-%d %H:%M:%S");
		//新加一列
		int Index = m_List.InsertColumn(m_nNewValueIndex, strTimeDisplay, LVCFMT_LEFT, 150, 0);
	}
	
	//处理报文
	if(!LoadDataFromSTTP(pSttpData))
	{
		WriteLog("CPTSoftBoard::OnSTTP20109 处理报文失败", XJ_LOG_LV3);
	}

	if( pSttpData->sttp_head.uMsgEndFlag == 1 ){
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);

	CString sLog;
	sLog.Format("保护[%s]召唤软压板成功!", m_pObj->m_sName);
	WriteLog(sLog, XJ_LOG_LV2);
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strMsg;
	strMsg.Format("%d,%d,%s", OPER_SUCCESS, XJ_OPER_CALL_SOFTBOARD, m_sCPU);
	
	pApp->AddNewManOperator(FUNC_QUY_CALLSOFT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SOFTBOARD, OPER_SUCCESS);

	//处理完毕,改变状态和按钮文字
	m_nCurrentStatus = 0;

	//更新控件可用性
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：LoadDataFromSTTP()
 功能概要：从STTP报文中读取软压板数据
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1	报文数据
**************************************************************/
//##ModelId=49B87B8A01B5
BOOL CPTSoftBoard::LoadDataFromSTTP( STTP_FULL_DATA * sttpData )
{
	if(sttpData == NULL)
		return FALSE;
	
	//解析软压板数据
	try
	{
		
		//读取每项值,加入到列表
		int nCount = sttpData->sttp_body.variant_member.value_data.size();
		for(int i = 0; i < nCount; i++)
		{
			CString id;
			id.Format("%d", sttpData->sttp_body.variant_member.value_data[i].id);
			CString Value = XJToMS(sttpData->sttp_body.variant_member.value_data[i].str_value);
			AddSoftBoardValue(id, Value, m_nNewValueIndex);
		}
	}
	catch (...)
	{
		WriteLog("CPTSoftBoard::LoadDataFromSTTP, 错误", XJ_LOG_LV3);
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
//##ModelId=49B87B8B000F
void CPTSoftBoard::OnSTTP20069( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2)
	{
		//没有处于召唤软压板状态,也没有处于软压板投退状态,不处理报文
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
		m_nCurrentStatus = 0;
		
		//更新控件可用性
		UpdateControlsEnable();
	}
	else
	{
		//操作失败
		//处理完毕,改变状态和按钮文字
		if(m_nCurrentStatus == 1)
		{
			//正在召唤软压板
				
			CString str;
			str.Format("保护[%s]召唤软压板失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			m_nCurrentStatus = 0;
			
			//更新控件可用性
			UpdateControlsEnable();
			AfxMessageBox(StringFromID(IDS_CALL_FAIL));
			
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CString strMsg;
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_SOFTBOARD, m_sCPU);
			
			pApp->AddNewManOperator(FUNC_QUY_CALLSOFT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SOFTBOARD, OPER_FAILD);
		}
		if(m_nCurrentStatus == 2)
		{
			//正在软压板投退
			CString str;
			str.Format("保护[%s]软压板投退失败!", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV1);
			
			m_bMonVerify = false;
			m_bOperVerify = false;

			m_nCurrentStatus = 0;

			//回复修改前的值
			RevertModifyValue();
			
			//更新控件可用性
			UpdateControlsEnable();
			AfxMessageBox(StringFromID(IDS_EXECUTE_MODIFYSOFT_FAIL));
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			if(m_bChecking)
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, m_sCPU, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				pApp->AddNewManOperator("软压板投退预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, m_sCPU, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				pApp->AddNewManOperator("软压板投退执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
		}	
	}
}

/*************************************************************
 函 数 名：OnSTTP20125()
 功能概要：响应20125(装置不支持功能回应报文)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8B001F
void CPTSoftBoard::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2)
	{
		//没有处于召唤软压板状态也没有处于软压板投退状态,不处理报文
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

	if(!IsVaild20069(pSttpData->sttp_body.nMsgId))
	{
		//不是本功能的报文
		return;
	}

	//关闭定时器
	KillTimer(m_nTimer);
	
	//处理报文
	//处理完毕,改变状态和按钮文字
	if(m_nCurrentStatus == 1)
	{
		//正在召唤软压板
		m_nCurrentStatus = 0;
		
		//更新控件可用性
		UpdateControlsEnable();

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_SOFTBOARD, m_sCPU);
		
		pApp->AddNewManOperator(FUNC_QUY_CALLSOFT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SOFTBOARD, OPER_FAILD);

		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));
	}
	if(m_nCurrentStatus == 2)
	{
		m_bMonVerify = false;
		m_bOperVerify = false;

		//正在软压板投退
		m_nCurrentStatus = 0;

		//回复修改前的值
		RevertModifyValue();
		
		//更新控件可用性
		UpdateControlsEnable();
		AfxMessageBox(StringFromID(IDS_CALLFAIL_UNSUPPORT));

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		if(m_bChecking)
		{
			CString strMsg;
			strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, m_sCPU, m_arrModifyList.GetSize());
			for(int i = 0; i < m_arrModifyList.GetSize(); i++)
			{
				STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
				CString strTemp = sttpData->str_value.c_str();
				if(g_OperTableType == 0)
					strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
				else if(g_OperTableType == 1)
				{
					CString sOldValue = FindNewValue(sttpData->id);
					strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
				}
			}
			pApp->AddNewManOperator("软压板投退预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, OPER_FAILD, m_nOperationNum);
		}
		else
		{
			CString strMsg;
			strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, m_sCPU, m_arrModifyList.GetSize());
			for(int i = 0; i < m_arrModifyList.GetSize(); i++)
			{
				STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
				CString strTemp = sttpData->str_value.c_str();
				if(g_OperTableType == 0)
					strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
				else if(g_OperTableType == 1)
				{
					CString sOldValue = FindNewValue(sttpData->id);
					strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
				}
			}
			pApp->AddNewManOperator("软压板投退执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, OPER_FAILD, m_nOperationNum);
		}
	}
}

/*************************************************************
 函 数 名：UpdateControlsEnable()
 功能概要：根据当前状态改变控件可用性
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8A01C5
void CPTSoftBoard::UpdateControlsEnable()
{
	//按状态分情况
	if(m_nCurrentStatus == 0)
	{	
		//召唤软压板
		SetDlgItemText(IDC_BTN_PTSOFT_CALL, StringFromID(IDS_BTN_CALLSOFT));
		//软压板投退
		SetDlgItemText(IDC_BTN_PTSOFT_MODIFY, StringFromID(IDS_BTN_MODIFYSOFT));
		GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(TRUE);

		//空闲状态,都可用
		GetDlgItem(IDC_BTN_PTSOFT_CALL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(TRUE);
		
		//打印和风格设置按钮
		GetDlgItem(IDC_BTN_PTSOFT_PRINT) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSOFT_EXCEL) ->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PTSOFT_STYLE) ->EnableWindow(TRUE);
		
		//下拉框
		GetDlgItem(IDC_CMB_PTSOFT_CPU) ->EnableWindow(TRUE);
		//停止动画
		m_gif.Stop();
		m_gif.ShowWindow(SW_HIDE);
		//停止计时
		KillTimer(m_nRecordTimer);
		m_nOPTime = 0;

		if(g_PTControlModel == 2)//广东模式
		{
			if(!m_bOperVerify)
			{
				GetDlgItem(IDC_BTN_PTSOFT_VERIFY2)->EnableWindow(false);
				m_btnVerify1.SetIcon(IDI_KEY1);
				m_btnVerify2.SetIcon(IDI_KEY1);
			}
			else
			{
				m_btnVerify1.SetIcon(IDI_TICK);
				if (m_bMonVerify)
				{
					m_btnVerify2.SetIcon(IDI_TICK);
				}
				else
				{
					m_btnVerify2.SetIcon(IDI_KEY1);
				}
				GetDlgItem(IDC_BTN_PTSOFT_VERIFY2)->EnableWindow(true);
			}
			
			if (m_bOperVerify && m_bMonVerify)
			{
				m_btnVerify1.SetIcon(IDI_TICK);
				m_btnVerify2.SetIcon(IDI_TICK);
				GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(TRUE);
			}
			else
			{
				m_btnVerify1.SetIcon(IDI_KEY1);
				m_btnVerify2.SetIcon(IDI_KEY1);
				GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(FALSE);
				m_sOperUser = _T("");
				m_sMonUser = _T("");
			}
		}
	}
	else if(m_nCurrentStatus > 0 && m_nCurrentStatus < 3)
	{
		//召唤或操作状态,除了自己其它的都不可用
		
		GetDlgItem(IDC_BTN_PTSOFT_CALL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(FALSE);
		
		//打印和风格设置按钮
		GetDlgItem(IDC_BTN_PTSOFT_PRINT) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSOFT_EXCEL) ->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PTSOFT_STYLE) ->EnableWindow(FALSE);
		
		//下拉框
		GetDlgItem(IDC_CMB_PTSOFT_CPU) ->EnableWindow(FALSE);
		//开始动画
		m_gif.ShowWindow(SW_SHOW);
		m_gif.Draw();
		//开始计时
		m_nRecordTimer = SetTimer(2, 1000, 0);
		m_nOPTime = 0;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND), m_nOPTime, StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTSB, str);
		
		//让自己可用
		switch(m_nCurrentStatus)
		{
		case 1: //召唤软压板
			SetDlgItemText(IDC_BTN_PTSOFT_CALL, CANCEL_CALL);
			GetDlgItem(IDC_BTN_PTSOFT_CALL) ->EnableWindow(TRUE);
			break;
		case 2: //软压板投退
			SetDlgItemText(IDC_BTN_PTSOFT_MODIFY, CANCEL_NULL);
			GetDlgItem(IDC_BTN_PTSOFT_MODIFY) ->EnableWindow(TRUE);
			break;
		default:
			break;
		}
	}
}

//##ModelId=49B87B8A038A
void CPTSoftBoard::OnBtnPtsoftModify() 
{
	OnBtnPtsoftModify2();
	return;

	// TODO: Add your control notification handler code here
	//响应按钮按下操作, 召唤或取消召唤
	m_bChecking = FALSE;
	if(m_pObj == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	/*if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_CONTROL))
		{
			CString strOut;
			//收集被修改的值
			CheckModifyData(m_arrModifyList, strOut);
			//回复修改前的值
			RevertModifyValue();
			return;
		}
	}*/
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		CString strOut;
		//收集被修改的值
		CheckModifyData(m_arrModifyList, strOut);
		//回复修改前的值
		RevertModifyValue();
		return;
	}

	if(m_nCurrentStatus == 0)
	{
		//空闲状态
		//检查用户有否修改定值
		if(m_List.m_arrEdited.GetSize() <= 0)
		{
			//无修改定值, 提示先修改
			AfxMessageBox(StringFromID(IDS_TIP_EDIT_SOFT));
			return;
		}

		m_nOperationNum = GetOperationNum();
		
		//组建要修改值的链表
		m_strOutPut = "";
		//记录新值
		CheckModifyData(m_arrModifyList, m_strOutPut);

		//要求用户再次确认
		/*if (AfxMessageBox(m_strOutPut, MB_OKCANCEL) != IDOK)
		{
			//回复修改前的值
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
			return;
		}*/
		
		//操作员确认
		CDlgCheck dlg(this, 2);
		dlg.m_strModify = m_strOutPut;
		if(dlg.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//普通模式,要求操作员验证
				CDlgValidateID dlgID(2);
				dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
				if(dlgID.DoModal() == IDOK)
				{
					m_sOperUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以操作员身份验证成功:软压板投退", m_sOperUser);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sOperUser, -1, OPER_SUCCESS, m_nOperationNum);
				}
				else
				{
					//无操作权限
					//回复修改前的值
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("操作员身份验证失败:软压板投退");
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, "", -1, OPER_FAILD, m_nOperationNum);
					return;
				}
			}
			
		}
		else
		{
			//不同意修改
			//回复修改前的值
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
			return;
		}

		//监护员确认
		CDlgCheck dlg1(this, 1);
		dlg1.m_strModify = m_strOutPut;
		if(dlg1.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//普通模式,要求监护员验证
				CDlgValidateID dlgID(1);
				dlgID.m_strComUserID = m_sOperUser;
				dlgID.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
				if(dlgID.DoModal() == IDOK)
				{
					m_sMonUser = dlgID.m_strUser;
					
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("用户%s以监护员身份验证成功:软压板投退", m_sMonUser);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, m_sMonUser, -1, OPER_SUCCESS, m_nOperationNum);
				}
				else
				{
					//无操作权限
					//回复修改前的值
					RevertModifyValue();
					CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
					CString str;
					str.Format("监护员身份验证失败:软压板投退", m_sMonUser);
					pApp->AddNewManOperator("用户验证", m_pObj->m_sID, str, "", -1, OPER_FAILD, m_nOperationNum);
					return;
				}
			}
			
		}
		else
		{
			//不同意修改
			//回复修改前的值
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
			return;
		}

		//组建报文
		STTP_FULL_DATA sttpData;
		pApp->m_SttpEngine.BuildDataFor20117SoftBoardModify(sttpData, m_pObj->m_sID, atoi(m_sCPU), &m_arrModifyList);

		//发送报文
		int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
		//判断是否成功
		if(nReturn != 1)
		{
			//失败
			if(nReturn == -1)
			{
				CString str;
				str.Format("保护[%s]发送软压板投退报文失败,原因为连接中断", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			else if(nReturn == -2)
			{
				CString str;
				str.Format("保护[%s]发送软压板投退报文失败,原因为缓存已满", m_pObj->m_sName);
				WriteLog(str, XJ_LOG_LV2);
			}
			AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));
			//回复修改前的值
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
		}
		else
		{
			//成功,状态改为正在软压板投退
			m_nCurrentStatus = 2;

			m_bChecking = TRUE;

			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nChangeSBTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);

			//增加人工操作
			//pApp->AddManOperator(FUNC_OPT_SETSOFTBOARD, m_pObj->m_sID);
			
			//改变控件可用性
			UpdateControlsEnable();

			CString str;
			str.Format("保护[%s]发送软压板投退报文成功", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
	}
	else if(m_nCurrentStatus == 2)
	{
		//软压板投退状态
		CancelCall();
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
	}
	else 
	{
		CString strOut;
		//收集被修改的值
		CheckModifyData(m_arrModifyList, strOut);
		//回复修改前的值
		RevertModifyValue();
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		return;
	}
}

void CPTSoftBoard::OnBtnPtsoftModify2() 
{
	// TODO: Add your control notification handler code here
	//响应按钮按下操作, 召唤或取消召唤
	m_bChecking = FALSE;
	CString str;

	if(m_pObj == NULL)
		return;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTSoftDataTable* pPTSoftData = pTagOutStore->GetPTSoftData();
	CXJUserStore *pUserStore = CXJUserStore::GetInstance();

	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		CString strOut;
		//收集被修改的值
		CheckModifyData(m_arrModifyList, strOut);
		//回复修改前的值
		RevertModifyValue();
		return;
	}

	if(m_nCurrentStatus == 0)
	{
		//空闲状态
		//检查用户有否修改定值
		if(m_List.m_arrEdited.GetSize() <= 0)
		{
			//无修改定值, 提示先修改
			AfxMessageBox(StringFromID(IDS_TIP_EDIT_SOFT));
			//return;
		}

		m_nOperationNum = GetOperationNum();
		
		//组建要修改值的链表
		m_strOutPut = "";
		//记录新值
		CheckModifyData(m_arrModifyList, m_strOutPut);
//		AfxMessageBox(m_strOutPut);

		// 载入新值
// 		str.Format("cpu: %s", m_sCPU);
// 		AfxMessageBox(str);
		pPTSoftData->ReLoad(QByteArray(m_pObj->m_sID.GetBuffer(0))
			, atoi(m_sCPU.GetBuffer(0)), m_arrModifyList, m_arrSoftBoard);
		
		//操作员确认
		CDlgDataCheck dlgCheck(this, XJ_USERGROUP_OPERATOR, XJ_TAGOUT_PTSOFTSET);
		dlgCheck.m_strModify = m_strOutPut;
		dlgCheck.m_sCPU = m_sCPU;
		if(dlgCheck.DoModal() == IDOK)
		{
			if(g_PTControlModel == 1)
			{
				//普通模式,要求操作员验证
				CDlgValidateUser dlgUser(XJ_USERGROUP_OPERATOR);
				//dlgUser.m_strFuncID = FUNC_XJBROWSER_CONTROL;
				dlgUser.m_nFuncID = XJ_OPER_PTSOFTSET_STATE_2;
				QByteArray sUserID = pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_2);
				QByteArray sLogUserID = pTagOutState->GetLogUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_2);
				dlgUser.m_strUser = sUserID.constData();
				if (!sUserID.isEmpty())
					dlgUser.m_strAuthUserID = sUserID.constData();
				else{
					dlgUser.m_strUser = sLogUserID.constData();
				}
				dlgUser.m_excludeUserList << pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_HANGOUT);
				if (pUserStore->hasFuncID(XJ_OPER_PTSOFTSET_STATE_2, pApp->m_User.m_strUSER_ID.GetBuffer(0)))
					dlgUser.m_strUser = pApp->m_User.m_strUSER_ID;
				if(dlgUser.DoModal() == IDOK)
				{
					if (dlgUser.m_strAuthUserID.IsEmpty()){
						pTagOutStore->ReLoadState();
						int nPTSetState = pTagOutState->GetStateID();
						
						CString str;
						if (XJ_OPER_HANGOUT != nPTSetState){
							CString sCurUserID = pTagOutState->GetOperUserID().constData();
							str.Format("用户[%s]正在对该装置进行软压板投退操作 或者 该装置已取消了挂牌，请稍后再试"
								, sCurUserID);
							AfxMessageBox(str, MB_OK | MB_ICONWARNING);
							
							RevertModifyValue();
							return;
						}
					}
					m_sOperUser = dlgUser.m_strUser;
					
					pTagOutState->Next_PTSET_SOFT_STATE_2(atoi(m_sCPU)
						, m_sOperUser.GetBuffer(0), m_arrModifyList, m_arrSoftBoard);
				}
				else
				{
					//无操作权限
					//回复修改前的值
					RevertModifyValue();

					str.Format("用户[%s]以操作员身份验证失败：密码验证有误", m_sOperUser);
					WriteLog(str, XJ_LOG_LV2);
					pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_2, m_sOperUser.GetBuffer(0)
						, QByteArray(str.GetBuffer(0)));

					return;
				}
			}
			
		}
		else
		{
			//不同意修改
			//回复修改前的值
			RevertModifyValue();
			m_bMonVerify = false;
			m_bOperVerify = false;
			UpdateControlsEnable();
			return;
		}

		//pPTSoftData->UnitTest_01();

		if (pTagOutState->IsWorkFlowEnableOnState(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_3))
		{
			//监护员确认
			CDlgDataCheck dlg1(this, XJ_USERGROUP_MONITOR, XJ_TAGOUT_PTSOFTSET);
			dlg1.m_strModify = m_strOutPut;
			dlg1.m_sCPU = m_sCPU;
			if(dlg1.DoModal() == IDOK)
			{
				if(g_PTControlModel == 1)
				{
					//普通模式,要求监护员验证
					CDlgValidateUser dlgUser(XJ_USERGROUP_MONITOR);
					//dlgUser.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
					dlgUser.m_nFuncID = XJ_OPER_PTSOFTSET_STATE_3;
					QByteArray sUserID = pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_3);
					dlgUser.m_strUser = sUserID.constData();
					if (!sUserID.isEmpty())
						dlgUser.m_strAuthUserID = sUserID.constData();
					dlgUser.m_excludeUserList << pTagOutState->GetLogUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_2); // 过滤掉本次操作员用户
					if (pTagOutState->IsWorkFlowEnableOnState(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_4)){ // 若启用运行员验证，需将本次挂牌用户过滤掉
						dlgUser.m_excludeUserList.AppendField(pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_HANGOUT));
					}

					if(dlgUser.DoModal() == IDOK)
					{
						m_sMonUser = dlgUser.m_strUser;
						
						pTagOutState->Next_PTSET_SOFT_STATE_3(m_sMonUser.GetBuffer(0));
					}
					else
					{
						m_sMonUser = dlgUser.m_strUser;
						
						//无操作权限
						//回复修改前的值
						RevertModifyValue();
						
						str.Format("用户[%s]以监护员身份验证失败：密码验证有误", m_sMonUser);
						WriteLog(str, XJ_LOG_LV2);
						pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_3, m_sMonUser.GetBuffer(0)
							, QByteArray(str.GetBuffer(0)));
						
						return;
					}
				}
				
			}
			else
			{
				//不同意修改
				//回复修改前的值
				RevertModifyValue();
				m_bMonVerify = false;
				m_bOperVerify = false;
				UpdateControlsEnable();
				
				str.Format("用户[%s]以监护员身份验证失败：不同意修改", m_sMonUser);
				WriteLog(str, XJ_LOG_LV2);
				pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_3, m_sMonUser.GetBuffer(0)
					, QByteArray(str.GetBuffer(0)));
				
				return;
			}
		}

		// ExcutePTSet_Soft();
	}
	else if(m_nCurrentStatus == 2)
	{
		//软压板投退状态
		CancelCall();
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
	}
	else 
	{
		CString strOut;
		//收集被修改的值
		CheckModifyData(m_arrModifyList, strOut);
		//回复修改前的值
		RevertModifyValue();
		m_bMonVerify = false;
		m_bOperVerify = false;
		UpdateControlsEnable();
		return;
	}
}

void CPTSoftBoard::ExcutePTSet_Soft()
{
	if (0 != m_nCurrentStatus)
		return;

	if (NULL == m_pObj){
		m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
		RevertModifyValue();
		return;
	}

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	
	CString str;

	//组建报文
	STTP_FULL_DATA sttpData;
	pApp->m_SttpEngine.BuildDataFor20117SoftBoardModify(sttpData, m_pObj->m_sID, atoi(m_sCPU), &m_arrModifyList);
	
	//发送报文
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//判断是否成功
	if(nReturn != 1)
	{
		//失败
		if(nReturn == -1)
		{
			str.Format("保护[%s]发送软压板投退执行报文失败,原因为连接中断", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
		else if(nReturn == -2)
		{
			str.Format("保护[%s]发送软压板投退执行报文失败,原因为缓存已满", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}else{
			str.Format("保护发送软压板投退执行报文失败: [%s]", m_pObj->m_sName);
			WriteLog(str, XJ_LOG_LV2);
		}
		KillTimer(m_nPTSetTimer);
		AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));

		//回复修改前的值
		RevertModifyValue();
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		UpdateControlsEnable();

		QByteArray &sUserID = pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_2);
		pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_5, sUserID.constData()
			, QByteArray(str.GetBuffer(0)));
		
		m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
	}
	else
	{
		//成功,状态改为正在软压板投退
		m_nCurrentStatus = 2;
		
		m_bChecking = TRUE;
		
		//启动定时器
		CDataEngine* pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nChangeSBTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		
		//增加人工操作
		//pApp->AddManOperator(FUNC_OPT_SETSOFTBOARD, m_pObj->m_sID);
		
		//改变控件可用性
		UpdateControlsEnable();
		
		CString str;
		str.Format("保护[%s]发送软压板投退报文成功", m_pObj->m_sName);
		WriteLog(str, XJ_LOG_LV2);
	}
}

/*************************************************************
 函 数 OnBtnViewProg()
 功能概要：软压板投退进度查看
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8E0271
void CPTSoftBoard::OnBtnViewProg() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	CCJTabCtrlBar &bar = pMainFrame->m_wndGlobalMsgBar;
	
	CRect rcRect;
	GetWindowRect(&rcRect);
	
	if (m_bAlreadyShowOnce){
		bar.GetWindowRect(rcRect);
		m_pointViewProg.x = rcRect.left - 2;
		m_pointViewProg.y = rcRect.top - 18;

	}else{
		LONG w = 600;
		LONG h = 220;

		m_pointViewProg.x = rcRect.right - w;
		m_pointViewProg.y = rcRect.bottom - h;
		m_bAlreadyShowOnce = true;
	}
	
	bar.ShowWindow(bar.IsVisible() ? SW_HIDE : SW_SHOW);
	pMainFrame->FloatControlBar(&bar, m_pointViewProg, CBRS_ALIGN_LEFT);

	
	CString str;
	str.Format("x: %d; y: %d", m_pointViewProg.x, m_pointViewProg.y);
	//AfxMessageBox(str);

}

/*************************************************************
 函 数 OnBtnViewHis()
 功能概要：软压板投退历史查看
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8E0271
void CPTSoftBoard::OnBtnViewHis() 
{
	// TODO: Add your control notification handler code here
	CDlgOperHis dlg;
	dlg.m_nType = 2;
	dlg.m_pObj = m_pObj;
	dlg.DoModal();
}

//##ModelId=49B87B8A01D4
void CPTSoftBoard::CheckModifyData( MODIFY_LIST& modifyList, CString& strOut )
{
	//投退软压板列表
	ClearModifyList();

	CPoint ptItem;
	CString strText, strPickList, strOutput = StringFromID(IDS_MODIFYSOFT_CHECK)+":\r\n";
	PT_SOFTBOARD *pts;
	for (int i = 0; i < m_List.m_arrEdited.GetSize(); i ++)
	{
		ptItem = m_List.m_arrEdited.GetAt(i);
		strText = m_List.GetItemText(ptItem.x, ptItem.y);
		
		//去掉"*"
		strText = strText.Left(strText.Find("*", 0));
		TRACE0(strText + "|\n");
		
		//取得该软压板的定义
		pts = (PT_SOFTBOARD *)m_arrSoftBoard.GetAt(ptItem.x);
		
		STTP_DATA * sttpData = new STTP_DATA;
		//写入软压板ID
		sttpData->id = atoi(pts->ID);
		sttpData->nCpu = atoi(m_sCPU);

		strOutput += pts->ID + "\t";
		strOutput += pts->Name + StringFromID(IDS_COMMON_FROM)+" -> ";
		strOutput += pts->Value + "\t"+StringFromID(IDS_COMMON_CHANGETO)+"-> ";
		strOutput += strText + "\r\n";
		
		//软压板的值
		//将可能的不同表现形式转换成0:1
		if(!strText.CompareNoCase("OFF") || !strText.CompareNoCase(StringFromID(IDS_CASE_CEASING)))
		{
			strText = "0";
		}
		if(!strText.CompareNoCase("ON") || !strText.CompareNoCase(StringFromID(IDS_CASE_COMMISSIONING)))
		{
			strText = "1";
		}

		sttpData->str_value = strText;

		//加入到链表
		modifyList.Add(sttpData);
	}
	strOut = strOutput;
}

/*************************************************************
 函 数 名：ClearModifyList()
 功能概要：清空修改链表
 返 回 值: void 
**************************************************************/
//##ModelId=49B87B8A01E5
void CPTSoftBoard::ClearModifyList()
{
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		delete sttpData;
	}
	m_arrModifyList.RemoveAll();
}

/*************************************************************
 函 数 名：OnSTTP20118()
 功能概要：响应软压板投退预校通知(20118)
 返 回 值: void 
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8B003E
void CPTSoftBoard::OnSTTP20118( WPARAM wParam,LPARAM lparam )
{
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	
	//检查是处于软压板投退状态
	if(m_nCurrentStatus != 2)
	{
		//非软压板投退状态
		return;
	}
	
	//取得报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lparam;
	if(pSttpData == NULL)
		return;
	
	//检查是否20118报文
	if(pSttpData->sttp_head.uMsgID != 20118)
	{
		//错误报文
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20118 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20118 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}


	CString strMsg;
	strMsg.Format("%d,%d,%d,,%d", pSttpData->sttp_body.nStatus, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, pSttpData->sttp_body.nCpu, m_arrModifyList.GetSize());
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		CString strTemp = sttpData->str_value.c_str();
		if(g_OperTableType == 0)
			strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
		else if(g_OperTableType == 1)
		{
			CString sOldValue = FindNewValue(sttpData->id);
			strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
		}
	}
	pApp->AddNewManOperator("软压板投退预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, pSttpData->sttp_body.nStatus, m_nOperationNum);
	WriteLog("CPTSoftBoard::OnSTTP20118 " + m_pObj->m_sName + " " + m_pObj->m_sID, XJ_LOG_LV3);

	//关闭预校定时器
	KillTimer(m_nTimer);
	
	CString strOutput = m_strOutPut;
	
	CString str;
	//请求确认
	if(pSttpData->sttp_body.nStatus == 0)
	{
		//成功
		strOutput += StringFromID(IDS_EXECUTE_MODIFYSOFT_CONFIRM);
		//提示用户操作结果
		//监护人确认
// 		CDlgCheck dlg(this, 3);
// 		dlg.m_strModify = strOutput;
// 		if (dlg.DoModal() == IDOK)
		{
			
			//发送执行报文
			//组建报文
			STTP_FULL_DATA sttpData;
			pApp->m_SttpEngine.BuildDataFor20113SoftBoardModifyExe(sttpData, m_pObj->m_sID, atoi(m_sCPU), &m_arrModifyList);
			
			//发送报文
			int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			//判断是否成功
			if(nReturn != 1)
			{
				//失败
				if(nReturn == -1)
				{
					str.Format("保护[%s]发送软压板投退执行报文失败,原因为连接中断", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				else if(nReturn == -2)
				{
					str.Format("保护[%s]发送软压板投退执行报文失败,原因为缓存已满", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}else{
					str.Format("保护发送软压板投退执行报文失败: [%]", m_pObj->m_sName);
					WriteLog(str, XJ_LOG_LV2);
				}
				KillTimer(m_nPTSetTimer);
				AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_FAIL));

				//改变状态
				m_nCurrentStatus = 0;
				//回复修改前的值
				RevertModifyValue();
				UpdateControlsEnable();

				pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_5, m_sOperUser.GetBuffer(0)
					, QByteArray(str.GetBuffer(0)));
				
				// 重启定时
				m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);

				return;
			}
			//启动定时器
			CDataEngine* pData = pApp->GetDataEngine();
			int nTimeOut = pData->m_nChangeSBTimeOut;
			m_nTimer = SetTimer(1, nTimeOut*1000, 0);
		}
// 		else
// 		{
// 			//放弃执行
// 			//修改状态为空闲
// 			m_nCurrentStatus = 0;
// 			m_bMonVerify = false;
// 			m_bOperVerify = false;
// 			//回复修改前的值
// 			RevertModifyValue();
// 		}
	}
	else
	{
		KillTimer(m_nPTSetTimer);
		//失败
		CString str;
		str.Format("%s", "软压板投退预校失败");
		WriteLog(str);
		AfxMessageBox(str);

		strOutput += StringFromID(IDS_VERIFY_MODIFYSOFT_FAIL);
		m_bMonVerify = false;
		m_bOperVerify = false;
		m_sOperUser = _T("");
		m_sMonUser = _T("");
		//修改状态为空闲
		m_nCurrentStatus = 0;
		//回复修改前的值
		RevertModifyValue();
		//提示用户操作结果
		//AfxMessageBox(strOutput, MB_OK);

		pTagOutState->RevertTo_PTSet_State_1(XJ_OPER_PTSOFTSET_STATE_5, m_sOperUser.GetBuffer(0)
			, QByteArray(str.GetBuffer(0)));
		
		// 重启定时
		m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
	}
	UpdateControlsEnable();

	m_bChecking = FALSE;
}

/*************************************************************
 函 数 名：OnSTTP20114()
 功能概要：响应软压板投退执行报文(20114)
 返 回 值: void
 参    数：param1	消息参数
		   Param2	报文数据
**************************************************************/
//##ModelId=49B87B8B004E
void CPTSoftBoard::OnSTTP20114( WPARAM wParam,LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//检查是处于软压板投退状态
	if(m_nCurrentStatus != 2)
	{
		//非软压板状态
		return;
	}
	
	//取得报文数据
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
		return;
	
	//检查是否20114报文
	if(pSttpData->sttp_head.uMsgID != 20114)
	{
		//错误报文
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20114 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != m_pObj->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CPTSoftBoard::OnSTTP20114 收到其它装置报文, 装置ID为%s", strID);
		WriteLog(str, XJ_LOG_LV3);
		return;
	}


	CString strMsg;
	strMsg.Format("%d,%d,%d,,%d", pSttpData->sttp_body.nStatus, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, pSttpData->sttp_body.nCpu, m_arrModifyList.GetSize());
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		CString strTemp = sttpData->str_value.c_str();
		if(g_OperTableType == 0)
			strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
		else if(g_OperTableType == 1)
		{
			CString sOldValue = FindNewValue(sttpData->id);
			strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
		}
	}
	pApp->AddNewManOperator("软压板投退执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, pSttpData->sttp_body.nStatus, m_nOperationNum);
	
	WriteLog("CPTSoftBoard::OnSTTP20114 " + m_pObj->m_sName + " " + m_pObj->m_sID, XJ_LOG_LV3);
	//关闭定时器
	KillTimer(m_nTimer);
	

	//修改状态
	m_nCurrentStatus = 0;
	//处理报文
	if(pSttpData->sttp_body.nStatus == 0)
	{
		//成功
		//清除修改标记
		m_List.ClearEdited();
		AfxMessageBox(StringFromID(IDS_EXECUTE_MODIFYSOFT_SUCCESS));
	}
	else
	{
		//失败
		//回复修改前的值
		
		RevertModifyValue();
		AfxMessageBox(StringFromID(IDS_EXECUTE_MODIFYSOFT_FAIL));
	}

	m_bMonVerify = false;
	m_bOperVerify = false;
	m_sOperUser = _T("");
	m_sMonUser = _T("");
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：OnPTFrameOpen()
 功能概要：响应保护属性页面打开消息, 刷新页面内容
 返 回 值: void
 参    数：param1	消息参数
		   Param2	二次设备指针
**************************************************************/
//##ModelId=49B87B8B0060
void CPTSoftBoard::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//更新对应的二次设备
	CSecObj * pObj = (CSecObj*)lParam;
	if(m_pObj == pObj)
		return;
	m_pObj = (CSecObj*)lParam;

	int nIndex = (int)wParam;

	RefreshControls();
	//页面还原
	m_nCurrentStatus = 0;
	UpdateControlsEnable();
	SetDlgItemText(IDC_STATIC_TIME_PTSB, "");
	//是否使用控制功能
	if(g_PTControl == 1)
	{
		if(g_PTControlModel)//广东模式
		{
			m_btnVerify1.ShowWindow(SW_HIDE);
			m_btnVerify2.ShowWindow(SW_HIDE);
		}
		if(m_pObj->m_nCanControl == 0)
			m_btnModify.ShowWindow(SW_SHOW);
	}

	m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
	m_btnModify.ShowWindow(SW_HIDE);

// 	CRect rcRect;
// 	((CWnd*)GetDlgItem(IDC_STATIC_PTSB_P3))->GetWindowRect(rcRect);
// 	ScreenToClient(&rcRect);
// 	GetDlgItem(IDC_BTN_PTSOFTSET_VIEWPROG)->SetWindowPos(NULL
// 		, rcRect.left + 3, rcRect.top, 0, 0
// 		,SWP_NOZORDER | SWP_NOSIZE );

	ReFillAll();
}

/*************************************************************
 函 数 名：RefreshControls()
 功能概要：刷新控件值, 在第一次启动和更新保护时调用
 返 回 值: void
**************************************************************/
//##ModelId=49B87B8A01F4
void CPTSoftBoard::RefreshControls()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSOFT_CPU);
	if(pCmb == NULL)
		return;
	//清空内容
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
 功能概要：保存当前选择CPU
 返 回 值: void
 参    数：param1	CPU下拉框
**************************************************************/
//##ModelId=49B87B8A01F5
BOOL CPTSoftBoard::SaveSelCPU( CComboBox * pCmb )
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

//##ModelId=49B87B8A0399
void CPTSoftBoard::OnSelchangeCmbPtsoftCpu() 
{
	// TODO: Add your control notification handler code here
	//保存当前选择的CPU
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSOFT_CPU);
	if(pCmb == NULL)
		return;

	if(SaveSelCPU(pCmb))
		ReFillAll();
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：设置LIST差异文本颜色
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A039B
void CPTSoftBoard::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

				//最新值列背景色,表示可修改
				if(nSubItem == m_nNewValueIndex)
				{
					//用蓝色背景
					lplvcd->clrTextBk = RGB(200,255,255);
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

/*************************************************************
 函 数 名：OnClickListPtsoft()
 功能概要：响应点击事件, 修改最新值列软压板值
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A03AB
void CPTSoftBoard::OnClickListPtsoft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	return;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;
	//是否可使用控制功能
	if(g_PTControl == 1)
	{
		if(nItem != -1)
		{
			if(nSubItem == m_nNewValueIndex)
			{
				//最新值列
				CString str;
				str.Format("%s;%s", DisplayValue(0), DisplayValue(1));
				m_List.ListSubItem(nItem, nSubItem, str);
			}
		}
	}
	*pResult = 0;
}

//##ModelId=49B87B8A03BA
void CPTSoftBoard::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器 
		KillTimer(m_nTimer);
		//提示
		CString str = "";
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CString strMsg;
		str = StringFromID(IDS_CALLFAIL_TIMEOUT);
		switch(m_nCurrentStatus)
		{
		case 1:
			//str = "召唤软压板超时!";
			
			strMsg.Format("%d,%d,%s", OPER_FAILD, XJ_OPER_CALL_SOFTBOARD, m_sCPU);
			pApp->AddNewManOperator(FUNC_QUY_CALLSOFT, m_pObj->m_sID, strMsg, XJ_OPER_CALL_SOFTBOARD, OPER_FAILD);
			break;
		case 2:
			//str = "软压板投退超时!";
			if(m_bChecking)
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, m_sCPU, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				pApp->AddNewManOperator("软压板投退预校", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION, OPER_FAILD, m_nOperationNum);
			}
			else
			{
				CString strMsg;
				strMsg.Format("%d,%d,%s,,%d", OPER_FAILD, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, m_sCPU, m_arrModifyList.GetSize());
				for(int i = 0; i < m_arrModifyList.GetSize(); i++)
				{
					STTP_DATA* sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
					CString strTemp = sttpData->str_value.c_str();
					if(g_OperTableType == 0)
						strMsg.Format(strMsg + ",%d," + strTemp, sttpData->id);
					else if(g_OperTableType == 1)
					{
						CString sOldValue = FindNewValue(sttpData->id);
						strMsg.Format(strMsg + ",%d," + strTemp+","+sOldValue, sttpData->id);
					}
				}
				pApp->AddNewManOperator("软压板投退执行", m_pObj->m_sID, strMsg, m_sOperUser, XJ_OPER_MODIFYSOFTBOARD_EXECUTE, OPER_FAILD, m_nOperationNum);
			}
			break;
		default:
			break;
		}
		if(m_nCurrentStatus == 1)
		{
			//正在召唤软压板
		}
		if(m_nCurrentStatus == 2)
		{
			//正在软压板投退

			//回复修改前的值
			RevertModifyValue();
		}
		//改变状态
		m_nCurrentStatus = 0;
		UpdateControlsEnable();
		AfxMessageBox(str);
	}

	if (nIDEvent == m_nPTSetTimer){
		KillTimer(m_nPTSetTimer);
		
		CString str;
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
		
		CXJUserStore *pUserStore = CXJUserStore::GetInstance();
		CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
		QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
		
		int nPTSetState = pTagOutState->GetStateID();
		int nHangoutType = pTagOutState->GetType();
		
		QByteArray baOperUserID;
		QByteArray baRunUserID;
		
		baOperUserID = pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_2);
		baRunUserID = pTagOutState->GetWorkFlowUserID(XJ_TAGOUT_PTSOFTSET, XJ_OPER_HANGOUT);
		CString sRunnerUserID = baRunUserID.constData();
		CString sOperUserID = baOperUserID.constData();
		
		// 软压板投退按钮是否可用： 
// 		if (XJ_OPER_HANGOUT == nPTSetState 
// 			&& XJ_TAGOUT_PTSOFTSET == nHangoutType
// 			&& (sOperUserID.IsEmpty() || pApp->m_User.m_strUSER_ID == sOperUserID
// 			|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER))){
// 			m_btnModify.EnableWindow(TRUE);
// 		}else{
// 			m_btnModify.EnableWindow(FALSE);
// 		}
		if (XJ_OPER_HANGOUT == nPTSetState 
			&& XJ_TAGOUT_PTSOFTSET == nHangoutType
			&& qstrcmp(baRunUserID.constData(), pApp->m_User.m_strUSER_ID.GetBuffer(0)) != 0){
			m_btnModify.EnableWindow(TRUE);
		}else{
			m_btnModify.EnableWindow(FALSE);
		}
		
		// 软压板投退按钮是否可见
// 		if (XJ_OPER_UNHANGOUT != nPTSetState 
// 			&& CString(pTagOutState->GetPTID().constData()) == m_pObj->m_sID
// 			&& XJ_TAGOUT_PTSOFTSET == nHangoutType
// 			&& (pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_OPERATOR)
// 			|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER)))
// 			m_btnModify.ShowWindow(SW_SHOW);
// 		else
// 			m_btnModify.ShowWindow(SW_HIDE);
		if (XJ_OPER_UNHANGOUT != nPTSetState 
			&& CString(pTagOutState->GetPTID().constData()) == m_pObj->m_sID
			&& XJ_TAGOUT_PTSOFTSET == nHangoutType
			&& pUserStore->hasFuncID(XJ_OPER_PTSOFTSET_STATE_2, pApp->m_User.m_strUSER_ID.GetBuffer(0)))
			m_btnModify.ShowWindow(SW_SHOW);
		else
			m_btnModify.ShowWindow(SW_HIDE);
		
		// 查看挂牌进度按钮是否可见
		if (XJ_OPER_UNHANGOUT != nPTSetState && CString(pTagOutState->GetPTID().constData()) == m_pObj->m_sID
			&& XJ_TAGOUT_PTSOFTSET == nHangoutType){
			m_btnViewProg.ShowWindow(SW_SHOW);
			if (XJ_TAGOUT_PTSOFTSET != nHangoutType)
				m_btnViewProg.SetWindowText("查看修改进度");
		}
		else
			m_btnViewProg.ShowWindow(SW_HIDE);
		
		QByteArray &thisComputer = CXJUtilsStore::GetInstance()->GetComputerName();
		if (5 == m_pObj->m_nRunStatu/* && pApp->m_User.m_strUSER_ID == sOperUserID*/){

			QByteArray msg;
			if (XJ_OPER_PTSOFTSET_STATE_4 == nPTSetState)
				msg << "挂牌员已验证软压板修改内容，修改内容将下发到子站，单击<确定>将执行修改";
			else if (!pTagOutState->IsWorkFlowEnableOnState(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_4)
				&& XJ_OPER_PTSOFTSET_STATE_3 == nPTSetState)
				msg << "监护员已验证软压板修改内容，修改内容将下发到子站，单击<确定>将执行修改";
// 			if (XJ_OPER_PTSOFTSET_STATE_3 == nPTSetState && 0 == m_nCurrentDetailStatus
// 				&& pTagOutState->GetLog(XJ_OPER_PTSOFTSET_STATE_2).GetFieldValue(1, 4) == thisComputer){
			if (((XJ_OPER_PTSOFTSET_STATE_4 == nPTSetState) 
				|| (!pTagOutState->IsWorkFlowEnableOnState(XJ_TAGOUT_PTSOFTSET, XJ_OPER_PTSOFTSET_STATE_4)
				&& XJ_OPER_PTSOFTSET_STATE_3 == nPTSetState)) 
				&& 0 == m_nCurrentDetailStatus
				&& pTagOutState->GetLog(XJ_OPER_PTSOFTSET_STATE_2).GetFieldValue(1, 4) == thisComputer)
			{
				AfxMessageBox("监护员已验证软压板修改内容，修改内容将下发到子站，单击<确定>将执行修改"
					, MB_OK|MB_ICONINFORMATION);	
				m_nCurrentDetailStatus = 1;
				
				pTagOutStore->ReLoadState();
				nPTSetState = pTagOutState->GetStateID();
				if (XJ_OPER_UNHANGOUT == nPTSetState){
					AfxMessageBox("运行员已经取消了相应装置的挂牌，此次定值区修改取消！");
					RevertModifyValue();
					//pStore->RevertModify();
					m_nCurrentDetailStatus = 0;
				}else{
					pTagOutState->SetFlags(2);		// 不允许此时取消挂牌操作
					ExcutePTSet_Soft();
				}
			}
		}

		if (pTagOutState->GetFlags() == 1 && CString(pTagOutState->GetPTID().constData()) == m_pObj->m_sID
			//&& pApp->m_User.m_strUSER_ID == sOperUserID
			&& pTagOutState->GetLog(XJ_OPER_PTSOFTSET_STATE_2).GetFieldValue(1, 4) == thisComputer){
			// 只有修改软压板的用户的工作站才执行界面回复操作
			KillTimer(m_nPTSetTimer);

			RevertModifyValue();
			pTagOutState->SetFlags(0);
			pTagOutState->Save();

			// 启用定时器
			m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
		}
		
		// 启用定时器
		m_nPTSetTimer = SetTimer(XJ_TAGOUT_PTSOFTSET, PTSETSOFT_TIMER_LEN*1000, NULL);
	}

	if(nIDEvent == m_nRecordTimer)
	{
		//改变持续时间显示
		m_nOPTime++;
		CString str;
		str.Format("%s %d %s", StringFromID(IDS_COMMON_SPEND),m_nOPTime,StringFromID(IDS_COMMON_SECOND));
		SetDlgItemText(IDC_STATIC_TIME_PTSB, str);
	}
	CViewBase::OnTimer(nIDEvent);
}

/*************************************************************
 函 数 名：OnPTFrameClose()
 功能概要：响应关闭窗口消息, 退出线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8B006F
void CPTSoftBoard::OnPTFrameClose( WPARAM wParam, LPARAM lParam )
{
	EndOwnerThread();
}

/*************************************************************
 函 数 名：ReFillAll()
 功能概要：重新读取和填充所有内容
 返 回 值: 成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B8A0232
BOOL CPTSoftBoard::ReFillAll()
{
	m_nQueryNo = GetNextQueryNo();
	
	GetDlgItem(IDC_STATIC_PTSOFT_LOADING)->ShowWindow(SW_SHOW);

	//清空内容
	EnterCriticalSection(&m_CriticalOper);
	m_HisValueMgr.ResetContent();
	LeaveCriticalSection(&m_CriticalOper);
	ClearSoftBoardConfig();
	ClearDataCol();
	m_List.DeleteAllItems();
	
	//读取数据
	if(MODE_SUB == g_role)
	{
		//直接读取
		LoadPTSoftBoardConfig(0);
		LoadPTHistorySoftBoardData(0);
		AddValueCol(0);
		FillListData(0);
		GetDlgItem(IDC_STATIC_PTSOFT_LOADING)->ShowWindow(SW_HIDE);
	}
	else
	{
		//启动线程
		StartThread();
	}

	return TRUE;
}

/*************************************************************
 函 数 名：ClearDataCol()
 功能概要：清除新加的数据列
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A0203
void CPTSoftBoard::ClearDataCol()
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
//##ModelId=49B87B8A0204
void CPTSoftBoard::CancelCall()
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
		//正在召唤软压板

		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20108, m_pObj->m_sID, atoi(m_sCPU));
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在软压板投退

		//回复修改前的值
		RevertModifyValue();
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20117, m_pObj->m_sID, atoi(m_sCPU));
		//组建取消报文
		pApp->m_SttpEngine.BuildDataFor20137Cancel(sttpData, 20113, m_pObj->m_sID, atoi(m_sCPU));
	}
	//发送报文
	pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//改变状态
	m_nCurrentStatus = 0;
	//更改控件可用性
	UpdateControlsEnable();
}

/*************************************************************
 函 数 名：SetSoftBoardValue()
 功能概要：设置指定的软压板值
 返 回 值: void
 参    数：param1	软压板ID
		   Param2	软压板值
		   Param3	列索引
**************************************************************/
//##ModelId=49B87B8A0213
void CPTSoftBoard::SetSoftBoardValue( CString ID, CString VALUE, int iCol )
{
	//先找到DI_ID==ID的索引
	CString strID = ID;
	strID.TrimLeft();
	strID.TrimRight();
	LVFINDINFO  fi;
	fi.flags = LVFI_STRING;
	fi.psz = strID;
	
	//查找此值在第几行
	int nItem = m_List.FindItem(&fi);
	if(nItem<0)
		return;
	
	//设置值
	PT_SOFTBOARD *pst = m_arrSoftBoard.GetAt(nItem);
	
	//设置ListCtrl的Value列内容
	m_List.SetItemText(nItem, m_nNewValueIndex, DisplayValue(atoi(pst->Value)));
}

/*************************************************************
 函 数 名：RevertModifyValue()
 功能概要：回复修改软压板值时修改的值
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A0222
void CPTSoftBoard::RevertModifyValue()
{
	if(m_arrModifyList.GetSize() < 0)
		return;
	for(int i = 0; i < m_arrModifyList.GetSize(); i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)m_arrModifyList.GetAt(i);
		CString strID;
		strID.Format("%d", sttpData->id);
		SetSoftBoardValue(strID, "", 0);
	}
	//清除修改标记
	m_List.ClearEdited();
	m_nCurrentDetailStatus = 0;
}

/*************************************************************
 函 数 名：EndOwnerThread()
 功能概要：退出自己所起的所有线程
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A0233
void CPTSoftBoard::EndOwnerThread()
{
	WriteLog("开始退出保护软压板页面读取数据线程", XJ_LOG_LV3);
	m_bExitThread = TRUE;
	while(GetThreadCount() > 0)
	{
		THREADNODE* pNode = GetFirstNode();
		if(pNode != NULL && pNode->hThread != NULL)
		{
			WaitForSingleObject(pNode->hThread, g_PTThreadExitTime);
		}
	}
	WriteLog("退出保护软压板页面读取数据线程完毕", XJ_LOG_LV3);
}

/*************************************************************
 函 数 名：IsVaild20069()
 功能概要：判断收到的20069是否自己的
 返 回 值: 是自己的失败报文返回TRUE, 否则返回FALSE
 参    数：param1	失败的操作报文ID
		   Param2
**************************************************************/
//##ModelId=49B87B8A0223
BOOL CPTSoftBoard::IsVaild20069( int nMsgID )
{
	if(nMsgID == 20108 && m_nCurrentStatus == 1)
	{
		//召唤软压板
		return TRUE;
	}
	if(nMsgID == 20113 && m_nCurrentStatus == 2)
	{
		//软压板投退
		return TRUE;
	}
	if(nMsgID == 20117 && m_nCurrentStatus == 2)
	{
		//软压板投退
		return TRUE;
	}
	return FALSE;
}

//##ModelId=49B87B8A03C8
BOOL CPTSoftBoard::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect bgRect;
	GetWindowRect(&bgRect);
	CRgn bgRgn;
    bgRgn.CreateRectRgnIndirect(bgRect);
	AddNoEraseControl(this, bgRgn, IDC_LIST_PTSOFT);
	EraseBK(pDC, bgRect, bgRgn, colBG);
	return TRUE;
//	return CViewBase::OnEraseBkgnd(pDC);
}

/*************************************************************
 函 数 名：DisplayValue()
 功能概要：得到软压板的值显示字符串
 返 回 值: 显示值
 参    数：param1	软压板值
		   Param2
**************************************************************/
//##ModelId=49B87B8A0225
CString CPTSoftBoard::DisplayValue( int nValue )
{
	//软压板值显示风格: 0: 1/0, 1: ON/OFF, 3: 投/退.
	if(0 == g_PTSoftStyle)
	{
		if(1 == nValue)
			return "1";
		else if(0 == nValue)
			return "0";
	}
	if(1 == g_PTSoftStyle)
	{
		if(1 == nValue)
			return "ON";
		else if(0 == nValue)
			return "OFF";
	}
	if(2 == g_PTSoftStyle)
	{
		if(1 == nValue)
			return StringFromID(IDS_CASE_COMMISSIONING);
		else if(0 == nValue)
			return StringFromID(IDS_CASE_CEASING);
	}
	CString str;
	str.Format("%d", nValue);
	return str;
}

//##ModelId=49B87B8B007F
void CPTSoftBoard::OnRefreshPTSoft( WPARAM wParam, LPARAM lParam )
{
	ReFillAll();	
}

//##ModelId=49B87B8A03CB
void CPTSoftBoard::OnBtnPtsoftStyle() 
{
	// TODO: Add your control notification handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->DoSysSetting(SYS_SETTING_PTPROP);
}

//##ModelId=49B87B8A029F
void CPTSoftBoard::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
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
		pApp->SetAppTile(str, StringFromID(IDS_SECPROP_SOFT));
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV), StringFromID(IDS_SECPROP_SOFT));

	CViewBase::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 函 数 名：TranslateSingleKey()
 功能概要：解释单个关键字的意义
 返 回 值: 关键字表示的值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87B8A0234
CString CPTSoftBoard::TranslateSingleKey( CString sKey )
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
//##ModelId=49B87B8A0242
BOOL CPTSoftBoard::InitCOM()
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
//##ModelId=49B87B8A0243
void CPTSoftBoard::ReleaseCOM()
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
 函 数 名：DoPrintSoftBoard()
 功能概要：打印软压板
 返 回 值: 
 参    数：param1	打印模板文件
		   Param2
**************************************************************/
//##ModelId=49B87B8A0244
void CPTSoftBoard::DoPrintSoftBoard( CString strPath )
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
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),strPath);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//设置回调函数
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInPTSoftBoard);
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

//##ModelId=49B87B8A03D8
void CPTSoftBoard::OnRoutineprintNew1() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintSoftBoard(SOFTBOARD_NEW1_TP);
}

//##ModelId=49B87B8A03DA
void CPTSoftBoard::OnRoutineprintNew2() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintSoftBoard(SOFTBOARD_NEW2_TP);
}

//##ModelId=49B87B8A03DC
void CPTSoftBoard::OnRoutineprintNew3() 
{
	// TODO: Add your command handler code here
	//打印
	DoPrintSoftBoard(SOFTBOARD_NEW3_TP);
}

/*************************************************************
 函 数 名：OnSTTP20157()
 功能概要：响应超时通知报文
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87B8B005D
void CPTSoftBoard::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pObj == NULL)
		return;
	if(m_nCurrentStatus != 1 && m_nCurrentStatus != 2)
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
		//正在召唤软压板				
		int nTimeOut = pData->m_nPTSoftBoardTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
	else if(m_nCurrentStatus == 2)
	{
		//正在投退软压板
		int nTimeOut = pData->m_nChangeSBTimeOut;
		m_nTimer = SetTimer(1, nTimeOut*1000, 0);
	}
}

void CPTSoftBoard::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CViewBase::OnLButtonDblClk(nFlags, point);
}

void CPTSoftBoard::OnDoubleclickedBtnPtsoftCall() 
{
	// TODO: Add your control notification handler code here
	
}

void CPTSoftBoard::OnBtnPtsoftVerify1() 
{
		m_nOperationNum = GetOperationNum();
	// TODO: Add your control notification handler code here
	CDlgValidateID dlgID(2);
	dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
	if(dlgID.DoModal() == IDOK)
	{
		m_sOperUser = dlgID.m_strUser;
		m_bOperVerify = true;
		m_btnVerify1.SetIcon(IDI_TICK);
		m_btnVerify2.EnableWindow(TRUE);
	}
}

void CPTSoftBoard::OnBtnPtsoftVerify2() 
{
		m_nOperationNum = GetOperationNum();
	// TODO: Add your control notification handler code here
	CDlgValidateID dlgID(1);
	dlgID.m_strFuncID = FUNC_XJBROWSER_CUSTODY;
	if(dlgID.DoModal() == IDOK)
	{
		m_sMonUser = dlgID.m_strUser;
		if (!m_sMonUser.CompareNoCase(m_sOperUser))
		{
			AfxMessageBox(StringFromID(IDS_VALIDATE_MULTI_IDENTITY));
			return;
		}
		m_bMonVerify = true;
		m_btnVerify2.SetIcon(IDI_TICK);
		if (m_bOperVerify)
		{
			m_btnModify.EnableWindow(TRUE);
		}
	}
}

/****************************************************
Date:2012/3/13  Author:LYH
函数名:   FindNewValue	
返回值:   CString	
功能概要: 
参数: int nID	
*****************************************************/
CString CPTSoftBoard::FindNewValue( int nID )
{
	for(int i = 0; i < m_arrSoftBoard.GetSize(); i++)
	{
		PT_SOFTBOARD* pts = (PT_SOFTBOARD*)m_arrSoftBoard.GetAt(i);
		if(pts == NULL)
			continue;
		if(atoi(pts->ID) == nID)
			return pts->Value;
	}
	return "";
}

void CPTSoftBoard::OnDblclkListPtsoft(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;
	//是否可使用控制功能
	if(g_PTControl == 1)
	{
		if(nItem != -1)
		{
			if(nSubItem == m_nNewValueIndex)
			{
				//最新值列
				CString str;
				str.Format("%s;%s", DisplayValue(0), DisplayValue(1));
				m_List.ListSubItem(nItem, nSubItem, str);
			}
		}
	}
	*pResult = 0;
}

/****************************************************
Date:2012/12/6  Author:LYH
函数名:   FindSoftBoard	
返回值:   PT_SOFTBOARD*	
功能概要: 
参数: CString sID	
*****************************************************/
PT_SOFTBOARD* CPTSoftBoard::FindSoftBoard( CString sID )
{
	for(int i = 0; i < m_arrSoftBoard.GetSize(); i++)
	{
		PT_SOFTBOARD* ps = (PT_SOFTBOARD*)m_arrSoftBoard.GetAt(i);
		if(ps == NULL)
			continue;
		if(ps->ID == sID)
			return ps;
	}
	return NULL;
}



void CPTSoftBoard::StartThread()
{
	m_bExitThread = FALSE;
	CWinThread* pThread = AfxBeginThread(PTSoftBoardLoad, this, THREAD_PRIORITY_BELOW_NORMAL);
	if(pThread)
		AddThreadNode(m_nQueryNo, pThread->m_hThread);
	CString strLog;
	strLog.Format("保护软压板查询线程启动成功,查询批次:%d", m_nQueryNo);
	WriteLog(strLog);
}

int CPTSoftBoard::GetNextQueryNo()
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

void CPTSoftBoard::RemoveThreadNode( int nNo )
{
	CString str;
	str.Format("开始退出保护软压板查询线程,查询号:%d", nNo);
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
	
	str.Format("保护软压板查询线程成功退出,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
}

void CPTSoftBoard::AddThreadNode( int nNo, HANDLE hThread )
{
	EnterCriticalSection(&m_CriticalSection);  
	if(hThread == NULL)
		return;
	THREADNODE* pNode = new THREADNODE;
	pNode->nNo = nNo;
	pNode->hThread = hThread;
	m_listThread.AddTail(pNode);
	CString str;
	str.Format("保护软压板查询线程成功启动,查询号:%d", nNo);
	WriteLog(str, XJ_LOG_LV3);
	LeaveCriticalSection(&m_CriticalSection);
}

THREADNODE_PTR CPTSoftBoard::GetFirstNode()
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

void CPTSoftBoard::AddValueCol( int nQueryNo )
{
	if(g_PTDIValues <= 0)
		return;
	if(m_arrSoftBoard.GetSize == 0)
		return;
	if(MODE_SUB != g_role && !IsCurrentQueryNo(nQueryNo))
		return;
	//加入值列
	CString strTimeDisplay;
	CString strTime = "";
	WriteLog("CPTSoftBoard::AddValueCol, 开始加入值列", XJ_LOG_LV3);
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
	
	WriteLog("CPTSoftBoard::AddValueCol, 加入值列完毕", XJ_LOG_LV3);
}

void CPTSoftBoard::OnThreadFillData( WPARAM wParam, LPARAM lParam )
{
	if(!IsCurrentQueryNo(wParam))
	{
		//过期的批次
		return;
	}
	AddValueCol(wParam);
	FillListData(wParam);
	GetDlgItem(IDC_STATIC_PTSOFT_LOADING)->ShowWindow(SW_HIDE);
}

int CPTSoftBoard::GetThreadCount()
{
	int nCount = 0;
	EnterCriticalSection(&m_CriticalSection);
	nCount = m_listThread.GetCount();
	LeaveCriticalSection(&m_CriticalSection);
	return nCount;
}

BOOL CPTSoftBoard::LoadDefaultCpu()
{
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_PTSOFT_CPU);
	if(pCmb == NULL)
		return TRUE;
	if( pCmb->GetCount() == 0 )
		return TRUE;
	if(m_pObj == NULL)
		return FALSE;
	
	WriteLog("CPTSoftBoard::LoadDefaultCpu, 查询开始", XJ_LOG_LV3);
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
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_SOFTBOARD_CFG, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLogEx("CPTSoftBoard::LoadDefaultCpu, 查询失败");
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		CString str;
		str.Format("CPTSoftBoard::LoadDefaultCpu,查询到%d条数据", nCount);
		WriteLogEx(str);
		if( nCount >= 1 )
		{
			str = pMemset.GetValue((UINT)0); //ID
			int c = atoi(str);
			for( int k = 0; k < pCmb->GetCount(); k++ )
			{
				CSecCPU * cpu = (CSecCPU *)pCmb->GetItemData(k);
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
		str.Format("CPTSoftBoard::LoadDefaultCpu,查询失败,原因为%s", sError);
		WriteLogEx(str);
		bReturn = FALSE;
	}
	
	WriteLog("CPTSoftBoard::LoadDefaultCpu, 查询结束", XJ_LOG_LV3);
	
	return bReturn;
}

void CPTSoftBoard::OnBtnPtsoftExcel() 
{
	// TODO: Add your control notification handler code here
	if( m_pObj == NULL )
		return;
	CTime t = CTime::GetCurrentTime();
	
	CString strName;
	strName.Format("%s(%s-CPU[%s])-%s.xls", StringFromID(IDS_SECPROP_SOFT),m_pObj->m_sName, m_sCPU, t.Format("%Y%m%d%H%M%S"));
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
