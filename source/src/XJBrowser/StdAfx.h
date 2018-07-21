// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9CE24E2_ABB2_4126_B1EB_E7159EA52EF3__INCLUDED_)
#define AFX_STDAFX_H__A9CE24E2_ABB2_4126_B1EB_E7159EA52EF3__INCLUDED_


#if(WINVER < 0x0500)
#undef WINVER
#define WINVER 0x0501
#endif

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0500

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
// #include <afxdisp.h>        // MFC Automation classes
// #include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxmt.h>
#import <msxml6.dll>
#include <afxole.h>
#pragma warning(disable:4700)

#include "PictureEx.h"	//Gif动画类
#include "..\..\ui_common\OXSplashWnd\OXSplashWnd.h"
#include "..\..\common\define.h"
#include "..\..\common\MessageLog.h"
#include "..\..\common\SttpDefine.h"
#include "..\..\common\MemSet.h"
#include "..\..\common\MemField.h"
#include "..\..\common\XJErrorID.h"
#include "ListCtrlEx.h"
#include "..\..\ui_common\button\XPStyleButtonST.h"
#include "..\..\ui_common\button\ThemeHelperST.h"
#include "HideHeaderCtrl.h"
#include "OXTreeCtrl.h"
#include "TreeListHeaderCtrl.h"
#include "TreeListTipCtrl.h"
#include "TreeListStaticCtrl.h"
#include "TreeListEditCtrl.h"
#include "TreeListComboCtrl.h"
#include "TreeListItem.h"
#include "TreeListCtrl.h"
#include "MyTreeListCtrl.h"
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "..\..\ui_common\Interface\SvgViewerInterface.h"
#include "..\..\ui_common\Interface/XJReportInterface.h"

#include "..\..\ui_common\CompareComboBox\CompareComboBox.h"


extern CString g_strOscName;
extern CString g_strStyleName;
extern CString g_StyleConfig;
extern CString g_strToolBarBK;
extern CString g_strStartBmp;
extern BOOL g_bExit;
extern int g_hook;

/** @brief			线程退出时间,超过时间未退出则强杀*/
extern DWORD	g_PTThreadExitTime;

/** @brief			日志表类型.0-旧表，7个字段。 1-新表，>7个字段*/
extern int g_OperTableType;

/** @brief			厂站通断图形名称. XXXX.svg*/
extern CString g_strStationSvg;
//全局变量定义
extern int	g_Temp;
extern int  g_style;
extern BOOL g_bLoginVerify;
extern BOOL g_bConnectCommServer;
extern BOOL g_bVerify;

extern COXSplashWnd g_wndWait;				//等待窗口
extern UINT g_dwESDataFormat;	//register for drag and drop

extern int g_ConnectServerButton;
extern int g_DeviceTreeButton;
extern int g_LogButton;
extern int g_HistoryQueryButton;
extern int g_AlarmCountButton;
extern int g_SystemSettingButton;
extern int g_CallAllButton;
extern int g_CommonFileButton;
extern int g_ExitButton;

//提醒事件
extern int g_RemindMaxCount; //最大显示数
extern int g_RemindTime; //每条事件的保留时间(分钟)


/** @brief			暂停所有定时器和线程*/
extern BOOL g_bSuspend;

extern BOOL	g_Exiting; //正在退出程序

//系统
extern  int         g_bSend20162;
extern  int         g_role;             //运行与监视角色 0:主站 1:子站
extern  int         g_FindDeviceWay;    //点击SVG图二次设备时查找设备方式 0:103组号 1:uri
extern  int			g_ShowOutputWnd;	//是否显示输出窗口
extern	int			g_ShowDeviceTreeWnd;//是否显示设备树窗口
extern	int			g_DeviceTreeType;	//设备树类型，0：只到厂站。1：站-电压等级-设备。2：站-电压等级-间隔-设备. 3:站-电压等级-设备(不删除上一级空节点). 4:站-二次设备
extern	int			g_PutOutToLogFile;	//是否输出到日志文件 0为不输出, >=1为输出
extern	int			g_OutPutToWnd;		//是否输出到日志窗口	0为不输出, >0为输出
extern	int			g_LogLevel;			//输出日志的等级, 1为重要的日志才输出; 2为普通日志输出; 3为所有日志都输出(少用);
extern  int			g_LoadSttpDll;      //调用的动态库选择 0为调用SttpNet.Dll; 1为调用SttpProxy.Dll
extern	CString		g_LocalPath;		//程序所在目录
extern	CString		g_UIConfig;			//UI配置文件的路径
extern	CString		g_LogPath;			//log文件位置
extern	CString		g_SysConfig;		//程序配置文件位置
extern  CString		g_SoundFile;		//声音文件位置
extern	CString		g_ImageFile;		//地理图背景文件位置
extern	CString		g_DefaultStation;	//程序启动时默认打开的厂站
extern	int			g_TabPosition;		//tab栏位置. 0-下方, 1-上方
extern	int			g_ListItemHeight;	//列表行高
extern	int			g_AutoCallMaxSaveNo; //自动总召最大批次号
extern	int			g_DisconnectTime;	//通讯断开多长时间再重连上要重新读库
extern  COLORREF	g_ListSpaceColor;	//列表间隔颜色
extern  COLORREF    g_ListSpaceColor2;
extern  COLORREF    g_ListSpaceColor3;
extern  COLORREF    g_ListSpaceColor4;
extern  COLORREF    g_ListHeadColor;
extern  COLORREF    g_TreeBkColor;
extern  COLORREF    g_TreeTextColor;
extern  COLORREF    g_TitleLightColor;
extern  COLORREF    g_TitleDeepColor;
extern	int			g_UseColorFaultReport; //是否使用带颜色的故障报告表格。1是，0否
extern	int			g_UseLandscapeMultiStation;//双端故障报告是否使用横向格式.1是，0否
extern	int			g_FaultReportFilter;//故障报告是否过滤特征值94和4
extern	CString			g_WorkStatusFileName;//子站工作状态文件,带后缀
extern  int			g_LoadDataModel;
extern	int			g_ThreadTime;
extern	int			g_SecType;//读取的二次设备的类型.为-1是读取所有类型值>0的设备.大于等于0时只读指定类型的设备
extern	CString			g_sAppName;//程序名
extern	int			g_PostTime;	//报文推送间隔
extern	int			g_UseLocalIp;//是否使用本地配置的IP, 0否1是。为0时在数据库中读取配置,默认为0 
extern	CString		g_LocalIp;//本地配置的IP
extern	int			g_LockTime;//登录锁定时间,分钟
extern	CTime		g_LastFailTime;//最后一次失败的时间
extern	int			g_LoginFailTimes;//登录失败次数
extern	int			g_UILockTime;//界面多久不操作会被锁定.分钟
extern	int			g_UseXJWave;//是否使用XJWave
extern	int			g_MarkDownOscPT;//是否在一次接线图中标记能下载录波文件的保护
extern	int			g_AutoStationCommMap;//是否自动绘制厂站通信状态图形
extern CString		g_Language; //语言.ch_ZN, en_US

//故障报告
extern	int			g_FaultActionTime;//故障报告中计算动作相对时间方法。0为本条动作信号时间减第一条动作的信号时间，1为本条的信号时间减启动时间
extern	int			g_FaultFilterIII;//故障报告是否过滤III类信息.0否，1是
//通讯状态状态对应颜色
extern COLORREF		XJ_STATUS_COMM_ON;		//断讯正常
extern COLORREF		XJ_STATUS_COMM_OFF;		//通讯断开
extern COLORREF		XJ_STATUS_COMM_PART;	//用于厂站通讯状态正常，但是站内有保护断开的情况

//运行状态对应颜色
extern COLORREF		XJ_STATUS_RUN_ON;		//运行
extern COLORREF		XJ_STATUS_RUN_CHECK;	//检修/挂牌
extern COLORREF		XJ_STATUS_RUN_OFF;		//停用
extern COLORREF		XJ_STATUS_RUN_DEBUG;	//调试(对码表)
extern COLORREF		XJ_STATUS_RUN_CODE;		//未接入
extern COLORREF		XJ_STATUS_RUN_MARK;		//定值修改装置挂牌
//事件
extern	int			g_ShowEventI;		//I类事件是否显示. 0-不显示. 1-显示
extern	int			g_AlarmEventI;		//I类事件是否报警. 0-不报警. 1-报警
extern  COLORREF	g_colEventI;		//I类事件颜色

extern	int			g_ShowEventII;		//II类事件是否显示. 0-不显示. 1-显示
extern	int			g_AlarmEventII;		//II类事件是否报警. 0-不报警. 1-报警
extern	COLORREF	g_colEventII;		//II类事件颜色

extern	int			g_ShowEventIII;		//III类事件是否显示. 0-不显示. 1-显示
extern	int			g_AlarmEventIII;	//III类事件是否报警. 0-不报警. 1-报警
extern	COLORREF	g_colEventIII;		//III类事件颜色

extern	COLORREF	g_colNotAcked;		//未确认事件文字颜色
extern	COLORREF	g_colAcked;			//已确认事件文字颜色

extern	int			g_BlinkDevice;		//收到装置的动作或告警时,是否闪烁此装置图形.0-否,1-是
extern	int			g_MustShowDevice;	//收到装置的动作或告警时,是否把所属厂站的图形画面推到最前面.0-否,1-是
extern	int			g_BlinkOwner;		//二次设备闪烁时,是否相关联的一次设备一起闪烁.0-否,1-是
extern  int			g_ShowEventDlg;		//收到装置的动作或告警时, 是否弹出事件属性框. 0-否, 1-是
extern	int			g_UseAlarmSound;	//是否使用声音报警. 0-否,1-是
extern	int			g_SoundTime;		//报警声音持续时间. >=0整数. 单位.秒
extern  int         g_FlashTime;
extern	int			g_BlinkStation;		//收到动作或告警时，是否闪烁厂站图形
extern	int			g_AutoChangeMap;	//要闪烁设备时，如果对应厂站图没在页面中显示，是否自动切换到对应厂站图
//主子站通讯状态窗口
extern	int			g_RefreshStationTime;//刷新厂站持续时间的间隔时间,单位(分钟)
extern	int			g_ShowCommON;		//列表中是否显示通信状态正常的厂站信息.0-不显示. 1-显示
extern	int			g_ShowCommOFF;		//列表中是否显示通信状态正常的厂站信息.0-不显示. 1-显示
extern  int         g_QueryDay;     //厂站通讯状态最大记录数

