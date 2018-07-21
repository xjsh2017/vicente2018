#if !defined(AFX_FAULTEVENT_H__A2E56667_8100_434E_8243_7C01D6EA95E8__INCLUDED_)
#define AFX_FAULTEVENT_H__A2E56667_8100_434E_8243_7C01D6EA95E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaultEvent.h : header file
//
#include "DeviceObj.h"
#include "DataEngine.h"
#include "vccommon_define.h"

class CFaultEvent;
/** @brief           历史事件链表*/
//##ModelId=49B87B90014A
typedef CTypedPtrList<CObList , CFaultEvent*>	HISTORYEVENT_LIST;

/////////////////////////////////////////////////////////////////////////////
// CFaultEvent

/**
 * @defgroup 	CFaultEvent	故障报告事件类
 * @{
 */
/**
 * @brief       描述故障报告的事件信息
*  @author      LYH
 * @version     ver1.0
 * @date        24/10/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87B9A01C5
class CFaultEvent : public CObject
{
	DECLARE_DYNCREATE(CFaultEvent)
public:
	//##ModelId=49B87B9A01C7
	CFaultEvent();           // protected constructor used by dynamic creation
	CFaultEvent(CDataEngine* pDataEngine);
	//##ModelId=49B87B9A01C8
	virtual ~CFaultEvent();
// Attributes
public:
	CDataEngine*	m_pDataEngine;
	/** @brief           值的单位*/
	CString		m_strUnit;
	/** @brief           故障报告事件类型*/
	//##ModelId=49B87B9A01CA
	int			m_nType;
	/** @brief           在数据中的ID*/
	//##ModelId=49B87B9A01D4
	int			m_nID;
	/** @brief           对应的故障信息包ID*/
	//##ModelId=49B87B9A01D5
	int			m_nMainRecID;
	/** @brief           二次设备ID*/
	//##ModelId=49B87B9A01D6
	CString		m_sSecID;
	/** @brief           启动时间*/
	//##ModelId=49B87B9A01D7
	CTime		m_tmStart;
	/** @brief           启动时间毫秒值*/
	//##ModelId=49B87B9A01D8
	int			m_nmsStart;
	/** @brief           接收时间*/
	//##ModelId=49B87B9A01D9
	CTime		m_tmReceiveTime;
	/** @brief           接收时间毫秒值*/
	//##ModelId=49B87B9A01E4
	int			m_nmsReceive;
	/** @brief           事件定义*/
	//##ModelId=49B87B9A01E5
	CString		m_sEventDef;
	/** @brief           事件值*/
	//##ModelId=49B87B9A01E6
	CString		m_sEventContent;
	/** @brief           事件定义名称*/
	//##ModelId=49B87B9A01E7
	CString		m_sEventDefName;
	/** @brief           信号时间*/
	//##ModelId=49B87B9A01E8
	CTime		m_tmSign;
	/** @brief           信号时间毫秒值*/
	//##ModelId=49B87B9A01E9
	int			m_nmsSign;
	/** @brief           对应的CPU号*/
	int			m_nCPU;

	/** @brief           事件关联的二次设备指针*/
	//##ModelId=49B87B9A01F5
	CSecObj*	m_pSec;

	/** @brief           被选择标志,增加事件用*/
	//##ModelId=49B87B9A01F9
	BOOL m_bSelected;

	/** @brief           所属事件号,保存历史事件用*/
	int		m_nNum;

	/** @brief           录波文件相对目录*/
	CString m_sDir; 

	/** @brief           旧的录波文件相对目录*/
	CString m_sOldDir; 

public:
/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息 
 *  @param 		[In]a param of Type  CMemSet*  数据集
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9A01FA
	BOOL LoadFromDB(CMemSet* pMemset);

/*
 *  @brief   	BuildReleation	 创建与设备的关系 
 *  @return 	BOOL 能与设备关联返回TRUE, 不能关联返回FALSE
 */
	//##ModelId=49B87B9A01FC
	BOOL BuildReleation();

/*
 *  @brief   	FindEventDefName	 查找事件定义的名称 
 *  @return 	BOOL 查找成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9A01FD
	BOOL FindEventDefName();

/*
 *  @brief   	MultiSaveData	 组织批量插入数据
 *  @param 		[Out]a param of Type  STTP_DBDATA_RECORD&  数据记录
 *  @return 	BOOL 组织成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9A01FE
	BOOL MultiSaveData(STTP_DBDATA_RECORD& sttpDB);

/*
 *  @brief   	Clone	 复制故障事件 
 *  @return 	CFaultEvent* 复制生成的故障事件
 */
	//##ModelId=49B87B9A0204
	CFaultEvent* Clone();

/*
 *  @brief   	GetFaultType	 将故障类型解释成文字 
 *  @param 		[In]a param of Type  CString  故障类型
 *  @return 	CString 解释后的文字
 */
	CString GetFaultType(CString sType);

/*
 *  @brief   	IsReturnEvent	 是否返回信息 
 *  @return 	BOOL 是返回信息返回TRUE,不是返回信息返回FALSE
 */
	BOOL IsReturnEvent();

/*
 *  @brief   	GetEventProperty	 取得事件等级 
 *  @return 	int 事件等级,默认返回0，0-I类，1-II类，2-III类
 */
	int GetEventProperty();

private:
/*
 *  @brief   	GetActChrsConfig	 得到指定的事件特征量 
 *  @param 		[In]a param of Type  int  特征量ID
 *  @return 	PT_ActionCharacter * 特征量对象
 */
	//##ModelId=49B87B9A0205
	PT_ActionCharacter * GetActChrsConfig(int nID);

/*
 *  @brief   	GetActChrsFromUniversal	 从通用特征量中查找特征量定义 
 *  @param 		[In]a param of Type  int  特征量ID
 *  @return 	PT_ActionCharacter * 特征量对象
 */
	//##ModelId=49B87B9A0207
	PT_ActionCharacter * GetActChrsFromUniversal(int nID);

/*
 *  @brief   	GetSignalConfig	 得到指定的事件信号量 
 *  @param 		[In]a param of Type  int  信号量ID
 *  @return 	PT_Signal * 信号量对象
 */
	//##ModelId=49B87B9A0209
	PT_Signal * GetSignalConfig(int nID);

/*
 *  @brief   	GetAlarmConfig	 得到指定的告警告信号量 
 *  @param 		[In]a param of Type  int  信号量ID
 *  @return 	PT_Signal * 信号量对象
 */
	//##ModelId=49B87B9A020B
	PT_Signal * GetAlarmConfig(int nID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAULTEVENT_H__A2E56667_8100_434E_8243_7C01D6EA95E8__INCLUDED_)
