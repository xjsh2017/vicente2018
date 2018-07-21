#pragma warning(disable : 4275)
#include "xjlib.h"

#ifdef OS_WINDOWS

//thread

//0 success; -1 error
int xj_thread_create(THREAD_HANDLE *phandle, THREAD_ID *pid, XJ_THREAD_ROUTINE routine, void *param, UINT stacksize)
{
	unsigned int dwId = 0;
	
	THREAD_HANDLE h = 0;
	
	h = (THREAD_HANDLE)_beginthreadex(0, stacksize, routine, param, 0, &dwId);

	if (NULL == h)
		return -1;
	
	if(h == (THREAD_HANDLE)-1)
		return -1;

	if(phandle)
	{
		*phandle = h;
		if (pid)
			*pid = dwId;

		return 0;
	}
	else
	{
		CloseHandle(h);

		return -1;
	}
}

//0 success; -1 error
int xj_thread_detach(THREAD_HANDLE phandle)
{
	CloseHandle(phandle);
	return 0;
}

//0 success; -1 error
int xj_thread_join(THREAD_HANDLE phandle, void **pretcode)
{
	if(WaitForSingleObject(phandle, INFINITE) == WAIT_OBJECT_0)
	{
		if(pretcode)
			GetExitCodeThread(phandle, (unsigned long*)pretcode);
		
		return 0;
	}

	return -1;
}

void xj_thread_exit(unsigned int retval)
{
	_endthreadex(retval);
}

//创建线程，默认detach状态。//0:成功;    其他:失败
int pro_create_thread(THREAD_HANDLE *phandle, THREAD_ID *pid, XJ_THREAD_ROUTINE routine, void *param)
{
	int nRet=0;

	nRet=xj_thread_create(phandle, pid, routine, param);

	return nRet;
}

//mutex

//0 success; other error
int xj_mutex_init(MUTEX *mutex)
{
	InitializeCriticalSection(mutex);
	return 0;
}

//0 success; other error
int xj_mutex_lock(MUTEX *mutex)
{
	EnterCriticalSection(mutex);
	return 0;
}

//0 success; other error
int xj_mutex_unlock(MUTEX *mutex)
{
	LeaveCriticalSection(mutex);
	return 0;
}

//0 success; other error
int xj_mutex_destroy(MUTEX *mutex)
{
	DeleteCriticalSection(mutex);
	return 0;
}

//cond

//0 success; other error
int xj_cond_init(HCOND *cond)
{
	return 0;
}

//0 success; other error
int xj_cond_wait(MUTEX *mutex, HCOND *cond, int milliseconds)
{
	return 0;
}

//0 success; other error
int xj_cond_signal(HCOND *cond)
{
	return 0;
}

//0 success; other error
int xj_cond_broadcast(HCOND *cond)
{
	return 0;
}

//0 success; other error
int xj_cond_destroy(HCOND *cond)
{
	return 0;
}

//no name semaphore

//0 success; -1 error
int xj_noname_sem_init(HNSEM *sem, unsigned int value)
{
	*sem = CreateSemaphore(0, value, 0x7FFFFFFF, NULL);
	if(*sem == NULL)
		return -1;

	return 0;
}

//0 success; -1 error
int xj_noname_sem_destroy(HNSEM *sem)
{
	if(CloseHandle(*sem))
		return 0;
	return -1;
}

//0 success; -1 error
int xj_noname_sem_wait(HNSEM *sem)
{
	int ret = WaitForSingleObject(*sem, INFINITE);

	if(ret == WAIT_OBJECT_0)
		return 0;
	else
		return -1;
}

//0 success; -1 error
int xj_noname_sem_post(HNSEM *sem)
{
	if(ReleaseSemaphore(*sem, 1, 0))
		return 0;
	
	return -1;
}

//0 success; -1 error
int xj_noname_sem_getvalue(HNSEM *sem)
{
	/*
	long value=0;

	if(ReleaseSemaphore(*sem, 0, &value))
		return -1;
	else
		return value;
		*/
	return 0;
}

//dynamic linking loader

