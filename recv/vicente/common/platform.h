#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "globaldef.h"
#include "include.h"

#ifdef OS_WINDOWS

	typedef DWORD		HPROCESS;
	typedef HANDLE		HRSPORT;

#endif

/*IPC objec*/
#ifdef OS_LINUX

	typedef int				HSEM;
	typedef sem_t			HNSEM;
	typedef pid_t			HPROCESS;
	typedef pthread_t		HTHREAD;
	typedef pthread_mutex_t	HMUTEX;	
	typedef pthread_cond_t  HCOND;
	typedef void*			XJHANDLE;
	typedef int				HRSPORT;

	typedef void *(*XJ_THREAD_ROUTINE)(void *);

#endif /*OS_LINUX*/

#ifdef OS_WINDOWS

	typedef SYSTEMTIME xj_systime;

#endif	

#ifdef OS_LINUX
	typedef struct
    {
		uint16 wYear;
		uint16 wMonth;
		uint16 wDayOfWeek;
		uint16 wDay;
		uint16 wHour;
		uint16 wMinute;
		uint16 wSecond;
		uint16 wMilliseconds;
	}xj_systime;
	
#endif /*OS_LINUX*/

#endif /*_PLATFORM_H_*/
