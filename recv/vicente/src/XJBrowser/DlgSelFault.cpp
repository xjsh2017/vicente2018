// DlgSelFault.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgSelFault.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelFault dialog


CDlgSelFault::CDlgSelFault(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelFault::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelFault)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSec = NULL;

	//����ʱ��
	CTime tm = CTime::GetCurrentTime();
	m_endTime = tm;
	CTimeSpan ts(30, 0, 0, 0);
	m_startTime = tm - ts;
	m_pFault = NULL;
}

CDlgSelFault::~CDlgSelFault()
{
	Clear();
}

void CDlgSelFault::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelFault)
	DDX_Control(pDX, IDC_LIST_FAULT, m_List);
	DDX_Control(pDX, IDC_CMB_FAULT_ONE, m_cmbPriDev);
	DDX_Control(pDX, IDC_CMB_FAULT_STATION, m_cmbStation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelFault, CDialog)
	//{{AFX_MSG_MAP(CDlgSelFault)
	ON_CBN_SELCHANGE(IDC_CMB_FAULT_NET, OnSelchangeCmbFaultNet)
	ON_CBN_SELCHANGE(IDC_CMB_FAULT_DEVICE_TYPE, OnSelchangeCmbFaultDeviceType)
	ON_CBN_SELCHANGE(IDC_CMB_FAULT_STATION, OnSelchangeCmbFaultStation)
	ON_BN_CLICKED(IDB_SELFAULT_QUERY, OnSelfaultQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelFault message handlers

BOOL CDlgSelFault::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//���������
	FillNet(); //����
	FillStation(); //��վ
	FillDeviceType(); //�豸����
	FillPriDevice(NULL, NULL, 0);//һ���豸

	AutoselBySec();

	CTime tmMin(1971, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_FAULT_TIME1);
	if(pTime != NULL)
	{
		pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
		pTime->SetRange(&tmMin, &tmMax);
		pTime->SetTime(&m_startTime);
	}
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DP_FAULT_TIME2);
	if(pTime2 != NULL)
	{
		pTime2->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
		pTime2->SetRange(&tmMin, &tmMax);
		pTime2->SetTime(&m_endTime);
	}
	
	InitListCtrl();

	OnSelfaultQuery();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelFault::FillNet()
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_NET);
	if(pCmb == NULL)
		return;
	//���ԭ������
	pCmb->ResetContent();
	//����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	POSITION pos = pData->m_NetList.GetHeadPosition();
	while(pos != NULL)
	{
		CNetObj* pNet = (CNetObj*)pData->m_NetList.GetNext(pos);
		if(pNet == NULL)
			continue;
		//���뵽������
		int nIndex = pCmb->AddString(/*pNet->m_sID*/pNet->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pNet);
		}
	}
	//����"����"ѡ��
	int nIndex = pCmb->AddString( StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
}

void CDlgSelFault::FillStation(CNetObj* pNet /*= NULL*/)
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	if(pCmb == NULL)
		return;
	//���ԭ������
	pCmb->ResetContent();
	//����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
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
		int nIndex;
		if (pNet == NULL || pNet != NULL && !pNet->m_sID.CompareNoCase(pStation->m_sNetID))
		{
			nIndex = pCmb->AddString(pStation->m_sName);
			if (pNet != NULL)
			{
				FillPriDevice(pStation, NULL, 0);
			}
			if(nIndex >= 0)
			{
				pCmb->SetItemData(nIndex, (DWORD)pStation);
			}
		}
	}
	//����"����"ѡ��
	int nIndex = pCmb->AddString( StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	m_cmbStation.OnDataFilled();
}

