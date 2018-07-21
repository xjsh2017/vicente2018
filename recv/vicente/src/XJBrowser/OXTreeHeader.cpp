// Copyright © Dundas Software Ltd. 1997 - 2002, All Rights Reserved
                         
// OXTreeHeader.cpp : implementation file
//
// Product Version: 7.52


#include "stdafx.h"
#include "OXTreeHeader.h"
#include "OXTreeCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXTreeHeader

COXTreeHeader::COXTreeHeader()
{
	m_nSortCol=-1;
	m_nSortOrder=0;

	m_bLButtonDown=FALSE;
}

COXTreeHeader::~COXTreeHeader()
{
}


BEGIN_MESSAGE_MAP(COXTreeHeader, CHeaderCtrl)
	//{{AFX_MSG_MAP(COXTreeHeader)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXTreeHeader message handlers

void COXTreeHeader::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	HD_HITTESTINFO hti;
	hti.pt = point;
	SendMessage(HDM_HITTEST,0,(LPARAM)(&hti));
	if(hti.flags & HHT_ONDIVIDER && hti.iItem != -1)
	{
		// the user double-clicked on column divider
		((COXTreeCtrl*)GetParent())->ResizeColToFit(hti.iItem);
	}
	else
		CHeaderCtrl::OnLButtonDblClk(nFlags, point);
}

void COXTreeHeader::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(GetFocus() != GetParent())
		GetParent()->SetFocus();
	m_bLButtonDown=TRUE;
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void COXTreeHeader::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CHeaderCtrl::OnLButtonUp(nFlags, point);

	if(m_bLButtonDown && GetFocus()==GetParent())
	{
		CWnd* pParentWnd=GetParent();
		ASSERT(pParentWnd->IsKindOf(RUNTIME_CLASS(COXTreeCtrl)));
		if(((COXTreeCtrl*)pParentWnd)->IsHeaderSorting())
		{
			CHeaderCtrl::OnLButtonDown(nFlags, point);
			HD_HITTESTINFO hti;
			hti.pt = point;
			SendMessage(HDM_HITTEST,0,(LPARAM)(&hti));
			if(hti.flags & HHT_ONHEADER  && hti.iItem != -1 && 
				!(hti.flags & HHT_ONDIVIDER))
			{
				// the user clicked on column 
				int nSortOrder=(hti.iItem==m_nSortCol) ? 
					(m_nSortOrder!=0 ? m_nSortOrder*-1 : 1) : 1;
				((COXTreeCtrl*)pParentWnd)->
					SortChildren(NULL,hti.iItem,nSortOrder==1 ? TRUE : FALSE);
			}
		}
	}
	m_bLButtonDown=FALSE;
}

BOOL COXTreeHeader::SortColumn(int nCol, int nSortOrder)
{
	ASSERT(nCol==-1 || (nCol>=0 && nCol<=GetItemCount()));

	if(nCol==-1)
		nSortOrder=0;

	CWnd* pParentWnd=GetParent();
	ASSERT(pParentWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)));

	if(nCol!=m_nSortCol)
	{
		// Change the item from ownder drawn	
		HD_ITEM hditem;	
		hditem.mask=HDI_FORMAT;
		GetItem(m_nSortCol,&hditem);
		hditem.fmt&=~HDF_OWNERDRAW;
		hditem.fmt|=HDF_STRING;
		SetItem(m_nSortCol,&hditem);

		if(nSortOrder!=0)
		{
			// Change the item to ownder drawn	
			HD_ITEM hditem;	
			hditem.mask=HDI_FORMAT;
			GetItem(nCol,&hditem);
			hditem.fmt|=HDF_OWNERDRAW;
			SetItem(nCol,&hditem);
		}

		// Invalidate header control so that it gets redrawn
		Invalidate();
	}

	m_nSortCol=nCol;
	m_nSortOrder=nSortOrder;

	return TRUE;
}

void COXTreeHeader::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{	
	CDC dc;
	dc.Attach( lpDrawItemStruct->hDC );	
	// Get the column rect
	CRect rcLabel( lpDrawItemStruct->rcItem );	
	// Save DC
	int nSavedDC = dc.SaveDC();
	// Set clipping region to limit drawing within column	
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcLabel );	
	dc.SelectObject( &rgn );
	rgn.DeleteObject(); 
	// Draw the background
	CBrush brush(::GetSysColor(COLOR_3DFACE));
	dc.FillRect(rcLabel,&brush);	
	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character
	int offset = dc.GetTextExtent(_T(" "), 1 ).cx*2;
	// Get the column text and format	
	TCHAR buf[256];	
	HD_ITEM hditem;	
	hditem.mask = HDI_TEXT | HDI_FORMAT;	
	hditem.pszText = buf;
	hditem.cchTextMax = 255;	
	GetItem( lpDrawItemStruct->itemID, &hditem );
	// Determine format for drawing column label
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | 
		DT_VCENTER | DT_END_ELLIPSIS ;	
	if( hditem.fmt & HDF_CENTER)
		uFormat |= DT_CENTER;	
	else if( hditem.fmt & HDF_RIGHT)
		uFormat |= DT_RIGHT;
	else
		uFormat |= DT_LEFT;
	// Adjust the rect if the mouse button is pressed on it
	if( lpDrawItemStruct->itemState == ODS_SELECTED )	
	{		
		rcLabel.left++;
		rcLabel.top += 2;		
		rcLabel.right++;	
	}
	// Adjust the rect further if Sort arrow is to be displayed
	if( lpDrawItemStruct->itemID == (UINT)m_nSortCol )	
	{
		rcLabel.right -= 3 * offset;	
	}
	rcLabel.left += offset;
	rcLabel.right -= offset;	
	// Draw column label
	if( rcLabel.left < rcLabel.right )
		dc.DrawText(buf,-1,rcLabel, uFormat);
	// Draw the Sort arrow	
	if( m_nSortOrder!=0 && lpDrawItemStruct->itemID == (UINT)m_nSortCol )	
	{
		CRect rcIcon( lpDrawItemStruct->rcItem );
		// Set up pens to use for drawing the triangle
		CPen penLight(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
		CPen penShadow(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
		CPen *pOldPen = dc.SelectObject( &penLight );		
		if( m_nSortOrder==1 )		
		{
			// Draw triangle pointing upwards
			dc.MoveTo( rcIcon.right - 2*offset, offset-1);
			dc.LineTo( rcIcon.right - 3*offset/2, rcIcon.bottom - offset );
			dc.LineTo( rcIcon.right - 5*offset/2-2, rcIcon.bottom - offset );
			dc.MoveTo( rcIcon.right - 5*offset/2-1, rcIcon.bottom - offset-1 );
			dc.SelectObject( &penShadow );
			dc.LineTo( rcIcon.right - 2*offset, offset-2);		
		}
		else		
		{
			// Draw triangle pointing downwords
			dc.MoveTo( rcIcon.right - 3*offset/2, offset-1);
			dc.LineTo( rcIcon.right - 2*offset-1, rcIcon.bottom - offset + 1 );
			dc.MoveTo( rcIcon.right - 2*offset-1, rcIcon.bottom - offset );
			dc.SelectObject( &penShadow );
			dc.LineTo( rcIcon.right - 5*offset/2-1, offset -1 );
			dc.LineTo( rcIcon.right - 3*offset/2, offset -1);		
		}
		// Restore the pen
		dc.SelectObject( pOldPen );	
	}	
	// Restore dc	
	dc.RestoreDC( nSavedDC );
	// Detach the dc before returning	
	dc.Detach();
}