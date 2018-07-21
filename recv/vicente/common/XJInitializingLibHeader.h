#ifndef XJInitializingLibHeader_h__
#define XJInitializingLibHeader_h__
//���θ澯��Ϣ
#pragma   warning   (disable   :   4786)
#pragma   warning   (disable   :   4275)

//���ù���ͷ�ļ�
#include "define.h"          //��׼��Ķ���
#include "SttpDefine.h"      //STTPNET����

//����ļ�����
const int MAX_FILE_LENGTH = 255;

/**
 * @brief       ���ݿ���ʻ�������
 * @author      ����
 * @version     ver1.0
 * @date        26/08/2010
 *
 * example
 * @code
 *
 *
 *
 * @endcode
 */
typedef struct _XJ_DATABASE_PARAM
{
/** @brief             �Ƿ�ʹ������ƽ̨�������ݿ�*/
	bool bUseDbServer;

/** @brief             ���ݿ�����1:SQL SERVER 2:ORACLE 3:MYSQL*/
	int  iDataBaseType;

/** @brief             ���ݿ�����˿�*/
	int  iDataBasePort;

/** @brief             ���ݿ����IP��ַ*/
	char chDataBaseIp[MAX_FILE_LENGTH];

/** @brief             ���ݿ�ʵ����*/
	char chDataBaseName[MAX_FILE_LENGTH];

/** @brief             ���ݿ�����û���*/
	char chUserName[MAX_FILE_LENGTH];

/** @brief             ���ݿ��������*/
	char chPassword[MAX_FILE_LENGTH];
}XJ_DATABASE_PARAM;

//���ݿ����Ͷ���
//SQL SERVER���ݿ�
const int _DATABASE_SQL_SERVER = 1;
//ORACLE���ݿ�
const int _DATABASE_ORACLE     = 2;
//MYSQL���ݿ�
const int _DATABASE_MYSQL      = 3;

//��������
//δ֪����
const int XJERROR_UNKNOWN_ERROR              =  0;
//����ͨѶ������ʧ��
const int XJERROR_CONNECT_XJCOMMUSERVER_FAIL = -1;
//��������ƽ̨ʧ��
const int XJERROR_CONNECT_XJDBSERVER_FAIL    = -2;
//�ɹ�
const int XJERROR_OPERATION_SUCCESS          =  1;

//�������ݿ�����
//ֱ�ӷ������ݿ�
const int XJACCESS_DATABASE                 = 0;
//ͨ������ƽ̨�������ݿ�
const int XJACCESS_DBSERVER                 = 1;

//��ʼ����վʱ�õ�����Ϣ����
const int STATION_INIT_ALL     = 0; //����վ����Ϣ
const int STATION_INIT_LINE    = 1; //��·
const int STATION_INIT_SECDEV  = 2; //�����豸
const int STATION_INIT_GROUP   = 3; //�����
const int STATION_INIT_BREAKER = 4; //����
const int STATION_INIT_BUS     = 5; //ĸ��
const int STATION_INIT_TRANS   = 6; //��ѹ��
const int STATION_INIT_GEN     = 7; //�����
const int STATION_INIT_REACTOR = 8; //�翹��
const int STATION_INIT_CAP     = 9; //������
const int STATION_INIT_SWITCH  = 10;//��բ

//��ʼ����վ��ͬ������
const int STATION_DIRECTIOR_DOWN = 0; //��վ��װ��Ϣ����վ
const int STATION_DIRECTIOR_GET  = 1; //��վ����վ������Ϣ

//��ʼ���豸ʱ�õ�����Ϣ����
const int DEVICE_INIT_ALL       = 0; //���豸����������
const int DEVICE_INIT_SETTING   = 1; //��ֵ
const int DEVICE_INIT_AI        = 2; //ģ����
const int DEVICE_INIT_SOFTBOARD = 3; //��ѹ��
const int DEVICE_INIT_ZONE      = 4; //��ֵ����
const int DEVICE_INIT_STAUTS    = 5; //״̬��
const int DEVICE_INIT_FAULT     = 6; //������
const int DEVICE_INIT_GROUP     = 7; //�����

