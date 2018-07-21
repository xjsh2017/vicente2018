// DLGMarked.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DLGMarked.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// DLGMarked dialog

DLGMarked::DLGMarked(CWnd* pParent /*=NULL*/)
	: CDialog(DLGMarked::IDD, pParent)
{
	m_bMark = FALSE;
	//{{AFX_DATA_INIT(DLGMarked)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DLGMarked::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DLGMarked)
	DDX_Control(pDX, IDC_CMB_MARKREASON, m_cmbMarkReason);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLGMarked, CDialog)
	//{{AFX_MSG_MAP(DLGMarked)
	ON_BN_CLICKED(IDC_BTN_MARK, OnBtnMark)
	ON_BN_CLICKED(IDC_BTN_UNMARK, OnBtnUnmark)
	ON_CBN_SELCHANGE(IDC_CMB_MARKREASON, OnSelchangeCmbMarkreason)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLGMarked message handlers

void DLGMarked::OnBtnMark() 
{
	// TODO: Add your control notification handler code here
	//更新挂牌状态
	m_bMark = TRUE;

	//判断是否存在已挂牌装置（一次仅一个保护）
	if (CheckMarkedDeviceFromState())
		return;
	
	//更新数据库的挂牌表
	if (InsertDBMark())
	{
		AfxMessageBox( StringFromID(IDS_HANGOUT_SUCCESS));
		SetDeviceState();
	}else{
		AfxMessageBox("请选择或输入挂牌原因.");
	}
}

void DLGMarked::OnBtnUnmark() 
{
	// TODO: Add your control notification handler code here
	//更新挂牌状态
	m_bMark = FALSE;
	
	//更新数据库的挂牌表
	if (InsertDBMark())
	{
		AfxMessageBox( StringFromID(IDS_UNHANGOUT_SUCCESS));
		SetDeviceState();
		
	}
}

BOOL DLGMarked::CheckMarkedDeviceFromState()
{
	BOOL bReturn = TRUE;
    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	CString str;
	PT_ZONE zone;
	CString sRecords;
	CSecObj *pObj = NULL;

	int nCurPTSetModSate = pApp->GetPTSetModState(zone, sRecords);
	if (0 == nCurPTSetModSate)
		return FALSE;

	pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(zone.PT_ID, TYPE_SEC);
	if (NULL == pObj)
		return FALSE;

	CString sUserID;
	CString sTime;
	vector<CString> v = pApp->SplitCString(sRecords, ";");
	if (v.size() > 0){
		vector<CString> v2 = pApp->SplitCString(v[0], ",");
		sUserID = v2[1];
		sTime = v2[0];
	}

	str.Format("您当前存在未完成定值修改的保护装置： \n\n挂牌装置：[%s] %s \n\n挂牌时间：%s \n\n执行用户：%s"
			, pObj->m_pStation ? pObj->m_pStation->m_sName : ""
			, pObj->m_sName
			, sTime
			, sUserID);
	AfxMessageBox(str, MB_ICONINFORMATION);

	return bReturn;
}

