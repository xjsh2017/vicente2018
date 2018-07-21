// ViewStaInfo.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"

#include "ViewStaInfo.h"
#include "MainFrm.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/** @brief           升降序作用位*/
int g_iStationInfoViewDesc;

/////////////////////////////////////////////////////////////////////////////
// CViewStaInfo

IMPLEMENT_DYNCREATE(CViewStaInfo, CFormView)

CViewStaInfo::CViewStaInfo()
	: CFormView(CViewStaInfo::IDD)
{
	g_ShowCommOFF = 1;
	g_ShowCommON = 1;
	g_iStationInfoViewDesc = -1;
	m_nTimer = 0;
	//{{AFX_DATA_INIT(CViewStaInfo)
	//}}AFX_DATA_INIT
}

CViewStaInfo::~CViewStaInfo()
{
}

void CViewStaInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewStaInfo)
	DDX_Control(pDX, IDC_LIST_STAINFO, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewStaInfo, CFormView)
	//{{AFX_MSG_MAP(CViewStaInfo)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_STAINFO, OnColumnclickListStationinfo)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_STAINFO, OnCustomDraw)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_COMM_ON, OnCommOn)
	ON_COMMAND(IDC_COMM_OFF, OnCommOff)
	ON_COMMAND(IDC_STATION_HISTORY, OnStationHistory)
	ON_COMMAND(IDC_COMM_EXPORT, OnCommExport)
	ON_COMMAND(IDC_COMM_PRINT, OnCommPrint)
	ON_COMMAND(IDC_STATION_TREE, OnStationTree)
	ON_COMMAND(IDC_STATION_MAP, OnStationMap)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(STATIONINFO_FRAME_OPEN, OnStaInfoOpen)
	ON_MESSAGE(WM_STTP_20144, OnSTTP20144)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewStaInfo diagnostics

#ifdef _DEBUG
void CViewStaInfo::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewStaInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewStaInfo message handlers
/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表,指定默认列宽
 返 回 值: 失败返回-1,成功返回>=0
**************************************************************/
//##ModelId=49B87B850169
int CViewStaInfo::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	//初始化列表风格
	InitListStyle();
	if (LoadProNameFromDB()==FALSE)
	{
		return false;
	}

