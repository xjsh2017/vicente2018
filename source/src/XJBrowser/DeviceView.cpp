// DeviceView.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DeviceView.h"
#include "DataEngine.h"
#include "XJBrowserView.h"
#include "DlgStationSvg.h"
#include "DlgSignalList.h"
#include "Dlg_DevRunStatusUpdate.h"
#include "GlobalFunc.h"
#include "DlgStationProp.h"
#include "DLGMarked.h"
#include "MainFrm.h"

#include "stores/XJPTSetStore.h"
#include "stores/core/qptsetcard.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/


/////////////////////////////////////////////////////////////////////////////
// CDeviceView

IMPLEMENT_DYNCREATE(CDeviceView, CFormView)

CDeviceView::CDeviceView()
	: CFormView(CDeviceView::IDD)
{
	//{{AFX_DATA_INIT(CDeviceView)
	m_strSelect = _T("");
	//}}AFX_DATA_INIT
	htCurrent = NULL;
	m_nListType = 0;
	m_pSelObj = NULL;
	m_nDZ_MOD_State = -1;
}

CDeviceView::~CDeviceView()
{
	WriteLog("CDeviceView::~CDeviceView()");
	htCurrent = NULL;
	m_bitmap.Detach();
	m_imageList.Detach();
}

void CDeviceView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceView)
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_comboSelect);
	DDX_Control(pDX, IDC_TREE_DEVICE, m_Tree);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_List);
	DDX_CBString(pDX, IDC_COMBO_DEVICE, m_strSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceView, CFormView)
	//{{AFX_MSG_MAP(CDeviceView)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DEVICE, OnSelchangedTreeDevice)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_DEVICE, OnDblclkTreeDevice)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DEVICE, OnCustomDraw)
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_TREE_OPENMAP, OnTreeOpenmap)
	ON_COMMAND(ID_TREE_SETPATH, OnTreeSetpath)
	ON_COMMAND(ID_PT_SIGNALLIST, OnPtSignallist)
	ON_COMMAND(ID_QUERY_EVENT, OnQueryEvent)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_CLICK, IDC_TREE_DEVICE, OnClickTreeDevice)
    ON_COMMAND(ID_MENUITEM_DevTree_PTRunUpdate, OnMENUITEMDevTreePTRunUpdate)
    ON_COMMAND(ID_TREE_STATION_CONFIG_CHANGE, OnStationConfigChange)
	ON_CBN_EDITCHANGE(IDC_COMBO_DEVICE, OnEditchangeComboDevice)
	ON_CBN_SELENDOK(IDC_COMBO_DEVICE, OnSelendokComboDevice)
	ON_COMMAND(ID_TREE_STATION_PROP, OnTreeStationProp)
	ON_COMMAND(ID_TREE_STATION_ADD, OnTreeStationAdd)
	ON_COMMAND(ID_DevTree_PTADD, OnDevTreePTADD)
	ON_COMMAND(ID_DevTree_PTCONFIGCHANGED, OnDevTreePTCONFIGCHANGED)
	ON_COMMAND(ID_MARK_DEVICE, OnMarkDevice)
	ON_COMMAND(ID_MARK_PTDEVICE, OnMarkPtdevice)
	ON_COMMAND(ID_TREE_STATION_TEST, OnTreeStationTest)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_PT_GENERAL_NEW,ID_PT_OSC_LIST_NEW,OnPTProp)
	ON_COMMAND_RANGE(ID_WR_GENERAL_NEW,ID_WR_OSC_LIST_NEW,OnWRProp)
	ON_MESSAGE(STATION_INFO_CHANGED, OnStationInfoChanged)
	ON_MESSAGE(STATION_CHANGED, OnStationChanged)
	ON_MESSAGE(VIEW_REFRESH, OnViewRefresh)
	ON_MESSAGE(STATIONINIT, OnStationInit)
	ON_MESSAGE(WM_STTP_20146, OnSTTP20146)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(COMMVIEW_QUERY_END, OnEndCallStationStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceView diagnostics

#ifdef _DEBUG
void CDeviceView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDeviceView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDeviceView message handlers

void CDeviceView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit();

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
	InitListCtrl();
	if (g_style == 3)
	{
		m_List.SetBkColor(g_TreeBkColor);
	}
	else if(g_style == 2)
	{
		m_List.SetBkColor(g_ListSpaceColor3);
	}
	
	m_nTimer = SetTimer(101, 3*1000, 0);
}

/*************************************************************
 函 数 名：InitDeviceTree()
 功能概要：创建设备树, 填充设备树
 返 回 值: 失败返回-1, 其它返回>=0
**************************************************************/
int CDeviceView::InitDeviceTree()
{
	FillTree();
	return 0;
}

BOOL CDeviceView::FillTree()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;

	htCurrent = NULL;
	//清空
	m_Tree.DeleteAllItems();

    POSITION pos;
    HTREEITEM hRoot;
	if (g_role != MODE_SUB)
	{
		//根节点
		hRoot = m_Tree.InsertItem( StringFromID(IDS_MODEL_AREA), 0, 0,NULL);
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
		htRoot = m_Tree.InsertItem(StringFromID(IDS_MODEL_SUBSTATION), IMAGEINDEX_STATION, IMAGEINDEX_STATION, NULL);
	}
	//厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj * pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
		{
			if( pObj->m_nStationType == 113 || pObj->m_nStationType == 114 )
				continue;

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
	else if(g_DeviceTreeType == 4)
		ClearEmptyNodeType4();

	if (m_pSelObj){
		LocateObjInTree(m_pSelObj);
	}

	return TRUE;
}

/*************************************************************
 函 数 名：InitListCtrl()
 功能概要：初始化列表框, 填充数据
 返 回 值: 返回值说明
**************************************************************/
int CDeviceView::InitListCtrl()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,14,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	for (int nCol=0; nCol < 2 ; nCol++)
	{
		lvCol.iSubItem=nCol;
		switch(nCol)
		{
		case 0://类型
			lvCol.pszText=StringFromID(IDS_COMMON_PROPERTY).GetBuffer(1);
			lvCol.cx = 60; //列宽50象素
			break;
		case 1://事件ID
			lvCol.pszText=StringFromID(IDS_COMMON_VALUE).GetBuffer(1);
			lvCol.cx = 60; //列宽60象素
			break;
		default:
			lvCol.cx=100;//列宽100象素
		}
		
		m_List.InsertColumn(nCol,&lvCol);
	}

	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_List.ModifyStyle(0, LVS_NOCOLUMNHEADER);

	//填充内容
	FillList();

	return 0;
}

/*************************************************************
 函 数 名：RegulateControlSize()
 功能概要：调整控件大小, 位置
 返 回 值: void
**************************************************************/
void CDeviceView::RegulateControlSize()
{
	if(m_List.GetSafeHwnd() == NULL)
		return;
	if(m_Tree.GetSafeHwnd() == NULL)
		return;
	
	CRect rc;
	GetClientRect(&rc);

	CRect rcCombo = rc;
	m_comboSelect.GetClientRect(&rcCombo);
	int nComboHeight = rcCombo.Height();
	rcCombo = rc;
	rcCombo.bottom = rcCombo.top + nComboHeight;
	m_comboSelect.MoveWindow(rcCombo);

	rc.top += nComboHeight;

	CRect rcList = rc;

	int nListHeight = 124;

	float fHeight = (float)rc.Height();
	rc.bottom = rc.top + fHeight-nListHeight;
	m_Tree.MoveWindow(rc);

	rcList.top = rcList.bottom - nListHeight;
	m_List.MoveWindow(rcList); 
	//调整列宽
	m_List.SetColumnWidth(0, int(95));
	m_List.SetColumnWidth(1, int(rcList.Width() - 99));
}

void CDeviceView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 函 数 名：FindNetPos
 功能概要：查找指定地区在树中的位置
 返 回 值: 树节点结构
 参    数：param1   指定地区ID
**************************************************************/
HTREEITEM CDeviceView::FindNetPos(CString sNetID)
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
BOOL CDeviceView::InsertNet(CNetObj* pObj)
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
//	m_Tree.SortChildren(htRoot);
	return TRUE;
}

