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
 * @defgroup 	CCommonFile	通用文件类
 * @{
 */
/**
 * @brief       描述通用文件信息
*  @author      LYH
 * @version     ver1.0
 * @date        27/11/08

 *
 * example
 * @code
 *    // 举例说明类如何使用
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
	/** @brief           文件序号*/
	//##ModelId=49B87BA903C9
	int		m_nID;
	/** @brief           厂站ID*/
	//##ModelId=49B87BA903CA
	CString	m_sStationID;
	/** @brief           对应的厂站指针*/
	//##ModelId=49B87BA903CC
	CStationObj* m_pStation;
	/** @brief           文件路径*/
	//##ModelId=49B87BA903D0
	CString m_sFilePath;
	/** @brief           文件名*/
	//##ModelId=49B87BA903D1
	CString m_sFileName;
	/** @brief           文件时间*/
	//##ModelId=49B87BA903D8
	CString m_sFileTime;
	/** @brief           文件大小*/
	//##ModelId=49B87BA903D9
	int		m_nFileSize;
	/** @brief           是否已下载, 0-未下载,1-已下载,2-部分下载*/
	//##ModelId=49B87BA903DA
	int		m_nIsDownload;
	/** @brief           文件的偏移位置*/
	//##ModelId=49B87BA903DB
	int		m_nOffset;

// Operations
public:
/*
 *  @brief   	LoadFromSTTP00204	 从00204报文中得到文件信息 
 *  @param 		[In]a param of Type  STTP_FULL_DATA*  报文数据
 *  @param 		[In]a param of Type  int 循环体索引
 *  @return 	BOOL 读取信息成功返回TRUE, 失败返回FALSE
 */
	//##ModelId=49B87BA903DC
	BOOL LoadFromSTTP00204(STTP_FULL_DATA* pSttpData, int nIndex);

/*
 *  @brief   	GetFileNameByPath	 由文件路径得到文件名(带后缀) 
 *  @param 		[In]a param of Type  CString  文件路径
 *  @return 	CString 文件名
 */
	//##ModelId=49B87BA903DF
	CString GetFileNameByPath(CString strPath);

/*
 *  @brief   	IsCommFileDownloaded	 判断指定通用文件是否已下载 
 *  @param 		[In]a param of Type  CString  指定厂站
 *  @param 		[In]a param of Type  CString  指定文件名
 *  @return 	int 0-未下载, 1-已下载, 2-部分下载
 */
	//##ModelId=49B87BAA0001
	int IsCommFileDownloaded(CString sStationID, CString sFileName);

/*
 *  @brief   	GetDownloadString	 得到是否下载描述 
 *  @return 	CString 是否下载描述
 */
	//##ModelId=49B87BAA0004
	CString GetDownloadString();

/*
 *  @brief   	GetFileRelatePath	 得到文件相对路径名 
 *  @return 	CString 相对路径
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
