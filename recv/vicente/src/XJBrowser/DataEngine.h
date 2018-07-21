// ***************************************************************
//  DataEngine   version:  1.0    date: 16:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 数据引擎类,负责读取和存储各设备数据
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************

#if !defined(AFX_DATAENGINE_H__CE10AD99_5FEB_4D62_B91B_5323AB0644D5__INCLUDED_)
#define AFX_DATAENGINE_H__CE10AD99_5FEB_4D62_B91B_5323AB0644D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataEngine.h : header file
//
#include "DeviceObj.h"
#include "..\..\COMMON\define.h"	// Added by ClassView
#include "Department.h"

extern CString virtualStationID;

/////////////////////////////////////////////////////////////////////////////
// CDataEngine document
/**
 * @defgroup 	CDataEngine	数据引擎类
 * @{
 */
/**
 * @brief       负责各设备数据,配置信息的读取和存储
*  @author      LYH
 * @version     ver1.0
 * @date        16/09/08
/** @} */ //OVER

//##ModelId=49B87BA9003E
class CDataEngine
{
public:
	//##ModelId=49B87BA9003F
	CDataEngine();

// Attributes
public:
	/** @brief           地区链表*/
	//##ModelId=49B87BA9004F
	NET_LIST		m_NetList;
	/** @brief           厂站链表*/
	//##ModelId=49B87BA9005E
	CMapStringToOb	m_StationMap;
	/** @brief           厂站通断原因表*/
	STATION_STATUS_CONFIG_MAP m_StationStatusConfigMap;

	/** @brief			分站链表*/
	CMapStringToOb m_SepStationMap;

	/** @brief           挂牌信息链表*/
	MARK_LIST		m_arrMark;

	/** @brief           所有二次设备链表, 用于刷新状态*/
	SEC_LIST		m_AllSEC;
	
	/** @brief           通用故障特征量列表，来自tb_pt_character*/
	//##ModelId=49B87BA90063
	CTypedPtrArray<CPtrArray,PT_ActionCharacter*> m_arrUniversalActChr;

	/** @brief           保护召唤定值超时时间(单位:秒)*/
	//##ModelId=49B87BA90067
	int		m_nPTSettingTimeOut;
	/** @brief           录波器召唤定值超时时间(单位:秒)*/
	//##ModelId=49B87BA90068
	int		m_nWRSettingTimeOut;
	/** @brief           保护召唤模拟量超时时间(单位:秒)*/
	//##ModelId=49B87BA9006D
	int		m_nPTAiTimeOut;
	/** @brief           保护召唤开关量超时时间(单位:秒)*/
	//##ModelId=49B87BA9006E
	int		m_nPTDiTimeOut;
	/** @brief           保护召唤软压板超时时间(单位:秒)*/
	//##ModelId=49B87BA9006F
	int		m_nPTSoftBoardTimeOut;
	/** @brief           保护召唤定值区号超时时间(单位:秒)*/
	//##ModelId=49B87BA90070
	int		m_nPTZoneTimeOut;
	/** @brief           召唤装置时间超时时间(单位:秒)*/
	//##ModelId=49B87BA90071
	int		m_nDevTimeTimeOut;
	/** @brief           召唤装置基本信息超时时间(单位:秒)*/
	//##ModelId=49B87BA9007D
	int		m_nDevGeneralTimeOut;
	/** @brief           定值修改超时时间(单位:秒)*/
	//##ModelId=49B87BA9007E
	int		m_nModifySettingTimeOut;
	/** @brief           定值区切换超时时间(单位:秒)*/
	//##ModelId=49B87BA9007F
	int		m_nChangeZoneTimeOut;
	/** @brief           软压板切换超时时间(单位:秒)*/
	//##ModelId=49B87BA90080
	int		m_nChangeSBTimeOut;
	/** @brief           装置对时超时时间(单位:秒)*/
	//##ModelId=49B87BA90081
	int		m_nVerifyTimeTimeOut;
	/** @brief           信号复归超时时间(单位:秒)*/
	//##ModelId=49B87BA9008C
	int		m_nSignalResetTimeOut;
	/** @brief           远方启动超时时间(单位:秒)*/
	//##ModelId=49B87BA9008D
	int		m_nRemoteStartTimeOut;
	/** @brief           召唤录波文件超时时间(单位:秒)*/
	//##ModelId=49B87BA9008E
	int		m_nWavFileTimeOut;
	/** @brief           召唤录波列表超时时间(单位:秒)*/
	//##ModelId=49B87BA9009C
	int		m_nWavFileListTimeOut;
	/** @brief           召唤历史事件超时时间(单位:秒)*/
	//##ModelId=49B87BA9009D
	int		m_nHistoryEventTimeOut;
	/** @brief           召唤通用文件超时时间(单位:秒)*/
	//##ModelId=49B87BA9009E
	int		m_nCommFileTimeOut;
	/** @brief           召唤通用文件列表超时时间(单位:秒)*/
	//##ModelId=49B87BA900AB
	int		m_nCommFileListTimeOut;
	/** @brief           召唤运行状态时间(单位:秒)*/
	int     m_nCommStatusTimeOut;
	CMemSet* m_pMemset;
	CWinThread* m_pThread;
	CRITICAL_SECTION m_criSection;
	/** @brief			检修单位MAP*/
	CMapWordToOb m_mapServiceDepartment;
	/** @brief           情况说明描述*/
	CStringArray	m_arrSituation;
	/** @brief           处理意见描述*/
	CStringArray	m_arrDispose;
// Operations
private:
	/*
	* @brief	ClearNetList 清空地区链表	
	* @return   void	
	*/
	void ClearNetList();
	/*
	* @brief	ClearSepStationList	清空分站链表
	* @return   void	
	*/
	void ClearSepStationList();

