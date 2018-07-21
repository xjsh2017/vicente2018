//
// File.h
//
// This is a File Operator  class
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//
#ifndef CFILE_H
#define CFILE_H

#include "define.h"

class CXJFile
{
private:
    
	//文件指针
	FILE * pfile ;

	char   m_chError[FILE_ERROR_MAX_LEN];

	char   m_chName[FILE_NAME_MAX_LEN];

protected:

public:

	CXJFile();

	~CXJFile();

	enum OpenFlags{
		//file mode
        modeCreate         =       0  ,         //0000 0000
        modeRead           =       1  ,         //0000 0001
        modeWrite          =       2  ,         //0000 0010
        modeReadWrite      =       4  ,         //0000 0100
        modeNoTruncate     =       8  ,         //0000 1000
        shareDenyNone      =       16 ,         //0001 0000
        shareDenyWrite     =       32 ,         //0010 0000
        shareDenyRead      =       64 ,         //0100 0000
        shareExclusive     =       128,         //1000 0000
	};

	enum PosFlags{
		//file position 
		begin                  =   0,          //文件开始
        current                =   1,          //文件当前位置
        end                    =   2,         //文件尾部
	};

	/* Input/Output */

	//打开文件
	BOOL Open(const char *, UINT nOpenFlags);

	//读文件
	UINT Read(void * lpbuf, UINT nCount);

	//写文件
	UINT Write(const void * lpBuf,UINT nCount);
    
	//强制写入
	BOOL Flush();

	//关闭文件
	BOOL Close();

	/*position */

	BOOL Seek(long lOff,UINT nFrom);

	BOOL SeekToBegin();

	BOOL SeekToEnd();

	long GetLength();

	/* 错误描述 */
	char * GetError();
};

#endif