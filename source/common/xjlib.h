#ifndef _XJLIB_H_
#define _XJLIB_H_

#include <string>
#include "define.h"	

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BOOL

typedef int BOOL;

#endif

#ifndef TRUE

#define TRUE 1

#endif

#ifndef FALSE

#define FALSE 0

#endif

#define XJLIB_MAX_CHAR_BUFF 256

/**	
  * @brief	默认栈大小512K
*/
#define XJTHREAD_DEFAULT_STACK_SIZE 0

#ifdef OS_WINDOWS

	#include <direct.h>
	
	/**	
	* @brief	函数属性的常量定义。
	*/
	#define OUTPUTAPI __stdcall

	/**
	* @name		XJ_THREAD_ROUTINE
	* @brief	线程处理函数类型的定义
	*/
	typedef unsigned int(OUTPUTAPI *XJ_THREAD_ROUTINE)(void *);

	typedef SYSTEMTIME xj_systime;

#endif	

#ifdef OS_LINUX

	/**	
	* @brief	函数属性的常量定义。
	*/
	#define OUTPUTAPI
	
	/**
	* @name		XJ_THREAD_ROUTINE
	* @brief	线程处理函数类型的定义
	*/
	typedef void *(*XJ_THREAD_ROUTINE)(void *);

	/**	
	* @brief	时间结构体。
	*/
	typedef struct
	{
		unsigned short wYear;
		unsigned short wMonth;
		unsigned short wDayOfWeek;
		unsigned short wDay;
		unsigned short wHour;
		unsigned short wMinute;
		unsigned short wSecond;
		unsigned short wMilliseconds;
	}xj_systime;
	
#endif /*OS_LINUX*/

//thread

/**
* @name		xj_thread_create
* @brief	创建一个线程。默认状态是可汇合(join)的，既非分离(detach)状态。
* @param	phandle		线程句柄的指针。
* @param	pid			线程id的指针。
* @param	routine		线程处理函数。
* @param	param		线程处理函数的参数指针。
* @param	stacksize	线程栈空间大小，单位为Byte，默认为512K。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_thread_create(THREAD_HANDLE *phandle, THREAD_ID *pid, XJ_THREAD_ROUTINE routine, void *param, UINT stacksize = XJTHREAD_DEFAULT_STACK_SIZE);

/**
* @name		xj_thread_detach
* @brief	对一个线程进行分离，此后该线程结束时自动释放资源等。
* @param	phandle	线程句柄。
* @return	0为成功；其它为具体错误码。
* @note		
*/
int xj_thread_detach(THREAD_HANDLE phandle);

/**
* @name		xj_thread_join
* @brief	阻塞在等待一个可汇合线程的结束。
* @param	phandle		线程句柄。
* @param	*pretcode	退出码。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_thread_join(THREAD_HANDLE phandle, void **pretcode);

/**
* @name		xj_thread_exit
* @brief	退出一个线程的执行。
* @param  retval	退出码。
* @return	无。
* @note 
*/
void xj_thread_exit(unsigned int retval);

int pro_create_thread(THREAD_HANDLE *phandle, THREAD_ID *pid, XJ_THREAD_ROUTINE routine, void *param);
//互斥锁

/**
* @name		xj_mutex_init
* @brief	初始化一个可递归的互斥锁。
* @param	mutex	互斥锁句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note		互斥锁用前，一定要对它初始化。
*/
int xj_mutex_init(MUTEX *mutex);

/**
* @name		xj_mutex_lock
* @brief	上锁。
* @param	mutex	互斥锁句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_mutex_lock(MUTEX *mutex);

/**
* @name		xj_mutex_unlock
* @brief	开锁。
* @param	mutex	互斥锁句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_mutex_unlock(MUTEX *mutex);

/**
* @name		xj_mutex_destroy
* @brief	销毁互斥锁。
* @param	mutex	互斥锁句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note		互斥锁用后，一定要将它销毁。
*/
int xj_mutex_destroy(MUTEX *mutex);

//条件变量

/**
* @name		xj_cond_init
* @brief	初始化一个条件变量。
* @param	cond	条件变量句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note		条件变量用前，一定要对它初始化。
*/
int xj_cond_init(HCOND *cond);

