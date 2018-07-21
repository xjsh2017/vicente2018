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
 �� �� ����InitList()
 ���ܸ�Ҫ����ʼ���ٻ�ѡ���б��
 �� �� ֵ: ��ʼ���ɹ�����>=0��ֵ, ʧ�ܷ���<0��ֵ
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB003BC
int CBatchSelectView::InitList()
{
	//���
	m_List.DeleteAllItems();

	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,20,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	
	char* arColumn[2]={"ѡ��","ֵ"};
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
	{
		lvCol.iSubItem=nCol;
		switch(nCol)
		{
		case 0://����
			lvCol.cx = 140; //�п�50����
			break;
		case 1://�¼�ID
			lvCol.cx = 90; //�п�60����
			break;
		default:
			lvCol.cx=100;//�п�100����
		}
		lvCol.pszText=arColumn[nCol];
		m_List.InsertColumn(nCol,&lvCol);
	}
	
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	//������
	if(m_List.InsertItem(LVIF_TEXT, 0, "��ֵ����", 0, 0, 0, 0)==-1)
		return -1;
	if(m_List.InsertItem(LVIF_TEXT, 1, "��ֵ", 0, 0, 0, 0)==-1)
		return -1;
	if(m_List.InsertItem(LVIF_TEXT, 2, "������", 0, 0, 0, 0)==-1)
		return -1;
	if(m_List.InsertItem(LVIF_TEXT, 3, "��ѹ��", 0, 0, 0, 0)==-1)
		return -1;
	if(m_List.InsertItem(LVIF_TEXT, 4, "ģ����", 0, 0, 0, 0)==-1)
		return -1;
	//����ֵ
	m_List.SetItemText(0, 1, (m_bZone)?"�ٻ�":"���ٻ�");
	m_List.SetItemText(1, 1, (m_bSetting)?"�ٻ�":"���ٻ�");
	m_List.SetItemText(2, 1, (m_bDigital)?"�ٻ�":"���ٻ�");
	m_List.SetItemText(3, 1, (m_bSoftBoard)?"�ٻ�":"���ٻ�");
	m_List.SetItemText(4, 1, (m_bAnalog)?"�ٻ�":"���ٻ�");

	return 0;
}

/*************************************************************
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ���С, λ��
 �� �� ֵ: void
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
	//�����б���С,�̶�150
/*	rcList.top = rcList.bottom - 150;
	m_List.MoveWindow(rcList);
	//�����豸����С
	CRect rcTree = rc;
	rcTree.bottom = rc.bottom - 150;
	m_Tree.MoveWindow(rcTree);
	//�����п�
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
 �� �� ����InitTree()
 ���ܸ�Ҫ����ʼ���豸��
 �� �� ֵ: ��ʼ���ɹ�����>=0��ֵ, ʧ�ܷ���<0��ֵ
 ��    ����param1
		   Param2
******************************r********************************/
//##ModelId=49B87BB003D8
int CBatchSelectView::InitTree()
{
	//���
	m_Tree.DeleteAllItems();
	if (g_role != MODE_SUB)
	{
		//�������ڵ�
		m_hRoot = m_Tree.InsertItem(STRING_NET, NULL);
		//��������
		BuildNet();
	}
	else
	{
		m_hRoot = m_Tree.InsertItem(STRING_STATION, NULL);
	}

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
void CBatchSelectView::BuildNet()
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
HTREEITEM CBatchSelectView::FindNetPos(CString sNetID)
{
	HTREEITEM htReturn = NULL;
	//���ڵ�
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL)
		return NULL;
	//��һ�������ڵ�
	HTREEITEM htNetItem = m_Tree.GetChildItem(htRoot);

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
BOOL CBatchSelectView::InsertNet(CNetObj* pObj)
{
	//���ڵ�Ϊ"����"
	//�����ڵ��ڵڶ���
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL)
		return FALSE;

	//����ڵ�
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, htRoot);
	if(htNew == NULL)
		return FALSE;
	pObj->m_Tag = htNew;
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//����
	//m_Tree.SortChildren(htRoot);
	return TRUE;
}

