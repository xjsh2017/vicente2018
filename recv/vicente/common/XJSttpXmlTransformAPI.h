/********************************************************************
	created:	2015/07/20
	created:	2015/07/20 10:44
	filename: 	D:\Vicente\Code\common\XJSttpXmlTransformAPI.h
	file path:	D:\Vicente\Code\common
	file base:	XJSttpXmlTransformAPI
	file ext:	h
	author:		pengl
	purpose:	
*********************************************************************/
#ifndef XJSttpXmlTransformAPI_h__
#define XJSttpXmlTransformAPI_h__

#include "define.h"
#include "XJSttpNetAPI.h"

/** @brief            mysql���ݿ�*/
const int XJ_STTP_XML_DB_TYPE_MYSQL = 1;
/** @brief            oracle���ݿ�*/
const int XJ_STTP_XML_DB_TYPE_ORACLE = 2;
/** @brief            sqlserver���ݿ�*/
const int XJ_STTP_XML_DB_TYPE_MS_SQL = 3;
/** @brief            �������ݿ�*/
const int XJ_STTP_XML_DB_TYPE_DAMENG = 4;
/** @brief            ������ݿ�*/
const int XJ_STTP_XML_DB_TYPE_KINGBASE = 5;

/**	\brief	The _XJ_TRANSFORM_DB_PARAM struct
���ݿ����
*/
typedef struct _XJ_TRANSFORM_DB_PARAM
{
	/**	\brief ���ݿ����� 1:mysql 2:oracle 3:sqlserver 4:���� 5:���*/
	int nDbType;
	
	/**	\brief ���ݿ�˿ں�*/
	int nDbPort;
	
	/**	\brief ���ݿ�����IP*/
	char chDbMainHost[30];
	
	/**	\brief ���ݿⱸ��IP*/
	char chDbSlaveHost[30];
	
	/**	\brief ���ݿ���*/
	char chDbName[50];
	
	/**	\brief ���ݿ��û���*/
	char chDbUser[50];
	
	/**	\brief ���ݿ�����*/
	char chDbPasswd[50];
	
	_XJ_TRANSFORM_DB_PARAM()
	{
		bzero(chDbMainHost, sizeof(chDbMainHost));
		bzero(chDbSlaveHost, sizeof(chDbSlaveHost));
		bzero(chDbName, sizeof(chDbName));
		bzero(chDbUser, sizeof(chDbUser));
		bzero(chDbPasswd, sizeof(chDbPasswd));
		
		nDbType = XJ_STTP_XML_DB_TYPE_ORACLE;
		nDbPort = -1;
		sprintf(chDbMainHost, "%s", "127.0.0.1");
		sprintf(chDbName, "%s", "ipofas");
		sprintf(chDbUser, "%s", "ipofas");
		sprintf(chDbPasswd, "%s", "sagacity");
	}
	
}XJ_TRANSFORM_DB_PARAM;

/**	\brief	The _XJ_CALL_CMD_PARAM_STRU struct
�����е���Ϣ
*/
typedef struct _XJ_CALL_CMD_PARAM_STRU
{
	/**	\brief ������*/
	int nRii;
	
	/**	\brief cpu��*/
	int nCpuCode;

	/**	\brief 103���*/
	int nGroupNum;
	
	/**	\brief 103��Ŀ��*/
	int nItemNum;
	
	/**	\brief ��ʼʱ�� yymmddHHMMSS*/
	char chStartTime[50];
	
	/**	\brief ��ʼʱ�����*/
	char chStartMs[25];
	
	/**	\brief ����ʱ�� yymmddHHMMSS*/
	char chEndTime[50];
	
	/**	\brief ����ʱ�����*/
	char chEndMs[25];
	
	/**	\brief ����յ���ʱ�� yymmddHHMMSS*/
	char chRecvTime[50];
	
	/**	\brief ����յ���ʱ�����*/
	char chRecvMs[25];

	/**	\brief ���ص��ļ���*/
	char chOscFileName[MAX_LINE_LENGTH];
	
	_XJ_CALL_CMD_PARAM_STRU()
	{
		nRii = -1;
		nGroupNum = -1;
		nItemNum = -1;
		nCpuCode = 0;
		bzero(chStartTime, sizeof(chStartTime));
		bzero(chStartMs, sizeof(chStartMs));
		bzero(chEndTime, sizeof(chEndTime));
		bzero(chEndMs, sizeof(chEndMs));
		bzero(chRecvTime, sizeof(chRecvTime));
		bzero(chRecvMs, sizeof(chRecvMs));
		bzero(chOscFileName, sizeof(chOscFileName));
	}
}XJ_CALL_CMD_PARAM_STRU;

/*
 *  @brief   	PFUN_INITTRANSFORMLIB	 ��ʼ����̬�⣬���������һ�� 
 *  @param 		[In]a param of Type  char*  ������Ϣ
 *  @param 		[In]a param of Type  XJ_TRANSFORM_DB_PARAM*  ���ݿ�����������߲��������ȡ�����ļ���Ĭ��ֵΪNULL
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_INITTRANSFORMLIB)(char* /*= NULL*/, XJ_TRANSFORM_DB_PARAM* /*= NULL*/);

/*
 *  @brief   	PFUN_ENDTRANSFORMLIB	 �˳����ͷ���Դ 
 *  @param 		[Out]a param of Type  char*  ������Ϣ
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_ENDTRANSFORMLIB)(char* /*= NULL*/);

/*
 *  @brief   	PFUN_CONVERTMODELINFOTOXML	 ת��ģ�����ݵ�xml�ļ� 
 *  @param 		[In]a param of Type  char*  ת�����xml�ļ����·���������255����
 *  @param 		[In]a param of Type  const char*  ��վID
 *  @param 		[Out]a param of Type  char*  ������Ϣ
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_CONVERTMODELINFOTOXML)(char*, const char*, char* /*= NULL*/);

/*
 *  @brief   	PFUN_CONVERTSTTPTOXML	 ��sttp����ת��ΪXML�ļ� 
 *  @param 		[Out]a param of Type  char*  ת�����xml�ļ����·���������255����
 *  @param 		[In]a param of Type  vector<STTP_FULL_DATA>&  sttp���Ķ��У����Ķ����еı���Ϊͬһ����ͬһ���ͱ���
 *  @param 		[Out]a param of Type  char*  ������Ϣ
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_CONVERTSTTPTOXML)(char*, XJ_CALL_CMD_PARAM_STRU&, vector<STTP_FULL_DATA>&, char* /*= NULL*/);

/*
 *  @brief   	PFUN_CONVERTXMLTOSTTP	 ��xml�ļ�ת��ΪSTTP���� 
 *  @param 		[In]a param of Type  const char*  xml�ļ�·��
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ת����ı���
 *  @param 		[Out]a param of Type  char*  ������Ϣ
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_CONVERTXMLTOSTTP)(const char*, STTP_FULL_DATA& sttpCmd, char* /*= NULL*/);

/*
 *  @brief   	PFUN_SETLOGPARAM	 ������־������������־ 
 *  @param 		[In]a param of Type  const char*  ��־·��
 *  @param 		[In]a param of Type  int  ��־����
 *  @param 		[In]a param of Type  int  ��־��������
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_SETLOGPARAM)(const char*, int, int /*= 5*/);



#endif // XJSttpXmlTransformAPI_h__