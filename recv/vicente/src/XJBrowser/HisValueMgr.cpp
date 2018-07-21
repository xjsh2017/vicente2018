// HisValueMgr.cpp: implementation of the CHisValueMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xjbrowser.h"
#include "HisValueMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHisValueMgr::CHisValueMgr()
{

}

CHisValueMgr::~CHisValueMgr()
{
	Clear();
}

void CHisValueMgr::Clear()
{
	for(int i = 0; i < m_Values.GetSize(); i++)
	{
		CHisValue* pValue = (CHisValue*)m_Values.GetAt(i);
		if(pValue)
			delete pValue;
	}
	m_Values.RemoveAll();
}

CString CHisValueMgr::GetTime( int nValueNo )
{
	if(nValueNo < 0 || nValueNo >= GetCount())
		return "";
	CHisValue* pValue = (CHisValue*)m_Values.GetAt(nValueNo);
	if(!pValue)
		return "";
	return pValue->GetTime();
}

void CHisValueMgr::SetTime( int nValueNo, CString sTime )
{
	if(nValueNo < 0 || nValueNo >= GetCount())
		return ;
	CHisValue* pValue = (CHisValue*)m_Values.GetAt(nValueNo);
	if(!pValue)
		return ;
	pValue->SetTime(sTime);
}

CString CHisValueMgr::GetValue( int nValueNo, int nID )
{
	if(nValueNo < 0 || nValueNo >= GetCount())
		return "";
	CHisValue* pValue = (CHisValue*)m_Values.GetAt(nValueNo);
	if(!pValue)
		return "";
	return pValue->GetValue(nID);
}

int CHisValueMgr::CreateValue( CString sTime )
{
	CHisValue* pValue = new CHisValue(sTime);
	if(!pValue)
		return -1;
	m_Values.Add(pValue);
	return m_Values.GetUpperBound();
}

void CHisValueMgr::AddValue( int nValueNo, int nID, CString sValue )
{
	if(nValueNo < 0 || nValueNo >= GetCount())
		return ;
	CHisValue* pValue = (CHisValue*)m_Values.GetAt(nValueNo);
	if(!pValue)
		return ;
	pValue->AddValue(nID, sValue);
}


