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
	//填充厂站下拉框
	FillStation();
	//填充一次设备类型
	FillDevType();

	//填充二次设备下拉框
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	MYASSERT_BOOL(pCmb);
	int nIndex = pCmb->GetCurSel();
	if(nIndex < 0)
		return FALSE;
	CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nIndex);
	//填充一次设备
	FillDevName(pStation);
	//填充二次设备
	FillSec(pStation);
	//设置时间格式
	SetTimeCtrl();
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_EVENTTYPE);
	MYASSERT_BOOL(pCmb);
	pCmb->InsertString(0, StringFromID(IDS_EVENTTYPE_ACTION));
	pCmb->InsertString(1, StringFromID(IDS_EVENTTYPE_ALARM));
	pCmb->InsertString(2, StringFromID(IDS_EVENTTYPE_OSC));
	pCmb->InsertString(3, StringFromID(IDS_EVENTTYPE_DI));
	pCmb->InsertString(4, StringFromID(IDS_COMMON_ALL));
	pCmb->SetCurSel(0);
	//8.TAB栏
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
	//初始化列表框风格
	InitListStyle();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 函 数 名：FillStation()
 功能概要：填充厂站下拉框
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40168
void CDlgFaultEventAdd::FillStation()
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//得到父窗口加载的厂站
	CDlgFaultReport* pParent = (CDlgFaultReport*)GetParent();
	CStationObj* pParentStation = NULL;
	int nParentIndex = -1;
	if(pParent != NULL)
	{
		if(pParent->m_pFaultReport != NULL)
		{
			//记录厂站指针
			if(pParent->m_pFaultReport->m_pStation1 != NULL)
				pParentStation = pParent->m_pFaultReport->m_pStation1;
			//记录故障信息包ID
			m_nMainRecID = pParent->m_pFaultReport->m_nID;
		}
	}
	//所有厂站
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
		int nIndex = pCmb->AddString(pStation->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pStation);
			if(pParentStation == pStation)
			{
				//记录位置
				nParentIndex = nIndex;
			}
		}
	}
	//加入"所有"选项
	int nIndex = pCmb->AddString("("+StringFromID(IDS_COMMON_ALL)+")");
	//设置预选择项
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
 函 数 名：FillSec()
 功能概要：填充指定厂站下的二次设备到下拉框
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40169
void CDlgFaultEventAdd::FillSec(CStationObj* pStation)
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SEC);
	//CComboBox* pCmb2 = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SECTYPE);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();

	if(pStation != NULL)
	{
		//指定了厂站
	//	FillSecType(pStation);
		FillSpecificSec(pStation, pCmb);
	}
	else
	{
		//所有厂站
	//	FillSecType(pStation);
		FillSpecificSec(NULL, pCmb);
	}
	//加入"所有"选项
	int nIndex = pCmb->AddString("("+StringFromID(IDS_COMMON_ALL)+")");
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	m_cmbSec.OnDataFilled();
}

/*************************************************************
 函 数 名：FillSpecificSec()
 功能概要：填充指定厂站下的二次设备到指定下拉框
 返 回 值: void
 参    数：param1	指定厂站,NULL表示所有厂站
		   Param2	指定下拉框
**************************************************************/
//##ModelId=49B87BA40178
void CDlgFaultEventAdd::FillSpecificSec( CStationObj* pStation, CComboBox* pCmb )
{
	//检查数据有效性
	if(pCmb == NULL)
		return;
	CString sStationID = "";
	if(pStation != NULL)
		sStationID = pStation->m_sID;

	//循环厂站下的保护和录波器
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	CComboBox* pCmbSec = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_DEVICE);
	if (pCmbSec==NULL)
		return;
	
	//所有厂站
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
		//加入到下拉框
		CString str;
		str = pObj->m_sName;
		int nIndex = pCmb->AddString(str);
		if(nIndex < 0)
			continue;
		//设置关联
		pCmb->SetItemData(nIndex, (DWORD)pObj);
	}
}

