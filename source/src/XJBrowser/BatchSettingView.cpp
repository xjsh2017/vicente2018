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
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ���С, λ��
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87BB003C8
void CBatchSettingView::RegulateControlSize()
{

	if(m_Tree.GetSafeHwnd() == NULL)
		return;
	
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	//�����豸����С
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
 �� �� ����InitTree()
 ���ܸ�Ҫ����ʼ���豸��
 �� �� ֵ: ��ʼ���ɹ�����>=0��ֵ, ʧ�ܷ���<0��ֵ
 ��    ����param1
		   Param2
******************************r********************************/
//##ModelId=49B87BB003D8
int CBatchSettingView::InitTree()
{
	//���
	m_Tree.DeleteAllItems();
	if (g_role != MODE_SUB)
	{
		//�������ڵ�
		m_hRoot = m_Tree.InsertItem(StringFromID(IDS_MODEL_AREA));
		m_Tree.SetItemState(m_hRoot, 0, TLIS_SHOWCHECKBOX);
		//��������
		BuildNet();
	}
	else
	{
		m_hRoot = m_Tree.InsertItem(STRING_STATION);
	}

	m_Tree.SetItemImage(m_hRoot, 0, 0, 0, 0);

    m_Tree.SetItemData(m_hRoot, 0);
	//������վ
	BuildStation();
	//ȥ���սڵ�
	ClearEmptyNode();
	return 0;
}

/*************************************************************
 �� �� ����BuildNet()
 ���ܸ�Ҫ�����豸���д�������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10000
void CBatchSettingView::BuildNet()
{
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������е���
	POSITION pos = pData->m_NetList.GetHeadPosition();
	while(pos != NULL)
	{
		//ȡ�õ���, ������
		CNetObj* pObj = (CNetObj*)pData->m_NetList.GetNext(pos);
		InsertNet(pObj);
	}
}

/*************************************************************
 �� �� ����FindNetPos
 ���ܸ�Ҫ������ָ�����������е�λ��
 �� �� ֵ: ���ڵ�ṹ
 ��    ����param1   ָ������ID
**************************************************************/
//##ModelId=49B87BB10001
CTreeListItem* CBatchSettingView::FindNetPos(CString sNetID)
{
	CTreeListItem* htReturn = NULL;
	//���ڵ�
	CTreeListItem* htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL)
		return NULL;
	//��һ�������ڵ�
	CTreeListItem* htNetItem = m_Tree.GetChildItem(htRoot);

	//ѭ������
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
 �� �� ����InsertNet()
 ���ܸ�Ҫ����������ڵ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ����ĵ�������
**************************************************************/
//##ModelId=49B87BB1000F
BOOL CBatchSettingView::InsertNet(CNetObj* pObj)
{
	//���ڵ�Ϊ"����"
	//�����ڵ��ڵڶ���
	CTreeListItem* htRoot = m_Tree.GetRootItem();
	MYASSERT_BOOL(htRoot);

	//����ڵ�
	CTreeListItem* htNew = m_Tree.InsertItem(pObj->m_sName, htRoot);
	m_Tree.SetItemImage(htNew, IMAGEINDEX_NETAREA, IMAGEINDEX_NETAREA, IMAGEINDEX_NETAREA, IMAGEINDEX_NETAREA);
    m_Tree.SetItemState(htNew, 0, TLIS_SHOWCHECKBOX);
	MYASSERT_BOOL(htNew);
	pObj->m_Tag0 = htNew;
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	return TRUE;
}

/*************************************************************
 �� �� ����InsertStation()
 ���ܸ�Ҫ�����볧վ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1  Ҫ����ĳ�վ����
**************************************************************/
//##ModelId=49B87BB1001F
BOOL CBatchSettingView::InsertStation(CStationObj* pObj)
{
    CTreeListItem* htNew;
	if(g_role != MODE_SUB)
	{
		//��վ�ڵ��ڵ����ڵ�����, Ϊ������
		//�Ȳ��ҵ����ڵ�
		CTreeListItem* htNet = FindNetPos(pObj->m_sNetID);
		if(htNet == NULL)
			return FALSE;
		//����ڵ�
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
	//��¼�ڵ�
	pObj->m_Tag0 = htNew;
	
	//�豸����
	m_Tree.SetItemData(htNew, (DWORD)pObj);

	//�����һ���豸����
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

	//�����һ���豸
	//ĸ��
	BuildBus(pObj, htBus);
	//m_Tree.SortChildren(htBus);
	//��·
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
 �� �� ����BuildStation()
 ���ܸ�Ҫ�����豸���д�����վ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10021
void CBatchSettingView::BuildStation()
{
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	//�������г�վ
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		//ȡ�õ���, ������
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj != NULL && !pObj->m_IsDistanceVirtual && pObj->m_nStationType != 113 && pObj->m_nStationType != 114 )
			InsertStation(pObj);
	}
}

/*************************************************************
 �� �� ����InsertDevice()
 ���ܸ�Ҫ�������豸���豸�� 
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	ָ�����ڵ�
		   Param2	ָ���豸
**************************************************************/
//##ModelId=49B87BB1002E
BOOL CBatchSettingView::InsertDevice( CTreeListItem* htParent, CDeviceObj* pObj)
{
	//���������Ч��
	if(htParent == NULL)
		return FALSE;
	if(pObj == NULL)
		return FALSE;
	//����
	CTreeListItem* htNew = m_Tree.InsertItem(pObj->m_sName, htParent);
	if(htNew == NULL)
		return FALSE;
	pObj->m_Tag0 = htNew;
	//��������
	m_Tree.SetItemImage(htNew, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1);
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//����CPU
	if(pObj->m_nType == TYPE_WAVEREC)
	{
		CSecObj* pSec = (CSecObj*)pObj;
		//��������CPU
		for(int i = 0; i < pSec->m_arrCPU.GetSize(); i++)
		{
			//ȡ��CPU
			CSecCPU* pCpu = (CSecCPU*)pSec->m_arrCPU.GetAt(i);
			if(pCpu == NULL)
				continue;
			//���뵽�豸��
			InsertCPU(pCpu, htNew);
		}
	}
	return TRUE;
}

/*************************************************************
 �� �� ����BuildBus()
 ���ܸ�Ҫ����ָ����վ������ĸ�߼��뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�λ��
**************************************************************/
//##ModelId=49B87BB1003E
void CBatchSettingView::BuildBus( CStationObj* pStation, CTreeListItem* htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_BUS, pStation->m_sID);
	//ѭ��,���뵽�豸��
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 �� �� ����BuildLine()
 ���ܸ�Ҫ����ָ����վ��������·���뵽�豸��
 �� �� ֵ: 
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�λ��
**************************************************************/
//##ModelId=49B87BB10041
void CBatchSettingView::BuildLine( CStationObj* pStation, CTreeListItem* htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_LINE, pStation->m_sID);
	//ѭ��,���뵽�豸��
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 �� �� ����BuildGen()
 ���ܸ�Ҫ����ָ����վ������������뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB10050
void CBatchSettingView::BuildGen( CStationObj* pStation, CTreeListItem* htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_GEN, pStation->m_sID);
	//ѭ��,���뵽�豸��
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 �� �� ����BuildTrans()
 ���ܸ�Ҫ����ָ����վ�����б�ѹ�����뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB1005E
void CBatchSettingView::BuildTrans( CStationObj* pStation, CTreeListItem* htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_TRANS, pStation->m_sID);
	//ѭ��,���뵽�豸��
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 �� �� ����BuildCap()
 ���ܸ�Ҫ����ָ����վ�����е��������뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB1006D
void CBatchSettingView::BuildCap( CStationObj* pStation, CTreeListItem* htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_CAP, pStation->m_sID);
	//ѭ��,���뵽�豸��
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 �� �� ����BuildReactor()
 ���ܸ�Ҫ����ָ����վ�����е翹�����뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB10070
void CBatchSettingView::BuildReactor( CStationObj* pStation, CTreeListItem* htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_REACTOR, pStation->m_sID);
	//ѭ��,���뵽�豸��
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 �� �� ����BuildBreak()
 ���ܸ�Ҫ����ָ����վ�����п��ؼ��뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB1007E
void CBatchSettingView::BuildBreak( CStationObj* pStation, CTreeListItem* htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_BREAK, pStation->m_sID);
	//ѭ��,���뵽�豸��
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 �� �� ����BuildSwitch()
 ���ܸ�Ҫ����ָ����վ�����е�բ���뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB10081
void CBatchSettingView::BuildSwitch( CStationObj* pStation, CTreeListItem* htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, TYPE_SWITCH, pStation->m_sID);
	//ѭ��,���뵽�豸��
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 �� �� ����BuildWaveRec()
 ���ܸ�Ҫ����ָ����վ������¼�������뵽�豸��
 �� �� ֵ: 
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB1008E
void CBatchSettingView::BuildWaveRec( CStationObj* pStation, CTreeListItem* htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_WAVEREC, pStation->m_sID);
	//ѭ��,���뵽�豸��
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertDevice(htParent, pObj);
	}
}

