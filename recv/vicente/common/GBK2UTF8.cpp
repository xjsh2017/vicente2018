/********************************************************************
	created:	2011/10/14
	created:	14:10:2011   11:22
	filename: 	GBK2UTF8.cpp
	file path:	D:\WORKSPACE\VICENTE\Code\common
	file base:	GBK2UTF8
	file ext:	cpp
	author:		rwp
	
	purpose:	��նԶ�̬���ʹ����
*********************************************************************/
#include "GBK2UTF8.h"

/**
* @brief	CGBK2UTF8 ���캯��
* @return	��
* @notes	��
* @sample	��
*/
CGBK2UTF8::CGBK2UTF8():pfun_utf82gbk(NULL),m_hLibHandle(NULL),pfun_gbk2utf8(NULL)
{

}

/**
* @brief	~CGBK2UTF8 ��������
* @return	virtual
* @notes	��
* @sample	��
*/
CGBK2UTF8::~CGBK2UTF8()
{
	FreeLib();
}

/**
* @brief	LoadLib ����ת����
* @return	bool true:�ɹ� false:ʧ��
* @notes	��
* @sample	��
*/
bool CGBK2UTF8::LoadLib()
{
	bool bReturn(false);
	m_hLibHandle = xj_load_library(LIBNAME_GBK2UTF8);
	if (m_hLibHandle)	
	{
		pfun_gbk2utf8 = (xj_convert_gbk_utf8)xj_get_addr(m_hLibHandle,FUNC_NAME_GBK2UTF8);
		pfun_utf82gbk = (xj_convert_utf8_gbk)xj_get_addr(m_hLibHandle,FUNC_NAME_UTF82GBK);
		bReturn = ((pfun_gbk2utf8)&&(pfun_utf82gbk))?true:false;
	}
	return bReturn;
}

/**
* @brief	convert_UTF8_GBK ת��uft8�ַ�����gbk�ַ���
* @param 	[In]const string& pSrcUTF8 uft8�ַ���
* @param 	[In]string& pRetGBK gbk�ַ���
* @return	bool true:�ɹ� false:ʧ��
* @notes	��
* @sample	��
*/
bool CGBK2UTF8::convert_UTF8_GBK( const string& pSrcUTF8, string& pRetGBK )
{
	bool bReturn(false);
	if (pfun_utf82gbk)
	{
		int src_size = pSrcUTF8.length();
		string sTemp("");
		sTemp.resize(src_size);/*������ת��uft8תΪgbkʱ���Ȼ��С��������ʱ��������������Ϊ��ͬ��С*/
		int res_size = pfun_utf82gbk(pSrcUTF8.c_str(),(char*)&sTemp[0],src_size);
		if (res_size>0)		{
			pRetGBK.resize(res_size+1);
			memcpy((char*)&pRetGBK[0],sTemp.c_str(),res_size);
			bReturn = true;
		}
	}
	return bReturn;
}

/**
* @brief	convert_GBK_UTF8 ת��gbk�ַ�����uft8�ַ���
* @param 	[In]const string& pSrcGBK gbk�ַ���
* @param 	[In]string& pRetUTF8 uft8�ַ���
* @return	bool true:�ɹ� false:ʧ��
* @notes	��
* @sample	��
*/
bool CGBK2UTF8::convert_GBK_UTF8( const string& pSrcGBK, string& pRetUTF8 )
{
	bool bReturn(false);
	if (pfun_gbk2utf8)
	{
		int src_size = pSrcGBK.length();
		string sTemp("");
		sTemp.resize(6*src_size);/*������ת��gbkת��Ϊutf8ʱ��䳤(1~6)��������ʱ��������������Ϊ����С*/
		int res_size = pfun_gbk2utf8(pSrcGBK.c_str(),(char*)&sTemp[0],sTemp.length());
		if (res_size>0)		{
			pRetUTF8.resize(res_size+1);
			memcpy((char*)&pRetUTF8[0],sTemp.c_str(),res_size);
			bReturn = true;
		}
	}
	return bReturn;
}

/**
* @brief	FreeLib �ͷŶ�̬��
* @return	void ��
* @notes	��
* @sample	��
*/
void CGBK2UTF8::FreeLib()
{
	if (m_hLibHandle)
	{
		xj_free_library(m_hLibHandle);
		m_hLibHandle = NULL;
		pfun_utf82gbk = NULL;
		pfun_gbk2utf8 = NULL;
	}
}