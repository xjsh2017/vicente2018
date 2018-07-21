// AboutDlgEx.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "AboutDlgEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlgEx dialog


//##ModelId=49B87BB50281
CAboutDlgEx::CAboutDlgEx(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlgEx::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlgEx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_TextVersion		= StringFromID(IDS_ABOUT_VERINFO);

	m_pFontVersionSub		= NULL;
	m_pFontVersionMain		= NULL;
	m_pFontHeaderSub		= NULL;
	m_pFontHeaderMain		= NULL;
	m_IdBitmap				= IDB_BMP_START;
	
	if (g_style)
	{
		m_IdIcon                = IDB_BMP_ABOUT;
	}
	else
	{
		m_IdIcon				= IDB_BMP_SUNTECH;
	}


	SetDefaultsFonts();
	SetDefaultsPositions();
	SetDefaultsText();
}


//##ModelId=49B87BB50380
void CAboutDlgEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlgEx)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlgEx, CDialog)
	//{{AFX_MSG_MAP(CAboutDlgEx)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlgEx message handlers

//##ModelId=49B87BB503D9
BOOL CAboutDlgEx::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//载入背景位图
	if ( m_bitmap.LoadBitmap( m_IdBitmap ) == 0 )
		ASSERT( -1 );

	//求得位图信息
	m_bitmap.GetBitmap( &m_bitmapinfo );

	//改变对话框大小
	MoveWindow( 0, 0, m_bitmapinfo.bmWidth, m_bitmapinfo.bmHeight, FALSE );

	//中央显示
	CenterWindow();

	//生成“关闭”按钮
	m_bnOK.Create( StringFromID(IDS_BTN_CLOSE), 
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				CRect( m_bitmapinfo.bmWidth - 100, m_bitmapinfo.bmHeight - 40, m_bitmapinfo.bmWidth - 25, m_bitmapinfo.bmHeight - 20 ),
				this,
				IDOK
				);
	
	//初始化内存DC
	m_dcMem.CreateCompatibleDC( GetDC() );
	m_dcMem.SelectObject( &m_bitmap );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//##ModelId=49B87BB503DB
void CAboutDlgEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	OnDraw( &m_dcMem );
	dc.BitBlt( 0, 0, m_bitmapinfo.bmWidth, m_bitmapinfo.bmHeight, &m_dcMem, 0, 0, SRCCOPY );
	
	// Do not call CDialog::OnPaint() for painting messages
}

//##ModelId=49B87BB50280
void CAboutDlgEx::SetDefaultsPos()
{

}

