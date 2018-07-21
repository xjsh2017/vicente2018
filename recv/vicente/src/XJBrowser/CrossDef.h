// ***************************************************************
//  CrossDef   version:  1.0    date: 31:5:2009     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: 预警表达式描述类, 用于描述预警的基本信息,当前预警状态
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
 * @defgroup 	CCrossDef	预警表达式描述类
 * @{
 */
/**
 * @brief       用于描述预警的基本信息,当前预警状态
*  @author      LYH
 * @version     ver1.0
 * @date        31/05/09

 *
 * example
 * @code
 *    // 举例说明类如何使用
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
	/** @brief           所属厂站ID*/
	CString	m_sStationID;
	/** @brief           所属厂站指针*/
	CStationObj* m_pStation;
	/** @brief           数据类型. 1.定值 2.模拟量 3.开关量 4.软压板*/
	int		m_nDataType;
	/** @brief           预警点名称*/
	CString	m_sName;
	/** @brief           该点越限告警类型*/
	int		m_nAlarmType;
	/** @brief           当前状态. 1、预警；0、正常*/
	int		m_nCurStatus;
	/** @brief           状态改变时间*/
	CTime	m_tmTime;

	/** @brief           预警数据点链表*/
	CROSSPOINT_LIST		m_listCross;
	/** @brief           是否已载入详细信息*/
	BOOL	m_bLoadDetail;
// Operations
public:
/*
 *  @brief   	LoadFromDB	 从数据库读入数据 
 *  @param 		[In]a param of Type  CMemSet*  数据库数据
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadFromDB(CMemSet* pMemSet);

/*
 *  @brief   	GetStatusDes	 取得指定状态的描述 
 *  @param 		[In]a param of Type  int  指定状态
 *  @return 	CString 状态描述
 */
	CString GetStatusDes(int nStatus);

/*
 *  @brief   	GetDataTypeDes	 取得数据类型描述 
 *  @param 		[In]a param of Type  int  指定类型
 *  @return 	CString 描述
 */
	CString GetDataTypeDes(int nType);

/*
 *  @brief   	LoadDetail	 从数据库读取指定批次的详细信息 
 *  @param 		[In]a param of Type  int  
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadDetail();

/*
 *  @brief   	LoadLastSaveNo	 从数据库读取最新批次
 *  @param 		[Out]a param of Type  int&  最新批次
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadLastSaveNo(int& nSaveNo);

/*
 *  @brief   	ClearDetail	 清除旧数据 
 *  @return 	void 
 */
	void ClearDetail();

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据结构
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9A013B
	BOOL LoadFromSTTP(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	BuildReleation	 建立关联关系 
 *  @return 	BOOL 关联成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildReleation();

// Implementation
public:
	virtual ~CCrossDef();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROSSDEF_H__31438932_E873_45E0_89C7_13075CF64430__INCLUDED_)
