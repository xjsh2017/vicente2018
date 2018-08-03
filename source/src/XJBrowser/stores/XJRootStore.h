#ifndef _XJROOTSTORE_H
#define _XJROOTSTORE_H

/////////////////////////////////////////////////////////////////////////////
// CXJRootStore

/**
 * @defgroup 	CXJRootStore	Ӧ��ʵ���ڴ����ݣ����ࣩ
 * @{
 */ 
/**
 * @brief       ��ӦĳӦ��ʵ��ʱ���õ������ݽṹ
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
