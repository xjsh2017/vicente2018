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
	/** @brief			对应的故障报告内容*/
	CFaultReport*	m_pReport;

	/** @brief			动作相对时间计算方式*/
	int m_nFaultActionTime;
	/** @brief			是否显示值为返回的动作*/
	BOOL m_bShowReturn;
	/** @brief			特征量中故障序号是否过滤*/
	BOOL			m_bFilterNULL;
	/** @brief			是否过滤故障测距=0*/
	BOOL			m_bFilterDistance;

	/** @brief			是否过滤III类信息*/
	BOOL			m_bFilterIII;

	CString m_sPath;
	/** @brief			单保护模板文件名称*/
	CString m_sSinglePtName;
	/** @brief			多保护模板文件名称*/
	CString m_sMultiPtName;
	/** @brief			当前显示的模板名*/
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
	* @brief	TranslateSingleKey	解释单个关键字代表的意义
	* @param	[In]a param of Type CString sKey	关键字内容	
	* @return   CString	关键字代表的值
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
	* @brief	InitDisplay	初始化显示
	* @return   void	
	*/
	CSize InitDisplay();

	/*
	* @brief	HitTestOscEvent	判断指定点是否在录波事件的值单元格上
	* @param	[In]a param of Type CPoint point	指定点坐标	
	* @param	[In]a param of Type CString & strPath	返回录波事件对应的录波文件路径	
	* @return   int	在录波事件的值单元格上返回单元格索引，不在返回-1
	*/
	int HitTestOscEvent(CPoint point, CString& strPath);

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

/*
 *  @brief   	GetEventByKey	 由关键字得到对应的故障事件 
 *  @param 		[In]a param of Type  CString  关键字
 *  @return 	CFaultEvent* 故障事件指针
 */
	//##ModelId=49B87B8401E4
	CFaultEvent* GetEventByKey(CString sKey);

/*
 *  @brief   	GetEventBySignleKey	 由单个关键字找到对应的故障事件 
 *  @param 		[In]a param of Type  CString  关键字
 *	@param		[Out]a param of Type CString  尾部关键字
 *  @return 	CFaultEvent* 故障事件指针
 */
	//##ModelId=49B87B8401E6
	CFaultEvent* GetEventBySignleKey(CString sKey, CString & sTail);

/*
 *  @brief   	GetFullPath	 得到指定二次设备的指定录波文件的完整路径 
 *  @param 		[In]a param of Type  CSecObj*  指定二次设备
 *  @param 		[In]a param of Type  CString  指定录波文件
 *  @return 	CString 完整路径
 */
	//##ModelId=49B87B8401F4
	CString GetOscFullPath(CSecObj* pSec, CString strFileName);
};

#endif // !defined(AFX_STTPENGINE_H__453CB061_5EB0_41F3_8BA4_6914D5512CD0__INCLUDED_)
