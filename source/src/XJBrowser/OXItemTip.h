// ==========================================================================
//						   Class Specification : 
//								COXItemTip
// ==========================================================================

// Product Version: 7.52

// Copyright © Dundas Software Ltd. 1997 - 2002, All Rights Reserved
                          
// //////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Generic class that provides tips for items from List Control that are not 
// entirely visible. Derived from COXHookWnd - generic class that is 
// used to subclass any CWnd and provides capability to handle any message 
// routed to the hooked window before it. Handles WM_MOUSEMOVE messages to handle 
// drawing tips. When mouse is moved over any of items that only partially seen
// then item tip will be displayed (the same functionality as CTreeCtrl provides)
//
// To use it:
//
//	1)	call Attach to hook any CListCtrl based control
//
//	2)	for ownerdraw CListCtrl you have to provide additional information
//		about current item. Such information can be requested in two ways.
//		There is WM_DOYOUNEEDITEMTIP message which COXItemTip will send to
//		corresponding CListCtrl each time it needs to request specific 
//		information by means of NEEDITEMTIPINFO structure. Another way to 
//		provide item information is to use callback function that can be
//		set while calling COXItemInfo::Attach fuction and should be defined as:
//
//			void CallbackNeedItemTip(LPARAM lpSource, LPARAM lpInfo)
//
//		where lpSource is a pointer to COXItemTip object that is requesting 
//		item information and lpInfo is pointer to NEEDITEMTIPINFO structure.
//
//		As I mentioned before you have to provide item information for
//		COXItemTip if CListCtrl is ownerdrawfixed. But you can use the same 
//		technique to amend any item info that was collected by COXItemTip for
//		standard CListCtrl too. E.g. you can change the color of text or 
//		background or even font used to draw item tip window.
//
//		It is up to a programmer to choose the best way of providing item info.
//		As a possible solution I would recommend the next:
//		
//			a)	you want to use item tip functionality in your CListCtrl derived
//				class. Then it's good idea to provide handler for 
//				WM_DOYOUNEEDITEMTIP message.
//			b)	you want to use item tip functionality for any CListCtrl derived
//				class and you don't want to make any changes in the source code
//				and don't want to write any specific derivation. Then you can easily
//				register callback function.
//
//		You cannot use both way simultaneously because if you register callback 
//		function then WM_DOYOUNEEDITEMTIP message won't be sent.
//
//		Here is the description of NEEDITEMTIPINFO structure:
//
//			typedef struct _tagNEEDITEMTIPINFO
//			{
//				POINT point;
//				int row;
//				int col;
//				RECT rectItem;
//				TCHAR itemText[MAX_ITEMTIP];
//				int sizeText;
//				int offset;
//				LPARAM pFont;
//				COLORREF clrText;
//				COLORREF clrBackground;
//				int alignment;
//				int result;
//			} NEEDITEMTIPINFO, *LPNEEDITEMTIPINFO;
//
//		where:
//			point		-	current coordinates of mouse cursor, using this parameter 
//							you can	identify the item cell
//			row			-	item row
//			col			-	item column
//			rectItem	-	rectangle where item tip should be drawn, in particular 
//							the left-top point is the most important here
//			itemText	-	text to draw in item tip
//			sizeText	-	size of text
//			offset		-	offset from the right and left edge of item tip window 
//							to draw text in
//			pFont		-	pointer to a CFont object to be used to draw text in
//							item tip window
//			clrText		-	color to be used to draw text in item tip window
//			clrBackground	color to be used to fill background in item tip window
//			alignment	-	alignment used to draw text in item tip window, can be:
//							DT_LEFT
//							DT_CENTER
//							DT_RIGHT
//			result		-	result of collecting item information, can be:
//							ID_NIT_SUCCESS				-	info was successfully 
//															collected
//							ID_NIT_WRONGFORMAT			-	wrong format of CListCtrl 
//															(e.g. it is not in Report 
//															view)
//							ID_NIT_OUTOFCONTROLBORDER	-	point is out of valid 
//															area of CListCtrl items
//															
//							ID_NIT_OUTOFITEMBORDER		-	point is within CListCtrl
//															but doesn't lay in any 
//															item
//							ID_NIT_NOTHIDDEN			-	text of (row,col) is 
//															entirely seen 
//							ID_NIT_CUSTOMREJECT			-	user requests not to 
//															display an item tip
//
//	
//		Before WM_DOYOUNEEDITEMTIP message or callback function is fired above 
//		described structure is filled by COXItemTip::NeedItemTip function. But
//		if a hooked CListCtrl is ownerdrawfixed then next elements could be defined 
//		incorrectly: row, col, rectItem, itemText, offset.  
//
//
//	3)	In the case when you don't have and don't want to provide item info using
//		above described ways but still don't like the settings that COXItemTip
//		uses by default you can use next functions to change them:
//
//			// sets font to draw text
//			void SetTextFont(CFont* pFont);
//			// gets font to draw text
//			CFont* GetTextFont();
//
//			// sets color of text
//			void SetTextColor(COLORREF clrText);
//			// gets color of text
//			COLORREF GetTextColor();
//
//			// sets color of text
//			void SetBkColor(COLORREF clrBackground);
//			// gets color of text
//			COLORREF GetBkColor();
//
//			// sets offset from the right and left edge of item tip window
//			// to draw text in
//			void SetOffset(int nOffset)
//			// gets offset from the right and left edge of item tip window
//			// to draw text in
//			int GetOffset();
//

