#if !defined(AFX_DECISIONREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_)
#define AFX_DECISIONREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#include "XJNotify.h"
#include "vccommon_define.h"
#include "DataEngine.h"
#include "FaultEvent.h"

/////////////////////////////////////////////////////////////////////////////
// CFaultReport command target

class CDecisionAppraise : public CObject
{
public:
	CDecisionAppraise(CDataEngine* pDataEngine, CString sDeviceID, CString sContent, int nDeviceType = TYPE_PROTECT);
	virtual ~CDecisionAppraise();

private:
	CDataEngine*	m_pDataEngine;
	/** @brief			设备ID*/
	CString	m_sDeviceID;
	/** @brief			评价内容*/
	CString	m_sContent;
	/** @brief			设备类型*/
	int	m_nDeviceType;
	/** @brief			对应设备*/
	CDeviceObj*	m_pDevice;

public:
	/*
	* @brief	GetDeviceName	取得设备名称
	* @return   CString	设备名称
	*/
	CString GetDeviceName();

	/*
	* @brief	GetAppraise	取得评价
	* @return   CString	评价内容
	*/
	CString GetAppraise();

	/*
	* @brief	GetAppraiseInt	取得INT类型的评价
	* @return   int	
	*/
	int GetAppraiseInt();
private:
/*
 *  @brief   	BuildReleation	 建立与设备的关联 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildReleation();
};

class CDecisionReport : public CXJNotify
{
public:
	CDecisionReport(CDataEngine* pDataEngine);
	virtual ~CDecisionReport();
// Attributes
public:
	/** @brief           决策包ID*/
	int			m_nID;
	/** @brief			对应的故障包ID*/
	int			m_nFaultID;
	/** @brief			电压等级*/
	int			m_nVoltage;
	/** @brief			故障类型*/
	CString		m_sFaultType;
	/** @brief			故障原因*/
	CString		m_sFaultReason;
	/** @brief			初次故障持续时间，单位:秒*/
	int			m_nFirstTime;
	/** @brief			重合闸不成功时故障持续时间,单位:秒*/
	int			m_nSecondTime;
	/** @brief			故障电流*/
	float		m_fFaultCurrent;
	/** @brief			故障描述*/
	CString		m_sFaultDescribe;
	/** @brief			录波情况*/
	CString		m_sWaveDescribe;
	/** @brief			保护设备评价*/
	CString		m_sProtectJudge;
	/** @brief			断路器行为评价*/
	CString		m_sBreakJudge;
	/** @brief			对策建议*/
	CString		m_sFaultAdvise;

	/** @brief			录波评价链表*/
	CObArray m_listWave;
	/** @brief			录波缺少链表*/
	CObArray m_listWaveLack;
	/** @brief			保护行为评价链表*/
	CObArray m_listProtect;
	/** @brief			保护动作缺少链表*/
	CObArray m_listProtectLack;
	/** @brief			断路器行为评价链表*/
	CObArray m_listBreak;

	/** @brief           动作信号量事件数组*/
	CPtrArray		m_listSign;

	/** @brief           保护设备数组*/
	CPtrArray		m_listSec;

	/** @brief           零时动作信号量数组*/
	CPtrArray		m_listTempSign;
	/** @brief           记录零时数组时保存的是哪个设备*/
	CSecObj*		m_pTempSec;

	/** @brief			故障元件名*/
	CString			m_sFaultDevice;
	/** @brief			故障相位*/
	CString			m_sFaultPhase;
	/** @brief			故障测距*/
	CString			m_sFaultDistance;

// Operations
public:
	
	virtual int GetNotifyType();
/*
 *  @brief   	LoadFromDB	 从数据库中载入
 *  @param 		[In]a param of Type  CMemSet*  数据集指针
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	virtual BOOL LoadFromDB(CMemSet* pMemset);

/*
 *  @brief   	BuildReleation	 建立与设备的关联 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	virtual BOOL BuildReleation();

/*
 *  @brief   	GetDescribe	 获得事件描述 
 *  @return 	virtual CString 事件描述字符串
 */
	virtual CString GetDescribe();

/*
 *  @brief   	GetSimpleDescribe	 获得事件简要描述 
 *  @return 	virtual CString 事件简要描述字符串
 */
	virtual CString GetSimpleDescribe();

/*
 *  @brief   	BuildTempList	 指定二次设备建立零时数组 
 *  @param 		[In]a param of Type  CSecObj*  指定二次设备
 *  @return 	void 
 */
	void BuildTempList(CSecObj* pSec);
private:
	/*
	* @brief	ClearWaveDescribe	清空录波评价数组
	* @return   void	
	*/
	void ClearWaveDescribe();

	/*
	* @brief	ClearProtectJudge	清空保护评价数组
	* @return   void	
	*/
	void ClearProtectJudge();

	/*
	* @brief	ClearBreakJudge	清空断路器评价数组
	* @return   void	
	*/
	void ClearBreakJudge();

	/*
	* @brief	ParseWaveDescribe	解析录波情况
	* @param	[In]a param of Type CString sStr		
	* @return   void	
	*/
	void ParseWaveDescribe(CString sStr);

	/*
	* @brief	ParseProtectJudge	解析保护评价
	* @param	[In]a param of Type CString sStr		
	* @return   void	
	*/
	void ParseProtectJudge(CString sStr);

	/*
	* @brief	ParseBreakJudge	解析断路器评价
	* @param	[In]a param of Type CString sStr		
	* @return   void	
	*/
	void ParseBreakJudge(CString sStr);

	/*
 *  @brief   	LoadDetail	 载入故障报告详细信息 
 *  @param 		[In]a param of Type  BOOL  是否无论有没有被载入都重新载入
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadDetail(BOOL bForce = FALSE);

	/*
	* @brief	ClearDetail	清除事件信息
	* @return   void	
	*/
	void ClearDetail();

/*
 *  @brief   	GetFaultTypeLetter	 将故障类型从整形转为字母形式 
 *  @param 		[In]a param of Type  int  故障类型代码
 *  @return 	CString 故障类型字符串
 */
	CString GetFaultTypeLetter(int nType);

/*
 *  @brief   	SortActionSign	 信号量时间和毫秒值排序动作信号列表 
 *  @return 	void 
 */
	void SortActionSign();

/*
 *  @brief   	CompareSign	 比较两个故障事件的动作信号时时间大小 
 *  @param 		[In]a param of Type  CFaultEvent*  故障事件1
 *  @param 		[In]a param of Type  CFaultEvent*  故障事件2
 *  @return 	int 1>2返回>0, 1=2返回0, 1<2返回<0
 */
	int CompareSign(CFaultEvent* pEvent1, CFaultEvent* pEvent2);

/*
 *  @brief   	AddSec	 将设备加入到二次设备数组 
 *  @param 		[In]a param of Type  CDeviceObj*  设备指针
 *  @return 	void 
 */
	void AddSec(CDeviceObj* pObj);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAULTREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_)
