// ***************************************************************
//  SttpEngine   version:  1.0    date: 25:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: STTP引擎,负责启动与关闭STTP客户端接口库,封装接口
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
#define AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SttpEngine.h : header file
//
#include "FaultReport.h"
#include "FaultEvent.h"

#include "vccommon_define.h"

//STTP报文相关定义
#define INQ 'I'		//消息类型INQ型
#define RES 'R'		//response型的
#define EX  'E'   //扩展型

#define FIX 'F'  //消息长度为固定型
#define VAR 'V'  //变长

/////////////////////////////////////////////////////////////////////////////
// CSttpEngine

/**
 * @defgroup 	CSttpEngine	STTP引擎
 * @{
 */
/**
 * @brief       负责启动与关闭STTP客户端接口库,封装接口
*  @author      LYH
 * @version     ver1.0
 * @date        25/09/08

 *
 * example
 * @code
 *    // 
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87B8900EA
class CSttpEngine
{
public:
	//##ModelId=49B87B8900EB
	CSttpEngine();           // protected constructor used by dynamic creation
	//##ModelId=49B87B8900FA
	~CSttpEngine();

// Attributes
public:
	CString	m_sDllName;
private:
	/** @brief           动态库句柄*/
	//##ModelId=49B87B890109
	HMODULE	m_hModule;
	

// Operations
public:

/*
 *  @brief   	XJSTTPConnectDestination	 与指定目标建立连接 
 *  @param 		[In]a param of Type  const STRUCT_DESTINATION_SERVER&  指定连接目标的结构体
 *  @param 		[In]a param of Type  int  客户端的类型编号，用于登录验证
 *  @param 		[Out]a param of Type  int &  输出连接状态 0-中断 1-连接
 *	@param		[In]a param of Type char*	  日志文件目录
 *  @param 		[In]a param of Type  int  运行模式
 *  @return 	XJHANDLE 创建的Sttp客户端对象的句柄
 */
	//##ModelId=49B87B89010A
	XJHANDLE XJSTTPConnectDestination(const STRUCT_DESTINATION_SERVER& pDestination , int pClientModle , int & pConnected, char* sLogPath,int pRunModle);

/*
 *  @brief   	XJSttpConnectServer	 建立与通信服务器的连接 
 *  @param 		[In]a param of Type  int  客户端的类型编号，用于登录验证
 *  @param 		[Out]a param of Type  int &  输出连接状态 0-中断 1-连接
 *	@param		[In]a param of Type char*	日志文件目录
 *  @param 		[In]a param of Type  int  运行模式0：同步模式（默认）1：异步模式
 *  @return 	XJHANDLE 创建的Sttp客户端对象的句柄
 */
	//##ModelId=49B87B890119
	XJHANDLE XJSttpConnectServer(int pClientModle, int & pConnected, char* sLogPath, int pRunModle);
	
/*
 *  @brief   	XJSttpCloseConnection	 关闭与服务端的连接 
 *  @param 		[In]a param of Type  XJHANDLE  欲关闭连接的句柄
 *  @return 	int 1:成功 0:无效的句柄
 */
	//##ModelId=49B87B89011E
	int XJSttpCloseConnection(XJHANDLE pClientHandle);
	
/*
 *  @brief   	XJSTTPActiveTest	 激活指定连接上的自动测试 
 *  @param 		[In]a param of Type  XJHANDLE  指定连接的句柄
 *  @return 	int 1:成功 0:无效的句柄
 */
	//##ModelId=49B87B890129
	int XJSTTPActiveTest(XJHANDLE pClientHandle);
	
/*
 *  @brief   	XJSTTPRead	 从指定连接上读取一条Sttp报文数据 
 *  @param 		[In]a param of Type  XJHANDLE  指定连接的句柄
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @return 	int 1:成功 同步模式下超时后无数据时返回0、异步模式下无数据立刻返回0，网络中断时返回-1
 */
	//##ModelId=49B87B890138
	int XJSTTPRead(XJHANDLE pClientHandle , STTP_FULL_DATA& pSttpData);
	
/*
 *  @brief   	XJSTTPWrite	 从指定连接上写入一条Sttp报文数据 
 *  @param 		[In]a param of Type  XJHANDLE  指定连接的句柄
 *  @param 		[In]a param of Type  const STTP_FULL_DATA&  报文数据
 *  @return 	int 1:成功 0:无效数据 -1:连接中断 -2:缓存已满
 */
	//##ModelId=49B87B890148
	int XJSTTPWrite(XJHANDLE pClientHandle , const STTP_FULL_DATA& pSttpData);
	