////////////////////////////////////////////////////////////////////////////////////


#ifndef _ITEMTIP_H
#define _ITEMTIP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#include "OXHookWnd.h"
#include "OXItemTipWnd.h"
#include "OXBaseSubclassedComboBox.h"

//#include <windowsx.h>
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

#ifndef _WIN32_IE
#if _MFC_VER>=0x0420
#define _WIN32_IE 0x0300
#else
#define _WIN32_IE 0x0200
#endif
#endif


// offset from left and right side of ItemTipWindow
#define ID_ITEMTIP_XOFFSET					6
#define ID_ITEMTIP_XOFFSET_LISTBOX			2
#define ID_ITEMTIP_XOFFSET_COMBOBOX			4

//result codes returned by NeedItemTip function
#define ID_NIT_SUCCESS						0x00000001
#define ID_NIT_WRONGFORMAT					0x00000002
#define ID_NIT_OUTOFCONTROLBORDER			0x00000003
#define ID_NIT_OUTOFITEMBORDER				0x00000004
#define ID_NIT_NOTHIDDEN					0x00000005
#define ID_NIT_CUSTOMREJECT					0x00000006

// max length of itemtip text
#define MAX_ITEMTIP							300


typedef struct _tagNEEDITEMTIPINFO
{
	POINT point;
	int row;
	int col;
	RECT rectItem;
	TCHAR itemText[MAX_ITEMTIP];
	int sizeText;
	int offset;
	LPARAM pFont;
	COLORREF clrText;
	COLORREF clrBackground;
	int alignment;
	int result;

	_tagNEEDITEMTIPINFO operator=(const _tagNEEDITEMTIPINFO& nitInfo)
	{
		if(this==&nitInfo)
			return *this;

		point.x=nitInfo.point.x;
		point.y=nitInfo.point.y;
		row=nitInfo.row;
		col=nitInfo.col;
		rectItem.left=nitInfo.rectItem.left;
		rectItem.top=nitInfo.rectItem.top;
		rectItem.right=nitInfo.rectItem.right;
		rectItem.bottom=nitInfo.rectItem.bottom;
		lstrcpyn(itemText,nitInfo.itemText,MAX_ITEMTIP);
		sizeText=nitInfo.sizeText;
		offset=nitInfo.offset;
		pFont=nitInfo.pFont;
		clrText=nitInfo.clrText;
		clrBackground=nitInfo.clrBackground;
		alignment=nitInfo.alignment;
		result=nitInfo.result;

		return *this;
	}

} NEEDITEMTIPINFO, *LPNEEDITEMTIPINFO;

