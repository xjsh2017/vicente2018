// XJBrowser.h : main header file for the XJBROWSER application
//

#if !defined(AFX_XJBROWSER_H__4F1B96FD_9E72_41FD_8D1C_A048D66C94A6__INCLUDED_)
#define AFX_XJBROWSER_H__4F1B96FD_9E72_41FD_8D1C_A048D66C94A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "XJBrowserDoc.h"
#include "LogMgr.h"			//日志管理
#include "DataEngine.h"
#include "SttpEngine.h"		//STTP引擎
#include "DBEngine.h"		//数据库引擎
#include "User.h"		//用户类
#include "XJBrowserView.h"
#include "EventManager.h"

#include "ViewFaultDetail.h"	//故障报告详细页面
#include "XJBrowserView.h"		//一次接线图页面
#include "PTSetting.h"			//保护定值页面
#include "PTAnalog.h"			//保护模拟量页面
#include "PTDigital.h"			//保护开关量页面
#include "PTSoftBoard.h"		//保护软压板页面
#include "WaveRecSetting.h"		//录波器定值页面
#include "PTAction.h"			//保护动作页面
#include "PTAlarm.h"			//保护告警页面
#include "AutoCallResult.h"		//自动总召结果页面
#include "ViewPTComm.h"			//保护通讯状态视图
#include "ViewPTRun.h"			//保护运行状态视图
#include "ViewComm.h"			//主子站通讯状态视图
#include "GeoView.h"			//地理图页面
#include "ViewStationInfo.h"	//厂站信息页面
#include "DownOscView.h"		//录波下载页面
#include "ViewCommMap.h"

#include "stdafx.h"


/////////////////////////////////////////////////////////////////////////////
// CXJBrowserApp:
// See XJBrowser.cpp for the implementation of this class
//
//##ModelId=49B87B7D0222
class CXJBrowserApp : public CWinApp
{
public:
	//##ModelId=49B87B7D0224
	CXJBrowserApp();
	//##ModelId=49B87B7D0271
	~CXJBrowserApp();
	
public:
	CBrush   m_brush;
	HANDLE   m_hPostEvent;
	HANDLE   m_hReceiveEvent;
	MENUINFO  lpcmi;  
	/** @brief           锁*/
	CRITICAL_SECTION m_CriticalSection;
	/** @brief           程序信号*/
	//##ModelId=49B87B7D0280
	HANDLE		hSem;
	/** @brief           LOG管理对象*/
	//##ModelId=49B87B7D0282
	CLogMgr		m_LogMgr;

	/** @brief           数据库引擎*/
	//##ModelId=49B87B7D0287
	CDBEngine	m_DBEngine;

	/** @brief           STTP引擎对象*/
	//##ModelId=49B87B7D0294
	CSttpEngine m_SttpEngine;

	/** @brief           STTP常连接句柄*/
	//##ModelId=49B87B7D02A0
	XJHANDLE		m_SttpHandle;

	/** @brief           当前登录的用户*/
	//##ModelId=49B87B7D02A5
	CUser  m_User;

	/** @brief           退出清理保护配置线程标志, 此值为TRUE时退出线程*/
	//##ModelId=49B87B7D02AF
	BOOL	m_bExitClearConfig;

	/** @brief           退出接收报文线程, 此值为TRUE时退出线程*/
	//##ModelId=49B87B7D02B0
	BOOL	m_bExitReceiveMsg;

	/** @brief           清理保护配置线程指针*/
	//##ModelId=49B87B7D02C0
	CWinThread* m_pClearConfigThread;
	/** @brief           接收报文线程指针*/
	//##ModelId=49B87B7D02CF
	CWinThread* m_pReceiveMsgThread;

	/** @brief           分发报文线程指针*/
	CWinThread* m_pPostMsgThread;

	CWinThread*	m_pPushMsgThread;

	/** @brief           故障报告详细信息视图指针,用于打印故障报告时的回调函数*/
	//##ModelId=49B87B7D02D4
	CViewFaultDetail*  m_pFaultDetail;