//##ModelId=49B87BB50273
void CAboutDlgEx::SetDefaultsText()
{
	//取得版本信息
	const DWORD dwSizeOfBuffer = MAX_PATH * 10;
	TCHAR szModuleFileName[ dwSizeOfBuffer ];
	GetModuleFileName( AfxGetInstanceHandle(), szModuleFileName, dwSizeOfBuffer );
	DWORD dwHandle = -1;
	DWORD dwSize = GetFileVersionInfoSize( szModuleFileName, &dwHandle );
	LPVOID lpData = new BYTE[dwSize];
	GetFileVersionInfo( szModuleFileName, dwHandle, dwSize, lpData );	
	UINT uiLen = dwSize;

	CString str;

	//取得项目简称
	LPVOID lpProject = NULL;
	str = "\\StringFileInfo\\"+StringFromID(IDS_LANG_CODE)+"\\Comments";
	VerQueryValue(
			lpData, 
			TEXT( str.GetBuffer(1) ), 
			&lpProject,
			&uiLen );
	CString vProject = LPCTSTR(lpProject);

	//取得项目全称
	LPVOID lpSpecialBuild = NULL;
	str = "\\StringFileInfo\\"+StringFromID(IDS_LANG_CODE)+"\\SpecialBuild";
	VerQueryValue(
			lpData, 
			TEXT( str.GetBuffer(1) ), 
			&lpSpecialBuild,
			&uiLen );
	CString vSpecialBuild = LPCTSTR(lpSpecialBuild);

	//取得部品名称
	LPVOID lpProductName = NULL;
	str = "\\StringFileInfo\\"+StringFromID(IDS_LANG_CODE)+"\\ProductName";
	VerQueryValue(
			lpData, 
			TEXT( str.GetBuffer(1) ), 
			&lpProductName,
			&uiLen );
	CString vProductName = LPCTSTR(lpProductName);

	//取得公司名
	LPVOID lpCompany = NULL;
	str = "\\StringFileInfo\\"+StringFromID(IDS_LANG_CODE)+"\\CompanyName";
	VerQueryValue(
			lpData, 
			TEXT( str.GetBuffer(1) ), 
			&lpCompany,
			&uiLen );
	CString vCompany = LPCTSTR(lpCompany);

	//取得授权年月
	LPVOID lpCopyright = NULL;
	str = "\\StringFileInfo\\"+StringFromID(IDS_LANG_CODE)+"\\LegalCopyright";
	VerQueryValue(
			lpData, 
			TEXT( str.GetBuffer(1) ), 
			&lpCopyright,
			&uiLen );
	CString vCopyright = LPCTSTR( lpCopyright );

	//取得版本号	
	LPVOID lpVersion = NULL;
	str = "\\StringFileInfo\\"+StringFromID(IDS_LANG_CODE)+"\\ProductVersion";
	VerQueryValue(
			lpData, 
			TEXT( str.GetBuffer(1) ), 
			&lpVersion,
			&uiLen );
	CString vMain = LPCTSTR( lpVersion );

	//取得开发代号	
	LPVOID lpInternalName = NULL;
	str = "\\StringFileInfo\\"+StringFromID(IDS_LANG_CODE)+"\\InternalName";
	VerQueryValue(
			lpData,
			TEXT( str.GetBuffer(1) ),
			&lpInternalName,
			&uiLen );
	CString vInternalName = LPCTSTR( lpInternalName );

	//释放存放版本信息的结构
    delete [] lpData;

	//设置版本信息
	AddStringHeaderMain( vProject);
	AddStringHeaderSub( vSpecialBuild );
	AddStringHeaderSub( vProductName );
	AddStringCustom( vCompany );
	AddStringCustom( vCopyright );

	CString vSeri( "111-111" );
	SetTextCustomized( StringFromID(IDS_ABOUT_REGISTER_USER)+": "+StringFromID(IDS_ABOUT_COMPANY_NAME));
	AddStringVersion(StringFromID(IDS_ABOUT_REGISTER_USER));
	AddStringVersion(StringFromID(IDS_ABOUT_SERIALNUMBER));
	AddStringVersion(StringFromID(IDS_ABOUT_INTERNALNAME));
	AddStringVersionRight(vMain);
	AddStringVersionRight(vSeri);
	AddStringVersionRight(vInternalName);
}

//##ModelId=49B87BB50290
void CAboutDlgEx::SetDefaultsFonts()
{
	m_nHeightHeaderMain			= 48;
	m_nWeightHeaderMain			= FW_BOLD;
	m_bItalicHeaderMain			= TRUE;
	m_nOutPreHeaderMain			= OUT_TT_PRECIS;
	m_FacenameHeaderMain		= "Courier New";

	m_nHeightHeaderSub			= 26;
	m_nWeightHeaderSub			= FW_BOLD;
	m_bItalicHeaderSub			= TRUE;
	m_nOutPreHeaderSub			= OUT_TT_PRECIS;
	m_FacenameHeaderSub			= "MS Song";

	m_nHeightVersionMain		= 12;
	m_nWeightVersionMain		= FW_BOLD;
	m_bItalicVersionMain		= FALSE;
	m_nOutPreVersionMain		= 0;
	m_FacenameVersionMain		= "MS Sans Serif";

	m_nHeightVersionSub			= 8;
	m_nWeightVersionSub			= FW_NORMAL;
	m_bItalicVersionSub			= FALSE;
	m_nOutPreVersionSub			= 0;
	m_FacenameVersionSub		= "MS Sans Serif";
}

