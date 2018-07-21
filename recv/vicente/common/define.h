//
// define.h
//
// This is a define and include file
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//

#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H
#pragma warning(disable : 4786)//����4786����

#ifdef OS_HPUNIX
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#endif
#endif

// c_runtime lib
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>
#include <signal.h>
#ifdef OS_AIX
	#ifndef _THREAD_SAFE
	#define _THREAD_SAFE 1
	#endif
#endif
#include <errno.h>
#include <string.h>  
#include <ctype.h>
#include <stddef.h>
#include <stdarg.h>

//standard c++ lib
#include <iostream>
#include <strstream>
#include <string>
#include <iterator>
#include <cassert>
#include <fstream>
#include <list>
#include <deque>
#include <map>
#include <numeric>            
#include <set>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <queue>
#include <complex>
#include <cctype>             
#include <new>
#include <cstdarg>             
#include <utility>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cerrno>
#include <csignal>
#include <ctime>

/*
 *	�ڲ�˽��ͷ�ļ�����
 */

#include "XJErrorID.h"
/* ---------------------------------------- */

using namespace std;
/* ------ Added by qingch   2009/02/10 ----- */
#ifdef OS_SUNUNIX
	#include <sys/sockio.h>
	#include <sys/param.h>
	#include <net/if_arp.h> 
        #include <poll.h>
	/* ���������� */
	#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
	#define SOL_IP IPPROTO_IP
	#define MSG_NOSIGNAL 0
	/* ------ commented by jjl   2011/06/09 ----- */
	//#ifndef __SOSELECT_TIMEOUT
	//#define __SOSELECT_TIMEOUT
        #ifndef __SOPOLL_TIMEOUT
        #define __SOPOLL_TIMEOUT	
   	#endif	
#endif
/* ---------------------------------------- */

#ifdef OS_AIX
    #include	<termio.h>
	
	/* ���������� */
	#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
	#define SOL_IP IPPROTO_IP	
	#define MSG_NOSIGNAL 0
	#ifndef __XJSOCKLEN_T
	#define __XJSOCKLEN_T	
		//typedef unsigned long            SOCK_LEN;  
		typedef __ulong32_t            SOCK_LEN;//apply for aix inttypes.h define
	#endif	
	#ifndef __SOSELECT_TIMEOUT
	#define __SOSELECT_TIMEOUT	
	#endif	
    #ifndef _BIG_ENDIAN_CPU_
    #define _BIG_ENDIAN_CPU_
    #endif	        
#endif

#ifdef OS_HPUNIX
	//#include	<termio.h>
			
	/* ���������� */
	#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
	#define SOL_IP IPPROTO_IP	
	#define MSG_NOSIGNAL 0
	#ifndef __XJSOCKLEN_T
	#define __XJSOCKLEN_T	
		typedef int            SOCK_LEN;  
	#endif	
		
	#ifndef __SOSELECT_TIMEOUT
	#define __SOSELECT_TIMEOUT	
	#endif	
	#ifndef _BIG_ENDIAN_CPU_
	#define _BIG_ENDIAN_CPU_
	#endif	        
#endif

