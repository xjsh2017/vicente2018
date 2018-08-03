#ifndef _XJPTSETSTORE_H
#define _XJPTSETSTORE_H

#include "XJRootStore.h"
#include "core/qbytearray.h"

/////////////////////////////////////////////////////////////////////////////
// CXJPTSetStore

/**
 * @defgroup 	CXJPTSetStore	定值修改内存数据
 * @{
 */ 
/**
 * @brief       定值修改时所用到的数据结构
 * @author      WPS
 * @version     ver1.0
 * @date        09/07/18

 *
 * example
 * @code
 *    // 举例说明类如何使用
 * @endcode
 */
/** @} */ //OVER

#define PT_ID_LEN 20

/** @brief			装置挂牌*/
int const XJ_OPER_HANGOUT = 101;
/** @brief			装置取消挂牌*/
int const XJ_OPER_UNHANGOUT = 102;

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

struct PT_SETTING_DATA{
	PT_SETTING* pts;
	QByteArray reserve1;	// 原值
	QByteArray reserve2;	// 备用
	QByteArray reserve3;	// 修改值
	BOOL bMod;
	
	PT_SETTING_DATA::PT_SETTING_DATA(){
		bMod = FALSE;
		pts = NULL;
		reserve1 = "";
		reserve2 = "";
		reserve3 = "";
	}
};
typedef CTypedPtrArray<CPtrArray, PT_SETTING_DATA*>		PT_SETTING_DATA_LIST;
typedef CTypedPtrArray<CPtrArray, PT_SETTING*>			PT_SETTING_LIST;

class QPTSetCard;
class QLogTable;
class CXJPTSetStorePrivate;
class CXJPTSetStore : public CXJRootStore
{
private:
	static CXJPTSetStore*	m_pInstance;
	CXJPTSetStorePrivate*	d_ptr;

public:
	static CXJPTSetStore *GetInstance();
	static void ReleaseInstance();

protected:
	CXJPTSetStore();           // protected constructor used by dynamic creation
	~CXJPTSetStore();

// Attributes
public:
	/** @brief           动态库句柄*/
	HMODULE	m_hModule;

// Operations
public:
	/*
	 *  @brief   	ReLoadStore	 获取状态机状态 
	 *  @param 		void
	 *  @return 	BOOL
	 */
	BOOL	ReLoad();	
	BOOL	ReLoadStore();

	/*
	 *  @brief   	SaveStore	 保存状态机 
	 *  @param 		[In]a param of Type  CString  功能模块ID
	 *  @param 		[In]a param of Type  CString  操作动作描述
	 *  @return 	PTSETSTORE	
	 */
	BOOL	Save();
	
	/*
	 *  @brief   	CheckStore	 检查状态机配置情况
	 *  @param 		void
	 *  @return 	int		-1: 配置检查失败;
							0: 配置存在;
							1: 配置不存在;
							10: 配置不存在，创建失败;
							11: 配置不存在，创建成功	
	 */
	int		Check();

	/* 控制卡 */
	QPTSetCard*	GetCard();
	/* 日志 */
	QLogTable*	GetLog();
	
	BOOL	Next(const char *card_data, const char *log_data);
	BOOL	Next(int nNextStateID, const char* szUserID, int nFlag = 0);
	BOOL	Next(int nNextStateID, const char* szUserID, const char* szPTID, int nFlag = 0);
	BOOL	Next(int nNextStateID, int nCPUID, int nZoneID, const char* szUserID, int nFlag = 0);
	
	BOOL	SaveRecallToDB(CString &sCPU, CString &sPTID, CTypedPtrArray<CPtrArray, PT_SETTING*> &arrSetting);
	BOOL	SaveModifyToDB(CString &sPTID, MODIFY_LIST &arrModifyList);
	BOOL	RevertModify();

	BOOL	SetCardFlags(int nFlag);

	/*
	 *  @brief   	AddManOperator	 添加人工操作日志 
	 *  @param 		[In]a param of Type  CString  FunID  功能模块ID
	 *  @param 		[In]a param of Type  CString  Act	 操作动作描述
	 *  @return 	void
	 */
	void AddNewManOperator(CString FunID, CString Act, CString strTime, CString strMsg
		, CString sUserID, int nOperType, int nOperResult  = 0, int num  = -1);
	void AddNewManOperator(int nStateID, const char* szTime, CString sUserID);

	CString		GetUserGroupNameByID(CString sUserGroupID);
	BOOL		SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));

	CString		GetFuncID(int nStateID);

// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJPTSETSTORE_H)