//##ModelId=49B87BB5029F
void CAboutDlgEx::SetDefaultsPositions()
{
	m_ptHeaderMain			= CPoint(  52,  76);
	m_ptHeaderSub			= CPoint(  52, 111);
	m_ptVersion				= CPoint(  52, 260);
	m_ptVersionsLeft		= CPoint( 121, 260);
	m_ptVersionsRight		= CPoint( 255, 260);
	m_ptCustomizedMain		= CPoint( 305, 260);
	m_ptCustomized			= CPoint( 305, 275);
	m_szDeltaHeaderMain		= CSize(    0,  30);
	m_szDeltaHeaderSub		= CSize(    0,  30);
	m_szDeltaVersionsLeft	= CSize(    0,  15);
	m_szDeltaVersionsRight	= CSize(    0,  15);
	m_szDeltaCustomized		= CSize(    0,  15);
}

//##ModelId=49B87BB502A2
void CAboutDlgEx::SetBitmapId(UINT idBitmap)			{ m_IdBitmap = idBitmap;	}
//##ModelId=49B87BB502AF
void CAboutDlgEx::SetIconId(UINT idIcon)				{ m_IdIcon	 = idIcon;		}

//##ModelId=49B87BB502B1
void CAboutDlgEx::SetFontHeaderMain(int nHeight, int nWeight, BYTE bItalic, BYTE nOutPrecision, LPCTSTR lpszFacename)
{
	m_nHeightHeaderMain		= nHeight;
	m_nWeightHeaderMain		= nWeight;
	m_bItalicHeaderMain		= bItalic;
	m_nOutPreHeaderMain		= nOutPrecision;
	m_FacenameHeaderMain	= lpszFacename;
}

//##ModelId=49B87BB502BF
void CAboutDlgEx::SetFontHeaderSub(int nHeight, int nWeight, BYTE bItalic, BYTE nOutPrecision, LPCTSTR lpszFacename)
{
	m_nHeightHeaderSub	= nHeight;
	m_nWeightHeaderSub	= nWeight;
	m_bItalicHeaderSub	= bItalic;
	m_nOutPreHeaderSub	= nOutPrecision;
	m_FacenameHeaderSub	= lpszFacename;
}

//##ModelId=49B87BB502C5
void CAboutDlgEx::SetFontVersionMain(int nHeight, int nWeight, BYTE bItalic, BYTE nOutPrecision, LPCTSTR lpszFacename)
{
	m_nHeightVersionMain	= nHeight;
	m_nWeightVersionMain	= nWeight;
	m_bItalicVersionMain	= bItalic;
	m_nOutPreVersionMain	= nOutPrecision;
	m_FacenameVersionMain	= lpszFacename;
}

//##ModelId=49B87BB502D2
void CAboutDlgEx::SetFontVersionSub(int nHeight, int nWeight, BYTE bItalic, BYTE nOutPrecision, LPCTSTR lpszFacename)
{
	m_nHeightVersionSub		= nHeight;
	m_nWeightVersionSub		= nWeight;
	m_bItalicVersionSub		= bItalic;
	m_nOutPreVersionSub		= nOutPrecision;
	m_FacenameVersionSub	= lpszFacename;
}

