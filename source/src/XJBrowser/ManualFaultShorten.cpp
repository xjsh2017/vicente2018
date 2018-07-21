// ManualFaultShorten.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ManualFaultShorten.h"
#include "GlobalFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualFaultShorten dialog


CManualFaultShorten::CManualFaultShorten(CWnd* pParent /*=NULL*/)
	: CDialog(CManualFaultShorten::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualFaultShorten)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_EventManager.ClearAllEvents();
	ClearEvent();
	m_nMainRecID = -1;
    pView = NULL;
}


void CManualFaultShorten::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualFaultShorten)
	DDX_Control(pDX, IDC_ACTION_LIST, m_List);
	DDX_Control(pDX, IDC_DEVICE_TREE, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManualFaultShorten, CDialog)
	//{{AFX_MSG_MAP(CManualFaultShorten)
	ON_NOTIFY(NM_CLICK, IDC_DEVICE_TREE, OnClickDeviceTree)
	ON_BN_CLICKED(IDC_BTN_PTOSCLIST_DOWN, OnBtnPtosclistDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualFaultShorten message handlers


CManualFaultShorten::~CManualFaultShorten()
{
	m_bitmap.Detach();
	m_imageList.Detach();
	ClearEvent();
	//m_EventManager.ClearAllEvents();
}



BOOL CManualFaultShorten::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	m_Tree.ModifyStyle( TVS_CHECKBOXES, 0 );
//	m_Tree.ModifyStyle( 0, TVS_CHECKBOXES );
	// TODO: Add extra initialization here
	//导入图标
	//	m_imageList.Create(IDB_BITMAP_TREE,16,1,RGB(255,255,255));
	if (g_style ==3)
	{
		m_imageList.Create(16, 16, ILC_MASK | ILC_COLOR32, 1, 1);
	}
	else
	{
		m_imageList.Create (16, 16, ILC_COLOR32, 1,1);
	}
	
	if (g_style == 2)
	{
		m_bitmap.LoadBitmap(IDB_BITMAP_TREE1);
		m_imageList.Add(&m_bitmap, RGB(222, 255, 255));
	}
	else if (g_style == 3)
	{
		m_bitmap.LoadBitmap(IDB_BITMAP_TREE2);
		m_imageList.Add(&m_bitmap, RGB(0,0,0));
	}
	else
	{
		m_bitmap.LoadBitmap(IDB_BITMAP_TREE);
		m_imageList.Add(&m_bitmap, RGB(0, 0, 0));
	}
	//m_imageList.SetBkColor(RGB(240, 255, 255));
	
	
	
	//设置图标
	m_Tree.SetImageList(&m_imageList,TVSIL_NORMAL);
	if (g_style == 2)
	{
		m_Tree.SetBkColor(g_ListSpaceColor3);
	}
	else if (g_style == 3)
	{
		m_Tree.SetBkColor(g_TreeBkColor);
		m_Tree.SetTextColor(g_TreeTextColor);
	}
	//初始化
	InitDeviceTree();


	//载入配置
	//LoadConfig();
	
	//初始化列表
	InitListCtrl();

	//设置时间框
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_START);
	if(pTime == NULL)
		return false;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_END);
	if(pTime2 == NULL)
		return false;
	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	pTime2->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//设置范围
	CTime tmMin(1972, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	pTime2->SetRange(&tmMin, &tmMax);
	//设置时间
	CTime tm = CTime::GetCurrentTime();
	CTimeSpan ts(7, 0, 0, 0);
	CTime tm1 = tm - ts;
	VERIFY(pTime->SetTime(&tm1));


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*************************************************************
 函 数 名：LoadListConfig()
 功能概要：载入列信息配置
 返 回 值: 载入成功返回TRUE, 失败返回FALSE
**************************************************************/
BOOL CManualFaultShorten::LoadListConfig()
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
			WriteLog("ViewAction::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
				WriteLog("ViewAction::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewActionConfig");

			//查找ViewActionConfig
			if(pSelected == NULL)
			{	
				//未找到
				WriteLog("未找到ViewActionConfig", XJ_LOG_LV3);
				WriteLog("ViewAction::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//找到
				if(!pSelected ->hasChildNodes())
				{
					//找不到子节点
					WriteLog("未找到ViewActionConfig下的配置", XJ_LOG_LV3);
					WriteLog("ViewAction::读取UIConfig失败,将使用默认列风格", XJ_LOG_LV3);
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
 函 数 名：InitListStyle()
 功能概要：初始化列表风格, 包括显示风格和列信息
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
int CManualFaultShorten::InitListStyle()
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
		//char* arColumn[10]={"序号","类别", "故障类型","二次设备", "装置启动时间","信息接收时间","消息内容","厂站名","一次设备","已确认"};
		for (int nCol=0; nCol < 10 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; //列宽50象素 序号
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 80; //列宽60象素 类别
				colName = StringFromID(IDS_COMMON_CLASS);
				break;
			case 2://
				lvCol.cx = 100; //列宽60象素 故障类型
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			case 3://
				lvCol.cx = 200; //列宽160象素 保护名称
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 4: 
				lvCol.cx = 180; //装置启动时间
				colName = StringFromID(IDS_EVENT_STARTTIME);
				break;
			case 5:
				lvCol.cx = 180; //信息接收时间
				colName = StringFromID(IDS_EVENT_RECIVETIME);
				break;
			case 6:
				lvCol.cx = 200; //信息内容
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 7:
				lvCol.cx = 140; //厂站
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 8:
				lvCol.cx = 160; //一次设备
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			case 9:
				lvCol.cx = 100; //已确认
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=100;//列宽100象素
			}
			lvCol.pszText= colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
		//默认隐藏第3列(故障类型)
		m_List.SetColumnHide(2, TRUE);
		//默认隐藏"确认"
		m_List.SetColumnHide(9, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}



/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表控件,包括初始化列表样式和填充列表数据
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
int CManualFaultShorten::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	//设置列表风格
	InitListStyle();
	/*
	//从数据库读取数据
	if(LoadDataFromDB(m_nMaxRow, m_bShowAcked))
	{
		//读取数据成功,填充列表
		FillListData();
	}
	else
	{
		//失败
		WriteLog("动作事件视图读取数据失败", 1);
	}
	*/
	return 0;
}


/*************************************************************
 函 数 名：InitDeviceTree()
 功能概要：创建设备树, 填充设备树
 返 回 值: 失败返回-1, 其它返回>=0
**************************************************************/
int CManualFaultShorten::InitDeviceTree()
{
	FillTree();
	return 0;
}


BOOL CManualFaultShorten::FillTree()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;
	
	//清空
	m_Tree.DeleteAllItems();
	
    POSITION pos;
    HTREEITEM hRoot;
	if (g_role != MODE_SUB)
	{
		//根节点
		hRoot = m_Tree.InsertItem(STRING_NET, 0, 0, NULL);
		//地区
		pos = pData->m_NetList.GetHeadPosition();
		while(pos != NULL)
		{
			CNetObj * pObj  = (CNetObj*)pData->m_NetList.GetNext(pos);
			if(pObj != NULL)
			{
				InsertNet(pObj);
			}
		}
	}
	else
	{
        HTREEITEM htRoot = m_Tree.GetRootItem();
        //根节点
		htRoot = m_Tree.InsertItem( StringFromID(IDS_MODEL_SUBSTATION), IMAGEINDEX_STATION, IMAGEINDEX_STATION, NULL);
	}
	//厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj * pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj != NULL && !pObj->m_IsDistanceVirtual && pObj->m_nStationType != 113 && pObj->m_nStationType != 114 )
		{
			if(g_DeviceTreeType == 0) //只显示到站的情况
				InsertStation(pObj);
			else  //站后还显示设备
				InsertStation_Voltage(pObj);
		}
	}
	
	//清除空节点
	if(g_DeviceTreeType == 1)
		ClearEmptyNodeType1();
	else if(g_DeviceTreeType == 2)
		ClearEmptyNodeType2();
	return TRUE;
}


/*************************************************************
 函 数 名：InsertNet()
 功能概要：插入地区节点
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1   要插入的地区对象
**************************************************************/
BOOL CManualFaultShorten::InsertNet(CNetObj* pObj)
{
	//根节点为"地区"
	//地区节点在第二层
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL)
		return FALSE;

	//插入节点
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, IMAGEINDEX_NETAREA, IMAGEINDEX_NETAREA, htRoot);
	//设置数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//排序
	//m_Tree.SortChildren(htRoot);
	return TRUE;
}


