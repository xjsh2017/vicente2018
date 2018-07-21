#if !defined(XJDISPLAYDECISION_H_)
#define XJDISPLAYDECISION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Interface/XJReportInterface.h"
#include "vccommon_define.h"
#include "XJReportDisplay.h"
#include "DecisionReport.h"

class CXJDisplayDecision : public CXJReportDisplay
{
public:	
	CXJDisplayDecision(CDecisionReport* pReport);
	virtual ~CXJDisplayDecision();

private:
	/** @brief			对应的决策报告内容*/
	CDecisionReport*	m_pReport;
	/** @brief			模板文件路径*/
	CString m_sPath;
	/** @brief			模板文件名称*/
	CString	m_sPtName;
	/** @brief			是否已经载入了模板文件*/
	BOOL m_bLoadTemplateFile;

	/** @brief			动作相对时间计算方式*/
	int m_nFaultActionTime;
	
public:
	/*
	* @brief	TranslateSingleKey	解释单个关键字代表的意义
	* @param	[In]a param of Type CString sKey	关键字内容	
	* @return   CString	关键字代表的值
	*/
	virtual CString TranslateSingleKey(CString sKey);

/*
	* @brief	InitDisplay	初始化显示
	* @return   void	
	*/
	CSize InitDisplay();

	void SetTemplateFilePath(CString sPath, CString sPtName);

	void SetDecisionReport(CDecisionReport* pReport);

	/*
	* @brief	GetFaultActionTime			
	* @return   int	
	*/	
	int GetFaultActionTime() const { return m_nFaultActionTime; }

	/*
	* @brief	SetFaultActionTime	
	* @param	[In]a param of Type int		
	* @return   	
	*/	
	void SetFaultActionTime(int FaultActionTime) { m_nFaultActionTime = FaultActionTime; }

private:
/*
 *  @brief   	GetValue_getcount	 得到关键字getcount的值
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 值
 */
	//##ModelId=49B87B8401F7
	CString GetValue_getcount(CString sKey);

/*
 *  @brief   	GetValue_OneLayer	 得到一层深度关键字的值 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 值
 */
	//##ModelId=49B87B840204
	CString GetValue_OneLayer(CString sKey);

/*
 *  @brief   	GetValue_TwoLayer	 得到两层深度关键字的值 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CString 值
 */
	//##ModelId=49B87B840206
	CString GetValue_TwoLayer(CString sKey);
};

#endif // !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
