#ifndef _XJUSERSTORE_H
#define _XJUSERSTORE_H

#include "XJRootStore.h"
#include "qptsetcard.h"

/////////////////////////////////////////////////////////////////////////////
// CXJUserStore

/**
 * @defgroup 	CXJUserStore	�û������ڴ�����
 * @{
 */ 
/**
 * @brief       �û�����ʱ���õ������ݽṹ
 * @author      WPS
 * @version     ver1.0
 * @date        09/07/18

 *
 * example
 * @code
 *    // ����˵�������ʹ��
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
	 *  @brief   	ReLoad	 ������������
	 *  @param 		void
	 *  @return 	BOOL
	 */
	BOOL		ReLoad();
	/*
	 *  @brief   	ReLoad	 ��������
	 *  @param 		void
	 *  @return 	BOOL
	 */
	BOOL		Save(const char *pszFilePath = NULL);

	int			GetUserFlags(const char* pszUserID, const char* pszUserGroupID);
	QByteArray	GetUserOwner(const char* pszUserID, const char* pszUserGroupID);

	void		SetUserFlags(const char *szUserID, const char *szUserGroupID, int nFlags);
	void		SetUserOwner(const char *szUserID, const char *szUserGroupID, QByteArray &owner);


// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJUSERSTORE_H)
