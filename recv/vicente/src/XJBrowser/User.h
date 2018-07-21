// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__C6B72BC5_ED1C_43F5_8C17_B015B882214E__INCLUDED_)
#define AFX_USER_H__C6B72BC5_ED1C_43F5_8C17_B015B882214E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=49B87B880082
class CUser  : public CObject
{
public:
	//##ModelId=49B87B88008D
	CUser();
	DECLARE_DYNCREATE(CUser)
	//##ModelId=49B87B88008E
	virtual ~CUser();
public:
	//##ModelId=49B87B880090
	CString m_strUSER_ID;
	//##ModelId=49B87B880091
	CString m_strGROUP_ID;
	//##ModelId=49B87B880092
	CString m_strName;
	//##ModelId=49B87B880093
	CString m_strPassword;
	//##ModelId=49B87B880094
	CString m_strNotes;

	/** @brief           是否可查看所有子站的信息*/
	BOOL m_bAllStation;
};

#endif // !defined(AFX_USER_H__C6B72BC5_ED1C_43F5_8C17_B015B882214E__INCLUDED_)