// 	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
// 	CDataEngine * pData = (CDataEngine*)pApp->GetDataEngine();
// 	if(pData == NULL)
// 		return -1;
// 	
// 	DEVICE_LIST listDevice;
// 	listDevice.RemoveAll();
// 	pData->GetStationList(listDevice);
// 	POSITION pos = listDevice.GetHeadPosition();
// 	
// 	while(pos != NULL)
// 	{
// 		//取得厂站指针
// 		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
// 		LoadStatusFromDB(pObj);
// 		
// 	}//FillListData();
	return 0;
}
/*************************************************************
函 数 名：InitListStyle()
功能概要：初始化列表风格, 包括显示风格和列信息
返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B850177
int CViewStaInfo::InitListStyle()
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
		//char* arColumn[14]={"地区","厂站名称","通讯状态","订购策略", "通讯状态改变时间", "通讯状态持续时间", "断开原因","子站厂家","子站IP","子站网关","子站型号","子站版本","通讯规约","历史记录"};
		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 14; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 60; // 地区
				colName = StringFromID(IDS_MODEL_AREA);
				break;
			case 1:
				lvCol.cx = 100; // 厂站名称
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 2://
				lvCol.cx = 100; // 通讯状态
				colName = StringFromID(IDS_COMMU_STATUS);
				break;
			case 3:
				lvCol.cx = 120; // 订购策略
				colName = StringFromID(IDS_ORDER_STRATEGY);
				break;
			case 4://
				lvCol.cx = 180; // 时间
				colName = StringFromID(IDS_STATUS_CHANGE_TIME);
				break;
			case 5:
				lvCol.cx = 200; // 持续时间
				colName = StringFromID(IDS_COMMON_DURATION);
				break;
			case 6:
				lvCol.cx = 200; // 断开原因
				colName = StringFromID(IDS_DISCONNECT_REASON);
				break;
			case 7:
				lvCol.cx = 120; // 子站厂家
				colName = StringFromID(IDS_MODEL_MANUFACTURER);
				break;
			case 8:
				lvCol.cx = 180; // ip
				colName = StringFromID(IDS_COMMON_IP);
				break;
			case 9:
				lvCol.cx = 160; // 网关
				colName = StringFromID(IDS_COMMON_GATEWAY);
				break;
			case 10:
				lvCol.cx = 100; // 子站型号
				colName = StringFromID(IDS_COMMON_MODEL);
				break;
			case 11:
				lvCol.cx = 100; // 子站版本
				colName = StringFromID(IDS_COMMON_VERSION);
				break;
			case 12:
				lvCol.cx = 160; // 通讯规约
				colName = StringFromID(IDS_COMMON_PROTOCOL);
				break;
			case 13:
				lvCol.cx = 0; //历史记录
				colName = StringFromID(IDS_HISTORY_RECORD);
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
 函 数 名：FillListData()
 功能概要：填充列表数据
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
//##ModelId=49B87B850178
int CViewStaInfo::FillListData()
{
	
	
	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	
	//if(m_pStation == NULL)
	//	return -1;
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return -1;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice/*, m_pStation->m_sID*/);
	//循环所有
	
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		//LoadStatusFromDB(pObj);
		CDeviceObj* pDev = pData->FindDevice(pObj->m_sNetID,TYPE_NET);
		if(pObj == NULL)
			continue;
		if ((!pObj->IsCommStatusON()&&0==g_ShowCommOFF)||(pObj->IsCommStatusON()&&0==g_ShowCommON))
		{
		}
		else
			AddStationInfoToList(pObj,pDev, 0);
	}


	//恢复刷新
	m_List.SetRedraw(TRUE);
	return true;

}
/*************************************************************
 函 数 名：RegulateControlSize()
 功能概要：调整控件位置
 返 回 值: 返回值说明
**************************************************************/
//##ModelId=49B87B8501A5
void CViewStaInfo::RegulateControlSize()
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

void CViewStaInfo::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();
	//初始化列表
	InitListCtrl();
	
	m_nTimer = SetTimer(1, g_RefreshTime*60*1000, 0);

	FillListData();
	//FillListData();
}