#ifdef OS_LINUX 

    //ͷ�ļ�����
    #include <sys/types.h>  
    #include <netinet/in.h>  
    #include <sys/socket.h>  
    #include <sys/wait.h>  
    #include <arpa/inet.h>       
    #include <unistd.h>         
    #include <sys/select.h>
    #include <sys/time.h>
    #include <sys/ioctl.h> 
    #include <termios.h>
    #include <netdb.h>
	#include <pthread.h>
    #include <fcntl.h>
	#include <sys/ipc.h>
    #include <sys/msg.h>
    #include <sys/stat.h>
	#include <dirent.h>

	#include <semaphore.h>
	#include <sys/ipc.h>
	#include <sys/sem.h>
	#include <signal.h>
	#include <dlfcn.h>  
	#include <termios.h>    

	// on Linux, socket is just a descriptor number
	typedef int                     SOCKET;
    #define INVALID_SOCKET          -1              //(~0)
    #define SOCKET_ERROR            -1              //(-1) 
    #define SOCKET_SEND_FLAG        MSG_NOSIGNAL

	#ifndef __XJSOCKLEN_T
	#define __XJSOCKLEN_T	
		typedef unsigned int            SOCK_LEN;       //linux��socket��һЩapi�в���unsigned int
	#endif	

	//BOOL define
	typedef int                     BOOL;
	#define FALSE                   0
    #define TRUE                    1

	//LPVOID define
	typedef void                    *LPVOID;
	//LPTSTR define
	typedef char*					LPTSTR;

	//thread define
	typedef pthread_t               THREAD_ID;      //linux���߳�IDΪpthread_t (unsigned int)
	typedef pthread_t               THREAD_HANDLE;  //linux���̴߳����󷵻�Ϊint
	typedef void *                  THREAD_FUNC;    //Linux���߳���ں�����������
    #define WINAPI                                  //Linux��û��WINAPI(__stdcall) 
    #define INVALID_THREAD          -1              //Linux���̴߳���ʧ�ܷ���-1
    #define THREAD_RETURN           NULL            //Linux���̵߳ķ���ֵ    
	
	//process define
	typedef pid_t					HPROCESS;
	
	//mutex
	typedef pthread_mutex_t         MUTEX;          //Linux���̻߳���������
	typedef pthread_cond_t			HCOND;

	//semaphore define
	typedef int						HSEM;
	typedef sem_t					HNSEM;
	
	//handle define
	typedef void*					XJHANDLE;

#endif

#ifdef OS_WINDOWS
    #define  sleep(x)                  Sleep(x*1000)  //������Linux����ͬ����ʽ  
    #include <Winsock2.h>
    #include <process.h>
    #include <conio.h>
	#include <Windows.h>
	#include <io.h>
	#include <sys/stat.h>


	//THREAD define
    typedef  unsigned long             THREAD_ID;      //windows���߳�IDΪDWORD(unsiged long)
    typedef  HANDLE                    THREAD_HANDLE;  //windows���̴߳����󷵻�ΪHANDLE (void *)
    typedef  unsigned int              THREAD_FUNC;    //windows���߳���ں�����������
	#define INVALID_THREAD             NULL            //CreateThread()ʧ�ܷ���NULL
	#define THREAD_RETURN              0               //�̷߳���ֵΪDWORD

	//socket define
	typedef int                        SOCK_LEN;       //windows��socket��һЩapi�в��� int
    #define SOCKET_SEND_FLAG           0

	//lock
	typedef CRITICAL_SECTION			MUTEX;          //windows�µĻ���������

	typedef struct 
	{
		unsigned int count;
		unsigned int lastop; //0 none,1 signal,2 broadcast
		HANDLE		hEvent;
	}HCOND;

	//semaphore define
	typedef HANDLE					HSEM;
	typedef HANDLE					HNSEM;
	
	//handle define
	typedef HANDLE					XJHANDLE;

#endif

//�������Ͷ���
typedef unsigned char                   BYTE;
typedef unsigned short                  WORD;
typedef unsigned short                  u_short;
typedef unsigned int                    UINT;
typedef char							sint8;
typedef unsigned char					uint8;
typedef short							sint16;
typedef unsigned short					uint16;
typedef int								sint32;
typedef unsigned int					uint32;
typedef long							slong;
typedef unsigned long					ulong;
typedef unsigned long					DWORD;
typedef float							float32;
typedef double							float64;

//function define
#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))
#define bzero(ptr,n) memset(ptr,0,n)
    
#ifdef _BIG_ENDIAN_CPU_
#define SWAP_16(b)\
{\
    unsigned short&a =  *(unsigned short*)(&(b));\
    a = (a>>8)|(a<<8);\
}
#define SWAP_32(b)\
{\
    unsigned &a = *(unsigned *)(&(b));\
    a = (a>>24)|((a>>8)&0xFF00)|((a<<8)&0xFF0000)|(a<<24);\
}
#else
#define SWAP_16(b)
#define SWAP_32(b)
#endif


