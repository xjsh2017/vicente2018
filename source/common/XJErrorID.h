/********************************************************************
	created:	2009/02/20
	created:	20:2:2009   10:48
	file base:	XJErrorID
	file ext:	h
	author:		qingch
	
	purpose:	ϵͳ�ڳ�����Ϣ��ͳһ�����봦��
*********************************************************************/

#ifndef XJErrorID_h__
#define XJErrorID_h__

//////////////////////////////////////////////////////////////////////////
//                   �ڲ�����Ŷ���                                   
//////////////////////////////////////////////////////////////////////////

/**                    �Զ�������׼��*/ 
#define  MYDEF_ERROR_START_NO     0xFFFF


/** @brief			   �Զ�����������*/
#define  MYDEF_ERROR_END_NO		  (MYDEF_ERROR_START_NO + 0x2FFF)


/** @brief	      ͨ�ô���������׼��*/
#define  COM_ERROR_START_NO	 (MYDEF_ERROR_START_NO + 0x0000)

/** @brief	      ͨ�ô�������������*/
#define  COM_ERROR_END_NO	 (COM_ERROR_START_NO + 0x0FFF)

/** @brief             �ڴ����ʧ��*/
#define  R_NEW_MEM_FAILED         (COM_ERROR_START_NO+1)

/** @brief             ��̬�����ʧ��*/
#define  R_LOAD_LIB_FAILED        (COM_ERROR_START_NO+2)

/** @brief             ��ú�����ַʧ��*/
#define  R_GET_FUNADDR_FAILED     (COM_ERROR_START_NO+3)

/** @brief             �������쳣*/
#define  R_RUN_EXCEPTION          (COM_ERROR_START_NO+4)

/** @brief             ��Ч��ַ��ָ��*/
#define R_INVALID_PARAMETER       (COM_ERROR_START_NO+5)

/** @brief             �����߳�ʧ��*/
#define R_CREATE_THREAD_FAILED    (COM_ERROR_START_NO+6)

/** @brief             �����߳�ʧ��*/
#define R_END_THREAD_FAILED       (COM_ERROR_START_NO+7)

/** @brief             ��½��֤ʧ��*/
#define R_VERIFY_FAILED           (COM_ERROR_START_NO+8)

/** @brief             ����ת����û���ҵ�ת������*/
#define STTP_NOT_FIND_FUNC        (COM_ERROR_START_NO+9)

/** @brief             ����ת���౨��ת��������ʧ��*/
#define STTP_TO_DATA_FAILED       (COM_ERROR_START_NO+10)

/** @brief             ����ת��������ת���ɱ���ʧ��*/
#define DATA_TO_STTP_FAILED       (COM_ERROR_START_NO+11)


/** @brief	      ҵ�����̴����Ż�׼��*/
#define  BUSINESS_ERROR_START_NO		(MYDEF_ERROR_START_NO + 0x1000)

/** @brief	      ҵ�����̴����Ž�����*/
#define  BUSINESS_ERROR_END_NO			(BUSINESS_ERROR_START_NO + 0x0FFF)

/** @brief             ��վ����վͨѶ�ж�*/
#define  B_SUB_DISCONTINUE				(BUSINESS_ERROR_START_NO+1)

/** @brief             ����վ���ڳ�ʼ������*/
#define  B_SUB_INITIALIZING				(BUSINESS_ERROR_START_NO+2)

/** @brief             ��վ����Ч����*/
#define  B_SUB_INVALID_COMMAND			(BUSINESS_ERROR_START_NO+3)

/** @brief             ��վ�ظ�ʧ��*/
#define  B_SUB_REPLY_FAILED				(BUSINESS_ERROR_START_NO+4)

/** @brief             ��վ����Ч�ظ�*/
#define  B_SUB_INVALID_REPLY			(BUSINESS_ERROR_START_NO+5)

/** @brief             ��վ�ظ���ʱ*/
#define  B_SUB_REPLY_TIMEOUT			(BUSINESS_ERROR_START_NO+6)

/** @brief             ��վ���ݴ���ʧ��*/
#define  B_SUB_DATAHANDLE_FAILED		(BUSINESS_ERROR_START_NO+7)

/** @brief             �����������쳣*/
#define  B_SUB_SC_LINK_ERROR			(BUSINESS_ERROR_START_NO+8)

/** @brief              ��Ч����������*/
#define  B_STTPNET_INVALID_COMMAND      (BUSINESS_ERROR_START_NO+9)

/** @brief              Sttp�ͻ��˻�������*/
#define  B_STTPNET_BUFFER_OVER       	(BUSINESS_ERROR_START_NO+10)

/** @brief              Sttp�ͻ��������������ж�*/
#define  B_STTPNET_DISCONTINUE      	(BUSINESS_ERROR_START_NO+11)

/** @brief              �ͻ��˽��ճ�ʱ*/
#define  B_CLIENT_RECV_TIMEOUT      	(BUSINESS_ERROR_START_NO+12)

/** @brief              ��������ʧ��*/
#define  B_POST_CMD_FAILED      		(BUSINESS_ERROR_START_NO+13)

/** @brief	      ���������Ż�׼��*/
#define  OTHER_ERROR_START_NO	 (MYDEF_ERROR_START_NO + 0x2000)

/** @brief	      ���������Ž�����*/
#define  OTHER_ERROR_END_NO		 (OTHER_ERROR_START_NO + 0x0FFF)

extern char* CommErrMsg[];

extern char* BusinessErrMsg[];

extern char* OtherErrMsg[];

extern char* getErrorMsg(int pErrorNo);

#endif // XJErrorID_h__