/* Generated by Together */

#include "stdafx.h"
#include "EProperty.h"
#include "EPropList.h"
#include "EPropView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(EPropView, CView)

BEGIN_MESSAGE_MAP(EPropView, CView)
	//{{AFX_MSG_MAP(CPropListView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

EPropView::EPropView()
{
}

EPropView::~EPropView()
{
}

void EPropView::OnDraw(CDC* pDC)
{
}

int EPropView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect;
	GetClientRect(&rect);

	m_PropCtrl.Create( this , -1, rect , this );

	return 0;
}

void EPropView::GetIDRange( int& nID_PROPERTY_RANGE_BEG , int& nID_PROPERTY_RANGE_END )
{
	nID_PROPERTY_RANGE_BEG = 600;
	nID_PROPERTY_RANGE_END = 700;
}

BOOL EPropView::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void EPropView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(&rect);

	m_PropCtrl.MoveWindow(&rect);
}

void EPropView::SetPropertyHost( IPropertyHost* pHost )
{
	m_PropCtrl.SetPropertyHost(pHost);
}

void EPropView::RefreshHostList()
{
	m_PropCtrl.RefreshHostList();
}

//
// IPropertyChangeListener
//
// This is called when user has edited a property, and someone needs
// to be notified on the change (eg other views). This default
// implementation updates all views.
//
//��ʹ���ĵ�����и��£������ֶ����²���
void EPropView::PropertyChanged(EProperty* pChanged)
{
//	GetDocument()->UpdateAllViews(this,0,NULL);
}