	/** @brief           一次接线图视图指针, 用于闪烁图形时的回调函数*/
	//##ModelId=49B87B7D02DF
	CXJBrowserView*		m_pMapView;

	/** @brief           保护定值页面指针, 用于打印定值的回调函数*/
	//##ModelId=49B87B7D02E4
	CPTSetting*			m_pPTSetting;

	/** @brief           保护模拟量页面指针, 用于打印模拟量的回调函数*/
	//##ModelId=49B87B7D02F2
	CPTAnalog*			m_pPTAnalog;

	/** @brief           保护开关量页面指针, 用于打印开关量的回调函数*/
	//##ModelId=49B87B7D02FE
	CPTDigital*			m_pPTDigital;

	/** @brief           保护软压板页面指针, 用于打印软压板的回调函数*/
	//##ModelId=49B87B7D030E
	CPTSoftBoard*		m_pPTSoftBoard;

	/** @brief           录波器定值页面指针, 用于打印定值的回调函数*/
	//##ModelId=49B87B7D0313
	CPTSetting*	m_pWRSetting;
		/** @brief           录波器开关量页面指针, 用于打印定值的回调函数*/
		//##ModelId=49B87B7D0313
	CPTDigital*	m_pWRDigital;
	/** @brief           录波器模拟量页面指针, 用于打印定值的回调函数*/
	//##ModelId=49B87B7D0313
	CPTAnalog*	m_pWRAnalog;
	/** @brief           保护动作页面指针, 用于打印动作事件列表的回调函数*/
	//##ModelId=49B87B7D031D
	CPTAction*			m_pPTAction;

	/** @brief           保护告警页面指针, 用于打印告警事件列表的回调函数*/
	//##ModelId=49B87B7D0322
	CPTAlarm*			m_pPTAlarm;

	/** @brief           自动总召结果页面指针, 用于打印自动总召结果列表的回调函数*/
	//##ModelId=49B87B7D0330
	CAutoCallResult*	m_pAutoCallResult;

	/** @brief           主子站通信状态页面指针, 用于打印主子站通信状态列表的回调函数*/
	//##ModelId=49B87B7D033D
	CViewComm*			m_pComm;

	/** @brief           保护通信状态页面指针, 用于打印的回调函数*/
	//##ModelId=49B87B7D0342
	CViewPTComm*		m_pPTComm;

	/** @brief           保护运行状态页面指针, 用于打印的回调函数*/
	//##ModelId=49B87B7D034F
	CViewPTRun*			m_pPTRun;

	/** @brief           厂站信息页面指针, 用于打印的回调函数*/
	CViewStationInfo*	m_pStationInfo;

	/** @brief           与通讯服务器连接情况.TRUE-连接正常.FALSE-连接中断*/
	//##ModelId=49B87B7D0360
	BOOL m_bConnect;
	/** @brief           按组保存通信情况*/
	int	m_ConnectStatus[2];

	BOOL m_bGetSysLogPath;

