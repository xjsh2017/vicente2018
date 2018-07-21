// BatchSelectView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "BatchSelectView.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CBatchSelectView

IMPLEMENT_DYNCREATE(CBatchSelectView, CFormView)

//##ModelId=49B87BB0038C
CBatchSelectView::CBatchSelectView()
	: CFormView(CBatchSelectView::IDD)
{
	//{{AFX_DATA_INIT(CBatchSelectView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bZone = TRUE;
	m_bAnalog = TRUE;
	m_bDigital = TRUE;
	m_bSetting = TRUE;
	m_bSoftBoard = TRUE;
	m_hRoot = NULL;
	m_nViewType = -1;
	m_pActiveStation = NULL;
}

//##ModelId=49B87BB100EA
CBatchSelectView::~CBatchSelectView()
{
	TRACE("CBatchSelectView::~CBatchSelectView \n");
}

//##ModelId=49B87BB100DC
void CBatchSelectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchSelectView)
	DDX_Control(pDX, IDC_TREE_BATCH_SELECT, m_Tree);
	DDX_Control(pDX, IDC_LIST_BATCH_SELECT, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatchSelectView, CFormView)
	//{{AFX_MSG_MAP(CBatchSelectView)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_TREE_BATCH_SELECT, OnClickTreeBatchSelect)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_BATCH_SELECT, OnCustomDraw)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE_BATCH_SELECT, OnCustomDrawTree)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BATCH_SELECT, OnClickListBatchSelect)
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_AUTOCALL_ACTIVE, OnAutocallActive)
	ON_COMMAND(ID_AUTOCALL_ALL, OnAutocallAll)
	//}}AFX_MSG_MAP
	ON_MESSAGE(BATCH_FRAME_OPEN, OnBatchFrameOpen)
	ON_MESSAGE(BATCH_DETAIL_UNCHECK, OnBatchDetailUncheck)
	ON_MESSAGE(BATCH_UNSELECT_ALL, OnBatchUnselectAll)
	ON_MESSAGE(BATCH_SELECT_ALL_PT, OnBatchSelectAllPT)
	ON_MESSAGE(BATCH_SELECT_ALL_WR, OnBatchSelectAllWR)
	ON_MESSAGE(BATCH_ENABLE_CONTROLS, OnBatchEnableControls)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchSelectView diagnostics

#ifdef _DEBUG
//##ModelId=49B87BB100FA
void CBatchSelectView::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87BB10109
void CBatchSelectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBatchSelectView message handlers

//##ModelId=49B87BB100DA
void CBatchSelectView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

	//m_Tree.ModifyStyle( TVS_CHECKBOXES, 0 );
	//m_Tree.ModifyStyle( 0, TVS_CHECKBOXES );
}

/*************************************************************
 函 数 名：InitList()
 功能概要：初始化召唤选项列表框
 返 回 值: 初始化成功返回>=0的值, 失败返回<0的值
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB003BC
int CBatchSelectView::InitList()
{
	//清空
	m_List.DeleteAllItems();

	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,20,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	
	char* arColumn[2]={"选项","值"};
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
	{
		lvCol.iSubItem=nCol;
		switch(nCol)
		{
		case 0://类型
			lvCol.cx = 140; //列宽50象素
			break;
		case 1://事件ID
			lvCol.cx = 90; //列宽60象素
			break;
		default:
			lvCol.cx=100;//列宽100象素
		}
		lvCol.pszText=arColumn[nCol];
		m_List.InsertColumn(nCol,&lvCol);
	}
	
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	//加入行
	if(m_List.InsertItem(LVIF_TEXT, 0, "定值区号", 0, 0, 0, 0)==-1)
		return -1;
	if(m_List.InsertItem(LVIF_TEXT, 1, "定值", 0, 0, 0, 0)==-1)
		return -1;
	if(m_List.InsertItem(LVIF_TEXT, 2, "开关量", 0, 0, 0, 0)==-1)
		return -1;
	if(m_List.InsertItem(LVIF_TEXT, 3, "软压板", 0, 0, 0, 0)==-1)
		return -1;
	if(m_List.InsertItem(LVIF_TEXT, 4, "模拟量", 0, 0, 0, 0)==-1)
		return -1;
	//设置值
	m_List.SetItemText(0, 1, (m_bZone)?"召唤":"不召唤");
	m_List.SetItemText(1, 1, (m_bSetting)?"召唤":"不召唤");
	m_List.SetItemText(2, 1, (m_bDigital)?"召唤":"不召唤");
	m_List.SetItemText(3, 1, (m_bSoftBoard)?"召唤":"不召唤");
	m_List.SetItemText(4, 1, (m_bAnalog)?"召唤":"不召唤");

	return 0;
}

/*************************************************************
 函 数 名：RegulateControlSize()
 功能概要：调整控件大小, 位置
 返 回 值: void
**************************************************************/
//##ModelId=49B87BB003C8
void CBatchSelectView::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	if(m_Tree.GetSafeHwnd() == NULL)
		return;
	
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	m_Tree.MoveWindow(rc);
	//调整列表框大小,固定150
