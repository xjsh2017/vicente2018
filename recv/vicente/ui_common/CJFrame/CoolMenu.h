////////////////////////////////////////////////////////////////
// 199 Microsoft Systems Journal. 
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// ==========================================================================  
// HISTORY:	  
// ==========================================================================  
//		1.01	13 Aug 1998 - Andrew Bancroft [ABancroft@lgc.com] - Since we've already 
//							  added the entire toolbar to the imagelist we need to 
//							  increment nNextImage even if we didn't add this button to 
//							  m_mapIDtoImage in the LoadToolbar() method.
//		1.01a	13 Aug 1998	- Peter Tewkesbury - Added AddSingleBitmap(...)
//							  method for adding a single bitmap to a pulldown
//							  menu item.
//		1.02	13 Aug 1998 - Omar L Francisco - Fixed bug with lpds->CtlType
//							  and lpds->itemData item checking.
//		1.03	12 Nov 1998	- Fixes debug assert in system menu. - Wang Jun
//		1.04	17 Nov 1998 - Fixes debug assert when you maximize a view - Wang Jun
//							  window, then try to use the system menu for the view.
//		1.05	09 Jan 1998 - Seain B. Conover [sc@tarasoft.com] - Fix for virtual 
//							  key names.
//		1.06	24 Feb 1999 - Michael Lange [michael.home@topdogg.com] - Fix for memory 
//							  leak in CMyItemData structure, added a destructor that 
//							  calls text.Empty().
//							- Boris Kartamishev [kbv@omegasoftware.com] - Fix for resource
//							  ID bug.
//							- Jeremy Horgan [jeremyhorgan@hotmail.com] - During 
//							  accelerator key processing OnInitMenuPopup() calls 
//							  ConvertMenu() which allocates a new CMyItemData for each 
//						      menu item. This is memory is normally freed by a call to 
//						      OnMenuSelect(), which is not called when processing 
//							  accelerator keys. This results in a memory leak. This was
//							  fixed by modifying the ~CCoolMenuManager() destructor.
//		1.07	24 Feb 1999 - Koji MATSUNAMI [kmatsu@inse.co.jp] - Fixed problem with 
//							  popup menus being drawn correctly as cool menus.
// ==========================================================================
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __COOLMENU_H__
#define __COOLMENU_H__

#include "SubClass.h"

//////////////////
// CCoolMenuManager implements "cool" menus with buttons in them. To use:
//
//  *	Instantiate in your CMainFrame.
//	 * Call Install to install it
//  * Call LoadToolbars or LoadToolbar to load toolbars
//
//  Don't forget to link with CoolMenu.cpp, Subclass.cpp and DrawTool.cpp!
//
//##ModelId=49B87BA901D4
class CCoolMenuManager : private CSubclassWnd {
public:
	DECLARE_DYNAMIC(CCoolMenuManager)
	//##ModelId=49B87BA901D6
	CCoolMenuManager();
	//##ModelId=49B87BA901E4
	~CCoolMenuManager();

	// You can set these any time
	//##ModelId=49B87BA901F4
	BOOL m_bShowButtons;			// use to control whether buttons are shown
	//##ModelId=49B87BA90203
	BOOL m_bAutoAccel;			// generate auto accelerators
	//##ModelId=49B87BA90204
	BOOL m_bUseDrawState;		// use ::DrawState for disabled buttons
	//##ModelId=49B87BA90205
	BOOL m_bDrawDisabledButtonsInColor; // draw disabled buttons in color
													// (only if m_bUseDrawState = FALSE)

	// public functions to use
	//void Install(CFrameWnd* pFrame);					// connect to main frame
	//##ModelId=49B87BA90206
	void Install(CWnd* pFrame);					// connect to main frame
	//##ModelId=49B87BA90213
	BOOL LoadToolbars(const UINT* arID, int n);	// load multiple toolbars
	//##ModelId=49B87BA90222
	BOOL LoadToolbar(UINT nIDToolbar);						// load one toolbar
	//##ModelId=49B87BA90232
	BOOL AddSingleBitmap(UINT nBitmapID, UINT n, UINT *nID);