// special callback function that can be registered in Attach function
typedef void (CALLBACK* NEEDITEMTIP_HELPER) (LPARAM lpSource, LPARAM lpInfo);

typedef struct _tagNMNEEDITEMTIPINFO
{
	NMHDR hdr;
	NEEDITEMTIPINFO nitInfo;
} NMNEEDITEMTIPINFO, *LPNMNEEDITEMTIPINFO;


template<class ATTACHED_CONTROL>
class COXBaseItemTip : public COXHookWnd 
{
protected:
	// pointer to hooked control
	ATTACHED_CONTROL* m_pAttachedCtrl;
	// item tip window used to display item tips
	COXItemTipWnd m_ItemTipWnd;
	
	// callback function that can be registered to provide or amen item info 
	// before it is displayed
	NEEDITEMTIP_HELPER m_pfnCallback;

	// font to draw text
	CFont* m_pFont;
	// color to draw text
	COLORREF m_clrText;
	// color of background
	COLORREF m_clrBackground;
	// offset to draw text
	int m_nOffset;

	// The heart of the class. Handle all needed messages. While deriving
	// your own class make sure to call this function. I wouldn't recommend you 
	// to change anything in it
	// --- In  :	msg	-	message ID
	//				wp	-	WPARAM
	//				lp	-	LPARAM
	// --- Out : 
	// --- Returns:	result of message handling. Different for different messages.
	// --- Effect :	Sets the number of color shades used in gradient filling. 
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);	

	// --- In  :	pnitInfo	-	pointer to NEEDITEMTIPINFO structure that 
	//								should be filled as a result of this function
	// --- Out : 
	// --- Returns:
	// --- Effect : fills pnitInfo with item information
	virtual void NeedItemTip(LPNEEDITEMTIPINFO pnitInfo);

	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Initializes the process of checking if item tip 
	//				should be displayed
	virtual void CheckItemTip();

