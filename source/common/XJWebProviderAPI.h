/********************************************************************
 *	created:	2008/10/01
 *	created:	2008:10:1   17:04
 *	filename: 	XJWebProviderAPI.h
 *	file path:	D:\WorkSpace\Vicente\Code\src\XJCommuServer\CommuServer\XJWebProvider
 *	file base:	XJWebProviderAPI
 *	file ext:	h
 *	author:		rwp
 *	
 *	purpose:	对外提供的接口参数文件
*********************************************************************/

#if !defined(AFX_XJWEBPROVIDERAPI_H__C20E2843_173D_4607_B56E_88C9C7A22F91__INCLUDED_)
#define AFX_XJWEBPROVIDERAPI_H__C20E2843_173D_4607_B56E_88C9C7A22F91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define NAME_SIZE 64								 //名字缓冲长度
#define WEBPRVDER_CONFIG_FILE_NAME "xjwebprovider.ini"  //配置文件

/**	\brief	The xjmsgmgr_conf struct
WEB数据提供者的基本配置结构
*/
typedef struct xjwebprovider_conf {
	char user[NAME_SIZE];					//使用者模块名称
	char websrv_ip[40];						//WEB服务器的IP
	int  websrv_port;						//WEB服务器端口
	char log_path[256];						//日志存放目录
	int log_level;							//日志级别,0:不记录日志到文件1:错误日志2:警告日志3:跟踪日志
	int log_day;							//日志保存天数
	bool bRecordMsg;						//是否记录报文
 	bool bRecvACK;							//是否接收服务侧确认
	int iRecvTimeOut;						//网络接口收超时(秒)
	int iSendTimeOut;						//网络接口发超时(秒)
	int  ireserved;							//保留备用
 	float  freserved;						//保留备用
}XJWEBPROVIDER_CONF;


#endif // !defined(AFX_XJWEBPROVIDERAPI_H__C20E2843_173D_4607_B56E_88C9C7A22F91__INCLUDED_)
