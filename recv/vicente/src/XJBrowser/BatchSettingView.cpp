// BatchSettingView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "BatchSettingView.h"
#include "GlobalFunc.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CBatchSettingView

IMPLEMENT_DYNCREATE(CBatchSettingView, CFormView)

//##ModelId=49B87BB0038C
CBatchSettingView::CBatchSettingView()
	: CFormView(CBatchSettingView::IDD)
{
	//{{AFX_DATA_INIT(CBatchSettingView)
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
CBatchSettingView::~CBatchSettingView()
{
	TRACE("CBatchSettingView::~CBatchSettingView \n");
}

//##ModelId=49B87BB100DC
void CBatchSettingView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchSettingView)
    DDX_Control(pDX, IDC_TREE, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatchSettingView, CFormView)
	//{{AFX_MSG_MAP(CBatchSettingView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_AUTOCALL_ACTIVE, OnAutocallActive)
	ON_COMMAND(ID_AUTOCALL_ALL, OnAutocallAll)
	ON_COMMAND(IDC_DEVICE_LIST, ShowDeviceList)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(BATCH_FRAME_OPEN, OnBatchFrameOpen)
	ON_MESSAGE(BATCH_DETAIL_UNCHECK, OnBatchDetailUncheck)
	ON_MESSAGE(BATCH_UNSELECT_ALL, OnBatchUnselectAll)
	ON_MESSAGE(BATCH_SELECT_ALL_PT, OnBatchSelectAllPT)
	ON_MESSAGE(BATCH_SELECT_ALL_WR, OnBatchSelectAllWR)
	ON_MESSAGE(BATCH_ENABLE_CONTROLS, OnBatchEnableControls)
    ON_MESSAGE(ITEM_SELECT, OnClickTree)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchSettingView diagnostics

#ifdef _DEBUG
//##ModelId=49B87BB100FA
void CBatchSettingView::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87BB10109
void CBatchSettingView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBatchSettingView message handlers

//##ModelId=49B87BB100DA
void CBatchSettingView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	//ResizeParentToFit();
}


/*************************************************************
 函 数 名：RegulateControlSize()
 功能概要：调整控件大小, 位置
 返 回 值: void
**************************************************************/
//##ModelId=49B87BB003C8
void CBatchSettingView::RegulateControlSize()
{

	if(m_Tree.GetSafeHwnd() == NULL)
		return;
	
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	//调整设备树大小
	CRect rcTree = rc;
	rcTree.bottom = rc.bottom;
	rcTree.right = rc.right;
	m_Tree.MoveWindow(rcTree);
}

//##ModelId=49B87BB10119
void CBatchSettingView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 函 数 名：InitTree()
 功能概要：初始化设备树
 返 回 值: 初始化成功返回>=0的值, 失败返回<0的值
 参    数：param1
		   Param2