	//文档窗口管理	
	//##ModelId=49B87B7E030D
	enum WindowType
	{
		//##ModelId=49B87B7E031C
		wtMap			= 0,	//主接线画面窗口
		//##ModelId=49B87B7E031D
		wtCommuView		= 1,    //主子站通讯状态窗口
		//##ModelId=49B87B7E031E
		wtAction		= 2,	//动作事件窗口
		//##ModelId=49B87B7E031F
		wtAlarm			= 3,	//告警事件窗口
		//##ModelId=49B87B7E032C
		wtFault			= 4,	//事故报告窗口
		//##ModelId=49B87B7E032D
		wtOSC			= 5,	//录波报告窗口
		//##ModelId=49B87B7E032E
		wtDI			= 6,	//开关量事件窗口
		//##ModelId=49B87B7E032F
		wtPTCommStatus	= 7,	//保护通讯状态窗口
		//##ModelId=49B87B7E0330
		wtPTRunStatus	= 8,	//保护运行状态窗口
		//##ModelId=49B87B7E033C
		wtSysAlarm		= 9,	//系统告警告事件窗口
		//##ModelId=49B87B7E033D
		wtPTSheet		= 10,	//保护属性窗口
		//##ModelId=49B87B7E033E
		wtWaveRecSheet	= 11,	//录波器属性窗口
		//##ModelId=49B87B7E033F
		wtBatchCall		= 12,	//批量召唤窗口
		//##ModelId=49B87B7E034B
		wtAutoCall		= 13,	//自动总召设置窗口
		//##ModelId=49B87B7E034C
		wtAutoCallResult= 14,	//自动总召结果窗口
		//##ModelId=49B87B7E034D
		wtDownload		= 15,	//通用文件下载窗口
		wtCross			= 16,	//预警窗口
		wtSOEI			= 17,	//I类事件窗口
		wtSOEII			= 18,	//II类事件窗口
		wtSOEIII		= 19,	//III类事件窗口
		wtSOEDebug		= 20,	//调试信息窗口
		wtGeo			= 21,	//地理图窗口
		wtStationInfo	= 22,	//厂站信息窗口
		wtDownOsc		= 23,	//录波下载页面
		wtStaInfo       = 24,    //厂站信息窗口
		wtDistance		= 25,	//小波测距信息窗口
		wtDisDevSheet	= 26,	//测距装置属性页面
		wtDraft			= 27	//施工图页面
	};
	enum { maxDocCount=28 };	//最大文档数
	//##ModelId=49B87B7D0354
	CMultiDocTemplate *m_pDocTempl[maxDocCount];	//窗口列表
	//##ModelId=49B87B7D035C
	CDocument* m_pDoc[maxDocCount];					//文档列表

	//CList<STTP_FULL_DATA, STTP_FULL_DATA&> m_listFullData; 
	CPtrList m_listFullData;

private:
	/** @brief           文件下载路径*/
	//##ModelId=49B87B7D036B
	CString	m_sDownDir;
	/** @brief           系统日志文件路径*/
	//##ModelId=49B87B7D037A
	CString m_sSysLogPath;

	/** @brief           网络存储器用户名*/
	CString m_sNetworkUser;
	/** @brief           网络存储器密码*/
	CString m_sNetworkPassword;

	BOOL	m_bCanLock;
	BOOL	m_bIsLocked;

	
public:
	void LockUI();

	/*
	 *  @brief   	MgrAllStations 当前用户是否可管理所有子站	 
	 *  @return 	BOOL	 
	 */
	BOOL MgrAllStations(){return m_User.m_bAllStation;};

	/*
	 *  @brief   	MgrStation 判断指定厂站是否在当前用户的管理范围内	 
	 *	@param 		[In]a param of Type  CString stationID	
	 *  @return 	BOOL	 
	 */
	BOOL MgrStation(CString stationID);

private:

	void QueryUserStation();

	void StartClearConfigThread();
	
/*
*  @brief   	InitDocTemplates	 初始化文档类型 
*  @return 	void 
	*/
	//##ModelId=49B87B7D037B
	void InitDocTemplates();

/*
 *  @brief   	DoLogin	 检查用户身份,登录系统 
 *  @return 	BOOL 登录成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7D0399
	BOOL DoLogin();

/*
 *  @brief   	DOLogOut	 登出系统,更换用户登录 
 *  @return 	BOOL 再登录成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7D03B9
	BOOL DoLogOut();

	/*
	* @brief	JudgeOperTableType	判断日志表类型
	* @return   void	
	*/
	void JudgeOperTableType();
public:

	void XJSetMenuInfo();
	/*
 *  @brief   	TryEnter	 测试某用户是否具有进入某功能的权限,目前只支持查询当前用户功能
 *  @param 		[In]a param of Type  LPCTSTR  功能ID
 *  @param 		[In]a param of Type  LPCTSTR  用户ID, NULL时用当前用户测试
 *  @param 		[In]a param of Type  BOOL  是否显示警告窗
 *  @return 	BOOL 可进入某功能的权限返回TRUE,不可用返回FALSE
 */
	//##ModelId=49B87B7D03D8
	BOOL TryEnter(LPCTSTR strFuncID, LPCTSTR strUserID = NULL, BOOL bShowAlarm = TRUE);

/*
*  @brief   	GetCurDocument	 得到当前文档 
*  @return 	CXJBrowserDoc*	 文档类指针
	*/
	//##ModelId=49B87B7E000F
	CXJBrowserDoc* GetCurDocument();

	void SetAppTile(CString sTitle, CString sDetail = "");
	
	/*
	*  @brief   	GetCurView	 得到当前视图 
	*  @return 	CView*		 视图类指针
	*/
	//##ModelId=49B87B7E002E
	CView* GetCurView();

/*
 *  @brief   	GetDataEngine	 得到数据引擎指针 
 *  @return 	CDataEngine * 数据引擎指针
 */
	//##ModelId=49B87B7E004E
	CDataEngine * GetDataEngine();
	
/*
 *  @brief   	WriteLog	 写日志 
 *  @param 		[In]a param of Type  CString  日志内容
 *  @param 		[In]a param of Type  int  日志输出等级
 *  @return 	void 
 */
	//##ModelId=49B87B7E005D
	void WriteLog(CString sLog, int nLV = XJ_LOG_LV3);

/*
 *  @brief   	WriteLog	 写日志文件到日志文件, 因为发现线程中写日志如果在等待线程关闭时会引起死锁 
 *  @param 		[In/Out]a param of Type  CString  日志内容
 *  @param 		[In/Out]a param of Type  int  日志输出等级
 *  @return 	void 
 */
	//##ModelId=49B87B7E008C
	void WriteLogEx(CString sLog, int nLV = XJ_LOG_LV3);

/*
 *  @brief   	STTPConnect	 建立与通讯服务器的连接
 *	@param		[Out]a param of Type int&	输出连接结果 0-失败, 1-成功
 *  @param 		[In]a param of Type  int  运行模式. 0：同步模式（默认）, 1：异步模式
 *  @return 	XJHANDLE 创建的Sttp客户端对象的句柄，失败时返回NULL
 */
	//##ModelId=49B87B7E009C
	XJHANDLE STTPConnect(int& pConnected, int pRunModle = 0);

/*
 *  @brief   	ConnectSTTPServer	 建立与通讯服务器的连接, 并开启接收报文线程 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7E00BB
	BOOL ConnectSTTPServer();
	BOOL ConnectSTTPServerB();
/*
 *  @brief   	DisConnectSTTPServer	 断开与通讯服务器的连接, 并关闭接收报文线程 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87B7E00DA
	BOOL DisConnectSTTPServer();

/*
 *  @brief   	SetSTTPStatus	 设置与通讯服务器连接状态标志 
 *  @param 		[In]a param of Type  BOOL  TRUE表示连接正常, FALSE表示连接失败
 *  @return 	void 
 */
	//##ModelId=49B87B7E00EA
	void SetSTTPStatus(BOOL bConnected, int group);

/*
 *  @brief   	GetSTTPStatus	 取得当前与通讯服务器的连接状态 
 *  @return 	BOOL 通讯正常返回TRUE, 通讯中断返回FALSE
 */
	//##ModelId=49B87B7E0109
	BOOL GetSTTPStatus();

/*
 *  @brief   	AddManOperator	 增加人工操作日志 
 *  @param 		[In]a param of Type  CString  功能模块ID
 *  @param 		[In]a param of Type  CString  操作动作描述
 *  @return 	void
 */
	void AddNewManOperator( CString FunID, CString Act, CString strMsg, int nOperType, int nOperResult  = 0, int num = -1);
	void AddNewManOperator( CString FunID, CString Act, CString strMsg, CString strUser, int nOperType, int nOperResult  = 0, int num  = -1);

