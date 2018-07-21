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
  * @brief	Ĭ��ջ��С512K
*/
#define XJTHREAD_DEFAULT_STACK_SIZE 0

#ifdef OS_WINDOWS

	#include <direct.h>
	
	/**	
	* @brief	�������Եĳ������塣
	*/
	#define OUTPUTAPI __stdcall

	/**
	* @name		XJ_THREAD_ROUTINE
	* @brief	�̴߳��������͵Ķ���
	*/
	typedef unsigned int(OUTPUTAPI *XJ_THREAD_ROUTINE)(void *);

	typedef SYSTEMTIME xj_systime;

#endif	

#ifdef OS_LINUX

	/**	
	* @brief	�������Եĳ������塣
	*/
	#define OUTPUTAPI
	
	/**
	* @name		XJ_THREAD_ROUTINE
	* @brief	�̴߳��������͵Ķ���
	*/
	typedef void *(*XJ_THREAD_ROUTINE)(void *);

	/**	
	* @brief	ʱ��ṹ�塣
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
* @brief	����һ���̡߳�Ĭ��״̬�ǿɻ��(join)�ģ��ȷǷ���(detach)״̬��
* @param	phandle		�߳̾����ָ�롣
* @param	pid			�߳�id��ָ�롣
* @param	routine		�̴߳�������
* @param	param		�̴߳������Ĳ���ָ�롣
* @param	stacksize	�߳�ջ�ռ��С����λΪByte��Ĭ��Ϊ512K��
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_thread_create(THREAD_HANDLE *phandle, THREAD_ID *pid, XJ_THREAD_ROUTINE routine, void *param, UINT stacksize = XJTHREAD_DEFAULT_STACK_SIZE);

/**
* @name		xj_thread_detach
* @brief	��һ���߳̽��з��룬�˺���߳̽���ʱ�Զ��ͷ���Դ�ȡ�
* @param	phandle	�߳̾����
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note		
*/
int xj_thread_detach(THREAD_HANDLE phandle);

/**
* @name		xj_thread_join
* @brief	�����ڵȴ�һ���ɻ���̵߳Ľ�����
* @param	phandle		�߳̾����
* @param	*pretcode	�˳��롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_thread_join(THREAD_HANDLE phandle, void **pretcode);

/**
* @name		xj_thread_exit
* @brief	�˳�һ���̵߳�ִ�С�
* @param  retval	�˳��롣
* @return	�ޡ�
* @note 
*/
void xj_thread_exit(unsigned int retval);

int pro_create_thread(THREAD_HANDLE *phandle, THREAD_ID *pid, XJ_THREAD_ROUTINE routine, void *param);
//������

/**
* @name		xj_mutex_init
* @brief	��ʼ��һ���ɵݹ�Ļ�������
* @param	mutex	�����������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note		��������ǰ��һ��Ҫ������ʼ����
*/
int xj_mutex_init(MUTEX *mutex);

/**
* @name		xj_mutex_lock
* @brief	������
* @param	mutex	�����������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_mutex_lock(MUTEX *mutex);

/**
* @name		xj_mutex_unlock
* @brief	������
* @param	mutex	�����������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_mutex_unlock(MUTEX *mutex);

/**
* @name		xj_mutex_destroy
* @brief	���ٻ�������
* @param	mutex	�����������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note		�������ú�һ��Ҫ�������١�
*/
int xj_mutex_destroy(MUTEX *mutex);

//��������

/**
* @name		xj_cond_init
* @brief	��ʼ��һ������������
* @param	cond	�������������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note		����������ǰ��һ��Ҫ������ʼ����
*/
int xj_cond_init(HCOND *cond);

/**
* @name		xj_cond_wait
* @brief	�ȴ�һ�������������ȴ�ʱ��Ϊmilliseconds���롣
* @param	mutex			�����������ָ�룬������֤�����������Ļ�����ʡ�
* @param	cond			�������������ָ�롣
* @param	milliseconds	�ȴ��ĺ�������Ϊ-1��ʾ�����ȴ���ֱ���������㡣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_cond_wait(MUTEX *mutex, HCOND *cond, int milliseconds);

/**
* @name		xj_cond_signal
* @brief	�������ڵȴ��������������̣߳�����ж���߳��ڵȴ�����һ���߳�
			�����������̵߳ĵ��Ȳ����������ġ�
* @param	cond	�������������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_cond_signal(HCOND *cond);

/**
* @name		xj_cond_broadcast
* @brief	�����������б���������������cond�ϵ��̣߳���Щ�̱߳����Ѻ��ٴξ���
			��Ӧ�Ļ����������Ա���С��ʹ�����������
* @param	cond	�������������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_cond_broadcast(HCOND *cond);

/**
* @name		xj_cond_destroy
* @brief	��������������
* @param	cond	�������������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note		���������ú�һ��Ҫ��������
*/
int xj_cond_destroy(HCOND *cond);

