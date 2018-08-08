// DLGMarked.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DLGMarked.h"
#include "MainFrm.h"

#include "DlgValidateID.h"

#include "XJTagOutStore.h"
#include "qptsetstatetable.h"
#include "XJUserStore.h"

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
	DDX_Control(pDX, IDC_LIST_CTRL1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLGMarked, CDialog)
	//{{AFX_MSG_MAP(DLGMarked)
	ON_BN_CLICKED(IDC_BTN_MARK, OnBtnMark)
	ON_BN_CLICKED(IDC_BTN_UNMARK, OnBtnUnmark)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnApplySetting)
	ON_CBN_SELCHANGE(IDC_CMB_MARKREASON, OnSelchangeCmbMarkreason)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CTRL1, OnClickList1)
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
	if (!CanMark())
		return;

	//������Աȷ��
	CString sRunUser;
	CDlgValidateID dlgID(0, 1);
	dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
	if(dlgID.DoModal() == IDOK)
	{	
		sRunUser = dlgID.m_strUser;

		//�������ݿ�Ĺ��Ʊ�
		if (InsertDBMark())
		{
			CString sInsert = "";
			((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON))->GetWindowText(sInsert);

			CXJTagOutStore::GetInstance()->GetState()->Next_1(sRunUser.GetBuffer(0)
				, m_pObj->m_sID.GetBuffer(0), sInsert.GetBuffer(0));
			
			AfxMessageBox( StringFromID(IDS_HANGOUT_SUCCESS), MB_OK | MB_ICONINFORMATION);
			SetDeviceState();
		}
	}
}

void DLGMarked::OnBtnUnmark() 
{
	// TODO: Add your control notification handler code here
	//���¹���״̬
	m_bMark = FALSE;

	// ����Ƿ����ȡ������
	if (!CanUnMark())
		return;
	
	//������Աȷ��
	CString sRunUser;
	CDlgValidateID dlgID(0, 1);
	dlgID.m_strFuncID = FUNC_XJBROWSER_CONTROL;
	if(dlgID.DoModal() == IDOK)
	{	
		sRunUser = dlgID.m_strUser;
		
		//�������ݿ�Ĺ��Ʊ�
		if (InsertDBMark())
		{
			CXJTagOutStore::GetInstance()->GetState()->Next_0(sRunUser.GetBuffer(0));
			
			AfxMessageBox( StringFromID(IDS_UNHANGOUT_SUCCESS), MB_OK | MB_ICONINFORMATION);
			SetDeviceState();
		}
	}
}

void DLGMarked::OnApplySetting()
{
	CXJUserStore *pUserStore = CXJUserStore::GetInstance();
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTSetDataTable *pPTValvSetData = pTagOutStore->GetPTSetData();
	
	CComboBox *pCMB = ((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON));
	int nIndex=pCMB->GetCurSel();
	CString str;
	pCMB->GetLBText(nIndex, str);
	int nReasonType = pTagOutState->GetTagOutReasonType(str.GetBuffer(0));
	QByteArray baReasonName = str.GetBuffer(0);
	QByteArrayMatrix &flow = pTagOutState->GetWorkFlow(nReasonType);
	int nTagOutRowIdx = pTagOutStore->GetTagOutRowIdx(nReasonType);

	CPoint pt;
	CString strText;
	nIndex = 0;
	for (int i = 0; i < m_List.m_arrEdited.GetSize(); i ++)
	{
		pt = m_List.m_arrEdited.GetAt(i);
		CString strText = m_List.GetItemText(pt.x, pt.y);
		
		nIndex = strText.Find("*", 0);
		if (nIndex != -1)
		{
			strText = strText.Left(nIndex);
			m_List.SetItemText(pt.x, pt.y, strText);

			int iRowFlow = m_List.GetItemData(pt.x);
			flow.SetFieldValue(iRowFlow, 5, QByteArray(strText.GetBuffer(0)));
			pTagOutState->SetFieldValue(nTagOutRowIdx, "reverse1", flow);
		}
	}

	pTagOutStore->SaveState("c:/tb_sys_config.txt");
	
	m_List.ClearEdited();
	((CWnd*)GetDlgItem(IDC_BTN_SAVE))->EnableWindow(FALSE);
}

