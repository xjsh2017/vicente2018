// ***************************************************************
//  SttpEngine   version:  1.0    date: 25:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: STTP����,����������ر�STTP�ͻ��˽ӿڿ�,��װ�ӿ�
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(XJNOTIFY_H_)
#define XJNOTIFY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataEngine.h"
#include "vccommon_define.h"

class CXJNotify : public CObject
{
public:	
	CXJNotify(CDataEngine* pDataEngine);
	virtual ~CXJNotify();

public:
	CDataEngine* m_pDataEngine;

public:
/*
 *  @brief   	LoadFromDB	 �����ݿ��ж�ȡ�¼���Ϣ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP�������ݽṹ
 *  @param 		[In]a param of Type  int  ָ���Ǳ��������еĵڼ���������
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	GetDescribe	 ����¼����� 
 *  @return 	virtual CString �¼������ַ���
 */
	virtual CString GetDescribe();

/*
 *  @brief   	GetSimpleDescribe	 ����¼���Ҫ���� 
 *  @return 	virtual CString �¼���Ҫ�����ַ���
 */
	virtual CString GetSimpleDescribe();

/*
 *  @brief   	BuildReleation	 �������豸�Ĺ��� 
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	virtual BOOL BuildReleation();

/*
 *  @brief   	Clone	 �����¼������������ 
 *  @return 	virtual CSTEvent* �������ɵ��¼�����
 */
	virtual CXJNotify* Clone();

/*
 *  @brief   	GetFaultType	 ���������ͽ��ͳ�����
 *  @return 	virtual CString	 ���ͺ������
 */
	virtual CString GetFaultType(CString sFaultType);

/*
 *  @brief   	GetTriggerType	 �õ����˵�� 
 *  @param 		[In]a param of Type  CString  �����д
 *  @return 	virtual CString ���˵��
 */
	virtual CString GetTriggerType(CString sTrigger);

	/*
	* @brief	GetNotifyType	ȡ��֪ͨ����
	* @return   int	֪ͨ����
	*/
	virtual int GetNotifyType();
};

#endif // !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
