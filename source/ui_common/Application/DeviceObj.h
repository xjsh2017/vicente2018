// ***************************************************************
//  DeviceObj   version:  1.0    date: 12:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 设备类, 记录各种设备的属性, 执行设备的属性读取操作
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_DEVICEOBJ_H__18F49415_AC3F_4CD2_BE1E_F4351573898C__INCLUDED_)
#define AFX_DEVICEOBJ_H__18F49415_AC3F_4CD2_BE1E_F4351573898C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceObj.h : header file
//
#include "vccommon_define.h"
#include "DBEngine.h"

class CDeviceObj;
class CSecCPU;
class CNetObj;
class CStationObj;

class CBreakObj;
class CBusObj;
class CGenObj;
class CCapObj;
class CLineObj;
class CReactorObj;
class CSwitchObj;
class CTransObj;
class CPmsObj;
class CBayObj;

class CSecObj;

/** @brief           定义设备链表*/
//##ModelId=49B87BA5002E
typedef CTypedPtrList<CObList , CDeviceObj*>	DEVICE_LIST;

/** @brief           地区链表*/
//##ModelId=49B87BA5003E
typedef	CTypedPtrList<CObList , CNetObj*>		NET_LIST;
/** @brief           厂站链表*/
//##ModelId=49B87BA5004E
typedef CTypedPtrList<CObList , CStationObj*>	STATION_LIST;

/** @brief           厂站通断原因表*/
typedef CMap<int, int, CString, CString>        STATION_STATUS_CONFIG_MAP;  


/** @brief           二次设备链表*/
//##ModelId=49B87BA500EB
typedef CTypedPtrList<CObList , CSecObj*>		SEC_LIST;

typedef CMap<int, int, CStationObj* , CStationObj* >            MAP_VOLTAGE;


/////////////////////////////////////////////////////////////////////////////
// CDeviceObj 设备基类

/**
 * @defgroup 	CDeviceObj 设备基础类
 * @{
 */
/**
 * @brief       设备基类,各设备类从此类继承
*  @author      LYH
 * @version     ver1.0
 * @date        12/09/08
/** @} */ //OVER
//##ModelId=49B87BA50138
class CDeviceObj : public CObject
{
public:
	//##ModelId=49B87BA50149
	CDeviceObj();
	DECLARE_DYNCREATE(CDeviceObj)

// Attributes
public:
	/** @brief           设备ID*/
	//##ModelId=49B87BA50186
	CString		m_sID;
	/** @brief           统一资源编号*/
	CString    m_sUri;
	/** @brief           设备名称*/
	//##ModelId=49B87BA50187
	CString		m_sName;
	/** @brief           用整形标识的设备类型.定义见stdafx.h*/
	//##ModelId=49B87BA50196
	int			m_nType;
	/** @brief           用字符串标识的设备类型. 定义见stdafx.h*/
	//##ModelId=49B87BA50197
	CString		m_sType;
	/** @brief           电压等级*/
	//##ModelId=49B87BA501A5
	int			m_nVoltage;
	/** @brief           所属厂站ID*/
	//##ModelId=49B87BA501A6
	CString		m_sStationID;
	/** @brief           运行状态,各种设备对应不同运行状态值*/
	//##ModelId=49B87BA501B5
	int			m_nRunStatu;
	/** @brief           通讯状态.0-正常,1-断开*/
	//##ModelId=49B87BA501C5
	int			m_nCommStatus;
	/** @brief           编号*/
	//##ModelId=49B87BA501C6
	int			m_nCode;
	/** @brief           103组号*/
	//##ModelId=49B87BA501D4
	int			m_n103Group;
	/** @brief           103条目号*/
	//##ModelId=49B87BA501D5
	int			m_n103Item;
	/** @brief           对应间隔ID*/
	//##ModelId=49B87BA501E4
	CString		m_sBayID;

	BOOL m_bHasSecDevice;

	/** @brief           在设备树中的位置*/
	HTREEITEM	m_htItem;

// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA50204
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	Save	 保存设备信息, 由子类实现
 *  @return 	virtual BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA50242
	virtual	BOOL Save(CDBEngine* pDBEngine);

// Implementation
public:
	//##ModelId=49B87BA50271
	virtual ~CDeviceObj();
};
//CDeviceObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CNetObj	地区
/**
 * @defgroup 	CNetObj	地区类
 * @{
 */
/**
 * @brief       地区类, 载入,记录,存储地区信息
*  @author      LYH
 * @version     ver1.0
 * @date        12/09/08
/** @} */ //OVER