extern  int			g_DoubleChannel;	//厂站是否使用双通道通信.0-不使用 .1-使用
extern	COLORREF	g_SCOnly0OFF;		//只有0号通道断开时的颜色
extern	COLORREF	g_SCOnly1OFF;		//只有1号通道断开时的颜色
extern	COLORREF	g_SCAllOFF;			//两个通道全断开时的颜色

extern int			g_ShowDevStatusInStation; //是否在厂站通讯图中显示站内装置的通讯情况
extern COLORREF		g_CommMapBK; //主子站通讯状态图形背景色
extern CString		g_CommMapFont;//主子站通讯状体图中厂站名的字体类型
extern int			g_CommMapFontSize;//主子站通讯状体图中厂站名的字体大小
extern COLORREF		g_CommMapFontColor;//主子站通讯状体图中厂站名的字体颜色

extern	int			g_DetailWndHeight;
//接线图窗口
//动作事件窗口
extern  int			g_ShowActionDetail;	//是否显示动作事件详细内容
//告警事件窗口
extern	int			g_ShowAlarmDetail;	//是否显示告警事件详细内容
//故障报告窗口
extern  int			g_FaultShowWave;//是否显示录波图形
//录波窗口
//开关量变位窗口
//通讯状态窗口
extern int			g_RefreshPTCommTime;	//刷新保护通信状态持续时间的间隔时间,单位(分钟)
extern int			g_ShowPTCommON;			//列表中是否显示通信状态正常的装置信息.0-不显示. 1-显示
extern int			g_ShowPTCommOFF;		//列表中是否显示通信状态断开的装置信息.0-不显示. 1-显示

//运行状态窗口
extern int			g_RefreshPTRunTime;		//刷新保护运行状态持续时间的间隔时间,单位(分钟)
extern int			g_ShowPTRunON;			//列表中是否显示运行状态正常的装置信息.0-不显示. 1-显示
extern int			g_ShowPTRunCheck;		//列表中是否显示运行状态检修的装置信息.0-不显示. 1-显示
extern int			g_ShowPTRunOFF;			//列表中是否显示运行状态停运的装置信息.0-不显示. 1-显示
extern int			g_ShowPTRunDebug;		//列表中是否显示运行状态调试的装置信息.0-不显示. 1-显示
extern int			g_ShowPTRunCode;		//列表中是否显示运行状态未接入的装置信息.0-不显示. 1-显示

//厂站信息窗口
extern int			g_RefreshTime;			//刷新保护通讯状态和运行状态持续时间的间隔时间,单位(分钟)

//系统告警告窗口
extern	int			g_ShowSysAlarmDetail; //是否显示系统告警事件详细内容
//批量召唤设置
extern	COLORREF	g_BatchCall;	//批量召唤选择中, "召唤"的文字颜色
extern	COLORREF	g_BatchNotCall;	//批量召唤选择中, "不召唤"的文字颜色
extern  COLORREF	g_BatchNotSupport;//批量召唤选择中, "不支持"的文字颜色
extern	COLORREF	g_BatchSuccess;	//批量召唤结果中, "成功"的文字颜色
extern	COLORREF	g_BatchFail;	//批量召唤结果中, "失败"的文字颜色
extern	COLORREF	g_BatchNULL;	//批量召唤结果中, "此项不召唤"的文字颜色
extern	COLORREF	g_BatchLater;	//批量召唤结果中, "尚未召唤"的文字颜色
extern	CString		g_AutoCall_IP;	//自动总召模块所在的IP地址
extern	int			g_AutoCall_Port;//自动总召模块的监听端口

//保护属性页
extern	int			g_PTSettingValues;	//定值历史值载入项数. 0-10整数
extern	int			g_PTSettingGroup;	//定值是否按组显示. 0-否, 1-是
extern	int			g_PTSettingCompare; //定值与哪项比较, 0-与前一次值比较, 1-与基准值比较
extern	int			g_PTDIValues;		//开关量历史值载入项数. 0-10整数
extern	int			g_PTDIStyle;		//开关量值显示风格.0: 1/0, 1: ON/OFF, 2: 投/退, 3: 合/开, 4:动作/返回.
extern	int			g_PTSoftValues;		//软压板历史值载入项数. 0-10整数
extern	int			g_PTSoftStyle;		//软压板值显示风格: 0: 1/0, 1: ON/OFF, 3: 投/退.
extern	int			g_PTAIValues;		//模拟量是否显示历史值 0:不显示. 1:显示
extern	int			g_PTAIRealtimeValue; //是否显示模拟量实时值，并定时刷新
extern	int			g_PTAIRealtimeRefresh; //模拟量实时值刷新时间(秒)
extern	int			g_PTControl;		//控制功能是否可用 0:不可用. 1:可用
extern	int			g_PTGeneralMore;	//基本属性页面的"显示更多"按钮是否显示.0-否, 1-是
extern	int			g_PTActionNum;		//动作事件显示数
extern	int			g_PTAlarmNum;		//告警事件显示数
extern	int			g_PTShowHistory;	//历史事件页面是否显示。0-否，1-是
extern	int			g_PTNewZoneTable;	//是否使用新的定值区号表.为1时使用tb_pt_zone_def, 为0时使用tb_secdev_zone_base
extern	int			g_PTControlModel;	//控制功能模式。1-普通。2-广东
extern	int			g_PTModifyZone;		//是否支持定值区切换
extern	int			g_PTShowGeneral;	//基本信息页面是否显示。0-否，1-是
extern	int			g_PTShowSetting;	//定值页面是否显示。0-否，1-是
extern	int			g_PTShowDI;			//开关量页面是否显示。0-否，1-是
extern	int			g_PTShowSoftBoard;	//软压板页面是否显示。0-否，1-是
extern	int			g_PTShowAI;			//模拟量页面是否显示。0-否，1-是
extern	int			g_PTShowAction;		//动作页面是否显示。0-否，1-是
extern	int			g_PTShowAlarm;		//告警页面是否显示。0-否，1-是
extern	int			g_PTShowOsc;		//本地录波页面是否显示。0-否，1-是
extern	int			g_PTShowOscList;	//远方录波页面是否显示。0-否，1-是
extern	int			g_PTShowNote;		//备注页面是否显示.0-否，1-是
extern	int			g_PTSettingJudge;	//定值修改时是否判定上下限。0-不判定，1-判定
extern	int			g_PTIndexType;		//显示序号值的类型。0-点号，1-序号（从1开始编）
extern	int			g_PTNotModifyCurrentZone; //是否禁止修改当前定值区的定值。 0-不禁止,1-禁止
extern	int			g_PTOscListCompare;	//召唤上来的录波列表是否与本地保存的录波文件对比。0-不对比,1-对比
extern	int			g_PTSetting160x; //16进制定值显示时是否强制在前面加上0x,0-不强制，根据子站送上了的值进行显示。1-强制加上0x

//录波器属性页
extern	int			g_WRShowGeneral;	//基本信息页面是否显示。0-否，1-是
extern	int			g_WRShowSetting;	//定值页面是否显示。0-否，1-是
extern	int			g_WRShowDI;			//开关量页面是否显示。0-否，1-是
extern	int			g_WRShowAI;			//模拟量页面是否显示。0-否，1-是
extern	int			g_WRShowOsc;		//本地录波页面是否显示。0-否，1-是
extern	int			g_WRShowOscList;	//远方录波页面是否显示。0-否，1-是
extern	int			g_WRShowNote;

//各页面是否显示设置
extern  BOOL		g_bShowActionWnd;		//是否显示动作事件窗口
extern	BOOL		g_bShowAlarmWnd;		//是否显示告警事件窗口
extern	BOOL		g_bShowFlautWnd;		//是否显示故障报告窗口
extern	BOOL		g_bShowDIWnd;			//是否显示开关量窗口
extern	BOOL		g_bShowOSCWnd;			//是否显示录波窗口
extern	BOOL		g_bShowPTCommWnd;		//是否显示保护通讯状态窗口
extern	BOOL		g_bShowPTRunWnd;		//是否显示保护运行状态窗口
extern	BOOL		g_bShowSysAlarmWnd;		//是否显示系统告警窗口
extern	BOOL		g_bShowCrossWnd;		//是否显示预警窗口
extern  BOOL		g_bShowSOEIWnd;			//是否显示I类事件窗口
extern	BOOL		g_bShowSOEIIWnd;		//是否显示II类事件窗口
extern	BOOL		g_bShowSOEIIIWnd;		//是否显示III类事件窗口
extern	BOOL		g_bShowSOEDebugWnd;		//是否显示调试事件窗口
extern	BOOL		g_bShowGeoWnd;			//是否显示地理图窗口
extern	BOOL		g_bShowDeviceMapWnd;	//是否显示一次接线图窗口
extern	BOOL		g_bShowDistanceWnd;		//是否显示小波测距窗口
extern	BOOL		g_bShowStationInfoWnd;	//是否显示厂站信息窗口
extern	BOOL		g_bShowRemindWnd;		//是否显示提醒窗口
extern	BOOL		g_bShowDraft;//是否开启显示施工图功能，开启后在一次接线图窗口中有一个"显示施工图"按钮，点击显示施工图


