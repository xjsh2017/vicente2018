
// DlgFaultReport.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgFaultReport.h"
#include "DlgFaultEventAdd.h"
#include "DlgAddOsc.h"
#include "ViewFault.h"
#include "ViewFaultQuery.h"
#include "GlobalFunc.h"
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFaultReport dialog

extern int g_iStationInfoViewAsc;
//##ModelId=49B87BA40030
CDlgFaultReport::CDlgFaultReport(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFaultReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFaultReport)
	m_sName = _T("");
	m_sDistance = _T("");
	m_sMs = _T("");
	//}}AFX_DATA_INIT
	m_pFaultReport = NULL;
	m_listChr.RemoveAll();
	m_listSign.RemoveAll();
	m_listAlarm.RemoveAll();
	m_listPTOSC.RemoveAll();
	m_listWROSC.RemoveAll();
	m_listDIChange.RemoveAll();
	m_listDistance.RemoveAll();
	m_listOSCParse.RemoveAll();
	m_bEventNeedSave = FALSE;
	m_nOldSortCol = -1;
	m_pWnd = NULL;
    m_nCurTabSel = 0;
	m_sFaultReason = _T("");
	m_sMs = _T("");
	m_sTitle = _T("");
	g_iStationInfoViewAsc = -1;
}


//##ModelId=49B87BA400DC
void CDlgFaultReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFaultReport)
	DDX_Control(pDX, IDC_DLG_FAULT_CMB_STATION2, m_cmbStation2);
	DDX_Control(pDX, IDC_DLG_FAULT_CMB_STATION1, m_cmbStation1);
	DDX_Control(pDX, IDC_DLG_FAULT_CMB_DEVICE, m_cmbDevice);
	DDX_Control(pDX, IDC_EDIT_FAULT_REASON, m_FaultReasonEdit);
	DDX_Control(pDX, IDC_DLG_FAULT_TAB, m_Tab);
	DDX_Control(pDX, IDC_DLG_FAULT_LIST, m_List);
	DDX_Text(pDX, IDC_DLG_FAULT_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_DLG_FAULT_EDIT_DIS, m_sDistance);
	DDX_Text(pDX, IDC_EDIT_ADDOSC_MS, m_sMs);
	DDV_MaxChars(pDX, m_sMs, 4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFaultReport, CDialog)
	//{{AFX_MSG_MAP(CDlgFaultReport)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DLG_FAULT_TAB, OnSelchangeDlgFaultTab)
	ON_BN_CLICKED(IDC_DLG_FAULT_REMOVE, OnDlgFaultRemove)
	ON_BN_CLICKED(IDC_DLG_FAULT_ADD, OnDlgFaultAdd)
	ON_BN_CLICKED(IDC_DLG_FAULT_ADDOSC, OnDlgFaultAddosc)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_DLG_FAULT_LIST, OnColumnclickDlgFaultList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFaultReport message handlers

//##ModelId=49B87BA400EA
BOOL CDlgFaultReport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_pFaultReport == NULL)
		return FALSE;
	//��ʼ������
	//1.������Ϣ������:
	m_sName = m_pFaultReport->m_sName;
	//2.���ϲ��
	m_sDistance = m_pFaultReport->m_sFaultDistance;
	//3.����ʱ��
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULT_TIME);
	if(pTime != NULL)
	{
		//������ʾ���ͷ�Χ
		pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
		//���÷�Χ
		CTime tmMin(1971, 1, 1, 1, 1, 1);
		CTime tmMax(2036, 12, 12, 12, 12, 12);
		pTime->SetRange(&tmMin, &tmMax);
		//����ʱ��
		CTime tm = m_pFaultReport->m_tmFaultTime;
		VERIFY(pTime->SetTime(&tm));
	}
	//4.����ֵ
	m_sMs.Format("%d", m_pFaultReport->m_nmsFaultTime);
	//5.��������
	InitCMBFaultType();
	//6.��վ1
	InitCMBStation(m_pFaultReport->m_sStation1ID, IDC_DLG_FAULT_CMB_STATION1);
	//7.��վ2
	InitCMBStation(m_pFaultReport->m_sStation2ID, IDC_DLG_FAULT_CMB_STATION2);
	//8.����Ԫ��
	InitCMBDevice();
	//9.�Ƿ�浵
    InitCMBIsSaved();

	//3.����ʱ��
	pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULT_CREATE_TIME);
	if(pTime != NULL)
	{
		//������ʾ���ͷ�Χ
		pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
		//���÷�Χ
		CTime tmMin(1971, 1, 1, 1, 1, 1);
		CTime tmMax(2036, 12, 12, 12, 12, 12);
		pTime->SetRange(&tmMin, &tmMax);
		//����ʱ��
		CTime tm = m_pFaultReport->m_tmCreateTime;
		VERIFY(pTime->SetTime(&tm));
	}

	//10.TAB��
	TC_ITEM TabCtrlItem;
	TabCtrlItem.mask = TCIF_TEXT;
	TabCtrlItem.pszText = StringFromID(IDS_FAULTEVENT_TYPE_CHAR).GetBuffer(1);
	m_Tab.InsertItem( 0, &TabCtrlItem );
	TabCtrlItem.pszText = StringFromID(IDS_FAULTEVENT_TYPE_ACTIONSIGNAL).GetBuffer(1);
	m_Tab.InsertItem( 1, &TabCtrlItem );
	TabCtrlItem.pszText = StringFromID(IDS_FAULTEVENT_TYPE_ALARMSIGNAL).GetBuffer(1);
	m_Tab.InsertItem( 2, &TabCtrlItem );
	TabCtrlItem.pszText = StringFromID(IDS_FAULTEVENT_TYPE_OSC).GetBuffer(1);
	m_Tab.InsertItem( 3, &TabCtrlItem );
	TabCtrlItem.pszText = StringFromID(IDS_FAULTEVENT_TYPE_DICHANGE).GetBuffer(1);
	m_Tab.InsertItem( 4, &TabCtrlItem );
	TabCtrlItem.pszText = StringFromID(IDS_FAULT_REASON).GetBuffer(1);
	m_Tab.InsertItem( 5, &TabCtrlItem );
	//11.list
	InitListStyle();
	//12
	if(!m_pFaultReport->m_bLoadDetail)
	{
		//��δ������ϸ��Ϣ, ����������ϸ��Ϣ
		m_pFaultReport->LoadDetail();
	}
	//13
	ReadDetail();
	//14.����б��
	FillListData(0);
	m_FaultReasonEdit.SetWindowText(m_sFaultReason);
	
	if (!m_nCurTabSel)
	{
		m_List.ShowWindow(SW_SHOW);
		m_FaultReasonEdit.ShowWindow(SW_HIDE);
	}
	else
	{
		SelCurTab(m_nCurTabSel);
	}
	//GetDlgItem(IDC_STATIC_FAULT_SAVE)->SetWindowText(m_sTitle + ":");
	//ˢ����ʾ
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 �� �� ����InitCMBFaultType()
 ���ܸ�Ҫ����ʼ����������������
 �� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA4007E
