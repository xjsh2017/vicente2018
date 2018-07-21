/********************************************************************
 *	created:	2008/10/01
 *	created:	2008:10:1   17:04
 *	filename: 	XJWebProviderAPI.h
 *	file path:	D:\WorkSpace\Vicente\Code\src\XJCommuServer\CommuServer\XJWebProvider
 *	file base:	XJWebProviderAPI
 *	file ext:	h
 *	author:		rwp
 *	
 *	purpose:	�����ṩ�Ľӿڲ����ļ�
*********************************************************************/

#if !defined(AFX_XJWEBPROVIDERAPI_H__C20E2843_173D_4607_B56E_88C9C7A22F91__INCLUDED_)
#define AFX_XJWEBPROVIDERAPI_H__C20E2843_173D_4607_B56E_88C9C7A22F91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define NAME_SIZE 64								 //���ֻ��峤��
#define WEBPRVDER_CONFIG_FILE_NAME "xjwebprovider.ini"  //�����ļ�

/**	\brief	The xjmsgmgr_conf struct
WEB�����ṩ�ߵĻ������ýṹ
*/
typedef struct xjwebprovider_conf {
	char user[NAME_SIZE];					//ʹ����ģ������
	char websrv_ip[40];						//WEB��������IP
	int  websrv_port;						//WEB�������˿�
	char log_path[256];						//��־���Ŀ¼
	int log_level;							//��־����,0:����¼��־���ļ�1:������־2:������־3:������־
	int log_day;							//��־��������
	bool bRecordMsg;						//�Ƿ��¼����
 	bool bRecvACK;							//�Ƿ���շ����ȷ��
	int iRecvTimeOut;						//����ӿ��ճ�ʱ(��)
	int iSendTimeOut;						//����ӿڷ���ʱ(��)
	int  ireserved;							//��������
 	float  freserved;						//��������
}XJWEBPROVIDER_CONF;


#endif // !defined(AFX_XJWEBPROVIDERAPI_H__C20E2843_173D_4607_B56E_88C9C7A22F91__INCLUDED_)
