// AutoCallResult.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "AutoCallResult.h"


// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif
/** @brief           升降序作用位*/
int g_iResultViewAsc;

CString g_sRootDirPath;

UINT ExportByStation(LPVOID pParam)
{
	CAutoCallResult* pView = (CAutoCallResult*)pParam;
	if( !pView )
		return -1;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return -1;
	Sleep(10);
	DEVICE_LIST	listStation;
	pData->GetStationList( listStation );

	POSITION pos = listStation.GetHeadPosition();
	int total = listStation.GetCount();
	int count = 0;
	int success = 0;
	int fail = 0;
	while(pos != NULL)
	{
		CStationObj*pStation = (CStationObj*)listStation.GetNext(pos);
		if(pStation == NULL)
			continue;
		CString strDir = g_sRootDirPath + "\\" + pStation->m_sNetID;
		CFileFind ifind;
		if( !ifind.FindFile( strDir ) )
			CreateDirectory( strDir, 0 );
		CString sFilePath = strDir + "\\" + pStation->m_sName + ".xls" ;

		if( pView->WriteResultToExcel( sFilePath, pStation->m_sID ) )
			success++;
		else
		{
			fail++;
			CString strLog;
			strLog.Format("[%s] %s", pStation->m_sID, IDS_AUTOCALLRESULT_EXPORT_FAIL );
			WriteLog( strLog, XJ_LOG_LV1 );
		}
		
		count++;
		if( pView->m_pExportProcess )
			pView->m_pExportProcess->PostMessage( EXPORT_PROCESS, total, count );
		Sleep( 1 );
	}
	if( pView->m_pExportProcess )
		pView->m_pExportProcess->PostMessage( EXPORT_RESULT, success, fail );
	return 0;
}

#define CALLRELUST_LIST_COLUMNS	25
//////////////////////////////////////////////////////////////////////////
//CCallResult
//##ModelId=49B87BB303CA
CCallResult::CCallResult()
{
	/** @brief           定值区号结果*/
	m_nZone = 0;
	/** @brief           定值结果*/
	m_nSetting = 0;
	/** @brief           开关量结果*/
	m_nDigital = 0;
	/** @brief           软压板结果*/
	m_nSoftBoard = 0;
	/** @brief           模拟量结果*/
	m_nAnalog = 0;

	/** @brief           定值区号结果说明*/
	m_sZoneNote = ERROR_NULL;
	/** @brief           定值结果说明*/
	m_sSettingNote = ERROR_NULL;
	/** @brief           开关量结果说明*/
	m_sDigitalNote = ERROR_NULL;
	/** @brief           软压板结果说明*/
	m_sSoftBoardNote = ERROR_NULL;
	/** @brief           模拟量结果说明*/
	m_sAnalogNote = ERROR_NULL;
	
	/** @brief           所属批次*/
	m_nSaveNo = 0;

	/** @brief           二次设备ID*/
	m_sSecID = "";
	/** @brief           二次设备指针*/
	m_pSec = NULL;
	/** @brief           CPU号*/
	m_nCPU = -1;
	/** @brief           完成时间*/
	CTime tm(1972, 1, 1, 1, 1, 1);
	m_tmEnd = tm;
	/** @brief           厂站ID*/
	m_sStationId = "";
	/** @brief           厂站指针*/
	m_pStation = NULL;

	m_sSituation = "";
	m_sDispose = "";
}

//##ModelId=49B87BB40000
CCallResult::~CCallResult()
{
	
}

/*************************************************************
 函 数 名：LoadDataFromDB()
 功能概要：从数据库读入数据信息
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数：param1	数据库对象
		   Param2
**************************************************************/
//##ModelId=49B87BB4006D
BOOL CCallResult::LoadDataFromDB( CMemSet* pMemset )
{
	//检查数据有效性
	if(pMemset == NULL)
		return FALSE;
	try
	{
		m_sSecID = pMemset->GetValue((UINT)0); //保护ID
		CString str;
		str = pMemset->GetValue(1); //CPU
		m_nCPU = atoi(str);
		str = pMemset->GetValue(2); //完成时间
		m_tmEnd = StringToTime(str);
		m_sStationId = pMemset->GetValue(3); //厂站ID
		str = pMemset->GetValue(4);
		m_nZone = atoi(str); //定值区号召唤结果
		str = pMemset->GetValue(5);
		m_nSetting = atoi(str); //定值召唤结果
		str = pMemset->GetValue(6);
		m_nSoftBoard = atoi(str); //软压板召唤结果
		str = pMemset->GetValue(7);
		m_nAnalog = atoi(str); //模拟量召唤结果
		str = pMemset->GetValue(8);
		m_nDigital = atoi(str); //开关量召唤结果
		str = pMemset->GetValue(10);
		m_nHistory = atoi(str);//历史事件召唤结果
		str = pMemset->GetValue(11);
		m_nOsc = atoi(str);//录波召唤结果

		if(m_nHistory == 2)
			m_sHistoryNote = pMemset->GetValue(12);
		if(m_nOsc == 2)
			m_sOscNote = pMemset->GetValue(13);
		if(m_nZone == 2)
			m_sZoneNote = pMemset->GetValue(14); //定值区号结果说明
		if(m_nSetting == 2)
			m_sSettingNote = pMemset->GetValue(15); //定值结果说明
		if(m_nDigital == 2)
			m_sDigitalNote = pMemset->GetValue(16); //开关量结果说明
		if(m_nSoftBoard == 2)
			m_sSoftBoardNote = pMemset->GetValue(17); //软压板结果说明
		if(m_nAnalog == 2)
			m_sAnalogNote = pMemset->GetValue(18); //模拟量结果说明
		str = pMemset->GetValue(19);
		m_nSaveNo = atoi(str); //批次
		m_sSituation = pMemset->GetRealDataValue( "call_notes" ); //情况说明
		m_sDispose = pMemset->GetRealDataValue( "call_suggest" ); //处理意见
	
	}
	catch (...)
	{
		WriteLog("CCallResult::LoadDataFromDB 读取数据出错", 3);
	}
	
	if(!BuildReleation())
	{
		WriteLog("CCallResult::LoadDataFromDB, 与设备关联失败");
		return FALSE;
	}
	return TRUE;
}