/*BOOL DLGMarked::CheckMarkedDeviceFromState()
{
	BOOL nReturn = FALSE;
    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
    
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//字段
	//value
	Field Field0;
	pApp->m_DBEngine.SetField(sql, Field0, "value", EX_STTP_DATA_TYPE_STRING);
	//reverse1
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "reverse1", EX_STTP_DATA_TYPE_STRING);
	//reverse2
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "reverse2", EX_STTP_DATA_TYPE_STRING);
	Field Field3;
	pApp->m_DBEngine.SetField(sql, Field3, "reverse3", EX_STTP_DATA_TYPE_STRING);
	
	CString str;
	//条件
	//USER_ID
	Condition condition1;
	str.Format("keyname = 'DZ_MOD_STATE'");
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult = FALSE;
	int nDZ_MOD_STATE_VALUE = -1;
	CString sPTID;
	CString sPT_ZONE;
	CString sRecords;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
		
		if (1 != nResult)
		{
			nReturn = FALSE;
			//AfxMessageBox("nResult = FALSE");
		}
		
		str.Format("nResult = %d", nResult);
		//AfxMessageBox(str);
	}
	catch (CException* e)
	{
		//AfxMessageBox("CDeviceView::CheckMarkedDeviceFromState, 查询失败");
		e->Delete();
		WriteLog("CDeviceView::CheckMarkedDeviceFromState, 查询失败", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		nReturn = FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		int nCount = pMemset->GetMemRowNum();
		
		CString str;
		str.Format("CDeviceView::CheckMarkedDeviceFromState, 读取到%d条数据", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		if(nCount > 0)
		{
            nDZ_MOD_STATE_VALUE = atoi(pMemset->GetValue((UINT)0));

			sPTID = pMemset->GetValue((UINT)1);
			sPT_ZONE = pMemset->GetValue((UINT)2);
			sRecords = pMemset->GetValue((UINT)3);

			CString sUserID;
			vector<CString> v = pApp->SplitCString(sRecords, ";");
			if (v.size() > 0){
				vector<CString> v2 = pApp->SplitCString(v[0], ",");
				sUserID = v2[1];
			}
			
			if (0 != nDZ_MOD_STATE_VALUE){
				str.Format("您当前存在未完成挂牌作业的保护装置： \n\n装置名称：%s \n\n执行记录：\n%s \n\n执行用户：%s"
				, sPTID
				, sRecords
				, sUserID);
				AfxMessageBox(str, MB_ICONINFORMATION);
				
				nReturn = TRUE;
			}
		}
	}
	else
	{
		CString str;
		str.Format("CDeviceView::CheckMarkedDeviceFromState,查询失败,原因为%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		nReturn = FALSE;	
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
    
    return nReturn;
}*/

/*************************************************************
 函 数 名：InsertDBMark()
 功能概要：更新数据库的装置挂牌信息
 返 回 值: 返回值说明
 参    数：param1   意义说明
           Param2   意义说明
**************************************************************/
BOOL DLGMarked::InsertDBMark()
{
	ASSERT(NULL != m_pObj);

	CString strTemp;
	strTemp.Format("PT_ID: %s, NAME: %s",m_pObj->m_sID, m_pObj->m_sName);
	//AfxMessageBox(strTemp);

	BOOL bReturn = TRUE;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	if (m_bMark)
	{
		//获取当前选择项
		CString sInsert = "";
		((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON))->GetWindowText(sInsert);
		if (sInsert == "")
		{
			return FALSE;
		}
		m_pObj->m_sSuspendreason = sInsert;

		//判断原因是否已经存在
		BOOL bInsert = Comparison(sInsert);
		
		//更新挂牌原因表
		if (bInsert)
		{
			//获得新原因的ID
			m_pObj->m_nSuspendreason = pApp->GetDataEngine()->m_arrMark.GetCount(); 
			
			//组建查询条件
			SQL_DATA sql;
			sql.Conditionlist.clear();
			sql.Fieldlist.clear();
			
			//设置字段
			
			//挂牌原因编号
			CString str = "";
			str.Format("%d", m_pObj->m_nSuspendreason);
			Field field1;
			pApp->m_DBEngine.SetField(sql, field1, "id", EX_STTP_DATA_TYPE_INT, str);
			
			//挂牌原因描述
			Field field2;
			pApp->m_DBEngine.SetField(sql, field2, "note", EX_STTP_DATA_TYPE_STRING, m_pObj->m_sSuspendreason);
			
			char * sError = new char[MAXMSGLEN];
			memset(sError, '\0', MAXMSGLEN);
			
			int nResult;
			try
			{
				nResult = pApp->m_DBEngine.XJInsertData(EX_STTP_INFO_TB_SUSPEND_REASON_DEF, sql, sError);
			}
			catch (...)
			{
				WriteLog("DLGMarked::InsertDBMark, 保存失败");
				delete[] sError;
				return FALSE;
			}
			
			if(nResult == 1)
			{
				//成功
				CMark* pObj = new CMark;
				pObj->m_sMarkInfo = sInsert;
				pObj->m_nMarkID = m_pObj->m_nSuspendreason;
				//读取数据成功,加入到链表
				pApp->GetDataEngine()->m_arrMark.AddTail(pObj);
				bReturn = TRUE;
			}
			else
				bReturn = FALSE;
			
			delete[] sError;
		}
	}
	
	//更新挂牌状态表
	//组建查询条件
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;	
	//字段
	//SVG_Path
	if (m_pObj->m_nSuspendreason != -1)
	{
		str.Format("%d", m_pObj->m_nSuspendreason);
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "suspendreason", EX_STTP_DATA_TYPE_INT, str);
	}
	m_pObj->m_nSuspendstatus = m_bMark;
	str.Format("%d",m_bMark);
	Field Field2;
	pApp->m_DBEngine.SetField(sql, Field2, "suspendstatus", EX_STTP_DATA_TYPE_INT, str);
	Field Field3;
	str.Format("%d", m_bMark ? 5 : 2);
	m_pObj->m_nRunStatu = m_bMark ? 5 : 2;
	pApp->m_DBEngine.SetField(sql, Field3, "run_status", EX_STTP_DATA_TYPE_INT, str);

	//条件
	//station_id
	Condition condition2;
	str.Format("name = '%s'", m_pObj->m_sName);
	pApp->m_DBEngine.SetCondition(sql, condition2, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJUpdateData(EX_STTP_INFO_SECDEV_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("DLGMarked::InsertDBMark, 更新失败");
		delete[] sError;
		return FALSE;
	}
	if(nResult == 1)
	{
		bReturn = TRUE;
	}
	else
	{
		CString str;
		str.Format("DLGMarked::InsertDBMark,更新失败,原因为%s", sError);
		WriteLog(str);
		bReturn = FALSE;
	}
	
	delete[] sError;
	sError = NULL;

	PT_ZONE zone;
	zone.PT_ID = m_pObj->m_sID;
	pApp->NextPTSetModState(m_bMark ? 1 : 0, zone, pApp->m_User.m_strUSER_ID);

	if (!m_bMark){
		PT_ZONE zone;
		pApp->GetPTSetModState(zone, CString());
		pApp->RevertTempPTSetToDB(zone);
	}
	
 	return bReturn;
}

