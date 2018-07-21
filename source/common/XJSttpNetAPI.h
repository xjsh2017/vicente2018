/********************************************************************
	created:	2008/10/14
	created:	14:10:2008   14:33
	file base:	XJSttpNetAPI
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef XJSttpNetAPI_h__
#define XJSttpNetAPI_h__

#include "define.h"

/**
 * @defgroup STTP客户端地址信息定义
 * @{
 */

//net 地址结构
typedef struct _STRUCT_NETADDR_INFO
{
	char	IpAddr[40];                                    //IP地址
	u_short	nPort;                                         //端口号
}STRUCT_NETADDR_INFO;

/**
 * @brief       服务端地址信息结构
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
typedef struct _STRUCT_SERVER_INFO
{
	/** @brief        是否有备用地址*/
	bool bHasReservedAddr;
	/** @brief        地址A（主）*/
	STRUCT_NETADDR_INFO netAddrA;
	/** @brief        地址B（备）*/
	STRUCT_NETADDR_INFO netAddrB;
}STRUCT_SERVER_INFO;


/**
 * @brief       目标服务端信息结构
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
typedef struct _STRUCT_DESTINATION
{
	/** @brief        是否有备用服务端*/
	bool bHasReservedServer;
	/** @brief        服务端A（主）*/
	STRUCT_SERVER_INFO serverA;
	/** @brief        服务端B（备）*/
	STRUCT_SERVER_INFO serverB;
}STRUCT_DESTINATION_SERVER;

/** @} */ //OVER






/**
 * @defgroup STTP 数据结构定义
 * @{
 */


/**
 * @brief       STTP单元数据 （定值、模拟量、开关量、软压板、动作、告警等数据的原结构）
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EAD9F00BB
typedef struct _STTP_DATA
{
	_STTP_DATA()
	{
		id = 0;
		nCpu = 0;
		str_value = "";
		bzero(ch_Time, 16);
		str_reserved = "";
		int_reserved = 0;
		flt_reserved = 0;
	}

    //##ModelId=489EADB001A5
	/** @brief        数据编号*/
    int id;
	
    //##ModelId=489EADDD03D8
	/** @brief        cpu号，仅历史事件开关量变位用到*/
    int nCpu;
	
    //##ModelId=489EADB90203
	/** @brief        数据值(字符型)*/
    string str_value;
	
    //##ModelId=489EAE1901B5
	/** @brief        动作/告警时间*/
    char ch_Time[16];
	
    //##ModelId=489EAE2701D4
	/** @brief        备用（字符型）*/
    string str_reserved;
	
    //##ModelId=489EAE5B007D
	/** @brief        备用（整型）*/
    int int_reserved;
	
    //##ModelId=489EAE6A0128
	/** @brief        备用（浮点型）*/
    float flt_reserved;
	
}STTP_DATA;

/**
 * @brief       STTP文件列表数据
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EAEE301E4
typedef struct _STTP_FILE_LIST_DATA
{
	_STTP_FILE_LIST_DATA()
	{
		nFileSize = 0;
		nFileNO = 0;
		bzero(ch_time, 30);
		strFielName = "";
		strFault = "";
	}

    //##ModelId=489EAF0C02BF
	/** @brief        文件大小*/
    int nFileSize;
	
    //##ModelId=489EAF1501A5
	/** @brief         文件编号*/
    int nFileNO;
	
    //##ModelId=489EAF250232
	/** @brief        故障时间*/
    char ch_time[30];
	
    //##ModelId=489EAF3203D8
	/** @brief        文件名*/
    string strFielName;
	
    //##ModelId=489EAF3E009C
	/** @brief        故障概要描述*/
    string strFault;
	
}STTP_FILE_LIST_DATA;

/**
 * @brief       读取数据库配置时单元数据类型（对应每个字段或条件）
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EE407006D
typedef struct _STTP_DBDATA_UNIT
{
	_STTP_DBDATA_UNIT()
	{
		nDataType = 0;
		nDataLength = 0;
		strValue = "";
		strReserved = "";
	}

    //##ModelId=489EE4A4033C
	/** @brief         数据类型*/
    int nDataType;
	
    //##ModelId=489EE4AF032C
	/** @brief         数据宽度*/
    int nDataLength;
	
    //##ModelId=489EE4B701E4
	/** @brief         数据内容*/
    string strValue;

	/** @brief         保留用于存入时间*/ 
    string strReserved;
	
}STTP_DBDATA_UNIT;