//msg define
#define MAXMSGLEN                0x2800                    //SOCKET���ݻ�������󳤶� 10240(10K)
#define MAX_SESSION              300                       //���ĻỰ��Դ��
#define CREATE_THREAD_NUM        10                        //��һ��������������
#define MAX_LINE_LENGTH          1024                      //�ļ���һ�е���󳤶�
#define FILE_ERROR_MAX_LEN       255                       //�����ļ�����ʱ,������������
#define FILE_NAME_MAX_LEN        255                       //�ļ�����󳤶�

#define MAX_MSGLENGTH            512                       //��Ϣ����󳤶�
#define MAX_FIELD_NAME_LEN       256                       //������󳤶�
#define MAX_FIELD_VALE_LEN       256                       //�ֶ�ֵ��󳤶�
#define MAX_CONDITION_LEN        512                       //������󳤶�

/****************************net--socket*********************************/

#define LOCAL_ACT_AS_SERVER	     0                         //Socket��Ϊ����������
#define LOCAL_ACT_AS_CLIENT	     1                         //socket��Ϊ�ͻ�������
#define LOCAL_ACT_AS_UNKNOW	    -1                         //socket����Ϊ����

#define ID_OUTPUT_SEND           0                         //����
#define ID_OUTPUT_READ           1                         //����

#define RECVTIME                 1                         //����ʱ��
#define SENDTIME                 2                         //����ʱ��
#define BLOCKING                 3                         //������ʽ
#define UNBLOCKING               4                         //��������ʽ
#define CONNTIME                 5                         //���ӳ�ʱʱ��
#define BROADCAST                6						   //�㲥��ʽ
#define REUSEADDR				 7						   //�˿ڹ���

#define ID_NET                   0                         //����
#define ID_COM                   1                         //����
#define ID_RAS                   2                         //modem 
#define ID_OTHERS                3                         //����

#define TIMEOUT                 -2                         //��socket�������ݳ�ʱʱ����ֵ

#define XJ_STTP_CMD_AUTOCALL_FLAG 2                       //�Զ�Ѳ���־


/**
 * @brief       ���ݿ�����
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
/** @brief        MYSQL���ݿ�*/
const int DATABASE_MYSQL          = 1;

/** @brief        ORACLE���ݿ�*/
const int DATABASE_ORACLE         = 2;

/** @brief        SQLSERVER���ݿ�*/
const int DATABASE_SQLSERVER      = 3;

/** @brief        �������ݿ�*/
const int DATABASE_DAMENG = 4;

/** @brief        ������ݿ�*/
const int DATABASE_KINGBASE = 5;

/**
 * @brief       �豸����
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */
//��������ʶ���豸����
const int TYPE_BUS		= 1;		//ĸ��
const int TYPE_LINE		= 2;		//��·
const int TYPE_BREAK	= 3;		//����
const int TYPE_GEN		= 4;		//�����
const int TYPE_REACTOR	= 5;		//��ѹ�翹��
const int TYPE_TRANS	= 6;		//��ѹ��
const int TYPE_SWITCH	= 7;		//��բ
const int TYPE_CAP      = 8;		//������
const int TYPE_PMS		= 9;		//���������
const int TYPE_BAY		= 10;		//���
const int TYPE_STATION	= 18;		//��վ
const int TYPE_NET		= 19;		//����
const int TYPE_SEC		= 20;		//�����豸//�������ɹ�ĳһ���豸�Ķ����豸
const int TYPE_PROTECT	= 21;		//���� 
const int TYPE_WAVEREC	= 22;		//¼����
const int TYPE_FD_DEV	= 23;		//���ϲ��װ����������
const int TYPE_FD_STATION	= 24;	//���ϲ��վ��������
const int TYPE_STATION_GROUP = 50;  //��վ�����
const int TYPE_DEVICE_GROUP  = 51;  //װ�������
const int TYPE_BREAKER_DEVICE = 53; //������һ���豸����
const int TYPE_STATION_SVG = 54;    //��վSVG
const int TYPE_CPU         = 99;    //CPU����

//�ҽ��ڶ����豸��
const int TYPE_STATION_STATUS = 1002;//��վͨѶ״̬
const int TYPE_AChannel    = 1000;
const int TYPE_OrderSetting = 1001;
const int TYPE_ZONE_BASE = 1002;//���Ż�����Ϣ
const int TYPE_ALARM_DEF = 1003;//�澯������Ϣ
const int TYPE_ACTION_DEF = 1004;//����������Ϣ
const int TYPE_SETTING_DEF = 1005;//��ֵ������Ϣ
const int TYPE_AI_DEF = 1006; //ģ����������Ϣ
const int TYPE_DI_DEF = 1007; //������������Ϣ
const int TYPE_SOFTBOARD_DEF = 1008; //��ѹ�������Ϣ
const int TYPE_OSC_AI_DEF = 1009;//����¼��ģ����ͨ��������Ϣ
const int TYPE_OSC_DI_DEF = 1010;//����¼��������ͨ��������Ϣ
const int TYPE_FRONT_COMMU_GROUP = 1011; //����ͨ�÷����������
const int TYPE_PT_CHARACTER_DEF = 1012; //��������������
const int TYPE_ZONE_DATA = 1013; //��ֵ������������
const int TYPE_SETTING_DATA = 1014; //��ֵ��������
const int TYPE_PT_DEVICE_GROUP = 1015; //װ�������
const int TYPE_PT_SETTING_DATA = 1016; //��ֵ��������
const int TYPE_PT_ZONE_DATA = 1017; //��ֵ��������
const int TYPE_PT_GROUP = 1018; //װ�������
const int TYPE_UPLOADATUO_CONFIG = 1019; //����Ϣ���ඨ��������Ϣ
const int TYPE_DEVMAINSTATION_CONFIG = 1020; //���豸������Ϣ

//���ַ��ͱ�ʶ���豸����
#define STRING_BUS		"ĸ��"		//ĸ��
#define STRING_LINE		"��·"		//��·
#define STRING_BREAK	"����"		//����
#define STRING_GEN		"�����"	//�����
#define STRING_CAP		"������"	//������
#define STRING_REACTOR	"��ѹ�翹��"//��ѹ�翹��
#define STRING_TRANS	"��ѹ��"	//��ѹ��
#define STRING_SWITCH	"��բ"		//��բ
#define STRING_PMS      "���������" //���������
#define STRING_BAY		"���"		//���
#define STRING_NET		"����"		//����
#define STRING_STATION	"��վ"		//��վ
#define	STRING_SEC		"�����豸"	//�����豸
#define STRING_PROTECT	"����װ��"	//����װ��
#define STRING_WAVEREC	"¼����"	//¼����
#define STRING_DISTANCE "���װ��"	//���װ��

//20149�����еĸ�������(�ⲿҪ������ƽ̨�����ڴ�Ҫ��)
const int TYPE_STATION_INFO    = 1;//��վ��Ϣ
const int TYPE_PROTECT_INFO    = 2;//װ����Ϣ
const int TYPE_NORMAL_INFO     = 3;//��ͨ����������Ϣ

//�����ڴ�ʱ��
const  int STATUS_NORMAL = 0;
const  int STATUS_UPDATE = 1;
const  int STATUS_WRITE  = 2;
const  int STATUS_READ   = 3;

//net ��ַ�ṹ
typedef struct _NETADDR
{
	char	IpAddr[40];                                    //IP��ַ
	u_short	nPort;                                         //�˿ں�
}NETADDR;

//socket�������ṹ
typedef struct _BUFFER
{
	WORD    len;                                           //���ĳ���
	char    buf[MAXMSGLEN];
}XJBUFFER;


