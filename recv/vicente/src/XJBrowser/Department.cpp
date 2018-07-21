// Department.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "Department.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDepartment
IMPLEMENT_DYNCREATE(CDepartment, CObject)
CDepartment::CDepartment()
{
	m_nID = -1;
	m_sName = "";
}

/****************************************************
Date:2012/12/11  Author:LYH
������:   CDepartment	
����ֵ:   	
���ܸ�Ҫ: 
����: int nID	
����: CString sName	
*****************************************************/
CDepartment::CDepartment( int nID, CString sName )
{
	m_nID = nID;
	m_sName = sName;
}

CDepartment::~CDepartment()
{
}

