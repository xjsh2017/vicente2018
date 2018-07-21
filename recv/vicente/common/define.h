//
// define.h
//
// This is a define and include file
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//

#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H
#pragma warning(disable : 4786)//屏蔽4786错误

#ifdef OS_HPUNIX
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#endif
#endif

// c_runtime lib
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>
#include <signal.h>
#ifdef OS_AIX
	#ifndef _THREAD_SAFE
	#define _THREAD_SAFE 1
	#endif
#endif
#include <errno.h>
#include <string.h>  
#include <ctype.h>
#include <stddef.h>
#include <stdarg.h>

//standard c++ lib
#include <iostream>
#include <strstream>
#include <string>
#include <iterator>
#include <cassert>
#include <fstream>
#include <list>
#include <deque>
#include <map>
#include <numeric>            
#include <set>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <queue>
#include <complex>
#include <cctype>             
#include <new>
#include <cstdarg>             
#include <utility>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cerrno>
#include <csignal>
#include <ctime>

/*
 *	内部私有头文件包含
 */

#include "XJErrorID.h"
/* ---------------------------------------- */

using namespace std;
/* ------ Added by qingch   2009/02/10 ----- */
#ifdef OS_SUNUNIX
	#include <sys/sockio.h>
	#include <sys/param.h>
	#include <net/if_arp.h> 
        #include <poll.h>
	/* 互斥量差异 */
	#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
	#define SOL_IP IPPROTO_IP
	#define MSG_NOSIGNAL 0
	/* ------ commented by jjl   2011/06/09 ----- */
	//#ifndef __SOSELECT_TIMEOUT
	//#define __SOSELECT_TIMEOUT
        #ifndef __SOPOLL_TIMEOUT
        #define __SOPOLL_TIMEOUT	
   	#endif	
#endif
/* ---------------------------------------- */

#ifdef OS_AIX
    #include	<termio.h>
	
	/* 互斥量差异 */
	#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
	#define SOL_IP IPPROTO_IP	
	#define MSG_NOSIGNAL 0
	#ifndef __XJSOCKLEN_T
	#define __XJSOCKLEN_T	
		//typedef unsigned long            SOCK_LEN;  
		typedef __ulong32_t            SOCK_LEN;//apply for aix inttypes.h define
	#endif	
	#ifndef __SOSELECT_TIMEOUT
	#define __SOSELECT_TIMEOUT	
	#endif	
    #ifndef _BIG_ENDIAN_CPU_
    #define _BIG_ENDIAN_CPU_
    #endif	        
#endif

#ifdef OS_HPUNIX
	//#include	<termio.h>
			
	/* 互斥量差异 */
	#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
	#define SOL_IP IPPROTO_IP	
	#define MSG_NOSIGNAL 0
	#ifndef __XJSOCKLEN_T
	#define __XJSOCKLEN_T	
		typedef int            SOCK_LEN;  
	#endif	
		
	#ifndef __SOSELECT_TIMEOUT
	#define __SOSELECT_TIMEOUT	
	#endif	
	#ifndef _BIG_ENDIAN_CPU_
	#define _BIG_ENDIAN_CPU_
	#endif	        
#endif