/*************************************************************
 函 数 名：BuildReleation()
 功能概要：建立与设备的关联
 返 回 值: 成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4008C
BOOL CCallResult::BuildReleation()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;
	//先查找二次设备指针
	m_pSec = (CSecObj*)pData->FindDevice(m_sSecID, TYPE_SEC);
	if(m_pSec == NULL)
		return FALSE;
	
	//厂站ID
	CStationObj * pStation = (CStationObj*)pData->FindDevice(m_sStationId, TYPE_STATION);
	m_pStation = pStation;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CAutoCallResult

IMPLEMENT_DYNCREATE(CAutoCallResult, CFormView)

//##ModelId=49B87BB40158
CAutoCallResult::CAutoCallResult()
	: CFormView(CAutoCallResult::IDD)
{
	//{{AFX_DATA_INIT(CAutoCallResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arrColum.RemoveAll();
	m_listResult.RemoveAll();
	m_arrSaveNo.RemoveAll();

	g_iResultViewAsc = -1;

	m_nNewSaveNo = 0;
	m_nOldSaveNo = 0;
	m_nMaxSaveNo = 10;
	m_nMinSaveNo = 1;

	m_ZoneStat.nFail = 0;
	m_ZoneStat.nSuccess = 0;

	m_SettingStat.nSuccess =0;
	m_SettingStat.nFail = 0;

	m_DigitalStat.nFail = 0;
	m_DigitalStat.nSuccess = 0;

	m_SoftBoardStat.nSuccess = 0;
	m_SoftBoardStat.nFail = 0;

	m_AnalogStat.nFail = 0;
	m_AnalogStat.nSuccess = 0;

	CTime tm(1971, 1, 1, 1, 1, 1);
	m_tmStart = tm;
	m_tmEnd = tm;

	m_nOldSortCol = -1;

	m_pStation = NULL;

	m_nSelSaveNo = 0;

	m_bShowNoFailed = TRUE;

	m_bSelectRoot = FALSE;
}

//##ModelId=49B87BB403C8
CAutoCallResult::~CAutoCallResult()
{
	Clear();
	ClearSaveNo();
}

//##ModelId=49B87BB403A9
void CAutoCallResult::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoCallResult)
	DDX_Control(pDX, IDC_LIST_AUTOCALL_SUMMARY, m_ListSummary);
	DDX_Control(pDX, IDC_LIST_AUTOCALL_RESULT, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoCallResult, CFormView)
	//{{AFX_MSG_MAP(CAutoCallResult)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_AUTOCALL_RESULT, OnCustomDraw)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_AUTOCALL_RESULT, OnColumnclickListAutocallResult)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_RESULT_EXPORT, OnResultExport)
	ON_COMMAND(IDC_RESULT_PRINT, OnResultPrint)
	ON_COMMAND(IDC_RESULT_PREV, OnResultPrev)
	ON_COMMAND(IDC_RESULT_NEXT, OnResultNext)
	ON_COMMAND(IDC_RESULT_SHOWFAILD, OnResultShowfaild)
	ON_COMMAND(IDC_WIN_CLOSE, OnWinClose)
	ON_COMMAND(IDC_RESULT_STATIONEXPORT, OnResultStationexport)
	//}}AFX_MSG_MAP
	ON_MESSAGE(AUTOCALL_RESULT_OPEN, OnAutoCallResultFrameOpen)
	ON_MESSAGE(AUTOCALL_RESULT_CHANGE, OnAutoCallResultChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoCallResult diagnostics

#ifdef _DEBUG
//##ModelId=49B87BB50000
void CAutoCallResult::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87BB5000F
void CAutoCallResult::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAutoCallResult message handlers

//##ModelId=49B87BB4038A
void CAutoCallResult::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//设置可滚动视图的必要属性
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);

	//初始化列表框样式
	InitListStyle();
	InitSummaryListStyle();
	
}

//##ModelId=49B87BB5002E
void CAutoCallResult::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87BB40232
BOOL CAutoCallResult::LoadListConfig()
{
	//读取UI配置文件
	BOOL bResult = TRUE;
	
// 	CoInitialize(NULL); //初始化COM
// 	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!", 3);
			WriteLog("AutoCallResultConfig::读取UIConfig失败,将使用默认列风格");
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
				WriteLog("AutoCallResultConfig::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//AutoCallResultConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到AutoCallResultConfig");
				WriteLog("AutoCallResultConfig::读取UIConfig失败,将使用默认列风格");
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到AutoCallResultConfig下的配置");
					WriteLog("AutoCallResultConfig::读取UIConfig失败,将使用默认列风格");
					bResult = FALSE;
				}
				else
				{
					//找到子节点列表指针
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					if( pChild->Getlength() == CALLRELUST_LIST_COLUMNS)
					{
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
					else
						bResult = FALSE;
				}
			}
		}		
// 	}	
// 	//关闭打开的COM
// 	CoUninitialize();
	return bResult;
}

//##ModelId=49B87BB40242
BOOL CAutoCallResult::SaveListConfig()
{
	//保存UIConfig失败", 3
	BOOL bResult = TRUE;
	
// 	CoInitialize(NULL); //初始化COM
// 	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//初始化MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!", 3);
			WriteLog("AutoCallResultConfig::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("AutoCallResultConfig::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//AutoCallResultConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到AutoCallResultConfig, 将新建节点和子节点");	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("AutoCallResultConfig");
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
						WriteLog("未找到BatchDetailViewConfig下的配置, 新建所有子节点");
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
						WriteLog("找到了AutoCallResultConfig下的配置, 先删除新建所有子节点");
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
// 	}
// 	//关闭打开的COM
// 	CoUninitialize();
	return bResult;
}

/*************************************************************
 函 数 名：NeedSave()
 功能概要：判断是否列信息有改变需要保存
 返 回 值: 需要保存返回TRUE, 不需要保存返回FALSE
**************************************************************/
//##ModelId=49B87BB40261
BOOL CAutoCallResult::NeedSave()
{
	BOOL bReturn = FALSE;

	//先读取最新列信息
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
 功能概要：调整控件大小,位置
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB40271
void CAutoCallResult::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	if(m_ListSummary.GetSafeHwnd() == NULL)
		return;
	//概述的高度为60, 其它的都为详细
	CRect rc;
	GetClientRect(&rc); //客户区大小
	
	CRect rcSummary = rc;
	rcSummary.bottom = rcSummary.top + 90;
	m_ListSummary.MoveWindow(rcSummary);
	
	CRect rect = rc;
	rect.top  = rcSummary.bottom;
	m_List.MoveWindow(rect);
}

