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
	
	//更新数据库的挂牌表
	if (InsertDBMark())
	{
		AfxMessageBox( StringFromID(IDS_HANGOUT_SUCCESS));
		SetDeviceState();
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

/*************************************************************
 函 数 名：InsertDBMark()
 功能概要：更新数据库的装置挂牌信息
 返 回 值: 返回值说明
 参    数：param1   意义说明
           Param2   意义说明
**************************************************************/
BOOL DLGMarked::InsertDBMark()
{
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