//�����ź���

/**
* @name		xj_noname_sem_init
* @brief	��ʼ��һ�������ź�����ֻ��Ϊ��ǰ���̵������̹߳���value�������ź���
			�ĳ�ʼֵ��
* @param	sem		�ź��������ָ�롣
* @param	value	�ź����ĳ�ʼֵ��
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note		�ź�����ǰ��һ��Ҫ������ʼ����
*/
int xj_noname_sem_init(HNSEM *sem, unsigned int value);

/**
* @name		xj_noname_sem_init
* @brief	����һ�������ź�����
* @param	sem		�ź��������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note		�ź����ú�һ��Ҫ��������
*/
int xj_noname_sem_destroy(HNSEM *sem);

/**
* @name		xj_noname_sem_wait
* @brief	�ȴ�һ�������ź���������ź���ֵ<=0����һֱ�ȴ���ֱ��ֵ>0��
* @param	sem		�ź��������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_noname_sem_wait(HNSEM *sem);

/**
* @name		xj_noname_sem_post
* @brief	�������������ź�����ֵ�������߳�����������ź�����ʱ�������������
			��ʹ���е�һ���̲߳���������ѡ�����ͬ�������̵߳ĵ��Ȳ��Ծ����ġ�
* @param	sem		�ź��������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_noname_sem_post(HNSEM *sem);

/**
* @name		xj_noname_sem_getvalue
* @brief	��������ź�����ֵ��
* @param	sem		�ź��������ָ�롣
* @return	>=0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_noname_sem_getvalue(HNSEM *sem);

//����������

/**
* @name		xj_load_library
* @brief	����һ������⣬�硰libmytest.so����Ҫָ�����������·����
* @param	filename	��������ơ�
* @return	��NULLΪ�ɹ���NULLΪʧ�ܡ�
* @note		ʹ�ù����ǰ���������ظÿ⡣
*/
XJHANDLE xj_load_library(const char *filename);

/**
* @name		*xj_get_addr
* @brief	�ӹ��������һ��ģ�飬�纯���ĵ�ַ�ȡ�
* @param	handle	���������������xj_load_library()���صľ����
* @param	module	ģ��ָ�롣
* @return	��NULLΪ�ɹ���NULLΪʧ�ܡ�
* @note 
*/
void *xj_get_addr(XJHANDLE handle, const char *module);

/**
* @name		xj_free_library
* @brief	�ͷ�һ������⡣
* @param	handle	���������
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note		ʹ�ù����⣬�����ͷŸÿ⡣
*/
int xj_free_library(XJHANDLE handle);

/**
* @name		*xj_get_liberror
* @brief	��ò��������Ĵ���������	
* @return	����������
* @note 
*/
char *xj_get_liberror();

/**
* @name		xj_sleep
* @brief	˯�ߺ�����
* @param	milliseconds	˯�ߵĺ�����Ŀ������>=0��
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_sleep(unsigned int milliseconds);

/**
* @name		xj_getlocaltime
* @brief	ȡϵͳʱ�亯����
* @param	psystime	���ʱ�仺������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_getlocaltime(xj_systime *psystime);

/**
* @name		xj_setlocaltime
* @brief	�޸�ϵͳʱ�亯����
* @param	psystime	���ʱ�仺������ָ�롣
* @return	0Ϊ�ɹ�������Ϊ��������롣
* @note 
*/
int xj_setlocaltime(const xj_systime *psystime);

/**
* @name		xj_pathfile_exist
* @brief	�ж��ļ���Ŀ¼�Ƿ���ڡ�
* @param	pathfile	�ļ���Ŀ¼���ơ�
* @return	0Ϊ���ڣ���0Ϊ�����ڡ�
* @note 
*/
int xj_pathfile_exist(const char *pathfile);

/**
* @name		xj_pathfile_exec
* @brief	�ж϶��ļ���Ŀ¼�Ƿ���ִ��Ȩ�ޡ�
* @param	pathfile	�ļ���Ŀ¼���ơ�
* @return	0Ϊ�У���0Ϊ�ޡ�
* @note 
*/
int xj_pathfile_exec(const char *pathfile);

/**
* @name		xj_pathfile_rw
* @brief	�ж϶��ļ���Ŀ¼�Ƿ��ж�дȨ�ޡ�
* @param	pathfile	�ļ���Ŀ¼���ơ�
* @return	0Ϊ�У���0Ϊ�ޡ�
* @note 
*/
int xj_pathfile_rw(const char *pathfile);