	BOOL NextPTSetModState(int nNextStateID, const PT_ZONE &data, const CString &sUserID, const CString &sRecords = CString());
	int  GetPTSetModState(PT_ZONE &data, CString &sRecords = CString(), CString &sFlag = CString());
	BOOL RevertPTSetModState(int nRevertStateID, int nFlag = 0);
	BOOL SetRevertModifyValueFlag(int nFlag);
	CString GetUserIDByState(int nState, CString &sRecords = CString());
	BOOL SaveTempPTSetToDB(CString &sPTID, MODIFY_LIST &arrModifyList);
	BOOL SaveNewPTSetToDB(CString &sCPU, CString &sPTID, CTypedPtrArray<CPtrArray, PT_SETTING*> &arrSetting);
	BOOL RevertTempPTSetToDB(const PT_ZONE &zone, int nFlag = 0);
	int CheckKeyNameForPTSet();

	CString GetUserGroupNameByID(CString sUserGroupID);
	BOOL SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));
	vector<CString> SplitCString(CString &str,const CString find, int limit=0);

/*
 *  @brief   	GetSVGView	 得到接线图指针 
 *  @return 	CXJBrowserView * 主接线图指针
 */
	//##ModelId=49B87B7E0128
	CXJBrowserView * GetSVGView();
	CViewCommMap * GetCommView();
	CGeoView* GetGeoView();
/*
 *  @brief   	LoadDownDir	  从数据库中读取文件下载路径,保存在m_sDownDir中
 *  @return 	void 
 */
	//##ModelId=49B87B7E0148
	void LoadDownDir();

/*
 *  @brief   	GetDownDir	 取得系统下载路径 
 *  @return 	CString 系统下载路径
 */
	//##ModelId=49B87B7E0157
	CString GetDownDir();

/*
 *  @brief   	LoadSysLogPath	 从数据库中读取日志文件路径,保存在m_sSysLogPath中
 *  @return 	void 
 */
	//##ModelId=49B87B7E0158
	void LoadSysLogPath();

/*
 *  @brief   	GetSysLogPath	 取得日志文件路径 
 *  @return 	CString 日志文件路径
 */
	//##ModelId=49B87B7E0167
	CString GetSysLogPath();

/*
 *  @brief   	ShowDevice	 显示某设备图形画面 
 *  @param 		[In]a param of Type  CDeviceObj*  设备指针
 *  @return 	void 
 */
	//##ModelId=49B87B7E0168
	void ShowDevice(CDeviceObj* pObj, BOOL bMoveFirst = TRUE);

/*
 *  @brief   	ShowStationInComm	 在厂站状态图形中显示厂站图形 
 *  @param 		[In]a param of Type  CStationObj* 指定厂站
 *  @return 	void 
 */
	void ShowStationInComm(CStationObj* pObj, BOOL bMoveFirst = TRUE);

/*
 *  @brief   	LocateInTree	 在设备树中定位厂站 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @return 	void 
 */
	//##ModelId=49B87B7E0177
	void LocateInTree(CStationObj* pObj);

/*
 *  @brief   	ShowEventProp	 显示事件属性对话框 
 *  @param 		[In]a param of Type  CXJEventManager*  事件管理指针
 *	@param		[In]a param of Type BOOL	是否是保护属性页面打开的对话框
 *  @return 	void 
 */
	//##ModelId=49B87B7E0186
	void ShowEventProp(CXJEventManager* pEventManager, BOOL bPT = FALSE);

/*
 *  @brief   	EndOwnerThread	 停止自己起的所有线程 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01A5
	void EndOwnerThread();

/*
 *  @brief   	LoadConfig	 读入所有配置文件 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01B5
	void LoadConfig();

/*
 *  @brief   	SaveConfig	 保存所有配置 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01B6
	void SaveConfig();

/*
 *  @brief   	LoadSysConfig	 从配置文件中读入系统配置 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01B7
	void LoadSysConfig();

/*
 *  @brief   	SaveSysConfig	 保存系统配置到配置文件
 *  @return 	void 
 */
	//##ModelId=49B87B7E01C5
	void SaveSysConfig();

