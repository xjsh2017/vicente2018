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
 * @defgroup STTP�ͻ��˵�ַ��Ϣ����
 * @{
 */

//net ��ַ�ṹ
typedef struct _STRUCT_NETADDR_INFO
{
	char	IpAddr[40];                                    //IP��ַ
	u_short	nPort;                                         //�˿ں�
}STRUCT_NETADDR_INFO;

/**
 * @brief       ����˵�ַ��Ϣ�ṹ
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
typedef struct _STRUCT_SERVER_INFO
{
	/** @brief        �Ƿ��б��õ�ַ*/
	bool bHasReservedAddr;
	/** @brief        ��ַA������*/
	STRUCT_NETADDR_INFO netAddrA;
	/** @brief        ��ַB������*/
	STRUCT_NETADDR_INFO netAddrB;
}STRUCT_SERVER_INFO;


/**
 * @brief       Ŀ��������Ϣ�ṹ
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
typedef struct _STRUCT_DESTINATION
{
	/** @brief        �Ƿ��б��÷����*/
	bool bHasReservedServer;
	/** @brief        �����A������*/
	STRUCT_SERVER_INFO serverA;
	/** @brief        �����B������*/
	STRUCT_SERVER_INFO serverB;
}STRUCT_DESTINATION_SERVER;

/** @} */ //OVER






/**
 * @defgroup STTP ���ݽṹ����
 * @{
 */


/**
 * @brief       STTP��Ԫ���� ����ֵ��ģ����������������ѹ�塢�������澯�����ݵ�ԭ�ṹ��
 * @author      ����
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
	/** @brief        ���ݱ��*/
    int id;
	
    //##ModelId=489EADDD03D8
	/** @brief        cpu�ţ�����ʷ�¼���������λ�õ�*/
    int nCpu;
	
    //##ModelId=489EADB90203
	/** @brief        ����ֵ(�ַ���)*/
    string str_value;
	
    //##ModelId=489EAE1901B5
	/** @brief        ����/�澯ʱ��*/
    char ch_Time[16];
	
    //##ModelId=489EAE2701D4
	/** @brief        ���ã��ַ��ͣ�*/
    string str_reserved;
	
    //##ModelId=489EAE5B007D
	/** @brief        ���ã����ͣ�*/
    int int_reserved;
	
    //##ModelId=489EAE6A0128
	/** @brief        ���ã������ͣ�*/
    float flt_reserved;
	
}STTP_DATA;

/**
 * @brief       STTP�ļ��б�����
 * @author      ����
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
	/** @brief        �ļ���С*/
    int nFileSize;
	
    //##ModelId=489EAF1501A5
	/** @brief         �ļ����*/
    int nFileNO;
	
    //##ModelId=489EAF250232
	/** @brief        ����ʱ��*/
    char ch_time[30];
	
    //##ModelId=489EAF3203D8
	/** @brief        �ļ���*/
    string strFielName;
	
    //##ModelId=489EAF3E009C
	/** @brief        ���ϸ�Ҫ����*/
    string strFault;
	
}STTP_FILE_LIST_DATA;

/**
 * @brief       ��ȡ���ݿ�����ʱ��Ԫ�������ͣ���Ӧÿ���ֶλ�������
 * @author      ����
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
	/** @brief         ��������*/
    int nDataType;
	
    //##ModelId=489EE4AF032C
	/** @brief         ���ݿ��*/
    int nDataLength;
	
    //##ModelId=489EE4B701E4
	/** @brief         ��������*/
    string strValue;

	/** @brief         �������ڴ���ʱ��*/ 
    string strReserved;
	
}STTP_DBDATA_UNIT;

/**
 * @brief       ��ȡ���ݿ�����ʱһ����¼�Ľṹ
 * @author      ����
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
	/** @brief             �ֶ�����*/
    vector<STTP_DBDATA_UNIT> field_list;
	
    //##ModelId=489EE4E4035B
	/** @brief             ��������*/
    vector<STTP_DBDATA_UNIT> condition_list;

	/** @brief             �����ַ���1*/
    string strReserved1;      
	
	/** @brief             �����ַ���2*/
    string strReserved2;      
	
	/** @brief             �����ַ���3*/
    string strReserved3;      
	
	/** @brief             ����int*/
    int nReserved;            
	
}STTP_DBDATA_RECORD;

