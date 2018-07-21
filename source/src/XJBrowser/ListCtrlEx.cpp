// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"
#include "ListCellEdit.h"
#include "ListCellListSel.h"
#include <afxdb.h>
#include "XJBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx
IMPLEMENT_DYNCREATE(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
{
	m_strOldText.Empty();
	m_nListType = 0;
	pMenu = new CMenu;
	pMenu->CreatePopupMenu();
}

CListCtrlEx::~CListCtrlEx()
{
	m_arrEdited.RemoveAll();
	if(pMenu != NULL)
	{
		pMenu->DestroyMenu();
		delete pMenu;
		pMenu = NULL;
	}
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_CONTEXTMENU()
	ON_COMMAND_RANGE(ITEM_1, ITEM_19, OnCommandItem)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

int CListCtrlEx::HitTestEx (CPoint& Point, int& nSubItem)
{
	nSubItem = 0;
	int ColumnNum = 0;
    int Row = HitTest (Point, NULL);
	
    // Make sure that the ListView is in LVS_REPORT
    if ((GetWindowLong (m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return Row;
	
    // Get the top and bottom row visible
    Row = GetTopIndex();
    int Bottom = Row + GetCountPerPage();
    if (Bottom > GetItemCount())
		Bottom = GetItemCount();
    
    // Get the number of columns
    CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
    int nColumnCount = pHeader->GetItemCount();
	
    // Loop through the visible rows
    for(; Row <= Bottom; Row++)
    {
		// Get bounding rect of item and check whether point falls in it.
		CRect Rect;
		GetItemRect (Row, &Rect, LVIR_BOUNDS);
		if (Rect.PtInRect (Point))
		{
			// Now find the column
			for (ColumnNum = 0; ColumnNum < nColumnCount; ColumnNum++)
			{
				int ColWidth = GetColumnWidth (ColumnNum);
				if (Point.x >= Rect.left && Point.x <= (Rect.left + ColWidth))
				{
					nSubItem = ColumnNum;
					return Row;
				}
				Rect.left += ColWidth;
			}
		}
    }

    return -1;
}

CEdit* CListCtrlEx::EditSubItem(int nItem, int nSubItem, double nMinValue, double nMaxValue, int type, int DataType/* = 3*/, float fStepValue/* = -1*/, int nTextLength /*= -1*/,int nJudge, bool intasfloat /*= false*/)
{
    // The returned pointer should not be saved

    // Make sure that the item is visible
    if (!EnsureVisible (nItem, TRUE)) return NULL;

    // Make sure that nCol is valid
    CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
    int nColumnCount = pHeader->GetItemCount();
    if (nSubItem >= nColumnCount || GetColumnWidth (nSubItem) < 5)
		return NULL;

    // Get the column offset
    int Offset = 0;
    for (int iColumn = 0; iColumn < nSubItem; iColumn++)
		Offset += GetColumnWidth (iColumn);

    CRect Rect;
    GetItemRect (nItem, &Rect, LVIR_BOUNDS);

    // Now scroll if we need to expose the column
    CRect ClientRect;
    GetClientRect (&ClientRect);
    if (Offset + Rect.left < 0 || Offset + Rect.left > ClientRect.right)
    {
		CSize Size;
		if (Offset + Rect.left > 0)
			Size.cx = -(Offset - Rect.left);
		else
			Size.cx = Offset - Rect.left;
		Size.cy = 0;
		Scroll (Size);
		Rect.left -= Size.cx;
    }

    // Get nSubItem alignment
    LV_COLUMN lvCol;
    lvCol.mask = LVCF_FMT;
    GetColumn (nSubItem, &lvCol);
    DWORD dwStyle;
    if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
    else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
    else dwStyle = ES_CENTER;

    Rect.left += Offset+4;
    Rect.right = Rect.left + GetColumnWidth (nSubItem) - 3;
    if (Rect.right > ClientRect.right)
		Rect.right = ClientRect.right;

    dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL;

	//保存原来的字符
	m_strOldText = GetItemText (nItem, nSubItem);
    CEdit *pEdit = new CListCellEdit (nItem, nSubItem, m_strOldText, nMinValue, nMaxValue,type, DataType, fStepValue, nTextLength,nJudge,intasfloat);
    pEdit->Create (dwStyle, Rect, this, IDB_EDITCELL);
    return pEdit;
}

void CListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
    if (GetFocus() != this) SetFocus();
	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
    if (GetFocus() != this) SetFocus();

	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CListCtrlEx::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CListCtrlEx::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
    LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
    LV_ITEM	*plvItem = &plvDispInfo->item;

    if (plvItem->pszText != NULL && m_strOldText != plvItem->pszText)
    {
		CString str = plvItem->pszText;
		if(m_nListType == 0)
			str += "*";
		if(m_nListType == 1)
		{
			for(int i = 0; i < this->GetItemCount(); i++)
			{
				CString strTemp = this->GetItemText(i,plvItem->iSubItem);
				if(!strTemp.IsEmpty())
				{
					if(plvItem->pszText!=NULL && plvItem->iItem != i)
					{
						if(strTemp == plvItem->pszText)
						{
							SetItemText (i, plvItem->iSubItem, "");
						}
					}
				}
			}
		}
		SetItemText (plvItem->iItem, plvItem->iSubItem, str);

		//查找是否已经存有此Item位置信息
		CPoint ptItem;
		BOOL bFound = FALSE;
		for (int i = 0; i < m_arrEdited.GetSize(); i ++)
		{
			ptItem = m_arrEdited.GetAt(i);
			if (ptItem.x == plvItem->iItem)
			{
				bFound = TRUE;
				break;
			}
		}

		//保存修改了的Item位置信息
		if (!bFound)
			m_arrEdited.Add(CPoint(plvItem->iItem, plvItem->iSubItem));
    }

    *pResult = FALSE;
}

CListBox* CListCtrlEx::ListSubItem(int nItem, int nSubItem, CString strPickList)
{
	// The returned pointer should not be saved
	
    // Make sure that the item is visible
    if (!EnsureVisible (nItem, TRUE)) return NULL;
	
    // Make sure that nCol is valid
    CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
    int nColumnCount = pHeader->GetItemCount();
    if (nSubItem >= nColumnCount || GetColumnWidth (nSubItem) < 5)
		return NULL;
	
    // Get the column offset
    int Offset = 0;
    for (int iColumn = 0; iColumn < nSubItem; iColumn++)
		Offset += GetColumnWidth (iColumn);
	
    CRect Rect;
    GetItemRect (nItem, &Rect, LVIR_BOUNDS);
	
    // Now scroll if we need to expose the column
    CRect ClientRect;
    GetClientRect (&ClientRect);
    if (Offset + Rect.left < 0 || Offset + Rect.left > ClientRect.right)
    {
		CSize Size;
		if (Offset + Rect.left > 0)
			Size.cx = -(Offset - Rect.left);
		else
			Size.cx = Offset - Rect.left;
		Size.cy = 0;
		Scroll (Size);
		Rect.left -= Size.cx;
    }
	
    // Get nSubItem alignment
    LV_COLUMN lvCol;
    lvCol.mask = LVCF_FMT;
    GetColumn (nSubItem, &lvCol);
	
    DWORD dwStyle;
    if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
    else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
    else dwStyle = ES_CENTER;
	
    Rect.left += Offset+4;
    Rect.right = Rect.left + GetColumnWidth (nSubItem) - 3;
    if (Rect.right > ClientRect.right)
		Rect.right = ClientRect.right;

	int nOffset;
	//修正rect的值
	int nPos = -1;
	int nCount = 1;
	nPos = strPickList.Find(";", 0);
	while(nPos != -1)
	{
		nPos = strPickList.Find(";", nPos+1);
		nCount++;
	}

	nOffset = nCount*10;
	
	Rect.bottom += nOffset;
	if (Rect.bottom > ClientRect.bottom)
	{
		Rect.top -= nOffset;
		if (Rect.top > 0)
		{
			Rect.bottom -= nOffset;
		}
		else
		{
			Rect.top = ClientRect.top + 20;
			Rect.bottom = Rect.top + nOffset;
		}
	}

    dwStyle |= WS_BORDER | WS_CHILD | WS_VSCROLL | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL;

	//保存原来的字符
	m_strOldText = GetItemText(nItem, nSubItem);
    CListCellListSel *pList = new CListCellListSel(nItem, nSubItem, strPickList);
    pList->Create(dwStyle, Rect, this, IDB_EDITCELL);

	return NULL;
}

void CListCtrlEx::SetListType(int nListType)
{
	//设置使用的场合
	m_nListType = nListType;
}

void CListCtrlEx::ClearEdited()
{
	CPoint pt;
	CString strText;
	int nIndex;
	//去除所有的*号
	for (int i = 0; i < m_arrEdited.GetSize(); i ++)
	{
		pt = m_arrEdited.GetAt(i);
		strText = GetItemText(pt.x, pt.y);

		nIndex = strText.Find("*", 0);
		if (nIndex != -1)
		{
			strText = strText.Left(nIndex);
			SetItemText(pt.x, pt.y, strText);
		}
	}

	//清空列表
	m_arrEdited.RemoveAll();

	//重绘
	Invalidate();
}

int CListCtrlEx::SetColumnHide(int nCol, BOOL bHide)
{
	int nColumnCount = m_hideHeader.GetItemCount();
	if(nCol<0 || nCol>=nColumnCount) return -1;
	int nHideReturn = m_hideHeader.m_aColum[nCol].bHide;
	m_hideHeader.m_aColum[nCol].bHide = bHide;
	if(bHide){		//是隐藏
		//if(m_ctlHeader.m_aColum[nCol].bHide) return nHideReturn;
	//	m_hideHeader.m_aColum[nCol].nItemWidth = GetColumnWidth(nCol);
		SetColumnWidth(nCol, 0);
	} else {		//是显示
		SetColumnWidth(nCol, m_hideHeader.m_aColum[nCol].nItemWidth);
	}
	return nHideReturn;
}

BOOL CListCtrlEx::GetColumnIsHide(int nCol)
{
	int nColumnCount = m_hideHeader.GetItemCount();
	if(nCol<0 || nCol>=nColumnCount) 
		return FALSE;
	else
		return m_hideHeader.m_aColum[nCol].bHide;
}

int CListCtrlEx::InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int nFormat/* = LVCFMT_LEFT*/, int nWidth/* = -1*/, int nSubItem/* = -1*/ )
{
	return InsertColumn(nCol,lpszColumnHeading, FALSE, nFormat, nWidth, nSubItem);
}

int CListCtrlEx::InsertColumn(int nCol, const LVCOLUMN *pColumn)
{
	return InsertColumn(nCol, pColumn, FALSE);
}

int CListCtrlEx::InsertColumn( int nCol, LPCTSTR lpszColumnHeading, BOOL bHide, int nFormat, int nWidth, int nSubItem )
{
	int nReturn = CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
	if(nReturn>=0){
		SColumn sColumn;
		//sColumn.bHide = bHide;
		sColumn.nItemWidth = nWidth;
		sColumn.sName = lpszColumnHeading;
		m_hideHeader.m_aColum.InsertAt(nReturn, sColumn);
		if(bHide){			//将该列宽度设置为 0
			SetColumnHide(nCol, TRUE);
		}
		m_hideHeader.m_aColum[nCol].bHide = bHide;
		
	}
	return nReturn;
}

int CListCtrlEx::InsertColumn(int nCol, const LVCOLUMN *pColumn, BOOL bHide)
{
	int nReturn = CListCtrl::InsertColumn(nCol, pColumn);
	if(nReturn>=0){
		SColumn sColumn;
		sColumn.bHide = bHide;
		sColumn.nItemWidth = pColumn->cx;
		sColumn.sName = pColumn->pszText;
		m_hideHeader.m_aColum.InsertAt(nReturn, sColumn);
		if(bHide){			//将该列宽度设置为 0
			SetColumnHide(nCol, TRUE);
		}
		m_hideHeader.m_aColum[nCol].bHide = bHide;
	}
	return nReturn;
}
	
BOOL CListCtrlEx::DeleteColumn( int nCol )
{
	BOOL bReturn = CListCtrl::DeleteColumn(nCol);
	if(bReturn){
		m_hideHeader.m_aColum.RemoveAt(nCol, 1);
	}
	return bReturn;
}

void CListCtrlEx::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT( GetStyle() & LVS_REPORT );
	
	CListCtrl::PreSubclassWindow();
	VERIFY( m_hideHeader.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() ) );
}

void CListCtrlEx::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CPoint pt = point;
	ScreenToClient(&pt);

	CWnd* pChildWnd = ChildWindowFromPoint(pt);
	if(pChildWnd != NULL && pChildWnd->GetSafeHwnd() != GetSafeHwnd())
	{
		char szClass [50];

        // Verify that this window handle is indeed the header
        // control's by checking its classname.
        GetClassName(pChildWnd->GetSafeHwnd(), szClass, 50);
        if (!lstrcmp (szClass, "SysHeader32"))
        {
			TRACE("CListCtrlEx::OnContextMenu -- 你右击了列头\n");
			if(pMenu != NULL)
			{
				//先清理刚才的菜单
				int nCount = pMenu->GetMenuItemCount();
				for(int j = 0; j < nCount; j++)
				{
					pMenu->DeleteMenu(0, MF_BYPOSITION);
				}
				//加入题头还分隔
				pMenu->AppendMenu(MF_STRING | MF_DISABLED, ITEM_TITLE, StringFromID(IDS_SELECT_SHOW_COLUMN));
				pMenu->AppendMenu(MF_SEPARATOR, ITEM_SEPARATOR, "");
				//加入各个列名
				int nColCount = m_hideHeader.GetItemCount();
				for(int i = 0; i < nColCount; i++)
				{
					SColumn  sc = m_hideHeader.m_aColum.GetAt(i);
					if(sc.bHide)
					{//隐藏的
						pMenu->AppendMenu(MF_STRING | MF_UNCHECKED, ITEM_1 + i, sc.sName);
					}
					else
					{
						pMenu->AppendMenu(MF_STRING | MF_CHECKED, ITEM_1 + i, sc.sName);
					}
				}

				pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			}
			
		}
	}
	//转发给所有者窗口
	CWnd * pParent = GetParent();
	::SendMessage(pParent->GetSafeHwnd(), WM_CONTEXTMENU, (WPARAM)LISTCTRLEX_MARK, (LPARAM)&point);
}

