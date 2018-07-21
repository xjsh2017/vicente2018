#if !defined(XJREPORTDISPLAY_H_)
#define XJREPORTDISPLAY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Interface/XJReportInterface.h"
#include "vccommon_define.h"

class CXJReportDisplay : public CObject
{
public:	
	CXJReportDisplay();
	virtual ~CXJReportDisplay();

public:
	CLSID clsid;
	LPCLASSFACTORY pClf;
	LPUNKNOWN pUnk;
	IXJReport * pReportDoc;
	HINSTANCE hdllInst;

public:
	static int TranslateKey(CString sKey, CString &sValue, void* pContent);

/*
 *  @brief   	IsValidKeyString	 �ж�һ�����йؼ��ֵ�����Ƿ���Ч��� 
 *  @param 		[In]a param of Type  CString  �ؼ������
 *  @return 	BOOL ��Ч����TRUE, ��Ч����FALSE
 */
BOOL	IsValidKeyString(CString sKey);

/*
 *  @brief   	FindOutAllKey	 �ҳ�һ����������еĹؼ��� 
 *  @param 		[In]a param of Type  CString  �ؼ������
 *  @param 		[Out]a param of Type  CStringArray&  ���ؼ���
 *  @return 	CString 
 */
CString FindOutAllKey( CString sKey, CStringArray& arrString );

/*
* @brief	TranslateSingleKey	���͵����ؼ��ִ��������
* @param	[In]a param of Type CString sKey	�ؼ�������	
* @return   CString	�ؼ��ִ����ֵ
*/
virtual CString TranslateSingleKey(CString sKey);

protected:
	/*
	* @brief	InitCOM	��ʼ��COM���
	* @return   BOOL �ɹ�����TRUE,ʧ�ܷ���FALSE
	*/
	BOOL InitCOM();

	/*
	* @brief	ReleaseCOM	�ͷ�COM���
	* @return   void	
	*/
	void ReleaseCOM();

	/*
	* @brief	InitCOMnoReg ��ʹ��ע����ʼ�����
	* @return   BOOL	
	*/
	BOOL InitCOMnoReg();

	void ReleaseCOMnoReg();

};

#endif // !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
