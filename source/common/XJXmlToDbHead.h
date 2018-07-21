#ifndef _XJXmlToDbHead_H_
#define _XJXmlToDbHead_H_
/** @brief			 XJXmlToDb.dll接口初始化用的数据库参数和日志参数*/
#include "define.h"
typedef struct  
{
	/** @brief			 数据库种类   1 -- sqlserver , 2 -- oracle  3 -- mysql*/
	int  dbtype;

	/** @brief           数据库ip*/
	string ip;		

	/** @brief           数据库名*/
	string db;	
	
	/** @brief			 用户名  */
	string user;		
	
	/** @brief           密码    */
	string password;     
	
	/** @brief           日志级别*/
	int loglevel;		
	
	/** @brief           日志路径*/
	char m_szLogPath[FILE_NAME_MAX_LEN];
	
} db_parameter;
/** @brief		     回调函数参数error信息的类别*/
const int ERRORMES    = -1;
const int WARNINGMES  = 0;
const int SUCCESSMES  = 1;
/** @brief		     编码类型*/
const int UTF8CODE    = 0;
const int GB2312CODE  = 1;
const int UNICODECODE = 2;
const int GBKCODE     = 3;
const int ASCIICODE   = 4;
/** @brief			 回调函数类型声明   参数n 为保留字段默认为0*/
typedef int (* FUNCallError)(const char * error ,XJHANDLE instance=NULL,int n = 0);
/** @brief			 用于将规定格式的xml数据导进数据库*/
typedef int (*XmlToDb)(const char * xml_file, XJHANDLE instance = NULL, FUNCallError f_CallPrint = NULL,int encoding = 0);
/** @brief			 用于初始化接口用   参数f_CallPrint 默认为空  若需要返回导入数据执行情况，则传入*/
typedef int (*InitXmlToDb)(const db_parameter * in_dbparam);
#endif