/**
* @name		xj_get_file_size
* @brief	���ָ���ļ��Ĵ�С����λ��byte��
* @param	file_name	�ļ����ơ�
* @return	>=0Ϊ�ļ���ʵ�ʴ�С��-1Ϊʧ�ܡ�
* @note 
*/
int xj_get_file_size(const char *file_name);

/**
* @name		xj_create_local_dir
* @brief	����ָ��Ŀ¼��·�����ƣ��ڱ��ش�����Ӧ��Ŀ¼��
* @param	szPath	Ŀ¼��·�����ƣ������Ǿ���·����ǰ·���µ�·������ǰ·����'.'��
* @return	TRUEΪ�ɹ���FALSEΪʧ�ܡ�
* @note		·�����ű��������Ӧ��ƽ̨����linux��Ϊ'/'��Windows��Ϊ'\'��
*/
BOOL xj_create_local_dir(char *szPath);

/**
* @name		isCommenceApi
* @brief	�ж�һ�ַ����Ƿ�Ϊ��ע�ͣ���ע�͵ı�־��sum��;��//���մ�Ҳ��Ϊ
*			��ע�͡�
* @param	pszBuff	�ַ���ָ�롣
* @return	TRUE �ǣ�FALSE �ǡ�
* @note 
*/
BOOL isCommenceApi(char *pszBuff);

/**
* @name		GetPrivateProfileStr
* @brief	�ӳ�ʼ���ļ��С�ָ����section�£�ȡ����ΪpszKeyName��ֵ��
* @param	pszSectorName	section����ָ�롣
* @param	pszKeyName		KeyName����ָ�롣
* @param	pszDefaultStr	Ĭ�ϵ��ַ���ָ�롣
* @param	pszReturnedStr	Ŀ�Ļ�������ָ�롣
* @param	uSize			Ŀ�Ļ������Ĵ�С��
* @param	pszFileName		��ʼ���ļ�����ָ�롣
* @return	�������ַ������ȡ�
* @note		�����ʧ���ˣ��᷵��Ĭ�ϵ��ַ�����
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
* @brief	�ӳ�ʼ���ļ��С�ָ����section�£�ȡ����ΪpszKeyName��ֵ��
* @param	pszSectorName	section����ָ�롣
* @param	pszKeyName		KeyName����ָ�롣
* @param	nDefaultValue	Ĭ��ֵ��
* @param	pszFileName		��ʼ���ļ�����ָ�롣
* @return	ȡ�õ�ֵ��
* @note		�����ʧ���ˣ��᷵��Ĭ�ϵ�ֵ��
*/
int GetPrivateProfileint(
						 char	*pszSectorName,
						 char	*pszKeyName,
						 int	nDefaultValue,
						 char	*pszFileName
						 );

/**
* @name		GetPrivateProfileEqualint
* @brief	�ӳ�ʼ���ļ��У�ȡ����ΪpszKeyName��ֵ��
* @param	pszKeyName		KeyName����ָ�롣
* @param	nDefaultValue	Ĭ��ֵ��
* @param	pszFileName		��ʼ���ļ�����ָ�롣
* @return	ȡ�õ�ֵ��
* @note		�����ʧ���ˣ��᷵��Ĭ�ϵ�ֵ��
*/
int GetPrivateProfileEqualint(
						 char	*pszKeyName,
						 int	nDefaultValue,
						 char	*pszFileName
						 );

/**
* @name		xj_modify_time
* @brief	�����ں�ʱ����к��뼶���޸ģ������Ӻͼ��������㡣
* @param	psystime		���޸����ں�ʱ���ָ�룬Ĭ��ֵΪNULL��
* @param	wMilliseconds	�����޸ĵĺ�������Ĭ��ֵΪ0��
* @param	op				���㣺'+'��'-'��Ĭ��Ϊ'+'��
* @return	0Ϊ�ɹ�����0Ϊʧ�ܡ�
* @note		ֻ���޸�1900��1��1��0ʱ0��0��֮���ʱ�䡣
*/
int xj_modify_time(xj_systime *psystime, unsigned short wMilliseconds, char op);

/**
* @name		xjtime_to_timet
* @brief	��ϵͳʱ��ת��Ϊ�롣
* @param	psystime		��ת��ʱ���ָ�룬Ĭ��ֵΪNULL��
* @return	��ת�����롣
* @note		��
*/
time_t xjtime_to_timet(xj_systime *psystime);

#ifdef __cplusplus
}
#endif

#endif/*_XJLIB_H_*/
