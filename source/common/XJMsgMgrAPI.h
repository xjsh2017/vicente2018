/********************************************************************
 *	created:	2008/09/25
 *	created:	2008:9:25   9:45
 *	filename: 	XJMsgMgrAPI.h
 *	file path:	d:\WorkSpace\Vicente\Code\src\XJCommuServer\CommuServer\XJMsgMgr
 *	file base:	XJMsgMgrAPI
 *	file ext:	hHRESULT
 *	author:		rwp
 *	
 *	purpose:	�������ĵĽӿ��������Ͷ���
*********************************************************************/
#if !defined(AFX_XJMSGMGRAPI_H__1DB0F148_43BE_4896_8B55_440856589BBE__INCLUDED_)
#define AFX_XJMSGMGRAPI_H__1DB0F148_43BE_4896_8B55_440856589BBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MSG_BUFFER_SIZE 0x2000				    //���Ļ��峤��
#define MSG_DES_BUFFER_SIZE (4* MSG_BUFFER_SIZE)//�����������峤��
#define NAME_SIZE 64							//���ֻ��峤��
#define MSG_DIRECT_SEND     0					//���ķ��ͷ���
#define MSG_DIRECT_RECV     1					//���Ľ��շ���
#define MSG_SAVE_FMT_STR     1					//���ı��ı����ַ���ʽ
#define MSG_SAVE_FMT_ASC     2					//���ı��ı�������Ƹ�ʽ
#define MSGMGR_CONFIG_FILE_NAME "xjmsgmgr.ini"  //�����ļ�

#define XJ_STTP_PROTOCL     1					//���STTP��Լ					
#define XJ_HD103_PROTOCL  100					//����103��Լ					
#define XJ_HB103_PROTOCL  101					//����103��Լ
#define XJ_DB103_PROTOCL  102					//����103��Լ
#define XJ_NF103_PROTOCL  103					//�Ϸ�103��Լ
#define XJ_HUB103_PROTOCL 104					//����103��Լ					
#define XJ_LL103_PROTOCL  105					//����103��Լ
#define XJ_ZJ103_PROTOCL  106					//�㽭103��Լ
#define XJ_BJ103_PROTOCL  107                   //����103��Լ
#define XJ_FJ103_PROTOCL  108                   //����103��Լ
#define XJ_JS103_PROTOCL  109                   //����103��Լ
#define XJ_NX103_PROTOCL  110                   //����103��Լ
#define XJ_HN103_PROTOCL  120                   //����103��Լ
#define XJ_SH103_PROTOCL  121 					//�Ϻ�103��Լ

#define XJ_HD104_PROTOCL  200					//����104��Լ					
#define XJ_HB104_PROTOCL  201					//����104��Լ
#define XJ_DB104_PROTOCL  202					//����104��Լ
#define XJ_NF104_PROTOCL  203					//�Ϸ�104��Լ
#define XJ_HUB104_PROTOCL 204					//����104��Լ					
#define XJ_LL104_PROTOCL  205					//����104��Լ
#define XJ_ZJ104_PROTOCL  206					//�㽭104��Լ
#define XJ_BJ104_PROTOCL  207                   //����104��Լ
#define XJ_FJ104_PROTOCL  208                   //����104��Լ
#define XJ_JS104_PROTOCL  209                   //����104��Լ
#define XJ_NX104_PROTOCL  210                   //����104��Լ
#define XJ_HN104_PROTOCL  220                   //����104��Լ
#define XJ_SH104_PROTOCL  221 					//�Ϻ�104��Լ

#define XJ_HD101_PROTOCL  300					//����101��Լ					
#define XJ_SH101_PROTOCL  301					//�Ϻ�101��Լ

/**	\brief	The xjmsgframe struct
�������ĵ��ṩ�������ߵı���֡��ʽ����
*/

typedef struct xjmsgframe {
		int		type;      						//��Լ����
		int		direct;      					//���ķ���0�����գ�1�����ͣ�������unkown(�Ƿ�)
		char	name[NAME_SIZE];				//���ơ����ڱ�ʶͬһIP���ӵ�ͬһ����������,ͬһ�����в�ͬʹ����
		char	from_ip[40];  					//���ͷ�IP���������������Բ�����IP����ģ�������IP�Բ�����ip��
		int		from_port;						//���ͷ��˿ڡ��������������Բ�����port����ģ�������IP�Բ�����port��
		char	to_ip[40];      				//���շ�IP��������������IP����ģ�������IP��ַ��
		int		to_port;						//���շ��˿ڡ��������������˿ڣ���ģ������߶˿ڣ�
		int		reserved;						//��������չ��
		int		save_msg_fmt;					//���ı����ʽ 1���ַ�������: ������
		int		buf_len;						//ʵ�ʱ��ĳ���
		int		des_len;						//ʵ����������
		char	buffer[MSG_BUFFER_SIZE];      	//��������
		char	descript[MSG_DES_BUFFER_SIZE];	//���������������ڱ��Ľ���
} XJMSGFRM;

/**	\brief	The xjmsgmgr_conf struct
�������ĵĻ������ýṹ
*/

typedef struct xjmsgmgr_conf {
	char user[NAME_SIZE];					//ʹ����ģ������
	char listen_ip[40];						//������������IP��Ϊ""ʱ��������������
	int  listen_port;						//�������������˿�
	char log_path[256];						//��־���Ŀ¼
	int  log_level;							//��־����,0:����¼��־���ļ�1:������־2:������־3:������־
	bool bRecordMsg;						//�Ƿ��¼����
	int  reserved;							//��������
}XJMSGMGR_CONF;

#endif // !defined(AFX_XJMSGMGRAPI_H__1DB0F148_43BE_4896_8B55_440856589BBE__INCLUDED_)
