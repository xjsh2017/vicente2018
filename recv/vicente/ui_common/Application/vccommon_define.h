#ifndef vccommon_define_
#define vccommon_define_

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <AFXTEMPL.H>

#include "..\..\common\define.h"
#include "..\..\common\MessageLog.h"
#include "..\..\common\SttpDefine.h"
#include "..\..\common\MemSet.h"
#include "..\..\common\MemField.h"
#include "..\..\common\XJErrorID.h"

//日志等级定义
int const XJ_LOG_LV1		= 1;	//1级日志, 重要, 必输出
int const XJ_LOG_LV2		= 2;	//2级日志, 一般
int const XJ_LOG_LV3		= 3;	//3级日志, 不重要, 调试用

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

//通知类型
int const XJ_NOTIFY_EVENT	= 9;	//事件通知
int const XJ_NOTIFY_FAULT	= 10;	//故障报告通知
int const XJ_NOTIFY_DECISION = 11;	//辅助决策通知

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

#define MYASSERT(p) \
if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return;\
}

#define MYASSERT_NULL(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return NULL;\
}

#define MYASSERT_CONTINUE(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	continue;\
}

#define MYASSERT_BOOL(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return FALSE;\
}

#define MYASSERT_STRING(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return "";\
}

#define MYASSERT_INT(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return -1;\
}

#define  MYDELETE(p) \
	if (p != NULL)\
{\
	delete p;\
	 p = NULL;\
}

void WriteLog(CString sLog, int nLogLevel = 3);

void WriteLogTemp(CString sLog, int nLogLevel = 3);

/*************************************************************
 函 数 名：StringToTimeSttp15()
 功能概要：将15位的STTP时间字符串转化为CTime
 返 回 值: 转化成的CTime对象
 参    数：param1 时间字符串
		   Parsm2 返回毫秒
**************************************************************/
CTime StringToTimeSttp15(CString & sTime, int& nms);

/*************************************************************
 函 数 名：StringToTimeSttp12()
 功能概要：将12位的STTP时间字符串转化为CTime,12位的时间不带毫秒值
 返 回 值: 转化成的CTime对象
 参    数：param1	时间字符串
**************************************************************/
CTime StringToTimeSttp12(CString & sTime);

/*************************************************************
 函 数 名：StringToTimeSttp26()
 功能概要：将26位的STTP时间字符串转化为CTime,毫秒值占6位
 返 回 值: CTime类型时间
 参    数：param1	时间字符串
		   Param2	毫秒值
**************************************************************/
CTime StringToTimeSttp26(CString & sTime, int& nms);

/*************************************************************
 函 数 名：StringToTime14()
 功能概要：将14数的时间字符串转换成时间对象
 返 回 值: CTime类型时间
 参    数：param1 时间字符串
		   Param2
**************************************************************/
CTime StringToTime14(CString sTime);

/*************************************************************
 函 数 名：XJToMS()
 功能概要：CXJString对象字符串转化为CString字符串
 返 回 值: 转化成CString对象
 参    数：param1	要转化的CXJString对象
**************************************************************/
CString XJToMS(string xjStr);

string WChar2Ansi(LPCWSTR pwszSrc);

/*************************************************************
 函 数 名：MSToXJ()
 功能概要：CString对象字符串转化为CXJString字符串
 返 回 值: 转化成CXJString对象
 参    数：param1	要转化的CString对象
**************************************************************/
string MSToXJ(CString str);

CTime StringToTimeWithMillisecond(CString sTime);

CTime StringToTimeWithoutMillisecond(CString sTime);

/*************************************************************
 函 数 名：StringToTime()
 功能概要：数据库中的时间字符串转化为时间.字符串格式为YYYY-MM-DD HH:MM:SS.MMMM
 返 回 值: 转化成的时间
 参    数：param1	时间字符串
**************************************************************/
CTime StringToTime(CString& sTime);


/*************************************************************
 函 数 名：ComplexExchange()
 功能概要：复数转换, 判断传入的字符串是否表示复数,如果是,转换成复数形式表示
 返 回 值: void
 参    数：param1	需转换的字符串	
		   Param2
**************************************************************/
CString	ComplexExchange(CString sStr);

/*************************************************************
 函 数 名：CompareComplex()
 功能概要：比较两个复数的大小
 返 回 值: 如果1比2大, 返回1. 1=2,返回0. 1<2,返回-1
 参    数：param1	复数1
		   Param2	复数2
**************************************************************/
int	CompareComplex(CString str1, CString str2);


inline BOOL XJASSERT(void* p, CString strFileName, int iLine)
{
	if (p == NULL)
	{
		CString strLog;
		strLog.Format(strFileName + _T(" ") + ":%d" + _T(" ") + _T("指针为空"), iLine);
		WriteLog(strLog, 3);
		return TRUE;
	}
	return FALSE;
}


inline LONGLONG StartCount()
{
//#ifdef _DEBUG	
	LARGE_INTEGER lv;

	// 获取CPU运行到现在的Tick数
	QueryPerformanceCounter( &lv );
	
	// 计算CPU运行到现在的时间
	// 比GetTickCount和timeGetTime更加精确
	return lv.QuadPart;
//#endif
}

inline void EndCount(CString strLog, LONGLONG startTime)
{
//#ifdef _DEBUG	
	LARGE_INTEGER lv;
    
	QueryPerformanceFrequency( &lv );
    // 1秒有多少tick
    LONGLONG Tick = lv.QuadPart;
	// 获取CPU运行到现在的Tick数
	QueryPerformanceCounter( &lv );
	
	// 计算CPU运行到现在的时间
	// 比GetTickCount和timeGetTime更加精确
	LONGLONG endTime = lv.QuadPart - startTime;
	double interval = (double)endTime/(double)Tick;
	strLog.Format(strLog + ":%f", interval);
	WriteLog(strLog, 3);
//#endif
}
#endif
