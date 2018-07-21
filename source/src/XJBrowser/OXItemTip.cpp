// ==========================================================================
// 							   Class Implementation : 
//									COXItemTip
// ==========================================================================

// Product Version: 7.52

// Copyright © Dundas Software Ltd. 1997 - 2002, All Rights Reserved
                          
// //////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <winuser.h>
#include "OXItemTip.h"
#include <afxcview.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(COXItemTip, COXHookWnd);

void COXItemTip::NeedItemTip(LPNEEDITEMTIPINFO pnitInfo)
{
	ASSERT(::IsWindow(m_hWndHooked));
	ASSERT(m_hWndHooked==m_pAttachedCtrl->m_hWnd);

	// Make sure that the ListCtrl is in report view
	if((m_pAttachedCtrl->GetStyle()&LVS_TYPEMASK)!=LVS_REPORT)
	{
		pnitInfo->result=ID_NIT_WRONGFORMAT;
		return;
	}

	// check if list control has any items
	if(m_pAttachedCtrl->GetItemCount()==0)
	{
		//changed 12/15/99
		pnitInfo->result=ID_NIT_OUTOFCONTROLBORDER;//ID_NIT_NOTHIDDEN;
		return;
	}


	CPoint point;
	point.x=pnitInfo->point.x;
	point.y=pnitInfo->point.y;

	CRect rectItem=pnitInfo->rectItem;

	// Get the top and bottom row visible
	int nTopRow=m_pAttachedCtrl->GetTopIndex();
	int nBottomRow=nTopRow+m_pAttachedCtrl->GetCountPerPage();
	if(nBottomRow>=m_pAttachedCtrl->GetItemCount())
	{
		nBottomRow=m_pAttachedCtrl->GetItemCount()-1;
	}

	int nRow=nTopRow;
	while(TRUE)
	{
		ASSERT(nRow<m_pAttachedCtrl->GetItemCount());

		m_pAttachedCtrl->GetItemRect(nRow,&rectItem,LVIR_BOUNDS);
		if(rectItem.PtInRect(point))
		{
			break;
		}
		nRow++;

		if(nRow>nBottomRow)
		{
			pnitInfo->result=ID_NIT_OUTOFCONTROLBORDER;
			return;
		}
	}
	pnitInfo->row=nRow;

	int nCol=0;
	int nColReal=0;
	while(TRUE)
	{
		nColReal=GetRealColumn(nCol);
		int nColWidth=m_pAttachedCtrl->GetColumnWidth(nColReal);
		if(nColWidth<0)
		{
			pnitInfo->result=ID_NIT_OUTOFCONTROLBORDER;
			return;
		}

		rectItem.right=rectItem.left+nColWidth;
		if(rectItem.PtInRect(point))
		{
			break;
		}
		rectItem.left=rectItem.right;
		nCol++;
	}
	pnitInfo->col=nColReal;

	if(nColReal==0)
	{
		CRect rcLabel;
		m_pAttachedCtrl->GetItemRect(nRow, &rcLabel, LVIR_LABEL);

		rectItem=rcLabel;
		rectItem.InflateRect(1,0);
		if(!rectItem.PtInRect(point))
		{
			pnitInfo->result=ID_NIT_OUTOFITEMBORDER;
			return;
		}
	}

	int nOffset=(nColReal==0 ? pnitInfo->offset/2 : pnitInfo->offset);
	rectItem.DeflateRect(nOffset,0);

	CString sText=m_pAttachedCtrl->GetItemText(nRow,nColReal);
	if(sText.IsEmpty())
	{
		pnitInfo->result=ID_NIT_NOTHIDDEN;
		return;
	}
	lstrcpyn(pnitInfo->itemText,sText,pnitInfo->sizeText);

	CClientDC dc(m_pAttachedCtrl);
	CFont* pOldFont=pnitInfo->pFont==NULL ? NULL : 
		dc.SelectObject((CFont*)pnitInfo->pFont);
	CRect rectText(0, 0, 0, 0);
	dc.DrawText(sText,&rectText,DT_CALCRECT|DT_LEFT|DT_SINGLELINE);
	if(pOldFont)
	{
		dc.SelectObject(pOldFont);
	}

	CRect rectClient;
	m_pAttachedCtrl->GetClientRect(&rectClient);

	LV_COLUMN lvc;
	memset(&lvc,0,sizeof(LV_COLUMN));
	lvc.mask=LVCF_FMT;
	VERIFY(m_pAttachedCtrl->GetColumn(nColReal, &lvc));

	int fmt=lvc.fmt&LVCFMT_JUSTIFYMASK;
	int nTextWidth=rectText.Width();
	switch(fmt)
	{
	case LVCFMT_LEFT:
		{
			rectText.left=rectItem.left;
			rectText.right=rectText.left+nTextWidth;
			pnitInfo->alignment=DT_LEFT;
			break;
		}
	case LVCFMT_RIGHT:
		{
			rectText.right=rectItem.right;
			rectText.left=rectText.right-nTextWidth;
			pnitInfo->alignment=DT_RIGHT;
			break;
		}
	case LVCFMT_CENTER:
		{
			int nCenter=rectItem.left+(rectItem.right-rectItem.left)/2;
			rectText.left=nCenter-nTextWidth/2;
			rectText.right=nCenter+nTextWidth/2;
			pnitInfo->alignment=DT_CENTER;
			break;
		}
	default:
		ASSERT(FALSE);
	}

	CRect rectItemWindow=rectItem;
	m_pAttachedCtrl->ClientToScreen(rectItemWindow);
	CWnd* pParentWnd=m_pAttachedCtrl->GetParent();
	BOOL bTopMostParent=FALSE;
	while(pParentWnd!=NULL)
	{
		if(pParentWnd->GetExStyle() & WS_EX_TOPMOST)
		{
			bTopMostParent=TRUE;
			break;
		}
		pParentWnd=pParentWnd->GetParent();
	}
	// check if item rectangle fits into the screen
	DWORD dwMessagePos=::GetMessagePos();
	CPoint ptHitTest(GET_X_LPARAM(dwMessagePos),GET_Y_LPARAM(dwMessagePos));
	CRect rectDisplay=COXItemTipWnd::GetMonitorRectFromPoint(ptHitTest,!bTopMostParent);

	if(rectText.Width()<=rectItem.Width() && 
		rectText.right<=rectClient.right && 
		rectText.left>=rectClient.left &&
		rectItemWindow.right<=rectDisplay.right &&
		rectItemWindow.left>=rectDisplay.left)
	{
		pnitInfo->result=ID_NIT_NOTHIDDEN;
		return;
	}

	if(rectItem.Width()<rectText.Width())
	{
		rectItem.right=rectItem.left+rectText.Width();
	}
	rectItem.InflateRect(nOffset,0);

	pnitInfo->rectItem.left=rectItem.left;
	pnitInfo->rectItem.right=rectItem.left+__min(rectItem.Width(),rectText.Width());
	pnitInfo->rectItem.top=rectItem.top;
	pnitInfo->rectItem.bottom=rectItem.bottom;

	pnitInfo->offset=nOffset;

	BOOL bSelected=(m_pAttachedCtrl->GetItemState(pnitInfo->row,LVIS_SELECTED)==
		LVIS_SELECTED);
	if(pnitInfo->clrText==ID_OX_COLOR_NONE)
	{
		pnitInfo->clrText=(bSelected ? ::GetSysColor(COLOR_HIGHLIGHTTEXT) : 
			m_pAttachedCtrl->GetTextColor());
	}
	if(pnitInfo->clrBackground==ID_OX_COLOR_NONE)
	{
		pnitInfo->clrBackground=(bSelected ? ::GetSysColor(COLOR_HIGHLIGHT) : 
			pnitInfo->clrBackground);
	}


	pnitInfo->result=ID_NIT_SUCCESS;
}