/*************************************************************
 函 数 名：InsertStation()
 功能概要：插入厂站
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1  要插入的厂站对象
**************************************************************/
BOOL CDeviceView::InsertStation(CStationObj* pObj)
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
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htNet);
		//排序
		m_Tree.SortChildren(htNet);
	} 
	else
	{
        HTREEITEM htRoot = m_Tree.GetRootItem();
        //根节点
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htRoot);
		//htNew= m_Tree.InsertItem(pObj->m_sName, 0, 0, htRoot);
	}
        //记录节点
		pObj->m_htItem = htNew;
		//设备数据
		m_Tree.SetItemData(htNew, (DWORD)pObj);
		
		
		//加入间隔
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CDataEngine * pData = pApp->GetDataEngine();
		if(pData == NULL)
			return FALSE;
		DEVICE_LIST listDevice;
		pData->GetPriDeviceList(listDevice, TYPE_BAY, pObj->m_sID);
		POSITION pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CBayObj* pBay = (CBayObj*)listDevice.GetNext(pos);
			InsertDevice(htNew, pBay);
		}
	
	return TRUE;
}

void CDeviceView::OnSelchangedTreeDevice(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM ht = m_Tree.GetSelectedItem();
	m_pSelObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(ht != htCurrent)
	{
		htCurrent = ht;
		FillList();
	//	RegulateControlSize();
	}

	*pResult = 0;
}

BOOL CDeviceView::FillList()
{
	//清空内容
	m_List.DeleteAllItems();
	m_nListType = 0;

	//填入内容 
	if(htCurrent != NULL)
	{
		CDeviceObj * pObj = (CDeviceObj*)(m_Tree.GetItemData(htCurrent));
		if(pObj != NULL)
		{
			if(pObj->m_nType == TYPE_STATION)
			{
				m_nListType = 1;
				CStationObj * pStation = (CStationObj*)pObj;
				//是厂站
				//第一行,厂站名
				m_List.InsertItem(LVIF_TEXT, 0, StringFromID(IDS_MODEL_SUBSTATION), 0, 0, 0, 0);
				m_List.SetItemText(0, 1, pStation->m_sName);
				//第二行,状态
				m_List.InsertItem(LVIF_TEXT, 1, StringFromID(IDS_FUNC_COMMUSTATUS), 0, 0, 0, 0);
				m_List.SetItemText(1, 1, pStation->IsCommStatusON()?StringFromID(IDS_COMMUSTATUS_ON):StringFromID(IDS_COMMUSTATUS_OFF));
				//第三行,通信断开保护
				m_List.InsertItem(LVIF_TEXT, 2, StringFromID(IDS_STAT_OFFDEVICE), 0, 0, 0, 0);
				CString str;
				str.Format("%d", pStation->GetCommAbnormityNum());
				m_List.SetItemText(2, 1, str);
				//第四行,运行异常保护
				m_List.InsertItem(LVIF_TEXT, 3, StringFromID(IDS_STAT_RUNABNORMAL), 0, 0, 0, 0);
				str.Format("%d", pStation->GetRunAbnormityNum());
				m_List.SetItemText(3, 1, str);
				//第五行,厂家
				m_List.InsertItem(LVIF_TEXT, 4, StringFromID(IDS_MODEL_MANUFACTURER), 0, 0, 0, 0);
				m_List.SetItemText(4, 1, pStation->m_sManufacturer);
				//第六行,IP
				m_List.InsertItem(LVIF_TEXT, 5, StringFromID(IDS_COMMON_IP), 0, 0, 0, 0);
				m_List.SetItemText(5, 1, pStation->m_sIP);
				//第七行,规约
				m_List.InsertItem(LVIF_TEXT, 6, StringFromID(IDS_COMMON_PROTOCOL), 0, 0, 0, 0);
				m_List.SetItemText(6, 1, pStation->GetProtocolName());
			}
			else if(pObj->m_nType == TYPE_NET)
			{
				m_nListType = 2;
				CNetObj * pNet = (CNetObj*)pObj;
				pNet->GetCommAbnormityNum();
				//是地区
				//第一行,地区名
				m_List.InsertItem(LVIF_TEXT, 0, StringFromID(IDS_MODEL_AREA), 0, 0, 0, 0);
				m_List.SetItemText(0, 1, pNet->m_sName);
				//第二行,通信断开厂站
				m_List.InsertItem(LVIF_TEXT, 1, StringFromID(IDS_STAT_OFFSTATION), 0, 0, 0, 0);
				CString str;
				str.Format("%d", pNet->m_nStationAbnormity);
				m_List.SetItemText(1, 1, str);
				//第三行,通信断开保护
				m_List.InsertItem(LVIF_TEXT, 2, StringFromID(IDS_STAT_OFFDEVICE), 0, 0, 0, 0);
				str.Format("%d", pNet->m_nPTCommAbnormity);
				m_List.SetItemText(2, 1, str);
				//第四行,运行异常保护
				m_List.InsertItem(LVIF_TEXT, 3, StringFromID(IDS_STAT_RUNABNORMAL), 0, 0, 0, 0);
				str.Format("%d", pNet->m_nPTRunAbnormity);
				m_List.SetItemText(3, 1, str);
			}
			else if(pObj->m_nType == TYPE_PROTECT || pObj->m_nType == TYPE_WAVEREC)
			{
				m_nListType = 3;
				CSecObj* pSec = (CSecObj*)pObj;
				//第一行,设备名
				m_List.InsertItem(LVIF_TEXT, 0, StringFromID(IDS_COMMON_DEVICENAME), 0, 0, 0, 0);
				m_List.SetItemText(0, 1, pSec->m_sName);
				//第二行,通信
				m_List.InsertItem(LVIF_TEXT, 1, StringFromID(IDS_FUNC_COMMUSTATUS), 0, 0, 0, 0);
				m_List.SetItemText(1, 1, GetCommStatusString(pSec->m_nCommStatus));
				//第三行,运行
				m_List.InsertItem(LVIF_TEXT, 2, StringFromID(IDS_FUNC_RUNSTATUS), 0, 0, 0, 0);
				m_List.SetItemText(2, 1, GetRunStatusString(pSec->m_nRunStatu));
				//第四行,子站名
				m_List.InsertItem(LVIF_TEXT, 3, StringFromID(IDS_MODEL_SUBSTATION), 0, 0, 0, 0);
				m_List.SetItemText(3, 1, pSec->m_pStation==NULL?"":pSec->m_pStation->m_sName);
				//第五行,一次设备
				m_List.InsertItem(LVIF_TEXT, 4, StringFromID(IDS_MODEL_PRIMARY), 0, 0, 0, 0);
				m_List.SetItemText(4, 1, pSec->m_pOwner == NULL?"":pSec->m_pOwner->m_sName);
				
			}
		}
	}

	return TRUE;
}

void CDeviceView::OnDblclkTreeDevice(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM ht = m_Tree.GetSelectedItem();
	if(ht == NULL)
		return;
	//判断是否厂站
	CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pObj->m_nType == TYPE_STATION)
	{
		//厂站
		CStationObj * pStation = (CStationObj*)pObj;

		if(g_bShowDeviceMapWnd) //显示一次接线图
		{
			//关闭之前图形,打开所选厂站图形
			CXJBrowserView* pSvgView = pApp->GetSVGView();
			if(pSvgView == NULL)
				return;
			if(pStation->m_sID != pSvgView->m_sStationID || pSvgView->m_nSvgType != 1)
			{
				//现在选择的厂站与当前打开的图形不是同一厂站
				//关闭旧的,打开新的
				pSvgView->CloseCurrentView();
				pSvgView->OpenStationView(pStation->m_sID);
				pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
			}
			else
			{
				//把视图推到前面
				pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
			}
		}
		else //显示厂站信息窗口
		{
			pApp->ShowStationInfo(pStation);
		}
	}
	else if(pObj->m_nType == TYPE_BAY)
	{
		if(g_bShowDeviceMapWnd) //显示一次接线图
		{
			//间隔
			CBayObj* pBay = (CBayObj*)pObj;
			//关闭之前图形,打开所选厂站图形
			CXJBrowserView* pSvgView = pApp->GetSVGView();
			if(pSvgView == NULL)
				return;
			if(pBay != pSvgView->m_pCurBay || pSvgView->m_nSvgType != 2)
			{
				//现在选择的厂站与当前打开的图形不是同一厂站
				//关闭旧的,打开新的
				pSvgView->CloseCurrentView();
				pSvgView->OpenBayView(pBay);
				pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
			}
			else
			{
				//把视图推到前面
				pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
			}
		}
	}
	else if(pObj->m_nType == TYPE_PROTECT || pObj->m_nType == TYPE_WAVEREC || pObj->m_nType == TYPE_SEC)
	{
		//打开属性页
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		CSecObj* pSec = (CSecObj*)pObj;
		//打开保护属性页

		if(pSec->m_nType == TYPE_PROTECT)
			pDoc->ShowSecPropPage(pSec);
		else
			pDoc->ShowSecPropPage(pSec, 1);
	}
	
	*pResult = 0;
}

