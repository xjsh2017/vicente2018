// Event.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "EventManager.h"

//#include "DataEngine.h"		//为了把Event对象中的ID翻译成Name 
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
 函 数 名：MoveFirst()
 功能概要：当前事件指针移动到事件列表最前面
 返 回 值: void
**************************************************************/
//##ModelId=49B87B9A0257
void CXJEventManager::MoveFirst()
{
	ASSERT(m_EventList.GetSize()>0);
	m_nPos=0;
	m_pCurrentEvent=m_EventList.GetAt(m_nPos);
}

/*************************************************************
 函 数 名：MoveLast()
 功能概要：当前事件指针移动到事件列表最后面
 返 回 值: void
**************************************************************/
//##ModelId=49B87B9A0258
void CXJEventManager::MoveLast()
{
	ASSERT(m_EventList.GetSize()>0);
	m_nPos=m_EventList.GetSize()-1;
	m_pCurrentEvent=m_EventList.GetAt(m_nPos);
}

/*************************************************************
 函 数 名：MoveNext()
 功能概要：当前事件指针移动到下一条事件
 返 回 值: void
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
 函 数 名：MovePrev()
 功能概要：当前事件指针移动到前一条事件
 返 回 值: void
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
 函 数 名：MoveTo()
 功能概要：移动当前事件指针到指定位置
 返 回 值: void
 参    数：param1   指定位置
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
 函 数 名：ClearAllEvents()
 功能概要：清空事件列表, 释放每个事件对象资源
 返 回 值: 返回值说明
 参    数：param1   意义说明
           Param2   意义说明
**************************************************************/
//##ModelId=49B87B9A0265
void CXJEventManager::ClearAllEvents()
{
	//释放每个event资源
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
 函 数 名：RemoveAtEvent()
 功能概要：从内存和链表中删除指定事件
 返 回 值: void
 参    数：param1	要删除的事件
		   Param2
**************************************************************/
//##ModelId=49B87B9A0251
void CXJEventManager::RemoveAtEvent( CXJEvent* pEvent )
{
	//判断数据有效性
	if(pEvent == NULL)
		return;
	//查找在链表中的位置
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