//各分隔窗口大小记录
extern  int		g_nCommSplitterWnd;	//厂站通断窗口分隔大小
extern	int		g_nActionSplitterWnd;	//动作窗口分隔大小
extern	int		g_nAlarmSplitterWnd;	//告警窗口分隔大小
extern	int		g_nFaultSplitterWnd;	//故障归档窗口分隔大小
extern	int		g_nSysAlarmSplitterWnd;//系统告警窗口分隔大小
extern	int		g_nSOESplitterWnd;		//SOE信息窗口分隔大小
extern	int		g_nCrossSplitterWnd;	//预警窗口分隔大小

//施工图窗口大小
extern int g_nDraftWndWidth;
extern int g_nDraftWndHeight;
//////////////////////////////////////////////////////////////////////////
//系统联动
#define MAX_PATH		260
#define PATH_STWAVE		"..\\STWave\\"	//STWave路径
#define EW_STWAVE		0				//遍历STWave
#define PATH_XJQUERY	"..\\XJQuery\\" //XJQuery路径
#define EW_XJQUERY		1				//遍历XJQUERY
#define PATH_XJALARMQUERY	"..\\XJAlarmQuery\\" //XJAlarmQuery路径
#define EW_XJALARMQUERY	2				//遍历XJALARMQUERY
#define PATH_XJWAVE		"..\\XJWave\\"	//XJWave路径
#define PATH_XJFAULTINVERSION	"..\\XJFaultInversion\\"

extern char g_arrStrTable[2][90];
extern BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
//向STWave发送消息WM_OPEN_WAVEFILE，通知其打开剪贴板中指定的文件
#define WM_OPEN_WAVEFILE		WM_USER + 1000
//向XJQuery发送消息WM_QUERY_EVENT，通知查询参数所指装置的事件信息
#define WM_QUERY_EVENT			WM_USER + 999
//向XJAlarmQuery发送消息WM_QUERY_ALARM，通知查询参数所指装置的事件信息
#define WM_QUERY_ALARM			WM_USER + 5001	
//////////////////////////////////////////////////////////////////////////
//全局函数定义

/*
 *  @brief   	WriteLog	 写日志到日志文件和日志输出窗口 
 *  @param 		[In]a param of Type  CString  日志字符串
 *  @param 		[In]a param of Type  int  日志等级
 *  @return 	void 
 */
void WriteLog(CString sLog, int nLV = 3);

void WriteLogTemp(CString sLog, int nLV = 3);

/*
 *  @brief   	WriteLogEx	 写日志到日志文件 
 *  @param 		[In]a param of Type  CString  日志字符串
 *  @param 		[In]a param of Type  int  日志等级
 *  @return 	void 
 */
void WriteLogEx(CString sLog, int nLV = 3);

/*
 *  @brief   	StringToTime	 将数据库中的19位时间字符串转换为CTime对象 
 *  @param 		[In]a param of Type  CString &  时间字符串
 *  @return 	CTime 时间对象
 */
CTime	StringToTime(CString & sTime);

/*
 *  @brief   	StringToTimeSttp15	 将STTP报文中15位的时间转换成CTime对象 
 *  @param 		[In]a param of Type  CString &  时间字符串
 *  @param 		[Out]a param of Type  int&  毫秒值
 *  @return 	CTime 时间对象
 */
CTime	StringToTimeSttp15(CString & sTime, int& nms);

/*
 *  @brief   	StringToTimeSttp12	 将STTP报文中12位的时间转换成CTime对象 
 *  @param 		[In]a param of Type  CString &  时间字符串
 *  @return 	CTime 时间对象
 */
CTime	StringToTimeSttp12(CString & sTime);

/*
 *  @brief   	StringToTimeSttp26	 将STTP报文中26位的时间转换成CTime对象 
 *  @param 		[In]a param of Type  CString &  时间字符串
 *  @param 		[Out]a param of Type  int&  毫秒值
 *  @return 	CTime 时间对象
 */
CTime	StringToTimeSttp26(CString & sTime, int& nms);

/*
 *  @brief   	XJToMS	 string类型字符串转换为CString字符串 
 *  @param 		[In/Out]a param of Type  string  string字符串
 *  @return 	CString CString字符串
 */
CString XJToMS(string xjStr);

/*
 *  @brief   	MSToXJ	 CString类型字符串转换为string字符串 
 *  @param 		[In]a param of Type  CString  CString字符串
 *  @return 	string string字符串
 */
string MSToXJ(CString str);

/*
 *  @brief   	ComplexExchange	 将指定字符串转换成复数形式 
 *  @param 		[In]a param of Type  CString  要转换的字符串
 *  @return 	CString 结果字符串
 */
CString	ComplexExchange(CString sStr);

/*
 *  @brief   	CompareComplex	 比较两个复数大小
 *  @param 		[In]a param of Type  CString  复数1
 *  @param 		[In]a param of Type  CString  复数2
 *  @return 	int  1>2 返回1, 1=2返回0, 1<2返回-1
 */
int		CompareComplex(CString str1, CString str2);

/*
 *  @brief   	StringToTime14	 将数据库中14个字节的时间转换成CTime对象 
 *  @param 		[In]a param of Type  CString  时间字符串
 *  @return 	CTime CTime对象
 */
CTime	StringToTime14(CString sTime);

/*
 *  @brief   	IsValidKeyString	 判断一条带有关键字的语句是否有效语句 
 *  @param 		[In]a param of Type  CString  关键字语句
 *  @return 	BOOL 有效返回TRUE, 无效返回FALSE
 */
BOOL	IsValidKeyString(CString sKey);

/*
 *  @brief   	FindOutAllKey	 找出一个语句中所有的关键字 
 *  @param 		[In]a param of Type  CString  关键字语句
 *  @param 		[Out]a param of Type  CStringArray&  各关键字
 *  @return 	CString 
 */
CString FindOutAllKey( CString sKey, CStringArray& arrString );

/*
 *  @brief   	TranslateKeyInFaultDetail	 故障报告页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInFaultDetail(CString sKey, CString & sValue);

/*
 *  @brief   	TranslateKeyInPTSetting	 保护定值页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInPTSetting(CString sKey, CString & sValue);

/*
 *  @brief   	TranslateKeyInPTSetting	 保护模拟量页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInPTAnalog(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInPTDigital	 保护开关量页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInPTDigital(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInPTSoftBoard	 保护软压板页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInPTSoftBoard(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInWRSetting	 录波器定值页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInWRSetting(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInWRSetting	 录波器开关量页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInWRDigital(CString sKey, CString &sValue);
/*
 *  @brief   	TranslateKeyInWRSetting	 录波器模拟量页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInWRAnalog(CString sKey, CString &sValue);
/*
 *  @brief   	TranslateKeyInPTAction	 保护动作页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInPTAction(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInPTAlarm	 保护告警页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInPTAlarm(CString sKey, CString &sValue);


/*
 *  @brief   	TranslateKeyInAutoCallResult	 自动总召页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int TranslateKeyInAutoCallResult(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInStationComm	 主子站通信状态页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int	TranslateKeyInStationComm(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInPTComm	 保护通信状态页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int	TranslateKeyInPTComm(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInPTRun	 保护运行状态页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int	TranslateKeyInPTRun(CString sKey, CString &sValue);

/*
 *  @brief   	TranslateKeyInStationInfo	 厂站信息页面解析报表打印关键字 
 *  @param 		[In]a param of Type  CString  关键字
 *  @param 		[Out]a param of Type  CString &  关键字表示的值
 *  @return 	int 
 */
int	TranslateKeyInStationInfo(CString sKey, CString &sValue);

/*
 *  @brief   	UpdateBlinkRect	 刷新闪烁图形区域 
 *  @param 		[In]a param of Type  CRect  指定闪烁图形需要刷新的区域
 *  @return 	int 刷新成功返回1,失败返回0
 */
int UpdateBlinkRect(CRect rcBlink);

/*
 *  @brief   	AckAllEvent	 确认指定表的所有事件 
 *  @param 		[In]a param of Type  int  指定数据表标识
 *	@param		[In]a param of Type	 int  指定事件类型
 *  @return 	BOOL 确认成功返回TRUE, 失败返回FALSE
 */
BOOL AckAllEvent(int nTable, int nEventType = -1);

/*
 *  @brief   	AckPTAllEvent	 确认指定保护的所有事件 
 *  @param 		[In]a param of Type  int  指定要确认的事件表
 *  @param 		[In]a param of Type  CString  指定保护
 *  @return 	BOOL 
 */
BOOL AckPTAllEvent(int nTable, CString sID);


/*
 *  @brief      GetOscFilePath       取得录波文件路径
 *  @param 		[In]a param of Type  CString  录波文件全路径
 *  @return 	void 
 */
void GetOscFilePath(CString& sFullPath);


/*
 *  @brief   	NeedShow	 判断指定类型的事件是否需要显示 
 *  @param 		[In]a param of Type  int  事件类型.0-I类, 1-II类, 2-III类
 *  @return 	BOOL 需要显示返回TRUE, 不需要显示返回FALSE
 */
BOOL NeedShow(int nEventType);

/*
 *  @brief   	NeedAlarm	 判断指定类型的事件是否需要报警 
 *  @param 		[In]a param of Type  int  事件类型.0-I类, 1-II类, 2-III类
 *  @return 	BOOL 需要报警返回TRUE, 不需要报警返回FALSE
 */
BOOL NeedAlarm(int nEventType);

/*
 *  @brief   	GetCommStatusString	 根据状态代码取得通讯状态描述字符串 
 *  @param 		[In]a param of Type  int  状态代码
 *  @return 	CString 通讯状态描述字符串
 */
	CString GetCommStatusString(int nStatus);
	/*
 *  @brief   	GetStationStatusString	 根据状态代码取得厂站通讯状态描述字符串 
 *  @param 		[In]a param of Type  int  状态代码
 *  @return 	CString 通讯状态描述字符串
 */
