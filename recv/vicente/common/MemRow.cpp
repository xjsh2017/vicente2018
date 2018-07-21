// MemRow.cpp: implementation of the CMemRow class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#pragma warning(disable:4786)
#include "MemRow.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemRow::CMemRow()
{
	nRowLength = 0;
}

CMemRow::~CMemRow()
{

}

void CMemRow::SetRowLength(int nLength)
{
	nRowLength = nLength;
}

int CMemRow::GetRowLength()
{
	return nRowLength;
}
