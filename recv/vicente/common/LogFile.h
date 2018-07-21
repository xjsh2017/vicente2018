//
// LogFile.h
//
// This is a Log  operator  class
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//

#ifndef LOG_FILE_H
#define LOG_FILE_H

#include "define.h"
#include "Lock.h"
#include "XJFile.h"
#include "xjlib.h"

using namespace std;

/** @brief             ��־�ļ�Ĭ������(��λ:�ֽ�)*/
#define  MAX_LOG_FILE_SIZE        0x800000

/** @brief              ÿһ��д����־������ֽ���*/
#define EVERY_LOG_MAX_LENGTH    100000

class CLogFile
{
public:

	CLogFile();
	virtual ~CLogFile();

	enum LogLevel {
		  nolog     =      0,          //����¼��־���ļ�
          error     =      1,          // ������־
          warning   =      2,          // ������־
		  trace     =      3,          // ������־
		  };

	//���������־����
	virtual bool Clear();

	//������־����
	virtual bool SetLogLevel(int nLevel);

	//�����־����
	virtual int  GetLogLevel();

	//������־���·��
	virtual bool SetLogPath(const char * pszPath);

	//������־�ļ���С(������λ:Kb)
	virtual bool SetLogSize(long nSize);

	
	//���ļ�
	virtual BOOL Open();
	virtual BOOL Open(const char * pszFileName);

	//�ر��ļ�
	virtual bool Close();

	//����ָ���ļ����ʽ������
	virtual int  FormatAdd(int nCurLevel,const char* formatString, ...);	

	//����ָ���ļ��������ַ���
	virtual void Add(const char * pchLog,int nCurLevel= CLogFile::trace);
	
	//����ָ���ĳ��ȼ�¼ָ��������������
	virtual void AddFixLen(const char * pchLog,int nLen,int nCurLevel = CLogFile::trace);

private:
	CXJFile   	*m_File;
	void		Flush( const char* data,int nCurLevel);
	void        Flush( const char* data,int nLen,int nCurLevel);

	//����־�������
	int m_nLevel;
	
	//�ļ���
    char m_FileName[FILE_NAME_MAX_LEN];

	//�ļ�·��
	char m_szLogPath[FILE_NAME_MAX_LEN];

	//������
	CLock     m_Lock;

	//�ļ���ǰ��С(��λ:�ֽ�)
	long      m_nCurrenSize;

	//�ļ�ָ�����Դﵽ�����ֵ(��λ:�ֽ�)
	long      m_nMaxSize;
};

#endif 