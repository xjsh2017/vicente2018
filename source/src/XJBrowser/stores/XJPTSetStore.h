#ifndef _XJPTSETSTORE_H
#define _XJPTSETSTORE_H

#include "XJRootStore.h"

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
	 *  @brief   	AddManOperator	 添加人工操作日志 
	 *  @param 		[In]a param of Type  CString  功能模块ID
	 *  @param 		[In]a param of Type  CString  操作动作描述
	 *  @return 	void
	 */
	void AddNewManOperator( CString FunID, CString Act, CString strMsg, int nOperType, int nOperResult  = 0, int num = -1);
	void AddNewManOperator( CString FunID, CString Act, CString strMsg, CString strUser, int nOperType, int nOperResult  = 0, int num  = -1);
	
	/*
	 *  @brief   	GetStatus	 获取状态机状态 
	 *  @param 		void
	 *  @return 	CPTSetCard* 返回修改卡指针	
	 */
	CXJPTSetStore* GetStore();

	/*
	 *  @brief   	SaveStore	 获取状态机状态 
	 *  @param 		[In]a param of Type  CString  功能模块ID
	 *  @param 		[In]a param of Type  CString  操作动作描述
	 *  @return 	PTSETSTORE	
	 */
	BOOL SaveStore();

	BOOL NextPTSetModState(int nNextStateID, const PT_ZONE &data, const CString &sUserID, const CString &sRecords = CString());
	BOOL RevertPTSetModState(int nRevertStateID, int nFlag = 0);
	BOOL SetRevertModifyValueFlag(int nFlag);
	CString GetUserIDByState(int nState, CString &sRecords = CString());
	BOOL SaveTempPTSetToDB(CString &sPTID, MODIFY_LIST &arrModifyList);
	BOOL SaveNewPTSetToDB(CString &sCPU, CString &sPTID, CTypedPtrArray<CPtrArray, PT_SETTING*> &arrSetting);
	BOOL RevertTempPTSetToDB(const PT_ZONE &zone, int nFlag = 0);
	
	/*
	 *  @brief   	GetStatus	 检查状态机配置情况
	 *  @param 		void
	 *  @return 	int		-1: 配置检查失败;
							0: 配置存在;
							1: 配置不存在;
							10: 配置不存在，创建失败;
							11: 配置不存在，创建成功	
	 */
	int			CheckStore();
	
	CString		GetUserGroupNameByID(CString sUserGroupID);
	BOOL		SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));

// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJPTSETSTORE_H)
