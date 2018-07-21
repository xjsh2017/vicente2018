/********************************************************************
	created:	2015/07/09
	created:	2015/07/09 16:40
	filename: 	D:\Vicente\Code\common\XJFtpClientManagerAPI.h
	file path:	D:\Vicente\Code\common
	file base:	XJFtpClientManagerAPI
	file ext:	h
	author:		pengl
	purpose:	XJFtpClientManager接口及结构地冠以
*********************************************************************/
#ifndef XJFtpClientManagerAPI_h__
#define XJFtpClientManagerAPI_h__

#include "define.h"

/**
 * @defgroup 文件属性
 * @{
 */
typedef struct _XJ_FTP_DIR_STRUCT{
	char cAttribute[13];
	char cNum[2];
	char cOwner[9];
	char cGroup[9];
	char cSize[9];
	char cMon[4];
	char cDay[3];
	char cYear_Time[6];
	char cName[51];
	char cPath[120];
}XJ_FTP_DIR_STRUCT;

/*
 *  @brief   	PFUN_CONNETFTPSERVER	 连接FTP服务端 
 *  @param 		[In]a param of Type  const char*  FTP服务端IP
 *  @param 		[In]a param of Type  const char*  访问FTP的用户名
 *  @param 		[In]a param of Type  const char*  访问FTP的密码
 *  @return 	XJHANDLE 连接正确后返回的句柄
 */
typedef XJHANDLE (*PFUN_CONNETFTPSERVER)(const char*, const char*, const char*);

/*
 *  @brief   	PFUN_CLOSEFTPCONNECT	 关闭与FTP服务端的链接 
 *  @param 		[In]a param of Type  XJHANDLE  链接句柄
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_CLOSEFTPCONNECT)(XJHANDLE);

/*
 *  @brief   	PFUN_DIRFTPSERVERFILE	 列出ftp服务端指定目录的文件列表 
 *  @param 		[In]a param of Type  XJHANDLE  链接句柄
 *  @param 		[In]a param of Type  const char*  ftp的路径
 *  @param 		[In]a param of Type  vector<XJ_FTP_DIR_STRUCT>&  该路径下的文件列表
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_DIRFTPSERVERFILE)(XJHANDLE, const char*, vector<XJ_FTP_DIR_STRUCT>&);

/*
 *  @brief   	PFUN_GETFTPSERVERFILE	 读取远程指定文件到本地存储 
 *  @param 		[In]a param of Type  XJHANDLE  链接句柄
 *  @param 		[In]a param of Type  const char*  远程文件路径
 *  @param 		[In]a param of Type  const char*  本地存储文件路径
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_GETFTPSERVERFILE)(XJHANDLE, const char*, const char*);

/*
 *  @brief   	PFUN_DELETEFTPSERVERFILE	 删除ftp服务器上的文件 
 *  @param 		[In]a param of Type  XJHANDLE  链接句柄
 *  @param 		[In]a param of Type  const char*  远程文件路径
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_DELETEFTPSERVERFILE)(XJHANDLE, const char*);

/*
 *  @brief   	PFUN_PUTFTPSERVERFILE	 将本地文件传到FTP服务端 
 *  @param 		[In]a param of Type  XJHANDLE  链接句柄
 *  @param 		[In]a param of Type  const char*  本地文件路径名
 *  @param 		[In]a param of Type  const char*  远程文件路径名
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_PUTFTPSERVERFILE)(XJHANDLE, const char*, const char*);

/*
 *  @brief   	PFUN_SETLIBLOGPARAM	 设置日志参数 
 *  @param 		[In]a param of Type  const char*  日志路径
 *  @param 		[In]a param of Type  int  日志级别（-1不设置）
 *  @param 		[In]a param of Type  int  日志保留天数（-1不设置）
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_SETLIBLOGPARAM)(const char*, int /*= -1*/, int /*= -1*/);

/*
 *  @brief   	GetFtpServerPwd	 获取ftp服务端根目录 
 *  @param 		[In]a param of Type  XJHANDLE  链接句柄
 *  @param 		[Out]a param of Type  char*  ftp服务端根目录
 *  @param 		[Out]a param of Type  int  缓冲区长度
 *  @return 	bool true：成功 false：失败
 */
typedef bool (*PFUN_GETFTPSERVERPWD)(XJHANDLE, char*, int);



#endif // XJFtpClientManagerAPI_h__