******************************r********************************/
//##ModelId=49B87BB003D8
int CBatchSettingView::InitTree()
{
	//清空
	m_Tree.DeleteAllItems();
	if (g_role != MODE_SUB)
	{
		//创建根节点
		m_hRoot = m_Tree.InsertItem(StringFromID(IDS_MODEL_AREA));
		m_Tree.SetItemState(m_hRoot, 0, TLIS_SHOWCHECKBOX);
		//创建地区
		BuildNet();
	}
	else
	{
		m_hRoot = m_Tree.InsertItem(STRING_STATION);
	}

	m_Tree.SetItemImage(m_hRoot, 0, 0, 0, 0);

    m_Tree.SetItemData(m_hRoot, 0);
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
void CBatchSettingView::BuildNet()
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
CTreeListItem* CBatchSettingView::FindNetPos(CString sNetID)
{
	CTreeListItem* htReturn = NULL;
	//根节点
	CTreeListItem* htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL)
		return NULL;
	//第一个地区节点
	CTreeListItem* htNetItem = m_Tree.GetChildItem(htRoot);

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
BOOL CBatchSettingView::InsertNet(CNetObj* pObj)
{
	//根节点为"地区"
	//地区节点在第二层
	CTreeListItem* htRoot = m_Tree.GetRootItem();
	MYASSERT_BOOL(htRoot);

	//插入节点
	CTreeListItem* htNew = m_Tree.InsertItem(pObj->m_sName, htRoot);
	m_Tree.SetItemImage(htNew, IMAGEINDEX_NETAREA, IMAGEINDEX_NETAREA, IMAGEINDEX_NETAREA, IMAGEINDEX_NETAREA);
    m_Tree.SetItemState(htNew, 0, TLIS_SHOWCHECKBOX);
	MYASSERT_BOOL(htNew);
	pObj->m_Tag0 = htNew;
	//设置数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	return TRUE;
}

/*************************************************************
 函 数 名：InsertStation()
 功能概要：插入厂站
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1  要插入的厂站对象
**************************************************************/
//##ModelId=49B87BB1001F
BOOL CBatchSettingView::InsertStation(CStationObj* pObj)
{
    CTreeListItem* htNew;
	if(g_role != MODE_SUB)
	{
		//厂站节点在地区节点下面, 为第三层
		//先查找地区节点
		CTreeListItem* htNet = FindNetPos(pObj->m_sNetID);
		if(htNet == NULL)
			return FALSE;
		//插入节点
		htNew = m_Tree.InsertItem(pObj->m_sName, htNet);
	}
	else
	{
		CTreeListItem* htRoot = m_Tree.GetRootItem();
		if (htRoot == NULL)
		{
			return FALSE;
		}
		htNew = m_Tree.InsertItem(pObj->m_sName, htRoot);
	}

	m_Tree.SetItemImage(htNew, pObj->IsCommStatusON()? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK
		, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK
		, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK
		, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK);
	m_Tree.SetItemData(htNew, (DWORD)pObj);
    m_Tree.SetItemState(htNew, 0, TLIS_SHOWCHECKBOX);
	//记录节点
	pObj->m_Tag0 = htNew;
	
	//设备数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);

	//加入各一次设备类型
	CTreeListItem* htBus		= m_Tree.InsertItem(StringFromID(IDS_MODEL_BUS), htNew);
	m_Tree.SetItemImage(htBus, IMAGEINDEX_BUS, IMAGEINDEX_BUS, IMAGEINDEX_BUS, IMAGEINDEX_BUS);
	m_Tree.SetItemState(htBus, 0, TLIS_SHOWCHECKBOX);
	m_Tree.SetItemData(htBus, TYPE_BUS);
	CTreeListItem* htLine	= m_Tree.InsertItem(StringFromID(IDS_MODEL_LINE), htNew);
	m_Tree.SetItemImage(htLine, IMAGEINDEX_LINE, IMAGEINDEX_LINE, IMAGEINDEX_LINE, IMAGEINDEX_LINE);
	m_Tree.SetItemState(htLine, 0, TLIS_SHOWCHECKBOX);
	m_Tree.SetItemData(htLine, TYPE_LINE);
	CTreeListItem* htGen		= m_Tree.InsertItem(StringFromID(IDS_MODEL_GEN), htNew);
    m_Tree.SetItemImage(htGen, IMAGEINDEX_GEN, IMAGEINDEX_GEN, IMAGEINDEX_GEN, IMAGEINDEX_GEN);
	m_Tree.SetItemState(htGen, 0, TLIS_SHOWCHECKBOX);
	m_Tree.SetItemData(htGen, TYPE_GEN);
	CTreeListItem* htTrans	= m_Tree.InsertItem(StringFromID(IDS_MODEL_TRANS), htNew);
	m_Tree.SetItemImage(htTrans, IMAGEINDEX_TRANS, IMAGEINDEX_TRANS, IMAGEINDEX_TRANS, IMAGEINDEX_TRANS);
	m_Tree.SetItemState(htTrans, 0, TLIS_SHOWCHECKBOX);
	m_Tree.SetItemData(htTrans, TYPE_TRANS);
	CTreeListItem* htCap	= m_Tree.InsertItem(StringFromID(IDS_MODEL_CAP), htNew);
    m_Tree.SetItemImage(htCap, IMAGEINDEX_CAP, IMAGEINDEX_CAP, IMAGEINDEX_CAP, IMAGEINDEX_CAP);
	m_Tree.SetItemState(htCap, 0, TLIS_SHOWCHECKBOX);
	m_Tree.SetItemData(htCap, TYPE_CAP);
	CTreeListItem* htReactor = m_Tree.InsertItem(StringFromID(IDS_MODEL_REACTOR), htNew);
    m_Tree.SetItemImage(htReactor, IMAGEINDEX_REACTOR, IMAGEINDEX_REACTOR, IMAGEINDEX_REACTOR, IMAGEINDEX_REACTOR);
	m_Tree.SetItemState(htReactor, 0, TLIS_SHOWCHECKBOX);
	m_Tree.SetItemData(htReactor, TYPE_REACTOR);
	CTreeListItem* htSwitch	= m_Tree.InsertItem(StringFromID(IDS_MODEL_SWITCH), htNew);
    m_Tree.SetItemImage(htSwitch, IMAGEINDEX_SWITCH, IMAGEINDEX_SWITCH, IMAGEINDEX_SWITCH, IMAGEINDEX_SWITCH);
	m_Tree.SetItemState(htSwitch, 0, TLIS_SHOWCHECKBOX);
	m_Tree.SetItemData(htSwitch, TYPE_SWITCH);
	CTreeListItem* htBreak	= m_Tree.InsertItem(StringFromID(IDS_MODEL_BREAKER), htNew);
    m_Tree.SetItemImage(htBreak, IMAGEINDEX_BREAK, IMAGEINDEX_BREAK, IMAGEINDEX_BREAK, IMAGEINDEX_BREAK);
	m_Tree.SetItemState(htBreak, 0, TLIS_SHOWCHECKBOX);
	m_Tree.SetItemData(htBreak, TYPE_BREAK);
	CTreeListItem* htWaveRec = m_Tree.InsertItem(StringFromID(IDS_MODEL_RECORDER), htNew);
	m_Tree.SetItemImage(htWaveRec, IMAGEINDEX_WAVEREC, IMAGEINDEX_WAVEREC, IMAGEINDEX_WAVEREC, IMAGEINDEX_WAVEREC);
	m_Tree.SetItemState(htWaveRec, 0, TLIS_SHOWCHECKBOX);
	m_Tree.SetItemData(htWaveRec, TYPE_WAVEREC);

	//加入各一次设备
	//母线
	BuildBus(pObj, htBus);
	//m_Tree.SortChildren(htBus);
	//线路
	BuildLine(pObj, htLine);
	//m_Tree.SortChildren(htLine);
	//Gen
	BuildGen(pObj, htGen);
	//m_Tree.SortChildren(htGen);
	//Trans
	BuildTrans(pObj, htTrans);
	//m_Tree.SortChildren(htTrans);
	//Cap
	BuildCap(pObj, htCap);
	//m_Tree.SortChildren(htCap);
	//Reactor
	BuildReactor(pObj, htReactor);
	//m_Tree.SortChildren(htReactor);
	//SWitch
	BuildSwitch(pObj, htSwitch);
	//m_Tree.SortChildren(htSwitch);
	//Break
	BuildBreak(pObj, htBreak);
	//m_Tree.SortChildren(htBreak);
	//waverec
	BuildWaveRec(pObj, htWaveRec);
	//m_Tree.SortChildren(htWaveRec);
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
void CBatchSettingView::BuildStation()
{
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	//增加所有厂站
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
BOOL CBatchSettingView::InsertDevice( CTreeListItem* htParent, CDeviceObj* pObj)
{
	//检查数据有效性
	if(htParent == NULL)
		return FALSE;
	if(pObj == NULL)
		return FALSE;
	//加入
	CTreeListItem* htNew = m_Tree.InsertItem(pObj->m_sName, htParent);
	if(htNew == NULL)
		return FALSE;
	pObj->m_Tag0 = htNew;
	//关联数据
	m_Tree.SetItemImage(htNew, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1);
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
void CBatchSettingView::BuildBus( CStationObj* pStation, CTreeListItem* htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_BUS, pStation->m_sID);
	//循环,加入到设备树
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
void CBatchSettingView::BuildLine( CStationObj* pStation, CTreeListItem* htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_LINE, pStation->m_sID);
	//循环,加入到设备树
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
void CBatchSettingView::BuildGen( CStationObj* pStation, CTreeListItem* htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_GEN, pStation->m_sID);
	//循环,加入到设备树
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
void CBatchSettingView::BuildTrans( CStationObj* pStation, CTreeListItem* htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_TRANS, pStation->m_sID);
	//循环,加入到设备树
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
void CBatchSettingView::BuildCap( CStationObj* pStation, CTreeListItem* htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_CAP, pStation->m_sID);
	//循环,加入到设备树
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
void CBatchSettingView::BuildReactor( CStationObj* pStation, CTreeListItem* htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_REACTOR, pStation->m_sID);
	//循环,加入到设备树
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
void CBatchSettingView::BuildBreak( CStationObj* pStation, CTreeListItem* htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_BREAK, pStation->m_sID);
	//循环,加入到设备树
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
void CBatchSettingView::BuildSwitch( CStationObj* pStation, CTreeListItem* htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_SWITCH, pStation->m_sID);
	//循环,加入到设备树
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
void CBatchSettingView::BuildWaveRec( CStationObj* pStation, CTreeListItem* htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_WAVEREC, pStation->m_sID);
	//循环,加入到设备树
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
BOOL CBatchSettingView::InsertProtect( CSecObj* pObj )
{
	//检查数据有效性
	if(pObj == NULL)
		return FALSE;
	if(pObj->m_nType != TYPE_PROTECT)
		return FALSE;
	if(pObj->m_pOwner == NULL)
		return FALSE;

	CTreeListItem* htTemp = NULL;
	htTemp = pObj->m_pOwner->m_Tag0;
	if(htTemp == NULL)
		return FALSE;
	//加入到设备树
	CString str;
	str = pObj->m_sName;
	CTreeListItem* htNew = m_Tree.InsertItem(str, htTemp);
	if(htNew == NULL)
		return FALSE;
	m_Tree.SetItemImage(htNew, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1);
	
	//记录位置
	pObj->m_Tag0 = htNew;
	
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
void CBatchSettingView::BuildProtect( CStationObj* pStation )
{
	if(pStation == NULL)
		return;
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_PROTECT, pStation->m_sID);
	//循环,加入到设备树
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
void CBatchSettingView::ClearEmptyNode()
{
	CTreeListItem* hTreeLevel1 = NULL;
	CTreeListItem* hTreeLevel2 = NULL;
	CTreeListItem* hTreeLevel3 = NULL;
	CTreeListItem* hTreeLevel4 = NULL;
	CTreeListItem* hTreeTemp = NULL;
	CString strTmp, strWave;
	strWave = StringFromID(IDS_MODEL_RECORDER);
	
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
void CBatchSettingView::CheckDevice( CDeviceObj* pObj, int nSelected )
{
	MYASSERT(pObj);
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
void CBatchSettingView::CycTreeDown( CTreeListItem* hRoot, BOOL bMark )
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
void CBatchSettingView::CycTreeUp( CTreeListItem* hCurrent )
{
	CTreeListItem* hParent = NULL; 
	CTreeListItem* hChild = NULL;
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
void CBatchSettingView::HitItemHelp( CTreeListItem* hit )
{
	//BOOL bMark;
	if(hit != NULL)
	{
		PostMessageToDetailSettingView(hit);
		/*
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
		*/
	}		
}



/*************************************************************
 函 数 名：OnBatchFrameOpen()
 功能概要：响应窗口打开消息, 刷新界面
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10148
void CBatchSettingView::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//刷新界面
	RegulateControlSize();
	m_nViewType = (int)lParam;
	//初始化
	//InitList();
	//InitTree();
}

/*************************************************************
 函 数 名：SetCheck_XJ()
 功能概要：设置指定节点的选择状态
 返 回 值: void
 参    数：param1	指定节点
		   Param2	TRUE-选中. FALSE-取消选择
**************************************************************/
//##ModelId=49B87BB100CD
void CBatchSettingView::SetCheck_XJ( CTreeListItem* hit, BOOL bCheck )
{
	CTreeListItem* hChild = NULL;
	CTreeListItem* hParent = NULL;

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
void CBatchSettingView::OnBatchDetailUncheck( WPARAM wParam, LPARAM lParam )
{
	//取得数据
	CDeviceObj* pObj = (CDeviceObj*)lParam;
	if(pObj == NULL)
		return;
	int nFlag = (int)wParam;
	CTreeListItem* ht = NULL;
	ht = pObj->m_Tag0;

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
void CBatchSettingView::OnBatchUnselectAll( WPARAM wParam, LPARAM lParam )
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
void CBatchSettingView::OnBatchSelectAllPT( WPARAM wParam, LPARAM lParam )
{
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_PROTECT);
	//循环所有
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//设置选择
		CTreeListItem* ht = NULL;
		ht = pObj->m_Tag0;
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
void CBatchSettingView::OnBatchSelectAllWR( WPARAM wParam, LPARAM lParam )
{
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_PROTECT);
	//循环所有
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//设置选择
		CTreeListItem* ht = NULL;
		ht = pObj->m_Tag0;
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
void CBatchSettingView::OnBatchEnableControls( WPARAM wParam, LPARAM lParam )
{

	BOOL bEnable = (BOOL)lParam;
	/*
	CWnd * pWnd = (CWnd*)GetDlgItem(IDC_LIST_BATCH_SELECT);
	if(pWnd != NULL)
		pWnd->EnableWindow(bEnable);
	pWnd = (CWnd*)GetDlgItem(IDC_TREE_BATCH_SELECT);
	if(pWnd != NULL)
		pWnd->EnableWindow(bEnable);
	*/
	m_Tree.EnableWindow(bEnable);
}

//##ModelId=49B87BB1013D
BOOL CBatchSettingView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
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
BOOL CBatchSettingView::InsertCPU( CSecCPU* pCpu, CTreeListItem* htParent)
{
	//检查数据有效性
	if(pCpu == NULL)
		return FALSE;
	if(htParent == NULL)
		return FALSE;
	//加入到设备树
	CString str;
	str.Format("%d(%s)", pCpu->code, pCpu->des);
	CTreeListItem* htNew = m_Tree.InsertItem(str, htParent);
	
	m_Tree.SetItemState(htNew, 0, TLIS_SHOWCHECKBOX);
	if(htNew == NULL)
		return FALSE;
	//记录位置
	pCpu->m_Tag0 = htNew;
	
	//设置数据
	m_Tree.SetItemData(htNew, (DWORD)pCpu);
	return TRUE;
}

void CBatchSettingView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	//只处理自动总召设置的情况
	if(m_nViewType != 0)
		return;
	CPoint ptClient = point;
	m_Tree.ScreenToClient(&ptClient);
	CTreeListItem* hItem = m_Tree.HitTest(ptClient);
	if(hItem != NULL)
	{
		//让节点被选中
		m_Tree.SelectItem(hItem);
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hItem);
		if ((DWORD)pObj < 100)
		{
			return;
		}
	//	if(pObj->m_nType == TYPE_STATION) //只处理厂站的情况
	//	{
			//弹出菜单
			//生成菜单
			CMenu Menu, *pMenu;
			if(!Menu.LoadMenu(IDR_MENU_BATCH_SETTING))
				return;
			pMenu = Menu.GetSubMenu(0);
			if (pObj->m_nType == TYPE_STATION)
			{
				m_pActiveStation = (CStationObj*)pObj;
			}
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
	//	}		
	}
}

void CBatchSettingView::OnAutocallActive() 
{
	// TODO: Add your command handler code here
	//取得被选择的项
	CTreeListItem* ht = m_Tree.GetSelectedItem();
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

void CBatchSettingView::OnAutocallAll() 
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

void CBatchSettingView::ShowDeviceList()
{
	//MYASSERT(m_pActiveStation);
	CWnd* pWnd = GetParent()->GetParent();
	MYASSERT(pWnd);
	DWORD pData = (DWORD)m_pActiveStation;

	pWnd->PostMessage(SHOW_BATCH, 0, (LPARAM)pData);
}

/*************************************************************
 函 数 名: IsEnableItem()
 功能概要: 判断指定节点是否可操作
 返 回 值: 可操作返回TRUE,不可操作返回FALSE
 参    数: param1 指定节点
		   Param2 
**************************************************************/
BOOL CBatchSettingView::IsEnableItem( CTreeListItem* ht )
{
	//自动总召且设置了可操作厂站时有效
	CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
	{
		//可能是硬编码,判断父节点
		CTreeListItem* htParent = m_Tree.GetParentItem(ht);
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
void CBatchSettingView::ClearCheck(CTreeListItem* ht)
{
	//判断数据有效性
	if(ht == NULL)
		return;
	//取消所有选择
	m_Tree.SetCheck(ht, FALSE);
	CTreeListItem* htChild = m_Tree.GetChildItem(ht);
	if(htChild != NULL)
		ClearCheck(htChild);
	CTreeListItem* htNext = m_Tree.GetNextItem(ht,TVGN_NEXT);
	if(htNext != NULL)
	{
		ClearCheck(htNext);
	}
}



int CBatchSettingView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect( 5, 5, 5, 5 );

	CRect left;
	left = rect;
	left.right = ( rect.left + rect.right ) / 2;

	CRect right;
	right = rect;
	right.left = ( rect.left + rect.right ) / 2;

	m_Tree.Create( 0x50000000, right, this, 1 );
	m_Tree.SetRedraw(FALSE);
	m_Tree.SetStyle
		( 0
		| TLC_TREELIST								// TreeList or List
//		| TLC_BKGNDIMAGE							// image background
//		| TLC_BKGNDCOLOR							// colored background ( not client area )
//		| TLC_DOUBLECOLOR							// double color background

		| TLC_MULTIPLESELECT						// single or multiple select
		| TLC_SHOWSELACTIVE							// show active column of selected item
    	| TLC_SHOWSELALWAYS							// show selected item always
		| TLC_SHOWSELFULLROWS						// show selected item in fullrow mode

//		| TLC_HEADER								// show header
//		| TLC_HGRID									// show horizonal grid lines
//		| TLC_VGRID									// show vertical grid lines
//		| TLC_TGRID									// show tree horizonal grid lines ( when HGRID & VGRID )
//		| TLC_HGRID_EXT								// show extention horizonal grid lines
//		| TLC_VGRID_EXT								// show extention vertical grid lines
//		| TLC_HGRID_FULL							// show full horizonal grid lines
//		| TLC_READONLY								// read only

		| TLC_TREELINE								// show tree line
		| TLC_ROOTLINE								// show root line
		| TLC_BUTTON								// show expand/collapse button [+]
//		| TLC_CHECKBOX								// show check box
//		| TLC_LOCKBOX								// show lock box
		| TLC_IMAGE									// show image
		| TLC_HOTTRACK								// show hover text 

//		| TLC_DRAG									// drag support
		| TLC_DROP									// drop support
		| TLC_DROPTHIS								// drop self support
//		| TLC_DROPROOT								// drop on root support

		| TLC_HEADDRAGDROP							// head drag drop
		| TLC_HEADFULLDRAG							// head funn drag
///		| TLC_NOAUTOCHECK							// do NOT auto set checkbox of parent & child
//		| TLC_NOAUTOLOCK							// do NOT auto set lockbox of parent & child
		);

	/*
	m_imageList.Create ( IDB_BITMAP_TREE, 16, 4, RGB(255, 255, 255) );
*/

	m_imageList.Create (16, 16, ILC_COLOR32, 1,1);
	m_bitmap.LoadBitmap(IDB_BITMAP_TREE);
	m_imageList.Add(&m_bitmap, RGB(255, 255, 255));

	m_Tree.SetImageList(&m_imageList);

	m_Tree.MoveWindow(&right, TRUE);
	m_Tree.InsertColumn(_T(""), TLF_DEFAULT_LEFT);
	m_Tree.SetColumnModify( 0, TLM_STATIC );
	m_Tree.SetItemHeight(18);
	
	InitTree();

	for( int iCol=0; iCol<m_Tree.GetColumnCount(); iCol++ )
	{
		m_Tree.SetColumnWidth( iCol, 360);
	}

	Invalidate();
	UpdateWindow();

	m_Tree.SetRedraw(TRUE);

	return 0;
}


/*************************************************************
 函 数 名：OnClickTreeBatchSetting()
 功能概要：响应设备树点击事件, 选择设备
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10128
void CBatchSettingView::OnClickTree(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	int uFlags = 0;
	HWND hWnd ;

	GetCursorPos(&pt);
	hWnd = ::WindowFromPoint(pt);
	::ScreenToClient(hWnd, &pt);

	CTreeListItem*  hit = m_Tree.HitTest(pt, &uFlags);

	if (uFlags)
	{	
		HitItemHelp(hit);
	}
}

/*************************************************************
 函 数 名：PostMessageToDetailSettingView
 功能概要：响应树形控件选择消息 
 返 回 值: 
 参    数：param1   
		   Param2  指定节点
**************************************************************/
void CBatchSettingView::PostMessageToDetailSettingView(CTreeListItem* hItem)
{
	if(hItem != NULL)
	{
		//发送消息给父窗口父窗口(因为父窗口是分隔窗口),让其转发给详细视图
		CWnd* pWnd = GetParent()->GetParent();
		if(pWnd != NULL)
		{
            DWORD pParentData = -1;
            DWORD pData = -1;
            CTreeListItem* hParentItem = m_Tree.GetParentItem(hItem);
			if (hParentItem != NULL)
			{
				pParentData = m_Tree.GetItemData(hParentItem);
			}

			if (0 < pParentData && pParentData < 11)
			{
				hParentItem = m_Tree.GetParentItem(hParentItem);
				pParentData = m_Tree.GetItemData(hParentItem);
			}
            pData = m_Tree.GetItemData(hItem);
	
			pWnd->PostMessage(BATCH_CHECK, (WPARAM)pParentData, (LPARAM)pData);
		}
	}
}
