//
// Lock.h
//
// This is a multithreading lock Operator  class
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//

#ifndef LOCK_H_HEADER_INCLUDED_BAD5A27F
#define LOCK_H_HEADER_INCLUDED_BAD5A27F

#include "define.h"

//##ModelId=4519DA5403C4
//##Documentation
//## �������࣬����̼߳����������
class CLock
{
  public:
	//���캯��
	 CLock();

	//��������
	~CLock();

	//��ʼ����
    //##ModelId=4519E3D00375
    BOOL init(char * phost = NULL);

	//ɾ����
    //##ModelId=4519E3E701F2
    BOOL del();

	//�����ȴ�ֱ��ȡ��ӵ��Ȩ
    //##ModelId=4519E3F40088
    BOOL lock();

	//�����Ƿ�ȡ��ӵ��Ȩ,�������Ϸ���
	BOOL trylock();

	//�ó�ӵ��Ȩ
    //##ModelId=4519E40E003F
    BOOL unlock();

  private:
    //##ModelId=4519E338018C
    MUTEX m_mutex;

	//��mutex����������
	char  ch_host[FILE_NAME_MAX_LEN];

	//��ʼ����־
	BOOL  bInit;

};



#endif /* LOCK_H_HEADER_INCLUDED_BAD5A27F */