void CListCtrlEx::OnCommandItem(UINT nID)
{
	if(nID < ITEM_1 || nID > ITEM_19)
		return;
	
	int nPos = nID - (int)ITEM_1; //列数,从0开始
	
	//改变列的显性
	SetColumnHide(nPos, !GetColumnIsHide(nPos));

	//转发给所有者窗口
	CWnd * pParent = GetParent();
	::SendMessage(pParent->GetSafeHwnd(), ITEM_CHANGEHIDE, (WPARAM)LISTCTRLEX_MARK, NULL);
}

BOOL CListCtrlEx::OnEraseBkgnd( CDC* pDC )
{
	return CListCtrl::OnEraseBkgnd(pDC);
	CRect dataRgnRect;
	CPoint dataRgnTopLeftPoint;
	CPoint dataRgnBottomRightPoint;
	GetItemPosition(0 , &dataRgnTopLeftPoint);
	GetItemPosition(GetItemCount() , &dataRgnBottomRightPoint);
	if(!GetHeaderCtrl()->GetItemRect(GetHeaderCtrl()->GetItemCount()-1, dataRgnRect))
		return CListCtrl::OnEraseBkgnd(pDC);
	dataRgnBottomRightPoint.x = dataRgnRect.right;

	dataRgnRect.SetRect((CPoint)(dataRgnTopLeftPoint- CPoint(2,2)), (CPoint)(dataRgnBottomRightPoint - CPoint(2,2)));
	ClientToScreen(dataRgnRect);
	
	//compute and erase the blank area. Using the Marco.
	ERASE_BKGND_BEGIN;
	ADD_NOERASE_RECT(dataRgnRect);
	ERASE_BKGND_END(pDC, GetBkColor());

	return false;	
}

