// qtabledefine.cpp : implementation file
//

#include "qtabledefine.h"


const char* ROOT_PATH = "d:/tables/";
const char* FILE_SUFFIX = ".txt";

#define  filepath(x, y, z) "x##z##y"

XJ_NAMESPACE_BEGIN

/** @brief			����ԭ���*/
const char* NAME_TB_TAGOUT_REASON = "tb_suspend_reason_def";
//const char* FILE_SAVE_PATH_TB_TAGOUT_REASON = filepath(ROOT_PATH, FILE_SUFFIX, NAME_TB_TAGOUT_REASON);
/** @brief			ϵͳ���ñ�*/
const char* NAME_TB_SYS_CFG = "tb_sys_config";
/** @brief			��ֵ�����*/
const char* NAME_TB_PT_SETTING_DEF = "tb_pt_setting_def";
/** @brief			��ֵ�鶨���*/
const char* NAME_TB_PT_ZONE_DEF = "tb_pt_zone_def";
/** @brief			��ѹ�嶨���*/
const char* NAME_TB_PT_SOFTBOARD_DEF = "tb_pt_softboard_def";

/** @brief			�û������*/
const char* NAME_TB_SYS_USER = "tb_sys_user";
/** @brief			�û��鶨���*/
const char* NAME_TB_SYS_GROUP = "tb_sys_group";
/** @brief			ϵͳ���ܶ����*/
const char* NAME_TB_SYS_FUNC = "tb_sys_func";
/** @brief			�û���Ȩ�޶����*/
const char* NAME_TB_SYS_GROUP_FUNC = "tb_sys_group_func";

QByteArray GetFileSavePath(const char* table_name)
{
	QByteArray s;

	s << ROOT_PATH << table_name << FILE_SUFFIX;

	return s;
}


XJ_NAMESPACE_END