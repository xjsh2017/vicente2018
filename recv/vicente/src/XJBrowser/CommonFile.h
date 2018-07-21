#if !defined(AFX_COMMONFILE_H__C4F4804A_E619_43F4_812D_AF9408BECEF1__INCLUDED_)
#define AFX_COMMONFILE_H__C4F4804A_E619_43F4_812D_AF9408BECEF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommonFile window

/**
 * @defgroup 	CCommonFile	ͨ���ļ���
 * @{
 */
/**
 * @brief       ����ͨ���ļ���Ϣ
*  @author      LYH
 * @version     ver1.0
 * @date        27/11/08

 *
 * example
 * @code
 *    // ����˵�������ʹ��
 * @endcode
 */
/** @} */ //OVER

//##ModelId=49B87BA903BB
class CCommonFile : public CObject
{
// Construction
public:
	//##ModelId=49B87BA903C8
	CCommonFile();

// Attributes
public:
	/** @brief           �ļ����*/
	//##ModelId=49B87BA903C9
	int		m_nID;
	/** @brief           ��վID*/
	//##ModelId=49B87BA903CA
	CString	m_sStationID;
	/** @brief           ��Ӧ�ĳ�վָ��*/
	//##ModelId=49B87BA903CC
	CStationObj* m_pStation;
	/** @brief           �ļ�·��*/
	//##ModelId=49B87BA903D0
	CString m_sFilePath;
	/** @brief           �ļ���*/
	//##ModelId=49B87BA903D1
	CString m_sFileName;
	/** @brief           �ļ�ʱ��*/
	//##ModelId=49B87BA903D8
	CString m_sFileTime;
	/** @brief           �ļ���С*/
	//##ModelId=49B87BA903D9
	int		m_nFileSize;
	/** @brief           �Ƿ�������, 0-δ����,1-������,2-��������*/
	//##ModelId=49B87BA903DA
	int		m_nIsDownload;
	/** @brief           �ļ���ƫ��λ��*/
	//##ModelId=49B87BA903DB
	int		m_nOffset;

// Operations
public:
/*
 *  @brief   	LoadFromSTTP00204	 ��00204�����еõ��ļ���Ϣ 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  ��������
 *  @param 		[In]a param of Type  int ѭ��������
 *  @return 	BOOL ��ȡ��Ϣ�ɹ�����TRUE, ʧ�ܷ���FALSE
 */
	//##ModelId=49B87BA903DC
	BOOL LoadFromSTTP00204(STTP_FULL_DATA* pSttpData, int nIndex);

/*
 *  @brief   	GetFileNameByPath	 ���ļ�·���õ��ļ���(����׺) 
 *  @param 		[In]a param of Type  CString  �ļ�·��
 *  @return 	CString �ļ���
 */
	//##ModelId=49B87BA903DF
	CString GetFileNameByPath(CString strPath);

/*
 *  @brief   	IsCommFileDownloaded	 �ж�ָ��ͨ���ļ��Ƿ������� 
 *  @param 		[In]a param of Type  CString  ָ����վ
 *  @param 		[In]a param of Type  CString  ָ���ļ���
 *  @return 	int 0-δ����, 1-������, 2-��������
 */
	//##ModelId=49B87BAA0001
	int IsCommFileDownloaded(CString sStationID, CString sFileName);

/*
 *  @brief   	GetDownloadString	 �õ��Ƿ��������� 
 *  @return 	CString �Ƿ���������
 */
	//##ModelId=49B87BAA0004
	CString GetDownloadString();

/*
 *  @brief   	GetFileRelatePath	 �õ��ļ����·���� 
 *  @return 	CString ���·��
 */
	//##ModelId=49B87BAA0005
	CString	GetFileRelatePath();

	CString GetFilePath();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonFile)
	//}}AFX_VIRTUAL

// Implementation
public:
	//##ModelId=49B87BAA0006
	virtual ~CCommonFile();

	// Generated message map functions
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONFILE_H__C4F4804A_E619_43F4_812D_AF9408BECEF1__INCLUDED_)
