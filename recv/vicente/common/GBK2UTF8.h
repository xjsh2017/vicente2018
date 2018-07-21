/********************************************************************
	created:	2011/10/12
	created:	12:10:2011   16:03
	filename: 	GBK2UTF8.h
	file path:	D:\WORKSPACE\VICENTE\Code\src\XJGBK2UTF8\TestlibGBK2UTF8
	file base:	GBK2UTF8
	file ext:	h
	author:		rwp
	
	purpose:	封闭对动态库的使用类
*********************************************************************/
#if !defined(AFX_GBK2UTF8_H__3382FC17_F144_4125_9980_8B540E4A494A__INCLUDED_)
#define AFX_GBK2UTF8_H__3382FC17_F144_4125_9980_8B540E4A494A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef OS_WINDOWS
#define LIBNAME_GBK2UTF8 "XJGBK2UTF8.dll"
#else 
#define LIBNAME_GBK2UTF8 "libXJGBK2UTF8.so"
#endif

#include <string>
#include "xjlib.h"

using namespace std;

/**
* @brief	xj_convert_utf8_gbk 转换utf8编码字符流到gbk编码字符流
* @param 	[In]char* pSrcUTF8Str utf8字符流
* @param 	[In]char* pbufDestGBK gbk编码字符缓冲区
* @param 	[In]int buf_len gbk编码缓冲区大小
* @return	int 返回gbk编码字符流实际长度
* @notes	无
* @sample	无
*/
typedef int  (*xj_convert_utf8_gbk)(const char* pSrcUTF8Str, char* pbufDestGBK,int buf_len);

/**
* @brief	xj_convert_gbk_utf8 转换gbk编码字符流到utf8编码字符流
* @param 	[In]const char* pSrcGBKStr gbk编码字符流
* @param 	[In]char* pbufDestUTF8 utf8编码字符流缓冲区
* @param 	[In]int buf_len utf8编码字符缓冲区长度
* @return	int 返回utf8编码字符实际长度
* @notes	无
* @sample	无
*/
typedef int (*xj_convert_gbk_utf8)(const char* pSrcGBKStr, char* pbufDestUTF8,int buf_len);

#define FUNC_NAME_UTF82GBK "xj_convert_utf8_gbk"
#define FUNC_NAME_GBK2UTF8 "xj_convert_gbk_utf8"

/**
 * @brief       class name: CGBK2UTF8
 * @use			封装对动态库(libXJGBK2UTF8.so)的使用类
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
 
class CGBK2UTF8  
{
public:
	/**
	* @brief	CGBK2UTF8 构造函数
	* @return	无
	* @notes	无
	* @sample	无
	*/
	CGBK2UTF8();
	/**
	* @brief	~CGBK2UTF8 析构函数
	* @return	virtual
	* @notes	无
	* @sample	无
	*/
	virtual ~CGBK2UTF8();

public:
	/**
	* @brief	LoadLib 引导转换库
	* @return	bool true:成功 false:失败
	* @notes	无
	* @sample	无
	*/
	bool LoadLib();
	/**
	* @brief	convert_UTF8_GBK 转换uft8字符流到gbk字符流
	* @param 	[In]const string& pSrcUTF8 uft8字符流
	* @param 	[In]string& pRetGBK gbk字符流
	* @return	bool true:成功 false:失败
	* @notes	无
	* @sample	无
	*/
	bool convert_UTF8_GBK(const string& pSrcUTF8, string& pRetGBK);
	/**
	* @brief	convert_GBK_UTF8 转换gbk字符流到uft8字符流
	* @param 	[In]const string& pSrcGBK gbk字符流
	* @param 	[In]string& pRetUTF8 uft8字符流
	* @return	bool true:成功 false:失败
	* @notes	无
	* @sample	无
	*/
	bool convert_GBK_UTF8(const string& pSrcGBK, string& pRetUTF8);
	/**
	* @brief	FreeLib 释放动态库
	* @return	void 无
	* @notes	无
	* @sample	无
	*/
	void FreeLib();
private:
	/**	\brief	动态库句柄 */
	XJHANDLE m_hLibHandle;
	/**	\brief	utf8转换gbk函数指针 */
	xj_convert_utf8_gbk pfun_utf82gbk;
	/**	\brief	gbk转换utf8函数指针 */
	xj_convert_gbk_utf8 pfun_gbk2utf8;
};

#endif // !defined(AFX_GBK2UTF8_H__3382FC17_F144_4125_9980_8B540E4A494A__INCLUDED_)