/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
//##ModelId=49B87B850186
BOOL CViewStaInfo::LoadListConfig()
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
			WriteLog("ViewStaInfo::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("ViewStaInfo::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewStaInfoConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewStaInfoConfig", XJ_LOG_LV3);
				WriteLog("ViewStationInfo::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewStaInfoConfig下的配置", XJ_LOG_LV3);
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


void CViewStaInfo::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CFormView::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}
/*************************************************************
 函 数 名：AddEventToList()
 功能概要：加入事件信息到列表,如果要加入的位置已经有数据了, 原先的数据
			将会自动下移,新数据在正确位置	
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数：param1   要加入的事件指针
           Param2   要加入的位置,从0开始计数
**************************************************************/
//##ModelId=49B87B8501B7
BOOL CViewStaInfo::AddStationInfoToList(CStationObj* pObj,CDeviceObj* pDev, int i)
{
	//判断数据有效性
	if(pObj == NULL)
		return FALSE;
	//判断数据有效性
	if(pDev == NULL)
		return FALSE;
	if(m_List.GetSafeHwnd() == NULL)
		return FALSE;
	
	//刷新持续时间
//	pSec->RefreshCommDurativeTime();
//	pSec->RefreshRunDurativeTime();
	pObj->RefreshDurativeTime();

	//"地区","厂站名称","通讯状态","订购策略", "通讯状态改变时间", "通讯状态持续时间", "断开原因","子站厂家","子站IP","子站网关","子站型号","子站版本","通讯规约","历史记录"
	//0:地区
	CString str;
//	str.Format("%s(%s)", pObj->m_sName, pObj->m_sModel);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, pDev->m_sName, 0, 0, 0, i) == -1)
		return FALSE;
	//1:厂站名称
	m_List.SetItemText(i, 1, pObj->m_sName);
	//2:通讯状态
	if (pObj->IsCommStatusON())
	{
		m_List.SetItemText(i, 2, StringFromID(IDS_COMMUSTATUS_OFF));
	}
	else
	{
		m_List.SetItemText(i, 2, StringFromID(IDS_COMMUSTATUS_OFF));
	}

	//3:订购策略
	if (pObj->m_nOrderStrategy!=0)
	{
		m_List.SetItemText(i, 3, StringFromID(IDS_ORDERSTRATEGY_SMART));
	}
	else
	{
		m_List.SetItemText(i,3,StringFromID(IDS_ORDERSTRATEGY_ALL));
	}
	
	//4:通讯状态改变时间
	m_List.SetItemText(i, 4, pObj->GetStatusTime().Format("%Y-%m-%d %H:%M:%S"));
	//5:通讯状态持续时间
	m_List.SetItemText(i, 5, pObj->GetStatusDurative().Format(GetDurationFormatStr()));
	//6:断开原因
	m_List.SetItemText(i, 6, pObj->GetStatusReason());
	//7:子站厂家
	m_List.SetItemText(i,7,pObj->m_sManufacturer);
	//8:ip
	m_List.SetItemText(i, 8, pObj->m_sIP);
	//9:网关
	m_List.SetItemText(i, 9, pObj->m_sTel);
	//10:子站型号
	m_List.SetItemText(i, 10, pObj->m_sStationModel);
	//11:子站版本
	m_List.SetItemText(i, 11, pObj->m_sStationVer);
	//12:通讯规约
	m_List.SetItemText(i, 12, pObj->m_sProtocol);
// 	m_List.SetColumnWidth(0,80);
// 	m_List.SetColumnWidth(7,80);
// 	m_List.SetColumnWidth(10,0);
// 	m_List.SetColumnWidth(11,0);
// 	m_List.SetColumnWidth(13,0);
	//关联数据
	m_List.SetItemData(i, (DWORD)pObj);
	return TRUE;
}
/*************************************************************
 函 数 名: CompareFunction()
 功能概要: 比较函数,排序时被调用
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数: param1 比较对象1
		   Param2 比较对象2
		   Param3 比较参数
**************************************************************/
int CALLBACK CViewStaInfo::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据

	CStationObj* pObj3 = (CStationObj*)lParam1;
	CStationObj* pObj4 = (CStationObj*)lParam2;