/*************************************************************
 函 数 名：InsertStation()
 功能概要：插入厂站
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1  要插入的厂站对象
**************************************************************/
BOOL CManualFaultShorten::InsertStation(CStationObj* pObj)
{
    HTREEITEM htNew;
    HTREEITEM htNet;
	if (g_role != MODE_SUB)
	{
		//厂站节点在地区节点下面, 为第三层
		//先查找地区节点
		htNet = FindNetPos(pObj->m_sNetID);
		if(htNet == NULL)
			return FALSE;
		
		//插入节点
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htNet);
	} 
	else
	{
        HTREEITEM htRoot = m_Tree.GetRootItem();
        //根节点
		htNew= m_Tree.InsertItem(pObj->m_sName, 0, 0, htRoot);
	}
        //记录节点
		pObj->m_Tag = htNew;
		//设备数据
		m_Tree.SetItemData(htNew, (DWORD)pObj);
		//排序
		m_Tree.SortChildren(htNet);
		
		//加入间隔
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine* pData = pApp->GetDataEngine();
		if(pData == NULL)
			return FALSE;
		DEVICE_LIST listDevice;
		listDevice.RemoveAll();
		pData->GetPriDeviceList(listDevice, TYPE_BAY, pObj->m_sID);
		POSITION pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CBayObj* pBay = (CBayObj*)listDevice.GetNext(pos);
			InsertDevice(htNew, pBay);
		}
	
	return TRUE;
}



/*************************************************************
 函 数 名: InsertStation_Voltage()
 功能概要: 插入厂站节点，厂站下接电压等级
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数: param1 要插入的厂站对象
		   Param2 
**************************************************************/
BOOL CManualFaultShorten::InsertStation_Voltage( CStationObj* pObj )
{
    HTREEITEM htNew;
	HTREEITEM htNet;
	if (g_role != MODE_SUB)
	{	
		//厂站节点在地区节点下面, 为第三层
		//先查找地区节点
		htNet = FindNetPos(pObj->m_sNetID);
		if(htNet == NULL)
			return FALSE;
		
		//插入节点
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htNet);
	    //排序
		m_Tree.SortChildren(htNet);
	}
	else
	{
		HTREEITEM htRoot = m_Tree.GetRootItem();
        //厂站节点
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htRoot);
	}

	//记录节点
	pObj->m_Tag = htNew;
	//设备数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);	

	if(g_DeviceTreeType == 4)
	{
		//二次设备直接挂在厂站下
		BuildSecToStation(pObj);
	}
	else
    {
		//对电压等级排序
		SortVoltage(pObj);
		
		//添加电压等级层
		for(int i = 0; i < pObj->m_arrVoltage.GetSize(); i++)
		{
			int nVoltage = pObj->m_arrVoltage.GetAt(i);
			InsertVoltage(nVoltage, htNew, pObj);
		}
		
		//添加保护
		BuildProtect(pObj);
		
		//添加录波器
		HTREEITEM htWaveRec = m_Tree.InsertItem(StringFromID(IDS_MODEL_RECORDER), IMAGEINDEX_WAVEREC, IMAGEINDEX_WAVEREC, htNew);
		BuildWaveRec(pObj, htWaveRec);
		m_Tree.SortChildren(htWaveRec);
		
		
		HTREEITEM htGPSRec = m_Tree.InsertItem(StringFromID(IDS_MODEL_GPS), IMAGEINDEX_SWITCH, IMAGEINDEX_SWITCH, htNew);
		BuildGPS(pObj, htGPSRec);
		m_Tree.SortChildren(htGPSRec);
	}
		
		return TRUE;
}


/*************************************************************
 函 数 名: InsertDevice()
 功能概要: 加入设备到设备树
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数: param1 指定父节点
		   Param2 指定设备
**************************************************************/
BOOL CManualFaultShorten::InsertDevice( HTREEITEM htParent, CDeviceObj* pObj)
{
	//检查数据有效性
	if(htParent == NULL)
		return FALSE;
	if(pObj == NULL)
		return FALSE;

	//确定图形索引
	/*
	int nImageIndex = IMAGEINDEX_STATION;
	if(pObj->m_nType == TYPE_BREAK)
		nImageIndex = IMAGEINDEX_BREAK;
	else if(pObj->m_nType == TYPE_BUS)
		nImageIndex = IMAGEINDEX_BUS;
	else if(pObj->m_nType == TYPE_CAP)
		nImageIndex = IMAGEINDEX_CAP;
	else if(pObj->m_nType == TYPE_GEN)
		nImageIndex = IMAGEINDEX_GEN;
	else if(pObj->m_nType == TYPE_LINE)
		nImageIndex = IMAGEINDEX_LINE;
	else if(pObj->m_nType == TYPE_REACTOR)
		nImageIndex = IMAGEINDEX_REACTOR;
	else if(pObj->m_nType == TYPE_SWITCH)
		nImageIndex = IMAGEINDEX_SWITCH;
	else if(pObj->m_nType == TYPE_TRANS)
		nImageIndex = IMAGEINDEX_TRANS;
	else if(pObj->m_nType == TYPE_WAVEREC)
		nImageIndex = IMAGEINDEX_WAVEREC;
		*/
	//加入
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, htParent);
	if(htNew == NULL)
		return FALSE;
	//记录节点位置
	pObj->m_Tag = htNew;
	//关联数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	return TRUE;
}

