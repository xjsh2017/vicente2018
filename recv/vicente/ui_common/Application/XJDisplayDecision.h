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
	/** @brief			��Ӧ�ľ��߱�������*/
	CDecisionReport*	m_pReport;
	/** @brief			ģ���ļ�·��*/
	CString m_sPath;
	/** @brief			ģ���ļ�����*/
	CString	m_sPtName;
	/** @brief			�Ƿ��Ѿ�������ģ���ļ�*/
	BOOL m_bLoadTemplateFile;

	/** @brief			�������ʱ����㷽ʽ*/
	int m_nFaultActionTime;
	
public:
	/*
	* @brief	TranslateSingleKey	���͵����ؼ��ִ��������
	* @param	[In]a param of Type CString sKey	�ؼ�������	
	* @return   CString	�ؼ��ִ����ֵ
	*/
	virtual CString TranslateSingleKey(CString sKey);

/*
	* @brief	InitDisplay	��ʼ����ʾ
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
 *  @brief   	GetValue_getcount	 �õ��ؼ���getcount��ֵ
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString ֵ
 */
	//##ModelId=49B87B8401F7
	CString GetValue_getcount(CString sKey);

/*
 *  @brief   	GetValue_OneLayer	 �õ�һ����ȹؼ��ֵ�ֵ 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString ֵ
 */
	//##ModelId=49B87B840204
	CString GetValue_OneLayer(CString sKey);

/*
 *  @brief   	GetValue_TwoLayer	 �õ�������ȹؼ��ֵ�ֵ 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CString ֵ
 */
	//##ModelId=49B87B840206
	CString GetValue_TwoLayer(CString sKey);
};

#endif // !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
