/********************************************************************
	created:	2010/08/26
	created:	26:8:2010   14:27
	filename: 	D:\VICENTE\CODE\COMMON\XJMsgMonitorAPI.h
	file path:	D:\VICENTE\CODE\COMMON
	file base:	XJMsgMonitorAPI
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#pragma   warning   (disable   :   4275) 
#ifndef XJMsgMonitorAPI_h__
#define XJMsgMonitorAPI_h__

#include "define.h"

const int FRAME_DATATYPE_STTP	= 0;
const int FRAME_DATATYPE_LOG	= 1;
const int FRAME_DATATYPE_103	= 2;
const int FRAME_DATATYPE_DETAIL	= 3;
const int FRAME_DATATYPE_OTHRT	= 4;

const int FRAME_LEVEL_ERROR		= 1;
const int FRAME_LEVEL_WARN		= 2;
const int FRAME_LEVEL_TRACE		= 3;

const int FRAME_DIRECTION_SEND	= 0;
const int FRAME_DIRECTION_RECV	= 1;

const int FRAME_COMMUMODE_TCP	= 0;
const int FRAME_COMMUMODE_COM	= 1;
const int FRAME_COMMUMODE_UDP	= 2;
const int FRAME_COMMUMODE_MODEM	= 3;

typedef struct xjframestruct {
	xjframestruct()
		:bData_type(0),bLevel(0),wSource_port(0),wDist_port(0),
		 bWay(0),bCommuMode(0),dwReserve1(0)
	{
		bzero(chModel_name,40);
		bzero(chSource_ip,40);
		bzero(chDist_ip,40);
		bzero(chTime,25);
		bzero(chReserve2,50);
		vMsg.clear();
		vDesc.clear();
	}
	/** @brief	报文中传输内容的类型(0-sttp,1-日志信息,2-103/104,3-报文全内容解释(如装置所有定值结果的全解释),4-其它规约)*/
	BYTE		bData_type;
	/** @brief	级别。1：错误，2：警告（可疑），3：正常跟踪, 其它待定 */
	BYTE		bLevel;  
	/** @brief	模块名*/
	char		chModel_name[40];	
	/** @brief	源IP (如果是串口通信，则填COM1、COM2等)*/
	char		chSource_ip[40];
	/** @brief	源端口(如果是串口通信，则填装置地址，总线方式下有多套装置)*/
	WORD		wSource_port;
	/** @brief	目标IP*/
	char		chDist_ip[40];
	/** @brief	目标端口*/
	WORD		wDist_port;
	/** @brief	相对于模块本身的报文方向， 0-发送 1-接收 其它待定*/
	BYTE		bWay;
	/** @brief	双方通信方式：1：tcp 2：com(串口)  3：udp 4：modem*/
	BYTE		bCommuMode;
	/** @brief	接收、发送报文的时刻；格式为：yyyy-mm-dd HH:MM:SS.3位毫秒*/
	char		chTime[25];
	/** @brief	保留、扩展用*/
	DWORD		dwReserve1;
	/** @brief	保留、扩展用*/
	char		chReserve2[50];
	/** @brief	报文内容*/
	vector<char> vMsg;
	/** @brief	描述内容:用于日志信息或报文解析
				如果是链路层抛出的解释为简要解释，较短，
				如果是应用层抛出的解释为详细解释，较长；
	*/
	vector<char> vDesc;
} XJFRAMESTRUCT;


const int FRAME103_DATATYPE_MESSAGE		= 0;
const int FRAME103_DATATYPE_LINKINFO	= 1;
const int FRAME103_DATATYPE_LINKALARM	= 2;
const int FRAME103_DATATYPE_APPDETAIL	= 3;
const int FRAME103_DATATYPE_APPALARM	= 4;

typedef struct _XJ103FRAMESTRUCT {
	_XJ103FRAMESTRUCT()
		:bData_type(0),wDist_port(0),bWay(0),dwMsgSerialNO(0),
		 bReserve1(0),bReserve2(0),wReserve3(0)
	{
		bzero(chStationID,13);
		bzero(chDist_ip,40);
		bzero(chTime,25);
		bzero(chReserve4,50);
		vMsg.clear();
		vDesc.clear();		
	}
	/** @brief	报文中传输内容的类型 0-原始报文与解析 1-链路过程提示 2-链路过程告警 3-应用信息详解 4-应用过程告警*/
	BYTE		bData_type;
	/** @brief	子站ID*/
	char		chStationID[13];
	/** @brief	目标IP*/
	char		chDist_ip[40];
	/** @brief	目标端口*/
	WORD		wDist_port;
	/** @brief	相对于模块本身的报文方向， 0-发送 1-接收 其它待定*/
	BYTE		bWay;
	/** @brief	接收、发送报文的时刻；格式为：yyyy-mm-dd HH:MM:SS.3位毫秒*/
	char		chTime[25];
	/** @brief	报文编号 用于类型1~4的内容与类型0的报文关联*/
	DWORD		dwMsgSerialNO;
	/** @brief	保留、扩展用*/
	BYTE		bReserve1;
	/** @brief	保留、扩展用*/
	BYTE		bReserve2;
	/** @brief	保留、扩展用*/
	WORD		wReserve3;
	/** @brief	保留、扩展用*/
	char		chReserve4[50];
	/** @brief	报文内容 当类型为0时有内容 其他的时候为空*/
	vector<char> vMsg;
	/** @brief	描述内容:用于报文解析或提示信息*/
	vector<char> vDesc;
} XJ103FRAMESTRUCT;

#endif // XJMsgMonitorAPI_h__