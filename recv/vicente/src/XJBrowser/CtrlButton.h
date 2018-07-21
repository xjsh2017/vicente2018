// CtrlButton.h 
//包括CColorButton 

#if !defined(CTRLBUTTON_H)
#define CTRLBUTTON_H

//##ModelId=49B87BA90186
class CFontButton:public CButton
{
private:
public:
	//##ModelId=49B87BA90197
	CFontButton(); 
	//##ModelId=49B87BA90198
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	//##ModelId=49B87BA9019B
	virtual int  ShowFontDialog();
	//##ModelId=49B87BA9019E
	LOGFONT  m_logFont;
	//##ModelId=49B87BA901A2
	COLORREF m_FontColor;	//字体颜色
	//##ModelId=49B87BA901A3
	int		 m_BKmode;		//是否透明背景(TRANSPARENT,OPAQUE)
	//##ModelId=49B87BA901A5
	COLORREF m_BKColor;		//字体背景色
protected:
	//{{AFX_MSG(CFontButton)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//##ModelId=49B87BA901A6
class CColorButton:public CButton
{
private:
public:
	//##ModelId=49B87BA901A8
	CColorButton(); 
	//##ModelId=49B87BA901A9
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	//##ModelId=49B87BA901B6
	virtual int  ShowColorDialog();
	//##ModelId=49B87BA901B8
	COLORREF m_Color;	//颜色
protected:
	//{{AFX_MSG(CColorButton)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif  // !defined(CTRLBUTTON_H)
