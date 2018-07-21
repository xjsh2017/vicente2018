// DlgOCSet.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgOCSet.h"

// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOCSet dialog


//##ModelId=49B87BA40001
CDlgOCSet::CDlgOCSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOCSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOCSet)
	m_nCyc = 24;
	//}}AFX_DATA_INIT
	pView = NULL;
	m_tmLastCallTime = CTime::GetCurrentTime();
}


//##ModelId=49B87BA40009
void CDlgOCSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOCSet)
	DDX_Text(pDX, IDC_DLG_OCSET_CYC, m_nCyc);
	DDV_MinMaxInt(pDX, m_nCyc, 1, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOCSet, CDialog)
	//{{AFX_MSG_MAP(CDlgOCSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOCSet message handlers

//##ModelId=49B87BA4000F
BOOL CDlgOCSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReadData();
	if(pView == NULL)
		return FALSE;
	//ʱ��
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_OCSET_TIME);
	if(pTime == NULL)
		return FALSE;
	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//���÷�Χ
	CTime tmMin(1971, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	/*
	CTime tm = CTime::GetCurrentTime();
	CTimeSpan ts(0, 1, 0, 0);
 	CTime tmStart = tm + ts;
	*/
	//����ʱ��
 	//VERIFY(pTime->SetTime(&tmStart));
	VERIFY(pTime->SetTime(&m_tmLastCallTime));
	//����SPIN��Χ
	CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_DLG_OCSET_SPIN);
	if(pSpin == NULL)
		return FALSE;
	pSpin->SetRange(1, 99);
	//��ʼ������
//	m_nCyc = atoi(pView->m_sCyc);
	//��ʼ��������
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_OCSET_TYPE);
	if(pCmb == NULL)
		return FALSE;
	pCmb->AddString( StringFromID(IDS_CYCLE_HOUR));
	pCmb->AddString( StringFromID(IDS_CYCLE_DAY));
	pCmb->AddString( StringFromID(IDS_CYCLE_MONTH));
	pCmb->AddString( StringFromID(IDS_CYCLE_YEAR));
	//��ʼ��ѡ��
	if(m_nCyc <= 99)
		pCmb->SetCurSel(0);
	else if(m_nCyc > 99 && m_nCyc <= (99 * 24))
		pCmb->SetCurSel(1);
	else if(m_nCyc > (99 * 24) && m_nCyc <= (99*24*30))
		pCmb->SetCurSel(2);
	else if(m_nCyc > (99 * 24 * 30))
		pCmb->SetCurSel(3);

	//��ʼ���༭��
	int nSel = pCmb->GetCurSel();
	switch(nSel)
	{
	case 0: //Сʱ
		break;
	case 1: //��
		m_nCyc /= 24;
		break;
	case 2: //��
		m_nCyc /= (24*30);
		break;
	case 3: //��
		m_nCyc /= (24*365);
	}
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//##ModelId=49B87BA40011
void CDlgOCSet::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData(TRUE);
	//�ж�ʱ����Ч��
	CTime tmNew;
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DLG_OCSET_TIME);
	if(pTime == NULL)
		return;
	pTime->GetTime(tmNew);
	CTime tm = CTime::GetCurrentTime();
	if(tmNew < tm)
	{
		//�轫ʱ�������ڽ���ʱ��
		AfxMessageBox( StringFromID(IDS_SETTIME_FUTURE));
		return;
	}
	CString sTime = tmNew.Format("%Y-%m-%d %H:%M:%S");

	//����
	//���жϻ���
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_DLG_OCSET_TYPE);
	if(pCmb == NULL)
		return;
	int nOrder = 1;
	int nSel = pCmb->GetCurSel();
	if(nSel == 1)
	{
		//��
		nOrder = 24;
	}
	else if(nSel == 2)
	{
		//��(30��)
		nOrder = 24*30;
	}
	else if(nSel == 3)
	{
		//��(365��)
		nOrder = 365*24;
	}

	//�ж�����
	if(m_nCyc < 1 || m_nCyc >99)
	{
		//����������1-99֮��
		AfxMessageBox( StringFromID(IDS_SETCYCLE_RANGE));
		return;
	}
	int nCyc = m_nCyc*nOrder;

	CString sCyc;
	sCyc.Format("%d", nCyc);

	//������Ϣ��������
	if(pView != NULL)
	{
		pView->SetCyc(sTime, sCyc);
		pView->PostMessage(AUTOCALL_SET_CHANGE, 0, 0);
	}	
	CDialog::OnOK();
}


void CDlgOCSet::ReadData()
{
	
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		for (int i = 0; i < nCount; i++)
		{
			CString str = pMemset->GetValue((UINT)0);
			if (str == "OC_CYC")
			{
				CString nCyc = pMemset->GetValue((UINT)1);
				m_nCyc = atoi(nCyc);
			}
			else if (str == "OC_STARTTIME")
			{
				CString time = pMemset->GetValue((UINT)1);
				m_tmLastCallTime = StringToTime(time);
			}
			pMemset->MoveNext();
		}
	}
	
	delete[] sError;
	delete pMemset;
}
