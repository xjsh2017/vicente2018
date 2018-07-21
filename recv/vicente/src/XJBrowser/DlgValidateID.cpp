// DlgValidateID.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgValidateID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgValidateID dialog


//##ModelId=49B87BA3035C
CDlgValidateID::CDlgValidateID(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgValidateID::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgValidateID)
	m_strPassword = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT
	m_strUserGroup = "";
	m_strFuncID = "";
	m_nPersonType = 0;
	m_strComUserID = "";
}


CDlgValidateID::CDlgValidateID(int nPersonType, CWnd* pParent /*=NULL*/)
: CDialog(CDlgValidateID::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgValidateID)
	m_strPassword = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT
	m_strUserGroup = "";
	m_strFuncID = "";
	m_nPersonType = nPersonType;
}


//##ModelId=49B87BA30362
void CDlgValidateID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgValidateID)
	DDX_Text(pDX, IDC_EDT_PASSWORD, m_strPassword);
	//DDX_Text(pDX, IDC_EDT_USERNAME, m_strUser);
	DDX_Text(pDX, IDC_CMB_USERNAME, m_strUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgValidateID, CDialog)
	//{{AFX_MSG_MAP(CDlgValidateID)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgValidateID message handlers

//##ModelId=49B87BA3036B
BOOL CDlgValidateID::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (m_nPersonType ==1)
	{
		SetWindowText( StringFromID(IDS_VALIDATE_GUARDIAN));
	}
	else if (m_nPersonType ==2)
	{
		SetWindowText( StringFromID(IDS_VALIDATE_OPERATOR));
	}
	// TODO: Add extra initialization here
	if (!m_strUser.IsEmpty() && m_strPassword != "")
	{
		//�Զ���¼
#ifdef _DEBUG
		OnOK();
#endif
	}
	FillUserName();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//##ModelId=49B87BA3036D
void CDlgValidateID::OnOK() 
{
	// TODO: Add extra validation here
	if(g_LoginFailTimes >= 3)
	{
		//��¼ʧ��3��
		CTime tmCur = CTime::GetCurrentTime();
		CTimeSpan tsTime = tmCur - g_LastFailTime;
		if(tsTime.GetTotalSeconds() > g_LockTime*60)
		{
			//�ѽ������
			g_LoginFailTimes = 0;
			CTime tm(1971, 1,1,1,1,1);
			g_LastFailTime = tm;
		}
		else
		{
			AfxMessageBox( StringFromID(IDS_VALIDATE_LOCKED));
			return;
		}
	}
	if(!UpdateData())
		return;
	if(m_strUser.IsEmpty())
	{
		AfxMessageBox(IDS_VALIDATE_INPUT_USER);
		GetDlgItem(IDC_EDT_USERNAME)->SetFocus();
		return;
	}
	
	if(m_strPassword.IsEmpty())
	{
		AfxMessageBox(IDS_VALIDATE_INPUT_PWD);
		GetDlgItem(IDC_EDT_PASSWORD)->SetFocus();
		return;
	}
	
	try
	{
		//�����֤
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

		//��user_id, password����group_id
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		//�ֶ�
		//group_id
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "group_id", EX_STTP_DATA_TYPE_STRING);

		CString str;
		//����
		//USER_ID
		Condition condition2;
		str.Format("USER_ID = '%s'", m_strUser);
		pApp->m_DBEngine.SetCondition(sql, condition2, str);
		
		//PASSWORD
		Condition condition3;
		str.Format("PASSWORD = '%s'", m_strPassword);
		pApp->m_DBEngine.SetCondition(sql, condition3, str);

		CMemSet* pMemset;
		pMemset = new CMemSet;
		
		char * sError = new char[MAXMSGLEN];
		memset(sError, '\0', MAXMSGLEN);
		
		int nResult;
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_USER_CFG, sql, sError, pMemset);
		if(pMemset != NULL && nResult == 1)
		{
			pMemset->MoveFirst();
			int nCount = pMemset->GetMemRowNum();
			if(nCount <= 0)
			{
				g_LoginFailTimes++;
				g_LastFailTime = CTime::GetCurrentTime();
				//û�в�ѯ�����ʾ�û����������
				AfxMessageBox( StringFromID(IDS_VALIDATE_USERPWD_ERROR));
				GetDlgItem(IDC_EDT_USERNAME)->SetFocus();
				delete[] sError;
				delete pMemset;
				return;
			}
			else
			{
				//ֻӦ����һ��ֵ
				m_strUserGroup = pMemset->GetValue((UINT)0); //�û���ID

				g_LoginFailTimes = 0;

				if(m_strFuncID != "")
				{
					if(m_strUser == pApp->m_User.m_strUSER_ID)
					{
						/*
						CString str;
						str.Format("�໤Ա����Ϊ��ǰ�û�!");
						AfxMessageBox(str);
						delete[] sError;
						delete pMemset;
						return;
						*/
					}
					if(m_strUser == m_strComUserID)
					{
						AfxMessageBox( StringFromID(IDS_VALIDATE_MULTI_IDENTITY));
						delete[] sError;
						delete pMemset;
						return;
					}
					if (g_bVerify)
					{
						//Ҫ��֤����Ȩ��
						if(!pApp->TryEnter(m_strFuncID, m_strUser, FALSE))
						{
							CString str;
							str.Format("%s:%s", StringFromID(IDS_VALIDATE_AUTHORITY), m_strFuncID);
							AfxMessageBox(str);
							delete[] sError;
							delete pMemset;
							return;
						}
					}
				}
			}
		}
		else
		{
			//��ѯ���ݿ�ʧ��,��Ϊ���ݿ�ͨ��������
			CString str;
			str.Format("%s", sError);
			AfxMessageBox(str);
			delete[] sError;
			delete pMemset;
			return;
		}

		delete[] sError;
		delete pMemset;
	}
	catch(...)
	{
		return;
	}
	EndDialog(IDOK);
}

/****************************************************
Date:2012/12/6  Author:LYH
������:   FillUserName	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CDlgValidateID::FillUserName()
{
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_USERNAME);
	if(pCmb == NULL)
		return;
	pCmb->ResetContent();
	//�����֤
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	//��user_id, password����group_id
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//group_id
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "user_id", EX_STTP_DATA_TYPE_STRING);
	
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_USER_CFG, sql, sError, pMemset);
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		char ch0 = '0';
		char ch9 = '9';
		
		for(int i = 0; i < nCount; i++)
		{
			BOOL bVaild = TRUE;
			CString sName = pMemset->GetValue((UINT)0);
			if(sName.GetLength() == 1)
			{
				char a = sName.GetAt(0);
				if(a >= ch0 && a <= ch9)
					bVaild = FALSE;
			}
			if(bVaild)
				pCmb->AddString(sName);
			pMemset->MoveNext();
		}
		
	}
	else
	{
		//��ѯ���ݿ�ʧ��,��Ϊ���ݿ�ͨ��������
		CString str;
		str.Format("��ѯ�û���ʧ��:%s", sError);
		WriteLog(str);
		delete[] sError;
		delete pMemset;
		return;
	}
	
	delete[] sError;
	delete pMemset;
}
