// ***************************************************************
//  SttpEngine   version:  1.0    date: 25:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: STTP����,����������ر�STTP�ͻ��˽ӿڿ�,��װ�ӿ�
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
#define AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SttpEngine.h : header file
//
#include "FaultReport.h"
#include "FaultEvent.h"

#include "vccommon_define.h"

//STTP������ض���
#define INQ 'I'		//��Ϣ����INQ��
#define RES 'R'		//response�͵�
#define EX  'E'   //��չ��

#define FIX 'F'  //��Ϣ����Ϊ�̶���
#define VAR 'V'  //�䳤

/////////////////////////////////////////////////////////////////////////////
// CSttpEngine

/**
 * @defgroup 	CSttpEngine	STTP����
 * @{
 */
/**
 * @brief       ����������ر�STTP�ͻ��˽ӿڿ�,��װ�ӿ�
*  @author      LYH
 * @version     ver1.0
 * @date        25/09/08

 *
 * example
 * @code
 *    // 
 * @endcode
 */
/** @} */ //OVER
//##ModelId=49B87B8900EA
class CSttpEngine
{
public:
	//##ModelId=49B87B8900EB
	CSttpEngine();           // protected constructor used by dynamic creation
	//##ModelId=49B87B8900FA
	~CSttpEngine();

// Attributes
public:
	CString	m_sDllName;
private:
	/** @brief           ��̬����*/
	//##ModelId=49B87B890109
	HMODULE	m_hModule;
	

// Operations
public:

/*
 *  @brief   	XJSTTPConnectDestination	 ��ָ��Ŀ�꽨������ 
 *  @param 		[In]a param of Type  const STRUCT_DESTINATION_SERVER&  ָ������Ŀ��Ľṹ��
 *  @param 		[In]a param of Type  int  �ͻ��˵����ͱ�ţ����ڵ�¼��֤
 *  @param 		[Out]a param of Type  int &  �������״̬ 0-�ж� 1-����
 *	@param		[In]a param of Type char*	  ��־�ļ�Ŀ¼
 *  @param 		[In]a param of Type  int  ����ģʽ
 *  @return 	XJHANDLE ������Sttp�ͻ��˶���ľ��
 */
	//##ModelId=49B87B89010A
	XJHANDLE XJSTTPConnectDestination(const STRUCT_DESTINATION_SERVER& pDestination , int pClientModle , int & pConnected, char* sLogPath,int pRunModle);

/*
 *  @brief   	XJSttpConnectServer	 ������ͨ�ŷ����������� 
 *  @param 		[In]a param of Type  int  �ͻ��˵����ͱ�ţ����ڵ�¼��֤
 *  @param 		[Out]a param of Type  int &  �������״̬ 0-�ж� 1-����
 *	@param		[In]a param of Type char*	��־�ļ�Ŀ¼
 *  @param 		[In]a param of Type  int  ����ģʽ0��ͬ��ģʽ��Ĭ�ϣ�1���첽ģʽ
 *  @return 	XJHANDLE ������Sttp�ͻ��˶���ľ��
 */
	//##ModelId=49B87B890119
	XJHANDLE XJSttpConnectServer(int pClientModle, int & pConnected, char* sLogPath, int pRunModle);
	
/*
 *  @brief   	XJSttpCloseConnection	 �ر������˵����� 
 *  @param 		[In]a param of Type  XJHANDLE  ���ر����ӵľ��
 *  @return 	int 1:�ɹ� 0:��Ч�ľ��
 */
	//##ModelId=49B87B89011E
	int XJSttpCloseConnection(XJHANDLE pClientHandle);
	
/*
 *  @brief   	XJSTTPActiveTest	 ����ָ�������ϵ��Զ����� 
 *  @param 		[In]a param of Type  XJHANDLE  ָ�����ӵľ��
 *  @return 	int 1:�ɹ� 0:��Ч�ľ��
 */
	//##ModelId=49B87B890129
	int XJSTTPActiveTest(XJHANDLE pClientHandle);
	
/*
 *  @brief   	XJSTTPRead	 ��ָ�������϶�ȡһ��Sttp�������� 
 *  @param 		[In]a param of Type  XJHANDLE  ָ�����ӵľ��
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @return 	int 1:�ɹ� ͬ��ģʽ�³�ʱ��������ʱ����0���첽ģʽ�����������̷���0�������ж�ʱ����-1
 */
	//##ModelId=49B87B890138
	int XJSTTPRead(XJHANDLE pClientHandle , STTP_FULL_DATA& pSttpData);
	
/*
 *  @brief   	XJSTTPWrite	 ��ָ��������д��һ��Sttp�������� 
 *  @param 		[In]a param of Type  XJHANDLE  ָ�����ӵľ��
 *  @param 		[In]a param of Type  const STTP_FULL_DATA&  ��������
 *  @return 	int 1:�ɹ� 0:��Ч���� -1:�����ж� -2:��������
 */
	//##ModelId=49B87B890148
	int XJSTTPWrite(XJHANDLE pClientHandle , const STTP_FULL_DATA& pSttpData);
	
/*
 *  @brief   	XJSTTPSetOption	 Ϊָ�����������ö����� 
 *  @param 		[In]a param of Type  XJHANDLE  ָ�����ӵľ��
 *  @param 		[In]a param of Type  int  �������� 1�����ճ�ʱ 2�����ͳ�ʱ
 *  @param 		[In]a param of Type  int  ��ʱʱ�䣨ms��
 *  @param 		[In]a param of Type  int  ����
 *  @return 	int 
 */
	//##ModelId=49B87B890157
	int XJSTTPSetOption(XJHANDLE pClientHandle , int pOptionName , int pTime , int pReserved);
	
/*
 *  @brief   	XJSTTPRegisterHandler	 ��ָ������ע��ص�����ָ�� 
 *  @param 		[In]a param of Type  XJHANDLE  ָ�����ӵľ��
 *  @param 		[In]a param of Type  PFUNCALLBACK  �ص�����ָ��
 *  @param 		[In]a param of Type  int  ������ 0��onConnectChange
 *  @return 	int 1:�ɹ�  0:ʧ��
 */
	//##ModelId=49B87B890167
	int XJSTTPRegisterHandler(XJHANDLE pClientHandle , PFUNCALLBACK pFunction, int pMask );

/*
 *  @brief   	XJSTTPSetLogPath	 ������־·�� 
 *  @param 		[In]a param of Type  XJHANDLE  ָ�����
 *  @param 		[In]a param of Type  const char*  ��־·��
 *  @return 	int 1:�ɹ�	0:ʧ��
 */
	//##ModelId=49B87B89016B
	int XJSTTPSetLogPath(XJHANDLE pClientHandle ,const char* pLogPath);

/*
 *  @brief   	BuildDataFor20015PTSetting	 �齨20015(������ֵ����Ҫ��)�������� 
 *	@param		[Out]a param of Type STTP_FULL_DATA ��������
 *  @param 		[In]a param of Type  CString	����װ��ID
 *  @param 		[In]a param of Type  int	CPU��
 *  @param 		[In]a param of Type  int	��ֵ����
 *  @param 		[In]a param of Type  int	������Դ 0-����װ��(Ĭ��), 1-���ݿ�.
 *  @param 		[In]a param of Type  int	��ֵ��(Ĭ��0),
											��Source=0ʱ	0   - ��֮sttp�ͻ����ٻ�ȫ����ֵ
															103 - ��֮��103��վ�ٻ�ָ��cpu�Ķ�ֵ
											��Source=1 ������,��Ϊ0

 *  @return 	BOOL �����ɹ�����TRUE, ʧ�ܷ���FALSE;
 */
	//##ModelId=49B87B890177
	BOOL BuildDataFor20015PTSetting(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom = 0, int nDataNum = 0);

/*
 *  @brief   	BuildDataFor20007PTAnalog	 �齨20007(����ģ��������ֵ����Ҫ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  ����װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B89017E
	BOOL BuildDataFor20007PTAnalog(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20011PTDigital	 �齨20011(��������������ֵ����Ҫ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  ����װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B890189
	BOOL BuildDataFor20011PTDigital(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20110PTGeneral	 �齨20110(����������Ϣ����Ҫ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  ����װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *	@param		[In]a param of Type	 int  ��־λ,"1"�ٻ�MMI�汾��Ϣ����ʱCPU�������壻"0"�ٻ�CPU�汾��Ϣ
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B890196
	BOOL BuildDataFor20110PTGeneral(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nFlag);

/*
 *  @brief   	BuildDataFor20106PTZone	 �齨20106(�ٻ���ֵ����Ҫ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  ����װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8901A5
	BOOL BuildDataFor20106PTZone(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);
	BOOL BuildDataFor20173PTZone(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20108PTSoftBoard	 �齨20108����(�ٻ���ѹ��Ҫ��)����
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  ����װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8901A9
	BOOL BuildDataFor20108PTSoftBoard(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20042DownloadWaveFile	 �齨20042(¼���ļ�����Ҫ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  CString  Ҫ���ص�¼���ļ���
 *  @param 		[In]a param of Type  int  ������Դ 0-��װ�� 1-����վ
 *  @param 		[In]a param of Type  int  ֧�ֶϵ�����ʱ���������ʼƫ���������´���ʱȫΪ��
 *  @param 		[In]a param of Type  int  Ҫ�����ļ���װ���еı�ţ�������ԴΪ0ʱ��Ч��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8901B6
	BOOL BuildDataFor20042DownloadWaveFile(STTP_FULL_DATA& sttpData, CString PTID, CString FileName, int nDataFrom, int nOffset, int nNo);

/*
 *  @brief   	BuildDataFor20025WaveFileList	 �齨20025(¼���ļ��б�����Ҫ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  ������Դ 0-��װ�� 1-����վ
 *  @param 		[In]a param of Type  CTime  ��ʼ��ѯʱ��,��ʽYYmmddHHMMSS
 *  @param 		[In]a param of Type  CTime  ������ѯʱ��,��ʽYYmmddHHMMSS
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8901C5
	BOOL BuildDataFor20025WaveFileList(STTP_FULL_DATA& sttpData, CString PTID, int nDataFrom, CTime tmStart, CTime tmEnd);

/*
 *  @brief   	BuildDataFor20051SettingModify	 �齨20051(������ֵ�޸�Ҫ��) ��������
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @param 		[In]a param of Type  int  ��ֵ����
 *  @param 		[In]a param of Type  int  ����Ŀ�꣺0-����װ�ã�1-���ݿ�
 *  @param 		[In]a param of Type  MODIFY_DATA*  Ҫ�޸ĵĶ�ֵ����������
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8901CB
	BOOL BuildDataFor20051SettingModify(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom, MODIFY_LIST* modifyList);

/*
 *  @brief   	BuildDataFor20053SettingModifyExe	 �齨20053(������ֵ�޸�ִ��Ҫ��) ��������
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @param 		[In]a param of Type  int  ��ֵ����
 *  @param 		[In]a param of Type  int  ����Ŀ�꣺0-����װ�ã�1-���ݿ�
 *  @param 		[In]a param of Type  MODIFY_DATA*  Ҫ�޸ĵĶ�ֵ����������
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8901DA
	BOOL BuildDataFor20053SettingModifyExe(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int nDataFrom, MODIFY_LIST* modifyList);

/*
 *  @brief   	BuildDataFor20047SettingChange	 �齨20047(������ֵ�л�ԤУҪ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @param 		[In]a param of Type  int  Ҫ�л����Ķ�ֵ����
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8901E5
	BOOL BuildDataFor20047SettingChange(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone);

	BOOL BuildDataFor20169SettingChange(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int id);

/*
 *  @brief   	BuildDataFor20055SettingChange	 �齨20055(������ֵ�л�ִ��Ҫ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @param 		[In]a param of Type  int  Ҫ�л����Ķ�ֵ����
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8901EA
	BOOL BuildDataFor20055SettingChangeExe(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone);
	BOOL BuildDataFor20171SettingChangeExe(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, int nZone, int id);

/*
 *  @brief   	BuildDataFor20117SoftBoardModify	 �齨20117(��ѹ��Ͷ��ԤУҪ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @param 		[In]a param of Type  MODIFY_LIST*  Ҫ�޸ĵ���ѹ������������
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8901F6
	BOOL BuildDataFor20117SoftBoardModify(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, MODIFY_LIST* modifyList);

/*
 *  @brief   	BuildDataFor20113SoftBoardModify	 �齨20113(��ѹ��Ͷ��ִ��Ҫ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @param 		[In]a param of Type  MODIFY_LIST*  Ҫ�޸ĵ���ѹ������������
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B8901FB
	BOOL BuildDataFor20113SoftBoardModifyExe(STTP_FULL_DATA& sttpData, CString PTID, int nCPU, MODIFY_LIST* modifyList);

/*
 *  @brief   	BuildDataFor20123WaveRecRun	 �齨20123(¼����Զ����������Ҫ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B890205
	BOOL BuildDataFor20123WaveRecRun(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20059WaveRecRun	 �齨20059(������ʱҪ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B890209
	BOOL BuildDataFor20059PTSetTime(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20088PTTime	 �齨20088(�ٻ�װ��ʱ��)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B890213
	BOOL BuildDataFor20088PTTime(STTP_FULL_DATA& sttpData, CString PTID);

/*
 *  @brief   	BuildDataFor20057ResetPT	 �齨20057(�����źŸ���)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  CPU��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B890216
	BOOL BuildDataFor20057ResetPT(STTP_FULL_DATA& sttpData, CString PTID, int nCPU);

/*
 *  @brief   	BuildDataFor20137Cancel	 �齨20137(ȡ�����������)�������� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  ��������
 *  @param 		[In]a param of Type  int  ����ID
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  װ��CPU��
 *  @param 		[In]a param of Type  int  װ�ö�ֵ����
 *  @param 		[In]a param of Type  int  �¼�����0�������� ��0������ȡ���ٻ���ʷ�¼� 1-������Ϣ 2���Լ���Ϣ 3��ѹ����Ϣ 
 *  @param 		[In]a param of Type  CString  ��ʼʱ��
 *  @param 		[In]a param of Type  CString  ����ʱ��
 *  @param 		[In]a param of Type  CString  ¼���ļ���
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B89021A
	BOOL BuildDataFor20137Cancel(STTP_FULL_DATA& sttpData, int MsgID, CString PTID, int nCPU, int nZone = 0, int nEventType = 0, CString strStartTime = "", CString strEndTime = "", CString strFile = "");

/*
 *  @brief   	BuildDataFor20153Batch	 �齨�����ٻ���ϢҪ��(20153)����
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *	@param		[In]a param of Type int ��������
 *	@param		[In]a param of Type int վID����ָ��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B890228
	BOOL BuildDataFor20153Batch(STTP_FULL_DATA& sttpData, int nType, CStringArray* arrStation = NULL);

/*
 *  @brief   	BuildDataFor20105HistoryEvent	 �齨������ʷ�����¼�Ҫ��(20105)���� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  CString  ����ID
 *  @param 		[In]a param of Type  int  �¼�����,1-������Ϣ 2���Լ���Ϣ 3��Ӳѹ����Ϣ 4��SOE  5-��� 6-�澯��Ϣ 0-ȫ�� 
 *  @param 		[In]a param of Type  CTime  ��ʼʱ��
 *  @param 		[In]a param of Type  CTime  ����ʱ��
 *  @param 		[In]a param of Type  int  ������Դ
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B89022C
	BOOL BuildDataFor20105HistoryEvent(STTP_FULL_DATA& sttpData, CString PT_ID, int nType, CTime tmStart, CTime tmEnd, int nSrc);

/*
 *  @brief   	BuildDataFor00203CommonFileList	 �齨�ٻ�ͨ���ļ��б�Ҫ��(00203)���� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  CString  ��վID
 *  @param 		[In]a param of Type  int  ��������:1-���ļ�����,2-ָ��ʱ����ڵ��ļ�����
 *  @param 		[In]a param of Type  CTime  ��ѯ��ʼʱ��
 *  @param 		[In]a param of Type  CTime  ��ѯ����ʱ��
 *  @param 		[In]a param of Type  CString  ��ѯ����
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B890237
	BOOL BuildDataFor00203CommonFileList(STTP_FULL_DATA& sttpData, CString sStationID, int nConditionType, CTime tmStart, CTime tmEnd, CString sCondition);

/*
 *  @brief   	BuildDataFor00210CommonFileDown	 �齨ͨ���ļ�����Ҫ��(00210)���� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  CString  ��վID
 *  @param 		[In]a param of Type  CString  �ļ���
 *  @param 		[In]a param of Type  int  ƫ����
 *  @param 		[In]a param of Type  CString  ��վIP
 *  @param 		[In]a param of Type  int  ��Ϣ��ʶ��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B89023E
	BOOL BuildDataFor00210CommonFileDown(STTP_FULL_DATA& sttpData, CString sStationID, CString sFileName, int nOffest, CString sIP, int nRII = 0);

/*
 *  @brief   	BuildDataFor20151FaultPacket	 �齨�Զ��鵵��Ϣ���仯֪ͨ(20151)���� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  int  ��Ϣ���仯����
 *  @param 		[In]a param of Type  CFaultReport*  ��Ϣ��ָ��
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B890247
	BOOL BuildDataFor20151FaultPacket(STTP_FULL_DATA& sttpData, int nType, CFaultReport* pReport);

/*
 *  @brief   	BuildDataFor20018FaultAlarm	 �齨�����澯�¼�����֪ͨ(20018)����
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  CFaultEvent*  �澯�¼�
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildDataFor20018FaultAlarm(STTP_FULL_DATA& sttpData, CFaultEvent* pEvent);

/*
 *  @brief   	BuildDataFor20010FaultDI	 �齨������������λ�¼�����֪ͨ(20010)���� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  CFaultEvent*  ��������λ�¼�
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildDataFor20010FaultDI(STTP_FULL_DATA& sttpData, CFaultEvent* pEvent);

/*
 *  @brief   	BuildDataFor20022FaultAction	 �齨���������¼�����֪ͨ(20022)���� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  �����¼�
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildDataFor20022FaultAction(STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent);

/*
 *  @brief   	BuildDataFor20116FaultAlarm	 �齨�����澯��ʷ�¼�����֪ͨ(20116)���� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  �澯�¼�
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildDataFor20116FaultAlarm(STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent);

/*
 *  @brief   	BuildDataFor20116FaultDI	 �齨������������ʷ�¼�����֪ͨ(20116)����  
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  �������¼�
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildDataFor20116FaultDI(STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent);

/*
 *  @brief   	BuildDataFor20116FaultAction	 �齨����������ʷ�¼�����֪ͨ(20116)���� 
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  HISTORYEVENT_LIST*  �����¼�
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildDataFor20116FaultAction(STTP_FULL_DATA& sttpData, HISTORYEVENT_LIST* listEvent);

/*
 *  @brief   	BuildDataFor20145StationStatus	 �齨��վͨѶ״̬����Ҫ�� (20145) ����
 *  @param 		[Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  CString  ��վID
 *  @param 		[In]a param of Type  int  ͨ����
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildDataFor20145StationStatus(STTP_FULL_DATA& sttpData, CString strStationID, int nChannel);

/*
 *  @brief   	BuildDataFor20145StationStatus	 �齨װ������״̬���ر���
 *  @param 		[In,Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  CString  װ��ID
 *  @param 		[In]a param of Type  int  װ��״̬ID
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildDataFor20079UpdatePtRunStatus(STTP_FULL_DATA& sttpData, CString& strPtID, int nPtStatus);

/*
 *  @brief   	BuildDataFor20145StationStatus	 3.139  ¼���ļ���������Ҫ��
 *  @param 		[In,Out]a param of Type  STTP_FULL_DATA&  �������ݽṹ
 *  @param 		[In]a param of Type  CFaultEvent ¼���¼�����
 *  @return 	BOOL �ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	BOOL BuildDataFor20162RenameOscFile( STTP_FULL_DATA& sttpData, CFaultEvent* pEvent );

	BOOL BuildDataFor20138StationConfigChange(STTP_FULL_DATA& sttpData, CString strStationName);

	BOOL BuildDataFor20003CommStatus(STTP_FULL_DATA& sttpData, CString strPTID);

	BOOL BuildDataFor20085CommStatus(STTP_FULL_DATA& sttpData, CString strOscID);

public:
/*
 *  @brief   	LoadSttpLib	 ����STTP�ͻ��˶�̬�� 
 *  @return 	BOOL ���سɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87B89024B
	BOOL LoadSttpLib();

/*
 *  @brief   	FreeSttpLib	 ж��STTP�ͻ��˶�̬�� 
 *  @return 	void 
 */
	void FreeSttpLib();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