//	if((pObj2 == NULL || pObj1 == NULL)&&(!(pObj2 == NULL && pObj1 == NULL)))
//		return 0;
	if ((pObj3 == NULL || pObj4 == NULL)&&(!(pObj3 == NULL && pObj4 == NULL)))
		return 0;
	
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"地区","厂站名称","通讯状态","订购策略", "通讯状态改变时间", "通讯状态持续时间", "断开原因","子站厂家","子站IP","子站网关","子站型号","子站版本","通讯规约","历史记录"
	switch(nCol)
	{
	case 0: //地区
 		str1.Format("%s",pObj3->m_sNetID);
 		str2.Format("%s",pObj4->m_sNetID);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 1://厂站名称
		str1.Format("%s(%s)",pObj3->m_sNetID,pObj3->m_sName);
		str2.Format("%s(%s)",pObj4->m_sNetID,pObj4->m_sName);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 2://通讯状态
		iResult = pObj3->IsCommStatusON() - pObj4->IsCommStatusON();

		break;

	case 3://订购策略
		str1.Format("%d",pObj3->m_nOrderStrategy);
		str2.Format("%d",pObj4->m_nOrderStrategy);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 4://通讯状态改变时间
		str1 = pObj3->GetStatusTime().Format("%Y-%m-%d %H:%M:%S");
		str2 = pObj4->GetStatusTime().Format("%Y-%m-%d %H:%M:%S");
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;

	case 5: //通讯状态持续时间
		iResult = pObj3->GetStatusDurative().GetTotalSeconds() - pObj4->GetStatusDurative().GetTotalSeconds();
		break;
	case 6://断开原因

		str1 = pObj3->GetStatusReason();
		str2 = pObj4->GetStatusReason();
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
 		break;
	case 7://子站厂家
		str1 = "";
		if(pObj3->m_sManufacturer != "")
			str1 = pObj3->m_sManufacturer;
		str2 = "";
		if(pObj4->m_sManufacturer != "")
			str2 = pObj4->m_sManufacturer;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 8://子站IP
		str1 = "";
		if(pObj3->m_sIP != "")
			str1 = pObj3->m_sIP;
		str2 = "";
		if(pObj4->m_sIP != "")
			str2 = pObj4->m_sIP;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 9://子站网关
		str1 = "";
		if(pObj3->m_sTel != "")
			str1 = pObj3->m_sTel;
		str2 = "";
		if(pObj4->m_sTel != "")
			str2 = pObj4->m_sTel;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 10://子站型号"
		break;
	case 11://子站版本
		break;
	case 12://通讯规约
		str1 = "";
		if(pObj3->m_sProtocol != "")
			str1 = pObj3->m_sProtocol;
		str2 = "";
		if(pObj4->m_sProtocol != "")
			str2 = pObj4->m_sProtocol;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 13://历史记录
		break;
	default:
		iResult = 0;
		break;
	}
	iResult *= g_iStationInfoViewDesc;
	return iResult;
}
/*************************************************************
 函 数 名: OnStationInfoOpen()
 功能概要: 响应页面打开或重新载入消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewStaInfo::OnStaInfoOpen( WPARAM wParam, LPARAM lParam )
{
	RegulateControlSize();
	//取得新厂站指针
	m_pStation = (CStationObj*)lParam;
	//载入数据
	FillListData();
}



/*************************************************************
 函 数 名：LoadStatusFromDB()
 功能概要：从硬盘数据库读入通讯规约名称
 返 回 值: 	
**************************************************************/
BOOL CViewStaInfo::LoadProNameFromDB()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//通过Station_ID查询最新一次值(Status, Time, Reason)
	
	//设置字段
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "ID", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Name", EX_STTP_DATA_TYPE_STRING);
	
	
	//设置条件
// 	Condition Condition0;
 	CString str;
// 	str.Format("ID = '%s'", pObj->m_sID);
// 	pApp->m_DBEngine.SetCondition(sql, Condition0, str);
	
	// 	Condition Condition1;
	// 	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC", EX_STTP_WHERE_ABNORMALITY);
	// 	
	CMemSet pMemset;
	//	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_COMMU_PROTOCOL_CFG, sql, sError, &pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CViewStaInfo::LoadStatusFromDB, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		//		delete pMemset;
		return FALSE;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		
		for(int i = 0; i < nCount; i++)
		{
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CDataEngine* pData = pApp->GetDataEngine();
			if(pData == NULL)
				return -1;
			DEVICE_LIST listDevice;
			listDevice.RemoveAll();
			pData->GetStationList(listDevice/*, m_pStation->m_sID*/);
			//循环所有
			
			POSITION pos = listDevice.GetHeadPosition();
			while(pos != NULL)
			{
				CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
				if (pObj->m_nProtocolId == atoi(pMemset.GetValue((UINT)0)))
				{
					pObj->m_sProtocol = pMemset.GetValue(1);
				}
			}
			pMemset.MoveNext();		
		}
		
	}
	else
	{
		str.Format("CViewStaInfo::LoadStatusFromDB, 查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
	}
	delete[] sError;
	//	delete pMemset;
	sError = NULL;
	//	pMemset = NULL;
	return TRUE;
}
void CViewStaInfo::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
					CStationObj * pObj = (CStationObj*)m_List.GetItemData(nItem); 
					if(pObj == NULL)
						break;
					if(nSubItem == 2) //通讯状态
					{
						if(!pObj->IsCommStatusON())
						{
							//断开
							lplvcd->clrText = XJ_STATUS_COMM_OFF;
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
void CViewStaInfo::OnColumnclickListStationinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
		g_iStationInfoViewDesc = - g_iStationInfoViewDesc;
	}
	else
	{
		g_iStationInfoViewDesc = 1;
		m_nOldSortCol = iCol;
	}
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}

