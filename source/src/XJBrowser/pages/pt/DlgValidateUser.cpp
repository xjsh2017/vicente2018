// DlgValidateID.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgValidateUser.h"

#include "qbytearraymatrix.h"
#include "XJUserStore.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgValidateUser dialog


//##ModelId=49B87BA3035C
CDlgValidateUser::CDlgValidateUser(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgValidateUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgValidateUser)
	m_strPassword = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT
	m_nUserGroupType = -1;
	m_strUserGroup = "";
	m_strAuthUserID = "";
	m_strFuncID = "";
	m_nValidateType = -1;
	m_strComUserID = "";
	m_nFuncID = -1;
}


CDlgValidateUser::CDlgValidateUser(int nUserGroupType, int nValidateType, CWnd* pParent /*=NULL*/)
: CDialog(CDlgValidateUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgValidateUser)
	m_strPassword = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT
	m_nUserGroupType = nUserGroupType;
	m_strUserGroup = "";
	m_strAuthUserID = "";
	m_strFuncID = "";
	m_nValidateType = nValidateType;
}

CDlgValidateUser::CDlgValidateUser(int nUserGroupType, CWnd* pParent /*=NULL*/)
: CDialog(CDlgValidateUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgValidateUser)
	m_strPassword = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT
	m_nUserGroupType = nUserGroupType;
	m_strFuncID = "";
	m_strAuthUserID = "";
}


//##ModelId=49B87BA30362
void CDlgValidateUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgValidateUser)
	DDX_Text(pDX, IDC_EDT_PASSWORD, m_strPassword);
	//DDX_Text(pDX, IDC_EDT_USERNAME, m_strUser);
	DDX_Text(pDX, IDC_CMB_USERNAME, m_strUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgValidateUser, CDialog)
	//{{AFX_MSG_MAP(CDlgValidateUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgValidateUser message handlers

//##ModelId=49B87BA3036B
BOOL CDlgValidateUser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CXJUserStore *pUserStore = CXJUserStore::GetInstance();
	QByteArray &baGroupName = pUserStore->GetUserGroupName(m_nUserGroupType);

	switch (m_nFuncID){
//	case XJ_FUNC_XJBROWSER_TAGOUT:
	case XJ_OPER_HANGOUT:
	case XJ_OPER_PTVALVSET_STATE_4:
	case XJ_OPER_PTZONESET_STATE_4:
	case XJ_OPER_PTSOFTSET_STATE_4:
		baGroupName = "挂牌员";
		break;
//	case XJ_FUNC_XJBROWSER_CONTROL:
	case XJ_OPER_PTVALVSET_STATE_2:
	case XJ_OPER_PTZONESET_STATE_2:
	case XJ_OPER_PTSOFTSET_STATE_2:
		baGroupName = "操作员";
		break;
	case XJ_FUNC_XJBROWSER_CUSTODY:
	case XJ_OPER_PTVALVSET_STATE_3:
	case XJ_OPER_PTZONESET_STATE_3:
	case XJ_OPER_PTSOFTSET_STATE_3:
		baGroupName = "监护员";
		break;
	default:
		baGroupName = "";
		break;
	}

	QByteArray title = baGroupName;
	title << "身份验证";
	SetWindowText(title.constData());

	// TODO: Add extra initialization here
	if (!m_strUser.IsEmpty() && m_strPassword != "")
	{
		//自动登录
#ifdef _DEBUG
		OnOK();
#endif
	}
	FillUserName2();

	if (!m_strAuthUserID.IsEmpty()){
		m_strUser = m_strAuthUserID;
		
		GetDlgItem(IDC_CMB_USERNAME)->EnableWindow(FALSE);
	}
	
	UpdateData(FALSE);
	
// 	if (!m_strUser.IsEmpty()){
// 		GetDlgItem(IDC_CMB_USERNAME)->PostMessage(WM_KILLFOCUS, 0, 0);
// 		GetDlgItem(IDC_EDT_PASSWORD)->SetFocus();
// 	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/****************************************************
Date:2018/08/6  Author:WPS
函数名:   FillUserName	
返回值:   void	
功能概要: 
*****************************************************/
void CDlgValidateUser::FillUserName()
{
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_USERNAME);
	if(pCmb == NULL)
		return;
	pCmb->ResetContent();
	//pCmb->ModifyStyle(CBS_DROPDOWN, CBS_DROPDOWNLIST);
	((CEdit*)pCmb->GetWindow(GW_CHILD))->SetReadOnly();
	//身份验证
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	CXJUserStore *pUserStore = CXJUserStore::GetInstance();
	QByteArrayMatrix arrlist = pUserStore->BuildComboxUserList(m_nUserGroupType);
	arrlist.SetDelimRow(";");
	
	for (int i = 1; i <= arrlist.GetRowCount(); i++){
		pCmb->AddString(arrlist.GetFieldValue(i, 1).constData());
	}
}
void CDlgValidateUser::FillUserName2()
{
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_USERNAME);
	if(pCmb == NULL)
		return;
	pCmb->ResetContent();
	((CEdit*)pCmb->GetWindow(GW_CHILD))->SetReadOnly();
	//身份验证
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	
	CXJUserStore *pUserStore = CXJUserStore::GetInstance();
	QByteArrayMatrix arrlist = pUserStore->BuildComboxUserList2(m_nFuncID);
	arrlist.SetDelimRow(";");
	
	for (int i = 1; i <= arrlist.GetRowCount(); i++){
		QByteArray &user_id = arrlist.GetFieldValue(i, 1);
		if (m_excludeUserList.valueContains(user_id))
			continue;

		pCmb->AddString(user_id.constData());
	}
}

//##ModelId=49B87BA3036D
void CDlgValidateUser::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	if(g_LoginFailTimes >= 3)
	{
		//登录失败3次
		CTime tmCur = CTime::GetCurrentTime();
		CTimeSpan tsTime = tmCur - g_LastFailTime;
		if(tsTime.GetTotalSeconds() > g_LockTime*60)
		{
			//已解除锁定
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

	if (!m_strAuthUserID.IsEmpty()){
		if (m_strUser != m_strAuthUserID){
			str.Format("授权用户已指定为：%s", m_strAuthUserID);
			AfxMessageBox(str);

			return;
		}
	}
	
	try
	{
		//身份验证
		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

		//用user_id, password查找group_id
		//组建查询条件
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();

		//字段
		//group_id
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "group_id", EX_STTP_DATA_TYPE_STRING);
		Field Field2;
		pApp->m_DBEngine.SetField(sql, Field2, "notes", EX_STTP_DATA_TYPE_STRING);

		CString str;
		//条件
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
				//没有查询到则表示用户名密码错误
				AfxMessageBox( StringFromID(IDS_VALIDATE_USERPWD_ERROR));
				GetDlgItem(IDC_EDT_USERNAME)->SetFocus();
				delete[] sError;
				delete pMemset;
				return;
			}
			else
			{
				//只应该有一个值
				m_strUserGroup = pMemset->GetValue((UINT)0); //用户组ID

				CString str;

				g_LoginFailTimes = 0;

				if(m_strFuncID != "")
				{
					if(m_strUser == pApp->m_User.m_strUSER_ID)
					{
						/*
						CString str;
						str.Format("监护员不能为当前用户!");
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
						//要验证功能权限
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
			//查询数据库失败,视为数据库通信有问题
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

void CDlgValidateUser::OnCancel() 
{
	if(!UpdateData())
		return;

	EndDialog(IDCANCEL);
}
