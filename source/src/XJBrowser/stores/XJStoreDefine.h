#ifndef _XJSTOREDEFINE_H
#define _XJSTOREDEFINE_H

#include "qbytearray.h"

#include <iostream>
#include <fstream>
using namespace std;

#define PT_ID_LEN 20
#define MAX_FIELD_VALE_LEN 256

// 用户组
/** @brief			运行组*/
int const XJ_USERGROUP_RUNNER = 101;
/** @brief			操作组*/
int const XJ_USERGROUP_OPERATOR = 102;
/** @brief			监视组*/
int const XJ_USERGROUP_MONITOR = 103;

/** @brief			挂牌原因: 未定义*/
int const XJ_TAGOUT_UNDEFINE = 100;
/** @brief			挂牌原因: 定值修改*/
int const XJ_TAGOUT_PTVALVSET = 200;
/** @brief			挂牌原因: 定值区切换*/
int const XJ_TAGOUT_PTZONESET = 300;
/** @brief			软压板投退*/
int const XJ_TAGOUT_PTSOFTSET = 400;

// 功能组
/** @brief			装置取消挂牌*/
int const XJ_OPER_UNHANGOUT = 101;
/** @brief			装置挂牌*/
int const XJ_OPER_HANGOUT = 102;

/** @brief			定值修改预校*/
int const XJ_OPER_PTVALVSET_PRECORRECTION = 202;
/** @brief			定值修改执行*/
int const XJ_OPER_PTVALVSET_EXECUTE = 203;
/** @brief			定值修改:修改核对*/
int const XJ_OPER_PTVALVSET_STATE_2 = 212;
/** @brief			定值修改:监视*/
int const XJ_OPER_PTVALVSET_STATE_3 = 213;
/** @brief			定值修改:验证*/
int const XJ_OPER_PTVALVSET_STATE_4 = 214;
/** @brief			定值修改:执行完成*/
int const XJ_OPER_PTVALVSET_STATE_5 = 215;

/** @brief			定值区切换预校*/
int const XJ_OPER_PTZONESET_PRECORRECTION = 302;
/** @brief			定值区切换执行*/
int const XJ_OPER_PTZONESET_EXECUTE = 303;
/** @brief			定值区切换:修改核对*/
int const XJ_OPER_PTZONESET_STATE_2 = 312;
/** @brief			定值区切换:监视*/
int const XJ_OPER_PTZONESET_STATE_3 = 313;
/** @brief			定值区切换:验证*/
int const XJ_OPER_PTZONESET_STATE_4 = 314;
/** @brief			定值区切换:执行完成*/
int const XJ_OPER_PTZONESET_STATE_5 = 315;

/** @brief			软压板投退预校*/
int const XJ_OPER_PTSOFTSET_PRECORRECTION = 402;
/** @brief			软压板投退执行*/
int const XJ_OPER_PTSOFTSET_EXECUTE = 403;
/** @brief			软压板投退:修改核对*/
int const XJ_OPER_PTSOFTSET_STATE_2 = 412;
/** @brief			软压板投退:监视*/
int const XJ_OPER_PTSOFTSET_STATE_3 = 413;
/** @brief			软压板投退:验证*/
int const XJ_OPER_PTSOFTSET_STATE_4 = 414;
/** @brief			软压板投退:执行完成*/
int const XJ_OPER_PTSOFTSET_STATE_5 = 415;

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
