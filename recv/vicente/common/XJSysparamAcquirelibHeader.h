#ifndef XJSysparamAcquirelibHeader_h__
#define XJSysparamAcquirelibHeader_h__
//���θ澯��Ϣ
#pragma   warning   (disable   :   4786)
#pragma   warning   (disable   :   4275)

#include "define.h"//��׼��Ķ���

/** @brief  		 ��̬�����ƶ��� */
#ifdef OS_WINDOWS
#define XJ_LIB_SYS_PARAM_ACQUIRE      "XJSysparamAcquirelib.dll"
#else
#define XJ_LIB_SYS_PARAM_ACQUIRE      "libXJSysparamAcquirelib.so"
#endif

/**	\brief	The _XJ_DISK_CAP_RESULT_ struct
�������������Ϣ
*/
typedef struct _XJ_DISK_CAP_RESULT_
{
	/**	\brief �������к�*/
	string strDiskName;
	
	/**	\brief ���ص㣬��windows��Ϊ�̷�*/
	string strMountedName;
	
	/**	\brief ���ðٷֱ�*/
	string strUsepercent;
	
	/**	\brief ����������λ��kb*/
	string strTotalsize;
	
	/**	\brief ʣ����������λ��kb*/
	string strFreesize;
}XJ_DISK_CAP_RESULT;

/*****************************�ӿڶ���*****************************/
/*
 *  @brief   	Start	 ��ʼ��������Ϣ 
 *  @param 		[Out]a param of Type  char*  ���صĸ�����Ϣ
 *  @return 	int 
 */
	typedef	int (*gSyspararmlib_Start)(char*);

/*
 *  @brief   	End	 �رգ�������Դ 
 *  @param 		[Out]a param of Type  char*  ���صĸ�����Ϣ
 *  @return 	int 
 */
	typedef	int (*gSyspararmlib_End)(char*);

/*
 *  @brief   	SetLibLogparam	 ������־���� 
 *  @param 		[In]a param of Type  char*  ��־·��
 *  @param 		[In]a param of Type  int  ��־����
 *  @param 		[In]a param of Type  long  ��־��С
 *  @return 	bool true���ɹ� false��ʧ��
 */
	typedef	bool (*gSyspararmlib_SetLibLogparam)(char*, int /*= CLogFile::trace*/, long /*= 10240*/);

/*
 *  @brief   	GetDiskpartitionInfo	 ��ȡ�����ϸ���������Ϣ 
 *  @param 		[Out]a param of Type  vector<XJ_DISK_CAP_RESULT>&  �����ϸ���������Ϣ
 *  @return 	bool true���ɹ� false��ʧ��
 */
	typedef	bool (*gSyspararmlib_GetDiskpartitionInfo)(vector<XJ_DISK_CAP_RESULT>&);

/*
 *  @brief   	GetDiskInfo	 ��ȡӲ����Ϣ 
 *  @param 		[Out]a param of Type  XJ_DISK_CAP_RESULT&  Ӳ����Ϣ�ṹ��
 *  @return 	bool true���ɹ� false��ʧ��
 */
	typedef	bool (*gSyspararmlib_GetDiskInfo)(XJ_DISK_CAP_RESULT&);

/** @} */ //OVER
#endif // XJSysparamAcquirelibHeader_h__

