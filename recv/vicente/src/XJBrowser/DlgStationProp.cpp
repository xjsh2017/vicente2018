// DlgStationProp.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgStationProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStationProp dialog


CDlgStationProp::CDlgStationProp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStationProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStationProp)
	m_sGetWay = _T("");
	m_sIP = _T("");
	m_sName = _T("");
	m_sNet = _T("");
	m_sPort = _T("");
	m_sProtocol = _T("");
	m_sManufacturer = _T("");
	//}}AFX_DATA_INIT
	m_pStation = NULL;
}


void CDlgStationProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStationProp)
	DDX_Text(pDX, IDC_EDIT_STAPROP_GETWAY, m_sGetWay);
	DDX_Text(pDX, IDC_EDIT_STAPROP_IP, m_sIP);
	DDX_Text(pDX, IDC_EDIT_STAPROP_NAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_STAPROP_NET, m_sNet);
	DDX_Text(pDX, IDC_EDIT_STAPROP_PORT, m_sPort);
	DDX_Text(pDX, IDC_EDIT_STAPROP_PROTOCOL, m_sProtocol);
	DDX_Text(pDX, IDC_EDIT_STAPROP_MANUFACTURER, m_sManufacturer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStationProp, CDialog)
	//{{AFX_MSG_MAP(CDlgStationProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStationProp message handlers

BOOL CDlgStationProp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_pStation == NULL)
		return FALSE;

	m_sGetWay = m_pStation->m_sTel;
	m_sIP = m_pStation->m_sIP;
	m_sName = m_pStation->m_sName;
	m_sManufacturer = m_pStation->m_sManufacturer;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData != NULL)
	{
		CDeviceObj* pNet = pData->FindDevice(m_pStation->m_sNetID, TYPE_NET);
		if(pNet != NULL)
			m_sNet = pNet->m_sName;
	}
	m_sPort = m_pStation->m_sPort;
	m_sProtocol = QueryProtocolName(m_pStation->m_nProtocolId);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************
Date:2011/10/14  Author:LYH
函数名:   QueryProtocolName	
返回值:   CString	
功能概要: 
参数: int nProtocolID	
*****************************************************/
CString CDlgStationProp::QueryProtocolName( int nProtocolID )
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
		
	CString str = "";
		
	//字段
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "Name", EX_STTP_DATA_TYPE_STRING);

	//条件
	Condition condition4;
	CString strFilter;
	strFilter.Format("ID=%d", nProtocolID);
	pApp->m_DBEngine.SetCondition(sql, condition4, strFilter);
			
	CMemSet* pMemset;
	pMemset = new CMemSet;
		
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
		
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_COMMU_PROTOCOL_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDlgStationProp::QueryProtocolName, 查询失败");
		delete[] sError;
		delete pMemset;
		return "";
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount >= 0)
		{
			CString str;
			str.Format("CDlgStationProp::QueryProtocolName, 读取到%d条数据", nCount);
			WriteLog(str);
		}
		if(nCount == 1)
			str = pMemset->GetValue((UINT)0);
		pMemset->MoveNext();
	}
	else
	{
		CString str;
		str.Format("CDlgStationProp::QueryProtocolName,查询失败,原因为%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return str;
}
