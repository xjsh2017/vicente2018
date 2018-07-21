// DlgAutoCallStop.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgAutoCallStop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoCallStop dialog


//##ModelId=49B87BA402FF
CDlgAutoCallStop::CDlgAutoCallStop(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoCallStop::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoCallStop)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hRoot = NULL;
	m_nStationNum = 0;
	m_pAutoCall = NULL;
}


//##ModelId=49B87BA4032C
void CDlgAutoCallStop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoCallStop)
	DDX_Control(pDX, IDC_LIST_AUOTCALL_STOP, m_List);
	DDX_Control(pDX, IDC_TREE_AUTOCAL_STOP, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoCallStop, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoCallStop)
	ON_NOTIFY(NM_CLICK, IDC_TREE_AUTOCAL_STOP, OnClickTreeAutocalStop)
	ON_BN_CLICKED(IDC_BTN_AUTOCALL_STOP, OnBtnAutocallStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*************************************************************
 函 数 名：InitTree()
 功能概要：初始化设备树
 返 回 值: 初始化成功返回>=0的值, 失败返回<0的值
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA4032F
int CDlgAutoCallStop::InitTree()
{
	//清空
	m_Tree.DeleteAllItems();
	//创建根节点
	m_hRoot = m_Tree.InsertItem(STRING_NET, NULL);
	//创建地区
	BuildNet();
	//创建厂站
	BuildStation();
	return 0;
}

/*************************************************************
 函 数 名：BuildNet()
 功能概要：在设备树中创建地区
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40330
void CDlgAutoCallStop::BuildNet()
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
 函 数 名：InsertNet()
 功能概要：插入地区节点
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1	要插入的地区对象
		   Param2
**************************************************************/
//##ModelId=49B87BA4033C
BOOL CDlgAutoCallStop::InsertNet( CNetObj* pObj )
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
	pObj->m_Tag2 = htNew;
	//设置数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//排序
	//m_Tree.SortChildren(htRoot);
	return TRUE;
}

/*************************************************************
 函 数 名：InsertStation()
 功能概要：插入厂站节点
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1	要插入的厂站对象
		   Param2
**************************************************************/
//##ModelId=49B87BA4033E
BOOL CDlgAutoCallStop::InsertStation( CStationObj* pObj )
{
	//厂站节点在地区节点下面, 为第三层
	//先查找地区节点
	HTREEITEM htNet = FindNetPos(pObj->m_sNetID);
	if(htNet == NULL)
		return FALSE;
	
	//插入节点
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, htNet);
	//记录节点
	pObj->m_Tag2 = htNew;
	//设备数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//排序
	m_Tree.SortChildren(htNet);

	return TRUE;
}

/*************************************************************
 函 数 名：BuildStation()
 功能概要：在设备树中创建厂站
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40340
void CDlgAutoCallStop::BuildStation()
{
	//取得数据引擎
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
		m_nStationNum++;
		//取得地区, 加入树
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj != NULL && !pObj->m_IsDistanceVirtual && pObj->m_nStationType != 113 && pObj->m_nStationType != 114 )
			InsertStation(pObj);
	}
}

/*************************************************************
 函 数 名：FindNetPos()
 功能概要：查找地区在树中的位置
 返 回 值: 树节点结构
 参    数：param1	地区ID
		   Param2
**************************************************************/
//##ModelId=49B87BA4034B
HTREEITEM CDlgAutoCallStop::FindNetPos( CString sNetID )
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
 函 数 名：CycTreeDown()
 功能概要：向下遍历指定节点的所有子节点, 对所有子节点做相同的选择操作
 返 回 值: void
 参    数：param1	指定节点
		   Param2	选择标志.TRUE为选中, FALSE为取消选择
**************************************************************/
//##ModelId=49B87BA4034D
void CDlgAutoCallStop::CycTreeDown( HTREEITEM hRoot, BOOL bMark )
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
//##ModelId=49B87BA40350
void CDlgAutoCallStop::CycTreeUp( HTREEITEM hCurrent )
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
//##ModelId=49B87BA4035C
void CDlgAutoCallStop::HitItemHelp( HTREEITEM hit )
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
/////////////////////////////////////////////////////////////////////////////
// CDlgAutoCallStop message handlers

//##ModelId=49B87BA4037A
BOOL CDlgAutoCallStop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//初始化设备树
	InitTree();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 函 数 名：CheckDevice()
 功能概要：对某设备进行了Check操作
 返 回 值: void
 参    数：param1	设备指针
		   Param2	1表示选中, 0表示取消选择
**************************************************************/
//##ModelId=49B87BA4035E
void CDlgAutoCallStop::CheckDevice( CDeviceObj* pObj, int nSelected )
{
	//检查数据有有性
	if(pObj == NULL)
		return;
	if(nSelected != 0 && nSelected != 1) //标志值无效
		return;
	if(pObj->m_nType != TYPE_STATION) //如果不是厂站,此类设备无效
		return;
	if(1 == nSelected)
	{
		//选中
		ListAddStation(pObj);
	}
	if(0 == nSelected)
	{
		//取消选择
		ListRemoveStation(pObj);
	}
}

//##ModelId=49B87BA4037C
void CDlgAutoCallStop::OnClickTreeAutocalStop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	UINT uFlags = 0;
	HWND hWnd ;
	
	GetCursorPos(&pt);
	hWnd = ::WindowFromPoint(pt);
	::ScreenToClient(hWnd, &pt);
	
	HTREEITEM  hit = m_Tree.HitTest(pt, &uFlags);
	if(uFlags & TVHT_ONITEMSTATEICON )
	{
		HitItemHelp(hit);
	}

	*pResult = 0;
}

