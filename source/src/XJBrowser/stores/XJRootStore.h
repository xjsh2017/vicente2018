#ifndef _XJROOTSTORE_H
#define _XJROOTSTORE_H

/////////////////////////////////////////////////////////////////////////////
// CXJRootStore

/**
 * @defgroup 	CXJRootStore	应用实例内存数据（基类）
 * @{
 */ 
/**
 * @brief       对应某应用实例时所用到的数据结构
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

class CXJRootStore
{
public:
	CXJRootStore(void);
	virtual ~CXJRootStore(void);

	static void Register(CXJRootStore *pInstance);
	static void UnRegister(CXJRootStore *pInstance);
	static void Release();

private:
	static list<CXJRootStore*> m_lstInstantce;
};

#endif
