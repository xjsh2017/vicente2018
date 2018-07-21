#ifndef XJInitializingLibHeader_h__
#define XJInitializingLibHeader_h__
//屏蔽告警消息
#pragma   warning   (disable   :   4786)
#pragma   warning   (disable   :   4275)

//引用公用头文件
#include "define.h"          //标准库的定义
#include "SttpDefine.h"      //STTPNET定义

//最大文件长度
const int MAX_FILE_LENGTH = 255;

/**
 * @brief       数据库访问基本参数
 * @author      李鹏
 * @version     ver1.0
 * @date        26/08/2010
 *
 * example
 * @code
 *
 *
 *
 * @endcode
 */
typedef struct _XJ_DATABASE_PARAM
{
/** @brief             是否使用数据平台访问数据库*/
	bool bUseDbServer;

/** @brief             数据库类型1:SQL SERVER 2:ORACLE 3:MYSQL*/
	int  iDataBaseType;

/** @brief             数据库监听端口*/
	int  iDataBasePort;

/** @brief             数据库访问IP地址*/
	char chDataBaseIp[MAX_FILE_LENGTH];

/** @brief             数据库实例名*/
	char chDataBaseName[MAX_FILE_LENGTH];

/** @brief             数据库访问用户名*/
	char chUserName[MAX_FILE_LENGTH];

/** @brief             数据库访问密码*/
	char chPassword[MAX_FILE_LENGTH];
}XJ_DATABASE_PARAM;

//数据库类型定义
//SQL SERVER数据库
const int _DATABASE_SQL_SERVER = 1;
//ORACLE数据库
const int _DATABASE_ORACLE     = 2;
//MYSQL数据库
const int _DATABASE_MYSQL      = 3;

//错误类型
//未知错误
const int XJERROR_UNKNOWN_ERROR              =  0;
//连接通讯服务器失败
const int XJERROR_CONNECT_XJCOMMUSERVER_FAIL = -1;
//连接数据平台失败
const int XJERROR_CONNECT_XJDBSERVER_FAIL    = -2;
//成功
const int XJERROR_OPERATION_SUCCESS          =  1;

//连接数据库类型
//直接访问数据库
const int XJACCESS_DATABASE                 = 0;
//通过数据平台访问数据库
const int XJACCESS_DBSERVER                 = 1;

//初始化厂站时用到的信息类型
const int STATION_INIT_ALL     = 0; //整个站的信息
const int STATION_INIT_LINE    = 1; //线路
const int STATION_INIT_SECDEV  = 2; //二次设备
const int STATION_INIT_GROUP   = 3; //组标题
const int STATION_INIT_BREAKER = 4; //开关
const int STATION_INIT_BUS     = 5; //母线
const int STATION_INIT_TRANS   = 6; //变压器
const int STATION_INIT_GEN     = 7; //发电机
const int STATION_INIT_REACTOR = 8; //电抗器
const int STATION_INIT_CAP     = 9; //电容器
const int STATION_INIT_SWITCH  = 10;//刀闸

//初始化厂站的同步方向
const int STATION_DIRECTIOR_DOWN = 0; //主站下装信息到子站
const int STATION_DIRECTIOR_GET  = 1; //主站从子站调用信息

//初始化设备时用到的信息类型
const int DEVICE_INIT_ALL       = 0; //该设备的所有属性
const int DEVICE_INIT_SETTING   = 1; //定值
const int DEVICE_INIT_AI        = 2; //模拟量
const int DEVICE_INIT_SOFTBOARD = 3; //软压板
const int DEVICE_INIT_ZONE      = 4; //定值区号
const int DEVICE_INIT_STAUTS    = 5; //状态量
const int DEVICE_INIT_FAULT     = 6; //故障量
const int DEVICE_INIT_GROUP     = 7; //组标题

//初始化设备的描述类型
const int DEVICE_DES_ALL      = 0; //初始化全部(描述和属性结构)
const int DEVICE_DES_DESCRIBE = 1; //装置描述
const int DEVICE_DES_PROPERTY = 2; //装置属性结构

//初始化默认组号(全部组,不区分组)
const int INIT_GROUP_NUM = 256;

//初始化默认CPU号(全部CPU,不区分CPU)
const int INIT_CPU_NUM   = 99;

//接口定义
/*
 *  @brief   	StartLib	 进行必要的初始化数据工作
 *  @param 		[In]a param of Type  int 模块编号
 *  @param 		[In]a param of Type  int 连接数据库的方式(0：直接访问数据库 1：通过数据平台访问数据库)
 *  @param 		[In]a param of Type  XJ_DATABASE_PARAM& 如果直接访问数据库需要传递连接数据库的参数
 *  @return 	int 1:成功 0:未知错误 -1:连接通讯服务器失败 -2:连接数据库/数据平台失败
 */
typedef	int (*StartLib)(int iModuleID, int iDbConnType, XJ_DATABASE_PARAM& database_param);

/*
 *  @brief   	Close	 关闭连接并释放资源
 *  @return 	int 1：成功 0：失败
 */
typedef	int (*EndLib)();

/*
 *  @brief   	SetLogPath	 设置日志路径
 *  @param 		[In]a param of Type  const char* 新的日志路径
 *  @return 	int 1：成功 0：失败
 */
typedef	int (*SetLibLogPath)(const char* pLogPath);

/*
 *  @brief   	RecvInitializeMsg	 接收通讯服务器返回的初始化过程信息
 *  @param 		[In]a param of Type  STTP_FULL_DATA& 通讯服务器返回的初始化过程信息
 *  @return 	int 1:成功 0:无数据 -1:连接中断
 */
typedef	int (*RecvInitializeMsg)(STTP_FULL_DATA& sttp_retrun_msg);

/*
 *  @brief   	StationInitialize	 单个厂站初始化
 *  @param 		[In]a param of Type  const char* 厂站ID
 *  @param 		[In]a param of Type  int 初始化信息类型
 *  @param 		[In]a param of Type  int 初始化同步方向
 *  @param 		[In]a param of Type  int 组号 默认为所有组
 *  @param 		[In]a param of Type  int CPU号 默认为所有CPU
 *  @return 	int 1：厂站初始化命令发送成功 0：厂站初始化命令发送失败
 */
typedef	int (*StationInitialize)(const char* pStationID, int iInitType, int iInitDirection, int iGroupNum, int iCpuNum);

/*
 *  @brief   	StationInitialize	 单设备初始化
 *  @param 		[In]a param of Type  const char* 设备ID
 *  @param 		[In]a param of Type  int 初始化信息类型
 *  @param 		[In]a param of Type  int 初始化描述类型
 *  @param 		[In]a param of Type  int 组号 默认为所有组
 *  @param 		[In]a param of Type  int CPU号 默认为所有CPU
 *  @return 	int 1：设备初始化命令发送成功 0：设备初始化命令发送失败
 */
typedef	int (*DeviceInitialize)(const char* pDeviceID, int iInitType, int iInitDes, int iGroupNum, int iCpuNum);

/*
 *  @brief   	RemoveStationInfo	 删除厂站及其下的配置
 *  @param 		[In]a param of Type  const char* 厂站ID
 *  @param 		[In]a param of Type  bool 是否删除厂站本身的信息
 *  @return 	int 1：成功 0：失败
 */
typedef int (*RemoveStationInfo)(const char* pStationID, bool bDelStationSelf);

/** @} */ //OVER
#endif // XJInitializingLibHeader_h__