/*************************************************************
 函 数 名: ClearEmptyNodeType1()
 功能概要: 清除设备树类型为1的设备树的空节点
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CManualFaultShorten::ClearEmptyNodeType1()
{
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL) //地区硬节点
		return;
	
	HTREEITEM htLevel1 = m_Tree.GetChildItem(htRoot);
	while(htLevel1 != NULL) //地区节点
	{
		HTREEITEM htLevel2 = m_Tree.GetChildItem(htLevel1);
		while(htLevel2 != NULL) //厂站节点
		{
			HTREEITEM htLevel3 = m_Tree.GetChildItem(htLevel2);
			while(htLevel3 != NULL) //电压等级节点
			{
				CString str = m_Tree.GetItemText(htLevel3);
				if(str != StringFromID(IDS_MODEL_RECORDER))
				{ 
					//非录波器层
					HTREEITEM htLevel4 = m_Tree.GetChildItem(htLevel3); 
					while(htLevel4 != NULL)//一次设备硬节点
					{
						HTREEITEM htTemp = htLevel4;
						htLevel4 = m_Tree.GetNextItem(htLevel4, TVGN_NEXT);
						if(!m_Tree.ItemHasChildren(htTemp))
						{
							//删除本节点
							m_Tree.DeleteItem(htTemp);
						}
					}
				}
				else
				{
					//录波器层
					HTREEITEM htTemp = htLevel3;
					htLevel3 = m_Tree.GetNextItem(htLevel3, TVGN_NEXT);
					if(!m_Tree.ItemHasChildren(htTemp))
					{
						//删除本节点
						m_Tree.DeleteItem(htTemp);
					}
				}
				htLevel3 = m_Tree.GetNextItem(htLevel3, TVGN_NEXT);
			}
			htLevel2 = m_Tree.GetNextItem(htLevel2, TVGN_NEXT);
		}
		htLevel1 = m_Tree.GetNextItem(htLevel1, TVGN_NEXT);
	}
}

/*************************************************************
 函 数 名: ClearEmptyNodeType2()
 功能概要: 清除设备树类型为2的设备树的空节点
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CManualFaultShorten::ClearEmptyNodeType2()
{
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL) //地区硬节点
		return;
	
	HTREEITEM htLevel1 = m_Tree.GetChildItem(htRoot);
	while(htLevel1 != NULL) //地区节点
	{
		HTREEITEM htLevel2 = m_Tree.GetChildItem(htLevel1);
		while(htLevel2 != NULL) //厂站节点
		{
			HTREEITEM htLevel3 = m_Tree.GetChildItem(htLevel2);
			while(htLevel3 != NULL) //电压等级节点
			{
				CString str = m_Tree.GetItemText(htLevel3);
				if(str != StringFromID(IDS_MODEL_RECORDER))
				{ 
					//非录波器层
					HTREEITEM htLevel4 = m_Tree.GetChildItem(htLevel3); 
					while(htLevel4 != NULL)//间隔节点
					{
						HTREEITEM htLevel5 = m_Tree.GetChildItem(htLevel4);
						while(htLevel5 != NULL)
						{
							HTREEITEM htTemp = htLevel5;
							htLevel5 = m_Tree.GetNextItem(htLevel5, TVGN_NEXT);
							if(!m_Tree.ItemHasChildren(htTemp))
							{
								//删除本节点
								m_Tree.DeleteItem(htTemp);
							}		
						}
						htLevel4 = m_Tree.GetNextItem(htLevel4, TVGN_NEXT);
					}
				}
				else
				{
					//录波器层
					HTREEITEM htTemp = htLevel3;
					htLevel3 = m_Tree.GetNextItem(htLevel3, TVGN_NEXT);
					if(!m_Tree.ItemHasChildren(htTemp))
					{
						//删除本节点
						m_Tree.DeleteItem(htTemp);
					}
				}
				htLevel3 = m_Tree.GetNextItem(htLevel3, TVGN_NEXT);
			}
			htLevel2 = m_Tree.GetNextItem(htLevel2, TVGN_NEXT);
		}
		htLevel1 = m_Tree.GetNextItem(htLevel1, TVGN_NEXT);
	}
}



/*************************************************************
 函 数 名: SortVoltage()
 功能概要: 对站内电压等级排序
 返 回 值: 
 参    数: param1 指定站
		   Param2 
**************************************************************/
void CManualFaultShorten::SortVoltage( CStationObj* pObj )
{
	if(pObj == NULL)
		return;
	for(int i = 0; i < pObj->m_arrVoltage.GetSize(); i++)
	{
		for(int k = 1; k < pObj->m_arrVoltage.GetSize() - i; k++)
		{
			int nV1 = pObj->m_arrVoltage.GetAt(k-1);
			int nV2 = pObj->m_arrVoltage.GetAt(k);
			if(nV1 > nV2)
			{
				pObj->m_arrVoltage.SetAt(k-1, nV2);
				pObj->m_arrVoltage.SetAt(k, nV1);
			}
		}
	}
}