//0 success; -1 error
XJHANDLE xj_load_library(const char *filename)
{
	XJHANDLE handle;

	if (filename == NULL)
	{
		return NULL;
	}

	handle=LoadLibrary(filename); 

	return handle;
}

//0 success; -1 error
void *xj_get_addr(XJHANDLE handle, const char *module)
{
	void *ptr=NULL; 
    
    if (handle==NULL || module==NULL)
    { 
        return NULL; 
    }
    
    ptr=(void *)GetProcAddress((HINSTANCE)handle, module); 

	return ptr;
}

//0 success; -1 error
int xj_free_library(XJHANDLE handle)
{
	if (handle == NULL)
	{
		return -1;
	}
	
    FreeLibrary((HINSTANCE)handle);

	return 0;
}

//NULL无错误,否则有错误
char *xj_get_liberror()
{
	/*
	* ----- commented by qingch   4/8/2009 ------
  	return NULL;
	*/
	return strerror(errno);
}

//some useful functions

//0 success; other error
int xj_sleep(unsigned int milliseconds)
{
	Sleep(milliseconds);
	return 0;
}

//0 success; -1 error
int xj_getlocaltime(xj_systime *psystime)
{
	GetLocalTime(psystime);
	return 0;
}

//0 success; -1 error
int xj_setlocaltime(const xj_systime *psystime)
{
	if(SetLocalTime(psystime))
		return 0;

	return -1;
}

//0 yes; -1 no
int xj_pathfile_exist(const char *pathfile)
{
	if( _access(pathfile,0) != 0)
		return -1;
	
	return 0;
	/*
	if(PathFileExists(pathfile))
		return 0;
	else
		return -1;
		*/
}

//0 yes; -1 no
int xj_pathfile_exec(const char *pathfile)
{
	if( _access(pathfile,2) != 0)
		return -1;
	
	return 0;
}

//0 yes; -1 no
int xj_pathfile_rw(const char *pathfile)
{
	if( _access(pathfile,6) != 0)
		return -1;

	return 0;
}

int xj_get_file_size(const char *file_name)
{
	struct _stat file_desc;

	if (_stat(file_name, &file_desc) == 0)
	{
		return file_desc.st_size;
	}

	return -1;
}

BOOL xj_create_local_dir(char *szPath)  
{
	int			iIndex1=0,iIndex2=0;
	string		strFilePath;
	string		strFullFilePath;

	strFilePath = szPath;
	if (strFilePath.at(0) == '.')
	{
		iIndex2=strFilePath.find('\\', iIndex1+1);

		if(iIndex2==-1)
		{
			return FALSE;
		}

		iIndex1=iIndex2;
		strFullFilePath=".";
	}
	else 
	{
		if (xj_pathfile_exist(strFilePath.c_str())==0)
		{
			return TRUE;
		}
	}

	while(iIndex1!=-1)
	{
		iIndex2=strFilePath.find('\\', iIndex1+1);

		if (iIndex2>0)
		{
			strFullFilePath += strFilePath.substr(iIndex1, iIndex2-iIndex1);

			if (xj_pathfile_exist(strFullFilePath.c_str())!=0)
			{
				if (_mkdir((const char *)strFullFilePath.c_str())<0)
				{
					return FALSE;
				}
			}
		}
		else if(iIndex2==-1)
		{
			if((unsigned int)iIndex1!=strFilePath.size()-1)
			{
				strFullFilePath += strFilePath.substr(iIndex1, strFilePath.size()-1);
					
				if (xj_pathfile_exist(strFullFilePath.c_str())!=0)
				{
					if (_mkdir((const char *)strFullFilePath.c_str())<0)
					{
						return FALSE;
					}
				}
			}
		}

		iIndex1=iIndex2;
	}

	return TRUE;
}

#endif//OS_WINDOW

#ifdef OS_LINUX

//thread

//0 success; -1 error
int xj_thread_create(THREAD_HANDLE *phandle, THREAD_ID *pid, XJ_THREAD_ROUTINE routine, void *param, UINT stacksize)
{
	pthread_t t;

	if (pthread_create(&t, 0, routine, param) != 0)
		return -1;

	if (phandle)
	{
		*phandle = t;
		if (pid)
			*pid = t;
	}
	else
	{
		if (pid)
			*pid = t;
		pthread_detach(t);
	}

	return 0;
}

