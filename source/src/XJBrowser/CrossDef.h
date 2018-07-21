// ***************************************************************
//  CrossDef   version:  1.0    date: 31:5:2009     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: Ԥ�����ʽ������, ��������Ԥ���Ļ�����Ϣ,��ǰԤ��״̬
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************

#if !defined(AFX_CROSSDEF_H__31438932_E873_45E0_89C7_13075CF64430__INCLUDED_)
#define AFX_CROSSDEF_H__31438932_E873_45E0_89C7_13075CF64430__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CrossDef.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCrossDef 

/**
 * @defgroup 	CCrossDef	Ԥ�����ʽ������
 * @{
 */
/**
 * @brief       ��������Ԥ���Ļ�����Ϣ,��ǰԤ��״̬
*  @author      LYH
 * @version     ver1.0
 * @date        31/05/09

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER
#include "CrossPoint.h"
typedef CTypedPtrList<CObList , CCrossPoint*>	CROSSPOINT_LIST;

class CCrossDef : public CObject
{
public:
	CCrossDef();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCrossDef)

// Attributes
public:
	/** @brief           ID*/
	int		m_nID;
	/** @brief           ������վID*/
	CString	m_sStationID;
	/** @brief           ������վָ��*/
	CStationObj* m_pStation;
	/** @brief           ��������. 1.��ֵ 2.ģ���� 3.������ 4.��ѹ��*/
	int		m_nDataType;
	/** @brief           Ԥ��������*/
	CString	m_sName;
	/** @brief           �õ�Խ�޸澯����*/
	int		m_nAlarmType;
	/** @brief           ��ǰ״̬. 1��Ԥ����0������*/
	int		m_nCurStatus;
	/** @brief           ״̬�ı�ʱ��*/
	CTime	m_tmTime;

	/** @brief           Ԥ�����ݵ�����*/
	CROSSPOINT_LIST		m_listCross;
	/** @brief           �Ƿ���������ϸ��Ϣ*/
	BOOL	m_bLoadDetail;
// Operations
public:
/*
 *  @brief   	LoadFromDB	 �����ݿ�������� 
 *  @param 		[In]a param of Type  CMemSet*  ���ݿ�����
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadFromDB(CMemSet* pMemSet);

/*
 *  @brief   	GetStatusDes	 ȡ��ָ��״̬������ 
 *  @param 		[In]a param of Type  int  ָ��״̬
 *  @return 	CString ״̬����
 */
	CString GetStatusDes(int nStatus);

/*
 *  @brief   	GetDataTypeDes	 ȡ�������������� 
 *  @param 		[In]a param of Type  int  ָ������
 *  @return 	CString ����
 */
	CString GetDataTypeDes(int nType);

/*
 *  @brief   	LoadDetail	 �����ݿ��ȡָ�����ε���ϸ��Ϣ 
 *  @param 		[In]a param of Type  int  
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadDetail();

/*
 *  @brief   	LoadLastSaveNo	 �����ݿ��ȡ��������
 *  @param 		[Out]a param of Type  int&  ��������
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadLastSaveNo(int& nSaveNo);

/*
 *  @brief   	ClearDetail	 ��������� 
 *  @return 	void 
 */
	void ClearDetail();

/*
 *  @brief   	LoadFromSttp	 ��STTP�����ж�ȡ�¼���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP�������ݽṹ
 *  @return 	virtual BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B9A013B
	BOOL LoadFromSTTP(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	BuildReleation	 ����������ϵ 
 *  @return 	BOOL �����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildReleation();

// Implementation
public:
	virtual ~CCrossDef();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROSSDEF_H__31438932_E873_45E0_89C7_13075CF64430__INCLUDED_)