//ϵͳʱ��ṹ
typedef struct _SYSTIME
{
	u_short wYear; 
    u_short wMonth; 
    u_short wDayOfWeek; 
	u_short wDayOfYear;
    u_short wDay; 
    u_short wHour; 
    u_short wMinute; 
    u_short wSecond; 
    u_short wMilliseconds; 
} SYSTIME;

//ϵͳ�����ܳ�ʱ���ýṹ
typedef struct _TIMEOUTS
{
	//�ٻ�
	UINT   call_protect_settingdata;            //�ٻ�������ֵ
	UINT   call_wav_settingdata;                //�ٻ�¼������ֵ
	UINT   call_protect_comm;                   //�ٻ�����ͨѶ״̬
	UINT   call_anai;                           //�ٻ�ģ����
	UINT   call_switch;                         //�ٻ�������
	UINT   call_softboard;                      //�ٻ���ѹ��
	UINT   call_runstatus;                      //�ٻ�����״̬
	UINT   call_wavfile_list;                   //�ٻ�¼���ļ��б�
	UINT   call_waverfile;                      //�ٻ�¼���ļ�
	UINT   call_common_file;                    //�ٻ�ͨ���ļ�
	UINT   call_common_file_list;               //�ٻ�ͨ���ļ��б�
	UINT   call_zone;                           //�ٻ���ֵ����
	UINT   call_wav_comm;                       //�ٻ�¼����ͨѶ״̬
	UINT   call_basic_info;                     //�ٻ�������Ϣ
	UINT   call_dev_time;                       //�ٻ�װ��ʱ��
    UINT   call_history_info;                   //�ٻ���ʷ�¼�
    UINT   call_single_config;                  //�ٻ�ĳ̨װ�õ�ĳ����Ϣ������
	UINT   call_total;                          //���ٻ�
	UINT   call_channel_status;                 //�ٻ�ͨ��״̬

	//����
	UINT   set_dev_time;                        //��ʱ
	UINT   set_setting_data;                    //�޸Ķ�ֵ
	UINT   set_zone;                            //��ֵ���л�
	UINT   set_soft_board;                      //ѹ��Ͷ��
	UINT   set_signal_reset;                    //�źŸ���
	UINT   set_wav_remote_touch;                //¼����Զ������
    UINT   set_event_signal;                    //��վ�ź���Զ�̶���

	//103
	UINT   t0;                                  //���ӽ�����ʱ
	UINT   t1;                                  //���ղ��Ա��ĳ�ʱ
	UINT   t2;                                  //������ȷ�ϳ�ʱ
	UINT   t3;                                  //���ڿ����·��Ͳ��Ա��ĳ�ʱ

}TIMEOUTS;

 
/**
 * @defgroup STTP ���Ľṹ����
 * @{
 */

/** @brief           STTP��������󳤶�(8192)*/
const UINT MAX_STTP_MSG_LEN = 0x2000; 

/**
 * @brief       STTP����ͷ�ṹ
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

typedef struct _STTPMSGHDR
{
	BYTE m_byteMsgSubsetId[3] ;              //������Ϣ��ʶ��RII
	BYTE m_byteMsgID[5] ;                    //����ID
	BYTE m_byteMsgType ;                     //�������
	BYTE m_byteMsgLength[5] ;                //���ĳ���
	BYTE m_byteMsgLengthType ;               //�������
	BYTE m_byteMsgVersion ;                  //������־
}STTPMSGHDR;


/**
 * @brief       STTPȫ���Ľṹ
 * @author      ����
 * @version     ver1.0
 * @date        03/09/2008
 *
 */

typedef struct _STTPMSG
{
    STTPMSGHDR   MsgHdr;                     //��Ϣͷ
	char         MsgBody[MAX_STTP_MSG_LEN] ; //��Ϣ��
}STTPMSG;

typedef vector<STTPMSG> STTPMSG_QUEUE;

/** @} */ //OVER