//##ModelId=49B87BB502DE
void CAboutDlgEx::SetStartHeaderMain(CPoint pt)		{ m_ptHeaderMain = pt;				}
//##ModelId=49B87BB502E0
void CAboutDlgEx::SetStartHeaderSub(CPoint pt)		{ m_ptHeaderSub = pt;				}
//##ModelId=49B87BB502EE
void CAboutDlgEx::SetStartVersion(CPoint pt)		{ m_ptVersion = pt;					}
//##ModelId=49B87BB502F0
void CAboutDlgEx::SetStartVersionLeft(CPoint pt)	{ m_ptVersionsLeft = pt;			}
//##ModelId=49B87BB502F2
void CAboutDlgEx::SetStartVersionRight(CPoint pt)	{ m_ptVersionsRight = pt;			}
//##ModelId=49B87BB502FD
void CAboutDlgEx::SetStartCustomizedMain(CPoint pt)	{ m_ptCustomizedMain = pt;			}
//##ModelId=49B87BB502FF
void CAboutDlgEx::SetStartCustomized(CPoint pt)		{ m_ptCustomized = pt;				}
//##ModelId=49B87BB50301
void CAboutDlgEx::SetDeltaHeaderMain(CSize delta)	{ m_szDeltaHeaderMain = delta;		}
//##ModelId=49B87BB5030E
void CAboutDlgEx::SetDeltaHeaderSub(CSize delta)	{ m_szDeltaHeaderSub = delta;		}
//##ModelId=49B87BB50310
void CAboutDlgEx::SetDeltaVersionLeft(CSize delta)	{ m_szDeltaVersionsLeft = delta;	}
//##ModelId=49B87BB5031C
void CAboutDlgEx::SetDeltaVersionRight(CSize delta)	{ m_szDeltaVersionsRight = delta;	}
//##ModelId=49B87BB5031E
void CAboutDlgEx::SetDeltaCustomized(CSize delta)	{ m_szDeltaCustomized = delta;		}
//##ModelId=49B87BB5032C
void CAboutDlgEx::SetTextCustomized(const char *s)	{ m_TextCustomized = s;		}
//##ModelId=49B87BB5032E
void CAboutDlgEx::SetTextVersion(const char *s)		{ m_TextVersion = s;		}

//##ModelId=49B87BB5033E
void CAboutDlgEx::AddStringVersion(CString s)
{
	m_ListTextVersions.AddTail(s);
}

//##ModelId=49B87BB50340
void CAboutDlgEx::AddStringVersionRight(CString s)
{
	m_ListTextVersionsRight.AddTail(s);
}

//##ModelId=49B87BB5034B
void CAboutDlgEx::AddStringCustom(CString s)
{
	m_ListTextCustomized.AddTail(s);
}

//##ModelId=49B87BB50330
void CAboutDlgEx::AddStringHeaderMain(CString s)
{
	m_ListTextHeaderMain.AddTail(s);
}

//##ModelId=49B87BB5033C
void CAboutDlgEx::AddStringHeaderSub(CString s)
{
	m_ListTextHeaderSub.AddTail(s);
}

//##ModelId=49B87BB5034D
void CAboutDlgEx::OnDraw(CDC *pDC)
{
	double		iUmrechnung		= -pDC->GetDeviceCaps(LOGPIXELSY)/72,
				dHeaderMain		= 0.5 + iUmrechnung * m_nHeightHeaderMain,
				dHeaderSub		= 0.5 + iUmrechnung * m_nHeightHeaderSub,
				dVersionMain	= 0.5 + iUmrechnung * m_nHeightVersionMain,
				dVersionSub		= 0.5 + iUmrechnung * m_nHeightVersionSub;
	UINT		uOldAlign;
	CFont		fontVersionSub,
				fontVersionMain,
				fontHeaderSub,
				fontHeaderMain,
				*pOldFont;
	COLORREF	oldColor;
	int			oldBkMode;

	fontHeaderMain.		CreateFont((int)dHeaderMain,	0,0,0, m_nWeightHeaderMain,		m_bItalicHeaderMain,	0,0,0, m_nOutPreHeaderMain,		0, PROOF_QUALITY,0, m_FacenameHeaderMain);
	fontHeaderSub.		CreateFont((int)dHeaderSub,		0,0,0, m_nWeightHeaderSub,		m_bItalicHeaderSub,		0,0,0, m_nOutPreHeaderSub,		0, PROOF_QUALITY,0, m_FacenameHeaderSub);
	fontVersionMain.	CreateFont((int)dVersionMain,	0,0,0, m_nWeightVersionMain,	m_bItalicVersionMain,	0,0,0, m_nOutPreVersionMain,	0, PROOF_QUALITY,0, m_FacenameVersionMain);
	fontVersionSub.		CreateFont((int)dVersionSub,	0,0,0, m_nWeightVersionSub,		m_bItalicVersionSub,	0,0,0, m_nOutPreVersionSub,		0, PROOF_QUALITY,0, m_FacenameVersionSub);

	m_pFontVersionSub		= &fontVersionSub;
	m_pFontVersionMain		= &fontVersionMain;
	m_pFontHeaderSub		= &fontHeaderSub;
	m_pFontHeaderMain		= &fontHeaderMain;
	
	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(pDC))
		return;

	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	pDC->BitBlt(0, 0, m_bitmapinfo.bmWidth, m_bitmapinfo.bmHeight, &dcImage, 0, 0, SRCCOPY);

	oldColor	= pDC->SetTextColor(RGB(0,0,0));
	oldBkMode	= pDC->SetBkMode(TRANSPARENT);
	uOldAlign	= pDC->SetTextAlign(TA_LEFT | TA_TOP);
	pOldFont	= (CFont *) pDC->SelectStockObject(ANSI_FIXED_FONT);

	//
	// real drawing
	//
	DrawEverything(pDC);

	//
	// old Settings
	//
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(oldBkMode);
	pDC->SetTextAlign(uOldAlign);
	pDC->SelectObject(pOldFont);
	dcImage.SelectObject(pOldBitmap);

	fontVersionSub.DeleteObject();
	fontVersionMain.DeleteObject();
	fontHeaderSub.DeleteObject();
	fontHeaderMain.DeleteObject();
}

