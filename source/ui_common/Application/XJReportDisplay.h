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
 *  @brief   	IsValidKeyString	 判断一条带有关键字的语句是否有效语句 
 *  @param 		[In]a param of Type  CString  关键字语句
 *  @return 	BOOL 有效返回TRUE, 无效返回FALSE
 */
BOOL	IsValidKeyString(CString sKey);

/*
 *  @brief   	FindOutAllKey	 找出一个语句中所有的关键字 
 *  @param 		[In]a param of Type  CString  关键字语句
 *  @param 		[Out]a param of Type  CStringArray&  各关键字
 *  @return 	CString 
 */
CString FindOutAllKey( CString sKey, CStringArray& arrString );

/*
* @brief	TranslateSingleKey	解释单个关键字代表的意义
* @param	[In]a param of Type CString sKey	关键字内容	
* @return   CString	关键字代表的值
*/
virtual CString TranslateSingleKey(CString sKey);

protected:
	/*
	* @brief	InitCOM	初始化COM组件
	* @return   BOOL 成功返回TRUE,失败返回FALSE
	*/
	BOOL InitCOM();

	/*
	* @brief	ReleaseCOM	释放COM组件
	* @return   void	
	*/
	void ReleaseCOM();

	/*
	* @brief	InitCOMnoReg 不使用注册表初始化组件
	* @return   BOOL	
	*/
	BOOL InitCOMnoReg();

	void ReleaseCOMnoReg();

};

#endif // !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