void CDlgSelFault::FillDeviceType()
{
	//���������Ч��
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	if(pCmb == NULL)
		return;
	//���ԭ������
	pCmb->ResetContent();
	//����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	//���뵽������
	int nIndex = pCmb->AddString( StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	pCmb->AddString( StringFromID(IDS_MODEL_BUS));
    pCmb->AddString( StringFromID(IDS_MODEL_LINE));
    pCmb->AddString( StringFromID(IDS_MODEL_BREAKER));
    pCmb->AddString( StringFromID(IDS_MODEL_GEN));
	pCmb->AddString( StringFromID(IDS_MODEL_REACTOR));
    pCmb->AddString( StringFromID(IDS_MODEL_TRANS));
    pCmb->AddString( StringFromID(IDS_MODEL_SWITCH));
    pCmb->AddString( StringFromID(IDS_MODEL_CAP));
}

void CDlgSelFault::FillPriDevice(CStationObj* pStation, int nVoltage, int nDeviceType)
{
	//���������Ч��
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_ONE);
	if(pCmb == NULL)
		return;
	//���ԭ������
	pCmb->ResetContent();
	
	//���������
	if(pStation != NULL)
	{
		if (nDeviceType != 0)
		{
			FillPriDeviceByType(pCmb, pStation, nDeviceType, nVoltage);
		}
		else
		{
			//ָ����վ
			FillAllPriDevice(pCmb, pStation, nDeviceType, nVoltage);
		}
	}
	else
	{
		CComboBox* pCmbStation = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
		if (pCmbStation == NULL)
		{
			return;
		}
		
		//���г�վ
		for (int i = 0; i < pCmbStation->GetCount(); i++)
		{
			CStationObj* pStation1 = (CStationObj*)pCmbStation->GetItemData(i);
			if(pStation1 == NULL)
				continue;
			if (nDeviceType != 0)
			{
				FillPriDeviceByType(pCmb, pStation1, nDeviceType, nVoltage);
			}
			else
			{
				//���뵽������
				FillAllPriDevice(pCmb, pStation1, nDeviceType, nVoltage);
			}
		}
	}
	
	//����"����"ѡ��
	int nIndex = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	
	m_cmbPriDev.OnDataFilled();
}

void CDlgSelFault::FillAllPriDevice(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage)
{
	//��·
	FillSpecificPriDevice(pCmb, pStation, TYPE_LINE, nVoltage);
	//ĸ��
	FillSpecificPriDevice(pCmb, pStation, TYPE_BUS, nVoltage);
	//��ѹ��
	FillSpecificPriDevice(pCmb, pStation, TYPE_TRANS, nVoltage);
	//������
	FillSpecificPriDevice(pCmb, pStation, TYPE_CAP, nVoltage);
	//�翹��
	FillSpecificPriDevice(pCmb, pStation, TYPE_REACTOR, nVoltage);
	//�����
	FillSpecificPriDevice(pCmb, pStation, TYPE_GEN, nVoltage);
	//����
	FillSpecificPriDevice(pCmb, pStation, TYPE_BREAK, nVoltage);
	//��բ
	FillSpecificPriDevice(pCmb, pStation, TYPE_SWITCH, nVoltage);
}

void CDlgSelFault::FillPriDeviceByType(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage)
{
	switch(nDeviceType)
	{
	case 1:
		FillSpecificPriDevice(pCmb, pStation, 1, nVoltage);
		break;
	case 2:
		FillSpecificPriDevice(pCmb, pStation, 2, nVoltage);
		break;
	case 3:
		FillSpecificPriDevice(pCmb, pStation, 3, nVoltage);
		break;
	case 4:
		FillSpecificPriDevice(pCmb, pStation, 4, nVoltage);
		break;
	case 5:
		FillSpecificPriDevice(pCmb, pStation, 5, nVoltage);
		break;
	case 6:
		FillSpecificPriDevice(pCmb, pStation, 6, nVoltage);
		break;
	case 7:
		FillSpecificPriDevice(pCmb, pStation, 7, nVoltage);
		break;
	case 8:
		FillSpecificPriDevice(pCmb, pStation, 8, nVoltage);
		break;
	}
}

void CDlgSelFault::FillSpecificPriDevice(CComboBox* pCmb, CStationObj* pStation, int nType, int nVoltage)
{
	//���������Ч��
	if(pCmb == NULL)
		return;
	if(pStation == NULL)
		return;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetPriDeviceList(listDevice, nType, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		if (nVoltage == 0 || (nVoltage != 0 && pObj->m_nVoltage == nVoltage))
		{	
			//���뵽������
			//if (pObj->m_bHasSecDevice)
			//{
			int nIndex = pCmb->AddString(pObj->m_sName);
			if(nIndex >= 0)
			{
				pCmb->SetItemData(nIndex, (DWORD)pObj);
			}
			//}
		}
	}
}

void CDlgSelFault::OnSelchangeCmbFaultNet() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_NET);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	CNetObj* pNet = (CNetObj*)pCmb->GetItemData(nSel);
	
	
	CComboBox* pCmbStation = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	if (pCmbStation == NULL)
	{
		return;
	}
	int nSelStation = pCmbStation->GetCurSel();
	if (nSelStation < 0)
	{
		return;
	}
	CStationObj* pStation = (CStationObj*)pCmbStation->GetItemData(nSelStation);
	
	CComboBox* pCmbDeviceType= (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	if (pCmbDeviceType == NULL)
	{
		return;
	}
	int nSelDeviceType = pCmbDeviceType->GetCurSel();
	
	CComboBox* pCmbDevice = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_ONE);
	if (pCmbDevice == NULL)
	{
		return;
	}
	int nSelDevice = pCmbDevice->GetCurSel();
	CDeviceObj* pDevice = (CDeviceObj*)pCmbDevice->GetItemData(nSelDevice);
	
	//��վ
	FillStation(pNet);
	FillPriDevice(pStation, NULL, nSelDeviceType);
}

