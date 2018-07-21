// ***************************************************************
//  EventManager   version:  1.0    date: 16:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: �¼�������,�洢������¼�����(CXJEvent).�ӿڷ���CRecordset
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

/** @brief           �¼��б����Ͷ���*/
//##ModelId=49B87B9A0214
typedef CTypedPtrArray<CPtrArray, CXJEvent*> EVENT_LIST;
/**
 * @defgroup 	CXJEventManager	�¼�������
 * @{
 */
/**
 * @brief       �¼�������,�洢������¼�����(CXJEvent).�ӿڷ���CRecordset
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
	/** @brief           �¼��б�*/
	//##ModelId=49B87B9A0227
	EVENT_LIST	m_EventList;
	/** @brief           ��ǰ�¼�*/
	//##ModelId=49B87B9A0233
	CXJEvent* m_pCurrentEvent;
	/** @brief           �������¼�����*/
	//##ModelId=49B87B9A0237
	int m_nEventType;
public:

/*
 *  @brief   	GetCount	 ���ذ������¼��� 
 *  @return 	inline int �������¼���
 */
	//##ModelId=49B87B9A0238
	inline int GetCount()
		{return m_EventList.GetSize();}	
/*
 *  @brief   	GetCurrentPos	 ����λ��ָ�� 
 *  @return 	inline int ��ǰ�¼�λ��
 */
	//##ModelId=49B87B9A0242
	inline int GetCurrentPos()
		{return m_nPos;}

/*
 *  @brief   	GetEventList	 �����¼��б� 
 *  @return 	inline EVENT_LIST* �������¼��б�
 */
	//##ModelId=49B87B9A0243
	inline EVENT_LIST* GetEventList()
		{return &m_EventList; }
	
/*
 *  @brief   	GetAtEvent	 ���ָ��λ�õ��¼���ָ�룩 
 *  @param 		[In]a param of Type  int  ָ��λ��
 *  @return 	inline CXJEvent* �¼�ָ��
 */
	//##ModelId=49B87B9A0244
	inline CXJEvent* GetAtEvent(int nIndex)		
		{return m_EventList.GetAt(nIndex);}

/*
 *  @brief   	AddEvent	 ����¼�
 *  @param 		[In]a param of Type  CXJEvent *  Ҫ��ӵ��¼�ָ��
 *  @return 	inline int ����ӵ��¼���λ��
 */
	//##ModelId=49B87B9A0246
	inline int AddEvent(CXJEvent *pEvent)	//����¼� 
		{ 	
			return m_EventList.Add(pEvent);
		}

/*
 *  @brief   	RemoveEvent	 ɾ��ָ���������¼�
 *  @param 		[In]a param of Type  int  λ������
 *  @return 	inline void 
 */
	//##ModelId=49B87B9A0248
	inline void RemoveEvent(int nIndex)
		{
			delete m_EventList.GetAt(nIndex); 	
			m_EventList.RemoveAt(nIndex);
		}
	
/*
 *  @brief   	RemoveAtEvent	 ���ڴ��������ɾ��ָ���¼� 
 *  @param 		[In]a param of Type  CXJEvent*  ָ��Ҫɾ�����¼�
 *  @return 	void 
 */
	//##ModelId=49B87B9A0251
	void RemoveAtEvent(CXJEvent* pEvent);

/*
 *  @brief   	InsertAtEvent	 �����¼����б��е�ָ��λ��
 *  @param 		[In]a param of Type  int  ָ��Ҫ�����λ��
 *  @param 		[In]a param of Type  CXJEvent *  ��������¼�ָ��
 *  @param 		[In]a param of Type  int  Ҫ����Ĵ���
 *  @return 	inline void 
 */
	//##ModelId=49B87B9A0253
	inline void InsertAtEvent(int nIndex,CXJEvent *pEvent, int nCount = 1)
		{
			m_EventList.InsertAt(nIndex,pEvent,nCount); 
		}

/*
 *  @brief   	MoveFirst	 �¼�ָ���Ƶ���ǰ
 *  @return 	void 
 */
	//##ModelId=49B87B9A0257
	void MoveFirst();			

/*
 *  @brief   	MoveLast	 �¼�ָ���Ƶ���ĩβ
 *  @return 	void 
 */
	//##ModelId=49B87B9A0258
	void MoveLast();

/*
 *  @brief   	MoveNext	 �¼�ָ���Ƶ���һ�� 
 *  @return 	void 
 */
	//##ModelId=49B87B9A0259
	void MoveNext();

/*
 *  @brief   	MovePrev	 �¼�ָ���Ƶ���һ��
 *  @return 	void 
 */
	//##ModelId=49B87B9A025A
	void MovePrev();

/*
 *  @brief   	MoveTo	 �¼�ָ���Ƶ�ָ��λ�� 
 *  @param 		[In]a param of Type  int  ָ��λ��
 *  @return 	void 
 */
	//##ModelId=49B87B9A0261
	void MoveTo(int nPos);		//�¼�ָ���Ƶ�ָ��λ��

/*
 *  @brief   	IsEOF	 �¼�ָ���Ƿ��¼��б��β�� 
 *  @return 	inline BOOL �Ѿ���β������TRUE, δ��β������FALSE
 */
	//##ModelId=49B87B9A0263
	inline BOOL IsEOF() 
		{return ( m_nPos==m_EventList.GetSize()-1 );}

/*
 *  @brief   	IsBOF	 �¼�ָ���Ƿ��¼��б��ǰ����
 *  @return 	inline BOOL �Ѿ���ǰ������TRUE, δ��ǰ������FALSE
 */
	//##ModelId=49B87B9A0264
	inline BOOL IsBOF() 
		{return ( m_nPos==0);}

/*
 *  @brief   	ClearAllEvents	 ɾ�������������¼�
 *  @return 	void 
 */
	//##ModelId=49B87B9A0265
	void ClearAllEvents();

	//��ʾ�¼����Դ���
//	int ShowEventProp(BOOL bModal=TRUE);		//��ʾm_EventList�����Կ�
private:
	//##ModelId=49B87B9A0266
	int m_nPos;	//��ǰ�¼����б��е�λ��
};
#endif
