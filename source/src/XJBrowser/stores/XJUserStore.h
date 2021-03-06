#ifndef _XJUSERSTORE_H
#define _XJUSERSTORE_H

#include "XJRootStore.h"
#include "qbytearraymatrix.h"

/////////////////////////////////////////////////////////////////////////////
// CXJUserStore

/**
 * @defgroup 	CXJUserStore	用户管理内存数据
 * @{
 */ 
/**
 * @brief       用户管理时所用到的数据结构
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


class CXJUserStorePrivate;
class CXJUserStore : public CXJRootStore
{
private:
	static CXJUserStore*	m_pInstance;
	CXJUserStorePrivate*	d_ptr;

public:
	static CXJUserStore *GetInstance();
	static void ReleaseInstance();

protected:
	CXJUserStore();           // protected constructor used by dynamic creation
	~CXJUserStore();

// Operations
public:
	/*
	 *  @brief   	ReLoad	 重新载入数据
	 *  @param 		void
	 *  @return 	BOOL
	 */
	BOOL		ReLoad();
	/*
	 *  @brief   	Save	 保存数据
	 *  @param 		void
	 *  @return 	BOOL
	 */
	BOOL		Save(const char *pszFilePath = NULL);

	/*
	 *  @brief   	Check	 检查数据（一些原始的数据记录）
	 *  @param 		void
	 *  @return 	BOOL
	 */
	BOOL		Check();

public:
	QByteArray			GetUserGroupName(int nType = XJ_USERGROUP_RUNNER);
	QByteArray			GetUserGroupName(const char* pszID, int nIDType = 0);
	QByteArray			GetUserGroupName(QByteArray &baID, int nIDType = 0);
	QByteArray			GetUserGroupID(const char* pszUserID);
	QByteArray			GetUserGroupID(QByteArray &baUserID);
	
	QByteArrayMatrix	GetFuncIDList(const char* pszID, int nIDType = 0);
	QByteArrayMatrix	GetFuncIDList(QByteArray &baID, int nIDType = 0);
	
	bool				hasFuncID(int nFuncID, const char* pszID, int nIDType = 0);
	bool				hasFuncID(int nFuncID, QByteArray &baID, int nIDType = 0);

	QByteArray			GetFuncID(int nFuncID);

	int					GetUserFlags(const char* pszUserID, const char* pszUserGroupID);
	QByteArray			GetUserOwner(const char* pszUserID, const char* pszUserGroupID);
	QByteArray			GetUserOwner(const char* pszUserID);

	void				SetUserFlags(const char *szUserID, const char *szUserGroupID, int nFlags);
	void				SetUserOwner(const char *szUserID, const char *szUserGroupID, QByteArray &owner);
	QByteArray			BuildComboxUserList(const char* pszUserGroupID);
	QByteArray			BuildComboxUserList(int nGroupType = XJ_USERGROUP_RUNNER);
	QByteArray			BuildComboxUserList2(int nFuncID);


// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJUSERSTORE_H)
