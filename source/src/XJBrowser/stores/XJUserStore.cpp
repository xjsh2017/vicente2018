// XJPTSetStore.cpp : implementation file
//

#include "stdafx.h"
#include "XJUserStore.h"

#include "qmemtable.h"

#include "XJBrowser.h"

// 用户表
class QUserTable : public QMemTable
{
public:
	QUserTable();
	~QUserTable();

public:	
	BOOL		ReLoad();
	BOOL		Save(const char *pszFilePath = NULL);

	QByteArray			GetName(const char* pszID, const char* pszGroupID);
	int					GetFlags(const char* pszID, const char* pszGroupID);
	QByteArray			GetOwner(const char* pszID, const char* pszGroupID);
	QByteArray			GetOwner(const char* pszID);

	void				SetFlags(const char *pszID, const char *pszGroupID, int nFlags);
	void				SetOwner(const char *pszID, const char *pszGroupID, QByteArray &owner);

	QByteArray			BuildComboxUserList(const char* pszUserGroupID);
};

// 用户组表
class QUserGroupTable : public QMemTable
{
public:
	QUserGroupTable();
	~QUserGroupTable();
	
public:	
	BOOL		ReLoad();
	BOOL		Save(const char *pszFilePath = NULL);

	QByteArray		GetName(const char* pszID);
};

// 用户类存储私有数据
class CXJUserStorePrivate 
{
public:
	CXJUserStorePrivate();
    ~CXJUserStorePrivate();
	
	QUserTable			m_table_user;
	QUserGroupTable		m_table_user_group;
	
