// ***************************************************************
//  Event   version:  1.0    date: 14:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 各事件类定义
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(EVENT_H)
#define EVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Deviceobj.h"
// Event.h : header file

/**
 * @defgroup 	CSTEvent	事件基类
 * @{
 */
/**
 * @brief       用于各事件子类继承,提供事件共同的属性
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08
/** @} */ //OVER
//##ModelId=49B87B9A02CE
class CXJEvent : public CObject
{
public:	
	
	//##ModelId=49B87B9A02DE
	CXJEvent();
	//##ModelId=49B87B9A038A
	virtual ~CXJEvent();

	/** @brief           是否是调试状态*/
	//##ModelId=49B87B9B003E
	int			m_IsDebug;
	/** @brief           事件所对应的微机装置指针(二次设备)*/
	//##ModelId=49B87B9B004F
	CSecObj *	m_pSec;
	/** @brief           对应数据表的ID*/
	//##ModelId=49B87B9B005D
	long		m_lID;
	/** @brief           事件信息*/
	//##ModelId=49B87B9B005E
	CString		m_sMsg;			//事件信息
	/** @brief           电网名称*/
	//##ModelId=49B87B9B006D
	CString		m_sNetName;
	/** @brief           厂站名称*/
	//##ModelId=49B87B9B007D
	CString		m_sStationName;     
	/** @brief           厂站ID*/
	//##ModelId=49B87B9B007E
	CString		m_sStationId;
	/** @brief           一次设备名称*/
	//##ModelId=49B87B9B008C
	CString		m_sDeviceName;
	/** @brief           二次设备名称*/
	//##ModelId=49B87B9B008D
	CString		m_sSecName;
	/** @brief           二次设备ID*/
	//##ModelId=49B87B9B009C
	CString		m_sSecID;
	/** @brief            一次设备的ID,用于信息的关联查询*/
	//##ModelId=49B87B9B00AB
	CString		m_sDeviceId;
	/** @brief           是否确认（0-未确认，1-已确认）*/
	//##ModelId=49B87B9B00AC
	int			m_nIsAck;
	/** @brief           发生时间*/
	//##ModelId=49B87B9B00BB
	CTime		m_tmTime;
	/** @brief           毫秒值*/
	//##ModelId=49B87B9B00BC
	int			m_nms;
	/** @brief           故障类型*/
	//##ModelId=49B87B9B00CB
	CString		m_sFaultType;
	/** @brief           事件属性. 0表示I类, 1表示II类, 2表示III类*/
	//##ModelId=49B87B9B00DA
	int			m_nProperty;


	/** @brief           动作完成时间*/
	//##ModelId=49B87B9B00DB
	CTime		m_tmTime2;	 
	/** @brief           动作完成时间的毫秒值*/
	//##ModelId=49B87B9B00EA
	int			m_nms2;

	CTime		m_tmMainReceiveTime;//主站接收时间

	//事件类型定义
	// XJ_EVENT_ACTION	= 0;	//保护动作
	// XJ_EVENT_ALARM	= 1;	//保护告警
	// XJ_EVENT_FAULT	= 2;	//事故报告
	// XJ_EVENT_OSC		= 3;	//录波
	// XJ_EVENT_DI		= 4;	//开关量变位信息
	// XJ_EVENT_PTCOMM	= 5;	//保护通讯状态
	// XJ_EVENT_PTRUN	= 6;	//保护运行状态
	// XJ_EVENT_SYSALARM = 7;	//系统告警
	/** @brief           事件类型(0:保护动作, 1:保护告警, 2:事故报告, 3:录波, 4:开关量, 5:保护通讯状态, 6:保护运行状态, 7:系统告警)*/
	//##ModelId=49B87B9B00FA
	int			m_nEventType;

	/** @brief           是否已载入详细信息, TRUE表示已载入, FALSE表示未载入*/
	//##ModelId=49B87B9B00FB
	BOOL		m_bLoadDetail;