/*************************************************************
 函 数 名：OnAutoCallResultFrameOpen()
 功能概要：响应窗口打开消息, 调整控件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5009E
void CAutoCallResult::OnAutoCallResultFrameOpen( WPARAM wParam, LPARAM lParam )
{
	RegulateControlSize();
	//初始化数据
// 	LoadDataFromDB();
// 	FillListData();
//  	GetOCTime("OC_LSTARTTIME", m_tmStart);
//  	GetOCTime("OC_LENDTIME", m_tmEnd);
// 	FillSummaryListData();
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格
 返 回 值: 失败返回-1, 成功返回>=0
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB40280
int CAutoCallResult::InitListStyle()
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
		
		CString colName="";
		for (int nCol=0; nCol < CALLRELUST_LIST_COLUMNS; nCol++)
		{
			colName="";
			lvCol.iSubItem=nCol;
			lvCol.cx = 100;
			switch(nCol)
			{
			case 0://
				lvCol.cx = 50; //"批次"
				colName.LoadString(IDS_AUTOCALLRESULT_NUMBER);
				break;
			case 1:
				lvCol.cx = 300;
				colName.LoadString(IDS_MODEL_SECONDARY);
				break;
			case 2:
				colName.LoadString(IDS_MODEL_CPU);
				break;
			case 3:
				colName.LoadString(IDS_AUTOCALLRESULT_ZONE);
				break;
			case 4:
				colName.LoadString(IDS_AUTOCALLRESULT_ZONE_NOTE);
				break;
			case 5:
				colName.LoadString(IDS_AUTOCALLRESULT_SETTING);
				break;
			case 6:
				colName.LoadString(IDS_AUTOCALLRESULT_SETTING_NOTE);
				break;
			case 7:
				colName.LoadString(IDS_AUTOCALLRESULT_DIGITAL);
				break;
			case 8:
				colName.LoadString(IDS_AUTOCALLRESULT_DIGITAL_NOTE);
				break;
			case 9:
				colName.LoadString(IDS_AUTOCALLRESULT_SOFT);
				break;
			case 10:
				colName.LoadString(IDS_AUTOCALLRESULT_SOFT_NOTE);
				break;
			case 11:
				colName.LoadString(IDS_AUTOCALLRESULT_ANALOG);
				break;
			case 12:
				colName.LoadString(IDS_AUTOCALLRESULT_ANALOG_NOTE);
				break;
			case 13:
				colName.LoadString(IDS_AUTOCALLRESULT_OSC);
				break;
			case 14:
				colName.LoadString(IDS_AUTOCALLRESULT_OSC_NOTE);
				break;
			case 15:
				colName.LoadString(IDS_AUTOCALLRESULT_HISEVENT);
				break;
			case 16:
				colName.LoadString(IDS_AUTOCALLRESULT_HISEVENT_NOTE);
				break;
			case 17:
				colName.LoadString(IDS_COMMON_FINISHTIME);
				break;
			case 18:
				colName.LoadString(IDS_AUTOCALLRESULT_OVERVIEW);
				break;
			case 19:
				colName.LoadString(IDS_AUTOCALLRESULT_RESOLUTION);
				break;
			case 20:
				colName.LoadString(IDS_MODEL_SUBSTATION);
				break;
			case 21:
				colName.LoadString(IDS_MODEL_PRIMARY);
				break;
			case 22:
				colName.LoadString(IDS_MODEL_MANUFACTURER);
				break;
			case 23:
				colName.LoadString(IDS_MODEL_MANAGEMENT);
				break;
			case 24:
				colName.LoadString(IDS_MODEL_MAINTENANCE);
				break;
			default:
				lvCol.cx=100;//列宽100象素
			}
			lvCol.pszText = colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
 		}
 	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	//	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}

/*************************************************************
 函 数 名：InitSummaryListStyle()
 功能概要：初始化概述列表框的风格
 返 回 值: 失败返回-1, 成功返回>=0
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB40290
int CAutoCallResult::InitSummaryListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,16,TRUE|ILC_COLOR32,1,0);   
	m_ListSummary.SetImageList(&m_l,LVSIL_SMALL);

	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;

	CString colName="";
	for (int nCol=0; nCol < 9 ; nCol++)
	{
		colName="";
		lvCol.iSubItem=nCol;
		lvCol.cx = 100;
		switch(nCol)
		{
		case 0://
			lvCol.cx = 70; //列宽50象素
			colName.LoadString(IDS_COMMON_ITEM);
			break;
		case 1:
			colName.LoadString(IDS_SECPROP_ZONE);
			break;
		case 2:
			colName.LoadString(IDS_SECPROP_SETTING);
			break;
		case 3:
			colName.LoadString(IDS_SECPROP_DIGITAL);
			break;
		case 4:
			colName.LoadString(IDS_SECPROP_SOFT);
			break;
		case 5:
			colName.LoadString(IDS_SECPROP_ANALOG);
			break;
		case 6:
			colName.LoadString(IDS_SECPROP_OSC);
			break;
		case 7:
			colName.LoadString(IDS_SECPROP_HISTORYEVENT);
			break;
		case 8:
			lvCol.cx = 240;
			colName.LoadString(IDS_AUTOCALLRESULT_SUMMARY);
			break;
		default:
			lvCol.cx=100;//列宽100象素
			break;
		}
		lvCol.pszText = colName.GetBuffer(1);
		m_ListSummary.InsertColumn(nCol,&lvCol);
	}

	//设置风格
	m_ListSummary.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);

	return 1;
}

/*************************************************************
 函 数 名：LoadDataFromDB()
 功能概要：从数据库读入数据
 返 回 值: 读取成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4029F
BOOL CAutoCallResult::LoadDataFromDB(int nSaveNo)
{
	//清除旧数据
	Clear();
	//清除统计结果
	ClearStat();
	
	BOOL bReturn = TRUE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;		
	
	//条件
	Condition Condition1;
	str.Format("Is_Auto = 0");
	pApp->m_DBEngine.SetCondition(sql, Condition1, str);

	if (m_pStation != NULL)
	{
		Condition con2;
		str.Format("station_id = '%s'", m_pStation->m_sID);
		pApp->m_DBEngine.SetCondition(sql, con2, str);
	}
	else if( !pApp->MgrAllStations() )
	{
		Condition con2;
		str.Format("(station_id in(select station_id from tb_user_related where user_id='%s'))", pApp->m_User.m_strUSER_ID);
		pApp->m_DBEngine.SetCondition(sql, con2, str);
	}

	//批次号
	Condition Con3;
	str.Format("SaveNo = %d", nSaveNo);
	pApp->m_DBEngine.SetCondition(sql, Con3, str);

	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OC_RESULT_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CAutoCallResult::LoadDataFromDB, 查询失败");
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
			CCallResult* pResult = new CCallResult;
			if(pResult->LoadDataFromDB(pMemset))
			{
				//加入到统计结果
				StatResult(pResult);
				//加入到链表
				m_listResult.AddTail(pResult);
			}
			else
			{
				delete pResult;
				pResult = NULL;
			}
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CAutoCallResult::LoadDataFromDB,查询失败,原因为%s", sError);
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
 功能概要：清除数据
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB402BF
void CAutoCallResult::Clear()
{
	POSITION pos = m_listResult.GetHeadPosition();
	while(pos != NULL)
	{
		CCallResult* pResult = (CCallResult*)m_listResult.GetNext(pos);
		if(pResult == NULL)
			continue;
		delete pResult;
		pResult = NULL;
	}
	m_listResult.RemoveAll();
}

/*************************************************************
 函 数 名：StatResult()
 功能概要：将指定结果加入到结果统计中
 返 回 值: void
 参    数：param1	指定结果
		   Param2
**************************************************************/
//##ModelId=49B87BB402C0
void CAutoCallResult::StatResult( CCallResult* pResult )
{
	//检查数据有效性
	if(pResult == NULL)
		return;

	if(pResult->m_tmEnd < m_tmStart)
		m_tmStart = pResult->m_tmEnd;
	if(pResult->m_tmEnd > m_tmEnd)
		m_tmEnd = pResult->m_tmEnd;
	//定值区号
	if(1 == pResult->m_nZone)
	{
		//成功
		m_ZoneStat.nSuccess++;
	}
	else if(2 == pResult->m_nZone)
	{
		//失败
		m_ZoneStat.nFail++;
	}

	//定值
	if(1 == pResult->m_nSetting)
	{
		//成功
		m_SettingStat.nSuccess++;
	}
	else if(2 == pResult->m_nSetting)
	{
		//失败
		m_SettingStat.nFail++;
	}

	//开关量
	if(1 == pResult->m_nDigital)
	{
		//成功
		m_DigitalStat.nSuccess++;
	}
	else if(2 == pResult->m_nDigital)
	{
		//失败
		m_DigitalStat.nFail++;
	}

	//软压板
	if(1 == pResult->m_nSoftBoard)
	{
		//成功
		m_SoftBoardStat.nSuccess++;
	}
	else if(2 == pResult->m_nSoftBoard)
	{
		//失败
		m_SoftBoardStat.nFail++;
	}

	//模拟量
	if(1 == pResult->m_nAnalog)
	{
		//成功
		m_AnalogStat.nSuccess++;
	}
	else if(2 == pResult->m_nAnalog)
	{
		//失败
		m_AnalogStat.nFail++;
	}
	//录波
	if(1 == pResult->m_nOsc)
		m_OscStat.nSuccess++;
	else if( 2== pResult->m_nOsc)
		m_OscStat.nFail++;
	//历史事件
	if(1 == pResult->m_nHistory)
		m_nHistoryStat.nSuccess++;
	else if( 2 == pResult->m_nHistory)
		m_nHistoryStat.nFail++;
}

/*************************************************************
 函 数 名：FillListData()
 功能概要：将所有召唤结果加入到列表框中显示
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB402DE
void CAutoCallResult::FillListData()
{
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	POSITION pos = m_listResult.GetHeadPosition();
	while(pos != NULL)
	{
		CCallResult* pResult = (CCallResult*)m_listResult.GetNext(pos);
		if(pResult == NULL)
			continue;
		//加入到显示
		AddResultToList(pResult);
	}
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 函 数 名：AddResultToList()
 功能概要：把指定结果加入到列表中显示
 返 回 值: void
 参    数：param1	指定结果
		   Param2	要加入的位置
**************************************************************/
//##ModelId=49B87BB402DF
void CAutoCallResult::AddResultToList( CCallResult* pResult, int nIndex)
{
	//检查数据有效性
	if(pResult == NULL)
		return;
	//判断是否需要显示
	if(!m_bShowNoFailed)
	{
		//不显示无失败的项目
		if(!HasFaildItem(pResult))
		{
			//没有失败条目
			return;
		}
	}
	if(nIndex < 0)
		return;
	if(m_List.GetSafeHwnd() == NULL)
		return;
	if(pResult->m_pSec == NULL)
		return;
	
	CString str;
	//0: 批次
	str.Format("%d", pResult->m_nSaveNo);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1: 二次设备名
	str = pResult->m_pSec->m_sName;
	m_List.SetItemText(nIndex, 1, str);
	//2: CPU号
	CSecCPU * pCpu = GetCPU(pResult->m_pSec, pResult->m_nCPU);
	if(pCpu != NULL)
	{
		str.Format("%d(%s)", pCpu->code, pCpu->des);
	}
	else
	{
		str.Format("%d", pResult->m_nCPU);
	}
	m_List.SetItemText(nIndex, 2, str);
	//3:定值区号结果
	str = GetResultString(pResult->m_nZone);
	m_List.SetItemText(nIndex, 3, str);
	//4:定值区号结果说明
	if(pResult->m_nZone == 2)//失败
		m_List.SetItemText(nIndex, 4, pResult->m_sZoneNote);
	else
		m_List.SetItemText(nIndex, 4, "-");
	//5:定值结果
	str = GetResultString(pResult->m_nSetting);
	m_List.SetItemText(nIndex, 5, str);
	//6:定值结果说明
	if(pResult->m_nSetting == 2)//失败
		m_List.SetItemText(nIndex, 6, pResult->m_sSettingNote);
	else
		m_List.SetItemText(nIndex, 6, "-");
	//7:开关量结果
	str = GetResultString(pResult->m_nDigital);
	m_List.SetItemText(nIndex, 7, str);
	//8:开关量结果说明
	if(pResult->m_nDigital == 2)//失败
		m_List.SetItemText(nIndex, 8, pResult->m_sDigitalNote);
	else
		m_List.SetItemText(nIndex, 8, "-");
	//9:软压板结果
	str = GetResultString(pResult->m_nSoftBoard);
	m_List.SetItemText(nIndex, 9, str);
	//10:软压板结果说明
	if(pResult->m_nSoftBoard == 2)//失败
		m_List.SetItemText(nIndex, 10, pResult->m_sSoftBoardNote);
	else
		m_List.SetItemText(nIndex, 10, "-");
	//11:模拟量结果
	str = GetResultString(pResult->m_nAnalog);
	m_List.SetItemText(nIndex, 11, str);
	//12:模拟量结果说明
	if(pResult->m_nAnalog == 2)
		m_List.SetItemText(nIndex, 12, pResult->m_sAnalogNote);
	else
		m_List.SetItemText(nIndex, 12, "-");
	//13:录波结果
	str = GetResultString(pResult->m_nOsc);
	m_List.SetItemText(nIndex, 13, str);
	//14:录波结果说明
	if(pResult->m_nOsc == 2)
		m_List.SetItemText(nIndex, 14, pResult->m_sOscNote);
	else
		m_List.SetItemText(nIndex, 14, "-");
	//15:历史事件结果
	str = GetResultString(pResult->m_nHistory);
	m_List.SetItemText(nIndex, 15, str);
	//16:历史事件结果说明
	if(pResult->m_nHistory == 2)
		m_List.SetItemText(nIndex, 16, pResult->m_sHistoryNote);
	else
		m_List.SetItemText(nIndex, 16, "-");
	//17:结束时间
	str = pResult->m_tmEnd.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 17, str);
	//18:情况说明
	m_List.SetItemText( nIndex, 18, GetDetailSituation(pResult->m_sSituation));
	//19:处理意见
	m_List.SetItemText( nIndex, 19, GetDetailDispose(pResult->m_sDispose));
	//20:厂站名
	str = "";
	if(pResult->m_pStation != NULL)
	{
		str = pResult->m_pStation->m_sName;
	}
	m_List.SetItemText(nIndex, 20, str);
	//21:一次设备名
	str = "";
	if(pResult->m_pSec->m_pOwner != NULL)
	{
		str = pResult->m_pSec->m_pOwner->m_sName;
	}
	m_List.SetItemText(nIndex, 21, str);


	//22:所属厂家
	str = "";
	if (pResult->m_pStation != NULL)
	{
		str = pResult->m_pStation->m_sManufacturer;
	}
	m_List.SetItemText(nIndex, 22, str);

	//23:所属调管单位
	str = "";
	if (pResult->m_pStation != NULL)
	{
		str = pResult->m_pStation->m_sManagedepartment;
	}
	m_List.SetItemText(nIndex, 23, str);

	//24:运维单位
	str = "";
	if (pResult->m_pStation != NULL)
	{
		str = pResult->m_pStation->m_sMaintaindepartment;
	}
	m_List.SetItemText(nIndex, 24, str);

	//关联数据
	m_List.SetItemData(nIndex, (DWORD)pResult);
}