/**
* @name		xj_cond_wait
* @brief	等待一个条件变量，等待时间为milliseconds毫秒。
* @param	mutex			互斥锁句柄的指针，用来保证对条件变量的互斥访问。
* @param	cond			条件变量句柄的指针。
* @param	milliseconds	等待的毫秒数，为-1表示持续等待，直到条件满足。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_cond_wait(MUTEX *mutex, HCOND *cond, int milliseconds);

/**
* @name		xj_cond_signal
* @brief	唤醒正在等待该条件变量的线程；如果有多个线程在等待，哪一个线程
			被唤醒是由线程的调度策略所决定的。
* @param	cond	条件变量句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_cond_signal(HCOND *cond);

/**
* @name		xj_cond_broadcast
* @brief	用来唤醒所有被阻塞在条件变量cond上的线程，这些线程被唤醒后将再次竞争
			相应的互斥锁，所以必须小心使用这个函数。
* @param	cond	条件变量句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_cond_broadcast(HCOND *cond);

/**
* @name		xj_cond_destroy
* @brief	销毁条件变量。
* @param	cond	条件变量句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note		条件变量用后，一定要销毁它。
*/
int xj_cond_destroy(HCOND *cond);

//无名信号量

/**
* @name		xj_noname_sem_init
* @brief	初始化一个无名信号量，只能为当前进程的所有线程共享，value给出了信号量
			的初始值。
* @param	sem		信号量句柄的指针。
* @param	value	信号量的初始值。
* @return	0为成功；其它为具体错误码。
* @note		信号量用前，一定要对它初始化。
*/
int xj_noname_sem_init(HNSEM *sem, unsigned int value);

/**
* @name		xj_noname_sem_init
* @brief	销毁一个无名信号量。
* @param	sem		信号量句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note		信号量用后，一定要销毁它。
*/
int xj_noname_sem_destroy(HNSEM *sem);

/**
* @name		xj_noname_sem_wait
* @brief	等待一个无名信号量。如果信号量值<=0，则一直等待，直到值>0。
* @param	sem		信号量句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_noname_sem_wait(HNSEM *sem);

/**
* @name		xj_noname_sem_post
* @brief	用来增加无名信号量的值。当有线程阻塞在这个信号量上时，调用这个函数
			会使其中的一个线程不在阻塞，选择机制同样是由线程的调度策略决定的。
* @param	sem		信号量句柄的指针。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_noname_sem_post(HNSEM *sem);

/**
* @name		xj_noname_sem_getvalue
* @brief	获得无名信号量的值。
* @param	sem		信号量句柄的指针。
* @return	>=0为成功；其它为具体错误码。
* @note 
*/
int xj_noname_sem_getvalue(HNSEM *sem);

//共享库的下载

/**
* @name		xj_load_library
* @brief	下载一个共享库，如“libmytest.so”。要指明共享库的相对路径。
* @param	filename	共享库名称。
* @return	非NULL为成功；NULL为失败。
* @note		使用共享库前，必须下载该库。
*/
XJHANDLE xj_load_library(const char *filename);

/**
* @name		*xj_get_addr
* @brief	从共享库下载一个模块，如函数的地址等。
* @param	handle	共享库句柄，必须是xj_load_library()返回的句柄。
* @param	module	模块指针。
* @return	非NULL为成功；NULL为失败。
* @note 
*/
void *xj_get_addr(XJHANDLE handle, const char *module);

/**
* @name		xj_free_library
* @brief	释放一个共享库。
* @param	handle	共享库句柄。
* @return	0为成功；其它为具体错误码。
* @note		使用共享库库，必须释放该库。
*/
int xj_free_library(XJHANDLE handle);

/**
* @name		*xj_get_liberror
* @brief	获得操作共享库的错误描述。	
* @return	错误描述。
* @note 
*/
char *xj_get_liberror();

/**
* @name		xj_sleep
* @brief	睡眠函数。
* @param	milliseconds	睡眠的毫秒数目，必须>=0。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_sleep(unsigned int milliseconds);

/**
* @name		xj_getlocaltime
* @brief	取系统时间函数。
* @param	psystime	存放时间缓冲区的指针。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_getlocaltime(xj_systime *psystime);

/**
* @name		xj_setlocaltime
* @brief	修改系统时间函数。
* @param	psystime	存放时间缓冲区的指针。
* @return	0为成功；其它为具体错误码。
* @note 
*/
int xj_setlocaltime(const xj_systime *psystime);