BOOL CDlgFaultReport::InitCMBFaultType()
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULT_CMB_TYPE);
	if(pCmb == NULL)
		return FALSE;
	if(m_pFaultReport == NULL)
		return FALSE;
	//�����������
	pCmb->ResetContent();
	//�������
	// 		0	δ֪
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_UNKNOWN));
	// 		1 A��ӵ�
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_A));
	// 		2 B��ӵ�
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_B));
	// 		3 C��ӵ�
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_C));
	// 		4 AB�����·
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_AB));
	// 		5 BC�����·
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_BC));
	// 		6 CA�����·
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_CA));
	// 		7 AB�����·�ӵ�
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_ABN));
	// 		8 BC�����·�ӵ�
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_BCN));
	// 		9 CA�����·�ӵ�
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_CAN));
	// 		10 ABC�����·
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_ABC));
	// 		11 ABC�����·�ӵ�
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_ABCN));
	//����"����"ѡ��
	pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	//����ѡ��
	int nCount = pCmb->GetCount();
	if(m_pFaultReport->m_nFaultType < nCount)
		pCmb->SetCurSel(m_pFaultReport->m_nFaultType);
	else
		pCmb->SetCurSel(0);
	return TRUE;
}

/*************************************************************
 �� �� ����InitCMBStation()
 ���ܸ�Ҫ����ʼ����վ������
 �� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	Ԥѡ��ĳ�վID
		   Param2	������ID
**************************************************************/
//##ModelId=49B87BA4007F
BOOL CDlgFaultReport::InitCMBStation( CString sID, UINT nID)
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(nID);
	if(pCmb == NULL)
		return FALSE;
	if(m_pFaultReport == NULL)
		return FALSE;
	//�����������
	pCmb->ResetContent();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;

	//���г�վ
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();

	//���볧վ
	int nIndex = -1; //Ԥѡ���λ��
	while(pos != NULL)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if(pStation == NULL)
			continue;
		//���뵽������
		int nPos = pCmb->AddString(pStation->m_sName);
		if(nPos < 0)
			continue;
		//��������
		pCmb->SetItemData(nPos, (DWORD)pStation);
		//
		if(pStation->m_sID == sID)
			nIndex = nPos;
	}
	//����ճ�վ
	int nNullPos = pCmb->AddString( StringFromID(IDS_COMMON_UNSPECIFIED));
	//ѡ��
	if(nIndex == -1)
		nIndex = nNullPos;
	pCmb->SetCurSel(nIndex);
	if( nID==IDC_DLG_FAULT_CMB_STATION1 )
		m_cmbStation1.OnDataFilled(TRUE, nIndex);
	else if( nID==IDC_DLG_FAULT_CMB_STATION2)
		m_cmbStation2.OnDataFilled(TRUE, nIndex);
	return FALSE;
}