/*************************************************************
 函 数 名：GetResultString()
 功能概要：取得结果字符串
 返 回 值: 结果字符串
 参    数：param1	结果标志
		   Param2
**************************************************************/
//##ModelId=49B87BB402EF
CString CAutoCallResult::GetResultString( int nResult )
{
	CString ret = "";
	//0:未召唤 1：成功 2：失败
	if(0 == nResult)
	{
		//未召唤
		ret.LoadString(IDS_CALLRESULT_NOTCALL);
	}
	if(1 == nResult)
	{
		//成功
		ret.LoadString(IDS_CALLRESULT_SUCCESS);
	}
	if(2 == nResult)
	{
		//失败
		ret.LoadString(IDS_CALLRESULT_FAILED);
	}
	if(3 == nResult)
	{
		//装置不支持
		ret.LoadString(IDS_CALLRESULT_NOSUPPORT);
	}
	if(4 == nResult)
	{
		ret.LoadString(IDS_CALLRESULT_NODATA);
	}
	return ret;
}

/*************************************************************
 函 数 名：GetCPU()
 功能概要：取得指定二次设备的指定CPU号的CPU对象
 返 回 值: 找到返回CPU对象的指针, 未找到返回NULL
 参    数：param1	指定二次设备
		   Param2	指定CPU号
**************************************************************/
//##ModelId=49B87BB402FE
CSecCPU* CAutoCallResult::GetCPU( CSecObj* pObj, int nCpu )
{
	//检查数据有效性
	if(pObj == NULL)
		return NULL;
	//查找
	CSecCPU* pCpu = NULL;
	for(int i = 0; i < pObj->m_arrCPU.GetSize(); i++)
	{
		CSecCPU* pTemp = (CSecCPU*)pObj->m_arrCPU.GetAt(i);
		if(pTemp->code == nCpu)
		{
			//找到, 退出循环
			pCpu = pTemp;
			break;
		}
	}
	return pCpu;
}

