// =============================================================================
// 							Class Implementation : COXSplashWnd
// =============================================================================
//

// Product Version: 7.52

// Copyright ?Dundas Software Ltd. 1997 - 2002, All Rights Reserved                      

// //////////////////////////////////////////////////////////////////////////

#include "OXSplashWnd.h"

#ifdef OX_SPLASHWND_JPEG
#include "OXDIB.h"
#endif
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// helper class COXSplashWndDIB

// Data members -------------------------------------------------------------
// protected:
	// DDB members:

	// CBitmap* m_pBitmap;
	// --- The loaded DDB bitmap 

	// CPalette* m_pPalette;		
	// --- The palette to use with the bitmap

	// DIB members:

	// BITMAPINFOHEADER m_bmih;	
	// --- The original bitmap header
	// CByteArray m_bits;			
	// --- The original bitmap bits
	// DWORD m_nWidthBits;			
	// --- Bits per scan line

	// The following members are used for quick access

	// int m_xMax;
	// --- Maximum x value in bitmap (zero-based)

	// int m_yMax; 
	// --- Maximum y value in bitmap (zero-based)

	// int m_nBitCount;
	// --- Bitmap bitcount

	// int m_nRightShift;
	// --- 8 - m_nBitCount

	// BOOL m_bTopdown;
	// --- Whether the bits are organized top-down or not

	// BYTE* m_pBits0;
	// ---- Pointer to the actual bits
// private:

// Member functions ---------------------------------------------------------
// public:
//##ModelId=49B87B9601C5
COXSplashWndDIB::COXSplashWndDIB()
	:
	m_pBitmap(NULL),
	m_pPalette(NULL),
	m_nWidthBits(0),
	m_xMax(0),
	m_yMax(0),
	m_nBitCount(0),
	m_nRightShift(0),
	m_bTopdown(FALSE),
	m_pBits0(NULL)
	{
	::ZeroMemory(&m_bmih, sizeof(m_bmih));
	}

//##ModelId=49B87B9601C6
COXSplashWndDIB::~COXSplashWndDIB()
	{
	delete m_pBitmap;
	delete m_pPalette;
	}

//##ModelId=49B87B9601C8
CBitmap* COXSplashWndDIB::GetBitmap()
	{
	return m_pBitmap;
	}

//##ModelId=49B87B9601C9
BOOL COXSplashWndDIB::GetBitmapInfo(BITMAP& bm)
	{
	if (m_pBitmap == NULL) 
		return FALSE;

	m_pBitmap->GetBitmap(&bm);
	return TRUE;
	}