/**
 * @brief       读取数据库配置时一条记录的结构
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EE3D2006D
typedef struct _STTP_DBDATA_RECORD
{
	_STTP_DBDATA_RECORD()
	{
		nReserved = 0;
		strReserved1 = "";
		strReserved2 = "";
		strReserved3 = "";
		field_list.clear();
		condition_list.clear();
	}

    //##ModelId=489EE4CA02FD
	/** @brief             字段链表*/
    vector<STTP_DBDATA_UNIT> field_list;
	
    //##ModelId=489EE4E4035B
	/** @brief             条件链表*/
    vector<STTP_DBDATA_UNIT> condition_list;

	/** @brief             备用字符串1*/
    string strReserved1;      
	
	/** @brief             备用字符串2*/
    string strReserved2;      
	
	/** @brief             备用字符串3*/
    string strReserved3;      
	
	/** @brief             备用int*/
    int nReserved;            
	
}STTP_DBDATA_RECORD;

/**
 * @brief       读取数据库配置信息结构（对应10001~10006扩展报文）
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EBE23001F
typedef struct _STTP_DATAFLAT_DATA
{
    //##ModelId=489EBE70035B
	/** @brief               配置类型*/
    int nCfgType;
	
    //##ModelId=489EBE770109
	/** @brief               信息类型*/
    int nInfoType;
	
    //##ModelId=489EBE87007D
	/** @brief               功能类型*/
    int nFunType;
	
    //##ModelId=489EBEDF003E
	/** @brief               记录链表（一条结果对应一条记录）*/
    vector <STTP_DBDATA_RECORD> record_list;
	
}STTP_DATAFLAT_DATA;

/**
 * @brief       sttp登陆报文报文体数据
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EB0AF032C
typedef struct _STTP_LOGIN_DATA
{
    //##ModelId=489EB0EA02AF
	/** @brief     模块名*/
    char chModle[17];
	
    //##ModelId=489EB0F4036B
	/** @brief     用户名*/;
    char chUser[17];
	
    //##ModelId=489EB10502AF
	/** @brief     密码*/
    char chPwd[17];
	
    //##ModelId=489EB1160213
	/** @brief     网络地址1*/
    NETADDR netAddr1;
	
    //##ModelId=489EB12100BB
	/** @brief     网络地址2*/
    NETADDR netAddr2;
	
    //##ModelId=489EB13102EE
	/** @brief     装置个数*/
    u_short uDevNum;
	
    //##ModelId=489EB14B01F4
	/** @brief     装置ID数组*/
    char chDevID[200][13];
	
}STTP_LOGIN_DATA;

/**
 * @brief       sttp文件数据（录播文件或通用文件传输）
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EB2310232
typedef struct _STTP_FILE_DATA
{
    //##ModelId=489EB26F03C8
	/** @brief     文件大小(单位:字节)*/
    int nSize;
	
    //##ModelId=489EB27601A5
	/** @brief     录波文件在装置中的编号*/
    int nFileNO;
	
    //##ModelId=489EBCE9009C
	/** @brief     文件起始偏移量*/
    int nOffset;
	
    //##ModelId=489EBD4C035B
	/** @brief     时间*/
    char ch_time[27];
	
    //##ModelId=489EBD620186
	/** @brief     录波文件名称(带相对路径)*/
    string strFileName;
	
    //##ModelId=489EBD6C01F4
	/** @brief     报文字符串内容*/
    string strReport;
	
}STTP_FILE_DATA;

/**
 * @brief       sttp报文体中可变成员部分
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EAD1703B9
typedef struct _STTP_VARI_MEMBER
{
    //##ModelId=489EAF580290
	/** @brief               定值、告警、动作、开关量、软压板等数据值列表*/
    vector<STTP_DATA> value_data;
	
    //##ModelId=489EAFBE0242
	/** @brief			     当存储动作信息时该部分存储特征量;*/
    vector<STTP_DATA> value_data2;
	
    //##ModelId=489EAFCE03A9
	/** @brief               文件列表信息*/
    vector <STTP_FILE_LIST_DATA> file_list;
	
    //##ModelId=489EB15E0271
	/** @brief               sttp 00101报文结构*/
    STTP_LOGIN_DATA login_data;
	
    //##ModelId=489EBDA2036B
	/** @brief               文件数据*/
    STTP_FILE_DATA file_data;
	
    //##ModelId=489EBF1F0186
    /** @brief               读取数据库信息结构*/
    STTP_DATAFLAT_DATA dataflat_data;
	
}STTP_VARI_MEMBER;

