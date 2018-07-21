#if !defined(AFX_FAULTREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_)
#define AFX_FAULTREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaultReport.h : header file
//
#include "FaultEvent.h"
/** @brief           故障事件链表*/

typedef CTypedPtrList<CObList , CFaultEvent*>	FAULTEVENT_LIST;

/////////////////////////////////////////////////////////////////////////////
// CFaultReport command target

class CFaultReport : public CObject
{
	DECLARE_DYNCREATE(CFaultReport)
public:
	
	CFaultReport();           // protected constructor used by dynamic creation
	virtual ~CFaultReport();
// Attributes
public:
	/** @brief           故障包ID*/
	int			m_nID;
	/** @brief           归档名称*/
	CString		m_sName;
	/** @brief           一次设备ID*/
	CString		m_sPriDeviceID;
	/** @brief           故障包创建时间*/
	CTime       m_tmCreateTime;
	/** @brief           故障时间*/
	CTime		m_tmFaultTime;
	/** @brief           故障时间毫秒值*/
	int			m_nmsFaultTime;
	/** @brief           故障测距*/
	CString		m_sFaultDistance;
	/** @brief           故障类型*/
	int			m_nFaultType;
	/** @brief           厂站1ID*/
	CString		m_sStation1ID;
	/** @brief           厂站2ID*/
	CString		m_sStation2ID;
	/** @brief           事件等级*/
	int			m_nEventLevel;
	/** @brief			故障包报文类型*/
	int			m_nRportType;

	/** @brief           厂站1指针*/
	CStationObj*	m_pStation1;
	/** @brief           厂站2指针*/
	CStationObj*	m_pStation2;
	/** @brief           一次设备指针*/
	CDeviceObj*		m_pPriDevice;

	/** @brief           详细信息是否已被载入*/
	BOOL			m_bLoadDetail;

	/** @brief           动作故障量事件数组*/
	CPtrArray		m_listChr;
	/** @brief           动作信号量事件数组*/
	CPtrArray		m_listSign;
	/** @brief           告警事件数组*/
	CPtrArray		m_listAlarm;
	/** @brief           保护录波数组*/
	CPtrArray		m_listPTOSC;
	/** @brief           录波器录波数组表*/
	CPtrArray		m_listWROSC;
	/** @brief           录波列表数组*/
	CPtrArray		m_listOSCList;
	/** @brief			小波测距数据*/
	CPtrArray		m_listWaveDistance;
	/** @brief			录波分析数据*/
	CPtrArray		m_listOSCParse;
	/** @brief			开关量变位数据*/
	CPtrArray		m_listDIChange;	
	/** @brief           测距信息*/
	CPtrArray		m_listDistance;

	/** @brief           保护设备数组*/
	CPtrArray		m_listSec;
	/** @brief           零时动作信号量数组*/
	CPtrArray		m_listTempSign;
	/** @brief           零时故障量数组*/
	CPtrArray		m_listTempChr;
	/** @brief           零时保护录波数组*/
	CPtrArray		m_listTempPTOSC;
	/** @brief           记录零时数组时保存的是哪个设备*/
	CSecObj*		m_pTempSec;
	/** @brief			临时开关量变位数组*/
	CPtrArray		m_listTempDIChange;

	CPtrArray		m_listSecStation1;
	CPtrArray		m_listSecStation2;
	CSecObj*		m_pTempSecStation1;
	CSecObj*		m_pTempSecStation2;
	
	CPtrArray		m_listTempSignStation1;
	CPtrArray		m_listTempChrStation1;
	CPtrArray		m_listTempPTOSCStation1;
	CPtrArray		m_listTempDIChangeStation1;
	
	CPtrArray		m_listTempSignStation2;
	CPtrArray		m_listTempChrStation2;
	CPtrArray		m_listTempPTOSCStation2;
	CPtrArray		m_listTempDIChangeStation2;

	/** @brief           是否显示返回信息*/
	BOOL			m_bShowReturn;
	/** @brief           是否存档*/
	BOOL            m_bIsSaved;
	/** @brief           是否是典型故障*/
	int            m_nTypicalcase;
	/** @brief           回放脚本路径*/
	CString            m_sFis_path;
    /** @brief           故障原因*/    
	CString         m_sFaultReason;
	/** @brief			是否检修故障包.0-非检修,1-检修*/
	int				m_nOverhaul;
// Operations
public:

	void UpdateFrom(CFaultReport* pReport);
	CFaultReport* Clone();
/*
 *  @brief   	LoadFromDB	 从数据库中载入故障包数据 
 *  @param 		[In]a param of Type  CMemSet*  数据集指针
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadFromDB(CMemSet* pMemset);

/*
 *  @brief   	BuildReleation	 建立与设备的关联 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildReleation();

/*
 *  @brief   	LoadDetail	 载入故障报告详细信息 
 *  @param 		[In]a param of Type  BOOL  是否无论有没有被载入都重新载入
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadDetail(BOOL bForce = FALSE);

/*
 *  @brief   	ClearDetail	 清除详细信息
 *  @return 	void 
 */
	void ClearDetail();

/*
 *  @brief   	GetFaultType	 得到故障类型的文字描述 
 *  @return 	CString 故障类型文字描述
 */
	CString GetFaultType();

/*
 *  @brief   	SaveToDB	 保存数据到数据库 
 *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	BOOL SaveToDB();

/*
 *  @brief   	MultiSaveEvent	 批量保存事件 
 *  @return 	int 保存成功0,删除数据失败返回1,保存数据失败返回2
 */
	int MultiSaveEvent();

/*
 *  @brief   	DeleteAllEventFromDB	 从数据库中删除所有事件 
 *  @return 	BOOL 删除成功返回TRUE, 失败返回FALSE
 */
	BOOL DeleteAllEventFromDB();

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据结构
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadFromSTTP(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	GetFaultTypeInt	 将故障类型从字符型转为整型 
 *  @param 		[In]a param of Type  CString  故障类型字符串
 *  @return 	int 故障类型标识
 */
	int GetFaultTypeInt(CString sType);

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

/*
 *  @brief   	BuildTempList	 指定二次设备建立零时数组 
 *  @param 		[In]a param of Type  CSecObj*  指定二次设备
 *  @return 	void 
 */
	void BuildTempList(CSecObj* pSec);

	void BuildTempListLandscape(CSecObj* pSecStation1, CSecObj* pSecStation2);

	BOOL DeleteFaultShortenData();

	CString GetFisPath();

	/*
	 *  @brief   	GetPTFaultDistance	取得保护上送的故障测距值 
	 *	@param 		[In]a param of Type  CString ptid	保护ID
	 *  @return 	CString	 故障测距值
	 */
	CString GetPTFaultDistance(CString ptid);
private:
	BOOL DeleteFaultShortenEvents();
	BOOL DeleteFaultAnalyData();

	void MatchSec();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAULTREPORT_H__E7043812_002D_4595_AE2D_C7128D4CBEAB__INCLUDED_)