//##ModelId=49B87BA502CE
class CNetObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA502DE
	CNetObj();
	//##ModelId=49B87BA5030D
	~CNetObj();
	DECLARE_DYNCREATE(CNetObj)

//Attribute
public:
	/** @brief           地区说明*/
	//##ModelId=49B87BA5033C
	CString		m_sNote;

	/** @brief           通信异常的厂站数*/
	//##ModelId=49B87BA5034B
	int		m_nStationAbnormity;
	/** @brief           保护通信状态异常的数量*/
	//##ModelId=49B87BA5034C
	int		m_nPTCommAbnormity;
	/** @brief           保护运行状态异常的数量*/
	//##ModelId=49B87BA5035B
	int		m_nPTRunAbnormity;

// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA5035C
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	Save	 保存设备信息, 由子类实现
 *  @return 	virtual BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA50399
	virtual	BOOL Save(CDBEngine* pDBEngine);
};

//CNetObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CStationObj 厂站
/**
 * @defgroup 	CStationObj	厂站类
 * @{
 */
/**
 * @brief       厂站类, 载入,记录,存储厂站信息
*  @author      LYH
 * @version     ver1.0
 * @date        12/09/08
/** @} */ //OVER

//##ModelId=49B87BA6009C
class CStationObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA600AC
	CStationObj();
	//##ModelId=49B87BA600DA
	~CStationObj();
	DECLARE_DYNCREATE(CStationObj)

//Attribute
	/** @brief           所属地区ID*/
	//##ModelId=49B87BA60109
	CString	m_sNetID;
	/** @brief           通讯规约ID*/
	//##ModelId=49B87BA60119
	int		m_nProtocolId;
	/** @brief           通讯规约名称*/
	//##ModelId=49B87BA6011A
	CString	m_sProtocol;
	/** @brief           通讯方式 1：tcp 2：com(串口) 3：udp 4：modem*/
	//##ModelId=49B87BA60128
	int		m_nCommMode;
	/** @brief           厂站对应的SVG文件路径(全路径)*/
	//##ModelId=49B87BA60129
	CString	m_sSvgPath;
	/** @brief           最后一次状态变位的时间*/
	//##ModelId=49B87BA60138
	CTime	m_tmNewStatus;
	/** @brief           最后一次状态变位的原因*/
	//##ModelId=49B87BA60139
	CString	m_sNewStatusReason;
	/** @brief           原因改变时间*/
	CTime m_tmReasonTime;

	/** @brief           IP选择标志。为2时为单机双通道*/
	int		m_nIPFlag;

	/** @brief           通道1状态*/
	int		m_nCommStatus1;
	/** @brief           通道1最后一次状态变位的时间*/
	CTime	m_tmNewStatus1;
	/** @brief           通道1最后一次状态变位的原因*/
	CString	m_sNewStatusReason1;
	/** @brief           通道1原因改变时间*/
	CTime m_tmReasonTime1;


	/** @brief           通讯状态断开的保护链表*/
	//##ModelId=49B87BA6014A
	SEC_LIST		m_arrCommAbnormity;
	/** @brief           运行状态异常的保护链表*/
	//##ModelId=49B87BA6015B
	SEC_LIST		m_arrRunAbnormity;
	/** @brief           IP地址*/
	//##ModelId=49B87BA60177
	CString			m_sIP;
	/** @brief           所属厂家信息*/
	//##ModelId=49B87BA60186
	CString			m_sManufacturer;
	/** @brief           电压等级列表*/
	CWordArray		m_arrVoltage;
	/** @brief			通信端口*/
	CString			m_sPort;
	/** @brief			网关信息*/
	CString			m_sTel;

	int m_nChannel;

	/** @brief			在地理图中的位置*/
	CPoint	m_ptCenter;

	/** @brief			数据库访问对象*/
	CDBEngine*	m_pDBEngine;