//0 success; -1 error
int xj_thread_detach(THREAD_HANDLE handle)
{
	return pthread_detach(handle);
}

//0 success; -1 error
int xj_thread_join(THREAD_HANDLE handle, void **pretcode)
{
	return pthread_join(handle, pretcode);
}

void xj_thread_exit(unsigned int retval)
{
	pthread_exit((void *)retval);
}

//创建线程，默认detach状态。//0:成功;    其他:失败
int pro_create_thread(THREAD_HANDLE *phandle, THREAD_ID *pid, XJ_THREAD_ROUTINE routine, void *param)
{
	int nRet=0;
	
	nRet=xj_thread_create(phandle, pid, routine, param);
	if(nRet==0)
		nRet=xj_thread_detach(*phandle);

	return nRet;
}

//mutex

//0 success; other error
int xj_mutex_init(MUTEX *mutex)
{
	pthread_mutexattr_t mt;

	pthread_mutexattr_init(&mt);
	pthread_mutexattr_settype(&mt, PTHREAD_MUTEX_RECURSIVE_NP);

	return pthread_mutex_init(mutex, &mt);
}

//0 success; other error
int xj_mutex_lock(MUTEX *mutex)
{
	return pthread_mutex_lock(mutex);
}

//0 success; other error
int xj_mutex_unlock(MUTEX *mutex)
{

	return pthread_mutex_unlock(mutex);
}

//0 success; other error
int xj_mutex_destroy(MUTEX *mutex)
{
	return pthread_mutex_destroy(mutex);
}

//cond

//0 success; other error
int xj_cond_init(HCOND *cond)
{
	return pthread_cond_init(cond, 0);
}

//0 success; other error
int xj_cond_wait(MUTEX *mutex, HCOND *cond, int milliseconds)
{
	int nRet=0;

	if (milliseconds == -1)
	{
		nRet = pthread_cond_wait(cond, mutex);
	}
	else
	{
		struct timeval 	tv;
		struct timespec tm;
		
		gettimeofday(&tv, 0);

		tm.tv_sec = tv.tv_sec + milliseconds / 1000;
		tm.tv_nsec = (milliseconds % 1000) * 1000 + tv.tv_usec;
		tm.tv_sec += tm.tv_nsec / 1000000;
		tm.tv_nsec = (tm.tv_nsec % 1000000) * 1000;

		nRet = pthread_cond_timedwait(cond, mutex, &tm);
	}

	if (nRet != 0)
	{
		if (nRet == ETIMEDOUT)
		{
			nRet=-1;//timeout
		}
		else
		{
			nRet=-2;//error
		}
	}

	return nRet;
}

//0 success; other error
int xj_cond_signal(HCOND *cond)
{
	return pthread_cond_signal(cond);
}

//0 success; other error
int xj_cond_broadcast(HCOND *cond)
{
	return pthread_cond_signal(cond);
}

//0 success; other error
int xj_cond_destroy(HCOND *cond)
{
	return pthread_cond_destroy(cond);
}

//no name semaphore

//0 success; -1 error
int xj_noname_sem_init(HNSEM *sem, unsigned int value)
{
	return sem_init(sem, 0, value);
}

//0 success; -1 error
int xj_noname_sem_destroy(HNSEM *sem)
{
	return sem_destroy(sem);
}

//0 success; -1 error
int xj_noname_sem_wait(HNSEM *sem)
{
	return sem_wait(sem);
}

//0 success; -1 error
int xj_noname_sem_post(HNSEM *sem)
{
	return sem_post(sem);
}

//0 success; -1 error
int xj_noname_sem_getvalue(HNSEM *sem)
{
	int value=0;

	sem_getvalue(sem, &value);
	return value;
}

//dynamic linking loader

//0 success; -1 error
XJHANDLE xj_load_library(const char *filename)
{
	void *handle;
	
	if (filename == NULL)
		return NULL;

	handle = dlopen(filename, RTLD_LAZY);

	if (!handle)
		return NULL;
	
	return handle;
}

