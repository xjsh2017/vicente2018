#ifndef _Q_TABLE_DEFINE_H
#define _Q_TABLE_DEFINE_H

#include "qbytearray.h"

#include <iostream>
#include <fstream>
using namespace std;

#define XJ_NAMESPACE_BEGIN  namespace XJ{
#define XJ_NAMESPACE_END	}

XJ_NAMESPACE_BEGIN

/** @brief			挂牌原因表*/
extern const char* NAME_TB_TAGOUT_REASON;
/** @brief			系统配置表*/
extern const char* NAME_TB_SYS_CFG;
/** @brief			定值定义表*/
extern const char* NAME_TB_PT_SETTING_DEF;
/** @brief			定值组定义表*/
extern const char* NAME_TB_PT_ZONE_DEF;
/** @brief			软压板定义表*/
extern const char* NAME_TB_PT_SOFTBOARD_DEF;
/** @brief			用户定义表*/
extern const char* NAME_TB_SYS_USER;
/** @brief			用户组定义表*/
extern const char* NAME_TB_SYS_GROUP;
/** @brief			系统功能定义表*/
extern const char* NAME_TB_SYS_FUNC;
/** @brief			用户组权限定义表*/
extern const char* NAME_TB_SYS_GROUP_FUNC;

extern QByteArray GetFileSavePath(const char* table_name);

XJ_NAMESPACE_END

#endif // !defined(_Q_TABLE_DEFINE_H)
