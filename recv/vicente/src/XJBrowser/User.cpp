// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "xjbrowser.h"
#include "User.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE (CUser,CObject)

//##ModelId=49B87B88008D
CUser::CUser()
{
	m_strUSER_ID = "";
	m_strGROUP_ID = "";
	m_strName = "";
	m_strPassword = "";
	m_strNotes = "";
	m_bAllStation = TRUE;
}

//##ModelId=49B87B88008E
CUser::~CUser()
{

}