//0 success; -1 error
void *xj_get_addr(XJHANDLE handle, const char *module)
{
	void *ptr=NULL;

	if (handle == NULL || module == NULL)
		return NULL;

	ptr = (void *)dlsym(handle, module);

	if (ptr == NULL)
		return NULL;

	return ptr;
}

//0 success; -1 error
int xj_free_library(XJHANDLE handle)
{
	if (handle == NULL)
		return -1;

	if (dlclose(handle) != 0)
		return -1;
	else
		return 0;
}

//NULL无错误,否则有错误
char *xj_get_liberror()
{
	return dlerror();
}

//some useful functions

//0 success; other error
int xj_sleep(unsigned int milliseconds)
{
	int	nRet;
	struct  timeval tm;

	tm.tv_sec = milliseconds / 1000;
	tm.tv_usec = (milliseconds % 1000)*1000;
	nRet = select(0, 0, 0, 0, &tm);

	return nRet;
}

//0 success; -1 error
int xj_getlocaltime(xj_systime *psystime)
{
	int	nRet=0;
	time_t 	t;
	struct 	tm *ptm=NULL;
	struct 	timeval tv;

	nRet = gettimeofday(&tv, 0);

	if (nRet != 0)
		return -1;
	
	t = tv.tv_sec;

	ptm = localtime(&t);

	if (ptm == NULL)
		return -1;

	psystime->wYear = ptm->tm_year + 1900;
	
	psystime->wMonth = ptm->tm_mon + 1;
	psystime->wDay = ptm->tm_mday;
	psystime->wDayOfWeek = ptm->tm_wday;
	psystime->wHour = ptm->tm_hour;
	psystime->wMinute = ptm->tm_min;
	psystime->wSecond = ptm->tm_sec;
	psystime->wMilliseconds = tv.tv_usec / 1000;

	return 0;
}

//0 success; -1 error
int xj_setlocaltime(const xj_systime *psystime)
{
	time_t t;
	struct tm mtm;
	struct timeval tv;

	mtm.tm_year = psystime->wYear - 1900;
	mtm.tm_mon = psystime->wMonth - 1;
	mtm.tm_mday = psystime->wDay;
	mtm.tm_wday = psystime->wDayOfWeek;
	mtm.tm_hour = psystime->wHour;
	mtm.tm_min = psystime->wMinute;
	mtm.tm_sec = psystime->wSecond;
	mtm.tm_isdst = 0;

	t = mktime(&mtm);
	if (t == -1)
		return -1;

	tv.tv_sec = t;
	tv.tv_usec = psystime->wMilliseconds * 1000;

	return settimeofday(&tv, 0);
}

//0 yes; -1 no
int xj_pathfile_exist(const char *pathfile)
{
	if (access(pathfile, F_OK) != 0)
	{
		return -1;
	}

	return 0;
}

//0 yes; -1 no
int xj_pathfile_exec(const char *pathfile)
{
	if (access(pathfile, X_OK) != 0)
	{
		return -1;
	}

	return 0;
}

//0 yes; -1 no
int xj_pathfile_rw(const char *pathfile)
{
	if (access(pathfile, R_OK|W_OK) != 0)
	{
		return -1;
	}

	return 0;
}

int xj_get_file_size(const char *file_name)
{
	struct stat file_desc;

	if (stat(file_name, &file_desc) == 0)
	{
		return file_desc.st_size;
	}

	return -1;
}

