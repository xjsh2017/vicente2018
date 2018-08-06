// XJPTSetStore.cpp : implementation file
//

#include "stdafx.h"
#include "XJUserStore.h"

#include "qmemtable.h"

#include "XJBrowser.h"


class QUserTable : public QMemTable
{
public:
	QUserTable();
	~QUserTable();

public:	
	BOOL		ReLoad();
	BOOL		Save(const char *pszFilePath = NULL);

	int			GetUserFlags(const char* pszUserID, const char* pszUserGroupID);
	QByteArray	GetUserOwner(const char* pszUserID, const char* pszUserGroupID);

	void		SetUserFlags(const char *pszUserID, const char *pszUserGroupID, int nFlags);
	void		SetUserOwner(const char *pszUserID, const char *pszUserGroupID, QByteArray &owner);
};

class CXJUserStorePrivate 
{
public:
	CXJUserStorePrivate();
    ~CXJUserStorePrivate();
	
	QUserTable	m_user_table;
	
	BOOL		ReLoad();
	BOOL		Save(const char *pszFilePath = NULL);
	
};


////////////////////////////////////////////////////////////
// QUserTable
//
QUserTable::QUserTable()
{
}

QUserTable::~QUserTable()
{

}

BOOL QUserTable::ReLoad()
{
	BOOL bReturn = FALSE;
	
	LoadInfo("tb_sys_user");
	LoadDataAll();

	return bReturn;
}

BOOL QUserTable::Save(const char *pszFilePath)
{
	BOOL bReturn = FALSE;

//  	CString str;
// 
// 	int nFlag;
// 	QByteArray sOwner;
// 	Q_UNUSED(nFlag);
// 	Q_UNUSED(sOwner);
	
// 	int nFlag = GetUserFlags("op1", "操作用户");
// 	QByteArray sOwner = GetUserOwner("op1", "操作用户");
// 	
// 	
// 	str.Format(" Flag: %d\n Owner: %s"
// 		, nFlag
// 		, sOwner.constData());
// 	AfxMessageBox(str);
	
// 	SetUserFlags("op1", "操作用户", 1);
// 	SetUserOwner("op1", "操作用户", QByteArray("hello"));
// 	nFlag = GetUserFlags("op1", "操作用户");
// 	sOwner = GetUserOwner("op1", "操作用户");
// 	
// 	str.Format(" Flag: %d\n Owner: %s"
// 		, nFlag
// 		, sOwner.constData());
// 	AfxMessageBox(str);

	if (SaveData())
		bReturn = TRUE;
	
	if (NULL != pszFilePath)
		FWrite(pszFilePath);

	return bReturn;
}

int QUserTable::GetUserFlags(const char* pszUserID, const char* pszUserGroupID)
{
	QByteArrayMatrix keyVals;
	keyVals.AppendField(pszUserID, true);
	keyVals.AppendField(pszUserGroupID);
	int nValRowIdx = GetValueRowIndex(keyVals);
	// AfxMessageBox(QByteArray::number(nValRowIdx));
	QByteArrayMatrix val = GetFieldValue(nValRowIdx, "notes");
	return val.GetFieldValue(1, 1).toInt();
}

QByteArray QUserTable::GetUserOwner(const char* pszUserID, const char* pszUserGroupID)
{
	QByteArrayMatrix keyVals;
	keyVals.AppendField(pszUserID, true);
	keyVals.AppendField(pszUserGroupID);
	int nValRowIdx = GetValueRowIndex(keyVals);
	// AfxMessageBox(QByteArray::number(nValRowIdx));
	QByteArrayMatrix val = GetFieldValue(nValRowIdx, "notes");
	return val.GetFieldValue(1, 2);
}

void QUserTable::SetUserFlags(const char *pszUserID, const char *pszUserGroupID, int nFlags)
{
	QByteArrayMatrix keyVals;
	keyVals.AppendField(pszUserID, true);
	keyVals.AppendField(pszUserGroupID);
	int nValRowIdx = GetValueRowIndex(keyVals);

	QByteArrayMatrix val = GetFieldValue(nValRowIdx, "notes");
	val.SetFieldValue(1, 1, QByteArray::number(nFlags));
	SetFieldValue(nValRowIdx, "notes", val);
}

void QUserTable::SetUserOwner(const char *pszUserID, const char *pszUserGroupID, QByteArray &owner)
{
	QByteArrayMatrix keyVals;
	keyVals.AppendField(pszUserID, true);
	keyVals.AppendField(pszUserGroupID);
	int nValRowIdx = GetValueRowIndex(keyVals);
	
	QByteArrayMatrix val = GetFieldValue(nValRowIdx, "notes");
	val.SetFieldValue(1, 2, owner);
	SetFieldValue(nValRowIdx, "notes", val);
}


////////////////////////////////////////////////////////////
// CXJUserStorePrivate
// 
CXJUserStorePrivate::CXJUserStorePrivate()
{
}

CXJUserStorePrivate::~CXJUserStorePrivate()
{
}

BOOL CXJUserStorePrivate::ReLoad()
{
	BOOL bReturn = FALSE;

	m_user_table.ReLoad();

	return bReturn;
}

BOOL CXJUserStorePrivate::Save(const char *pszFilePath/* = NULL*/)
{
	return m_user_table.Save(pszFilePath);
}

////////////////////////////////////////////////////////////
// CXJUserStore
// 
CXJUserStore *CXJUserStore::m_pInstance = NULL;

CXJUserStore *CXJUserStore::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CXJUserStore;
		m_pInstance->Register(m_pInstance);
	}
	return m_pInstance;
}

void CXJUserStore::ReleaseInstance()
{
	m_pInstance->UnRegister(m_pInstance);
	DELETE_POINTER(m_pInstance);
}

CXJUserStore::CXJUserStore()
	: d_ptr(new CXJUserStorePrivate)
{
}

CXJUserStore::~CXJUserStore()
{
	DELETE_POINTER(d_ptr);
}

BOOL CXJUserStore::ReLoad()
{
	BOOL bReturn = FALSE;
	if (NULL == d_ptr)
		return bReturn;

	return d_ptr->ReLoad();
}

BOOL CXJUserStore::Save(const char *pszFilePath)
{
	if (NULL == d_ptr)
		return FALSE;

	return d_ptr->Save(pszFilePath);
}

int CXJUserStore::GetUserFlags(const char* pszUserID, const char* pszUserGroupID)
{
	int nReturn = 0;
	if (NULL == d_ptr)
		return nReturn;

	return d_ptr->m_user_table.GetUserFlags(pszUserID, pszUserGroupID);
}

QByteArray CXJUserStore::GetUserOwner(const char* pszUserID, const char* pszUserGroupID)
{
	QByteArray s;
	if (NULL == d_ptr)
		return s;

	return d_ptr->m_user_table.GetUserOwner(pszUserID, pszUserGroupID);
}

void CXJUserStore::SetUserFlags(const char *szUserID, const char *szUserGroupID, int nFlags)
{
	if (NULL == d_ptr)
		return;
	
	d_ptr->m_user_table.SetUserFlags(szUserID, szUserGroupID, nFlags);
}

void CXJUserStore::SetUserOwner(const char *szUserID, const char *szUserGroupID, QByteArray &owner)
{
	if (NULL == d_ptr)
		return;
	
	d_ptr->m_user_table.SetUserOwner(szUserID, szUserGroupID, owner);
}
