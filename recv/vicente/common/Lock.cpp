// Lock.cpp: implementation of the CLock class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#include "Lock.h"

//////////////////////////////////////////////////////////////////////////
//                  Class  CLock   实现                                    
//////////////////////////////////////////////////////////////////////////

/*************************************************************
 函 数 名 : CLock()
 功能概要 : 构造函数
 返 回 值 : 无 
 参    数 : 
            char * phost : 标志是那个模块在使用该互斥体 
*************************************************************/
CLock::CLock()
{
	bzero(ch_host,FILE_NAME_MAX_LEN);

	bInit = FALSE;
}

/*************************************************************
 函 数 名 : CLock()
 功能概要 : 析构函数
 返 回 值 : 无 
 参    数 : 无
*************************************************************/
CLock::~CLock()
{
	if(bInit)
		del();
}

/*************************************************************
 函 数 名 : init()
 功能概要 : 初始化互斥体对象
 返 回 值 : BOOL 
 参    数 : 
            char * phost : 标志是那个模块在使用该互斥体
*************************************************************/
//##ModelId=4519E3D00375
BOOL CLock::init(char * phost)
{
	int n =0;

	//初始化ch_host
	if( (phost == NULL) ||
		(strlen(phost) > FILE_NAME_MAX_LEN) )
		strcpy(ch_host,"CLock");
	else
		strcpy(ch_host,phost);

#ifdef OS_WINDOWS 
	//init windows mutex
	//CRITICAL_SECTION 默认支持递归访问 
	::InitializeCriticalSection(&m_mutex);
#endif

#ifdef OS_LINUX

	//init linux mutex
	//设置mutex属性为支持递归
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	// 支持递归
	n = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP);  
	if(n != 0){
		printf("init the mutex attr of %s failed,reason:%s(%d)\n",\
			   ch_host,strerror(n),n);
		return FALSE;
	}
	n = pthread_mutex_init(&m_mutex,&attr);
	pthread_mutexattr_destroy(&attr);
    
	if(n != 0){
		printf("init the mutex of %s failed,reason:%s(%d)\n", \
			   ch_host,strerror(n),n);
		return FALSE;
	}
	
#endif
	bInit = TRUE;
	return bInit;
}

/*************************************************************
 函 数 名 : del()
 功能概要 : 删除互斥体对象
 返 回 值 : BOOL
 参    数 : 无
*************************************************************/
//##ModelId=4519E3E701F2
BOOL CLock::del()
{
	int n =0;

#ifdef OS_WINDOWS
	//del windows mutex
	::DeleteCriticalSection(&m_mutex);
#endif

#ifdef OS_LINUX

	//del linux mutex
	n= pthread_mutex_destroy(&m_mutex);
    
	if(n != 0){
		printf("del the mutex of %s failed,reason:%s(%d)\n", \
			   ch_host,strerror(n),n);
		return FALSE;
	}
	
#endif
	bInit = FALSE;
	return TRUE;
}

/*************************************************************
 函 数 名 : lock()
 功能概要 : 阻塞等待获得拥有权
 返 回 值 : void 
 参    数 : 无
*************************************************************/
//##ModelId=4519E3F40088
BOOL CLock::lock()
{
	int n =0;

#ifdef OS_WINDOWS
	//get ownership blocking
	::EnterCriticalSection(&m_mutex);
#endif

#ifdef OS_LINUX

	//get ownership blocking
	n= pthread_mutex_lock(&m_mutex);
    
	if(n != 0){
		printf("blocking get the mutex ownership of %s failed,reason:%s(%d)\n", \
			   ch_host,strerror(n),n);
		return FALSE;
	}

#endif	
	return TRUE;
}

/*************************************************************
 函 数 名 : trylock()
 功能概要 : 无论是否取得拥有权,调用立即返回(但WINDOWS下还是阻塞)
 返 回 值 : bool 
            TRUE :  成功获得用有权
			FALSE:  互斥体被其它线程锁定,获得失败
 参    数 : 无
*************************************************************/
BOOL CLock::trylock()
{
	int n =0;

#ifdef OS_WINDOWS
	//get ownership unblocking
	/*n = TryEnterCriticalSection(&m_mutex);  //WINCE支持,2000下不支持
	if(n == FALSE){
		printf("TryEnterCriticalSection the owneship of %s failed,reason:%s(%d)",\
			    ch_host,strerror(errno),errno);
		return FALSE;
	}*/
    ::EnterCriticalSection(&m_mutex);
#endif

#ifdef OS_LINUX

	//get ownership unblocking
	n= pthread_mutex_trylock(&m_mutex);
    
	if(n != 0){
		printf("unblocking get the mutex ownership of %s failed,reason:%s(%d)\n", \
			   ch_host,strerror(n),n);
		return FALSE;
	}

#endif	
	return TRUE;
}

/*************************************************************
 函 数 名 : unlock()
 功能概要 : 让出拥有权
 返 回 值 : BOOL
 参    数 : 无
*************************************************************/
//##ModelId=4519E40E003F
BOOL CLock::unlock()
{
	int n =0;

#ifdef OS_WINDOWS
	//release ownership 
	::LeaveCriticalSection(&m_mutex);
#endif

#ifdef OS_LINUX

	//release ownership 
	n= pthread_mutex_unlock(&m_mutex);
    
	if(n != 0){
		printf("release the mutex ownership of %s failed,reason:%s(%d)\n", \
			   ch_host,strerror(n),n);
		return FALSE;
	}
#endif		
	return TRUE;
}