//厂站下各设备链表, 保护用MAP保存,其它设备用LIST
private:
	/** @brief           母线链表*/
	//##ModelId=49B87BA60197
	//BUS_LIST		m_BusList;
	CMapStringToOb	m_BusMap;
	/** @brief           开关链表*/
	//##ModelId=49B87BA601A6
	CMapStringToOb	m_BreakMap;
	/** @brief           间隔链表*/
	//##ModelId=49B87BA601B6
	CMapStringToOb	m_BayMap;
	/** @brief           线路链表*/
	//##ModelId=49B87BA601C6
	CMapStringToOb	m_LineMap;
	/** @brief           电容器链表*/
	//##ModelId=49B87BA601D5
	CMapStringToOb	m_CapMap;
	/** @brief           电抗器链表*/
	//##ModelId=49B87BA601DA
	CMapStringToOb	m_ReactorMap;
	/** @brief           刀闸链表*/
	//##ModelId=49B87BA601E8
	CMapStringToOb	m_SwitchMap;
	/** @brief           保护管理机链表*/
	//##ModelId=49B87BA601F5
	CMapStringToOb	m_PmsMap;
	/** @brief           变压器链表*/
	//##ModelId=49B87BA60204
	CMapStringToOb	m_TransMap;
	/** @brief           发电机链表*/
	//##ModelId=49B87BA60214
	CMapStringToOb	m_GenMap;
	
	CMapStringToOb	m_SecMap;
	/** @brief          以Uri为Key的Map*/
	CMapStringToOb  m_UriMap;	

	

public:
	BOOL LoadData(CDBEngine* pDBEngine);
	BOOL BuildData(CDBEngine* pDBEngine);
	void InitLock();
	/*
 *  @brief   	GetPriDeviceList	 取得一次设备链表 
 *  @param 		[In]a param of Type  DEVICE_LIST& 用于保存一次设备的链表
 *  @param 		[In]a param of Type  int 指定要查找的一次设备类型，-1表示所有类型
 *  @return 	
 */
	void GetPriDeviceList(DEVICE_LIST& listDevice, int nDeviceType);

/*
 *  @brief   	GetSecList	 取得二次设备链表 
 *  @param 		[In]a param of Type  DEVICE_LIST& 用于保存二次设备的链表
 *  @param 		[In]a param of Type  CString 指定要一次设备ID，为空时表示不区分一次设备
 *  @param 		[In]a param of Type  int 指定要查找的二次设备类型，-1和type_sec表示所有类型
 *  @return 	
 */
	void GetSecList(DEVICE_LIST& listDevice, CString sPriID, int nSecType);

	/*
	* @brief	GetSecList	取得二次设备链表
	* @param	[In]a param of Type DEVICE_LIST & listDevice 用于保存二次设备的链表		
	* @param	[In]a param of Type int nPriType 指定要一次设备类型	,为-1时为所有设备
	* @param	[In]a param of Type int nSecType 指定要查找的二次设备类型，-1和type_sec表示所有类型		
	* @return   void	
	*/
	void GetSecList(DEVICE_LIST& listDevice, int nPriType, int nSecType);
private:
	CXJLock* m_pLock;
private:
/*
 *  @brief   	BuildBusMap	 读取本厂站下母线配置并填入母线MAP 
 *  @return 	BOOL 成功返回true，失败返回false
 */
	BOOL BuildBusMap(CDBEngine* pDBEngine);
	BOOL BuildLineMap(CDBEngine* pDBEngine);
	BOOL BuildBreakMap(CDBEngine* pDBEngine);
	BOOL BuildBayMap(CDBEngine* pDBEngine);
	BOOL BuildCapMap(CDBEngine* pDBEngine);
	BOOL BuildReactorMap(CDBEngine* pDBEngine);
	BOOL BuildSwitchMap(CDBEngine* pDBEngine);
	BOOL BuildPmsMap(CDBEngine* pDBEngine);
	BOOL BuildTransMap(CDBEngine* pDBEngine);
	BOOL BuildGenMap(CDBEngine* pDBEngine);
	BOOL BuildSecMap(CDBEngine* pDBEngine);

	void GetGeoPosition(CString strPosition);
public:

/*
 *  @brief   	AddDevice	 加入设备到各自链表中 
 *  @param 		[In]a param of Type  int  设备类型
 *  @param 		[In]a param of Type  CDeviceObj*  设备指针
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA602AF
	BOOL AddDevice(int nType, CDeviceObj* pObj);

// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA60251
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	Save	 保存设备信息, 由子类实现
 *  @return 	virtual BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA60280
	virtual	BOOL Save(CDBEngine* pDBEngine);

/*
 *  @brief   	Clear	 清除厂站下所有设备信息 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA602DE
	BOOL Clear();

/*
 *  @brief   	FindDevice	 查找本厂站下的设备 
 *  @param 		[In]a param of Type  CString  设备ID
 *  @param 		[In]a param of Type  int  设备类型
 *  @return 	CDeviceObj* 
 */
	//##ModelId=49B87BA6030D
	CDeviceObj* FindDevice(CString sID, int nType);

/*
 *  @brief   	FindDeviceNoLock	 查找本厂站下的设备,无锁版 
 *  @param 		[In]a param of Type  CString  设备ID
 *  @param 		[In]a param of Type  int  设备类型
 *  @return 	CDeviceObj* 
 */
	CDeviceObj* FindDeviceNoLock(CString sID, int nType);