/**
 * @brief       sttp 报文体基本数据结构
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489E86B90128
typedef struct _STTP_BODY_BASIC
{
    //##ModelId=489E8732008C
	/** @brief              消息ID*/
    int nMsgId;
	
    //##ModelId=489E875A034B
	/** @brief              装置CPU号*/
    int nCpu;
	
    //##ModelId=489E87650177
	/** @brief              装置定值区号*/
    int nZone;
	
    //##ModelId=489E87750242
	/** @brief				状态,可表示为通信状态或者功能完成状态*/
    int nStatus;
	
    //##ModelId=489E8796006D
	/** @brief              标志位(取装置基本信息时用)*/
    int nFlag;
	
    //##ModelId=489E87A10232
	/** @brief				数据来源*/
    int nSource;
	
    //##ModelId=489E87AC003E
	/** @brief              命令来源*/
    int nCmdSource;
	
    //##ModelId=489E87C80167
	/** @brief              事件类型(取历史事件使用)*/
    int nEventType;
	
    //##ModelId=489E87DD01C5
	/** @brief              配置变化类型*/
    int nChangeConfigType;
	
    //##ModelId=489E87E40222
	/** @brief              配置变化信息类型*/
    int nChangeInfoType;
	
    //##ModelId=489E9F450213
	/** @brief              站点ID*/
    char ch_station_id[13];
	
    //##ModelId=489E9F610242
	/** @brief              装置PT_ID*/
    char ch_pt_id[13];
	
    //##ModelId=489E9F7C01F4
	/** @brief              19位STTP格式时间，起始时间*/
    char ch_time_20_BIT1[21];
	
    //##ModelId=489E9F8C034B
	/** @brief              19位STTP格式时间，完成时间*/
    char ch_time_20_BIT2[21];
	
    //##ModelId=489E9FAA02FD
	/** @brief              15位STTP格式时间，起始时间*/
    char ch_time_15_BIT1[16];
	
    //##ModelId=489E9FB40138
	/** @brief              15位STTP格式时间，完成时间*/
    char ch_time_15_BIT2[16];
	
    //##ModelId=489E9FC700DA
    /** @brief              12位STTP格式时间*/
    char ch_time_12_BIT1[13];
	
    //##ModelId=489E9FD002AF
	/** @brief              12位sttp格式时间2*/
    char ch_time_12_BIT2[13];
	
    //##ModelId=489E9FE5036B
	/** @brief              版本号*/
    char ch_version[65];
	
    //##ModelId=489E9FF20280
	/** @brief              校验码*/
    char ch_check_code[65];
	
    //##ModelId=489EA108034B
	/** @brief              处理名*/
    char ch_HandlerName[33];
	
    //##ModelId=489EA0300196
	/** @brief              文件相对路径*/
    string strRelativePath;
	
    //##ModelId=489EA041002E
	/** @brief              文件绝对路径*/
    string strAbsolutePath;
	
    //##ModelId=489EA0E00157
	/** @brief              带路径的文件名*/
    string strFilenameWithPath;
	
    //##ModelId=489EA14A0109
	/** @brief              消息内容*/
    string strMessage;
	
    //##ModelId=489EAD2C035B
	/** @brief              可变部分*/
    STTP_VARI_MEMBER variant_member;
	
}STTP_BODY_BASIC;

/**
 * @brief       sttp 报文头结构
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489E85FB03C8
typedef struct _STTP_HEAD_DATA
{
    //##ModelId=489E861A00CB
	/** @brief        返回信息标识符 */
    u_short uMsgRii;
	
    //##ModelId=489E86210242
	/** @brief        报文ID*/
    u_short uMsgID;
	
    //##ModelId=489E862600FA
	/** @brief        报文类别*/
    u_short uMsgType;
	
    //##ModelId=489E862A0242
	/** @brief        报文长度*/
    u_short uMsgLength;
	
    //##ModelId=489E862E035B
	/** @brief        长度类别*/
    u_short uMsgLengthType;
	
    //##ModelId=489E863502AF
	/** @brief        后续为标志*/
    u_short uMsgEndFlag;
	
}STTP_HEAD_DATA;

/**
 * @brief       sttp 报文头结构
 * @author      程清
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489E85D200BB
typedef struct _STTP_FULL_DATA
{
    //##ModelId=489E867100CB
	/** @brief    sttp报文头数据*/
    STTP_HEAD_DATA sttp_head;
	
    //##ModelId=489E86E7002E
	/** @brief    sttp报文体基本数据*/
    STTP_BODY_BASIC sttp_body;
	
}STTP_FULL_DATA;
/** @} */ //OVER

typedef int (*PFUNCALLBACK)( XJHANDLE pClientHandle , int pStatus , int pReserved);

typedef int (*PFUNONCONNECTCHANGE)( void* pParam , int pStatus , void* pReserved);

typedef struct _DevRegisterTable{
	//##ModelId=489EB13102EE
	/** @brief     装置个数*/
	u_short uDevNum;

	//##ModelId=489EB14B01F4
	/** @brief     装置ID数组*/
	char chDevID[200][13];
} DevRegisterTable;

