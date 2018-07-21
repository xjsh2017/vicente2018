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
#include "LogView.h"


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
class CLogMgr : public CObject
{
public:
	//##ModelId=49B87B990205
	CLogMgr();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLogMgr)

// Attributes
public:
private:
	//##ModelId=49B87B990206
	CString		m_sFullPath;	//��־�ļ�ȫ·��
	//##ModelId=49B87B990208
	CLogView*	m_pLogView;		//��־����ָ��
	/** @brief           ��־�ļ�*/
	//##ModelId=49B87B99020D
	CMessageLog		m_File;
	/** @brief           �Ƿ��Ѵ���־�ļ�*/
	//##ModelId=49B87B990213
	BOOL		m_bLogOpened;

// Operations
public:
/*
 *  @brief   	SetLogPath	 ������־�ļ�·�� 
 *  @param 		[In]a param of Type  CString&  Ҫ���õ�·��
 *  @return 	void 
 */
	//##ModelId=49B87B990214
	void SetLogPath(CString& path);

/*
 *  @brief   	SetLogView	 ������־����ָ�� 
 *  @param 		[In]a param of Type  CLogView*  ��־�������ָ��
 *  @return 	void 
 */
	//##ModelId=49B87B990216
	void SetLogView(CLogView* pView);

/*
 *  @brief   	WriteLog	 д��־����,���ⲿ���� 
 *  @param 		[In]a param of Type  CString&  ��־�ַ���
 *  @param 		[In]a param of Type  const int&  ��־�ȼ�
 *  @return 	void 
 */
	//##ModelId=49B87B990218
	void WriteLog(CString& sLog, const int& nLV = 2);

/*
 *  @brief   	WriteLog	 д��־����־�ļ�
 *  @param 		[In]a param of Type  CString  ��־����
 *  @param 		[In]a param of Type  int  ��־�ȼ�
 *  @return 	void 
 */
	//##ModelId=49B87B990223
	void WriteLogEx(CString sLog, int nLV = 2);

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
