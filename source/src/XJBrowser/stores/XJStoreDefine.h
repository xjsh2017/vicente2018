#ifndef _XJSTOREDEFINE_H
#define _XJSTOREDEFINE_H

#include "qbytearray.h"

#include <iostream>
#include <fstream>
using namespace std;

#define PT_ID_LEN 20
#define MAX_FIELD_VALE_LEN 256

// 用户组
/** @brief			装置取消挂牌*/
int const XJ_USERGROUP_RUNNER = 101;
/** @brief			装置取消挂牌*/
int const XJ_USERGROUP_OPERATOR = 102;
/** @brief			装置取消挂牌*/
int const XJ_USERGROUP_MONITOR = 103;

// 功能组
/** @brief			装置取消挂牌*/
int const XJ_OPER_UNHANGOUT = 101;
/** @brief			装置挂牌*/
int const XJ_OPER_HANGOUT = 102;

/** @brief			定值修改*/
int const XJ_OPER_PTSET = 201;
/** @brief			定值修改预校*/
int const XJ_OPER_PTSET_PRECORRECTION = 202;
/** @brief			定值修改执行*/
int const XJ_OPER_PTSET_EXECUTE = 203;
/** @brief			定值修改:修改核对*/
int const XJ_OPER_PTSET_STATE_2 = 204;
/** @brief			定值修改:监视*/
int const XJ_OPER_PTSET_STATE_3 = 205;
/** @brief			定值修改:验证*/
int const XJ_OPER_PTSET_STATE_4 = 206;
/** @brief			定值修改:修改完成*/
int const XJ_OPER_PTSET_STATE_5 = 207;

/** @brief			定值区修改*/
int const XJ_OPER_PTZONESET = 301;
/** @brief			定值区修改预校*/
int const XJ_OPER_PTZONESET_PRECORRECTION = 302;
/** @brief			定值区修改执行*/
int const XJ_OPER_PTZONESET_EXECUTE = 303;

/** @brief			软压板修改*/
int const XJ_OPER_PTSOFTSET = 401;
/** @brief			软压板投退预校*/
int const XJ_OPER_PTSOFTSET_PRECORRECTION = 402;
/** @brief			软压板投退执行*/
int const XJ_OPER_PTSOFTSET_EXECUTE = 403;

#define oo(str) .append(str)
#define oon(num) .append(QByteArray::number(num))
#define oc(x) oo(x.GetDelimCol())
#define or(x) oo(x.GetDelimRow())

//字段类型
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
//字段的显示类型
enum FIELD_DISP_TYPE_ENUM
{
	//XJModelingTool中的显示格式
	DBI_DISP_TYPE_CHAR =		 1,
		DBI_DISP_TYPE_UCHAR=         2,
		DBI_DISP_TYPE_SHORT=         3,
		DBI_DISP_TYPE_INT  =         4,
		DBI_DISP_TYPE_LONG =         5,
		DBI_DISP_TYPE_FLOAT=         6,
		DBI_DISP_TYPE_DOUBLE=        7,
		//单菜单类型  列表选项中只能单选
		DBI_DISP_TYPE_SINGLEMENU=    8,
		//多菜单类型  列表选项可以多选(用二进制数表示 第n位为列表中第n选项)
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