/*************************************************************
 函 数 名：OnCustomDraw()
 功能概要：
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
void CDeviceView::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				if (g_style == 3)
				{
					lplvcd->clrText = g_TreeTextColor;
					lplvcd->clrTextBk = g_TreeBkColor;
				}
				else if(g_style == 2)
				{
					lplvcd->clrTextBk = g_ListSpaceColor3;
				}
				
				if(m_nListType == 1 || m_nListType == 2)
				{
					//当前状态行
					if(nItem  == 1)
					{
						//值列
						if(nSubItem == 1)
						{
							CString str = m_List.GetItemText(nItem, nSubItem);
							//断开时特殊显示
							if(str == StringFromID(IDS_COMMUSTATUS_OFF))
							{
								lplvcd->clrText = XJ_STATUS_COMM_OFF;
							}
						}
					}
					//通信异常保护数,运行状态异常保护数
					if(nItem == 2 || nItem == 3 || nItem == 1)
					{
						//值列s
						if(nSubItem == 1)
						{
							CString str = m_List.GetItemText(nItem, nSubItem);
							int nCount = atoi(str);
							//异常数大于0时特殊显示
							if(nCount > 0)
							{
								lplvcd->clrText = XJ_STATUS_COMM_OFF;
							}
						}
					}
				}
				else if(m_nListType == 3) //二次设备
				{
					//当前状态行
					if(nItem  == 1)
					{
						//值列
						if(nSubItem == 1)
						{
							CString str = m_List.GetItemText(nItem, nSubItem);
							//断开时特殊显示
							if(str != GetCommStatusString(0))
							{
								lplvcd->clrText = XJ_STATUS_COMM_OFF;
							}
						}
					}
					if(nItem  == 2)
					{
						//值列
						if(nSubItem == 1)
						{
							CString str = m_List.GetItemText(nItem, nSubItem);
							//断开时特殊显示
							if(str != GetRunStatusString(2))
							{
								lplvcd->clrText = XJ_STATUS_COMM_OFF;
							}
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
 函 数 名：LocateInTree()
 功能概要：在设备树中定位指定厂站
 返 回 值: void
 参    数：param1	指定厂站
**************************************************************/
void CDeviceView::LocateInTree( CStationObj* pStation )
{
	if(pStation == NULL)
		return;
	//判断保存的节点是否有效
	if(pStation->m_htItem == NULL)
		return;
	m_Tree.SelectItem(pStation->m_htItem);
	m_Tree.EnsureVisible(pStation->m_htItem);
}

void CDeviceView::LocateObjInTree( CDeviceObj* pObj )
{
	if(pObj == NULL)
		return;
	//判断保存的节点是否有效
	if(pObj->m_htItem == NULL)
		return;
	m_Tree.SelectItem(pObj->m_htItem);
	m_Tree.EnsureVisible(pObj->m_htItem);
}

void CDeviceView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
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
	    CMenu Menu, *pMenu;
		if(pObj->m_nType == TYPE_STATION || pObj->m_nType == TYPE_BAY) //只处理厂站的情况
		{
			//弹出菜单
			//生成菜单
			if(!Menu.LoadMenu(IDR_MENU_TREE_STATION))
				return;
			pMenu = Menu.GetSubMenu(0);


			if (g_role == MODE_MAIN)
			{
				Menu.DeleteMenu(ID_TREE_STATION_CONFIG_CHANGE, MF_BYCOMMAND);
				Menu.DeleteMenu(ID_TREE_STATION_ADD, MF_BYCOMMAND);
			}
			else if(g_role == MODE_SUB)
			{
				Menu.DeleteMenu(ID_TREE_SETPATH, MF_BYCOMMAND);
				Menu.DeleteMenu(ID_TREE_STATION_ADD, MF_BYCOMMAND);
				Menu.DeleteMenu(ID_TREE_STATION_TEST, MF_BYCOMMAND);
			}
	
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
		else if(pObj->m_nType == TYPE_PROTECT)
		{
			//弹出保护菜单
			if(!Menu.LoadMenu(IDR_MENU_PROTECT))
				return;
			pMenu = Menu.GetSubMenu(0);
			if (g_role != MODE_SUB)
			{
				Menu.DeleteMenu(ID_MENUITEM_DevTree_PTRunUpdate, MF_BYCOMMAND);
			}
			if(g_role == MODE_MAIN)
			{
				Menu.DeleteMenu(ID_DevTree_PTCONFIGCHANGED, MF_BYCOMMAND);
				Menu.DeleteMenu(ID_DevTree_PTADD, MF_BYCOMMAND);
			}

			CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
			//CString strTemp;
			//strTemp.Format("UserID: %s, UserGroupID: %s",pApp->m_User.m_strUSER_ID, pApp->m_User.m_strGROUP_ID);
			//AfxMessageBox(strTemp);
			if (pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_RUNNER)
				|| pApp->m_User.m_strGROUP_ID == StringFromID(IDS_USERGROUP_SUPER)){
			}else{
				Menu.DeleteMenu(ID_MARK_DEVICE, MF_BYCOMMAND);
			}
	
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
		else if(pObj->m_nType == TYPE_WAVEREC)
		{
			//弹出录波器菜单
			if(!Menu.LoadMenu(IDR_MENU_WAVEREC))
				return;
			pMenu = Menu.GetSubMenu(0);
			if (g_role != MODE_SUB)
			{
				Menu.DeleteMenu(ID_MENUITEM_DevTree_PTRunUpdate, MF_BYCOMMAND);
			}
			if(g_role == MODE_MAIN)
			{
				Menu.DeleteMenu(ID_DevTree_PTCONFIGCHANGED, MF_BYCOMMAND);
				Menu.DeleteMenu(ID_DevTree_PTADD, MF_BYCOMMAND);
			}
		
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}	
		
	}
}

void CDeviceView::OnTreeOpenmap() 
{
	// TODO: Add your command handler code here
	HTREEITEM ht = m_Tree.GetSelectedItem();
	if(ht == NULL)
		return;
	//判断是否厂站
	CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
		return;
	if(pObj == NULL)
		return;
	if(pObj->m_nType == TYPE_STATION)
	{
		//厂站
		CStationObj * pStation = (CStationObj*)pObj;
		
		//关闭之前图形,打开所选厂站图形
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserView* pSvgView = pApp->GetSVGView();
		if(pSvgView == NULL)
			return;
		if(pStation->m_sID != pSvgView->m_sStationID || pSvgView->m_nSvgType != 1)
		{
			//现在选择的厂站与当前打开的图形不是同一厂站
			//关闭旧的,打开新的
			pSvgView->CloseCurrentView();
			pSvgView->OpenStationView(pStation->m_sID);
			pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
		else
		{
			//把视图推到前面
			pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
	}
	else if(pObj->m_nType == TYPE_BAY)
	{
		//间隔
		CBayObj* pBay = (CBayObj*)pObj;
		//关闭之前图形,打开所选厂站图形
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserView* pSvgView = pApp->GetSVGView();
		if(pSvgView == NULL)
			return;
		if(pBay != pSvgView->m_pCurBay || pSvgView->m_nSvgType != 2)
		{
			//现在选择的厂站与当前打开的图形不是同一厂站
			//关闭旧的,打开新的
			pSvgView->CloseCurrentView();
			pSvgView->OpenBayView(pBay);
			pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
		else
		{
			//把视图推到前面
			pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
	}
}


void CDeviceView::OnTreeSetpath() 
{
	// TODO: Add your command handler code here
	HTREEITEM ht = m_Tree.GetSelectedItem();
	if(ht == NULL)
		return;
	//判断是否厂站
	CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
		return;
	if(pObj->m_nType == TYPE_STATION)
	{
		CStationObj * pStation = (CStationObj*)pObj;

		CDlgStationSvg dlg;
		dlg.m_pStation = pStation;
		dlg.DoModal();
	}
	else if(pObj->m_nType == TYPE_BAY)
	{
		CBayObj* pBay = (CBayObj*)pObj;

		CDlgStationSvg dlg;
		dlg.m_nType = 2;
		dlg.m_pBay = pBay;
		dlg.DoModal();
	}
}

/*************************************************************
 函 数 名: InsertDevice()
 功能概要: 加入设备到设备树
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数: param1 指定父节点
		   Param2 指定设备
**************************************************************/
BOOL CDeviceView::InsertDevice( HTREEITEM htParent, CDeviceObj* pObj)
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

	HTREEITEM htNew;
	htNew = m_Tree.InsertItem(pObj->m_sName, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, htParent);
	if(htNew == NULL)
		return FALSE;
	//记录节点位置
	pObj->m_htItem = htNew;
	//关联数据
		m_Tree.SetItemData(htNew, (DWORD)pObj);
	return TRUE;
}

/*************************************************************
 函 数 名: InsertStation_Voltage()
 功能概要: 插入厂站节点，厂站下接电压等级
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数: param1 要插入的厂站对象
		   Param2 
**************************************************************/
BOOL CDeviceView::InsertStation_Voltage( CStationObj* pObj )
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
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htNet);
	    //排序
		m_Tree.SortChildren(htNet);
	}
	else
	{
		HTREEITEM htRoot = m_Tree.GetRootItem();
        //厂站节点
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htRoot);
	}

        //记录节点
		pObj->m_htItem = htNew;
		//设备数据
		m_Tree.SetItemData(htNew, (DWORD)pObj);	
	
	if(g_DeviceTreeType == 4)
	{
		//二次设备直接挂在厂站下
		BuildSecToStation(pObj);
		//排序
		m_Tree.SortChildren(htNew);
	}
	else
	{
		if( !pObj->m_IsDistanceVirtual)
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
			if( m_Tree.ItemHasChildren(htWaveRec) )
				m_Tree.SortChildren(htWaveRec);
			else
				m_Tree.DeleteItem(htWaveRec);
			
			
			HTREEITEM htGPSRec = m_Tree.InsertItem(StringFromID(IDS_MODEL_GPS), IMAGEINDEX_SWITCH, IMAGEINDEX_SWITCH, htNew);
			BuildGPS(pObj, htGPSRec);
			if( m_Tree.ItemHasChildren(htGPSRec) )
				m_Tree.SortChildren(htGPSRec);
			else
				m_Tree.DeleteItem(htGPSRec);
			
		}
		else
		{
			HTREEITEM htDistance = m_Tree.InsertItem(StringFromID(IDS_MODEL_DISTANCE), IMAGEINDEX_DISTANCE, IMAGEINDEX_DISTANCE, htNew);
			BuildDistance(pObj->m_sNetID, htDistance);
			if( m_Tree.ItemHasChildren(htDistance) )
				m_Tree.SortChildren(htDistance);
		}
	}	
	return TRUE;
}

