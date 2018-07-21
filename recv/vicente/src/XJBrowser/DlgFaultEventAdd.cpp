// DlgFaultEventAdd.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgFaultEventAdd.h"
#include "FaultReport.h"
#include "DlgFaultReport.h"
#include "GlobalFunc.h"
// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFaultEventAdd dialog

extern int g_iStationInfoViewAsc;
//##ModelId=49B87BA4012A
CDlgFaultEventAdd::CDlgFaultEventAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFaultEventAdd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFaultEventAdd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	g_iStationInfoViewAsc = -1;
	m_nOldSortCol = -1;
	m_nMainRecID = -1;
	m_listAlarm.RemoveAll();
	m_listChr.RemoveAll();
	m_listOSC.RemoveAll();
	m_listSign.RemoveAll();
	m_listDIChange.RemoveAll();
}


//##ModelId=49B87BA401E4
void CDlgFaultEventAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFaultEventAdd)
	DDX_Control(pDX, IDC_DLG_FAULTEVENT_CMB_DEVICE, m_cmbPriDev);
	DDX_Control(pDX, IDC_DLG_FAULTEVENT_CMB_SEC, m_cmbSec);
	DDX_Control(pDX, IDC_DLG_FAULTEVENT_CMB_STATION, m_cmbStation);
	DDX_Control(pDX, IDC_DLG_FAULTEVENT_TAB, m_Tab);
	DDX_Control(pDX, IDC_DLG_FAULTITEM_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFaultEventAdd, CDialog)
	//{{AFX_MSG_MAP(CDlgFaultEventAdd)
	ON_BN_CLICKED(IDC_DLG_FAULTEVENT_QUERY, OnDlgFaulteventQuery)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DLG_FAULTEVENT_TAB, OnSelchangeDlgFaulteventTab)
	ON_NOTIFY(NM_CLICK, IDC_DLG_FAULTITEM_LIST, OnClickDlgFaultitemList)
	ON_CBN_SELCHANGE(IDC_DLG_FAULTEVENT_CMB_STATION, OnSelchangeDlgFaulteventCmbStation)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_DLG_FAULTITEM_LIST, OnColumnclickDlgFaultitemList)
	ON_CBN_SELCHANGE(IDC_DLG_FAULTEVENT_CMB_DEVICE, OnSelchangeDlgFaulteventCmbDevice)
	ON_CBN_SELCHANGE(IDC_DLG_FAULTEVENT_CMB_SECTYPE, OnSelchangeDlgFaulteventCmbSectype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDlgFaultEventAdd message handlers

//##ModelId=49B87BA401E7
BOOL CDlgFaultEventAdd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//��䳧վ������
	FillStation();
	//���һ���豸����
	FillDevType();

	//�������豸������
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	MYASSERT_BOOL(pCmb);
	int nIndex = pCmb->GetCurSel();
	if(nIndex < 0)
		return FALSE;
	CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nIndex);
	//���һ���豸
	FillDevName(pStation);
	//�������豸
	FillSec(pStation);
	//����ʱ���ʽ
	SetTimeCtrl();
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_EVENTTYPE);
	MYASSERT_BOOL(pCmb);
	pCmb->InsertString(0, StringFromID(IDS_EVENTTYPE_ACTION));
	pCmb->InsertString(1, StringFromID(IDS_EVENTTYPE_ALARM));
	pCmb->InsertString(2, StringFromID(IDS_EVENTTYPE_OSC));
	pCmb->InsertString(3, StringFromID(IDS_EVENTTYPE_DI));
	pCmb->InsertString(4, StringFromID(IDS_COMMON_ALL));
	pCmb->SetCurSel(0);
	//8.TAB��
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
	//��ʼ���б����
	InitListStyle();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 �� �� ����FillStation()
 ���ܸ�Ҫ����䳧վ������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40168
void CDlgFaultEventAdd::FillStation()
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	if(pCmb == NULL)
		return;
	//���ԭ������
	pCmb->ResetContent();
	//����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�õ������ڼ��صĳ�վ
	CDlgFaultReport* pParent = (CDlgFaultReport*)GetParent();
	CStationObj* pParentStation = NULL;
	int nParentIndex = -1;
	if(pParent != NULL)
	{
		if(pParent->m_pFaultReport != NULL)
		{
			//��¼��վָ��
			if(pParent->m_pFaultReport->m_pStation1 != NULL)
				pParentStation = pParent->m_pFaultReport->m_pStation1;
			//��¼������Ϣ��ID
			m_nMainRecID = pParent->m_pFaultReport->m_nID;
		}
	}
	//���г�վ
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if(pStation == NULL)
			continue;
		//���뵽������
		int nIndex = pCmb->AddString(pStation->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pStation);
			if(pParentStation == pStation)
			{
				//��¼λ��
				nParentIndex = nIndex;
			}
		}
	}
	//����"����"ѡ��
	int nIndex = pCmb->AddString("("+StringFromID(IDS_COMMON_ALL)+")");
	//����Ԥѡ����
	if(nParentIndex > -1)
	{
		pCmb->SetCurSel(nParentIndex);
	}
	else
	{
		if(nIndex > -1)
			pCmb->SetCurSel(nIndex);
	}
	m_cmbStation.OnDataFilled(TRUE, nIndex);
}