/*************************************************************
 函 数 名: BuildBreak()
 功能概要: 把指定厂站指定电压等级的所有开关加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定父节点
		   Param3 指定电压等级
		   param4 指定间隔
**************************************************************/
void CManualFaultShorten::BuildBreak( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_BREAK, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	bool bDelete = false;
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
	
		if(g_DeviceTreeType == 2)
		{
			if(pObj->m_sBayID == sBayID)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
		else
		{
			if(pObj->m_nVoltage == nVoltage)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
	}

	if (!bDelete)
	{
		m_Tree.DeleteItem(htParent);
	}
}

/*************************************************************
 函 数 名: BuildBus()
 功能概要: 把指定厂站指定电压等级的所有母线加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定父节点
		   Param3 指定电压等级
		   param4 指定间隔
**************************************************************/
void CManualFaultShorten::BuildBus( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID)
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_BUS, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	bool bDelete = false;
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
	
		if(g_DeviceTreeType == 2)
		{
			if(pObj->m_sBayID == sBayID)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
		else
		{
			if(pObj->m_nVoltage == nVoltage)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
	}

	if (!bDelete)
	{
		m_Tree.DeleteItem(htParent);
	}
}

/*************************************************************
 函 数 名: BuildCap()
 功能概要: 把指定厂站指定电压等级的所有电容器加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定父节点
		   Param3 指定电压等级
		   param4 指定间隔
**************************************************************/
void CManualFaultShorten::BuildCap( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_CAP, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	bool bDelete = false;
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
	
		if(g_DeviceTreeType == 2)
		{
			if(pObj->m_sBayID == sBayID)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
		else
		{
			if(pObj->m_nVoltage == nVoltage)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
	}

	if (!bDelete)
	{
		m_Tree.DeleteItem(htParent);
	}
}

/*************************************************************
 函 数 名: BuildGen()
 功能概要: 把指定厂站指定电压等级的所有发电机加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定父节点
		   Param3 指定电压等级
		   param4 指定间隔
**************************************************************/
void CManualFaultShorten::BuildGen( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_GEN, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	bool bDelete = false;
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
	
		if(g_DeviceTreeType == 2)
		{
			if(pObj->m_sBayID == sBayID)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
		else
		{
			if(pObj->m_nVoltage == nVoltage)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
	}

	if (!bDelete)
	{
		m_Tree.DeleteItem(htParent);
	}
}

/*************************************************************
 函 数 名: BuildGen()
 功能概要: 把指定厂站指定电压等级的所有发电机加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定父节点
		   Param3 指定电压等级
		   param4 指定间隔
**************************************************************/
void CManualFaultShorten::BuildLine( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_LINE, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	bool bDelete = false;
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
	
		if(g_DeviceTreeType == 2)
		{
			if(pObj->m_sBayID == sBayID)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
		else
		{
			if(pObj->m_nVoltage == nVoltage)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
	}

	if (!bDelete)
	{
		m_Tree.DeleteItem(htParent);
	}
}

/*************************************************************
 函 数 名: BuildReactor()
 功能概要: 把指定厂站指定电压等级的所有电抗器加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定父节点
		   Param3 指定电压等级
		   param4 指定间隔
**************************************************************/
void CManualFaultShorten::BuildReactor( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_REACTOR, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	bool bDelete = false;
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		
		if(g_DeviceTreeType == 2)
		{
			if(pObj->m_sBayID == sBayID)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
		else
		{
			if(pObj->m_nVoltage == nVoltage)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
	}

	if (!bDelete)
	{
		m_Tree.DeleteItem(htParent);
	}
}

/*************************************************************
 函 数 名: BuildSwitch()
 功能概要: 把指定厂站指定电压等级的所有刀闸加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定父节点
		   Param3 指定电压等级
		   param4 指定间隔
**************************************************************/
void CManualFaultShorten::BuildSwitch( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_SWITCH, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	bool bDelete = false;
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
	
		if(g_DeviceTreeType == 2)
		{
			if(pObj->m_sBayID == sBayID)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
		else
		{
			if(pObj->m_nVoltage == nVoltage)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
	}

	if (!bDelete)
	{
		m_Tree.DeleteItem(htParent);
	}
}

/*************************************************************
 函 数 名: BuildTrans()
 功能概要: 把指定厂站指定电压等级的所有变压器加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定父节点
		   Param3 指定电压等级
		   param4 指定间隔
**************************************************************/
void CManualFaultShorten::BuildTrans( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_TRANS, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	bool bDelete = false;
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		CTransObj* pTrans = (CTransObj*)pObj;
	
		if(g_DeviceTreeType == 2)
		{
			if(pObj->m_sBayID == sBayID)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
		else
		{
			if(pTrans->m_nVoltageH == nVoltage)
			{
				InsertDevice(htParent, pObj);
				bDelete = true;
			}
		}
	}

	if (!bDelete)
	{
		m_Tree.DeleteItem(htParent);
	}
}

/*************************************************************
 函 数 名: BuildProtect()
 功能概要: 把指定厂站的所有保护加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 
**************************************************************/
void CManualFaultShorten::BuildProtect( CStationObj* pStation)
{
	if(pStation == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_PROTECT, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CString key;
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		InsertProtect(pObj);
	}
}

/*************************************************************
 函 数 名: InsertProtect()
 功能概要: 加入保护到设备树
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数: param1 指定保护
		   Param2 
**************************************************************/
BOOL CManualFaultShorten::InsertProtect( CSecObj* pObj)
{
	//检查数据有效性
	if(pObj == NULL)
		return FALSE;
	if(pObj->m_nType != TYPE_PROTECT)
		return FALSE;
	if(pObj->m_pOwner == NULL)
		return FALSE;
	if(pObj->m_pOwner->m_Tag == NULL)
		return FALSE;
	//加入到设备树
	CString str;
	str = pObj->m_sName;
	HTREEITEM htNew = m_Tree.InsertItem(str, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, pObj->m_pOwner->m_Tag);
	if(htNew == NULL)
		return FALSE;
	
	//记录位置
	pObj->m_Tag = htNew;
	m_Tree.SortChildren(pObj->m_pOwner->m_Tag);

	//设置数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);	
	
	return TRUE;
}

/*************************************************************
 函 数 名: BuildWaveRec()
 功能概要: 把指定厂站的所有录波器加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定父节点
**************************************************************/
void CManualFaultShorten::BuildWaveRec( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_WAVEREC, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		InsertDevice(htParent, pObj);
	}
}


void CManualFaultShorten::BuildGPS( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if (pObj != NULL && pObj->m_nSecType == 15)
		{
			InsertDevice(htParent, pObj);
		}
	}
}


/*************************************************************
 函 数 名: InsertFastness()
 功能概要: 添加硬节点
 返 回 值: 
 参    数: param1 指定父节点
		   Param2 指定厂站
		   param3 指定电压等级
		   param4 指定间隔ID
**************************************************************/
void CManualFaultShorten::InsertFastness( HTREEITEM htParent, CStationObj* pObj, int nVoltage, CString sBayID )
{
	//加入各一次设备类型
	HTREEITEM htBus		= m_Tree.InsertItem( StringFromID(IDS_MODEL_BUS), IMAGEINDEX_BUS, IMAGEINDEX_BUS, htParent);
	HTREEITEM htLine	= m_Tree.InsertItem( StringFromID(IDS_MODEL_LINE), IMAGEINDEX_LINE, IMAGEINDEX_LINE, htParent);
	HTREEITEM htGen		= m_Tree.InsertItem( StringFromID(IDS_MODEL_GEN), IMAGEINDEX_GEN, IMAGEINDEX_GEN, htParent);
	HTREEITEM htTrans	= m_Tree.InsertItem( StringFromID(IDS_MODEL_TRANS), IMAGEINDEX_TRANS, IMAGEINDEX_TRANS, htParent);
	HTREEITEM htCap		= m_Tree.InsertItem( StringFromID(IDS_MODEL_CAP), IMAGEINDEX_CAP, IMAGEINDEX_CAP, htParent);
	HTREEITEM htReactor = m_Tree.InsertItem( StringFromID(IDS_MODEL_REACTOR), IMAGEINDEX_REACTOR, IMAGEINDEX_REACTOR, htParent);
	HTREEITEM htSwitch	= m_Tree.InsertItem( StringFromID(IDS_MODEL_SWITCH), IMAGEINDEX_SWITCH, IMAGEINDEX_SWITCH, htParent);
	HTREEITEM htBreak	= m_Tree.InsertItem( StringFromID(IDS_MODEL_BREAKER), IMAGEINDEX_BREAK, IMAGEINDEX_BREAK, htParent);
	
	//change
	//加入各一次设备
	//断路器
	BuildBreak(pObj, htBreak, nVoltage, sBayID);
	m_Tree.SortChildren(htBreak);
	//母线
	BuildBus(pObj, htBus, nVoltage, sBayID);
	m_Tree.SortChildren(htBus);
	//电容器
	BuildCap(pObj, htCap, nVoltage, sBayID);
	m_Tree.SortChildren(htCap);
	//发电机
	BuildGen(pObj, htGen, nVoltage, sBayID);
	m_Tree.SortChildren(htGen);
	//线路
	BuildLine(pObj, htLine, nVoltage, sBayID);
	m_Tree.SortChildren(htLine);
	//电抗器
	BuildReactor(pObj, htReactor, nVoltage, sBayID);
	m_Tree.SortChildren(htReactor);
	//刀闸
	BuildSwitch(pObj, htSwitch, nVoltage, sBayID);
	m_Tree.SortChildren(htSwitch);
	//变压器
	BuildTrans(pObj, htTrans, nVoltage, sBayID);
	m_Tree.SortChildren(htTrans);
}

/*************************************************************
 函 数 名: BuildBay()
 功能概要: 把指定厂站指定电压等级的所有间隔加入到设备树
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定父节点
		   param3 指定电压等级
**************************************************************/
void CManualFaultShorten::BuildBay( CStationObj* pStation, HTREEITEM htParent, int nVoltage )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_BAY, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CBayObj* pBay = (CBayObj*)listDevice.GetNext(pos);
		if(pBay == NULL)
			continue;
		if(pBay->m_nVoltage == nVoltage)
			InsertBay(htParent, pStation, pBay);
	}
}

/*************************************************************
 函 数 名: InsertBay()
 功能概要: 插入间隔到设备树
 返 回 值: 
 参    数: param1 指定父节点
		   Param2 指定厂站
		   Param3 指定间隔
**************************************************************/
void CManualFaultShorten::InsertBay( HTREEITEM htParent, CStationObj* pObj, CBayObj* pBay)
{
	//检查数据有效性
	if(htParent == NULL)
		return;
	if(pObj == NULL)
		return;
	if(pBay == NULL)
		return;
	
	//确定图形索引
	int nImageIndex = IMAGEINDEX_STATION;
	//加入
	HTREEITEM htNew = m_Tree.InsertItem(pBay->m_sName, nImageIndex, nImageIndex, htParent);
	if(htNew == NULL)
		return;
	//记录节点位置
	pBay->m_Tag = htNew;
	//关联数据
	m_Tree.SetItemData(htNew, (DWORD)pBay);

	//插入硬节点
	InsertFastness(htNew, pObj, 0, pBay->m_sID);
}


/*************************************************************
 函 数 名：FindNetPos
 功能概要：查找指定地区在树中的位置
 返 回 值: 树节点结构
 参    数：param1   指定地区ID
**************************************************************/
HTREEITEM CManualFaultShorten::FindNetPos(CString sNetID)
{
	HTREEITEM htReturn = NULL;
	//根节点
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL)
		return NULL;
	//第一个地区节点
	HTREEITEM htNetItem = m_Tree.GetChildItem(htRoot);

	//循环查找
	while(htNetItem != NULL)
	{
		CNetObj * pObj = (CNetObj*)m_Tree.GetItemData(htNetItem);
		if(pObj->m_sID == sNetID)
		{
			htReturn = htNetItem;
			break;
		}
		htNetItem = m_Tree.GetNextItem(htNetItem, TVGN_NEXT);
	}
	return htReturn;
}


/*************************************************************
 函 数 名: InsertVoltage()
 功能概要: 插入电压等级节点,节点下跟设备
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数: param1 指定电压等级
		   Param2 指定父节点
**************************************************************/
BOOL CManualFaultShorten::InsertVoltage( int nVoltage, HTREEITEM htParent, CStationObj* pObj )
{
	if(htParent == NULL)
		return FALSE;
	if(pObj == NULL)
		return FALSE;
	//插入节点
	CString str;
	str.Format("%d kV", nVoltage);
	HTREEITEM htNew = m_Tree.InsertItem(str, IMAGEINDEX_VOLTAGE, IMAGEINDEX_VOLTAGE, htParent);


	if(g_DeviceTreeType == 2)
	{
		//插入间隔
		BuildBay(pObj, htNew, nVoltage);
	}
	else
	{
		//插入硬节点
		InsertFastness(htNew, pObj, nVoltage, "");
	}

	return TRUE;
}

/*************************************************************
 函 数 名: ClearCheck()
 功能概要: 清除所有选择
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CManualFaultShorten::ClearCheck(HTREEITEM ht)
{
	//判断数据有效性
	if(ht == NULL)
		return;
	//取消所有选择
	m_Tree.SetCheck(ht, FALSE);
	HTREEITEM htChild = m_Tree.GetChildItem(ht);
	if(htChild != NULL)
		ClearCheck(htChild);
	HTREEITEM htNext = m_Tree.GetNextItem(ht,TVGN_NEXT);
	if(htNext != NULL)
	{
		ClearCheck(htNext);
	}
}



/*************************************************************
 函 数 名：SetCheck_XJ()
 功能概要：设置指定节点的选择状态
 返 回 值: void
 参    数：param1	指定节点
		   Param2	TRUE-选中. FALSE-取消选择
**************************************************************/
//##ModelId=49B87BB100CD
void CManualFaultShorten::SetCheck_XJ( HTREEITEM hit, BOOL bCheck )
{
	CycTreeDown(hit, bCheck);
//	CycTreeUp(hit, bCheck);
}

/*************************************************************
 函 数 名：CycTreeDown()
 功能概要：向下遍历指定节点的所有子节点, 对所有子节点做相同的选择操作
 返 回 值: void
 参    数：param1	指定节点
		   Param2	选择标志.TRUE为选中, FALSE为取消选择
**************************************************************/
//##ModelId=49B87BB100BB
void CManualFaultShorten::CycTreeDown( HTREEITEM hRoot, BOOL bMark )
{
	HTREEITEM child = m_Tree.GetChildItem(hRoot);
	AddOrRemoveSecObj(hRoot, bMark);
	while(child)
	{
		m_Tree.SetCheck(child, bMark );
		AddOrRemoveSecObj(child, bMark);
		CycTreeDown(child, bMark );
		child = m_Tree.GetNextItem(child, TVGN_NEXT);
	}

}


void CManualFaultShorten::AddOrRemoveSecObj( HTREEITEM hRoot, BOOL bMark )
{
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(hRoot);
	if (pDevice == NULL )
	{
		return;
	}

	if (bMark && pDevice->IsKindOf(RUNTIME_CLASS(CSecObj)))
	{
		m_SECMap.SetAt(pDevice->m_sID, pDevice);
	} 

	if (!bMark && pDevice->IsKindOf(RUNTIME_CLASS(CSecObj)))
	{
		m_SECMap.RemoveKey(pDevice->m_sID);
	}
}


/*************************************************************
 函 数 名：CycTreeUp()
 功能概要：向上遍历节点的父节点, 如果当前节点是父节点的最后一个被选择的子节点, 为父节点也加上选择标志
 返 回 值: void
 参    数：param1	指定节点
		   Param2
**************************************************************/
//##ModelId=49B87BB100BE
void CManualFaultShorten::CycTreeUp( HTREEITEM hCurrent, BOOL bCheck)
{
	HTREEITEM parent = m_Tree.GetParentItem(hCurrent);
	if(parent == NULL)
		return;
	
	if(bCheck)
		m_Tree.SetCheck(parent, bCheck);
	else
	{
		HTREEITEM bro = m_Tree.GetNextItem(hCurrent, TVGN_NEXT);
		BOOL bFlag = false;
		while(bro)
		{
			if(m_Tree.GetCheck(bro))
			{
				bFlag = true;
				break;
			}
			bro = m_Tree.GetNextItem(bro, TVGN_NEXT);
		}
		if(!bFlag)
		{
			bro = m_Tree.GetNextItem(hCurrent, TVGN_PREVIOUS);
			while(bro)
			{
				if(m_Tree.GetCheck(bro))
				{
					bFlag = true;
					break;
				}
				bro = m_Tree.GetNextItem(bro, TVGN_PREVIOUS);
			}
		}
		if(!bFlag)
			m_Tree.SetCheck(parent, false);
	}
	
	CycTreeUp(parent, m_Tree.GetCheck(parent));
	m_Tree.GetSelectedItem();
}


void CManualFaultShorten::OnClickDeviceTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	CPoint point;
    UINT uFlag;   //接收有关点击测试的信息的整数
    HTREEITEM hTree;
    BOOL bCheck;
    GetCursorPos(&point);    //获取屏幕鼠标坐标
    m_Tree.ScreenToClient(&point);           //转化成客户坐标
    hTree = m_Tree.HitTest(point,&uFlag);    //返回与CtreeCtrl关联的光标的当前位置和句柄
	
    if (hTree && (TVHT_ONITEMSTATEICON & uFlag))  //点中复选框
	{
		bCheck = m_Tree.GetCheck(hTree);      //获取当前复选状态
		SetCheck_XJ(hTree, !bCheck);                //设置子项复选状态
	}
	
	*pResult = 0;
}

