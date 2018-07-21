// ViewStationInfo.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ViewStationInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @brief           升降序作用位*/
int g_iStationInfoViewAsc;
/////////////////////////////////////////////////////////////////////////////
// CViewStationInfo

IMPLEMENT_DYNCREATE(CViewStationInfo, CFormView)

CViewStationInfo::CViewStationInfo()
	: CFormView(CViewStationInfo::IDD)
{
	//{{AFX_DATA_INIT(CViewStationInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	g_iStationInfoViewAsc = -1;
	m_arrColum.RemoveAll();
	m_pStation = NULL;
	m_nOldSortCol = -1;
}

CViewStationInfo::~CViewStationInfo()
{
}

void CViewStationInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewStationInfo)
	DDX_Control(pDX, IDC_LIST_STATIONINFO, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewStationInfo, CFormView)
	//{{AFX_MSG_MAP(CViewStationInfo)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_STATIONINFO, OnColumnclickListStationinfo)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_STATIONINFO, OnCustomDraw)
	ON_COMMAND(IDC_STATIONINFO_SEC, OnStationinfoSec)
	ON_COMMAND(IDC_STATIONINFO_COMM_H, OnStationinfoCommH)
	ON_COMMAND(IDC_STATIONINFO_RUN_H, OnStationinfoRunH)
	ON_COMMAND(IDC_STATIONINFO_EXCLEL, OnStationinfoExclel)
	ON_COMMAND(IDC_STATIONINFO_PRINT, OnStationinfoPrint)
	ON_COMMAND(IDC_STATIONINFO_REFRESH, OnStationinfoRefresh)
	ON_WM_CONTEXTMENU()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STATIONINFO, OnDblclkListStationinfo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(STATIONINFO_FRAME_OPEN, OnStationInfoOpen)
	ON_MESSAGE(WM_STTP_20002, OnSTTP20002)
	ON_MESSAGE(WM_STTP_20079, OnSTTP20079)
	ON_MESSAGE(STATIONINIT, OnStationInit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewStationInfo diagnostics

#ifdef _DEBUG
void CViewStationInfo::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewStationInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewStationInfo message handlers

void CViewStationInfo::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	//初始化列表
	InitListCtrl();
	//开始刷新保护通信状态和运行状态持续时间定时器
	m_nTimer = SetTimer(1, g_RefreshTime*60*1000, 0);
}

/*************************************************************
 函 数 名: InitListCtrl()
 功能概要: 初始化列表
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewStationInfo::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	//初始化列表风格
	InitListStyle();
	//填充数据
//	FillListData();
	return 0;
}

/*************************************************************
 函 数 名: InitListStyle()
 功能概要: 初始化列表样式
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewStationInfo::InitListStyle()
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
		//char* arColumn[8]={"设备名称","通讯状态", "通讯状态改变时间", "通讯状态持续时间","运行状态", "运行状态改变时间", "运行状态持续时间", "一次设备"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 8; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 300; //设备名称
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 1://
				lvCol.cx = 100; // 通讯状态
				colName = StringFromID(IDS_FUNC_COMMUSTATUS);
				break;
			case 2://
				lvCol.cx = 180; // 时间
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 3://
				lvCol.cx = 180; // 持续时间
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 4://
				lvCol.cx = 100; //运行状态
				colName = StringFromID(IDS_FUNC_RUNSTATUS);
				break;
			case 5:
				lvCol.cx = 180; //时间
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 6:
				lvCol.cx = 180; //持续时间
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 7:
				lvCol.cx = 160; // 一次设备
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			default:
				lvCol.cx=50;//列宽100象素
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

/*************************************************************
 函 数 名: FillListData()
 功能概要: 填充列表数据
 返 回 值: 失败返回-1, 成功返回>=0
 参    数: param1 
		   Param2 
**************************************************************/
int CViewStationInfo::FillListData()
{
	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();

	if(m_pStation == NULL)
		return -1;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return -1;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, m_pStation->m_sID);
	//循环所有
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		AddEventToList(pObj, 0);
	}
	//恢复刷新
	m_List.SetRedraw(TRUE);
	
	return 0;
}

