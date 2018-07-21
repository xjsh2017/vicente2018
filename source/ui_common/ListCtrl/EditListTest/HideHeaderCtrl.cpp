// CHideHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HideHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHideHeaderCtrl

CHideHeaderCtrl::CHideHeaderCtrl()
{
	m_bUseHideCursor = FALSE;
}

CHideHeaderCtrl::~CHideHeaderCtrl()
{
	m_aColum.RemoveAll();
}


BEGIN_MESSAGE_MAP(CHideHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CHideHeaderCtrl)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHideHeaderCtrl message handlers

BOOL CHideHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	
	//TRACE("nHitTest=%d, message=%d\n", nHitTest, message);
	return m_bUseHideCursor ? 
			TRUE : CHeaderCtrl::OnSetCursor(pWnd, nHitTest, message);
}

void CHideHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	HD_HITTESTINFO hd_hittestinfo;
	hd_hittestinfo.pt = point;
	BOOL bFind = FALSE;
	SendMessage(HDM_HITTEST, 0, (LPARAM)(&hd_hittestinfo));
	if(hd_hittestinfo.flags == HHT_ONDIVOPEN) {
	//	TRACE("Divider zero! item index=%d(x:%d, y:%d)\n", hd_hittestinfo.iItem, point.x, point.y);
	}
	if( hd_hittestinfo.iItem>0 && hd_hittestinfo.iItem<GetItemCount() && 
		m_aColum[hd_hittestinfo.iItem].bHide && hd_hittestinfo.flags == HHT_ONDIVOPEN )
	{
		bFind = TRUE;
	//	TRACE("Divider zero(this item)! item index=%d(x:%d, y:%d)\n", hd_hittestinfo.iItem, point.x, point.y);
	}
	m_bUseHideCursor = bFind;
	CHeaderCtrl::OnMouseMove(nFlags, point);
}


BOOL CHideHeaderCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	NMHDR* pnmh = (NMHDR*)lParam;
	//TRACE("code=%d\n", pnmh->code);
	//ע�⣺���治��ʹ�� pnmh->code==HDN_BEGINTRACK
	if (pnmh->code==HDN_BEGINTRACKA || pnmh->code==HDN_BEGINTRACKW) {
		if(m_bUseHideCursor) return *pLResult=TRUE; 
	}

	return CHeaderCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