/*************************************************************
 函 数 名：FillSummaryListData()
 功能概要：填充概述列表框数据
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4031C
void CAutoCallResult::FillSummaryListData()
{
	if(m_ListSummary.GetSafeHwnd() == NULL)
		return;
	//清除旧数据
	m_ListSummary.DeleteAllItems();
	if(m_pStation == NULL)
		return;

	//"项目", "定值区号","定值","开关量","软压板","模拟量","概述"
	CString str;
	CString stemp = "";
	CString sNotstart = stemp.LoadString(IDS_AUTOCALL_NOTSTART);
	//第一行: 总数
	//0: 项目
	str.LoadString(IDS_AUTOCALLRESULT_TOTAL);
	if(m_ListSummary.InsertItem(LVIF_TEXT|LVIF_PARAM, 0, str, 0, 0, 0, 0) == -1)
		return;
	//1:定值区号
	str.Format("%d", m_ZoneStat.nFail+ m_ZoneStat.nSuccess);
	m_ListSummary.SetItemText(0, 1, str);
	//2:定值
	str.Format("%d", m_SettingStat.nFail + m_SettingStat.nSuccess);
	m_ListSummary.SetItemText(0, 2, str);
	//3:开关量
	str.Format("%d", m_DigitalStat.nFail + m_SettingStat.nSuccess);
	m_ListSummary.SetItemText(0, 3, str);
	//4:软压板
	str.Format("%d", m_SoftBoardStat.nFail + m_SoftBoardStat.nSuccess);
	m_ListSummary.SetItemText(0, 4, str);
	//5:模拟量
	str.Format("%d", m_AnalogStat.nFail + m_AnalogStat.nSuccess);
	m_ListSummary.SetItemText(0, 5, str);
	//6:录波
	str.Format("%d", m_OscStat.nFail + m_OscStat.nSuccess);
	m_ListSummary.SetItemText(0, 6, str);
	//7:历史事件
	str.Format("%d", m_nHistoryStat.nFail + m_nHistoryStat.nSuccess);
	m_ListSummary.SetItemText(0, 7, str);
	//8:概述
	stemp.LoadString(IDS_AUTOCALL_STARTTIME);
	str.Format("%s:%s", stemp ,m_tmStart.Format("%Y-%m-%d %H:%M:%S"));
	m_ListSummary.SetItemText(0, 8, str);

	//第二行: 成功数
	//0: 项目
	str.LoadString(IDS_AUTOCALLRESULT_SUCCESS_NUM);
	if(m_ListSummary.InsertItem(LVIF_TEXT|LVIF_PARAM, 1, str, 0, 0, 0, 1) == -1)
		return;
	//1:定值区号
	str.Format("%d", m_ZoneStat.nSuccess);
	m_ListSummary.SetItemText(1, 1, str);
	//2:定值
	str.Format("%d", m_SettingStat.nSuccess);
	m_ListSummary.SetItemText(1, 2, str);
	//3:开关量
	str.Format("%d", m_SettingStat.nSuccess);
	m_ListSummary.SetItemText(1, 3, str);
	//4:软压板
	str.Format("%d", m_SoftBoardStat.nSuccess);
	m_ListSummary.SetItemText(1, 4, str);
	//5:模拟量
	str.Format("%d", m_AnalogStat.nSuccess);
	m_ListSummary.SetItemText(1, 5, str);
	//6:录波
	str.Format("%d", m_OscStat.nSuccess);
	m_ListSummary.SetItemText(1, 6, str);
	//7:历史事件
	str.Format("%d", m_nHistoryStat.nSuccess);
	m_ListSummary.SetItemText(1, 7, str);
	//6:概述
	CTime tm(1971, 1, 1, 1, 1, 1);
	stemp.LoadString(IDS_AUTOCALL_ENDTIME);
	str.Format("%s:%s",  stemp, m_tmEnd==tm?sNotstart:m_tmEnd.Format("%Y-%m-%d %H:%M:%S"));
	m_ListSummary.SetItemText(1, 8, str);

	//第三行: 失败数
	//0: 项目
	str.LoadString(IDS_AUTOCALLRESULT_FAILED_NUM);
	if(m_ListSummary.InsertItem(LVIF_TEXT|LVIF_PARAM, 2, str, 0, 0, 0, 2) == -1)
		return;
	//1:定值区号
	str.Format("%d", m_ZoneStat.nFail);
	m_ListSummary.SetItemText(2, 1, str);
	//2:定值
	str.Format("%d", m_SettingStat.nFail);
	m_ListSummary.SetItemText(2, 2, str);
	//3:开关量
	str.Format("%d", m_DigitalStat.nFail);
	m_ListSummary.SetItemText(2, 3, str);
	//4:软压板
	str.Format("%d", m_SoftBoardStat.nFail);
	m_ListSummary.SetItemText(2, 4, str);
	//5:模拟量
	str.Format("%d", m_AnalogStat.nFail);
	m_ListSummary.SetItemText(2, 5, str);
	//6:录波
	str.Format("%d", m_OscStat.nFail);
	m_ListSummary.SetItemText(2, 6, str);
	//7:历史事件
	str.Format("%d", m_nHistoryStat.nFail);
	m_ListSummary.SetItemText(2, 7, str);
	//6:概述
	CTimeSpan tmSpan;
	tmSpan = m_tmEnd - m_tmStart;
	stemp.LoadString(IDS_AUTOCALLRESULT_TOTALTIME);
	CString sformat = GetDurationFormatStr();
	str.Format("%s:%s", stemp, tmSpan<0?stemp.LoadString(IDS_AUTOCALL_NOTSTART):tmSpan.Format(sformat));
	m_ListSummary.SetItemText(2, 8, str);
}

//##ModelId=49B87BB40399
BOOL CAutoCallResult::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::DestroyWindow();
}

//##ModelId=49B87BB5004E
void CAutoCallResult::OnDestroy() 
{
	//保存UI设置
	if(NeedSave())
		SaveListConfig();
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：响应列表个性化显示消息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5005D
void CAutoCallResult::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				//"批次", "二次设备", "CPU号", "定值区号结果", "定值区号结果说明", "定值结果", "定值结果说明", "开关量结果", "开关量结果说明", "软压板结果", "软压板结果说明", "模拟量结果", "模拟量结果说明", "完成时间", "厂站名", "一次设备", "所属调管单位", "运维单位"
				//结果列
				if(nSubItem == 3 || nSubItem == 5 || nSubItem == 7 || nSubItem == 9 || nSubItem == 11)
				{
					CString str = m_List.GetItemText(nItem, nSubItem);
					CString stemp;

					if(stemp.LoadString(IDS_CALLRESULT_SUCCESS) && str.CompareNoCase(stemp) == 0)
					{
						lplvcd->clrText = g_BatchSuccess;
					}
					else if( stemp.LoadString(IDS_CALLRESULT_FAILED) && str.CompareNoCase(stemp) == 0)
					{
						lplvcd->clrText = g_BatchFail;
					}
					else if( stemp.LoadString(IDS_CALLRESULT_NOTCALL) && str.CompareNoCase(stemp) == 0)
					{
						lplvcd->clrText = g_BatchNULL;
					}
					else if( stemp.LoadString(IDS_CALLRESULT_NOSUPPORT) && str.CompareNoCase(stemp) == 0 )
					{
						lplvcd->clrText = g_BatchNotSupport;
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
 函 数 名：CompareFunction()
 功能概要：比较函数,用于列表排序
 返 回 值: 比较结果.1, 0, -1
 参    数：param1 比较对象1
		   Param2 比较对象2
		   param3 比较参数
**************************************************************/
//##ModelId=49B87BB40214
int CALLBACK CAutoCallResult::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CCallResult* pResult1 = (CCallResult*)lParam1;
	CCallResult* pResult2 = (CCallResult*)lParam2;
	if(pResult2 == NULL || pResult1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"批次", "二次设备", "CPU号", "定值区号结果", "定值区号结果说明", 
	//"定值结果", "定值结果说明", "开关量结果", "开关量结果说明", 
	//"软压板结果", "软压板结果说明", "模拟量结果", "模拟量结果说明", 
	//"完成时间", "厂站名", "一次设备", "所属调管单位", "运维单位"
	switch(nCol)
	{
	case 0: //批次
		iResult = 0;
		break;
	case 1: //二次设备
		str1 = pResult1->m_pSec->m_sName;
		str2 = pResult2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2: //CPU
		iResult = pResult1->m_nCPU - pResult2->m_nCPU;
		break;
	case 3: //定值区号结果
		iResult = pResult1->m_nZone - pResult2->m_nZone;
		break;
	case 4: //定值区号结果说明
		str1 = pResult1->m_sZoneNote;
		str2 = pResult2->m_sZoneNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5:	//定值结果
		iResult = pResult1->m_nSetting - pResult2->m_nSetting;
		break;
	case 6: //定值结果说明
		str1 = pResult1->m_sSettingNote;
		str2 = pResult2->m_sSettingNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 7: //开关量结果
		iResult = pResult1->m_nDigital - pResult2->m_nDigital;
		break;
	case 8: //开关量结果说明
		str1 = pResult1->m_sDigitalNote;
		str2 = pResult2->m_sDigitalNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 9: //软压板结果
		iResult = pResult1->m_nSoftBoard - pResult2->m_nSoftBoard;
		break;
	case 10: //软压板结果说明
		str1 = pResult1->m_sSoftBoardNote;
		str2 = pResult2->m_sSoftBoardNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 11: //模拟量结果
		iResult = pResult1->m_nAnalog - pResult2->m_nAnalog;
		break;
	case 12: //模拟量结果说明
		str1 = pResult1->m_sAnalogNote;
		str2 = pResult2->m_sAnalogNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 13: //录波结果
		iResult = pResult1->m_nOsc - pResult2->m_nOsc;
		break;
	case 14: //录波结果说明
		str1 = pResult1->m_sOscNote;
		str2 = pResult2->m_sOscNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 15: //历史事件结果
		iResult = pResult1->m_nHistory - pResult2->m_nHistory;
		break;
	case 16: //历史事件结果说明
		str1 = pResult1->m_sHistoryNote;
		str2 = pResult2->m_sHistoryNote;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 17: //结束时间
		str1 = pResult1->m_tmEnd.Format("%Y-%m-%d %H:%M:%S");
		str2 = pResult2->m_tmEnd.Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 18: //厂站名
		if(pResult1->m_pStation != NULL)
		{
			str1 = pResult1->m_pStation->m_sName;
		}
		else
			str1 = "";
		if(pResult2->m_pStation != NULL)
		{
			str2 = pResult2->m_pStation->m_sName;
		}
		else
			str2 = "";
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 19: //一次设备
		if(pResult1->m_pSec->m_pOwner != NULL)
		{
			str1 = pResult1->m_pSec->m_pOwner->m_sName;
		}
		else
			str1 = "";
		if(pResult2->m_pSec->m_pOwner != NULL)
		{
			str2 = pResult2->m_pSec->m_pOwner->m_sName;
		}
		else
			str2 = "";
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 20: //所属厂家
		if (pResult1->m_pStation != NULL && pResult2->m_pStation != NULL)
		{
			str1 = pResult1->m_pStation->m_sManufacturer;
			str2 = pResult2->m_pStation->m_sManufacturer;
			iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
			str1.ReleaseBuffer(0);
			str2.ReleaseBuffer(0);
		}
		break;
	case 21: //调管单位
		if (pResult1->m_pStation != NULL && pResult2->m_pStation != NULL)
		{
			str1 = pResult1->m_pStation->m_sManagedepartment;
			str2 = pResult2->m_pStation->m_sManagedepartment;
			iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
			str1.ReleaseBuffer(0);
			str2.ReleaseBuffer(0);
		}
		break;
	case 22: //运维单位
		{
			str1 = pResult1->m_pStation->m_sMaintaindepartment;
			str2 = pResult2->m_pStation->m_sMaintaindepartment;
			iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
			str1.ReleaseBuffer(0);
			str2.ReleaseBuffer(0);
		}
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iResultViewAsc;
	return iResult;
}

//##ModelId=49B87BB5006D
void CAutoCallResult::OnColumnclickListAutocallResult(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iResultViewAsc = - g_iResultViewAsc;
	}
	else
	{
		g_iResultViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名：GetOCTime()
 功能概要：从数据库查询自动总召的时间
 返 回 值: 查询成功返回TRUE, 失败返回FALSE
 参    数：param1	对应tb_sys_config中的KEY
		   Param2	时间
**************************************************************/
//##ModelId=49B87BB4031D
BOOL CAutoCallResult::GetOCTime( CString sKey, CTime& tmTime )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//条件:KeyName,查询Value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Value", EX_STTP_DATA_TYPE_STRING);
	
	Condition condition1;
	CString str;
	str.Format("KEYNAME='%s'", sKey);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CAutoCallResult::GetOCTime, 查询失败");
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//查询成功
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			str = pMemset->GetValue(UINT(0));
			tmTime = StringToTime(str);
		}
	}
	else
	{
		CString str;
		str.Format("CAutoCallResult::GetOCTime, 查询失败,原因为%s", sError);
		WriteLog(str);
		return FALSE;
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	return TRUE;
}

