// ACResultSel.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ACResultSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CACResultSel

IMPLEMENT_DYNCREATE(CACResultSel, CFormView)

//##ModelId=49B87BB50205
CACResultSel::CACResultSel()
	: CFormView(CACResultSel::IDD)
{
	//{{AFX_DATA_INIT(CACResultSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hRoot = NULL;
}

//##ModelId=49B87BB50227
CACResultSel::~CACResultSel()
{
}

//##ModelId=49B87BB50224
void CACResultSel::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACResultSel)
	DDX_Control(pDX, IDC_TREE_ACRESULTSEL, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CACResultSel, CFormView)
	//{{AFX_MSG_MAP(CACResultSel)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ACRESULTSEL, OnSelchangedTreeAcresultsel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(AUTOCALL_RESULT_OPEN, OnAutoCallResultFrameOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACResultSel diagnostics

#ifdef _DEBUG
//##ModelId=49B87BB50229
void CACResultSel::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87BB5022B
void CACResultSel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/*************************************************************
 函 数 名：InsertNet()
 功能概要：插入地区节点
 返 回 值: 插入成功返回TRUE, 失败返回FALSE
 参    数：param1	要插入的地区对象
		   Param2
**************************************************************/
//##ModelId=49B87BB50213
BOOL CACResultSel::InsertNet( CNetObj* pObj )
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
	//记录位置
	pObj->m_Tag1 = htNew;
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
//##ModelId=49B87BB50215
BOOL CACResultSel::InsertStation( CStationObj* pObj )
{
	//厂站节点在地区节点下面, 为第三层
	//先查找地区节点
	HTREEITEM htNet = FindNetPos(pObj->m_sNetID);
	if(htNet == NULL)
		return FALSE;
	
	//插入节点
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, htNet);
	//记录节点
	//	pObj->m_htItem = htNew;
	pObj->m_Tag1 = htNew;
	//设备数据
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//排序
	m_Tree.SortChildren(htNet);
	return TRUE;
}

/*************************************************************
 函 数 名：FindNetPos()
 功能概要：查找地区在树中的位置
 返 回 值: 树节点结构
 参    数：param1	地区ID
		   Param2
**************************************************************/
//##ModelId=49B87BB50217
HTREEITEM CACResultSel::FindNetPos( CString sNetID )
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
 函 数 名：RegulateControlSize()
 功能概要：调整控件大小,位置
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB50219
void CACResultSel::RegulateControlSize()
{
	if(m_Tree.GetSafeHwnd() == NULL)
		return;
	
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	//调整设备树大小
	CRect rcTree = rc;
	m_Tree.MoveWindow(rcTree);
}


/////////////////////////////////////////////////////////////////////////////
// CACResultSel message handlers

//##ModelId=49B87BB50233
void CACResultSel::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 函 数 名：BuildNet()
 功能概要：在设备树中创建地区
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5021A
void CACResultSel::BuildNet()
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
 函 数 名：BuildStation()
 功能概要：在设备树中创建厂站
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5021B
void CACResultSel::BuildStation()
{
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//增加所有厂站
	DEVICE_LIST listStation;
	listStation.RemoveAll();
	pData->GetStationList(listStation);
	POSITION pos = listStation.GetHeadPosition();
	while(pos != NULL)
	{
		//取得地区, 加入树
		CStationObj* pObj = (CStationObj*)listStation.GetNext(pos);
		if(pObj != NULL && !pObj->m_IsDistanceVirtual && pObj->m_nStationType != 113 && pObj->m_nStationType != 114 )
			InsertStation(pObj);
	}
}

/*************************************************************
 函 数 名：InitTree()
 功能概要：初始化设备树
 返 回 值: 初始化成功返回>=0的值, 失败返回<0的值
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5021C
int CACResultSel::InitTree()
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

//##ModelId=49B87BB50222
void CACResultSel::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/*************************************************************
 函 数 名：OnAutoCallResultFrameOpen()
 功能概要：响应窗口打开消息, 调整控件
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BB50242
void CACResultSel::OnAutoCallResultFrameOpen( WPARAM wParam, LPARAM lParam )
{
	InitTree();
}

//##ModelId=49B87BB50238
void CACResultSel::OnSelchangedTreeAcresultsel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM ht = m_Tree.GetSelectedItem();
	if(ht != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
		//通知显示窗口
		CWnd* pWnd = GetParent()->GetParent(); //父窗口
		//发送消息
		if(pWnd != NULL)
		{
			pWnd->PostMessage(AUTOCALL_RESULT_CHANGE, 0, (LPARAM)pObj);
		}
	}
	*pResult = 0;
}