/*************************************************************
 函 数 名：SetDeviceState()
 功能概要：设置挂牌状态
 返 回 值: 返回值说明
 参    数：param1   意义说明
           Param2   意义说明
**************************************************************/
void DLGMarked::SetDeviceState()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	pMainFrame->m_pDeviceView->FillTree();
	CXJBrowserView * pView = pApp->GetSVGView();
	pView->SetDeviceCol(m_pObj);//设置颜色
}


void DLGMarked::OnSelchangeCmbMarkreason() 
{
	// TODO: Add your control notification handler code here
	
// 	int nIndex=((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON))->GetCurSel();
// 	if (nIndex!=0)
// 	{
// 		CStationObj* pStationid=(CStationObj*)m_cmbMarkReason.GetItemData(nIndex);
// 		
// 		CString sStationID=	pStationid->m_sID;
// 		m_sChooseStationID.Format("%s",sStationID);
// 	}
}

/*************************************************************
 函 数 名: InitComBox()
 功能概要: 载入combox列表信息 默认为第一项
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
void DLGMarked::InitComBox()
{
	//用厂站中的数据填充列表
	m_cmbMarkReason.ResetContent();
	
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	
	POSITION pos = pData->m_arrMark.GetHeadPosition();
	//m_cmbMarkReason.InsertString(0,"所有");
	while(pos != NULL)
	{
		CMark* pObj = (CMark*)pData->m_arrMark.GetNext(pos);
		if(pObj == NULL)
			continue;
		//加入到下拉框
		int nIndex = pObj->m_nMarkID;
			
		m_cmbMarkReason.InsertString(nIndex,pObj->m_sMarkInfo);
		m_cmbMarkReason.SetItemData(nIndex,(DWORD)pObj);
		nIndex++;
	}
	//m_cmbMarkReason.SetCurSel(0);
}

BOOL DLGMarked::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitComBox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*************************************************************
 函 数 名: Comparison()
 功能概要: 比较描述原因是否是原有的
 返 回 值: 
 参    数: param1 
		   Param2 
**************************************************************/
BOOL DLGMarked::Comparison(CString sMarkInfo)
{
	//加载新数据
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return TRUE;
	
	POSITION pos = pData->m_arrMark.GetHeadPosition();
	while(pos != NULL)
	{
		CMark* pObj = (CMark*)pData->m_arrMark.GetNext(pos);
		if(pObj == NULL)
			continue;
		if (pObj->m_sMarkInfo == sMarkInfo)
		{
			return FALSE;
		}
	}
	return TRUE;
}
