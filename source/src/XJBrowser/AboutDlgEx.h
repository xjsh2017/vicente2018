#if !defined(ABOUTDLGEX_H)
#define ABOUTDLGEX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDlgEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAboutDlgEx dialog

//##ModelId=49B87BB50271
class CAboutDlgEx : public CDialog
{
// Construction
public:
	//##ModelId=49B87BB50273
	void SetDefaultsText();
	//##ModelId=49B87BB50280
	void SetDefaultsPos();
	//##ModelId=49B87BB50281
	CAboutDlgEx(CWnd* pParent = NULL);   // standard constructor

	//
	// Setting Text-effects and positions
	//
	//##ModelId=49B87BB50290
	virtual void	SetDefaultsFonts(void);			// sets the fonts
	//##ModelId=49B87BB5029F
	virtual void	SetDefaultsPositions(void);		// sets default positions: please overload to fit to your bitmaps

	// 
	// Setting the bitmap (very important!)
	// you could create a SetBitmap(CBitmap *) function here, i was too lazy for that!
	//
	//##ModelId=49B87BB502A2
	void			SetBitmapId(UINT idBitmap);	
	//##ModelId=49B87BB502AF
	void			SetIconId(UINT idIcon);	

	//
	// setting the font-properties
	//
	//##ModelId=49B87BB502B1
	void			SetFontHeaderMain(int nHeight, int nWeight, BYTE bItalic, BYTE nOutPrecision, LPCTSTR lpszFacename);
	//##ModelId=49B87BB502BF
	void			SetFontHeaderSub(int nHeight, int nWeight, BYTE bItalic, BYTE nOutPrecision, LPCTSTR lpszFacename);
	//##ModelId=49B87BB502C5
	void			SetFontVersionMain(int nHeight, int nWeight, BYTE bItalic, BYTE nOutPrecision, LPCTSTR lpszFacename);
	//##ModelId=49B87BB502D2
	void			SetFontVersionSub(int nHeight, int nWeight, BYTE bItalic, BYTE nOutPrecision, LPCTSTR lpszFacename);
	
	// 
	// Setting text-positions
	//
	//##ModelId=49B87BB502DE
	void			SetStartHeaderMain(CPoint pt);
	//##ModelId=49B87BB502E0
	void			SetStartHeaderSub(CPoint pt);
	//##ModelId=49B87BB502EE
	void			SetStartVersion(CPoint pt);
	//##ModelId=49B87BB502F0
	void			SetStartVersionLeft(CPoint pt);
	//##ModelId=49B87BB502F2
	void			SetStartVersionRight(CPoint pt);
	//##ModelId=49B87BB502FD
	void			SetStartCustomizedMain(CPoint pt);
	//##ModelId=49B87BB502FF
	void			SetStartCustomized(CPoint pt);
	//##ModelId=49B87BB50301
	void			SetDeltaHeaderMain(CSize delta);
	//##ModelId=49B87BB5030E
	void			SetDeltaHeaderSub(CSize delta);
	//##ModelId=49B87BB50310
	void			SetDeltaVersionLeft(CSize delta);
	//##ModelId=49B87BB5031C
	void			SetDeltaVersionRight(CSize delta);
	//##ModelId=49B87BB5031E
	void			SetDeltaCustomized(CSize delta);
	
	//
	// setting the text
	//
	//##ModelId=49B87BB5032C
	void			SetTextCustomized(const char *s);
	//##ModelId=49B87BB5032E
	void			SetTextVersion(const char *s);
	//##ModelId=49B87BB50330
	void			AddStringHeaderMain(CString s);
	//##ModelId=49B87BB5033C
	void			AddStringHeaderSub(CString s);
	//##ModelId=49B87BB5033E
	void			AddStringVersion(CString s);
	//##ModelId=49B87BB50340
	void			AddStringVersionRight(CString s);
	//##ModelId=49B87BB5034B
	void			AddStringCustom(CString s);