/**
 * @brief       ��ȡ���ݿ�������Ϣ�ṹ����Ӧ10001~10006��չ���ģ�
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EBE23001F
typedef struct _STTP_DATAFLAT_DATA
{
    //##ModelId=489EBE70035B
	/** @brief               ��������*/
    int nCfgType;
	
    //##ModelId=489EBE770109
	/** @brief               ��Ϣ����*/
    int nInfoType;
	
    //##ModelId=489EBE87007D
	/** @brief               ��������*/
    int nFunType;
	
    //##ModelId=489EBEDF003E
	/** @brief               ��¼����һ�������Ӧһ����¼��*/
    vector <STTP_DBDATA_RECORD> record_list;
	
}STTP_DATAFLAT_DATA;

/**
 * @brief       sttp��½���ı���������
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EB0AF032C
typedef struct _STTP_LOGIN_DATA
{
    //##ModelId=489EB0EA02AF
	/** @brief     ģ����*/
    char chModle[17];
	
    //##ModelId=489EB0F4036B
	/** @brief     �û���*/;
    char chUser[17];
	
    //##ModelId=489EB10502AF
	/** @brief     ����*/
    char chPwd[17];
	
    //##ModelId=489EB1160213
	/** @brief     �����ַ1*/
    NETADDR netAddr1;
	
    //##ModelId=489EB12100BB
	/** @brief     �����ַ2*/
    NETADDR netAddr2;
	
    //##ModelId=489EB13102EE
	/** @brief     װ�ø���*/
    u_short uDevNum;
	
    //##ModelId=489EB14B01F4
	/** @brief     װ��ID����*/
    char chDevID[200][13];
	
}STTP_LOGIN_DATA;

/**
 * @brief       sttp�ļ����ݣ�¼���ļ���ͨ���ļ����䣩
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EB2310232
typedef struct _STTP_FILE_DATA
{
    //##ModelId=489EB26F03C8
	/** @brief     �ļ���С(��λ:�ֽ�)*/
    int nSize;
	
    //##ModelId=489EB27601A5
	/** @brief     ¼���ļ���װ���еı��*/
    int nFileNO;
	
    //##ModelId=489EBCE9009C
	/** @brief     �ļ���ʼƫ����*/
    int nOffset;
	
    //##ModelId=489EBD4C035B
	/** @brief     ʱ��*/
    char ch_time[27];
	
    //##ModelId=489EBD620186
	/** @brief     ¼���ļ�����(�����·��)*/
    string strFileName;
	
    //##ModelId=489EBD6C01F4
	/** @brief     �����ַ�������*/
    string strReport;
	
}STTP_FILE_DATA;

/**
 * @brief       sttp�������пɱ��Ա����
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489EAD1703B9
typedef struct _STTP_VARI_MEMBER
{
    //##ModelId=489EAF580290
	/** @brief               ��ֵ���澯������������������ѹ�������ֵ�б�*/
    vector<STTP_DATA> value_data;
	
    //##ModelId=489EAFBE0242
	/** @brief			     ���洢������Ϣʱ�ò��ִ洢������;*/
    vector<STTP_DATA> value_data2;
	
    //##ModelId=489EAFCE03A9
	/** @brief               �ļ��б���Ϣ*/
    vector <STTP_FILE_LIST_DATA> file_list;
	
    //##ModelId=489EB15E0271
	/** @brief               sttp 00101���Ľṹ*/
    STTP_LOGIN_DATA login_data;
	
    //##ModelId=489EBDA2036B
	/** @brief               �ļ�����*/
    STTP_FILE_DATA file_data;
	
    //##ModelId=489EBF1F0186
    /** @brief               ��ȡ���ݿ���Ϣ�ṹ*/
    STTP_DATAFLAT_DATA dataflat_data;
	
}STTP_VARI_MEMBER;