/*************************************************************
 �� �� ����InitCMBDevice()
 ���ܸ�Ҫ����ʼ��һ���豸������, 
 �� �� ֵ: ��ʼ���ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA4008E
BOOL CDlgFaultReport::InitCMBDevice()
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULT_CMB_DEVICE);
	if(pCmb == NULL)
		return FALSE;
	if(m_pFaultReport == NULL)
		return FALSE;
	//�����������
	pCmb->ResetContent();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;

	//���ҳ�վ1�Ķ���ָ��
	CStationObj* pStation = (CStationObj*)m_pFaultReport->m_pStation1;
	if(pStation == NULL)
	{
		pCmb->ShowWindow(TRUE);
		pCmb->ShowWindow(FALSE);
		(CStatic*)GetDlgItem(IDC_STATIC_FAULTPART)->ShowWindow(FALSE);
		return FALSE;
	}
	else
	{
		pCmb->ShowWindow(TRUE);
		(CStatic*)GetDlgItem(IDC_STATIC_FAULTPART)->ShowWindow(TRUE);
	}
	//ѭ����������һ���豸
	//��·
	FillSpecificPriDevice(pCmb, pStation, TYPE_LINE);
	//ĸ��
	FillSpecificPriDevice(pCmb, pStation, TYPE_BUS);
	//��ѹ��
	FillSpecificPriDevice(pCmb, pStation, TYPE_TRANS);
	//������
	FillSpecificPriDevice(pCmb, pStation, TYPE_CAP);
	//�翹��
	FillSpecificPriDevice(pCmb, pStation, TYPE_REACTOR);
	//�����
	FillSpecificPriDevice(pCmb, pStation, TYPE_GEN);
	//����
	FillSpecificPriDevice(pCmb, pStation, TYPE_BREAK);
	//��բ
	FillSpecificPriDevice(pCmb, pStation, TYPE_SWITCH);

	CDeviceObj* pDevice = m_pFaultReport->m_pPriDevice;
	if(pDevice == NULL)
		return FALSE;

	//����Ԥѡ���һ���豸
	int nIndex = 0;
	int nCount = pCmb->GetCount();
	for(int i = 0; i < nCount; i++)
	{
		//�Ƚ�
		CDeviceObj* pObj = (CDeviceObj*)pCmb->GetItemData(i);
		if(pObj == NULL)
			continue;
		if(pObj->m_sID == pDevice->m_sID)
		{
			nIndex = i;
			break;
		}
	}
	pCmb->SetCurSel(nIndex);
	m_cmbDevice.OnDataFilled(TRUE, nIndex);
	return TRUE;
}

BOOL CDlgFaultReport::InitCMBIsSaved()
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_CMB_FAULT_SAVE);
	if(pCmb == NULL)
		return FALSE;
	if(m_pFaultReport == NULL)
		return FALSE;
	//�����������
	pCmb->ResetContent();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
    pCmb->AddString( StringFromID(IDS_COMMON_YES));
	pCmb->AddString( StringFromID(IDS_COMMON_NOT));
	if (m_pFaultReport->m_bIsSaved)
	{
		pCmb->SetCurSel(0);
	}
	else
	{
		pCmb->SetCurSel(1);
	}
	return FALSE;
}


/*************************************************************
 �� �� ����FillSpecificPriDevice()
 ���ܸ�Ҫ����ָ����վ�µ�ָ�����͵�һ���豸��䵽ָ�������� 
 �� �� ֵ: void
 ��    ����param1	������ָ��
		   Param2	��վָ��
		   param3	һ���豸����
**************************************************************/
//##ModelId=49B87BA4009C
void CDlgFaultReport::FillSpecificPriDevice( CComboBox* pCmb, CStationObj* pStation, int nType)
{
	//���������Ч��
	if(pCmb == NULL)
		return;
	if(pStation == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return ;
	
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, nType, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//���뵽������
		int nIndex = pCmb->AddString(pObj->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pObj);
		}
	}
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400AB
void CDlgFaultReport::InitListStyle()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
		
	//�����п�,��С����
	for (int nCol=0; nCol < 5 ; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0://
			lvCol.cx = 300; // �����豸
			colName = StringFromID(IDS_MODEL_SECONDARY);
			break;
		case 1://
			lvCol.cx = 160; // ����ʱ��
			colName = StringFromID(IDS_EVENT_STARTTIME);
			break;
		case 2://
			lvCol.cx = 180; // �¼�����
			colName = StringFromID(IDS_EVENT_DESC);
			break;
		case 3:
			lvCol.cx = 80; //�¼�ֵ
			colName = StringFromID(IDS_EVENT_VALUE);
			break;
		case 4:
			lvCol.cx = 160; //����ʱ��
			colName = StringFromID(IDS_EVENT_RECIVETIME);
			break;
		default:
			lvCol.cx = 100;
			break;
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_List.InsertColumn(nCol,&lvCol);
	}

	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
}

