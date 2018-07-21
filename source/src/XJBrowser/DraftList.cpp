// DraftList.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DraftList.h"
#include "DraftDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDraftList

IMPLEMENT_DYNCREATE(CDraftList, CListView)

CDraftList::CDraftList()
{
	m_nSelected = 0;
}

CDraftList::~CDraftList()
{
}


BEGIN_MESSAGE_MAP(CDraftList, CListView)
	//{{AFX_MSG_MAP(CDraftList)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDraftList drawing

void CDraftList::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDraftList diagnostics

#ifdef _DEBUG
void CDraftList::AssertValid() const
{
	CListView::AssertValid();
}

void CDraftList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDraftList message handlers

void CDraftList::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CListCtrl& m_list = GetListCtrl();
	LONG lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//获取当前窗口风格 
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位 
	lStyle |= LVS_LIST; //设置列表风格 
	lStyle |= LVS_SHOWSELALWAYS;
	lStyle |= LVS_SINGLESEL;
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle); //设置窗口风格 
	DWORD dwStyle = m_list.GetExtendedStyle(); 

	CDraftDocument* pDoc = (CDraftDocument*)GetDocument();
	if(pDoc)
	{
		for( int i = 0; i < pDoc->m_arrFiles.size(); i++ )
		{
			m_list.InsertItem( i, pDoc->m_arrFiles[i].m_sTitle );
		}
		if( m_list.GetItemCount() > 0)
			m_list.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
}

void CDraftList::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if((pNMListView->uChanged   &   LVIF_STATE)&&
		(pNMListView->uNewState   &   LVIS_SELECTED)) 
	{
		//事实上鼠标点击的选择动作, 会产生三次ITEMCHANGED消息, 
		//1是A的文字背景消失触发，2是A取消选中触发，3是B被选中触发
		//为避免刷新太频繁, 这里只处理被选中消息, 这样处理会漏掉选择了空行的消息
		//所以把选择空行放在NM_CLICK中处理, 对于移动键盘上下键导致的选择到空行不做处理
		
		int nSelected = pNMListView->iItem;//如果点击的是空白区，这个值应该是-1；
		if(nSelected < 0)
			return;

		m_nSelected = nSelected;
		
		CDraftDocument* pDoc = (CDraftDocument*)GetDocument();
		pDoc->SetSelected(nSelected);
		pDoc->UpdateAllViews(0, 1010, 0 );
	}
	
	*pResult = 0;
}

void CDraftList::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	if(pNMHDR ->code == NM_CUSTOMDRAW)
	{
		LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
		int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //行索引
		//TRACE1("CViewAction::OnCustomDraw  %d\n", nItem);
		switch(lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			{
				*pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
			//	lplvcd->clrTextBk = g_ListSpaceColor2;
				break;
			}
		case CDDS_ITEMPREPAINT:
			{
				*pResult = CDRF_NOTIFYSUBITEMDRAW;
			//	lplvcd->clrTextBk = g_ListSpaceColor2;
				break;
			}
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM :
			{
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec); //行索引
	
				//先给默认值, 后面再特殊处理
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);
				
				if(nItem  != -1 && nItem == m_nSelected)
				{
					lplvcd ->clrText = RGB(255, 0, 0);
				}
		
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}
