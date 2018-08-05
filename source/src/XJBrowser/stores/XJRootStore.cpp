// XJRootStore.cpp : implementation file
//

#include "StdAfx.h"
#include "XJRootStore.h"


list<CXJRootStore*> CXJRootStore::m_lstInstantce = list<CXJRootStore*>();

CXJRootStore::CXJRootStore(void)
{
}

CXJRootStore::~CXJRootStore(void)
{
} 

void CXJRootStore::Register(CXJRootStore *pInstance)
{
	if (find(m_lstInstantce.begin(), m_lstInstantce.end(), pInstance) == m_lstInstantce.end())
		m_lstInstantce.push_back(pInstance);
}

void CXJRootStore::UnRegister(CXJRootStore *pInstance)
{
	list<CXJRootStore*>::iterator it;
	for (it = m_lstInstantce.begin(); it != m_lstInstantce.end(); ++it)
	{
		if ((*it) == pInstance)
		{
			m_lstInstantce.erase(it);
			return;
		}
	}
}

void CXJRootStore::Release()
{
	list<CXJRootStore*>::iterator it;
	for (it = m_lstInstantce.begin(); it != m_lstInstantce.end(); ++it)
	{
		DELETE_POINTER(*it);
	}
	m_lstInstantce.clear();
}