	/** @brief           是否返回信息标志。0-不是返回信息，1-是返回信息*/
	int			m_nIsReturn;
/*
 *  @brief   	GetDevice	 得到事件对应的一次设备 
 *  @return 	事件所对应的一次设备指针
    virtual CDeviceObj* 
 */
	//##ModelId=49B87B9B0109
	virtual CDeviceObj* GetDevice()	{return (m_pSec!=NULL)?m_pSec->m_pOwner:NULL; }

/*
 *  @brief   	Serialize	 序例化事件 
 *  @param 		[In]a param of Type  CArchive &  序例化类对象
 *  @return 	virtual void 
 */
	//##ModelId=49B87B9B010B
	virtual	void Serialize(CArchive &ar);	

/*
 *  @brief   	Ack	 事件确认，在派生类中实现
 *  @return 	virtual BOOL  TRUE-成功，FALSE-失败
 */
	//##ModelId=49B87B9B01B5
	virtual	BOOL Ack(){return FALSE;};	

/*
 *  @brief   	GetDescribe	 获得事件描述 
 *  @return 	virtual CString 事件描述字符串
 */
	//##ModelId=49B87B9B01B7
	virtual CString GetDescribe();	

/*
 *  @brief   	Clone	 复制事件对象各个属性 
 *  @return 	virtual CSTEvent* 复制生成的事件对象
 */
	//##ModelId=49B87B9B0261
	virtual CXJEvent* Clone()=0;	 

/*
 *  @brief   	GetFaultType	 将故障类型解释成文字
 *  @return 	virtual CString	 解释后的文字
 */
	//##ModelId=49B87B9B0263
	virtual CString GetFaultType();

/*
 *  @brief   	GetTriggerType	 得到相别说明 
 *  @param 		[In]a param of Type  CString  相别缩写
 *  @return 	virtual CString 相别说明
 */
	//##ModelId=49B87B9B030D
	virtual CString GetTriggerType(CString sTrigger);

/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9B03A9
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP报文数据结构
 *  @param 		[In]a param of Type  int  指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9C006D
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);


/*
 *  @brief   	BuildReleation	 建立事件与设备的关系 
 *  @param 		[In/Out]a param of Type  int  二次设备类型
 *  @return 	virtual BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9C0109
	virtual BOOL BuildReleation(int nType);

/*
 *  @brief   	QueryNewEventID	 查询新事件的ID(
 *  @return 	BOOL 查询失败返回FALSE, 成功返回TRUE
 */
	//##ModelId=49B87B9C01B5
	virtual BOOL QueryNewEventID();

/*
 *  @brief   	IsReturnEvent	 判断是否"返回"事件
 *  @return 	BOOL 
 */
	virtual BOOL IsReturnEvent();
	
	
	//将故障特征量和信号量变位作为基类的属性
	//格式为：
	//故障特征量数目，故障特征量1ID，故障特征量1值，…，故障特征量N1的ID，故障特征量N1值，
	//信号量数目，开关量1ID，开关量1值，…，开关量N2的ID，开关量N2值，
	/** @brief           本事件涉及的故障特征量列表*/
	//##ModelId=49B87B9C0252
	CTypedPtrArray<CPtrArray,PT_ActionCharacter*> m_ActChrs;
	/** @brief           本事件涉及的信号量列表*/
	//##ModelId=49B87B9C0262
	CTypedPtrArray<CPtrArray,PT_Signal*> m_Signals;

protected:
/*
 *  @brief   	Clear	 清除点表配置 
 *  @return 	BOOL 
 */
	//##ModelId=49B87B9C0271
	BOOL Clear();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventAlarm 告警事件
/**
 * @defgroup 	CXJEventAlarm	告警事件类
 * @{
 */