BOOL xj_create_local_dir(char *szPath)  
{
	int			iIndex1=0,iIndex2=0;
	string		strFilePath;
	string		strFullFilePath;

	strFilePath = szPath;
	if (strFilePath.at(0) == '.')
	{
		iIndex2=strFilePath.find('/', iIndex1+1);

		if(iIndex2==-1)
		{
			return FALSE;
		}

		iIndex1=iIndex2;
		strFullFilePath=".";
	}
	else 
	{
		if (strFilePath.at(0) != '/')
		{
			strFilePath = "/"+strFilePath;
		}	
	
		if (xj_pathfile_exist(strFilePath.c_str())==0)
		{
			return TRUE;
		}
	}

	while(iIndex1!=-1)
	{
		iIndex2=strFilePath.find('/', iIndex1+1);

		if (iIndex2>0)
		{
			strFullFilePath += strFilePath.substr(iIndex1, iIndex2-iIndex1);

			if (xj_pathfile_exist(strFullFilePath.c_str())!=0)
			{
				if (mkdir((const char *)strFullFilePath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO)<0)
				{
					return FALSE;
				}
			}
		}
		else if(iIndex2==-1)
		{
			if((unsigned int)iIndex1!=strFilePath.size()-1)
			{
				strFullFilePath += strFilePath.substr(iIndex1, strFilePath.size()-1);
					
				if (xj_pathfile_exist(strFullFilePath.c_str())!=0)
				{
					if (mkdir((const char *)strFullFilePath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO)<0)
					{
						return FALSE;
					}
				}
			}
		}

		iIndex1=iIndex2;
	}

	return TRUE;
}

#endif//OS_LINUX

BOOL isCommenceApi(char *pszBuff)
{
	int		nLen=0;
	char	szTempBuff[XJLIB_MAX_CHAR_BUFF];
	
	if (NULL == pszBuff)
	{
		return TRUE;	// 空串，判为行注释。
	}
	else
	{
		memset(szTempBuff, 0, XJLIB_MAX_CHAR_BUFF);
		nLen = strlen(pszBuff);
	}

	if (nLen >= XJLIB_MAX_CHAR_BUFF)
	{
		strncpy(szTempBuff, pszBuff, XJLIB_MAX_CHAR_BUFF-1);
	}
	else
	{
		strncpy(szTempBuff, pszBuff, nLen);
	}
	
	if (strcmp(szTempBuff, (char*)"sum") == 0) 
	{
		return TRUE;	// sum，判为行注释。
	}
	
	if ((szTempBuff[0] == ';') || 
		((szTempBuff[0] == '/') && (szTempBuff[1] == '/')) || 
		((szTempBuff[0] == '/') && (szTempBuff[1] == '*'))) 
	{
		return TRUE;	// ;、//和/*，判为行注释。
	}
	else
	{	
		return FALSE;
	}
}

unsigned int GetPrivateProfileStr(
						char	*pszSectorName,
						char	*pszKeyName,
						char	*pszDefaultStr,
						char	*pszReturnedStr,
						unsigned int  uSize,
						char	*pszFileName
						)
{
	unsigned int	i=0;
	BOOL	bHead=FALSE;
	FILE	*pFile=NULL;
	unsigned int	uReturnedLen=0;
	char	st[XJLIB_MAX_CHAR_BUFF],st1[XJLIB_MAX_CHAR_BUFF];
	char    st2[XJLIB_MAX_CHAR_BUFF],st3[XJLIB_MAX_CHAR_BUFF];
	char	string[XJLIB_MAX_CHAR_BUFF];

	// 用默认串初始化目的缓冲区
	uReturnedLen = strlen(pszDefaultStr);
	if (uReturnedLen >= uSize)
	{
		uReturnedLen = uSize-1;
	}
	memset(pszReturnedStr, 0, uSize);
	strncpy(pszReturnedStr, pszDefaultStr, uReturnedLen);
	
	pFile = fopen(pszFileName, "r");
	if (NULL == pFile)
	{
		return uReturnedLen;	// 返回默认串长度
	}

	while (fgets(st, XJLIB_MAX_CHAR_BUFF, pFile) != NULL)	// 从文件取一个字符串
	{
		if (isCommenceApi(st) == TRUE)
		{
			continue ;	// 行注释
		}

		sscanf(st, "%s,%s", st1, st2);	// 格式化到串st1和st2

		//查找section键名
		if (FALSE == bHead)
		{
			if (('[' == st1[0]) && (']' == st1[strlen(st1)-1])) 
			{
				strcpy(st3, (char*)"[");
				strcat(st3, pszSectorName);
				strcat(st3, (char*)"]");

				if (strcmp((char*)st1, (char*)st3) == 0)
				{
					bHead = TRUE;	// 发现section
				}
			}

			continue;
		}

		if ('[' == st1[0]) 
		{
			fclose(pFile);	// 该section没搜索到Keyname键名
			return uReturnedLen;
		}

		for (i=0; i<strlen(st); i++)	// 循环搜索Keyname键名
		{
			string[i] = st1[i];

			if ('=' == st1[i])
			{
				string[i] = '\0';
				if (strcmp(pszKeyName, (char*)string) != 0)
				{
					strcpy(string, (char*)"");
					break;	//跳过一个键
				}
				else	//找到
				{
					strcpy((char*)string, &st1[i+1]);	// 拷贝串
					sscanf(string, "%s%s", st2, st3);	// 格式化串

					uReturnedLen = strlen(st2);
					if (uReturnedLen >= uSize)
					{
						uReturnedLen = uSize-1;
					}
					memset(pszReturnedStr, 0, uSize);
					strncpy(pszReturnedStr, (char*)st2, uReturnedLen);	// 取串值

					fclose(pFile);
					return uReturnedLen;
				}
			}
		}
	}

	fclose(pFile);
	return uReturnedLen;	// 搜索文件结束，并返回。
}