/*	rcList.top = rcList.bottom - 150;
	m_List.MoveWindow(rcList);
	//调整设备树大小
	CRect rcTree = rc;
	rcTree.bottom = rc.bottom - 150;
	m_Tree.MoveWindow(rcTree);
	//调整列宽
	float fListWidth = (float)rcList.Width();
	fListWidth -= 5;
	float fCol1 = (float)(fListWidth * 0.6);
	float fCol2 = (float)(fListWidth * 0.4);
	m_List.SetColumnWidth(0, int(fCol1));
	m_List.SetColumnWidth(1, int(fCol2));*/
}

//##ModelId=49B87BB10119
void CBatchSelectView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	//RegulateControlSize();
}

/*************************************************************
 函 数 名：InitTree()
 功能概要：初始化设备树
 返 回 值: 初始化成功返回>=0的值, 失败返回<0的值
 参    数：param1
		   Param2
******************************r********************************/
//##ModelId=49B87BB003D8
int CBatchSelectView::InitTree()
{
	//清空
	m_Tree.DeleteAllItems();
	if (g_role != MODE_SUB)
	{
		//创建根节点
		m_hRoot = m_Tree.InsertItem(STRING_NET, NULL);
		//创建地区
		BuildNet();
	}
	else
	{
		m_hRoot = m_Tree.InsertItem(STRING_STATION, NULL);
	}

	//创建厂站
	BuildStation();
	//去除空节点
	ClearEmptyNode();
	return 0;
}

/*************************************************************
 函 数 名：BuildNet()
 功能概要：在设备树中创建地区
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10000
void CBatchSelectView::BuildNet()
{
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有地区
	POSITION pos = pData->m_NetList.GetHeadPosition();
	while(pos != NULL)
	{
		//取得地区, 加入树
		CNetObj* pObj = (CNetObj*)pData->m_NetList.GetNext(pos);
		InsertNet(pObj);
	}
}

/*************************************************************
 函 数 名：FindNetPos
 功能概要：查找指定地区在树中的位置
 返 回 值: 树节点结构
 参    数：param1   指定地区ID
**************************************************************/
//##ModelId=49B87BB10001
HTREEITEM CBatchSelectView::FindNetPos(CString sNetID)
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
 函 数 名：InsertNet()
 功能概要：插入地区节点
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1   要插入的地区对象
**************************************************************/
//##ModelId=49B87BB1000F
BOOL CBatchSelectView::InsertNet(CNetObj* pObj)
{
	//根节点为"地区"
	//地区节点在第二层
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL)
		return FALSE;

	//插入节点
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, htRoot);
	if(htNew == NULL)
		return FALSE;
	pObj->m_Tag = htNew;
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
//##ModelId=49B87BB1001F
BOOL CBatchSelectView::InsertStation(CStationObj* pObj)
{
    HTREEITEM htNew;
	if(g_role != MODE_SUB)
	{
		//厂站节点在地区节点下面, 为第三层
		//先查找地区节点
		HTREEITEM htNet = FindNetPos(pObj->m_sNetID);
		if(htNet == NULL)
			return FALSE;
		//插入节点
		htNew = m_Tree.InsertItem(pObj->m_sName, htNet);
		//排序
		m_Tree.SortChildren(htNet);
	}
	else
	{
		HTREEITEM htRoot = m_Tree.GetRootItem();
		if (htRoot == NULL)
		{
			return FALSE;
		}
		htNew = m_Tree.InsertItem(pObj->m_sName, htRoot);
	}

	//记录节点
	pObj->m_Tag = htNew;
	//设备数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//加入各一次设备类型
	HTREEITEM htBus		= m_Tree.InsertItem(STRING_BUS, htNew);
	HTREEITEM htLine	= m_Tree.InsertItem(STRING_LINE, htNew);
	HTREEITEM htGen		= m_Tree.InsertItem(STRING_GEN, htNew);
	HTREEITEM htTrans	= m_Tree.InsertItem(STRING_TRANS, htNew);
	HTREEITEM htCap		= m_Tree.InsertItem(STRING_CAP, htNew);
	HTREEITEM htReactor = m_Tree.InsertItem(STRING_REACTOR, htNew);
	HTREEITEM htSwitch	= m_Tree.InsertItem(STRING_SWITCH, htNew);
	HTREEITEM htBreak	= m_Tree.InsertItem(STRING_BREAK, htNew);
	HTREEITEM htWaveRec = m_Tree.InsertItem(STRING_WAVEREC, htNew);

	//加入各一次设备
	//母线
	BuildBus(pObj, htBus);
	m_Tree.SortChildren(htBus);
	//线路
	BuildLine(pObj, htLine);
	m_Tree.SortChildren(htLine);
	//Gen
	BuildGen(pObj, htGen);
	m_Tree.SortChildren(htGen);
	//Trans
	BuildTrans(pObj, htTrans);
	m_Tree.SortChildren(htTrans);
	//Cap
	BuildCap(pObj, htCap);
	m_Tree.SortChildren(htCap);
	//Reactor
	BuildReactor(pObj, htReactor);
	m_Tree.SortChildren(htReactor);
	//SWitch
	BuildSwitch(pObj, htSwitch);
	m_Tree.SortChildren(htSwitch);
	//Break
	BuildBreak(pObj, htBreak);
	m_Tree.SortChildren(htBreak);
	//waverec
	BuildWaveRec(pObj, htWaveRec);
	m_Tree.SortChildren(htWaveRec);
	//protect
	BuildProtect(pObj);
	return TRUE;
}