/*************************************************************
 函 数 名：ReFillAll()
 功能概要：重新载入所有数据
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4032D
void CAutoCallResult::ReFillAll(int nSaveNo)
{
//	g_wndWait.Show(SPLASH_NOTIMER);
	//查询批次与最新批次
	if(m_pStation != NULL && nSaveNo == -1)
	{
		QuerySaveNo(m_pStation->m_sID);
		QueryNewSaveNo(m_pStation->m_sID);
	}
	else
	{
		QuerySaveNo("");
		QueryNewSaveNo("");
	}
	if(nSaveNo < 1)
		m_nSelSaveNo = m_nNewSaveNo;
	else
		m_nSelSaveNo = nSaveNo;
	LoadDataFromDB(m_nSelSaveNo);
	FillListData();
	FillSummaryListData();
//	g_wndWait.Hide();
}

/*************************************************************
 函 数 名：OnAutoCallResultChange()
 功能概要：响应选择改变消息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB500AB
void CAutoCallResult::OnAutoCallResultChange( WPARAM wParam, LPARAM lParam )
{
	//取得选择的设备
	CDeviceObj* pDevice = (CDeviceObj*)lParam;
	if(pDevice == NULL)
	{
		//全部显示
		ReFillAll(-1);
		m_bSelectRoot = TRUE;
		return;
	}
	m_bSelectRoot = FALSE;
	m_nSelSaveNo = 0;
	//判断是不是厂站
	if(pDevice->m_nType != TYPE_STATION)
	{

		m_pStation = NULL;
		return;
	}
	else
	{
		CStationObj* pStation = (CStationObj*)pDevice;
		if(pStation != m_pStation)
		{
			m_pStation = pStation;
		}
		else
			return;
	}

	//刷新数据
	ReFillAll(-1);
	//m_pStation = NULL;
}

/*************************************************************
 函 数 名：ClearStat()
 功能概要：清除统计结果
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4033C
void CAutoCallResult::ClearStat()
{
	m_ZoneStat.nFail = 0;
	m_ZoneStat.nSuccess = 0;
	
	m_SettingStat.nSuccess =0;
	m_SettingStat.nFail = 0;
	
	m_DigitalStat.nFail = 0;
	m_DigitalStat.nSuccess = 0;
	
	m_SoftBoardStat.nSuccess = 0;
	m_SoftBoardStat.nFail = 0;
	
	m_AnalogStat.nFail = 0;
	m_AnalogStat.nSuccess = 0;

	m_OscStat.nFail = 0;
	m_OscStat.nSuccess = 0;

	m_nHistoryStat.nFail = 0;
	m_nHistoryStat.nSuccess = 0;

	CTime tm(1971, 1, 1, 1, 1, 1);
	m_tmEnd = tm;
	CTime tm1(2029, 1, 1, 1, 1, 1);
	m_tmStart = tm1;
}

//##ModelId=49B87BB5007E
void CAutoCallResult::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_RESULT))
				return;
			pMenu = Menu.GetSubMenu(0);

			if(GetPrevSaveNo(m_nSelSaveNo) > 0)
				pMenu->EnableMenuItem(IDC_RESULT_PREV, MF_ENABLED);
			if(GetNextSaveNo(m_nSelSaveNo) > 0)
				pMenu->EnableMenuItem(IDC_RESULT_NEXT, MF_ENABLED);
			if( !m_bSelectRoot )
				pMenu->EnableMenuItem(IDC_RESULT_STATIONEXPORT, MF_DISABLED);

			if(!m_bShowNoFailed)
			{
				//隐藏无失败项目
				pMenu->CheckMenuItem(IDC_RESULT_SHOWFAILD, MF_CHECKED);
			}
			else
			{
				//显示
				pMenu->CheckMenuItem(IDC_RESULT_SHOWFAILD, MF_UNCHECKED);
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

//##ModelId=49B87BB5008D
void CAutoCallResult::OnResultExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName = "";

	if(m_pStation != NULL)
	{
		strName.Format("[%s]%s-%s.xls", m_pStation->m_sName, StringFromID(IDS_FUNC_AUTOCALLRESULT), t.Format("%Y%m%d%H%M%S"));
	}
	else
	{
		strName.Format("%s-%s.xls", StringFromID(IDS_FUNC_AUTOCALLRESULT), t.Format("%Y%m%d%H%M%S"));
	}

	//取得用户选择文件夹
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;
	if(WriteResultToExcel(strPath))
	{
		//导出成功
		if(AfxMessageBox(IDS_COMMON_EXPORT_SUCCESS, MB_OKCANCEL) == IDOK)
		{
			CString str = "/n,/select,";
			str += strPath;
			ShellExecute(GetSafeHwnd(), NULL, "Explorer.exe", str, NULL, SW_SHOW); 
		}
	}
	else
	{
		//导出失败
		AfxMessageBox(IDS_COMMON_EXPORT_FAIL);
	}
}

/*************************************************************
 函 数 名：AddResultToExcel()
 功能概要：将指定的结果项写入到指定的Excel文件中
 返 回 值: void
 参    数：param1	数据库对象
		   Param2	结果项
**************************************************************/
//##ModelId=49B87BB4033D
void CAutoCallResult::AddResultToExcel( CDatabase* pDatabase, CCallResult* pResult )
{
	if(pResult == NULL)
		return;

	//判断是否需要显示
	if(!m_bShowNoFailed)
	{
		//不显示无失败的项目
		if(!HasFaildItem(pResult))
		{
			//没有失败条目
			return;
		}
	}

	if(pResult->m_pSec == NULL)
		return;

	CString strSaveNO;
	strSaveNO.Format("%d", pResult->m_nSaveNo);

	CString strSQL;
	
	CString strCPU = "";
	CSecCPU * pCpu = GetCPU(pResult->m_pSec, pResult->m_nCPU);
	if(pCpu != NULL)
	{
		strCPU.Format("%d(%s)", pCpu->code, pCpu->des);
	}
	else
	{
		strCPU.Format("%d", pResult->m_nCPU);
	}
	//8:厂站名
	CString strStation = "";
	if(pResult->m_pStation != NULL)
	{
		strStation = pResult->m_pStation->m_sName;
	}
	//9:一次设备名
	CString strDevice = "";
	if(pResult->m_pSec->m_pOwner != NULL)
	{
		strDevice = pResult->m_pSec->m_pOwner->m_sName;
	}

	CString strManufacturer = "";
	if (pResult->m_pStation != NULL)
	{
		strManufacturer = pResult->m_pStation->m_sManufacturer;
	}

	//17:所属调管单位
	CString sManagedepartment = "";
	if (pResult->m_pStation != NULL)
	{
		sManagedepartment = pResult->m_pStation->m_sManagedepartment;
	}
	
	//18:运维单位
	CString sMaintaindepartment = "";
	if (pResult->m_pStation != NULL)
	{
		sMaintaindepartment = pResult->m_pStation->m_sMaintaindepartment;
	}

	strSQL.Format("INSERT INTO Log VALUES ('%s', '%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')",
		strSaveNO, pResult->m_pSec->m_sName,  strCPU, 
		GetResultString(pResult->m_nZone), pResult->m_nZone==2?pResult->m_sZoneNote:"-", 
		GetResultString(pResult->m_nSetting), pResult->m_nSetting==2?pResult->m_sSettingNote:"-", 
		GetResultString(pResult->m_nDigital), pResult->m_nDigital==2?pResult->m_sDigitalNote:"-", 
		GetResultString(pResult->m_nSoftBoard), pResult->m_nSoftBoard==2?pResult->m_sSoftBoardNote:"-", 
		GetResultString(pResult->m_nAnalog), pResult->m_nAnalog==2?pResult->m_sAnalogNote:"-",
		GetResultString(pResult->m_nOsc), pResult->m_nOsc==2?pResult->m_sOscNote:"-",
		GetResultString(pResult->m_nHistory), pResult->m_nHistory==2?pResult->m_sHistoryNote:"-",
		pResult->m_tmEnd.Format("%Y-%m-%d %H:%M:%S"),GetDetailSituation(pResult->m_sSituation),
		GetDetailDispose(pResult->m_sDispose),
		strStation, strDevice, strManufacturer, sManagedepartment, sMaintaindepartment);

	pDatabase->ExecuteSQL(strSQL);
}