/*************************************************************
 函 数 名: InsertVoltage()
 功能概要: 插入电压等级节点,节点下跟设备
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数: param1 指定电压等级
		   Param2 指定父节点
**************************************************************/
BOOL CDeviceView::InsertVoltage( int nVoltage, HTREEITEM htParent, CStationObj* pObj )
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
 函 数 名: SortVoltage()
 功能概要: 对站内电压等级排序
 返 回 值: 
 参    数: param1 指定站
		   Param2 
**************************************************************/
void CDeviceView::SortVoltage( CStationObj* pObj )
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
void CDeviceView::BuildBreak( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
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
void CDeviceView::BuildBus( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID)
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
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
void CDeviceView::BuildCap( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
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
void CDeviceView::BuildGen( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
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
void CDeviceView::BuildLine( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
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
void CDeviceView::BuildReactor( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
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
void CDeviceView::BuildSwitch( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
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
void CDeviceView::BuildTrans( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
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
void CDeviceView::BuildProtect( CStationObj* pStation)
{
	if(pStation == NULL)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
	pData->GetSecList(listDevice, "", TYPE_PROTECT, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj *)listDevice.GetNext(pos);
		if(pObj != NULL)
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
BOOL CDeviceView::InsertProtect( CSecObj* pObj)
{
	//检查数据有效性
	if(pObj == NULL)
		return FALSE;
	if(pObj->m_nType != TYPE_PROTECT)
		return FALSE;
	if(pObj->m_pOwner == NULL)
		return FALSE;
	if(pObj->m_pOwner->m_htItem == NULL)
		return FALSE;
	//加入到设备树
	CString str;
	HTREEITEM htNew = NULL;
	str = pObj->m_sName;
	if (pObj->m_nSuspendstatus == 1)
	{
		htNew = m_Tree.InsertItem(str, IMAGEINDEX_PROTECT2, IMAGEINDEX_SELSEC/*IMAGEINDEX_PROTECT2*/, pObj->m_pOwner->m_htItem);		
	}
	else
	{
		htNew = m_Tree.InsertItem(str, IMAGEINDEX_PROTECT1, IMAGEINDEX_SELSEC/*IMAGEINDEX_PROTECT1*/, pObj->m_pOwner->m_htItem);	
	}
	if(htNew == NULL)
		return FALSE;
	
	
	//记录位置
	pObj->m_htItem = htNew;
	m_Tree.SortChildren(pObj->m_pOwner->m_htItem);

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
void CDeviceView::BuildWaveRec( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//循环,加入到设备树
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
	pData->GetSecList(listDevice, "", TYPE_WAVEREC, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj *)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertPTDevice(htParent, pObj);
	}
}


void CDeviceView::BuildGPS( CStationObj* pStation, HTREEITEM htParent )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	
	//循环,加入到设备
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
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
void CDeviceView::InsertFastness( HTREEITEM htParent, CStationObj* pObj, int nVoltage, CString sBayID )
{
	//加入各一次设备类型
	HTREEITEM htBus		= m_Tree.InsertItem(StringFromID(IDS_MODEL_BUS), IMAGEINDEX_BUS, IMAGEINDEX_BUS, htParent);
	HTREEITEM htLine	= m_Tree.InsertItem(StringFromID(IDS_MODEL_LINE), IMAGEINDEX_LINE, IMAGEINDEX_LINE, htParent);
	HTREEITEM htGen		= m_Tree.InsertItem(StringFromID(IDS_MODEL_GEN), IMAGEINDEX_GEN, IMAGEINDEX_GEN, htParent);
	HTREEITEM htTrans	= m_Tree.InsertItem(StringFromID(IDS_MODEL_TRANS), IMAGEINDEX_TRANS, IMAGEINDEX_TRANS, htParent);
	HTREEITEM htCap		= m_Tree.InsertItem(StringFromID(IDS_MODEL_CAP), IMAGEINDEX_CAP, IMAGEINDEX_CAP, htParent);
	HTREEITEM htReactor = m_Tree.InsertItem(StringFromID(IDS_MODEL_REACTOR), IMAGEINDEX_REACTOR, IMAGEINDEX_REACTOR, htParent);
	HTREEITEM htSwitch	= m_Tree.InsertItem(StringFromID(IDS_MODEL_SWITCH), IMAGEINDEX_SWITCH, IMAGEINDEX_SWITCH, htParent);
	HTREEITEM htBreak	= m_Tree.InsertItem(StringFromID(IDS_MODEL_BREAKER), IMAGEINDEX_BREAK, IMAGEINDEX_BREAK, htParent);
	
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
void CDeviceView::BuildBay( CStationObj* pStation, HTREEITEM htParent, int nVoltage )
{
	//检查数据有效性
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
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
void CDeviceView::InsertBay( HTREEITEM htParent, CStationObj* pObj, CBayObj* pBay)
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
	pBay->m_htItem = htNew;
	//关联数据
	m_Tree.SetItemData(htNew, (DWORD)pBay);

	//插入硬节点
	InsertFastness(htNew, pObj, 0, pBay->m_sID);
}

/*************************************************************
 函 数 名: ClearEmptyNodeType1()
 功能概要: 清除设备树类型为1的设备树的空节点
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDeviceView::ClearEmptyNodeType1()
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
				if(str != STRING_WAVEREC)
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
void CDeviceView::ClearEmptyNodeType2()
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

void CDeviceView::OnPTProp( UINT nID )
{
	HTREEITEM htItem = m_Tree.GetSelectedItem();
	if(htItem == NULL)
		return;
	//取第一个
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//是保护
		CSecObj* pObj = (CSecObj*)pDevice;
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		
		pDoc->ShowSecPropPage(pObj, (nID - ID_PT_GENERAL_NEW));
	}
}

void CDeviceView::OnWRProp( UINT nID )
{
	HTREEITEM htItem = m_Tree.GetSelectedItem();
	if(htItem == NULL)
		return;
	//取得
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_WAVEREC)
	{
		//是录波器
		CSecObj* pObj = (CSecObj*)pDevice;
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		
		pDoc->ShowSecPropPage(pObj, (nID - ID_WR_GENERAL_NEW));
	}
}

void CDeviceView::OnPtSignallist() 
{
	// TODO: Add your command handler code here
	HTREEITEM htItem = m_Tree.GetSelectedItem();
	if(htItem == NULL)
		return;
	//取得
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//是保护
		CSecObj* pObj = (CSecObj*)pDevice;
		pObj->RefreshConfig();
		CDlgSignalList dlg;
		dlg.m_pObj = pObj;
		dlg.DoModal();
	}
}

void CDeviceView::OnQueryEvent() 
{
	// TODO: Add your command handler code here
	HTREEITEM htItem = m_Tree.GetSelectedItem();
	if(htItem == NULL)
		return;
	//取得
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//是保护
		CSecObj* pObj = (CSecObj*)pDevice;
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		
		//组建查询参数
		CString sQuery = "";
		sQuery.Format("0$$$ $$$99$$$ $$$%s$$$99$$$0$$$1", pObj->m_sID);
		OpenXJQuery(sQuery, GetSafeHwnd());
	}
}

void CDeviceView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnLButtonDown(nFlags, point);
}

void CDeviceView::OnClickTreeDevice(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	UINT uFlags = 0;
	HWND hWnd ;
	
	GetCursorPos(&pt);
	hWnd = ::WindowFromPoint(pt);
	::ScreenToClient(hWnd, &pt);
	
	HTREEITEM  hit = m_Tree.HitTest(pt, &uFlags);
	if(uFlags & TVHT_ONITEM)
	{
		//复制
		m_Tree.SelectItem(hit);//让节点选中
		CDeviceObj *pData = (CDeviceObj*)m_Tree.GetItemData(hit);
		if(pData != NULL)
		{
			HGLOBAL hPointer = GlobalAlloc(GMEM_FIXED | GMEM_SHARE, sizeof(CDeviceObj*));
			CDeviceObj **pPointer = (CDeviceObj**)GlobalLock(hPointer);
			if(pPointer)
			{
				memcpy(pPointer, (&pData), sizeof(CDeviceObj*));
				GlobalUnlock(hPointer);
				m_dataSource.CacheGlobalData(g_dwESDataFormat, hPointer);
				//just do a copy
				m_dataSource.DoDragDrop(DROPEFFECT_COPY, CRect(0,0,0,0));
			}		
		}
	}
	*pResult = 0;
}

void CDeviceView::OnStationInfoChanged( WPARAM wParam, LPARAM lParam )
{
	//取得厂站
	CStationObj* pStation = (CStationObj*)lParam;
	if(pStation == NULL)
		return;
	//取得列表的当前显示
	if(htCurrent == NULL)
		return;
	CDeviceObj * pObj = (CDeviceObj*)(m_Tree.GetItemData(htCurrent));
	if(pObj == NULL)
		return;
	//判断类型
	if(pObj->m_nType == TYPE_STATION)//厂站
	{
		CStationObj* pCurrent = (CStationObj*)pObj;
		if(pCurrent == pStation) //同一厂站
		{
			//重新载入数据
			FillList();
		}
	}
	else if(pObj->m_nType == TYPE_NET)//地区
	{
		if(pObj->m_sID == pStation->m_sNetID)//变化的是本地区下的设备
		{
			//重新载入数据
			FillList();
		}
	}
}


//保护运行状态上载
void CDeviceView::OnMENUITEMDevTreePTRunUpdate() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	HTREEITEM ht = m_Tree.GetSelectedItem();
	if(ht == NULL)
		return;
	//判断是否厂站
	CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
		return;
	if(pObj->m_nType != TYPE_PROTECT && pObj->m_nType != TYPE_WAVEREC && pObj->m_nType != TYPE_SEC)
		return;

	if(!pApp->TryEnter("系统配置"))
	{
		return;
	}
	
		//2 dlg
		CDlg_DevRunStatusUpdate dlg;
		dlg.m_cstrPTID = pObj->m_sID;
		dlg.m_cstrPTName = pObj->m_sName;
		dlg.DoModal();
		pObj->m_nRunStatu = dlg.m_nStatus;
}


void CDeviceView::OnStationConfigChange()
{

	try
	{
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
       	//检查通讯情况
		if(!pApp->GetSTTPStatus())
		{
			AfxMessageBox(STTP_DISCONNECT);
			return;
		}
	
		STTP_FULL_DATA sttpData;
		CDataEngine* pData = pApp->GetDataEngine();
		CString strTemp = "";
		
		HTREEITEM ht = m_Tree.GetSelectedItem();
		if(ht == NULL)
			return;
		//判断是否厂站
		CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
		if(pObj == NULL)
			return;
		if(pObj->m_nType != TYPE_STATION)
			return;
		
		CStationObj* pStationObj = (CStationObj*)pObj;
		MYASSERT(pStationObj);

		if (pApp->m_SttpEngine.BuildDataFor20138StationConfigChange(sttpData, pStationObj->m_sID))
		{
			int result = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			if (result)
			{
				strTemp = StringFromID(IDS_STATIONCHANGE_NOTIFY_SUCCESS);
				AfxMessageBox(strTemp);
				WriteLog(strTemp);
			}
			else
			{
				strTemp = StringFromID(IDS_STATIONCHANGE_NOTIFY_FAIL);
				AfxMessageBox(strTemp);
				WriteLog(strTemp);
			}
		}
		else
		{
			strTemp.Format(StringFromID(IDS_STATIONCHANGE_BUILD_FAIL));
			AfxMessageBox(strTemp);
			WriteLog(strTemp);
		}
		//SendMessage(WM_CLOSE);
		
	}
	catch (...)
	{
		AfxMessageBox(StringFromID(IDS_STATIONCHANGE_NOTIFY_FAIL));		
	}
}


void CDeviceView::OnStationChanged( WPARAM wParam, LPARAM lParam )
{
	CStationObj* pStation = (CStationObj*)lParam;
	MYASSERT(pStation);
	m_Tree.SetItemImage(pStation->m_htItem, pStation->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pStation->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK);
	//FillTree();
}

void CDeviceView::BuildSecToStation( CStationObj* pStation )
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

BOOL CDeviceView::InsertSecToStation( CSecObj* pObj )
{
	//检查数据有效性
	if(pObj == NULL)
		return FALSE;
	if(pObj->m_pStation == NULL)
		return FALSE;
	if(pObj->m_pStation->m_htItem == NULL)
		return FALSE;
	//加入到设备树
	CString str;
	str = pObj->m_sName;
	HTREEITEM htNew = m_Tree.InsertItem(str, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, pObj->m_pStation->m_htItem);
	if(htNew == NULL)
		return FALSE;
	
	//记录位置
	pObj->m_htItem = htNew;
	//m_Tree.SortChildren(pObj->m_pStation->m_htItem);
	
	//设置数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);	
	
	return TRUE;
}

/****************************************************
Date:2011/10/14  Author:LYH
函数名:   IsChinese	
返回值:   BOOL	
功能概要: 
参数: CString str	
*****************************************************/
BOOL CDeviceView::IsChinese( CString str )
{
	TBYTE ucHigh, ucLow;
	
	
    if ( (TBYTE)str[0] < 0x80 )
		return FALSE;
    ucHigh = (TBYTE)str[0];
    ucLow  = (TBYTE)str[1];
    if ( ucHigh < 0xa1 || ucLow < 0xa1)
		return FALSE;
    return TRUE;
}

/****************************************************
Date:2011/10/14  Author:LYH
函数名:   CompareStation	
返回值:   void	
功能概要: 
参数: CString str	
*****************************************************/
void CDeviceView::CompareStation( CString str )
{
	if(str == "")
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp *)AfxGetApp();
	CDataEngine * pDB = pApp ->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pDB->GetStationList(listDevice);

	if(IsChinese(str)) //输入为中文,直接与厂站名比较
	{
		int length = str.GetLength();
		CComboBox * pCMB = (CComboBox *)GetDlgItem(IDC_COMBO_DEVICE);
		if(pCMB == NULL)
			return;

		POSITION pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CStationObj * pObj = (CStationObj*)listDevice.GetNext(pos);
			if(pObj != NULL)
			{
				if(pObj ->m_sName.GetLength() < length)
					continue;
				CString strTemp = pObj->m_sName;
				for (int i=0; i<strTemp.GetLength(); i++)
				{
					int a = (TBYTE)strTemp[i];
					if ( !IsChinese(strTemp) )
						strTemp = strTemp.Right(strTemp.GetLength() - 1);
					else
						break;
				}
				if(strTemp.GetLength() < length)
					continue;
				//strTemp = strTemp.Left(length);
				
				if(/*strTemp == str*/strTemp.Find(str, 0) != -1)
				{			
					pCMB ->AddString(pObj ->m_sName);
					int Index = pCMB ->FindString(0,pObj ->m_sName);
					if(Index != -1)
					{
						pCMB ->SetItemDataPtr(Index, pObj);
					}
				}
			}
		}
	}
	else
	{
		//拼音
		int length = str.GetLength();
		str.MakeUpper();
		CComboBox * pCMB = (CComboBox *)GetDlgItem(IDC_COMBO_DEVICE);
		if(pCMB == NULL)
			return;
		POSITION pos = listDevice.GetHeadPosition();
		while(pos != NULL)
		{
			CStationObj * pObj = (CStationObj*)listDevice.GetNext(pos);
			if(pObj == NULL)
				continue;
			if(pObj ->m_sName.GetLength() < length)
				continue;
			CString strTemp;
			GetFirstLetter(pObj ->m_sName, strTemp);
			CString strPY = strTemp.Left(length);
			
			if(/*strPY == str*/strTemp.Find(str, 0) != -1)
			{
				//加入到下拉框				
				pCMB ->AddString(pObj ->m_sName);
				int Index = pCMB ->FindString(0,pObj ->m_sName);
				if(Index != -1)
				{
					pCMB ->SetItemDataPtr(Index, pObj);
				}
			}
		}
	}
	CComboBox * pCMB = (CComboBox *)GetDlgItem(IDC_COMBO_DEVICE);
	if(pCMB != NULL)
	{
		::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)));
		pCMB ->ShowDropDown(TRUE);
	}
}

void CDeviceView::OnEditchangeComboDevice() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CComboBox * pCMB = (CComboBox *)GetDlgItem(IDC_COMBO_DEVICE);
	if(pCMB != NULL)
	{
		//清空
		//	pCMB ->ResetContent();
		int count = pCMB ->GetCount();
		for (int i=0;i < count; i++)
		{
			pCMB->DeleteString( 0 );
		}
		
		//进行比较,填充下拉框
		CompareStation(m_strSelect);
	}
}

void CDeviceView::OnSelendokComboDevice() 
{
	// TODO: Add your control notification handler code here
	CComboBox * pCMB = (CComboBox *)GetDlgItem(IDC_COMBO_DEVICE);
	if(pCMB != NULL)
	{
		int Index = pCMB ->GetCurSel();
		if(Index != -1)
		{
			//得到厂站指针
			CStationObj * pObj = (CStationObj *)pCMB ->GetItemDataPtr(Index);
			//设备树定位
			LocateInTree(pObj);
		}
		//清空
		pCMB ->ResetContent();
	}
}

void CDeviceView::OnTreeStationProp() 
{
	// TODO: Add your command handler code here
	HTREEITEM ht = m_Tree.GetSelectedItem();
	if(ht == NULL)
		return;
	//判断是否厂站
	CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
		return;
	if(pObj->m_nType == TYPE_STATION)
	{
		CStationObj * pStation = (CStationObj*)pObj;
		
		CDlgStationProp dlg;
		dlg.m_pStation = pStation;
		dlg.DoModal();
	}
}

/****************************************************
Date:2011/12/16  Author:LYH
函数名:   ClearEmptyNodeType4	
返回值:   void	
功能概要: 
*****************************************************/
void CDeviceView::ClearEmptyNodeType4()
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
			HTREEITEM htTemp = htLevel2;
			htLevel2 = m_Tree.GetNextItem(htLevel2, TVGN_NEXT);
			if(!m_Tree.ItemHasChildren(htTemp))
				m_Tree.DeleteItem(htTemp);
		}
		HTREEITEM htTempNet = htLevel1;
		htLevel1 = m_Tree.GetNextItem(htLevel1, TVGN_NEXT);
		if(!m_Tree.ItemHasChildren(htTempNet))
			m_Tree.DeleteItem(htTempNet);
	}
}