/*************************************************************
 函 数 名：BuildStation()
 功能概要：在设备树中创建厂站
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10021
void CBatchSelectView::BuildStation()
{
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		//取得地区, 加入树
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj != NULL && !pObj->m_IsDistanceVirtual && pObj->m_nStationType != 113 && pObj->m_nStationType != 114 )
			InsertStation(pObj);
	}
}

/*************************************************************
 函 数 名：InsertDevice()
 功能概要：加入设备到设备树 
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数：param1	指定父节点
		   Param2	指定设备
**************************************************************/
//##ModelId=49B87BB1002E
BOOL CBatchSelectView::InsertDevice( HTREEITEM htParent, CDeviceObj* pObj)
{
	//检查数据有效性
	if(htParent == NULL)
		return FALSE;
	if(pObj == NULL)
		return FALSE;
	//加入
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, htParent);
	if(htNew == NULL)
		return FALSE;

	pObj->m_Tag = htNew;
	//关联数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//加入CPU
	if(pObj->m_nType == TYPE_WAVEREC)
	{
		CSecObj* pSec = (CSecObj*)pObj;
		//加入所有CPU
		for(int i = 0; i < pSec->m_arrCPU.GetSize(); i++)
		{
			//取得CPU
			CSecCPU* pCpu = (CSecCPU*)pSec->m_arrCPU.GetAt(i);
			if(pCpu == NULL)
				continue;
			//加入到设备树
			InsertCPU(pCpu, htNew);
		}
		//排序
		m_Tree.SortChildren(htNew);
	}
	return TRUE;
}