int COXItemTip::GetRealColumn(int nCol)
{
#if (_WIN32_IE >= 0x0300)
	ASSERT(::IsWindow(m_hWndHooked));

	//ASSERT(GetHookedWnd()->IsKindOf(RUNTIME_CLASS(CListCtrl)));
	//changed 12/15/99
	ASSERT(GetHookedWnd()->IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
		GetHookedWnd()->IsKindOf(RUNTIME_CLASS(CListView)));

	ASSERT(m_hWndHooked==m_pAttachedCtrl->m_hWnd);

    // Get the header control 
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_pAttachedCtrl->GetDlgItem(0);
    ASSERT(pHeader);

    // get the current number of columns 
    int nCount=pHeader->GetItemCount();
	ASSERT(nCol<nCount);

    // find the real column number. We will request new HDI_ORDER info
    for (int nIndex=0; nIndex<nCount; nIndex++)
    {
        HD_ITEM hdItem;
        hdItem.mask=HDI_ORDER;
        BOOL bReturn = pHeader->GetItem(nIndex,&hdItem);
        ASSERT(bReturn);

        if(hdItem.iOrder==nCol)
        {
            return nIndex;
        }
    }

	// we shouldn't be here
    ASSERT(FALSE);

    return -1;
#else
	return nCol;
#endif
}


