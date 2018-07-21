// Event.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "EventManager.h"

//#include "DataEngine.h"		//Ϊ�˰�Event�����е�ID�����Name 
//////////////////////////////////////////////////////////////////////////
//##ModelId=49B87B9A0224
CXJEventManager::CXJEventManager()
{
	m_nPos=0;
	m_nEventType =0; 
	m_pCurrentEvent=NULL;
	m_EventList.RemoveAll();
}

//##ModelId=49B87B9A0225
CXJEventManager::~CXJEventManager()
{
	ClearAllEvents();
}

/*************************************************************
 �� �� ����MoveFirst()
 ���ܸ�Ҫ����ǰ�¼�ָ���ƶ����¼��б���ǰ��
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B9A0257
void CXJEventManager::MoveFirst()
{
	ASSERT(m_EventList.GetSize()>0);
	m_nPos=0;
	m_pCurrentEvent=m_EventList.GetAt(m_nPos);
}

/*************************************************************
 �� �� ����MoveLast()
 ���ܸ�Ҫ����ǰ�¼�ָ���ƶ����¼��б������
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B9A0258
void CXJEventManager::MoveLast()
{
	ASSERT(m_EventList.GetSize()>0);
	m_nPos=m_EventList.GetSize()-1;
	m_pCurrentEvent=m_EventList.GetAt(m_nPos);
}

/*************************************************************
 �� �� ����MoveNext()
 ���ܸ�Ҫ����ǰ�¼�ָ���ƶ�����һ���¼�
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B9A0259
void CXJEventManager::MoveNext()
{
	ASSERT(m_EventList.GetSize()>0);
	ASSERT(m_nPos<m_EventList.GetSize()-1);
	m_nPos++;
	m_pCurrentEvent=m_EventList.GetAt(m_nPos);
} 

/*************************************************************
 �� �� ����MovePrev()
 ���ܸ�Ҫ����ǰ�¼�ָ���ƶ���ǰһ���¼�
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B9A025A
void CXJEventManager::MovePrev()
{
	ASSERT(m_EventList.GetSize()>0);
	ASSERT(m_nPos>0);
	m_nPos--;
	m_pCurrentEvent=m_EventList.GetAt(m_nPos);
}

/*************************************************************
 �� �� ����MoveTo()
 ���ܸ�Ҫ���ƶ���ǰ�¼�ָ�뵽ָ��λ��
 �� �� ֵ: void
 ��    ����param1   ָ��λ��
**************************************************************/
//##ModelId=49B87B9A0261
void CXJEventManager::MoveTo(int nPos)
{
	ASSERT(m_EventList.GetSize()>0);
	ASSERT( ( nPos>=0 ) || ( nPos<m_EventList.GetSize() ) );
	m_nPos=nPos;
	m_pCurrentEvent=m_EventList.GetAt(nPos);
}

/*************************************************************
 �� �� ����ClearAllEvents()
 ���ܸ�Ҫ������¼��б�, �ͷ�ÿ���¼�������Դ
 �� �� ֵ: ����ֵ˵��
 ��    ����param1   ����˵��
           Param2   ����˵��
**************************************************************/
//##ModelId=49B87B9A0265
void CXJEventManager::ClearAllEvents()
{
	//�ͷ�ÿ��event��Դ
	CXJEvent * pEvent;
	for(int i=0; i<m_EventList.GetSize(); i++)
	{
		pEvent = m_EventList.GetAt(i);
		if(pEvent != NULL)
		{
			delete pEvent;
			pEvent = NULL;
		}
	}
	m_EventList.RemoveAll();
}

/*************************************************************
 �� �� ����RemoveAtEvent()
 ���ܸ�Ҫ�����ڴ��������ɾ��ָ���¼�
 �� �� ֵ: void
 ��    ����param1	Ҫɾ�����¼�
		   Param2
**************************************************************/
//##ModelId=49B87B9A0251
void CXJEventManager::RemoveAtEvent( CXJEvent* pEvent )
{
	//�ж�������Ч��
	if(pEvent == NULL)
		return;
	//�����������е�λ��
	int nIndex = -1;
	for(int i = 0; i < m_EventList.GetSize(); i++)
	{
		CXJEvent* pObj = (CXJEvent*)m_EventList.GetAt(i);
		if(pObj == pEvent)
		{
			nIndex = i;
			break;
		}
	}
	if(nIndex > -1)
	{
		RemoveEvent(nIndex);
	}
}
