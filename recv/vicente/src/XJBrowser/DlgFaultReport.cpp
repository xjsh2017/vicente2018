
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
	//初始化数据
	//1.故障信息包名称:
	m_sName = m_pFaultReport->m_sName;
	//2.故障测距
	m_sDistance = m_pFaultReport->m_sFaultDistance;
	//3.故障时间
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULT_TIME);
	if(pTime != NULL)
	{
		//设置显示风格和范围
		pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
		//设置范围
		CTime tmMin(1971, 1, 1, 1, 1, 1);
		CTime tmMax(2036, 12, 12, 12, 12, 12);
		pTime->SetRange(&tmMin, &tmMax);
		//设置时间
		CTime tm = m_pFaultReport->m_tmFaultTime;
		VERIFY(pTime->SetTime(&tm));
	}
	//4.毫秒值
	m_sMs.Format("%d", m_pFaultReport->m_nmsFaultTime);
	//5.故障类型
	InitCMBFaultType();
	//6.厂站1
	InitCMBStation(m_pFaultReport->m_sStation1ID, IDC_DLG_FAULT_CMB_STATION1);
	//7.厂站2
	InitCMBStation(m_pFaultReport->m_sStation2ID, IDC_DLG_FAULT_CMB_STATION2);
	//8.故障元件
	InitCMBDevice();
	//9.是否存档
    InitCMBIsSaved();

	//3.故障时间
	pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULT_CREATE_TIME);
	if(pTime != NULL)
	{
		//设置显示风格和范围
		pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
		//设置范围
		CTime tmMin(1971, 1, 1, 1, 1, 1);
		CTime tmMax(2036, 12, 12, 12, 12, 12);
		pTime->SetRange(&tmMin, &tmMax);
		//设置时间
		CTime tm = m_pFaultReport->m_tmCreateTime;
		VERIFY(pTime->SetTime(&tm));
	}

	//10.TAB栏
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
		//还未载入详细信息, 现在载入详细信息
		m_pFaultReport->LoadDetail();
	}
	//13
	ReadDetail();
	//14.填充列表框
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
	//刷新显示
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 函 数 名：InitCMBFaultType()
 功能概要：初始化故障类型下拉框
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA4007E
BOOL CDlgFaultReport::InitCMBFaultType()
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULT_CMB_TYPE);
	if(pCmb == NULL)
		return FALSE;
	if(m_pFaultReport == NULL)
		return FALSE;
	//清空所有数据
	pCmb->ResetContent();
	//填充数据
	// 		0	未知
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_UNKNOWN));
	// 		1 A相接地
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_A));
	// 		2 B相接地
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_B));
	// 		3 C相接地
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_C));
	// 		4 AB两相短路
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_AB));
	// 		5 BC两相短路
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_BC));
	// 		6 CA两相短路
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_CA));
	// 		7 AB两相短路接地
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_ABN));
	// 		8 BC两相短路接地
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_BCN));
	// 		9 CA两相短路接地
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_CAN));
	// 		10 ABC三相短路
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_ABC));
	// 		11 ABC三相短路接地
	pCmb->AddString(StringFromID(IDS_FAULTTYPE_ABCN));
	//加入"所有"选项
	pCmb->AddString(StringFromID(IDS_COMMON_ALL));
	//设置选择
	int nCount = pCmb->GetCount();
	if(m_pFaultReport->m_nFaultType < nCount)
		pCmb->SetCurSel(m_pFaultReport->m_nFaultType);
	else
		pCmb->SetCurSel(0);
	return TRUE;
}