/**
 * @brief       sttp ������������ݽṹ
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489E86B90128
typedef struct _STTP_BODY_BASIC
{
    //##ModelId=489E8732008C
	/** @brief              ��ϢID*/
    int nMsgId;
	
    //##ModelId=489E875A034B
	/** @brief              װ��CPU��*/
    int nCpu;
	
    //##ModelId=489E87650177
	/** @brief              װ�ö�ֵ����*/
    int nZone;
	
    //##ModelId=489E87750242
	/** @brief				״̬,�ɱ�ʾΪͨ��״̬���߹������״̬*/
    int nStatus;
	
    //##ModelId=489E8796006D
	/** @brief              ��־λ(ȡװ�û�����Ϣʱ��)*/
    int nFlag;
	
    //##ModelId=489E87A10232
	/** @brief				������Դ*/
    int nSource;
	
    //##ModelId=489E87AC003E
	/** @brief              ������Դ*/
    int nCmdSource;
	
    //##ModelId=489E87C80167
	/** @brief              �¼�����(ȡ��ʷ�¼�ʹ��)*/
    int nEventType;
	
    //##ModelId=489E87DD01C5
	/** @brief              ���ñ仯����*/
    int nChangeConfigType;
	
    //##ModelId=489E87E40222
	/** @brief              ���ñ仯��Ϣ����*/
    int nChangeInfoType;
	
    //##ModelId=489E9F450213
	/** @brief              վ��ID*/
    char ch_station_id[13];
	
    //##ModelId=489E9F610242
	/** @brief              װ��PT_ID*/
    char ch_pt_id[13];
	
    //##ModelId=489E9F7C01F4
	/** @brief              19λSTTP��ʽʱ�䣬��ʼʱ��*/
    char ch_time_20_BIT1[21];
	
    //##ModelId=489E9F8C034B
	/** @brief              19λSTTP��ʽʱ�䣬���ʱ��*/
    char ch_time_20_BIT2[21];
	
    //##ModelId=489E9FAA02FD
	/** @brief              15λSTTP��ʽʱ�䣬��ʼʱ��*/
    char ch_time_15_BIT1[16];
	
    //##ModelId=489E9FB40138
	/** @brief              15λSTTP��ʽʱ�䣬���ʱ��*/
    char ch_time_15_BIT2[16];
	
    //##ModelId=489E9FC700DA
    /** @brief              12λSTTP��ʽʱ��*/
    char ch_time_12_BIT1[13];
	
    //##ModelId=489E9FD002AF
	/** @brief              12λsttp��ʽʱ��2*/
    char ch_time_12_BIT2[13];
	
    //##ModelId=489E9FE5036B
	/** @brief              �汾��*/
    char ch_version[65];
	
    //##ModelId=489E9FF20280
	/** @brief              У����*/
    char ch_check_code[65];
	
    //##ModelId=489EA108034B
	/** @brief              ������*/
    char ch_HandlerName[33];
	
    //##ModelId=489EA0300196
	/** @brief              �ļ����·��*/
    string strRelativePath;
	
    //##ModelId=489EA041002E
	/** @brief              �ļ�����·��*/
    string strAbsolutePath;
	
    //##ModelId=489EA0E00157
	/** @brief              ��·�����ļ���*/
    string strFilenameWithPath;
	
    //##ModelId=489EA14A0109
	/** @brief              ��Ϣ����*/
    string strMessage;
	
    //##ModelId=489EAD2C035B
	/** @brief              �ɱ䲿��*/
    STTP_VARI_MEMBER variant_member;
	
}STTP_BODY_BASIC;

/**
 * @brief       sttp ����ͷ�ṹ
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489E85FB03C8
typedef struct _STTP_HEAD_DATA
{
    //##ModelId=489E861A00CB
	/** @brief        ������Ϣ��ʶ�� */
    u_short uMsgRii;
	
    //##ModelId=489E86210242
	/** @brief        ����ID*/
    u_short uMsgID;
	
    //##ModelId=489E862600FA
	/** @brief        �������*/
    u_short uMsgType;
	
    //##ModelId=489E862A0242
	/** @brief        ���ĳ���*/
    u_short uMsgLength;
	
    //##ModelId=489E862E035B
	/** @brief        �������*/
    u_short uMsgLengthType;
	
    //##ModelId=489E863502AF
	/** @brief        ����Ϊ��־*/
    u_short uMsgEndFlag;
	
}STTP_HEAD_DATA;

/**
 * @brief       sttp ����ͷ�ṹ
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *  
 */
//##ModelId=489E85D200BB
typedef struct _STTP_FULL_DATA
{
    //##ModelId=489E867100CB
	/** @brief    sttp����ͷ����*/
    STTP_HEAD_DATA sttp_head;
	
    //##ModelId=489E86E7002E
	/** @brief    sttp�������������*/
    STTP_BODY_BASIC sttp_body;
	
}STTP_FULL_DATA;
/** @} */ //OVER

typedef int (*PFUNCALLBACK)( XJHANDLE pClientHandle , int pStatus , int pReserved);

typedef int (*PFUNONCONNECTCHANGE)( void* pParam , int pStatus , void* pReserved);

typedef struct _DevRegisterTable{
	//##ModelId=489EB13102EE
	/** @brief     װ�ø���*/
	u_short uDevNum;

	//##ModelId=489EB14B01F4
	/** @brief     װ��ID����*/
	char chDevID[200][13];
} DevRegisterTable;

/*************************************************************
 �� �� ����zero_sttp_full_data()
 ���ܸ�Ҫ: ��ʼ��STTP_FULL_DATA�ڴ�
 �� �� ֵ: void
 ��    ����data      ����Ҫ��ʼ���ڴ��STTP_FULL_DATA
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