/////////////////////////////
//
// Drawings
//
/////////////////////////////
//##ModelId=49B87BB50350
void CAboutDlgEx::DrawEverything(CDC *pDC)
{
	DrawPre(pDC);
	DrawHeaderMain(pDC);
	DrawHeaderSub(pDC);
	DrawVersionMain(pDC);
	DrawVersionSub(pDC);
	DrawVersionSubRight(pDC);
	DrawCustomizedMain(pDC);
	DrawCustomizedSub(pDC);
	DrawPost(pDC);
}

/////////////////////////////
//
// Header Main
//
/////////////////////////////
//##ModelId=49B87BB5035D
void CAboutDlgEx::DrawHeaderMain(CDC *pDC)
{
	POSITION	pos;
	CString		str;
	int			xPos,	yPos,
				dx,		dy;

	pDC->SetTextAlign(TA_LEFT | TA_BOTTOM);
	pDC->SelectObject(m_pFontHeaderMain);

	xPos = m_ptHeaderMain.x;	dx = m_szDeltaHeaderMain.cx;
	yPos = m_ptHeaderMain.y;	dy = m_szDeltaHeaderMain.cy;
	for(pos = m_ListTextHeaderMain.GetHeadPosition(); pos!=NULL;  )
	{
		str = m_ListTextHeaderMain.GetNext(pos);
		pDC->TextOut(xPos, yPos, str);
		xPos += dx;
		yPos += dy;
	}
}

/////////////////////////////
//
// Header Sub
//
/////////////////////////////
//##ModelId=49B87BB50360
void CAboutDlgEx::DrawHeaderSub(CDC *pDC)
{
	POSITION	pos;
	CString		str;
	int			xPos,	yPos,
				dx,		dy;

	pDC->SetTextAlign(TA_LEFT | TA_BOTTOM);
	pDC->SelectObject(m_pFontHeaderSub);

	xPos = m_ptHeaderSub.x;	dx = m_szDeltaHeaderSub.cx;
	yPos = m_ptHeaderSub.y;	dy = m_szDeltaHeaderSub.cy;
	for(pos = m_ListTextHeaderSub.GetHeadPosition(); pos!=NULL;  )
	{
		str = m_ListTextHeaderSub.GetNext(pos);
		pDC->TextOut(xPos, yPos, str);

		xPos += dx;
		yPos += dy;
	}
}

