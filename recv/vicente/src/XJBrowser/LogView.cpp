// ***************************************************************
//  LogView   version:  1.0    date: 2:9:2008     auther:LYH
//  ---------------------------------------------------------------------------------------
//  description: ��־�����ʵ��
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
	m_nMaxLineCount=200;	//�������ȱʡ200��
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

//�����������
//##ModelId=49B87B9901D7
void CLogView::SetMaxLineCount(int nValue)
{
	if(nValue>0)
		m_nMaxLineCount=nValue;
}


/////////////////////////////////////////////////////////////////////////////
// CLogView message handlers
 
//�������������ַ����������ʱ����
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
	//��cursor�Ƶ�ĩβ
	int len=GetWindowTextLength();
	pEdit->SetSel(len,len,TRUE);
	//�����־�ı�
	pEdit ->ReplaceSel(strOut); //modify by liuyuhua 20080903
//	CTime tm=CTime::GetCurrentTime();
// 	pEdit->ReplaceSel(tm.Format("  [%Y��%m��%d�� %Hʱ%M��%S��] ") + strOut+ "\r\n" );
	//ɾ��ǰ���У���֤������m_nMaxLineCount��
	int nDel=pEdit->GetLineCount()-m_nMaxLineCount;
	if(nDel>0)//������ָ����
	{
		int nLength=0;
		for(int i=0; i<nDel; i++)//����ǰ������е��ַ�����
		{
			nLength	+= pEdit->LineLength(i);
		}
		pEdit->SetSel(0, nLength+2,TRUE);//����ѡ�в����
		pEdit->Clear();
		//��cursor�Ƶ�ĩβ
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
	//���أ���ֹCEdit���� SetmodifyFlag()���³���
	
}

//##ModelId=49B87B9901DA
void CLogView::DoCopy()
{
	GetEditCtrl().Copy();	
}

//##ModelId=49B87B9901D9
void CLogView::DoClear()
{
//���Edit���ݲ��ͷŻ���
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
//�жϲ˵���Ч��
//	CEdit* pCtrl=&GetEditCtrl();
//�����˵�
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
//���μ����¼�����ֹ�û�����
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
			logbrush.lbColor = g_TreeBkColor;//////////����ǹؼ�,ѡ����Ҫ�ı���ɫ 
			HBRUSH   m_hBrush=(HBRUSH)::CreateBrushIndirect(&logbrush);//ʹ�����API 
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
			logbrush.lbColor = g_ListSpaceColor3;//////////����ǹؼ�,ѡ����Ҫ�ı���ɫ 
			HBRUSH   m_hBrush=(HBRUSH)::CreateBrushIndirect(&logbrush);//ʹ�����API 
			return   m_hBrush;
		}
		return CEditView::OnCtlColor(pDC, this, nCtlColor);
	}
	else
	{
		return NULL;
	}
}
