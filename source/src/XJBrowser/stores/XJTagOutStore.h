#ifndef _XJPTSETSTORE_H
#define _XJPTSETSTORE_H

#include "XJRootStore.h"
#include "qbytearraymatrix.h"

/////////////////////////////////////////////////////////////////////////////
// CXJTagOutStore

/**
 * @defgroup 	CXJTagOutStore	上锁挂牌内存数据
 * @{
 */ 
/**
 * @brief       上锁挂牌时所用到的数据结构
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

class QPTSetStateTable;

class QPTSetDataTable;
class QPTSetZoneDataTable;
class QPTSetSBDataTable;

class CXJTagOutStorePrivate;
class CXJTagOutStore : public CXJRootStore
{
private:
	static CXJTagOutStore*	m_pInstance;
	CXJTagOutStorePrivate*	d_ptr;

public:
	static CXJTagOutStore *GetInstance();
	static void ReleaseInstance();

protected:
	CXJTagOutStore();           // protected constructor used by dynamic creation
	~CXJTagOutStore();

// Attributes
public:
	/** @brief           动态库句柄*/
	HMODULE	m_hModule;

public:
	/*
	 *  @brief   	ReLoadStore	 重新载入状态机 
	 *  @param 		void
	 *  @return 	BOOL
	 */
	BOOL	ReLoadState();

	/** @brief           状态机*/
	QPTSetStateTable*	GetState();

	/** @brief           定值修改数据*/
	QPTSetDataTable*	GetPTSetData();

// Operations
public:
	/*
	 *  @brief   	SaveStore	 保存状态机 
	 *  @param 		[In]a param of Type  CString  功能模块ID
	 *  @param 		[In]a param of Type  CString  操作动作描述
	 *  @return 	PTSETSTORE	
	 */
	BOOL	Save();
	BOOL	SaveState(const char *pszFilePath = NULL);
	BOOL	SaveData(const char *pszFilePath = NULL);
	
	/*
	 *  @brief   	CheckStore	 检查状态机配置情况
	 *  @param 		void
	 *  @return 	BOOL		FALSE - 配置检查失败;
							    TRUE  - 配置检查成功
	 */
	BOOL	Check();
	
	BOOL	RevertModify();

	/*
	 *  @brief   	AddManOperator	 添加人工操作日志 
	 *  @param 		[In]a param of Type  CString  FunID  功能模块ID
	 *  @param 		[In]a param of Type  CString  Act	 操作动作描述
	 *  @return 	void
	 */
	void AddNewManOperator(CString FunID, CString Act, CString strTime, CString strMsg
		, CString sUserID, int nOperType, int nOperResult  = 0, int num  = -1);
	void AddNewManOperator(int nStateID, const char* szTime, CString sUserID, int nOperType = OPER_SUCCESS, CString strMsg = CString());

	BOOL		SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));

	QByteArray		GetFuncID(int nStateID);
	QByteArray		GetSubFuncID(int nStateID);

	int				GetTagOutRowIdx(int nTagOutType = XJ_TAGOUT_UNDEFINE);

// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJPTSETSTORE_H)