/*************************************************************
 函 数 名：InitCMBStation()
 功能概要：初始化厂站下拉框
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数：param1	预选择的厂站ID
		   Param2	下拉框ID
**************************************************************/
//##ModelId=49B87BA4007F
BOOL CDlgFaultReport::InitCMBStation( CString sID, UINT nID)
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(nID);
	if(pCmb == NULL)
		return FALSE;
	if(m_pFaultReport == NULL)
		return FALSE;
	//清空所有数据
	pCmb->ResetContent();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;

	//所有厂站
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();

	//插入厂站
	int nIndex = -1; //预选择的位置
	while(pos != NULL)
	{
		CStationObj* pStation = (CStationObj*)listDevice.GetNext(pos);
		if(pStation == NULL)
			continue;
		//加入到下拉框
		int nPos = pCmb->AddString(pStation->m_sName);
		if(nPos < 0)
			continue;
		//设置数据
		pCmb->SetItemData(nPos, (DWORD)pStation);
		//
		if(pStation->m_sID == sID)
			nIndex = nPos;
	}
	//插入空厂站
	int nNullPos = pCmb->AddString( StringFromID(IDS_COMMON_UNSPECIFIED));
	//选择
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
 函 数 名：InitCMBDevice()
 功能概要：初始化一次设备下拉框, 
 返 回 值: 初始化成功返回TRUE, 失败返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA4008E
BOOL CDlgFaultReport::InitCMBDevice()
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULT_CMB_DEVICE);
	if(pCmb == NULL)
		return FALSE;
	if(m_pFaultReport == NULL)
		return FALSE;
	//清空所有数据
	pCmb->ResetContent();

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return FALSE;

	//查找厂站1的对象指针
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
	//循环加入所有一次设备
	//线路
	FillSpecificPriDevice(pCmb, pStation, TYPE_LINE);
	//母线
	FillSpecificPriDevice(pCmb, pStation, TYPE_BUS);
	//变压器
	FillSpecificPriDevice(pCmb, pStation, TYPE_TRANS);
	//电容器
	FillSpecificPriDevice(pCmb, pStation, TYPE_CAP);
	//电抗器
	FillSpecificPriDevice(pCmb, pStation, TYPE_REACTOR);
	//发电机
	FillSpecificPriDevice(pCmb, pStation, TYPE_GEN);
	//开关
	FillSpecificPriDevice(pCmb, pStation, TYPE_BREAK);
	//刀闸
	FillSpecificPriDevice(pCmb, pStation, TYPE_SWITCH);

	CDeviceObj* pDevice = m_pFaultReport->m_pPriDevice;
	if(pDevice == NULL)
		return FALSE;

	//查找预选择的一次设备
	int nIndex = 0;
	int nCount = pCmb->GetCount();
	for(int i = 0; i < nCount; i++)
	{
		//比较
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
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_CMB_FAULT_SAVE);
	if(pCmb == NULL)
		return FALSE;
	if(m_pFaultReport == NULL)
		return FALSE;
	//清空所有数据
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
 函 数 名：FillSpecificPriDevice()
 功能概要：将指定厂站下的指定类型的一次设备填充到指定下拉框 
 返 回 值: void
 参    数：param1	下拉框指针
		   Param2	厂站指针
		   param3	一次设备类型
**************************************************************/
//##ModelId=49B87BA4009C
void CDlgFaultReport::FillSpecificPriDevice( CComboBox* pCmb, CStationObj* pStation, int nType)
{
	//检查数据有效性
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
		//加入到下拉框
		int nIndex = pCmb->AddString(pObj->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pObj);
		}
	}
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400AB
void CDlgFaultReport::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
		
	//分配列宽,最小需求
	for (int nCol=0; nCol < 5 ; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0://
			lvCol.cx = 300; // 二次设备
			colName = StringFromID(IDS_MODEL_SECONDARY);
			break;
		case 1://
			lvCol.cx = 160; // 启动时间
			colName = StringFromID(IDS_EVENT_STARTTIME);
			break;
		case 2://
			lvCol.cx = 180; // 事件描述
			colName = StringFromID(IDS_EVENT_DESC);
			break;
		case 3:
			lvCol.cx = 80; //事件值
			colName = StringFromID(IDS_EVENT_VALUE);
			break;
		case 4:
			lvCol.cx = 160; //接收时间
			colName = StringFromID(IDS_EVENT_RECIVETIME);
			break;
		default:
			lvCol.cx = 100;
			break;
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_List.InsertColumn(nCol,&lvCol);
	}

	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
}