/**
* @name		xj_pathfile_exist
* @brief	判断文件或目录是否存在。
* @param	pathfile	文件或目录名称。
* @return	0为存在；非0为不存在。
* @note 
*/
int xj_pathfile_exist(const char *pathfile);

/**
* @name		xj_pathfile_exec
* @brief	判断对文件或目录是否有执行权限。
* @param	pathfile	文件或目录名称。
* @return	0为有；非0为无。
* @note 
*/
int xj_pathfile_exec(const char *pathfile);

/**
* @name		xj_pathfile_rw
* @brief	判断对文件或目录是否有读写权限。
* @param	pathfile	文件或目录名称。
* @return	0为有；非0为无。
* @note 
*/
int xj_pathfile_rw(const char *pathfile);

/**
* @name		xj_get_file_size
* @brief	获得指定文件的大小，单位：byte。
* @param	file_name	文件名称。
* @return	>=0为文件的实际大小；-1为失败。
* @note 
*/
int xj_get_file_size(const char *file_name);

/**
* @name		xj_create_local_dir
* @brief	根据指定目录的路径名称，在本地创建相应的目录。
* @param	szPath	目录的路径名称，可以是绝对路径或当前路径下的路径，当前路径是'.'。
* @return	TRUE为成功；FALSE为失败。
* @note		路径符号必须符合相应的平台，如linux下为'/'，Windows下为'\'。
*/
BOOL xj_create_local_dir(char *szPath);

/**
* @name		isCommenceApi
* @brief	判断一字符串是否为行注释？行注释的标志：sum、;和//，空串也判为
*			行注释。
* @param	pszBuff	字符串指针。
* @return	TRUE 是；FALSE 非。
* @note 
*/
BOOL isCommenceApi(char *pszBuff);

/**
* @name		GetPrivateProfileStr
* @brief	从初始化文件中、指定的section下，取名称为pszKeyName的值。
* @param	pszSectorName	section名称指针。
* @param	pszKeyName		KeyName名称指针。
* @param	pszDefaultStr	默认的字符串指针。
* @param	pszReturnedStr	目的缓冲区的指针。
* @param	uSize			目的缓冲区的大小。
* @param	pszFileName		初始化文件名称指针。
* @return	读到的字符串长度。
* @note		如果读失败了，会返回默认的字符串。
*/
unsigned int GetPrivateProfileStr(
						char	*pszSectorName,
						char	*pszKeyName,
						char	*pszDefaultStr,
						char	*pszReturnedStr,
						unsigned int  uSize,
						char	*pszFileName
						);

/**
* @name		GetPrivateProfileint
* @brief	从初始化文件中、指定的section下，取名称为pszKeyName的值。
* @param	pszSectorName	section名称指针。
* @param	pszKeyName		KeyName名称指针。
* @param	nDefaultValue	默认值。
* @param	pszFileName		初始化文件名称指针。
* @return	取得的值。
* @note		如果读失败了，会返回默认的值。
*/
int GetPrivateProfileint(
						 char	*pszSectorName,
						 char	*pszKeyName,
						 int	nDefaultValue,
						 char	*pszFileName
						 );

/**
* @name		GetPrivateProfileEqualint
* @brief	从初始化文件中，取名称为pszKeyName的值。
* @param	pszKeyName		KeyName名称指针。
* @param	nDefaultValue	默认值。
* @param	pszFileName		初始化文件名称指针。
* @return	取得的值。
* @note		如果读失败了，会返回默认的值。
*/
int GetPrivateProfileEqualint(
						 char	*pszKeyName,
						 int	nDefaultValue,
						 char	*pszFileName
						 );

/**
* @name		xj_modify_time
* @brief	对日期和时间进行毫秒级的修改，包括加和减两种运算。
* @param	psystime		待修改日期和时间的指针，默认值为NULL。
* @param	wMilliseconds	用来修改的毫秒数，默认值为0。
* @param	op				运算：'+'或'-'，默认为'+'。
* @return	0为成功；非0为失败。
* @note		只能修改1900年1月1日0时0分0秒之后的时间。
*/
int xj_modify_time(xj_systime *psystime, unsigned short wMilliseconds, char op);

/**
* @name		xjtime_to_timet
* @brief	把系统时间转换为秒。
* @param	psystime		待转换时间的指针，默认值为NULL。
* @return	被转换的秒。
* @note		。
*/
time_t xjtime_to_timet(xj_systime *psystime);

#ifdef __cplusplus
}
#endif

#endif/*_XJLIB_H_*/