	// should never need to call:
	//##ModelId=49B87BA90236
	virtual void Destroy(); // destroys everything--to re-load new toolbars?
	//##ModelId=49B87BA90242
	virtual void Refresh(); // called when system colors, etc change
	//##ModelId=49B87BA90252
	static  HBITMAP GetMFCDotBitmap();	// get..
	//##ModelId=49B87BA90254
	static  void    FixMFCDotBitmap();	// and fix MFC's dot bitmap

	//##ModelId=49B87BA90261
	static BOOL bTRACE;	// Set TRUE to see extra diagnostics in DEBUG code

protected:
//	CFrameWnd*		m_pFrame;		// frame window I belong to
	//##ModelId=49B87BA90272
	CWnd*			m_pFrame;
	//##ModelId=49B87BA90277
	CUIntArray		m_arToolbarID;	// array of toolbar IDs loaded
	//##ModelId=49B87BA90281
	CImageList		m_ilButtons;	// image list for all buttons
	//##ModelId=49B87BA90291
	CMapWordToPtr	m_mapIDtoImage;// maps command ID -> image list index
	//##ModelId=49B87BA90296
	CMapWordToPtr	m_mapIDtoAccel;// maps command ID -> ACCEL*
	//##ModelId=49B87BA9029F
	HACCEL			m_hAccel;		// current accelerators, if any
	//##ModelId=49B87BA902A1
	ACCEL*			m_pAccel;		// ..and table in memory
	//##ModelId=49B87BA902B0
	CPtrList			m_menuList;		// list of HMENU's initialized
	//##ModelId=49B87BA902B4
	CSize				m_szBitmap;		// size of button bitmap
	//##ModelId=49B87BA902BF
	CSize				m_szButton;		// size of button (including shadow)
	//##ModelId=49B87BA902C1
	CFont				m_fontMenu;		// menu font

	// helpers
	//##ModelId=49B87BA902D1
	void DestroyAccel();
	//##ModelId=49B87BA902DE
	void DrawMenuText(CDC& dc, CRect rc, CString text, COLORREF color);
	//##ModelId=49B87BA902F0
	BOOL Draw3DCheckmark(CDC& dc, const CRect& rc, BOOL bSelected,
				HBITMAP hbmCheck=NULL);
	//##ModelId=49B87BA90300
	void ConvertMenu(CMenu* pMenu,UINT nIndex,BOOL bSysMenu,BOOL bShowButtons);
	//##ModelId=49B87BA90310
	void LoadAccel(HACCEL hAccel);
	//##ModelId=49B87BA9031C
	CString GetVirtualKeyName( const CString strVirtKey ) const;
	//##ModelId=49B87BA9031F
	BOOL AppendAccelName(CString& sItemName, UINT nID);
	//##ModelId=49B87BA9032D
	CFont* GetMenuFont();

	// Get button index for given command ID, or -1 if not found
	//##ModelId=49B87BA9033C
	int  GetButtonIndex(WORD nID) {
		void* val;
		return m_mapIDtoImage.Lookup(nID, val) ? (int)val : -1;
	}

	// Get ACCEL structure associated with a given command ID
	//##ModelId=49B87BA9033E
	ACCEL* GetAccel(WORD nID) {
		void* val;
		return m_mapIDtoAccel.Lookup(nID, val) ? (ACCEL*)val : NULL;
	}

	// window proc to hook frame using CSubclassWnd implementation
	//##ModelId=49B87BA90340
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);

	// CSubclassWnd message handlers 
	//##ModelId=49B87BA9035B
	virtual void OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu);
	//##ModelId=49B87BA9036B
	virtual BOOL OnMeasureItem(LPMEASUREITEMSTRUCT lpms);
	//##ModelId=49B87BA9036E
	virtual BOOL OnDrawItem(LPDRAWITEMSTRUCT lpds);
	//##ModelId=49B87BA9037C
	virtual LONG OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	//##ModelId=49B87BA9038C
	virtual void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
};

//////////////////
// Friendly version of MENUITEMINFO initializes itself
//
//##ModelId=3CE242510146
/*struct CMenuItemInfo : public MENUITEMINFO {
	//##ModelId=3CE242510166
	CMenuItemInfo()
	{ memset(this, 0, sizeof(MENUITEMINFO));
	  cbSize = sizeof(MENUITEMINFO);
	}
};*/

#endif // __COOLMENU_H__