#ifdef OS_LINUX 

    //头文件包含
    #include <sys/types.h>  
    #include <netinet/in.h>  
    #include <sys/socket.h>  
    #include <sys/wait.h>  
    #include <arpa/inet.h>       
    #include <unistd.h>         
    #include <sys/select.h>
    #include <sys/time.h>
    #include <sys/ioctl.h> 
    #include <termios.h>
    #include <netdb.h>
	#include <pthread.h>
    #include <fcntl.h>
	#include <sys/ipc.h>
    #include <sys/msg.h>
    #include <sys/stat.h>
	#include <dirent.h>

	#include <semaphore.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <signal.h>
	#include <dlfcn.h>  
	#include <termios.h>    

	// on Linux, socket is just a descriptor number
	typedef int                     SOCKET;
    #define INVALID_SOCKET          -1              //(~0)
    #define SOCKET_ERROR            -1              //(-1) 
    #define SOCKET_SEND_FLAG        MSG_NOSIGNAL

	#ifndef __XJSOCKLEN_T
	#define __XJSOCKLEN_T	
		typedef unsigned int            SOCK_LEN;       //linux下socket的一些api中采用unsigned int
	#endif	

	//BOOL define
	typedef int                     BOOL;
	#define FALSE                   0
    #define TRUE                    1

	//LPVOID define
	typedef void                    *LPVOID;
	//LPTSTR define
	typedef char*					LPTSTR;

	//thread define
	typedef pthread_t               THREAD_ID;      //linux下线程ID为pthread_t (unsigned int)
	typedef pthread_t               THREAD_HANDLE;  //linux下线程创建后返回为int
	typedef void *                  THREAD_FUNC;    //Linux下线程入口函数返回类型
    #define WINAPI                                  //Linux下没有WINAPI(__stdcall) 
    #define INVALID_THREAD          -1              //Linux下线程创建失败返回-1
    #define THREAD_RETURN           NULL            //Linux下线程的返回值    
	
	//process define
	typedef pid_t					HPROCESS;
	
	//mutex
	typedef pthread_mutex_t         MUTEX;          //Linux下线程互坼体类型
	typedef pthread_cond_t			HCOND;

	//semaphore define
	typedef int						HSEM;
	typedef sem_t					HNSEM;
	
	//handle define
	typedef void*					XJHANDLE;

#endif

#ifdef OS_WINDOWS
    #define  sleep(x)                  Sleep(x*1000)  //采用与Linux下相同的形式  
    #include <Winsock2.h>
    #include <process.h>
    #include <conio.h>
	#include <Windows.h>
	#include <io.h>
	#include <sys/stat.h>


	//THREAD define
    typedef  unsigned long             THREAD_ID;      //windows下线程ID为DWORD(unsiged long)
    typedef  HANDLE                    THREAD_HANDLE;  //windows下线程创建后返回为HANDLE (void *)
    typedef  unsigned int              THREAD_FUNC;    //windows下线程入口函数返回类型
	#define INVALID_THREAD             NULL            //CreateThread()失败返回NULL
	#define THREAD_RETURN              0               //线程返回值为DWORD

	//socket define
	typedef int                        SOCK_LEN;       //windows下socket的一些api中采用 int
    #define SOCKET_SEND_FLAG           0

	//lock
	typedef CRITICAL_SECTION			MUTEX;          //windows下的互坼体类型

	typedef struct 
	{
		unsigned int count;
		unsigned int lastop; //0 none,1 signal,2 broadcast
		HANDLE		hEvent;
	}HCOND;

	//semaphore define
	typedef HANDLE					HSEM;
	typedef HANDLE					HNSEM;
	
	//handle define
	typedef HANDLE					XJHANDLE;

#endif

//数据类型定义
typedef unsigned char                   BYTE;
typedef unsigned short                  WORD;
typedef unsigned short                  u_short;
typedef unsigned int                    UINT;
typedef char							sint8;
typedef unsigned char					uint8;
typedef short							sint16;
typedef unsigned short					uint16;
typedef int								sint32;
typedef unsigned int					uint32;
typedef long							slong;
typedef unsigned long					ulong;
typedef unsigned long					DWORD;
typedef float							float32;
typedef double							float64;

//function define
#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))
#define bzero(ptr,n) memset(ptr,0,n)
    
#ifdef _BIG_ENDIAN_CPU_
#define SWAP_16(b)\
{\
    unsigned short&a =  *(unsigned short*)(&(b));\
    a = (a>>8)|(a<<8);\
}
#define SWAP_32(b)\
{\
    unsigned &a = *(unsigned *)(&(b));\
    a = (a>>24)|((a>>8)&0xFF00)|((a<<8)&0xFF0000)|(a<<24);\
}
#else
#define SWAP_16(b)
#define SWAP_32(b)
#endif