/*************************************************************
 �� �� ����InsertProtect()
 ���ܸ�Ҫ�����뱣�����豸��
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	ָ������
		   Param2
**************************************************************/
//##ModelId=49B87BB1009C
BOOL CBatchSettingView::InsertProtect( CSecObj* pObj )
{
	//���������Ч��
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
	//���뵽�豸��
	CString str;
	str = pObj->m_sName;
	CTreeListItem* htNew = m_Tree.InsertItem(str, htTemp);
	if(htNew == NULL)
		return FALSE;
	m_Tree.SetItemImage(htNew, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1);
	
	//��¼λ��
	pObj->m_Tag0 = htNew;
	
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//��������CPU
	for(int i = 0; i < pObj->m_arrCPU.GetSize(); i++)
	{
		//ȡ��CPU
		CSecCPU* pCpu = (CSecCPU*)pObj->m_arrCPU.GetAt(i);
		if(pCpu == NULL)
			continue;
		//���뵽�豸��
		InsertCPU(pCpu, htNew);
	}
	//����
//	m_Tree.SortChildren(htNew);

	return TRUE;
}

/*************************************************************
 �� �� ����BuildProtect()
 ���ܸ�Ҫ����ָ����վ�����б������뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2
**************************************************************/
//##ModelId=49B87BB100A1
void CBatchSettingView::BuildProtect( CStationObj* pStation )
{
	if(pStation == NULL)
		return;
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_PROTECT, pStation->m_sID);
	//ѭ��,���뵽�豸��
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj != NULL)
			InsertProtect(pObj);
	}
}