void CManualFaultShorten::OnBtnPtosclistDown() 
{
	// TODO: Add your control notification handler code here
	if (m_SECMap.GetCount() == 0)
	{
		AfxMessageBox( StringFromID(IDS_PACK_SELRELAYS));
		return;
	}

	OnFaultCreate();

/*
	if (!QueryActionEvent())
	{
		AfxMessageBox("未查询到动作事件!");
	}
	*/

}



/*************************************************************
 函 数 名：QueryNewEventID()
 功能概要：查询新事件在数据库中的ID(因为新事件是由STTP报文解析来的,不知道在数据库中的ID)
 返 回 值: 查询成功返回TRUE,失败返回FALSE(有结果且仅有一个结果视为查询成功)
**************************************************************/
//##ModelId=49B87BA0002E
BOOL CManualFaultShorten::QueryActionEvent()
{
	if (m_SECMap.GetCount() > 10)
	{
		AfxMessageBox( StringFromID(IDS_PACK_RELAY_TOOMUCH));
		return FALSE;
	}

	BOOL bReturn = TRUE;
	
	//m_EventManager.ClearAllEvents();
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//查找最新的num条记录
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();


	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "msg", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
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
	
	Field Field9;
	pApp->m_DBEngine.SetField(sql, Field9, "faulttype", EX_STTP_DATA_TYPE_STRING);
	
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "isdebug", EX_STTP_DATA_TYPE_INT);
	
	Field Field11;
	pApp->m_DBEngine.SetField(sql, Field11, "fdata", EX_STTP_DATA_TYPE_STRING);
	
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "property", EX_STTP_DATA_TYPE_INT);
	////根据PT_ID,curtime,ms,recvtime,ms2查找id
	
	//条件
	//PT_ID
	Condition condition2;
	CString str;
	str = "PT_ID in(";

	POSITION pos = m_SECMap.GetStartPosition();
	CSecObj* pSec =  NULL;
	CString strTemp = "";
	while(pos != NULL)
	{
		m_SECMap.GetNextAssoc(pos ,strTemp, (void*&)pSec);
		if(pSec == NULL)
		{
			continue;
		}
		str += "'";
		str += pSec->m_sID;
		str += "',";
	}
	str.TrimRight(",");
	str += ")";
	
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_START);
	MYASSERT_BOOL(pTime);
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_END);
	MYASSERT_BOOL(pTime2);
	CTime tmStart;
	pTime->GetTime(tmStart);
	CTime tmEnd;
	pTime2->GetTime(tmEnd);

	//起始时间
	Condition con3;
	str.Format("curtime > '%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con3, str);
	//结束时间
	Condition con4;
	str.Format("curtime < '%s'", tmEnd.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);

	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ACTION_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CManualFaultShorten::QueryActionEvent, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		return FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CManualFaultShorten::QueryActionEvent, 读取到%d条数据", nCount);
			WriteLog(str, XJ_LOG_LV3);
			ClearEvent();
		}
		else
		{
			AfxMessageBox( StringFromID(IDS_PACK_NOEVENT));
			return FALSE;
		}
		
		
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventAction * pEvent = new CXJEventAction;
			if(pEvent->LoadFromDB(pMemset))
			{
				//加入到链表
				m_EventManager.AddEvent(pEvent);
				//解释事件
				pEvent->TranslateFData();
				//分解事件
				BreakAction(pEvent);
			}
			else
			{
				//读取数据失败, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CManualFaultShorten::QueryActionEvent, 查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		bReturn = FALSE;
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return bReturn;
}