//##ModelId=49B87B9601D4
BOOL COXSplashWndDIB::LoadResource(LPCTSTR lpszResourceName, BOOL bPrepareGetPixel)
{
	BOOL bResult = FALSE;

	HINSTANCE hInstance = NULL;
    HRSRC hRsrc = NULL;
    HGLOBAL hGlobal = NULL;
    HBITMAP hBitmapFinal = NULL;
	HPALETTE hPal = NULL;
    LPBITMAPINFOHEADER lpbi = NULL;
    HDC hdc = NULL;
    int iNumColors = 0;

	if (m_pBitmap != NULL)	
	{ 
		delete m_pBitmap;	 
		m_pBitmap  = NULL;
	}
	if (m_pPalette != NULL)	
	{ 
		delete m_pPalette; 
		m_pPalette = NULL;
	}

	hInstance = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
	if (hInstance != NULL)
		hRsrc = ::FindResource(hInstance, lpszResourceName, RT_BITMAP);
	if (hRsrc != NULL)
		hGlobal = ::LoadResource(hInstance, hRsrc);

	if (hGlobal != NULL)
	{
		lpbi = (LPBITMAPINFOHEADER)::LockResource(hGlobal);
		if (lpbi != NULL)
		{
			hdc = ::GetDC(NULL);
			if (hdc != NULL)
			{
				hPal = CreateDIBPalette ((LPBITMAPINFO)lpbi, iNumColors);
				if ((hPal == NULL) && (::GetDeviceCaps(hdc, SIZEPALETTE) == 256))
				{
					// .... Trying to show bitmap without palette on 256-color display
					//      so let's use halftone palette (better than nothing)
					hPal = ::CreateHalftonePalette(hdc);
				}
				if (hPal != NULL)
				{
					::SelectPalette(hdc, hPal, FALSE);
					::RealizePalette(hdc);
				}
		
				hBitmapFinal = ::CreateDIBitmap(hdc, (LPBITMAPINFOHEADER)lpbi,
					(LONG)CBM_INIT,	(LPSTR)lpbi + lpbi->biSize + 
					iNumColors * sizeof(RGBQUAD), (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
				if (hBitmapFinal != NULL)
				{
					m_pBitmap = new CBitmap;
					m_pBitmap->Attach(hBitmapFinal);
					if (hPal != NULL)
					{
						m_pPalette = new CPalette;
						m_pPalette->Attach(hPal);
					}
					if (bPrepareGetPixel)
						PrepareGetPixel(*lpbi, iNumColors);
					bResult = TRUE;
				}
				else
				{
					if(hPal!=NULL)
						::DeleteObject((HGDIOBJ)hPal);
				}

				::ReleaseDC(NULL,hdc);
			}
			::UnlockResource(hGlobal);
		}
		::FreeResource(hGlobal);
	}

    return bResult;
} 

#ifdef OX_SPLASHWND_JPEG
//##ModelId=49B87B9601E4
BOOL COXSplashWndDIB::LoadJPEGFile(LPCTSTR lpszFileName, BOOL bPrepareGetPixel)
{
	BOOL bResult = FALSE;

    HBITMAP hBitmapFinal = NULL;
	HPALETTE hPal = NULL;
    LPBITMAPINFOHEADER lpbi = NULL;
    HDC hdc = NULL;
    int iNumColors = 0;

	COXDIB dib;
	if(!dib.ReadJPEG(lpszFileName))
		return FALSE;
	CMemFile memFile;
	if (!dib.Write(&memFile,FALSE))
	{
		memFile.Close();
		return FALSE;
	}

	if (m_pBitmap != NULL)	
	{ 
		delete m_pBitmap;	 
		m_pBitmap  = NULL;
	}
	if (m_pPalette != NULL)	
	{ 
		delete m_pPalette; 
		m_pPalette = NULL;
	}

	if(memFile.GetLength()>0)
	{
		BYTE* pBuffer=(BYTE*)memFile.Detach();
		if(pBuffer!=NULL)
		{
			pBuffer=pBuffer+14;
			lpbi=(LPBITMAPINFOHEADER)pBuffer;
		
			hdc=::GetDC(NULL);
			if(hdc!=NULL)
			{
				hPal=CreateDIBPalette((LPBITMAPINFO)lpbi,iNumColors);
				if((hPal==NULL) && (::GetDeviceCaps(hdc,SIZEPALETTE)==256))
				{
					// .... Trying to show bitmap without palette on 256-color display
					//      so let's use halftone palette (better than nothing)
					hPal=::CreateHalftonePalette(hdc);
				}
				if(hPal!=NULL)
				{
					SelectPalette(hdc,hPal,FALSE);
					RealizePalette(hdc);
				}
		
				hBitmapFinal=::CreateDIBitmap(hdc,(LPBITMAPINFOHEADER)lpbi,
					(LONG)CBM_INIT,(LPSTR)lpbi+lpbi->biSize+
					iNumColors * sizeof(RGBQUAD),(LPBITMAPINFO)lpbi,DIB_RGB_COLORS);
				if(hBitmapFinal!=NULL)
				{
					m_pBitmap=new CBitmap;
					m_pBitmap->Attach(hBitmapFinal);
					if(hPal!=NULL)
					{
						m_pPalette=new CPalette;
						m_pPalette->Attach(hPal);
					}
					if (bPrepareGetPixel)
						PrepareGetPixel(*lpbi, iNumColors);
					bResult=TRUE;
				}
				else
				{
					if(hPal!=NULL)
						::DeleteObject((HGDIOBJ)hPal);
				}

				::ReleaseDC(NULL,hdc);
			}
			::free(pBuffer-14);
		}
	}

    return bResult;


}
#endif
//##ModelId=49B87B9601D7
BOOL COXSplashWndDIB::LoadFile(LPCTSTR lpszFileName, BOOL bPrepareGetPixel)
{
	BOOL bResult = FALSE;

    HBITMAP hBitmapFinal = NULL;
	HPALETTE hPal = NULL;
    LPBITMAPINFOHEADER lpbi = NULL;
    HDC hdc = NULL;
    int iNumColors = 0;

	CFile file;
	CMemFile memFile;
	CFileException fileException;
	if(!file.Open(lpszFileName,CFile::modeRead|CFile::shareDenyWrite,&fileException))
	{
		TRACE(_T("COXSplashWndDIB::LoadFile : Could not open DIB file : %s\n"), 
			lpszFileName);
		return FALSE;
	}
	if(!CopyFile(&memFile,&file))
	{
		TRACE(_T("COXSplashWndDIB::LoadFile : Could not copy DIB file : %s\n"), 
			lpszFileName);
		return FALSE;
	}
	file.Close();

	if (m_pBitmap != NULL)	
	{ 
		delete m_pBitmap;	 
		m_pBitmap  = NULL;
	}
	if (m_pPalette != NULL)	
	{ 
		delete m_pPalette; 
		m_pPalette = NULL;
	}

	if(memFile.GetLength()>0)
	{
		BYTE* pBuffer=(BYTE*)memFile.Detach();
		if(pBuffer!=NULL)
		{
			pBuffer=pBuffer+14;
			lpbi=(LPBITMAPINFOHEADER)pBuffer;
		
			hdc=::GetDC(NULL);
			if(hdc!=NULL)
			{
				hPal=CreateDIBPalette((LPBITMAPINFO)lpbi,iNumColors);
				if((hPal==NULL) && (::GetDeviceCaps(hdc,SIZEPALETTE)==256))
				{
					// .... Trying to show bitmap without palette on 256-color display
					//      so let's use halftone palette (better than nothing)
					hPal=::CreateHalftonePalette(hdc);
				}
				if(hPal!=NULL)
				{
					SelectPalette(hdc,hPal,FALSE);
					RealizePalette(hdc);
				}
		
				hBitmapFinal=::CreateDIBitmap(hdc,(LPBITMAPINFOHEADER)lpbi,
					(LONG)CBM_INIT,(LPSTR)lpbi+lpbi->biSize+
					iNumColors * sizeof(RGBQUAD),(LPBITMAPINFO)lpbi,DIB_RGB_COLORS);
				if(hBitmapFinal!=NULL)
				{
					m_pBitmap=new CBitmap;
					m_pBitmap->Attach(hBitmapFinal);
					if(hPal!=NULL)
					{
						m_pPalette=new CPalette;
						m_pPalette->Attach(hPal);
					}
					if (bPrepareGetPixel)
						PrepareGetPixel(*lpbi, iNumColors);
					bResult=TRUE;
				}
				else
				{
					if(hPal!=NULL)
						::DeleteObject((HGDIOBJ)hPal);
				}

				::ReleaseDC(NULL,hdc);
			}
			::free(pBuffer-14);
		}
	}

    return bResult;
} 


//##ModelId=49B87B9601F7
HPALETTE COXSplashWndDIB::CreateDIBPalette (LPBITMAPINFO lpbmi, int& nNumColors)
	{ 
	LPBITMAPINFOHEADER  lpbi = NULL;
	LPLOGPALETTE     lpPal = NULL;
	HANDLE           hLogPal = NULL;   
	HPALETTE         hPal = NULL;   
	int              i = 0;   

	// ... Initialize return variable
	nNumColors = 0;
   
	lpbi = (LPBITMAPINFOHEADER)lpbmi;   
	if (lpbi->biBitCount <= 8)
		nNumColors = (1 << lpbi->biBitCount);   
	else   
		nNumColors = 0;  // No palette needed for 24 BPP DIB   
   
	if (lpbi->biClrUsed > 0)   
		nNumColors = lpbi->biClrUsed;  // Use biClrUsed
   
	if (nNumColors != 0)
		{
		hLogPal = ::GlobalAlloc (GHND, sizeof (LOGPALETTE) +      
								  sizeof (PALETTEENTRY) * nNumColors);   
		lpPal = (LPLOGPALETTE) ::GlobalLock (hLogPal);      
		lpPal->palVersion    = (WORD)0x300;      
		lpPal->palNumEntries = (WORD)nNumColors;      
   
		for (i = 0;  i < nNumColors;  i++)      
			{
			lpPal->palPalEntry[i].peRed   = lpbmi->bmiColors[i].rgbRed;   	  
			lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen; 	    
			lpPal->palPalEntry[i].peBlue  = lpbmi->bmiColors[i].rgbBlue;  	   
			lpPal->palPalEntry[i].peFlags = 0;   	  
			}   	  
		hPal = ::CreatePalette (lpPal);      
		::GlobalUnlock (hLogPal);      
		::GlobalFree   (hLogPal);      
		}
	return hPal;   
	} 

//##ModelId=49B87B9601E7
BOOL COXSplashWndDIB::Draw(CDC* pDC)
{
	if (m_pBitmap == NULL)
		return FALSE;

	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(pDC))
		return FALSE;

	if (m_pPalette != NULL)
	{
		dcImage.SelectPalette(m_pPalette, FALSE);
		dcImage.RealizePalette();
		pDC->SelectPalette(m_pPalette, FALSE);
		pDC->RealizePalette();
	}

	BITMAP bm;
	m_pBitmap->GetBitmap(&bm);
	CBitmap* pOldBitmap = dcImage.SelectObject(m_pBitmap);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);

	return TRUE;
}