/*************************************************************
 �� �� ����ClearEmptyNode()
 ���ܸ�Ҫ��ȥ��û���ӽڵ�Ľڵ�
 �� �� ֵ: void
 ��    ����param1
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
 �� �� ����CheckDevice()
 ���ܸ�Ҫ����ĳ�豸������Check����, ����Ǳ�����¼����, ֪ͨ��ϸ��ͼ
 �� �� ֵ: void
 ��    ����param1	�豸ָ��
		   Param2	1��ʾѡ��, 0��ʾȡ��ѡ��
**************************************************************/
//##ModelId=49B87BB100AD
void CBatchSettingView::CheckDevice( CDeviceObj* pObj, int nSelected )
{
	MYASSERT(pObj);
	//�������������
	if(pObj == NULL)
		return;
	if(nSelected != 0 && nSelected != 1) //��־ֵ��Ч
		return;
	if(pObj->m_nType != TYPE_CPU) //�����豸��Ч
		return;
	//������Ϣ�������ڸ�����(��Ϊ�������Ƿָ�����),����ת������ϸ��ͼ
	CWnd* pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//wParam����д��־λ, lParam����д�豸ָ��
		pWnd->PostMessage(BATCH_CHECK, (WPARAM)nSelected, (LPARAM)pObj);
	}
}

