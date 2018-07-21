// ListCellEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ListCellEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <math.h>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CListCellEdit

CListCellEdit::	CListCellEdit(int nItem, int nSubItem, CString strInitText)
:m_bEscape (FALSE)
{
    m_nItem = nItem;
    m_nSubItem = nSubItem;
    m_strInitText = strInitText;
	m_dMinValue = -1;
	m_dMaxValue = -1;
	m_bModifyed = FALSE;
	m_fStepValue = -1;
	m_nTextLength = -1;
	m_bIntAsFloat = false;
}

CListCellEdit::	CListCellEdit(int nItem, int nSubItem, CString strInitText, double dMin, double dMax, int type)
:m_bEscape (FALSE)
{
    m_nItem = nItem;
    m_nSubItem = nSubItem;
    m_strInitText = strInitText;
	m_dMinValue = dMin;
	m_dMaxValue = dMax;
	m_nType = type;
	m_bModifyed = FALSE;
	m_fStepValue = -1;
	m_nTextLength = -1;
	m_bIntAsFloat = false;
}

CListCellEdit::	CListCellEdit(int nItem, int nSubItem, CString strInitText, double dMin, double dMax, int type, int nDataType,float fStepValue, int nTextLength,  int nJudge, bool intasfloat)
:m_bEscape (FALSE)
{
    m_nItem = nItem;
    m_nSubItem = nSubItem;
    m_strInitText = strInitText;
	m_dMinValue = dMin;
	m_dMaxValue = dMax;
	m_nType = type;
	m_bModifyed = FALSE;
	m_nDataType = nDataType;
	m_nJudge = nJudge;
	m_fStepValue = fStepValue;
	m_nTextLength = nTextLength;
	m_bIntAsFloat = intasfloat;
}

CListCellEdit::~CListCellEdit()
{
}


BEGIN_MESSAGE_MAP(CListCellEdit, CEdit)
	//{{AFX_MSG_MAP(CListCellEdit)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCellEdit message handlers

void CListCellEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	//SetListItemText();
	if(GetCapture() == NULL)
		DestroyWindow();
}

void CListCellEdit::OnNcDestroy() 
{
	CEdit::OnNcDestroy();
	
	// TODO: Add your message handler code here
    delete this;
}

BOOL CListCellEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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
	
	m_tool.RelayEvent(pMsg);
	return CEdit::PreTranslateMessage(pMsg);
}

void CListCellEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if( nChar == VK_ESCAPE )
			m_bEscape = TRUE;
		GetParent()->SetFocus();

		return;
	}

	// Resize edit control if needed
	// Get text extent
	CString str;

	GetWindowText( str );
	CWindowDC dc(this);
	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = dc.SelectObject( pFont );
	CSize size = dc.GetTextExtent( str );
	dc.SelectObject( pFontDC );
	size.cx += 5;			   	// add some extra buffer

	// Get client rect
	CRect rect, parentrect;
	GetClientRect( &rect );
	GetParent()->GetClientRect( &parentrect );

	// Transform rect to parent coordinates
	ClientToScreen( &rect );
	GetParent()->ScreenToClient( &rect );

	// Check whether control needs to be resized
	// and whether there is space to grow
	if( size.cx > rect.Width() )
	{
		if( size.cx + rect.left < parentrect.right )
			rect.right = rect.left + size.cx;
		else
			rect.right = parentrect.right;
		MoveWindow( &rect );
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

int CListCellEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Set the proper font
	CFont* font = GetParent()->GetFont();
	SetFont(font);

	SetWindowText( m_strInitText );
	SetFocus();
	SetSel( 0, -1 );
	SetCapture();
	
	EnableToolTips(TRUE);
    m_tool.Create(this);
	m_tool.AddTool(this,"");
	m_tool.SetDelayTime(1000);
	m_tool.Activate(true);

	return 0;
}

void CListCellEdit::SetListItemText()
{
    CString Text;
    GetWindowText (Text);
	m_bModifyed = TRUE;

    // Send Notification to parent of ListView ctrl
    LV_DISPINFO dispinfo;
    dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
    dispinfo.hdr.idFrom = GetDlgCtrlID();
    dispinfo.hdr.code = LVN_ENDLABELEDIT;

    dispinfo.item.mask = LVIF_TEXT;
    dispinfo.item.iItem = m_nItem;
    dispinfo.item.iSubItem = m_nSubItem;
    dispinfo.item.pszText = m_bEscape ? NULL : LPTSTR ((LPCTSTR) Text);
    dispinfo.item.cchTextMax = Text.GetLength();
	
    GetParent()->GetParent()->SendMessage (WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM) &dispinfo);
}

void CListCellEdit::OnLButtonUp( UINT nFlags, CPoint point )
{
	CEdit::OnLButtonUp(nFlags, point);
	SetCapture();
	m_tool.UpdateTipText("双击退出编辑状态", this);
}