/*************************************************************
 �� �� ����FillSec()
 ���ܸ�Ҫ�����ָ����վ�µĶ����豸��������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40169
void CDlgFaultEventAdd::FillSec(CStationObj* pStation)
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SEC);
	//CComboBox* pCmb2 = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SECTYPE);
	if(pCmb == NULL)
		return;
	//���ԭ������
	pCmb->ResetContent();

	if(pStation != NULL)
	{
		//ָ���˳�վ
	//	FillSecType(pStation);
		FillSpecificSec(pStation, pCmb);
	}
	else
	{
		//���г�վ
	//	FillSecType(pStation);
		FillSpecificSec(NULL, pCmb);
	}
	//����"����"ѡ��
	int nIndex = pCmb->AddString("("+StringFromID(IDS_COMMON_ALL)+")");
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	m_cmbSec.OnDataFilled();
}

/*************************************************************
 �� �� ����FillSpecificSec()
 ���ܸ�Ҫ�����ָ����վ�µĶ����豸��ָ��������
 �� �� ֵ: void
 ��    ����param1	ָ����վ,NULL��ʾ���г�վ
		   Param2	ָ��������
**************************************************************/
//##ModelId=49B87BA40178
void CDlgFaultEventAdd::FillSpecificSec( CStationObj* pStation, CComboBox* pCmb )
{
	//���������Ч��
	if(pCmb == NULL)
		return;
	CString sStationID = "";
	if(pStation != NULL)
		sStationID = pStation->m_sID;

	//ѭ����վ�µı�����¼����
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	CComboBox* pCmbSec = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_DEVICE);
	if (pCmbSec==NULL)
		return;
	
	//���г�վ
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	int nSec = pCmbSec->GetCurSel();
	CString str;
	pCmbSec->GetLBText(nSec,str);
	if (str != "("+StringFromID(IDS_COMMON_ALL)+")" && str != "")
	{
		CSecObj* pSec = (CSecObj*)pCmbSec->GetItemData(nSec);
		CString sID = "";
		if (pSec!=NULL)
		{
			sID=pSec->m_sID;
		}
		pData->GetSecList(listDevice, sID, TYPE_SEC, sStationID);
	}
	else
	{
		pData->GetSecList(listDevice, "", TYPE_SEC, sStationID);
	}
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		if(pObj->m_nType != TYPE_PROTECT && pObj->m_nType != TYPE_WAVEREC)
			continue;
		//���뵽������
		CString str;
		str = pObj->m_sName;
		int nIndex = pCmb->AddString(str);
		if(nIndex < 0)
			continue;
		//���ù���
		pCmb->SetItemData(nIndex, (DWORD)pObj);
	}
}