	/*
	* @brief	LoadStationFromDB 从数据库载入指定厂站的信息	
	* @param	[In]a param of Type CString sStationID	指定要载入的厂站ID	
	* @param	[In]a param of Type CStationObj * pStation	保存厂站信息的对象，如果为NULL，函数将新建厂站对象并在返回值中返回	
	* @return   CStationObj* 厂站对象	
	*/
	CStationObj* LoadStationFromDB(CString sStationID, CStationObj* pStation);

private:
/*
 *  @brief   	BuildNetList	 建立地区链表 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA900AC
	BOOL BuildNetList();

/*
 *  @brief   	BuildStationList	 建立厂站链表 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA900BB
	BOOL BuildStationList();

	/*
 *  @brief   	BuildSepStationList	 建立分站站链表 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA900BB
	BOOL BuildSepStationList();

/*
 *  @brief   	BuildBusList	 建立母线链表 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA900DA
	BOOL BuildBusList();

/*
 *  @brief   	BuildBreakList	 建立开关链表
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA900DB
	BOOL BuildBreakList();

/*
 *  @brief   	BuildBayList	 建立间隔链表 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA900EA
	BOOL BuildBayList();

/*
 *  @brief   	BuildLineList	 建立线路链表 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA90109
	BOOL BuildLineList();

/*
 *  @brief   	BuildCapList	 建立电容器链表 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA9010A
	BOOL BuildCapList();

/*
 *  @brief   	BuildReactorList	 建立电抗器链表 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA90119
	BOOL BuildReactorList();

/*
 *  @brief   	BuildTransList	 建立变压器链表 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA90128
	BOOL BuildTransList();

/*
 *  @brief   	BuildGenList	建立发电机链表
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA90129
	BOOL BuildGenList();

/*
 *  @brief   	BuildSwitchList	 建立刀闸链表 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA90138
	BOOL BuildSwitchList();

/*
 *  @brief   	BuildPmsList	 建立保护管理机链表s 
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA90139
	BOOL BuildPmsList();

/*
 *  @brief   	BuildSecList	 建立二次设备链表 
 *  @return 	BOOL 建立成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA90148
	BOOL BuildSecList();


/*
 *  @brief   	BuildCpuList	 建立CPU链表
 *  @return 	BOOL 建立成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA90149
	BOOL BuildCpuList();

/*
 *  @brief   	BuildZoneList	 建立定值区号链表 
 *  @return 	BOOL 建立成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA9014A
	BOOL BuildZoneList();

/*
 *  @brief   	BuildZoneList	 建立定值区号链表,从tb_secdev_zone_base读取数据
 *  @return 	BOOL 建立成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildZoneListOld();

/* 
 *  @brief   	BuildStationStatusConfig()  建立厂站通断原因表
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA900AC
	BOOL BuildStationStatusConfig();

	/* 
 *  @brief   	LoadManagedepartmentfromDB()  查询所调管单位
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
	BOOL LoadManagedepartmentfromDB();
	/* 
 *  @brief   	LoadMaintaindepartmentfromDB()  查询运维单位
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
BOOL LoadMaintaindepartmentfromDB();

/* 
 *  @brief   	LoadServiceDepartmentFromDB()  查询检修单位,建立检修单位MAP表
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
BOOL LoadServiceDepartmentFromDB();

/****************************************************

	/* 
 *  @brief   	LoadMarkInfofromDB()  查询挂牌信息
 *  @return 	BOOL 建立成功返回TRUE,失败返回FALSE
 */
BOOL LoadMarkInfofromDB();
/*
 *  @brief   	Clear	 清除数据 
 *  @return 	void 
 */
	//##ModelId=49B87BA90158
	void Clear();

/*
 *  @brief   	ClearRefreshSec	 清空用于刷新设备状态的二次设备链表 
 *  @return 	void 
 */
	void ClearRefreshSec();


/*
 *  @brief   	BuildUniversalActChr	 创建通用的动作特征量 
 *  @return 	BOOL 创建成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA90159
	BOOL BuildUniversalActChr();

/*
 *  @brief   	GetFuncTimeOut	 从数据库中取得各通信操作功能超时时间 
 *  @return 	BOOL 读取成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA9015A
	BOOL GetFuncTimeOut();

	CStationObj* AddVirtualStation(CString stationID, CString stationName, CString netID);

	/*
	 *  @brief   	LoadSituationAndDisposeFromDB	 从数据库查询巡检情况说明和处理意见的描述配置
	 *  @return 	BOOL	查询成功返回TRUE, 失败返回FALSE 
	 */
	BOOL LoadSituationAndDisposeFromDB();

