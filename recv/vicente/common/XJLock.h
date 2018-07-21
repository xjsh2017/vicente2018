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
 * @defgroup 	CLockableObject	类说明
 * @{
 */
/**
 * @brief       可锁定对象基类
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
     *  @brief   	构造函数 
     *  @return 	无 
     */
	CLockableObject()	{}
    /*
     *  @brief   	析构函数
     *  @return 	无 
     */
	virtual ~CLockableObject(){}
    /*
     *  @brief   	锁定 
     *  @return 	bool True-成功 False-失败 
     */
	virtual bool Lock() = 0;
    /*
     *  @brief   	解除锁定 
     *  @return 	bool True-成功 False-失败
     */
	virtual bool Unlock() = 0;
};
/** @} */ //OVER

/**
 * @defgroup 	CLockUp	类说明
 * @{
 */
/**
 * @brief       可锁定对象的Guard类
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
     *  @brief   	构造函数
     *  @param 		[In/Out]CLockableObject*  可锁定对象指针
     *  @return 	无 
     */
	explicit CLockUp(CLockableObject* pLockable);
    /*
     *  @brief   	析构函数
     *  @return 	无 
     */
	~CLockUp();
private:
	CLockUp(const CLockUp&); //禁用copy构造函数
	CLockUp& operator= (const CLockUp&);//禁用assign操作函数
	/** @brief  		 可锁定对象指针 */
	CLockableObject* m_pLockable;
};
/** @} */ //OVER


/**
 * @defgroup 	CXJLock	类说明
 * @{
 */
/**
 * @brief       CLock类的包装，简化初始化，删除操作
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
     *  @brief   	构造函数
     *  @param 		[In]const char*  标志是那个模块在使用该互斥体
     *  @return 	无 
     */

	explicit CXJLock(const char* pHost);
    /*
     *  @brief   	析构函数 
     *  @return 	无 
     */

	virtual ~CXJLock();
    /*
     *  @brief   	阻塞取得所有权
     *  @return 	bool True-成功 False-失败
     */

	virtual bool Lock();
    /*
     *  @brief   	让出所有权
     *  @return 	bool True-成功 False-失败
     */

	virtual bool Unlock();
    /*
     *  @brief   	非阻塞取得所有权 
     *  @return 	bool True-成功 False-失败
     */

	bool Trylock();
private:
	CXJLock(const CXJLock&); //禁用copy构造函数
	CXJLock& operator= (const CXJLock&); //禁用assign操作函数
	/** @brief  		 CLock互斥体对象 */
	CLock m_lock;
};
/** @} */ //OVER

#endif // XJLock_h__
