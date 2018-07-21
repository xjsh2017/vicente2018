#if !defined(XJDISPLAYFAULT_H_)
#define XJDISPLAYFAULT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Interface/XJReportInterface.h"
#include "vccommon_define.h"
#include "XJReportDisplay.h"
#include "FaultReport.h"

class CXJDisplayFault : public CXJReportDisplay
{
public:	
	CXJDisplayFault(CFaultReport* pReport);
	virtual ~CXJDisplayFault();

private:
	/** @brief			��Ӧ�Ĺ��ϱ�������*/
	CFaultReport*	m_pReport;

	/** @brief			�������ʱ����㷽ʽ*/
	int m_nFaultActionTime;
	/** @brief			�Ƿ���ʾֵΪ���صĶ���*/
	BOOL m_bShowReturn;
	/** @brief			�������й�������Ƿ����*/
	BOOL			m_bFilterNULL;
	/** @brief			�Ƿ���˹��ϲ��=0*/
	BOOL			m_bFilterDistance;

	/** @brief			�Ƿ����III����Ϣ*/
	BOOL			m_bFilterIII;

	CString m_sPath;
	/** @brief			������ģ���ļ�����*/
	CString m_sSinglePtName;
	/** @brief			�ౣ��ģ���ļ�����*/
	CString m_sMultiPtName;
	/** @brief			��ǰ��ʾ��ģ����*/
	CString m_sCurrentPtName;

public:
/*
	* @brief	GetFilterNULL			
	* @return   BOOL	
	*/	
	BOOL GetFilterNULL() const { return m_bFilterNULL; }

	/*
	* @brief	SetFilterNULL	
	* @param	[In]a param of Type BOOL		
	* @return   	
	*/	
	void SetFilterNULL(BOOL FilterNULL) { m_bFilterNULL = FilterNULL; }

	/*
	* @brief	GetFilterDistance			
	* @return   BOOL	
	*/	
	BOOL GetFilterDistance() const { return m_bFilterDistance; }

	/*
	* @brief	SetFilterDistance	
	* @param	[In]a param of Type BOOL		
	* @return   	
	*/	
	void SetFilterDistance(BOOL FilterDistance) { m_bFilterDistance = FilterDistance; }

	/*
	* @brief	GetFilterIII			
	* @return   BOOL	
	*/	
	BOOL GetFilterIII() const { return m_bFilterIII; }

	/*
	* @brief	SetFilterIII	
	* @param	[In]a param of Type BOOL		
	* @return   	
	*/	
	void SetFilterIII(BOOL FilterIII) { m_bFilterIII = FilterIII; }
	/*
	* @brief	TranslateSingleKey	���͵����ؼ��ִ��������
	* @param	[In]a param of Type CString sKey	�ؼ�������	
	* @return   CString	�ؼ��ִ����ֵ
	*/
	virtual CString TranslateSingleKey(CString sKey);

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

	/*
	* @brief	GetShowReturn			
	* @return   BOOL	
	*/	
	BOOL GetShowReturn() const { return m_bShowReturn; }

	/*
	* @brief	SetShowReturn	
	* @param	[In]a param of Type BOOL		
	* @return   	
	*/	
	void SetShowReturn(BOOL ShowReturn) { m_bShowReturn = ShowReturn; }

	/*
	* @brief	SetTemplateFilePath	
	* @param	[In]a param of Type CString sPath		
	* @param	[In]a param of Type CString sSingle		
	* @param	[In]a param of Type CString sMulti		
	* @return   void	
	*/
	void SetTemplateFilePath(CString sPath, CString sSingle, CString sMulti);

	/*
	* @brief	SetFaultReport	
	* @param	[In]a param of Type CFaultReport * pReport		
	* @return   void	
	*/
	void SetFaultReport(CFaultReport* pReport);

/*
	* @brief	InitDisplay	��ʼ����ʾ
	* @return   void	
	*/
	CSize InitDisplay();

	/*
	* @brief	HitTestOscEvent	�ж�ָ�����Ƿ���¼���¼���ֵ��Ԫ����
	* @param	[In]a param of Type CPoint point	ָ��������	
	* @param	[In]a param of Type CString & strPath	����¼���¼���Ӧ��¼���ļ�·��	
	* @return   int	��¼���¼���ֵ��Ԫ���Ϸ��ص�Ԫ�����������ڷ���-1
	*/
	int HitTestOscEvent(CPoint point, CString& strPath);

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

/*
 *  @brief   	GetEventByKey	 �ɹؼ��ֵõ���Ӧ�Ĺ����¼� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *  @return 	CFaultEvent* �����¼�ָ��
 */
	//##ModelId=49B87B8401E4
	CFaultEvent* GetEventByKey(CString sKey);

/*
 *  @brief   	GetEventBySignleKey	 �ɵ����ؼ����ҵ���Ӧ�Ĺ����¼� 
 *  @param 		[In]a param of Type  CString  �ؼ���
 *	@param		[Out]a param of Type CString  β���ؼ���
 *  @return 	CFaultEvent* �����¼�ָ��
 */
	//##ModelId=49B87B8401E6
	CFaultEvent* GetEventBySignleKey(CString sKey, CString & sTail);

/*
 *  @brief   	GetFullPath	 �õ�ָ�������豸��ָ��¼���ļ�������·�� 
 *  @param 		[In]a param of Type  CSecObj*  ָ�������豸
 *  @param 		[In]a param of Type  CString  ָ��¼���ļ�
 *  @return 	CString ����·��
 */
	//##ModelId=49B87B8401F4
	CString GetOscFullPath(CSecObj* pSec, CString strFileName);
};

#endif // !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