/*************************************************************
 �� �� ����SetTimeCtrl()
 ���ܸ�Ҫ������ʱ��ؼ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40186
void CDlgFaultEventAdd::SetTimeCtrl()
{
	//���������Ч��
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULTEVENT_TIME1);
	if(pTime == NULL)
		return;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULTEVENT_TIME2);
	if(pTime2 == NULL)
		return;
	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	pTime2->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//���÷�Χ
	CTime tmMin(1971, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	pTime2->SetRange(&tmMin, &tmMax);
	//����ʱ��
	CTime tm = CTime::GetCurrentTime();
	CTimeSpan ts(30, 0, 0, 0);
	CTime tm1 = tm - ts;
	VERIFY(pTime->SetTime(&tm1));
}

/*************************************************************
 �� �� ����InitListStyle()
 ���ܸ�Ҫ����ʼ���б���
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40187
void CDlgFaultEventAdd::InitListStyle()
{
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	
	char* arColumn[5]={"�����豸","����ʱ��","�¼�����", "�¼�ֵ", "����ʱ��"};
	
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
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
}

/*************************************************************
 �� �� ����ClearEvent()
 ���ܸ�Ҫ��ɾ�������¼�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40196
void CDlgFaultEventAdd::ClearEvent()
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
	//�澯�¼�
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		if(pEvent != NULL)
			delete pEvent;
	}
	m_listAlarm.RemoveAll();
	//����¼��
	for(i = 0; i < m_listOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
		if(pEvent != NULL)
			delete pEvent;
	}
	m_listOSC.RemoveAll();
	//��������λ
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		if(pEvent != NULL)
			delete pEvent;
	}
}

//##ModelId=49B87BA401F4
void CDlgFaultEventAdd::OnDlgFaulteventQuery() 
{
	// TODO: Add your control notification handler code here
	//��վ�����
	//����б����ʾ
	m_List.DeleteAllItems();
	//�������
	ClearEvent();
	//�õ���ѯ����

	//�����豸ID
	CString sSec = "";
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SEC);
	if(pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		if(nIndex > -1)
		{
			//��Ч����
			CSecObj* pSec = (CSecObj*)pCmb->GetItemData(nIndex);
			if(pSec != NULL)
			{
				sSec = pSec->m_sID;
			}
		}
	}

	m_sPridevID = "";
	m_sStationID = "";
	m_nPridevType = -1;
	
	//һ���豸
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_DEVICE);
	if(pCmb != NULL)
	{
		CDeviceObj* pPri = (CDeviceObj*)pCmb->GetItemData(pCmb->GetCurSel());
		if(pPri)
			m_sPridevID = pPri->m_sID;
	}
	
	//һ���豸����
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SECTYPE);
	if(pCmb)
	{
		int nItem = pCmb->GetCurSel()+1;
		if (nItem>10||nItem<=0)
		{
			nItem = -1;
		}
		m_nPridevType = nItem;
	}
	
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	if(pCmb)
	{
		CDeviceObj* pStation = (CDeviceObj*)pCmb->GetItemData(pCmb->GetCurSel());
		if(pStation)
			m_sStationID = pStation->m_sID;
	}


	//��ʼʱ��
	CTime tmStart(1971,1,1,1,1,1);
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULTEVENT_TIME1);
	if(pTime != NULL)
	{
		pTime->GetTime(tmStart);
	}
	//����ʱ��
	CTime tmEnd(1971,1,1,1,1,1);
	pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULTEVENT_TIME2);
	if(pTime != NULL)
	{
		pTime->GetTime(tmEnd);
	}

	int nEventType = 4;
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_EVENTTYPE);
	if(pCmb != NULL)
		nEventType = pCmb->GetCurSel();

	g_wndWait.Show();
	//��ѯ����
	if(nEventType == 0 || nEventType == 4)
		QueryAction(sSec, tmStart, tmEnd);
	//��ѯ�澯
	if(nEventType == 4 || nEventType == 1)
		QueryAlarm(sSec, tmStart, tmEnd);
	//¼����
	if(nEventType == 4 || nEventType == 2)
		QueryOSC(sSec, tmStart, tmEnd);
	//��������λ
	if(nEventType == 4 || nEventType == 3)
		QueryDIChange(sSec, tmStart, tmEnd);

	//���б������ʾ
	int nIndex = m_Tab.GetCurSel();
	FillListData(nIndex);

	g_wndWait.Hide();
}

//##ModelId=49B87BA40138
CDlgFaultEventAdd::~CDlgFaultEventAdd()
{
	ClearEvent();	
}

/*************************************************************
 �� �� ����QueryAction()
 ���ܸ�Ҫ����ѯ�����¼�, ��䶯������ֵ����Ͷ����ź�����
 �� �� ֵ: void
 ��    ����Param1	�����豸ID, ""�������ж����豸
		   Param2	��ʼʱ��
		   Param3	����ʱ��
**************************************************************/
//##ModelId=49B87BA40197
void CDlgFaultEventAdd::QueryAction( CString sSec, CTime tmStart, CTime tmEnd )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//�ֶ�
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

		
	//����
	//PT_ID
	if(sSec != "")
	{
		Condition condition1;
		str.Format("pt_id='%s'", sSec);
		pApp->m_DBEngine.SetCondition(sql, condition1, str);
	}
	else
	{
		if( m_sPridevID != "" )
		{
			Condition condition1;
			str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_id='%s')", m_sPridevID);
			pApp->m_DBEngine.SetCondition(sql, condition1, str);
			
		}
		else
		{
			if( m_nPridevType != -1)
			{
				if(m_sStationID == "")
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_type=%d)", m_nPridevType);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
				else
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_type=%d and station_id='%s')", m_nPridevType, m_sStationID);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
			}
			else
			{
				if(m_sStationID != "")
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where station_id='%s')", m_sStationID);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
			}
		}
	}

	//��ʼʱ��
	Condition con4;
	str.Format("curtime > '%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//����ʱ��
	Condition con5;
	str.Format("curtime < '%s'", tmEnd.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con5, str);
			
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
		WriteLog("CDlgFaultEventAdd::QueryAction, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDlgFaultEventAdd::QueryAction, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventAction* pEvent = new CXJEventAction;
			if(pEvent->LoadFromDB(pMemset))
			{
				//�����¼�
				pEvent->TranslateFData();
				//�ֽ��¼�
				BreakAction(pEvent);
			}
			delete pEvent;
			pEvent = NULL;
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDlgFaultEventAdd::QueryAction,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return;
}

/*************************************************************
 �� �� ����BreakAction()
 ���ܸ�Ҫ���ֽ⶯���¼�������ֵ���ź���Ϊ���������¼�,�ֱ𱣴�
 �� �� ֵ: void
 ��    ����param1	ָ�������¼�
		   Param2
**************************************************************/
//##ModelId=49B87BA401A6
void CDlgFaultEventAdd::BreakAction( CXJEventAction* pEvent )
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
		pFaultEvent->m_nType = XJ_FAULT_EVENT_CHR; //��������ֵ
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
		pFaultEvent->m_nType = XJ_FAULT_EVENT_SING; //�����ź�
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

/*************************************************************
 �� �� ����FillListData()
 ���ܸ�Ҫ�����ֵ���б��
 �� �� ֵ: 
 ��    ����param1	Ҫ����������.0-��������ֵ.1-������Ϣ��.2-�澯.3-¼����
		   Param2
**************************************************************/
//##ModelId=49B87BA401A8
void CDlgFaultEventAdd::FillListData( int nType )
{
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//���ԭ����ֵ
	m_List.DeleteAllItems();
	//��������ֵ
	if(0 == nType)
	{
		m_List.DeleteColumn(5);
		for(int i = 0; i < m_listChr.GetSize(); i++)
		{
			CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
			if(pEvent == NULL)
				continue;
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
			if(pEvent == NULL)
				continue;
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
			if(pEvent == NULL)
				continue;
			AddEventToList(pEvent);
		}
	}
	//¼����
	if(3 == nType)
	{
		m_List.DeleteColumn(5);
		for(int i = 0; i < m_listOSC.GetSize(); i++)
		{
			CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
			if(pEvent == NULL)
				continue;
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
			if(pEvent == NULL)
				continue;
			AddEventToList(pEvent);
		}
	}
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 �� �� ����AddEventToList()
 ���ܸ�Ҫ�������¼����б�
 �� �� ֵ: 
 ��    ����param1	�¼�ָ��
		   Param2	Ҫ�����λ������
**************************************************************/
//##ModelId=49B87BA401B5
void CDlgFaultEventAdd::AddEventToList( CFaultEvent* pEvent, int nIndex /*= 0*/ )
{
	//���������Ч��
	if(pEvent == NULL)
		return;
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

	if(pEvent->m_bSelected)
		m_List.SetCheck(nIndex);
	//�����豸
	m_List.SetItemData(nIndex, (DWORD)pEvent);
}

//##ModelId=49B87BA401F6
void CDlgFaultEventAdd::OnSelchangeDlgFaulteventTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_Tab.GetCurSel();
	FillListData(nIndex);
	*pResult = 0;
}

/*************************************************************
 �� �� ����QueryAlarm()
 ���ܸ�Ҫ����ѯ�澯�¼�
 �� �� ֵ: 
 ��    ����param1	�����豸ID, ""�������ж����豸
		   Param2	��ʼʱ��
		   Param3	����ʱ��
**************************************************************/
//##ModelId=49B87BA401B8
void CDlgFaultEventAdd::QueryAlarm( CString sSec, CTime tmStart, CTime tmEnd )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//�ֶ�
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "msg", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field31;
	pApp->m_DBEngine.SetField(sql, Field31, "alarm_id", EX_STTP_DATA_TYPE_INT);
	
	Field Field32;
	pApp->m_DBEngine.SetField(sql, Field32, "value", EX_STTP_DATA_TYPE_INT);
	
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
	
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "isdebug", EX_STTP_DATA_TYPE_INT);
	
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "property", EX_STTP_DATA_TYPE_INT);
	
	//����
	//PT_ID
	if(sSec != "")
	{
		Condition condition1;
		str.Format("pt_id='%s'", sSec);
		pApp->m_DBEngine.SetCondition(sql, condition1, str);
	}
	else
	{
		if( m_sPridevID != "" )
		{
			Condition condition1;
			str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_id='%s')", m_sPridevID);
			pApp->m_DBEngine.SetCondition(sql, condition1, str);
			
		}
		else
		{
			if( m_nPridevType != -1)
			{
				if(m_sStationID == "")
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_type=%d)", m_nPridevType);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
				else
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_type=%d and station_id='%s')", m_nPridevType, m_sStationID);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
			}
			else
			{
				if(m_sStationID != "")
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where station_id='%s')", m_sStationID);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
			}
		}
	}
	
	//��ʼʱ��
	Condition con4;
	str.Format("curtime > '%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//����ʱ��
	Condition con5;
	str.Format("curtime < '%s'", tmEnd.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con5, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_ALARM_DATA, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDlgFaultEventAdd::QueryAlarm, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDlgFaultEventAdd::QueryAlarm, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventAlarm* pEvent = new CXJEventAlarm;
			if(pEvent->LoadFromDB(pMemset))
			{
				//�����¼�
				pEvent->TranslateFData();
				//�ֽ��¼�
				BreakAlarm(pEvent);
			}
			delete pEvent;
			pEvent = NULL;
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDlgFaultEventAdd::QueryAlarm,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return;
}

/*************************************************************
 �� �� ����BreakAlarm()
 ���ܸ�Ҫ���ֽ�澯�¼�Ϊ�����¼�-�澯����,���浽���ϸ澯����
 �� �� ֵ: void
 ��    ����param1	ָ���澯�¼�
		   Param2
**************************************************************/
//##ModelId=49B87BA401C8
void CDlgFaultEventAdd::BreakAlarm( CXJEventAlarm* pEvent )
{
	//���������Ч��
	if(pEvent == NULL)
		return;
	//�ź���
	for(int i = 0; i < pEvent->m_Signals.GetSize(); i++)
	{
		PT_Signal * pSgn = (PT_Signal*)pEvent->m_Signals.GetAt(i);
		if(pSgn == NULL)
			continue;
		//���������¼�
		CFaultEvent* pFaultEvent = new CFaultEvent;
		pFaultEvent->m_nType = XJ_FAULT_EVENT_ALARM; //���ϸ澯
		pFaultEvent->m_nMainRecID = m_nMainRecID;;//���ϰ�ID
		pFaultEvent->m_tmStart = pEvent->m_tmTime;
		pFaultEvent->m_nmsStart = pEvent->m_nms;
		pFaultEvent->m_tmReceiveTime = pEvent->m_tmTime2;
		pFaultEvent->m_nmsReceive = pEvent->m_nms2;
		pFaultEvent->m_pSec = pEvent->m_pSec;
		pFaultEvent->m_sEventContent.Format("%d", pSgn->nValue);
		pFaultEvent->m_sEventDef.Format("%d", pSgn->nID);
		pFaultEvent->m_tmSign = pEvent->m_tmTime;
		pFaultEvent->m_nmsSign = pEvent->m_nms;
		pFaultEvent->m_sEventDefName = pSgn->strName;
		pFaultEvent->m_sSecID = pEvent->m_sSecID;
		//���뵽����
		m_listAlarm.Add(pFaultEvent);
	}
}

/*************************************************************
 �� �� ����QueryOSC()
 ���ܸ�Ҫ����ѯ¼���¼�
 �� �� ֵ: void
 ��    ����param1	�����豸ID, ""�������ж����豸
		   Param2	��ʼʱ��
		   Param3	����ʱ��
**************************************************************/
//##ModelId=49B87BA401CA
void CDlgFaultEventAdd::QueryOSC( CString sSec, CTime tmStart, CTime tmEnd )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//�ֶ�
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "filename", EX_STTP_DATA_TYPE_STRING);
	
	Field Field31;
	pApp->m_DBEngine.SetField(sql, Field31, "trigger_time", EX_STTP_DATA_TYPE_TIME);
	
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "ms", EX_STTP_DATA_TYPE_INT);
	
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "savepath", EX_STTP_DATA_TYPE_STRING);
	
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "Bfaultfile", EX_STTP_DATA_TYPE_INT);
	
	Field Field7;
	pApp->m_DBEngine.SetField(sql, Field7, "fault_type", EX_STTP_DATA_TYPE_STRING);
	
	Field Field8;
	pApp->m_DBEngine.SetField(sql, Field8, "trigger_type", EX_STTP_DATA_TYPE_STRING);
	
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "reclose_time", EX_STTP_DATA_TYPE_INT);
	
	Field Field12;
	pApp->m_DBEngine.SetField(sql, Field12, "Fault_distance", EX_STTP_DATA_TYPE_STRING);

	Field Field13;
	pApp->m_DBEngine.SetField(sql, Field13, "Is_Ack", EX_STTP_DATA_TYPE_INT);

	Field Field14;
	pApp->m_DBEngine.SetField(sql, Field14, "Reverse2", EX_STTP_DATA_TYPE_STRING);
	
	
	//����
	//PT_ID
	if(sSec != "")
	{
		Condition condition1;
		str.Format("pt_id='%s'", sSec);
		pApp->m_DBEngine.SetCondition(sql, condition1, str);
	}
	else
	{
		if( m_sPridevID != "" )
		{
			Condition condition1;
			str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_id='%s')", m_sPridevID);
			pApp->m_DBEngine.SetCondition(sql, condition1, str);
			
		}
		else
		{
			if( m_nPridevType != -1)
			{
				if(m_sStationID == "")
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_type=%d)", m_nPridevType);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
				else
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_type=%d and station_id='%s')", m_nPridevType, m_sStationID);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
			}
			else
			{
				if(m_sStationID != "")
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where station_id='%s')", m_sStationID);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
			}
		}
	}
	
	//��ʼʱ��
	Condition con4;
	str.Format("trigger_time > '%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//����ʱ��
	Condition con5;
	str.Format("trigger_time < '%s'", tmEnd.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con5, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OSC_LIST, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDlgFaultEventAdd::QueryOSC, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDlgFaultEventAdd::QueryOSC, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventOSC* pEvent = new CXJEventOSC;
			if(pEvent->LoadFromDB_(pMemset))
			{
				//�ֽ��¼�
				BreakOSC(pEvent);
			}
			delete pEvent;
			pEvent = NULL;
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDlgFaultEventAdd::QueryOSC,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return;
}

/*************************************************************
 �� �� ����BreakOSC()
 ���ܸ�Ҫ���ֽ�¼���¼�Ϊ����¼��
 �� �� ֵ: void
 ��    ����param1	ָ��¼���¼�
		   Param2
**************************************************************/
//##ModelId=49B87BA401D7
void CDlgFaultEventAdd::BreakOSC( CXJEventOSC* pEvent )
{
	//���������Ч��
	if(pEvent == NULL)
		return;
	//���������¼�
	CFaultEvent* pFaultEvent = new CFaultEvent;
	pFaultEvent->m_nType = XJ_FAULT_EVENT_OSCREPORT; //¼����
	pFaultEvent->m_nMainRecID = m_nMainRecID;//���ϰ�ID
	pFaultEvent->m_tmStart = pEvent->m_tmTime;
	pFaultEvent->m_nmsStart = pEvent->m_nms;
	pFaultEvent->m_tmReceiveTime = pEvent->m_tmTime;
	pFaultEvent->m_nmsReceive = pEvent->m_nms;
	pFaultEvent->m_pSec = pEvent->m_pSec;
	pFaultEvent->m_sEventContent.Format("%d", pEvent->m_nFileSize); //�ļ���С
	pFaultEvent->m_sEventDef = pEvent->m_sFileName;
	pFaultEvent->m_tmSign = pEvent->m_tmTime;
	pFaultEvent->m_nmsSign = pEvent->m_nms;
	pFaultEvent->m_sEventDefName = pEvent->m_sFileName;
	pFaultEvent->m_sSecID = pEvent->m_sSecID;

	if(pFaultEvent->m_pSec != NULL)
	{
		pFaultEvent->m_sEventDef.Replace("/", "\\");
		if(pFaultEvent->m_sEventDef.Find("\\", 0) == -1)
		{
			if(pFaultEvent->m_pSec->m_nType == TYPE_PROTECT)
				pFaultEvent->m_sEventDef = pFaultEvent->m_pSec->m_sStationID + "\\����\\" + pFaultEvent->m_pSec->m_sName + "(" + pFaultEvent->m_pSec->m_sID + ")" + "\\"+pFaultEvent->m_sEventDef;
			else if(pFaultEvent->m_pSec->m_nType == TYPE_WAVEREC)
				pFaultEvent->m_sEventDef = pFaultEvent->m_pSec->m_sStationID + "\\¼����\\" + pFaultEvent->m_pSec->m_sName + "(" + pFaultEvent->m_pSec->m_sID + ")" + "\\"+pFaultEvent->m_sEventDef;
		}
	}

	//���뵽����
	m_listOSC.Add(pFaultEvent);
}


void CDlgFaultEventAdd::OnClickDlgFaultitemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//�õ���ѡ��
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
	if(nSelected < 0)
		return;
	//�ж��Ƿ�check
	CPoint pt = pNMListView->ptAction;
	UINT uFlag;
	int nIndex = m_List.HitTest(pt, &uFlag);
	if(uFlag == LVHT_ONITEMSTATEICON)
	{
		//�õ���ѡ���е�����
		CFaultEvent* pEvent = (CFaultEvent*)m_List.GetItemData(nIndex);
		/*
		if(pEvent != NULL)
		{
			pEvent->m_bSelected = !pEvent->m_bSelected;
		}
		*/
		MYASSERT(pEvent);
		CString str = _T("");
		if(FALSE == m_List.GetCheck(nIndex))
        {
           // m_List.SetCheck(nIndex, TRUE);
			pEvent->m_bSelected = 1;
        }
        else                              //ע������ط�������
        {
           // m_List.SetCheck(nIndex, FALSE);
			pEvent->m_bSelected = 0;
        }
	}

	*pResult = 0;
}