//DATAMSG ���(���뵽CSttpDeque���еĽ��)
typedef struct _STTP_DEQUE_MSG
{
	STTPMSG        msg;
	UINT           nTimes;              //���ڼ�¼����Ϣ�ڶ����б�ɨ�赽�Ĵ���
	bool           bTreat;              //�Ƿ������־(�涨ʱ����û��ȡ�߰������ݴ���)
}STTP_DEQUE_MSG;

typedef struct _FILE_STATUS_STRUCT
{
	char cFileName[100];
	time_t stWriteTime;
	int   nFileSize;
}FILE_STATUS_STRUCT;

#ifndef MFC_ADO_USED_DEFINE

//�ֶνṹ
typedef struct _Field
{
	_Field()
	{
		FieldType = -1;
		bzero(FieldName, MAX_FIELD_NAME_LEN);
		bzero(FieldValue, MAX_FIELD_VALE_LEN);
	}

	int  FieldType;       //�ֶ�����
	char FieldName[MAX_FIELD_NAME_LEN];  //�ֶ���
	char FieldValue[MAX_FIELD_VALE_LEN]; //�ֶ�ֵ
}Field;

//�����ṹ
typedef struct _Condition
{
	_Condition()
	{
		ConditionType = -1;
		bzero(ConditionContent, MAX_CONDITION_LEN);
	}

	UINT ConditionType; //�Ƿ�Ϊ�����ֶ� Ϊ0Ϊwhere Ϊ1Ϊorder����group
	char ConditionContent[MAX_CONDITION_LEN]; //��������
}Condition;

//SQL���ṹ
typedef struct _SQL_DATA
{
	vector <Field> Fieldlist;          //�ֶ�����
	vector <Condition> Conditionlist;  //��������
}SQL_DATA;

#endif


/**	\brief	The _MutiSQL_DATA struct ֱ��ִ��SQL���ṹ��

*/
const int MAX_SQL_LENGTH = 0x1C00; //7K
typedef struct _MutiSQL_DATA
{
	_MutiSQL_DATA()
	{
		Funtype = -1;
		bzero(SQL_BODY_Content, MAX_SQL_LENGTH);
	}

	int  Funtype;//�������ͱ�ʶΪ��ѯ(��Ҫ���ؽ������),��������ֻ����ִ�н��(ִ�гɹ�ʧ��)
	char SQL_BODY_Content[MAX_SQL_LENGTH];//sql�������
}MutiSQL_DATA;

typedef struct _REALDATA_CONDITION
{
	//add constructors for default value
	//2014-10-09 pengl
	_REALDATA_CONDITION()
	{
		IsUse = false;
		station_id = "";
		pt_id = "";
		cpu_code = "";
		breaker_id = "";
		reverse1 = "";
		reverse2 = "";
	}

	/** @brief             �Ƿ����station_id,pt_id,cpu_code��������ѯ*/
	bool      IsUse;

	/** @brief             ��վID*/
	string station_id;

	/** @brief             ����ID*/
	string pt_id;

	/** @brief             CPU��*/
	string cpu_code;

	/** @brief             ���ر��*/
	string breaker_id;//���ҿ�����һ���豸����ʱ��

	/** @brief             ����*/
	string reverse1;

	/** @brief             ����*/
	string reverse2;

}REALDATA_CONDITION;


/**
 * @brief       ���ڴ洢�󳤶�����ʱ�õ����ֶ�ֵ�ṹ
 * @author      ����
 * @version     ver1.0
 * @date        14/11/2009
 *
 */
//�ֶνṹ
typedef struct _BLOB_FIELD
{
	//add constructors for default value
	//2014-10-09 pengl
	_BLOB_FIELD()
	{
		nBlobFieldType = -1;
		strBlobFieldName = "";
		strBlobFieldValue = "";
	}

	int    nBlobFieldType;       //�ֶ�����
	string strBlobFieldName;     //�ֶ���
	string strBlobFieldValue;    //�ֶ�ֵ
}BLOB_FIELD;

