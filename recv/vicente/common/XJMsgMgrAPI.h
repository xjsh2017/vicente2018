/********************************************************************
 *	created:	2008/09/25
 *	created:	2008:9:25   9:45
 *	filename: 	XJMsgMgrAPI.h
 *	file path:	d:\WorkSpace\Vicente\Code\src\XJCommuServer\CommuServer\XJMsgMgr
 *	file base:	XJMsgMgrAPI
 *	file ext:	hHRESULT
 *	author:		rwp
 *	
 *	purpose:	报文中心的接口数据类型定义
*********************************************************************/
#if !defined(AFX_XJMSGMGRAPI_H__1DB0F148_43BE_4896_8B55_440856589BBE__INCLUDED_)
#define AFX_XJMSGMGRAPI_H__1DB0F148_43BE_4896_8B55_440856589BBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MSG_BUFFER_SIZE 0x2000				    //报文缓冲长度
#define MSG_DES_BUFFER_SIZE (4* MSG_BUFFER_SIZE)//报文描述缓冲长度
#define NAME_SIZE 64							//名字缓冲长度
#define MSG_DIRECT_SEND     0					//报文发送方向
#define MSG_DIRECT_RECV     1					//报文接收方向
#define MSG_SAVE_FMT_STR     1					//报文报文保存字符格式
#define MSG_SAVE_FMT_ASC     2					//报文报文保存二进制格式
#define MSGMGR_CONFIG_FILE_NAME "xjmsgmgr.ini"  //配置文件

#define XJ_STTP_PROTOCL     1					//许继STTP规约					
#define XJ_HD103_PROTOCL  100					//华东103规约					
#define XJ_HB103_PROTOCL  101					//华北103规约
#define XJ_DB103_PROTOCL  102					//东北103规约
#define XJ_NF103_PROTOCL  103					//南方103规约
#define XJ_HUB103_PROTOCL 104					//湖北103规约					
#define XJ_LL103_PROTOCL  105					//辽宁103规约
#define XJ_ZJ103_PROTOCL  106					//浙江103规约
#define XJ_BJ103_PROTOCL  107                   //北京103规约
#define XJ_FJ103_PROTOCL  108                   //福建103规约
#define XJ_JS103_PROTOCL  109                   //江苏103规约
#define XJ_NX103_PROTOCL  110                   //宁夏103规约
#define XJ_HN103_PROTOCL  120                   //河南103规约
#define XJ_SH103_PROTOCL  121 					//上海103规约

#define XJ_HD104_PROTOCL  200					//华东104规约					
#define XJ_HB104_PROTOCL  201					//华北104规约
#define XJ_DB104_PROTOCL  202					//东北104规约
#define XJ_NF104_PROTOCL  203					//南方104规约
#define XJ_HUB104_PROTOCL 204					//湖北104规约					
#define XJ_LL104_PROTOCL  205					//辽宁104规约
#define XJ_ZJ104_PROTOCL  206					//浙江104规约
#define XJ_BJ104_PROTOCL  207                   //北京104规约
#define XJ_FJ104_PROTOCL  208                   //福建104规约
#define XJ_JS104_PROTOCL  209                   //江苏104规约
#define XJ_NX104_PROTOCL  210                   //宁夏104规约
#define XJ_HN104_PROTOCL  220                   //河南104规约
#define XJ_SH104_PROTOCL  221 					//上海104规约

#define XJ_HD101_PROTOCL  300					//华东101规约					
#define XJ_SH101_PROTOCL  301					//上海101规约

/**	\brief	The xjmsgframe struct
报文中心的提供给调用者的报文帧格式定义
*/

typedef struct xjmsgframe {
		int		type;      						//规约类型
		int		direct;      					//报文方向。0：接收，1：发送，其它：unkown(非法)
		char	name[NAME_SIZE];				//名称、用于标识同一IP连接到同一个服务器端,同一进程中不同使用者
		char	from_ip[40];  					//发送方IP、用于填入宿主对侧网络IP（本模块调用者IP对侧网络ip）
		int		from_port;						//发送方端口、用于填入宿主对侧网络port（本模块调用者IP对侧网络port）
		char	to_ip[40];      				//接收方IP、用于填入宿主IP（本模块调用者IP地址）
		int		to_port;						//接收方端口、用于填入宿主端口（本模块调用者端口）
		int		reserved;						//保留、扩展用
		int		save_msg_fmt;					//报文保存格式 1、字符。其它: 二进制
		int		buf_len;						//实际报文长度
		int		des_len;						//实际描述长度
		char	buffer[MSG_BUFFER_SIZE];      	//报文内容
		char	descript[MSG_DES_BUFFER_SIZE];	//报文描述、可用于报文解析
} XJMSGFRM;

/**	\brief	The xjmsgmgr_conf struct
报文中心的基本配置结构
*/

typedef struct xjmsgmgr_conf {
	char user[NAME_SIZE];					//使用者模块名称
	char listen_ip[40];						//报文中心侦听IP、为""时将侦听所有网段
	int  listen_port;						//报文中心侦听端口
	char log_path[256];						//日志存放目录
	int  log_level;							//日志级别,0:不记录日志到文件1:错误日志2:警告日志3:跟踪日志
	bool bRecordMsg;						//是否记录报文
	int  reserved;							//保留备用
}XJMSGMGR_CONF;

#endif // !defined(AFX_XJMSGMGRAPI_H__1DB0F148_43BE_4896_8B55_440856589BBE__INCLUDED_)