//##ModelId=49B87BB503BA
void CAboutDlgEx::DrawBigSmallToggled(CDC *pDC, CString &str, int xStart, int yStart, CFont *pFontBig, CFont *pFontSmall)
{
	// 
	// all capital letters: fontHeaderSubBig
	// all small    -=-     fontHeaderSubSmall
	//
	char		*pText,
				*pPosMax,
				*pPos,
				*pTemp,
				*pPartString;
	BOOL		bBig;
	int			delta;
	CSize		size;
	int			xTemp;
	
	pText		= new char [str.GetLength() + 1];
	pPartString = new char [str.GetLength() + 1];
	
	strcpy(pText, str);
	pPos		= pText;
	pPosMax		= pPos + strlen(pText);
	xTemp		= xStart;
	
	for( ; pPos<pPosMax ; )
	{
		// first one of partstring
		bBig = isupper(*pPos);
		
		// extract part
		pTemp = pPos;
		if( bBig )
		{
			while( /*!islower*/ isupper(*pTemp) && (pTemp<pPosMax) )	// all extra-characters
				pTemp++;
		}
		else
		{
			while( !isupper(*pTemp) && (pTemp<pPosMax) )
				pTemp++;
		}
		strncpy(pPartString, pPos, (pTemp-pPos) );
		*(pPartString +(pTemp-pPos)) = '\0';

		// set Font
		if( bBig )	
		{	
			pDC->SelectObject(pFontBig);
			delta = 2;
		}
		else
		{
			pDC->SelectObject(pFontSmall);
			delta = 0;
		}

		// TextOut
		pDC->TextOut(xTemp, yStart+delta, pPartString, strlen(pPartString));
		
		// set actual position
		size = pDC->GetTextExtent(pPartString, strlen(pPartString));	
		xTemp += size.cx;
		
		pPos += strlen(pPartString);
	}

	delete pText;
	delete pPartString;
}

/////////////////////////////
//
// Version Main
//
/////////////////////////////
//##ModelId=49B87BB50363
void CAboutDlgEx::DrawVersionMain(CDC *pDC)
{
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
	pDC->SelectObject(m_pFontVersionMain);
	pDC->TextOut(m_ptVersion.x, m_ptVersion.y, m_TextVersion);
}

/////////////////////////////
//
// Version Sub
//
/////////////////////////////
//##ModelId=49B87BB5036B
void CAboutDlgEx::DrawVersionSub(CDC *pDC)
{
	POSITION	pos;
	CString		str;
	int			xPos,	yPos,
				dx,		dy;

	pDC->SetTextAlign(TA_LEFT | TA_TOP);
	pDC->SelectObject(m_pFontVersionSub);
	
	xPos = m_ptVersionsLeft.x;	dx = m_szDeltaVersionsLeft.cx;
	yPos = m_ptVersionsLeft.y;	dy = m_szDeltaVersionsLeft.cy;
	for(pos = m_ListTextVersions.GetHeadPosition(); pos!=NULL;  )
	{
		str = m_ListTextVersions.GetNext(pos);
		pDC->TextOut(xPos, yPos, str);
		xPos += dx;
		yPos += dy;
	}
}

/////////////////////////////
//
// Version Sub Right
//
/////////////////////////////
//##ModelId=49B87BB5036E
void CAboutDlgEx::DrawVersionSubRight(CDC *pDC)
{
	POSITION	pos;
	CString		str;
	int			xPos,	yPos,
				dx,		dy;

	pDC->SetTextAlign(TA_RIGHT | TA_TOP);
	pDC->SelectObject(m_pFontVersionSub);
	
	xPos = m_ptVersionsRight.x;	dx = m_szDeltaVersionsRight.cx;
	yPos = m_ptVersionsRight.y;	dy = m_szDeltaVersionsRight.cy;
	for(pos = m_ListTextVersionsRight.GetHeadPosition(); pos!=NULL; )
	{
		str = m_ListTextVersionsRight.GetNext(pos);
		pDC->TextOut(xPos, yPos, str);
		xPos += dx;
		yPos += dy;
	}
}

