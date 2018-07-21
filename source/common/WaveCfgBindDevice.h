/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   14:13
	filename: 	D:\WORKSPACE\VICENTE\Code\common\WaveCfgBindDevice.h
	file path:	D:\WORKSPACE\VICENTE\Code\common
	file base:	WaveCfgBindDevice
	file ext:	h
	author:		rwp
	
	purpose:	���ڷ�װ��¼��Comtrade��ʽCFG�ļ������豸��̬��ʹ����
*********************************************************************/
#if !defined(WAVECFGBINDDEVICE_H)
#define WAVECFGBINDDEVICE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef OS_WINDOWS
#define LIBNAME_WAVECFGBINDDEVICE "XJWaveCFGAssociate.dll"
#else 
#define LIBNAME_WAVECFGBINDDEVICE "libXJWaveCFGAssociate.so"
#endif

#include <string>
#include "xjlib.h"

using namespace std;

/**
* @brief	XJWaveCFGBindDevice ��Commtrade��ʽ¼���ļ�CFG�ļ��еĵ�һ�������޸�Ϊ�뱣��ϵͳ�е��豸��
* @param 	[In]const char* pSrcWaveCFGName ȫ·��Commtrade��ʽ¼���ļ�CFG�ļ���
* @param 	[In]const char* pStationID ����ϵͳ��վID
* @param 	[In]const char* pPTID ����ϵͳ�е�װ��ID
* @return	int 0: �ɹ�������������
* @notes	   �����������޸�¼���ļ�֮CFG�ĵ�һ������Ϊ����ϵͳ�е��豸ID��
		    �Ա��ļ��������豸��������Ҫ����¼��������Ŀ
* @sample	��
*/
typedef int (*XJWaveCFGBindDevice)(const char* pSrcWaveCFGName, const char* pStationID,const char* pPTID, char* pError);

#define FUNC_NAME_BINDDEVICE "XJWaveCFGBindDevice"


/**
 * @brief       class name: CWaveCfgBindDevice
 * @use			��װ�Զ�̬��(libXJWaveCFGAssociate.so)��ʹ����
 * @author      rwp
 * @version     ver1.0
 * @date        
 *
 * example
 * @code
 *
 *
 *
 * @endcode
 */
 
class CWaveCfgBindDevice  
{
public:
	/**
	* @brief	CWaveCfgBindDevice ���캯��
	* @return	��
	* @notes	��
	* @sample	��
	*/
	CWaveCfgBindDevice();
	/**
	* @brief	~CWaveCfgBindDevice ��������
	* @return	virtual
	* @notes	��
	* @sample	��
	*/
	virtual ~CWaveCfgBindDevice();

public:
	/**
	* @brief	LoadLib ����ת����
	* @return	bool true:�ɹ� false:ʧ��
	* @notes	��
	* @sample	��
	*/
	bool LoadLib();

	/**
	* @brief	BindWaveCfgToDevice �޸�¼��CFG�ļ���������ʹ֮�뱣��ϵͳ�е��豸����
	* @param 	[In]const char* pSrcWaveCFGName ¼��CFG�ļ���
	* @param 	[In]const char* pStationID վID
	* @param 	[In]const char* pPTID �豸ID 
	* @param 	[In]string& pError ������Ϣ
	* @return	int 0���ɹ� ������ʧ��
	* @notes	��
	* @sample	��
	*/
	int BindWaveCfgToDevice(const char* pSrcWaveCFGName,const char* pStationID,const char* pPTID,char* pError);
	/**
	* @brief	FreeLib �ͷŶ�̬��
	* @return	void ��
	* @notes	��
	* @sample	��
	*/
	void FreeLib();
private:
	/**	\brief	��̬���� */
	XJHANDLE m_hLibHandle;
	/**	\brief	utf8ת��gbk����ָ�� */
	XJWaveCFGBindDevice pfun_XJWaveCFGBindDevice;
};

#endif // !defined(WAVECFGBINDDEVICE_H)
