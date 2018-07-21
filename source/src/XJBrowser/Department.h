#if !defined(AFX_DEPARTMENT_H__6E5E7BB8_517A_4591_85B3_C652A0DE3FD5__INCLUDED_)
#define AFX_DEPARTMENT_H__6E5E7BB8_517A_4591_85B3_C652A0DE3FD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Department.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDepartment 

class CDepartment : public CObject
{
// Construction
public:
	CDepartment();
	CDepartment(int nID, CString sName);
	DECLARE_DYNCREATE(CDepartment)
	virtual ~CDepartment();
private:
	/** @brief			单位ID*/
	int m_nID;
	/** @brief			单位名称*/
	CString m_sName;
public:
	/*
	* @brief	GetID			
	* @return   int	
	*/	
	int GetID() const { return m_nID; }
	
	/*
	* @brief	SetID	
	* @param	[In]a param of Type int		
	* @return   	
	*/	
	void SetID(int ID) { m_nID = ID; }
	/*
	* @brief	GetName			
	* @return   CString	
	*/	
	CString GetName() const { return m_sName; }
	
	/*
	* @brief	SetName	
	* @param	[In]a param of Type CString		
	* @return   	
	*/	
	void SetName(CString Name) { m_sName = Name; }
};

#endif // !defined(AFX_DEPARTMENT_H__6E5E7BB8_517A_4591_85B3_C652A0DE3FD5__INCLUDED_)