/////////////////////////////
//
// Customized Main
//
/////////////////////////////
//##ModelId=49B87BB50371
void CAboutDlgEx::DrawCustomizedMain(CDC *pDC)
{
	pDC->SetTextAlign(TA_LEFT | TA_TOP);
	pDC->SelectObject(m_pFontVersionMain);
	pDC->TextOut(m_ptCustomizedMain.x, m_ptCustomizedMain.y, m_TextCustomized);
}

/////////////////////////////
//
// Customized Sub
//
/////////////////////////////
//##ModelId=49B87BB50374
void CAboutDlgEx::DrawCustomizedSub(CDC *pDC)
{
	POSITION	pos;
	CString		str;
	int			xPos,	yPos,
				dx,		dy;

	pDC->SetTextAlign(TA_LEFT | TA_TOP);
	pDC->SelectObject(m_pFontVersionSub);
	
	xPos = m_ptCustomized.x;	dx = m_szDeltaCustomized.cx;
	yPos = m_ptCustomized.y;	dy = m_szDeltaCustomized.cy;
	for(pos = m_ListTextCustomized.GetHeadPosition(); pos!=NULL; )
	{
		str = m_ListTextCustomized.GetNext(pos);
		pDC->TextOut(xPos, yPos, str);
		xPos += dx;
		yPos += dy;
	}
}

/////////////////////////////
//
// DrawPre
// DrawPost
//
/////////////////////////////
//##ModelId=49B87BB5037A
void CAboutDlgEx::DrawPre(CDC *pDC)
{
	CBitmap bmpIcon;
	bmpIcon.LoadBitmap( m_IdIcon );

	BITMAP bmpInfo;
	bmpIcon.GetBitmap( &bmpInfo );

	CRect rect;
	GetClientRect( &rect );

	DrawTransparentBitmap( 
					pDC, 
					&bmpIcon, 
					( short )rect.Width() / 2 - bmpInfo.bmWidth / 2, 
					( short )rect.Height() / 2 - bmpInfo.bmHeight / 2 + 10,
					RGB( 255, 255, 255 ) 
					);
}

//##ModelId=49B87BB5037D
void CAboutDlgEx::DrawPost(CDC *pDC)
{
	// use in your derived classes!
}

//////////////////////////////////////////////////////////////////////////////////////////77
//
// DrawTransparentBitmap
//
//////////////////////////////////////////////////////////////////////////////////////////77