/*************************************************************
 函 数 名：BuildBus()
 功能概要：把指定厂站的所有母线加入到设备树
 返 回 值: void
 参    数：param1	指定厂站
		   Param2	指定父节点位置
**************************************************************/
//##ModelId=49B87BB1003E
void CBatchSelectView::BuildBus( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_BUS, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 函 数 名：BuildLine()
 功能概要：把指定厂站的所有线路加入到设备树
 返 回 值: 
 参    数：param1	指定厂站
		   Param2	指定父节点位置
**************************************************************/
//##ModelId=49B87BB10041
void CBatchSelectView::BuildLine( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	//循环,加入到设备树
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_LINE, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 函 数 名：BuildGen()
 功能概要：把指定厂站的所发电机加入到设备树
 返 回 值: void
 参    数：param1	指定厂站
		   Param2	指定父节点
**************************************************************/
//##ModelId=49B87BB10050
void CBatchSelectView::BuildGen( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	//循环,加入到设备树
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_BUS, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 函 数 名：BuildTrans()
 功能概要：把指定厂站的所有变压器加入到设备树
 返 回 值: void
 参    数：param1	指定厂站
		   Param2	指定父节点
**************************************************************/
//##ModelId=49B87BB1005E
void CBatchSelectView::BuildTrans( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_TRANS, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 函 数 名：BuildCap()
 功能概要：把指定厂站的所有电容器加入到设备树
 返 回 值: void
 参    数：param1	指定厂站
		   Param2	指定父节点
**************************************************************/
//##ModelId=49B87BB1006D
void CBatchSelectView::BuildCap( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_CAP, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 函 数 名：BuildReactor()
 功能概要：把指定厂站的所有电抗器加入到设备树
 返 回 值: void
 参    数：param1	指定厂站
		   Param2	指定父节点
**************************************************************/
//##ModelId=49B87BB10070
void CBatchSelectView::BuildReactor( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_REACTOR, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 函 数 名：BuildBreak()
 功能概要：把指定厂站的所有开关加入到设备树
 返 回 值: void
 参    数：param1	指定厂站
		   Param2	指定父节点
**************************************************************/
//##ModelId=49B87BB1007E
void CBatchSelectView::BuildBreak( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_BREAK, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 函 数 名：BuildSwitch()
 功能概要：把指定厂站的所有刀闸加入到设备树
 返 回 值: void
 参    数：param1	指定厂站
		   Param2	指定父节点
**************************************************************/
//##ModelId=49B87BB10081
void CBatchSelectView::BuildSwitch( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_SWITCH, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 函 数 名：BuildWaveRec()
 功能概要：把指定厂站的所有录波器加入到设备树
 返 回 值: 
 参    数：param1	指定厂站
		   Param2	指定父节点
**************************************************************/
//##ModelId=49B87BB1008E
void CBatchSelectView::BuildWaveRec( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_WAVEREC, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 函 数 名：InsertProtect()
 功能概要：加入保护到设备树
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数：param1	指定保护
		   Param2
**************************************************************/
//##ModelId=49B87BB1009C
BOOL CBatchSelectView::InsertProtect( CSecObj* pObj )
{
	//检查数据有效性
	if(pObj == NULL)
		return FALSE;
	if(pObj->m_nType != TYPE_PROTECT)
		return FALSE;
	if(pObj->m_pOwner == NULL)
		return FALSE;

	HTREEITEM htTemp = NULL;

	htTemp = pObj->m_pOwner->m_Tag;
	if(htTemp == NULL)
		return FALSE;
	//加入到设备树
	CString str;
	str = pObj->m_sName;
	HTREEITEM htNew = m_Tree.InsertItem(str, htTemp);
	if(htNew == NULL)
		return FALSE;
	//记录位置
	
	pObj->m_Tag = htNew;
	m_Tree.SortChildren(pObj->m_pOwner->m_Tag);
		
	//设置数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//加入所有CPU
	for(int i = 0; i < pObj->m_arrCPU.GetSize(); i++)
	{
		//取得CPU
		CSecCPU* pCpu = (CSecCPU*)pObj->m_arrCPU.GetAt(i);
		if(pCpu == NULL)
			continue;
		//加入到设备树
		InsertCPU(pCpu, htNew);
	}
	//排序
//	m_Tree.SortChildren(htNew);

	return TRUE;
}

/*************************************************************
 函 数 名：BuildProtect()
 功能概要：把指定厂站的所有保护加入到设备树
 返 回 值: void
 参    数：param1	指定厂站
		   Param2
**************************************************************/
//##ModelId=49B87BB100A1
void CBatchSelectView::BuildProtect( CStationObj* pStation )
{
	if(pStation == NULL)
		return;
	//循环,加入到设备树
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_PROTECT, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertProtect(pObj);
	}
}

/*************************************************************
 函 数 名：ClearEmptyNode()
 功能概要：去除没有子节点的节点
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB100AC
void CBatchSelectView::ClearEmptyNode()
{
	HTREEITEM hTreeLevel1,hTreeLevel2,hTreeLevel3,hTreeLevel4,hTreeTemp;
	CString strTmp, strWave;
	strWave = "录波器";
	
	hTreeLevel1 = m_Tree.GetChildItem(m_hRoot);
	while(hTreeLevel1 != NULL)
	{
		if(m_Tree.ItemHasChildren(hTreeLevel1))
		{
			hTreeLevel2 = m_Tree.GetChildItem(hTreeLevel1);
			while(hTreeLevel2 != NULL)
			{
				if(m_Tree.ItemHasChildren(hTreeLevel2))
				{
					hTreeLevel3 = m_Tree.GetChildItem(hTreeLevel2);
					while(hTreeLevel3 != NULL)
					{
						strTmp = m_Tree.GetItemText(hTreeLevel3);
						if(m_Tree.ItemHasChildren(hTreeLevel3))
						{
							if(!strTmp.CompareNoCase(strWave))
								hTreeLevel3 = m_Tree.GetNextItem(hTreeLevel3, TVGN_NEXT);
							else{
								
								hTreeLevel4 = m_Tree.GetChildItem(hTreeLevel3);
								while(hTreeLevel4 != NULL)
								{
									
									if(m_Tree.ItemHasChildren(hTreeLevel4))
									{
										hTreeLevel4 = m_Tree.GetNextItem(hTreeLevel4, TVGN_NEXT);
									}
									else
									{
										hTreeTemp = hTreeLevel4;
										hTreeLevel4 = m_Tree.GetNextItem(hTreeLevel4, TVGN_NEXT);
										m_Tree.DeleteItem(hTreeTemp);
									}
								}
								hTreeLevel3 = m_Tree.GetNextItem(hTreeLevel3, TVGN_NEXT);
							}
						}
						else
						{
							hTreeTemp = hTreeLevel3;
							hTreeLevel3 = m_Tree.GetNextItem(hTreeLevel3, TVGN_NEXT);
							m_Tree.DeleteItem(hTreeTemp);
						}
					}
					hTreeLevel2 = m_Tree.GetNextItem(hTreeLevel2, TVGN_NEXT);
				}
				else
				{
					hTreeTemp = hTreeLevel2;
					hTreeLevel2 = m_Tree.GetNextItem(hTreeLevel2, TVGN_NEXT);
					m_Tree.DeleteItem(hTreeTemp);
				}
			}
			hTreeLevel1 = m_Tree.GetNextItem(hTreeLevel1, TVGN_NEXT);
		}
		else
		{
			hTreeTemp = hTreeLevel1;
			hTreeLevel1 = m_Tree.GetNextItem(hTreeLevel1, TVGN_NEXT);
			m_Tree.DeleteItem(hTreeTemp);
		}
	}
}

/*************************************************************
 函 数 名：CheckDevice()
 功能概要：对某设备进行了Check操作, 如果是保护或录波器, 通知详细视图
 返 回 值: void
 参    数：param1	设备指针
		   Param2	1表示选中, 0表示取消选择
**************************************************************/
//##ModelId=49B87BB100AD
void CBatchSelectView::CheckDevice( CDeviceObj* pObj, int nSelected )
{
	//检查数据有有性
	if(pObj == NULL)
		return;
	if(nSelected != 0 && nSelected != 1) //标志值无效
		return;
	if(pObj->m_nType != TYPE_CPU) //此类设备无效
		return;
	//发送消息给父窗口父窗口(因为父窗口是分隔窗口),让其转发给详细视图
	CWnd* pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//wParam参数写标志位, lParam参数写设备指针
		pWnd->PostMessage(BATCH_CHECK, (WPARAM)nSelected, (LPARAM)pObj);
	}
}

/*************************************************************
 函 数 名：CycTreeDown()
 功能概要：向下遍历指定节点的所有子节点, 对所有子节点做相同的选择操作
 返 回 值: void
 参    数：param1	指定节点
		   Param2	选择标志.TRUE为选中, FALSE为取消选择
**************************************************************/
//##ModelId=49B87BB100BB
void CBatchSelectView::CycTreeDown( HTREEITEM hRoot, BOOL bMark )
{
	if(hRoot != NULL)
	{
		//设置选择/取消选择
		m_Tree.SetCheck(hRoot, bMark);
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hRoot);
		if(pObj != NULL)
		{
			if(bMark)
				CheckDevice(pObj, 1); //选中
			else
				CheckDevice(pObj, 0); //取消选择
		}
		//继续遍历
		CycTreeDown(m_Tree.GetChildItem(hRoot), bMark);
		CycTreeDown(m_Tree.GetNextItem(hRoot, TVGN_NEXT), bMark);
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
void CBatchSelectView::CycTreeUp( HTREEITEM hCurrent )
{
	HTREEITEM hParent, hChild;
	if(hCurrent != NULL)
	{
		//取得父节点
		hParent = m_Tree.GetParentItem(hCurrent);
		if(hParent != NULL)
		{
			//遍历父节点的所有子节点, 看是否全被选中了
			hChild = m_Tree.GetChildItem(hParent);
			while(hChild != NULL)
			{
				if(hChild != hCurrent)
				{
					if(!m_Tree.GetCheck(hChild) )
						return ;
				}
				hChild = m_Tree.GetNextItem(hChild, TVGN_NEXT);
			}
			
			m_Tree.SetCheck(hParent, TRUE);
		}
		CycTreeUp(m_Tree.GetParentItem(hCurrent));
	}
}

/*************************************************************
 函 数 名：HitItemHelp()
 功能概要：处理节点被点击情况
 返 回 值: void
 参    数：param1	被点击的节点
		   Param2
**************************************************************/
//##ModelId=49B87BB100CB
void CBatchSelectView::HitItemHelp( HTREEITEM hit )
{
	HTREEITEM hChild, hParent;
	BOOL bMark;
	if(hit != NULL)
	{
		if(hit != NULL)
		{
			hChild = m_Tree.GetChildItem(hit);
			hParent = m_Tree.GetParentItem(hit);
			
		//	UpdateData(TRUE);
			if(!m_Tree.GetCheck(hit))
			{
				//被选中
				CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hit);
				if(pObj != NULL)
					CheckDevice(pObj, 1);
				bMark = TRUE;
				CycTreeDown(hChild, bMark);		//当父节点选中时,子节点全部选中
				CycTreeUp(hit);					//当子节点全部选中时,父节点选中
			}
			else
			{
				//取消选择
				CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hit);
				if(pObj != NULL)
					CheckDevice(pObj, 0);
				bMark = FALSE;
				CycTreeDown(hChild, bMark);		//当父节点取消选择时,子节点全部取消选择
				
				while(hParent != NULL)			//当子节点有一个取消选择时,父节点取消选择
				{
					m_Tree.SetCheck(hParent, FALSE);
					hParent = m_Tree.GetParentItem(hParent);
				}
			}
		}		
	}
}

/*************************************************************
 函 数 名：OnClickTreeBatchSelect()
 功能概要：响应设备树点击事件, 选择设备
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10128
void CBatchSelectView::OnClickTreeBatchSelect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	UINT uFlags = 0;
	HWND hWnd ;
	
	GetCursorPos(&pt);
	hWnd = ::WindowFromPoint(pt);
	::ScreenToClient(hWnd, &pt);

	HTREEITEM  hit = m_Tree.HitTest(pt, &uFlags);
	//判断厂站是否可操作
	if(!IsEnableItem(hit))
		return;

	if(uFlags & TVHT_ONITEMSTATEICON )
	{
		HitItemHelp(hit);
	}
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：响应列表个性化显示消息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB1012C
void CBatchSelectView::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				
				//不召唤的用红色字体显示
				//值列
				if(nSubItem == 1)
				{
					CString str = m_List.GetItemText(nItem, nSubItem);
					//断开时特殊显示
					if(str == "不召唤")
					{
						lplvcd->clrText = g_BatchNotCall;
					}
					if(str == "召唤")
					{
						lplvcd->clrText = g_BatchCall;
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

//##ModelId=49B87BB10139
void CBatchSelectView::OnClickListBatchSelect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem; //行
	int nSubItem = pNMListView->iSubItem; //列
	if(nItem >= 0)
	{
		//是有效的行
		if(nSubItem == 1)
		{
			//值列
			if(nItem == 0)
			{
				//定值区号
				m_bZone = !m_bZone; //反向
				m_List.SetItemText(nItem, nSubItem, m_bZone?"召唤":"不召唤");
				//发送消息
				CWnd* pWnd = GetParent()->GetParent();
				if(pWnd != NULL)
				{
					//wParam参数放入选项标志, lParam参数放入是否召唤标志
					pWnd->SendMessage(BATCH_OPTION, 4, (DWORD)m_bZone);
				}
			}
			if(nItem == 1)
			{
				//定值项
				m_bSetting = !m_bSetting; //反向
				m_List.SetItemText(nItem, nSubItem, m_bSetting?"召唤":"不召唤");
				//发送消息
				CWnd* pWnd = GetParent()->GetParent();
				if(pWnd != NULL)
				{
					//wParam参数放入选项标志, lParam参数放入是否召唤标志
					pWnd->SendMessage(BATCH_OPTION, 0, (DWORD)m_bSetting);
				}
			}
			if(nItem == 2)
			{
				//开关量
				m_bDigital = !m_bDigital; //反向
				m_List.SetItemText(nItem, nSubItem, m_bDigital?"召唤":"不召唤");
				//发送消息
				CWnd* pWnd = GetParent()->GetParent();
				if(pWnd != NULL)
				{
					//wParam参数放入选项标志, lParam参数放入是否召唤标志
					pWnd->SendMessage(BATCH_OPTION, 1, (DWORD)m_bDigital);
				}
			}
			if(nItem == 3)
			{
				//软压板
				m_bSoftBoard = !m_bSoftBoard; //反向
				m_List.SetItemText(nItem, nSubItem, m_bSoftBoard?"召唤":"不召唤");
				//发送消息
				CWnd* pWnd = GetParent()->GetParent();
				if(pWnd != NULL)
				{
					//wParam参数放入选项标志, lParam参数放入是否召唤标志
					pWnd->SendMessage(BATCH_OPTION, 2, (DWORD)m_bSoftBoard);
				}
			}
			if(nItem == 4)
			{
				//模拟量
				m_bAnalog = !m_bAnalog; //反向
				m_List.SetItemText(nItem, nSubItem, m_bAnalog?"召唤":"不召唤");
				//发送消息
				CWnd* pWnd = GetParent()->GetParent();
				if(pWnd != NULL)
				{
					//wParam参数放入选项标志, lParam参数放入是否召唤标志
					pWnd->SendMessage(BATCH_OPTION, 3, (DWORD)m_bAnalog);
				}
			}
		}
	}
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnBatchFrameOpen()
 功能概要：响应窗口打开消息, 刷新界面
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10148
void CBatchSelectView::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//刷新界面
	RegulateControlSize();
	m_nViewType = (int)lParam;
	//初始化
	InitList();
	InitTree();
}

/*************************************************************
 函 数 名：SetCheck_XJ()
 功能概要：设置指定节点的选择状态
 返 回 值: void
 参    数：param1	指定节点
		   Param2	TRUE-选中. FALSE-取消选择
**************************************************************/
//##ModelId=49B87BB100CD
void CBatchSelectView::SetCheck_XJ( HTREEITEM hit, BOOL bCheck )
{
	HTREEITEM hChild, hParent;

	//判断数据有效性
	if(hit == NULL)
		return;
	//设置状态
	m_Tree.SetCheck(hit, bCheck);

	hChild = m_Tree.GetChildItem(hit);
	hParent = m_Tree.GetParentItem(hit);
	
	if(bCheck)
	{
		//被选中
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hit);
		if(pObj != NULL)
			CheckDevice(pObj, 1);
		CycTreeDown(hChild, bCheck);		//当父节点选中时,子节点全部选中
		CycTreeUp(hit);					//当子节点全部选中时,父节点选中
	}
	else
	{
		//取消选择
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hit);
		if(pObj != NULL)
			CheckDevice(pObj, 0);
		CycTreeDown(hChild, bCheck);		//当父节点取消选择时,子节点全部取消选择
		
		while(hParent != NULL)			//当子节点有一个取消选择时,父节点取消选择
		{
			m_Tree.SetCheck(hParent, FALSE);
			hParent = m_Tree.GetParentItem(hParent);
		}
	}
}

/*************************************************************
 函 数 名：OnBatchDetailUncheck()
 功能概要：从详细视图取消某设备选择
 返 回 值: void
 参    数：param1
		   Param2	设备指针
**************************************************************/
//##ModelId=49B87BB1014C
void CBatchSelectView::OnBatchDetailUncheck( WPARAM wParam, LPARAM lParam )
{
	//取得数据
	CDeviceObj* pObj = (CDeviceObj*)lParam;
	if(pObj == NULL)
		return;
	int nFlag = (int)wParam;
	HTREEITEM ht = NULL;

	ht = pObj->m_Tag;
	if(ht != NULL)
	{
		if(nFlag == 0)
			SetCheck_XJ(ht, FALSE);
		else if(nFlag == 1)
			SetCheck_XJ(ht, TRUE);
	}
}

/*************************************************************
 函 数 名：OnBatchUnselectAll()
 功能概要：响应取消所有选择消息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10157
void CBatchSelectView::OnBatchUnselectAll( WPARAM wParam, LPARAM lParam )
{
	//判断数据有效性
	if(m_hRoot == NULL)
		return;
	//取消所有选择
	SetCheck_XJ(m_hRoot, FALSE);
}

/*************************************************************
 函 数 名：OnBatchSelectAllPT()
 功能概要：响应保护全选消息, 选择所有保护
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB1015B
void CBatchSelectView::OnBatchSelectAllPT( WPARAM wParam, LPARAM lParam )
{
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	pData->GetSecList(listDevice, "", TYPE_PROTECT, "");
	//循环所有厂站
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//设置选择
		HTREEITEM ht = NULL;
		ht = pObj->m_Tag;
		if(ht != NULL)
		{
			SetCheck_XJ(ht, TRUE);
		}
	}
}

/*************************************************************
 函 数 名：OnBatchSelectAllWR()
 功能概要：响应录波器全选消息, 选择所有录波器
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB1015F
void CBatchSelectView::OnBatchSelectAllWR( WPARAM wParam, LPARAM lParam )
{
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	pData->GetSecList(listDevice, "", TYPE_WAVEREC, "");
	//循环所有
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//设置选择
		HTREEITEM ht = NULL;
		ht = pObj->m_Tag;
		if(ht != NULL)
		{
			SetCheck_XJ(ht, TRUE);
		}
	}
}

/*************************************************************
 函 数 名：OnBatchEnableControls()
 功能概要：响应控件可用性改变消息, 改变控件可用性
 返 回 值: void
 参    数：param1
		   Param2	可用/不可用
**************************************************************/
//##ModelId=49B87BB10169
void CBatchSelectView::OnBatchEnableControls( WPARAM wParam, LPARAM lParam )
{
	BOOL bEnable = (BOOL)lParam;
	CWnd * pWnd = (CWnd*)GetDlgItem(IDC_LIST_BATCH_SELECT);
	if(pWnd != NULL)
		pWnd->EnableWindow(bEnable);
	pWnd = (CWnd*)GetDlgItem(IDC_TREE_BATCH_SELECT);
	if(pWnd != NULL)
		pWnd->EnableWindow(bEnable);
}

//##ModelId=49B87BB1013D
BOOL CBatchSelectView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
// 	ERASE_BKGND_BEGIN;
//     ADD_NOERASE_CONTROL(IDC_LIST_BATCH_SELECT);
//     ADD_NOERASE_CONTROL(IDC_TREE_BATCH_SELECT);
//     ERASE_BKGND_END(pDC, GetSysColor(COLOR_3DFACE));
//     return TRUE;
	return CFormView::OnEraseBkgnd(pDC);
}

/*************************************************************
 函 数 名：InsertCPU()
 功能概要：加入CPU到设备树
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数：param1	指定CPU
		   Param2	父节点位置
**************************************************************/
//##ModelId=49B87BB1009E
BOOL CBatchSelectView::InsertCPU( CSecCPU* pCpu, HTREEITEM htParent)
{
	//检查数据有效性
	if(pCpu == NULL)
		return FALSE;
	if(htParent == NULL)
		return FALSE;
	//加入到设备树
	CString str;
	str.Format("%d(%s)", pCpu->code, pCpu->des);
	HTREEITEM htNew = m_Tree.InsertItem(str, htParent);
	if(htNew == NULL)
		return FALSE;
	//记录位置
	pCpu->m_Tag = htNew;
	
	//设置数据
	m_Tree.SetItemData(htNew, (DWORD)pCpu);
	return TRUE;
}

void CBatchSelectView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	//只处理自动总召设置的情况
	if(m_nViewType != 0)
		return;
	CPoint ptClient = point;
	m_Tree.ScreenToClient(&ptClient);
	HTREEITEM hItem = m_Tree.HitTest(ptClient);
	if(hItem != NULL)
	{
		//让节点被选中
		m_Tree.SelectItem(hItem);
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hItem);
		if(pObj == NULL)
			return;
		if(pObj->m_nType == TYPE_STATION) //只处理厂站的情况
		{
			//弹出菜单
			//生成菜单
			CMenu Menu, *pMenu;
			if(!Menu.LoadMenu(IDR_MENU_AUTOCALL))
				return;
			pMenu = Menu.GetSubMenu(0);

			CStationObj* pStation = (CStationObj*)pObj;
			if(m_pActiveStation == NULL)
			{
				//所有厂站可操作
				pMenu->EnableMenuItem(ID_AUTOCALL_ACTIVE, MF_ENABLED);
			}
			else
			{
				if(m_pActiveStation == pStation)
				{
					//当前可操作厂站
					pMenu->EnableMenuItem(ID_AUTOCALL_ALL, MF_ENABLED);
				}
				else
				{
					//被选厂站不是可操作厂站
					pMenu->EnableMenuItem(ID_AUTOCALL_ACTIVE, MF_ENABLED);
					pMenu->EnableMenuItem(ID_AUTOCALL_ALL, MF_ENABLED);
				}
			}
			
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}		
	}
}

void CBatchSelectView::OnAutocallActive() 
{
	// TODO: Add your command handler code here
	//取得被选择的项
	HTREEITEM ht = m_Tree.GetSelectedItem();
	CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj->m_nType != TYPE_STATION)
		return;
	//设置可操作厂站
	m_pActiveStation = (CStationObj*)pObj;
	m_Tree.Invalidate(FALSE);
	//取消所有选择
	ClearCheck(m_hRoot);

	//展开
	m_Tree.Expand(ht, TVE_EXPAND);
	//发送消息
	CWnd* pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//wParam参数放入选项标志, lParam参数放入是否召唤标志
		pWnd->SendMessage(AUTOCALL_STATION_CHANGE, 0, (LPARAM)m_pActiveStation);
	}
}