/*************************************************************
 函 数 名：ListAddStation()
 功能概要：把厂站加入到列表中显示
 返 回 值: 
 参    数：param1	设备指针
		   Param2
**************************************************************/
//##ModelId=49B87BA4036B
void CDlgAutoCallStop::ListAddStation( CDeviceObj* pObj )
{
	//检查数据有效性
	if(pObj == NULL)
		return;
	if(m_List.GetSafeHwnd() == NULL)
		return;

	//查找在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pObj;
	int nIndex = m_List.FindItem(&lvFind);

	if(nIndex > -1)
	{
		//已存在
		return;
	}

	//增加
	nIndex = m_List.InsertItem(0, pObj->m_sName);
	//关联对象
	m_List.SetItemData(nIndex, (DWORD)pObj);
}

/*************************************************************
 函 数 名：ListRemoveStation()
 功能概要：把厂站从列表中移除
 返 回 值: 
 参    数：param1	设备指针
		   Param2
**************************************************************/
//##ModelId=49B87BA4036D
void CDlgAutoCallStop::ListRemoveStation( CDeviceObj* pObj )
{
	//检查数据有效性
	if(pObj == NULL)
		return;
	if(m_List.GetSafeHwnd() == NULL)
		return;
	
	//查找在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pObj;
	int nIndex = m_List.FindItem(&lvFind);
	
	if(nIndex == -1)
	{
		//不存在
		return;
	}

	//移除
	m_List.DeleteItem(nIndex);
}

/*************************************************************
 函 数 名：ConnectAutoCall()
 功能概要：与自动总召模块建立连接
 返 回 值: 连接成功返回1, 失败返回0
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA4036F
int CDlgAutoCallStop::ConnectAutoCall()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//组建客户端结构
	STRUCT_NETADDR_INFO addrInfo;
	bzero(&addrInfo, sizeof(STRUCT_NETADDR_INFO));
	strncpy(addrInfo.IpAddr, g_AutoCall_IP, 40);
	addrInfo.nPort = g_AutoCall_Port;
	STRUCT_SERVER_INFO serverInfo;
	bzero(&serverInfo, sizeof(STRUCT_SERVER_INFO));
	serverInfo.bHasReservedAddr = false;
	serverInfo.netAddrA = addrInfo;
	STRUCT_DESTINATION_SERVER destInfo;
	bzero(&destInfo, sizeof(STRUCT_DESTINATION_SERVER));
	destInfo.bHasReservedServer = false;
	destInfo.serverA = serverInfo;
	//连接
	int nResult = 0;
	CString sSysLogPath = pApp->GetSysLogPath();
	char * pPath = (char*)sSysLogPath.GetBuffer(0);
	m_pAutoCall = pApp->m_SttpEngine.XJSTTPConnectDestination(destInfo, STTPNET_MODLE_STFRONT, nResult, pPath, STTPNET_RUNMODLE_SYNC);
	sSysLogPath.ReleaseBuffer(0);
	if(nResult == 0)
	{
		if(m_pAutoCall != NULL)
		{
			//断开
			pApp->m_SttpEngine.XJSttpCloseConnection(m_pAutoCall);
		}
		m_pAutoCall = NULL;
	}
	return nResult;
}

/*************************************************************
 函 数 名：DisConnectAutoCall()
 功能概要：关闭与自动总召的连接
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40370
void CDlgAutoCallStop::DisConnectAutoCall()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pAutoCall != NULL)
	{
		pApp->m_SttpEngine.XJSttpCloseConnection(m_pAutoCall);
		m_pAutoCall = NULL;
	}
}

//##ModelId=49B87BA40380
void CDlgAutoCallStop::OnBtnAutocallStop() 
{
	// TODO: Add your control notification handler code here
	int nNum = m_List.GetItemCount();
	if(nNum < 1)
	{
		//没有选择厂站
		AfxMessageBox( StringFromID(IDS_STOPAUTOCALL_SELECTSTATION));
		return;
	}
	
	//组织报文
	CStringArray arrStation;
	for(int i = 0; i < m_List.GetItemCount(); i++)
	{
		CDeviceObj* pObj = (CDeviceObj*)m_List.GetItemData(i);
		arrStation.Add(pObj->m_sID);
	}
	STTP_FULL_DATA sttpData;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(nNum == m_nStationNum)
	{
		//全部厂站
		if(!pApp->m_SttpEngine.BuildDataFor20153Batch(sttpData, 3, NULL))
		{
			AfxMessageBox( StringFromID(IDS_COMMON_BUILDMSG_FAIL));
			return; //组建报文失败
		}
	}
	else
	{
		if(!pApp->m_SttpEngine.BuildDataFor20153Batch(sttpData, 3, &arrStation))
		{
			AfxMessageBox( StringFromID(IDS_COMMON_BUILDMSG_FAIL));
			return; //组建报文失败
		}
	}
	
	//发送报文
	int nResult = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	if(nResult != 1)
	{
		AfxMessageBox( StringFromID(IDS_STOPAUTOCALL_SENDFAIL));
		return; //发送失败
	}
	//
	AfxMessageBox( StringFromID(IDS_STOPAUTOCALL_SENDSUCCESS));
}
