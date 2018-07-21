#ifndef XJServerAccepterAPI_h__
#define XJServerAccepterAPI_h__

#include "../../../common/define.h"

typedef	struct {
	char szLog_path[FILE_NAME_MAX_LEN];		//��־���Ŀ¼
	int  iLog_level;						//��־����,0:����¼��־���ļ�1:������־2:������־3:������־
	int  iServerRole;						//��������ɫ 0-��վͨѶ������ 1-��վͨѶ������	
	bool bRecordMsg;						//�Ƿ��¼����
	int  iLogDay;							//��־��¼����
	bool bReserved;							//��������
	int  iReserved;							//��������
	char reservedStr[256];					//��������
}STRUCT_XJSERVERACCEPTER;

/** @brief            ����˹�����������������*/
typedef int (*PFUNSTARTSERVERACCEPTER) (const STRUCT_XJSERVERACCEPTER&);
/** @brief            ����˹�����ֹͣ��������*/
typedef int (*PFUNSTOPSERVERACCEPTER) ();

#ifdef OS_LINUX
#define  LIB_SERVERACCEPTER_NAME "libXJServerAccepter.so" 
#else
#define  LIB_SERVERACCEPTER_NAME "XJServerAccepter.dll" 
#endif

#endif // XJServerAccepterAPI_h__