//msg define
#define MAXMSGLEN                0x2800                    //SOCKET数据缓冲区最大长度 10240(10K)
#define MAX_SESSION              300                       //最大的会话资源数
#define CREATE_THREAD_NUM        10                        //第一次启动创建个数
#define MAX_LINE_LENGTH          1024                      //文件中一行的最大长度
#define FILE_ERROR_MAX_LEN       255                       //操作文件错误时,错误描述长度
#define FILE_NAME_MAX_LEN        255                       //文件名最大长度

#define MAX_MSGLENGTH            512                       //消息的最大长度
#define MAX_FIELD_NAME_LEN       256                       //表名最大长度
#define MAX_FIELD_VALE_LEN       256                       //字段值最大长度
#define MAX_CONDITION_LEN        512                       //条件最大长度

/****************************net--socket*********************************/

#define LOCAL_ACT_AS_SERVER	     0                         //Socket作为服务器监听
#define LOCAL_ACT_AS_CLIENT	     1                         //socket作为客户端连接
#define LOCAL_ACT_AS_UNKNOW	    -1                         //socket功能为定义

#define ID_OUTPUT_SEND           0                         //发送
#define ID_OUTPUT_READ           1                         //接收

#define RECVTIME                 1                         //接收时间
#define SENDTIME                 2                         //发送时间
#define BLOCKING                 3                         //阻塞方式
#define UNBLOCKING               4                         //非阻塞方式
#define CONNTIME                 5                         //连接超时时间
#define BROADCAST                6						   //广播方式
#define REUSEADDR				 7						   //端口共用

#define ID_NET                   0                         //网络
#define ID_COM                   1                         //串口
#define ID_RAS                   2                         //modem 
#define ID_OTHERS                3                         //其它

#define TIMEOUT                 -2                         //从socket接收数据超时时返回值

#define XJ_STTP_CMD_AUTOCALL_FLAG 2                       //自动巡检标志


/**
 * @brief       数据库类型
 * @author      李鹏
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
/** @brief        MYSQL数据库*/
const int DATABASE_MYSQL          = 1;

/** @brief        ORACLE数据库*/
const int DATABASE_ORACLE         = 2;

/** @brief        SQLSERVER数据库*/
const int DATABASE_SQLSERVER      = 3;

/** @brief        达梦数据库*/
const int DATABASE_DAMENG = 4;

/** @brief        金仓数据库*/
const int DATABASE_KINGBASE = 5;

