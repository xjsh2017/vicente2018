// CHideHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HideHeaderCtrl.h"
#include "XJBrowser.h"


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
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
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
	//注意：下面不能使用 pnmh->code==HDN_BEGINTRACK
	if (pnmh->code==HDN_BEGINTRACKA || pnmh->code==HDN_BEGINTRACKW) {
		if(m_bUseHideCursor) return *pLResult=TRUE; 
	}

	return CHeaderCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

void CHideHeaderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect, rectItem, clientRect;
	GetClientRect(&rect);
	GetClientRect(&clientRect);
	CMemDC memDC(&dc, rect);
	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);
	
	if (g_style == 3)
	{
		memDC.FillSolidRect(&rect, g_ListHeadColor);//CListCtrl标头背景颜色
	}
	else
	{
		memDC.FillSolidRect(&rect, RGB(255,235,155));//CListCtrl标头背景颜色
	}
	int nItems = GetItemCount();
	CBitmap bitmap;
	
	bitmap.LoadBitmap(IDB_LIST_BK);//标题中区分项目的线条，在资源中应该是一条1个像素宽的竖线图片
	
	for(int i = 0; i <nItems; i++)
	{
		
		TCHAR buf1[256];
		HD_ITEM hditem1;
		
		hditem1.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem1.pszText = buf1;
		hditem1.cchTextMax = 255;
		GetItem( i, &hditem1 );
		
		GetItemRect(i, &rect);
		CBitmap* pOldBitmap = NULL;
		
		//make sure we draw the start piece
		//on the first item so it has a left border
		
		//For the following items we will just use the
		//right border of the previous items as the left
		//border
		if(hditem1.iOrder==0)
		{
		}
		else
		{
			pOldBitmap = bitmapDC.SelectObject(&bitmap);
			memDC.BitBlt(rect.left,rect.top,1,20,&bitmapDC,0,0,SRCCOPY);
		}
		//draw the end piece of the column header
		if(i==nItems-1)
		{
			memDC.BitBlt((rect.right-0), 0, 2, 20, &bitmapDC,0,0,SRCCOPY);
			bitmapDC.SelectObject(pOldBitmap);
		}
		
		//rectItem.DeflateRect(2,2,2,2);
		UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_TOP |DT_CENTER | DT_END_ELLIPSIS | DT_VCENTER ;
		DRAWITEMSTRUCT DrawItemStruct;
		GetItemRect(i, &rectItem);
		
		
		DrawItemStruct.CtlType  = 100;
		DrawItemStruct.hDC   = dc.GetSafeHdc();
		DrawItemStruct.itemAction = ODA_DRAWENTIRE; 
		DrawItemStruct.hwndItem  = GetSafeHwnd(); 
		DrawItemStruct.rcItem = rectItem;
		DrawItemStruct.itemID = i;
		DrawItem(&DrawItemStruct);
		
		
		CFont font;
		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = 12;
		if( g_Language.CompareNoCase("en_US") == 0 )
			lf.lfHeight = 16;
		lf.lfWeight = 600;
		strcpy(lf.lfFaceName, StringFromID(IDS_DEFAULT_FONT));
		font.CreateFontIndirect(&lf);
		
		CFont* def_font = memDC.SelectObject(&font);
		
		memDC.SetBkMode(TRANSPARENT);
		memDC.SetTextColor(RGB(0,0,0));//CListCtrlt头部字体颜色
		//rectItem.DeflateRect(2,2,2,2);
		
		TCHAR buf[256];
		HD_ITEM hditem;
		
		hditem.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem.pszText = buf;
		hditem.cchTextMax = 255;
		GetItem( DrawItemStruct.itemID, &hditem );
		
		memDC.DrawText(buf, &rectItem, uFormat);
		memDC.SelectObject(def_font);
		font.DeleteObject();
	}
}

void CHideHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	
}

BOOL CHideHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return false;	
}