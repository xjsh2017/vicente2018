// ***************************************************************
//  LogView   version:  1.0    date: 2:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 日志输出类实现
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
// LogView.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "LogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogView

IMPLEMENT_DYNCREATE(CLogView, CListView)

//##ModelId=49B87B9901C7
CLogView::CLogView()
{
	m_nMaxLineCount=200;	//最大行数缺省200行
}
 
//##ModelId=49B87B9901C8
CLogView::~CLogView()
{

}


BEGIN_MESSAGE_MAP(CLogView, CEditView)
	//{{AFX_MSG_MAP(CLogView)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_LOG_CLEARALL, OnLogClearAll)
	ON_COMMAND(ID_LOG_SELECTALL, OnLogSelectall)
	ON_WM_CHAR()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLogView diagnostics

#ifdef _DEBUG
//##ModelId=49B87B9901DD
void CLogView::AssertValid() const
{
	CEditView::AssertValid();
}

//##ModelId=49B87B9901E4
void CLogView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

//设置最大行数
//##ModelId=49B87B9901D7
void CLogView::SetMaxLineCount(int nValue)
{
	if(nValue>0)
		m_nMaxLineCount=nValue;
}


/////////////////////////////////////////////////////////////////////////////
// CLogView message handlers
 
//向输出窗口添加字符串，并添加时间标记
//##ModelId=49B87B9901DB
void CLogView::AppendString(const CString& strOut)
{
	CEdit *pEdit=&GetEditCtrl();

	if(pEdit == NULL)
		return;
	if(pEdit->GetSafeHwnd() == NULL)
		return;
	if(!::IsWindow(pEdit->GetSafeHwnd()))
		return;
	//把cursor移到末尾
	int len=GetWindowTextLength();
	pEdit->SetSel(len,len,TRUE);
	//添加日志文本
	pEdit ->ReplaceSel(strOut); //modify by liuyuhua 20080903
//	CTime tm=CTime::GetCurrentTime();
// 	pEdit->ReplaceSel(tm.Format("  [%Y年%m月%d日 %H时%M分%S秒] ") + strOut+ "\r\n" );
	//删除前面行，保证不超过m_nMaxLineCount行
	int nDel=pEdit->GetLineCount()-m_nMaxLineCount;
	if(nDel>0)//超过了指定行
	{
		int nLength=0;
		for(int i=0; i<nDel; i++)//计算前面多余行的字符总数
		{
			nLength	+= pEdit->LineLength(i);
		}
		pEdit->SetSel(0, nLength+2,TRUE);//将其选中并清除
		pEdit->Clear();
		//把cursor移到末尾
		len=GetWindowTextLength();
		pEdit->SetSel(len,len);
	}
}

//##ModelId=49B87B9901CA
BOOL CLogView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;
//	cs.dwExStyle |= 
//	cs.style |= ES_READONLY; //ES_MULTILINE 
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping
	return TRUE;
}

//##ModelId=49B87B9901E7
void CLogView::OnChange() 
{
	//拦截，防止CEdit调用 SetmodifyFlag()导致出错
	
}

//##ModelId=49B87B9901DA
void CLogView::DoCopy()
{
	GetEditCtrl().Copy();	
}

//##ModelId=49B87B9901D9
void CLogView::DoClear()
{
//清空Edit内容并释放缓存
	CEdit* pEdit=&GetEditCtrl();
	if(pEdit == NULL)
		return;
	if(pEdit->GetSafeHwnd() == NULL)
		return;
	if(!::IsWindow(pEdit->GetSafeHwnd()))
		return;
	pEdit->SetRedraw(FALSE);
	pEdit->SetSel(0, -1,TRUE);
	pEdit->Clear();
	pEdit->SetRedraw(TRUE);

	// Clear the undo buffer.
	if (pEdit->CanUndo())
	{
	   pEdit->EmptyUndoBuffer();
	}
}

//##ModelId=49B87B9901E9
void CLogView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu Menu,*pMenu;
	Menu.LoadMenu(IDR_MENU_LOGVIEW);
	pMenu=Menu.GetSubMenu(0);
//判断菜单有效性
//	CEdit* pCtrl=&GetEditCtrl();
//弹出菜单
	pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, point.x, point.y, this);
	
}

//##ModelId=49B87B9901ED
void CLogView::OnEditCopy() 
{
	DoCopy();	
}

//##ModelId=49B87B9901EF
void CLogView::OnLogClearAll() 
{
	DoClear();	
}

//##ModelId=49B87B9901F1
void CLogView::OnLogSelectall() 
{
	GetEditCtrl().SetSel(0,-1,TRUE);	
}

//##ModelId=49B87B9901F5
void CLogView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//屏蔽键盘事件，防止用户输入
//	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

HBRUSH CLogView::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	if (g_style == 3)
	{
		// TODO: Return a non-NULL brush if the parent's handler should not be called
		//	HBRUSH   hbr   =   CEditView::OnCtlColor(pDC, this, nCtlColor); 
		LOGBRUSH   logbrush; 
		//COLORREF     bkcolor(RGB(255,255,0)); 
		//COLORREF     textcolor(RGB(0,0,255)); 
		logbrush.lbHatch   =   0; 
		logbrush.lbStyle   =   BS_SOLID; 
		switch(nCtlColor) 
		{   
		case   CTLCOLOR_EDIT: 
		case   CTLCOLOR_BTN: 
		case   CTLCOLOR_STATIC:
			pDC-> SetTextColor(g_TreeTextColor); 
			pDC-> SetBkMode(TRANSPARENT); 
			pDC-> SetBkColor(g_TreeBkColor); 
			logbrush.lbColor = g_TreeBkColor;//////////这句是关键,选中你要的背景色 
			HBRUSH   m_hBrush=(HBRUSH)::CreateBrushIndirect(&logbrush);//使用这个API 
			return   m_hBrush;
		}
		return CEditView::OnCtlColor(pDC, this, nCtlColor);
	}
	else if (g_style == 2)
	{
		// TODO: Return a non-NULL brush if the parent's handler should not be called
		//	HBRUSH   hbr   =   CEditView::OnCtlColor(pDC, this, nCtlColor); 
		LOGBRUSH   logbrush; 
		//COLORREF     bkcolor(RGB(255,255,0)); 
		//COLORREF     textcolor(RGB(0,0,255)); 
		logbrush.lbHatch   =   0; 
		logbrush.lbStyle   =   BS_SOLID; 
		switch(nCtlColor) 
		{   
		case   CTLCOLOR_EDIT: 
		case   CTLCOLOR_BTN: 
		case   CTLCOLOR_STATIC:
			pDC-> SetTextColor(RGB(0,0,0)); 
			pDC-> SetBkMode(TRANSPARENT); 
			pDC-> SetBkColor(g_ListSpaceColor3); 
			logbrush.lbColor = g_ListSpaceColor3;//////////这句是关键,选中你要的背景色 
			HBRUSH   m_hBrush=(HBRUSH)::CreateBrushIndirect(&logbrush);//使用这个API 
			return   m_hBrush;
		}
		return CEditView::OnCtlColor(pDC, this, nCtlColor);
	}
	else
	{
		return NULL;
	}
}