CString GetStationStatusString( int nStatus );
/*
 *  @brief   	GetRunStatusString	 根据状态代码取得运行状态描述字符串 
 *  @param 		[In]a param of Type  int  状态代码
 *  @return 	CString 运行状态描述字符串 
 */
	CString GetRunStatusString(int nStatus);

/*
 *  @brief   	GetSelectDir	 取得用户选择的文件夹路径 
 *  @return 	CString 用户选择的文件夹路径
 */
	CString GetSelectDir();


/*
 *  @brief   	OpenWaveFile	 打开录波文件 
 *  @param 		[In]a param of Type  CString  录波文件全路径
 *  @return 	void 
 */
void OpenWaveFile(CString sFullPath, HWND hWnd);

/*
 *  @brief   	OpenXJQuery	 打开事件查询与统计模块 
 *  @param 		[In]a param of Type  CString  启动参数
 *  @param 		[In]a param of Type  HWND  打开窗口
 *  @return 	void 
 */
void OpenXJQuery(CString sCmd, HWND hWnd);

/*
 *  @brief   	OpenXJAlarmQuery	 打开告警统计模块 
 *  @param 		[In]a param of Type  CString  启动参数
 *  @param 		[In]a param of Type  HWND  打开窗口
 *  @return 	void 
 */
void OpenXJAlarmQuery(CString sCmd, HWND hWnd);

void OpenXJFaultInversion(CString sFullPath, HWND hWnd);


/*
 *  @brief   	GetSituationDescribe	取得自动巡检中的情况说明描述	 
 *	@param 		[In]a param of Type  int nSituationNo	情况说明编号
 *  @return 	CString	 自动巡检中的情况说明描述
 */
CString GetSituationDescribe( int nSituationNo );

/*
 *  @brief   	GetDetailSituation	 取得自动巡检中的详细情况说明
 *	@param 		[In]a param of Type  CString sSituation 情况说明编号组	
 *  @return 	CString	 自动巡检中的详细情况说明
 */
CString GetDetailSituation( CString sSituation );

/*
 *  @brief   	GetDisposeOpinion	取得自动巡检中的处理意见描述
 *	@param 		[In]a param of Type  int nOpinion	处理意见编号
 *  @return 	CString	 自动巡检中的处理意见描述
 */
CString GetDisposeOpinion( int nOpinion );

CString GetDetailDispose( CString sDispose );

CString GetDurationFormatStr();

CString StringFromID(UINT id);


//文件名称
#define INI_File		"XJBrowser.ini"		//ini,程序配置文件
#define UI_File			"UIConfig.xml"		//xml,界面配置文件(List)
#define LOG_File		"XJBrowser_Log.txt"	//LOG文件
#define SOUND_File		"Virus.wav"			//报警声音文件
#define STATIONCOMM_File "StationComm.svg"	//主子站通讯状态图文件
#define IMAGE_FILE		"Image/Background.bmp" //地理图背景

//打印模板
#define FAULTREPORT_SINGLE_PT	"ReportPrint/FaultReport_Single.xml"	//故障报告打印模板,单保护
#define FAULTREPORT_MULTI_PT	"ReportPrint/FaultReport_Multi.xml"	//故障报告打印模板,多保护
#define FAULTREPORT_MULTI_COLOR	"ReportPrint/FaultReport_Multi_Color.xml"
#define FAULTREPORT_SINGLE_COLOR "ReportPrint/FaultReport_Single_Color.xml"
#define FAULTREPORT_SINGLESTATION_MULTI_PT	"ReportPrint/FaultReport_SingleStation_Multi.xml"	//故障报告打印模板,单端多保护
#define FAULTREPORT_SINGLESTATION_MULTI_COLOR	"ReportPrint/FaultReport_SingleStation_Multi_Color.xml"//单端多保护

#define FAULTREPORT_MULTI_PT_LANDSCAPE	"ReportPrint/FaultReport_Multi_landscape.xml" //故障报告打印模板,双端
#define FAULTREPORT_MULTI_COLOR_LANDSCAPE	"ReportPrint/FaultReport_Multi_Color_landscape.xml" //故障报告打印模板,双端

#define SETTING_NEW1_TP		"ReportPrint/Setting_new1.xml" //定值打印模板, 只打印最新一次值
#define SETTING_NEW2_TP		"ReportPrint/Setting_new2.xml" //定值打印模板, 打印最新二次值
#define SETTING_NEW3_TP		"ReportPrint/Setting_new3.xml" //定值打印模板, 打印最新三次值
#define SETTING_ORDER1_TP	"ReportPrint/Setting_order1.xml" //定值打印模板(有基准值), 只打印最新一次值
#define SETTING_ORDER2_TP	"ReportPrint/Setting_order2.xml" //定值打印模板(有基准值), 只打印最新两次值
#define SETTING_ORDER3_TP	"ReportPrint/Setting_order3.xml" //定值打印模板(有基准值), 只打印最新三次值

#define ANALOG_NEW1_TP		"ReportPrint/Analog_new1.xml" //模拟量打印模板, 只打印最新一次值
#define ANALOG_NEW2_TP		"ReportPrint/Analog_new2.xml" //模拟量打印模板, 打印最新二次值
#define ANALOG_NEW3_TP		"ReportPrint/Analog_new3.xml" //模拟量打印模板, 打印最新三次值

#define DIGITAL_NEW1_TP		"ReportPrint/Digital_new1.xml" //开关量打印模板, 只打印最新一次值
#define DIGITAL_NEW2_TP		"ReportPrint/Digital_new2.xml" //开关量打印模板, 打印最新二次值
#define DIGITAL_NEW3_TP		"ReportPrint/Digital_new3.xml" //开关量打印模板, 打印最新三次值

#define SOFTBOARD_NEW1_TP	"ReportPrint/SoftBoard_new1.xml" //软压板打印模板, 只打印最新一次值
#define SOFTBOARD_NEW2_TP	"ReportPrint/SoftBoard_new2.xml" //软压板打印模板, 打印最新二次值
#define SOFTBOARD_NEW3_TP	"ReportPrint/SoftBoard_new3.xml" //软压板打印模板, 打印最新三次值

#define WRSETTING_NEW1_TP	"ReportPrint/WRSetting_new1.xml" //录波器定值打印模板, 只打印最新一次值
#define WRSETTING_NEW2_TP	"ReportPrint/WRSetting_new2.xml" //录波器定值打印模板, 打印最新二次值
#define WRSETTING_NEW3_TP	"ReportPrint/WRSetting_new3.xml" //录波器定值打印模板, 打印最新三次值

#define PTACTION_LIST		"ReportPrint/PTActionList.xml"	 //保护动作事件列表
#define PTALARM_LIST		"ReportPrint/PTAlarmList.xml"	 //保护告警事件列表

#define AUTORESULT_LIST		"ReportPrint/AutoCallResult.xml"	//自动总召结果打印模板

#define STATIONCOMM_LIST	"ReportPrint/StationStatus.xml"		//主子站通信状态打印模板
#define STATIONCOMM_LIST_DOUBLE "ReportPrint/StationStatusDouble.xml"
#define PTCOMM_LIST			"ReportPrint/PTCommStatus.xml"		//保护通信状态打印模板
#define PTRUN_LIST			"ReportPrint/PTRunStatus.xml"		//保护通信状态打印模板
#define STATIONINFO_LIST	"ReportPrint/StationInfo.xml"		//厂站所有保护信息打印模板

#define ERROR_NULL			"No error"

//自定义结构
//故障特征值 tb_pt_action_character
typedef struct PT_ActionCharacter
{
	int		nID;			//特征值ID
	CString strName;		//特征值名
	CString strCodeName;	//特征值代码名
	CString strValue;		//特征值
	CString strUnits;		//单位
	int     nEventType;     //信号量级别：0 一类事件 1 二类事件 2 三类事件

	CString	strPT_ID;		//保护ID
	int		nCPU;			//保护CPU
	const PT_ActionCharacter& operator = (const PT_ActionCharacter& character)
	{
		nID         = character.nID;
		strName     = character.strName;
		strValue    = character.strValue;
		strCodeName = character.strCodeName;
		strUnits	= character.strUnits;
		strPT_ID	= character.strPT_ID;
		nCPU		= character.nCPU;
		nEventType	= character.nEventType;
		return *this;
	}

	PT_ActionCharacter()
	{
		nID         = -1;
		strName     = "";
		strValue    = "";
		strCodeName = "";
		strUnits	= "";
		strPT_ID	= "";
		nCPU		= -1;
		nEventType	= 2;
	}
}PT_ActionCharacter;

//信号量
//##ModelId=49B87B890263
typedef struct PT_Signal
{
	int	    nID;				//信号序号
	CString strName;		//信号名称
	CString strCodeName;	//信号代码名
	int     nEventType;         //信号量级别：0 一类事件 1 二类事件 2 三类事件  3,通讯告警事件
	CString strTime;			//信号发生时间
	int	    nValue;				//信号值：1-ON,0-OFF
	int		nRelativeTime;		//相对时间
	
	CString strPT_ID;		//保护ID
	CString strPTName;		//保护名称
	CString strPT_Model;	//保护型号
	int		nCPU;			//保护CPU号
	
	CString strStationId;   //保护所在厂站ID
	CString strStation;		//保护所在的厂站名称
	
    const PT_Signal& operator =(const PT_Signal& sign)
	{
		nID          = sign.nID;     
        strName      = sign.strName;
		strCodeName  = sign.strCodeName;
		nEventType   = sign.nEventType;
		strTime      = sign.strTime;
		nValue       = sign.nValue;
		strPT_ID     = sign.strPT_ID;
		strPTName    = sign.strPTName;
		strPT_Model  = sign.strPT_Model;
		nCPU		 = sign.nCPU;
		strStationId = sign.strStationId;
		strStation   = sign.strStation;
		nRelativeTime = sign.nRelativeTime;
		return *this;
	}
}PT_Signal;

