// ==========================================================================
// 					Class Specification : COXSplashWndDIB
// ==========================================================================
// Header file : COXSplashWndDib.h
// ==========================================================================
// Product Version: 7.52
//
// Properties:
//		NO	Abstract class (does not have any objects)
//		NO	Derived from CObject
//
//		NO	Is a CWnd.                     
//		NO	Two stage creation (constructor & Create())
//		NO	Has a message map
//		NO	Needs a resource (template)
//
//		NO	Persistent objects (saveable on disk)      
//		NO	Uses exceptions
//
// Description:
//
//		This is a helper class for the splash window
//		It contains the DIB and its related functions
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __OXSPLASHWNDDIB_H__
#define __OXSPLASHWNDDIB_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef CLR_NONE
#define CLR_NONE 0xFFFFFFFFL
#endif // CLR_NONE

#ifndef CLR_DEFAULT
#define CLR_DEFAULT 0xFF000000L
#endif // CLR_DEFAULT


/////////////////////////////////////////////////////////////////////////////
// helper class COXSplashWndDIB
//##ModelId=49B87B960186
class OX_CLASS_DECL COXSplashWndDIB
{
// Data members -------------------------------------------------------------
public:
protected:
	// DDB members:
	//##ModelId=49B87B960188
	CBitmap* m_pBitmap;
	//##ModelId=49B87B96018D
	CPalette* m_pPalette;

	// DIB members:
	//##ModelId=49B87B960197
	BITMAPINFOHEADER m_bmih;
	//##ModelId=49B87B96019C
	CByteArray m_bits;
	//##ModelId=49B87B9601A0
	DWORD m_nWidthBits;

	// GetPixel() cache
	//##ModelId=49B87B9601A5
	int m_xMax;
	//##ModelId=49B87B9601A6
	int m_yMax; 
	//##ModelId=49B87B9601A7
	int m_nBitCount;
	//##ModelId=49B87B9601B5
	int m_nRightShift;
	//##ModelId=49B87B9601B6
	BOOL m_bTopdown;
	//##ModelId=49B87B9601B7
	BYTE* m_pBits0;

private:

// Member functions ---------------------------------------------------------
public:
	//##ModelId=49B87B9601C5
	COXSplashWndDIB();
	// --- In      : 
	// --- Out     :
	// --- Returns : 
	// --- Effect  : Constructor of the object

	//##ModelId=49B87B9601C6
	virtual ~COXSplashWndDIB();
	// --- In      : 
	// --- Out     :
	// --- Returns : 
	// --- Effect  : Destructor of the object

	//##ModelId=49B87B9601C8
	CBitmap* GetBitmap();
	// --- In      : 
	// --- Out     :
	// --- Returns : The DDB bitmap of thei object
	// --- Effect  : 

	//##ModelId=49B87B9601C9
	BOOL GetBitmapInfo(BITMAP& bm);
	// --- In      : 
	// --- Out     : bm
	// --- Returns : Whether it succeeded or not
	// --- Effect  : Gets info from this bitmap object

	//##ModelId=49B87B9601D4
	BOOL LoadResource(LPCTSTR lpszResourceName, BOOL bPrepareGetPixel);
	// --- In      : lpszResourceName, specifies the bitmap resource
	//				 bPrepareGetPixel, specifies whether a future GetPixel() will
	//					be called; if TRUE, some data members will be initialized
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE otherwise.
	// --- Effect  : load and create a DDB from DIB/DDB in resource

	//##ModelId=49B87B9601D7
	BOOL LoadFile(LPCTSTR lpszFileName, BOOL bPrepareGetPixel);
	// --- In      : lpszFileName, specifies the bitmap file
	//				 bPrepareGetPixel, specifies whether a future GetPixel() will
	//					be called; if TRUE, some data members will be initialized
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE otherwise.
	// --- Effect  : load and create a DDB from DIB/DDB file

#ifdef OX_SPLASHWND_JPEG
	//##ModelId=49B87B9601E4
	BOOL LoadJPEGFile(LPCTSTR lpszFileName, BOOL bPrepareGetPixel);
	// --- In      : lpszFileName, specifies the bitmap file
	//				 bPrepareGetPixel, specifies whether a future GetPixel() will
	//					be called; if TRUE, some data members will be initialized
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE otherwise.
	// --- Effect  : load and create a DDB from DIB/DDB file
#endif
	//##ModelId=49B87B9601E7
	BOOL Draw(CDC* pDC);
	// --- In      : pDC, pointer to the target CDC
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : draw the bitmap in a device context

	//##ModelId=49B87B9601E9
	COLORREF GetPixel(POINT pt);
	// --- In      : pt, the point to retrieve color
	// --- Out     :
	// --- Returns : color value; CLR_INVALID if out of bound
	// --- Effect  : obtain color of a pixel in the bitmap
	//				 NOTE: have to specify TRUE in LoadResource(..., bPrepareGetPixel)

	//##ModelId=49B87B9601F4
	COLORREF GetPixel(int x, int y);
	// --- In      : (x, y), the point to retrieve color
	// --- Out     :
	// --- Returns : color value; CLR_INVALID if out of bound
	// --- Effect  : obtain color of a pixel in the bitmap
	//				 NOTE: have to specify TRUE in LoadResource(..., bPrepareGetPixel)

	//##ModelId=49B87B9601F7
	static HPALETTE CreateDIBPalette (LPBITMAPINFO lpbmi, int& nNumColors);
	// --- In      : lpbmi: Pointer to the DIB
	// --- Out     : nNumColors : Number of colors in palette
	// --- Returns : A palette created from the DIB bitmap
	// --- Effect  : 

protected:
	//##ModelId=49B87B9601FB
	void PrepareGetPixel(BITMAPINFOHEADER& bmih, int iNumColors);
	//##ModelId=49B87B960205
	BOOL CopyFile(CFile* pDestFile, CFile* pSourceFile); 

private:
	friend class COXSplashWnd;
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OXSPLASHWNDDIB_H__

// end of OXSplashWndDIB.h