/****************************************************
Date:2012/2/9  Author:LYH
函数名:   OnViewRefresh	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CDeviceView::OnViewRefresh( WPARAM wParam, LPARAM lParam )
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
		OnStationChanged(NULL, (LPARAM)pObj);
	}

	WriteLog("CDeviceView::OnViewRefresh, 设备树强制刷新完毕", XJ_LOG_LV3);
}

void CDeviceView::OnTreeStationAdd() 
{
	// TODO: Add your command handler code here
	try
	{
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
       	//检查通讯情况
		if(!pApp->GetSTTPStatus())
		{
			AfxMessageBox(STTP_DISCONNECT);
			return;
		}

		STTP_FULL_DATA sttpData;
		CDataEngine* pData = pApp->GetDataEngine();
		CString strTemp = "";
		
		HTREEITEM ht = m_Tree.GetSelectedItem();
		if(ht == NULL)
			return;
		//判断是否厂站
		CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
		if(pObj == NULL)
			return;
		if(pObj->m_nType != TYPE_STATION)
			return;

		CStationObj* pStationObj = (CStationObj*)pObj;
		MYASSERT(pStationObj);

		CStationObj* pSep = (CStationObj*)pData->FindSepByStation(pStationObj->m_sID);
		CString sSepID = "";
		if(pSep != NULL)
			sSepID = pSep->m_sID;
		
		if (pApp->m_SttpEngine.BuildDataFor20138StationAdd(sttpData, pStationObj->m_sID, sSepID))
		{
			int result = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			if (result)
			{
				strTemp = StringFromID(IDS_NEWSTATION_NOTIFY_SUCCESS);
				AfxMessageBox(strTemp);
				WriteLog(strTemp);
			}
			else
			{
				strTemp = StringFromID(IDS_NEWSTATION_NOTIFY_FAIL);
				AfxMessageBox(strTemp);
				WriteLog(strTemp);
			}
		}
		else
		{
			strTemp.Format(StringFromID(IDS_STATIONCHANGE_BUILD_FAIL));
			AfxMessageBox(strTemp);
			WriteLog(strTemp);
		}	
	}
	catch (...)
	{
		AfxMessageBox(StringFromID(IDS_NEWSTATION_NOTIFY_FAIL));		
	}
}

void CDeviceView::OnDevTreePTADD() 
{
	// TODO: Add your command handler code here
	try
	{
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
       	//检查通讯情况
		if(!pApp->GetSTTPStatus())
		{
			AfxMessageBox(STTP_DISCONNECT);
			return;
		}
		
		STTP_FULL_DATA sttpData;
		CDataEngine* pData = pApp->GetDataEngine();
		CString strTemp = "";
		
		HTREEITEM ht = m_Tree.GetSelectedItem();
		if(ht == NULL)
			return;
		//判断是否厂站
		CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
		if(pObj == NULL)
			return;
		if(pObj->m_nType != TYPE_PROTECT && pObj->m_nType != TYPE_WAVEREC)
			return;
		
		CSecObj* pSec = (CSecObj*)pObj;
		MYASSERT(pSec);
		
		if (pApp->m_SttpEngine.BuildDataFor20138SecConfigChange(sttpData, pSec->m_sStationID, pSec->m_sID, TRUE))
		{
			int result = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			if (result)
			{
				strTemp = StringFromID(IDS_NEWDEV_NOTIFY_SUCCESS);
				AfxMessageBox(strTemp);
				WriteLog(strTemp);
			}
			else
			{
				strTemp = StringFromID(IDS_NEWDEV_NOTIFY_FAIL);
				AfxMessageBox(strTemp);
				WriteLog(strTemp);
			}
		}
		else
		{
			strTemp.Format(StringFromID(IDS_STATIONCHANGE_BUILD_FAIL));
			AfxMessageBox(strTemp);
			WriteLog(strTemp);
		}	
	}
	catch (...)
	{
		AfxMessageBox(StringFromID(IDS_NEWDEV_NOTIFY_FAIL));		
	}
}

void CDeviceView::OnDevTreePTCONFIGCHANGED() 
{
	// TODO: Add your command handler code here
	try
	{
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
       	//检查通讯情况
		if(!pApp->GetSTTPStatus())
		{
			AfxMessageBox(StringFromID(IDS_STTP_DISCONNECT));
			return;
		}
		
		STTP_FULL_DATA sttpData;
		CDataEngine* pData = pApp->GetDataEngine();
		CString strTemp = "";
		
		HTREEITEM ht = m_Tree.GetSelectedItem();
		if(ht == NULL)
			return;
		//判断是否厂站
		CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
		if(pObj == NULL)
			return;
		if(pObj->m_nType != TYPE_PROTECT && pObj->m_nType != TYPE_WAVEREC)
			return;
		
		CSecObj* pSec = (CSecObj*)pObj;
		MYASSERT(pSec);
		
		if (pApp->m_SttpEngine.BuildDataFor20138SecConfigChange(sttpData, pSec->m_sStationID, pSec->m_sID))
		{
			int result = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
			if (result)
			{
				strTemp = StringFromID(IDS_STATIONCHANGE_NOTIFY_SUCCESS);
				AfxMessageBox(strTemp);
				WriteLog(strTemp);
			}
			else
			{
				strTemp = StringFromID(IDS_STATIONCHANGE_NOTIFY_FAIL);
				AfxMessageBox(strTemp);
				WriteLog(strTemp);
			}
		}
		else
		{
			strTemp.Format(StringFromID(IDS_STATIONCHANGE_BUILD_FAIL));
			AfxMessageBox(strTemp);
			WriteLog(strTemp);
		}	
	}
	catch (...)
	{
		AfxMessageBox(StringFromID(IDS_STATIONCHANGE_NOTIFY_FAIL));		
	}
}

void CDeviceView::OnMarkDevice() 
{
	// TODO: Add your command handler code here
	HTREEITEM htItem = m_Tree.GetSelectedItem();
	if(htItem == NULL)
		return;
	//取得
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//是保护
		CSecObj* pObj = (CSecObj*)pDevice;
		pObj->RefreshConfig();
		DLGMarked dlg;
		dlg.m_pObj = pObj;
		dlg.DoModal();
	}
	
}

void CDeviceView::OnMarkPtdevice() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	HTREEITEM htItem = m_Tree.GetSelectedItem();
	if(htItem == NULL)
		return;
	//取得
	CSecObj * pDevice = (CSecObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_WAVEREC)
	{
		//2 dlg
		DLGMarked dlg;
		dlg.m_pObj = pDevice;
		dlg.DoModal();
	}
}
/*************************************************************
 函 数 名: InsertPTDevice()
 功能概要: 加入保护设备到设备树
 返 回 值: 加入成功返回TRUE, 失败返回FALSE
 参    数: param1 指定父节点
		   Param2 指定设备
**************************************************************/
BOOL CDeviceView::InsertPTDevice(HTREEITEM htParent, CDeviceObj* pObj)
{
		//检查数据有效性
	if(htParent == NULL)
		return FALSE;
	if(pObj == NULL)
		return FALSE;

	CSecObj* pSec=(CSecObj*)pObj;
	HTREEITEM htNew;
	if (pSec->m_nSuspendstatus == 1)
	{
		htNew = m_Tree.InsertItem(pSec->m_sName, IMAGEINDEX_PROTECT2, IMAGEINDEX_SELSEC/*IMAGEINDEX_PROTECT2*/, htParent);	
		
	}
	else
	{
		htNew = m_Tree.InsertItem(pSec->m_sName, IMAGEINDEX_PROTECT1, IMAGEINDEX_SELSEC/*IMAGEINDEX_PROTECT1*/, htParent);
	}
	if(htNew == NULL)
		return FALSE;
	//记录节点位置
	pObj->m_htItem = htNew;
	//关联数据
		m_Tree.SetItemData(htNew, (DWORD)pObj);
	return TRUE;
}