/*************************************************************
 �� �� ����CycTreeDown()
 ���ܸ�Ҫ�����±���ָ���ڵ�������ӽڵ�, �������ӽڵ�����ͬ��ѡ�����
 �� �� ֵ: void
 ��    ����param1	ָ���ڵ�
		   Param2	ѡ���־.TRUEΪѡ��, FALSEΪȡ��ѡ��
**************************************************************/
//##ModelId=49B87BB100BB
void CBatchSettingView::CycTreeDown( CTreeListItem* hRoot, BOOL bMark )
{
	if(hRoot != NULL)
	{
		//����ѡ��/ȡ��ѡ��
		m_Tree.SetCheck(hRoot, bMark);
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hRoot);
		if(pObj != NULL)
		{
			if(bMark)
				CheckDevice(pObj, 1); //ѡ��
			else
				CheckDevice(pObj, 0); //ȡ��ѡ��
		}
		//��������
		CycTreeDown(m_Tree.GetChildItem(hRoot), bMark);
		CycTreeDown(m_Tree.GetNextItem(hRoot, TVGN_NEXT), bMark);
	}
}

/*************************************************************
 �� �� ����CycTreeUp()
 ���ܸ�Ҫ�����ϱ����ڵ�ĸ��ڵ�, �����ǰ�ڵ��Ǹ��ڵ�����һ����ѡ����ӽڵ�, Ϊ���ڵ�Ҳ����ѡ���־
 �� �� ֵ: void
 ��    ����param1	ָ���ڵ�
		   Param2
**************************************************************/
//##ModelId=49B87BB100BE
void CBatchSettingView::CycTreeUp( CTreeListItem* hCurrent )
{
	CTreeListItem* hParent = NULL; 
	CTreeListItem* hChild = NULL;
	if(hCurrent != NULL)
	{
		//ȡ�ø��ڵ�
		hParent = m_Tree.GetParentItem(hCurrent);
		if(hParent != NULL)
		{
			//�������ڵ�������ӽڵ�, ���Ƿ�ȫ��ѡ����
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
 �� �� ����HitItemHelp()
 ���ܸ�Ҫ������ڵ㱻������
 �� �� ֵ: void
 ��    ����param1	������Ľڵ�
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
			//��ѡ��
			CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hit);
			if(pObj != NULL)
				CheckDevice(pObj, 1);
			bMark = TRUE;
			CycTreeDown(hChild, bMark);		//�����ڵ�ѡ��ʱ,�ӽڵ�ȫ��ѡ��
		 	CycTreeUp(hit);					//���ӽڵ�ȫ��ѡ��ʱ,���ڵ�ѡ��
		}
		else
		{
			//ȡ��ѡ��
			CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hit);
			if(pObj != NULL)
				CheckDevice(pObj, 0);
			bMark = FALSE;
			CycTreeDown(hChild, bMark);		//�����ڵ�ȡ��ѡ��ʱ,�ӽڵ�ȫ��ȡ��ѡ��
			
			while(hParent != NULL)			//���ӽڵ���һ��ȡ��ѡ��ʱ,���ڵ�ȡ��ѡ��
			{
				m_Tree.SetCheck(hParent, FALSE);
				hParent = m_Tree.GetParentItem(hParent);
			}
		}
		*/
	}		
}



/*************************************************************
 �� �� ����OnBatchFrameOpen()
 ���ܸ�Ҫ����Ӧ���ڴ���Ϣ, ˢ�½���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10148
void CBatchSettingView::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//ˢ�½���
	RegulateControlSize();
	m_nViewType = (int)lParam;
	//��ʼ��
	//InitList();
	//InitTree();
}

/*************************************************************
 �� �� ����SetCheck_XJ()
 ���ܸ�Ҫ������ָ���ڵ��ѡ��״̬
 �� �� ֵ: void
 ��    ����param1	ָ���ڵ�
		   Param2	TRUE-ѡ��. FALSE-ȡ��ѡ��
**************************************************************/
//##ModelId=49B87BB100CD
void CBatchSettingView::SetCheck_XJ( CTreeListItem* hit, BOOL bCheck )
{
	CTreeListItem* hChild = NULL;
	CTreeListItem* hParent = NULL;

	//�ж�������Ч��
	if(hit == NULL)
		return;
	//����״̬
	m_Tree.SetCheck(hit, bCheck);

	hChild = m_Tree.GetChildItem(hit);
	hParent = m_Tree.GetParentItem(hit);
	
	if(bCheck)
	{
		//��ѡ��
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hit);
		if(pObj != NULL)
			CheckDevice(pObj, 1);
		CycTreeDown(hChild, bCheck);		//�����ڵ�ѡ��ʱ,�ӽڵ�ȫ��ѡ��
		CycTreeUp(hit);					//���ӽڵ�ȫ��ѡ��ʱ,���ڵ�ѡ��
	}
	else
	{
		//ȡ��ѡ��
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hit);
		if(pObj != NULL)
			CheckDevice(pObj, 0);
		CycTreeDown(hChild, bCheck);		//�����ڵ�ȡ��ѡ��ʱ,�ӽڵ�ȫ��ȡ��ѡ��
		
		while(hParent != NULL)			//���ӽڵ���һ��ȡ��ѡ��ʱ,���ڵ�ȡ��ѡ��
		{
			m_Tree.SetCheck(hParent, FALSE);
			hParent = m_Tree.GetParentItem(hParent);
		}
	}
}

