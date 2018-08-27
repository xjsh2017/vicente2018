#ifndef _Q_TABLE_DEFINE_H
#define _Q_TABLE_DEFINE_H

#include "qbytearray.h"

#include <iostream>
#include <fstream>
using namespace std;

#define XJ_NAMESPACE_BEGIN  namespace XJ{
#define XJ_NAMESPACE_END	}

XJ_NAMESPACE_BEGIN

/** @brief			����ԭ���*/
extern const char* NAME_TB_TAGOUT_REASON;
/** @brief			ϵͳ���ñ�*/
extern const char* NAME_TB_SYS_CFG;
/** @brief			��ֵ�����*/
extern const char* NAME_TB_PT_SETTING_DEF;
/** @brief			��ֵ�鶨���*/
extern const char* NAME_TB_PT_ZONE_DEF;
/** @brief			��ѹ�嶨���*/
extern const char* NAME_TB_PT_SOFTBOARD_DEF;
/** @brief			�û������*/
extern const char* NAME_TB_SYS_USER;
/** @brief			�û��鶨���*/
extern const char* NAME_TB_SYS_GROUP;
/** @brief			ϵͳ���ܶ����*/
extern const char* NAME_TB_SYS_FUNC;
/** @brief			�û���Ȩ�޶����*/
extern const char* NAME_TB_SYS_GROUP_FUNC;

extern QByteArray GetFileSavePath(const char* table_name);

XJ_NAMESPACE_END

#endif // !defined(_Q_TABLE_DEFINE_H)
