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


	//��������
	//LoadConfig();
	
	//��ʼ���б�
	InitListCtrl();

	//����ʱ���
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_START);
	if(pTime == NULL)
		return false;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_PTOSCLIST_END);
	if(pTime2 == NULL)
		return false;
	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	pTime2->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//���÷�Χ
	CTime tmMin(1972, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	pTime2->SetRange(&tmMin, &tmMax);
	//����ʱ��
	CTime tm = CTime::GetCurrentTime();
	CTimeSpan ts(7, 0, 0, 0);
	CTime tm1 = tm - ts;
	VERIFY(pTime->SetTime(&tm1));


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*************************************************************
 �� �� ����LoadListConfig()
 ���ܸ�Ҫ����������Ϣ����
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
BOOL CManualFaultShorten::LoadListConfig()
{
	//��ȡUI�����ļ�
	BOOL bResult = TRUE;
	
	CoInitialize(NULL); //��ʼ��COM
	{
		MSXML2::IXMLDOMDocumentPtr pDoc;
		HRESULT	hr;
		
		//��ʼ��MSXML
		hr=pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if(FAILED(hr))
		{  
			WriteLog("�޷�����DOMDocument���������Ƿ�װ��MS XML Parser ���п�!", XJ_LOG_LV3);
			WriteLog("ViewAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
			bResult = FALSE;
		} 	
		//����UIConfig�ļ� 
		if(bResult)
		{
			if(!pDoc->load((_variant_t)g_UIConfig))
			{
				//����ʧ��
				MSXML2::IXMLDOMParseErrorPtr errPtr = pDoc ->GetparseError();
				CString str;
				str.Format("when load UIConfig, error in line %d, char %d\n%s", errPtr ->Getline(), errPtr ->Getlinepos(), (char *)errPtr ->Getreason());
				WriteLog(str, XJ_LOG_LV3);
				WriteLog("ViewAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
		}

		if(bResult)
		{
			MSXML2::IXMLDOMNodePtr pSelected;
			pSelected = pDoc->selectSingleNode("//ViewActionConfig");

			//����ViewActionConfig
			if(pSelected == NULL)
			{	
				//δ�ҵ�
				WriteLog("δ�ҵ�ViewActionConfig", XJ_LOG_LV3);
				WriteLog("ViewAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
				bResult = FALSE;
			}
			else
			{	//�ҵ�
				if(!pSelected ->hasChildNodes())
				{
					//�Ҳ����ӽڵ�
					WriteLog("δ�ҵ�ViewActionConfig�µ�����", XJ_LOG_LV3);
					WriteLog("ViewAction::��ȡUIConfigʧ��,��ʹ��Ĭ���з��", XJ_LOG_LV3);
					bResult = FALSE;
				}
				else
				{
					//�ҵ��ӽڵ��б�ָ��
					MSXML2::IXMLDOMNodeListPtr pChild;
					pChild = pSelected ->GetchildNodes();
					//ѭ����ȡ�ӽڵ���Ϣ
					for(int i = 0; i < pChild ->Getlength(); i++)
					{
						//�õ��ӽڵ�
						MSXML2::IXMLDOMNodePtr pNode;
						pNode = pChild ->Getitem(i);
						//�õ��ӽڵ������б�ָ��
						MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap;
						pAttrMap = pNode ->Getattributes();
						
						//�����Խṹ
						SColumn	col;
						
						//ѭ����ȡ����
						for(int j = 0; j < pAttrMap ->Getlength(); j++)
						{
							MSXML2::IXMLDOMNodePtr			pItem;
							pItem = pAttrMap ->Getitem(j);
							CString strValue = (char *)(_bstr_t)pItem ->GetnodeValue();
							int nHide = 0;
							int nWidth = 0;
							switch(j)
							{
							case 0: //����
								col.sName = strValue;
								break;
							case 1: //�п�
								nWidth = atoi(strValue);
								col.nItemWidth = nWidth;
								break;
							case 2:	//�Ƿ�����
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
	//�رմ򿪵�COM
	CoUninitialize();
	return bResult;
}



/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���, ������ʾ��������Ϣ
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
int CManualFaultShorten::InitListStyle()
{
	//�Ȳ���UIConfig�����ļ����Ƿ��д��б������Ϣ
	//�����,�������ļ�����Ϣ��������
	//���û��,��Ĭ����������,�����浽�����ļ�

	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvCol.fmt=LVCFMT_CENTER;
	if(LoadListConfig())
	{
		//�������óɹ�
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
		//��������ʧ��
		//char* arColumn[10]={"���","���", "��������","�����豸", "װ������ʱ��","��Ϣ����ʱ��","��Ϣ����","��վ��","һ���豸","��ȷ��"};
		for (int nCol=0; nCol < 10 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 60; //�п�50���� ���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 80; //�п�60���� ���
				colName = StringFromID(IDS_COMMON_CLASS);
				break;
			case 2://
				lvCol.cx = 100; //�п�60���� ��������
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			case 3://
				lvCol.cx = 200; //�п�160���� ��������
				colName = StringFromID(IDS_MODEL_SECONDARY);
				break;
			case 4: 
				lvCol.cx = 180; //װ������ʱ��
				colName = StringFromID(IDS_EVENT_STARTTIME);
				break;
			case 5:
				lvCol.cx = 180; //��Ϣ����ʱ��
				colName = StringFromID(IDS_EVENT_RECIVETIME);
				break;
			case 6:
				lvCol.cx = 200; //��Ϣ����
				colName = StringFromID(IDS_COMMON_CONTENT);
				break;
			case 7:
				lvCol.cx = 140; //��վ
				colName = StringFromID(IDS_MODEL_SUBSTATION);
				break;
			case 8:
				lvCol.cx = 160; //һ���豸
				colName = StringFromID(IDS_MODEL_PRIMARY);
				break;
			case 9:
				lvCol.cx = 100; //��ȷ��
				colName = StringFromID(IDS_ACK_ISACKED);
				break;
			default:
				lvCol.cx=100;//�п�100����
			}
			lvCol.pszText= colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
		//Ĭ�����ص�3��(��������)
		m_List.SetColumnHide(2, TRUE);
		//Ĭ������"ȷ��"
		m_List.SetColumnHide(9, TRUE);
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
	return 0;
}



/*************************************************************
 �� �� ����InitListCtrl()
 ���ܸ�Ҫ����ʼ���б�ؼ�,������ʼ���б���ʽ������б�����
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
int CManualFaultShorten::InitListCtrl()
{
	if(m_List.GetSafeHwnd() == NULL)
		return -1;
	//�����б���
	InitListStyle();
	/*
	//�����ݿ��ȡ����
	if(LoadDataFromDB(m_nMaxRow, m_bShowAcked))
	{
		//��ȡ���ݳɹ�,����б�
		FillListData();
	}
	else
	{
		//ʧ��
		WriteLog("�����¼���ͼ��ȡ����ʧ��", 1);
	}
	*/
	return 0;
}


/*************************************************************
 �� �� ����InitDeviceTree()
 ���ܸ�Ҫ�������豸��, ����豸��
 �� �� ֵ: ʧ�ܷ���-1, ��������>=0
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
	
	//���
	m_Tree.DeleteAllItems();
	
    POSITION pos;
    HTREEITEM hRoot;
	if (g_role != MODE_SUB)
	{
		//���ڵ�
		hRoot = m_Tree.InsertItem(STRING_NET, 0, 0, NULL);
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
		htRoot = m_Tree.InsertItem( StringFromID(IDS_MODEL_SUBSTATION), IMAGEINDEX_STATION, IMAGEINDEX_STATION, NULL);
	}
	//��վ
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj * pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj != NULL && !pObj->m_IsDistanceVirtual && pObj->m_nStationType != 113 && pObj->m_nStationType != 114 )
		{
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
	return TRUE;
}


/*************************************************************
 �� �� ����InsertNet()
 ���ܸ�Ҫ����������ڵ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ����ĵ�������
**************************************************************/
BOOL CManualFaultShorten::InsertNet(CNetObj* pObj)
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
	//m_Tree.SortChildren(htRoot);
	return TRUE;
}


/*************************************************************
 �� �� ����InsertStation()
 ���ܸ�Ҫ�����볧վ
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1  Ҫ����ĳ�վ����
**************************************************************/
BOOL CManualFaultShorten::InsertStation(CStationObj* pObj)
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
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htNet);
	} 
	else
	{
        HTREEITEM htRoot = m_Tree.GetRootItem();
        //���ڵ�
		htNew= m_Tree.InsertItem(pObj->m_sName, 0, 0, htRoot);
	}
        //��¼�ڵ�
		pObj->m_Tag = htNew;
		//�豸����
		m_Tree.SetItemData(htNew, (DWORD)pObj);
		//����
		m_Tree.SortChildren(htNet);
		
		//������
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
 �� �� ��: InsertStation_Voltage()
 ���ܸ�Ҫ: ���볧վ�ڵ㣬��վ�½ӵ�ѹ�ȼ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 Ҫ����ĳ�վ����
		   Param2 
**************************************************************/
BOOL CManualFaultShorten::InsertStation_Voltage( CStationObj* pObj )
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
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htNet);
	    //����
		m_Tree.SortChildren(htNet);
	}
	else
	{
		HTREEITEM htRoot = m_Tree.GetRootItem();
        //��վ�ڵ�
		htNew = m_Tree.InsertItem(pObj->m_sName, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, pObj->m_nCommStatus == 1 ? IMAGEINDEX_STATION : IMAGEINDEX_STATION_BREAK, htRoot);
	}

	//��¼�ڵ�
	pObj->m_Tag = htNew;
	//�豸����
	m_Tree.SetItemData(htNew, (DWORD)pObj);	

	if(g_DeviceTreeType == 4)
	{
		//�����豸ֱ�ӹ��ڳ�վ��
		BuildSecToStation(pObj);
	}
	else
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
		m_Tree.SortChildren(htWaveRec);
		
		
		HTREEITEM htGPSRec = m_Tree.InsertItem(StringFromID(IDS_MODEL_GPS), IMAGEINDEX_SWITCH, IMAGEINDEX_SWITCH, htNew);
		BuildGPS(pObj, htGPSRec);
		m_Tree.SortChildren(htGPSRec);
	}
		
		return TRUE;
}


/*************************************************************
 �� �� ��: InsertDevice()
 ���ܸ�Ҫ: �����豸���豸��
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ�����ڵ�
		   Param2 ָ���豸
**************************************************************/
BOOL CManualFaultShorten::InsertDevice( HTREEITEM htParent, CDeviceObj* pObj)
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
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, htParent);
	if(htNew == NULL)
		return FALSE;
	//��¼�ڵ�λ��
	pObj->m_Tag = htNew;
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	return TRUE;
}

/*************************************************************
 �� �� ��: ClearEmptyNodeType1()
 ���ܸ�Ҫ: ����豸������Ϊ1���豸���Ŀսڵ�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CManualFaultShorten::ClearEmptyNodeType1()
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
void CManualFaultShorten::ClearEmptyNodeType2()
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



/*************************************************************
 �� �� ��: SortVoltage()
 ���ܸ�Ҫ: ��վ�ڵ�ѹ�ȼ�����
 �� �� ֵ: 
 ��    ��: param1 ָ��վ
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
 �� �� ��: BuildBreak()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������п��ؼ��뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CManualFaultShorten::BuildBreak( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildBus()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ�������ĸ�߼��뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CManualFaultShorten::BuildBus( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID)
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildCap()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������е��������뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CManualFaultShorten::BuildCap( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildGen()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������з�������뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CManualFaultShorten::BuildGen( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildGen()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������з�������뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CManualFaultShorten::BuildLine( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildReactor()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������е翹�����뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CManualFaultShorten::BuildReactor( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildSwitch()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������е�բ���뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CManualFaultShorten::BuildSwitch( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildTrans()
 ���ܸ�Ҫ: ��ָ����վָ����ѹ�ȼ������б�ѹ�����뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 ָ�����ڵ�
		   Param3 ָ����ѹ�ȼ�
		   param4 ָ�����
**************************************************************/
void CManualFaultShorten::BuildTrans( CStationObj* pStation, HTREEITEM htParent, int nVoltage, CString sBayID )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
 �� �� ��: BuildProtect()
 ���ܸ�Ҫ: ��ָ����վ�����б������뵽�豸��
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
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
 �� �� ��: InsertProtect()
 ���ܸ�Ҫ: ���뱣�����豸��
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ������
		   Param2 
**************************************************************/
BOOL CManualFaultShorten::InsertProtect( CSecObj* pObj)
{
	//���������Ч��
	if(pObj == NULL)
		return FALSE;
	if(pObj->m_nType != TYPE_PROTECT)
		return FALSE;
	if(pObj->m_pOwner == NULL)
		return FALSE;
	if(pObj->m_pOwner->m_Tag == NULL)
		return FALSE;
	//���뵽�豸��
	CString str;
	str = pObj->m_sName;
	HTREEITEM htNew = m_Tree.InsertItem(str, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, pObj->m_pOwner->m_Tag);
	if(htNew == NULL)
		return FALSE;
	
	//��¼λ��
	pObj->m_Tag = htNew;
	m_Tree.SortChildren(pObj->m_pOwner->m_Tag);

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
void CManualFaultShorten::BuildWaveRec( CStationObj* pStation, HTREEITEM htParent )
{
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸��
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
	//���������Ч��
	if(pStation == NULL)
		return;
	if(htParent == NULL)
		return;
	//ѭ��,���뵽�豸
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
 �� �� ��: InsertFastness()
 ���ܸ�Ҫ: ���Ӳ�ڵ�
 �� �� ֵ: 
 ��    ��: param1 ָ�����ڵ�
		   Param2 ָ����վ
		   param3 ָ����ѹ�ȼ�
		   param4 ָ�����ID
**************************************************************/
void CManualFaultShorten::InsertFastness( HTREEITEM htParent, CStationObj* pObj, int nVoltage, CString sBayID )
{
	//�����һ���豸����
	HTREEITEM htBus		= m_Tree.InsertItem( StringFromID(IDS_MODEL_BUS), IMAGEINDEX_BUS, IMAGEINDEX_BUS, htParent);
	HTREEITEM htLine	= m_Tree.InsertItem( StringFromID(IDS_MODEL_LINE), IMAGEINDEX_LINE, IMAGEINDEX_LINE, htParent);
	HTREEITEM htGen		= m_Tree.InsertItem( StringFromID(IDS_MODEL_GEN), IMAGEINDEX_GEN, IMAGEINDEX_GEN, htParent);
	HTREEITEM htTrans	= m_Tree.InsertItem( StringFromID(IDS_MODEL_TRANS), IMAGEINDEX_TRANS, IMAGEINDEX_TRANS, htParent);
	HTREEITEM htCap		= m_Tree.InsertItem( StringFromID(IDS_MODEL_CAP), IMAGEINDEX_CAP, IMAGEINDEX_CAP, htParent);
	HTREEITEM htReactor = m_Tree.InsertItem( StringFromID(IDS_MODEL_REACTOR), IMAGEINDEX_REACTOR, IMAGEINDEX_REACTOR, htParent);
	HTREEITEM htSwitch	= m_Tree.InsertItem( StringFromID(IDS_MODEL_SWITCH), IMAGEINDEX_SWITCH, IMAGEINDEX_SWITCH, htParent);
	HTREEITEM htBreak	= m_Tree.InsertItem( StringFromID(IDS_MODEL_BREAKER), IMAGEINDEX_BREAK, IMAGEINDEX_BREAK, htParent);
	
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
void CManualFaultShorten::BuildBay( CStationObj* pStation, HTREEITEM htParent, int nVoltage )
{
	//���������Ч��
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
 �� �� ��: InsertBay()
 ���ܸ�Ҫ: ���������豸��
 �� �� ֵ: 
 ��    ��: param1 ָ�����ڵ�
		   Param2 ָ����վ
		   Param3 ָ�����
**************************************************************/
void CManualFaultShorten::InsertBay( HTREEITEM htParent, CStationObj* pObj, CBayObj* pBay)
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
	pBay->m_Tag = htNew;
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pBay);

	//����Ӳ�ڵ�
	InsertFastness(htNew, pObj, 0, pBay->m_sID);
}


/*************************************************************
 �� �� ����FindNetPos
 ���ܸ�Ҫ������ָ�����������е�λ��
 �� �� ֵ: ���ڵ�ṹ
 ��    ����param1   ָ������ID
**************************************************************/
HTREEITEM CManualFaultShorten::FindNetPos(CString sNetID)
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
 �� �� ��: InsertVoltage()
 ���ܸ�Ҫ: �����ѹ�ȼ��ڵ�,�ڵ��¸��豸
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ��: param1 ָ����ѹ�ȼ�
		   Param2 ָ�����ڵ�
**************************************************************/
BOOL CManualFaultShorten::InsertVoltage( int nVoltage, HTREEITEM htParent, CStationObj* pObj )
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
 �� �� ��: ClearCheck()
 ���ܸ�Ҫ: �������ѡ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CManualFaultShorten::ClearCheck(HTREEITEM ht)
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



/*************************************************************
 �� �� ����SetCheck_XJ()
 ���ܸ�Ҫ������ָ���ڵ��ѡ��״̬
 �� �� ֵ: void
 ��    ����param1	ָ���ڵ�
		   Param2	TRUE-ѡ��. FALSE-ȡ��ѡ��
**************************************************************/
//##ModelId=49B87BB100CD
void CManualFaultShorten::SetCheck_XJ( HTREEITEM hit, BOOL bCheck )
{
	CycTreeDown(hit, bCheck);
//	CycTreeUp(hit, bCheck);
}

/*************************************************************
 �� �� ����CycTreeDown()
 ���ܸ�Ҫ�����±���ָ���ڵ�������ӽڵ�, �������ӽڵ�����ͬ��ѡ�����
 �� �� ֵ: void
 ��    ����param1	ָ���ڵ�
		   Param2	ѡ���־.TRUEΪѡ��, FALSEΪȡ��ѡ��
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
 �� �� ����CycTreeUp()
 ���ܸ�Ҫ�����ϱ����ڵ�ĸ��ڵ�, �����ǰ�ڵ��Ǹ��ڵ�����һ����ѡ����ӽڵ�, Ϊ���ڵ�Ҳ����ѡ���־
 �� �� ֵ: void
 ��    ����param1	ָ���ڵ�
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
    UINT uFlag;   //�����йص�����Ե���Ϣ������
    HTREEITEM hTree;
    BOOL bCheck;
    GetCursorPos(&point);    //��ȡ��Ļ�������
    m_Tree.ScreenToClient(&point);           //ת���ɿͻ�����
    hTree = m_Tree.HitTest(point,&uFlag);    //������CtreeCtrl�����Ĺ��ĵ�ǰλ�ú;��
	
    if (hTree && (TVHT_ONITEMSTATEICON & uFlag))  //���и�ѡ��
	{
		bCheck = m_Tree.GetCheck(hTree);      //��ȡ��ǰ��ѡ״̬
		SetCheck_XJ(hTree, !bCheck);                //�������ѡ״̬
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
		AfxMessageBox("δ��ѯ�������¼�!");
	}
	*/

}



/*************************************************************
 �� �� ����QueryNewEventID()
 ���ܸ�Ҫ����ѯ���¼������ݿ��е�ID(��Ϊ���¼�����STTP���Ľ�������,��֪�������ݿ��е�ID)
 �� �� ֵ: ��ѯ�ɹ�����TRUE,ʧ�ܷ���FALSE(�н���ҽ���һ�������Ϊ��ѯ�ɹ�)
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
	
	//�������µ�num����¼
	//�齨��ѯ����
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
	////����PT_ID,curtime,ms,recvtime,ms2����id
	
	//����
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

	//��ʼʱ��
	Condition con3;
	str.Format("curtime > '%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con3, str);
	//����ʱ��
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
		WriteLog("CManualFaultShorten::QueryActionEvent, ��ѯʧ��", XJ_LOG_LV3);
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
			str.Format("CManualFaultShorten::QueryActionEvent, ��ȡ��%d������", nCount);
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
			//�����¼�
			CXJEventAction * pEvent = new CXJEventAction;
			if(pEvent->LoadFromDB(pMemset))
			{
				//���뵽����
				m_EventManager.AddEvent(pEvent);
				//�����¼�
				pEvent->TranslateFData();
				//�ֽ��¼�
				BreakAction(pEvent);
			}
			else
			{
				//��ȡ����ʧ��, delete
				delete pEvent;
				pEvent = NULL;
			}
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CManualFaultShorten::QueryActionEvent, ��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����BreakAction()
 ���ܸ�Ҫ���ֽ⶯���¼�������ֵ���ź���Ϊ���������¼�,�ֱ𱣴�
 �� �� ֵ: void
 ��    ����param1	ָ�������¼�
		   Param2
**************************************************************/
//##ModelId=49B87BA401A6
void CManualFaultShorten::BreakAction( CXJEventAction* pEvent )
{
	//���������Ч��
	if(pEvent == NULL)
		return;
	//����ֵ
	for(int i = 0; i < pEvent->m_ActChrs.GetSize(); i++)
	{
		PT_ActionCharacter* pAC = (PT_ActionCharacter*)pEvent->m_ActChrs.GetAt(i);
		if(pAC == NULL)
		{
			continue;
		}
		//���������¼�
		CFaultEvent* pFaultEvent = new CFaultEvent;
		pFaultEvent->m_nType = 1; //��������ֵ
		pFaultEvent->m_nMainRecID = m_nMainRecID;;//���ϰ�ID
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
		//���뵽����
		m_listChr.Add(pFaultEvent);
	}
	//�ź���
	for(i = 0; i < pEvent->m_Signals.GetSize(); i++)
	{
		PT_Signal * pSgn = (PT_Signal*)pEvent->m_Signals.GetAt(i);
		if(pSgn == NULL)
			continue;
		//���������¼�
		CFaultEvent* pFaultEvent = new CFaultEvent;
		pFaultEvent->m_nType = 2; //�����ź�
		pFaultEvent->m_nMainRecID = m_nMainRecID;;//���ϰ�ID
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
		//���뵽����
		m_listSign.Add(pFaultEvent);
	}
}


void CManualFaultShorten::OnFaultCreate() 
{
	// TODO: �½����ϰ�
	//1.��ѯ���ݿ������µİ�ID
	m_nMainRecID = QueryLatsetID();
	if(m_nMainRecID < 0)
	{
		WriteLog("δ��ѯ���Ϸ���ID", XJ_LOG_LV3);
		//return;
	}
	//2.�½���
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
	//4.���浽���ݿ�
	if(!AddReportToDB(pReport))
	{
		//����ʧ��
		AfxMessageBox(StringFromID(IDS_PACK_SAVEPACKAGE_FAIL));
		delete pReport;
		ClearEvent();
		return;
	}


	if (MultiSaveEvent() != 0)
	{
		//����ʧ��
		AfxMessageBox( StringFromID(IDS_PACK_SAVEEVENT_FAIL));
		delete pReport;
		ClearEvent();
		return;
	}
	
	//3.��ӵ������LIST��ʾ
	pView->m_listFaultReport.AddTail(pReport);
	pView->AddReportToList(pReport, 0); //��һ��*/
	FillListData();
	AfxMessageBox( StringFromID(IDS_PACK_SUCCESS));
	pView->OnFaultRefresh() ;
}


/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 �� �� ��: QueryLatsetID()
 ���ܸ�Ҫ: ��ѯ���µĹ��ϰ�ID
 �� �� ֵ: ���µĹ��ϰ�ID
 ��    ��: param1 
		   Param2 
**************************************************************/
int CManualFaultShorten::QueryLatsetID()
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	int nReturn = -1;
	
	//��ѯ�����1����¼
	CString str;		
	
	//top 1
	Field Field1;
	str.Format("%d", 1);
	pApp->m_DBEngine.SetField(sql, Field1, str, EX_STTP_DATA_TYPE_TOP);
	//����
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
		WriteLog("CManualFaultShorten::QueryLatsetID, ��ѯʧ��", XJ_LOG_LV3);
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
			//ֻӦ�ò�ѯ��һ��
			CString strID = pMemset->GetValue(UINT(0));
			nReturn = atoi(strID);
		}
	}
	else
	{
		CString str;
		str.Format("CManualFaultShorten::QueryLatsetID,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ��: CreateFaultReport()
 ���ܸ�Ҫ: �½����ϰ�
 �� �� ֵ: �´����Ĺ��ϰ�
 ��    ��: param1 ��ID
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
 �� �� ����SaveEvent()
 ���ܸ�Ҫ�����汻ѡ����¼������ϰ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA401D9
void CManualFaultShorten::SaveEvent(CFaultReport* pReport)
{
	//���������Ч��

	MYASSERT(pReport);
	//��������ֵ
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		//�õ��¼�
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//��ѡ��ļ��뵽�����ڵĶ�Ӧ������
		CFaultEvent* pNew = pEvent->Clone();
		pReport->m_listChr.Add(pNew);
	}
	//�����ź�
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		//�õ��¼�
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//��ѡ��ļ��뵽�����ڵĶ�Ӧ������
		CFaultEvent* pNew = pEvent->Clone();
		pReport->m_listSign.Add(pNew);
	}
	/*
	//�澯
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		//�õ��¼�
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//��ѡ��ļ��뵽�����ڵĶ�Ӧ������
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			pParent->m_listAlarm.Add(pNew);
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//¼��
	for(i = 0; i < m_listOSC.GetSize(); i++)
	{
		//�õ��¼�
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//��ѡ��ļ��뵽�����ڵĶ�Ӧ������
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			if(pNew->m_pSec == NULL)
				continue;
			if(pNew->m_pSec->m_nType == TYPE_PROTECT)
			{
				//����
				pParent->m_listPTOSC.Add(pNew);
			}
			else if(pNew->m_pSec->m_nType == TYPE_WAVEREC)
			{
				//¼����
				pParent->m_listWROSC.Add(pNew);
			}
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//ˢ�¸������б��
	pParent->UpdateList();
	*/
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 �� �� ��: AddReportToDB()
 ���ܸ�Ҫ: �����ϰ����뵽���ݿ�,�¼�������
 �� �� ֵ: ����ɹ�����TRUE,ʧ�ܷ���FALSE
 ��    ��: param1 ָ�����ϰ�
		   Param2 
**************************************************************/
BOOL CManualFaultShorten::AddReportToDB( CFaultReport* pReport )
{
	BOOL bReturn = TRUE;

	if(pReport == NULL)
		return FALSE;
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��ѯ�����nNum����¼
	CString str;		
	
	//top
	//�ֶ�
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
		WriteLog("CManualFaultShorten::AddReportToDB, ʧ��", XJ_LOG_LV3);
		delete[] sError;
		return FALSE;
	}
	
	if(nResult != 1)
	{
		CString str;
		str.Format("CManualFaultShorten::AddReportToDB,ʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;
}


/*************************************************************
 CreateDate: 13:1:2010     Author:LYH
 �� �� ��: MultiSaveEvent()
 ���ܸ�Ҫ: ���������¼�
 �� �� ֵ: ����ɹ�0,ɾ������ʧ�ܷ���1,��������ʧ�ܷ���2
 ��    ��: param1 
		   Param2 
**************************************************************/
int CManualFaultShorten::MultiSaveEvent()
{
	/*
	if(!DeleteAllEventFromDB())
	{
		//ɾ��ʧ��
		WriteLog("CManualFaultShorten::MultiSaveEvent, ɾ��������ʧ��");
		return 1;
	}
	*/
	if(m_listChr.GetSize() == 0 && m_listSign.GetSize() == 0)
		return 0;
	//ѭ������ѡ����
	STTP_FULL_DATA sttpData;
	//��սṹ
	zero_sttp_full_data(sttpData);
	int i = 0;
	//��������ֵ
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//�����ź���
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return FALSE;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	/*
	//�澯�ź���
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//����¼��
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//¼����¼��
	for(i = 0; i < m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWROSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
		if(!pEvent->MultiSaveData(sttpDB))
			return 2;
		sttpData.sttp_body.variant_member.dataflat_data.record_list.push_back(sttpDB);
	}
	//¼���б�
	for(i = 0; i < m_listOSCList.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCList.GetAt(i);
		if(pEvent == NULL)
			continue;
		STTP_DBDATA_RECORD sttpDB;
		sttpDB.condition_list.clear();
		sttpDB.field_list.clear();
		//����
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
	
	//����ID
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
		WriteLog("CManualFaultShorten::MultiSaveEvent, ����ʧ��", XJ_LOG_LV3);
		delete[] sError;
		return 2;
	}
	
	if(nResult == 1)
	{
		//�ɹ�
		nReturn = 0;
	}
	else
		nReturn = 2;

	zero_sttp_full_data(sttpData);
	
	delete[] sError;
	if(nReturn == 0) //�ɹ�
	{
		WriteLog("CManualFaultShorten::MultiSaveEvent, ����ɹ�", XJ_LOG_LV3);
	}
	else
	{
		WriteLog("CManualFaultShorten::MultiSaveEvent, ����ʧ��", XJ_LOG_LV3);
	}
	return nReturn;	
}

/*************************************************************
 �� �� ����ClearEvent()
 ���ܸ�Ҫ��ɾ�������¼�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40196
void CManualFaultShorten::ClearEvent()
{
	//��ո�����
	//����������
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent != NULL)
			delete pEvent;
	}
	m_listChr.RemoveAll();
	//�����ź���
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
 �� �� ����FillListData()
 ���ܸ�Ҫ������б�����
 �� �� ֵ: ʧ�ܷ���-1, �ɹ�����>=0
**************************************************************/
int CManualFaultShorten::FillListData()
{
	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
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

	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
	return 0;
}


/*************************************************************
 �� �� ����AddEventToList()
 ���ܸ�Ҫ�������¼���Ϣ���б�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1   Ҫ������¼�ָ��
           Param2   Ҫ�����λ��,��0��ʼ����
**************************************************************/
BOOL CManualFaultShorten::AddEventToList(CXJEventAction * pEvent, int i)
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;

	//0:���
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	//1:���
	if(pEvent->m_IsDebug == XJ_EVENTINFO_DEBUG)
	{
		//����
		m_List.SetItemText(i, 1, StringFromID(IDS_RUNSTATUS_TESTING));
	}
	else
	{
		//����
		m_List.SetItemText(i, 1, pEvent->GetAlarmType());
	}
	//2.��������
	m_List.SetItemText(i, 2, pEvent->GetFaultType());
	//3:��������
	CString str;
	str = pEvent->m_pSec->m_sName;
	m_List.SetItemText(i, 3, str);
	//4:װ������ʱ��
	CString sTime;
	sTime.Format("%s.%03d", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms);
	m_List.SetItemText(i, 4, sTime);
	//5:��Ϣ����ʱ��
	sTime.Format("%s.%03d", pEvent->m_tmTime2.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nms2);
	m_List.SetItemText(i, 5, sTime);
	//6:��Ϣ����
	m_List.SetItemText(i, 6, pEvent->m_sMsg);
	//7:��վ��
	m_List.SetItemText(i, 7, pEvent->m_sStationName);
	//8:һ���豸��
	m_List.SetItemText(i, 8, pEvent->m_sDeviceName);
	//9:��ȷ��
	m_List.SetItemText(i, 9, (pEvent->m_nIsAck==1)? StringFromID(IDS_ACK_YES):"");
	//��������s
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
	//���������Ч��
	if(pObj == NULL)
		return FALSE;
	if(pObj->m_pStation == NULL)
		return FALSE;
	if(pObj->m_pStation->m_Tag == NULL)
		return FALSE;
	//���뵽�豸��
	CString str;
	str = pObj->m_sName;
	HTREEITEM htNew = m_Tree.InsertItem(str, IMAGEINDEX_PROTECT1, IMAGEINDEX_PROTECT1, pObj->m_pStation->m_Tag);
	if(htNew == NULL)
		return FALSE;
	
	//��¼λ��
	pObj->m_Tag = htNew;
	m_Tree.SortChildren(pObj->m_pStation->m_Tag);
	
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pObj);	
	
	return TRUE;
}