/*************************************************************
 函 数 名：SetTimeCtrl()
 功能概要：设置时间控件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40186
void CDlgFaultEventAdd::SetTimeCtrl()
{
	//检查数据有效性
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULTEVENT_TIME1);
	if(pTime == NULL)
		return;
	CDateTimeCtrl* pTime2 = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULTEVENT_TIME2);
	if(pTime2 == NULL)
		return;
	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	pTime2->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//设置范围
	CTime tmMin(1971, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	pTime2->SetRange(&tmMin, &tmMax);
	//设置时间
	CTime tm = CTime::GetCurrentTime();
	CTimeSpan ts(30, 0, 0, 0);
	CTime tm1 = tm - ts;
	VERIFY(pTime->SetTime(&tm1));
}

/*************************************************************
 函 数 名：InitListStyle()
 功能概要：初始化列表风格
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40187
void CDlgFaultEventAdd::InitListStyle()
{
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	
	char* arColumn[5]={"二次设备","启动时间","事件描述", "事件值", "接收时间"};
	
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
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
}

/*************************************************************
 函 数 名：ClearEvent()
 功能概要：删除所有事件
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40196
void CDlgFaultEventAdd::ClearEvent()
{
	//清空各链表
	//动作特征量
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		if(pEvent != NULL)
			delete pEvent;
	}
	m_listChr.RemoveAll();
	//动作信号量
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		if(pEvent != NULL)
			delete pEvent;
	}
	m_listSign.RemoveAll();
	//告警事件
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		if(pEvent != NULL)
			delete pEvent;
	}
	m_listAlarm.RemoveAll();
	//保护录波
	for(i = 0; i < m_listOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
		if(pEvent != NULL)
			delete pEvent;
	}
	m_listOSC.RemoveAll();
	//开关量变位
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
	//清空旧数据
	//清空列表框显示
	m_List.DeleteAllItems();
	//清除链表
	ClearEvent();
	//得到查询条件

	//二次设备ID
	CString sSec = "";
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SEC);
	if(pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		if(nIndex > -1)
		{
			//有效索引
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
	
	//一次设备
	pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_DEVICE);
	if(pCmb != NULL)
	{
		CDeviceObj* pPri = (CDeviceObj*)pCmb->GetItemData(pCmb->GetCurSel());
		if(pPri)
			m_sPridevID = pPri->m_sID;
	}
	
	//一次设备类型
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


	//起始时间
	CTime tmStart(1971,1,1,1,1,1);
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_FAULTEVENT_TIME1);
	if(pTime != NULL)
	{
		pTime->GetTime(tmStart);
	}
	//结束时间
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
	//查询动作
	if(nEventType == 0 || nEventType == 4)
		QueryAction(sSec, tmStart, tmEnd);
	//查询告警
	if(nEventType == 4 || nEventType == 1)
		QueryAlarm(sSec, tmStart, tmEnd);
	//录波简报
	if(nEventType == 4 || nEventType == 2)
		QueryOSC(sSec, tmStart, tmEnd);
	//开关量变位
	if(nEventType == 4 || nEventType == 3)
		QueryDIChange(sSec, tmStart, tmEnd);

	//在列表框中显示
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
 函 数 名：QueryAction()
 功能概要：查询动作事件, 填充动作特征值链表和动作信号链表
 返 回 值: void
 参    数：Param1	二次设备ID, ""代表所有二次设备
		   Param2	起始时间
		   Param3	结束时间
**************************************************************/
//##ModelId=49B87BA40197
void CDlgFaultEventAdd::QueryAction( CString sSec, CTime tmStart, CTime tmEnd )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str;
		
	//字段
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

		
	//条件
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

	//起始时间
	Condition con4;
	str.Format("curtime > '%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//结束时间
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
		WriteLog("CDlgFaultEventAdd::QueryAction, 查询失败");
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
			str.Format("CDlgFaultEventAdd::QueryAction, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventAction* pEvent = new CXJEventAction;
			if(pEvent->LoadFromDB(pMemset))
			{
				//解释事件
				pEvent->TranslateFData();
				//分解事件
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
		str.Format("CDlgFaultEventAdd::QueryAction,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	return;
}

/*************************************************************
 函 数 名：BreakAction()
 功能概要：分解动作事件的特征值和信号量为多条故障事件,分别保存
 返 回 值: void
 参    数：param1	指定动作事件
		   Param2
**************************************************************/
//##ModelId=49B87BA401A6
void CDlgFaultEventAdd::BreakAction( CXJEventAction* pEvent )
{
	//检查数据有效性
	if(pEvent == NULL)
		return;
	//特征值
	for(int i = 0; i < pEvent->m_ActChrs.GetSize(); i++)
	{
		PT_ActionCharacter* pAC = (PT_ActionCharacter*)pEvent->m_ActChrs.GetAt(i);
		if(pAC == NULL)
		{
			continue;
		}
		//创建故障事件
		CFaultEvent* pFaultEvent = new CFaultEvent;
		pFaultEvent->m_nType = XJ_FAULT_EVENT_CHR; //动作特征值
		pFaultEvent->m_nMainRecID = m_nMainRecID;;//故障包ID
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
		//加入到链表
		m_listChr.Add(pFaultEvent);
	}
	//信号量
	for(i = 0; i < pEvent->m_Signals.GetSize(); i++)
	{
		PT_Signal * pSgn = (PT_Signal*)pEvent->m_Signals.GetAt(i);
		if(pSgn == NULL)
			continue;
		//创建故障事件
		CFaultEvent* pFaultEvent = new CFaultEvent;
		pFaultEvent->m_nType = XJ_FAULT_EVENT_SING; //动作信号
		pFaultEvent->m_nMainRecID = m_nMainRecID;;//故障包ID
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
		//加入到链表
		m_listSign.Add(pFaultEvent);
	}
}

/*************************************************************
 函 数 名：FillListData()
 功能概要：填充值到列表框
 返 回 值: 
 参    数：param1	要填充入的类型.0-动作特征值.1-动作信息号.2-告警.3-录波简报
		   Param2
**************************************************************/
//##ModelId=49B87BA401A8
void CDlgFaultEventAdd::FillListData( int nType )
{
	//填充数据时禁止刷新
	m_List.SetRedraw(FALSE);
	//清空原来的值
	m_List.DeleteAllItems();
	//动作特征值
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
			if(pEvent == NULL)
				continue;
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
			if(pEvent == NULL)
				continue;
			AddEventToList(pEvent);
		}
	}
	//录波简报
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
	//开关量变位
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
	//恢复刷新
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 函 数 名：AddEventToList()
 功能概要：加入事件到列表
 返 回 值: 
 参    数：param1	事件指针
		   Param2	要插入的位置索引
**************************************************************/
//##ModelId=49B87BA401B5
void CDlgFaultEventAdd::AddEventToList( CFaultEvent* pEvent, int nIndex /*= 0*/ )
{
	//检查数据有效性
	if(pEvent == NULL)
		return;
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

	if(pEvent->m_bSelected)
		m_List.SetCheck(nIndex);
	//关联设备
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
 函 数 名：QueryAlarm()
 功能概要：查询告警事件
 返 回 值: 
 参    数：param1	二次设备ID, ""代表所有二次设备
		   Param2	起始时间
		   Param3	结束时间
**************************************************************/
//##ModelId=49B87BA401B8
void CDlgFaultEventAdd::QueryAlarm( CString sSec, CTime tmStart, CTime tmEnd )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//字段
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
	
	//条件
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
	
	//起始时间
	Condition con4;
	str.Format("curtime > '%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//结束时间
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
		WriteLog("CDlgFaultEventAdd::QueryAlarm, 查询失败");
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
			str.Format("CDlgFaultEventAdd::QueryAlarm, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventAlarm* pEvent = new CXJEventAlarm;
			if(pEvent->LoadFromDB(pMemset))
			{
				//解释事件
				pEvent->TranslateFData();
				//分解事件
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
		str.Format("CDlgFaultEventAdd::QueryAlarm,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return;
}

/*************************************************************
 函 数 名：BreakAlarm()
 功能概要：分解告警事件为故障事件-告警类型,保存到故障告警链表
 返 回 值: void
 参    数：param1	指定告警事件
		   Param2
**************************************************************/
//##ModelId=49B87BA401C8
void CDlgFaultEventAdd::BreakAlarm( CXJEventAlarm* pEvent )
{
	//检查数据有效性
	if(pEvent == NULL)
		return;
	//信号量
	for(int i = 0; i < pEvent->m_Signals.GetSize(); i++)
	{
		PT_Signal * pSgn = (PT_Signal*)pEvent->m_Signals.GetAt(i);
		if(pSgn == NULL)
			continue;
		//创建故障事件
		CFaultEvent* pFaultEvent = new CFaultEvent;
		pFaultEvent->m_nType = XJ_FAULT_EVENT_ALARM; //故障告警
		pFaultEvent->m_nMainRecID = m_nMainRecID;;//故障包ID
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
		//加入到链表
		m_listAlarm.Add(pFaultEvent);
	}
}

/*************************************************************
 函 数 名：QueryOSC()
 功能概要：查询录波事件
 返 回 值: void
 参    数：param1	二次设备ID, ""代表所有二次设备
		   Param2	起始时间
		   Param3	结束时间
**************************************************************/
//##ModelId=49B87BA401CA
void CDlgFaultEventAdd::QueryOSC( CString sSec, CTime tmStart, CTime tmEnd )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//字段
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
	
	
	//条件
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
	
	//起始时间
	Condition con4;
	str.Format("trigger_time > '%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//结束时间
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
		WriteLog("CDlgFaultEventAdd::QueryOSC, 查询失败");
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
			str.Format("CDlgFaultEventAdd::QueryOSC, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventOSC* pEvent = new CXJEventOSC;
			if(pEvent->LoadFromDB_(pMemset))
			{
				//分解事件
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
		str.Format("CDlgFaultEventAdd::QueryOSC,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return;
}

/*************************************************************
 函 数 名：BreakOSC()
 功能概要：分解录波事件为故障录波
 返 回 值: void
 参    数：param1	指定录波事件
		   Param2
**************************************************************/
//##ModelId=49B87BA401D7
void CDlgFaultEventAdd::BreakOSC( CXJEventOSC* pEvent )
{
	//检查数据有效性
	if(pEvent == NULL)
		return;
	//创建故障事件
	CFaultEvent* pFaultEvent = new CFaultEvent;
	pFaultEvent->m_nType = XJ_FAULT_EVENT_OSCREPORT; //录波简报
	pFaultEvent->m_nMainRecID = m_nMainRecID;//故障包ID
	pFaultEvent->m_tmStart = pEvent->m_tmTime;
	pFaultEvent->m_nmsStart = pEvent->m_nms;
	pFaultEvent->m_tmReceiveTime = pEvent->m_tmTime;
	pFaultEvent->m_nmsReceive = pEvent->m_nms;
	pFaultEvent->m_pSec = pEvent->m_pSec;
	pFaultEvent->m_sEventContent.Format("%d", pEvent->m_nFileSize); //文件大小
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
				pFaultEvent->m_sEventDef = pFaultEvent->m_pSec->m_sStationID + "\\保护\\" + pFaultEvent->m_pSec->m_sName + "(" + pFaultEvent->m_pSec->m_sID + ")" + "\\"+pFaultEvent->m_sEventDef;
			else if(pFaultEvent->m_pSec->m_nType == TYPE_WAVEREC)
				pFaultEvent->m_sEventDef = pFaultEvent->m_pSec->m_sStationID + "\\录波器\\" + pFaultEvent->m_pSec->m_sName + "(" + pFaultEvent->m_pSec->m_sID + ")" + "\\"+pFaultEvent->m_sEventDef;
		}
	}

	//加入到链表
	m_listOSC.Add(pFaultEvent);
}


void CDlgFaultEventAdd::OnClickDlgFaultitemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//得到所选行
	NM_LISTVIEW*   pNMListView=(NM_LISTVIEW*)pNMHDR;     
	int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
	if(nSelected < 0)
		return;
	//判断是否check
	CPoint pt = pNMListView->ptAction;
	UINT uFlag;
	int nIndex = m_List.HitTest(pt, &uFlag);
	if(uFlag == LVHT_ONITEMSTATEICON)
	{
		//得到被选择行的数据
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
        else                              //注意这个地方的区别
        {
           // m_List.SetCheck(nIndex, FALSE);
			pEvent->m_bSelected = 0;
        }
	}

	*pResult = 0;
}

/*************************************************************
 函 数 名：SaveEvent()
 功能概要：保存被选择的事件到故障包
 返 回 值: void
 参    数：param1
		   Param2
**************************************************************/
//##ModelId=49B87BA401D9
void CDlgFaultEventAdd::SaveEvent()
{
	//检查数据有效性
	CDlgFaultReport* pParent = (CDlgFaultReport*)GetParent();
	if (pParent == NULL)
	{
		MYASSERT(pParent);
		return;
	}
	//动作特征值
	int i = 0;
	for(i = 0; i < m_listChr.GetSize(); i++)
	{
		//得到事件
		CFaultEvent* pEvent = (CFaultEvent*)m_listChr.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//被选择的加入到父窗口的对应链表中
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			pParent->m_listChr.Add(pNew);
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//动作信号
	for(i = 0; i < m_listSign.GetSize(); i++)
	{
		//得到事件
		CFaultEvent* pEvent = (CFaultEvent*)m_listSign.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//被选择的加入到父窗口的对应链表中
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			pParent->m_listSign.Add(pNew);
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//告警
	for(i = 0; i < m_listAlarm.GetSize(); i++)
	{
		//得到事件
		CFaultEvent* pEvent = (CFaultEvent*)m_listAlarm.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//被选择的加入到父窗口的对应链表中
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			pParent->m_listAlarm.Add(pNew);
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//录波
	for(i = 0; i < m_listOSC.GetSize(); i++)
	{
		//得到事件
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//被选择的加入到父窗口的对应链表中
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			if(pNew->m_pSec == NULL)
				continue;
			if(pNew->m_pSec->m_nType == TYPE_PROTECT)
			{
				//保护
				pParent->m_listPTOSC.Add(pNew);
			}
			else if(pNew->m_pSec->m_nType == TYPE_WAVEREC)
			{
				//录波器
				pParent->m_listWROSC.Add(pNew);
			}
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//开关量变位
	for(i = 0; i < m_listDIChange.GetSize(); i++)
	{
		//得到事件
		CFaultEvent* pEvent = (CFaultEvent*)m_listDIChange.GetAt(i);
		MYASSERT_CONTINUE(pEvent);
		//被选择的加入到父窗口的对应链表中
		if(pEvent->m_bSelected)
		{
			CFaultEvent* pNew = pEvent->Clone();
			pParent->m_listDIChange.Add(pNew);
			pParent->m_bEventNeedSave = TRUE;
		}
	}
	//刷新父窗口列表框
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
	//取得厂站
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
	//重新填充设备名称下拉框
	FillDevName(pStation);
	//重新载入设备
	FillSec(pStation);
}
/*************************************************************
 函 数 名: CompareFunction()
 功能概要: 比较函数,排序时被调用
 返 回 值: 比较结果.负数表示1比2小. 0表示相等. 正数表示1比2大
 参    数: param1 比较对象1
		   Param2 比较对象2
		   Param3 比较参数
**************************************************************/
int CALLBACK CDlgFaultEventAdd::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
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

void CDlgFaultEventAdd::OnColumnclickDlgFaultitemList(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CDlgFaultEventAdd::FillSecType(CStationObj* pStation)
{
	//循环厂站下的保护和录波器
	//取得数据引擎
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
	return;
	CComboBox* pCmbDevice = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_DEVICE);
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SEC);
	//所有厂站
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
		//加入到下拉框
		CString str;
		str = pObj->m_sName;
		int nIndex = pCmb->AddString(str);
		if(nIndex < 0)
			continue;
		//设置关联
		pCmb->SetItemData(nIndex, (DWORD)pObj);
	}
}

void CDlgFaultEventAdd::FillDevName(CStationObj* pStation)
{
	//取得设备名称下拉框指针

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_DEVICE);
	if(pCmb == NULL)
		return;
	//清除旧数据
	pCmb->ResetContent();
	
	//设备链表
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	//取得符合条件的设备
	GetDevNameList(listDevice);
	//将设备加入到下拉框
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		//加入
		int nIndex = pCmb->AddString(pObj->m_sName);
		if(nIndex >= 0)
		{
			pCmb->SetItemData(nIndex, (DWORD)pObj);
		}
	}
	
	//清空链表
	listDevice.RemoveAll();	
	
	//加入"所有"选项
	int nIndex = pCmb->AddString( "("+StringFromID(IDS_COMMON_ALL)+")" );
	if(nIndex >= 0)
		pCmb->SetCurSel(nIndex);
	m_cmbPriDev.OnDataFilled();
}
//一次设备
void CDlgFaultEventAdd::OnSelchangeDlgFaulteventCmbDevice() 
{
	// TODO: Add your control notification handler code here
	//取得厂站
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_STATION);
	if(pCmb == NULL)
		return;
	int nSel = pCmb->GetCurSel();
	if(nSel < 0)
		return;
	CStationObj* pStation = (CStationObj*)pCmb->GetItemData(nSel);
	FillSec(pStation);
}
//类型
void CDlgFaultEventAdd::OnSelchangeDlgFaulteventCmbSectype() 
{
	// TODO: Add your control notification handler code here
	//取得厂站
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
 函 数 名：FillDevType()
 功能概要：填充设备类型下拉框列表
 返 回 值: 
 参    数：param1
		   Param2
**************************************************************/
void CDlgFaultEventAdd::FillDevType()
{
	//检查数据有效性
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_FAULTEVENT_CMB_SECTYPE);
	if(pCmb == NULL)
		return;
	//清空原有数据
	pCmb->ResetContent();
	//加入"所有"选项
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
 函 数 名: GetDevNameList()
 功能概要: 取得根据厂站和设备类型决定的一次设备
 返 回 值: 
 参    数: param1 保存一次设备的链表
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
函数名:   QueryDIChange	
返回值:   void	
功能概要: 
参数: CString sSec	
参数: CTime tmStart	
参数: CTime tmEnd	
*****************************************************/
void CDlgFaultEventAdd::QueryDIChange( CString sSec, CTime tmStart, CTime tmEnd )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//字段
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

	
	//条件
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
	
	//起始时间
	Condition con4;
	str.Format("time > '%s'", tmStart.Format("%Y-%m-%d %H:%M:%S"));
	pApp->m_DBEngine.SetCondition(sql, con4, str);
	//结束时间
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
		WriteLog("CDlgFaultEventAdd::QueryDIChange, 查询失败");
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
			str.Format("CDlgFaultEventAdd::QueryDIChange, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//创建事件
			CXJEventDI* pEvent = new CXJEventDI;
			if(pEvent->LoadFromDB(pMemset))
			{
				//分解事件
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
		str.Format("CDlgFaultEventAdd::QueryDIChange,查询失败,原因为%s", sError);
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
	//检查数据有效性
	if(pEvent == NULL)
		return;
	
	//创建故障事件
	CFaultEvent* pFaultEvent = new CFaultEvent;
	pFaultEvent->m_nType = XJ_FAULT_EVENT_DICHANGE; //故障告警
	pFaultEvent->m_nMainRecID = m_nMainRecID;;//故障包ID
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
	//加入到链表
	m_listDIChange.Add(pFaultEvent);
}

