/********************************************************************
	created:	2011/10/14
	created:	14:10:2011   11:22
	filename: 	GBK2UTF8.cpp
	file path:	D:\WORKSPACE\VICENTE\Code\common
	file base:	GBK2UTF8
	file ext:	cpp
	author:		rwp
	
	purpose:	封闭对动态库的使用类
*********************************************************************/
#include "GBK2UTF8.h"

/**
* @brief	CGBK2UTF8 构造函数
* @return	无
* @notes	无
* @sample	无
*/
CGBK2UTF8::CGBK2UTF8():pfun_utf82gbk(NULL),m_hLibHandle(NULL),pfun_gbk2utf8(NULL)
{

}

/**
* @brief	~CGBK2UTF8 析构函数
* @return	virtual
* @notes	无
* @sample	无
*/
CGBK2UTF8::~CGBK2UTF8()
{
	FreeLib();
}

/**
* @brief	LoadLib 引导转换库
* @return	bool true:成功 false:失败
* @notes	无
* @sample	无
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
* @brief	convert_UTF8_GBK 转换uft8字符流到gbk字符流
* @param 	[In]const string& pSrcUTF8 uft8字符流
* @param 	[In]string& pRetGBK gbk字符流
* @return	bool true:成功 false:失败
* @notes	无
* @sample	无
*/
bool CGBK2UTF8::convert_UTF8_GBK( const string& pSrcUTF8, string& pRetGBK )
{
	bool bReturn(false);
	if (pfun_utf82gbk)
	{
		int src_size = pSrcUTF8.length();
		string sTemp("");
		sTemp.resize(src_size);/*由于在转换uft8转为gbk时长度会变小，这里临时缓冲区长度设置为相同大小*/
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
* @brief	convert_GBK_UTF8 转换gbk字符流到uft8字符流
* @param 	[In]const string& pSrcGBK gbk字符流
* @param 	[In]string& pRetUTF8 uft8字符流
* @return	bool true:成功 false:失败
* @notes	无
* @sample	无
*/
bool CGBK2UTF8::convert_GBK_UTF8( const string& pSrcGBK, string& pRetUTF8 )
{
	bool bReturn(false);
	if (pfun_gbk2utf8)
	{
		int src_size = pSrcGBK.length();
		string sTemp("");
		sTemp.resize(6*src_size);/*由于在转换gbk转换为utf8时会变长(1~6)，这里临时缓冲区长度设置为最大大小*/
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
* @brief	FreeLib 释放动态库
* @return	void 无
* @notes	无
* @sample	无
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