//##ModelId=49B87B9601E9
COLORREF COXSplashWndDIB::GetPixel(POINT pt) 
{ 
	return GetPixel(pt.x, pt.y);
}

//##ModelId=49B87B9601F4
COLORREF COXSplashWndDIB::GetPixel(int x, int y)
{
	UINT nBitStart = 0;
	BYTE* pPixel = NULL;

	if (x < 0 || m_xMax < x || y < 0 || m_yMax < y)
		return CLR_INVALID;

	// retrieve bit data
	nBitStart = m_nBitCount * x + m_nWidthBits * (m_bTopdown ? y : (m_yMax - y));
	pPixel = m_pBits0 + (nBitStart >> 3);

	if (m_nBitCount == 24)
		return RGB(*(pPixel+2), *(pPixel+1), *pPixel);

	ASSERT(m_pPalette && (m_nBitCount == 1 || 
						  m_nBitCount == 4 || 
						  m_nBitCount == 8));
	BYTE nIndex = 0;
	nIndex = *pPixel;
	if (m_nBitCount < 8)
	{
		nIndex <<= (nBitStart & 7);
		nIndex >>= m_nRightShift;
	}

	PALETTEENTRY pe;
	if (m_pPalette->GetPaletteEntries(nIndex, 1, &pe))
		return RGB(pe.peRed, pe.peGreen, pe.peBlue);

	return CLR_INVALID;
}

