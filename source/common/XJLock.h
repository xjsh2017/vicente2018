/********************************************************************
	created:	2008/09/18
	created:	17:9:2008   14:37
	file base:	XJLock
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef XJLock_h__
#define XJLock_h__

#include "Lock.h"

/**
 * @defgroup 	CLockableObject	��˵��
 * @{
 */
/**
 * @brief       �������������
*  @author      qingch
 * @version     ver1.0
 * @date        20/09/08

 *
 * example
 * @code
 *    
 * @endcode
 */
class CLockableObject {
public:
    /*
     *  @brief   	���캯�� 
     *  @return 	�� 
     */
	CLockableObject()	{}
    /*
     *  @brief   	��������
     *  @return 	�� 
     */
	virtual ~CLockableObject(){}
    /*
     *  @brief   	���� 
     *  @return 	bool True-�ɹ� False-ʧ�� 
     */
	virtual bool Lock() = 0;
    /*
     *  @brief   	������� 
     *  @return 	bool True-�ɹ� False-ʧ��
     */
	virtual bool Unlock() = 0;
};
/** @} */ //OVER

/**
 * @defgroup 	CLockUp	��˵��
 * @{
 */
/**
 * @brief       �����������Guard��
*  @author      qingch
 * @version     ver1.0
 * @date        20/09/08

 *
 * example
 * @code
 *    
 * @endcode
 */
class CLockUp {
public:
    /*
     *  @brief   	���캯��
     *  @param 		[In/Out]CLockableObject*  ����������ָ��
     *  @return 	�� 
     */
	explicit CLockUp(CLockableObject* pLockable);
    /*
     *  @brief   	��������
     *  @return 	�� 
     */
	~CLockUp();
private:
	CLockUp(const CLockUp&); //����copy���캯��
	CLockUp& operator= (const CLockUp&);//����assign��������
	/** @brief  		 ����������ָ�� */
	CLockableObject* m_pLockable;
};
/** @} */ //OVER


/**
 * @defgroup 	CXJLock	��˵��
 * @{
 */
/**
 * @brief       CLock��İ�װ���򻯳�ʼ����ɾ������
*  @author      qingch
 * @version     ver1.0
 * @date        01/10/08

 *
 * example
 * @code
 *    
 * @endcode
 */
class CXJLock:public CLockableObject{
public:
    /*
     *  @brief   	���캯��
     *  @param 		[In]const char*  ��־���Ǹ�ģ����ʹ�øû�����
     *  @return 	�� 
     */

	explicit CXJLock(const char* pHost);
    /*
     *  @brief   	�������� 
     *  @return 	�� 
     */

	virtual ~CXJLock();
    /*
     *  @brief   	����ȡ������Ȩ
     *  @return 	bool True-�ɹ� False-ʧ��
     */

	virtual bool Lock();
    /*
     *  @brief   	�ó�����Ȩ
     *  @return 	bool True-�ɹ� False-ʧ��
     */

	virtual bool Unlock();
    /*
     *  @brief   	������ȡ������Ȩ 
     *  @return 	bool True-�ɹ� False-ʧ��
     */

	bool Trylock();
private:
	CXJLock(const CXJLock&); //����copy���캯��
	CXJLock& operator= (const CXJLock&); //����assign��������
	/** @brief  		 CLock��������� */
	CLock m_lock;
};
/** @} */ //OVER

#endif // XJLock_h__
