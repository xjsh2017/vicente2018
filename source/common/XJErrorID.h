/********************************************************************
	created:	2009/02/20
	created:	20:2:2009   10:48
	file base:	XJErrorID
	file ext:	h
	author:		qingch
	
	purpose:	系统内出错信息的统一定义与处理
*********************************************************************/

#ifndef XJErrorID_h__
#define XJErrorID_h__

//////////////////////////////////////////////////////////////////////////
//                   内部错误号定义                                   
//////////////////////////////////////////////////////////////////////////

/**                    自定义错误基准号*/ 
#define  MYDEF_ERROR_START_NO     0xFFFF


/** @brief			   自定义错误结束号*/
#define  MYDEF_ERROR_END_NO		  (MYDEF_ERROR_START_NO + 0x2FFF)


/** @brief	      通用错误编号区基准号*/
#define  COM_ERROR_START_NO	 (MYDEF_ERROR_START_NO + 0x0000)

/** @brief	      通用错误编号区结束号*/
#define  COM_ERROR_END_NO	 (COM_ERROR_START_NO + 0x0FFF)

/** @brief             内存分配失败*/
#define  R_NEW_MEM_FAILED         (COM_ERROR_START_NO+1)

/** @brief             动态库加载失败*/
#define  R_LOAD_LIB_FAILED        (COM_ERROR_START_NO+2)

/** @brief             获得函数地址失败*/
#define  R_GET_FUNADDR_FAILED     (COM_ERROR_START_NO+3)

/** @brief             运行中异常*/
#define  R_RUN_EXCEPTION          (COM_ERROR_START_NO+4)

/** @brief             无效地址或指针*/
#define R_INVALID_PARAMETER       (COM_ERROR_START_NO+5)

/** @brief             启动线程失败*/
#define R_CREATE_THREAD_FAILED    (COM_ERROR_START_NO+6)

/** @brief             结束线程失败*/
#define R_END_THREAD_FAILED       (COM_ERROR_START_NO+7)

/** @brief             登陆验证失败*/
#define R_VERIFY_FAILED           (COM_ERROR_START_NO+8)

/** @brief             报文转换类没有找到转换函数*/
#define STTP_NOT_FIND_FUNC        (COM_ERROR_START_NO+9)

/** @brief             报文转换类报文转换成数据失败*/
#define STTP_TO_DATA_FAILED       (COM_ERROR_START_NO+10)

/** @brief             报文转换类数据转换成报文失败*/
#define DATA_TO_STTP_FAILED       (COM_ERROR_START_NO+11)


/** @brief	      业务流程错误编号基准号*/
#define  BUSINESS_ERROR_START_NO		(MYDEF_ERROR_START_NO + 0x1000)

/** @brief	      业务流程错误编号结束号*/
#define  BUSINESS_ERROR_END_NO			(BUSINESS_ERROR_START_NO + 0x0FFF)

/** @brief             主站与子站通讯中断*/
#define  B_SUB_DISCONTINUE				(BUSINESS_ERROR_START_NO+1)

/** @brief             该子站正在初始化配置*/
#define  B_SUB_INITIALIZING				(BUSINESS_ERROR_START_NO+2)

/** @brief             子站的无效命令*/
#define  B_SUB_INVALID_COMMAND			(BUSINESS_ERROR_START_NO+3)

/** @brief             子站回复失败*/
#define  B_SUB_REPLY_FAILED				(BUSINESS_ERROR_START_NO+4)

/** @brief             子站的无效回复*/
#define  B_SUB_INVALID_REPLY			(BUSINESS_ERROR_START_NO+5)

/** @brief             子站回复超时*/
#define  B_SUB_REPLY_TIMEOUT			(BUSINESS_ERROR_START_NO+6)

/** @brief             子站数据处理失败*/
#define  B_SUB_DATAHANDLE_FAILED		(BUSINESS_ERROR_START_NO+7)

/** @brief             短链接连接异常*/
#define  B_SUB_SC_LINK_ERROR			(BUSINESS_ERROR_START_NO+8)

/** @brief              无效的命令数据*/
#define  B_STTPNET_INVALID_COMMAND      (BUSINESS_ERROR_START_NO+9)

/** @brief              Sttp客户端缓存已满*/
#define  B_STTPNET_BUFFER_OVER       	(BUSINESS_ERROR_START_NO+10)

/** @brief              Sttp客户端与服务端连接中断*/
#define  B_STTPNET_DISCONTINUE      	(BUSINESS_ERROR_START_NO+11)

/** @brief              客户端接收超时*/
#define  B_CLIENT_RECV_TIMEOUT      	(BUSINESS_ERROR_START_NO+12)

/** @brief              发布命令失败*/
#define  B_POST_CMD_FAILED      		(BUSINESS_ERROR_START_NO+13)

/** @brief	      其他错误编号基准号*/
#define  OTHER_ERROR_START_NO	 (MYDEF_ERROR_START_NO + 0x2000)

/** @brief	      其他错误编号结束号*/
#define  OTHER_ERROR_END_NO		 (OTHER_ERROR_START_NO + 0x0FFF)

extern char* CommErrMsg[];

extern char* BusinessErrMsg[];

extern char* OtherErrMsg[];

extern char* getErrorMsg(int pErrorNo);

#endif // XJErrorID_h__