/*************************************************************
 �� �� ����InsertStation()
 ���ܸ�Ҫ�����볧վ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1  Ҫ����ĳ�վ����
**************************************************************/
//##ModelId=49B87BB1001F
BOOL CBatchSelectView::InsertStation(CStationObj* pObj)
{
    HTREEITEM htNew;
	if(g_role != MODE_SUB)
	{
		//��վ�ڵ��ڵ����ڵ�����, Ϊ������
		//�Ȳ��ҵ����ڵ�
		HTREEITEM htNet = FindNetPos(pObj->m_sNetID);
		if(htNet == NULL)
			return FALSE;
		//����ڵ�
		htNew = m_Tree.InsertItem(pObj->m_sName, htNet);
		//����
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

	//��¼�ڵ�
	pObj->m_Tag = htNew;
	//�豸����
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//�����һ���豸����
	HTREEITEM htBus		= m_Tree.InsertItem(STRING_BUS, htNew);
	HTREEITEM htLine	= m_Tree.InsertItem(STRING_LINE, htNew);
	HTREEITEM htGen		= m_Tree.InsertItem(STRING_GEN, htNew);
	HTREEITEM htTrans	= m_Tree.InsertItem(STRING_TRANS, htNew);
	HTREEITEM htCap		= m_Tree.InsertItem(STRING_CAP, htNew);
	HTREEITEM htReactor = m_Tree.InsertItem(STRING_REACTOR, htNew);
	HTREEITEM htSwitch	= m_Tree.InsertItem(STRING_SWITCH, htNew);
	HTREEITEM htBreak	= m_Tree.InsertItem(STRING_BREAK, htNew);
	HTREEITEM htWaveRec = m_Tree.InsertItem(STRING_WAVEREC, htNew);

	//�����һ���豸
	//ĸ��
	BuildBus(pObj, htBus);
	m_Tree.SortChildren(htBus);
	//��·
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
 �� �� ����BuildStation()
 ���ܸ�Ҫ�����豸���д�����վ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10021
void CBatchSelectView::BuildStation()
{
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
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
BOOL CBatchSelectView::InsertDevice( HTREEITEM htParent, CDeviceObj* pObj)
{
	//���������Ч��
	if(htParent == NULL)
		return FALSE;
	if(pObj == NULL)
		return FALSE;
	//����
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, htParent);
	if(htNew == NULL)
		return FALSE;

	pObj->m_Tag = htNew;
	//��������
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
		//����
		m_Tree.SortChildren(htNew);
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
void CBatchSelectView::BuildBus( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
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
 �� �� ����BuildLine()
 ���ܸ�Ҫ����ָ����վ��������·���뵽�豸��
 �� �� ֵ: 
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�λ��
**************************************************************/
//##ModelId=49B87BB10041
void CBatchSelectView::BuildLine( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
	//ѭ��,���뵽�豸��
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
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
 �� �� ����BuildGen()
 ���ܸ�Ҫ����ָ����վ������������뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB10050
void CBatchSelectView::BuildGen( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
	//ѭ��,���뵽�豸��
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
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
 �� �� ����BuildTrans()
 ���ܸ�Ҫ����ָ����վ�����б�ѹ�����뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB1005E
void CBatchSelectView::BuildTrans( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
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
 �� �� ����BuildCap()
 ���ܸ�Ҫ����ָ����վ�����е��������뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB1006D
void CBatchSelectView::BuildCap( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
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
 �� �� ����BuildReactor()
 ���ܸ�Ҫ����ָ����վ�����е翹�����뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB10070
void CBatchSelectView::BuildReactor( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
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
 �� �� ����BuildBreak()
 ���ܸ�Ҫ����ָ����վ�����п��ؼ��뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB1007E
void CBatchSelectView::BuildBreak( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
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
 �� �� ����BuildSwitch()
 ���ܸ�Ҫ����ָ����վ�����е�բ���뵽�豸��
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB10081
void CBatchSelectView::BuildSwitch( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
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
 �� �� ����BuildWaveRec()
 ���ܸ�Ҫ����ָ����վ������¼�������뵽�豸��
 �� �� ֵ: 
 ��    ����param1	ָ����վ
		   Param2	ָ�����ڵ�
**************************************************************/
//##ModelId=49B87BB1008E
void CBatchSelectView::BuildWaveRec( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
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
 �� �� ����InsertProtect()
 ���ܸ�Ҫ�����뱣�����豸��
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	ָ������
		   Param2
**************************************************************/
//##ModelId=49B87BB1009C
BOOL CBatchSelectView::InsertProtect( CSecObj* pObj )
{
	//���������Ч��
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
	//���뵽�豸��
	CString str;
	str = pObj->m_sName;
	HTREEITEM htNew = m_Tree.InsertItem(str, htTemp);
	if(htNew == NULL)
		return FALSE;
	//��¼λ��
	
	pObj->m_Tag = htNew;
	m_Tree.SortChildren(pObj->m_pOwner->m_Tag);
		
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
void CBatchSelectView::BuildProtect( CStationObj* pStation )
{
	if(pStation == NULL)
		return;
	//ѭ��,���뵽�豸��
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
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
 �� �� ����ClearEmptyNode()
 ���ܸ�Ҫ��ȥ��û���ӽڵ�Ľڵ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB100AC
void CBatchSelectView::ClearEmptyNode()
{
	HTREEITEM hTreeLevel1,hTreeLevel2,hTreeLevel3,hTreeLevel4,hTreeTemp;
	CString strTmp, strWave;
	strWave = "¼����";
	
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
void CBatchSelectView::CheckDevice( CDeviceObj* pObj, int nSelected )
{
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
void CBatchSelectView::CycTreeDown( HTREEITEM hRoot, BOOL bMark )
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
void CBatchSelectView::CycTreeUp( HTREEITEM hCurrent )
{
	HTREEITEM hParent, hChild;
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
		}		
	}
}

/*************************************************************
 �� �� ����OnClickTreeBatchSelect()
 ���ܸ�Ҫ����Ӧ�豸������¼�, ѡ���豸
 �� �� ֵ: 
 ��    ����param1
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
	//�жϳ�վ�Ƿ�ɲ���
	if(!IsEnableItem(hit))
		return;

	if(uFlags & TVHT_ONITEMSTATEICON )
	{
		HitItemHelp(hit);
	}
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ����Ӧ�б���Ի���ʾ��Ϣ
 �� �� ֵ: 
 ��    ����param1
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
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //������
				int nSubItem = lplvcd->iSubItem; //������
				//�ȸ�Ĭ��ֵ, ���������⴦��
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				//���ٻ����ú�ɫ������ʾ
				//ֵ��
				if(nSubItem == 1)
				{
					CString str = m_List.GetItemText(nItem, nSubItem);
					//�Ͽ�ʱ������ʾ
					if(str == "���ٻ�")
					{
						lplvcd->clrText = g_BatchNotCall;
					}
					if(str == "�ٻ�")
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
	int nItem = pNMListView->iItem; //��
	int nSubItem = pNMListView->iSubItem; //��
	if(nItem >= 0)
	{
		//����Ч����
		if(nSubItem == 1)
		{
			//ֵ��
			if(nItem == 0)
			{
				//��ֵ����
				m_bZone = !m_bZone; //����
				m_List.SetItemText(nItem, nSubItem, m_bZone?"�ٻ�":"���ٻ�");
				//������Ϣ
				CWnd* pWnd = GetParent()->GetParent();
				if(pWnd != NULL)
				{
					//wParam��������ѡ���־, lParam���������Ƿ��ٻ���־
					pWnd->SendMessage(BATCH_OPTION, 4, (DWORD)m_bZone);
				}
			}
			if(nItem == 1)
			{
				//��ֵ��
				m_bSetting = !m_bSetting; //����
				m_List.SetItemText(nItem, nSubItem, m_bSetting?"�ٻ�":"���ٻ�");
				//������Ϣ
				CWnd* pWnd = GetParent()->GetParent();
				if(pWnd != NULL)
				{
					//wParam��������ѡ���־, lParam���������Ƿ��ٻ���־
					pWnd->SendMessage(BATCH_OPTION, 0, (DWORD)m_bSetting);
				}
			}
			if(nItem == 2)
			{
				//������
				m_bDigital = !m_bDigital; //����
				m_List.SetItemText(nItem, nSubItem, m_bDigital?"�ٻ�":"���ٻ�");
				//������Ϣ
				CWnd* pWnd = GetParent()->GetParent();
				if(pWnd != NULL)
				{
					//wParam��������ѡ���־, lParam���������Ƿ��ٻ���־
					pWnd->SendMessage(BATCH_OPTION, 1, (DWORD)m_bDigital);
				}
			}
			if(nItem == 3)
			{
				//��ѹ��
				m_bSoftBoard = !m_bSoftBoard; //����
				m_List.SetItemText(nItem, nSubItem, m_bSoftBoard?"�ٻ�":"���ٻ�");
				//������Ϣ
				CWnd* pWnd = GetParent()->GetParent();
				if(pWnd != NULL)
				{
					//wParam��������ѡ���־, lParam���������Ƿ��ٻ���־
					pWnd->SendMessage(BATCH_OPTION, 2, (DWORD)m_bSoftBoard);
				}
			}
			if(nItem == 4)
			{
				//ģ����
				m_bAnalog = !m_bAnalog; //����
				m_List.SetItemText(nItem, nSubItem, m_bAnalog?"�ٻ�":"���ٻ�");
				//������Ϣ
				CWnd* pWnd = GetParent()->GetParent();
				if(pWnd != NULL)
				{
					//wParam��������ѡ���־, lParam���������Ƿ��ٻ���־
					pWnd->SendMessage(BATCH_OPTION, 3, (DWORD)m_bAnalog);
				}
			}
		}
	}
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnBatchFrameOpen()
 ���ܸ�Ҫ����Ӧ���ڴ���Ϣ, ˢ�½���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10148
void CBatchSelectView::OnBatchFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//ˢ�½���
	RegulateControlSize();
	m_nViewType = (int)lParam;
	//��ʼ��
	InitList();
	InitTree();
}

/*************************************************************
 �� �� ����SetCheck_XJ()
 ���ܸ�Ҫ������ָ���ڵ��ѡ��״̬
 �� �� ֵ: void
 ��    ����param1	ָ���ڵ�
		   Param2	TRUE-ѡ��. FALSE-ȡ��ѡ��
**************************************************************/
//##ModelId=49B87BB100CD
void CBatchSelectView::SetCheck_XJ( HTREEITEM hit, BOOL bCheck )
{
	HTREEITEM hChild, hParent;

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
void CBatchSelectView::OnBatchDetailUncheck( WPARAM wParam, LPARAM lParam )
{
	//ȡ������
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
 �� �� ����OnBatchUnselectAll()
 ���ܸ�Ҫ����Ӧȡ������ѡ����Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB10157
void CBatchSelectView::OnBatchUnselectAll( WPARAM wParam, LPARAM lParam )
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
void CBatchSelectView::OnBatchSelectAllPT( WPARAM wParam, LPARAM lParam )
{
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	pData->GetSecList(listDevice, "", TYPE_PROTECT, "");
	//ѭ�����г�վ
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//����ѡ��
		HTREEITEM ht = NULL;
		ht = pObj->m_Tag;
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
void CBatchSelectView::OnBatchSelectAllWR( WPARAM wParam, LPARAM lParam )
{
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	pData->GetSecList(listDevice, "", TYPE_WAVEREC, "");
	//ѭ������
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//����ѡ��
		HTREEITEM ht = NULL;
		ht = pObj->m_Tag;
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
 �� �� ����InsertCPU()
 ���ܸ�Ҫ������CPU���豸��
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	ָ��CPU
		   Param2	���ڵ�λ��
**************************************************************/
//##ModelId=49B87BB1009E
BOOL CBatchSelectView::InsertCPU( CSecCPU* pCpu, HTREEITEM htParent)
{
	//���������Ч��
	if(pCpu == NULL)
		return FALSE;
	if(htParent == NULL)
		return FALSE;
	//���뵽�豸��
	CString str;
	str.Format("%d(%s)", pCpu->code, pCpu->des);
	HTREEITEM htNew = m_Tree.InsertItem(str, htParent);
	if(htNew == NULL)
		return FALSE;
	//��¼λ��
	pCpu->m_Tag = htNew;
	
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pCpu);
	return TRUE;
}

void CBatchSelectView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	//ֻ�����Զ��������õ����
	if(m_nViewType != 0)
		return;
	CPoint ptClient = point;
	m_Tree.ScreenToClient(&ptClient);
	HTREEITEM hItem = m_Tree.HitTest(ptClient);
	if(hItem != NULL)
	{
		//�ýڵ㱻ѡ��
		m_Tree.SelectItem(hItem);
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hItem);
		if(pObj == NULL)
			return;
		if(pObj->m_nType == TYPE_STATION) //ֻ����վ�����
		{
			//�����˵�
			//���ɲ˵�
			CMenu Menu, *pMenu;
			if(!Menu.LoadMenu(IDR_MENU_AUTOCALL))
				return;
			pMenu = Menu.GetSubMenu(0);

			CStationObj* pStation = (CStationObj*)pObj;
			if(m_pActiveStation == NULL)
			{
				//���г�վ�ɲ���
				pMenu->EnableMenuItem(ID_AUTOCALL_ACTIVE, MF_ENABLED);
			}
			else
			{
				if(m_pActiveStation == pStation)
				{
					//��ǰ�ɲ�����վ
					pMenu->EnableMenuItem(ID_AUTOCALL_ALL, MF_ENABLED);
				}
				else
				{
					//��ѡ��վ���ǿɲ�����վ
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
	//ȡ�ñ�ѡ�����
	HTREEITEM ht = m_Tree.GetSelectedItem();
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

void CBatchSelectView::OnAutocallAll() 
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
				//�ȸ�Ĭ��ֵ, ���������⴦��
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
				if(pObj != NULL && m_pActiveStation != NULL)
				{
					if(pObj->m_nType == TYPE_STATION) //��վ
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
 �� �� ��: IsEnableItem()
 ���ܸ�Ҫ: �ж�ָ���ڵ��Ƿ�ɲ���
 �� �� ֵ: �ɲ�������TRUE,���ɲ�������FALSE
 ��    ��: param1 ָ���ڵ�
		   Param2 
**************************************************************/
BOOL CBatchSelectView::IsEnableItem( HTREEITEM ht )
{
	if(m_nViewType == 1)
	{
		//�����ٻ�����
		return TRUE;
	}
	if(m_pActiveStation == NULL)
		return TRUE;

	//�Զ������������˿ɲ�����վʱ��Ч
	CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
	{
		//������Ӳ����,�жϸ��ڵ�
		HTREEITEM htParent = m_Tree.GetParentItem(ht);
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
void CBatchSelectView::ClearCheck(HTREEITEM ht)
{
	//�ж�������Ч��
	if(ht == NULL)
		return;
	//ȡ������ѡ��
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