/**
 * @brief       描述告警事件
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08
/** @} */ //OVER
//##ModelId=49B87B9C038A
class CXJEventAlarm : public CXJEvent
{
public:
	//##ModelId=49B87B9C039A
	CXJEventAlarm();
	//##ModelId=49B87B9D004E
	CXJEventAlarm(CXJEventAlarm* pEvent);
	//##ModelId=49B87B9D00DA
	~CXJEventAlarm();
public:		 
	/** @brief           产生告警事件的源*/
	//##ModelId=49B87B9D0177
	long	m_lAlarmID;
	/** @brief           产生告警事件的源的值。0-返回，1-动作*/
	//##ModelId=49B87B9D0178
	int		m_nValue;   
	int     m_nCount;
public:
/*
 *  @brief   	TranslateFData	 解释事件FData字段得到事件详细信息
 *  @return 	BOOL 解释成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87B9D0186
	BOOL TranslateFData();

/*
 *  @brief   	TranslateSttpData	 解析告警报文数据 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据结构
 *	@param		[In]a param of Type  指定是报文数据中的第几个数据项
 *  @return 	BOOL 解析成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9D0203
	BOOL TranslateSttpData(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	Ack	 确认事件 
 *  @return 	virtual BOOL 确认成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9D0290
	virtual	BOOL Ack();
	
/*
 *  @brief   	GetDescribe	 得到事件详细描述
 *  @return 	virtual CString 事件描述字符串
 */
	//##ModelId=49B87B9D031C
	virtual CString GetDescribe(); 

/*
 *  @brief   	Clone	 复制事件对象所有属性 
 *  @return 	virtual CSTEvent* 复制生成的对象
 */
	//##ModelId=49B87B9D03B9
	virtual CXJEvent* Clone();

/*
 *  @brief   	GetAlarmType	 取得告警事件类别 
 *  @return 	CString 告警事件类别,I,II,III
 */
	//##ModelId=49B87B9E005D
	CString GetAlarmType();

/*
 *  @brief   	IsReturnEvent	 判断是否"返回"事件
 *  @return 	BOOL 
 */
	virtual BOOL IsReturnEvent();
protected:
/*
 *  @brief   	GetSignalConfig	 得到指定信息号 
 *  @param 		[In/Out]a param of Type  int  信息号ID
 *  @return 	PT_Signal * 信息号对象
 */
	//##ModelId=49B87B9E00DA
	PT_Signal * GetSignalConfig(int nID){ if(m_pSec) return m_pSec->FindAlarmSignal(nID); return NULL; }

public:
/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9E0157
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据结构
 *	@param		[In]a param of Type	 指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9E01E4
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

private:
/*
 *  @brief   	QueryNewEventID	 查询新事件的ID(
 *  @return 	void 
 */
	//##ModelId=49B87B9E0261
	BOOL QueryNewEventID();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventAction 动作事件
/**
 * @defgroup 	CSTEventAction	动作事件类
 * @{
 */
/**
 * @brief       描述动作事件
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08

/** @} */ //OVER
//##ModelId=49B87B9E033C
class CXJEventAction : public CXJEvent
{
public:
	//##ModelId=49B87B9E034B
	CXJEventAction();
	//##ModelId=49B87B9E03B9
	CXJEventAction(CXJEventAction* pEvent);
	//##ModelId=49B87B9F003E
	~CXJEventAction();
public:
	/** @brief           故障数据，对应TB_PT_ACTION的FDATA字段*/
	//##ModelId=49B87B9F00AB
	CString		m_FData;
	
	int         m_nCount;

	/** @brief			相对时间*/
	int			m_nRelativeTime;

public:
/*
 *  @brief   	Ack	 确认事件 
 *  @return 	virtual BOOL 确认成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87B9F00BB
	virtual	BOOL Ack();	 

/*
 *  @brief   	GetDescribe	 得到事件详细描述 
 *  @return 	virtual CString 事件详细描述格代化字符串
 */
	//##ModelId=49B87B9F0128
	virtual CString GetDescribe();	 

/*
 *  @brief   	Clone	 复制事件对象 
 *  @return 	virtual CSTEvent* 复制的事件对象
 */
	//##ModelId=49B87B9F0196
	virtual CXJEvent* Clone();

/*
 *  @brief   	TranslateFData	 解析FData内容 
 *  @return 	BOOL 解析成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87B9F01F4
	BOOL TranslateFData();

/*
 *  @brief   	TranslateSttpData	 解析STTP报文中信息号和特征量的内容
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据结构
 *  @return 	BOOL 解析成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9F0261
	BOOL TranslateSttpData(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	GetAlarmType	 取得故障类型,I,II,III类 
 *  @return 	CString 
 */
	//##ModelId=49B87B9F02BF
	CString	GetAlarmType();