/*
 *  @brief   	XJSTTPSetOption	 为指定的连接设置定参数 
 *  @param 		[In]a param of Type  XJHANDLE  指定连接的句柄
 *  @param 		[In]a param of Type  int  参数类型 1：接收超时 2：发送超时
 *  @param 		[In]a param of Type  int  超时时间（ms）
 *  @param 		[In]a param of Type  int  备用
 *  @return 	int 
 */
	//##ModelId=49B87B890157
	int XJSTTPSetOption(XJHANDLE pClientHandle , int pOptionName , int pTime , int pReserved);
	
/*
 *  @brief   	XJSTTPRegisterHandler	 向指定连接注册回调函数指针 
 *  @param 		[In]a param of Type  XJHANDLE  指定连接的句柄
 *  @param 		[In]a param of Type  PFUNCALLBACK  回调函数指针
 *  @param 		[In]a param of Type  int  功能码 0：onConnectChange
 *  @return 	int 1:成功  0:失败
 */
	//##ModelId=49B87B890167
	int XJSTTPRegisterHandler(XJHANDLE pClientHandle , PFUNCALLBACK pFunction, int pMask );

/*
 *  @brief   	XJSTTPSetLogPath	 设置日志路径 
 *  @param 		[In]a param of Type  XJHANDLE  指定句柄
 *  @param 		[In]a param of Type  const char*  日志路径
 *  @return 	int 1:成功	0:失败
 */
	//##ModelId=49B87B89016B
	int XJSTTPSetLogPath(XJHANDLE pClientHandle ,const char* pLogPath);

/*
 *  @brief   	BuildDataFor20015PTSetting	 组建20015(保护定值下载要求)报文数据 
 *	@param		[Out]a param of Type STTP_FULL_DATA 报文数据
 *  @param 		[In]a param of Type  CString	保护装置ID
 *  @param 		[In]a param of Type  int	CPU号
 *  @param 		[In]a param of Type  int	定值区号
 *  @param 		[In]a param of Type  int	数据来源 0-保护装置(默认), 1-数据库.
 *  @param 		[In]a param of Type  int	定值数(默认0),
											当Source=0时	0   - 来之sttp客户端召唤全部定值
															103 - 来之于103主站召唤指定cpu的定值
											当Source=1 无意义,设为0

 *  @return 	BOOL 创建成功返回TRUE, 失败返回FALSE;
 */
	//##ModelId=49B87B890177
	BOOL BuildDataFor20015PTSetting(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom = 0, int nDataNum = 0);

