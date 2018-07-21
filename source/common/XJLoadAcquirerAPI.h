/********************************************************************
	created:	2011/11/11
	created:	11:11:2011   14:03
	filename: 	\VICENTE\CODE\COMMON\XJLoadAcquirerAPI.h
	file path:	\VICENTE\CODE\COMMON
	file base:	XJLoadAcquirerAPI
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/
#ifndef XJLoadAcquirerAPI_h__
#define XJLoadAcquirerAPI_h__

#pragma   warning   (disable   :   4275)

#include "define.h"
#include "LogFile.h"

typedef	struct _STRUCT_XJLOADACQUIRER{
	int	 iProThreadWeight;					//�����߳���ԴȨ��
	int  iProHandlerWeight;					//���̾����ԴȨ��
	int  iProCPUWeight;						//����CPU��ԴȨ��
	int  iProMemoryWeight;					//�����ڴ���ԴȨ��
	int	 iSrvThreadWeight;					//�������߳���ԴȨ��
	int  iSrvHandlerWeight;					//�����������ԴȨ��
	int  iSrvCPUWeight;						//������CPU��ԴȨ��
	int  iSrvMemoryWeight;					//�������ڴ���ԴȨ��
	bool bReserved;							//��������
	int  iReserved;							//��������
	char reservedStr[256];					//��������
}stXJLoadAcquirer;


#ifdef OS_LINUX

/** @brief            ��������ƶ���*/
#define LIB_LOADACQUIRER_NAME              "libXJLoadAcquirer.so"

#endif

#ifdef OS_WINDOWS

/** @brief            ��������ƶ���*/
#define LIB_LOADACQUIRER_NAME              "XJLoadAcquirer.dll"

#endif

    /*
     *  @brief   	�������زɼ���
     *  @param 		[In]const stXJLoadAcquirer&  �ӿڲ���
     *  @return 	int 0-�ɹ� -1-ʧ��
     */

typedef	int (*PStartLoadAcquirer)(const stXJLoadAcquirer& , CLogFile& );
    /*
     *  @brief   	ֹͣ���زɼ���
     *  @return 	int 0-�ɹ� -1-ʧ�� 
     */

typedef	int (*PStopLoadAcquirer)();	

    /*
     *  @brief   	�����ص÷�
     *  @return 	int 
	*/
typedef	int (*PAcquireLoadSoccer)();

#endif // XJLoadAcquirerAPI_h__