/**
 * @brief       设备类型
 * @author      李鹏
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
//用整数标识的设备分类
const int TYPE_BUS		= 1;		//母线
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
const int TYPE_SEC		= 20;		//二次设备//其它不可归某一次设备的二次设备
const int TYPE_PROTECT	= 21;		//保护 
const int TYPE_WAVEREC	= 22;		//录波器
const int TYPE_FD_DEV	= 23;		//故障测距装置虚拟类型
const int TYPE_FD_STATION	= 24;	//故障测距站虚拟类型
const int TYPE_STATION_GROUP = 50;  //厂站组标题
const int TYPE_DEVICE_GROUP  = 51;  //装置组标题
const int TYPE_BREAKER_DEVICE = 53; //开关与一次设备关联
const int TYPE_STATION_SVG = 54;    //厂站SVG
const int TYPE_CPU         = 99;    //CPU类型

//挂接在二次设备下
const int TYPE_STATION_STATUS = 1002;//厂站通讯状态
const int TYPE_AChannel    = 1000;
const int TYPE_OrderSetting = 1001;
const int TYPE_ZONE_BASE = 1002;//区号基本信息
const int TYPE_ALARM_DEF = 1003;//告警基本信息
const int TYPE_ACTION_DEF = 1004;//动作基本信息
const int TYPE_SETTING_DEF = 1005;//定值基本信息
const int TYPE_AI_DEF = 1006; //模拟量基本信息
const int TYPE_DI_DEF = 1007; //开关量基本信息
const int TYPE_SOFTBOARD_DEF = 1008; //软压板基本信息
const int TYPE_OSC_AI_DEF = 1009;//保护录波模拟量通道配置信息
const int TYPE_OSC_DI_DEF = 1010;//保护录波开关量通道配置信息
const int TYPE_FRONT_COMMU_GROUP = 1011; //保护通用分类服务配置
const int TYPE_PT_CHARACTER_DEF = 1012; //单个保护特征量
const int TYPE_ZONE_DATA = 1013; //定值最新区号数据
const int TYPE_SETTING_DATA = 1014; //定值最新数据
const int TYPE_PT_DEVICE_GROUP = 1015; //装置组标题
const int TYPE_PT_SETTING_DATA = 1016; //定值最新数据
const int TYPE_PT_ZONE_DATA = 1017; //定值区号数据
const int TYPE_PT_GROUP = 1018; //装置组标题
const int TYPE_UPLOADATUO_CONFIG = 1019; //按信息种类定制配置信息
const int TYPE_DEVMAINSTATION_CONFIG = 1020; //按设备定制信息

//用字符型标识的设备分类
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
#define STRING_DISTANCE "测距装置"	//测距装置

//20149报文中的更新类型(外部要求数据平台更新内存要求)
const int TYPE_STATION_INFO    = 1;//厂站信息
const int TYPE_PROTECT_INFO    = 2;//装置信息
const int TYPE_NORMAL_INFO     = 3;//普通参数配置信息

//更新内存时用
const  int STATUS_NORMAL = 0;
const  int STATUS_UPDATE = 1;
const  int STATUS_WRITE  = 2;
const  int STATUS_READ   = 3;

//net 地址结构
typedef struct _NETADDR
{
	char	IpAddr[40];                                    //IP地址
	u_short	nPort;                                         //端口号
}NETADDR;

//socket缓冲区结构
typedef struct _BUFFER
{
	WORD    len;                                           //报文长度
	char    buf[MAXMSGLEN];
}XJBUFFER;


//系统时间结构
typedef struct _SYSTIME
{
	u_short wYear; 
    u_short wMonth; 
    u_short wDayOfWeek; 
	u_short wDayOfYear;
    u_short wDay; 
    u_short wHour; 
    u_short wMinute; 
    u_short wSecond; 
    u_short wMilliseconds; 
} SYSTIME;

//系统各功能超时设置结构
typedef struct _TIMEOUTS
{
	//召唤
	UINT   call_protect_settingdata;            //召唤保护定值
	UINT   call_wav_settingdata;                //召唤录波器定值
	UINT   call_protect_comm;                   //召唤保护通讯状态
	UINT   call_anai;                           //召唤模拟量
	UINT   call_switch;                         //召唤开关量
	UINT   call_softboard;                      //召唤软压板
	UINT   call_runstatus;                      //召唤运行状态
	UINT   call_wavfile_list;                   //召唤录波文件列表
	UINT   call_waverfile;                      //召唤录波文件
	UINT   call_common_file;                    //召唤通用文件
	UINT   call_common_file_list;               //召唤通用文件列表
	UINT   call_zone;                           //召唤定值区号
	UINT   call_wav_comm;                       //召唤录波器通讯状态
	UINT   call_basic_info;                     //召唤基本信息
	UINT   call_dev_time;                       //召唤装置时间
    UINT   call_history_info;                   //召唤历史事件
    UINT   call_single_config;                  //召唤某台装置的某个信息表配置
	UINT   call_total;                          //总召唤
	UINT   call_channel_status;                 //召唤通道状态

	//控制
	UINT   set_dev_time;                        //对时
	UINT   set_setting_data;                    //修改定值
	UINT   set_zone;                            //定值区切换
	UINT   set_soft_board;                      //压板投退
	UINT   set_signal_reset;                    //信号复归
	UINT   set_wav_remote_touch;                //录波器远方触发
    UINT   set_event_signal;                    //子站信号量远程定制

	//103
	UINT   t0;                                  //连接建立超时
	UINT   t1;                                  //接收测试报文超时
	UINT   t2;                                  //无数据确认超时
	UINT   t3;                                  //长期空闲下发送测试报文超时

}TIMEOUTS;

 
/**
 * @defgroup STTP 报文结构定义
 * @{
 */

/** @brief           STTP报文体最大长度(8192)*/
const UINT MAX_STTP_MSG_LEN = 0x2000; 