/****************************************************
Date:2013/6/4  Author:LYH
函数名:   OnStationInit	
返回值:   void	
功能概要: 
参数: WPARAM wParam	
参数: LPARAM lParam	
*****************************************************/
void CDeviceView::OnStationInit( WPARAM wParam, LPARAM lParam )
{
	FillTree();
	FillList();
}

void CDeviceView::OnTreeStationTest() 
{
	// TODO: Add your command handler code here
	HTREEITEM ht = m_Tree.GetSelectedItem();
	if(ht == NULL)
		return;
	//判断是否厂站
	CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
		return;
	if(pObj->m_nType == TYPE_STATION)
	{
		CStationObj * pStation = (CStationObj*)pObj;
		
		TestStationStatus(pStation);
	}
}

void CDeviceView::BuildDistance( CString sNetID, HTREEITEM htParent )
{
	//检查数据有效性
	if(htParent == NULL)
		return;
	
	//循环,加入到设备
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine * pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	DEVICE_LIST listDevice;
	pData->GetSecList(listDevice, "", TYPE_SEC, "", 21);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if ( pObj != NULL && pObj->m_pStation != NULL && pObj->m_pStation->m_sNetID == sNetID )
		{
			InsertDevice(htParent, pObj);
		}
	}
}

/****************************************************
Date:2013/10/8  Author:LYH
函数名:   TestStationStatus	
返回值:   void	
功能概要: 
参数: CStationObj * pStation	
*****************************************************/
void CDeviceView::TestStationStatus( CStationObj* pStation )
{
	if(!pStation)
		return;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_arrCall.GetSize() > 0)
	{
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
		if(pCall->pStation != pStation)
		{
			AfxMessageBox(StringFromID(IDS_TESTSTATION_BUSY));
		}
		
		return;
	}
	
	if (g_bVerify)
	{
		//验证权限
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	
	//检查通讯情况
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(StringFromID(IDS_STATIONCHANGE_NOTIFY_FAIL));
		return;
	}
	
	m_sResult = "";
	
	//查询厂站状态
	if(pStation->m_nIPFlag == 0) //单通道
	{
		AddToCallList(pStation, 0);
	}
	else //双通道
	{
		AddToCallList(pStation, 0);
		AddToCallList(pStation, 1);
	}
	
	//开始召唤
	if(m_arrCall.GetSize() > 0)
	{
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
		if(pCall != NULL)
		{
			StartCall(pCall);
		}
	}
}

