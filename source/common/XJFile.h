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
    
	//�ļ�ָ��
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
		begin                  =   0,          //�ļ���ʼ
        current                =   1,          //�ļ���ǰλ��
        end                    =   2,         //�ļ�β��
	};

	/* Input/Output */

	//���ļ�
	BOOL Open(const char *, UINT nOpenFlags);

	//���ļ�
	UINT Read(void * lpbuf, UINT nCount);

	//д�ļ�
	UINT Write(const void * lpBuf,UINT nCount);
    
	//ǿ��д��
	BOOL Flush();

	//�ر��ļ�
	BOOL Close();

	/*position */

	BOOL Seek(long lOff,UINT nFrom);

	BOOL SeekToBegin();

	BOOL SeekToEnd();

	long GetLength();

	/* �������� */
	char * GetError();
};

#endif