/**
 * @brief       STTP报文头结构
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

typedef struct _STTPMSGHDR
{
	BYTE m_byteMsgSubsetId[3] ;              //返回信息标识符RII
	BYTE m_byteMsgID[5] ;                    //报文ID
	BYTE m_byteMsgType ;                     //报文类别
	BYTE m_byteMsgLength[5] ;                //报文长度
	BYTE m_byteMsgLengthType ;               //长度类别
	BYTE m_byteMsgVersion ;                  //结束标志
}STTPMSGHDR;


/**
 * @brief       STTP全报文结构
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

typedef struct _STTPMSG
{
    STTPMSGHDR   MsgHdr;                     //消息头
	char         MsgBody[MAX_STTP_MSG_LEN] ; //消息体
}STTPMSG;

typedef vector<STTPMSG> STTPMSG_QUEUE;

/** @} */ //OVER

//DATAMSG 结果(加入到CSttpDeque队列的结果)
typedef struct _STTP_DEQUE_MSG
{
	STTPMSG        msg;
	UINT           nTimes;              //用于记录该消息在队列中被扫描到的次数
	bool           bTreat;              //是否清楚标志(规定时间内没有取走按费数据处理)
}STTP_DEQUE_MSG;

typedef struct _FILE_STATUS_STRUCT
{
	char cFileName[100];
	time_t stWriteTime;
	int   nFileSize;
}FILE_STATUS_STRUCT;

#ifndef MFC_ADO_USED_DEFINE

//字段结构
typedef struct _Field
{
	_Field()
	{
		FieldType = -1;
		bzero(FieldName, MAX_FIELD_NAME_LEN);
		bzero(FieldValue, MAX_FIELD_VALE_LEN);
	}

	int  FieldType;       //字段类型
	char FieldName[MAX_FIELD_NAME_LEN];  //字段名
	char FieldValue[MAX_FIELD_VALE_LEN]; //字段值
}Field;

//条件结构
typedef struct _Condition
{
	_Condition()
	{
		ConditionType = -1;
		bzero(ConditionContent, MAX_CONDITION_LEN);
	}

	UINT ConditionType; //是否为常规字段 为0为where 为1为order或者group
	char ConditionContent[MAX_CONDITION_LEN]; //条件内容
}Condition;

//SQL语句结构
typedef struct _SQL_DATA
{
	vector <Field> Fieldlist;          //字段链表
	vector <Condition> Conditionlist;  //条件链表
}SQL_DATA;

#endif


/**	\brief	The _MutiSQL_DATA struct 直接执行SQL语句结构体

*/
const int MAX_SQL_LENGTH = 0x1C00; //7K
typedef struct _MutiSQL_DATA
{
	_MutiSQL_DATA()
	{
		Funtype = -1;
		bzero(SQL_BODY_Content, MAX_SQL_LENGTH);
	}

	int  Funtype;//功能类型标识为查询(需要返回结果内容),其它操作只返回执行结果(执行成功失败)
	char SQL_BODY_Content[MAX_SQL_LENGTH];//sql语句内容
}MutiSQL_DATA;

typedef struct _REALDATA_CONDITION
{
	//add constructors for default value
	//2014-10-09 pengl
	_REALDATA_CONDITION()
	{
		IsUse = false;
		station_id = "";
		pt_id = "";
		cpu_code = "";
		breaker_id = "";
		reverse1 = "";
		reverse2 = "";
	}

	/** @brief             是否根据station_id,pt_id,cpu_code等条件查询*/
	bool      IsUse;

	/** @brief             厂站ID*/
	string station_id;

	/** @brief             保护ID*/
	string pt_id;

	/** @brief             CPU号*/
	string cpu_code;

	/** @brief             开关编号*/
	string breaker_id;//查找开关与一次设备关联时用

	/** @brief             备用*/
	string reverse1;

	/** @brief             备用*/
	string reverse2;

}REALDATA_CONDITION;


/**
 * @brief       用于存储大长度数据时用到的字段值结构
 * @author      李鹏
 * @version     ver1.0
 * @date        14/11/2009
 *
 */