/*************************************************************
 �� �� ����SaveEvent()
 ���ܸ�Ҫ�����汻ѡ����¼������ϰ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA401D9
void CDlgFaultEventAdd::SaveEvent()
{
	//���������Ч��
	CDlgFaultReport* pParent = (CDlgFaultReport*)GetParent();
	if (pParent == NULL)
	{
		MYASSERT(pParent);
		return;
	}
	//��������ֵ
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		//�õ��¼�
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//��ѡ��ļ��뵽�����ڵĶ�Ӧ������
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			pParent->m_listChr.Add(pNew);
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//�����ź�
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		//�õ��¼�
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//��ѡ��ļ��뵽�����ڵĶ�Ӧ������
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			pParent->m_listSign.Add(pNew);
			pParent->m_bEventNeedSave = TRUE;
		}
	}
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
	//��������λ
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		//�õ��¼�
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//��ѡ��ļ��뵽�����ڵĶ�Ӧ������
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			pParent->m_listDIChange.Add(pNew);
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//ˢ�¸������б��
	pParent->UpdateList();
}

//##ModelId=49B87BA40208
void CDlgFaultEventAdd::OnOK() 
{
	// TODO: Add extra validation here
	SaveEvent();
	CDialog::OnOK();
}

//##ModelId=49B87BA4020A
void CDlgFaultEventAdd::OnSelchangeDlgFaulteventCmbStation() 
{
	// TODO: Add your control notification handler code here
	//ȡ�ó�վ
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
	//��������豸����������
	FillDevName(pStation);
	//���������豸
	FillSec(pStation);
}
/*************************************************************
 �� �� ��: CompareFunction()
 ���ܸ�Ҫ: �ȽϺ���,����ʱ������
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ��: param1 �Ƚ϶���1
		   Param2 �Ƚ϶���2
		   Param3 �Ƚϲ���
**************************************************************/
int CALLBACK CDlgFaultEventAdd::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
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

