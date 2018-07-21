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
	//���¹���״̬
	m_bMark = TRUE;

	//�ж��Ƿ�����ѹ���װ�ã�һ�ν�һ��������
	if (CheckMarkedDeviceFromState())
		return;
	
	//�������ݿ�Ĺ��Ʊ�
	if (InsertDBMark())
	{
		AfxMessageBox( StringFromID(IDS_HANGOUT_SUCCESS));
		SetDeviceState();
	}else{
		AfxMessageBox("��ѡ����������ԭ��.");
	}
}

void DLGMarked::OnBtnUnmark() 
{
	// TODO: Add your control notification handler code here
	//���¹���״̬
	m_bMark = FALSE;
	
	//�������ݿ�Ĺ��Ʊ�
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

	str.Format("����ǰ����δ��ɶ�ֵ�޸ĵı���װ�ã� \n\n����װ�ã�[%s] %s \n\n����ʱ�䣺%s \n\nִ���û���%s"
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
	
	//�ֶ�
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
	//����
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
		//AfxMessageBox("CDeviceView::CheckMarkedDeviceFromState, ��ѯʧ��");
		e->Delete();
		WriteLog("CDeviceView::CheckMarkedDeviceFromState, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		nReturn = FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		int nCount = pMemset->GetMemRowNum();
		
		CString str;
		str.Format("CDeviceView::CheckMarkedDeviceFromState, ��ȡ��%d������", nCount);
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
				str.Format("����ǰ����δ��ɹ�����ҵ�ı���װ�ã� \n\nװ�����ƣ�%s \n\nִ�м�¼��\n%s \n\nִ���û���%s"
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
		str.Format("CDeviceView::CheckMarkedDeviceFromState,��ѯʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����InsertDBMark()
 ���ܸ�Ҫ���������ݿ��װ�ù�����Ϣ
 �� �� ֵ: ����ֵ˵��
 ��    ����param1   ����˵��
           Param2   ����˵��
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
		//��ȡ��ǰѡ����
		CString sInsert = "";
		((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON))->GetWindowText(sInsert);
		if (sInsert == "")
		{
			return FALSE;
		}
		m_pObj->m_sSuspendreason = sInsert;

		//�ж�ԭ���Ƿ��Ѿ�����
		BOOL bInsert = Comparison(sInsert);
		
		//���¹���ԭ���
		if (bInsert)
		{
			//�����ԭ���ID
			m_pObj->m_nSuspendreason = pApp->GetDataEngine()->m_arrMark.GetCount(); 
			
			//�齨��ѯ����
			SQL_DATA sql;
			sql.Conditionlist.clear();
			sql.Fieldlist.clear();
			
			//�����ֶ�
			
			//����ԭ����
			CString str = "";
			str.Format("%d", m_pObj->m_nSuspendreason);
			Field field1;
			pApp->m_DBEngine.SetField(sql, field1, "id", EX_STTP_DATA_TYPE_INT, str);
			
			//����ԭ������
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
				WriteLog("DLGMarked::InsertDBMark, ����ʧ��");
				delete[] sError;
				return FALSE;
			}
			
			if(nResult == 1)
			{
				//�ɹ�
				CMark* pObj = new CMark;
				pObj->m_sMarkInfo = sInsert;
				pObj->m_nMarkID = m_pObj->m_nSuspendreason;
				//��ȡ���ݳɹ�,���뵽����
				pApp->GetDataEngine()->m_arrMark.AddTail(pObj);
				bReturn = TRUE;
			}
			else
				bReturn = FALSE;
			
			delete[] sError;
		}
	}
	
	//���¹���״̬��
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;	
	//�ֶ�
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

	//����
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
		WriteLog("DLGMarked::InsertDBMark, ����ʧ��");
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
		str.Format("DLGMarked::InsertDBMark,����ʧ��,ԭ��Ϊ%s", sError);
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
 �� �� ����SetDeviceState()
 ���ܸ�Ҫ�����ù���״̬
 �� �� ֵ: ����ֵ˵��
 ��    ����param1   ����˵��
           Param2   ����˵��
**************************************************************/
void DLGMarked::SetDeviceState()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	pMainFrame->m_pDeviceView->FillTree();
	CXJBrowserView * pView = pApp->GetSVGView();
	pView->SetDeviceCol(m_pObj);//������ɫ
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
 �� �� ��: InitComBox()
 ���ܸ�Ҫ: ����combox�б���Ϣ Ĭ��Ϊ��һ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void DLGMarked::InitComBox()
{
	//�ó�վ�е���������б�
	m_cmbMarkReason.ResetContent();
	
	//����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	
	POSITION pos = pData->m_arrMark.GetHeadPosition();
	//m_cmbMarkReason.InsertString(0,"����");
	while(pos != NULL)
	{
		CMark* pObj = (CMark*)pData->m_arrMark.GetNext(pos);
		if(pObj == NULL)
			continue;
		//���뵽������
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
 �� �� ��: Comparison()
 ���ܸ�Ҫ: �Ƚ�����ԭ���Ƿ���ԭ�е�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
BOOL DLGMarked::Comparison(CString sMarkInfo)
{
	//����������
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
