#ifndef _XJ_UTILS_STORE_H
#define _XJ_UTILS_STORE_H

#include "XJRootStore.h"

/////////////////////////////////////////////////////////////////////////////
// CXJUtilsStore

/**
 * @defgroup 	CXJUtilsStore	�û������ڴ�����
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


class CXJUtilsStorePrivate;
class CXJUtilsStore : public CXJRootStore
{
private:
	static CXJUtilsStore*	m_pInstance;
	CXJUtilsStorePrivate*	d_ptr;

public:
	static CXJUtilsStore *GetInstance();
	static void ReleaseInstance();

protected:
	CXJUtilsStore();           // protected constructor used by dynamic creation
	~CXJUtilsStore();

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

public:
	QByteArray			GetComputerName();


// Implementation
public:

	// Generated message map functions
protected:
};

#endif // !defined(_XJ_UTILS_STORE_H)
