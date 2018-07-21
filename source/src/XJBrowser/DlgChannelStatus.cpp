// DlgChannelStatus.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgChannelStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChannelStatus dialog


CDlgChannelStatus::CDlgChannelStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChannelStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChannelStatus)
	m_sStation = _T("");
	m_sStatus = _T("");
	m_sTime = _T("");
	m_sTime2 = _T("");
	m_sIP = _T("");
	m_sChannel = _T("");
	//}}AFX_DATA_INIT
}


void CDlgChannelStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChannelStatus)
	DDX_Control(pDX, IDC_LIST_CS, m_List);
	DDX_Text(pDX, IDC_EDIT_CS_STATION, m_sStation);
	DDX_Text(pDX, IDC_EDIT_CS_STATUS, m_sStatus);
	DDX_Text(pDX, IDC_EDIT_CS_TIME, m_sTime);
	DDX_Text(pDX, IDC_EDIT_CS_TIME2, m_sTime2);
	DDX_Text(pDX, IDC_EDIT_CS_IP, m_sIP);
	DDX_Text(pDX, IDC_EDIT_CS_CHANNEL, m_sChannel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChannelStatus, CDialog)
	//{{AFX_MSG_MAP(CDlgChannelStatus)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_CS, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChannelStatus message handlers

BOOL CDlgChannelStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_pStation == NULL)
		return FALSE;
	m_sChannel.Format("%d", m_nChannel);
	m_sStation = m_pStation->m_sName;
	m_sStatus = m_pStation->GetStatusDesc(m_nChannel);
	
	if(m_nChannel == 0)
	{
		m_sIP = m_pStation->m_sIP;
		m_sTime = m_pStation->m_tmNewStatus.Format("%Y-%m-%d %H:%M:%S");
		m_sTime2 = m_pStation->m_sDurativeTime;
	}
	else
	{
		m_sIP = m_pStation->m_sIPReserve;
		m_sTime = m_pStation->m_tmNewStatus1.Format("%Y-%m-%d %H:%M:%S");
		m_sTime2 = m_pStation->m_sDurativeTime1;
	}

	InitListStyle();
	LoadData();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgChannelStatus::InitListStyle()
{
	//空图形列表, 用来调整行高
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	//分配列宽,最小需求
	for (int nCol=0; nCol < 4 ; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0:
			lvCol.cx = 60; //ID
			colName = StringFromID(IDS_COMMON_NO);
			break;
		case 1://
			lvCol.cx = 180; // 时间
			colName = StringFromID(IDS_COMMON_TIME);
			break;
		case 2://
			lvCol.cx = 100; // 状态
			colName = StringFromID(IDS_COMMU_STATUS);
			break;
		case 3:
			lvCol.cx = 200;
			colName = StringFromID(IDS_DISCONNECT_REASON);
		default:
			lvCol.cx=200;//列宽100象素
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_List.InsertColumn(nCol,&lvCol);
	}
	//默认隐藏第一列(序号)
	//m_List.SetColumnHide(0, TRUE);
	
	//设置风格
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
}

void CDlgChannelStatus::AddDataToList( int nIndex, CString sTime, CString sStatus, CString sReason )
{
	
	//0:序号
	CString sID;
	sID.Format("%d", nIndex);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, 0, sID, 0, 0, 0, 0) == -1)
		return;
	
	//1:时间
	m_List.SetItemText(0, 1, sTime);
	
	//2:状态
	m_List.SetItemText(0, 2, sStatus);

	//原因
	m_List.SetItemText(0, 3, sReason);

}

/****************************************************
Date:2013/7/19  Author:LYH
函数名:   LoadData	
返回值:   void	
功能概要: 
*****************************************************/
void CDlgChannelStatus::LoadData()
{
	if(m_pStation == NULL)
		return;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//通过Station_ID查询最新一次值(Status, Time, Reason)
	
	//设置字段
	Field Field0;
	pApp->m_DBEngine.SetField(sql, Field0, "100", EX_STTP_DATA_TYPE_TOP);
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Status", EX_STTP_DATA_TYPE_INT);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Time", EX_STTP_DATA_TYPE_TIME);
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "Reason", EX_STTP_DATA_TYPE_STRING);

	
	//设置条件
	Condition Condition0;
	CString str;
	str.Format("STATION_ID = '%s'", m_pStation->m_sID);
	pApp->m_DBEngine.SetCondition(sql, Condition0, str);

	Condition Condition2;
	str.Format("reserve1 = %d", m_nChannel);
	pApp->m_DBEngine.SetCondition(sql, Condition2, str);
	
	Condition Condition1;
	pApp->m_DBEngine.SetCondition(sql, Condition1, "order by Reserve2 DESC, Time DESC, id desc", EX_STTP_WHERE_ABNORMALITY);
	
	//进行查询
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_STATION_STATUS_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDlgChannelStatus::LoadData(), 查询失败");
		delete[] sError;
		delete pMemset;
		return;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString str;
		str.Format("CDlgChannelStatus::LoadData(), 厂站[%s]读取到%d条数据", m_pStation->m_sID, nCount);
		WriteLog(str);
		
		CString strStatus;
		CString strTime;
		CString strReason;
		if(nCount > 0)
		{
			for(int k = 0; k < nCount - 1; k++)
			{
				pMemset->MoveNext();
			}
			for(int i = 0; i < nCount; i++)
			{
				strStatus = pMemset->GetValue((UINT)0);
				strStatus = GetStationStatusString(atoi(strStatus));
				strTime = pMemset->GetValue(1);
				strReason = pMemset->GetValue(2);
				strReason = m_pStation->GetReason(atoi(strReason));
				AddDataToList(i+1, strTime, strStatus, strReason);
				pMemset->MoveForward();
			}
		}

		if (0 == nCount)
		{
			CString strLog;
			strLog.Format("CDlgChannelStatus::LoadData(), 未找到[%s]通道[%d]通断记录", m_pStation->m_sID, m_nChannel);
			WriteLog(strLog);
		}
	}
	else
	{
		CString str;
		str.Format("CDlgChannelStatus::LoadData(), 查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

void CDlgChannelStatus::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
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
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //行索引
				int nSubItem = lplvcd->iSubItem; //列索引
				//先给默认值, 后面再特殊处理
				lplvcd ->clrText = RGB(0, 0, 0);
				
				if (nSubItem == 2)
				{
					CString strStatus = m_List.GetItemText(nItem, nSubItem);
					if (!strStatus.CompareNoCase( StringFromID(IDS_COMMUSTATUS_OFF)))
					{
						lplvcd->clrText = RGB(255, 0, 0);
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