BOOL CListCtrlEx::ExportExcel(CString strFullPath)
{
	CDatabase database;
	CString strDriver = "MICROSOFT EXCEL DRIVER (*.XLS)";
	CString strSQL,str;
	
	strSQL.Format("DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s",
		strDriver, strFullPath, strFullPath);
	
	CFileFind find;
	BOOL IsFind = FALSE;
	IsFind = find.FindFile(strFullPath, 0);
	
	try
	{
		if( database.OpenEx(strSQL,CDatabase::noOdbcDialog) )
		{
			CString strVM = "";
			if(!IsFind)
			{
				strSQL = "CREATE TABLE Log(";
				strVM = "INSERT INTO Log (";
				int nColCount = m_hideHeader.GetItemCount();
				BOOL bFirst = TRUE;
				for(int h = 0; h < nColCount; h++)
				{
					SColumn  sc = m_hideHeader.m_aColum.GetAt(h);
					CString str;
					if( !sc.bHide )
					{
						CString str;
						if( bFirst )
						{
							str.Format("'%s' TEXT", sc.sName);
							strVM += ( "'" + sc.sName + "'");
						}
						else
						{
							str.Format(", '%s' TEXT", sc.sName);
							strVM += ",";
							strVM += ( "'" + sc.sName + "'");
						}
						bFirst = FALSE;
						strSQL += str;
					}
				}
				strSQL += ")";
				strVM += ") VALUES (";
				database.ExecuteSQL(strSQL);
			}
			for(int i = 0; i < GetItemCount(); i++)
			{
				CString strV = strVM;
				int nColCount = m_hideHeader.GetItemCount();
				BOOL bFirst = TRUE;
				for(int h = 0; h < nColCount; h++)
				{
					SColumn  sc = m_hideHeader.m_aColum.GetAt(h);
					CString str;
					if( !sc.bHide )
					{
						CString str;
						if( bFirst )
						{
							strV += ( "'" + GetItemText(i, h) + "'");
						}
						else
						{
							strV += ",";
							strV += ( "'" + GetItemText(i, h) + "'");
						}
						bFirst = FALSE;
						strSQL += str;
					}
				}
				strV += ")";
				database.ExecuteSQL(strV);
			}
		}
		else
			return FALSE;
		database.Close();
	}
	catch(CDBException * e)
	{
		TRACE("数据库错误: " + e->m_strError);
		return FALSE;
	}
	return TRUE;
}
