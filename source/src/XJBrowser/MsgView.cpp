// ***************************************************************
//  LogView   version:  1.0    date: 2:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: 消息输出类实现
//  ---------------------------------------------------------------------------------------
//  Copyright (C) 2008 - All Rights Reserved
// ***************************************************************
// update record:
// ***************************************************************
// MsgView.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "MsgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgView

IMPLEMENT_DYNCREATE(CMsgView, CEditView)

//##ModelId=49B87B9901C7
CMsgView::CMsgView()
{
	m_nMaxLineCount=200;	//最大行数缺省200行
}
 
//##ModelId=49B87B9901C8
CMsgView::~CMsgView()
{
	KillTimer(m_nTimer);
}


BEGIN_MESSAGE_MAP(CMsgView, CEditView)
	//{{AFX_MSG_MAP(CMsgView)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_LOG_CLEARALL, OnLogClearAll)
	ON_COMMAND(ID_LOG_SELECTALL, OnLogSelectall)
	ON_WM_CHAR()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMsgView diagnostics

#ifdef _DEBUG
//##ModelId=49B87B9901DD
void CMsgView::AssertValid() const
{
	CEditView::AssertValid();
}

//##ModelId=49B87B9901E4
void CMsgView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

//设置最大行数
//##ModelId=49B87B9901D7
void CMsgView::SetMaxLineCount(int nValue)
{
	if(nValue>0)
		m_nMaxLineCount=nValue;
}


/////////////////////////////////////////////////////////////////////////////
// CMsgView message handlers

void CMsgView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();

	m_nTimer = SetTimer(501, 6*1000, 0);
}
 
//向输出窗口添加字符串，并添加时间标记
//##ModelId=49B87B9901DB
void CMsgView::AppendString(const CString& strOut)
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
BOOL CMsgView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CEditView::PreCreateWindow(cs))
		return FALSE;
//	cs.dwExStyle |= 
//	cs.style |= ES_READONLY; //ES_MULTILINE 
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping
	
	return TRUE;
}

//##ModelId=49B87B9901E7
void CMsgView::OnChange() 
{
	//拦截，防止CEdit调用 SetmodifyFlag()导致出错
	
}

//##ModelId=49B87B9901DA
void CMsgView::DoCopy()
{
	GetEditCtrl().Copy();	
}

//##ModelId=49B87B9901D9
void CMsgView::DoClear()
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
void CMsgView::OnContextMenu(CWnd* pWnd, CPoint point) 
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
void CMsgView::OnEditCopy() 
{
	DoCopy();	
}

//##ModelId=49B87B9901EF
void CMsgView::OnLogClearAll() 
{
	DoClear();	
}

//##ModelId=49B87B9901F1
void CMsgView::OnLogSelectall() 
{
	GetEditCtrl().SetSel(0,-1,TRUE);	
}

//##ModelId=49B87B9901F5
void CMsgView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//屏蔽键盘事件，防止用户输入
//	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

HBRUSH CMsgView::CtlColor(CDC* pDC, UINT nCtlColor) 
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

void CMsgView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
	
	if(nIDEvent == m_nTimer)
	{
		//关闭定时器
		//KillTimer(m_nTimer);

		PT_ZONE zone;
		CString sRecords;
		int nCurState = pApp->GetPTSetModState(zone, sRecords);
		if (nCurState < 0)
			return;

		if (zone.PT_ID.IsEmpty())
			return;
		
		CSecObj* pObj = (CSecObj*)pApp->GetDataEngine()->FindDevice(zone.PT_ID, TYPE_SEC);
		if (NULL == pObj)
			return;

		CString c_dz_mod_state[] = {
			StringFromID(IDS_DZ_MOD_STATE_0),
				StringFromID(IDS_DZ_MOD_STATE_1),
				StringFromID(IDS_DZ_MOD_STATE_2),
				StringFromID(IDS_DZ_MOD_STATE_3),
				StringFromID(IDS_DZ_MOD_STATE_4),
				StringFromID(IDS_DZ_MOD_STATE_5)
		};
		
		//AfxMessageBox(strCNT);
		DoClear();
		vector<CString> v = pApp->SplitCString(sRecords, ';');
		for (string::size_type i = 0; i < v.size(); i++){
			CString strState;
			if (i+1 <= 5){
				strState = c_dz_mod_state[i+1];
				str.Format("[%d/6]: %s[%s][%s]\r\n", i+1, v[i], pObj->m_sName, strState);
				
				AppendString(str);
			}else{
				strState = "";
				str.Format("%s[%s]\r\n", v[i], pObj->m_sName);
			}
		}

		
		//m_nTimer = SetTimer(501, 6*1000, 0);
	}
}