//字段结构
typedef struct _BLOB_FIELD
{
	//add constructors for default value
	//2014-10-09 pengl
	_BLOB_FIELD()
	{
		nBlobFieldType = -1;
		strBlobFieldName = "";
		strBlobFieldValue = "";
	}

	int    nBlobFieldType;       //字段类型
	string strBlobFieldName;     //字段名
	string strBlobFieldValue;    //字段值
}BLOB_FIELD;

/**
 * @brief       用于存储大长度数据时用到的条件结构
 * @author      李鹏
 * @version     ver1.0
 * @date        14/11/2009
 *
 */
//条件结构
typedef struct _BLOB_CONDITION
{
	//add constructors for default value
	//2014-10-09 pengl
	_BLOB_CONDITION()
	{
		nBlobConditionType = -1;
		strBlobConditionContent = "";
	}

	UINT   nBlobConditionType;       //是否为常规字段 为0为where 为1为order或者group
	string strBlobConditionContent; //条件内容
}BLOB_CONDITION;

/**
 * @brief       用于存储大长度数据时用到的SQL结构
 * @author      李鹏
 * @version     ver1.0
 * @date        14/11/2009
 *
 */
//SQL语句结构
typedef struct BLOB_SQL_DATA
{
	vector <BLOB_FIELD>     BlobFieldlist;       //字段链表
	vector <BLOB_CONDITION> Blob_Conditionlist;  //条件链表
}BLOB_DATA;

/**	\brief	The _MutiSQL_DATA struct 直接执行SQL语句结构体适用于大长度语句的SQL结构
  * @author      李鹏
  * @version     ver1.0
  * @date        23/11/2009
  *
*/
typedef struct BLOB_MutiSQL_DATA
{
	//add constructors for default value
	//2014-10-09 pengl
	BLOB_MutiSQL_DATA()
	{
		nFuntype = -1;
		strSqlBodyContent = "";
	}

	int     nFuntype;//功能类型标识为查询(需要返回结果内容),其它操作只返回执行结果(执行成功失败)
	string  strSqlBodyContent;//sql语句内容
}BLOB_MUTISQL_DATA;

/*  全局函数声明  */
extern void GetSysTime(SYSTIME & curTime);
extern void CreateSavePath(char * szPath);
extern void GetIniKey(const char *filename, 
						   const char *title,
				           const char *key,
						   char *chResult);
extern void EndThread();
extern void MySleep(unsigned int nmilliseconds);
extern void GetFileStatusInDir(char *dir, vector <FILE_STATUS_STRUCT> *pFileStatusList,int nLevel);
extern int  GetFileStatus(char *filename, FILE_STATUS_STRUCT *pFileStatusStruct);
extern string GetFormatPath(string sourcePath);
extern string GetFileNameFromFullFileName(string sFullFileName);
extern string GetFilePathFromFullFileName(string sFullFileName);
extern bool PingDestination(const string pIp);
extern WORD ReverseWordEndian(WORD pSourse);
extern int ReverseIntEndian(int pSourse);
extern float ReverseFloatEndian(float pSourse);
extern string FormatGatewayList(string pGatewayA, string pGatewayB);
extern void GetGatewayFromList(string pGatewayList, string& pGatewayA, string& pGatewayB);

		/**
		* @brief	GetFileExFileName	取得.分隔的文件扩展名
		* @param 	[In]const string& pSrcFileName	文件名
		* @param 	[In]string& strExtName	返回的扩展名
		* @return	bool	true:成功 false:失败
		* @notes	无
		* @sample	无
		*/
extern 	bool GetFileExFileName(const string& pSrcFileName,string& strExtName);

#ifndef MFC_ADO_USED_DEFINE

extern void AddField(SQL_DATA& sql, const char* pFieldName, int nType, const char* pFieldValue = NULL);
extern void AddCondition(SQL_DATA& sql, const char* pContent, int nType = 0);
extern void Zero_SQL_DATA(SQL_DATA& pSqlData);

#endif

extern void Zero_RealData_Condition(REALDATA_CONDITION& pRealDataCondition);
#endif

