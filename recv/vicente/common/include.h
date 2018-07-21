#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#ifdef  OS_WINDOWS

#	include <stdio.h>
#	include <windows.h>
#	include <string.h>

#endif/*OS_WINDOWS*/

#ifdef OS_LINUX

#	include <unistd.h>
#	include <errno.h>
#	include <stdio.h>
#	include <pthread.h>
#	include <semaphore.h>//noname_semaphore
#	include <sys/types.h>
#	include <sys/ipc.h>
#	include <sys/sem.h>
#	include <sys/time.h>
#	include <sys/select.h>
#	include <sys/wait.h>
#	include <signal.h>
#	include <dlfcn.h>  
#	include <sys/stat.h>   
#	include <fcntl.h>
#	include <termios.h>      

#endif/*OS_LINUX*/

#endif/*_INCLUDE_H_*/
