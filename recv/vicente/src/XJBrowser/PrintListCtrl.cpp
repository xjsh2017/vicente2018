// ==========================================================================
// PrintListCtrl.cpp
// 
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 29/03/2000
// by            : MMA
//
// Modification done :
//
// - Comment from Chris (2000/01/27) corrected.
// - Comment from Danijel Ilisin (2000/01/28) corrected.
// - Comment from Alan Gamboa (2000/03/01) corrected.
// - Adding Print Preview (2000/03/29) from Jeff Rucker
// ==========================================================================

// ==========================================================================
// Les Includes
// ==========================================================================

#include "stdafx.h"
#include <commctrl.h>
#include <commdlg.h>
#include <malloc.h>
#include "PrintListCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// ==========================================================================
// Les Defines
// ==========================================================================

#define HEIGHT_SEPARATOR1   20
#define HEIGHT_SEPARATOR2   10
#define HEIGHT_SEPARATOR3    6
#define HEIGHT_SEPARATOR4    2
#define HEIGHT_SEPARATOR5   10

/////////////////////////////////////////////////////////////////////////////
// class CPrintListCtrl

//##ModelId=3CE2422300E5
CPrintListCtrl::CPrintListCtrl()
 {
  m_hListCtrl                  = NULL;
  m_hPrintDC                   = NULL;
  m_bResizeColumnsToFitContent = FALSE;
  m_bPrintHead                 = TRUE;
  m_bPrintFoot                 = TRUE;
  m_bTruncateColumn            = TRUE;
  m_bPrintColumnSeparator      = TRUE;
  m_bPrintLineSeparator        = FALSE;
  m_bPrintHeadBkgnd            = FALSE;
  m_bPrintFootBkgnd            = FALSE;
  m_bHeaderLine1               = FALSE;
  m_bHeaderLine2               = FALSE;
  m_iScreenFtHead              = 10;
  m_iScreenFtFoot              = 10;
  m_iScreenFtList              = 10;
  m_uiHeaderLine1Format        = DT_RIGHT;
  m_uiHeaderLine2Format        = DT_RIGHT;
  m_clrHeadBkgnd               = RGB(255,255,255);
  m_clrFootBkgnd               = RGB(255,255,255);
  m_hDevMode                   = NULL;

  strcpy(m_szFtHead, "Arial");
  strcpy(m_szFtFoot, "Arial");
  strcpy(m_szFtList, "Arial");

  memset(m_szHeaderLine1, 0, sizeof(m_szHeaderLine1));
  memset(m_szHeaderLine2, 0, sizeof(m_szHeaderLine2));

  CreateDevModeObject();
 }

// --------------------------------------------------------------------------