//保护CPU TB_SECDEV_CPU_BASE
//##ModelId=49B87B890272
typedef struct PT_CPU
{
	int   code;
	CString   des;
	CString   ver;
	int       from_zone;
	int		  to_zone;
	int       fun;
	
    const PT_CPU& operator =(const PT_CPU& sign)
	{
		code      = sign.code;     
        des       = sign.des;
	//	ver		  = sign.ver;
		from_zone    = sign.from_zone;
		to_zone      = sign.to_zone;
	//	fun			 = sign.fun;
		return *this;
	}
}PT_CPU;

//定值区
typedef struct PT_ZONE
{
	
	int		state; //标识是否当前定值区号,0-否,1-是
	
	CString  PT_ID;//保护ID
	int		cpu; //cpu号
	int		code; //定值区号
	CString zone_name;
	CString code_name;
	int     group;
	int     item;
	int     minValue;
	int     maxValue;
	int     stepValue;
	int     zoneValue;


	const PT_ZONE& operator =(const PT_ZONE& sign)
	{
		code = sign.code;
		state = sign.state;
		cpu = sign.cpu;
		PT_ID = sign.PT_ID;
		zone_name = sign.zone_name;
		code_name = sign.code_name;
		group = sign.group;
		item = sign.item;
		minValue = sign.minValue;
		maxValue = sign.maxValue;
		stepValue = sign.stepValue;
		zoneValue = sign.zoneValue;
		return *this;
	}

}PT_ZONE;

//保护开关量定义
typedef struct PT_DIGITAL
{
	CString	ID;			//开关量ID
	CString CodeName;	//开关量代码
	CString	Name;		//开关量名称
	int     CpuCode;
	CString	Value;		//实际值

	const PT_DIGITAL& operator =(const PT_DIGITAL& digital)
	{
		ID			= digital.ID;
		CodeName	= digital.CodeName;
		Name		= digital.Name;
		CpuCode     = digital.CpuCode;
		Value		= digital.Value;
		return *this;
	}
}PT_DIGITAL;

//保护软压板定义
typedef struct PT_SOFTBOARD{
	CString ID;			//软压板ID
	CString CodeName;	//软压板代码
	CString Name;		//软压板名称
	CString Value;		//实际值

	const PT_SOFTBOARD& operator =(const PT_SOFTBOARD& softboard)
	{
		ID			= softboard.ID;
		CodeName	= softboard.CodeName;
		Name		= softboard.Name;
		Value		= softboard.Value;
		return *this;
	}
}PT_SOFTBOARD;

//保护模拟量定义
typedef struct PT_ANALOG{
	CString ID;			//模拟量ID
	CString CodeName;	//模拟量代码
	CString Name;		//模拟量名称
	CString Value;		//实际值
	CString Unit;		//值单位
	CString Precision;	//值精度

	const PT_ANALOG& operator =(const PT_ANALOG& analog)
	{
		ID			= analog.ID;
		CodeName	= analog.CodeName;
		Name		= analog.Name;
		Value		= analog.Value;
		Unit		= analog.Unit;
		Precision	= analog.Precision;
		return *this;
	}
}PT_ANALOG;

//录波文件定义
typedef struct PT_OSC{
	CString FileName;	//文件名称,带后缀名
	CString	FileType;	//文件类型,即后缀名
	float	FileSize;	//文件大小,单位bytes
	CTime	ModifyTime;	//最后修改时间
	CString	DirPath;	//目录路径 
	int		Code;		//序号

	const PT_OSC& operator =(const PT_OSC& osc)
	{
		FileName	= osc.FileName;
		FileType	= osc.FileType;
		FileSize	= osc.FileSize;
		ModifyTime	= osc.ModifyTime;
		DirPath		= osc.DirPath;
		return *this;
	}

}PT_OSC;

typedef struct THREADNODE_TAG{
	int		nNo;
	HANDLE	hThread;
}THREADNODE, *THREADNODE_PTR;

//定值修改定义
//##ModelId=49B87B8902BF
typedef CTypedPtrArray<CPtrArray, STTP_DATA*> MODIFY_LIST; //修改值链表

//定值定义
//##ModelId=49B87B8902C1
typedef struct PT_SETTING{
	CString ID;			//定值ID
	CString CodeName;	//定值代码
	CString Name;		//定值名称
	int		Group;		//组号
	CString Value;		//实际值
	CString VPickList;	//值说明
	CString Unit;		//值单位
	CString	Precision;	//值精度 格式"2,2"
	int		DataType;   //值类型 0-浮点 1－整型 2 控制字3-字符串
	CString OrderValue; //基准值
	CString minValue;
	CString maxValue;
	CString stepValue;
	int     sValue;
	int		Item;
	int		nRefType;//为0时表示为参数配置，不可修改
	int		ntimeMStoS;//值特殊处理，值为1时将毫秒时间显示为秒时间

	const PT_SETTING& operator =(const PT_SETTING& setting)
	{
		ID			= setting.ID;
		CodeName	= setting.CodeName;
		Name		= setting.Name;
		Group		= setting.Group;
		Value		= setting.Value;
		VPickList	= setting.VPickList;
		Unit		= setting.Unit;
		Precision	= setting.Precision;
		DataType	= setting.DataType;
		OrderValue	= setting.OrderValue;
		minValue    = setting.minValue;
		maxValue    = setting.maxValue;
		stepValue   = setting.stepValue;
		sValue      = setting.sValue;
		Item		= setting.Item;
		nRefType	= setting.nRefType;
		ntimeMStoS	 = setting.ntimeMStoS;
		return *this;
	}
}PT_SETTING;

//103组属性
//##ModelId=49B87B8902CF
typedef struct PT_GROUP{
	int		Group;	//组号
	CString	Name;	//名称
	const PT_GROUP& operator =(const PT_GROUP& gp)
	{
		Group = gp.Group;
		Name = gp.Name;
		return *this;
	}
}PT_GROUP;

//系统告警详细信息
typedef struct SYSALARM{
	CString		sName; //名称
	CString		sOldValue;	//旧值
	CString		sNewValue;	//新值
	const SYSALARM& operator =(const SYSALARM& sa)
	{
		sNewValue = sa.sName;
		sOldValue = sa.sOldValue;
		sNewValue = sa.sNewValue;
		return *this;
	}
}SYSALARM;

//保护备注信息
typedef struct PT_NOTE{
	CString id;
	CString pt_id;
	CString note_name;
	CString note_content;
	CTime	create_time;
	CTime	update_time;
	const PT_NOTE& operator =(const PT_NOTE& note)
	{
		id = note.id;
		pt_id = note.pt_id;
		note_name = note.note_name;
		note_content = note.note_content;
		create_time = note.create_time;
		update_time = note.update_time;
	}
}PT_NOTE;

//List值的第一列与第二列差异
const COLORREF XJ_LIST_ONE_TWO		= RGB(255, 0, 0);	//红色
//第二列与第三列的差异
const COLORREF XJ_LIST_TWO_THREE	= RGB(255, 0, 255); //粉红
//控制类型的值用灰色背景
const COLORREF XJ_LIST_TYPE_CONTROL	= RGB(220, 220, 220);

//程序模式
const int MODE_MAIN = 0; //主站
const int MODE_SUB = 1;//子站
const int MODE_SEP = 2;	//分站

//用整数标识的设备分类
/*const int TYPE_BUS		= 1;		//母线
const int TYPE_LINE		= 2;		//线路
const int TYPE_BREAK	= 3;		//开关
const int TYPE_GEN		= 4;		//发电机
const int TYPE_REACTOR	= 5;		//高压电抗器
const int TYPE_TRANS	= 6;		//变压器
const int TYPE_SWITCH	= 7;		//刀闸
const int TYPE_CAP      = 8;		//电容器
const int TYPE_PMS		= 9;		//保护管理机
const int TYPE_BAY		= 10;		//间隔

const int TYPE_STATION	= 18;		//厂站
const int TYPE_NET		= 19;		//地区

const int TYPE_SEC		= 20;		//二次设备
const int TYPE_PROTECT	= 21;		//保护 
const int TYPE_WAVEREC	= 22;		//录波器*/

//用于设备树中Node的Image的Index常数
const int IMAGEINDEX_BUS		=1;		//母线
const int IMAGEINDEX_LINE		=2;		//线路
const int IMAGEINDEX_GEN		=3;		//发电机
const int IMAGEINDEX_CAP		=14;	//发电机
const int IMAGEINDEX_TRANS		=4;		//变压器
const int IMAGEINDEX_REACTOR	=5;		//高压电抗器
const int IMAGEINDEX_SWITCH		=6;		//刀闸
const int IMAGEINDEX_BREAK		=7;		//开关
const int IMAGEINDEX_STATION	=13;	//厂站8
const int IMAGEINDEX_STATION_BREAK = 8;
const int IMAGEINDEX_PROTECT1	=9;		//保护装置1
const int IMAGEINDEX_PROTECT2	=10;	//保护装置2
const int IMAGEINDEX_WAVEREC	=11;	//录波器
const int IMAGEINDEX_NETAREA    =12;    //地区
const int IMAGEINDEX_PMS        =15;    //保护管理机
const int IMAGEINDEX_VOLTAGE    =16;    //电压等级
const int IMAGEINDEX_SELSEC		=17;	//当前选择的二次设备
const int IMAGEINDEX_DISTANCE	=18;	//测距装置