/*************************************************************
 函 数 名：BreakAction()
 功能概要：分解动作事件的特征值和信号量为多条故障事件,分别保存
 返 回 值: void
 参    数：param1	指定动作事件
		   Param2
**************************************************************/
//##ModelId=49B87BA401A6
void CManualFaultShorten::BreakAction( CXJEventAction* pEvent )
{
	//检查数据有效性
	if(pEvent == NULL)
		return;
	//特征值
	for(int i = 0; i < pEvent->m_ActChrs.GetSize(); i++)
	{
		PT_ActionCharacter* pAC = (PT_ActionCharacter*)pEvent->m_ActChrs.GetAt(i);
		if(pAC == NULL)
		{
			continue;
		}
		//创建故障事件
		CFaultEvent* pFaultEvent = new CFaultEvent;
		pFaultEvent->m_nType = 1; //动作特征值
		pFaultEvent->m_nMainRecID = m_nMainRecID;;//故障包ID
		pFaultEvent->m_tmStart = pEvent->m_tmTime;
		pFaultEvent->m_nmsStart = pEvent->m_nms;
		pFaultEvent->m_tmReceiveTime = pEvent->m_tmTime2;
		pFaultEvent->m_nmsReceive = pEvent->m_nms2;
		pFaultEvent->m_pSec = pEvent->m_pSec;
		pFaultEvent->m_sEventContent = pAC->strValue;
		pFaultEvent->m_sEventDef.Format("%d", pAC->nID);
		pFaultEvent->m_sEventDefName = pAC->strName;
		pFaultEvent->m_sSecID = pEvent->m_sSecID;
		pFaultEvent->m_strUnit = pAC->strUnits;
		//加入到链表
		m_listChr.Add(pFaultEvent);
	}
	//信号量
	for(i = 0; i < pEvent->m_Signals.GetSize(); i++)
	{
		PT_Signal * pSgn = (PT_Signal*)pEvent->m_Signals.GetAt(i);
		if(pSgn == NULL)
			continue;
		//创建故障事件
		CFaultEvent* pFaultEvent = new CFaultEvent;
		pFaultEvent->m_nType = 2; //动作信号
		pFaultEvent->m_nMainRecID = m_nMainRecID;;//故障包ID
		pFaultEvent->m_tmStart = pEvent->m_tmTime;
		pFaultEvent->m_nmsStart = pEvent->m_nms;
		pFaultEvent->m_tmReceiveTime = pEvent->m_tmTime2;
		pFaultEvent->m_nmsReceive = pEvent->m_nms2;
		pFaultEvent->m_pSec = pEvent->m_pSec;
		pFaultEvent->m_sEventContent.Format("%d", pSgn->nValue);
		pFaultEvent->m_sEventDef.Format("%d",pSgn->nID);
		//--------------------------------------------------
		pFaultEvent->m_tmSign = StringToTimeSttp26(pSgn->strTime, pFaultEvent->m_nmsSign);
		pFaultEvent->m_sEventDefName = pSgn->strName;
		pFaultEvent->m_sSecID = pEvent->m_sSecID;
		//加入到链表
		m_listSign.Add(pFaultEvent);
	}
}