	BOOL		ReLoad();
	BOOL		Save(const char *pszFilePath = NULL);

public:
	/*
	 *  @brief   	GetUserGroupName	 获取用户组ID名 
	 *  @param 		[In] pszID		const char*		ID
	 *  @param 		[In] nIDType	int				ID 类别: 0 - 用户ID，1 - 用户组ID
	 *  @return 	QByteArray	
	 */
	QByteArray			GetUserGroupName(const char* pszID, int nIDType = 0);
	QByteArray			GetUserGroupName(QByteArray &baID, int nIDType = 0);
	
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
	//Save("c:/tb_sys_user.txt");

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

QByteArray QUserTable::GetName(const char* pszID, const char* pszGroupID)
{
	QByteArrayMatrix keyVals;
	keyVals.AppendField(pszID, true);
	keyVals.AppendField(pszGroupID);
	int nValRowIdx = GetRowIndex(keyVals);
	// AfxMessageBox(QByteArray::number(nValRowIdx));
	return GetFieldValue(nValRowIdx, "name");
}

int QUserTable::GetFlags(const char* pszID, const char* pszGroupID)
{
	QByteArrayMatrix keyVals;
	keyVals.AppendField(pszID, true);
	keyVals.AppendField(pszGroupID);
	int nValRowIdx = GetRowIndex(keyVals);
	// AfxMessageBox(QByteArray::number(nValRowIdx));
	QByteArrayMatrix val = GetFieldValue(nValRowIdx, "notes");
	return val.GetFieldValue(1, 1).toInt();
}

QByteArray QUserTable::GetOwner(const char* pszID, const char* pszGroupID)
{
	return GetName(pszID, pszGroupID);
}

QByteArray QUserTable::GetOwner(const char* pszID)
{
	QByteArray s;
	int nRowCount = GetRowCount();

	for (int i = 1; i <= nRowCount; i++){
		if (QByteArray(pszID) != GetFieldValue(i, "user_id"))
			continue;

		return GetFieldValue(i, "name");
		break;
	}

	return s;
}

void QUserTable::SetFlags(const char *pszID, const char *pszGroupID, int nFlags)
{
	QByteArrayMatrix keyVals;
	keyVals.AppendField(pszID, true);
	keyVals.AppendField(pszGroupID);
	int nValRowIdx = GetRowIndex(keyVals);

	QByteArrayMatrix val = GetFieldValue(nValRowIdx, "notes");
	val.SetFieldValue(1, 1, QByteArray::number(nFlags));
	SetFieldValue(nValRowIdx, "notes", val);
}

void QUserTable::SetOwner(const char *pszID, const char *pszGroupID, QByteArray &owner)
{
	QByteArrayMatrix keyVals;
	keyVals.AppendField(pszID, true);
	keyVals.AppendField(pszGroupID);
	int nValRowIdx = GetRowIndex(keyVals);
	
	SetFieldValue(nValRowIdx, "name", owner);
}

QByteArray QUserTable::BuildComboxUserList(const char* pszUserGroupID)
{
	QByteArray s;

	for (int i = 1; i <= GetRowCount(); i++){
		QByteArray &group = GetFieldValue(i, 2);
		if (group != QByteArray(pszUserGroupID))
			continue;

		if (!s.isEmpty())
			s << ";";

		s << GetFieldValue(i, 1);
	}

	//AfxMessageBox(s.constData());

	return s;
}

////////////////////////////////////////////////////////////
// QUserGroupTable
//
QUserGroupTable::QUserGroupTable()
{
}

QUserGroupTable::~QUserGroupTable()
{
	
}

BOOL QUserGroupTable::ReLoad()
{
	BOOL bReturn = FALSE;
	
	LoadInfo("tb_sys_group");
	LoadDataAll();
	//Save("c:/tb_sys_group.txt");
	
	return bReturn;
}

BOOL QUserGroupTable::Save(const char *pszFilePath)
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

QByteArray QUserGroupTable::GetName(const char* pszID)
{
	QByteArrayMatrix keyVals;
	keyVals.AppendField(pszID, true);
	int nValRowIdx = GetRowIndex(keyVals);
	// AfxMessageBox(QByteArray::number(nValRowIdx));
	return GetFieldValue(nValRowIdx, "name");
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

	m_table_user.ReLoad();
	m_table_user_group.ReLoad();

	return bReturn;
}

BOOL CXJUserStorePrivate::Save(const char *pszFilePath/* = NULL*/)
{
	return m_table_user.Save(pszFilePath);
}

QByteArray CXJUserStorePrivate::GetUserGroupName(const char* pszID, int nIDType/* = 0*/)
{
	QByteArray s;

	if (0 == nIDType){

	}else if (1 == nIDType){
		
	}

	return s;
}

QByteArray CXJUserStorePrivate::GetUserGroupName(QByteArray &baID, int nIDType/* = 0*/)
{
	QByteArray s;
	
	if (0 == nIDType){
		
	}else if (1 == nIDType){

	}
	
	return s;
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

QByteArray CXJUserStore::GetUserGroupIDName(int nType/* = XJ_USERGROUP_RUNNER*/)
{
	QByteArray s;

	switch (nType){
	case XJ_USERGROUP_RUNNER:
		return QByteArray("运行用户");
	case XJ_USERGROUP_OPERATOR:
		return QByteArray("操作用户");
	case XJ_USERGROUP_MONITOR:
		return QByteArray("监护用户");
	}

	return s;
}

QByteArray CXJUserStore::GetUserGroupName(int nType/* = XJ_USERGROUP_RUNNER*/)
{
	QByteArray val = GetUserGroupIDName(nType);
	return d_ptr->m_table_user_group.GetName(val);
}

int CXJUserStore::GetUserFlags(const char* pszUserID, const char* pszUserGroupID)
{
	int nReturn = 0;
	if (NULL == d_ptr)
		return nReturn;

	return d_ptr->m_table_user.GetFlags(pszUserID, pszUserGroupID);
}

QByteArray CXJUserStore::GetUserOwner(const char* pszUserID, const char* pszUserGroupID)
{
	QByteArray s;
	if (NULL == d_ptr)
		return s;

	return d_ptr->m_table_user.GetOwner(pszUserID, pszUserGroupID);
}

QByteArray CXJUserStore::GetUserOwner(const char* pszUserID)
{
	QByteArray s;
	if (NULL == d_ptr)
		return s;
	
	return d_ptr->m_table_user.GetOwner(pszUserID);
}

void CXJUserStore::SetUserFlags(const char *szUserID, const char *szUserGroupID, int nFlags)
{
	if (NULL == d_ptr)
		return;
	
	d_ptr->m_table_user.SetFlags(szUserID, szUserGroupID, nFlags);
}

void CXJUserStore::SetUserOwner(const char *szUserID, const char *szUserGroupID, QByteArray &owner)
{
	if (NULL == d_ptr)
		return;
	
	d_ptr->m_table_user.SetOwner(szUserID, szUserGroupID, owner);
}

QByteArray CXJUserStore::BuildComboxUserList(const char* pszUserGroupID)
{
	if (NULL == d_ptr)
		return QByteArray();

	return d_ptr->m_table_user.BuildComboxUserList(pszUserGroupID);
}

QByteArray CXJUserStore::BuildComboxUserList(int nGroupType)
{
	if (NULL == d_ptr)
		return QByteArray();
	
	return d_ptr->m_table_user.BuildComboxUserList(GetUserGroupIDName(nGroupType).constData());
}