void CListCellEdit::OnLButtonDblClk( UINT nFlags, CPoint point )
{
//	CEdit::OnLButtonDblClk(nFlags, point);
	CString Text;
    GetWindowText (Text);
	//判断是否有效输入
	if (Text.IsEmpty())
	{
		AfxMessageBox("修改的值不能为空");
		DestroyWindow();
		return;
	}

	if(!IsVaildInput(Text, m_nDataType))
	{
		AfxMessageBox("输入了非法字符,请重新输入!");
		DestroyWindow();
		return;
	}

	if(m_nJudge == 1)
	{
		if(!IsVaildStep(Text))
		{
			AfxMessageBox("修改的值不符合步长限制,请重新输入!");
			DestroyWindow();
			return;
		}
	}

	if(m_nDataType == 2 || m_nDataType == 4)
	{
		if(m_nTextLength > 0)
		{
			if(Text.GetLength() != m_nTextLength)
			{
				CString str;
				str.Format("输入的字符位数错误，请输入%d个字符!", m_nTextLength);
				AfxMessageBox(str);
				DestroyWindow();
				return;
			}
		}
	}

	double source = atof(Text);
	if(m_nDataType <2 && m_nJudge == 1)
	{
		if (source > m_dMaxValue && m_dMaxValue != 0)
		{
			AfxMessageBox("修改的值大于值上限!");
			DestroyWindow();
			return;
		}
		else if (source < m_dMinValue)
		{
			AfxMessageBox("修改的值小于值下限!");
			DestroyWindow();
			return;
		}
	}

	if (m_nType == 2)
	{
		int nValue = atoi(Text);
		if (nValue != 31)
		{
			AfxMessageBox("仅支持将在线定值区(区号指定为31)投入运行,禁止其他切区操作!");
			return;
		}
	}
	SetListItemText();
	
	DestroyWindow();
//	ReleaseCapture();
}

BOOL CListCellEdit::IsVaildInput(CString sSource, int nDataType)
{
	char ch0 = '0';
	char ch9 = '9';
	char chDel = '.';
	char chMinus = '-';
	if(nDataType == 3)//字符串
		return TRUE;
	else if(nDataType == 0)//浮点型
	{
		for(int i = 0; i < sSource.GetLength(); i++)
		{
			char a = sSource.GetAt(i);
			if(a < ch0 || a > ch9)
			{
				if( (i== 0) && (a == chMinus))
					continue;
				if(a != chDel)
					return FALSE;
			}
		}
	}
	else if(nDataType == 1)//整型
	{
		for(int i = 0; i < sSource.GetLength(); i++)
		{
			char a = sSource.GetAt(i);
			if(a < ch0 || a >ch9)
			{
				if( (i== 0) && (a == chMinus))
					continue;
				if( m_bIntAsFloat && a == chDel)
				{
					continue;			
				}
				
				return FALSE;
			}
		}
	}
	else if(nDataType == 2)//十六进制控制字
	{
		for(int i = 0; i < sSource.GetLength(); i++)
		{
			char a = sSource.GetAt(i);
			if(!((a >= ch0 && a <=ch9) || (a>='A' && a <= 'F')))
			{
				return FALSE;
			}
		}
	}
	else if(nDataType == 4)//二进制控制字
	{
		for(int i = 0; i < sSource.GetLength(); i++)
		{
			char a = sSource.GetAt(i);
			if(a != '0' && a != '1')
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

long getLong(double d )
{
	long ceilV = ceil(d);
	long floorV = floor(d);

	if( fabs(d-ceilV) < fabs(d-floorV) )
		return ceilV;
	return floorV;
}

/****************************************************
Date:2012/5/23  Author:LYH
函数名:   IsVaildStep	
返回值:   BOOL	
功能概要: 
参数: CString sSource	
*****************************************************/
BOOL CListCellEdit::IsVaildStep( CString sSource )
{
	if(m_nDataType == 1 )//整型
	{
		if( m_bIntAsFloat )
			return TRUE;
		int nDif = abs(atoi(sSource) - atoi(m_strInitText));
		if(m_fStepValue <= 0)
			return TRUE;
		int nStep = m_fStepValue;
		if(nDif%nStep != 0)
			return FALSE;
	}
	else if(m_nDataType == 0)//浮点
	{
		if(m_fStepValue <= 0.00001)
			return TRUE;

		int precision = 0;
		CString strStep;
		strStep.Format("%f", m_fStepValue);
		while( strStep.Find('.', 0) >= 0 )
		{
			if( strStep.GetAt(strStep.GetLength()-1) == '0')
				strStep = strStep.Left(strStep.GetLength()-1);
			else
				break;
		}

		int pos = strStep.Find('.', 0);
		CString PrecisonDecimal="";
		if( pos >= 0 && pos != strStep.GetLength() -1 )
		{
			PrecisonDecimal = strStep.Mid(pos+1);
			if( !PrecisonDecimal.IsEmpty() )
			precision = PrecisonDecimal.GetLength();
		}

		CString slog;

		double fDif = fabs((atof(sSource) - atof(m_strInitText)));
		double fZoomDif = fDif * ( pow(10, precision) );
		long nZoomDif = getLong(fZoomDif);

		double fZoomStep = m_fStepValue* (pow(10, precision));
		long nZoomStep = getLong(fZoomStep);

		slog.Format("定值步长: %s, 小数部分:%s, 精度:%d",
			strStep, PrecisonDecimal, precision);
		WriteLog(slog);
		slog.Format("定值步长判断: 原值:%s, 输入值:%s, 差值:%f, 放大差值:%f, 放大差值整数化:%d, 放大步长:%f, 放大步长整数化:%d", 
			m_strInitText, sSource, fDif, fZoomDif, nZoomDif, fZoomStep, nZoomStep);
			WriteLog(slog);

		if( fabs(fZoomDif - nZoomDif) > 0.0001 )
		{
			
			slog.Format("定值步长判断: 差值放大后存在小数部分:%f, 不符合步长限制", 
				fZoomDif - nZoomDif);
			WriteLog(slog);
			return FALSE;
		}

		if( 0 == nZoomStep)
			return TRUE;
		
		if( nZoomDif%nZoomStep == 0 )
			return TRUE;

		slog.Format("定值步长判断: 放大差值整数化:%d, 放大步长:%d, 不能整除, 不符合步长限制", 
			nZoomDif, nZoomStep);
			WriteLog(slog);
		
		return FALSE;
	}
	return TRUE;
}