void CManualFaultShorten::OnFaultCreate() 
{
	// TODO: 新建故障包
	//1.查询数据库中最新的包ID
	m_nMainRecID = QueryLatsetID();
	if(m_nMainRecID < 0)
	{
		WriteLog("未查询到合法的ID", XJ_LOG_LV3);
		//return;
	}
	//2.新建包
	CFaultReport* pReport = CreateFaultReport(++m_nMainRecID);
	MYASSERT(pReport);
	if(!QueryActionEvent())
	{
		delete pReport;
		return;
	}

	if(m_listChr.GetSize() == 0 && m_listSign.GetSize() == 0)
	{
		if(AfxMessageBox(StringFromID(IDS_PACK_EMPTY), MB_YESNO) == IDNO)
		{
			return;
		}
	}

	SaveEvent(pReport);
	//4.保存到数据库
	if(!AddReportToDB(pReport))
	{
		//保存失败
		AfxMessageBox(StringFromID(IDS_PACK_SAVEPACKAGE_FAIL));
		delete pReport;
		ClearEvent();
		return;
	}


	if (MultiSaveEvent() != 0)
	{
		//保存失败
		AfxMessageBox( StringFromID(IDS_PACK_SAVEEVENT_FAIL));
		delete pReport;
		ClearEvent();
		return;
	}
	
	//3.添加到链表和LIST显示
	pView->m_listFaultReport.AddTail(pReport);
	pView->AddReportToList(pReport, 0); //第一行*/
	FillListData();
	AfxMessageBox( StringFromID(IDS_PACK_SUCCESS));
	pView->OnFaultRefresh() ;
}


/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 函 数 名: QueryLatsetID()
 功能概要: 查询最新的故障包ID
 返 回 值: 最新的故障包ID
 参    数: param1 
		   Param2 