/*
//用字符串标识的设备分类
#define STRING_BUS		"母线"		//母线
#define STRING_LINE		"线路"		//线路
#define STRING_BREAK	"开关"		//开关
#define STRING_GEN		"发电机"	//发电机
#define STRING_CAP		"电容器"	//电容器
#define STRING_REACTOR	"高压电抗器"//高压电抗器
#define STRING_TRANS	"变压器"	//变压器
#define STRING_SWITCH	"刀闸"		//刀闸
#define STRING_PMS      "保护管理机" //保护管理机
#define STRING_BAY		"间隔"		//间隔
#define STRING_NET		"地区"		//地区
#define STRING_STATION	"厂站"		//厂站
#define	STRING_SEC		"二次设备"	//二次设备
#define STRING_PROTECT	"保护装置"	//保护装置
#define STRING_WAVEREC	"录波器"	//录波器
*/ 

//事件类型定义
int const XJ_EVENT_ACTION	= 0;	//保护动作
int const XJ_EVENT_ALARM	= 1;	//保护告警
int const XJ_EVENT_FAULT	= 2;	//事故报告
int const XJ_EVENT_OSC		= 3;	//录波
int const XJ_EVENT_DI		= 4;	//开关量变位信息
int const XJ_EVENT_PTCOMM	= 5;	//保护通讯状态
int const XJ_EVENT_PTRUN	= 6;	//保护运行状态
int const XJ_EVENT_SYSALARM = 7;	//系统告警
int const XJ_EVENT_COMM		= 8;	//主子站通讯状态
int const XJ_EVENT_DISTANCE	= 9;	//小波测距

//事件信息类型定义
int const XJ_EVENTINFO_ALL		= 0;	//所有
int const XJ_EVENTINFO_NORMAL	= 1;	//正常信息
int const XJ_EVENTINFO_DEBUG	= 2;	//检修信息

//故障报告的事件类型定义
int const XJ_FAULT_EVENT_CHR		= 1; //动作事件特征量
int const XJ_FAULT_EVENT_SING		= 2; //动作事件信号量
int const XJ_FAULT_EVENT_ALARM		= 3; //告警事件
int const XJ_FAULT_EVENT_OSCREPORT	= 4; //录波简报
int const XJ_FAULT_EVENT_OSCLIST	= 5; //录波列表
int const XJ_FAULT_EVENT_WAVEDISTANCE	= 6; //小波测距
int const XJ_FAULT_EVENT_OSCPARSE		= 7; //录波联网
int const XJ_FAULT_EVENT_DICHANGE	= 8; //开关量变位
int const XJ_FAULT_EVENT_DISTANCE	= 9; //故障测距

//视图类定义
int const XJ_VIEW_MAP		= 1; //接线图视图
int const XJ_VIEW_COMM		= 2; //主子站通讯视图
int const XJ_VIEW_ACTION	= 3; //动作事件视图
int const XJ_VIEW_ALARM		= 4; //告警事件视图
int const XJ_VIEW_FAULT		= 5; //事故报告事件视图
int const XJ_VIEW_OSC		= 6; //录波事件视图
int const XJ_VIEW_DI		= 7; //开关量事件视图
int const XJ_VIEW_PTCOMM	= 8; //保护通讯状态视图
int const XJ_VIEW_PTRUN		= 9; //保护运行状态视图
int const XJ_VIEW_SYSALARM	= 10;//系统告警视图
int const XJ_VIEW_PTPRO		= 11;//保护属性视图
int const XJ_VIEW_WRPRO		= 12;//录波器属性视图
int const XJ_VIEW_DEVPRO	= 13;//其它一次设备属性视图
int const XJ_VIEW_STAINFO   = 14;//厂站信息视图

//日志等级定义
int const XJ_LOG_LV1		= 1;	//1级日志, 重要, 必输出
int const XJ_LOG_LV2		= 2;	//2级日志, 一般
int const XJ_LOG_LV3		= 3;	//3级日志, 不重要, 调试用

//厂站初始化类型
int const STATIONINIT_CHANGE	= 1; //厂站配置改变
int const STATIONINIT_CREATE	= 2; //新建厂站
int const STATIONINIT_DELETE	= 3; //删除厂站

//向主框架发送的消息
int const	WM_NEWDOCUMENTOPENED	= WM_USER + 1001; //新文档消息
int const	WM_DOCUMENTCLOSEED		= WM_USER + 1002; //关闭文档消息
int const	WM_STATUS_INFO			= WM_USER + 1003; //状态条信息改变
int const	WM_PTFRAME_CLOSE		= WM_USER + 1004; //保护属性视图关闭
int const	WM_PTFRAME_OPEN			= WM_USER + 1005; //保护属性视图打开
int const	WM_WRFRAME_CLOSE		= WM_USER + 1006; //录波器属性视图关闭
int const	WM_WRFRAME_OPEN			= WM_USER + 1007; //录波器属性视图打开
int const	WM_EVENTPROP_CLOSE		= WM_USER + 1008; //事件属性对话框关闭
int const	WM_EVENTPROP_ACK		= WM_USER + 1009; //事件属性对话框确认事件栏事件
int const	WM_EVENTLIST_ACK		= WM_USER + 1010; //事件列表确认事件
int const	WM_EVENTPT_ACK			= WM_USER + 1011; //保护事件列表确认事件
int const	WM_EVENTPROP_ACK_PT		= WM_USER + 1012; //事件属性对话框确认保护属性窗口打开的事件
int const	WM_EVENTLIST_PASSIVE_ACK= WM_USER + 1013; //事件栏被动确认
int const	WM_EVENTPT_PASSIVE_ACK	= WM_USER + 1014; //保护属性页面事件列表被动确认
int const	WM_WINDOW_REFRESH		= WM_USER + 1015; //通知窗口刷新
int const	WM_MDITAB_REFRESH		= WM_USER + 1016; //通知TAB栏刷新
int const	WM_ALL_CLOSE			= WM_USER + 1017; //主程序关闭消息
int const	FAULTREPORT_SEL_CHANGED = WM_USER + 1018; //故障报告列表选择改变消息
int const	FAULTREPORT_PRINT		= WM_USER + 1019; //故障报告打印通知
int const	BATCH_CHECK				= WM_USER + 1020; //批量召唤, 对某设备进行了选择或取消选择操作
int const	BATCH_OPTION			= WM_USER + 1021; //批量召唤, 召唤选项更改
int const	BATCH_UNSELECT_ALL		= WM_USER + 1022; //批量召唤, 取消所有选择
int const	BATCH_SELECT_ALL_PT		= WM_USER + 1023; //批量召唤, 选择所有保护
int const	BATCH_SELECT_ALL_WR		= WM_USER + 1024; //批量召唤, 选择所有录波器
int const	BATCH_FRAME_CLOSE		= WM_USER + 1025; //批量召唤, 关闭窗口
int const	BATCH_FRAME_OPEN		= WM_USER + 1026; //批量召唤, 打开窗口
int const	BATCH_DETAIL_UNCHECK	= WM_USER + 1027; //批量召唤, 从详细视图中取消了某设备选择
int const	BATCH_ENABLE_CONTROLS	= WM_USER + 1028; //批量召唤, 详细视图通知设备树视图让控件禁用/可用
int const	PTOSCLIST_CALLFILE_END	= WM_USER + 1029; //召唤录波文件完成
int const	FAULTREPORT_EDITED		= WM_USER + 1030; //故障报告信息变更通知
int const	FAULTDETAIL_REFRESH		= WM_USER + 1031; //通知故障报告详细视图刷新
int const	AUTOCALL_RESULT_OPEN	= WM_USER + 1032; //自动总召结果窗口打开消息
int const	AUTOCALL_RESULT_CLOSE	= WM_USER + 1033; //自动总召结果窗口关闭消息
int const	AUTOCALL_SET_CHANGE		= WM_USER + 1034; //自动总召设置改变消息
int const	REFRESH_ACTIONVIEW		= WM_USER + 1035; //刷新动作事件显示
int const	REFRESH_ALARMVIEW		= WM_USER + 1036; //刷新告警事件显示
int const	REFRESH_PTSETTING		= WM_USER + 1037; //刷新保护定值页面
int const	REFRESH_PTDI			= WM_USER + 1038; //刷新保护开关量页面
int const	REFRESH_PTSOFTBOARD		= WM_USER + 1039; //刷新保护软压板页面
int const	PT_STATUS_CHANGED		= WM_USER + 1040; //保护状态改变.
int const	STATION_STATUS_CHANGED	= WM_USER + 1041; //厂站状态改变.
int const	DEVICETREE_HIDE			= WM_USER + 1042; //显示或隐藏设备树, 以0为参数时隐藏, 1为参数时显示
int const	ALARM_SOUND				= WM_USER + 1043; //通知开启报警声音.LPARAM为0时通知开始报警,为1时通知停止报警
int const	DOWNLOAD_OPEN			= WM_USER + 1044; //通用文件下载窗口打开消息
int const	DOWNLOAD_CLOSE			= WM_USER + 1045; //通用文件下载窗口关闭消息
int const	COMMONFILE_DOWNLOAD_END = WM_USER + 1046; //通用文件下载完成通知
int const	AUTOCALL_RESULT_CHANGE	= WM_USER + 1047; //自动总召结果显示,厂站选择改变
int const	STATIONINFO_FRAME_OPEN	= WM_USER + 1048; //厂站信息窗口打开
int const	STATIONINFO_FRAME_CLOSE	= WM_USER + 1049; //厂站信息窗口关闭
int const	AUTOCALL_STATION_CHANGE	= WM_USER + 1050; //自动总召可操作厂站改变
int const	DOWNOSC_FRAME_OPEN		= WM_USER + 1051; //录波下载页面打开
int const	DOWNOSC_FRAME_CLOSE		= WM_USER + 1052; //录波下载页面关闭
int const	COMMVIEW_QUERY_END		= WM_USER + 1053; //手动召唤厂站通讯状态完成
int const	STATION_INFO_CHANGED	= WM_USER + 1054; //厂站下设备通断情况变化
int const   ITEM_SELECT             = WM_USER + 1055; //CListTreeCtrl item选中通知
int const   SHOW_BATCH              = WM_USER + 1056; //显示厂站设备列表 
int const   CHECK_CHANGE            = WM_USER + 1057; //check状态改变
int	const	BATCHCALL_END			= WM_USER + 1060;//批量召唤完成