/*************************************************************
 函 数 名：WriteResultToExcel()
 功能概要：将结果导出到Excel
 返 回 值: 
 参    数：param1	文件路径
		   Param2
**************************************************************/
//##ModelId=49B87BB4034C
BOOL CAutoCallResult::WriteResultToExcel( CString strFullPath )
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
				strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT,\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT)",
					StringFromID(IDS_AUTOCALLRESULT_NUMBER), StringFromID(IDS_MODEL_SECONDARY), StringFromID(IDS_MODEL_CPU), StringFromID(IDS_AUTOCALLRESULT_ZONE), 
					StringFromID(IDS_AUTOCALLRESULT_ZONE_NOTE), StringFromID(IDS_AUTOCALLRESULT_SETTING), StringFromID(IDS_AUTOCALLRESULT_SETTING_NOTE), 
					StringFromID(IDS_AUTOCALLRESULT_DIGITAL), StringFromID(IDS_AUTOCALLRESULT_DIGITAL_NOTE), StringFromID(IDS_AUTOCALLRESULT_SOFT), StringFromID(IDS_AUTOCALLRESULT_SOFT_NOTE), 
					StringFromID(IDS_AUTOCALLRESULT_ANALOG), StringFromID(IDS_AUTOCALLRESULT_ANALOG_NOTE), StringFromID(IDS_AUTOCALLRESULT_OSC), StringFromID(IDS_AUTOCALLRESULT_OSC_NOTE), 
					StringFromID(IDS_AUTOCALLRESULT_HISEVENT), StringFromID(IDS_AUTOCALLRESULT_HISEVENT_NOTE), StringFromID(IDS_COMMON_FINISHTIME), StringFromID(IDS_AUTOCALLRESULT_OVERVIEW), 
					StringFromID(IDS_AUTOCALLRESULT_RESOLUTION), StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_MODEL_PRIMARY), StringFromID(IDS_MODEL_MANUFACTURER),
					StringFromID(IDS_MODEL_MANAGEMENT), StringFromID(IDS_MODEL_MAINTENANCE));

				database.ExecuteSQL(strSQL);
			}

			POSITION pos = m_listResult.GetHeadPosition();
			while(pos != NULL)
			{
				CCallResult* pResult = (CCallResult*)m_listResult.GetNext(pos);
				if(pResult == NULL)
					continue;
				AddResultToExcel(&database, pResult);
			}
		}
		else
			return FALSE;

		database.Close();
	}
	catch(CDBException * e)
	{
		WriteLog("创建Excel表错误: " + e->m_strError, XJ_LOG_LV1);
		
		return FALSE;
	}
	return TRUE;
}

BOOL CAutoCallResult::WriteResultToExcel(CString strFullPath, CString sStationID)
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
			
			POSITION pos = m_listResult.GetHeadPosition();
			while(pos != NULL)
			{
				CCallResult* pResult = (CCallResult*)m_listResult.GetNext(pos);
				if(pResult == NULL)
					continue;
				if( pResult->m_sStationId == sStationID )
				{
					if(!IsFind)
					{
						//新建
						strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT,\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT)",
							StringFromID(IDS_AUTOCALLRESULT_NUMBER), StringFromID(IDS_MODEL_SECONDARY), StringFromID(IDS_MODEL_CPU), StringFromID(IDS_AUTOCALLRESULT_ZONE), 
							StringFromID(IDS_AUTOCALLRESULT_ZONE_NOTE), StringFromID(IDS_AUTOCALLRESULT_SETTING), StringFromID(IDS_AUTOCALLRESULT_SETTING_NOTE), 
							StringFromID(IDS_AUTOCALLRESULT_DIGITAL), StringFromID(IDS_AUTOCALLRESULT_DIGITAL_NOTE), StringFromID(IDS_AUTOCALLRESULT_SOFT), StringFromID(IDS_AUTOCALLRESULT_SOFT_NOTE), 
							StringFromID(IDS_AUTOCALLRESULT_ANALOG), StringFromID(IDS_AUTOCALLRESULT_ANALOG_NOTE), StringFromID(IDS_AUTOCALLRESULT_OSC), StringFromID(IDS_AUTOCALLRESULT_OSC_NOTE), 
							StringFromID(IDS_AUTOCALLRESULT_HISEVENT), StringFromID(IDS_AUTOCALLRESULT_HISEVENT_NOTE), StringFromID(IDS_COMMON_FINISHTIME), StringFromID(IDS_AUTOCALLRESULT_OVERVIEW), 
							StringFromID(IDS_AUTOCALLRESULT_RESOLUTION), StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_MODEL_PRIMARY), StringFromID(IDS_MODEL_MANUFACTURER),
							StringFromID(IDS_MODEL_MANAGEMENT), StringFromID(IDS_MODEL_MAINTENANCE));
						database.ExecuteSQL(strSQL);
						IsFind = TRUE;
					}
					AddResultToExcel(&database, pResult);
				}
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
 函 数 名：TranslateSingleKey()
 功能概要：解释单个关键字的意义
 返 回 值: 关键字表示的值
 参    数：param1	关键字
		   Param2