/*
 *  @brief   	BuildDataFor20007PTAnalog	 组建20007(保护模拟量采样值下载要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  保护装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B89017E
	BOOL BuildDataFor20007PTAnalog(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20011PTDigital	 组建20011(保护开关量采样值下载要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  保护装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B890189
	BOOL BuildDataFor20011PTDigital(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20110PTGeneral	 组建20110(保护基本信息下载要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  保护装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *	@param		[In]a param of Type	 int  标志位,"1"召唤MMI版本信息，此时CPU号无意义；"0"召唤CPU版本信息
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B890196
	BOOL BuildDataFor20110PTGeneral(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nFlag);

/*
 *  @brief   	BuildDataFor20106PTZone	 组建20106(召唤定值区号要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  保护装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8901A5
	BOOL BuildDataFor20106PTZone(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);
	BOOL BuildDataFor20173PTZone(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20108PTSoftBoard	 组建20108报文(召唤软压板要求)数据
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  保护装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8901A9
	BOOL BuildDataFor20108PTSoftBoard(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20042DownloadWaveFile	 组建20042(录波文件下载要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  CString  要下载的录波文件名
 *  @param 		[In]a param of Type  int  数据来源 0-从装置 1-从子站
 *  @param 		[In]a param of Type  int  支持断点续传时，传输的起始偏移量，重新传输时全为零
 *  @param 		[In]a param of Type  int  要下载文件在装置中的编号（当数据源为0时有效）
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8901B6
	BOOL BuildDataFor20042DownloadWaveFile(STTP_FULL_DATA& sttpData, CString PTID, CString FileName, int nDataFrom, int nOffset, int nNo);

/*
 *  @brief   	BuildDataFor20025WaveFileList	 组建20025(录波文件列表下载要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  数据来源 0-从装置 1-从子站
 *  @param 		[In]a param of Type  CTime  起始查询时间,格式YYmmddHHMMSS
 *  @param 		[In]a param of Type  CTime  结束查询时间,格式YYmmddHHMMSS
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8901C5
	BOOL BuildDataFor20025WaveFileList(STTP_FULL_DATA& sttpData, CString PTID, int nDataFrom, CTime tmStart, CTime tmEnd);

/*
 *  @brief   	BuildDataFor20051SettingModify	 组建20051(保护定值修改要求) 报文数据
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @param 		[In]a param of Type  int  定值区号
 *  @param 		[In]a param of Type  int  数据目标：0-保护装置，1-数据库
 *  @param 		[In]a param of Type  MODIFY_DATA*  要修改的定值数据项链表
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8901CB
	BOOL BuildDataFor20051SettingModify(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom, MODIFY_LIST* modifyList);

/*
 *  @brief   	BuildDataFor20053SettingModifyExe	 组建20053(保护定值修改执行要求) 报文数据
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @param 		[In]a param of Type  int  定值区号
 *  @param 		[In]a param of Type  int  数据目标：0-保护装置，1-数据库
 *  @param 		[In]a param of Type  MODIFY_DATA*  要修改的定值数据项链表
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8901DA
	BOOL BuildDataFor20053SettingModifyExe(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom, MODIFY_LIST* modifyList);

/*
 *  @brief   	BuildDataFor20047SettingChange	 组建20047(保护定值切换预校要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @param 		[In]a param of Type  int  要切换到的定值区号
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8901E5
	BOOL BuildDataFor20047SettingChange(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone);

	BOOL BuildDataFor20169SettingChange(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int id);

/*
 *  @brief   	BuildDataFor20055SettingChange	 组建20055(保护定值切换执行要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @param 		[In]a param of Type  int  要切换到的定值区号
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8901EA
	BOOL BuildDataFor20055SettingChangeExe(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone);
	BOOL BuildDataFor20171SettingChangeExe(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int id);

/*
 *  @brief   	BuildDataFor20117SoftBoardModify	 组建20117(软压板投退预校要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @param 		[In]a param of Type  MODIFY_LIST*  要修改的软压板数据项链表
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8901F6
	BOOL BuildDataFor20117SoftBoardModify(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, MODIFY_LIST* modifyList);

/*
 *  @brief   	BuildDataFor20113SoftBoardModify	 组建20113(软压板投退执行要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @param 		[In]a param of Type  MODIFY_LIST*  要修改的软压板数据项链表
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B8901FB
	BOOL BuildDataFor20113SoftBoardModifyExe(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, MODIFY_LIST* modifyList);

/*
 *  @brief   	BuildDataFor20123WaveRecRun	 组建20123(录波器远方触发下载要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B890205
	BOOL BuildDataFor20123WaveRecRun(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20059WaveRecRun	 组建20059(保护对时要求)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B890209
	BOOL BuildDataFor20059PTSetTime(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20088PTTime	 组建20088(召唤装置时间)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B890213
	BOOL BuildDataFor20088PTTime(STTP_FULL_DATA& sttpData, CString PTID);

/*
 *  @brief   	BuildDataFor20057ResetPT	 组建20057(保护信号复归)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  CPU号
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B890216
	BOOL BuildDataFor20057ResetPT(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20137Cancel	 组建20137(取消操作命令报文)报文数据 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据
 *  @param 		[In]a param of Type  int  报文ID
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  装置CPU号
 *  @param 		[In]a param of Type  int  装置定值区号
 *  @param 		[In]a param of Type  int  事件类型0：无意义 非0：用于取消召唤历史事件 1-动作信息 2－自检信息 3－压板信息 
 *  @param 		[In]a param of Type  CString  起始时间
 *  @param 		[In]a param of Type  CString  结束时间
 *  @param 		[In]a param of Type  CString  录波文件名
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B89021A
	BOOL BuildDataFor20137Cancel(STTP_FULL_DATA& sttpData, int MsgID, CString PTID, int nCPU, int nZone = 0, int nEventType = 0, CString strStartTime = "", CString strEndTime = "", CString strFile = "");

/*
 *  @brief   	BuildDataFor20153Batch	 组建批量召唤信息要求(20153)报文
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *	@param		[In]a param of Type int 命令类型
 *	@param		[In]a param of Type int 站ID数组指针
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B890228
	BOOL BuildDataFor20153Batch(STTP_FULL_DATA& sttpData, int nType, CStringArray* arrStation = NULL);

/*
 *  @brief   	BuildDataFor20105HistoryEvent	 组建下载历史动作事件要求(20105)报文 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  CString  保护ID
 *  @param 		[In]a param of Type  int  事件类型,1-动作信息 2－自检信息 3－硬压板信息 4－SOE  5-测距 6-告警信息 0-全部 
 *  @param 		[In]a param of Type  CTime  起始时间
 *  @param 		[In]a param of Type  CTime  结束时间
 *  @param 		[In]a param of Type  int  数据来源
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B89022C
	BOOL BuildDataFor20105HistoryEvent(STTP_FULL_DATA& sttpData, CString PT_ID, int nType, CTime tmStart, CTime tmEnd, int nSrc);

/*
 *  @brief   	BuildDataFor00203CommonFileList	 组建召唤通用文件列表要求(00203)报文 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  CString  厂站ID
 *  @param 		[In]a param of Type  int  条件类型:1-按文件名称,2-指定时间段内的文件名称
 *  @param 		[In]a param of Type  CTime  查询起始时间
 *  @param 		[In]a param of Type  CTime  查询结束时间
 *  @param 		[In]a param of Type  CString  查询条件
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B890237
	BOOL BuildDataFor00203CommonFileList(STTP_FULL_DATA& sttpData, CString sStationID, int nConditionType, CTime tmStart, CTime tmEnd, CString sCondition);

/*
 *  @brief   	BuildDataFor00210CommonFileDown	 组建通用文件下载要求(00210)报文 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  CString  厂站ID
 *  @param 		[In]a param of Type  CString  文件名
 *  @param 		[In]a param of Type  int  偏移量
 *  @param 		[In]a param of Type  CString  子站IP
 *  @param 		[In]a param of Type  int  信息标识符
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B89023E
	BOOL BuildDataFor00210CommonFileDown(STTP_FULL_DATA& sttpData, CString sStationID, CString sFileName, int nOffest, CString sIP, int nRII = 0);

/*
 *  @brief   	BuildDataFor20151FaultPacket	 组建自动归档信息包变化通知(20151)报文 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  int  信息包变化类型
 *  @param 		[In]a param of Type  CFaultReport*  信息包指针
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B890247
	BOOL BuildDataFor20151FaultPacket(STTP_FULL_DATA& sttpData, int nType, CFaultReport* pReport);

/*
 *  @brief   	BuildDataFor20018FaultAlarm	 组建保护告警事件上载通知(20018)报文
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  CFaultEvent*  告警事件
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildDataFor20018FaultAlarm(STTP_FULL_DATA& sttpData, CFaultEvent* pEvent);

/*
 *  @brief   	BuildDataFor20010FaultDI	 组建保护开关量变位事件上载通知(20010)报文 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  CFaultEvent*  开关量变位事件
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildDataFor20010FaultDI(STTP_FULL_DATA& sttpData, CFaultEvent* pEvent);

/*
 *  @brief   	BuildDataFor20022FaultAction	 组建保护动作事件上载通知(20022)报文 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  动作事件
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildDataFor20022FaultAction(STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent);

/*
 *  @brief   	BuildDataFor20116FaultAlarm	 组建保护告警历史事件上载通知(20116)报文 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  告警事件
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildDataFor20116FaultAlarm(STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent);

/*
 *  @brief   	BuildDataFor20116FaultDI	 组建保护开关量历史事件上载通知(20116)报文  
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  开关量事件
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildDataFor20116FaultDI(STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent);

/*
 *  @brief   	BuildDataFor20116FaultAction	 组建保护动作历史事件上载通知(20116)报文 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  动作事件
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildDataFor20116FaultAction(STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent);

/*
 *  @brief   	BuildDataFor20145StationStatus	 组建厂站通讯状态下载要求 (20145) 报文
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  CString  厂站ID
 *  @param 		[In]a param of Type  int  通道号
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildDataFor20145StationStatus(STTP_FULL_DATA& sttpData, CString strStationID, int nChannel);

/*
 *  @brief   	BuildDataFor20145StationStatus	 组建装置运行状态上载报文
 *  @param 		[In,Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  CString  装置ID
 *  @param 		[In]a param of Type  int  装置状态ID
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildDataFor20079UpdatePtRunStatus(STTP_FULL_DATA& sttpData, CString& strPtID, int nPtStatus);

/*
 *  @brief   	BuildDataFor20145StationStatus	 3.139  录波文件重命令名要求
 *  @param 		[In,Out]a param of Type  STTP_FULL_DATA&  报文数据结构
 *  @param 		[In]a param of Type  CFaultEvent 录波事件对象
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL BuildDataFor20162RenameOscFile( STTP_FULL_DATA& sttpData, CFaultEvent* pEvent );

	BOOL BuildDataFor20138StationConfigChange(STTP_FULL_DATA& sttpData, CString strStationName);

	BOOL BuildDataFor20003CommStatus(STTP_FULL_DATA& sttpData, CString strPTID);

	BOOL BuildDataFor20085CommStatus(STTP_FULL_DATA& sttpData, CString strOscID);

public:
/*
 *  @brief   	LoadSttpLib	 加载STTP客户端动态库 
 *  @return 	BOOL 加载成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B89024B
	BOOL LoadSttpLib();

/*
 *  @brief   	FreeSttpLib	 卸载STTP客户端动态库 
 *  @return 	void 
 */
	void FreeSttpLib();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
