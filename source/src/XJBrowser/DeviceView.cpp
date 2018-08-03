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

	//����ͼ��
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
	
	

	//����ͼ��
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
	//��ʼ��
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
 �� �� ����InitDeviceTree()
 ���ܸ�Ҫ�������豸��, ����豸��
 �� �� ֵ: ʧ�ܷ���-1, ��������>=0
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
	//���
	m_Tree.DeleteAllItems();

    POSITION pos;
    HTREEITEM hRoot;
	if (g_role != MODE_SUB)
	{
		//���ڵ�
		hRoot = m_Tree.InsertItem( StringFromID(IDS_MODEL_AREA), 0, 0,NULL);
		//����
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
        //���ڵ�
		htRoot = m_Tree.InsertItem(StringFromID(IDS_MODEL_SUBSTATION), IMAGEINDEX_STATION, IMAGEINDEX_STATION, NULL);
	}
	//��վ
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

			if(g_DeviceTreeType == 0) //ֻ��ʾ��վ�����
				InsertStation(pObj);
			else  //վ����ʾ�豸
				InsertStation_Voltage(pObj);
		}
	}

	//����սڵ�
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
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б��, �������
 �� �� ֵ: ����ֵ˵��
**************************************************************/
int CDeviceView::InitListCtrl()
{
	//��ͼ���б�, ���������и�
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
		case 0://����
			lvCol.pszText=StringFromID(IDS_COMMON_PROPERTY).GetBuffer(1);
			lvCol.cx = 60; //�п�50����
			break;
		case 1://�¼�ID
			lvCol.pszText=StringFromID(IDS_COMMON_VALUE).GetBuffer(1);
			lvCol.cx = 60; //�п�60����
			break;
		default:
			lvCol.cx=100;//�п�100����
		}
		
		m_List.InsertColumn(nCol,&lvCol);
	}

	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
	m_List.ModifyStyle(0, LVS_NOCOLUMNHEADER);

	//�������
	FillList();

	return 0;
}

/*************************************************************
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ���С, λ��
 �� �� ֵ: void
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
	//�����п�
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
 �� �� ����FindNetPos
 ���ܸ�Ҫ������ָ�����������е�λ��
 �� �� ֵ: ���ڵ�ṹ
 ��    ����param1   ָ������ID
**************************************************************/
HTREEITEM CDeviceView::FindNetPos(CString sNetID)
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
BOOL CDeviceView::InsertNet(CNetObj* pObj)
{
	//���ڵ�Ϊ"����"
	//�����ڵ��ڵڶ���
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL)
		return FALSE;

	//����ڵ�
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, IMAGEINDEX_NETAREA, IMAGEINDEX_NETAREA, htRoot);
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//����
//	m_Tree.SortChildren(htRoot);
	return TRUE;
}