int GetPrivateProfileint(
						 char	*pszSectorName,
						 char	*pszKeyName,
						 int	nDefaultValue,
						 char	*pszFileName
						 )
{
	unsigned int	i=0;
	BOOL	bHead=FALSE;
	FILE	*pFile=NULL;
	int	nReturnedLen=0;
	char	st[XJLIB_MAX_CHAR_BUFF],st1[XJLIB_MAX_CHAR_BUFF];
	char    st2[XJLIB_MAX_CHAR_BUFF],st3[XJLIB_MAX_CHAR_BUFF];
	char	string[XJLIB_MAX_CHAR_BUFF];//define buffer

	// 用默认值初始化归还值
	nReturnedLen = nDefaultValue;
	
	pFile = fopen(pszFileName, "r");
	if (NULL == pFile)
	{
		return nReturnedLen;
	}

	while (fgets(st, XJLIB_MAX_CHAR_BUFF, pFile) != NULL)	//get str form file
	{
		if (isCommenceApi(st) == TRUE)
		{
			continue ;
		}

		sscanf(st, "%s,%s", st1, st2);

		if (FALSE == bHead)
		{
			if (('[' == st1[0]) && (']' == st1[strlen(st1)-1])) 
			{
				strcpy(st3, (char*)"[");
				strcat(st3, pszSectorName);
				strcat(st3, (char*)"]");

				if (strcmp((char*)st1, (char*)st3) == 0)
				{
					bHead = TRUE;
				}
			}

			continue;
		}

		if ('[' == st1[0]) //未搜索到
		{
			fclose(pFile);
			return nReturnedLen;
		}

		for (i=0; i<strlen(st); i++)	//
		{
			string[i] = st1[i];

			if ('=' == st1[i])
			{
				string[i] = '\0';
				if (strcmp(pszKeyName, (char*)string) != 0)	//
				{
					strcpy(string, (char*)"");
					break;
				}
				else	//找到
				{
					strcpy((char*)string, &st1[i+1]);	//
					sscanf(string, "%s%s", st2, st3);
					nReturnedLen = atoi(st2);

					fclose(pFile);
					return nReturnedLen;
				}
			}
		}
	}

	fclose(pFile);
	return nReturnedLen;
}