/*************************************************************
 函 数 名: LoadListConfig()
 功能概要: 载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewStationInfo::LoadListConfig()
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
			WriteLog("ViewStationInfo::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("ViewStationInfo::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewStationInfoConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewStationInfoConfig", XJ_LOG_LV3);
				WriteLog("ViewStationInfo::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewStationInfoConfig下的配置", XJ_LOG_LV3);
					WriteLog("ViewStationInfo::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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

/*************************************************************
 函 数 名: SaveListConfig()
 功能概要: 保存列信息配置
 返 回 值: 保存成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewStationInfo::SaveListConfig()
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
			WriteLog("ViewStationInfo::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewStationInfo::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewStationInfoConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewStationInfoConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewStationInfoConfig");
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
						WriteLog("未找到ViewStationInfoConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
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
						WriteLog("找到了ViewStationInfoConfig下的配置, 先删除新建所有子节点", XJ_LOG_LV3);
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
 函 数 名: NeedSave()
 功能概要: 判断是否需要保存列表的列信息
 返 回 值: 需要保存返回TRUE,不需要保存返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewStationInfo::NeedSave()
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
 函 数 名: RegulateControlSize()
 功能概要: 调整控件大小,位置 
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewStationInfo::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	//客户区大小
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	
	//调整列表框大小,占整个高度的(1 - 0.618)
	float fHList = (float)rcList.Height();
	fHList = float(fHList);
	rcList.bottom = (long)fHList;
	m_List.MoveWindow(&rcList, TRUE);
}

void CViewStationInfo::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 函 数 名: AddEventToList()
 功能概要: 把指定装置的通讯状态,运行状态加入到列表中显示
 返 回 值: 加入成功返回TRUE,失败返回FALSE
 参    数: param1 要加入的装置
		   Param2 加入的位置
**************************************************************/
BOOL CViewStationInfo::AddEventToList( CSecObj * pSec, int i )
{
	//判断数据有效性
	if(pSec == NULL)
		return FALSE;
	if(m_List.GetSafeHwnd() == NULL)
		return FALSE;

	//刷新持续时间
	pSec->RefreshCommDurativeTime();
	pSec->RefreshRunDurativeTime();
	
	//"设备名称","通讯状态", "通讯状态改变时间", "通讯状态持续时间","运行状态", "运行状态改变时间", "运行状态持续时间", "一次设备"
	
	//0:设备名称
	CString str;
	str = pSec->m_sName;
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 0, i) == -1)
		return FALSE;
	//1:通讯状态
	m_List.SetItemText(i, 1, GetCommStatusString(pSec->m_nCommStatus));
	//2:通讯状态改变时间
	m_List.SetItemText(i, 2, pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
	//3:通讯状态持续时间
	m_List.SetItemText(i, 3, pSec->m_sCommDurativeTime);
	//4:运行状态
	m_List.SetItemText(i, 4, GetRunStatusString(pSec->m_nRunStatu));
	//5:运行状态改变时间
	m_List.SetItemText(i, 5, pSec->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
	//6:运行状态持续时间
	m_List.SetItemText(i, 6, pSec->m_sRunDurativeTime);
	//7:一次设备
	str = "";
	if(pSec->m_pOwner != NULL)
	{
		str = pSec->m_pOwner->m_sName;
	}
	m_List.SetItemText(i, 7, str);
	
	//关联数据
	m_List.SetItemData(i, (DWORD)pSec);
	return TRUE;
}


void CViewStationInfo::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

/*************************************************************
 函 数 名: OnStationInfoOpen()
 功能概要: 响应页面打开或重新载入消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewStationInfo::OnStationInfoOpen( WPARAM wParam, LPARAM lParam )
{
	RegulateControlSize();
	//取得新厂站指针
	m_pStation = (CStationObj*)lParam;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	if(m_pStation != NULL)
	{
		CString str;
		str.Format("[%s]%s", m_pStation->m_sName, StringFromID(IDS_DEVICE_INFO));
		pApp->SetAppTile(str);
	}
	//载入数据
	FillListData();
}

void CViewStationInfo::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	if(m_pStation != NULL)
	{
		CString str;
		str.Format("[%s]%s", m_pStation->m_sName, StringFromID(IDS_DEVICE_INFO));
		pApp->SetAppTile(str);
	}

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*************************************************************
 函 数 名: OnSTTP20002()
 功能概要: 响应通讯状态改变报文
 返 回 值: 
 参    数: param1 设备对象指针
		   Param2 
**************************************************************/
void CViewStationInfo::OnSTTP20002( WPARAM wParam, LPARAM lParam )
{
	CSecObj* pObj = (CSecObj*)wParam;
	if(pObj == NULL)
		return;
	if(m_pStation == NULL)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
		return;
	//改变列表中的显示
	ChangeCommStatusInList(pObj);
}

/*************************************************************
 函 数 名: OnSTTP20079()
 功能概要: 响应运行状态改变报文
 返 回 值: 
 参    数: param1 设备对象指针
		   Param2 
**************************************************************/
void CViewStationInfo::OnSTTP20079( WPARAM wParam, LPARAM lParam )
{
	CSecObj* pObj = (CSecObj*)wParam;
	if(pObj == NULL)
		return;
	if(m_pStation == NULL)
		return;
	if(pObj->m_sStationID != m_pStation->m_sID)
		return;
	//改变列表中的显示
	ChangeRunStatusInList(pObj);
}

/*************************************************************
 函 数 名: ChangeCommStatusInList()
 功能概要: 改变指定二次设备的通信状态在通信状态列表框中的显示
 返 回 值: 
 参    数: param1 指定二次设备
		   Param2 
**************************************************************/
void CViewStationInfo::ChangeCommStatusInList( CSecObj* pSec )
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
	
	if(nIndex < 0)
	{
		//未找到，不显示
		return;
	}
	else
	{
		//改变值
		//1:通讯状态
		m_List.SetItemText(nIndex, 1, GetCommStatusString(pSec->m_nCommStatus));
		//2:通讯状态改变时间
		m_List.SetItemText(nIndex, 2, pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		//3:通讯状态持续时间
		m_List.SetItemText(nIndex, 3, pSec->m_sCommDurativeTime);
		//刷新
		m_List.Update(nIndex);
	}
}

/*************************************************************
 函 数 名: ChangeRunStatusInList()
 功能概要: 改变指定二次设备的运行状态在通信状态列表框中的显示
 返 回 值: 
 参    数: param1 指定二次设备
		   Param2 
**************************************************************/
void CViewStationInfo::ChangeRunStatusInList( CSecObj* pSec )
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
	
	if(nIndex < 0)
	{
		//未找到，不显示
		return;
	}
	else
	{
		//改变值
		//4:运行状态
		m_List.SetItemText(nIndex, 4, GetRunStatusString(pSec->m_nRunStatu));
		//5:运行状态改变时间
		m_List.SetItemText(nIndex, 5, pSec->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
		//6:运行状态持续时间
		m_List.SetItemText(nIndex, 6, pSec->m_sRunDurativeTime);
		//刷新
		m_List.Update(nIndex);
	}
}

/*************************************************************
 函 数 名: CompareFunction()
 功能概要: 比较函数,排序时被调用
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数: param1 比较对象1
		   Param2 比较对象2
		   Param3 比较参数
**************************************************************/
int CALLBACK CViewStationInfo::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CSecObj* pObj1 = (CSecObj*)lParam1;
	CSecObj* pObj2 = (CSecObj*)lParam2;
	if(pObj2 == NULL || pObj1 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"设备名称","通讯状态", "通讯状态改变时间", "通讯状态持续时间","运行状态", "运行状态改变时间", "运行状态持续时间", "一次设备"
	switch(nCol)
	{
	case 0: //保护名称
		str1 = pObj1->m_sName;
		str2 = pObj2->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 1: //通讯状态
		iResult = pObj1->m_nCommStatus - pObj2->m_nCommStatus;
		break;
	case 2: //通讯状态改变时间
		str1.Format("%s", pObj1->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		str2.Format("%s", pObj2->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"));
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //通讯状态持续时间
		iResult = pObj1->m_lCommDurative - pObj2->m_lCommDurative;
		break;
	case 4: //运行状态
		iResult = pObj1->m_nRunStatu - pObj2->m_nRunStatu;
		break;
	case 5: //运行状态改变时间
		str1.Format("%s", pObj1->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
		str2.Format("%s", pObj2->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"));
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 6: //运行状态持续时间
		iResult = pObj1->m_lRunDurative - pObj2->m_lRunDurative;
		break;
	case 7: //一次设备名
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
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iStationInfoViewAsc;
	return iResult;
}

void CViewStationInfo::OnColumnclickListStationinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iStationInfoViewAsc = - g_iStationInfoViewAsc;
	}
	else
	{
		g_iStationInfoViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

void CViewStationInfo::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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

					//得到事件对象
					CSecObj * pSec = (CSecObj*)m_List.GetItemData(nItem);
					if(pSec == NULL)
						break;
					if(nSubItem == 1) //通讯状态
					{
						if(pSec->m_nCommStatus == 1)
						{
							//断开
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
						}
					}
					if(nSubItem == 4) //运行状态
					{
						switch(pSec->m_nRunStatu)
						{
						case 0: //检修
							lplvcd->clrText = XJ_STATUS_RUN_CHECK;
							break;
						case 1: //停用
							lplvcd->clrText = XJ_STATUS_RUN_OFF;
							break;
// 						case 2: //投运
// 							lplvcd->clrTextBk = XJ_STATUS_RUN_ON;
// 							break;
						case 3: //未接入
							lplvcd->clrText = XJ_STATUS_RUN_CODE;
							break;
						case 4: //调试
							lplvcd->clrText = XJ_STATUS_RUN_DEBUG;
							break;
						default:
							break;
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

void CViewStationInfo::OnStationinfoSec() 
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
	//显示该事件的二次设备属性
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CXJBrowserDoc * pDoc = pApp->GetCurDocument();
	if(pObj->m_nType == TYPE_WAVEREC)
		pDoc->ShowSecPropPage(pObj, 1);
	else
		pDoc->ShowSecPropPage(pObj);
}

void CViewStationInfo::OnStationinfoCommH() 
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

void CViewStationInfo::OnStationinfoRunH() 
{
	// TODO: Add your command handler code here
	//准备打开查询与统计的参数
	//保护运行状态查询: 3$$$厂站ID$$$设备类型$$$设备ID $$$运行状态$$$自动查询(0-不自动查询, 1-自动查询)
	CString sSQL = "";
	int nCount = m_List.GetSelectedCount();
	if(nCount < 1)
	{
		sSQL = "3$$$ $$$99$$$ $$$99$$$0";
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

void CViewStationInfo::OnStationinfoExclel() 
{
	// TODO: Add your command handler code here
	if(m_pStation == NULL)
		return;
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("[%s]%s%s.xls", m_pStation->m_sName, StringFromID(IDS_DEVICE_INFO),t.Format("%Y%m%d%H%M%S"));
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

void CViewStationInfo::OnStationinfoPrint() 
{
	// TODO: Add your command handler code here
	
	//打印
	InitCOM();
	//打开模板文件
	BOOL bOpened = FALSE;
	if(pReportDoc != NULL)
	{
		CSize szTemp = OpenReportFile(pReportDoc, STATIONINFO_LIST);
		if(szTemp.cx != 0 && szTemp.cy != 0)
			bOpened = TRUE;
		else{
			CString sLog;
			sLog.Format("%s.\r\n%s", StringFromID(IDS_TIP_OPEN_PRINTTEMPLATE_FAIL),STATIONINFO_LIST);
			WriteLog(sLog, XJ_LOG_LV1);
			AfxMessageBox(sLog);
		}
	}
	//设置回调函数
	if(pReportDoc != NULL && bOpened)
		pReportDoc->SetQueryFunction(TranslateKeyInStationInfo);
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

void CViewStationInfo::OnStationinfoRefresh() 
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

void CViewStationInfo::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_STATIONINFO))
				return;
			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount == 1)
			{
				//只选择了一个
				pMenu->EnableMenuItem(IDC_STATIONINFO_SEC, MF_ENABLED);
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
}

/*************************************************************
 函 数 名: WriteResultToExcel()
 功能概要: 将结果导出到Excel
 返 回 值: 导出成功返回TRUE, 失败返回FALSE
 参    数: param1 文件路径
		   Param2 
**************************************************************/
BOOL CViewStationInfo::WriteResultToExcel( CString strFullPath )
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
				//"设备名称","通讯状态", "通讯状态改变时间", "通讯状态持续时间","运行状态", "运行状态改变时间", "运行状态持续时间", "一次设备"
				strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT)",
					StringFromID(IDS_MODEL_SECONDARY), StringFromID(IDS_FUNC_COMMUSTATUS), StringFromID(IDS_STATUS_CHANGE_TIME), StringFromID(IDS_COMMON_DURATION),
					StringFromID(IDS_FUNC_RUNSTATUS), StringFromID(IDS_STATUS_CHANGE_TIME), StringFromID(IDS_COMMON_DURATION), StringFromID(IDS_MODEL_PRIMARY));
				database.ExecuteSQL(strSQL);
			}
			for(int i = 0; i < m_List.GetItemCount(); i++)
			{
				CSecObj* pObj = (CSecObj*)m_List.GetItemData(i);
				AddResultToExcel(&database, pObj);
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
 函 数 名: AddResultToExcel()
 功能概要: 将指定的结果项写入到指定的Excel文件中
 返 回 值: 
 参    数: param1 数据库对象
		   Param2 设备对象
**************************************************************/
void CViewStationInfo::AddResultToExcel( CDatabase* pDatabase, CSecObj* pSec )
{
	//检查数据有效性
	if(pSec == NULL)
		return;
	if(pDatabase == NULL)
		return;
	
	//一次设备
	CString strDevice = "";
	if(pSec->m_pOwner != NULL)
	{
		strDevice = pSec->m_pOwner->m_sName;
	}
	
	//"设备名称","状态", "状态改变时间", "状态持续时间", "厂站","一次设备"
	CString strSQL;
	strSQL.Format("INSERT INTO Log VALUES ('%s','%s','%s','%s','%s', '%s', '%s','%s')",
		pSec->m_sName, GetCommStatusString(pSec->m_nCommStatus), pSec->m_tmNewCommStatus.Format("%Y-%m-%d %H:%M:%S"), pSec->m_sCommDurativeTime, 
		GetRunStatusString(pSec->m_nRunStatu), pSec->m_tmNewRunStatus.Format("%Y-%m-%d %H:%M:%S"), pSec->m_sRunDurativeTime, strDevice);
	pDatabase->ExecuteSQL(strSQL);
}

/*************************************************************
 函 数 名: InitCOM()
 功能概要: 初始化打印模板COM组件
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewStationInfo::InitCOM()
{
	return InitReportComNoReg(pClf, pReportDoc);
}

/*************************************************************
 函 数 名: ReleaseCOM()
 功能概要: 删除COM组件
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewStationInfo::ReleaseCOM()
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
 函 数 名: TranslateSingleKey()
 功能概要: 解释单个关键字的意义
 返 回 值: 关键字表示的值
 参    数: param1 关键字
		   Param2 
**************************************************************/
CString CViewStationInfo::TranslateSingleKey( CString sKey )
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
		if(strTail == "DEVICE_RUN$")
		{
			//运行状态
			return GetRunStatusString(pObj->m_nRunStatu);
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

/*************************************************************
 函 数 名: RefreshDuravieTime()
 功能概要: 刷新LIST中指定位置的持续时间显示
 返 回 值: 
 参    数: param1 指定在LIST中的位置
		   Param2 
**************************************************************/
void CViewStationInfo::RefreshDuravieTime( int nIndex )
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
	//刷新通讯状态持续时间
	pObj->RefreshCommDurativeTime();
	//更改显示
	m_List.SetItemText(nIndex, 3, pObj->m_sCommDurativeTime);

	//刷新运行状态持续时间
	pObj->RefreshRunDurativeTime();
	//更改显示
	m_List.SetItemText(nIndex, 6, pObj->m_sRunDurativeTime);

	//刷新
	m_List.Update(nIndex);
}

/*************************************************************
 函 数 名: RefreshTime()
 功能概要: 刷新所有保护的通信状态和运行状态的持续时间
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewStationInfo::RefreshTime()
{
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
}

void CViewStationInfo::OnTimer(UINT nIDEvent) 
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

void CViewStationInfo::OnDblclkListStationinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected >= 0)
	{
		//得到该行对应的设备
		//得到设备
		CSecObj* pObj = (CSecObj*)m_List.GetItemData(nSelected);
		if(pObj == NULL)
			return;
		//显示该二次设备属性
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		if(pObj->m_nType == TYPE_WAVEREC)
			pDoc->ShowSecPropPage(pObj, 1);
		else
			pDoc->ShowSecPropPage(pObj);
	}
	*pResult = 0;
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewStationInfo::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	FillListData();
	WriteLog("StationInit,厂站内装置信息页面刷新完毕", XJ_LOG_LV3);
}
