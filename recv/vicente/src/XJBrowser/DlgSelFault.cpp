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

	//设置时间
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

	//填充下拉框
	FillNet(); //地区
	FillStation(); //厂站
	FillDeviceType(); //设备类型
	FillPriDevice(NULL, NULL, 0);//一次设备

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
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_NET);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加载新数据
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
		//加入到下拉框
		int nIndex = pCmb->AddString(/*pNet->m_sID*/pNet->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pNet);
		}
	}
	//加入"所有"选项
	int nIndex = pCmb->AddString( StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
}

void CDlgSelFault::FillStation(CNetObj* pNet /*= NULL*/)
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_STATION);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加载新数据
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
		//加入到下拉框
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
	//加入"所有"选项
	int nIndex = pCmb->AddString( StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	m_cmbStation.OnDataFilled();
}

void CDlgSelFault::FillDeviceType()
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_DEVICE_TYPE);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	//加入到下拉框
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
	//检查数据有效性
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_CMB_FAULT_ONE);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	
	//填充新数据
	if(pStation != NULL)
	{
		if (nDeviceType != 0)
		{
			FillPriDeviceByType(pCmb, pStation, nDeviceType, nVoltage);
		}
		else
		{
			//指定厂站
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
		
		//所有厂站
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
				//加入到下拉框
				FillAllPriDevice(pCmb, pStation1, nDeviceType, nVoltage);
			}
		}
	}
	
	//加入"所有"选项
	int nIndex = pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	
	m_cmbPriDev.OnDataFilled();
}

void CDlgSelFault::FillAllPriDevice(CComboBox* pCmb, CStationObj* pStation, int nDeviceType, int nVoltage)
{
	//线路
	FillSpecificPriDevice(pCmb, pStation, TYPE_LINE, nVoltage);
	//母线
	FillSpecificPriDevice(pCmb, pStation, TYPE_BUS, nVoltage);
	//变压器
	FillSpecificPriDevice(pCmb, pStation, TYPE_TRANS, nVoltage);
	//电容器
	FillSpecificPriDevice(pCmb, pStation, TYPE_CAP, nVoltage);
	//电抗器
	FillSpecificPriDevice(pCmb, pStation, TYPE_REACTOR, nVoltage);
	//发电机
	FillSpecificPriDevice(pCmb, pStation, TYPE_GEN, nVoltage);
	//开关
	FillSpecificPriDevice(pCmb, pStation, TYPE_BREAK, nVoltage);
	//刀闸
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
	//检查数据有效性
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
			//加入到下拉框
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
	
	//厂站
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
	//一次设备
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

	//组织查询条件
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	ASSERT(pApp);
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	//条件
	CString str = "";
	
	CString sSelNet="";
	//地区
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


	//厂站
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
	
	//设备类型
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

	//一次设备
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
		//一次设备
		Condition con2;
		if(sSelPriID != "")
		{
			str.Format("PRIDEVICEID='%s'", sSelPriID);
			pApp->m_DBEngine.SetCondition(sql, con2, str);
		}
	}
	else
	{
		//厂站
		Condition con1;
		if(sSelStationID != "")
		{
			str.Format("(STATION1_ID='%s' or STATION2_ID='%s')", sSelStationID, sSelStationID);
			pApp->m_DBEngine.SetCondition(sql, con1, str);
		}
	}

	//起始时间
	CTime tmSelStart;
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_FAULT_TIME1);
	if(pTime != NULL)
	{
		pTime->GetTime(tmSelStart);
	}
	//结束时间
	CTime tmSelEnd;
	pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_FAULT_TIME2);
	if(pTime != NULL)
	{
		pTime->GetTime(tmSelEnd);
	}
	//起始时间
	Condition con4;
	str.Format("FAULTTIME > '%s'", tmSelStart.Format("%Y%m%d%H%M%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//结束时间
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
		WriteLog("OnSelfaultQuery::QueryFaultReport, 查询失败");
		return;
	}
	if(nResult == 1)
	{
		pMemset.MoveFirst();
		int nCount = pMemset.GetMemRowNum();
		for(int i = 0; i < nCount; i++)
		{
			//创建对象
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
		str.Format("CViewFaultQuery::QueryFaultReport,查询失败,原因为%s", sError);
		WriteLog(str);
		return;
	}

	FillListData();
	
}

int CDlgSelFault::InitListCtrl()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	//先查找UIConfig配置文件中是否有此列表的列信息
	//如果有,按配置文件的信息来设置列
	//如果没有,按默认来设置列,并保存到配置文件
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	if(1)
	{		
		//分配列宽,最小需求
		for (int nCol=0; nCol < 8 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0:
				lvCol.cx = 60; //序号
				colName = StringFromID( IDS_COMMON_NO );
				break;
			case 1://
				lvCol.cx = 240; // 事故包名称
				colName = StringFromID( IDS_FAULTREPORT_NAME );
				break;
			case 2://
				lvCol.cx = 150; // 创建时间
				colName = StringFromID(IDS_COMMON_CREATETIME);
				break;
			case 3:
				lvCol.cx = 150; // 故障时间
				colName = StringFromID(IDS_FAULT_TIME);
				break;
			case 4://
				lvCol.cx = 180; // 故障元件
				colName = StringFromID(IDS_FAULT_DEVICE);
				break;
			case 5://
				lvCol.cx = 100; // 厂站1
				colName = StringFromID(IDS_FAULT_STATION1);
				break;
			case 6:
				lvCol.cx = 100; //厂站2
				colName = StringFromID(IDS_FAULT_STATION2);
				break;
			case 7:
				lvCol.cx = 100; //故障类型
				colName = StringFromID(IDS_FAULT_TYPE);
				break;
			default:
				lvCol.cx=90;//列宽100象素
			}
			lvCol.pszText=colName.GetBuffer(1);
			m_List.InsertColumn(nCol,&lvCol);
		}
		//默认隐藏第一列(序号)
		m_List.SetColumnHide(0, TRUE);
	}
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
}