/*************************************************************
 �� �� ����InsertStation()
 ���ܸ�Ҫ�����볧վ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1  Ҫ����ĳ�վ����
**************************************************************/
BOOL CDeviceView::InsertStation(CStationObj* pObj)
{
    HTREEITEM htNew;
    HTREEITEM htNet;
	if (g_role != MODE_SUB)
	{
		//��վ�ڵ��ڵ����ڵ�����, Ϊ������
		//�Ȳ��ҵ����ڵ�
		htNet = FindNetPos(pObj->m_sNetID);
		if(htNet == NULL)
			return FALSE;
		
		//����ڵ�
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htNet);
		//����
		m_Tree.SortChildren(htNet);
	} 
	else
	{
        HTREEITEM htRoot = m_Tree.GetRootItem();
        //���ڵ�
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htRoot);
		//htNew= m_Tree.InsertItem(pObj->m_sName, 0, 0, htRoot);
	}
        //��¼�ڵ�
		pObj->m_htItem = htNew;
		//�豸����
		m_Tree.SetItemData(htNew, (DWORD)pObj);
		
		
		//������
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
	//�������
	m_List.DeleteAllItems();
	m_nListType = 0;

	//�������� 
	if(htCurrent != NULL)
	{
		CDeviceObj * pObj = (CDeviceObj*)(m_Tree.GetItemData(htCurrent));
		if(pObj != NULL)
		{
			if(pObj->m_nType == TYPE_STATION)
			{
				m_nListType = 1;
				CStationObj * pStation = (CStationObj*)pObj;
				//�ǳ�վ
				//��һ��,��վ��
				m_List.InsertItem(LVIF_TEXT, 0, StringFromID(IDS_MODEL_SUBSTATION), 0, 0, 0, 0);
				m_List.SetItemText(0, 1, pStation->m_sName);
				//�ڶ���,״̬
				m_List.InsertItem(LVIF_TEXT, 1, StringFromID(IDS_FUNC_COMMUSTATUS), 0, 0, 0, 0);
				m_List.SetItemText(1, 1, pStation->IsCommStatusON()?StringFromID(IDS_COMMUSTATUS_ON):StringFromID(IDS_COMMUSTATUS_OFF));
				//������,ͨ�ŶϿ�����
				m_List.InsertItem(LVIF_TEXT, 2, StringFromID(IDS_STAT_OFFDEVICE), 0, 0, 0, 0);
				CString str;
				str.Format("%d", pStation->GetCommAbnormityNum());
				m_List.SetItemText(2, 1, str);
				//������,�����쳣����
				m_List.InsertItem(LVIF_TEXT, 3, StringFromID(IDS_STAT_RUNABNORMAL), 0, 0, 0, 0);
				str.Format("%d", pStation->GetRunAbnormityNum());
				m_List.SetItemText(3, 1, str);
				//������,����
				m_List.InsertItem(LVIF_TEXT, 4, StringFromID(IDS_MODEL_MANUFACTURER), 0, 0, 0, 0);
				m_List.SetItemText(4, 1, pStation->m_sManufacturer);
				//������,IP
				m_List.InsertItem(LVIF_TEXT, 5, StringFromID(IDS_COMMON_IP), 0, 0, 0, 0);
				m_List.SetItemText(5, 1, pStation->m_sIP);
				//������,��Լ
				m_List.InsertItem(LVIF_TEXT, 6, StringFromID(IDS_COMMON_PROTOCOL), 0, 0, 0, 0);
				m_List.SetItemText(6, 1, pStation->GetProtocolName());
			}
			else if(pObj->m_nType == TYPE_NET)
			{
				m_nListType = 2;
				CNetObj * pNet = (CNetObj*)pObj;
				pNet->GetCommAbnormityNum();
				//�ǵ���
				//��һ��,������
				m_List.InsertItem(LVIF_TEXT, 0, StringFromID(IDS_MODEL_AREA), 0, 0, 0, 0);
				m_List.SetItemText(0, 1, pNet->m_sName);
				//�ڶ���,ͨ�ŶϿ���վ
				m_List.InsertItem(LVIF_TEXT, 1, StringFromID(IDS_STAT_OFFSTATION), 0, 0, 0, 0);
				CString str;
				str.Format("%d", pNet->m_nStationAbnormity);
				m_List.SetItemText(1, 1, str);
				//������,ͨ�ŶϿ�����
				m_List.InsertItem(LVIF_TEXT, 2, StringFromID(IDS_STAT_OFFDEVICE), 0, 0, 0, 0);
				str.Format("%d", pNet->m_nPTCommAbnormity);
				m_List.SetItemText(2, 1, str);
				//������,�����쳣����
				m_List.InsertItem(LVIF_TEXT, 3, StringFromID(IDS_STAT_RUNABNORMAL), 0, 0, 0, 0);
				str.Format("%d", pNet->m_nPTRunAbnormity);
				m_List.SetItemText(3, 1, str);
			}
			else if(pObj->m_nType == TYPE_PROTECT || pObj->m_nType == TYPE_WAVEREC)
			{
				m_nListType = 3;
				CSecObj* pSec = (CSecObj*)pObj;
				//��һ��,�豸��
				m_List.InsertItem(LVIF_TEXT, 0, StringFromID(IDS_COMMON_DEVICENAME), 0, 0, 0, 0);
				m_List.SetItemText(0, 1, pSec->m_sName);
				//�ڶ���,ͨ��
				m_List.InsertItem(LVIF_TEXT, 1, StringFromID(IDS_FUNC_COMMUSTATUS), 0, 0, 0, 0);
				m_List.SetItemText(1, 1, GetCommStatusString(pSec->m_nCommStatus));
				//������,����
				m_List.InsertItem(LVIF_TEXT, 2, StringFromID(IDS_FUNC_RUNSTATUS), 0, 0, 0, 0);
				m_List.SetItemText(2, 1, GetRunStatusString(pSec->m_nRunStatu));
				//������,��վ��
				m_List.InsertItem(LVIF_TEXT, 3, StringFromID(IDS_MODEL_SUBSTATION), 0, 0, 0, 0);
				m_List.SetItemText(3, 1, pSec->m_pStation==NULL?"":pSec->m_pStation->m_sName);
				//������,һ���豸
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
	//�ж��Ƿ�վ
	CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
		return;
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	if(pObj->m_nType == TYPE_STATION)
	{
		//��վ
		CStationObj * pStation = (CStationObj*)pObj;

		if(g_bShowDeviceMapWnd) //��ʾһ�ν���ͼ
		{
			//�ر�֮ǰͼ��,����ѡ��վͼ��
			CXJBrowserView* pSvgView = pApp->GetSVGView();
			if(pSvgView == NULL)
				return;
			if(pStation->m_sID != pSvgView->m_sStationID || pSvgView->m_nSvgType != 1)
			{
				//����ѡ��ĳ�վ�뵱ǰ�򿪵�ͼ�β���ͬһ��վ
				//�رվɵ�,���µ�
				pSvgView->CloseCurrentView();
				pSvgView->OpenStationView(pStation->m_sID);
				pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
			}
			else
			{
				//����ͼ�Ƶ�ǰ��
				pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
			}
		}
		else //��ʾ��վ��Ϣ����
		{
			pApp->ShowStationInfo(pStation);
		}
	}
	else if(pObj->m_nType == TYPE_BAY)
	{
		if(g_bShowDeviceMapWnd) //��ʾһ�ν���ͼ
		{
			//���
			CBayObj* pBay = (CBayObj*)pObj;
			//�ر�֮ǰͼ��,����ѡ��վͼ��
			CXJBrowserView* pSvgView = pApp->GetSVGView();
			if(pSvgView == NULL)
				return;
			if(pBay != pSvgView->m_pCurBay || pSvgView->m_nSvgType != 2)
			{
				//����ѡ��ĳ�վ�뵱ǰ�򿪵�ͼ�β���ͬһ��վ
				//�رվɵ�,���µ�
				pSvgView->CloseCurrentView();
				pSvgView->OpenBayView(pBay);
				pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
			}
			else
			{
				//����ͼ�Ƶ�ǰ��
				pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
			}
		}
	}
	else if(pObj->m_nType == TYPE_PROTECT || pObj->m_nType == TYPE_WAVEREC || pObj->m_nType == TYPE_SEC)
	{
		//������ҳ
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		CSecObj* pSec = (CSecObj*)pObj;
		//�򿪱�������ҳ

		if(pSec->m_nType == TYPE_PROTECT)
			pDoc->ShowSecPropPage(pSec);
		else
			pDoc->ShowSecPropPage(pSec, 1);
	}
	
	*pResult = 0;
}

/*************************************************************
 �� �� ����OnCustomDraw()
 ���ܸ�Ҫ��
 �� �� ֵ: 
 ��    ����param1
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
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //������
				int nSubItem = lplvcd->iSubItem; //������
				//�ȸ�Ĭ��ֵ, ���������⴦��
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
					//��ǰ״̬��
					if(nItem  == 1)
					{
						//ֵ��
						if(nSubItem == 1)
						{
							CString str = m_List.GetItemText(nItem, nSubItem);
							//�Ͽ�ʱ������ʾ
							if(str == StringFromID(IDS_COMMUSTATUS_OFF))
							{
								lplvcd->clrText = XJ_STATUS_COMM_OFF;
							}
						}
					}
					//ͨ���쳣������,����״̬�쳣������
					if(nItem == 2 || nItem == 3 || nItem == 1)
					{
						//ֵ��s
						if(nSubItem == 1)
						{
							CString str = m_List.GetItemText(nItem, nSubItem);
							int nCount = atoi(str);
							//�쳣������0ʱ������ʾ
							if(nCount > 0)
							{
								lplvcd->clrText = XJ_STATUS_COMM_OFF;
							}
						}
					}
				}
				else if(m_nListType == 3) //�����豸
				{
					//��ǰ״̬��
					if(nItem  == 1)
					{
						//ֵ��
						if(nSubItem == 1)
						{
							CString str = m_List.GetItemText(nItem, nSubItem);
							//�Ͽ�ʱ������ʾ
							if(str != GetCommStatusString(0))
							{
								lplvcd->clrText = XJ_STATUS_COMM_OFF;
							}
						}
					}
					if(nItem  == 2)
					{
						//ֵ��
						if(nSubItem == 1)
						{
							CString str = m_List.GetItemText(nItem, nSubItem);
							//�Ͽ�ʱ������ʾ
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
 �� �� ����LocateInTree()
 ���ܸ�Ҫ�����豸���ж�λָ����վ
 �� �� ֵ: void
 ��    ����param1	ָ����վ
**************************************************************/
void CDeviceView::LocateInTree( CStationObj* pStation )
{
	if(pStation == NULL)
		return;
	//�жϱ���Ľڵ��Ƿ���Ч
	if(pStation->m_htItem == NULL)
		return;
	m_Tree.SelectItem(pStation->m_htItem);
	m_Tree.EnsureVisible(pStation->m_htItem);
}

void CDeviceView::LocateObjInTree( CDeviceObj* pObj )
{
	if(pObj == NULL)
		return;
	//�жϱ���Ľڵ��Ƿ���Ч
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
		//�ýڵ㱻ѡ��
		m_Tree.SelectItem(hItem);
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hItem);
		if(pObj == NULL)
			return;
	    CMenu Menu, *pMenu;
		if(pObj->m_nType == TYPE_STATION || pObj->m_nType == TYPE_BAY) //ֻ����վ�����
		{
			//�����˵�
			//���ɲ˵�
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
			//���������˵�
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
			//����¼�����˵�
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
	//�ж��Ƿ�վ
	CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
		return;
	if(pObj == NULL)
		return;
	if(pObj->m_nType == TYPE_STATION)
	{
		//��վ
		CStationObj * pStation = (CStationObj*)pObj;
		
		//�ر�֮ǰͼ��,����ѡ��վͼ��
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserView* pSvgView = pApp->GetSVGView();
		if(pSvgView == NULL)
			return;
		if(pStation->m_sID != pSvgView->m_sStationID || pSvgView->m_nSvgType != 1)
		{
			//����ѡ��ĳ�վ�뵱ǰ�򿪵�ͼ�β���ͬһ��վ
			//�رվɵ�,���µ�
			pSvgView->CloseCurrentView();
			pSvgView->OpenStationView(pStation->m_sID);
			pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
		else
		{
			//����ͼ�Ƶ�ǰ��
			pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
	}
	else if(pObj->m_nType == TYPE_BAY)
	{
		//���
		CBayObj* pBay = (CBayObj*)pObj;
		//�ر�֮ǰͼ��,����ѡ��վͼ��
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserView* pSvgView = pApp->GetSVGView();
		if(pSvgView == NULL)
			return;
		if(pBay != pSvgView->m_pCurBay || pSvgView->m_nSvgType != 2)
		{
			//����ѡ��ĳ�վ�뵱ǰ�򿪵�ͼ�β���ͬһ��վ
			//�رվɵ�,���µ�
			pSvgView->CloseCurrentView();
			pSvgView->OpenBayView(pBay);
			pSvgView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
		else
		{
			//����ͼ�Ƶ�ǰ��
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
	//�ж��Ƿ�վ
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
 �� �� ��: InsertDevice()
 ���ܸ�Ҫ: �����豸���豸��
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ�����ڵ�
		   Param2 ָ���豸
**************************************************************/
BOOL CDeviceView::InsertDevice( HTREEITEM htParent, CDeviceObj* pObj)
{
	//���������Ч��
	if(htParent == NULL)
		return FALSE;
	if(pObj == NULL)
		return FALSE;

	//ȷ��ͼ������
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
	//����

	HTREEITEM htNew;
	htNew = m_Tree.InsertItem(pObj->m_sName, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, htParent);
	if(htNew == NULL)
		return FALSE;
	//��¼�ڵ�λ��
	pObj->m_htItem = htNew;
	//��������
		m_Tree.SetItemData(htNew, (DWORD)pObj);
	return TRUE;
}

/*************************************************************
 �� �� ��: InsertStation_Voltage()
 ���ܸ�Ҫ: ���볧վ�ڵ㣬��վ�½ӵ�ѹ�ȼ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 Ҫ����ĳ�վ����
		   Param2 
**************************************************************/
BOOL CDeviceView::InsertStation_Voltage( CStationObj* pObj )
{
    HTREEITEM htNew;
	HTREEITEM htNet;
	if (g_role != MODE_SUB)
	{	
		//��վ�ڵ��ڵ����ڵ�����, Ϊ������
		//�Ȳ��ҵ����ڵ�
		htNet = FindNetPos(pObj->m_sNetID);
		if(htNet == NULL)
			return FALSE;
		
		//����ڵ�
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htNet);
	    //����
		m_Tree.SortChildren(htNet);
	}
	else
	{
		HTREEITEM htRoot = m_Tree.GetRootItem();
        //��վ�ڵ�
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->IsCommStatusON() ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htRoot);
	}

        //��¼�ڵ�
		pObj->m_htItem = htNew;
		//�豸����
		m_Tree.SetItemData(htNew, (DWORD)pObj);	
	
	if(g_DeviceTreeType == 4)
	{
		//�����豸ֱ�ӹ��ڳ�վ��
		BuildSecToStation(pObj);
		//����
		m_Tree.SortChildren(htNew);
	}
	else
	{
		if( !pObj->m_IsDistanceVirtual)
		{
			//�Ե�ѹ�ȼ�����
			SortVoltage(pObj);
			
			//��ӵ�ѹ�ȼ���
			for(int i = 0; i < pObj->m_arrVoltage.GetSize(); i++)
			{
				int nVoltage = pObj->m_arrVoltage.GetAt(i);
				InsertVoltage(nVoltage, htNew, pObj);
			}
			
			//��ӱ���
			BuildProtect(pObj);
			
			//���¼����
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
 �� �� ��: InsertVoltage()
 ���ܸ�Ҫ: �����ѹ�ȼ��ڵ�,�ڵ��¸��豸
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ����ѹ�ȼ�
		   Param2 ָ�����ڵ�
**************************************************************/
BOOL CDeviceView::InsertVoltage( int nVoltage, HTREEITEM htParent, CStationObj* pObj )
{
	if(htParent == NULL)
		return FALSE;
	if(pObj == NULL)
		return FALSE;
	//����ڵ�
	CString str;
	str.Format("%d kV", nVoltage);
	HTREEITEM htNew = m_Tree.InsertItem(str, IMAGEINDEX_VOLTAGE, IMAGEINDEX_VOLTAGE, htParent);


	if(g_DeviceTreeType == 2)
	{
		//������
		BuildBay(pObj, htNew, nVoltage);
	}
	else
	{
		//����Ӳ�ڵ�
		InsertFastness(htNew, pObj, nVoltage, "");
	}



	return TRUE;
}

/*************************************************************
 �� �� ��: SortVoltage()
 ���ܸ�Ҫ: ��վ�ڵ�ѹ�ȼ�����
 �� �� ֵ: 
 ��    ��: param1 ָ��վ
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
 �� �� ��: BuildBreak()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������п��ؼ��뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CDeviceView::BuildBreak( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildBus()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ�������ĸ�߼��뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CDeviceView::BuildBus( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID)
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildCap()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������е��������뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CDeviceView::BuildCap( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildGen()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������з�������뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CDeviceView::BuildGen( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildGen()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������з�������뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CDeviceView::BuildLine( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildReactor()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������е翹�����뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CDeviceView::BuildReactor( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildSwitch()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������е�բ���뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CDeviceView::BuildSwitch( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildTrans()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������б�ѹ�����뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CDeviceView::BuildTrans( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildProtect()
 ���ܸ�Ҫ: ��ָ����վ�����б������뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
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
 �� �� ��: InsertProtect()
 ���ܸ�Ҫ: ���뱣�����豸��
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ������
		   Param2 
**************************************************************/
BOOL CDeviceView::InsertProtect( CSecObj* pObj)
{
	//���������Ч��
	if(pObj == NULL)
		return FALSE;
	if(pObj->m_nType != TYPE_PROTECT)
		return FALSE;
	if(pObj->m_pOwner == NULL)
		return FALSE;
	if(pObj->m_pOwner->m_htItem == NULL)
		return FALSE;
	//���뵽�豸��
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
	
	
	//��¼λ��
	pObj->m_htItem = htNew;
	m_Tree.SortChildren(pObj->m_pOwner->m_htItem);

	//��������
	m_Tree.SetItemData(htNew, (DWORD)pObj);	
	
	return TRUE;
}

/*************************************************************
 �� �� ��: BuildWaveRec()
 ���ܸ�Ҫ: ��ָ����վ������¼�������뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
**************************************************************/
void CDeviceView::BuildWaveRec( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	
	//ѭ��,���뵽�豸
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
 �� �� ��: InsertFastness()
 ���ܸ�Ҫ: ���Ӳ�ڵ�
 �� �� ֵ: 
 ��    ��: param1 ָ�����ڵ�
		   Param2 ָ����վ
		   param3 ָ����ѹ�ȼ�
		   param4 ָ�����ID
**************************************************************/
void CDeviceView::InsertFastness( HTREEITEM htParent, CStationObj* pObj, int nVoltage, CString sBayID )
{
	//�����һ���豸����
	HTREEITEM htBus		= m_Tree.InsertItem(StringFromID(IDS_MODEL_BUS), IMAGEINDEX_BUS, IMAGEINDEX_BUS, htParent);
	HTREEITEM htLine	= m_Tree.InsertItem(StringFromID(IDS_MODEL_LINE), IMAGEINDEX_LINE, IMAGEINDEX_LINE, htParent);
	HTREEITEM htGen		= m_Tree.InsertItem(StringFromID(IDS_MODEL_GEN), IMAGEINDEX_GEN, IMAGEINDEX_GEN, htParent);
	HTREEITEM htTrans	= m_Tree.InsertItem(StringFromID(IDS_MODEL_TRANS), IMAGEINDEX_TRANS, IMAGEINDEX_TRANS, htParent);
	HTREEITEM htCap		= m_Tree.InsertItem(StringFromID(IDS_MODEL_CAP), IMAGEINDEX_CAP, IMAGEINDEX_CAP, htParent);
	HTREEITEM htReactor = m_Tree.InsertItem(StringFromID(IDS_MODEL_REACTOR), IMAGEINDEX_REACTOR, IMAGEINDEX_REACTOR, htParent);
	HTREEITEM htSwitch	= m_Tree.InsertItem(StringFromID(IDS_MODEL_SWITCH), IMAGEINDEX_SWITCH, IMAGEINDEX_SWITCH, htParent);
	HTREEITEM htBreak	= m_Tree.InsertItem(StringFromID(IDS_MODEL_BREAKER), IMAGEINDEX_BREAK, IMAGEINDEX_BREAK, htParent);
	
	//change
	//�����һ���豸
	//��·��
	BuildBreak(pObj, htBreak, nVoltage, sBayID);
	m_Tree.SortChildren(htBreak);
	//ĸ��
	BuildBus(pObj, htBus, nVoltage, sBayID);
	m_Tree.SortChildren(htBus);
	//������
	BuildCap(pObj, htCap, nVoltage, sBayID);
	m_Tree.SortChildren(htCap);
	//�����
	BuildGen(pObj, htGen, nVoltage, sBayID);
	m_Tree.SortChildren(htGen);
	//��·
	BuildLine(pObj, htLine, nVoltage, sBayID);
	m_Tree.SortChildren(htLine);
	//�翹��
	BuildReactor(pObj, htReactor, nVoltage, sBayID);
	m_Tree.SortChildren(htReactor);
	//��բ
	BuildSwitch(pObj, htSwitch, nVoltage, sBayID);
	m_Tree.SortChildren(htSwitch);
	//��ѹ��
	BuildTrans(pObj, htTrans, nVoltage, sBayID);
	m_Tree.SortChildren(htTrans);
}

/*************************************************************
 �� �� ��: BuildBay()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������м�����뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   param3 ָ����ѹ�ȼ�
**************************************************************/
void CDeviceView::BuildBay( CStationObj* pStation, HTREEITEM htParent, int nVoltage )
{
	//���������Ч��
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
 �� �� ��: InsertBay()
 ���ܸ�Ҫ: ���������豸��
 �� �� ֵ: 
 ��    ��: param1 ָ�����ڵ�
		   Param2 ָ����վ
		   Param3 ָ�����
**************************************************************/
void CDeviceView::InsertBay( HTREEITEM htParent, CStationObj* pObj, CBayObj* pBay)
{
	//���������Ч��
	if(htParent == NULL)
		return;
	if(pObj == NULL)
		return;
	if(pBay == NULL)
		return;
	
	//ȷ��ͼ������
	int nImageIndex = IMAGEINDEX_STATION;
	//����
	HTREEITEM htNew = m_Tree.InsertItem(pBay->m_sName, nImageIndex, nImageIndex, htParent);
	if(htNew == NULL)
		return;
	//��¼�ڵ�λ��
	pBay->m_htItem = htNew;
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pBay);

	//����Ӳ�ڵ�
	InsertFastness(htNew, pObj, 0, pBay->m_sID);
}

/*************************************************************
 �� �� ��: ClearEmptyNodeType1()
 ���ܸ�Ҫ: ����豸������Ϊ1���豸���Ŀսڵ�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDeviceView::ClearEmptyNodeType1()
{
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL) //����Ӳ�ڵ�
		return;
	
	HTREEITEM htLevel1 = m_Tree.GetChildItem(htRoot);
	while(htLevel1 != NULL) //�����ڵ�
	{
		HTREEITEM htLevel2 = m_Tree.GetChildItem(htLevel1);
		while(htLevel2 != NULL) //��վ�ڵ�
		{
			HTREEITEM htLevel3 = m_Tree.GetChildItem(htLevel2);
			while(htLevel3 != NULL) //��ѹ�ȼ��ڵ�
			{
				CString str = m_Tree.GetItemText(htLevel3);
				if(str != STRING_WAVEREC)
				{ 
					//��¼������
					HTREEITEM htLevel4 = m_Tree.GetChildItem(htLevel3); 
					while(htLevel4 != NULL)//һ���豸Ӳ�ڵ�
					{
						HTREEITEM htTemp = htLevel4;
						htLevel4 = m_Tree.GetNextItem(htLevel4, TVGN_NEXT);
						if(!m_Tree.ItemHasChildren(htTemp))
						{
							//ɾ�����ڵ�
							m_Tree.DeleteItem(htTemp);
						}
					}
				}
				else
				{
					//¼������
					HTREEITEM htTemp = htLevel3;
					htLevel3 = m_Tree.GetNextItem(htLevel3, TVGN_NEXT);
					if(!m_Tree.ItemHasChildren(htTemp))
					{
						//ɾ�����ڵ�
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
 �� �� ��: ClearEmptyNodeType2()
 ���ܸ�Ҫ: ����豸������Ϊ2���豸���Ŀսڵ�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDeviceView::ClearEmptyNodeType2()
{
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL) //����Ӳ�ڵ�
		return;
	
	HTREEITEM htLevel1 = m_Tree.GetChildItem(htRoot);
	while(htLevel1 != NULL) //�����ڵ�
	{
		HTREEITEM htLevel2 = m_Tree.GetChildItem(htLevel1);
		while(htLevel2 != NULL) //��վ�ڵ�
		{
			HTREEITEM htLevel3 = m_Tree.GetChildItem(htLevel2);
			while(htLevel3 != NULL) //��ѹ�ȼ��ڵ�
			{
				CString str = m_Tree.GetItemText(htLevel3);
				if(str != StringFromID(IDS_MODEL_RECORDER))
				{ 
					//��¼������
					HTREEITEM htLevel4 = m_Tree.GetChildItem(htLevel3); 
					while(htLevel4 != NULL)//����ڵ�
					{
						HTREEITEM htLevel5 = m_Tree.GetChildItem(htLevel4);
						while(htLevel5 != NULL)
						{
							HTREEITEM htTemp = htLevel5;
							htLevel5 = m_Tree.GetNextItem(htLevel5, TVGN_NEXT);
							if(!m_Tree.ItemHasChildren(htTemp))
							{
								//ɾ�����ڵ�
								m_Tree.DeleteItem(htTemp);
							}		
						}
						htLevel4 = m_Tree.GetNextItem(htLevel4, TVGN_NEXT);
					}
				}
				else
				{
					//¼������
					HTREEITEM htTemp = htLevel3;
					htLevel3 = m_Tree.GetNextItem(htLevel3, TVGN_NEXT);
					if(!m_Tree.ItemHasChildren(htTemp))
					{
						//ɾ�����ڵ�
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
	//ȡ��һ��
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//�Ǳ���
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
	//ȡ��
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_WAVEREC)
	{
		//��¼����
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
	//ȡ��
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//�Ǳ���
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
	//ȡ��
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//�Ǳ���
		CSecObj* pObj = (CSecObj*)pDevice;
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CXJBrowserDoc * pDoc = pApp->GetCurDocument();
		
		//�齨��ѯ����
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
		//����
		m_Tree.SelectItem(hit);//�ýڵ�ѡ��
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
	//ȡ�ó�վ
	CStationObj* pStation = (CStationObj*)lParam;
	if(pStation == NULL)
		return;
	//ȡ���б�ĵ�ǰ��ʾ
	if(htCurrent == NULL)
		return;
	CDeviceObj * pObj = (CDeviceObj*)(m_Tree.GetItemData(htCurrent));
	if(pObj == NULL)
		return;
	//�ж�����
	if(pObj->m_nType == TYPE_STATION)//��վ
	{
		CStationObj* pCurrent = (CStationObj*)pObj;
		if(pCurrent == pStation) //ͬһ��վ
		{
			//������������
			FillList();
		}
	}
	else if(pObj->m_nType == TYPE_NET)//����
	{
		if(pObj->m_sID == pStation->m_sNetID)//�仯���Ǳ������µ��豸
		{
			//������������
			FillList();
		}
	}
}


//��������״̬����
void CDeviceView::OnMENUITEMDevTreePTRunUpdate() 
{
	// TODO: Add your command handler code here
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

	HTREEITEM ht = m_Tree.GetSelectedItem();
	if(ht == NULL)
		return;
	//�ж��Ƿ�վ
	CDeviceObj * pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
	if(pObj == NULL)
		return;
	if(pObj->m_nType != TYPE_PROTECT && pObj->m_nType != TYPE_WAVEREC && pObj->m_nType != TYPE_SEC)
		return;

	if(!pApp->TryEnter("ϵͳ����"))
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
       	//���ͨѶ���
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
		//�ж��Ƿ�վ
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
	//���������Ч��
	if(pObj == NULL)
		return FALSE;
	if(pObj->m_pStation == NULL)
		return FALSE;
	if(pObj->m_pStation->m_htItem == NULL)
		return FALSE;
	//���뵽�豸��
	CString str;
	str = pObj->m_sName;
	HTREEITEM htNew = m_Tree.InsertItem(str, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, pObj->m_pStation->m_htItem);
	if(htNew == NULL)
		return FALSE;
	
	//��¼λ��
	pObj->m_htItem = htNew;
	//m_Tree.SortChildren(pObj->m_pStation->m_htItem);
	
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pObj);	
	
	return TRUE;
}

/****************************************************
Date:2011/10/14  Author:LYH
������:   IsChinese	
����ֵ:   BOOL	
���ܸ�Ҫ: 
����: CString str	
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
������:   CompareStation	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString str	
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

	if(IsChinese(str)) //����Ϊ����,ֱ���볧վ���Ƚ�
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
		//ƴ��
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
				//���뵽������				
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
		//���
		//	pCMB ->ResetContent();
		int count = pCMB ->GetCount();
		for (int i=0;i < count; i++)
		{
			pCMB->DeleteString( 0 );
		}
		
		//���бȽ�,���������
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
			//�õ���վָ��
			CStationObj * pObj = (CStationObj *)pCMB ->GetItemDataPtr(Index);
			//�豸����λ
			LocateInTree(pObj);
		}
		//���
		pCMB ->ResetContent();
	}
}

void CDeviceView::OnTreeStationProp() 
{
	// TODO: Add your command handler code here
	HTREEITEM ht = m_Tree.GetSelectedItem();
	if(ht == NULL)
		return;
	//�ж��Ƿ�վ
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
������:   ClearEmptyNodeType4	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CDeviceView::ClearEmptyNodeType4()
{
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL) //����Ӳ�ڵ�
		return;
	
	HTREEITEM htLevel1 = m_Tree.GetChildItem(htRoot);
	while(htLevel1 != NULL) //�����ڵ�
	{
		HTREEITEM htLevel2 = m_Tree.GetChildItem(htLevel1);
		while(htLevel2 != NULL) //��վ�ڵ�
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
������:   OnViewRefresh	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CDeviceView::OnViewRefresh( WPARAM wParam, LPARAM lParam )
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	
	//���г�վ
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

	WriteLog("CDeviceView::OnViewRefresh, �豸��ǿ��ˢ�����", XJ_LOG_LV3);
}

void CDeviceView::OnTreeStationAdd() 
{
	// TODO: Add your command handler code here
	try
	{
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
       	//���ͨѶ���
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
		//�ж��Ƿ�վ
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
       	//���ͨѶ���
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
		//�ж��Ƿ�վ
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
       	//���ͨѶ���
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
		//�ж��Ƿ�վ
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
	//ȡ��
	CDeviceObj * pDevice = (CDeviceObj*)m_Tree.GetItemData(htItem);
	if(pDevice == NULL)
		return;
	if(pDevice->m_nType == TYPE_PROTECT)
	{
		//�Ǳ���
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
	//ȡ��
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
 �� �� ��: InsertPTDevice()
 ���ܸ�Ҫ: ���뱣���豸���豸��
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ�����ڵ�
		   Param2 ָ���豸
**************************************************************/
BOOL CDeviceView::InsertPTDevice(HTREEITEM htParent, CDeviceObj* pObj)
{
		//���������Ч��
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
	//��¼�ڵ�λ��
	pObj->m_htItem = htNew;
	//��������
		m_Tree.SetItemData(htNew, (DWORD)pObj);
	return TRUE;
}

/****************************************************
Date:2013/6/4  Author:LYH
������:   OnStationInit	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
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
	//�ж��Ƿ�վ
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
	//���������Ч��
	if(htParent == NULL)
		return;
	
	//ѭ��,���뵽�豸
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
������:   TestStationStatus	
����ֵ:   void	
���ܸ�Ҫ: 
����: CStationObj * pStation	
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
		//��֤Ȩ��
		if(!pApp->TryEnter(FUNC_XJBROWSER_OPERATE))
			return;
	}
	
	//���ͨѶ���
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(StringFromID(IDS_STATIONCHANGE_NOTIFY_FAIL));
		return;
	}
	
	m_sResult = "";
	
	//��ѯ��վ״̬
	if(pStation->m_nIPFlag == 0) //��ͨ��
	{
		AddToCallList(pStation, 0);
	}
	else //˫ͨ��
	{
		AddToCallList(pStation, 0);
		AddToCallList(pStation, 1);
	}
	
	//��ʼ�ٻ�
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
 �� �� ��: QueryStationStatus()
 ���ܸ�Ҫ: ��ѯ��վͨѶ״̬
 �� �� ֵ: int ʧ�ܷ���<0���������ɹ�����0 
 ��    ��: param1 ָ����վ
		   Param2 ָ��ͨ��
**************************************************************/
int CDeviceView::QueryStationStatus( CStationObj* pObj, int nChannel /*= 0*/ )
{
	//�����ȷ��
	MYASSERT_INT(pObj);

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//���뱨����������
	STTP_FULL_DATA sttpData;
	//��֯20145����
	if(!pApp->m_SttpEngine.BuildDataFor20145StationStatus(sttpData, pObj->m_sID, nChannel))
	{
		//��֯����ʧ��
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
	
	//���ͱ���
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ж��Ƿ�ɹ�
	if(nReturn != 1)
	{
		//ʧ��
		if(nReturn == -1)
		{
			CString str;
			str.Format("��վ%s�����ٻ�״̬����ʧ��,ԭ��Ϊ�����ж�", pObj->m_sID);
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
			str.Format("��վ%s�����ٻ�״̬����ʧ��,ԭ��Ϊ��������", pObj->m_sID);
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
			str.Format("��վ%s�����ٻ�״̬����ʧ��,ԭ��Ϊδ�ҵ���Ӧ������", pObj->m_sID);
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
			str.Format("��վ%s�����ٻ�״̬����ʧ��,ԭ��δ֪", pObj->m_sID);
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
		//������ʱ��
		CDataEngine * pData = pApp->GetDataEngine();
		int nTimeOut = pData->m_nDevGeneralTimeOut;
		m_nQueryTimer = SetTimer(2, nTimeOut*1000, 0);
	}
	return 0;
}

/*************************************************************
 �� �� ��: OnSTTP20146()
 ���ܸ�Ҫ: ��Ӧ20146(��վͨѶ״̬����֪ͨ)
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDeviceView::OnSTTP20146( WPARAM wParam, LPARAM lParam )
{
	WriteLog("CDeviceView::OnSTTP20146");
	if(m_arrCall.GetSize() <= 0)
	{
		//û�����ٻ���վͨѶ״̬,��������
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20146����
	if(pSttpData->sttp_head.uMsgID != 20146)
	{
		//������
		CString str;
		str.Format("CDeviceView::OnSTTP20146 �յ�������,����IDΪ%d", pSttpData->sttp_head.uMsgID);
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
	
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_station_id;
	if(strID != pCall->pStation->m_sID)
	{
		//���Ǳ�װ�õı���
		CString str;
		str.Format("CDeviceView::OnSTTP20146 �յ�������վ����, ��վIDΪ%s", strID);
		WriteLog(str);
		return;
	}

	int nChannel = pSttpData->sttp_body.nFlag;
	if(nChannel != pCall->nChannel)
	{
		//���Ǳ�ͨ���ı���
		CString str;
		str.Format("CDeviceView::OnSTTP20146 �յ�����ͨ���ı���, ͨ��Ϊ%d", nChannel);
		WriteLog(str);
		return;
	}
	
	//�رն�ʱ��
	KillTimer(m_nQueryTimer);
	
	//������
	
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
			//�������
			//SendMessage(COMMVIEW_QUERY_END);
			// ���ʱ��Ϊ��,��������ͨѶ״̬���˴���
			WriteLog("CDeviceView::OnSTTP20146, �յ�ʱ��Ϊ��");
			tmStation =  CTime::GetCurrentTime();
			tmReason =  CTime::GetCurrentTime();
		}
		else
		{
			tmStation = StringToTimeSttp12(strStationTime);
			tmReason = StringToTimeSttp12(strReasonTime);
		}
		pCall->pStation->SetStatus(nStatus, tmStation, nReason, nChannel, tmReason, pCall->pStation->m_nIPFlag);
		
		//֪ͨ�豸����ͼ�ν���
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		CMainFrame* pFrame=(CMainFrame*)pApp->GetMainWnd();
		if(pFrame != NULL)
		{
			WriteLog("CDeviceView::OnSTTP20146, ״̬��λ������֪ͨ");
			pFrame->PostMessage(STATION_CHANGED, 0, (LPARAM)pCall->pStation);
		}
	}
	else
	{
		WriteLog("CDeviceView::OnSTTP20146, ״̬δ��λ��������֪ͨ");
	}
	
	//�������
	SendMessage(COMMVIEW_QUERY_END);
}

/*************************************************************
 �� �� ��: OnSTTP20069()
 ���ܸ�Ҫ: ��Ӧ20069(�ٻ�ʧ��֪ͨ)
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDeviceView::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//��ǰû�����ٻ�
	if(m_arrCall.GetSize() <= 0)
	{
		return;
	}
	
	//ȡ�ñ���
	STTP_FULL_DATA * pSttpData = (STTP_FULL_DATA*)lParam;
	if(pSttpData == NULL)
	{
		//����Ϊ��
		return;
	}
	
	//����Ƿ�20069����
	if(pSttpData->sttp_head.uMsgID != 20069)
	{
		//������
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
	//����Ƿ��Լ�װ�õı���
	CString strID = pSttpData->sttp_body.ch_pt_id;
	if(strID != pCall->pStation->m_sID)
	{
		//���Ǳ�װ�õı���
		return;
	}
	
	//�رն�ʱ��
	KillTimer(m_nQueryTimer);
	
	//������
	int nStatus = pSttpData->sttp_body.nStatus;
	if(0)
	{
		//�����ɹ�
	}
	else
	{
		//����ʧ��
		CString str;
		str.Format("��վ %s �ٻ�ͨѶ״̬ʧ��!", pCall->pStation->m_sID);
		WriteLog(str);
	}
	
	//�������
	SendMessage(COMMVIEW_QUERY_END);
}

/*************************************************************
 �� �� ��: ClearCallList()
 ���ܸ�Ҫ: ����ٻ��б�
 �� �� ֵ: 
 ��    ��: param1 
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
 �� �� ��: AddToCallList()
 ���ܸ�Ҫ: ��ָ����վָ��ͨ�����뵽���ٻ��б�
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ��ͨ��
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
 �� �� ��: StartCall()
 ���ܸ�Ҫ: ���ٻ�������ٻ�
 �� �� ֵ: 
 ��    ��: param1 ָ���ٻ���
		   Param2 
**************************************************************/
void CDeviceView::StartCall( StationStauts_Call* pCall)
{
	MYASSERT(pCall);
	if(QueryStationStatus(pCall->pStation, pCall->nChannel) < 0)
	{
		//���в�ѯʧ��
		pCall->nRelust = -1;
	}
	else
	{
		//���в�ѯ�ɹ�
		AfxMessageBox(StringFromID(IDS_CALL_SENDMSG_SUCCESS));
	}
}

/*************************************************************
 �� �� ��: OnEndCallStationStatus()
 ���ܸ�Ҫ: ��Ӧ�ٻ������Ϣ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDeviceView::OnEndCallStationStatus( WPARAM wParam, LPARAM lParam )
{
	//ȡ���б��еĵ�һ������
	if(m_arrCall.GetSize() <= 0)
	{
		return;
	}
	StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
	MYASSERT(pCall);
	CStationObj* pStation = pCall->pStation;
	MYASSERT(pStation);
	//�ٻ����
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
	
	//�ж��Ƿ��ٻ�ȫ������
	if(m_arrCall.GetSize() > 0)
	{
		//�����ٻ�
		StationStauts_Call* pCall = (StationStauts_Call*)m_arrCall.GetAt(0);
		StartCall(pCall);
	}
	else
	{
		//�ٻ�����
		AfxMessageBox(m_sResult);
	}
	//ɾ����һ��
	MYDELETE(pCall);
}

void CDeviceView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	if(nIDEvent == m_nTimer)
	{
		//�رն�ʱ��
		//KillTimer(m_nTimer);

		CXJPTSetStore *p = CXJPTSetStore::GetInstance();
		QPTSetCard &card = *(reinterpret_cast<QPTSetCard *>(p->GetCard()));
		QLogTable &log = *(reinterpret_cast<QLogTable *>(p->GetLog()));
		
		int nState = card.GetStateID();
		if (-1 == m_nDZ_MOD_State){
			m_nDZ_MOD_State = nState;
		}

		// ֻ�ڹ��ƺ�ȡ������ʱˢ��
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
