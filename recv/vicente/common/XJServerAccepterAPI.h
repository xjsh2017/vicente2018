#ifndef XJServerAccepterAPI_h__
#define XJServerAccepterAPI_h__

#include "../../../common/define.h"

typedef	struct {
	char szLog_path[FILE_NAME_MAX_LEN];		//日志存放目录
	int  iLog_level;						//日志级别,0:不记录日志到文件1:错误日志2:警告日志3:跟踪日志
	int  iServerRole;						//服务器角色 0-主站通讯服务器 1-子站通讯服务器	
	bool bRecordMsg;						//是否记录报文
	int  iLogDay;							//日志记录天数
	bool bReserved;							//保留备用
	int  iReserved;							//保留备用
	char reservedStr[256];					//保留备用
}STRUCT_XJSERVERACCEPTER;

/** @brief            服务端管理器启动函数定义*/
typedef int (*PFUNSTARTSERVERACCEPTER) (const STRUCT_XJSERVERACCEPTER&);
/** @brief            服务端管理器停止函数定义*/
typedef int (*PFUNSTOPSERVERACCEPTER) ();

#ifdef OS_LINUX
#define  LIB_SERVERACCEPTER_NAME "libXJServerAccepter.so" 
#else
#define  LIB_SERVERACCEPTER_NAME "XJServerAccepter.dll" 
#endif

#endif // XJServerAccepterAPI_h__