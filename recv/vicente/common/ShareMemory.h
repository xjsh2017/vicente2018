/********************************************************************
	created:	2009/04/23
	created:	23:4:2009   9:51
	file base:	ShareMemory
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#pragma once

#include <stdio.h>
#include <stdlib.h>  
#include <string>

#ifdef OS_WINDOWS

	#include <windows.h>

#endif

#ifdef OS_LINUX 
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/mman.h>
	#include <sys/types.h>  
	#include <sys/stat.h>

	#define FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)  
	
#ifdef OS_HPUNIX
		const char ipc_path[] = "./";
#else
		const char ipc_path[] = "/";
#endif	
	
#endif

using namespace std;

class CSharedMemory
{
public:
	enum
	{
		// Return values of the class-functions.
		MEM_ERROR_UNKNOWN		= -1,
		MEM_SUCCESS				= 0,
		MEM_ERROR_CLOSED		= 1,
		MEM_ERROR_OPENED		= 2,
		MEM_ERROR_ISEXISTS		= 3,
		MEM_ERROR_DATASIZE		= 4,
		MEM_ERROR_CREATEFAILED	= 5,
		MEM_ERROR_OPENFAILED	= 6,
		MEM_ERROR_MAPPEDFAILED	= 7,
		MEM_ERROR_REMOVEFAILED	= 8,
	};

    CSharedMemory()
    {
#ifdef OS_WINDOWS
		mappingFile = NULL;

#endif
	
#ifdef OS_LINUX 
		mappingFile = -1;
       
#endif
        pAddress = NULL;
    }

    ~CSharedMemory()
	{
		Delete();
	}
	
    inline int Create( string pName , size_t Length )
	{
#ifdef OS_WINDOWS
        //    检查参数
        if( NULL != pAddress || NULL != mappingFile )
		{
			printf("[Create]find share memory opened！\n");
			return MEM_ERROR_OPENED;
		}

        mappingFile = CreateFileMapping( INVALID_HANDLE_VALUE , NULL , PAGE_READWRITE , 0 , Length , pName.c_str() );

        if( mappingFile != NULL && GetLastError() == ERROR_ALREADY_EXISTS )
        {
            // 已经存在
			printf("[Create]CreateFileMapping，share memory existed！\n");

            CloseHandle( mappingFile );
            mappingFile = NULL;
            return MEM_ERROR_ISEXISTS;
        }
	    else if( NULL == mappingFile )
        {
            // 创建失败
			printf("[Create]CreateFileMapping，open share memory failed！\n");
            return MEM_ERROR_CREATEFAILED;
        }
	
        //    映射内存
        pAddress = MapViewOfFileEx( mappingFile , FILE_MAP_ALL_ACCESS , 0 , 0 , 0 , 0 );
		
        if( NULL != pAddress )
		{
			printf("[Create]create share memory succeed！\n");
			return MEM_SUCCESS; 
		}

		printf("[Create]MapViewOfFileEx failed，create share memory failed！\n");

        CloseHandle( mappingFile );
        mappingFile = NULL;
		
        return MEM_ERROR_MAPPEDFAILED;
#endif

#ifdef OS_LINUX 
        //    检查参数
        if( NULL != pAddress || -1 != mappingFile )
		{
			printf("[Create]find share memory opened！\n");
			return MEM_ERROR_OPENED;
		}

		int flags = O_RDWR| O_CREAT | O_EXCL;	
		
		ipc_name = ipc_path;
		ipc_name += pName;
		mappingFile = shm_open(ipc_name.c_str(),flags,FILE_MODE);

        if( -1 == mappingFile )
        {
			if( errno != EEXIST){
				// 创建失败
				printf("[Create]shm_open，open share memory failed！\n");
				return MEM_ERROR_CREATEFAILED;
			}
            // 已经存在
			printf("[Create]shm_open， share memory existed！\n");
            return MEM_ERROR_ISEXISTS;
        }

		ftruncate(mappingFile,Length);

		pAddress= mmap(NULL, Length, PROT_READ|PROT_WRITE, MAP_SHARED, mappingFile, 0);
		//close(mappingFile);

        if( NULL != pAddress )
		{
			printf("[Create]create share memory succeed（filemapping：%d）！\n", 
				mappingFile);
			return MEM_SUCCESS; 
		}

		printf("[Create]mmap failed，create share memory failed！\n");

		if (-1 != mappingFile)
		{
			close(mappingFile);
		}
		
		mappingFile = -1;
        return MEM_ERROR_MAPPEDFAILED;
#endif
	}

    inline int Open( string pName , bool Write = false )
	{
#ifdef OS_WINDOWS
        //    检查参数
        if( NULL != pAddress || NULL != mappingFile )
		{
			printf("[Open]find share memory already opened！\n");
			return MEM_ERROR_OPENED;
		}
		
        //    打开共享内存
		
        DWORD dwAccess = Write ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ ;
		
        mappingFile = OpenFileMapping( dwAccess , FALSE , pName.c_str() );

		if( NULL == mappingFile )
        {
            // 打开失败
			printf("[Open]OpenFileMapping，open share memory fail！\n");
            return MEM_ERROR_OPENFAILED;
        }
		
        //    映射内存
        //pAddress = MapViewOfFileEx( mappingFile , FILE_MAP_ALL_ACCESS , 0 , 0 , 0 , 0 );
		pAddress = MapViewOfFileEx( mappingFile , dwAccess , 0 , 0 , 0 , 0 );

        if( NULL != pAddress )
		{
			printf("[Open]open share memory succeed！\n");
			return MEM_SUCCESS; 
		}
		
		printf("[Open]MapViewOfFileEx failed，open share memory fail！\n");

        CloseHandle( mappingFile );
        mappingFile = NULL;
		
        return MEM_ERROR_MAPPEDFAILED;
#endif
		
#ifdef OS_LINUX 
        //    检查参数
        if( NULL != pAddress || -1 != mappingFile )
		{
			printf("[Open]find share memory already opened！\n");
			return MEM_ERROR_OPENED;
		}
		
		int flags = Write ? O_RDWR : O_RDONLY;	
		int prot = Write ? PROT_READ|PROT_WRITE : PROT_READ;

		ipc_name = ipc_path;		
		ipc_name += pName;
		mappingFile = shm_open(ipc_name.c_str(), flags, FILE_MODE);	
		
        if( -1 == mappingFile )// 打开失败
        {
			printf("[Open]shm_open，open share memory fail！\n");
            return MEM_ERROR_OPENFAILED;
        }
		
		struct stat lsstat;
		fstat(mappingFile, &lsstat);
		
		pAddress= mmap(NULL, lsstat.st_size, prot, MAP_SHARED, mappingFile, 0);
		//close(mappingFile);

        if( NULL != pAddress )
		{
			printf("[Open]open share memory succeed（mappingfile：%d）！\n", 
				mappingFile);
			return MEM_SUCCESS; 
		}

		printf("[Open]mmap failed，open share memory fail！\n");

		if (-1 != mappingFile)
		{
			close(mappingFile);
		}

		mappingFile = -1;
        return MEM_ERROR_MAPPEDFAILED;
#endif
	}

    inline int Close( )
    {
        return Delete( );
    }

    inline void * Address()
    {
        return pAddress;
    }

    static bool Exists( string pName)
    {
        CSharedMemory ShareMem;
		
        return ((MEM_SUCCESS == ShareMem.Open( pName ))?true:false);
    }

    static int Remove(string pName )
    {
#ifdef OS_WINDOWS
		return MEM_SUCCESS;
#endif
		
#ifdef OS_LINUX 
		string px_ipc_name(ipc_path);
		px_ipc_name += pName;
        return ((0 == shm_unlink(px_ipc_name.c_str()))?MEM_SUCCESS:false);
#endif
    }

private:
    inline int Delete()
	{
#ifdef OS_WINDOWS
        //    检查参数
        if( NULL == pAddress || NULL == mappingFile )
		{
			printf("[Delete]find share memory not opened！\n");
			return MEM_ERROR_CLOSED;
		}

        //    删除共享
        UnmapViewOfFile( pAddress );
        CloseHandle( mappingFile );
		
        pAddress = NULL;
        mappingFile = NULL;
		
        return MEM_SUCCESS;
#endif

#ifdef OS_LINUX 
        //    检查参数
        if( (NULL == pAddress) || (-1 == mappingFile) )
		{
			printf("[Delete]find share memory not opened！\n");
			return MEM_ERROR_CLOSED;
		}

        //    删除共享
		struct stat lsstat;
		fstat(mappingFile,&lsstat);
		munmap(pAddress,lsstat.st_size);
		shm_unlink(ipc_name.c_str());

		if (-1 != mappingFile)
		{
			close(mappingFile);
		}

        pAddress = NULL;
        mappingFile = -1;

        return MEM_SUCCESS;
#endif

	}

protected:
#ifdef OS_WINDOWS
    HANDLE    mappingFile;
    LPVOID    pAddress;
    //DWORD    dwAccess;
#endif
	
#ifdef OS_LINUX 
    int		  mappingFile;
    void*     pAddress;
	string    ipc_name;
#endif
};