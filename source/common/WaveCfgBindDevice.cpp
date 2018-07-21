/********************************************************************
	created:	2011/10/20
	created:	20:10:2011   14:12
	filename: 	WaveCfgBindDevice.cpp
	file path:	\VICENTE\Code\common
	file base:	WaveCfgBindDevice
	file ext:	cpp
	author:		rwp
	
	purpose:	���ڷ�װ��¼��Comtrade��ʽCFG�ļ������豸��̬��ʹ����
*********************************************************************/
#include "WaveCfgBindDevice.h"

/**
* @brief	CWaveCfgBindDevice ���캯��
* @return	��
* @notes	��
* @sample	��
*/
CWaveCfgBindDevice::CWaveCfgBindDevice():pfun_XJWaveCFGBindDevice(NULL),m_hLibHandle(NULL)
{

}

/**
* @brief	~CWaveCfgBindDevice ��������
* @return	virtual
* @notes	��
* @sample	��
*/
CWaveCfgBindDevice::~CWaveCfgBindDevice()
{
	FreeLib();
}

/**
* @brief	LoadLib ����ת����
* @return	bool true:�ɹ� false:ʧ��
* @notes	��
* @sample	��
*/
bool CWaveCfgBindDevice::LoadLib()
{
	bool bReturn(false);
	m_hLibHandle = xj_load_library(LIBNAME_WAVECFGBINDDEVICE);
	if (m_hLibHandle)	
	{
		pfun_XJWaveCFGBindDevice = (XJWaveCFGBindDevice)xj_get_addr(m_hLibHandle,FUNC_NAME_BINDDEVICE);
		bReturn = (pfun_XJWaveCFGBindDevice)?true:false;
	}
	return bReturn;
}

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
int CWaveCfgBindDevice::BindWaveCfgToDevice(const char* pSrcWaveCFGName,const char* pStationID,const char* pPTID,char* pError)
{
	int iReturn(-1);
	if (pfun_XJWaveCFGBindDevice)
	{
		iReturn = pfun_XJWaveCFGBindDevice(pSrcWaveCFGName,pStationID,pPTID,pError);
	}
	return iReturn;
}



/**
* @brief	FreeLib �ͷŶ�̬��
* @return	void ��
* @notes	��
* @sample	��
*/
void CWaveCfgBindDevice::FreeLib()
{
	if (m_hLibHandle)
	{
		xj_free_library(m_hLibHandle);
		m_hLibHandle = NULL;
		pfun_XJWaveCFGBindDevice = NULL;
	}
}