void CDlgSelFault::FillListData()
{
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//删除列表数据
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
	
	//恢复刷新
	m_List.SetRedraw(TRUE);
}

void CDlgSelFault::AddReportToList(CFaultReport* pReport, int nIndex)
{
	//检查数据有效性
	if(pReport == NULL)
		return;
	if(nIndex < 0)
		return;
	
	CString str;
	//0:序号
	str.Format("%d", pReport->m_nID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1:事故包名称
	m_List.SetItemText(nIndex, 1, pReport->m_sName);
	//2:创建时间
	str = pReport->m_tmCreateTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 2, str);
	//3:故障时间
	str = pReport->m_tmFaultTime.Format("%Y-%m-%d %H:%M:%S");
	m_List.SetItemText(nIndex, 3, str);
	//4:故障元件
	if(pReport->m_pPriDevice != NULL)
	{
		str = pReport->m_pPriDevice->m_sName;
	}
	else
		str = "";
	m_List.SetItemText(nIndex, 4, str);
	//5:厂站1
	str = "";
	if(pReport->m_pStation1 != NULL)
	{
		str = pReport->m_pStation1->m_sName;
	}
	m_List.SetItemText(nIndex, 5, str);
	//6:厂站2
	str = "";
	if(pReport->m_pStation2 != NULL)
	{
		str = pReport->m_pStation2->m_sName;
	}
	m_List.SetItemText(nIndex, 6, str);
	//7:故障类型
	str = pReport->GetFaultType();
	m_List.SetItemText(nIndex, 7, str);
	
	//关联到DATA
	m_List.SetItemData(nIndex, (DWORD)pReport);
}

void CDlgSelFault::Clear()
{
	//清空链表
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
	//无选择
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