/**
 * @brief       ���ڴ洢�󳤶�����ʱ�õ��������ṹ
 * @author      ����
 * @version     ver1.0
 * @date        14/11/2009
 *
 */
//�����ṹ
typedef struct _BLOB_CONDITION
{
	//add constructors for default value
	//2014-10-09 pengl
	_BLOB_CONDITION()
	{
		nBlobConditionType = -1;
		strBlobConditionContent = "";
	}

	UINT   nBlobConditionType;       //�Ƿ�Ϊ�����ֶ� Ϊ0Ϊwhere Ϊ1Ϊorder����group
	string strBlobConditionContent; //��������
}BLOB_CONDITION;

/**
 * @brief       ���ڴ洢�󳤶�����ʱ�õ���SQL�ṹ
 * @author      ����
 * @version     ver1.0
 * @date        14/11/2009
 *
 */
//SQL���ṹ
typedef struct BLOB_SQL_DATA
{
	vector <BLOB_FIELD>     BlobFieldlist;       //�ֶ�����
	vector <BLOB_CONDITION> Blob_Conditionlist;  //��������
}BLOB_DATA;

/**	\brief	The _MutiSQL_DATA struct ֱ��ִ��SQL���ṹ�������ڴ󳤶�����SQL�ṹ
  * @author      ����
  * @version     ver1.0
  * @date        23/11/2009
  *
*/
typedef struct BLOB_MutiSQL_DATA
{
	//add constructors for default value
	//2014-10-09 pengl
	BLOB_MutiSQL_DATA()
	{
		nFuntype = -1;
		strSqlBodyContent = "";
	}

	int     nFuntype;//�������ͱ�ʶΪ��ѯ(��Ҫ���ؽ������),��������ֻ����ִ�н��(ִ�гɹ�ʧ��)
	string  strSqlBodyContent;//sql�������
}BLOB_MUTISQL_DATA;

/*  ȫ�ֺ�������  */
extern void GetSysTime(SYSTIME & curTime);
extern void CreateSavePath(char * szPath);
extern void GetIniKey(const char *filename, 
						   const char *title,
				           const char *key,
						   char *chResult);
extern void EndThread();
extern void MySleep(unsigned int nmilliseconds);
extern void GetFileStatusInDir(char *dir, vector <FILE_STATUS_STRUCT> *pFileStatusList,int nLevel);
extern int  GetFileStatus(char *filename, FILE_STATUS_STRUCT *pFileStatusStruct);
extern string GetFormatPath(string sourcePath);
extern string GetFileNameFromFullFileName(string sFullFileName);
extern string GetFilePathFromFullFileName(string sFullFileName);
extern bool PingDestination(const string pIp);
extern WORD ReverseWordEndian(WORD pSourse);
extern int ReverseIntEndian(int pSourse);
extern float ReverseFloatEndian(float pSourse);
extern string FormatGatewayList(string pGatewayA, string pGatewayB);
extern void GetGatewayFromList(string pGatewayList, string& pGatewayA, string& pGatewayB);

		/**
		* @brief	GetFileExFileName	ȡ��.�ָ����ļ���չ��
		* @param 	[In]const string& pSrcFileName	�ļ���
		* @param 	[In]string& strExtName	���ص���չ��
		* @return	bool	true:�ɹ� false:ʧ��
		* @notes	��
		* @sample	��
		*/
extern 	bool GetFileExFileName(const string& pSrcFileName,string& strExtName);

#ifndef MFC_ADO_USED_DEFINE

extern void AddField(SQL_DATA& sql, const char* pFieldName, int nType, const char* pFieldValue = NULL);
extern void AddCondition(SQL_DATA& sql, const char* pContent, int nType = 0);
extern void Zero_SQL_DATA(SQL_DATA& pSqlData);

#endif

extern void Zero_RealData_Condition(REALDATA_CONDITION& pRealDataCondition);
#endif

