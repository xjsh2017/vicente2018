// MemRow.h: interface for the CMemRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMROW_H__5541E489_7C7E_4726_A476_4751CE1E8D09__INCLUDED_)
#define AFX_MEMROW_H__5541E489_7C7E_4726_A476_4751CE1E8D09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EntityContainer.h"

class CMemRow  
{
public:
	CMemRow();
	virtual ~CMemRow();
public:
	void SetRowLength(int nLength);
	int  GetRowLength();
private:
	UINT nRowLength;
};

#endif // !defined(AFX_MEMROW_H__5541E489_7C7E_4726_A476_4751CE1E8D09__INCLUDED_)