void CBatchSelectView::OnAutocallAll() 
{
	// TODO: Add your command handler code here
	//设置所有厂站可操作
	m_pActiveStation = NULL;
	//刷新树
	m_Tree.Invalidate(FALSE);
	//取消所有选择
	ClearCheck(m_hRoot);
	//发送消息
	CWnd* pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//wParam参数放入选项标志, lParam参数放入是否召唤标志
		pWnd->SendMessage(AUTOCALL_STATION_CHANGE, 0, (LPARAM)m_pActiveStation);
	}
}

void CBatchSelectView::OnCustomDrawTree( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(pNMHDR ->code == NM_CUSTOMDRAW)
	{
		LPNMTVCUSTOMDRAW  lplvcd = (LPNMTVCUSTOMDRAW)pNMHDR;
		switch(lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			{
				*pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
				break;
			}
		case CDDS_ITEMPREPAINT:
			{
				HTREEITEM ht = (HTREEITEM)lplvcd->nmcd.dwItemSpec;
				//先给默认值, 后面再特殊处理
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
				if(pObj != NULL && m_pActiveStation != NULL)
				{
					if(pObj->m_nType == TYPE_STATION) //厂站
					{
						if(pObj->m_sID == m_pActiveStation->m_sID)
						{
							lplvcd->clrText = RGB(255, 0, 0);
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
 函 数 名: IsEnableItem()
 功能概要: 判断指定节点是否可操作
 返 回 值: 可操作返回TRUE,不可操作返回FALSE
 参    数: param1 指定节点
		   Param2 
**************************************************************/
BOOL CBatchSelectView::IsEnableItem( HTREEITEM ht )
{
	if(m_nViewType == 1)
	{
		//批量召唤设置
		return TRUE;
	}
	if(m_pActiveStation == NULL)
		return TRUE;

	//自动总召且设置了可操作厂站时有效
	CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
	{
		//可能是硬编码,判断父节点
		HTREEITEM htParent = m_Tree.GetParentItem(ht);
		//得到父节点
		CDeviceObj* pParent = (CDeviceObj*)m_Tree.GetItemData(htParent);
		if(pParent == NULL)
			return FALSE;
		if(pParent->m_nType == TYPE_STATION)
		{
			//厂站
			if(pParent->m_sID == m_pActiveStation->m_sID)
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			//非厂站
			if(pParent->m_sStationID == m_pActiveStation->m_sID)
				return TRUE;
			else
				return FALSE;
		}
	}
	else
	{
		if(pObj->m_nType == TYPE_STATION)
		{
			//厂站
			if(pObj->m_sID == m_pActiveStation->m_sID)
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			//非厂站
			if(pObj->m_sStationID == m_pActiveStation->m_sID)
				return TRUE;
			else
				return FALSE;
		}
	}
	return FALSE;
}

/*************************************************************
 函 数 名: ClearCheck()
 功能概要: 清除所有选择
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CBatchSelectView::ClearCheck(HTREEITEM ht)
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


