// qtabledefine.cpp : implementation file
//

#include "qtabledefine.h"


const char* ROOT_PATH = "d:/tables/";
const char* FILE_SUFFIX = ".txt";

#define  filepath(x, y, z) "x##z##y"

XJ_NAMESPACE_BEGIN

/** @brief			挂牌原因表*/
const char* NAME_TB_TAGOUT_REASON = "tb_suspend_reason_def";
//const char* FILE_SAVE_PATH_TB_TAGOUT_REASON = filepath(ROOT_PATH, FILE_SUFFIX, NAME_TB_TAGOUT_REASON);
/** @brief			系统配置表*/
const char* NAME_TB_SYS_CFG = "tb_sys_config";
/** @brief			定值定义表*/
const char* NAME_TB_PT_SETTING_DEF = "tb_pt_setting_def";
/** @brief			定值组定义表*/
const char* NAME_TB_PT_ZONE_DEF = "tb_pt_zone_def";
/** @brief			软压板定义表*/
const char* NAME_TB_PT_SOFTBOARD_DEF = "tb_pt_softboard_def";

/** @brief			用户定义表*/
const char* NAME_TB_SYS_USER = "tb_sys_user";
/** @brief			用户组定义表*/
const char* NAME_TB_SYS_GROUP = "tb_sys_group";
/** @brief			系统功能定义表*/
const char* NAME_TB_SYS_FUNC = "tb_sys_func";
/** @brief			用户组权限定义表*/
const char* NAME_TB_SYS_GROUP_FUNC = "tb_sys_group_func";

QByteArray GetFileSavePath(const char* table_name)
{
	QByteArray s;

	s << ROOT_PATH << table_name << FILE_SUFFIX;

	return s;
}


XJ_NAMESPACE_END