/*************************************************************
 函 数 名: QueryStationStatus()
 功能概要: 查询厂站通讯状态
 返 回 值: int 失败返回<0的整数，成功返回0 
 参    数: param1 指定厂站
		   Param2 指定通道
**************************************************************/
int CDeviceView::QueryStationStatus( CStationObj* pObj, int nChannel /*= 0*/ )
{
	//检测正确性
	MYASSERT_INT(pObj);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	//组织20145报文
	if(!pApp->m_SttpEngine.BuildDataFor20145StationStatus(sttpData, pObj->m_sID, nChannel))
	{
		//组织报文失败
		if(m_arrCall.GetSize() > 0)
		{
			StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
			if(pCall != NULL)
			{
				pCall->nRelust = -2;
			}
		}
		SendMessage(COMMVIEW_QUERY_END);
		return -1;
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
			str.Format("厂站%s发送召唤状态报文失败,原因为连接中断", pObj->m_sID);
			WriteLog(str);
			if(m_arrCall.GetSize() > 0)
			{
				StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
				if(pCall != NULL)
				{
					pCall->nRelust = -2;
				}
			}
		}
		else if(nReturn == -2)
		{
			CString str;
			str.Format("厂站%s发送召唤状态报文失败,原因为缓存已满", pObj->m_sID);
			WriteLog(str);
			if(m_arrCall.GetSize() > 0)
			{
				StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
				if(pCall != NULL)
				{
					pCall->nRelust = -2;
				}
			}
		}else if(nReturn == 0 ){
			CString str;
			str.Format("厂站%s发送召唤状态报文失败,原因为未找到对应服务器", pObj->m_sID);
			WriteLog(str);
			if(m_arrCall.GetSize() > 0)
			{
				StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
				if(pCall != NULL)
				{
					pCall->nRelust = -2;
				}
			}
		}
		else{
			CString str;
			str.Format("厂站%s发送召唤状态报文失败,原因未知", pObj->m_sID);
			WriteLog(str);
			if(m_arrCall.GetSize() > 0)
			{
				StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
				if(pCall != NULL)
				{
					pCall->nRelust = -2;
				}
			}
		}
		SendMessage(COMMVIEW_QUERY_END);
		return -1;
	}
	else
	{
		//启动定时器
		CDataEngine * pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nDevGeneralTimeOut;
		m_nQueryTimer = SetTimer(2, nTimeOut*1000, 0);
	}
	return 0;
}

