#ifndef _XJSTOREDEFINE_H
#define _XJSTOREDEFINE_H

#include "qbytearray.h"

#include <iostream>
#include <fstream>
using namespace std;

#define PT_ID_LEN 20
#define MAX_FIELD_VALE_LEN 256

// �û���
/** @brief			װ��ȡ������*/
int const XJ_USERGROUP_RUNNER = 101;
/** @brief			װ��ȡ������*/
int const XJ_USERGROUP_OPERATOR = 102;
/** @brief			װ��ȡ������*/
int const XJ_USERGROUP_MONITOR = 103;

// ������
/** @brief			װ��ȡ������*/
int const XJ_OPER_UNHANGOUT = 101;
/** @brief			װ�ù���*/
int const XJ_OPER_HANGOUT = 102;

/** @brief			��ֵ�޸�*/
int const XJ_OPER_PTSET = 201;
/** @brief			��ֵ�޸�ԤУ*/
int const XJ_OPER_PTSET_PRECORRECTION = 202;
/** @brief			��ֵ�޸�ִ��*/
int const XJ_OPER_PTSET_EXECUTE = 203;
/** @brief			��ֵ�޸�:�޸ĺ˶�*/
int const XJ_OPER_PTSET_STATE_2 = 204;
/** @brief			��ֵ�޸�:����*/
int const XJ_OPER_PTSET_STATE_3 = 205;
/** @brief			��ֵ�޸�:��֤*/
int const XJ_OPER_PTSET_STATE_4 = 206;
/** @brief			��ֵ�޸�:�޸����*/
int const XJ_OPER_PTSET_STATE_5 = 207;

/** @brief			��ֵ���޸�*/
int const XJ_OPER_PTZONESET = 301;
/** @brief			��ֵ���޸�ԤУ*/
int const XJ_OPER_PTZONESET_PRECORRECTION = 302;
/** @brief			��ֵ���޸�ִ��*/
int const XJ_OPER_PTZONESET_EXECUTE = 303;

/** @brief			��ѹ���޸�*/
int const XJ_OPER_PTSOFTSET = 401;
/** @brief			��ѹ��Ͷ��ԤУ*/
int const XJ_OPER_PTSOFTSET_PRECORRECTION = 402;
/** @brief			��ѹ��Ͷ��ִ��*/
int const XJ_OPER_PTSOFTSET_EXECUTE = 403;

#define oo(str) .append(str)
#define oon(num) .append(QByteArray::number(num))
#define oc(x) oo(x.GetDelimCol())
#define or(x) oo(x.GetDelimRow())

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
