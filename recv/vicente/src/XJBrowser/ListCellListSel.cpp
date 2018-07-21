// ListCellListSel.cpp : implementation file
//

#include "stdafx.h"
#include "ListCellListSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCellListSel

CListCellListSel::CListCellListSel(int nItem, int nSubItem, CString strInitText)
:m_bEscape (FALSE)
{
    m_nItem = nItem;
    m_nSubItem = nSubItem;
    m_strInitText = strInitText;
}

CListCellListSel::~CListCellListSel()
{
}


BEGIN_MESSAGE_MAP(CListCellListSel, CListBox)
	//{{AFX_MSG_MAP(CListCellListSel)
	ON_WM_CREATE()
	ON_WM_NCDESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCellListSel message handlers

int CListCellListSel::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Set the proper font
	CFont* font = GetParent()->GetFont();
	SetFont(font);

	//解析选项
	CString strTmp;
	while (1)
	{
		strTmp = m_strInitText.Left(m_strInitText.Find(";", 0));
		if (strTmp.IsEmpty())	
		{
			m_strInitText.TrimLeft();
			m_strInitText.TrimRight();
			if (!m_strInitText.IsEmpty())
			{
				AddString(m_strInitText);
			}

			break;
		}
		else
		{
			AddString(strTmp);
		}

		m_strInitText.Delete(0, m_strInitText.Find(";", 0) + 1);
	}

	SetFocus();
	SetCapture();

	return 0;
}

void CListCellListSel::SetListItemText()
{
	CString strText;
	int Index;
	Index = GetCurSel();
	if (Index == -1)
		return;
    GetText(Index, strText);

	//只显示值
	strText = strText.Right(strText.GetLength() - strText.Find(":", 0) - 1);

    //Send Notification to parent of ListView ctrl
    LV_DISPINFO dispinfo;
    dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
    dispinfo.hdr.idFrom = GetDlgCtrlID();
    dispinfo.hdr.code = LVN_ENDLABELEDIT;

    dispinfo.item.mask = LVIF_TEXT;
    dispinfo.item.iItem = m_nItem;
    dispinfo.item.iSubItem = m_nSubItem;
    dispinfo.item.pszText = m_bEscape ? NULL : LPTSTR ((LPCTSTR) strText);
    dispinfo.item.cchTextMax = strText.GetLength();

    GetParent()->GetParent()->SendMessage (WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM) &dispinfo);
}

void CListCellListSel::OnNcDestroy() 
{
	CListBox::OnNcDestroy();
	    delete this;
	// TODO: Add your message handler code here
	
}

BOOL CListCellListSel::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//TRACE("the message is %d\n", pMsg->message);
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_DELETE
			|| pMsg->wParam == VK_ESCAPE
			|| GetKeyState( VK_CONTROL)
			)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;		    	// DO NOT process further
		}
	}
	return CListBox::PreTranslateMessage(pMsg);
}

void CListCellListSel::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);
//	SetListItemText();
	if(GetCapture() == NULL)
		DestroyWindow();
	// TODO: Add your message handler code here
	
}

void CListCellListSel::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	SetListItemText();

	DestroyWindow();	

//	CListBox::OnLButtonDblClk(nFlags, point);
}

void CListCellListSel::OnLButtonUp( UINT nFlags, CPoint point )
{
	CListBox::OnLButtonUp(nFlags, point);
	SetCapture();
}