///////////////////////////////////////////////////////
///////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(COXEditTip, COXHookWnd);

BOOL COXEditTip::Attach(CWnd* pWnd, NEEDITEMTIP_HELPER pfnCallback/*=NULL*/)
{
	if(!COXBaseItemTip<CEdit>::Attach(pWnd,pfnCallback))
		return FALSE;

	if(!InstallSpy())
	{
		Detach();
		return FALSE;
	}

	return TRUE;
}

void COXEditTip::NeedItemTip(LPNEEDITEMTIPINFO pnitInfo)
{
	ASSERT(::IsWindow(m_hWndHooked));
	ASSERT(m_hWndHooked==m_pAttachedCtrl->m_hWnd);

	// we already been there so we don't need to show the tip again
	if(IsMouseOver())
	{
		pnitInfo->result=ID_NIT_CUSTOMREJECT;
		return;
	}

	DWORD dwStyle=m_pAttachedCtrl->GetStyle();
	// Make sure that the Edit control has single line style
	if((dwStyle&ES_MULTILINE)==ES_MULTILINE)
	{
		pnitInfo->result=ID_NIT_WRONGFORMAT;
		return;
	}

	CString sText;
	m_pAttachedCtrl->GetWindowText(sText);
	if(sText.IsEmpty())
	{
		pnitInfo->result=ID_NIT_NOTHIDDEN;
		return;
	}
	if(sText.GetLength()>pnitInfo->sizeText)
	{
		pnitInfo->result=ID_NIT_WRONGFORMAT;
		return;
	}

	CRect rectItem;
	m_pAttachedCtrl->GetRect(rectItem);
	CRect rectClient;
	m_pAttachedCtrl->GetClientRect(rectClient);
	CRect rectWindow;
	m_pAttachedCtrl->GetWindowRect(rectWindow);
	m_pAttachedCtrl->ScreenToClient(rectWindow);

	pnitInfo->offset=rectItem.left-rectWindow.left;

	CClientDC dc(m_pAttachedCtrl);
	CFont* pOldFont=pnitInfo->pFont==NULL ? NULL : 
		dc.SelectObject((CFont*)pnitInfo->pFont);
	CRect rectText(0, 0, 0, 0);
	dc.DrawText(sText,&rectText,DT_CALCRECT|DT_LEFT|DT_SINGLELINE);
	if(pOldFont)
	{
		dc.SelectObject(pOldFont);
	}

	if(rectText.Width()<=rectItem.Width() && rectText.Height()<=rectItem.Height())
	{
		pnitInfo->result=ID_NIT_NOTHIDDEN;
		return;
	}
	rectItem.right=rectItem.left+rectText.Width();
	rectItem.bottom=rectItem.top+rectText.Height();

	if((dwStyle&ES_CENTER)==ES_CENTER)
		pnitInfo->alignment=DT_CENTER;
	else if((dwStyle&ES_RIGHT)==ES_RIGHT)
		pnitInfo->alignment=DT_RIGHT;
	else
		pnitInfo->alignment=DT_LEFT;

	pnitInfo->rectItem.left=rectWindow.left;
	pnitInfo->rectItem.top=rectWindow.top;
	pnitInfo->rectItem.right=rectWindow.left+rectItem.Width();
	pnitInfo->rectItem.bottom=rectWindow.bottom;

	lstrcpyn(pnitInfo->itemText,sText,pnitInfo->sizeText);

	if(pnitInfo->clrText==ID_OX_COLOR_NONE)
		pnitInfo->clrText=::GetSysColor(COLOR_WINDOWTEXT);

	pnitInfo->result=ID_NIT_SUCCESS;
}