	/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9F030D
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据结构
 *	@param		[In]a param of Type	 int	指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9F036B
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);
private:
/*
 *  @brief   	RefreshConfig	 刷新对应二次设备的信号量列表 
 *  @return 	BOOL 刷新成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B9F03B9
	BOOL RefreshConfig(){ if(!m_pSec) return FALSE; m_pSec->RefreshConfig(); return TRUE;};

/*
 *  @brief   	QueryNewEventID	 查询新事件的ID(
 *  @return 	void 
 */
	//##ModelId=49B87BA0002E
	BOOL QueryNewEventID();

protected:
/*
 *  @brief   	GetActChrsConfig	 得到指定的事件特征量 
 *  @param 		[In]a param of Type  int  特征量ID
 *  @return 	PT_ActionCharacter * 特征量对象
 */
	//##ModelId=49B87BA0007D
	PT_ActionCharacter * GetActChrsConfig(int nID){ if(m_pSec) return m_pSec->FindActionCharacter(nID); return NULL; };

/*
 *  @brief   	GetActChrsFromUniversal	 从通用特征量中查找特征量定义 
 *  @param 		[In]a param of Type  int  特征量ID
 *  @return 	PT_ActionCharacter * 特征量对象
 */
	//##ModelId=49B87BA000DA
	PT_ActionCharacter * GetActChrsFromUniversal(int nID);

/*
 *  @brief   	GetSignalConfig	 得到指定的事件信号量 
 *  @param 		[In]a param of Type  int  信号量ID
 *  @return 	PT_Signal * 信号量对象
 */
	//##ModelId=49B87BA00128
	PT_Signal * GetSignalConfig(int nID){ if(m_pSec) return m_pSec->FindActionSignal(nID); return NULL; };
};

//////////////////////////////////////////////////////////////////////////
//CXJEventFault 事故报告

//////////////////////////////////////////////////////////////////////////
//CXJEventDI	开关量
/**
 * @defgroup 	CXJEventDI	开关量变位事件类
 * @{
 */
/**
 * @brief       描述开关量变位事件
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08
/** @} */ //OVER
//##ModelId=49B87BA001C5
class CXJEventDI : public CXJEvent
{
public:
	//##ModelId=49B87BA001D4
	CXJEventDI();
	//##ModelId=49B87BA00213
	CXJEventDI(CXJEventDI* pEvent);
	//##ModelId=49B87BA00261
	~CXJEventDI();

//attribute
public:
	/** @brief           CPU号*/
	//##ModelId=49B87BA0029F
	int		m_nCPU;
	/** @brief           开关量ID*/
	//##ModelId=49B87BA002AF
	int		m_nDIID;
	/** @brief           采样值*/
	//##ModelId=49B87BA002B0
	int		m_nValue;
	/** @brief           开关量名称*/
	//##ModelId=49B87BA002B1
	CString	m_sDIName;
//operation
public:
	/*
 *  @brief   	Ack	 事件确认，在派生类中实现
 *  @return 	virtual BOOL  TRUE-成功，FALSE-失败
 */
	//##ModelId=49B87BA002B2
	virtual	BOOL Ack();	

/*
 *  @brief   	Clone	 复制事件对象各个属性 
 *  @return 	virtual CSTEvent* 复制生成的事件对象
 */
	//##ModelId=49B87BA002FD
	virtual CXJEvent* Clone();

/*
 *  @brief   	GetDescribe	 得到事件详细描述
 *  @return 	virtual CString 事件描述字符串
 */
	//##ModelId=49B87BA0033C
	virtual CString GetDescribe();

/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA0037A
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP报文数据结构
 *  @param 		[In]a param of Type  int  指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA003B9
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	GetDIName	 查库得到开关量名称 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA1000F
	BOOL GetDIName();

private:
/*
 *  @brief   	QueryNewEventID	 查询新事件的ID(
 *  @return 	void 
 */
	//##ModelId=49B87BA1004E
	BOOL QueryNewEventID();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventOSC	录波
/**
 * @defgroup 	CXJEventOSC	二次设备录波事件
 * @{
 */
/**
 * @brief       描述二次设备录波事件
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08
/** @} */ //OVER
//##ModelId=49B87BA100FA
class CXJEventOSC : public CXJEvent
{
public:
	//##ModelId=49B87BA100FC
	CXJEventOSC();
	//##ModelId=49B87BA10128
	CXJEventOSC(CXJEventOSC* pEvent);
	//##ModelId=49B87BA10167
	~CXJEventOSC();
//attribute
public:
	/** @brief           数据文件名*/
	//##ModelId=49B87BA10196
	CString		m_sFileName;
	/** @brief           文件路径,相对路径*/
	//##ModelId=49B87BA10197
	CString		m_sFilePath;
	/** @brief           是否有故障标识 1-有故障 0-无故障*/
	//##ModelId=49B87BA101A5
	int			m_nHaveFault;
	/** @brief           跳闸相别*/
	//##ModelId=49B87BA101A6
	CString		m_sTriggerType;
	/** @brief           重合闸时间*/
	//##ModelId=49B87BA101A7
	int			m_nRecloseTime;
	/** @brief           故障测距*/
	//##ModelId=49B87BA101A8
	CString		m_sFaultDistance;
	/** @brief           是否已下载.0-否.1-是.2-部分下载.-1-下载失败.-2-取消下载*/
	//##ModelId=49B87BA101B5
	int			m_nDownloaded;
	/** @brief           录波文件大小*/
	//##ModelId=49B87BA101B6
	int			m_nFileSize;
	/** @brief           录波文件在装置中的编号*/
	//##ModelId=49B87BA101B7
	int			m_nFileNum;
	/** @brief           数据源*/
	//##ModelId=49B87BA101B8
	int			m_nDataSrc;