//��ʼ���豸����������
const int DEVICE_DES_ALL      = 0; //��ʼ��ȫ��(���������Խṹ)
const int DEVICE_DES_DESCRIBE = 1; //װ������
const int DEVICE_DES_PROPERTY = 2; //װ�����Խṹ

//��ʼ��Ĭ�����(ȫ����,��������)
const int INIT_GROUP_NUM = 256;

//��ʼ��Ĭ��CPU��(ȫ��CPU,������CPU)
const int INIT_CPU_NUM   = 99;

//�ӿڶ���
/*
 *  @brief   	StartLib	 ���б�Ҫ�ĳ�ʼ�����ݹ���
 *  @param 		[In]a param of Type  int ģ����
 *  @param 		[In]a param of Type  int �������ݿ�ķ�ʽ(0��ֱ�ӷ������ݿ� 1��ͨ������ƽ̨�������ݿ�)
 *  @param 		[In]a param of Type  XJ_DATABASE_PARAM& ���ֱ�ӷ������ݿ���Ҫ�����������ݿ�Ĳ���
 *  @return 	int 1:�ɹ� 0:δ֪���� -1:����ͨѶ������ʧ�� -2:�������ݿ�/����ƽ̨ʧ��
 */
typedef	int (*StartLib)(int iModuleID, int iDbConnType, XJ_DATABASE_PARAM& database_param);

/*
 *  @brief   	Close	 �ر����Ӳ��ͷ���Դ
 *  @return 	int 1���ɹ� 0��ʧ��
 */
typedef	int (*EndLib)();

/*
 *  @brief   	SetLogPath	 ������־·��
 *  @param 		[In]a param of Type  const char* �µ���־·��
 *  @return 	int 1���ɹ� 0��ʧ��
 */
typedef	int (*SetLibLogPath)(const char* pLogPath);

/*
 *  @brief   	RecvInitializeMsg	 ����ͨѶ���������صĳ�ʼ��������Ϣ
 *  @param 		[In]a param of Type  STTP_FULL_DATA& ͨѶ���������صĳ�ʼ��������Ϣ
 *  @return 	int 1:�ɹ� 0:������ -1:�����ж�
 */
typedef	int (*RecvInitializeMsg)(STTP_FULL_DATA& sttp_retrun_msg);

/*
 *  @brief   	StationInitialize	 ������վ��ʼ��
 *  @param 		[In]a param of Type  const char* ��վID
 *  @param 		[In]a param of Type  int ��ʼ����Ϣ����
 *  @param 		[In]a param of Type  int ��ʼ��ͬ������
 *  @param 		[In]a param of Type  int ��� Ĭ��Ϊ������
 *  @param 		[In]a param of Type  int CPU�� Ĭ��Ϊ����CPU
 *  @return 	int 1����վ��ʼ������ͳɹ� 0����վ��ʼ�������ʧ��
 */
typedef	int (*StationInitialize)(const char* pStationID, int iInitType, int iInitDirection, int iGroupNum, int iCpuNum);

/*
 *  @brief   	StationInitialize	 ���豸��ʼ��
 *  @param 		[In]a param of Type  const char* �豸ID
 *  @param 		[In]a param of Type  int ��ʼ����Ϣ����
 *  @param 		[In]a param of Type  int ��ʼ����������
 *  @param 		[In]a param of Type  int ��� Ĭ��Ϊ������
 *  @param 		[In]a param of Type  int CPU�� Ĭ��Ϊ����CPU
 *  @return 	int 1���豸��ʼ������ͳɹ� 0���豸��ʼ�������ʧ��
 */
typedef	int (*DeviceInitialize)(const char* pDeviceID, int iInitType, int iInitDes, int iGroupNum, int iCpuNum);

/*
 *  @brief   	RemoveStationInfo	 ɾ����վ�����µ�����
 *  @param 		[In]a param of Type  const char* ��վID
 *  @param 		[In]a param of Type  bool �Ƿ�ɾ����վ�������Ϣ
 *  @return 	int 1���ɹ� 0��ʧ��
 */
typedef int (*RemoveStationInfo)(const char* pStationID, bool bDelStationSelf);

/** @} */ //OVER
#endif // XJInitializingLibHeader_h__