void CDlgFaultEventAdd::OnColumnclickDlgFaultitemList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CDlgFaultEventAdd::FillSecType(CStationObj* pStation)
{
	//ѭ����վ�µı�����¼����
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
	return;
	CComboBox* pCmbDevice = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_DEVICE);
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SEC);
	//���г�վ
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	int nSec = pCmbDevice->GetCurSel();
	CString str;
	pCmbDevice->GetLBText(nSec,str);
	CString sStationID = "";
	if (pStation!=NULL)
	{
		sStationID = pStation->m_sID;
	}
	
	if (str != "("+StringFromID(IDS_COMMON_ALL)+")" && str != "")
	{
		CDeviceObj* pSec = (CDeviceObj*)pCmbDevice->GetItemData(nSec);
		CString sID = "";
		if (pSec != NULL)
		{
			sID = pSec->m_sID;
		}
		pData->GetSecList(listDevice, sID, TYPE_SEC, sStationID);
	}
	else
	{
		pData->GetSecList(listDevice, "", TYPE_SEC, sStationID);
	}
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj* pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		if(pObj->m_nType != TYPE_PROTECT && pObj->m_nType != TYPE_WAVEREC)
			continue;
		//���뵽������
		CString str;
		str = pObj->m_sName;
		int nIndex = pCmb->AddString(str);
		if(nIndex < 0)
			continue;
		//���ù���
		pCmb->SetItemData(nIndex, (DWORD)pObj);
	}
}