///////////////////////////////////////////////////////
///////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(COXListBoxTip, COXHookWnd);

void COXListBoxTip::NeedItemTip(LPNEEDITEMTIPINFO pnitInfo)
{
	ASSERT(::IsWindow(m_hWndHooked));
	ASSERT(m_hWndHooked==m_pAttachedCtrl->m_hWnd);

	CPoint ptTest(pnitInfo->point);
	BOOL bOutside=TRUE;
	int nItemIndex=m_pAttachedCtrl->ItemFromPoint(ptTest,bOutside);
	if(nItemIndex<0 || nItemIndex>=m_pAttachedCtrl->GetCount() || bOutside)
	{
		pnitInfo->result=ID_NIT_OUTOFCONTROLBORDER;
		return;
	}


	CString sText;
	m_pAttachedCtrl->GetText(nItemIndex,sText);
	if(sText.IsEmpty())
	{
		pnitInfo->result=ID_NIT_NOTHIDDEN;
		return;
	}


	CRect rectItem;
	m_pAttachedCtrl->GetItemRect(nItemIndex,rectItem);

	pnitInfo->offset=ID_ITEMTIP_XOFFSET_LISTBOX;

	if(!rectItem.PtInRect(ptTest))
	{
		pnitInfo->result=ID_NIT_OUTOFITEMBORDER;
		return;
	}

	pnitInfo->row=nItemIndex;

	CClientDC dc(m_pAttachedCtrl);
	CFont* pOldFont=pnitInfo->pFont==NULL ? NULL : 
		dc.SelectObject((CFont*)pnitInfo->pFont);
	CRect rectText(0, 0, 0, 0);
	dc.DrawText(sText,&rectText,DT_CALCRECT|DT_LEFT|DT_SINGLELINE);
	if(pOldFont)
	{
		dc.SelectObject(pOldFont);
	}
	rectText.InflateRect(pnitInfo->offset,0);
	if(rectText.Width()<=rectItem.Width() && rectText.Height()<=rectItem.Height())
	{
		pnitInfo->result=ID_NIT_NOTHIDDEN;
		return;
	}
	rectItem.right=rectItem.left+rectText.Width();
	rectItem.bottom=rectItem.top+__max(rectText.Height(),rectItem.Height());

	pnitInfo->alignment=DT_LEFT;

	pnitInfo->rectItem.left=rectItem.left;
	pnitInfo->rectItem.top=rectItem.top;
	pnitInfo->rectItem.right=rectItem.right;
	pnitInfo->rectItem.bottom=rectItem.bottom;

	lstrcpyn(pnitInfo->itemText,sText,pnitInfo->sizeText);

	BOOL bSelected=(m_pAttachedCtrl->GetSel(pnitInfo->row)!=0);
	if(pnitInfo->clrText==ID_OX_COLOR_NONE)
	{
		pnitInfo->clrText=(bSelected ? ::GetSysColor(COLOR_HIGHLIGHTTEXT) : 
			::GetSysColor(COLOR_WINDOWTEXT));
	}
	if(pnitInfo->clrBackground==ID_OX_COLOR_NONE)
	{
		pnitInfo->clrBackground=(bSelected ? ::GetSysColor(COLOR_HIGHLIGHT) : 
			pnitInfo->clrBackground);
	}

	pnitInfo->result=ID_NIT_SUCCESS;
}