/*
 *  @brief   	FindDeviceBy103	 根据103组号条目号查找设备 
 *  @param 		[In]a param of Type  CString  103组号条目号.格式:103组号:103条目号
 *  @param 		[In]a param of Type  int  设备类型
 *  @return 	CDeviceObj * 设备指针
 */
	//##ModelId=49B87BA6035B
	CDeviceObj * FindDeviceBy103(CString str103, int nType);


/*
 *  @brief   	FindDeviceBy103	 根据uri查找设备 
 *  @param 		[In]a param of Type  CString uri
 *  @return 	CDeviceObj * 设备指针
 */
	//##ModelId=49B87BA6035B
	CDeviceObj * FindDeviceByUri(CString strUri);

/*
 *  @brief   	LoadStatusFromDB	 从数据库读入子站与主站通讯状态 
 *  @param 		[In]a param of Type  int  通道
 *  @return 	void 
 */
	void LoadStatusFromDB(int nChannel, CDBEngine* pDBEngine);

	/*
	* @brief	GetDurativeTime	取得状态持续时间
	* @param	[In]a param of Type CTimeSpan & tsDurative1	主通道状态持续时间	
	* @param	[In]a param of Type CTimeSpan & tsDurative2	备用通道状态持续时间	
	* @return   void	
	*/
	void GetDurativeTime(CTimeSpan& tsDurative1, CTimeSpan& tsDurative2);

/*
 *  @brief   	LoadStatusFromSTTP	 从报文中读入主子站通讯状态 
 *  @param 		[In]a param of Type  STTP_FILE_DATA*  报文内容
 *  @return 	void 
 */
	//##ModelId=49B87BA603B9
	void LoadStatusFromSTTP(STTP_FULL_DATA* pSttpData);

/*
 *  @brief   	SecCommChange	 保护通讯状态变化 
 *  @param 		[In]a param of Type  CSecObj*  状态变化的保护
 *  @param 		[In]a param of Type  int  变化后的状态
 *  @return 	void 
 */
	//##ModelId=49B87BA603D8
	void SecCommChange(CSecObj* pObj, int nStatus);

/*
 *  @brief   	SecRunChange	 保护运行状态变化 
 *  @param 		[In]a param of Type  CSecObj*  状态变化的保护
 *  @param 		[In]a param of Type  int  变化后的状态
 *  @return 	void 
 */
	//##ModelId=49B87BA7001F
	void SecRunChange(CSecObj* pObj, int nStatus);

/*
 *  @brief   	GetCommAbnormityNum	 取得本厂站下通讯状态异常的装置数 
 *  @return 	int 通讯状态异常的装置数
 */
	//##ModelId=49B87BA7003E
	int GetCommAbnormityNum();

/*
 *  @brief   	GetRunAbnormityNum	 取得本厂站下运行状态异常的保护数 
 *  @return 	int 运行状态异常的保护数
 */
	//##ModelId=49B87BA7004E
	int GetRunAbnormityNum();

/*
 *  @brief   	SaveSvgPath	 保存SVG文件路径到数据库 
 *  @param 		[In]a param of Type  CString  SVG路径(相对路径)
 *  @return 	BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA7005D
	BOOL SaveSvgPath(CString sPath, CDBEngine* pDBEngine);

/*
 *  @brief   	GetReason	 得到厂站状态改变原因描述 
 *  @param 		[In]a param of Type  int  状态改变原因
 *  @return 	CString 状态改变原因描述
 */
	//##ModelId=49B87BA7007D
	CString GetReason(int nType);

/*
 *  @brief   	StatVoltage	 统计站内电压等级 
 *  @param 		[In]a param of Type  int  电压等级
 *  @return 	void 
 */
	void StatVoltage(int nVoltage);

/*
 *  @brief   	SetStatus	 设置厂站状态 
 *  @param 		[In]a param of Type  int  状态
 *  @param 		[In]a param of Type  CTime  时间
 *  @param 		[In]a param of Type  int  原因
 *  @param 		[In]a param of Type  int  通道
 *  @return 	void 
 */
	void SetStatus(int nStatus, CTime tm, int nReason, int nChannel, CTime tmReasonTime, int nIPFlag);
};

//CStationObj end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBusObj	母线
/**
 * @defgroup 	CBusObj	母线类
 * @{
 */