	BOOL QueryPTNoteCount(); 



public:
/*
 *  @brief   	QueryHasSettingConfig	 查询各二次设备是否有定值配置 
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL QueryHasSettingConfig();

/*
 *  @brief   	QueryHasDigitalConfig	 查询各二次设备是否有开关量配置 
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL QueryHasDigitalConfig();

/*
 *  @brief   	QueryHasSoftBoardConfig	 查询各二次设备是否有软压板配置 
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL QueryHasSoftBoardConfig();

/*
 *  @brief   	QueryHasAnalogConfig	 查询各二次设备是否有模拟量配置 
 *  @return 	BOOL 查询成功返回TRUE, 失败返回FALSE
 */
	BOOL QueryHasAnalogConfig();


/*
 *  @brief   	LoadAllSec	 载入所有二次设备 
 *  @return 	void 
 */
	void LoadAllSec();

/*
 *  @brief   	RefreshDevStatus	 刷新设备状态 
 *  @return 	void 
 */
	void RefreshDevStatus();

/*
 *  @brief   	RefreshStationStatus	 刷新子站状态 
 *  @return 	void 
 */
	BOOL RefreshStationStatus();

/*
 *  @brief   	LoadData	 载入数据 
 *  @return 	BOOL 载入成功返回TRUE,失败返回FALSE
 */
	//##ModelId=49B87BA9015B
	BOOL LoadData();

/*
 *  @brief   	FindDevice	 查找设备 
 *  @param 		[In]a param of Type  CString  设备ID
 *  @param 		[In]a param of Type  int  设备类型
 *  @return 	CDeviceObj * 设备对象指针
 */
	//##ModelId=49B87BA90167
	CDeviceObj * FindDevice(CString DeviceID, int DeviceType, CString StationID = "");

	/*
	* @brief	FindSepByStation	查找指定厂站的分站
	* @param	[In]a param of Type CString sStationID	指定厂站ID	
	* @return   CDeviceObj*	
	*/
	CDeviceObj* FindSepByStation(CString sStationID);

