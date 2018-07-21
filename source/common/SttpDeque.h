//
// Deque.h
//
// This is a STTP deque Operator  class
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//
#ifndef INCLUDE_H_DEQUE
#define INCLUDE_H_DEQUE

#include "define.h"
#include "LogFile.h"
#include "Lock.h"
#include "SttpDefine.h"

using namespace std;

class CSttpDeque
{
public:

	//������������
	CSttpDeque(int m_Size = 50);
	virtual ~CSttpDeque();

	//ɾ��ͷһ��Ԫ��
	bool RemoveHead();
 
	//����һ��Ԫ�ص�β��
	bool Insert(STTP_DEQUE_MSG &item); 

	//ȡ��һ��Ԫ�ز�ɾ��
	bool Extract(STTP_DEQUE_MSG &item,int nIndex=0);

	//ȡ��һ��Ԫ�ص���ɾ��
	bool Fetch(STTP_DEQUE_MSG &item,int nIndex=0);

	//�������Ԫ��
	bool Clear();

	//ȷ�ϸö����Ƿ���Ԫ��
	bool IsEmpty() ;

	//���ظö��е�ǰԪ�ظ���
	int size() ;

	//���ö��е��������
	bool SetMaxSize(UINT nNum);
	
	//������־����
	void SetLogLevel(int nLevel);
	
	//�޸�Ԫ������
	bool SetAt(STTP_DEQUE_MSG item,int nIndex=0);
	
	//�����־�������
	int         GetLogLevel();

	//������־·��
	void        SetLogPath(char *pPath);
	
	//��ʼ����־�ļ�
	bool        SetLogFileName(char * pFileName);

private:

	//���������������Ԫ�ظ���
	 UINT                m_MaxSize;

	 //list STL
	 list <STTP_DEQUE_MSG>    m_mDeque;

	 //��
	 CLock               m_Lock;

	 //��־��¼
	 CLogFile            m_LogFile;

protected:

};

#endif