///////////////////////////////////////////////////////
///////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(COXComboBoxTipHelper, CComboBox);

void COXComboBoxTipHelper::SubclassListBoxCtrl(CWnd* pWnd)
{
	COXBaseSubclassedComboBox<CComboBox, CListBox, CEdit>::SubclassListBoxCtrl(pWnd);
	m_listBoxTip.Attach(GetListBoxCtrl());
}

void COXComboBoxTipHelper::SubclassEditCtrl(CWnd* pWnd)
{
	COXBaseSubclassedComboBox<CComboBox, CListBox, CEdit>::SubclassEditCtrl(pWnd);
	m_editTip.Attach(GetEditCtrl());
}

////////////////

IMPLEMENT_DYNAMIC(COXComboBoxTip, COXHookWnd);

void COXComboBoxTip::NeedItemTip(LPNEEDITEMTIPINFO pnitInfo)
{
	ASSERT(::IsWindow(m_hWndHooked));
	ASSERT(m_hWndHooked==m_pAttachedCtrl->m_hWnd);

	DWORD dwStyle=m_pAttachedCtrl->GetStyle();
	// Make sure that the combo box control has dropdown style
	if((dwStyle&CBS_DROPDOWNLIST)!=CBS_DROPDOWNLIST)
	{
		pnitInfo->result=ID_NIT_WRONGFORMAT;
		return;
	}

	CPoint ptTest(pnitInfo->point);
	CRect rectItem;
	m_pAttachedCtrl->GetClientRect(rectItem);
	rectItem.right-=::GetSystemMetrics(SM_CXVSCROLL);
	if(!rectItem.PtInRect(ptTest))
	{
		pnitInfo->result=ID_NIT_OUTOFITEMBORDER;
		return;
	}


	CString sText;
	m_pAttachedCtrl->GetWindowText(sText);
	if(sText.IsEmpty())
	{
		pnitInfo->result=ID_NIT_NOTHIDDEN;
		return;
	}

	pnitInfo->offset=ID_ITEMTIP_XOFFSET_COMBOBOX;

	CClientDC dc(m_pAttachedCtrl);
	CFont* pOldFont=pnitInfo->pFont==NULL ? NULL : 
		dc.SelectObject((CFont*)pnitInfo->pFont);
	CRect rectText(0, 0, 0, 0);
	dc.DrawText(sText,&rectText,DT_CALCRECT|DT_LEFT|DT_SINGLELINE);
	if(pOldFont)
	{
		dc.SelectObject(pOldFont);
	}
	rectText.InflateRect(pnitInfo->offset,0);
	if(rectText.Width()<=rectItem.Width() && rectText.Height()<=rectItem.Height())
	{
		pnitInfo->result=ID_NIT_NOTHIDDEN;
		return;
	}
	rectItem.right=rectItem.left+rectText.Width();
	rectItem.bottom=rectItem.top+__max(rectText.Height(),rectItem.Height());

	pnitInfo->alignment=DT_LEFT;

	pnitInfo->rectItem.left=rectItem.left;
	pnitInfo->rectItem.top=rectItem.top;
	pnitInfo->rectItem.right=rectItem.right;
	pnitInfo->rectItem.bottom=rectItem.bottom;

	lstrcpyn(pnitInfo->itemText,sText,pnitInfo->sizeText);

	if(pnitInfo->clrText==ID_OX_COLOR_NONE)
	{
		pnitInfo->clrText=::GetSysColor(COLOR_WINDOWTEXT);
	}

	pnitInfo->result=ID_NIT_SUCCESS;
}

