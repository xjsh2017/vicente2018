#ifndef _XJSTOREDEFINE_H
#define _XJSTOREDEFINE_H

#include "qbytearray.h"

#include <iostream>
#include <fstream>
using namespace std;

#define PT_ID_LEN 20
#define MAX_FIELD_VALE_LEN 256

// �û���
/** @brief			������*/
int const XJ_USERGROUP_RUNNER = 101;
/** @brief			������*/
int const XJ_USERGROUP_OPERATOR = 102;
/** @brief			������*/
int const XJ_USERGROUP_MONITOR = 103;

/** @brief			����ԭ��: δ����*/
int const XJ_TAGOUT_UNDEFINE = 100;
/** @brief			����ԭ��: ��ֵ�޸�*/
int const XJ_TAGOUT_PTVALVSET = 200;
/** @brief			����ԭ��: ��ֵ���л�*/
int const XJ_TAGOUT_PTZONESET = 300;
/** @brief			��ѹ��Ͷ��*/
int const XJ_TAGOUT_PTSOFTSET = 400;

// ������
/** @brief			װ��ȡ������*/
int const XJ_OPER_UNHANGOUT = 101;
/** @brief			װ�ù���*/
int const XJ_OPER_HANGOUT = 102;

/** @brief			��ֵ�޸�ԤУ*/
int const XJ_OPER_PTVALVSET_PRECORRECTION = 202;
/** @brief			��ֵ�޸�ִ��*/
int const XJ_OPER_PTVALVSET_EXECUTE = 203;
/** @brief			��ֵ�޸�:�޸ĺ˶�*/
int const XJ_OPER_PTVALVSET_STATE_2 = 212;
/** @brief			��ֵ�޸�:����*/
int const XJ_OPER_PTVALVSET_STATE_3 = 213;
/** @brief			��ֵ�޸�:��֤*/
int const XJ_OPER_PTVALVSET_STATE_4 = 214;
/** @brief			��ֵ�޸�:ִ�����*/
int const XJ_OPER_PTVALVSET_STATE_5 = 215;

/** @brief			��ֵ���л�ԤУ*/
int const XJ_OPER_PTZONESET_PRECORRECTION = 302;
/** @brief			��ֵ���л�ִ��*/
int const XJ_OPER_PTZONESET_EXECUTE = 303;
/** @brief			��ֵ���л�:�޸ĺ˶�*/
int const XJ_OPER_PTZONESET_STATE_2 = 312;
/** @brief			��ֵ���л�:����*/
int const XJ_OPER_PTZONESET_STATE_3 = 313;
/** @brief			��ֵ���л�:��֤*/
int const XJ_OPER_PTZONESET_STATE_4 = 314;
/** @brief			��ֵ���л�:ִ�����*/
int const XJ_OPER_PTZONESET_STATE_5 = 315;

/** @brief			��ѹ��Ͷ��ԤУ*/
int const XJ_OPER_PTSOFTSET_PRECORRECTION = 402;
/** @brief			��ѹ��Ͷ��ִ��*/
int const XJ_OPER_PTSOFTSET_EXECUTE = 403;
/** @brief			��ѹ��Ͷ��:�޸ĺ˶�*/
int const XJ_OPER_PTSOFTSET_STATE_2 = 412;
/** @brief			��ѹ��Ͷ��:����*/
int const XJ_OPER_PTSOFTSET_STATE_3 = 413;
/** @brief			��ѹ��Ͷ��:��֤*/
int const XJ_OPER_PTSOFTSET_STATE_4 = 414;
/** @brief			��ѹ��Ͷ��:ִ�����*/
int const XJ_OPER_PTSOFTSET_STATE_5 = 415;

//�ֶ�����
enum FIELD_TYPE_ENUM
{
	DBI_FIELD_TYPE_CHAR = 1,
	DBI_FIELD_TYPE_VARCHAR = 2,
	DBI_FIELD_TYPE_TINYINT = 3,
	DBI_FIELD_TYPE_SMALLINT = 4,
	DBI_FIELD_TYPE_INT = 5,
	DBI_FIELD_TYPE_REAL = 6,
	DBI_FIELD_TYPE_FLOAT = 7,
	DBI_FIELD_TYPE_BIT = 8,
	DBI_FIELD_TYPE_DATETIME = 9,
	DBI_FIELD_TYPE_BINARY = 10
};
//�ֶε���ʾ����
enum FIELD_DISP_TYPE_ENUM
{
	//XJModelingTool�е���ʾ��ʽ
	DBI_DISP_TYPE_CHAR =		 1,
		DBI_DISP_TYPE_UCHAR=         2,
		DBI_DISP_TYPE_SHORT=         3,
		DBI_DISP_TYPE_INT  =         4,
		DBI_DISP_TYPE_LONG =         5,
		DBI_DISP_TYPE_FLOAT=         6,
		DBI_DISP_TYPE_DOUBLE=        7,
		//���˵�����  �б�ѡ����ֻ�ܵ�ѡ
		DBI_DISP_TYPE_SINGLEMENU=    8,
		//��˵�����  �б�ѡ����Զ�ѡ(�ö���������ʾ ��nλΪ�б��е�nѡ��)
		DBI_DISP_TYPE_MUTIMENU=      9,		
		DBI_DISP_TYPE_STRING  =   	 10,
		DBI_DISP_TYPE_DATE    =   	 11,
		DBI_DISP_TYPE_TIME    =   	 12,
		DBI_DISP_TYPE_DATETIME=      13,
		DBI_DISP_TYPE_FORMULASTRING= 14,
		DBI_DISP_TYPE_BINARY  =      15
};


extern QByteArray GetTime(int nType = 0);

#endif // !defined(_XJSTOREDEFINE_H)