//##ModelId=49B87BB503CB
void CAboutDlgEx::DrawTransparentBitmap(CDC *pDC, CBitmap *pBitmap, short xStart, short yStart, 
						   COLORREF cTransparentColor)
{
	COLORREF	cColor;
	CBitmap		*pBMAndBack,
				*pBMAndObject, 
				*pBMAndMem, 
				*pBMSave,
				*pBMBackOld, 
				*pBMObjectOld, 
				*pBMMemOld, 
				*pBMSaveOld;
	CDC			*pDCMem, 
				*pDCBack, 
				*pDCObject, 
				*pDCTemp, 
				*pDCSave;
	POINT		ptSize;
	int			mapMode = pDC->GetMapMode(),
				i;
	BITMAP		bitMap;

	pDCTemp = new CDC();
	pDCTemp->CreateCompatibleDC(pDC);
	pDCTemp->SetMapMode(mapMode);
	pDCTemp->SelectObject(pBitmap);

	i			= pBitmap->GetBitmap(&bitMap);
	ptSize.x	= bitMap.bmWidth;
	ptSize.y	= bitMap.bmHeight;
	pDCTemp->DPtoLP(&ptSize, 1);      // Convert from device to logical points

	// Create some DCs to hold temporary data.
	pDCBack = new CDC();	pDCBack->CreateCompatibleDC(pDC);		pDCBack->SetMapMode(mapMode);
	pDCObject = new CDC();	pDCObject->CreateCompatibleDC(pDC);		pDCObject->SetMapMode(mapMode);
	pDCMem  = new CDC();	pDCMem->CreateCompatibleDC(pDC);		pDCMem->SetMapMode(mapMode);
	pDCSave = new CDC();	pDCSave->CreateCompatibleDC(pDC);		pDCSave->SetMapMode(mapMode);

	// Create a bitmap for each DC. DCs are required for a number of GDI functions.

	// Monochrome DC
	pBMAndBack		= new CBitmap();	pBMAndBack->CreateBitmap(abs(ptSize.x), abs(ptSize.y), 1, 1, NULL);
	pBMAndObject	= new CBitmap();	pBMAndObject->CreateBitmap(abs(ptSize.x), abs(ptSize.y), 1, 1, NULL);
	
	pBMAndMem		= new CBitmap();	pBMAndMem->CreateCompatibleBitmap(pDC, abs(ptSize.x), abs(ptSize.y));
	pBMSave			= new CBitmap();	pBMSave->CreateCompatibleBitmap(pDC, abs(ptSize.x), abs(ptSize.y));

	// Each DC must select a bitmap object to store pixel data.
	pBMBackOld   = pDCBack->SelectObject(pBMAndBack);
	pBMObjectOld = pDCObject->SelectObject(pBMAndObject);
	pBMMemOld    = pDCMem->SelectObject(pBMAndMem);
	pBMSaveOld   = pDCSave->SelectObject(pBMSave);

	// Set proper mapping mode.
//	pDCTemp->SetMapMode(pDC->GetMapMode());

	// Save the bitmap sent here, because it will be overwritten.
	pDCSave->BitBlt(0, 0, ptSize.x, ptSize.y, pDCTemp, 0, 0, SRCCOPY);

	// Set the background color of the source DC to the color.
	// contained in the parts of the bitmap that should be transparent
	cColor = pDCTemp->SetBkColor(cTransparentColor);

	// Create the object mask for the bitmap by performing a BitBlt
	// from the source bitmap to a monochrome bitmap.
	pDCObject->BitBlt(0, 0, ptSize.x, ptSize.y, pDCTemp, 0, 0, SRCCOPY);

	// Set the background color of the source DC back to the original color.
	pDCTemp->SetBkColor(cColor);

	// Create the inverse of the object mask.
	pDCBack->BitBlt(0, 0, ptSize.x, ptSize.y, pDCObject, 0, 0, NOTSRCCOPY);

	// Copy the background of the main DC to the destination.
	pDCMem->BitBlt(0, 0, ptSize.x, ptSize.y, pDC, xStart, yStart, SRCCOPY);

	// Mask out the places where the bitmap will be placed.
	pDCMem->BitBlt(0, 0, ptSize.x, ptSize.y, pDCObject, 0, 0, SRCAND);

	// Mask out the transparent colored pixels on the bitmap.
	// ??
	pDCTemp->BitBlt(0, 0, ptSize.x, ptSize.y, pDCBack, 0, 0, SRCAND);

	// XOR the bitmap with the background on the destination DC.
	pDCMem->BitBlt(0, 0, ptSize.x, ptSize.y, pDCTemp, 0, 0, SRCPAINT);

	// Copy the destination to the screen.
	pDC->BitBlt(xStart, yStart, ptSize.x, ptSize.y, pDCMem, 0, 0, SRCCOPY);

	// Place the original bitmap back into the bitmap sent here.
	pDCTemp->BitBlt(0, 0, ptSize.x, ptSize.y - abs(ptSize.y), pDCSave, 0, 0, SRCCOPY);
	
	// Delete the memory bitmaps.
	DeleteObject(pDCBack->SelectObject(pBMBackOld));
	DeleteObject(pDCObject->SelectObject(pBMObjectOld));
	DeleteObject(pDCMem->SelectObject(pBMMemOld));
	DeleteObject(pDCSave->SelectObject(pBMSaveOld));

	delete pBMAndBack;
	delete pBMAndObject;
	delete pBMAndMem;
	delete pBMSave;


	// Delete the memory DCs.
	pDCMem->DeleteDC();			delete pDCMem;
	pDCBack->DeleteDC();		delete pDCBack;
	pDCObject->DeleteDC();		delete pDCObject;
	pDCSave->DeleteDC();		delete pDCSave;
	pDCTemp->DeleteDC();		delete pDCTemp;
}