/*************************************************************
 �� �� ����ClearDetail()
 ���ܸ�Ҫ�������ϸ��Ϣ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400AC
void CDlgFaultReport::ClearDetail()
{
	//��ո�����
	//����������
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		delete pEvent;
	}
	m_listChr.RemoveAll();
	//�����ź���
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		delete pEvent;
	}
	m_listSign.RemoveAll();
	//�澯�¼�
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		delete pEvent;
	}
	m_listAlarm.RemoveAll();
	//����¼��
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		delete pEvent;
	}
	m_listPTOSC.RemoveAll();
	//¼����¼��
	for(i = 0; i < m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWROSC.GetAt(i);
		delete pEvent;
	}
	m_listWROSC.RemoveAll();
	//��������λ
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		delete pEvent;
	}
	m_listDIChange.RemoveAll();
	//С�����
	for(i = 0; i < m_listDistance.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDistance.GetAt(i);
		delete pEvent;
	}
	m_listDistance.RemoveAll();
	//¼������
	for(i = 0; i < m_listOSCParse.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCParse.GetAt(i);
		delete pEvent;
	}
	m_listOSCParse.RemoveAll();
}

//##ModelId=49B87BA4003E
CDlgFaultReport::~CDlgFaultReport()
{
	ClearDetail();
}

/*************************************************************
 �� �� ����ReadDetail()
 ���ܸ�Ҫ��������ϸ��Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400AD
void CDlgFaultReport::ReadDetail()
{
	//����������
	int i = 0;
	m_listChr.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listChr.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listChr.Add(pNew);
	}
	
	//�����ź���
	m_listSign.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listSign.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listSign.Add(pNew);
	}

	//�澯�¼�
	m_listAlarm.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listAlarm.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listAlarm.Add(pNew);
	}
	
	//����¼��
	m_listPTOSC.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listPTOSC.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listPTOSC.Add(pNew);
	}
	
	//¼����¼��
	m_listWROSC.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listWROSC.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listWROSC.Add(pNew);
	}

	//��������λ
	m_listDIChange.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listDIChange.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listDIChange.Add(pNew);
	}

	//С�����
	m_listDistance.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listWaveDistance.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listWaveDistance.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listDistance.Add(pNew);
	}

	//С�����
	m_listOSCParse.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listOSCParse.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listOSCParse.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listOSCParse.Add(pNew);
	}

	m_sFaultReason = m_pFaultReport->m_sFaultReason;
	
}

/*************************************************************
 �� �� ����FillListData()
 ���ܸ�Ҫ�����ֵ���б��
 �� �� ֵ: 
 ��    ����param1	Ҫ����������.0-��������ֵ.1-������Ϣ��.2-�澯.3-¼����
		   Param2
**************************************************************/
//##ModelId=49B87BA400AE
void CDlgFaultReport::FillListData( int nType )
{
	//���ԭ����ֵ
	m_List.DeleteAllItems();
	//��������ֵ
	if(0 == nType)
	{
		m_List.DeleteColumn(5);
		for(int i = 0; i < m_listChr.GetSize(); i++)
		{
			CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
			MYASSERT_CONTINUE(pEvent);
			AddEventToList(pEvent);
		}
	}
	//�����ź�ֵ
	if(1 == nType)
	{
		m_List.DeleteColumn(5);
		LV_COLUMN lvCol;
		lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvCol.fmt=LVCFMT_CENTER;
		lvCol.iSubItem = 5;
		lvCol.cx = 160;
		lvCol.pszText = StringFromID(IDS_EVENT_SIGNALTIME).GetBuffer(1);
		m_List.InsertColumn(5, &lvCol);
		for(int i = 0; i < m_listSign.GetSize(); i++)
		{
			CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
			MYASSERT_CONTINUE(pEvent);
			AddEventToList(pEvent);
		}
	}
	//�澯�ź�ֵ
	if(2 == nType)
	{
		m_List.DeleteColumn(5);
		for(int i = 0; i < m_listAlarm.GetSize(); i++)
		{
			CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
			MYASSERT_CONTINUE(pEvent);
			AddEventToList(pEvent);
		}
	}
	//¼����
	if(3 == nType)
	{
		m_List.DeleteColumn(5);
		for(int i = 0; i < m_listPTOSC.GetSize(); i++)
		{
			CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
			MYASSERT_CONTINUE(pEvent);
			AddEventToList(pEvent);
		}
		for(i = 0; i < m_listWROSC.GetSize(); i++)
		{
			CFaultEvent* pEvent = (CFaultEvent*)m_listWROSC.GetAt(i);
			MYASSERT_CONTINUE(pEvent);
			AddEventToList(pEvent);
		}
	}
	//��������λ
	if(4 == nType)
	{
		m_List.DeleteColumn(5);
		for(int i = 0; i < m_listDIChange.GetSize(); i++)
		{
			CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
			MYASSERT_CONTINUE(pEvent);
			AddEventToList(pEvent);
		}
	}
}