void CDlgFaultEventAdd::FillDevName(CStationObj* pStation)
{
	//ȡ���豸����������ָ��

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_DEVICE);
	if(pCmb == NULL)
		return;
	//���������
	pCmb->ResetContent();
	
	//�豸����
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	//ȡ�÷����������豸
	GetDevNameList(listDevice);
	//���豸���뵽������
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//����
		int nIndex = pCmb->AddString(pObj->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pObj);
		}
	}
	
	//�������
	listDevice.RemoveAll();	
	
	//����"����"ѡ��
	int nIndex = pCmb->AddString( "("+StringFromID(IDS_COMMON_ALL)+")" );
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	m_cmbPriDev.OnDataFilled();
}
//һ���豸
void CDlgFaultEventAdd::OnSelchangeDlgFaulteventCmbDevice() 
{
	// TODO: Add your control notification handler code here
	//ȡ�ó�վ
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
	FillSec(pStation);
}
//����
void CDlgFaultEventAdd::OnSelchangeDlgFaulteventCmbSectype() 
{
	// TODO: Add your control notification handler code here
	//ȡ�ó�վ
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
	FillDevName(pStation);
	FillSec(pStation);
}
/*************************************************************
 �� �� ����FillDevType()
 ���ܸ�Ҫ������豸�����������б�
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
void CDlgFaultEventAdd::FillDevType()
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SECTYPE);
	if(pCmb == NULL)
		return;
	//���ԭ������
	pCmb->ResetContent();
	//����"����"ѡ��
	pCmb->AddString( StringFromID(IDS_MODEL_BUS));
	pCmb->AddString( StringFromID(IDS_MODEL_LINE));
	pCmb->AddString( StringFromID(IDS_MODEL_BREAKER));
	pCmb->AddString( StringFromID(IDS_MODEL_GEN));
	pCmb->AddString( StringFromID(IDS_MODEL_CAP));
	pCmb->AddString( StringFromID(IDS_MODEL_REACTOR));
	pCmb->AddString( StringFromID(IDS_MODEL_TRANS));
	pCmb->AddString( StringFromID(IDS_MODEL_TRANS));
	pCmb->AddString( StringFromID(IDS_MODEL_RELAY));
	pCmb->AddString( StringFromID(IDS_MODEL_BAY));
	int nIndex = pCmb->AddString( "("+StringFromID(IDS_COMMON_ALL)+")" );
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);	
}
/*************************************************************
 �� �� ��: GetDevNameList()
 ���ܸ�Ҫ: ȡ�ø��ݳ�վ���豸���;�����һ���豸
 �� �� ֵ: 
 ��    ��: param1 ����һ���豸������
		   Param2 
**************************************************************/
void CDlgFaultEventAdd::GetDevNameList( DEVICE_LIST& listDevice )
{
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	CStationObj* pStation = NULL;
	CString sStationID = "";
//	int nType = -1;
	if(nSel > -1)
	{	
		pStation = (CStationObj*)pCmb->GetItemData(nSel);
		if (pStation != NULL)
		{
			sStationID = pStation->m_sID;
			
		}
	
	}
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SECTYPE);
	int nItem = pCmb->GetCurSel()+1;
	if (nItem>10||nItem<=0)
	{
		nItem = -1;
	}
	
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	pData->GetPriDeviceList(listDevice,nItem,sStationID);

	
}