/*************************************************************
 函 数 名: OnSTTP20146()
 功能概要: 响应20146(厂站通讯状态下载通知)
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDeviceView::OnSTTP20146( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CDeviceView::OnSTTP20146");
	if(m_arrCall.GetSize() <= 0)
	{
		//没有在召唤厂站通讯状态,不处理报文
		return;
	}
	
	//取得报文
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//报文为空
		return;
	}
	
	//检查是否20146报文
	if(pSttpData->sttp_head.uMsgID != 20146)
	{
		//错误报文
		CString str;
		str.Format("CDeviceView::OnSTTP20146 收到错误报文,报文ID为%d", pSttpData->sttp_head.uMsgID);
		WriteLog(str);
		return;
	}

	StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
	if(pCall != NULL)
	{
		pCall->nRelust = 0;
	}
	else
		return;
	MYASSERT(pCall->pStation); 
	
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_station_id;
	if(strID != pCall->pStation->m_sID)
	{
		//不是本装置的报文
		CString str;
		str.Format("CDeviceView::OnSTTP20146 收到其它厂站报文, 厂站ID为%s", strID);
		WriteLog(str);
		return;
	}

	int nChannel = pSttpData->sttp_body.nFlag;
	if(nChannel != pCall->nChannel)
	{
		//不是本通道的报文
		CString str;
		str.Format("CDeviceView::OnSTTP20146 收到其它通道的报文, 通道为%d", nChannel);
		WriteLog(str);
		return;
	}
	
	//关闭定时器
	KillTimer(m_nQueryTimer);
	
	//处理报文
	
	int nStatus = pSttpData->sttp_body.nStatus;
	int nReason = pSttpData->sttp_body.nEventType;

	if ((nChannel == 0 && nStatus != pCall->pStation->m_nCommStatus) || (nChannel == 1 && nStatus != pCall->pStation->m_nCommStatus1))
	{
		CString strStationTime = pSttpData->sttp_body.ch_time_12_BIT1;
		CString strReasonTime = pSttpData->sttp_body.ch_time_12_BIT2;
		CTime tmStation;
		CTime tmReason;
		if (strStationTime == "" || strReasonTime == "")
		{
			//处理完毕
			//SendMessage(COMMVIEW_QUERY_END);
			// 如果时间为空,还需设置通讯状态，此处略
			WriteLog("CDeviceView::OnSTTP20146, 收到时间为空");
			tmStation =  CTime::GetCurrentTime();
			tmReason =  CTime::GetCurrentTime();
		}
		else
		{
			tmStation = StringToTimeSttp12(strStationTime);
			tmReason = StringToTimeSttp12(strReasonTime);
		}
		pCall->pStation->SetStatus(nStatus, tmStation, nReason, nChannel, tmReason, pCall->pStation->m_nIPFlag);
		
		//通知设备树和图形界面
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CMainFrame* pFrame=(CMainFrame*)pApp->GetMainWnd();
		if(pFrame != NULL)
		{
			WriteLog("CDeviceView::OnSTTP20146, 状态变位，发送通知");
			pFrame->PostMessage(STATION_CHANGED, 0, (LPARAM)pCall->pStation);
		}
	}
	else
	{
		WriteLog("CDeviceView::OnSTTP20146, 状态未变位，不发送通知");
	}
	
	//处理完毕
	SendMessage(COMMVIEW_QUERY_END);
}

/*************************************************************
 函 数 名: OnSTTP20069()
 功能概要: 响应20069(召唤失败通知)
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDeviceView::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//当前没有在召唤
	if(m_arrCall.GetSize() <= 0)
	{
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
	
	StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
	if(pCall != NULL)
	{
		pCall->nRelust = -2;
	}
	else
		return;

	if(pCall->pStation == NULL)
		return;
	//检查是否自己装置的报文
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != pCall->pStation->m_sID)
	{
		//不是本装置的报文
		return;
	}
	
	//关闭定时器
	KillTimer(m_nQueryTimer);
	
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
		str.Format("厂站 %s 召唤通讯状态失败!", pCall->pStation->m_sID);
		WriteLog(str);
	}
	
	//处理完毕
	SendMessage(COMMVIEW_QUERY_END);
}

/*************************************************************
 函 数 名: ClearCallList()
 功能概要: 清空召唤列表
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDeviceView::ClearCallList()
{
	for(int i = 0; i < m_arrCall.GetSize(); i++)
	{
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(i);
		MYASSERT_CONTINUE(pCall);
		MYDELETE(pCall);
	}
	m_arrCall.RemoveAll();
}

/*************************************************************
 函 数 名: AddToCallList()
 功能概要: 将指定厂站指定通道加入到待召唤列表
 返 回 值: 
 参    数: param1 指定厂站
		   Param2 指定通道
**************************************************************/
void CDeviceView::AddToCallList( CStationObj* pObj, int nChannel )
{
	MYASSERT(pObj);
	StationStauts_Call* pNew = new StationStauts_Call;
	pNew->pStation = pObj;
	pNew->nChannel = nChannel;
	m_arrCall.Add(pNew);
}

/*************************************************************
 函 数 名: StartCall()
 功能概要: 对召唤项进行召唤
 返 回 值: 
 参    数: param1 指定召唤项
		   Param2 
**************************************************************/
void CDeviceView::StartCall( StationStauts_Call* pCall)
{
	MYASSERT(pCall);
	if(QueryStationStatus(pCall->pStation, pCall->nChannel) < 0)
	{
		//进行查询失败
		pCall->nRelust = -1;
	}
	else
	{
		//进行查询成功
		AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_SUCCESS));
	}
}

/*************************************************************
 函 数 名: OnEndCallStationStatus()
 功能概要: 响应召唤完成消息
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void CDeviceView::OnEndCallStationStatus( WPARAM wParam, LPARAM lParam )
{
	//取得列表中的第一个对象
	if(m_arrCall.GetSize() <= 0)
	{
		return;
	}
	StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
	MYASSERT(pCall);
	CStationObj* pStation = pCall->pStation;
	MYASSERT(pStation);
	//召唤结果
	CString str;
	if(pCall->nRelust == 0)
	{
		if(pCall->nChannel == 0)
			str.Format("%s:%s, %s:%d, %s:%s\n", StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, StringFromID(IDS_COMMU_CHANNEL), 
			pCall->nChannel, StringFromID(IDS_COMMU_STATUS), pCall->pStation->m_nCommStatus==0?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
		else if(pCall->nChannel == 1)
			str.Format("%s:%s, %s:%d, %s:%s\n", StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, StringFromID(IDS_COMMU_CHANNEL), 
				pCall->nChannel, StringFromID(IDS_COMMU_STATUS), pCall->pStation->m_nCommStatus1==0?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
	}
	else if(pCall->nRelust == -1)
	{
		str.Format("%s:%s, %s:%d, %s\n", StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, StringFromID(IDS_COMMU_CHANNEL), pCall->nChannel, StringFromID(IDS_CALLFAIL_SEND));
	}
	else if(pCall->nRelust == -2)
	{
		str.Format("%s:%s, %s:%d, %s\n", StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, StringFromID(IDS_COMMU_CHANNEL), pCall->nChannel, StringFromID(IDS_CALLFAIL_RESPONSE));
	}
	else if(pCall->nRelust == -3)
	{
		str.Format("%s:%s, %s:%d, %s\n", StringFromID(IDS_MODEL_SUBSTATION), pCall->pStation->m_sName, StringFromID(IDS_COMMU_CHANNEL), pCall->nChannel, StringFromID(IDS_CALLFAIL_TIMEOUT));
	}
	m_sResult += str;
	m_arrCall.RemoveAt(0);
	
	//判断是否召唤全部结束
	if(m_arrCall.GetSize() > 0)
	{
		//继续召唤
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
		StartCall(pCall);
	}
	else
	{
		//召唤结束
		AfxMessageBox(m_sResult);
	}
	//删除第一项
	MYDELETE(pCall);
}

void CDeviceView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		//KillTimer(m_nTimer);

		CXJPTSetStore *p = CXJPTSetStore::GetInstance();
		QPTSetCard &card = *(reinterpret_cast<QPTSetCard *>(p->GetCard()));
		QLogTable &log = *(reinterpret_cast<QLogTable *>(p->GetLog()));
		
		int nState = card.GetStateID();
		if (-1 == m_nDZ_MOD_State){
			m_nDZ_MOD_State = nState;
		}

		// 只在挂牌和取消挂牌时刷新
		if (nState != m_nDZ_MOD_State && (0 == nState || 1 == nState)){
			m_nDZ_MOD_State = nState;

			if(pApp->GetDataEngine() != NULL)
			{
				KillTimer(m_nTimer);
				
				pApp->GetDataEngine()->RefreshAllSecFromDB();
				FillTree();
				CXJBrowserView * pView = pApp->GetSVGView();

				CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(card.GetPTID().data(), TYPE_SEC);
				if (pObj){
					pObj->m_nRunStatu = 5;
					pView->SetDeviceCol(pObj);
				}
				
				m_nTimer = SetTimer(101, 3*1000, 0);
			}
		}

	}
}