// protected:
//##ModelId=49B87B9601FB
void COXSplashWndDIB::PrepareGetPixel(BITMAPINFOHEADER& bmih, int iNumColors)
	// --- In      : bmih: The DIB
	//				 nNumColors : Number of colors in palette
	// --- Out     : 
	// --- Returns : 
	// --- Effect  : Prepares some internal variables for future GetPixel() calls.
{
	// ... others are not supported
	ASSERT(bmih.biCompression == BI_RGB);

	::CopyMemory(&m_bmih, &bmih, bmih.biSize);

	// DWROD aligned
	DWORD nWidthDWORD = (bmih.biWidth * bmih.biBitCount + 31) >> 5;
	m_nWidthBits = nWidthDWORD << 5;

	DWORD nSizeImage = (nWidthDWORD << 2) * abs(bmih.biHeight);
	m_bits.SetSize(nSizeImage);
	::CopyMemory(m_bits.GetData(), 
		(BYTE*)&bmih + bmih.biSize + iNumColors * sizeof(RGBQUAD), nSizeImage);

	// fill up cache values for GetPixel()
	m_xMax = m_bmih.biWidth - 1;
	m_yMax = abs(m_bmih.biHeight) - 1;
	m_bTopdown = (m_bmih.biHeight < 0);
	m_pBits0 = m_bits.GetData();
	m_nBitCount = m_bmih.biBitCount;
	m_nRightShift = 8 - m_bmih.biBitCount;
}


//##ModelId=49B87B960205
BOOL COXSplashWndDIB::CopyFile(CFile* pDestFile, CFile* pSourceFile) 
{
	ASSERT(pDestFile!=NULL);
	ASSERT(pSourceFile!=NULL);

	// use buffers
	const int nBufferLength = 1024;
	BYTE pBuffer[nBufferLength + 1];
	int nLengthRead(0);
	BOOL bSuccess = TRUE;

	TRY
	{
		do
		{
			nLengthRead = pSourceFile->Read(pBuffer, nBufferLength);
			pDestFile->Write(pBuffer, nLengthRead);
		}
		while (nLengthRead == nBufferLength);	// So while not EOF
	}
	CATCH(CFileException, pxFile)
	{
		TRACE(_T("COXSplashWndDIB::CopyFile : Catching FileException (%XH)\n"),
			pxFile->m_cause);
		bSuccess = FALSE;
	}
	END_CATCH      

	return bSuccess;
}

/////////////////////////////////////////////////////////////////////////////