/****************************************************
Date:2013/7/25  Author:LYH
������:   QueryDIChange	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString sSec	
����: CTime tmStart	
����: CTime tmEnd	
*****************************************************/
void CDlgFaultEventAdd::QueryDIChange( CString sSec, CTime tmStart, CTime tmEnd )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//�ֶ�
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "id", EX_STTP_DATA_TYPE_INT);
	
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "msg", EX_STTP_DATA_TYPE_STRING);
	
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "pt_id", EX_STTP_DATA_TYPE_STRING);
	
	Field Field31;
	pApp->m_DBEngine.SetField(sql, Field31, "cpu", EX_STTP_DATA_TYPE_INT);
	
	Field Field32;
	pApp->m_DBEngine.SetField(sql, Field32, "di_id", EX_STTP_DATA_TYPE_INT);
	
	Field Field4;
	pApp->m_DBEngine.SetField(sql, Field4, "is_ack", EX_STTP_DATA_TYPE_INT);

	Field Field41;
	pApp->m_DBEngine.SetField(sql, Field41, "value", EX_STTP_DATA_TYPE_INT);
	
	Field Field5;
	pApp->m_DBEngine.SetField(sql, Field5, "time", EX_STTP_DATA_TYPE_TIME);
	
	Field Field6;
	pApp->m_DBEngine.SetField(sql, Field6, "ms", EX_STTP_DATA_TYPE_INT);
		
	Field Field10;
	pApp->m_DBEngine.SetField(sql, Field10, "isdebug", EX_STTP_DATA_TYPE_INT);

	
	//����
	//PT_ID
	if(sSec != "")
	{
		Condition condition1;
		str.Format("pt_id='%s'", sSec);
		pApp->m_DBEngine.SetCondition(sql, condition1, str);
	}
	else
	{
		if( m_sPridevID != "" )
		{
			Condition condition1;
			str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_id='%s')", m_sPridevID);
			pApp->m_DBEngine.SetCondition(sql, condition1, str);
			
		}
		else
		{
			if( m_nPridevType != -1)
			{
				if(m_sStationID == "")
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_type=%d)", m_nPridevType);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
				else
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where primdev_type=%d and station_id='%s')", m_nPridevType, m_sStationID);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
			}
			else
			{
				if(m_sStationID != "")
				{
					Condition condition1;
					str.Format("pt_id IN (select pt_id from tb_secdev_base where station_id='%s')", m_sStationID);
					pApp->m_DBEngine.SetCondition(sql, condition1, str);
				}
			}
		}
	}
	
	//��ʼʱ��
	Condition con4;
	str.Format("time > '%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//����ʱ��
	Condition con5;
	str.Format("time < '%s'", tmEnd.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con5, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_CHANGE_DI_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDlgFaultEventAdd::QueryDIChange, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDlgFaultEventAdd::QueryDIChange, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventDI* pEvent = new CXJEventDI;
			if(pEvent->LoadFromDB(pMemset))
			{
				//�ֽ��¼�
				BreakDIChange(pEvent);
			}
			delete pEvent;
			pEvent = NULL;
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDlgFaultEventAdd::QueryDIChange,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return;
}

void CDlgFaultEventAdd::BreakDIChange( CXJEventDI* pEvent )
{
	//���������Ч��
	if(pEvent == NULL)
		return;
	
	//���������¼�
	CFaultEvent* pFaultEvent = new CFaultEvent;
	pFaultEvent->m_nType = XJ_FAULT_EVENT_DICHANGE; //���ϸ澯
	pFaultEvent->m_nMainRecID = m_nMainRecID;;//���ϰ�ID
	pFaultEvent->m_tmStart = pEvent->m_tmTime;
	pFaultEvent->m_nmsStart = pEvent->m_nms;
	pFaultEvent->m_tmReceiveTime = pEvent->m_tmTime;
	pFaultEvent->m_nmsReceive = pEvent->m_nms;
	pFaultEvent->m_pSec = pEvent->m_pSec;
	pFaultEvent->m_sEventContent.Format("%d", pEvent->m_nValue);
	pFaultEvent->m_sEventDef.Format("%d", pEvent->m_nDIID);
	pFaultEvent->m_sEventDefName = pEvent->m_sMsg;
	pFaultEvent->m_tmSign = pEvent->m_tmTime;
	pFaultEvent->m_nmsSign = pEvent->m_nms;
	pFaultEvent->m_nWaveFileNo = pEvent->m_nCPU;
	pFaultEvent->m_sSecID = pEvent->m_sSecID;
	//���뵽����
	m_listDIChange.Add(pFaultEvent);
}