/**
 * @brief       描述母线属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BA700AB
class CBusObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA700BC
	CBusObj();
	//##ModelId=49B87BA700CB
	~CBusObj();
	DECLARE_DYNCREATE(CBusObj)
//Attribute
	/** @brief           接线方式 0：3/2接线；1：双母接线；2：双母接线旁路母线；3：单母线；4：单母线分段；*/
	//##ModelId=49B87BA700DA
	int		m_nBusType;
// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA700EA
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	Save	 保存设备信息, 由子类实现
 *  @return 	virtual BOOL 保存成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA700FA
	virtual	BOOL Save(CDBEngine* pDBEngine);

};
//CBusObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CLineObj	线路
/**
 * @defgroup 	CLineObj	路线类
 * @{
 */
/**
 * @brief       描述路线属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    // 
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA70128
class CLineObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA7012A
	CLineObj();
	//##ModelId=49B87BA70138
	~CLineObj();
	DECLARE_DYNCREATE(CLineObj)
//Attribute
public:

// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA70157
	virtual	BOOL Load(CMemSet * pMemSet);
};

//CLineObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBreakObj	开关
/**
 * @defgroup 	CBreakObj	开关类
 * @{
 */
/**
 * @brief       描述开关属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA70196
class CBreakObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA70198
	CBreakObj();
	//##ModelId=49B87BA701A5
	~CBreakObj();
	DECLARE_DYNCREATE(CBreakObj)
//Attribute
public:
	/** @brief           0：3/2母线侧开关1：3/2中间开关 2：双母母联开关 3：旁路开关 4：单母分段开关 5：线路开关*/
	//##ModelId=49B87BA701B5
	int		m_nBreakType;

// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA701C5
	virtual	BOOL Load(CMemSet * pMemSet);

};
//CBreakObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CGenObj	发电机
/**
 * @defgroup 	CGenObj	发电机类
 * @{
 */
/**
 * @brief       描述发电机属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BA701F4
class CGenObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA70204
	CGenObj();
	//##ModelId=49B87BA70213
	~CGenObj();
	DECLARE_DYNCREATE(CGenObj)
//Attribute
public:
	/** @brief           容量,功率,单位为MVA*/
	//##ModelId=49B87BA70222
	int		m_nVolume;
// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA70223
	virtual	BOOL Load(CMemSet * pMemSet);

};
//CGenObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CReactorObj	高压电抗器
/**
 * @defgroup 	CReactorObj	高压电抗器类
 * @{
 */
/**
 * @brief       描述高压电抗器属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA70261
class CReactorObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA70263
	CReactorObj();
	//##ModelId=49B87BA70271
	~CReactorObj();
	DECLARE_DYNCREATE(CReactorObj)
//Attribute
public:
	/** @brief           容量, 单位MVA*/
	//##ModelId=49B87BA70280
	int		m_nVolume;
// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA70281
	virtual	BOOL Load(CMemSet * pMemSet);

};
//CReactorObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CTransObj	变压器
/**
 * @defgroup 	CTransObj	变压器类
 * @{
 */
/**
 * @brief       描述变压器属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA702BF
class CTransObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA702CE
	CTransObj();
	//##ModelId=49B87BA702CF
	~CTransObj();
	DECLARE_DYNCREATE(CTransObj)
//Attribute
public:
	/** @brief           变压器类型 1: 三卷变 0：两卷变*/
	//##ModelId=49B87BA702DE
	int		m_nTransType;
	/** @brief           额定容量*/
	//##ModelId=49B87BA702DF
	int		m_nVolume;
	/** @brief           高压侧电压*/
	//##ModelId=49B87BA702EE
	int		m_nVoltageH;
	/** @brief           中压侧电压*/
	//##ModelId=49B87BA702EF
	int		m_nVoltageM;
	/** @brief           低压侧电压*/
	//##ModelId=49B87BA702F0
	int		m_nVoltageL;
// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA702F1
	virtual	BOOL Load(CMemSet * pMemSet);

};
//CTransObj		end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CSwitchObj	刀闸
/**
 * @defgroup 	CSwitchObj	刀闸类
 * @{
 */
/**
 * @brief       描述刀闸属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA7032C
class CSwitchObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA7032E
	CSwitchObj();
	//##ModelId=49B87BA7033C
	~CSwitchObj();
	DECLARE_DYNCREATE(CSwitchObj)
//Attribute
public:
	/** @brief           刀闸类型 0:非接地刀闸 1:接地刀闸*/
	//##ModelId=49B87BA7034B
	int		m_nSwitchType;
// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA7034C
	virtual	BOOL Load(CMemSet * pMemSet);
};

//CSwitchObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CCapObj	电容器
/**
 * @defgroup 	CCapObj	电容器类
 * @{
 */
/**
 * @brief       描述电容器属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA7037A
class CCapObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA7037C
	CCapObj();
	//##ModelId=49B87BA7038A
	~CCapObj();
	DECLARE_DYNCREATE(CCapObj)
//Attribute
public:
	/** @brief           容量,单位MVA*/
	//##ModelId=49B87BA70399
	int		m_nVolume;
// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA7039A
	virtual	BOOL Load(CMemSet * pMemSet);
};
//CCapObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CPmsObj	保护管理机
/**
 * @defgroup 	CPmsObj	保护管理机类
 * @{
 */
/**
 * @brief       描述保护管理机属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA703C8
class CPmsObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA703CA
	CPmsObj();
	//##ModelId=49B87BA703D8
	~CPmsObj();
	DECLARE_DYNCREATE(CPmsObj)
//Attribute
public:
// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA703D9
	virtual	BOOL Load(CMemSet * pMemSet);	
};
//CPmsObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CBayObj	间隔
/**
 * @defgroup 	CBayObj	间隔类
 * @{
 */
/**
 * @brief       描述间隔属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA8001F
class CBayObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA80021
	CBayObj();
	//##ModelId=49B87BA8002E
	~CBayObj();
	DECLARE_DYNCREATE(CBayObj)
//Attribute
public:
	/** @brief           SVG图形路径*/
	CString		m_sSvgPath;
// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA8002F
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	SaveSvgPath	 保存SVG路径到数据库 
 *  @param 		[In]a param of Type  CString  SVG文件路径
 *  @return 	void 
 */
	BOOL SaveSvgPath(CString strPath, CDBEngine* pDBEngine);
};
//CBayObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CSecObj	二次设备
/**
 * @defgroup 	CSecObj	二次设备类
 * @{
 */
/**
 * @brief       描述二次设备属性
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    //
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA800CB
class CSecObj : public CDeviceObj
{
public:
	//##ModelId=49B87BA800CD
	CSecObj();
	//##ModelId=49B87BA800DA
	~CSecObj();
	DECLARE_DYNCREATE(CSecObj)
//Attribute
public:
/** @brief           0：子站；1：母差保护；2：变压器保护 3：线路保护；4：发变组保护；
5：断路器保护；6：电抗器保护；8：电容器保护；9：故障录波器；
11母联保护；12低频解列装置；13：测控装置；14：安全自动装置；
	15：GPS装置；16:电能表；17:稳压装置；18: 低周减载装置；20:其他装置*/
	//##ModelId=49B87BA800DB
	int m_nSecType;	
	/** @brief           保护种类	主保护，后备保护…*/
	//##ModelId=49B87BA800EA
	CString	m_sKind;
	/** @brief           设备型号*/
	//##ModelId=49B87BA800EB
	CString	m_sModel;
	/** @brief           设备版本号(界面显示: 软件版本号)*/
	//##ModelId=49B87BA800EC
	CString	m_sSysVer;
	/** @brief           CPU个数*/
	//##ModelId=49B87BA800ED
	int		m_nCpuNum;
	/** @brief           设备时间(界面显示: 程序生成时间)*/
	//##ModelId=49B87BA800EE
	CString	m_sDevTime;
	/** @brief           设备校检码(界面显示: 通信校检码)*/
	//##ModelId=49B87BA800FA
	CString	m_sCrc;
	/** @brief           设备站内地址(界面显示: 装置地址)*/
	//##ModelId=49B87BA800FB
	CString	m_sAddr;
	/** @brief           通信口号(界面显示: 通信口)*/
	//##ModelId=49B87BA800FC
	int		m_nComID;
	/** @brief           所属一次设备ID*/
	//##ModelId=49B87BA800FD
	CString	m_sOwnerID;
	/** @brief           所属一次设备指针*/
	//##ModelId=49B87BA800FF
	CDeviceObj* m_pOwner;
	/** @brief           所属一次设备类型 所属一次设备类型：
								1;		母线
								2;		线路
								3;		开关
								4;		发电机
								5;		高压电抗器
								6;		变压器
								7;		刀闸
								8;		高压电容器
								9;		厂站
								10;		保护装置
								11;		录波器	*/
	//##ModelId=49B87BA8010B
	int		m_nOwnerType;
	/** @brief           保护类型ID*/
	//##ModelId=49B87BA8010C
	int		m_nPTTypeID;
	/** @brief           规约类型：141为modem，142为modbus，143为mlink*/
	//##ModelId=49B87BA80119
	int		m_nProtocolType;
	/** @brief           保护管理机ID*/
	//##ModelId=49B87BA8011A
	CString	m_sPMSID;
	/** @brief           装置103地址*/
	//##ModelId=49B87BA8011B
	int		m_n103Addr;

	/** @brief           所属厂站指针*/
	//##ModelId=49B87BA80129
	CStationObj* m_pStation;

	/** @brief           最后一次通信状态变位的时间*/
	//##ModelId=49B87BA8012D
	CTime	m_tmNewCommStatus;

	/** @brief           最后一次运行状态变位的时间*/
	//##ModelId=49B87BA8013A
	CTime	m_tmNewRunStatus;

	bool    m_bNewDevice;

	//信号量列表定义  add by yinzhehong 2005-09020
	/** @brief           故障特征量列表，来自表tb_pt_Action_character*/
	//##ModelId=49B87BA8014B
	CTypedPtrArray<CPtrArray,PT_ActionCharacter*> m_arrActChr;
	/** @brief           故障信号量列表，来自表tb_pt_event_def*/
	//##ModelId=49B87BA80158
	CTypedPtrArray<CPtrArray,PT_Signal*>          m_arrActionSignal;
    /** @brief           告警信号量列表，来自表tb_pt_alarm_def*/
	//##ModelId=49B87BA80168
    CTypedPtrArray<CPtrArray,PT_Signal*>          m_arrAlarmSignal;
	/** @brief           保护CPU信息*/
	//##ModelId=49B87BA8016D
	CTypedPtrArray<CPtrArray, CSecCPU*>			  m_arrCPU;