	/** @brief           偏移量*/
	//##ModelId=49B87BA101B9
	int			m_nOffset;
	
//operation
public:
	/*
 *  @brief   	Ack	 事件确认，在派生类中实现
 *  @return 	virtual BOOL  TRUE-成功，FALSE-失败
 */
	//##ModelId=49B87BA101C5
	virtual	BOOL Ack();	

/*
 *  @brief   	Clone	 复制事件对象各个属性 
 *  @return 	virtual CSTEvent* 复制生成的事件对象
 */
	//##ModelId=49B87BA101E4
	virtual CXJEvent* Clone();

/*
 *  @brief   	GetDescribe	 得到事件详细描述
 *  @return 	virtual CString 事件描述字符串
 */
	//##ModelId=49B87BA10222
	virtual CString GetDescribe();

/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA10242
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA10242
	virtual	BOOL LoadFromDB_(CMemSet * pMemSet);


/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据结构
 *  @param 		[In]a param of Type  int  指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA10280
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	LoadFromSttpForPT	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据结构
 *  @param 		[In]a param of Type  int  指定是报文数据中的第几个数据项
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA102AF
	BOOL LoadFromSttpForPT(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	IsDownloaded	 判断当前录波列表是否已下载 
 *  @return 	BOOL 0-未下载. 1-以下载. 2-部分下载
 */
	//##ModelId=49B87BA1030D
	int IsDownloaded();

private:
/*
 *  @brief   	QueryNewEventID	 查询新事件的ID(
 *  @return 	void 
 */
	//##ModelId=49B87BA102DE
	BOOL QueryNewEventID();

public:
/*
 *  @brief   	GetFilePath	 得到本地录波文件全路径 
 *  @return 	CString 本地录波文件全路径
 */
	//##ModelId=49B87BA1032C
	CString GetFilePath();

/*
 *  @brief   	GetFilePathNoSuff	 得到不带后缀名的路径 
 *  @return 	CString 文件路径
 */
	//##ModelId=49B87BA1034B
	CString GetFilePathNoSuff();

/*
 *  @brief   	GetFileRelatePath	 得到不带后缀名的相对路径 
 *  @return 	CString 相对路径
 */
	//##ModelId=49B87BA1037A
	CString GetFileRelatePath();

/*
 *  @brief   	GetDownloadString	 得到是否下载的文字描述 
 *  @return 	CString 是否下载的文字描述
 */
	//##ModelId=49B87BA10399
	CString GetDownloadString();

/*
 *  @brief   	GetFileDir	 取得文件目录 
 *  @return 	CString 文件目录
 */
	CString GetFileDir();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventPTComm 保护通讯状态
/**
 * @defgroup 	CXJEventPTComm	保护通讯状态事件类
 * @{
 */
/**
 * @brief       描述保护通讯状态变位事件
*  @author      LYH
 * @version     ver1.0
 * @date        14/09/08
/** @} */ //OVER
//##ModelId=49B87BA2000F
class CXJEventPTComm : public CXJEvent
{
public:
	//##ModelId=49B87BA20011
	CXJEventPTComm();
	//##ModelId=49B87BA2002E
	CXJEventPTComm(CXJEventPTComm * pEvent);
	//##ModelId=49B87BA2004E
	~CXJEventPTComm();
//attribute
public:
	/** @brief           状态 0：正常  1: 断开*/
	//##ModelId=49B87BA2006D
	int		m_nStatus;
//operation
public:

/*
 *  @brief   	Ack	 确认事件
 *  @return 	BOOL 确认成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA2007D
	BOOL Ack();
/*
 *  @brief   	Clone	 复制事件对象各个属性 
 *  @return 	virtual CSTEvent* 复制生成的事件对象
 */
	//##ModelId=49B87BA2008C
	virtual CXJEvent* Clone();

/*
 *  @brief   	GetDescribe	 得到事件详细描述
 *  @return 	virtual CString 事件描述字符串
 */
	//##ModelId=49B87BA200AB
	virtual CString GetDescribe();

/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA200CB
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP报文数据结构
 *  @param 		[In]a param of Type  int  指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA200EA
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

private:
/*
 *  @brief   	QueryNewEventID	 查询新事件的ID(
 *  @return 	查询成功返回TRUE, 失败返回FALSE 
 */
	//##ModelId=49B87BA20109
	BOOL QueryNewEventID();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventPTRun	保护运行状态
//##ModelId=49B87BA20167
class CXJEventPTRun : public CXJEvent
{
public:
	//##ModelId=49B87BA20169
	CXJEventPTRun();
	//##ModelId=49B87BA20186
	CXJEventPTRun(CXJEventPTRun * pEvent);
	//##ModelId=49B87BA20196
	~CXJEventPTRun();
//attribute
	/** @brief           运行状态 0:运行 1：检修/挂牌 2：停用 3：调试 4: 对码表*/
	//##ModelId=49B87BA201B5
	int		m_nStatus;
//operation
/*
 *  @brief   	Ack	 确认事件
 *  @return 	BOOL 确认成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA201B6
	BOOL Ack();

/*
 *  @brief   	Clone	 复制事件对象各个属性 
 *  @return 	virtual CSTEvent* 复制生成的事件对象
 */
	//##ModelId=49B87BA201C5
	virtual CXJEvent* Clone();

/*
 *  @brief   	GetDescribe	 得到事件详细描述
 *  @return 	virtual CString 事件描述字符串
 */
	//##ModelId=49B87BA201D4
	virtual CString GetDescribe();

/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA201F4
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP报文数据结构
 *  @param 		[In]a param of Type  int  指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA20203
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

private:
/*
 *  @brief   	QueryNewEventID	 查询新事件的ID(
 *  @return 	查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA20222
	BOOL QueryNewEventID();
};


//////////////////////////////////////////////////////////////////////////
//CXJEventSysAlarm 系统异常告警
//##ModelId=49B87BA202CE
class CXJEventSysAlarm : public CXJEvent
{
public:
	//##ModelId=49B87BA202D0
	CXJEventSysAlarm();
	//##ModelId=49B87BA202DE
	CXJEventSysAlarm(CXJEventSysAlarm* pEvent);
	//##ModelId=49B87BA202EE
	~CXJEventSysAlarm();
//attribute
/** @brief           类型1－定值区号；2－软压板；3－与前一次对比或子站直接上送的定值变化; 4-与基准值对比定值变化; 5-子站配置变化*/
	//##ModelId=49B87BA202FD
	int		m_nType;
	/** @brief           告警信息的详细描述*/
	//##ModelId=49B87BA202FE
	CString	m_sDetailMsg;
	/** @brief           告警对应的CPU号*/
	//##ModelId=49B87BA202FF
	CString		m_sCPU;
	/** @brief           告警对应的定值区号*/
	//##ModelId=49B87BA2030D
	CString		m_sZone;
	/** @brief           保存格式化的详细信息*/
	//##ModelId=49B87BA2030E
	CString		m_sDetail;

	/** @brief           保存定值定义*/
	//##ModelId=49B87BA20310
	CTypedPtrArray<CPtrArray, PT_SETTING*> arrSetting;
	/** @brief           保存软压板定义*/
	//##ModelId=49B87BA2031D
	CTypedPtrArray<CPtrArray, PT_SOFTBOARD*> arrSoftBoard;

	/** @brief           详细信息*/
	CTypedPtrArray<CPtrArray, SYSALARM*> m_arrDetail;
//operation
/*
 *  @brief   	Ack	 确认事件
 *  @return 	BOOL 确认成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA20321
	BOOL Ack();
/*
 *  @brief   	Clone	 复制事件对象各个属性 
 *  @return 	virtual CSTEvent* 复制生成的事件对象
 */
	//##ModelId=49B87BA2032C
	virtual CXJEvent* Clone();

/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA2033C
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP报文数据结构
 *  @param 		[In]a param of Type  int  指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA2034B
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

	BOOL Load20138Sttp(STTP_FULL_DATA& sttpData);

	BOOL Load20176Sttp(STTP_FULL_DATA& sttpData);

/*
 *  @brief   	GetDescribe	 获得事件描述 
 *  @return 	virtual CString 事件描述字符串
 */
	//##ModelId=49B87BA2035B
	virtual CString GetDescribe();

private:
	CString GetSubStationChangeType(int nType);
/*
 *  @brief   	QueryNewEventID	 查询新事件的ID(
 *  @return 	查询成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA2036B
	BOOL QueryNewEventID();

	/*
	* @brief	GetDescribeCommOFF 取得子站接入设备通讯断开率越限描述	
	* @return   CString	子站接入设备通讯断开率越限详细描述
	*/
	CString GetDescribeCommOFF();

/*
 *  @brief   	GetDescribeSubStation	 取得子站配置变化详细描述 
 *  @return 	CString 子站配置变化详细描述
 */
	//##ModelId=49B87BA2037A
	CString GetDescribeSubStation();

/*
 *  @brief   	GetDescribeSetting	 取得定值变化的详细描述 
 *  @return 	CString 定值变化详细描述
 */
	//##ModelId=49B87BA2037B
	CString GetDescribeSetting();

/*
 *  @brief   	GetDescribeSoftBoard	 取得软压板变化的详细描述  
 *  @return 	CString 软压板变化的详细描述
 */
	//##ModelId=49B87BA2038A
	CString GetDescribeSoftBoard();

/*
 *  @brief   	GetDescribeZone	 取得定值区变化的详细描述 
 *  @return 	CString 定值区变化的详细描述
 */
	//##ModelId=49B87BA2038B
	CString GetDescribeZone();

/*
 *  @brief   	GetDescribeOrder	 取得与基准值对比定值变化的详细描述
 *  @return 	CString 详细描述
 */
	//##ModelId=49B87BA2038C
	CString GetDescribeOrder();

/*
 *  @brief   	TranslateDetailMsg	 解析事件详细描述保存在m_sDetail中 
 *  @param 		[In]a param of Type  int  事件类型
 *  @return 	void 
 */
	//##ModelId=49B87BA2038D
	void TranslateDetailMsg(int nType);

/*
 *  @brief   	TranslateSubStation	 解析子站配置变化的详细描述 
 *  @return 	void 
 */
	//##ModelId=49B87BA2039A
	void TranslateSubStation();

/*
 *  @brief   	TranslateSetting	 解析定值变化的详细描述 
 *  @return 	void 
 */
	//##ModelId=49B87BA203A9
	void TranslateSetting();

/*
 *  @brief   	TranslateOrder	 解析与基准值对比定值的详细描述
 *  @return 	void 
 */
	//##ModelId=49B87BA203B9
	void TranslateOrder();

/*
 *  @brief   	GetSettingDes	 得到指定定值ID的定值描述 
 *  @param 		[In]a param of Type  CString  定值ID
 *  @return 	CString 定值描述
 */
	//##ModelId=49B87BA203BA
	CString GetSettingDes(CString sID);

/*
 *  @brief   	GetSettingConfig	 读取指定保护指定CPU的所有定值定义 
 *  @param 		[In]a param of Type  CString  指定保护
 *  @param 		[In]a param of Type  int  指定CPU号
 *	@param		[In]a param of Type	 int   指定定值区号
 *  @return 	void 
 */
	//##ModelId=49B87BA203BC
	void GetSettingConfig(CString PT_ID, int nCPU, int nZone);

/*
 *  @brief   	TranslateZone	 解析定值区号变化的详细描述 
 *  @return 	void 
 */
	//##ModelId=49B87BA203CB
	void TranslateZone();

/*
 *  @brief   	TranslateSoftBoard	 解析软压板变化的详细描述  
 *  @return 	void 
 */
	//##ModelId=49B87BA203CC
	void TranslateSoftBoard();

/*
 *  @brief   	GetSettingDes	 得到指定软压板ID的定值描述 
 *  @param 		[In]a param of Type  CString  软压板ID
 *  @return 	CString 软压板描述
 */
	//##ModelId=49B87BA203D8
	CString GetSoftBoardDes(CString sID);

/*
 *  @brief   	GetSoftBoardConfig	 得到指定保护指定CPU下的软压板定义
 *  @param 		[In]a param of Type  CString  保护ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @return 	void 
 */
	//##ModelId=49B87BA203DA
	void GetSoftBoardConfig(CString PT_ID, int nCPU);

/*
 *  @brief   	GetMsg	 得到简单描述信息 
 *  @return 	CString 简单描述信息
 */
	//##ModelId=49B87BA30001
	CString	GetMsg();

/*
 *  @brief   	ClearSettingConfig	 清除定值定义 
 *  @return 	void 
 */
	//##ModelId=49B87BA30002
	void ClearSettingConfig();

/*
 *  @brief   	ClearSoftBoardConfig	 清除软压板定义 
 *  @return 	void 
 */
	//##ModelId=49B87BA30003
	void ClearSoftBoardConfig();
};

//////////////////////////////////////////////////////////////////////////
//CXJEventComm	主子站通讯状态变位事件
/**
 * @defgroup 	CXJEventComm	主子站通讯状态变位事件
 * @{
 */
/**
 * @brief       把主子让通讯变位当成事件处理
*  @author      LYH
 * @version     ver1.0
 * @date        06/10/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BA3002E
class CXJEventComm : public CXJEvent
{
public:
	//##ModelId=49B87BA30030
	CXJEventComm();
	//##ModelId=49B87BA3003E
	CXJEventComm(CXJEventComm* pEvent);
	//##ModelId=49B87BA30040
	~CXJEventComm();
//attribute
public:
	/** @brief           通讯状态*/
	//##ModelId=49B87BA30041
	int m_nStatus;
//operation
public:
/*
 *  @brief   	Clone	 复制事件对象各个属性 
 *  @return 	virtual CSTEvent* 复制生成的事件对象
 */
	//##ModelId=49B87BA30042
	virtual CXJEvent* Clone();

/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA30044
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  TTP报文数据结构
 *  @param 		[In]a param of Type  int  指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA30047
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	BuildReleation	 建立事件与设备的关系 
 *  @param 		[In]a param of Type  int  二次设备类型
 *  @return 	virtual BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA3004B
	virtual	BOOL BuildReleation(int nType);
};

class CXJEventDistance : public CXJEvent
{
public:
	CXJEventDistance();
	CXJEventDistance(CXJEventDistance* pEvent);
	~CXJEventDistance();
public:
	/** @brief			线路名称*/
	CString	m_sLineName;
	/** @brief			电压等级*/
	int		m_nVoltage;
	/** @brief			线路总长度*/
	float	m_fLineTotalLength;
	/** @brief			故障点到M端的距离*/
	float	m_fDistance;
	/** @brief			杆塔位置*/
	int		m_nTowerPos;
	/** @brief			维护局名称*/
	CString	m_sManagerUnit;
public:
	/*
 *  @brief   	Ack	 事件确认，在派生类中实现
 *  @return 	virtual BOOL  TRUE-成功，FALSE-失败
 */
	virtual	BOOL Ack();	
/*
 *  @brief   	GetDescribe	 得到事件详细描述
 *  @return 	virtual CString 事件描述字符串
 */
	virtual CString GetDescribe();
	/*
 *  @brief   	Clone	 复制事件对象各个属性 
 *  @return 	virtual CXJEvent* 复制生成的事件对象
 */
	virtual CXJEvent* Clone();

/*
 *  @brief   	LoadFromDB	 从数据库中读取事件信息
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	virtual	BOOL LoadFromDB(CMemSet * pMemSet);

/*
 *  @brief   	LoadFromSttp	 从STTP报文中读取事件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据结构
 *  @param 		[In]a param of Type  int  指定是报文数据中的第几个数据项
 *  @return 	virtual BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	virtual BOOL LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex);

private:
/*
 *  @brief   	QueryNewEventID	 查询新事件的ID
 *  @return 	void 
 */
	BOOL QueryNewEventID();
};

#endif