/*************************************************************
 �� �� OnClickList1()
 ���ܸ�Ҫ����Ӧ����¼�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B8A03AB
void DLGMarked::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	CXJUserStore *pUserStore = CXJUserStore::GetInstance();
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTSetDataTable *pPTValvSetData = pTagOutStore->GetPTSetData();

	CComboBox *pCMB = ((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON));
	int nIndex=pCMB->GetCurSel();
	CString str;
	pCMB->GetLBText(nIndex, str);
	int nReasonType = pTagOutState->GetTagOutReasonType(str.GetBuffer(0));
	QByteArray baReasonName = str.GetBuffer(0);
	QByteArrayMatrix &flow = pTagOutState->GetWorkFlow(nReasonType);

	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	int nSubItem = pNMListView->iSubItem;

	int nRowIdx = static_cast<int> (pLVCD->nmcd.dwItemSpec); //������
	int nColIdx = pLVCD->iSubItem; //������

	if(nItem != -1)
	{
		if(nSubItem == 2)
		{
			//AfxMessageBox("hello");
			//����ֵ��

			int iRowFlow = m_List.GetItemData(nItem);
			int nGroupType = flow.GetFieldValue(iRowFlow, 4).toInt();
			int nStateID = flow.GetFieldValue(iRowFlow, 2).toInt();

			if (pTagOutState->GetType() == XJ_TAGOUT_UNDEFINE ||
				!pTagOutState->GetLogs().contains(nStateID)){
				m_List.ListSubItem(nItem, nSubItem, pUserStore->BuildComboxUserList(nGroupType).constData());
			}
			//AfxMessageBox(QByteArray::number(nGroupType).constData());
			//AfxMessageBox(QByteArray::number(nStateID).constData());
		}

		if (m_List.m_arrEdited.GetSize() > 0){
			((CWnd*)GetDlgItem(IDC_BTN_SAVE))->EnableWindow(TRUE);
		}
	}

	*pResult = 0;
}

BOOL DLGMarked::CanMark()
{
	BOOL bReturn = TRUE;
	
	CString str;
    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
	CXJTagOutStore *pPTSetStore = CXJTagOutStore::GetInstance();
	pPTSetStore->ReLoadState();	
	QPTSetStateTable *pPTSetState = pPTSetStore->GetState();

	QByteArrayMatrix &log = pPTSetState->GetLogs();

	int nPTSetState = pPTSetState->GetStateID();
	
	// ״̬����δ����״̬
	if (XJ_OPER_UNHANGOUT != nPTSetState){
		CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(pPTSetState->GetPTID().constData(), TYPE_SEC);
		if (NULL != pObj){
			if (pObj == m_pObj){
				str.Format("װ���ѹ��ƣ�");
			}else{
				str.Format("��ǰ�Ѵ��ڹ��Ƶı���װ�ã� \n\n-----------------------\n"
					"����װ�ã�[ %s ] [ %s ] \n\n"
					"����ԭ��%s \n\n"
					"����ʱ�䣺%s \n\n"
					"ִ���û���%s"
					, pObj->m_pStation ? pObj->m_pStation->m_sName : ""
					, pObj->m_sName
					, pPTSetState->GetTypeName().constData()
					, pPTSetState->GetLog(XJ_OPER_HANGOUT).GetFieldValue(1, 1).constData()
					, pPTSetState->GetRunnerUserID().constData());
			}
			AfxMessageBox(str, MB_ICONWARNING);
		}
		
		return FALSE;
	}

	CString sInsert = "";
	((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON))->GetWindowText(sInsert);
	if (sInsert == "")
	{
		AfxMessageBox("��ѡ����������ԭ��.");

		return FALSE;
	}
	
	return bReturn;
}

BOOL DLGMarked::CanUnMark()
{
	BOOL bReturn = TRUE;
	
	CXJBrowserApp *pApp = (CXJBrowserApp *)AfxGetApp();
	CXJTagOutStore *pStore = CXJTagOutStore::GetInstance();
	pStore->ReLoadState();	
	QPTSetStateTable *pState = pStore->GetState();
	
	QByteArrayMatrix &log = pState->GetLogs();
	
	int nPTSetState = pState->GetStateID();
	int nFlag = pState->GetFlags();
	int nType = pState->GetType();
	CString sRunnerUserID = pState->GetRunnerUserID().constData();

	if (XJ_OPER_UNHANGOUT == nPTSetState || CString(pState->GetPTID().constData()) != m_pObj->m_sID){
		CString str;
		str.Format("��װ��δ���ƣ�");
		AfxMessageBox(str);
		
		return FALSE;
	}
	
	if (sRunnerUserID != pApp->m_User.m_strUSER_ID){
		CString str;
		str.Format("��װ������Ȩ���û� [%s]�����޷�����ȡ��������", sRunnerUserID);
		AfxMessageBox(str);
		
		return FALSE;
	}

	if (XJ_OPER_PTVALVSET_STATE_4 == nPTSetState){
		CString str;

		if (nFlag == 2){
			if (XJ_TAGOUT_PTVALVSET == nType)
				str.Format("��װ�ã�����Ա�����·���ֵ�޸�ִ����������ʱ�޷�ȡ�����ƣ����Ժ����ԣ�", sRunnerUserID);
			else if (XJ_TAGOUT_PTZONESET == nType)
				str.Format("��װ�ã�����Ա�����·���ֵ���л�ִ����������ʱ�޷�ȡ�����ƣ����Ժ����ԣ�", sRunnerUserID);
			else if (XJ_TAGOUT_PTSOFTSET == nType)
				str.Format("��װ�ã�����Ա�����·���ѹ��Ͷ��ִ����������ʱ�޷�ȡ�����ƣ����Ժ����ԣ�", sRunnerUserID);

			AfxMessageBox(str, MB_OK | MB_ICONWARNING);

			return FALSE;
		}else{
			/*str.Format("��װ�ã�������Ա��δ�·���ֵ�޸ģ���ȷ��Ҫȡ��������", sRunnerUserID);
			int n = AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION);
			if (n == IDYES){
				//AfxMessageBox("yes");
			}else{
				
				return;
			}*/
		}
	}
	
	return TRUE;
}

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
	
	//����װ���豸��
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
	


	//store->Next(1, pApp->m_User.m_strUSER_ID);

	//pApp->NextPTSetModState(m_bMark ? 1 : 0, zone, pApp->m_User.m_strUSER_ID);
	
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

	if (m_List.m_arrEdited.GetSize() > 0){
		int nID = AfxMessageBox("����δ���棬 ��ȷ��Ҫ������", MB_YESNO);
		if (nID == IDNO){
			return;
		}
		
		m_List.ClearEdited();
		((CWnd*)GetDlgItem(IDC_BTN_SAVE))->EnableWindow(FALSE);
	}

	FillData();
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
	InitListStyle();
	
	// TODO: Add extra initialization here
	InitComBox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int DLGMarked::InitListStyle()
{
	//��ͼ���б�, ���������и�
	CImageList  m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	//�Ȳ���UIConfig�����ļ����Ƿ��д��б������Ϣ
	//�����,�������ļ�����Ϣ��������
	//���û��,��Ĭ����������,�����浽�����ļ�
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_LEFT;
	{
		//��������ʧ��
		//char* arColumn[11]={"���","��ֵ����", "��ֵ����", "��λ", "��׼ֵ", "���", "��Ŀ��","����", "��Сֵ/���ֵ", "����", "��������"};
		//�����п�,��С����
		for (int nCol=0; nCol < 4 ; nCol++)
		{
			lvCol.iSubItem=nCol;
			CString colName = "";
			switch(nCol)
			{
			case 0://
				lvCol.cx = 50; // ���
				colName = StringFromID(IDS_COMMON_NO);
				break;
			case 1://
				lvCol.cx = 220; // ��������
				colName = "��������";
				break;
			case 2://
				lvCol.cx = 100; // ִ���û�
				colName = "ִ���û�";
				break;
			case 3:
				lvCol.cx = 60; // �Ƿ�����
				colName = "����";
				break;
			default:
				lvCol.cx = 100;
				break;
			}
			lvCol.pszText=colName.GetBuffer(1);
			int result = m_List.InsertColumn(nCol,&lvCol);
		}
		//Ĭ�����ص�һ��(���)
		//m_List.SetColumnHide(0, TRUE);
		m_List.SetColumnHide(3, TRUE);
	}
	
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	return 0;
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

void DLGMarked::FillData()
{
	CXJTagOutStore *pTagOutStore = CXJTagOutStore::GetInstance();
	QPTSetStateTable *pTagOutState = pTagOutStore->GetState();
	QPTSetDataTable *pPTValvSetData = pTagOutStore->GetPTSetData();
	
	m_List.SetRedraw(FALSE);
	m_List.DeleteAllItems();
	m_List.SetRedraw(TRUE);

	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//EnterCriticalSection(&m_CriticalOper); 

	CComboBox *pCMB = ((CComboBox*)GetDlgItem(IDC_CMB_MARKREASON));
	int nIndex=pCMB->GetCurSel();
	CString str;
	pCMB->GetLBText(nIndex, str);
	
	int nReasonType = pTagOutState->GetTagOutReasonType(str.GetBuffer(0));
	QByteArray baReasonName = str.GetBuffer(0);
	
	//AfxMessageBox(pTagOutState->GetTagOutReasonName(nReasonType).constData());

	QByteArrayMatrix &flow = pTagOutState->GetWorkFlow(nReasonType);
	//AfxMessageBox(flow.constData());

	nIndex = 0;
	int nRowCount = flow.GetRowCount();
	for(int i = 1; i <= nRowCount; i ++)
	{
		CString str;
		
		int nTagOutType = flow.GetFieldValue(i, 1).toInt();
		int nStateID = flow.GetFieldValue(i, 2).toInt();
		int nEnable = flow.GetFieldValue(i, 3).toInt();
		QByteArray &usrGroupID = flow.GetFieldValue(i, 4);
		QByteArray &usrID = flow.GetFieldValue(i, 5);
		QByteArray typeName;// = pTagOutStore->GetSubFuncID(nStateID);
		typeName << baReasonName << " : " << pTagOutStore->GetSubFuncID(nStateID);

		if (nStateID == XJ_OPER_HANGOUT || nStateID == XJ_OPER_UNHANGOUT)
			continue;

		if (nEnable == 0)
			continue;

		str.Format("%d", nIndex+1);
		m_List.InsertItem(nIndex, str); //ID

		m_List.SetItemText(nIndex, 1, typeName.constData()); // ��������

		m_List.SetItemText(nIndex, 2, usrID); // ִ���û�
		
		m_List.SetItemText(nIndex, 3, QByteArray::number(nEnable).constData()); // �Ƿ�����

		m_List.SetItemData(nIndex, (DWORD)i);
		
		nIndex++;
	}
	//�ָ�ˢ��
	//LeaveCriticalSection(&m_CriticalOper);
	m_List.SetRedraw(TRUE);
	
	
	//WriteLog("CDlgCheckPro::FillListData, ����", XJ_LOG_LV3);
}