void CDlgSelFault::OnSelchangeCmbFaultDeviceType() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	
    CComboBox* pCmbStation = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	int nSelStation = pCmbStation->GetCurSel();
	if(nSelStation < 0)
		return;
    CStationObj* pStation = (CStationObj*)pCmbStation->GetItemData(nSelStation);
	
	FillPriDevice(pStation, NULL, nSel);
}


void CDlgSelFault::OnSelchangeCmbFaultStation() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
	//һ���豸
	FillPriDevice(pStation, NULL, 0);
}

void CDlgSelFault::AutoselBySec()
{
	if( NULL == m_pSec )
		return;
	if( !m_pSec->m_sStationID.IsEmpty() )
	{
		CComboBox* pCmbStation = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
		for( int i = 0; i < pCmbStation->GetCount(); i++ )
		{
			CStationObj* station = (CStationObj*)pCmbStation->GetItemData(i);
			if( NULL != station && station->m_sID == m_pSec->m_sStationID)
			{
				pCmbStation->SetCurSel(i);
				break;
			}
		}
	}
	OnSelchangeCmbFaultStation();

	if( !m_pSec->m_sOwnerID.IsEmpty() )
	{
		CComboBox* pCmbDevice = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_ONE);
		for( int k = 0; k < pCmbDevice->GetCount(); k++ )
		{
			CDeviceObj* dev = (CDeviceObj*)pCmbDevice->GetItemData(k);
			if( NULL != dev && dev->m_sID == m_pSec->m_sOwnerID )
			{
				pCmbDevice->SetCurSel(k);
				break;
			}
		}
	}
	
}

void CDlgSelFault::OnSelfaultQuery() 
{
	// TODO: Add your control notification handler code here

	Clear();
	m_List.DeleteAllItems();

	//��֯��ѯ����
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	ASSERT(pApp);
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	//����
	CString str = "";
	
	CString sSelNet="";
	//����
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_NET);
	if(pCmb != NULL)
	{
		int nSel = pCmb->GetCurSel();
		if(nSel >= 0)
		{
			CNetObj* pNet = (CNetObj*)pCmb->GetItemData(nSel);
			if(pNet != NULL)
				sSelNet = pNet->m_sNote;
		}
	}


	//��վ
	CString sSelStationID = "";
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	if(pCmb != NULL)
	{
		int nSel = pCmb->GetCurSel();
		if(nSel >= 0)
		{
			CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
			if(pStation != NULL)
				sSelStationID = pStation->m_sID;
		}
	}
	
	//�豸����
	int nSelDeviceType = 0;
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	if(pCmb != NULL)
	{
		int nSel = pCmb->GetCurSel();
		if(nSel >= 0)
		{
			nSelDeviceType = nSel;
		}
	}

	//һ���豸
	CString sSelPriID="";
	pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_ONE);
	if(pCmb != NULL)
	{
		int nSel = pCmb->GetCurSel();
		if(nSel >= 0)
		{
			CDeviceObj* pObj = (CDeviceObj*)pCmb->GetItemData(nSel);
			if(pObj != NULL)
				sSelPriID = pObj->m_sID;
		}
	}
	
	
	if ( !sSelPriID.IsEmpty() )
	{
		//һ���豸
		Condition con2;
		if(sSelPriID != "")
		{
			str.Format("PRIDEVICEID='%s'", sSelPriID);
			pApp->m_DBEngine.SetCondition(sql, con2, str);
		}
	}
	else
	{
		//��վ
		Condition con1;
		if(sSelStationID != "")
		{
			str.Format("(STATION1_ID='%s' or STATION2_ID='%s')", sSelStationID, sSelStationID);
			pApp->m_DBEngine.SetCondition(sql, con1, str);
		}
	}

	//��ʼʱ��
	CTime tmSelStart;
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_FAULT_TIME1);
	if(pTime != NULL)
	{
		pTime->GetTime(tmSelStart);
	}
	//����ʱ��
	CTime tmSelEnd;
	pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_FAULT_TIME2);
	if(pTime != NULL)
	{
		pTime->GetTime(tmSelEnd);
	}
	//��ʼʱ��
	Condition con4;
	str.Format("FAULTTIME > '%s'", tmSelStart.Format("%Y%m%d%H%M%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//����ʱ��
	Condition con5;
	str.Format("FAULTTIME < '%s'", tmSelEnd.Format("%Y%m%d%H%M%S"));
	pApp->m_DBEngine.SetCondition(sql, con5, str);
	
	
	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError, &pMemset);
	}
	catch (...)
	{
		WriteLog("OnSelfaultQuery::QueryFaultReport, ��ѯʧ��");
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		for(int i = 0; i < nCount; i++)
		{
			//��������
			CFaultReport* pReport = new CFaultReport();
			if( NULL == pReport )
			{
				continue;
			}
			if(!pReport->LoadFromDB(&pMemset))
			{
				delete pReport;
			}
			else
			{
				m_listFaultReport.AddTail(pReport);
			}
			
			pMemset.MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CViewFaultQuery::QueryFaultReport,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
		return;
	}

	FillListData();
	
}

