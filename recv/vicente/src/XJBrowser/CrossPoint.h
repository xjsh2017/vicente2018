// ***************************************************************
//  CrossPoint   version:  1.0    date: 31:5:2009     author:LYH
//  ---------------------------------------------------------------------------------------
//  description: 预警点描述类, 用于描述预警下各预警点的说明及状态
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
 * @defgroup 	CCrossPoint	预警点描述类
 * @{
 */
/**
 * @brief       用于描述预警下各预警点的说明及状态
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

class CCrossPoint : public CObject
{
public:
	CCrossPoint();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCrossPoint)

// Attributes
public:
	/** @brief           ID(序号)*/
	int		m_nID;
	/** @brief           所属预警表达式的ID*/
	int		m_nCrossID;
	/** @brief           数据类型. 1.定值 2.模拟量 3.开关量 4.软压板*/
	int		m_nDataType;
	/** @brief           所属厂站ID*/
	CString	m_sStationID;
	/** @brief           所属厂站指针*/
	CStationObj*	m_pStation;
	/** @brief           所属二次设备ID*/
	CString	m_sDevID;
	/** @brief           所属二次设备指针*/
	CSecObj*	m_pSec;
	/** @brief           所属CPU*/
	int		m_nCPU;
	/** @brief           数据点定义的ID*/
	int		m_nDataPointID;
	/** @brief           数据点的值*/
	CString m_sDataPointValue;
	/** @brief           计算后的值*/
	CString m_sSectionValue;
	/** @brief           状态改变时间*/
	CTime	m_tmTime;
	/** @brief           1、预警；0、复归*/
	int		m_nCrossType;
	/** @brief           批次号*/
	int		m_nSaveNo;
	/** @brief           数据点名称*/
	CString	m_sDataPointName;

// Operations
public:
/*
 *  @brief   	LoadFromDB	 从数据库读入数据 
 *  @param 		[In]a param of Type  CMemSet*  数据库数据
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadFromDB(CMemSet* pMemSet);

/*
 *  @brief   	BuildReleation	 建立与设备的关联 
 *  @param 		[In]a param of Type  int  设备类型
 *  @return 	BOOL 关联成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildReleation(int nType);

/*
 *  @brief   	GetStatusDes	 取得预警点指定状态的描述 
 *  @param 		[In]a param of Type  int  指定状态
 *  @return 	CString 状态描述
 */
	CString GetStatusDes(int nType);

/*
 *  @brief   	QueryDataPointName	 查询数据点名称 
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL QueryDataPointName();

/*
 *  @brief   	QueryAIDataPointName	查询指定模拟量点的名称 
 *  @param 		[In]a param of Type  CString  二次设备ID
 *  @param 		[In]a param of Type  int  模拟量ID
 *  @param 		[In]a param of Type  int  CPU
 *  @param 		[Out]a param of Type  CString &  模拟量名称
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL QueryAIDataPointName(CString sPTID, int nID, int nCPU, CString & sName);

/*
 *  @brief   	QueryDIDataPointName	查询指定开关量点的名称 
 *  @param 		[In]a param of Type  CString  二次设备ID
 *  @param 		[In]a param of Type  int  开关量ID
 *  @param 		[In]a param of Type  int  CPU
 *  @param 		[Out]a param of Type  CString &  开关量名称
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL QueryDIDataPointName(CString sPTID, int nID, int nCPU, CString & sName);

/*
 *  @brief   	QuerySettingDataPointName	查询指定定值的名称 
 *  @param 		[In]a param of Type  CString  二次设备ID
 *  @param 		[In]a param of Type  int  定值ID
 *  @param 		[In]a param of Type  int  CPU
 *  @param 		[Out]a param of Type  CString &  定值名称
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL QuerySettingDataPointName(CString sPTID, int nID, int nCPU, CString & sName);

/*
 *  @brief   	QuerySoftBoardDataPointName	查询指定软压板的名称 
 *  @param 		[In]a param of Type  CString  二次设备ID
 *  @param 		[In]a param of Type  int  软压板ID
 *  @param 		[In]a param of Type  int  CPU
 *  @param 		[Out]a param of Type  CString &  软压板名称
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL QuerySoftBoardDataPointName(CString sPTID, int nID, int nCPU, CString & sName);



// Implementation
public:
	virtual ~CCrossPoint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROSSPOINT_H__09ECC6ED_7A4C_4433_87D2_B756251FE2D7__INCLUDED_)