/*************************************************************
 函 数 名：zero_sttp_full_data()
 功能概要: 初始化STTP_FULL_DATA内存
 返 回 值: void
 参    数：data      保存要初始化内存的STTP_FULL_DATA
***************************************************************/
static void zero_sttp_full_data(STTP_FULL_DATA & data)
{
	// head 
	bzero(&data.sttp_head,sizeof(STTP_HEAD_DATA));

	// body
	data.sttp_body.nMsgId             				  = 0;        				
	data.sttp_body.nCpu                               = 0;          			
	data.sttp_body.nZone                              = 0;          			
	data.sttp_body.nStatus                            = 0;          			
	data.sttp_body.nFlag                              = 0;          			
	data.sttp_body.nSource                            = 0;          			
	data.sttp_body.nCmdSource                         = 0;          			
	data.sttp_body.nEventType                         = 0;          			
	data.sttp_body.nChangeConfigType                  = 0;          			
	data.sttp_body.nChangeInfoType                    = 0;          			
	bzero( data.sttp_body.ch_station_id	 ,    sizeof( data.sttp_body.ch_station_id  ));     
	bzero( data.sttp_body.ch_pt_id           ,sizeof( data.sttp_body.ch_pt_id       ));     
	bzero( data.sttp_body.ch_time_20_BIT1    ,sizeof( data.sttp_body.ch_time_20_BIT1));     
	bzero( data.sttp_body.ch_time_20_BIT2    ,sizeof( data.sttp_body.ch_time_20_BIT2));     
	bzero( data.sttp_body.ch_time_15_BIT1    ,sizeof( data.sttp_body.ch_time_15_BIT1));     
	bzero( data.sttp_body.ch_time_15_BIT2    ,sizeof( data.sttp_body.ch_time_15_BIT2));     
	bzero( data.sttp_body.ch_time_12_BIT1    ,sizeof( data.sttp_body.ch_time_12_BIT1));     
	bzero( data.sttp_body.ch_time_12_BIT2    ,sizeof( data.sttp_body.ch_time_12_BIT2));     
	bzero( data.sttp_body.ch_version         ,sizeof( data.sttp_body.ch_version     ));     
	bzero( data.sttp_body.ch_check_code      ,sizeof( data.sttp_body.ch_check_code  ));     
	bzero( data.sttp_body.ch_HandlerName     ,sizeof( data.sttp_body.ch_HandlerName ));     
	data.sttp_body.strRelativePath                   = "";					
	data.sttp_body.strAbsolutePath                   = "";					
	data.sttp_body.strFilenameWithPath               = "";					
	data.sttp_body.strMessage                        = "";					

	
	// verible

	// login data
	bzero(&data.sttp_body.variant_member.login_data,sizeof(STTP_LOGIN_DATA));

	// value data
	if(data.sttp_body.variant_member.value_data.size()>0)
		data.sttp_body.variant_member.value_data.clear();

	// value data 2
    if(data.sttp_body.variant_member.value_data2.size()>0)
		data.sttp_body.variant_member.value_data2.clear();

	// file list
	if(data.sttp_body.variant_member.file_list.size()>0)
		data.sttp_body.variant_member.file_list.clear();

	// file_data
	data.sttp_body.variant_member.file_data.nFileNO = 0;
	data.sttp_body.variant_member.file_data.nOffset = 0;
	data.sttp_body.variant_member.file_data.nSize   = 0;
	bzero( data.sttp_body.variant_member.file_data.ch_time, 27 );
	data.sttp_body.variant_member.file_data.strFileName = "";
	data.sttp_body.variant_member.file_data.strReport = "";

	// data_flat_data
	data.sttp_body.variant_member.dataflat_data.nCfgType = 0;
	data.sttp_body.variant_member.dataflat_data.nFunType = 0;
	data.sttp_body.variant_member.dataflat_data.nInfoType = 0;
	int recrd_num = data.sttp_body.variant_member.dataflat_data.record_list.size();
	if (  recrd_num >0 )
	{
		for (int i =0; i<recrd_num; i++)
		{
			data.sttp_body.variant_member.dataflat_data.record_list[i].field_list.clear();	
			data.sttp_body.variant_member.dataflat_data.record_list[i].condition_list.clear();
			data.sttp_body.variant_member.dataflat_data.record_list[i].nReserved = 0;
			data.sttp_body.variant_member.dataflat_data.record_list[i].strReserved1 = "";
			data.sttp_body.variant_member.dataflat_data.record_list[i].strReserved2 = "";
			data.sttp_body.variant_member.dataflat_data.record_list[i].strReserved3 = "";
		}
		data.sttp_body.variant_member.dataflat_data.record_list.clear();
	}
}

#endif // XJSttpNetAPI_h__