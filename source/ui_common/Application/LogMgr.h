// ***************************************************************
//  LogMgr   version:  1.0    date: 3:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ��־�����ඨ��
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
#if !defined(AFX_LOGMGR_H__B5F11381_25C6_46D2_9142_EABA7E289851__INCLUDED_)
#define AFX_LOGMGR_H__B5F11381_25C6_46D2_9142_EABA7E289851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogMgr.h : header file
//
#include "vccommon_define.h"

/////////////////////////////////////////////////////////////////////////////
// CLogMgr view

/**
 * @defgroup 	CLogMgr	��־������
 * @{
 */
/**
 * @brief       1.д��־�ļ� 2.������־�ļ�(��һ����С�����־�ļ�) 3.������־�Ƿ�д�ļ�,�Ƿ��������־������� 4.��֯��־���
*  @author      LYH
 * @version     ver1.0
 * @date        03/09/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87B990203
class CLogMgr
{
private:
	//##ModelId=49B87B990205
	CLogMgr(CString sPath, CString sFolder = "Temp_Log", int nLogLevel = XJ_LOG_LV3, BOOL bOutPutToFile = TRUE);

public:
	static CLogMgr* getInstance();
	static CLogMgr* CreateInstance(CString sPath, CString sFolder = "Temp_Log", int nLogLevel = XJ_LOG_LV3, BOOL bOutPutToFile = TRUE);

public:
	static CLogMgr* m_instance;
// Attributes
private:
	/** @brief           ��־�ļ�*/
	//##ModelId=49B87B99020D
	CMessageLog		m_File;

	/** @brief			��־�ȼ�*/
	int			m_nLogLevel;

	/** @brief			�Ƿ�������ļ�*/
	BOOL		m_bOutPutToFile;

// Operations
public:
/*
 *  @brief   	SetLogPath	 ������־�ļ�·�� 
 *  @param 		[In]a param of Type  CString&  Ҫ���õ�·��
 *  @return 	void 
 */
	//##ModelId=49B87B990214
	void SetLogPath(CString& path,CString sFolder = "Temp_Log");

/*
 *  @brief   	WriteLog	 д��־����,���ⲿ���� 
 *  @param 		[In]a param of Type  CString&  ��־�ַ���
 *  @param 		[In]a param of Type  const int&  ��־�ȼ�
 *  @return 	void 
 */
	//##ModelId=49B87B990218
	void WriteLog(CString& sLog, const int& nLV = 2);

	/*
	* @brief	GetLogLevel			
	* @return   int	
	*/	
	int GetLogLevel() const { return m_nLogLevel; }

	/*
	* @brief	SetLogLevel	
	* @param	[In]a param of Type int		
	* @return   	
	*/	
	void SetLogLevel(int LogLevel) { m_nLogLevel = LogLevel; m_File.SetLogLevel(m_nLogLevel);}

	/*
	* @brief	GetOutPutToFile			
	* @return   BOOL	
	*/	
	BOOL GetOutPutToFile() const { return m_bOutPutToFile; }

	/*
	* @brief	SetOutPutToFile	
	* @param	[In]a param of Type BOOL		
	* @return   	
	*/	
	void SetOutPutToFile(BOOL OutPutToFile) { m_bOutPutToFile = OutPutToFile; }

private:
/*
 *  @brief   	MarkLogString	 ��֯��־����ĸ�ʽ
 *  @param 		[In]a param of Type  CString*  Ҫ������ַ���
 *  @return 	CString		��ʽ���������ַ���
 */
	//##ModelId=49B87B990226
	CString	MarkLogString(CString& sLog);

/*
 *  @brief   	OutputToFile	 �������־�ļ� 
 *  @param 		[In]a param of Type  CString&  ������ַ���
 *  @return 	void 
 */
	//##ModelId=49B87B990228
	void OutputToFile(CString& sLog, int nLv = 2);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogMgr)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87B990233
	virtual ~CLogMgr();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGMGR_H__B5F11381_25C6_46D2_9142_EABA7E289851__INCLUDED_)
