// XJPTSetStore.cpp : implementation file
//

#include "stdafx.h"
#include "XJUtilsStore.h"

#include "qmemtable.h"

#include "XJBrowser.h"


// 用户类存储私有数据
class CXJUtilsStorePrivate 
{
public:
	CXJUtilsStorePrivate();
    ~CXJUtilsStorePrivate();
	
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
// CXJUtilsStorePrivate
// 
CXJUtilsStorePrivate::CXJUtilsStorePrivate()
{
}

CXJUtilsStorePrivate::~CXJUtilsStorePrivate()
{
}

BOOL CXJUtilsStorePrivate::ReLoad()
{
	BOOL bReturn = FALSE;

	return bReturn;
}

BOOL CXJUtilsStorePrivate::Save(const char *pszFilePath/* = NULL*/)
{
	return TRUE;
}

QByteArray CXJUtilsStorePrivate::GetUserGroupName(const char* pszID, int nIDType/* = 0*/)
{
	QByteArray s;

	if (0 == nIDType){

	}else if (1 == nIDType){
		
	}

	return s;
}

QByteArray CXJUtilsStorePrivate::GetUserGroupName(QByteArray &baID, int nIDType/* = 0*/)
{
	QByteArray s;
	
	if (0 == nIDType){
		
	}else if (1 == nIDType){

	}
	
	return s;
}

////////////////////////////////////////////////////////////
// CXJUtilsStore
// 
CXJUtilsStore *CXJUtilsStore::m_pInstance = NULL;

CXJUtilsStore *CXJUtilsStore::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CXJUtilsStore;
		m_pInstance->Register(m_pInstance);
	}
	return m_pInstance;
}

void CXJUtilsStore::ReleaseInstance()
{
	m_pInstance->UnRegister(m_pInstance);
	DELETE_POINTER(m_pInstance);
}

CXJUtilsStore::CXJUtilsStore()
	: d_ptr(new CXJUtilsStorePrivate)
{
}

CXJUtilsStore::~CXJUtilsStore()
{
	DELETE_POINTER(d_ptr);
}

BOOL CXJUtilsStore::ReLoad()
{
	BOOL bReturn = FALSE;
	if (NULL == d_ptr)
		return bReturn;

	return d_ptr->ReLoad();
}

BOOL CXJUtilsStore::Save(const char *pszFilePath)
{
	if (NULL == d_ptr)
		return FALSE;

	return d_ptr->Save(pszFilePath);
}

QByteArray CXJUtilsStore::GetComputerName()
{
	if (NULL == d_ptr)
		return QByteArray();

	DWORD nSize = MAX_COMPUTERNAME_LENGTH + 1; 
	char szComputer[MAX_COMPUTERNAME_LENGTH+1]; 
	memset( szComputer, 0, sizeof(szComputer) );
	::GetComputerName( szComputer, &nSize );
	
	return QByteArray(szComputer);
}