**************************************************************/
int CManualFaultShorten::QueryLatsetID()
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
		WriteLog("CManualFaultShorten::QueryLatsetID, 查询失败", XJ_LOG_LV3);
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
		str.Format("CManualFaultShorten::QueryLatsetID,查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
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
CFaultReport* CManualFaultShorten::CreateFaultReport(int nID)
{
	CFaultReport* pReport = new CFaultReport;
	pReport->m_nID = nID;
	CTime t = CTime::GetCurrentTime();
	CString strName;
	strName.Format("%s%s", StringFromID(IDS_PACK_NAME_PREFIX),t.Format("%Y%m%d%H%M%S"));
	pReport->m_sName = strName;
	pReport->m_tmCreateTime = t;
	pReport->m_tmFaultTime = t;

	return pReport;
}


/*************************************************************
 函 数 名：SaveEvent()
 功能概要：保存被选择的事件到故障包
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA401D9
void CManualFaultShorten::SaveEvent(CFaultReport* pReport)
{
	//检查数据有效性

	MYASSERT(pReport);
	//动作特征值
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		//得到事件
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//被选择的加入到父窗口的对应链表中
		CFaultEvent* pNew = pEvent->Clone();
		pReport->m_listChr.Add(pNew);
	}
	//动作信号
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		//得到事件
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//被选择的加入到父窗口的对应链表中
		CFaultEvent* pNew = pEvent->Clone();
		pReport->m_listSign.Add(pNew);
	}
	/*
	//告警
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		//得到事件
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//被选择的加入到父窗口的对应链表中
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			pParent->m_listAlarm.Add(pNew);
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//录波
	for(i = 0; i < m_listOSC.GetSize(); i++)
	{
		//得到事件
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//被选择的加入到父窗口的对应链表中
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			if(pNew->m_pSec == NULL)
				continue;
			if(pNew->m_pSec->m_nType == TYPE_PROTECT)
			{
				//保护
				pParent->m_listPTOSC.Add(pNew);
			}
			else if(pNew->m_pSec->m_nType == TYPE_WAVEREC)
			{
				//录波器
				pParent->m_listWROSC.Add(pNew);
			}
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//刷新父窗口列表框
	pParent->UpdateList();
	*/
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 函 数 名: AddReportToDB()
 功能概要: 将故障包加入到数据库,事件不处理
 返 回 值: 加入成功返回TRUE,失败返回FALSE
 参    数: param1 指定故障包
		   Param2 
**************************************************************/
BOOL CManualFaultShorten::AddReportToDB( CFaultReport* pReport )
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
		WriteLog("CManualFaultShorten::AddReportToDB, 失败", XJ_LOG_LV3);
		delete[] sError;
		return FALSE;
	}
	
	if(nResult != 1)
	{
		CString str;
		str.Format("CManualFaultShorten::AddReportToDB,失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}


/*************************************************************
 CreateDate: 13:1:2010     Author:LYH
 函 数 名: MultiSaveEvent()
 功能概要: 批量保存事件
 返 回 值: 保存成功0,删除数据失败返回1,保存数据失败返回2
 参    数: param1 
		   Param2 
**************************************************************/
int CManualFaultShorten::MultiSaveEvent()
{
	/*
	if(!DeleteAllEventFromDB())
	{
		//删除失败
		WriteLog("CManualFaultShorten::MultiSaveEvent, 删除旧数据失败");
		return 1;
	}
	*/
	if(m_listChr.GetSize() == 0 && m_listSign.GetSize() == 0)
		return 0;
	//循环所有选择项
	STTP_FULL_DATA sttpData;
	//清空结构
	zero_sttp_full_data(sttpData);
	int i = 0;
	//动作特征值
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//动作信号量
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return FALSE;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	/*
	//告警信号量
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//保护录波
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//录波器录波
	for(i = 0; i < m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWROSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//录波列表
	for(i = 0; i < m_listOSCList.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCList.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//保存
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	*/

	STTP_DBDATA_RECORD sttpDB;
	sttpDB.condition_list.clear();
	sttpDB.field_list.clear();

	//MainRecID
	STTP_DBDATA_UNIT field;
	CString str;
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "MainRecID";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//保护ID
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "PTID";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//EventType
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "EventType";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//FaultStartTime
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "FaultStartTime";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//FaultReceiveTime
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "FaultReceiveTime";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//EventDef
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "EventDef";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//EventContent
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "EventContent";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);
	
	//SignalTime
	field.nDataType = EX_STTP_DATA_TYPE_STRING;
	str = "SignalTime";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);

	//IsKeyEvent
	field.nDataType = EX_STTP_DATA_TYPE_INT;
	str = "IsKeyEvent";
	field.strValue = str;
	field.nDataLength = str.GetLength();
	sttpDB.field_list.push_back(field);

	sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nReturn = 0;
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJInsertMutiData(EX_STTP_INFO_FAULTSHORTENEVENT_CFG, sttpData, sError);
	}
	catch (...)
	{
		WriteLog("CManualFaultShorten::MultiSaveEvent, 保存失败", XJ_LOG_LV3);
		delete[] sError;
		return 2;
	}
	
	if(nResult == 1)
	{
		//成功
		nReturn = 0;
	}
	else
		nReturn = 2;

	zero_sttp_full_data(sttpData);
	
	delete[] sError;
	if(nReturn == 0) //成功
	{
		WriteLog("CManualFaultShorten::MultiSaveEvent, 保存成功", XJ_LOG_LV3);
	}
	else
	{
		WriteLog("CManualFaultShorten::MultiSaveEvent, 保存失败", XJ_LOG_LV3);
	}
	return nReturn;	
}

/*************************************************************
 函 数 名：ClearEvent()
 功能概要：删除所有事件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40196
void CManualFaultShorten::ClearEvent()
{
	//清空各链表
	//动作特征量
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent != NULL)
			delete pEvent;
	}
	m_listChr.RemoveAll();
	//动作信号量
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
			delete pEvent;
	}
	m_listSign.RemoveAll();

	m_EventManager.ClearAllEvents();

}


/*************************************************************
 函 数 名：FillListData()
 功能概要：填充列表数据
 返 回 值: 失败返回-1, 成功返回>=0
**************************************************************/
int CManualFaultShorten::FillListData()
{
	//用m_EventManager中的数据填充列表
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
	m_List.DeleteAllItems();
	CXJEventAction * pEvent = NULL;
	EVENT_LIST * pList = m_EventManager.GetEventList();
	if(pList != NULL)
	{
		for(int i = 0; i < pList->GetSize(); i++)
		{
			pEvent = (CXJEventAction*)pList->GetAt(i);
			AddEventToList(pEvent, i);
		}
	}

	//恢复刷新
	m_List.SetRedraw(TRUE);
	return 0;
}


/*************************************************************
 函 数 名：AddEventToList()
 功能概要：加入事件信息到列表
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数：param1   要加入的事件指针
           Param2   要加入的位置,从0开始计数
**************************************************************/
BOOL CManualFaultShorten::AddEventToList(CXJEventAction * pEvent, int i)
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
	//2.故障类型
	m_List.SetItemText(i, 2, pEvent->GetFaultType());
	//3:保护名称
	CString str;
	str = pEvent->m_pSec->m_sName;
	m_List.SetItemText(i, 3, str);
	//4:装置启动时间
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 4, sTime);
	//5:信息接收时间
	sTime.Format("%s.%03d", pEvent->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms2);
	m_List.SetItemText(i, 5, sTime);
	//6:消息内容
	m_List.SetItemText(i, 6, pEvent->m_sMsg);
	//7:厂站名
	m_List.SetItemText(i, 7, pEvent->m_sStationName);
	//8:一次设备名
	m_List.SetItemText(i, 8, pEvent->m_sDeviceName);
	//9:已确认
	m_List.SetItemText(i, 9, (pEvent->m_nIsAck==1)? StringFromID(IDS_ACK_YES):"");
	//设置数据s
	m_List.SetItemData(i, (LPARAM)(pEvent));
	return TRUE;
}

void CManualFaultShorten::BuildSecToStation( CStationObj* pStation )
{
	if(pStation == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CString key;
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		InsertSecToStation(pObj);
	}
}

BOOL CManualFaultShorten::InsertSecToStation( CSecObj* pObj )
{
	//检查数据有效性
	if(pObj == NULL)
		return FALSE;
	if(pObj->m_pStation == NULL)
		return FALSE;
	if(pObj->m_pStation->m_Tag == NULL)
		return FALSE;
	//加入到设备树
	CString str;
	str = pObj->m_sName;
	HTREEITEM htNew = m_Tree.InsertItem(str, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, pObj->m_pStation->m_Tag);
	if(htNew == NULL)
		return FALSE;
	
	//记录位置
	pObj->m_Tag = htNew;
	m_Tree.SortChildren(pObj->m_pStation->m_Tag);
	
	//设置数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);	
	
	return TRUE;
}