int GetPrivateProfileEqualint(
						 char	*pszKeyName,
						 int	nDefaultValue,
						 char	*pszFileName
						 )
{
	unsigned int	i=0;
	FILE	*pFile=NULL;
	int	nReturnedLen=0;
	char	st[XJLIB_MAX_CHAR_BUFF],st1[XJLIB_MAX_CHAR_BUFF];
	char    st2[XJLIB_MAX_CHAR_BUFF],st3[XJLIB_MAX_CHAR_BUFF];
	char	string[XJLIB_MAX_CHAR_BUFF];//define buffer

	// 用默认值初始化归还值
	nReturnedLen = nDefaultValue;
	
	pFile = fopen(pszFileName, "r");
	if (NULL == pFile)
	{
		return nReturnedLen;
	}

	while (fgets(st, XJLIB_MAX_CHAR_BUFF, pFile) != NULL)	//get char form file
	{
		if (isCommenceApi(st) == TRUE)
		{
			continue ;
		}

		sscanf(st, "%s,%s", st1, st2);
		for (i=0; i<strlen(st); i++)	//circle
		{
			string[i] = st1[i];

			if ('=' == st1[i])
			{
				string[i] = '\0';
				if (strcmp(pszKeyName, (char*)string) != 0)//judge
				{
					strcpy(string, (char*)"");
					break;
				}
				else	//找到
				{
					strcpy((char*)string, &st1[i+1]);//copy string
					sscanf(string, "%s%s", st2, st3);
					nReturnedLen = atoi(st2);

					fclose(pFile);
					return nReturnedLen;
				}
			}
		}
	}

	fclose(pFile);
	return nReturnedLen;
}

int xj_modify_time(xj_systime *psystime=NULL, unsigned short wMilliseconds=0, char op='+')
{
	time_t t;
	struct tm mtm;
	struct timeval tv;
	struct tm *ptm=NULL;
	unsigned short seconds=0;

	if (psystime==NULL)
	{
		return -1;
	}

	mtm.tm_year = psystime->wYear - 1900;
	mtm.tm_mon = psystime->wMonth - 1;
	mtm.tm_mday = psystime->wDay;
	mtm.tm_wday = psystime->wDayOfWeek;
	mtm.tm_hour = psystime->wHour;
	mtm.tm_min = psystime->wMinute;
	mtm.tm_sec = psystime->wSecond;
	mtm.tm_isdst = 0;

	t = mktime(&mtm);
	if (t == -1)
		return -1;

	tv.tv_sec = t;	//秒
	tv.tv_usec = psystime->wMilliseconds*1000;	//微秒

	if (op=='+')
	{
		tv.tv_usec += wMilliseconds*1000;	//微秒
		
		tv.tv_sec += tv.tv_usec/1000000;	//秒
		tv.tv_usec = tv.tv_usec%1000000;	//微秒
	}
	else if (op=='-')
	{
		seconds=wMilliseconds/1000;			//秒
		wMilliseconds=wMilliseconds%1000;	//毫秒

		tv.tv_sec -= seconds;				//秒

		int nTempValue = wMilliseconds*1000;	//微秒

		if (tv.tv_usec >= nTempValue)
		{
			tv.tv_usec -= nTempValue;
		}
		else
		{
			tv.tv_sec -= 1;
			tv.tv_usec += 1000000;
			tv.tv_usec -= nTempValue;
		}
	}
	else
	{
		return -1;
	}

	
	time_t t_temp(tv.tv_sec);
	ptm = localtime(&t_temp);

	//ptm = localtime(static_cast<time_t*>(&(tv.tv_sec)));

	if (ptm == NULL)
		return -1;

	psystime->wYear = ptm->tm_year + 1900;
	
	psystime->wMonth = ptm->tm_mon + 1;
	psystime->wDay = ptm->tm_mday;
	psystime->wDayOfWeek = ptm->tm_wday;
	psystime->wHour = ptm->tm_hour;
	psystime->wMinute = ptm->tm_min;
	psystime->wSecond = ptm->tm_sec;
	psystime->wMilliseconds = tv.tv_usec / 1000;
	
	return 0;
}

time_t xjtime_to_timet(xj_systime *psystime=NULL)
{
	tm tm_systime;

	if (psystime==NULL)
		return 0;

	tm_systime.tm_year= psystime->wYear-1900;
	tm_systime.tm_mon = psystime->wMonth-1;
	tm_systime.tm_mday= psystime->wDay;
	tm_systime.tm_hour= psystime->wHour;
	tm_systime.tm_min = psystime->wMinute;
	tm_systime.tm_sec = psystime->wSecond;
    tm_systime.tm_isdst = -1;
	tzset();
	return mktime(&tm_systime);
}
