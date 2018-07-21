/* Generated by Together */

#ifndef EICONTEXTBUTTONPROPERTY_H
#define EICONTEXTBUTTONPROPERTY_H

class EPropWndEdit;
class IPropertyHost;
class EPropListCtrl;

#define ID_PROPERTY_OFFSET_EDIT   0
#define ID_PROPERTY_OFFSET_BUTTON 1
#define ID_PROPERTY_OFFSET_COMBO  2

#include "EProperty.h"

class EIconTextButtonProperty : public EProperty
{
public:
	enum eFormatFlags
	{
		DEFAULT		= 0x00,
		THOUSANDS	= 0x01,
		SCIENTIFIC	= 0x02,
	};


	static int s_nMargin;

protected:

	bool m_bEnabled;

	int m_nButtonWidth;
	int m_nIconWidth;
	int m_nTextAlignment;	//0=left, 1=center, 2=right

	EPropWndEdit* m_pEdit;
	//CEdit* m_pEdit;
    CButton*      m_pButton;

	CRect m_IconRect;
	CRect m_TextRect;
	CRect m_ButtRect;

	CString m_sInitialValueText;

protected:

	CString FormatThousands( const CString& s );
	char GetThousandSeparator();

public:    

	EIconTextButtonProperty( IPropertyHost* pHost , const CString& sName , void* pProperty , bool bEnabled );
	virtual ~EIconTextButtonProperty();

	virtual void Draw( CDC* pDC , EPropWnd* pParent );
	virtual int GetHeight( CDC* pDC );

	//
	//
	//
	virtual void OnPropActivate( EPropWnd* pParent );
	virtual void OnPropClose();
	virtual CString GetSortValueString();

	//
	// Override these for special types
	//

	virtual int GetIconWidth();		//if you want to draw an icon, return icon width here
	virtual bool HasButton();		//if you want a button to the right, override this and return true
	virtual void DrawIcon( CDC* pDC , EPropWnd* pParent , const CRect& Rect );	//override this to draw icon, also override GetIconWidth() to enable icon on your property
	virtual CString GetTextValue();	//override this and return the text label of yout property
	virtual int GetPreferedWidth( CDC* pDC );	//do your best to return prefered width of your property to enable the view to calculate scroll page size

	//
	//
	//

	EIconTextButtonProperty* SetTextAlign( int nAlignment );	//0=left, 1=center, 2=right
//instead, parse argument to SetTextAlign()
//	void SetTextAlignmentLeft()   { m_nTextAlignment = 0; }
//	void SetTextAlignmentCenter() { m_nTextAlignment = 1; }
//	void SetTextAlignmentRight()  { m_nTextAlignment = 2; }

protected:

	virtual bool HasEdit();			//if you want user to be able to edit text, override and return true

};
#endif //EICONTEXTBUTTONPROPERTY_H