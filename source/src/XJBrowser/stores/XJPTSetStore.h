#ifndef _XJPTSETSTORE_H
#define _XJPTSETSTORE_H

#include "XJRootStore.h"
#include "qbytearraymatrix.h"

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

class QPTSetStateTable;

class QPTSetDataTable;
class QPTSetZoneDataTable;
class QPTSetSBDataTable;

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
	BOOL	Save(const char *pszFilePath/* = NULL*/);
	
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
	
	BOOL	RevertModify();

	/*
	 *  @brief   	AddManOperator	 添加人工操作日志 
	 *  @param 		[In]a param of Type  CString  FunID  功能模块ID
	 *  @param 		[In]a param of Type  CString  Act	 操作动作描述
	 *  @return 	void
	 */
	void AddNewManOperator(CString FunID, CString Act, CString strTime, CString strMsg
		, CString sUserID, int nOperType, int nOperResult  = 0, int num  = -1);
	void AddNewManOperator(int nStateID, const char* szTime, CString sUserID);

	CString		GetUserTypeName(CString sUserGroupID);
	CString		GetUserTypeName(int nUserType);
	BOOL		SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag = CString(""));

	CString		GetFuncID(int nStateID);

// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJPTSETSTORE_H)