/*************************************************************
 �� �� ����OnBatchDetailUncheck()
 ���ܸ�Ҫ������ϸ��ͼȡ��ĳ�豸ѡ��
 �� �� ֵ: void
 ��    ����param1
		   Param2	�豸ָ��
**************************************************************/
//##ModelId=49B87BB1014C
void CBatchSettingView::OnBatchDetailUncheck( WPARAM wParam, LPARAM lParam )
{
	//ȡ������
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
 �� �� ����OnBatchUnselectAll()
 ���ܸ�Ҫ����Ӧȡ������ѡ����Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10157
void CBatchSettingView::OnBatchUnselectAll( WPARAM wParam, LPARAM lParam )
{
	//�ж�������Ч��
	if(m_hRoot == NULL)
		return;
	//ȡ������ѡ��
	SetCheck_XJ(m_hRoot, FALSE);
}

/*************************************************************
 �� �� ����OnBatchSelectAllPT()
 ���ܸ�Ҫ����Ӧ����ȫѡ��Ϣ, ѡ�����б���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB1015B
void CBatchSettingView::OnBatchSelectAllPT( WPARAM wParam, LPARAM lParam )
{
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_PROTECT);
	//ѭ������
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//����ѡ��
		CTreeListItem* ht = NULL;
		ht = pObj->m_Tag0;
		if(ht != NULL)
		{
			SetCheck_XJ(ht, TRUE);
		}
	}
}

/*************************************************************
 �� �� ����OnBatchSelectAllWR()
 ���ܸ�Ҫ����Ӧ¼����ȫѡ��Ϣ, ѡ������¼����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB1015F
void CBatchSettingView::OnBatchSelectAllWR( WPARAM wParam, LPARAM lParam )
{
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_PROTECT);
	//ѭ������
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//����ѡ��
		CTreeListItem* ht = NULL;
		ht = pObj->m_Tag0;
		if(ht != NULL)
		{
			SetCheck_XJ(ht, TRUE);
		}
	}
}

/*************************************************************
 �� �� ����OnBatchEnableControls()
 ���ܸ�Ҫ����Ӧ�ؼ������Ըı���Ϣ, �ı�ؼ�������
 �� �� ֵ: void
 ��    ����param1
		   Param2	����/������
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
 �� �� ����InsertCPU()
 ���ܸ�Ҫ������CPU���豸��
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	ָ��CPU
		   Param2	���ڵ�λ��
**************************************************************/
//##ModelId=49B87BB1009E
BOOL CBatchSettingView::InsertCPU( CSecCPU* pCpu, CTreeListItem* htParent)
{
	//���������Ч��
	if(pCpu == NULL)
		return FALSE;
	if(htParent == NULL)
		return FALSE;
	//���뵽�豸��
	CString str;
	str.Format("%d(%s)", pCpu->code, pCpu->des);
	CTreeListItem* htNew = m_Tree.InsertItem(str, htParent);
	
	m_Tree.SetItemState(htNew, 0, TLIS_SHOWCHECKBOX);
	if(htNew == NULL)
		return FALSE;
	//��¼λ��
	pCpu->m_Tag0 = htNew;
	
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pCpu);
	return TRUE;
}

