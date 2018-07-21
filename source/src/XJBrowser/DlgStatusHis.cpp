// DlgStatusHis.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgStatusHis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStatusHis dialog


CDlgStatusHis::CDlgStatusHis(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStatusHis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStatusHis)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nType = 0;
	m_EventManager.ClearAllEvents();
	m_pSec = NULL;
}


void CDlgStatusHis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStatusHis)
	DDX_Control(pDX, IDC_LIST_PTSTATUS_HIS, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStatusHis, CDialog)
	//{{AFX_MSG_MAP(CDlgStatusHis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgStatusHis::~CDlgStatusHis()
{
	//���
	m_EventManager.ClearAllEvents();
}
/////////////////////////////////////////////////////////////////////////////
// CDlgStatusHis message handlers

BOOL CDlgStatusHis::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//���ͱ�������Чֵ
	if(m_nType != 1 && m_nType != 2)
		return FALSE;
	//�豸������Ч
	if(m_pSec == NULL)
		return FALSE;
	//���ô�������
	CString strCaption = "";
	if (m_nType == 1)
		strCaption = m_pSec->m_sName + StringFromID(IDS_COMMUSTATUS_HISTORY);
	else if (m_nType == 2)
		strCaption = m_pSec->m_sName + StringFromID(IDS_RUNSTATUS_HISTORY);
	SetWindowText(strCaption);

	//��ʼ���б�
	InitListStyle();
	//��ѯ����
	QueryData();
	//�������
	FillList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 �� �� ��: QueryCommHis()
 ���ܸ�Ҫ: ��ѯ��ʷͨѶ״̬
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDlgStatusHis::QueryCommHis()
{
	//���������Ч��
	if(m_pSec == NULL)
		return;
	//��վ�����
	m_EventManager.ClearAllEvents();

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//�ֶ�,�����ֶ�
	
	//����
	//PT_ID
	Condition condition1;
	str.Format("pt_id='%s'", m_pSec->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//����������ʱ���С����
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by ID DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition2);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_COMMU_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDlgStatusHis::QueryCommHis, ��ѯʧ��");
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
			str.Format("CDlgStatusHis::QueryCommHis, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventPTComm* pEvent = new CXJEventPTComm;
			if(pEvent->LoadFromDB(pMemset))
			{
				//���뵽����
				m_EventManager.AddEvent(pEvent);
			}
			else
			{
				delete pEvent;
			}
			pEvent = NULL;
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDlgStatusHis::QueryCommHis,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

/*************************************************************
 �� �� ��: QueryRunHis()
 ���ܸ�Ҫ: ��ѯ��ʷ����״̬
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDlgStatusHis::QueryRunHis()
{
	//���������Ч��
	if(m_pSec == NULL)
		return;
	//��վ�����
	m_EventManager.ClearAllEvents();
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//�ֶ�,�����ֶ�
	
	//����
	//PT_ID
	Condition condition1;
	str.Format("pt_id='%s'", m_pSec->m_sID);
	pApp->m_DBEngine.SetCondition(sql, condition1, str);

	//����������ʱ���С����
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	str.Format("order by ID DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition2);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_RUN_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLog("CDlgStatusHis::QueryRunHis, ��ѯʧ��");
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
			str.Format("CDlgStatusHis::QueryRunHis, ��ȡ��%d������", nCount);
			WriteLog(str);
		}
		for(int i = 0; i < nCount; i++)
		{
			//�����¼�
			CXJEventPTRun* pEvent = new CXJEventPTRun;
			if(pEvent->LoadFromDB(pMemset))
			{
				//���뵽����
				m_EventManager.AddEvent(pEvent);
			}
			else
			{
				delete pEvent;
			}
			pEvent = NULL;
			
			pMemset->MoveNext();
		}
	}
	else
	{
		CString str;
		str.Format("CDlgStatusHis::QueryRunHis,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
}

/*************************************************************
 �� �� ��: FillList()
 ���ܸ�Ҫ: ����б��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDlgStatusHis::FillList()
{
	//��m_EventManager�е���������б�
	//�������ʱ��ֹˢ��
	m_List.SetRedraw(FALSE);
	//ɾ���б�����
	m_List.DeleteAllItems();
	if(m_nType == 1) //ͨѶ״̬
	{
		CXJEventPTComm * pEvent = NULL;
		EVENT_LIST * pList = m_EventManager.GetEventList();
		if(pList != NULL)
		{
			for(int i = 0; i < pList->GetSize(); i++)
			{
				pEvent = (CXJEventPTComm*)pList->GetAt(i);
				AddPTCommToList(pEvent, i);
			}
		}
	}
	else if(m_nType == 2) //����״̬
	{
		CXJEventPTRun * pEvent = NULL;
		EVENT_LIST * pList = m_EventManager.GetEventList();
		if(pList != NULL)
		{
			for(int i = 0; i < pList->GetSize(); i++)
			{
				pEvent = (CXJEventPTRun*)pList->GetAt(i);
				AddPTRunToList(pEvent, i);
			}
		}
	}
	
	//�ָ�ˢ��
	m_List.SetRedraw(TRUE);
}

/*************************************************************
 �� �� ��: QueryData()
 ���ܸ�Ҫ: ��ѯ����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDlgStatusHis::QueryData()
{
	if(m_nType == 1) //ͨѶ״̬
	{
		QueryCommHis();
	}
	else if(m_nType == 2) //����״̬
	{
		QueryRunHis();
	}
}

/*************************************************************
 �� �� ��: AddPTCommToList()
 ���ܸ�Ҫ: ��ָ���¼���䵽�б��
 �� �� ֵ: ���ɹ�����TRUE,���򷵻�FALSE
 ��    ��: param1 ָ���¼�
		   Param2 ָ�����б���е�λ��
**************************************************************/
BOOL CDlgStatusHis::AddPTCommToList( CXJEventPTComm* pEvent, int i )
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;
	
	//0:���
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;

	//1:ʱ��
	CString sTime;
	sTime.Format("%s", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	m_List.SetItemText(i, 1, sTime);

	//2:״̬
	m_List.SetItemText(i, 2, GetCommStatusString(pEvent->m_nStatus));
	
	return TRUE;
}

/*************************************************************
 �� �� ��: AddPTRunToList()
 ���ܸ�Ҫ: ��ָ���¼���䵽�б��
 �� �� ֵ: ���ɹ�����TRUE,���򷵻�FALSE
 ��    ��: param1 ָ���¼�
		   Param2 ָ�����б���е�λ��
**************************************************************/
BOOL CDlgStatusHis::AddPTRunToList( CXJEventPTRun* pEvent, int i )
{
	//�¼�Ϊ��, ����FALSE
	if(pEvent == NULL)
		return FALSE;
	
	//0:���
	CString sID;
	sID.Format("%d", pEvent->m_lID);
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, i, sID, 0, 0, 0, i) == -1)
		return FALSE;
	
	//1:ʱ��
	CString sTime;
	sTime.Format("%s", pEvent->m_tmTime.Format("%Y-%m-%d %H:%M:%S"));
	m_List.SetItemText(i, 1, sTime);
	
	//2:״̬
	m_List.SetItemText(i, 2, GetRunStatusString(pEvent->m_nStatus));
	
	return TRUE;
}

/*************************************************************
 �� �� ��: InitListStyle()
 ���ܸ�Ҫ: ��ʼ���б���
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDlgStatusHis::InitListStyle()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);

	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	//��������ʧ��
	//char* arColumn[3]={"���", "״̬�ı�ʱ��", "�ı��״̬"};
	
	//�����п�,��С����
	for (int nCol=0; nCol < 3 ; nCol++)
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
			lvCol.cx = 180; // ʱ��
			colName = StringFromID(IDS_STATUS_CHANGE_TIME);
			break;
		case 2://
			lvCol.cx = 100; // ״̬
			colName = StringFromID(IDS_STATUS_AFTER_CHANGE);
			break;
		default:
			lvCol.cx=50;//�п�100����
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_List.InsertColumn(nCol,&lvCol);
	}
	//Ĭ�����ص�һ��(���)
	m_List.SetColumnHide(0, TRUE);

	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
}