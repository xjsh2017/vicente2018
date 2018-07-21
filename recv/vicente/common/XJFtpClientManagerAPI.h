/********************************************************************
	created:	2015/07/09
	created:	2015/07/09 16:40
	filename: 	D:\Vicente\Code\common\XJFtpClientManagerAPI.h
	file path:	D:\Vicente\Code\common
	file base:	XJFtpClientManagerAPI
	file ext:	h
	author:		pengl
	purpose:	XJFtpClientManager�ӿڼ��ṹ�ع���
*********************************************************************/
#ifndef XJFtpClientManagerAPI_h__
#define XJFtpClientManagerAPI_h__

#include "define.h"

/**
 * @defgroup �ļ�����
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
 *  @brief   	PFUN_CONNETFTPSERVER	 ����FTP����� 
 *  @param 		[In]a param of Type  const char*  FTP�����IP
 *  @param 		[In]a param of Type  const char*  ����FTP���û���
 *  @param 		[In]a param of Type  const char*  ����FTP������
 *  @return 	XJHANDLE ������ȷ�󷵻صľ��
 */
typedef XJHANDLE (*PFUN_CONNETFTPSERVER)(const char*, const char*, const char*);

/*
 *  @brief   	PFUN_CLOSEFTPCONNECT	 �ر���FTP����˵����� 
 *  @param 		[In]a param of Type  XJHANDLE  ���Ӿ��
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_CLOSEFTPCONNECT)(XJHANDLE);

/*
 *  @brief   	PFUN_DIRFTPSERVERFILE	 �г�ftp�����ָ��Ŀ¼���ļ��б� 
 *  @param 		[In]a param of Type  XJHANDLE  ���Ӿ��
 *  @param 		[In]a param of Type  const char*  ftp��·��
 *  @param 		[In]a param of Type  vector<XJ_FTP_DIR_STRUCT>&  ��·���µ��ļ��б�
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_DIRFTPSERVERFILE)(XJHANDLE, const char*, vector<XJ_FTP_DIR_STRUCT>&);

/*
 *  @brief   	PFUN_GETFTPSERVERFILE	 ��ȡԶ��ָ���ļ������ش洢 
 *  @param 		[In]a param of Type  XJHANDLE  ���Ӿ��
 *  @param 		[In]a param of Type  const char*  Զ���ļ�·��
 *  @param 		[In]a param of Type  const char*  ���ش洢�ļ�·��
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_GETFTPSERVERFILE)(XJHANDLE, const char*, const char*);

/*
 *  @brief   	PFUN_DELETEFTPSERVERFILE	 ɾ��ftp�������ϵ��ļ� 
 *  @param 		[In]a param of Type  XJHANDLE  ���Ӿ��
 *  @param 		[In]a param of Type  const char*  Զ���ļ�·��
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_DELETEFTPSERVERFILE)(XJHANDLE, const char*);

/*
 *  @brief   	PFUN_PUTFTPSERVERFILE	 �������ļ�����FTP����� 
 *  @param 		[In]a param of Type  XJHANDLE  ���Ӿ��
 *  @param 		[In]a param of Type  const char*  �����ļ�·����
 *  @param 		[In]a param of Type  const char*  Զ���ļ�·����
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_PUTFTPSERVERFILE)(XJHANDLE, const char*, const char*);

/*
 *  @brief   	PFUN_SETLIBLOGPARAM	 ������־���� 
 *  @param 		[In]a param of Type  const char*  ��־·��
 *  @param 		[In]a param of Type  int  ��־����-1�����ã�
 *  @param 		[In]a param of Type  int  ��־����������-1�����ã�
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_SETLIBLOGPARAM)(const char*, int /*= -1*/, int /*= -1*/);

/*
 *  @brief   	GetFtpServerPwd	 ��ȡftp����˸�Ŀ¼ 
 *  @param 		[In]a param of Type  XJHANDLE  ���Ӿ��
 *  @param 		[Out]a param of Type  char*  ftp����˸�Ŀ¼
 *  @param 		[Out]a param of Type  int  ����������
 *  @return 	bool true���ɹ� false��ʧ��
 */
typedef bool (*PFUN_GETFTPSERVERPWD)(XJHANDLE, char*, int);



#endif // XJFtpClientManagerAPI_h__