void CBatchSettingView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	//ֻ�����Զ��������õ����
	if(m_nViewType != 0)
		return;
	CPoint ptClient = point;
	m_Tree.ScreenToClient(&ptClient);
	CTreeListItem* hItem = m_Tree.HitTest(ptClient);
	if(hItem != NULL)
	{
		//�ýڵ㱻ѡ��
		m_Tree.SelectItem(hItem);
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hItem);
		if ((DWORD)pObj < 100)
		{
			return;
		}
	//	if(pObj->m_nType == TYPE_STATION) //ֻ����վ�����
	//	{
			//�����˵�
			//���ɲ˵�
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
	//ȡ�ñ�ѡ�����
	CTreeListItem* ht = m_Tree.GetSelectedItem();
	CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj->m_nType != TYPE_STATION)
		return;
	//���ÿɲ�����վ
	m_pActiveStation = (CStationObj*)pObj;
	m_Tree.Invalidate(FALSE);
	//ȡ������ѡ��
	ClearCheck(m_hRoot);

	//չ��
	m_Tree.Expand(ht, TVE_EXPAND);
	//������Ϣ
	CWnd* pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//wParam��������ѡ���־, lParam���������Ƿ��ٻ���־
		pWnd->SendMessage(AUTOCALL_STATION_CHANGE, 0, (LPARAM)m_pActiveStation);
	}
}

void CBatchSettingView::OnAutocallAll() 
{
	// TODO: Add your command handler code here
	//�������г�վ�ɲ���
	m_pActiveStation = NULL;
	//ˢ����
	m_Tree.Invalidate(FALSE);
	//ȡ������ѡ��
	ClearCheck(m_hRoot);
	//������Ϣ
	CWnd* pWnd = GetParent()->GetParent();
	if(pWnd != NULL)
	{
		//wParam��������ѡ���־, lParam���������Ƿ��ٻ���־
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
 �� �� ��: IsEnableItem()
 ���ܸ�Ҫ: �ж�ָ���ڵ��Ƿ�ɲ���
 �� �� ֵ: �ɲ�������TRUE,���ɲ�������FALSE
 ��    ��: param1 ָ���ڵ�
		   Param2 
**************************************************************/
BOOL CBatchSettingView::IsEnableItem( CTreeListItem* ht )
{
	//�Զ������������˿ɲ�����վʱ��Ч
	CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
	{
		//������Ӳ����,�жϸ��ڵ�
		CTreeListItem* htParent = m_Tree.GetParentItem(ht);
		//�õ����ڵ�
		CDeviceObj* pParent = (CDeviceObj*)m_Tree.GetItemData(htParent);
		if(pParent == NULL)
			return FALSE;
		if(pParent->m_nType == TYPE_STATION)
		{
			//��վ
			if(pParent->m_sID == m_pActiveStation->m_sID)
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			//�ǳ�վ
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
			//��վ
			if(pObj->m_sID == m_pActiveStation->m_sID)
				return TRUE;
			else
				return FALSE;
		}
		else
		{
			//�ǳ�վ
			if(pObj->m_sStationID == m_pActiveStation->m_sID)
				return TRUE;
			else
				return FALSE;
		}
	}
	return FALSE;
}

/*************************************************************
 �� �� ��: ClearCheck()
 ���ܸ�Ҫ: �������ѡ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CBatchSettingView::ClearCheck(CTreeListItem* ht)
{
	//�ж�������Ч��
	if(ht == NULL)
		return;
	//ȡ������ѡ��
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
 �� �� ����OnClickTreeBatchSetting()
 ���ܸ�Ҫ����Ӧ�豸������¼�, ѡ���豸
 �� �� ֵ: 
 ��    ����param1
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
 �� �� ����PostMessageToDetailSettingView
 ���ܸ�Ҫ����Ӧ���οؼ�ѡ����Ϣ 
 �� �� ֵ: 
 ��    ����param1   
		   Param2  ָ���ڵ�
**************************************************************/
void CBatchSettingView::PostMessageToDetailSettingView(CTreeListItem* hItem)
{
	if(hItem != NULL)
	{
		//������Ϣ�������ڸ�����(��Ϊ�������Ƿָ�����),����ת������ϸ��ͼ
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