	/*
	* @brief	FindSepStation	查找指定分站
	* @param	[In]a param of Type CString sSepID	指定分站ID	
	* @return   CDeviceObj*	
	*/
	CDeviceObj* FindSepStation(CString sSepID);

/*
 *  @brief   	FindDeviceBy103	 通过103组号条目号查找设备 
 *  @param 		[In]a param of Type  CString  103组号条目号,格式: 组号#条目号
 *  @param 		[In]a param of Type  int  设备类型
 *  @param 		[In]a param of Type  CString  厂站ID
 *  @return 	CDeviceObj * 设备指针
 */
	//##ModelId=49B87BA9016A
	CDeviceObj * FindDeviceBy103(CString str103, int DeviceType, CString strStaionID);


/*
 *  @brief   	FindDeviceBy103	 根据uri查找设备 
 *  @param 		[In]a param of Type  CString uri
 *  @param 		[In]a param of Type  CString  厂站ID
 *  @return 	CDeviceObj * 设备指针
 */
	//##ModelId=49B87BA6035B
	CDeviceObj * FindDeviceByUri(CString strUri, CString strStaionID);



/*
 *  @brief   	QueryStationStatus	 查询厂站状态 
 *  @return 	void 
 */
	void QueryStationStatus();


	void Load20162Config();

/*
 *  @brief   	GetPriDeviceList	 取得一次设备链表 
 *  @param 		[In]a param of Type  DEVICE_LIST& 用于保存一次设备的链表
 *  @param 		[In]a param of Type  int 指定要查找的一次设备类型，-1表示所有类型
 *  @param 		[In]a param of Type  CString 指定所属厂站ID，为空时表示所有厂站
 *  @return 	
 */
	void GetPriDeviceList(DEVICE_LIST& listDevice, int nDeviceType, CString sStationID="");

/*
 *  @brief   	GetSecList	 取得二次设备链表 
 *  @param 		[In]a param of Type  DEVICE_LIST& 用于保存二次设备的链表
 *  @param 		[In]a param of Type  CString 指定一次设备，为空时表示所有
 *  @param 		[In]a param of Type  int 指定要查找的二次设备类型，-1或TYPE_SEC表示所有类型, TYPE_PROTECT表示保护，TYPE_WAVEREC表示录波器
 *  @param 		[In]a param of Type  CString 指定所属厂站ID，为空时表示所有厂站
 *	@param		[In]a param of Type  int 指定二次设备的细分类型，对应tb_secdev_base表的type字段
 *  @return 	
 */
	void GetSecList(DEVICE_LIST& listDevice, CString sPriID = "", int nSecType = TYPE_SEC, CString sStationID = "", int nDetailKind = -1 );

/*
 *  @brief   	GetStationList	 取得厂站链表 
 *  @param 		[In]a param of Type  DEVICE_LIST& 用于保存厂站的链表
 *  @param 		[In]a param of Type  CString 指定地区ID，为空时表示所有地区
 *  @return 	
 */
	void GetStationList(DEVICE_LIST& listDevice, CString sNetID = "");

/*
 *  @brief   	GetSortStationList	 取得经过排序的厂站链表，按去除电压等级表示的厂站名进行排序(如500kV殿山变只按殿山变来比较)
 *  @param 		[In]a param of Type  DEVICE_LIST& 用于保存厂站的链表
 *  @param 		[In]a param of Type  CString 指定地区ID，为空时表示所有地区
 *  @return 	
 */
	void GetSortStationList(DEVICE_LIST& listDevice, CString sNetID = "");

	/*
	* @brief	GetServiceDepartmentName 取得检修单位名称	
	* @param	[In]a param of Type int nID	检修单位ID	
	* @return   CString	检修单位名称
	*/
	CString GetServiceDepartmentName(int nID);

	/*
	* @brief	StationInit	厂站初始化
	* @param	[In]a param of Type CString sStationID	厂站ID	
	* @param	[In]a param of Type int nType	初始化类型	
	* @return   CStationObj* 厂站对象	
	*/
	CStationObj* StationInit(CString sStationID, int nType);

		BOOL QueryPTNotes(CString ptid, CPtrArray& arr);

// Implementation

	//##ModelId=49B87BA90179
	virtual ~CDataEngine();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAENGINE_H__CE10AD99_5FEB_4D62_B91B_5323AB0644D5__INCLUDED_)
