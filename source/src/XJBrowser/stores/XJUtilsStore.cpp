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
