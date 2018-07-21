// ***************************************************************
//  EventManager   version:  1.0    date: 16:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 事件管理器,存储并浏览事件对象(CXJEvent).接口仿照CRecordset
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(EVENTMANAGER_H)
#define EVENTMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Event.h"

/** @brief           事件列表类型定义*/
//##ModelId=49B87B9A0214
typedef CTypedPtrArray<CPtrArray, CXJEvent*> EVENT_LIST;
/**
 * @defgroup 	CXJEventManager	事件管理类
 * @{
 */
/**
 * @brief       事件管理器,存储并浏览事件对象(CXJEvent).接口仿照CRecordset
*  @author      LYH
 * @version     ver1.0
 * @date        16/09/08
/** @} */ //OVER
//##ModelId=49B87B9A0222
class CXJEventManager: public CObject
{
public:
	
	//##ModelId=49B87B9A0224
	CXJEventManager();
	//##ModelId=49B87B9A0225
	~CXJEventManager();
	/** @brief           事件列表*/
	//##ModelId=49B87B9A0227
	EVENT_LIST	m_EventList;
	/** @brief           当前事件*/
	//##ModelId=49B87B9A0233
	CXJEvent* m_pCurrentEvent;
	/** @brief           包含的事件类型*/
	//##ModelId=49B87B9A0237
	int m_nEventType;
public:

/*
 *  @brief   	GetCount	 返回包含的事件数 
 *  @return 	inline int 包含的事件数
 */
	//##ModelId=49B87B9A0238
	inline int GetCount()
		{return m_EventList.GetSize();}	
/*
 *  @brief   	GetCurrentPos	 返回位置指针 
 *  @return 	inline int 当前事件位置
 */
	//##ModelId=49B87B9A0242
	inline int GetCurrentPos()
		{return m_nPos;}

/*
 *  @brief   	GetEventList	 返回事件列表 
 *  @return 	inline EVENT_LIST* 包含的事件列表
 */
	//##ModelId=49B87B9A0243
	inline EVENT_LIST* GetEventList()
		{return &m_EventList; }
	
/*
 *  @brief   	GetAtEvent	 获得指定位置的事件（指针） 
 *  @param 		[In]a param of Type  int  指定位置
 *  @return 	inline CXJEvent* 事件指针
 */
	//##ModelId=49B87B9A0244
	inline CXJEvent* GetAtEvent(int nIndex)		
		{return m_EventList.GetAt(nIndex);}

/*
 *  @brief   	AddEvent	 添加事件
 *  @param 		[In]a param of Type  CXJEvent *  要添加的事件指针
 *  @return 	inline int 新添加的事件的位置
 */
	//##ModelId=49B87B9A0246
	inline int AddEvent(CXJEvent *pEvent)	//添加事件 
		{ 	
			return m_EventList.Add(pEvent);
		}

/*
 *  @brief   	RemoveEvent	 删除指定索引的事件
 *  @param 		[In]a param of Type  int  位置索引
 *  @return 	inline void 
 */
	//##ModelId=49B87B9A0248
	inline void RemoveEvent(int nIndex)
		{
			delete m_EventList.GetAt(nIndex); 	
			m_EventList.RemoveAt(nIndex);
		}
	
/*
 *  @brief   	RemoveAtEvent	 从内存和链表中删除指定事件 
 *  @param 		[In]a param of Type  CXJEvent*  指定要删除的事件
 *  @return 	void 
 */
	//##ModelId=49B87B9A0251
	void RemoveAtEvent(CXJEvent* pEvent);

/*
 *  @brief   	InsertAtEvent	 插入事件到列表中的指定位置
 *  @param 		[In]a param of Type  int  指定要插入的位置
 *  @param 		[In]a param of Type  CXJEvent *  待插入的事件指针
 *  @param 		[In]a param of Type  int  要插入的次数
 *  @return 	inline void 
 */
	//##ModelId=49B87B9A0253
	inline void InsertAtEvent(int nIndex,CXJEvent *pEvent, int nCount = 1)
		{
			m_EventList.InsertAt(nIndex,pEvent,nCount); 
		}

/*
 *  @brief   	MoveFirst	 事件指针移到最前
 *  @return 	void 
 */
	//##ModelId=49B87B9A0257
	void MoveFirst();			

/*
 *  @brief   	MoveLast	 事件指针移到最末尾
 *  @return 	void 
 */
	//##ModelId=49B87B9A0258
	void MoveLast();

/*
 *  @brief   	MoveNext	 事件指针移到下一个 
 *  @return 	void 
 */
	//##ModelId=49B87B9A0259
	void MoveNext();

/*
 *  @brief   	MovePrev	 事件指针移到上一个
 *  @return 	void 
 */
	//##ModelId=49B87B9A025A
	void MovePrev();

/*
 *  @brief   	MoveTo	 事件指针移到指定位置 
 *  @param 		[In]a param of Type  int  指定位置
 *  @return 	void 
 */
	//##ModelId=49B87B9A0261
	void MoveTo(int nPos);		//事件指针移到指定位置

/*
 *  @brief   	IsEOF	 事件指针是否到事件列表的尾部 
 *  @return 	inline BOOL 已经到尾部返回TRUE, 未到尾部返回FALSE
 */
	//##ModelId=49B87B9A0263
	inline BOOL IsEOF() 
		{return ( m_nPos==m_EventList.GetSize()-1 );}

/*
 *  @brief   	IsBOF	 事件指针是否到事件列表的前部部
 *  @return 	inline BOOL 已经到前部返回TRUE, 未到前部返回FALSE
 */
	//##ModelId=49B87B9A0264
	inline BOOL IsBOF() 
		{return ( m_nPos==0);}

/*
 *  @brief   	ClearAllEvents	 删除包含的所有事件
 *  @return 	void 
 */
	//##ModelId=49B87B9A0265
	void ClearAllEvents();

	//显示事件属性窗口
//	int ShowEventProp(BOOL bModal=TRUE);		//显示m_EventList的属性框
private:
	//##ModelId=49B87B9A0266
	int m_nPos;	//当前事件在列表中的位置
};
#endif