/*************************************************************
 函 数 名：ClearDetail()
 功能概要：清除详细信息
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400AC
void CDlgFaultReport::ClearDetail()
{
	//清空各链表
	//动作特征量
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		delete pEvent;
	}
	m_listChr.RemoveAll();
	//动作信号量
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		delete pEvent;
	}
	m_listSign.RemoveAll();
	//告警事件
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		delete pEvent;
	}
	m_listAlarm.RemoveAll();
	//保护录波
	for(i = 0; i < m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listPTOSC.GetAt(i);
		delete pEvent;
	}
	m_listPTOSC.RemoveAll();
	//录波器录波
	for(i = 0; i < m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listWROSC.GetAt(i);
		delete pEvent;
	}
	m_listWROSC.RemoveAll();
	//开关量变位
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		delete pEvent;
	}
	m_listDIChange.RemoveAll();
	//小波测距
	for(i = 0; i < m_listDistance.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDistance.GetAt(i);
		delete pEvent;
	}
	m_listDistance.RemoveAll();
	//录波分析
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
 函 数 名：ReadDetail()
 功能概要：读入详细信息
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400AD
void CDlgFaultReport::ReadDetail()
{
	//动作特征量
	int i = 0;
	m_listChr.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listChr.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listChr.Add(pNew);
	}
	
	//动作信号量
	m_listSign.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listSign.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listSign.Add(pNew);
	}

	//告警事件
	m_listAlarm.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listAlarm.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listAlarm.Add(pNew);
	}
	
	//保护录波
	m_listPTOSC.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listPTOSC.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listPTOSC.Add(pNew);
	}
	
	//录波器录波
	m_listWROSC.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listWROSC.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listWROSC.Add(pNew);
	}

	//开关量变位
	m_listDIChange.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listDIChange.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listDIChange.Add(pNew);
	}

	//小波测距
	m_listDistance.RemoveAll();
	for(i = 0; i < m_pFaultReport->m_listWaveDistance.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pFaultReport->m_listWaveDistance.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_listDistance.Add(pNew);
	}

	//小波测距
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
 函 数 名：FillListData()
 功能概要：填充值到列表框
 返 回 值: 
 参    数：param1	要填充入的类型.0-动作特征值.1-动作信息号.2-告警.3-录波简报
		   Param2
**************************************************************/
//##ModelId=49B87BA400AE
void CDlgFaultReport::FillListData( int nType )
{
	//清空原来的值
	m_List.DeleteAllItems();
	//动作特征值
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
	//动作信号值
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
	//告警信号值
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
	//录波简报
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
	//开关量变位
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
 函 数 名：AddEventToList()
 功能概要：加入事件到列表
 返 回 值: 
 参    数：param1	事件指针
		   Param2	要插入的位置索引
**************************************************************/
//##ModelId=49B87BA400BC
void CDlgFaultReport::AddEventToList( CFaultEvent* pEvent, int nIndex /*= 0*/ )
{
	//检查数据有效性
	MYASSERT(pEvent);
	if(nIndex < 0)
		return;
	//"二次设备","启动时间","事件描述", "事件值", "接收时间"
	CString str = "";
	if(pEvent->m_pSec != NULL)
		str = pEvent->m_pSec->m_sName;
	//0:二次设备
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return;
	//1:启动时间
	str.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
	m_List.SetItemText(nIndex, 1, str);
	//2:事件描述
	if(pEvent->m_nType == 4)
		m_List.SetItemText(nIndex, 2, pEvent->m_sEventDef);
	else
		m_List.SetItemText(nIndex, 2, pEvent->m_sEventDefName);
	//3:事件值
	str = pEvent->m_sEventContent;
	if(pEvent->m_nType == XJ_FAULT_EVENT_SING || pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
	{
		//信号量
		int nContent = atoi(str);
		if(nContent == 1)
			str = StringFromID(IDS_CASE_ACTION);
		else if(nContent == 0)
			str = StringFromID(IDS_CASE_RETURN);
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
	{
		//特征值
		if(pEvent->m_strUnit != "" && pEvent->m_sEventContent != "")
			str.Format("%s(%s)", pEvent->m_sEventContent, pEvent->m_strUnit);
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_DICHANGE)
	{
		//信号量
		int nContent = atoi(str);
		if(nContent == 1)
			str = "ON";
		else if(nContent == 0)
			str = "OFF";
	}
	m_List.SetItemText(nIndex, 3, str);
	//4:接收时间
	str.Format("%s.%03d", pEvent->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsReceive);
	m_List.SetItemText(nIndex, 4, str);
	//5:信号量发生时间
	str.Format("%s.%03d", pEvent->m_tmSign.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsSign);
	m_List.SetItemText(nIndex, 5, str);

	//关联设备
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
 函 数 名：NeedSaveReport()
 功能概要：判断本故障信息包数据是否需要保存
 返 回 值: 需要保存返回TRUE, 不需要保存返回FALSE
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400CB
BOOL CDlgFaultReport::NeedSaveReport()
{
	BOOL bResult = FALSE;
	//得到界面最新值
	UpdateData(TRUE);
	//判断信息包名
	if(m_sName != m_pFaultReport->m_sName)
	{
		//改变值
		m_pFaultReport->m_sName = m_sName;
		bResult = TRUE;
	}
	//故障时间
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


	//毫秒值
	if (atoi(m_sMs) != m_pFaultReport->m_nmsFaultTime)
	{
		m_pFaultReport->m_nmsFaultTime = atoi(m_sMs);
		bResult = TRUE;
	}
	//故障元件
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
	//故障类型
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
	//故障测距
	if(m_sDistance != m_pFaultReport->m_sFaultDistance)
	{
		m_pFaultReport->m_sFaultDistance = m_sDistance;
		bResult = TRUE;
	}
	//厂站1
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
	//厂站2
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
	//是否存档
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
		//更新数据库
		if(!m_pFaultReport->SaveToDB())
		{
			//更新故障包失败
			AfxMessageBox( StringFromID(IDS_FAULTREPORT_UPDATE_FAIL));
			return;
		}
	    WriteLog("CDlgFaultReport::OnOK 数据保存结果成功\n");
	}
	int nSave = SaveEvent();
	if(nSave == 1) //删除时出错
	{
		//删除事件失败
		AfxMessageBox( StringFromID(IDS_FAULTEVENT_DELETE_FAIL));
		return;
	}
	else if(nSave == 2) //保存时出错
	{
		//保存事件失败
		AfxMessageBox( StringFromID(IDS_FAULTEVENT_UPDATE_FAIL));
		return;
	}
	//发送消息给窗口
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
	//无选择
	if(nCount < 1)
	{
		AfxMessageBox( StringFromID(IDS_TIP_SELECTITEM));
		return;
	}
	//记录要删除的事件
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
				//增加到要删除链表
				arrRemove.Add(pEvent);
			}
		}
	}

	//循环删除
	for(i = 0; i < arrRemove.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)arrRemove.GetAt(i);
		RemoveFromList(pEvent);
		RemoveFromArray(pEvent);
	}
	//清空
	arrRemove.RemoveAll();
	//设置需要保存标志
	m_bEventNeedSave = TRUE;
}

