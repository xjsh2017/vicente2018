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
	/** @brief	�����д������ݵ�����(0-sttp,1-��־��Ϣ,2-103/104,3-����ȫ���ݽ���(��װ�����ж�ֵ�����ȫ����),4-������Լ)*/
	BYTE		bData_type;
	/** @brief	����1������2�����棨���ɣ���3����������, �������� */
	BYTE		bLevel;  
	/** @brief	ģ����*/
	char		chModel_name[40];	
	/** @brief	ԴIP (����Ǵ���ͨ�ţ�����COM1��COM2��)*/
	char		chSource_ip[40];
	/** @brief	Դ�˿�(����Ǵ���ͨ�ţ�����װ�õ�ַ�����߷�ʽ���ж���װ��)*/
	WORD		wSource_port;
	/** @brief	Ŀ��IP*/
	char		chDist_ip[40];
	/** @brief	Ŀ��˿�*/
	WORD		wDist_port;
	/** @brief	�����ģ�鱾��ı��ķ��� 0-���� 1-���� ��������*/
	BYTE		bWay;
	/** @brief	˫��ͨ�ŷ�ʽ��1��tcp 2��com(����)  3��udp 4��modem*/
	BYTE		bCommuMode;
	/** @brief	���ա����ͱ��ĵ�ʱ�̣���ʽΪ��yyyy-mm-dd HH:MM:SS.3λ����*/
	char		chTime[25];
	/** @brief	��������չ��*/
	DWORD		dwReserve1;
	/** @brief	��������չ��*/
	char		chReserve2[50];
	/** @brief	��������*/
	vector<char> vMsg;
	/** @brief	��������:������־��Ϣ���Ľ���
				�������·���׳��Ľ���Ϊ��Ҫ���ͣ��϶̣�
				�����Ӧ�ò��׳��Ľ���Ϊ��ϸ���ͣ��ϳ���
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
	/** @brief	�����д������ݵ����� 0-ԭʼ��������� 1-��·������ʾ 2-��·���̸澯 3-Ӧ����Ϣ��� 4-Ӧ�ù��̸澯*/
	BYTE		bData_type;
	/** @brief	��վID*/
	char		chStationID[13];
	/** @brief	Ŀ��IP*/
	char		chDist_ip[40];
	/** @brief	Ŀ��˿�*/
	WORD		wDist_port;
	/** @brief	�����ģ�鱾��ı��ķ��� 0-���� 1-���� ��������*/
	BYTE		bWay;
	/** @brief	���ա����ͱ��ĵ�ʱ�̣���ʽΪ��yyyy-mm-dd HH:MM:SS.3λ����*/
	char		chTime[25];
	/** @brief	���ı�� ��������1~4������������0�ı��Ĺ���*/
	DWORD		dwMsgSerialNO;
	/** @brief	��������չ��*/
	BYTE		bReserve1;
	/** @brief	��������չ��*/
	BYTE		bReserve2;
	/** @brief	��������չ��*/
	WORD		wReserve3;
	/** @brief	��������չ��*/
	char		chReserve4[50];
	/** @brief	�������� ������Ϊ0ʱ������ ������ʱ��Ϊ��*/
	vector<char> vMsg;
	/** @brief	��������:���ڱ��Ľ�������ʾ��Ϣ*/
	vector<char> vDesc;
} XJ103FRAMESTRUCT;

#endif // XJMsgMonitorAPI_h__