private:
	//防止数据库中无点表信息, 但每次有事件时都读库
	/** @brief           数据库中无特征量*/
	//##ModelId=49B87BA8017A
	BOOL	m_bNoActChr;
	/** @brief           数据库中无动作信号量*/
	//##ModelId=49B87BA8017B
	BOOL	m_bNoActSign;
	/** @brief           数据库中无告警信号量*/
	//##ModelId=49B87BA80186
	BOOL	m_bNoAlaSign;
// Operations
public:
/*
 *  @brief   	Load	 载入设备信息, 由子类实现 
 *  @return 	virtual BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA80187
	virtual	BOOL Load(CMemSet * pMemSet);

/*
 *  @brief   	LoadSimple	 载入设备信息 
 *  @param 		[In]a param of Type  CMemSet*  数据库数据
 *  @return 	BOOL 载入成功返回TRUE, 失败返回FALSE
 */
	BOOL LoadSimple(CMemSet* pMemSet);

/*
 *  @brief   	RefreshConfig	 刷新保护配置 
 *  @param 		[In/Out]a param of Type  BOOL  如果已经有配置是否强制刷新
 *  @return 	BOOL 刷新成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA80198
	BOOL RefreshConfig(CDBEngine* pDBEngine, BOOL bRefresh = FALSE);

/*
 *  @brief   	SetCommStatus	 改变装置通讯状态 
 *  @param 		[In]a param of Type  int  指定通讯状态. 0-正常.1-断开
 *  @return 	void 
 */

	//##ModelId=49B87BA801A6
	void SetCommStatus(int nStatus);

/*
 *  @brief   	SetRunStatus	 改变装置运行状态 
 *  @param 		[In]a param of Type  int  指定运行状态
 *  @return 	void 
 */
	//##ModelId=49B87BA801A8
	void SetRunStatus(int nStatus);

/*
 *  @brief   	AddZone	 加入定值区号 
 *  @param 		[In]a param of Type  PT_ZONE*  定值区号结构
 *  @return 	BOOL 加入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA801B6
	BOOL AddZone(PT_ZONE* ptz);

/*
 *  @brief   	RefreshCommDurativeTime	 刷新通信状态持续时间 
 *  @return 	void 
 */
	//##ModelId=49B87BA801B8
	CTimeSpan GetCommDurativeTime();

/*
 *  @brief   	RefreshRunDurativeTime	 刷新运行状态持续时间 
 *  @return 	void 
 */
	//##ModelId=49B87BA801B9
	CTimeSpan GetRunDurativeTime();

