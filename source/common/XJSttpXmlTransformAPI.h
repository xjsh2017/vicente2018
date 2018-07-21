/********************************************************************
	created:	2015/07/20
	created:	2015/07/20 10:44
	filename: 	D:\Vicente\Code\common\XJSttpXmlTransformAPI.h
	file path:	D:\Vicente\Code\common
	file base:	XJSttpXmlTransformAPI
	file ext:	h
	author:		pengl
	purpose:	
*********************************************************************/
#ifndef XJSttpXmlTransformAPI_h__
#define XJSttpXmlTransformAPI_h__

#include "define.h"
#include "XJSttpNetAPI.h"

/** @brief            mysql数据库*/
const int XJ_STTP_XML_DB_TYPE_MYSQL = 1;
/** @brief            oracle数据库*/
const int XJ_STTP_XML_DB_TYPE_ORACLE = 2;
/** @brief            sqlserver数据库*/
const int XJ_STTP_XML_DB_TYPE_MS_SQL = 3;
/** @brief            达梦数据库*/
const int XJ_STTP_XML_DB_TYPE_DAMENG = 4;
/** @brief            金仓数据库*/
const int XJ_STTP_XML_DB_TYPE_KINGBASE = 5;

/**	\brief	The _XJ_TRANSFORM_DB_PARAM struct
数据库参数
*/
typedef struct _XJ_TRANSFORM_DB_PARAM
{
	/**	\brief 数据库类型 1:mysql 2:oracle 3:sqlserver 4:达梦 5:金仓*/
	int nDbType;
	
	/**	\brief 数据库端口号*/
	int nDbPort;
	
	/**	\brief 数据库主机IP*/
	char chDbMainHost[30];
	
	/**	\brief 数据库备机IP*/
	char chDbSlaveHost[30];
	
	/**	\brief 数据库名*/
	char chDbName[50];
	
	/**	\brief 数据库用户名*/
	char chDbUser[50];
	
	/**	\brief 数据库密码*/
	char chDbPasswd[50];
	
	_XJ_TRANSFORM_DB_PARAM()
	{
		bzero(chDbMainHost, sizeof(chDbMainHost));
		bzero(chDbSlaveHost, sizeof(chDbSlaveHost));
		bzero(chDbName, sizeof(chDbName));
		bzero(chDbUser, sizeof(chDbUser));
		bzero(chDbPasswd, sizeof(chDbPasswd));
		
		nDbType = XJ_STTP_XML_DB_TYPE_ORACLE;
		nDbPort = -1;
		sprintf(chDbMainHost, "%s", "127.0.0.1");
		sprintf(chDbName, "%s", "ipofas");
		sprintf(chDbUser, "%s", "ipofas");
		sprintf(chDbPasswd, "%s", "sagacity");
	}
	
}XJ_TRANSFORM_DB_PARAM;

/**	\brief	The _XJ_CALL_CMD_PARAM_STRU struct
命令中的信息
*/
typedef struct _XJ_CALL_CMD_PARAM_STRU
{
	/**	\brief 计数器*/
	int nRii;
	
	/**	\brief cpu号*/
	int nCpuCode;

	/**	\brief 103组号*/
	int nGroupNum;
	
	/**	\brief 103条目号*/
	int nItemNum;
	
	/**	\brief 开始时间 yymmddHHMMSS*/
	char chStartTime[50];
	
	/**	\brief 开始时间毫秒*/
	char chStartMs[25];
	
	/**	\brief 结束时间 yymmddHHMMSS*/
	char chEndTime[50];
	
	/**	\brief 结束时间毫秒*/
	char chEndMs[25];
	
	/**	\brief 结果收到的时间 yymmddHHMMSS*/
	char chRecvTime[50];
	
	/**	\brief 结果收到的时间毫秒*/
	char chRecvMs[25];

	/**	\brief 下载的文件名*/
	char chOscFileName[MAX_LINE_LENGTH];
	
	_XJ_CALL_CMD_PARAM_STRU()
	{
		nRii = -1;
		nGroupNum = -1;
		nItemNum = -1;
		nCpuCode = 0;
		bzero(chStartTime, sizeof(chStartTime));
		bzero(chStartMs, sizeof(chStartMs));
		bzero(chEndTime, sizeof(chEndTime));
		bzero(chEndMs, sizeof(chEndMs));
		bzero(chRecvTime, sizeof(chRecvTime));
		bzero(chRecvMs, sizeof(chRecvMs));
		bzero(chOscFileName, sizeof(chOscFileName));
	}
}XJ_CALL_CMD_PARAM_STRU;

/*
 *  @brief   	PFUN_INITTRANSFORMLIB	 初始化动态库，启动后调用一次 
 *  @param 		[In]a param of Type  char*  错误信息
 *  @param 		[In]a param of Type  XJ_TRANSFORM_DB_PARAM*  数据库参数，调用者不给出则读取配置文件，默认值为NULL
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_INITTRANSFORMLIB)(char* /*= NULL*/, XJ_TRANSFORM_DB_PARAM* /*= NULL*/);

/*
 *  @brief   	PFUN_ENDTRANSFORMLIB	 退出，释放资源 
 *  @param 		[Out]a param of Type  char*  错误信息
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_ENDTRANSFORMLIB)(char* /*= NULL*/);

/*
 *  @brief   	PFUN_CONVERTMODELINFOTOXML	 转换模型数据到xml文件 
 *  @param 		[In]a param of Type  char*  转换后的xml文件存放路径，请分配255长度
 *  @param 		[In]a param of Type  const char*  厂站ID
 *  @param 		[Out]a param of Type  char*  错误信息
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_CONVERTMODELINFOTOXML)(char*, const char*, char* /*= NULL*/);

/*
 *  @brief   	PFUN_CONVERTSTTPTOXML	 将sttp报文转换为XML文件 
 *  @param 		[Out]a param of Type  char*  转换后的xml文件存放路径，请分配255长度
 *  @param 		[In]a param of Type  vector<STTP_FULL_DATA>&  sttp报文队列，报文队列中的报文为同一批次同一类型报文
 *  @param 		[Out]a param of Type  char*  错误信息
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_CONVERTSTTPTOXML)(char*, XJ_CALL_CMD_PARAM_STRU&, vector<STTP_FULL_DATA>&, char* /*= NULL*/);

/*
 *  @brief   	PFUN_CONVERTXMLTOSTTP	 将xml文件转换为STTP报文 
 *  @param 		[In]a param of Type  const char*  xml文件路径
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  转换后的报文
 *  @param 		[Out]a param of Type  char*  错误信息
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_CONVERTXMLTOSTTP)(const char*, STTP_FULL_DATA& sttpCmd, char* /*= NULL*/);

/*
 *  @brief   	PFUN_SETLOGPARAM	 设置日志参数，重载日志 
 *  @param 		[In]a param of Type  const char*  日志路径
 *  @param 		[In]a param of Type  int  日志级别
 *  @param 		[In]a param of Type  int  日志保留天数
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_SETLOGPARAM)(const char*, int, int /*= 5*/);



#endif // XJSttpXmlTransformAPI_h__