/*
 *  @brief   	LoadEventConfig	 从配置文件读入事件配置 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01D4
	void LoadEventConfig();

/*
 *  @brief   	SaveEventConfig	 保存事件配置到配置文件
 *  @return 	void 
 */
	//##ModelId=49B87B7E01D5
	void SaveEventConfig();

/*
 *  @brief   	LoadCommStatus	 从配置文件读入主子站通讯状态页面配置
 *  @return 	void 
 */
	//##ModelId=49B87B7E01E4
	void LoadCommStatusConfig();

/*
 *  @brief   	SaveCommStatus	 保存主子站通讯状态页面配置到配置文件 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01E5
	void SaveCommStatusConfig();

/*
 *  @brief   	LoadActionViewConfig	 从配置文件读入动作事件页面配置 
 *  @return 	void 
 */
	//##ModelId=49B87B7E01F4
	void LoadActionViewConfig();

/*
 *  @brief   	SaveActionViewConfig	 保存动作事件页面配置到配置文件
 *  @return 	void 
 */
	//##ModelId=49B87B7E01F5
	void SaveActionViewConfig();

/*
 *  @brief   	LoadAlarmViewConfig	 从配置文件读入告警事件页面配置 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0203
	void LoadAlarmViewConfig();

/*
 *  @brief   	SaveAlarmViewConfig	 保存告警事件页面配置到配置文件 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0204
	void SaveAlarmViewConfig();

/*
 *  @brief   	LoadSysAlarmViewConfig	 从配置文件读入系统告警页面配置 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0213
	void LoadSysAlarmViewConfig();

/*
 *  @brief   	SaveSysAlarmViewConfig	 保存系统告警页面配置到配置文件 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0214
	void SaveSysAlarmViewConfig();

/*
 *  @brief   	LoadBatchConfig	 载入批量召唤配置从配置文件 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0222
	void LoadBatchConfig();

/*
 *  @brief   	SaveBatchConfig	 保存批量召唤配置到配置文件 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0223
	void SaveBatchConfig();

/*
 *  @brief   	LoadPTCommViewConfig	 读入保护通讯状态页面配置 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0224
	void LoadPTCommViewConfig();

/*
 *  @brief   	SavePTCommViewConfig	 保存保护通讯状态页面配置到配置文件 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0232
	void SavePTCommViewConfig();

/*
 *  @brief   	LoadPTRunViewConfig	 读入保护运行状态页面配置 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0233
	void LoadPTRunViewConfig();

/*
 *  @brief   	SavePTRunViewConfig	 保存保护运行状态页面配置
 *  @return 	void 
 */
	//##ModelId=49B87B7E0234
	void SavePTRunViewConfig();

/*
 *  @brief   	LoadPTPropConfig	 读入保护属性页面配置 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0242
	void LoadPTPropConfig();

/*
 *  @brief   	SavePTPropConfig	 保存保护属性页面配置 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0243
	void SavePTPropConfig();

/*
 *  @brief   	LoadWndConfig	 读入窗口显示配置 
 *  @return 	void 
 */
	void LoadWndConfig();

/*
 *  @brief   	SaveWndConfig	 保存窗口显示配置 
 *  @return 	void 
 */
	void SaveWndConfig();

/*
 *  @brief   	LoadSplitterWndConfig	 读入分隔窗口大小配置 
 *  @return 	void 
 */
	void LoadSplitterWndConfig();

/*
 *  @brief   	SaveSplitterWndConfig	 保存分隔窗口大小配置 
 *  @return 	void 
 */
	void SaveSplitterWndConfig();

/*
 *  @brief   	OpenDefaultStation	 打开默认的厂站 
 *  @param 		[In]a param of Type  CString  厂站ID
 *  @return 	void 
 */
	//##ModelId=49B87B7E0244
	void OpenDefaultStation(CString strID);
	void OpenDefaultStation();

/*
 *  @brief   	QueryUserGroup	 指定用户名查找用户组名 
 *  @param 		[In]a param of Type  CString  用户名
 *  @return 	CString 用户组名
 */
	//##ModelId=49B87B7E0251
	CString QueryUserGroup(CString sUserID);