/*
 *  @brief   	LoadCommStatusFromSTTP	 从STTP报文中读取通信状态信息
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据
 *  @param 		[In]a param of Type  int  索引
 *  @return 	void 
 */
	//##ModelId=49B87BA801BA
	void LoadCommStatusFromSTTP(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	LoadRunStatusFromSTTP	 从STTP报文中读取运行状态信息
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  STTP报文数据
 *  @param 		[In]a param of Type  int  索引
 *  @return 	void 
 */
	//##ModelId=49B87BA801C6
	void LoadRunStatusFromSTTP(STTP_FULL_DATA& sttpData, int nIndex);

/*
 *  @brief   	RefreshStatus	 改变本设备的状态为指定设备的状态
 *  @param 		[In]a param of Type  CSecObj*  指定设备
 *  @return 	void 
 */
	void RefreshStatus(CSecObj* pSec);

/*
 *  @brief   	FindCpu	 指定CPU号查找CPU信息 
 *  @param 		[In]a param of Type  int  指定CPU号
 *  @return 	CSecCPU* CPU信息
 */
	CSecCPU* FindCpu(int nCpu);

private:
/*
 *  @brief   	GetActionChr	 从数据库读取保护动作特征值 
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA801C9
	BOOL GetActionChr(CDBEngine* pDBEngine);

/*
 *  @brief   	GetActionSignal	 从数据库中读取保护动作信号量 
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA801D4
	BOOL GetActionSignal(CDBEngine* pDBEngine);

/*
 *  @brief   	GetAlarmSignal	 从数据库中读取保护告警信号量 
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA801D5
	BOOL GetAlarmSignal(CDBEngine* pDBEngine);

/*
 *  @brief   	ClearConfig	 清除配置信息
 *  @param 		[In]a param of Type  int  要清除哪些配置信息.0-全部, 1-特征量, 2-动作信号, 3-告警信号
 *  @return 	BOOL 清除成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA801D6
	BOOL ClearConfig(int nAim = 0);

/*
 *  @brief   	GetCPUInfo	 从数据库读取保护CPU信息 
 *  @return 	BOOL 
 */
	//##ModelId=49B87BA801E5
	BOOL GetCPUInfo(CDBEngine* pDBEngine);
};

//CSecObj	end
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//CSecCPU 二次设备CPU类
/**
 * @defgroup 	CSecCPU	二次设备CPU类
 * @{
 */
/**
 * @brief       描述二次设备CPU信息
*  @author      LYH
 * @version     ver1.0
 * @date        23/09/08

 *
 * example
 * @code
 *    // 
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87BA80213
class CSecCPU : public CDeviceObj
{
public:
	//##ModelId=49B87BA80215
	CSecCPU();
	//##ModelId=49B87BA80222
	~CSecCPU();
	DECLARE_DYNCREATE(CSecCPU)
//attribute
public:
	/** @brief           装置ID*/
	//##ModelId=49B87BA80223
	CString	  PT_ID;
	/** @brief           CPU号*/
	//##ModelId=49B87BA80224
	int		  code;
	/** @brief           CPU描述*/
	//##ModelId=49B87BA80225
	CString   des;
	/** @brief           版本*/
	//##ModelId=49B87BA80226
	CString   ver;
	/** @brief           起始定值区号*/
	//##ModelId=49B87BA80227
	int       from_zone;
	/** @brief           结束定值区号*/
	//##ModelId=49B87BA80228
	int		  to_zone;
	/** @brief           功能号*/
	//##ModelId=49B87BA80229
	int       fun;
	/** @brief           是否使用定值区号链表.FALSE时使用from_zone & to_zone*/
	//##ModelId=49B87BA80232
	BOOL	  m_bUseSpeZone;
	/** @brief           定值区号链表*/
	//##ModelId=49B87BA80234
	CTypedPtrArray<CPtrArray, PT_ZONE*>	m_arrZONE;
	/** @brief           本CPU下定值的配置数*/
	int		m_nSettingDefs;
	/** @brief           本CPU下开关量的配置数*/
	int		m_nDigitalDefs;
	/** @brief           本CPU下软压板的配置数*/
	int		m_nSoftBoardDefs;
	/** @brief           本CPU下模拟量的配置数*/
	int		m_nAnalogDefs;
//operation
public:
/*
 *  @brief   	Load	 从数据库读入数据 
 *  @param 		[In]a param of Type  CMemSet *  数据集指针
 *  @return 	BOOL 读入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA80238
	BOOL Load(CMemSet * pMemSet);
private:
/*
 *  @brief   	GetZone	 从数据库读入定值区号 
 *  @return 	BOOL 读入成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA8023A
	BOOL GetZone(CDBEngine* pDBEngine);

/*
 *  @brief   	ClearZone	 清空定值区号链表 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA80242
	BOOL ClearZone();
};
//CSecCPU	end
//////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEOBJ_H__18F49415_AC3F_4CD2_BE1E_F4351573898C__INCLUDED_)