//##ModelId=3CE2422300F4
CPrintListCtrl::~CPrintListCtrl()
 {
  ClearPrintDC();
  DeleteDevModeObject();
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242220356
void CPrintListCtrl::ClearPrintDC()
 {
  if (m_hPrintDC) DeleteDC(m_hPrintDC);

  m_hPrintDC = NULL;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242220365
void CPrintListCtrl::CreateDevModeObject()
 {
  DeleteDevModeObject();

  m_hDevMode = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof(DEVMODE));
  
  DEVMODE *pstDevMode = (DEVMODE *)GlobalLock(m_hDevMode);

  pstDevMode->dmSize = sizeof(DEVMODE);

  GlobalUnlock(m_hDevMode);
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242220375
void CPrintListCtrl::DeleteDevModeObject()
 {
  if (m_hDevMode) GlobalFree(m_hDevMode);

  m_hDevMode = NULL;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242220385
int CPrintListCtrl::GetFootHeight(HFONT hFootFont)
 {
  LOGFONT stLF;

  GetObject(hFootFont, sizeof(stLF), &stLF);

  return stLF.lfHeight + (2 * HEIGHT_SEPARATOR4);
 }

// --------------------------------------------------------------------------

//##ModelId=3CE2422203A4
int CPrintListCtrl::GetLengthyStringFromColumn(int iCol)
 {
  if (!m_hListCtrl) return FALSE;

  HWND hHeaderCtrl = (HWND)SendMessage(m_hListCtrl, LVM_GETHEADER, 0L, 0L);

  if (!hHeaderCtrl) return FALSE;

  int iColCount  = (int)SendMessage(hHeaderCtrl, HDM_GETITEMCOUNT, 0L, 0L);
  int iLineCount = (int)SendMessage(m_hListCtrl, LVM_GETITEMCOUNT, 0L, 0L);
  int iMax       = 0;

  if (iCol < 0 || iCol >= iColCount) return -1;

  HDITEM stHDI;
  char   szBuf[255];

  stHDI.mask       = HDI_TEXT;
  stHDI.pszText    = szBuf;
  stHDI.cchTextMax = sizeof(szBuf);

  if (SendMessage(hHeaderCtrl, HDM_GETITEM, (WPARAM)(int)iCol, (LPARAM)(LPHDITEM)&stHDI))
   {
    if ((int)strlen(szBuf) > iMax) iMax = strlen(szBuf);
   }

  for (int L=0; L<iLineCount; L++)
   {
    LVITEM stLVI;

    stLVI.iSubItem   = iCol;
    stLVI.pszText    = szBuf;
    stLVI.cchTextMax = sizeof(szBuf);

    if (SendMessage(m_hListCtrl, LVM_GETITEMTEXT, (WPARAM)(int)L, (LPARAM)(LPLVITEM)&stLVI))
     {
      if ((int)strlen(szBuf) > iMax) iMax = strlen(szBuf);
     }
   }

  return iMax;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE2422203B3
int CPrintListCtrl::GetTextSize(LPCTSTR lpszText, SIZE *pstSize)
 {
  int  iWidth;
  SIZE stSize;

  GetTextExtentPoint32(m_hPrintDC, lpszText, strlen(lpszText), &stSize);

  iWidth = stSize.cx;
  
  if (!pstSize) return iWidth;

  memcpy(pstSize, &stSize, sizeof(SIZE));

  return iWidth;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230104
BOOL CPrintListCtrl::Print(HWND hParent, LPCTSTR lpszDocName, BOOL bDefault)
 {
  if (!m_hPrintDC && !PrintSetup(hParent,bDefault)) return FALSE;

  if (!m_hPrintDC) return FALSE;

  if (!m_hListCtrl) return FALSE;

  HWND hHeaderCtrl = (HWND)SendMessage(m_hListCtrl, LVM_GETHEADER, 0L, 0L);

  if (!hHeaderCtrl) return FALSE;

  if (m_bResizeColumnsToFitContent) ResizeColumnsToFitContent();

  int iColCount  = (int)SendMessage(hHeaderCtrl, HDM_GETITEMCOUNT, 0L, 0L);
  int iLineCount = (int)SendMessage(m_hListCtrl, LVM_GETITEMCOUNT, 0L, 0L);

  int   *piColWidthTable  = new int[iColCount];
  int   *piColFormatTable = new int[iColCount];
  char **pszColTitleTable = new char *[iColCount];

  // Get system time
  GetLocalTime(&m_stST);

  // Create all fonts to use for printing
  int iPrinterFtHead = ScreenHeightToPrinterHeight(m_iScreenFtHead);
  int iPrinterFtList = ScreenHeightToPrinterHeight(m_iScreenFtList);
  int iPrinterFtFoot = ScreenHeightToPrinterHeight(m_iScreenFtFoot);

  HFONT hHeadFont = CreateFont(iPrinterFtHead, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, m_szFtHead);
  HFONT hListFont = CreateFont(iPrinterFtList, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, m_szFtList);
  HFONT hFootFont = CreateFont(iPrinterFtFoot, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, m_szFtFoot);

  if (!hHeadFont || !hListFont || !hFootFont)
   {
    if (hHeadFont) DeleteObject(hHeadFont);
    if (hListFont) DeleteObject(hListFont);
    if (hFootFont) DeleteObject(hFootFont);

    return FALSE;
   }

  // Get size of a character in list font
  HFONT hOldFont = (HFONT)SelectObject(m_hPrintDC, hListFont);
  char  szChar[] = "X";
  SIZE  stSize;

  GetTextExtentPoint32(m_hPrintDC, szChar, strlen(szChar), &stSize);

  SelectObject(m_hPrintDC, hOldFont);

  // Get all information about column size, alignment, ...
  for (int C=0; C<iColCount; C++)
   {
    HDITEM stHDI;
    char   szTitle[255];

    memset(szTitle, 0, sizeof(szTitle));

    stHDI.mask       = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
    stHDI.cchTextMax = sizeof(szTitle);
    stHDI.pszText    = szTitle;

    if (!SendMessage(hHeaderCtrl, HDM_GETITEM, (WPARAM)(int)C, (LPARAM)(LPHDITEM)&stHDI)) continue;

    int iMax = GetLengthyStringFromColumn(C) + 1;

    //piColWidthTable[C] = ScreenWidthToPrinterWidth(stHDI.cxy) + 20;

    piColWidthTable[C]  = stHDI.cxy ? stSize.cx * iMax : 0;
    
    if ((stHDI.fmt & HDF_CENTER) == HDF_CENTER) piColFormatTable[C] = DT_CENTER;
    else if ((stHDI.fmt & HDF_RIGHT) == HDF_RIGHT) piColFormatTable[C] = DT_RIGHT;
         else piColFormatTable[C] = DT_LEFT;

    if (stHDI.cchTextMax > 0 && stHDI.pszText)
     {
      pszColTitleTable[C] = new char[stHDI.cchTextMax];

      ASSERT(pszColTitleTable[C]);

      lstrcpy(pszColTitleTable[C], stHDI.pszText);
     }
   }

  // Get page dimension
  int iPageWidth  = GetDeviceCaps(m_hPrintDC, HORZRES);
  int iPageHeight = GetDeviceCaps(m_hPrintDC, VERTRES);

  // Start printing
  DOCINFO stDI;

  stDI.cbSize       = sizeof(stDI);
  stDI.fwType       = 0;
  stDI.lpszDatatype = 0;
  stDI.lpszOutput   = 0;
  stDI.lpszDocName  = lpszDocName;

  StartDoc(m_hPrintDC, &stDI);

  BOOL bNewPage     = TRUE;
  int  iPageCounter = 0;
  int  iOffsetY     = 0;

  for (int L=0; L<iLineCount; L++)
   {
    if (bNewPage)
     {
      StartPage(m_hPrintDC);
      bNewPage = FALSE;
      iOffsetY = 0;
      iPageCounter++;

      iPageWidth  = GetDeviceCaps(m_hPrintDC, HORZRES); // CORRECTION 02/03/2000
      iPageHeight = GetDeviceCaps(m_hPrintDC, VERTRES); // CORRECTION 02/03/2000

      if (m_bPrintHead)
       {
        iOffsetY = PrintHead(iOffsetY, hHeadFont, lpszDocName, iPageCounter);
       }

      iOffsetY = PrintListCtrlHeader(iOffsetY, hListFont, iColCount, piColWidthTable, piColFormatTable, pszColTitleTable);

      if (m_bPrintFoot)
       {
        PrintFoot(iPageHeight - GetFootHeight(hFootFont), hFootFont, iPageCounter);

        iPageHeight -= (GetFootHeight(hFootFont) + HEIGHT_SEPARATOR5);
       }
     }

    iOffsetY = PrintListCtrlLine(iPageWidth, iPageHeight, iOffsetY, hListFont, m_hListCtrl, L, piColWidthTable, piColFormatTable, &bNewPage);

    if (iOffsetY == -1 && bNewPage)
     {
      EndPage(m_hPrintDC);
      L--;
     }
   }

  if (!bNewPage) EndPage(m_hPrintDC);

  EndDoc(m_hPrintDC);

  // Free all allocated memories and objects
  if (hHeadFont) DeleteObject(hHeadFont);
  if (hListFont) DeleteObject(hListFont);
  if (hFootFont) DeleteObject(hFootFont);

  if (pszColTitleTable)
   {
    for (int C=0; C<iColCount; C++)
     if (pszColTitleTable[C]) delete [] pszColTitleTable[C];

    delete [] pszColTitleTable;
   }

  if (piColFormatTable) delete [] piColFormatTable;

  if (piColWidthTable) delete [] piColWidthTable;

  return TRUE;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE2422203D3
void CPrintListCtrl::PrintFoot(int iOffsetY, HFONT hFootFont, int iPageCounter)
 {
  LOGFONT stLF;

  GetObject(hFootFont, sizeof(stLF), &stLF);

  int   iPageWidth = GetDeviceCaps(m_hPrintDC, HORZRES);
  char  szBuf[255];
  RECT  stRect;
  RECT  stFrameRect;
  HFONT hOldFont = (HFONT)SelectObject(m_hPrintDC, hFootFont);
  int   iOldMode = SetBkMode(m_hPrintDC, TRANSPARENT);

  stFrameRect.left   = 0;
  stFrameRect.top    = iOffsetY;
  stFrameRect.right  = iPageWidth - 1;
  stFrameRect.bottom = iOffsetY + stLF.lfHeight + (2 * HEIGHT_SEPARATOR4);

  if (m_bPrintFootBkgnd)
   {
    HBRUSH hBrush    = CreateSolidBrush(m_clrFootBkgnd);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hPrintDC, hBrush);
  
    FillRect(m_hPrintDC, &stFrameRect, hBrush);

    SelectObject(m_hPrintDC, hOldBrush);
   }

  // Print page counter
  stRect.left   = 2;
  stRect.top    = iOffsetY;
  stRect.right  = iPageWidth / 2;
  stRect.bottom = iOffsetY + stLF.lfHeight + (2 * HEIGHT_SEPARATOR4);

  sprintf(szBuf, "Page : %d", iPageCounter);

  DrawText(m_hPrintDC, szBuf, strlen(szBuf), &stRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

  // Print date and time
  stRect.left   = iPageWidth / 2;
  stRect.top    = iOffsetY;
  stRect.right  = iPageWidth - 3;
  stRect.bottom = iOffsetY + stLF.lfHeight + (2 * HEIGHT_SEPARATOR4);

  sprintf(szBuf, "%02d/%02d/%04d  %02d:%02d:%02d", m_stST.wDay,
                                                   m_stST.wMonth,
                                                   m_stST.wYear,
                                                   m_stST.wHour,
                                                   m_stST.wMinute,
                                                   m_stST.wSecond);

  DrawText(m_hPrintDC, szBuf, strlen(szBuf), &stRect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

  // Print frame rectangle
  FrameRect(m_hPrintDC, &stFrameRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

  // Restore objects
  SetBkMode(m_hPrintDC, iOldMode);

  SelectObject(m_hPrintDC, hOldFont);
 }

// --------------------------------------------------------------------------

//##ModelId=3CE24223000A
int CPrintListCtrl::PrintHead(int iOffsetY, HFONT hHeadFont, LPCTSTR lpszTitle, int iPageCounter)
 {
  LOGFONT stLF, stTitleLF;
  int     iPageWidth, iPageHeight;
  HFONT   hTitleFont;

  GetObject(hHeadFont, sizeof(stLF), &stLF);

  memcpy(&stTitleLF, &stLF, sizeof(LOGFONT));

  stTitleLF.lfHeight = stTitleLF.lfHeight * 2;

  hTitleFont = CreateFontIndirect(&stTitleLF);

  iPageWidth  = GetDeviceCaps(m_hPrintDC, HORZRES);
  iPageHeight = GetDeviceCaps(m_hPrintDC, VERTRES);

  RECT  stRect;
  char  szBuf[255];
  HFONT hOldFont;
  int   iTopFrame = iOffsetY;

  int iOldMode = SetBkMode(m_hPrintDC, TRANSPARENT);

  // Print head background if wanted
  RECT stFrameRect;

  stFrameRect.left   = 0;
  stFrameRect.top    = iTopFrame;
  stFrameRect.right  = iPageWidth - 1;
  stFrameRect.bottom = iOffsetY + (2 * stLF.lfHeight) + stTitleLF.lfHeight;

  if (m_bPrintHeadBkgnd)
   {
    HBRUSH hBrush    = CreateSolidBrush(m_clrHeadBkgnd);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hPrintDC, hBrush);
  
    FillRect(m_hPrintDC, &stFrameRect, hBrush);

    SelectObject(m_hPrintDC, hOldBrush);
   }

  // Print page counter
  stRect.left   = 0;
  stRect.top    = iOffsetY;
  stRect.right  = iPageWidth;
  stRect.bottom = iOffsetY + stLF.lfHeight;

  hOldFont = (HFONT)SelectObject(m_hPrintDC, hHeadFont);
  
  if (!m_bPrintFoot)
   {
    sprintf(szBuf, "Page : %d", iPageCounter);
    
    DrawText(m_hPrintDC, szBuf, strlen(szBuf), &stRect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
   }
  else if (m_bHeaderLine1)
        {
         sprintf(szBuf, m_szHeaderLine1);
    
         DrawText(m_hPrintDC, szBuf, strlen(szBuf), &stRect, DT_SINGLELINE | m_uiHeaderLine1Format | DT_VCENTER);
        }
  
  iOffsetY += stLF.lfHeight;

  // Print title
  stRect.left   = 0;
  stRect.top    = iOffsetY;
  stRect.right  = iPageWidth;
  stRect.bottom = iOffsetY + stTitleLF.lfHeight;

  SelectObject(m_hPrintDC, hTitleFont);
  DrawText(m_hPrintDC, lpszTitle, strlen(lpszTitle), &stRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
  iOffsetY += stTitleLF.lfHeight;

  // Print page counter
  stRect.left   = 0;
  stRect.top    = iOffsetY;
  stRect.right  = iPageWidth;
  stRect.bottom = iOffsetY + stLF.lfHeight;

  SelectObject(m_hPrintDC, hHeadFont);
  
  if (!m_bPrintFoot)
   {
    sprintf(szBuf, "%02d/%02d/%04d  %02d:%02d:%02d", m_stST.wDay,
                                                     m_stST.wMonth,
                                                     m_stST.wYear,
                                                     m_stST.wHour,
                                                     m_stST.wMinute,
                                                     m_stST.wSecond);

    DrawText(m_hPrintDC, szBuf, strlen(szBuf), &stRect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
   }
  else if (m_bHeaderLine2)
        {
         sprintf(szBuf, m_szHeaderLine2);
    
         DrawText(m_hPrintDC, szBuf, strlen(szBuf), &stRect, DT_SINGLELINE | m_uiHeaderLine2Format | DT_VCENTER);
        }
  
  iOffsetY += stLF.lfHeight;

  // Print frame
  FrameRect(m_hPrintDC, &stFrameRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

  // Restore default values
  SetBkMode(m_hPrintDC, iOldMode);

  SelectObject(m_hPrintDC, hOldFont);

  DeleteObject(hTitleFont);

  return (iOffsetY + HEIGHT_SEPARATOR1);
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230029
int CPrintListCtrl::PrintListCtrlHeader(int iOffsetY, HFONT hListFont, int iColCount, int *piColWidthTable, int *piColFormatTable, char **pszColTitleTable)
 {
  LOGFONT stLF;
  RECT    stRect;
  int     iOffsetX    = 0;
  HFONT   hOldFont    = (HFONT)SelectObject(m_hPrintDC, hListFont);
  int     iPageWidth  = GetDeviceCaps(m_hPrintDC, HORZRES);
  int     iPageHeight = GetDeviceCaps(m_hPrintDC, VERTRES);

  GetObject(hListFont, sizeof(stLF), &stLF);

  // Print columns titles
  for (int C=0; C<iColCount; C++)
   {
    if (m_bTruncateColumn && (iOffsetX + piColWidthTable[C]) > iPageWidth) break;

    if (iOffsetX > iPageWidth) break;

    if (!piColWidthTable[C]) continue;

    stRect.left   = iOffsetX;
    stRect.top    = iOffsetY;
    stRect.right  = iOffsetX + piColWidthTable[C] - 1;
    stRect.bottom = iOffsetY + stLF.lfHeight;

    CRect cDrawRect = stRect;

    cDrawRect.DeflateRect(4,0);

    DrawText(m_hPrintDC, pszColTitleTable[C], strlen(pszColTitleTable[C]), cDrawRect, DT_SINGLELINE | DT_VCENTER | piColFormatTable[C]);

    iOffsetX += piColWidthTable[C];

    // Print column separator if wanted
    if (m_bPrintColumnSeparator && C > 0)
     {
      MoveToEx(m_hPrintDC, stRect.left-2, stRect.top, NULL);
      LineTo(m_hPrintDC, stRect.left-2, stRect.bottom + HEIGHT_SEPARATOR2);
     }
   }

  iOffsetY += stLF.lfHeight;

  // Print separator line
  HPEN hPen    = CreatePen(PS_SOLID, 3, RGB(0,0,0));
  HPEN hOldPen = (HPEN)SelectObject(m_hPrintDC, hPen);

  MoveToEx(m_hPrintDC, 0, iOffsetY, NULL);
  LineTo(m_hPrintDC, iPageWidth, iOffsetY);

  // Restore default values
  SelectObject(m_hPrintDC, hOldPen);

  SelectObject(m_hPrintDC, hOldFont);

  return (iOffsetY + HEIGHT_SEPARATOR2);
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230048
int CPrintListCtrl::PrintListCtrlLine(int iPageWidth, int iPageHeight, int iOffsetY, HFONT hListFont, HWND hListCtrl, int iLine, int *piColWidthTable, int *piColFormatTable, BOOL *pbNewPage)
 {
  *pbNewPage = FALSE;

  if (!hListCtrl) return -1;

  HWND hHeaderCtrl = (HWND)SendMessage(hListCtrl, LVM_GETHEADER, 0L, 0L);

  if (!hHeaderCtrl) return -1;

  int iColCount   = (int)SendMessage(hHeaderCtrl, HDM_GETITEMCOUNT, 0L, 0L);
  int iLineCount  = (int)SendMessage(hListCtrl, LVM_GETITEMCOUNT, 0L, 0L);

  LOGFONT stLF;

  GetObject(hListFont, sizeof(stLF), &stLF);

  if ((iOffsetY + stLF.lfHeight) > iPageHeight)
   {
    *pbNewPage = TRUE;

    return -1;
   }

  int   iOffsetX = 0;
  HFONT hOldFont = (HFONT)SelectObject(m_hPrintDC, hListFont);

  for (int C=0; C<iColCount; C++)
   {
    if (m_bTruncateColumn && (iOffsetX + piColWidthTable[C]) > iPageWidth) break;
    
    if (iOffsetX > iPageWidth) break;

    if (!piColWidthTable[C]) continue;

    LVITEM stLVI;
    char   szBuf[255];
    RECT   stRect;

    memset(szBuf, 0, sizeof(szBuf));

    stLVI.iSubItem   = C;
    stLVI.pszText    = szBuf;
    stLVI.cchTextMax = sizeof(szBuf);

    SendMessage(hListCtrl, LVM_GETITEMTEXT, (WPARAM)(int)iLine, (LPARAM)(LPLVITEM)&stLVI);

    stRect.left   = iOffsetX;
    stRect.top    = iOffsetY;
    stRect.right  = iOffsetX + piColWidthTable[C];
    stRect.bottom = iOffsetY + stLF.lfHeight;

    //int iWidth = GetTextSize(szBuf);
    
    //if (iWidth > (stRect.right - stRect.left)) strcpy(szBuf,"...");

    CRect cDrawRect = stRect;

    cDrawRect.DeflateRect(4,0);

    DrawText(m_hPrintDC, szBuf, strlen(szBuf), cDrawRect, DT_SINGLELINE | DT_VCENTER | piColFormatTable[C]);

    iOffsetX += piColWidthTable[C];

    // Print column separator if wanted
    if (m_bPrintColumnSeparator && C > 0)
     {
      MoveToEx(m_hPrintDC, stRect.left-2, stRect.top, NULL);
      LineTo(m_hPrintDC, stRect.left-2, stRect.bottom + HEIGHT_SEPARATOR3);
     }
   }

  iOffsetY += stLF.lfHeight;

  // Print line separator if wanted

  if (m_bPrintLineSeparator)
   {
    MoveToEx(m_hPrintDC, 0, iOffsetY + (int)(HEIGHT_SEPARATOR3 / 2), NULL);
    LineTo(m_hPrintDC, iPageWidth, iOffsetY + (int)(HEIGHT_SEPARATOR3 / 2));
   }

  SelectObject(m_hPrintDC, hOldFont);

  return (iOffsetY + HEIGHT_SEPARATOR3);
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230123
BOOL CPrintListCtrl::PrintSetup(HWND hParent, BOOL bDefault)
 {
  PRINTDLG stPD;

  memset(&stPD, 0, sizeof(stPD));

  stPD.lStructSize = sizeof(stPD);
  stPD.hwndOwner   = hParent;
  stPD.Flags       = PD_ALLPAGES | PD_DISABLEPRINTTOFILE |
                     PD_NOPAGENUMS | PD_NOSELECTION | PD_RETURNDC;
  stPD.hDevNames   =  NULL;
  stPD.hDevMode    = bDefault ? NULL : m_hDevMode;

  if (bDefault)
   {
    stPD.Flags |= PD_RETURNDEFAULT;
   }

  BOOL bRet = PrintDlg(&stPD);

  if (bRet == FALSE && CommDlgExtendedError()) return FALSE;

  if (bRet == FALSE && !CommDlgExtendedError()) return TRUE;

  ClearPrintDC();

  m_hPrintDC = stPD.hDC;

  return (m_hPrintDC != NULL);
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230078
BOOL CPrintListCtrl::ResizeColumnToFitContent(int iCol)
 {
  if (!m_hListCtrl) return FALSE;

  HWND hHeaderCtrl = (HWND)SendMessage(m_hListCtrl, LVM_GETHEADER, 0L, 0L);

  if (!hHeaderCtrl) return FALSE;
  
  int iColCount = (int)SendMessage(hHeaderCtrl, HDM_GETITEMCOUNT, 0L, 0L);

  if (iCol < 0 || iCol >= iColCount) return FALSE;

  return SendMessage(m_hListCtrl, LVM_SETCOLUMNWIDTH, (WPARAM)(int)iCol, MAKELPARAM((int)LVSCW_AUTOSIZE_USEHEADER,0));
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230088
BOOL CPrintListCtrl::ResizeColumnsToFitContent()
 {
  if (!m_hListCtrl) return FALSE;

  HWND hHeaderCtrl = (HWND)SendMessage(m_hListCtrl, LVM_GETHEADER, 0L, 0L);

  if (!hHeaderCtrl) return FALSE;
  
  int iColCount = (int)SendMessage(hHeaderCtrl, HDM_GETITEMCOUNT, 0L, 0L);

  for (int C=0; C<iColCount; C++)
   {
    ResizeColumnToFitContent(C);
   }

  return TRUE;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230097
int CPrintListCtrl::ScreenHeightToPrinterHeight(int iScreenHeight)
 {
  if (!m_hPrintDC) return -1;

  HDC hScreenDC = GetDC(NULL);

  if (!hScreenDC) return -1;

  int iPrinterHeight = (iScreenHeight * GetDeviceCaps(m_hPrintDC,LOGPIXELSY)) / GetDeviceCaps(hScreenDC,LOGPIXELSY);

  ReleaseDC(NULL, hScreenDC);

  return iPrinterHeight;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230099
int CPrintListCtrl::ScreenWidthToPrinterWidth(int iScreenWidth)
 {
  if (!m_hPrintDC) return -1;

  HDC hScreenDC = GetDC(NULL);

  if (!hScreenDC) return -1;

  int iPrinterWidth = (iScreenWidth * GetDeviceCaps(m_hPrintDC,LOGPIXELSX)) / GetDeviceCaps(hScreenDC,LOGPIXELSX);

  ReleaseDC(NULL, hScreenDC);

  return iPrinterWidth;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230134
BOOL CPrintListCtrl::SetDefaultPrinterSettings(HWND hParent)
 {
  ClearPrintDC();

  return PrintSetup(hParent,TRUE);
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230142
void CPrintListCtrl::SetHeaderLine1(char *szLine, UINT uiFormat)
 {
  if (!szLine)
   {
    m_bHeaderLine1 = FALSE;
    memset(m_szHeaderLine1, 0, sizeof(m_szHeaderLine1));
    return;
   }

  m_bHeaderLine1 = TRUE;

  strcpy(m_szHeaderLine1, szLine);

  m_uiHeaderLine1Format = uiFormat;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230152
void CPrintListCtrl::SetHeaderLine2(char *szLine, UINT uiFormat)
 {
  if (!szLine)
   {
    m_bHeaderLine2 = FALSE;
    memset(m_szHeaderLine2, 0, sizeof(m_szHeaderLine2));
    return;
   }

  m_bHeaderLine2 = TRUE;

  strcpy(m_szHeaderLine2, szLine);

  m_uiHeaderLine2Format = uiFormat;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230162
BOOL CPrintListCtrl::SetPrinterOrientation(BOOL bLandscape)
 {
  if (!m_hDevMode) return FALSE;

  DEVMODE *pstDevMode = (DEVMODE *)GlobalLock(m_hDevMode);

  pstDevMode->dmFields |= DM_ORIENTATION;

  pstDevMode->dmOrientation = bLandscape ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT;

  GlobalUnlock(m_hDevMode);

  return TRUE;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230171
BOOL CPrintListCtrl::SetPrinterPaperSize(int iValue)
 {
  if (!m_hDevMode) return FALSE;

  DEVMODE *pstDevMode = (DEVMODE *)GlobalLock(m_hDevMode);

  pstDevMode->dmFields |= DM_PAPERSIZE;

  pstDevMode->dmPaperSize = iValue;

  GlobalUnlock(m_hDevMode);

  return TRUE;
 }

// --------------------------------------------------------------------------
// Functions for print preview
// --------------------------------------------------------------------------

#ifdef _AFXDLL
//##ModelId=3CE242230181
void CPrintListCtrl::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
 {
  m_hPrintDC = pDC->m_hAttribDC;

  // Create all fonts to use for printing
  int iPrinterFtHead = ScreenHeightToPrinterHeight(m_iScreenFtHead);
  int iPrinterFtList = ScreenHeightToPrinterHeight(m_iScreenFtList);
  int iPrinterFtFoot = ScreenHeightToPrinterHeight(m_iScreenFtFoot);

  m_hHeadFont = CreateFont(iPrinterFtHead, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, m_szFtHead);
  m_hListFont = CreateFont(iPrinterFtHead, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, m_szFtList);
  m_hFootFont = CreateFont(iPrinterFtFoot, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, m_szFtFoot);

  // Get size of a character in list font
  HFONT hOldFont = (HFONT)SelectObject(m_hPrintDC, m_hListFont);
  char  szChar[] = "X";
  SIZE  stSize;

  GetTextExtentPoint32(m_hPrintDC, szChar, strlen(szChar), &stSize);

  SelectObject(m_hPrintDC, hOldFont);

  // Get all information about column size, alignment, ...
  for (int C=0; C<m_iColCount; C++)
   {
    HDITEM stHDI;
    char   szTitle[255];

    memset(szTitle, 0, sizeof(szTitle));

    stHDI.mask       = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
    stHDI.cchTextMax = sizeof(szTitle);
    stHDI.pszText    = szTitle;

    HWND hHeaderCtrl = (HWND)SendMessage(m_hListCtrl, LVM_GETHEADER, 0L, 0L);
    if (!SendMessage(hHeaderCtrl, HDM_GETITEM, (WPARAM)(int)C, (LPARAM)(LPHDITEM)&stHDI)) continue;

    int iMax = GetLengthyStringFromColumn(C) + 1;

    //piColWidthTable[C] = ScreenWidthToPrinterWidth(stHDI.cxy) + 20;

    m_piColWidthTable[C]  = stHDI.cxy ? stSize.cx * iMax : 0;
    
    if ((stHDI.fmt & HDF_CENTER) == HDF_CENTER) m_piColFormatTable[C] = DT_CENTER;
    else if ((stHDI.fmt & HDF_RIGHT) == HDF_RIGHT) m_piColFormatTable[C] = DT_RIGHT;
         else m_piColFormatTable[C] = DT_LEFT;

    if (stHDI.cchTextMax > 0 && stHDI.pszText)
     {
      m_pszColTitleTable[C] = new char[stHDI.cchTextMax];

      ASSERT(m_pszColTitleTable[C]);

      lstrcpy(m_pszColTitleTable[C], stHDI.pszText);
     }
   }

  // Calculate the lines per page - First add the Page header + Footer
  LOGFONT stLF;
  
  GetObject(m_hHeadFont, sizeof(stLF), &stLF);
  
  int iOffset      = 0;
  int iTotalHeight = GetDeviceCaps(pDC->m_hAttribDC, VERTRES);

  if (m_bPrintHead)
   {
    iOffset = (stLF.lfHeight * 4) + HEIGHT_SEPARATOR1;
   }

  if (m_bPrintFoot)
   {
    iTotalHeight -= (GetFootHeight(m_hFootFont) + HEIGHT_SEPARATOR5);
   }

  // Add the size of the list control header
  GetObject(m_hListFont, sizeof(stLF), &stLF);

  iOffset += stLF.lfHeight + HEIGHT_SEPARATOR2;

  iTotalHeight -= iOffset;

  int iLineHeight = stLF.lfHeight + HEIGHT_SEPARATOR3;

  m_nLinesPerPage = iTotalHeight/iLineHeight;

  int nMaxPages = (m_iLineCount/m_nLinesPerPage) + 1;

  pInfo->SetMaxPage(nMaxPages);
  pInfo->SetMinPage(1);
 }

// --------------------------------------------------------------------------

//##ModelId=3CE242230191
void CPrintListCtrl::OnEndPrinting(CDC *pDC, CPrintInfo *pInfo)
 {
  // Free all allocated memories and objects
  if (m_hHeadFont) DeleteObject(m_hHeadFont);
  if (m_hListFont) DeleteObject(m_hListFont);
  if (m_hFootFont) DeleteObject(m_hFootFont);

  if (m_pszColTitleTable)
   {
    for (int C=0; C<m_iColCount; C++)
     if (m_pszColTitleTable[C]) delete [] m_pszColTitleTable[C];

    delete [] m_pszColTitleTable;
   }

  if (m_piColFormatTable) delete [] m_piColFormatTable;

  if (m_piColWidthTable) delete [] m_piColWidthTable;

  m_hPrintDC = NULL;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE2422301A0
BOOL CPrintListCtrl::OnPreparePrinting(CPrintInfo *pInfo, CString csDocName)
 {
  m_csDocName = csDocName;

  if (!m_hListCtrl) return FALSE;

  HWND hHeaderCtrl = (HWND)SendMessage(m_hListCtrl, LVM_GETHEADER, 0L, 0L);

  if (!hHeaderCtrl) return FALSE;

  if (m_bResizeColumnsToFitContent) ResizeColumnsToFitContent();

  m_iColCount  = (int)SendMessage(hHeaderCtrl, HDM_GETITEMCOUNT, 0L, 0L);
  m_iLineCount = (int)SendMessage(m_hListCtrl, LVM_GETITEMCOUNT, 0L, 0L);

  m_piColWidthTable  = new int[m_iColCount];
  m_piColFormatTable = new int[m_iColCount];
  m_pszColTitleTable = new char *[m_iColCount];

  // Get system time
  GetLocalTime(&m_stST);

  // We don't use standard footer
  pInfo->m_strPageDesc.Empty();
 
  return TRUE;
 }

// --------------------------------------------------------------------------

//##ModelId=3CE2422301A3
void CPrintListCtrl::OnPrint(CDC *pDC, CPrintInfo *pInfo)
 {
  BOOL bNewPage  = TRUE;
  int  iOffsetY  = 0;
  int iStartLine = 0;
  int iEndLine   = 0;

  // Get page dimension
  m_hPrintDC    = pDC->m_hDC;
  m_iPageWidth  = GetDeviceCaps(pDC->m_hAttribDC, HORZRES);
  m_iPageHeight = GetDeviceCaps(pDC->m_hAttribDC, VERTRES);

  if (!m_hHeadFont || !m_hListFont || !m_hFootFont)
   {
    if (m_hHeadFont) DeleteObject(m_hHeadFont);
    if (m_hListFont) DeleteObject(m_hListFont);
    if (m_hFootFont) DeleteObject(m_hFootFont);
    pInfo->m_bContinuePrinting = FALSE;
    // I could not figure out how to stop the print preview if the font allocation failed
    // But, it never seems to happen, so I doubt the user can get this
    ASSERT(0);
    return ;
   }
  
  iStartLine = m_nLinesPerPage * (pInfo->m_nCurPage - 1);
  iEndLine   = iStartLine + m_nLinesPerPage;

  if (iEndLine>m_iLineCount) iEndLine = m_iLineCount;

  int iPageHeight = 0;

  iOffsetY = 0;
      
  if (m_bPrintHead)
   {
    iOffsetY = PrintHead(iOffsetY, m_hHeadFont, m_csDocName, pInfo->m_nCurPage);
   }

  iOffsetY = PrintListCtrlHeader(iOffsetY, m_hListFont, m_iColCount, m_piColWidthTable, m_piColFormatTable, m_pszColTitleTable);

  if (m_bPrintFoot)
   {
    PrintFoot(m_iPageHeight - GetFootHeight(m_hFootFont), m_hFootFont, pInfo->m_nCurPage);
 
    iPageHeight = m_iPageHeight - (GetFootHeight(m_hFootFont) + HEIGHT_SEPARATOR5);
   }
 
  for (int L=iStartLine; L<iEndLine; L++)
   {
    iOffsetY = PrintListCtrlLine(m_iPageWidth, iPageHeight, iOffsetY, m_hListFont, m_hListCtrl, L, m_piColWidthTable, m_piColFormatTable, &bNewPage);
 
    if (iOffsetY==-1) break;
   }
 }
#endif

// ==========================================================================
// ==========================================================================