/*
 *  @brief   	DoSysSetting	 进入系统设置页面
 *  @param 		[In]a param of Type  int  初始打开哪个SHEET页. SYS_SETTING_GENERAL为基本设置页面. SYS_SETTING_INFOCASE为信息定制页面. const SYS_SETTING_STATUS为状态设置页面. SYS_SETTING_PTPTOP为保护属性设置页面
 *  @return 	void 
 */
	//##ModelId=49B87B7E0253
	void DoSysSetting(int nPage);

/*
 *  @brief   	ClearAllTempView	 清除保存的零时视图指针 
 *  @return 	void 
 */
	//##ModelId=49B87B7E0261
	void ClearAllTempView();

/*
 *  @brief   	AddNetworkDisk	 挂载网络存储器 
 *  @return 	BOOL 成功返回TRUE, 失败返回FALSE
 */
	BOOL AddNetworkDisk();

/*
 *  @brief   	ShowStationInfo	 打开指定厂站的厂站信息视图 
 *  @param 		[In]a param of Type  CStationObj*  指定厂站
 *  @return 	void 
 */
	void ShowStationInfo(CStationObj* pStation);

/*
 *  @brief   	ShowDownOsc	 打开录波下载窗口，并下载指定录波 
 *  @param 		[In]a param of Type  CXJEventOSC*  指定下载的录波
 *  @return 	void 
 */
	void ShowDownOsc(CXJEventOSC* pOsc);

/*
 *  @brief   	AddSTTPData	 将STTP加入到队列 
 *  @param 		[In]a param of Type  STTP_FULL_DATA&  sttp报文
 *  @return 	void 
 */
	void AddSTTPData(STTP_FULL_DATA* sttpData);

	STTP_FULL_DATA* GetFirstSTTPData();

/*
 *  @brief   	RemoveSTTPData	 将STTP队列的第一个数据删除 
 *  @return 	void 
 */
	void RemoveSTTPData();

	void RemoveAllSTTPData();

/*
 *  @brief   	QueryPtRunStatus	 查询二次设备运行状态
 *  @param 		[In]a param of Type  CString  二次设备ID
 *  @param 		[In,out]a param of Type  int  状态值
 *  @param 		[In,out]a param of Type  CString  二次设备运行时间
 *  @return 	void 
 */
	BOOL QueryPtRunStatus(CString strPtID,  int& nRunStatus, CString& strRunTime);

/*
 *  @brief   	UpdatePtRunStatus	 更新二次设备运行状态
 *  @param 		[In]a param of Type  CString  二次设备ID
 *  @param 		[In]a param of Type  状态值
 *  @return 	void 
 */
	void UpdatePtRunStatus(CString strPtID, int nRunStatus);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXJBrowserApp)
	public:
	virtual BOOL InitInstance();
	
	
	
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CXJBrowserApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	afx_msg void OnSttpConnect();
	afx_msg void OnUpdateSttpConnect(CCmdUI* pCmdUI);
	afx_msg void OnSttpDisconnect();
	afx_msg void OnUpdateSttpDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnSysLogout();
	afx_msg void OnUpdateSysLogout(CCmdUI* pCmdUI);
	afx_msg void OnSysExit();
	afx_msg void OnUpdateSysExit(CCmdUI* pCmdUI);
	afx_msg void OnHelpContent();
	afx_msg void OnUpdateHelpContent(CCmdUI* pCmdUI);
	afx_msg void OnSysSetting();
	afx_msg void OnUpdateSysSetting(CCmdUI* pCmdUI);
	afx_msg void OnAppCall();
	afx_msg void OnCallAll();
	afx_msg void OnFileDownload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


typedef struct
{
	CDeviceObj* pPoint;
	int   nId;
}TimerTick;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XJBROWSER_H__4F1B96FD_9E72_41FD_8D1C_A048D66C94A6__INCLUDED_)