int const	STATION_CHANGED	= WM_USER + 1058; //厂站下通断情况变化
int const   SHOW_RESULT = WM_USER + 1059; 

int const	FAULTREPORT_EXPORT_EXCEL		= WM_USER + 1061; //故障报告导出到Excel通知

int const	TEST_STATIONSTATUS = WM_USER + 1062; //测试厂站状态通知
int const	ADD_REMIND	= WM_USER + 1063; //增加一条提醒内容
int const	PUSHTOP_FVIEW = WM_USER + 1064; //将指定页面前置
int	const	THREAD_FILL_DATA = WM_USER + 1065; //由线程发出让界面重新填充数据

int const	WM_DISDEVFRAME_CLOSE		= WM_USER + 1066; //测距装置属性视图关闭
int const	WM_DISDEVFRAME_OPEN			= WM_USER + 1067; //测距装置属性视图打开

//页面定义
int const FVIEW_ACTION = 1;
int const FVIEW_ALARM = 2;
int const FVIEW_OSC = 3;
int const FVIEW_FAULT = 4;
//动作事件视图消息,从WM_USER+1100开始
int const	VIEWACTION_SELECTCHANGED	= WM_USER +	1100; //动作视图列表选择改变

//告警事件视图消息,从WM_USER+1120开始
int const	VIEWALARM_SELECTCHANGED	= WM_USER + 1120; //告警视图选择改变

//系统警告事件视图消息,从WM_USER+1130开始
int const	VIEWSYSALARM_SELECTCHANGED = WM_USER + 1130; //系统告警选择改变

//预警视图消息,从WM_USER+1105开始
int const	VIEWCROSS_SELECTCHANGED	= WM_USER +	1105; //预警视图列表选择改变

//所有视图强制刷新
int const	VIEW_REFRESH = WM_USER + 1140;
int const	COMM_STATUS_CHANGE = WM_USER + 1141; //通讯状态改变. LPARAM 0-正常, 1-断开

//分类SOE视图通知
int const	VIEW_SOE_LIST_ACK = WM_USER + 1150;	//从SOE事件列表确认事件
int const	VIEWSOE_SELECTCHANGED = WM_USER + 1153; //SOE事件列表选择改变

//线程通知
int const	THREAD_WORKEND	= WM_USER + 1154; //线程工作完毕通知
int	const	STARTLOCK = WM_USER + 1155;//可以开始计时锁定通知

//厂站初始化通知
int	const	STATIONINIT = WM_USER + 1156;//厂站初始化

int const	EXPORT_PROCESS	= WM_USER + 1157; //导出过程信息
int const	EXPORT_RESULT	= WM_USER + 1158; //导出结果信息

int const	REFRESH_COMMMAP = WM_USER + 1159; //刷新主子站通讯状态图形

int const	SHOWTOOPTIP = WM_USER + 1160; 

//CJ控件
int const	IDC_BUTTON_HIDE		= WM_APP + 9000;
int const	IDC_BUTTON_MINI		= WM_APP + 9001;

//用户功能(对应TB_SYS_FUNC中的Func_ID)
#define FUNC_XJBROWSER_BROWSER	 "运行与监视查看"		//浏览运行与监视的信息
#define FUNC_XJBROWSER_OPERATE   "运行与监视操作"		//监控功能的权力,包括召唤定值,开关量,模拟量,软压板功能
#define FUNC_XJBROWSER_CONTROL	 "控制功能"				//执行控制功能的权力,包括定值修改,定值组切换,软压板投退,保护信号复归,对时,召唤时间.录波器对时,远方触发
#define FUNC_XJBROWSER_SIGNAL    "信号量设置"			//修改装置信号量设置的权力
#define FUNC_XJBROWSER_CUSTODY	 "监护功能"				//监护控制功能执行, ,包括定值修改,定值组切换,软压板投退的执行监护
#define FUNC_XJBROWSER_SETTING	 "运行与监视设置"		//更改运行与监视配置

//具体操作定义
#define FUNC_QUY_CALLSETTING		"召唤定值"
#define FUNC_QUY_CALLTIME			"召唤装置时间"
#define FUNC_QUY_CALLDIGITAL		"召唤开关量"
#define FUNC_QUY_CALLANALOG			"召唤模拟量"
#define FUNC_QUY_CALLHEVENT			"召唤历史事件"
#define FUNC_QUY_CALLZONE			"召唤当前定值区号"
#define FUNC_QUY_CALLSOFT			"召唤软压板"
#define FUNC_QUY_CALLGENERAL		"召唤装置基本信息"
#define FUNC_QUY_CALLOSCLIST		"召唤录波列表"
#define FUNC_QUY_CALLOSC			"召唤录波文件"
#define FUNC_QUY_CALLFILELIST		"召唤通用文件列表"
#define FUNC_QUY_DOWNLOADFILE		"召唤通用文件"
#define FUNC_QUY_STATIONSTATUS		"召唤厂站通讯状态"

#define FUNC_SIGNAL_SETTING			"信号量设置"
#define FUNC_SIGNAL_SYNC			"同步子站信号量设置"

#define FUNC_OPT_SIGNRESET			"信号复归"
#define FUNC_OPT_RUNWAVEREC			"远方启动录波器"
#define FUNC_OPT_SETPTTIME			"保护对时"
#define FUNC_OPT_SETWRTIME			"录波器对时"
#define FUNC_OPT_ACKWRALARM			"录波器告警事件确认"

#define FUNC_OPT_CHANGEZONE			"定值组切换"
#define FUNC_OPT_SETSETTING			"修改定值"
#define FUNC_OPT_SETSOFTBOARD		"软压板投退"

/*
#define FUNC_SYS_LOGOUT				"注销用户"
#define FUNC_SYS_LOGIN				"用户登陆"
#define FUNC_SYS_CONNECT			"连接通信服务器"
#define FUNC_SYS_DISCONNECT			"断开通信服务器"
#define FUNC_SYS_SETTING			"更改系统配置"
*/
#define FUNC_SYS_LOGOUT				""
#define FUNC_SYS_LOGIN				""
#define FUNC_SYS_CONNECT			""
#define FUNC_SYS_DISCONNECT			""
#define FUNC_SYS_SETTING			""

//召唤操作相关
extern CString	CANCEL_CALL;
extern CString CANCEL_NULL;
extern CString STTP_DISCONNECT;

//操作类型
/** @brief			定值区切换预校*/
int const XJ_OPER_CHANGEZONE_PRECORRECTION = 0;
/** @brief			定值区切换执行*/
int const XJ_OPER_CHANGEZONE_EXECUTE = 1;
/** @brief			定值修改预校*/
int const XJ_OPER_MODIFYSETTING_PRECORRECTION = 2;
/** @brief			定值修改执行*/
int const XJ_OPER_MODIFYSETTING_EXECUTE = 3;
/** @brief			软压板投退预校*/
int const XJ_OPER_MODIFYSOFTBOARD_PRECORRECTION = 4;
/** @brief			软压板投退执行*/
int const XJ_OPER_MODIFYSOFTBOARD_EXECUTE = 5;
/** @brief			信号复归*/
int const XJ_OPER_SIGNRESET = 6;
/** @brief			远方起动*/
int const XJ_OPER_RemoteStart = 7;
/** @brief			召唤定值*/
int const XJ_OPER_CALL_SETTING = 8;
/** @brief			召唤装置时间*/
int const XJ_OPER_CALL_DEVTIME = 9;
/** @brief			召唤开关量*/
int const XJ_OPER_CALL_DIGTIAL = 10;
/** @brief			召唤模拟量*/
int const XJ_OPER_CALL_ANALOG = 11;
/** @brief			召唤历史事件*/
int const XJ_OPER_CALL_HISTORY = 12;
/** @brief			召唤当前定值区号*/
int const XJ_OPER_CALL_ZONE = 13;
/** @brief			召唤软压板*/
int const XJ_OPER_CALL_SOFTBOARD = 14;
/** @brief			召唤装置基本信息*/
int const XJ_OPER_CALL_DEVINFO = 15;
/** @brief			召唤录波列表*/
int const XJ_OPER_CALL_OSCLIST = 16;
/** @brief			召唤录波文件*/
int const XJ_OPER_CALL_OSCFILE = 17;
/** @brief			召唤通用文件列表*/
int const XJ_OPER_CALL_COMMFILELIST = 18;
/** @brief			召唤通用文件*/
int const XJ_OPER_CALL_COMMFILE = 19;
/** @brief			召唤厂站通讯状态*/
int const XJ_OPER_CALL_STATIONCOMM = 20;
/** @brief			取消召唤*/
int const XJ_OPER_CALL_CANCEL = 21;
/** @brief			保护对时*/
int const XJ_OPER_CALL_PROTECT_SETTIME = 22;
/** @brief			录波器对时*/
int const XJ_OPER_CALL_WAVEREC_SETTIME = 23;
/** @brief			运行与监视用户登录*/
int const XJ_OPER_LOGIN = 24;
/** @brief			连接通信服务器*/
int const XJ_OPER_CONNECT = 25;
/** @brief			断开通信服务器*/
int const XJ_OPER_DISCONNECT = 26;
/** @brief			更改系统配置*/
int const XJ_OPER_CHANGECONF = 27;

/** @brief			操作成功*/
int const OPER_SUCCESS = 0;
/** @brief			操作失败*/
int const OPER_FAILD = 1;