/*************************************************************
 函 数 名: SaveListConfig()
 功能概要: 保存列信息配置
 返 回 值: 保存成功返回TRUE, 失败返回FALSE
 参    数: param1 
		   Param2 
**************************************************************/
BOOL CViewStaInfo::SaveListConfig()
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
			WriteLog("ViewStaInfo::保存UIConfig失败", XJ_LOG_LV3);
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
				WriteLog("ViewStaInfo::保存UIConfig失败", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{
				//加载文件成功
				MSXML2::IXMLDOMNodePtr pRoot;
				pRoot = pDoc ->selectSingleNode("//UIConfig");
				MSXML2::IXMLDOMNodePtr pSelected;
				pSelected = pDoc->selectSingleNode("//ViewStaInfoConfig");
				
				//查找ViewActionConfig
				if(pSelected == NULL)
				{	
					//未找到,新建节点
					WriteLog("未找到ViewStaInfoConfig, 将新建节点和子节点", XJ_LOG_LV3);	
					//新建节点
					MSXML2::IXMLDOMElementPtr pNew = NULL;
					pNew = pDoc ->createElement("ViewStaInfoConfig");
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
						WriteLog("未找到ViewStaInfoConfig下的配置, 新建所有子节点", XJ_LOG_LV3);
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
						WriteLog("找到了ViewStaInfoConfig下的配置, 先删除新建所有子节点", XJ_LOG_LV3);
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
BOOL CViewStaInfo::NeedSave()
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


void CViewStaInfo::OnDestroy() 
{
	//保存UI配置
	if(NeedSave())
		SaveListConfig();

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


void CViewStaInfo::OnContextMenu(CWnd* pWnd, CPoint point) 
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
			if(!Menu.LoadMenu(IDR_MENU_STATION))
				return;
			pMenu = Menu.GetSubMenu(0);
			
			int nCount = m_List.GetSelectedCount();
			
			if(nCount == 1)
			{
				//只选择了一个
				pMenu->EnableMenuItem(IDC_STATION_MAP, MF_GRAYED);
				pMenu->EnableMenuItem(IDC_STATION_TREE, MF_GRAYED);
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

void CViewStaInfo::OnCommOn() 
{
	// TODO: Add your command handler code here
	g_ShowCommON == 0 ? g_ShowCommON = 1 :	g_ShowCommON = 0;

	FillListData();
}

void CViewStaInfo::OnCommOff() 
{
	// TODO: Add your command handler code here
	g_ShowCommOFF == 0 ? g_ShowCommOFF = 1 :  g_ShowCommOFF = 0;
	
	FillListData();
}

void CViewStaInfo::OnStationHistory() 
{
	// TODO: Add your command handler code here
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
	//	int nIndex=((CListCtrl*)GetDlgItem(IDC_LIST_STAINFO))->GetItemPosition();
		
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		DEVICE_LIST listDevice;
		listDevice.RemoveAll();
		pData->GetStationList(listDevice);
		POSITION pos = NULL;
		for(pos = listDevice.GetHeadPosition(); pos != NULL;)
		{
			CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
//			if (!pStation->m_sName.CompareNoCase(m_List.GetItemText(nIndex)))
		//	{
				//
				sSQL.Format("1$$$%s$$$99$$$1", pStation->m_sID);
				break;
		//	}		
		}
	}
	//打开查询与统计
	OpenXJQuery(sSQL, this->GetSafeHwnd());
}

void CViewStaInfo::OnCommExport() 
{
	// TODO: Add your command handler code here
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("%s%s.xls", StringFromID(IDS_TOOLBAR_STATIONINFO),t.Format("%Y%m%d%H%M%S"));
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

void CViewStaInfo::OnCommPrint() 
{
	// TODO: Add your command handler code here
	
}

void CViewStaInfo::OnStationTree() 
{
	// TODO: Add your command handler code here
	
}

void CViewStaInfo::OnStationMap() 
{
	// TODO: Add your command handler code here
	
}
/*************************************************************
 函 数 名：WriteResultToExcel()
 功能概要：将列表中的数据导出到Excel
 返 回 值: 导出成功返回TRUE, 失败返回FALSE
 参    数：param1	Excel文件保存位置
		   Param2
**************************************************************/
BOOL CViewStaInfo::WriteResultToExcel( CString strFullPath )
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
					//"地区","厂站名称","通讯状态","订购策略", "通讯状态改变时间", "通讯状态持续时间", "断开原因","子站厂家","子站IP","子站网关","子站型号","子站版本","通讯规约","历史记录"
				strSQL.Format("CREATE TABLE Log(\"%s\" TEXT, \"%s\" TEXT,\"%s\" TEXT, \"%s\" TEXT,\"%s\" TEXT,\"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, \"%s\" TEXT, 通讯规约 TEXT)",
					StringFromID(IDS_MODEL_AREA), StringFromID(IDS_MODEL_SUBSTATION), StringFromID(IDS_COMMU_STATUS), StringFromID(IDS_ORDER_STRATEGY),
					StringFromID(IDS_STATUS_CHANGE_TIME), StringFromID(IDS_COMMON_DURATION),StringFromID(IDS_DISCONNECT_REASON), StringFromID(IDS_MODEL_MANUFACTURER),
					StringFromID(IDS_COMMON_IP), StringFromID(IDS_COMMON_GATEWAY), StringFromID(IDS_COMMON_MODEL), StringFromID(IDS_COMMON_VERSION),
					StringFromID(IDS_COMMON_PROTOCOL));
				database.ExecuteSQL(strSQL);
			}

			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CDataEngine* pData = pApp->GetDataEngine();
			if(pData == NULL)
				return -1;
			for(int i = 0; i < m_List.GetItemCount(); i++)
			{
				CStationObj* pObj = (CStationObj*)m_List.GetItemData(i);
				CDeviceObj* pDev = pData->FindDevice(pObj->m_sNetID,TYPE_NET);
				if(pObj == NULL)
					continue;
				AddResultToExcel(&database, pObj,pDev);
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
void CViewStaInfo::AddResultToExcel(CDatabase* pDatabase,CStationObj* pStation,CDeviceObj* pDev)
{
	//检查数据有效性
	MYASSERT(pDatabase);
	MYASSERT(pStation);

		//"地区","厂站名称","通讯状态","订购策略", "通讯状态改变时间", "通讯状态持续时间", "断开原因","子站厂家","子站IP","子站网关","子站型号","子站版本","通讯规约","历史记录"
	CString strSQL;
	strSQL.Format("INSERT INTO Log VALUES ('%s','%s','%s','%s','%s', '%s','%s','%s','%s', '%s', '%s', '%s', '%s')",
		pDev->m_sName,pStation->m_sName, (pStation->IsCommStatusON())?StringFromID(IDS_COMMUSTATUS_ON):StringFromID(IDS_COMMUSTATUS_OFF),
		(pStation->m_nOrderStrategy==0)?StringFromID(IDS_ORDERSTRATEGY_ALL):StringFromID(IDS_ORDERSTRATEGY_SMART), 
		pStation->GetStatusTime().Format("%Y-%m-%d %H:%M:%S"), pStation->GetStatusDurative().Format(GetDurationFormatStr()), 
		pStation->GetStatusReason(), pStation->m_sManufacturer, pStation->m_sIP, pStation->m_sTel, pStation->m_sStationModel,  
		pStation->m_sStationVer,pStation->m_sProtocol);
	pDatabase->ExecuteSQL(strSQL);
}


void CViewStaInfo::OnTimer(UINT nIDEvent) 
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
/*************************************************************
 函 数 名: RefreshTime()
 功能概要: 刷新所有保护的通信状态和运行状态的持续时间
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CViewStaInfo::RefreshTime()
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

/*************************************************************
 函 数 名: RefreshDuravieTime()
 功能概要: 刷新LIST中指定位置的持续时间显示
 返 回 值: 
 参    数: param1 指定在LIST中的位置
		   Param2 
**************************************************************/
void CViewStaInfo::RefreshDuravieTime( int nIndex )
{
	//总数
	int nCount = m_List.GetItemCount();
	//判断索引有效性
	if(nIndex < 0)
		return;
	if(nIndex >= nCount)
		return;
	CStationObj* pObj = (CStationObj*)m_List.GetItemData(nIndex);
	if(pObj == NULL)
		return;


	//刷新运行状态持续时间
	pObj->RefreshDurativeTime();
	//更改显示
	m_List.SetItemText(nIndex, 5, pObj->GetStatusDurative().Format(GetDurationFormatStr()));


	//刷新
	m_List.Update(nIndex);
}
/*************************************************************
 函 数 名: OnSTTP20002()
 功能概要: 响应通讯状态改变报文
 返 回 值: 
 参    数: param1 设备对象指针
		   Param2 
**************************************************************/
void CViewStaInfo::OnSTTP20144( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CViewStaInfo::OnSTTP20144 start", XJ_LOG_LV3);
	
	//查找厂站
	CStationObj * pObj = (CStationObj*)lParam;
	if(!pObj)
		return;
	
	ChangeStationStatusInList(pObj);

    //SetTimer(1, g_RefreshStationTime*60*1000, 0);
	WriteLog("CViewStaInfo::OnSTTP20144 neat end", XJ_LOG_LV3);
}
void CViewStaInfo::ChangeStationStatusInList(CStationObj* pObj)
{
//判断数据有效性
	if(pObj == NULL)
		return;
	if(m_List.GetSafeHwnd() == NULL)
		return;
	
	//查找在列表中的位置
	LVFINDINFO lvFind;
	int nIndex = -1;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pObj;
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
			//2:通讯状态
	if (pObj->IsCommStatusON())
	{
		m_List.SetItemText(nIndex, 2, StringFromID(IDS_COMMUSTATUS_ON));
	}
	else
	{
		m_List.SetItemText(nIndex, 2, StringFromID(IDS_COMMUSTATUS_OFF));
	}
	
	//4:通讯状态改变时间
	m_List.SetItemText(nIndex, 4, pObj->GetStatusTime().Format("%Y-%m-%d %H:%M:%S"));
	//5:通讯状态持续时间
	m_List.SetItemText(nIndex, 5, pObj->GetStatusDurative().Format(GetDurationFormatStr()));
	//6:断开原因
	m_List.SetItemText(nIndex, 6, pObj->GetStatusReason());
	}
}

void CViewStaInfo::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	pApp->SetAppTile(StringFromID(IDS_TOOLBAR_STATIONINFO));
//	GetParentFrame()->SetWindowText("厂站信息");
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/****************************************************
Date:2013/6/3  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CViewStaInfo::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	FillListData();
	WriteLog("StationInit,厂站信息页面刷新完毕", XJ_LOG_LV3);
}

void CViewStaInfo::OnViewRefresh(WPARAM wParam, LPARAM lParam)
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	
	//所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		ChangeStationStatusInList(pObj);
	}
	
	WriteLog("CViewStaInfo::OnViewRefresh, 厂站信息页面强制刷新完毕", XJ_LOG_LV3);
}
