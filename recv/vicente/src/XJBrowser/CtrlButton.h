// CtrlButton.h 
//����CColorButton 

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
	COLORREF m_FontColor;	//������ɫ
	//##ModelId=49B87BA901A3
	int		 m_BKmode;		//�Ƿ�͸������(TRANSPARENT,OPAQUE)
	//##ModelId=49B87BA901A5
	COLORREF m_BKColor;		//���屳��ɫ
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
	COLORREF m_Color;	//��ɫ
protected:
	//{{AFX_MSG(CColorButton)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif  // !defined(CTRLBUTTON_H)