public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXBaseItemTip();

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destroys the object
	virtual ~COXBaseItemTip();


	// --- In  :	pWnd		-	pointer to created ATTACHED_CONTROL-derived control
	//								to be hooked
	//				pfnCallback	-	callback function to be called to request item 
	//								info (must  be implemented if ATTACHED_CONTROL is 
	//								ownerdrawfixed). If pfnCallback is NULL then
	//								WM_DOYOUNEEDITEMTIP message will be sent (this 
	//								is an alternative way of providing item info).
	// --- Out : 
	// --- Returns:	TRUE if pWnd was successfully hooked, otherwise FALSE
	// --- Effect : Hooks window
	virtual BOOL Attach(CWnd* pWnd, NEEDITEMTIP_HELPER pfnCallback=NULL);

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Unhooks attached window if any was hooked
	virtual void Detach();


	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : set font, text color, background color and offset to their
	//				default values.
	void Reset();


	// --- In  :	pFont	-	pointer to CFont object to be used to draw text
	//							in item tip window
	// --- Out : 
	// --- Returns:
	// --- Effect : sets the font to draw text. By default we use the font 
	//				associated with hooked CListCtrl.
	inline void SetTextFont(CFont* pFont) { m_pFont=pFont; }
	
	// --- In  :	
	// --- Out : 
	// --- Returns:	pointer to CFont object to be used to draw text	in item tip 
	//				window
	// --- Effect : gets the font to draw text
	inline CFont* GetTextFont() const { return m_pFont; }


	// --- In  :	clrText	-	color to be used to draw text in item tip window
	// --- Out : 
	// --- Returns:
	// --- Effect : sets color of text. By default we use the color returned by 
	//				CListCtrl::GetTextColor function applied to the hooked CListCtrl.
	inline void SetTextColor(COLORREF clrText) { m_clrText=clrText; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	color to be used to draw text in item tip window
	// --- Effect : gets the color to draw text
	inline COLORREF GetTextColor() const { return m_clrText; }


	// --- In  :	clrBackground	-	color to be used to fill background 
	//									in item tip window
	// --- Out : 
	// --- Returns:
	// --- Effect : sets color of background. By default we use the background color 
	//				used in ToolTip controls.
	inline void SetBkColor(COLORREF clrBackground) { m_clrBackground=clrBackground; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	color to be used to fill background in item tip window
	// --- Effect : gets the color of background
	inline COLORREF GetBkColor() const { return m_clrBackground; }


	// --- In  :	nOffset	-	offset from the right and left edge of item tip 
	//							window to draw text in
	// --- Out : 
	// --- Returns:
	// --- Effect : sets the size of an offset. By default we use the 
	//				ID_ITEMTIP_XOFFSET constant
	inline void SetOffset(int nOffset) { m_nOffset=nOffset; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	offset from the right and left edge of item tip window to draw 
	//				text in
	// --- Effect : gets the size of an offset.
	inline int GetOffset() const { return m_nOffset; }
};

///////////////////////////////////////////////////////////////////////

// Constructor
template<class ATTACHED_CONTROL> 
COXBaseItemTip<ATTACHED_CONTROL>::COXBaseItemTip()
{
	m_pAttachedCtrl=NULL;
	m_pfnCallback=NULL;

	Reset();
}

// Destructor
template<class ATTACHED_CONTROL>
COXBaseItemTip<ATTACHED_CONTROL>::~COXBaseItemTip()
{
	if(::IsWindow(m_ItemTipWnd.m_hWnd))
	{
		m_ItemTipWnd.DestroyWindow();
	}
}

template<class ATTACHED_CONTROL> 
void COXBaseItemTip<ATTACHED_CONTROL>::Reset()
{
	m_pFont=NULL;
	m_clrText=ID_OX_COLOR_NONE;
	m_clrBackground=ID_OX_COLOR_NONE;
	m_nOffset=ID_ITEMTIP_XOFFSET;
}

// Attach ATTACHED_CONTROL handler.
template<class ATTACHED_CONTROL> 
BOOL COXBaseItemTip<ATTACHED_CONTROL>::Attach(CWnd* pWnd, 
											  NEEDITEMTIP_HELPER pfnCallback)
{
	ASSERT(pWnd!=NULL);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	HookWindow(pWnd);

	m_pAttachedCtrl=(ATTACHED_CONTROL*)pWnd;
	ASSERT(m_pAttachedCtrl!=NULL);

	m_pfnCallback=pfnCallback;

	if(!::IsWindow(m_ItemTipWnd.GetSafeHwnd()))
	{
		m_ItemTipWnd.Create(pWnd);
	}

	return TRUE;
}

// Detach ATTACHED_CONTROL handler. Called by default when hooked window is about
// to be destroyed.
template<class ATTACHED_CONTROL>
void COXBaseItemTip<ATTACHED_CONTROL>::Detach()
{
	UnhookWindow();

	m_pAttachedCtrl=NULL;
	m_pfnCallback=NULL;
}

////////////////////////////////////////////////////////////////////////////
// Message handler handles ItemTip-related messages
//
template<class ATTACHED_CONTROL>
LRESULT COXBaseItemTip<ATTACHED_CONTROL>::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(::IsWindow(m_hWndHooked));
	ASSERT(m_hWndHooked==m_pAttachedCtrl->m_hWnd);

	switch (msg) 
	{
	case WM_MOUSEMOVE: 
		{
			if(wp==0)
			{
				CheckItemTip();
			}
			break;
		}

	case WM_KEYDOWN: 
	case WM_SYSKEYDOWN: 
		{
			if(m_ItemTipWnd.GetStyle()&WS_VISIBLE)
			{
				m_ItemTipWnd.Hide();
			}
			break;
		}

	case WM_SHOWWINDOW:
		{
			if(!(BOOL)wp && m_ItemTipWnd.GetStyle()&WS_VISIBLE)
			{
				m_ItemTipWnd.Hide();
			}
			break;
		}
	
	case WM_WINDOWPOSCHANGED: 
		{
			if(m_ItemTipWnd.GetStyle()&WS_VISIBLE)
			{
				m_ItemTipWnd.Hide();
				CheckItemTip();
			}
			break;
		}
	}

	// I don't handle it: pass along
	return COXHookWnd::WindowProc(msg, wp, lp);
}

template<class ATTACHED_CONTROL>
void COXBaseItemTip<ATTACHED_CONTROL>::NeedItemTip(LPNEEDITEMTIPINFO pnitInfo)
{
	ASSERT(::IsWindow(m_hWndHooked));
	ASSERT(m_hWndHooked==m_pAttachedCtrl->m_hWnd);

	pnitInfo->result=ID_NIT_WRONGFORMAT;

	return;
}


template<class ATTACHED_CONTROL>
void COXBaseItemTip<ATTACHED_CONTROL>::CheckItemTip()
{
	DWORD dwMessagePos=::GetMessagePos();
	CPoint point(GET_X_LPARAM(dwMessagePos),GET_Y_LPARAM(dwMessagePos));
	::ScreenToClient(m_hWndHooked,&point);

	if(m_ItemTipWnd.IsWindowVisible() && ::IsWindowVisible(m_hWndHooked))
	{
		return;
	}

	NEEDITEMTIPINFO nitInfo;
	nitInfo.point.x=point.x;
	nitInfo.point.y=point.y;
	nitInfo.row=-1;
	nitInfo.col=-1;
	nitInfo.rectItem.left=0;
	nitInfo.rectItem.right=0;
	nitInfo.rectItem.top=0;
	nitInfo.rectItem.bottom=0;
	nitInfo.sizeText=MAX_ITEMTIP;
	nitInfo.offset=m_nOffset;
	nitInfo.pFont=(m_pFont==NULL ? 
		(LPARAM)GetHookedWnd()->GetFont() : (LPARAM)m_pFont);
	nitInfo.clrText=m_clrText;
	nitInfo.clrBackground=m_clrBackground;
	nitInfo.alignment=DT_LEFT;
	nitInfo.result=ID_NIT_SUCCESS;

	NeedItemTip(&nitInfo);

	if(m_pfnCallback==NULL)
	{
		::SendMessage(m_hWndHooked,WM_DOYOUNEEDITEMTIP,(WPARAM)0,(LPARAM)&nitInfo);
	}
	else
	{
		m_pfnCallback((LPARAM)this,(LPARAM)&nitInfo);
	}

	// notify parent window
	if(GetHookedWnd()->GetParent()!=NULL)
	{
		NMNEEDITEMTIPINFO nmnitInfo;
		nmnitInfo.hdr.hwndFrom=m_hWndHooked;
		nmnitInfo.hdr.idFrom=::GetDlgCtrlID(m_hWndHooked);
		nmnitInfo.hdr.code=WN_DOYOUNEEDITEMTIP;
		nmnitInfo.nitInfo=nitInfo;
		GetHookedWnd()->GetParent()->SendMessage(
			WM_NOTIFY,(WPARAM)nmnitInfo.hdr.idFrom,(LPARAM)&nmnitInfo);
		nitInfo=nmnitInfo.nitInfo;
	}

	if(nitInfo.result==ID_NIT_SUCCESS)
	{
		CString sText=(LPCTSTR)nitInfo.itemText;
		CRect rect=nitInfo.rectItem;
		m_ItemTipWnd.Display(rect,sText,nitInfo.offset,
			nitInfo.alignment,(CFont*)nitInfo.pFont,
			nitInfo.clrText,nitInfo.clrBackground);
	}
}

///////////////////////////////////////////////////////


class OX_CLASS_DECL COXItemTip : public COXBaseItemTip<CListCtrl> 
{
protected:
	DECLARE_DYNAMIC(COXItemTip);

	// --- In  :	pnitInfo	-	pointer to NEEDITEMTIPINFO structure that 
	//								should be filled as a result of this function
	// --- Out : 
	// --- Returns:
	// --- Effect : fills pnitInfo with item information
	virtual void NeedItemTip(LPNEEDITEMTIPINFO pnitInfo);

private:
	// --- In  :	nCol	-	number of column
	// --- Out : 
	// --- Returns:	real number of column taking into account order of column
	//				that could be changed starting from comctl32.dll version 4.70
	// --- Effect : 
	int GetRealColumn(int nCol);
};


///////////////////////////////////////////////////////


class OX_CLASS_DECL COXEditTip : public COXBaseItemTip<CEdit> 
{
protected:
	DECLARE_DYNAMIC(COXEditTip);

	// --- In  :	pnitInfo	-	pointer to NEEDITEMTIPINFO structure that 
	//								should be filled as a result of this function
	// --- Out : 
	// --- Returns:
	// --- Effect : fills pnitInfo with item information
	virtual void NeedItemTip(LPNEEDITEMTIPINFO pnitInfo);

public:
	// --- In  :	pWnd		-	pointer to created ATTACHED_CONTROL-derived control
	//								to be hooked
	//				pfnCallback	-	callback function to be called to request item 
	//								info (must  be implemented if ATTACHED_CONTROL is 
	//								ownerdrawfixed). If pfnCallback is NULL then
	//								WM_DOYOUNEEDITEMTIP message will be sent (this 
	//								is an alternative way of providing item info).
	// --- Out : 
	// --- Returns:	TRUE if pWnd was successfully hooked, otherwise FALSE
	// --- Effect : Hooks window
	virtual BOOL Attach(CWnd* pWnd, NEEDITEMTIP_HELPER pfnCallback=NULL);
};


///////////////////////////////////////////////////////


class OX_CLASS_DECL COXListBoxTip : public COXBaseItemTip<CListBox> 
{
protected:
	DECLARE_DYNAMIC(COXListBoxTip);

	// --- In  :	pnitInfo	-	pointer to NEEDITEMTIPINFO structure that 
	//								should be filled as a result of this function
	// --- Out : 
	// --- Returns:
	// --- Effect : fills pnitInfo with item information
	virtual void NeedItemTip(LPNEEDITEMTIPINFO pnitInfo);
};


///////////////////////////////////////////////////////

class OX_CLASS_DECL COXComboBoxTipHelper : 
public COXBaseSubclassedComboBox<CComboBox, CListBox, CEdit> 
{
protected:
	DECLARE_DYNAMIC(COXComboBoxTipHelper);

	COXEditTip m_editTip;
	COXListBoxTip m_listBoxTip;

protected:
	virtual void SubclassListBoxCtrl(CWnd* pWnd);
	virtual void SubclassEditCtrl(CWnd* pWnd);
};


class OX_CLASS_DECL COXComboBoxTip : public COXBaseItemTip<COXComboBoxTipHelper> 
{
protected:
	DECLARE_DYNAMIC(COXComboBoxTip);

protected:
	// --- In  :	pnitInfo	-	pointer to NEEDITEMTIPINFO structure that 
	//								should be filled as a result of this function
	// --- Out : 
	// --- Returns:
	// --- Effect : fills pnitInfo with item information
	virtual void NeedItemTip(LPNEEDITEMTIPINFO pnitInfo);
};


#endif // _ITEMTIP_H