/*************************************************************
 函 数 名：RemoveFromList()
 功能概要：从列表框中移除事件
 返 回 值: 
 参    数：param1	指定要移除的事件
		   Param2
**************************************************************/
//##ModelId=49B87BA400CC
void CDlgFaultReport::RemoveFromList( CFaultEvent* pEvent )
{
	//检查数据有效性
	if(pEvent == NULL)
		return;

	//查找事件在列表中的位置
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pEvent;
	int nIndex = m_List.FindItem(&lvFind);

	//未找到
	if(nIndex < 0)
		return;

	//从列表框中移除
	m_List.DeleteItem(nIndex);
}

/*************************************************************
 函 数 名：RemoveFromArray()
 功能概要：从事件链表中删除事件
 返 回 值: void
 参    数：param1	指定要移除的事件
		   Param2
**************************************************************/
//##ModelId=49B87BA400CE
void CDlgFaultReport::RemoveFromArray( CFaultEvent* pEvent )
{
	//检查数据有效性
	if(pEvent == NULL)
		return;
	if(pEvent->m_nType == XJ_FAULT_EVENT_CHR)
	{
		//动作事件特征量
		for(int i = 0; i < m_listChr.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listChr.GetAt(i);
			if(pNew == pEvent)
			{
				//从链表中移除
				m_listChr.RemoveAt(i);
				//删除
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_SING)
	{
		//动作事件信号量
		for(int i = 0; i < m_listSign.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listSign.GetAt(i);
			if(pNew == pEvent)
			{
				//从链表中移除
				m_listSign.RemoveAt(i);
				//删除
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_ALARM)
	{
		//告警事件
		for(int i = 0; i < m_listAlarm.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listAlarm.GetAt(i);
			if(pNew == pEvent)
			{
				//从链表中移除
				m_listAlarm.RemoveAt(i);
				//删除
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_OSCREPORT)
	{
		//保护录波
		for(int i = 0; i < m_listPTOSC.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listPTOSC.GetAt(i);
			if(pNew == pEvent)
			{
				//从链表中移除
				m_listPTOSC.RemoveAt(i);
				//删除
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
		//录波器录波
		for(i = 0; i < m_listWROSC.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listWROSC.GetAt(i);
			if(pNew == pEvent)
			{
				//从链表中移除
				m_listWROSC.RemoveAt(i);
				//删除
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
	}
	if(pEvent->m_nType == XJ_FAULT_EVENT_DICHANGE)
	{
		//告警事件
		for(int i = 0; i < m_listDIChange.GetSize(); i++)
		{
			CFaultEvent* pNew = (CFaultEvent*)m_listDIChange.GetAt(i);
			if(pNew == pEvent)
			{
				//从链表中移除
				m_listDIChange.RemoveAt(i);
				//删除
				delete pEvent;
				pEvent = NULL;
				return;
			}
		}
	}
}

/*************************************************************
 函 数 名：SaveEvent()
 功能概要：保存故障事件
 返 回 值: 保存成功0,删除数据失败返回1,保存数据失败返回2
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA400DA
int CDlgFaultReport::SaveEvent()
{
	//判断是否需要保存
	if(!m_bEventNeedSave)
		return 0;
	if(m_pFaultReport == NULL)
		return 1;
	//删除旧信息
	m_pFaultReport->ClearDetail();
	//保存新信息
	//动作特征量
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listChr.Add(pNew);
	}
	
	//动作信号量
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listSign.Add(pNew);
	}
	
	//告警事件
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listAlarm.Add(pNew);
	}
	
	//保护录波
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
	
	//录波器录波
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

	//开关量变位
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listDIChange.Add(pNew);
	}

	//小波测距
	for(i = 0; i < m_listDistance.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listDistance.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listWaveDistance.Add(pNew);
	}
	
	//录波分析
	for(i = 0; i < m_listOSCParse.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSCParse.GetAt(i);
		CFaultEvent* pNew = pEvent->Clone();
		m_pFaultReport->m_listOSCParse.Add(pNew);
	}

	//保存到数据库
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
 函 数 名：UpdateList()
 功能概要：刷新列表框显示
 返 回 值: void
 参    数：param1
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
	//发送消息给通信服务器
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//检查通讯情况
	
	
	if(!pApp->GetSTTPStatus())
	{
		AfxMessageBox(STTP_DISCONNECT);
		return;
	}

	
	//申请报文数据容器
	STTP_FULL_DATA sttpData;
	
	//组织20162报文
	if(!pApp->m_SttpEngine.BuildDataFor20162RenameOscFile(sttpData, pEvent))
	{
		//组织报文失败
		CString str;
		str.Format("组建录波文件重命令名要求报文失败,故障归档包ID:%d, 故障事件ID:%d", pEvent->m_nMainRecID, pEvent->m_nID);
		WriteLog(str);
		return;
	}
	
	//发送报文
	int nReturn = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	//判断是否成功
	if(nReturn != 1)
	{
		CString str;
		str.Format("发送录波文件重命令名要求报文失败,故障归档包ID:%d, 故障事件ID:%d", pEvent->m_nMainRecID, pEvent->m_nID);
		WriteLog(str);
		return;
	}
}


void CDlgFaultReport::OnColumnclickDlgFaultList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//排序所点击的列
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//颠倒顺序排列
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
 函 数 名: CompareFunction()
 功能概要: 比较函数,排序时被调用
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数: param1 比较对象1
		   Param2 比较对象2
		   Param3 比较参数
**************************************************************/
int CALLBACK CDlgFaultReport::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//取得比较数据
	CFaultEvent* pEvent1 = (CFaultEvent*)lParam1;
	CFaultEvent* pEvent2 = (CFaultEvent*)lParam2;
	if(pEvent1 == NULL || pEvent2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"二次设备","启动时间","事件描述", "事件值", "接收时间"
	switch(nCol)
	{
	case 0:	//二次设备
		str1 = pEvent1->m_pSec->m_sName;
		str2 = pEvent2->m_pSec->m_sName;
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;

	case 1: //启动时间
		str1.Format("%s.%03d", pEvent1->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nmsStart);
		str2.Format("%s.%03d", pEvent2->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nmsStart);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);

		break;
	case 2: //事件描述
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
	case 3: //事件值
		str1 = pEvent1->m_sEventContent;
		if(pEvent1->m_nType == XJ_FAULT_EVENT_SING || pEvent1->m_nType == XJ_FAULT_EVENT_ALARM)
		{
			//信号量
			int nContent = atoi(str1);
			if(nContent == 1)
				str1 = StringFromID(IDS_CASE_ACTION);
			else if(nContent == 0)
				str1 = StringFromID(IDS_CASE_RETURN);
		}
		if(pEvent1->m_nType == XJ_FAULT_EVENT_CHR)
		{
			//特征值
			if(pEvent1->m_strUnit != "" && pEvent1->m_sEventContent != "")
				str1.Format("%s(%s)", pEvent1->m_sEventContent, pEvent1->m_strUnit);
		}
		if(pEvent2->m_nType == XJ_FAULT_EVENT_SING || pEvent2->m_nType == XJ_FAULT_EVENT_ALARM)
		{
			//信号量
			int nContent = atoi(str2);
			if(nContent == 1)
				str2 = StringFromID(IDS_CASE_ACTION);
			else if(nContent == 0)
				str2 = StringFromID(IDS_CASE_RETURN);
		}
		if(pEvent2->m_nType == XJ_FAULT_EVENT_CHR)
		{
			//特征值
			if(pEvent2->m_strUnit != "" && pEvent2->m_sEventContent != "")
				str2.Format("%s(%s)", pEvent2->m_sEventContent, pEvent2->m_strUnit);
		}

		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);

		break;
	case 4: //接收时间
		str1.Format("%s.%03d", pEvent1->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nmsReceive);
		str2.Format("%s.%03d", pEvent2->m_tmReceiveTime.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nmsReceive);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;
	case 5: //信号量发生时间
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
