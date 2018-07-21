/********************************************************************
	created:	2011/10/12
	created:	12:10:2011   16:03
	filename: 	GBK2UTF8.h
	file path:	D:\WORKSPACE\VICENTE\Code\src\XJGBK2UTF8\TestlibGBK2UTF8
	file base:	GBK2UTF8
	file ext:	h
	author:		rwp
	
	purpose:	��նԶ�̬���ʹ����
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
* @brief	xj_convert_utf8_gbk ת��utf8�����ַ�����gbk�����ַ���
* @param 	[In]char* pSrcUTF8Str utf8�ַ���
* @param 	[In]char* pbufDestGBK gbk�����ַ�������
* @param 	[In]int buf_len gbk���뻺������С
* @return	int ����gbk�����ַ���ʵ�ʳ���
* @notes	��
* @sample	��
*/
typedef int  (*xj_convert_utf8_gbk)(const char* pSrcUTF8Str, char* pbufDestGBK,int buf_len);

/**
* @brief	xj_convert_gbk_utf8 ת��gbk�����ַ�����utf8�����ַ���
* @param 	[In]const char* pSrcGBKStr gbk�����ַ���
* @param 	[In]char* pbufDestUTF8 utf8�����ַ���������
* @param 	[In]int buf_len utf8�����ַ�����������
* @return	int ����utf8�����ַ�ʵ�ʳ���
* @notes	��
* @sample	��
*/
typedef int (*xj_convert_gbk_utf8)(const char* pSrcGBKStr, char* pbufDestUTF8,int buf_len);

#define FUNC_NAME_UTF82GBK "xj_convert_utf8_gbk"
#define FUNC_NAME_GBK2UTF8 "xj_convert_gbk_utf8"

/**
 * @brief       class name: CGBK2UTF8
 * @use			��װ�Զ�̬��(libXJGBK2UTF8.so)��ʹ����
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
	* @brief	CGBK2UTF8 ���캯��
	* @return	��
	* @notes	��
	* @sample	��
	*/
	CGBK2UTF8();
	/**
	* @brief	~CGBK2UTF8 ��������
	* @return	virtual
	* @notes	��
	* @sample	��
	*/
	virtual ~CGBK2UTF8();

public:
	/**
	* @brief	LoadLib ����ת����
	* @return	bool true:�ɹ� false:ʧ��
	* @notes	��
	* @sample	��
	*/
	bool LoadLib();
	/**
	* @brief	convert_UTF8_GBK ת��uft8�ַ�����gbk�ַ���
	* @param 	[In]const string& pSrcUTF8 uft8�ַ���
	* @param 	[In]string& pRetGBK gbk�ַ���
	* @return	bool true:�ɹ� false:ʧ��
	* @notes	��
	* @sample	��
	*/
	bool convert_UTF8_GBK(const string& pSrcUTF8, string& pRetGBK);
	/**
	* @brief	convert_GBK_UTF8 ת��gbk�ַ�����uft8�ַ���
	* @param 	[In]const string& pSrcGBK gbk�ַ���
	* @param 	[In]string& pRetUTF8 uft8�ַ���
	* @return	bool true:�ɹ� false:ʧ��
	* @notes	��
	* @sample	��
	*/
	bool convert_GBK_UTF8(const string& pSrcGBK, string& pRetUTF8);
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
	xj_convert_utf8_gbk pfun_utf82gbk;
	/**	\brief	gbkת��utf8����ָ�� */
	xj_convert_gbk_utf8 pfun_gbk2utf8;
};

#endif // !defined(AFX_GBK2UTF8_H__3382FC17_F144_4125_9980_8B540E4A494A__INCLUDED_)