int CDlgSelFault::InitListCtrl()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	//�Ȳ���UIConfig�����ļ����Ƿ��д��б������Ϣ
	//�����,�������ļ�����Ϣ��������
	//���û��,��Ĭ����������,�����浽�����ļ�
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	if(1)
	{		
		//�����п�,��С����
		for (int nCol=0; nCol < 8 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 60; //���
				colName = StringFromID( IDS_COMMON_NO );
				break;
			case 1://
				lvCol.cx = 240; // �¹ʰ�����
				colName = StringFromID( IDS_FAULTREPORT_NAME );
				break;
			case 2://
				lvCol.cx = 150; // ����ʱ��
				colName = StringFromID(IDS_COMMON_CREATETIME);
				break;
			case 3:
				lvCol.cx = 150; // ����ʱ��
				colName = StringFromID(IDS_FAULT_TIME);
				break;
			case 4://
				lvCol.cx = 180; // ����Ԫ��
				colName = StringFromID(IDS_FAULT_DEVICE);
				break;
			case 5://
				lvCol.cx = 100; // ��վ1
				colName = StringFromID(IDS_FAULT_STATION1);
				break;
			case 6:
				lvCol.cx = 100; //��վ2
				colName = StringFromID(IDS_FAULT_STATION2);
				break;
			case 7:
				lvCol.cx = 100; //��������
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			default:
				lvCol.cx=90;//�п�100����
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		m_List.SetColumnHide(0, TRUE);
	}
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

void CDlgSelFault::FillListData()
{
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	
	POSITION pos = m_listFaultReport.GetHeadPosition();
	int nIndex = 0;
	while(pos != NULL)
	{
		CFaultReport* pReport = (CFaultReport*)m_listFaultReport.GetNext(pos);
		ASSERT(pReport);
		AddReportToList(pReport, nIndex);
		nIndex++;
	}
	
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
}

void CDlgSelFault::AddReportToList(CFaultReport* pReport, int nIndex)
{
	//���������Ч��
	if(pReport == NULL)
		return;
	if(nIndex < 0)
		return;
	
	CString str;
	//0:���
	str.Format("%d", pReport->m_nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1:�¹ʰ�����
	m_List.SetItemText(nIndex, 1, pReport->m_sName);
	//2:����ʱ��
	str = pReport->m_tmCreateTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 2, str);
	//3:����ʱ��
	str = pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 3, str);
	//4:����Ԫ��
	if(pReport->m_pPriDevice != NULL)
	{
		str = pReport->m_pPriDevice->m_sName;
	}
	else
		str = "";
	m_List.SetItemText(nIndex, 4, str);
	//5:��վ1
	str = "";
	if(pReport->m_pStation1 != NULL)
	{
		str = pReport->m_pStation1->m_sName;
	}
	m_List.SetItemText(nIndex, 5, str);
	//6:��վ2
	str = "";
	if(pReport->m_pStation2 != NULL)
	{
		str = pReport->m_pStation2->m_sName;
	}
	m_List.SetItemText(nIndex, 6, str);
	//7:��������
	str = pReport->GetFaultType();
	m_List.SetItemText(nIndex, 7, str);
	
	//������DATA
	m_List.SetItemData(nIndex, (DWORD)pReport);
}

void CDlgSelFault::Clear()
{
	//�������
	POSITION pos = m_listFaultReport.GetHeadPosition();
	while(pos != NULL)
	{
		CFaultReport* pReport = (CFaultReport*)m_listFaultReport.GetNext(pos);
		if (pReport!=NULL)
		{
			delete pReport;
			pReport = NULL;
		}
		
	}
	m_listFaultReport.RemoveAll();
}

void CDlgSelFault::OnOK() 
{
	// TODO: Add extra validation here
	int nCount = m_List.GetSelectedCount();
	//��ѡ��
	if(nCount != 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECT_ONEITEM));
		return;
	}
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex != -1)
	{
		CFaultReport* report = (CFaultReport*)m_List.GetItemData(nIndex);
		m_pFault = report;
	}

	CDialog::OnOK();
}