**************************************************************/
//##ModelId=49B87BB4037C
CString CAutoCallResult::TranslateSingleKey( CString sKey )
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
		//取得头部
		CString strHead = sKey.Left(nFind);
		//取得事件
		CCallResult* pResult = NULL;
		if(strHead == "$AUTORESULT_LIST")
		{
			int nCount = 0;
			POSITION pos = NULL;
			pos = m_listResult.GetHeadPosition();
			while(pos != NULL)
			{
				pResult = (CCallResult*)m_listResult.GetNext(pos);
				if(nCount == nIndex)
					break;
				nCount++;
			}
		}
		if(pResult == NULL)
			return "";
		if(pResult->m_pSec == NULL)
			return "";
		
		CString str;
		//取得尾部字符
		CString strTail = sKey.Right(sKey.GetLength() - (nFind2 + 3));
		if(strTail == "PT_NAME$")
		{
			//二次设备名
			return pResult->m_pSec->m_sName;
		}
		if(strTail == "PT_MODEL$")
		{
			//二次设备型号
			return pResult->m_pSec->m_sModel;
		}
		if(strTail == "PT_CPU$")
		{
			//CPU号
			str = "";
			str.Format("%d", pResult->m_nCPU);
			return str;
		}
		if(strTail == "ZONE_RESULT$")
		{
			//定值区号结果
			return GetResultString(pResult->m_nZone);
		}
		if(strTail == "SETTING_RESULT$")
		{
			//定值结果
			return GetResultString(pResult->m_nSetting);
		}
		if(strTail == "DIGITAL_RESULT$")
		{
			//开关量结果
			return GetResultString(pResult->m_nDigital);
		}
		if(strTail == "SOFTBOARD_RESULT$")
		{
			//软压板结果
			return GetResultString(pResult->m_nSoftBoard);
		}
		if(strTail == "ANALOG_RESULT$")
		{
			//模拟量结果
			return GetResultString(pResult->m_nAnalog);
		}
		if(strTail == "COMPLETE_TIME$")
		{
			//结束时间
			return pResult->m_tmEnd.Format("%Y-%m-%d %H:%M:%S");
		}
		if(strTail == "STATION_NAME$")
		{
			//厂站名
			str = "";
			if(pResult->m_pStation != NULL)
			{
				str = pResult->m_pStation->m_sName;
			}
			return str;
		}
		if(strTail == "DEVICE_NAME$")
		{
			//9:一次设备名
			str = "";
			if(pResult->m_pSec->m_pOwner != NULL)
			{
				str = pResult->m_pSec->m_pOwner->m_sName;
			}
			return str;
		}
	}
	else
	{
		//判断是否有.GetCount()方法
		nFind = sKey.Find(".GetCount()", 0);
		if(nFind > -1)
		{
			//有.GetCount()方法
			if(sKey == "$AUTORESULT_LIST.GetCount()$")
			{
				sReturn.Format("%d", m_List.GetItemCount());
				return sReturn;
			}
		}
		else
		{
			if(sKey == "$STATION_NAME$")
			{
				//厂站名
				if(m_pStation == NULL)
					return "";
				return m_pStation->m_sName;
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
//##ModelId=49B87BB4035B
BOOL CAutoCallResult::InitCOM()
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
//##ModelId=49B87BB4035C
void CAutoCallResult::ReleaseCOM()
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

//##ModelId=49B87BB5008F
void CAutoCallResult::OnResultPrint() 
{
	// TODO: Add your command handler code here
	
	//打印
	InitCOM();
	//打开模板文件
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, AUTORESULT_LIST);
		if(szTemp.cx != 0 && szTemp.cy != 0)
		{
			bOpened = TRUE;
		}
		else
		{
			CString sLog;
			sLog.Format("打开打印模板文件失败.\r\n%s", AUTORESULT_LIST);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//设置回调函数
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInAutoCallResult);
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
 函 数 名：QuerySaveNo()
 功能概要：查询指定站点的总召批次
 返 回 值: void
 参    数：param1	站ID
		   Param2
**************************************************************/
//##ModelId=49B87BB4035D
void CAutoCallResult::QuerySaveNo( CString sStationID )
{
	ClearSaveNo();
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//字段
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "SaveNo", EX_STTP_DATA_TYPE_INT);

	
	//条件
	Condition Condition1;
	str.Format("Is_Auto = 0");
	pApp->m_DBEngine.SetCondition(sql, Condition1, str);

	if (sStationID != "")
	{
		Condition con2;
		str.Format("station_id = '%s'", sStationID);
		pApp->m_DBEngine.SetCondition(sql, con2, str);
	}

	//按时间排序
// 	Condition condition3;
// 	str.Format("order by TIME DESC");
// 	pApp->m_DBEngine.SetCondition(sql, condition3, str, EX_STTP_WHERE_ABNORMALITY);//非where
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OC_RESULT_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CAutoCallResult::QuerySaveNo, 查询失败");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		for(int i = 0; i < nCount; i++)
		{
			//保存到数组中
			SaveNumber* sn = new SaveNumber;
			CString str = pMemset->GetValue(UINT(0));
			sn->nSaveNo = atoi(str);
// 			str = pMemset->GetValue(1);
// 			sn->tmTime = StringToTime(str);
			m_arrSaveNo.Add(sn);
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CAutoCallResult::QuerySaveNo,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	//排序
	SortSaveNo();
}

/*************************************************************
 函 数 名：QueryNewSaveNo()
 功能概要：查询指定站点的最新的总召批次号
 返 回 值: 
 参    数：param1	站ID
		   Param2
**************************************************************/
//##ModelId=49B87BB4035F
void CAutoCallResult::QueryNewSaveNo( CString sStationID )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//字段
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "SaveNo", EX_STTP_DATA_TYPE_INT);

	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "1", EX_STTP_DATA_TYPE_TOP);
	
	//条件
	Condition Condition1;
	str.Format("Is_Auto = 0");
	pApp->m_DBEngine.SetCondition(sql, Condition1, str);

	if (sStationID != "")
	{
		Condition con2;
		str.Format("station_id = '%s'", sStationID);
		pApp->m_DBEngine.SetCondition(sql, con2, str);
	}

	//按时间排序
	Condition condition3;
	str.Format("order by TIME DESC");
	pApp->m_DBEngine.SetCondition(sql, condition3, str, EX_STTP_WHERE_ABNORMALITY);//非where
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OC_RESULT_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CAutoCallResult::QueryNewSaveNo, 查询失败");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		for(int i = 0; i < nCount; i++)
		{
			//保存到数组中
			CString str = pMemset->GetValue(UINT(0));
			m_nNewSaveNo = atoi(str);
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CAutoCallResult::QueryNewSaveNo,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

/*************************************************************
 函 数 名：GetPrevSaveNo()
 功能概要：取得上一批次的批次号
 返 回 值: 上一批次批次号
 参    数：param1 当前批次号
		   Param2
**************************************************************/
//##ModelId=49B87BB4036C
int CAutoCallResult::GetPrevSaveNo( int nCurSaveNo )
{
	if(nCurSaveNo == m_nOldSaveNo)
	{
		//是最老的
		return -1;
	}

	int nSaveNo = nCurSaveNo - 1;
	if(nSaveNo < m_nMinSaveNo)
	{
		nSaveNo = m_nMaxSaveNo;
	}

	return nSaveNo;
}

/*************************************************************
 函 数 名：GetNextSaveNo()
 功能概要：取得下一批次的批次号
 返 回 值: 下一批次批次号
 参    数：param1	当前批次号
		   Param2
**************************************************************/
//##ModelId=49B87BB4036E
int CAutoCallResult::GetNextSaveNo( int nCurSaveNo )
{
	if(nCurSaveNo == m_nNewSaveNo)
		return -1; //是最新的

	int nSaveNo = nCurSaveNo + 1;
	if(nSaveNo > m_nMaxSaveNo)
	{
		nSaveNo = m_nMinSaveNo;
	}
	
	return nSaveNo;
}

//##ModelId=49B87BB50091
void CAutoCallResult::OnResultPrev() 
{
	// TODO: Add your command handler code here
	ReFillAll(GetPrevSaveNo(m_nSelSaveNo));
}

//##ModelId=49B87BB5009C
void CAutoCallResult::OnResultNext() 
{
	// TODO: Add your command handler code here
	ReFillAll(GetNextSaveNo(m_nSelSaveNo));
}

/*************************************************************
 函 数 名：ClearSaveNo()
 功能概要：清除批次记录
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4037A
void CAutoCallResult::ClearSaveNo()
{
	for(int i = 0; i < m_arrSaveNo.GetSize(); i++)
	{
		SaveNumber * sn = (SaveNumber*)m_arrSaveNo.GetAt(i);
		if(sn != NULL)
			delete sn;
	}
	m_arrSaveNo.RemoveAll();
}

/*************************************************************
 函 数 名：SortSaveNo()
 功能概要：对批次按时间从旧到新进行排序
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB4037B
void CAutoCallResult::SortSaveNo()
{
	int nMax = 0;
	int nMin = 99;
	//查找最大,最小批次号
	for(int i = 0; i < m_arrSaveNo.GetSize(); i++)
	{
		SaveNumber* sn = (SaveNumber*)m_arrSaveNo.GetAt(i);
		if(sn == NULL)
			continue;
		if(sn->nSaveNo > nMax)
			nMax = sn->nSaveNo;
		if(sn->nSaveNo < nMin)
			nMin = sn->nSaveNo;
	}
	m_nMaxSaveNo = nMax;
	m_nMinSaveNo = nMin;

	if(m_nNewSaveNo == nMax)
	{
		//最新的批次号等于最大的批次号, 则最小的批次号为最老的
		m_nOldSaveNo = nMin;
	}
	else if(m_nNewSaveNo < nMax)
	{
		//最新的批次号小于最大的批次号, 则最老的批次号为最新的批次号+1
		m_nOldSaveNo = m_nNewSaveNo+1;
	}
}

/*************************************************************
 函 数 名: HasFaildItem()
 功能概要: 判断一个自动总召的结果项是否有失败条目
 返 回 值: 有失败条目返回TRUE, 无失败条目返回FALSE
 参    数: param1 指定自动总召结果项对象
		   Param2 
**************************************************************/
BOOL CAutoCallResult::HasFaildItem( CCallResult* pResult )
{
	if(pResult == NULL)
		return TRUE;
	//0:未召唤 1：成功 2：失败 3: 装置不支持
	if(2 == pResult->m_nZone) //定值区号
		return TRUE;
	if(2 == pResult->m_nSetting) //定值
		return TRUE;
	if(2 == pResult->m_nDigital) //开关量
		return TRUE;
	if(2 == pResult->m_nAnalog) //模拟量
		return TRUE;
	if(2 == pResult->m_nSoftBoard) //软压板
		return TRUE;
	if(2 == pResult->m_nOsc)//录波
		return TRUE;
	if(2 == pResult->m_nHistory)//历史事件
		return TRUE;
	return FALSE;
}

void CAutoCallResult::OnResultShowfaild() 
{
	// TODO: Add your command handler code here
	m_bShowNoFailed = !m_bShowNoFailed;
	//重新填充列表
	FillListData();
}

void CAutoCallResult::OnWinClose() 
{
	// TODO: Add your command handler code here
	GetParentFrame()->SendMessage(WM_CLOSE);
}

void CAutoCallResult::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_FUNC_AUTOCALLRESULT));
	
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CAutoCallResult::OnResultStationexport() 
{
	// TODO: Add your command handler code here
	m_pExportProcess = NULL;

	CTime t = CTime::GetCurrentTime();
	CString strName = "";
	
	strName.Format("%s-%s", StringFromID(IDS_FUNC_AUTOCALLRESULT), t.Format("%Y%m%d%H%M%S"));

	
	//取得用户选择文件夹
	CString strPath = GetSelectDir();
	if(strPath.IsEmpty())
		return;
	strPath += strName;

	//创建文件夹
	CreateDirectory(strPath,NULL);

	g_sRootDirPath = strPath;

	CDlgExportProcess dlg;
	m_pExportProcess = &dlg;
	AfxBeginThread(ExportByStation,this,THREAD_PRIORITY_BELOW_NORMAL);
	dlg.DoModal();
}
