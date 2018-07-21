// ***************************************************************
//  CrossPoint   version:  1.0    date: 31:5:2009     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: Ԥ����������, ��������Ԥ���¸�Ԥ�����˵����״̬
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_CROSSPOINT_H__09ECC6ED_7A4C_4433_87D2_B756251FE2D7__INCLUDED_)
#define AFX_CROSSPOINT_H__09ECC6ED_7A4C_4433_87D2_B756251FE2D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CrossPoint.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCrossPoint document

/**
 * @defgroup 	CCrossPoint	Ԥ����������
 * @{
 */
/**
 * @brief       ��������Ԥ���¸�Ԥ�����˵����״̬
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

class CCrossPoint : public CObject
{
public:
	CCrossPoint();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCrossPoint)

// Attributes
public:
	/** @brief           ID(���)*/
	int		m_nID;
	/** @brief           ����Ԥ�����ʽ��ID*/
	int		m_nCrossID;
	/** @brief           ��������. 1.��ֵ 2.ģ���� 3.������ 4.��ѹ��*/
	int		m_nDataType;
	/** @brief           ������վID*/
	CString	m_sStationID;
	/** @brief           ������վָ��*/
	CStationObj*	m_pStation;
	/** @brief           ���������豸ID*/
	CString	m_sDevID;
	/** @brief           ���������豸ָ��*/
	CSecObj*	m_pSec;
	/** @brief           ����CPU*/
	int		m_nCPU;
	/** @brief           ���ݵ㶨���ID*/
	int		m_nDataPointID;
	/** @brief           ���ݵ��ֵ*/
	CString m_sDataPointValue;
	/** @brief           ������ֵ*/
	CString m_sSectionValue;
	/** @brief           ״̬�ı�ʱ��*/
	CTime	m_tmTime;
	/** @brief           1��Ԥ����0������*/
	int		m_nCrossType;
	/** @brief           ���κ�*/
	int		m_nSaveNo;
	/** @brief           ���ݵ�����*/
	CString	m_sDataPointName;

// Operations
public:
/*
 *  @brief   	LoadFromDB	 �����ݿ�������� 
 *  @param 		[In]a param of Type  CMemSet*  ���ݿ�����
 *  @return 	BOOL ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL LoadFromDB(CMemSet* pMemSet);

/*
 *  @brief   	BuildReleation	 �������豸�Ĺ��� 
 *  @param 		[In]a param of Type  int  �豸����
 *  @return 	BOOL �����ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildReleation(int nType);

/*
 *  @brief   	GetStatusDes	 ȡ��Ԥ����ָ��״̬������ 
 *  @param 		[In]a param of Type  int  ָ��״̬
 *  @return 	CString ״̬����
 */
	CString GetStatusDes(int nType);

/*
 *  @brief   	QueryDataPointName	 ��ѯ���ݵ����� 
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL QueryDataPointName();

/*
 *  @brief   	QueryAIDataPointName	��ѯָ��ģ����������� 
 *  @param 		[In]a param of Type  CString  �����豸ID
 *  @param 		[In]a param of Type  int  ģ����ID
 *  @param 		[In]a param of Type  int  CPU
 *  @param 		[Out]a param of Type  CString &  ģ��������
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL QueryAIDataPointName(CString sPTID, int nID, int nCPU, CString & sName);

/*
 *  @brief   	QueryDIDataPointName	��ѯָ��������������� 
 *  @param 		[In]a param of Type  CString  �����豸ID
 *  @param 		[In]a param of Type  int  ������ID
 *  @param 		[In]a param of Type  int  CPU
 *  @param 		[Out]a param of Type  CString &  ����������
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL QueryDIDataPointName(CString sPTID, int nID, int nCPU, CString & sName);

/*
 *  @brief   	QuerySettingDataPointName	��ѯָ����ֵ������ 
 *  @param 		[In]a param of Type  CString  �����豸ID
 *  @param 		[In]a param of Type  int  ��ֵID
 *  @param 		[In]a param of Type  int  CPU
 *  @param 		[Out]a param of Type  CString &  ��ֵ����
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL QuerySettingDataPointName(CString sPTID, int nID, int nCPU, CString & sName);

/*
 *  @brief   	QuerySoftBoardDataPointName	��ѯָ����ѹ������� 
 *  @param 		[In]a param of Type  CString  �����豸ID
 *  @param 		[In]a param of Type  int  ��ѹ��ID
 *  @param 		[In]a param of Type  int  CPU
 *  @param 		[Out]a param of Type  CString &  ��ѹ������
 *  @return 	BOOL ��ѯ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL QuerySoftBoardDataPointName(CString sPTID, int nID, int nCPU, CString & sName);



// Implementation
public:
	virtual ~CCrossPoint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROSSPOINT_H__09ECC6ED_7A4C_4433_87D2_B756251FE2D7__INCLUDED_)
