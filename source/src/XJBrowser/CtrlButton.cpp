// CtrlButton.cpp

#include "stdafx.h"
//#include "STDraw.h"
#include "CtrlButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//CFontButton

BEGIN_MESSAGE_MAP(CFontButton, CButton)
	//{{AFX_MSG_MAP(CFontButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//##ModelId=49B87BA90197
CFontButton::CFontButton()
{
	m_BKmode=TRANSPARENT;
	m_BKColor=RGB(255,255,255);
	m_FontColor=RGB(0,0,0);

	//��ʼ��m_logFont, zyz
 	m_logFont.lfHeight=-12;
	m_logFont.lfWidth=0;
	m_logFont.lfEscapement=0;
	m_logFont.lfOrientation=0;
	m_logFont.lfWeight=FW_NORMAL;
	m_logFont.lfItalic=0;
	m_logFont.lfUnderline=0;
	m_logFont.lfStrikeOut=0;
	m_logFont.lfCharSet=GB2312_CHARSET;
	m_logFont.lfOutPrecision=OUT_DEFAULT_PRECIS;
	m_logFont.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	m_logFont.lfQuality=PROOF_QUALITY;
	m_logFont.lfPitchAndFamily=VARIABLE_PITCH | FF_ROMAN;
	strcpy(m_logFont.lfFaceName, "����");
/*
	//strcpy(m_logFont.lfFaceName, _T("����"));
	const TCHAR fntName[20]=_T("Times New Roman"); //_T("Times New Roman");  
	memset((void*)&m_logFont,0,sizeof(LOGFONT));
	memcpy((void*)&m_logFont.lfFaceName,(void*)fntName,sizeof(fntName));
	
	m_logFont.lfWeight=FW_REGULAR;
	m_logFont.lfWidth=0;
	m_logFont.lfHeight= -12;
	m_logFont.lfItalic=FALSE;
	m_logFont.lfOrientation=0;
	m_logFont.lfEscapement=0;
	m_logFont.lfUnderline=FALSE;
	m_logFont.lfStrikeOut=FALSE;
	m_logFont.lfOutPrecision=OUT_OUTLINE_PRECIS;
	m_logFont.lfCharSet=GB2312_CHARSET;
	m_logFont.lfOutPrecision=OUT_DEFAULT_PRECIS; 
	m_logFont.lfPitchAndFamily=FF_MODERN|DEFAULT_PITCH;
*/
}

//##ModelId=49B87BA90198
void CFontButton::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
//����ȱʡ��(ժ��MSDN)
   UINT uStyle = DFCS_BUTTONPUSH;
   // This code only works with buttons.
   ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
   // If drawing selected, add the pushed style to DrawFrameControl.
   if (lpDrawItemStruct->itemState & ODS_SELECTED)
      uStyle |= DFCS_PUSHED;
   // Draw the button frame.
   ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, 
      DFC_BUTTON, uStyle);

//��û��ƻ���
	HDC hdc =lpDrawItemStruct->hDC;
	CRect rc=lpDrawItemStruct->rcItem;
	rc.DeflateRect(4,4,4,4); //��Сһ�㣬��ռ�ñ߽�
//����hdc
	CFont ft;
	ft.CreateFontIndirect(&m_logFont);
	HFONT hOldFont=HFONT(::SelectObject(hdc,ft));
	//����Sample
	CString strSample(_T("��ǩ"));
	SetBkMode(hdc,m_BKmode);
	SetBkColor(hdc,m_BKColor);
	SetTextColor(hdc,m_FontColor);
	DrawText(hdc,strSample,strSample.GetLength(),rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
//�ָ��豸����
	ft.DeleteObject();
	SelectObject(hdc,hOldFont);
}

//##ModelId=49B87BA9019B
int CFontButton::ShowFontDialog()
{
	CFontDialog dlg(&m_logFont, CF_EFFECTS|CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT);
	dlg.m_cf.rgbColors=m_FontColor;
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetCurrentFont(&m_logFont);
		m_FontColor=dlg.GetColor();
		Invalidate();
		return IDOK;
	}
	else
		return IDCANCEL;
}


//CColorButton

BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//##ModelId=49B87BA901A8
CColorButton::CColorButton()
{
	m_Color=RGB(0,0,0);
}

//##ModelId=49B87BA901A9
void CColorButton::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	if(FALSE==IsWindowEnabled())	//���Disabled
	{
	   ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, 
	      DFC_BUTTON, DFCS_BUTTONPUSH);
		return;
	}
//����ȱʡ��(ժ��MSDN)
   UINT uStyle = DFCS_BUTTONPUSH;  // This code only works with buttons.
   ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
   if (lpDrawItemStruct->itemState & ODS_SELECTED)// If drawing selected, add the pushed style to DrawFrameControl.
      uStyle |= DFCS_PUSHED;
   // Draw the button frame.
   ::DrawFrameControl(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, 
      DFC_BUTTON, uStyle);

//��û��ƻ���
	HDC hdc =lpDrawItemStruct->hDC;
	CRect rc=lpDrawItemStruct->rcItem;
	rc.DeflateRect(4,4,4,4); //��Сһ�㣬��ռ�ñ߽�
	CBrush brush;
	LOGBRUSH logbrush;
	logbrush.lbColor=m_Color;
	logbrush.lbStyle=BS_SOLID;
//	logbrush.lbHatch=HS_HORIZONTAL;
	brush.CreateBrushIndirect(&logbrush);
	HBRUSH hOldBrush=(HBRUSH)SelectObject(hdc,brush);
	SetBkColor(hdc,m_Color);
	FillRect(hdc,&rc,(HBRUSH)brush);
//�ָ��豸����
	brush.DeleteObject();
	SelectObject(hdc,hOldBrush);
}

//##ModelId=49B87BA901B6
int CColorButton::ShowColorDialog()
{
	CColorDialog dlg(m_Color);
	if (dlg.DoModal() == IDOK)
	{
		m_Color=dlg.GetColor();
		Invalidate();
		return IDOK;
	}
	else
		return IDCANCEL;
}