/** @brief           打开保护属性页面时,如果读取数据失败,重试次数*/
int const PT_LOAD_COUNT		= 3;
/** @brief           打开保护属性页面时,如果读取数据失败,隔多少秒再重试*/
int const PT_LOAD_SLEEP		= 2;

//系统设置各页面索引
int const SYS_SETTING_GENERAL = 0;
int const SYS_SETTING_INFOCASE = 1;
int const SYS_SETTING_STATUS = 2;
int const SYS_SETTING_PTPROP = 3;

//保护属性页中各页面索引
int const PT_PROP_GENERAL	= 0;
int const PT_PROP_SETTING	= 1;
int const PT_PROP_DIGITAL	= 2;
int const PT_PROP_SOFT		= 3;
int const PT_PROP_ANALOG	= 4;
int const PT_PROP_ACTION	= 5;
int const PT_PROP_ALARM		= 6;
int const PT_PROP_HISTORY	= 7;
int const PT_PROP_OSC		= 8;
int const PT_PROP_OSCLIST	= 9;
int const PT_PROP_NOTE		= 10;

//录波器属性页中各页面索引
int const WR_PROP_GENERAL	= 0;
int const WR_PROP_SETTING	= 1;
int const WR_PROP_DIGITAL	= 2;
int const WR_PROP_ANALOG	= 3;
int const WR_PROP_OSC		= 4;
int const WR_PROP_OSCLIST	= 5;
int const WR_PROP_NOTE		= 6;

//通讯服务相关
int const	XJ_CLIENT_BROWSER		= 1;

//STTP报文相关定义
#define INQ 'I'		//消息类型INQ型
#define RES 'R'		//response型的
#define EX  'E'   //扩展型

#define FIX 'F'  //消息长度为固定型
#define VAR 'V'  //变长

//////////////////////////////////////////////////////////////////////////
//消息使用SendMessage发送,以保证在消息处理完之前传递的参数有效
//SendMessage的第二个参数(LPARAM lparam)中放报文数据结构(STTP_FULL_DATA)指针

//响应STTP报文消息
const long WM_STTP_FIRST		=WM_USER +2000;	//开始消息
//召唤回应相关
const long WM_STTP_00901		=WM_USER +901;  //操作成功报文
const long WM_STTP_20008		=WM_USER +2008;	//接收20008报文——保护模拟量采样值下载通知
const long WM_STTP_20012		=WM_USER +2012;	//接收20012报文——开关量下载通知
const long WM_STTP_20016		=WM_USER +2016; //接收20016报文——保护定值下载通知
const long WM_STTP_20026		=WM_USER +2026; //接收20026报文----录波列表下载通知
const long WM_STTP_20043		=WM_USER +2043;	//接收20043报文----录波文件下载完成通知
const long WM_STTP_20048		=WM_USER +2048; //接收20048报文——保护定值组切换通知
const long WM_STTP_20170		=WM_USER +2170; //接收20048报文——保护定值组切换通知
const long WM_STTP_20052		=WM_USER +2052; //接收20052报文——保护定值修改通知
const long WM_STTP_20054		=WM_USER +2054; //接收20054报文——保护定值修改执行通知
const long WM_STTP_20056		=WM_USER +2056; //接收20056报文——保护定值切换执行通知
const long WM_STTP_20172		=WM_USER +2172; //接收20056报文——保护定值切换执行通知
const long WM_STTP_20058		=WM_USER +2058;	//接收20058报文——保护信号复归通知
const long WM_STTP_20060		=WM_USER +2060;	//接收20060报文——保护对时通知 
const long WM_STTP_20069		=WM_USER +2069;	//接收20069报文——控制报文通用返回通知
const long WM_STTP_20089		=WM_USER +2089; //接收20089报文----召唤装置时间通知
const long WM_STTP_20107		=WM_USER +2107;	//接收20107报文——召唤保护当前定值区号通知
const long WM_STTP_20174		=WM_USER +2174;	//接收20174报文——召唤保护当前定值区号通知
const long WM_STTP_20109		=WM_USER +2109;	//接收20109报文——召唤保护软压板通知
const long WM_STTP_20111		=WM_USER +2111;	//接收20111报文——召唤基本信息通知
const long WM_STTP_20114		=WM_USER +2114;	//接收20114报文——软压板投退执行通知
const long WM_STTP_20116		=WM_USER +2116; //接收20116报文----下载历史动作事件通知
const long WM_STTP_20118		=WM_USER +2118;	//接收20118报文——软压板投退通知
const long WM_STTP_20124		=WM_USER +2124;	//接收20124报文——录波器远方触发下载通知
const long WM_STTP_20125		=WM_USER +2125;	//接收20125报文——功能不支持通知
const long WM_STTP_20154		=WM_USER +2154; //接收20154报文----批量召唤回应
const long WM_STTP_20157		=WM_USER +2157; //接收20157报文----超时返回通知
const long WM_STTP_20004        =WM_USER +2004; //接受20004报文----召唤保护通讯状态
const long WM_STTP_20086        =WM_USER +2086; //接受20086报文----召唤录波器通讯状态
//自动上送相关报文
const long WM_STTP_20002		=WM_USER + 2002; //接收20002报文,保护通讯状态上载通知
const long WM_STTP_20006		=WM_USER + 2006; //接收20006报文,保护模拟量采样值上载通知
const long WM_STTP_20010		=WM_USER + 2010; //接收20010报文,保护开关量采样值上载通知
const long WM_STTP_20014		=WM_USER + 2014; //接收20014报文,保护定值上载通知
const long WM_STTP_20018		=WM_USER + 2018; //接收20018报文,保护告警事件上载通知
const long WM_STTP_20022		=WM_USER + 2022; //接收20022报文,保护动作事件上载通知
const long WM_STTP_20079		=WM_USER + 2079; //接收20079报文,装置运行状态上载通知
const long WM_STTP_20084		=WM_USER + 2084; //接收20084报文,录波器通讯状态上载通知
const long WM_STTP_20132		=WM_USER + 2132; //接收20132报文,录波文件自动上送通知
const long WM_STTP_20138		=WM_USER + 2138; //接收20138报文,子站配置变化通知
const long WM_STTP_20139		=WM_USER + 2139; //接收20139报文,定值区号变化上载通知
const long WM_STTP_20141		=WM_USER + 2141; //接收20141报文,软压板变化上载通知
const long WM_STTP_20151		=WM_USER + 2151; //接收20151报文,自动归档信息包变化通知
const long WM_STTP_20144		=WM_USER + 2144; //接收20144报文,厂站通讯状态变化上载通知
const long WM_STTP_20155		=WM_USER + 2155; //接收20155报文,周期召唤信息变化告警通知
const long WM_STTP_20159		=WM_USER + 2159; //接收20159报文,预警结构通知
const long WM_STTP_20146		=WM_USER + 2146; //接收20146报文,厂站通讯状态下载通知
const long WM_STTP_20176		=WM_USER + 2176; //接收20176报文,系统告警
const long WM_STTP_20168		=WM_USER + 2168; //接收20168报文,小波测距通知
const long WM_STTP_20177		=WM_USER + 2177; //接收20177报文,站点本地重启要求
//const long WM_GETTIP            =WM_USER + 8888;
//
const long WM_STTP_LAST			=WM_USER +2999;	//结束消息
//通用文件下载
const long WM_STTP_00204		=WM_USER + 30204; //接收00204报文,通用文件信息一览通知
const long WM_STTP_00212		=WM_USER + 30212; //接收00212报文,文件下载成功通知

void AddNoEraseControl(CWnd* pWnd, CRgn & bgRgn, int nID);
void EraseBK(CDC* pDC, CRect bgRect, CRgn & bgRgn, COLORREF colBK);

void GetFirstLetter(CString strName, CString& strFirstLetter);
CString FirstLetter(int nCode);

#ifndef XJHANDLE
#define XJHANDLE HANDLE
#endif

#ifndef XJBROWSER
#define XJBROWSER 
#endif

BOOL InitSVGComNoReg(IClassFactory*& pFactory, ISvgViewer*& pSvgDoc);
BOOL InitReportComNoReg(IClassFactory*& pFactory, IXJReport*& pReportDoc);

CString QueryDIName(CString sPTID, int nDIID, int nCPU);

int GetOperationNum();

/*
 *  @brief   	DelPostfixFromPath	 从路径中去除文件后缀
 *	@param 		[In]a param of Type  CString & str	
 *  @return 	BOOL	 
 */
BOOL DelPostfixFromPath(CString& str);

CSize OpenReportFile(IXJReport*& pDoc, CString filename);

const COLORREF colBG = GetSysColor(COLOR_3DFACE);
const UINT WM_PAINTMYCAPTION = WM_USER+5;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include <afxtempl.h>
#include <afxpriv.h>
#include <..\src\afximpl.h>
#include <Afxcview.h>

#define AFX_IDW_SIZEBAR_LEFT	AFX_IDW_DOCKBAR_LEFT	+ 4
#define AFX_IDW_SIZEBAR_RIGHT	AFX_IDW_DOCKBAR_RIGHT	+ 5
#define AFX_IDW_SIZEBAR_TOP		AFX_IDW_DOCKBAR_TOP		+ 6
#define AFX_IDW_SIZEBAR_BOTTOM	AFX_IDW_DOCKBAR_BOTTOM	+ 7

const DWORD dwSizeBarMap[4][2] =
{
	{ AFX_IDW_SIZEBAR_TOP,      CBRS_TOP    },
	{ AFX_IDW_SIZEBAR_BOTTOM,   CBRS_BOTTOM },
	{ AFX_IDW_SIZEBAR_LEFT,     CBRS_LEFT   },
	{ AFX_IDW_SIZEBAR_RIGHT,    CBRS_RIGHT  },
};
#define safe_delete(p){if(p){delete p;p=NULL;}}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9CE24E2_ABB2_4126_B1EB_E7159EA52EF3__INCLUDED_)