/*************************************************************
 �� �� ����AddEventToList()
 ���ܸ�Ҫ�������¼����б�
 �� �� ֵ: 
 ��    ����param1	�¼�ָ��
		   Param2	Ҫ�����λ������
**************************************************************/
//##ModelId=49B87BA400BC
void CDlgFaultReport::AddEventToList( CFaultEvent* pEvent, int nIndex /*= 0*/ )
{
	//���������Ч��
	MYASSERT(pEvent);
	if(nIndex < 0)
		return;
	//"�����豸","����ʱ��","�¼�����", "�¼�ֵ", "����ʱ��"
	CString str = "";
	if(pEvent->m_pSec != NULL)
		str = pEvent->m_pSec->m_sName;
	//0:�����豸
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1:����ʱ��
	str.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
	m_List.SetItemText(nIndex, 1, str);
	//2:�¼�����
	if(pEvent->m_nType == 4)
		m_List.SetItemText(nIndex, 2, pEvent->m_sEventDef);
	else
		m_List.SetItemText(nIndex, 2, pEvent->m_sEventDefName);
	//3:�¼�ֵ
	str = pEvent->m_sEventContent;
	if(pEvent->m_nType == XJ_FAULT_EVENT_SING || pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
	{
		//�ź���
		int nContent = atoi(str);
		if(nContent == 1)
			str = StringFromID(IDS_CASE_ACTION);
		else if(nContent == 0)
			str = StringFromID(IDS_CASE_RETURN);
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
	{
		//����ֵ
		if(pEvent->m_strUnit != "" && pEvent->m_sEventContent != "")
			str.Format("%s(%s)", pEvent->m_sEventContent, pEvent->m_strUnit);
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_DICHANGE)
	{
		//�ź���
		int nContent = atoi(str);
		if(nContent == 1)
			str = "ON";
		else if(nContent == 0)
			str = "OFF";
	}
	m_List.SetItemText(nIndex, 3, str);
	//4:����ʱ��
	str.Format("%s.%03d", pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsReceive);
	m_List.SetItemText(nIndex, 4, str);
	//5:�ź�������ʱ��
	str.Format("%s.%03d", pEvent->m_tmSign.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsSign);
	m_List.SetItemText(nIndex, 5, str);

	//�����豸
	m_List.SetItemData(nIndex, (DWORD)pEvent);
}

//##ModelId=49B87BA400EC
void CDlgFaultReport::OnSelchangeDlgFaultTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_Tab.GetCurSel();
	if (nIndex != 5)
	{
		m_List.ShowWindow(SW_SHOW);
		m_FaultReasonEdit.ShowWindow(SW_HIDE);
	}
	else
	{
        m_List.ShowWindow(SW_HIDE);
		m_FaultReasonEdit.ShowWindow(SW_SHOW);
	}
	FillListData(nIndex);
	*pResult = 0;
}

/*************************************************************
 �� �� ����NeedSaveReport()
 ���ܸ�Ҫ���жϱ�������Ϣ�������Ƿ���Ҫ����
 �� �� ֵ: ��Ҫ���淵��TRUE, ����Ҫ���淵��FALSE
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400CB
BOOL CDlgFaultReport::NeedSaveReport()
{
	BOOL bResult = FALSE;
	//�õ���������ֵ
	UpdateData(TRUE);
	//�ж���Ϣ����
	if(m_sName != m_pFaultReport->m_sName)
	{
		//�ı�ֵ
		m_pFaultReport->m_sName = m_sName;
		bResult = TRUE;
	}
	//����ʱ��
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULT_TIME);
	if(pTime != NULL)
	{
		CTime tmNew(1972, 1, 1, 1, 1, 1);
		pTime->GetTime(tmNew);
		if(tmNew != m_pFaultReport->m_tmFaultTime)
		{
			m_pFaultReport->m_tmFaultTime = tmNew;
			bResult = TRUE;
		}
	}

	pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULT_CREATE_TIME);
	if(pTime != NULL)
	{
		CTime tmNew(1972, 1, 1, 1, 1, 1);
		pTime->GetTime(tmNew);
		if(tmNew != m_pFaultReport->m_tmCreateTime)
		{
			m_pFaultReport->m_tmCreateTime = tmNew;
			bResult = TRUE;
		}
	}


	//����ֵ
	if (atoi(m_sMs) != m_pFaultReport->m_nmsFaultTime)
	{
		m_pFaultReport->m_nmsFaultTime = atoi(m_sMs);
		bResult = TRUE;
	}
	//����Ԫ��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULT_CMB_DEVICE);
	if(pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		if(nIndex >= 0)
		{
			CDeviceObj* pDevice = (CDeviceObj*)pCmb->GetItemData(nIndex);
			if(pDevice != NULL)
			{
				if(pDevice->m_sID != m_pFaultReport->m_sPriDeviceID)
				{
					m_pFaultReport->m_sPriDeviceID = pDevice->m_sID;
					m_pFaultReport->m_pPriDevice = pDevice;
					bResult = TRUE;
				}
			}
		}
	}
	//��������
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULT_CMB_TYPE);
	if(pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		if(nIndex >= 0)
		{
			if(m_pFaultReport->m_nFaultType != nIndex)
			{
				m_pFaultReport->m_nFaultType = nIndex;
				bResult = TRUE;
			}
		}
	}
	//���ϲ��
	if(m_sDistance != m_pFaultReport->m_sFaultDistance)
	{
		m_pFaultReport->m_sFaultDistance = m_sDistance;
		bResult = TRUE;
	}
	//��վ1
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULT_CMB_STATION1);
	if(pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		if(nIndex >= 0)
		{
			CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nIndex);
			if(pStation != NULL)
			{
				if(pStation->m_sID != m_pFaultReport->m_sStation1ID)
				{
					m_pFaultReport->m_sStation1ID = pStation->m_sID;
					m_pFaultReport->m_pStation1 = pStation;
					bResult = TRUE;
				}
			}
			else
			{
				if(m_pFaultReport->m_pStation1 != NULL)
					bResult = TRUE;
				m_pFaultReport->m_sStation1ID = "";
				m_pFaultReport->m_pStation1 = NULL;
			}
		}
	}
	//��վ2
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULT_CMB_STATION2);
	if(pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		if(nIndex >= 0)
		{
			CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nIndex);
			if(pStation != NULL)
			{
				if(pStation->m_sID != m_pFaultReport->m_sStation2ID)
				{
					m_pFaultReport->m_sStation2ID = pStation->m_sID;
					m_pFaultReport->m_pStation2 = pStation;
					bResult = TRUE;
				}
			}
			else
			{
				if(m_pFaultReport->m_pStation2 != NULL)
					bResult = TRUE;
				m_pFaultReport->m_sStation2ID = "";
				m_pFaultReport->m_pStation2 = NULL;
			}
		}
	}
	//�Ƿ�浵
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_CMB_FAULT_SAVE);
	if (pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		if (nIndex != (!m_pFaultReport->m_bIsSaved))
		{
			m_pFaultReport->m_bIsSaved = !nIndex;
			bResult =TRUE;
		}
	}

	CString strFaultReason;
	m_FaultReasonEdit.GetWindowText(strFaultReason);
	if (strFaultReason.CompareNoCase(m_sFaultReason))
	{
		m_sFaultReason = strFaultReason;
		m_pFaultReport->m_sFaultReason = strFaultReason;
		bResult = TRUE;
	}
	return bResult;
}

//##ModelId=49B87BA400FB
void CDlgFaultReport::OnOK() 
{
	// TODO: Add extra validation here
	CString strFaultReason;
	m_FaultReasonEdit.GetWindowText(strFaultReason);
	if (strFaultReason.GetLength() > 2000)
	{
		AfxMessageBox( StringFromID(IDS_FAULTREASON_TOOLONG));
		return;
	}
	BOOL bNeedSaveReport = NeedSaveReport();
	if(bNeedSaveReport)
	{	
		//�������ݿ�
		if(!m_pFaultReport->SaveToDB())
		{
			//���¹��ϰ�ʧ��
			AfxMessageBox( StringFromID(IDS_FAULTREPORT_UPDATE_FAIL));
			return;
		}
	    WriteLog("CDlgFaultReport::OnOK ���ݱ������ɹ�\n");
	}
	int nSave = SaveEvent();
	if(nSave == 1) //ɾ��ʱ����
	{
		//ɾ���¼�ʧ��
		AfxMessageBox( StringFromID(IDS_FAULTEVENT_DELETE_FAIL));
		return;
	}
	else if(nSave == 2) //����ʱ����
	{
		//�����¼�ʧ��
		AfxMessageBox( StringFromID(IDS_FAULTEVENT_UPDATE_FAIL));
		return;
	}
	//������Ϣ������
	if(m_pWnd != NULL)
	{
		m_pWnd->SendMessage(FAULTREPORT_EDITED, 0, (LPARAM)m_pFaultReport);
	}

	if (bNeedSaveReport)
	{
		int result = 0;
		if (m_pWnd->IsKindOf(RUNTIME_CLASS(CViewFault)))
		{
			result = ((CViewFault*)m_pWnd)->IssueToWeb(m_pFaultReport);
			if (result == 0)
			{
				AfxMessageBox( StringFromID(IDS_FAULTREPORT_SYNC_SUCCESS));
			}
		}
		else
		{
			result = ((CViewFaultQuery*)m_pWnd)->IssueToWeb(m_pFaultReport);
			if (result == 0)
			{
				AfxMessageBox( StringFromID(IDS_FAULTREPORT_SYNC_SUCCESS));
			}
		}
	}

	CDialog::OnOK();
}

//##ModelId=49B87BA400FD
void CDlgFaultReport::OnDlgFaultRemove() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_List.GetSelectedCount();
	//��ѡ��
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	//��¼Ҫɾ�����¼�
	CPtrArray arrRemove;
	int nIndex = -1;
	int i = 0;
	for(i = 0; i < nCount; i++)
	{
		nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
		if(nIndex != -1)
		{
			CFaultEvent* pEvent = (CFaultEvent*)m_List.GetItemData(nIndex);
			if(pEvent != NULL)
			{
				//���ӵ�Ҫɾ������
				arrRemove.Add(pEvent);
			}
		}
	}

	//ѭ��ɾ��
	for(i = 0; i < arrRemove.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)arrRemove.GetAt(i);
		RemoveFromList(pEvent);
		RemoveFromArray(pEvent);
	}
	//���
	arrRemove.RemoveAll();
	//������Ҫ�����־
	m_bEventNeedSave = TRUE;
}

/*************************************************************
 �� �� ����RemoveFromList()
 ���ܸ�Ҫ�����б�����Ƴ��¼�
 �� �� ֵ: 
 ��    ����param1	ָ��Ҫ�Ƴ����¼�
		   Param2
**************************************************************/
//##ModelId=49B87BA400CC
void CDlgFaultReport::RemoveFromList( CFaultEvent* pEvent )
{
	//���������Ч��
	if(pEvent == NULL)
		return;

	//�����¼����б��е�λ��
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pEvent;
	int nIndex = m_List.FindItem(&lvFind);

	//δ�ҵ�
	if(nIndex < 0)
		return;

	//���б�����Ƴ�
	m_List.DeleteItem(nIndex);
}

/*************************************************************
 �� �� ����RemoveFromArray()
 ���ܸ�Ҫ�����¼�������ɾ���¼�
 �� �� ֵ: void
 ��    ����param1	ָ��Ҫ�Ƴ����¼�
		   Param2
**************************************************************/
//##ModelId=49B87BA400CE
void CDlgFaultReport::RemoveFromArray( CFaultEvent* pEvent )
{
	//���������Ч��
	if(pEvent == NULL)
		return;
	if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
	{
		//�����¼�������
		for(int i = 0; i < m_listChr.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listChr.GetAt(i);
			if(pNew == pEvent)
			{
				//���������Ƴ�
				m_listChr.RemoveAt(i);
				//ɾ��
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_SING)
	{
		//�����¼��ź���
		for(int i = 0; i < m_listSign.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listSign.GetAt(i);
			if(pNew == pEvent)
			{
				//���������Ƴ�
				m_listSign.RemoveAt(i);
				//ɾ��
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
	{
		//�澯�¼�
		for(int i = 0; i < m_listAlarm.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listAlarm.GetAt(i);
			if(pNew == pEvent)
			{
				//���������Ƴ�
				m_listAlarm.RemoveAt(i);
				//ɾ��
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_OSCREPORT)
	{
		//����¼��
		for(int i = 0; i < m_listPTOSC.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listPTOSC.GetAt(i);
			if(pNew == pEvent)
			{
				//���������Ƴ�
				m_listPTOSC.RemoveAt(i);
				//ɾ��
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
		//¼����¼��
		for(i = 0; i < m_listWROSC.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listWROSC.GetAt(i);
			if(pNew == pEvent)
			{
				//���������Ƴ�
				m_listWROSC.RemoveAt(i);
				//ɾ��
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_DICHANGE)
	{
		//�澯�¼�
		for(int i = 0; i < m_listDIChange.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listDIChange.GetAt(i);
			if(pNew == pEvent)
			{
				//���������Ƴ�
				m_listDIChange.RemoveAt(i);
				//ɾ��
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
	}
}

/*************************************************************
 �� �� ����SaveEvent()
 ���ܸ�Ҫ����������¼�
 �� �� ֵ: ����ɹ�0,ɾ������ʧ�ܷ���1,��������ʧ�ܷ���2
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400DA
int CDlgFaultReport::SaveEvent()
{
	//�ж��Ƿ���Ҫ����
	if(!m_bEventNeedSave)
		return 0;
	if(m_pFaultReport == NULL)
		return 1;
	//ɾ������Ϣ
	m_pFaultReport->ClearDetail();
	//��������Ϣ
	//����������
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listChr.Add(pNew);
	}
	
	//�����ź���
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listSign.Add(pNew);
	}
	
	//�澯�¼�
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listAlarm.Add(pNew);
	}
	
	//����¼��
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listPTOSC.Add(pNew);
		if ( !pEvent->m_sOldDir.IsEmpty() && g_bSend20162 == 1)
		{
			RenameOscFile(pEvent);
		}
	}
	
	//¼����¼��
	for(i = 0; i < m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWROSC.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listWROSC.Add(pNew);
		if ( !pEvent->m_sOldDir.IsEmpty() && g_bSend20162 == 1)
		{
			RenameOscFile(pEvent);
		}
	}

	//��������λ
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listDIChange.Add(pNew);
	}

	//С�����
	for(i = 0; i < m_listDistance.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDistance.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listWaveDistance.Add(pNew);
	}
	
	//¼������
	for(i = 0; i < m_listOSCParse.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCParse.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listOSCParse.Add(pNew);
	}

	//���浽���ݿ�
	int nResult = m_pFaultReport->MultiSaveEvent();
	return nResult;
}

//##ModelId=49B87BA400FF
void CDlgFaultReport::OnDlgFaultAdd() 
{
	// TODO: Add your control notification handler code here
	CDlgFaultEventAdd dlg(this);
	dlg.DoModal();
}

/*************************************************************
 �� �� ����UpdateList()
 ���ܸ�Ҫ��ˢ���б����ʾ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400DB
void CDlgFaultReport::UpdateList()
{
	int nIndex = m_Tab.GetCurSel();
	FillListData(nIndex);
}

void CDlgFaultReport::OnDlgFaultAddosc() 
{
	// TODO: Add your control notification handler code here
	CDlgAddOsc dlg(this);
	dlg.m_pReport = m_pFaultReport;
	dlg.DoModal();
	UpdateList();
}

void CDlgFaultReport::SelCurTab(int nSel)
{
	m_Tab.SetCurSel(nSel);
    m_Tab.SetCurFocus(nSel);
	if (nSel != 4)
	{
		m_List.ShowWindow(SW_SHOW);
		m_FaultReasonEdit.ShowWindow(SW_HIDE);
	}
	else
	{
        m_List.ShowWindow(SW_HIDE);
		m_FaultReasonEdit.ShowWindow(SW_SHOW);
	}
}



void CDlgFaultReport::RenameOscFile( CFaultEvent* pEvent )
{
	MYASSERT(pEvent);
	//������Ϣ��ͨ�ŷ�����
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//���ͨѶ���
	
	
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}

	
	//���뱨����������
	STTP_FULL_DATA sttpData;
	
	//��֯20162����
	if(!pApp->m_SttpEngine.BuildDataFor20162RenameOscFile(sttpData, pEvent))
	{
		//��֯����ʧ��
		CString str;
		str.Format("�齨¼���ļ���������Ҫ����ʧ��,���Ϲ鵵��ID:%d, �����¼�ID:%d", pEvent->m_nMainRecID, pEvent->m_nID);
		WriteLog(str);
		return;
	}
	
	//���ͱ���
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//�ж��Ƿ�ɹ�
	if(nReturn != 1)
	{
		CString str;
		str.Format("����¼���ļ���������Ҫ����ʧ��,���Ϲ鵵��ID:%d, �����¼�ID:%d", pEvent->m_nMainRecID, pEvent->m_nID);
		WriteLog(str);
		return;
	}
}


void CDlgFaultReport::OnColumnclickDlgFaultList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iStationInfoViewAsc = - g_iStationInfoViewAsc;
	}
	else
	{
		g_iStationInfoViewAsc = 1;
		m_nOldSortCol = iCol;
	}

	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}
/*************************************************************
 �� �� ��: CompareFunction()
 ���ܸ�Ҫ: �ȽϺ���,����ʱ������
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ��: param1 �Ƚ϶���1
		   Param2 �Ƚ϶���2
		   Param3 �Ƚϲ���
**************************************************************/
int CALLBACK CDlgFaultReport::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CFaultEvent* pEvent1 = (CFaultEvent*)lParam1;
	CFaultEvent* pEvent2 = (CFaultEvent*)lParam2;
	if(pEvent1 == NULL || pEvent2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"�����豸","����ʱ��","�¼�����", "�¼�ֵ", "����ʱ��"
	switch(nCol)
	{
	case 0:	//�����豸
		str1 = pEvent1->m_pSec->m_sName;
		str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;

	case 1: //����ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nmsStart);
		str2.Format("%s.%03d", pEvent2->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nmsStart);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);

		break;
	case 2: //�¼�����
		if(pEvent1->m_nType == 4 && pEvent2->m_nType == 4)
		{
			str1 = pEvent1->m_sEventDef;
			str2 = pEvent2->m_sEventDef;
		}
		else
		{
			str1 = pEvent1->m_sEventDefName;
			str2 = pEvent2->m_sEventDefName;
		}
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 3: //�¼�ֵ
		str1 = pEvent1->m_sEventContent;
		if(pEvent1->m_nType == XJ_FAULT_EVENT_SING || pEvent1->m_nType == XJ_FAULT_EVENT_ALARM)
		{
			//�ź���
			int nContent = atoi(str1);
			if(nContent == 1)
				str1 = StringFromID(IDS_CASE_ACTION);
			else if(nContent == 0)
				str1 = StringFromID(IDS_CASE_RETURN);
		}
		if(pEvent1->m_nType == XJ_FAULT_EVENT_CHR)
		{
			//����ֵ
			if(pEvent1->m_strUnit != "" && pEvent1->m_sEventContent != "")
				str1.Format("%s(%s)", pEvent1->m_sEventContent, pEvent1->m_strUnit);
		}
		if(pEvent2->m_nType == XJ_FAULT_EVENT_SING || pEvent2->m_nType == XJ_FAULT_EVENT_ALARM)
		{
			//�ź���
			int nContent = atoi(str2);
			if(nContent == 1)
				str2 = StringFromID(IDS_CASE_ACTION);
			else if(nContent == 0)
				str2 = StringFromID(IDS_CASE_RETURN);
		}
		if(pEvent2->m_nType == XJ_FAULT_EVENT_CHR)
		{
			//����ֵ
			if(pEvent2->m_strUnit != "" && pEvent2->m_sEventContent != "")
				str2.Format("%s(%s)", pEvent2->m_sEventContent, pEvent2->m_strUnit);
		}

		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);

		break;
	case 4: //����ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nmsReceive);
		str2.Format("%s.%03d", pEvent2->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nmsReceive);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //�ź�������ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmSign.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nmsSign);
		str2.Format("%s.%03d", pEvent2->m_tmSign.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nmsSign);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	default:
		iResult = 0;
		break;
	}

	iResult *= g_iStationInfoViewAsc;
	return iResult;
}