	//##ModelId=49B87BB5034D
	virtual void	OnDraw(CDC *pDC);				// set and reset CDC-Settings
	//##ModelId=49B87BB50350
	virtual void	DrawEverything(CDC *pDC);		// calls drawing-subfunctions
	//##ModelId=49B87BB5035D
	virtual void	DrawHeaderMain(CDC *pDC);		// Drawing: Main Header
	//##ModelId=49B87BB50360
	virtual void	DrawHeaderSub(CDC *pDC);		// Drawing: Sub- Header
	//##ModelId=49B87BB50363
	virtual void	DrawVersionMain(CDC *pDC);		// Drawing: "Version"
	//##ModelId=49B87BB5036B
	virtual void	DrawVersionSub(CDC *pDC);		// Drawing: Version -Text (left)
	//##ModelId=49B87BB5036E
	virtual void	DrawVersionSubRight(CDC *pDC);	// Drawing: Version -Text (right)
	//##ModelId=49B87BB50371
	virtual void	DrawCustomizedMain(CDC *pDC);	// Drawing: "Customized for"
	//##ModelId=49B87BB50374
	virtual void	DrawCustomizedSub(CDC *pDC);	// Drawing: Customized second line
	//##ModelId=49B87BB5037A
	virtual void	DrawPre(CDC *pDC);				// Drawing: before all other stuff
	//##ModelId=49B87BB5037D
	virtual void	DrawPost(CDC *pDC);				// Drawing: after all other stuff

// Dialog Data
	//{{AFX_DATA(CAboutDlgEx)
	enum { IDD = IDD_ABOUT };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlgEx)
	public:
	protected:
	//##ModelId=49B87BB50380
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//##ModelId=49B87BB5038B
	BITMAP m_bitmapinfo;
	//##ModelId=49B87BB50390
	CDC m_dcMem;
	//##ModelId=49B87BB5039C
	CBitmap m_bitmap;
	//##ModelId=49B87BB503AA
	CButton m_bnOK;

	//##ModelId=49B87BB503AE
	UINT		m_IdBitmap;
	//##ModelId=49B87BB503B9
	UINT		m_IdIcon;
	CFont		*m_pFontVersionSub,
				*m_pFontVersionMain,
				*m_pFontHeaderSub,
				*m_pFontHeaderMain;

	double		m_nHeightHeaderMain,		m_nHeightVersionMain,	m_nHeightVersionSub,	m_nHeightHeaderSub;
	int			m_nWeightHeaderMain,		m_nWeightVersionMain,	m_nWeightVersionSub,	m_nWeightHeaderSub;
	BYTE		m_bItalicHeaderMain,		m_bItalicVersionMain,	m_bItalicVersionSub,	m_bItalicHeaderSub;
	BYTE		m_nOutPreHeaderMain,		m_nOutPreVersionMain,	m_nOutPreVersionSub,	m_nOutPreHeaderSub;
	CString		m_FacenameHeaderMain,		m_FacenameVersionMain,	m_FacenameVersionSub,	m_FacenameHeaderSub;
	CPoint		m_ptHeaderMain,				// starting-points for text
				m_ptHeaderSub,	
				m_ptVersion,
				m_ptVersionsLeft,
				m_ptVersionsRight,
				m_ptCustomizedMain,
				m_ptCustomized;
	CSize		m_szDeltaHeaderMain,		// size between lines
				m_szDeltaHeaderSub,
				m_szDeltaVersionsLeft,
				m_szDeltaVersionsRight,
				m_szDeltaCustomized;
	CString		m_TextCustomized,
				m_TextVersion;
	CStringList	m_ListTextHeaderMain,
				m_ListTextHeaderSub,
				m_ListTextVersions,
				m_ListTextVersionsRight,
				m_ListTextCustomized;

	//##ModelId=49B87BB503BA
	void	DrawBigSmallToggled(CDC *pDC, CString &str, int xStart, int yStart, CFont *pFontBig, CFont *pFontSmall);
	//##ModelId=49B87BB503CB
	void	DrawTransparentBitmap(CDC *pDC, CBitmap *pBitmap, short xStart, short yStart, COLORREF cTransparentColor);
	// Generated message map functions
	//{{AFX_MSG(CAboutDlgEx)
	//##ModelId=49B87BB503D9
	virtual BOOL OnInitDialog();
	//##ModelId=49B87BB503DB
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(ABOUTDLGEX_H)
