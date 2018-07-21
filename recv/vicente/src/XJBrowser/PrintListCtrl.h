// ==========================================================================
// PrintListCtrl.h
// 
// Author : Marquet Mike
//          mike.marquet@altavista.net
//
// Last Modified : 29/03/2000
// by            : MMA
// ==========================================================================

#if !defined(AFX_PRINTLISTCTRL_H__D5C2BEC1_C744_11D3_A0E2_004005555C30__INCLUDED_)
#define AFX_PRINTLISTCTRL_H__D5C2BEC1_C744_11D3_A0E2_004005555C30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* ==========================================================================

                          PRINT PREVIEW SETTINGS


   Add this in the message map off a CView derived class :

     ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

   Add these functions to the derived CView class :

     void CXListView::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
      {
       m_cPrintListCtrl.OnBeginPrinting(pDC, pInfo);
       CListView::OnBeginPrinting(pDC, pInfo);
      }

     void CXListView::OnEndPrinting(CDC *pDC, CPrintInfo *pInfo)
      {
       m_cPrintListCtrl.OnEndPrinting(pDC, pInfo);
       CListView::OnEndPrinting(pDC, pInfo);
      }

     BOOL CXListView::OnPreparePrinting(CPrintInfo *pInfo)
      {
       CListCtrl &cList = GetListCtrl();
       HWND       hWnd  = cList.m_hWnd;

       m_cPrintListCtrl.SetListCtrl(hWnd);
       m_cPrintListCtrl.OnPreparePrinting(pInfo, GetDocument()->GetTitle());
   
       return DoPreparePrinting(pInfo);
      }

     void CXListView::OnPrint(CDC *pDC, CPrintInfo *pInfo)
      {
       m_cPrintListCtrl.OnPrint(pDC, pInfo);
      }

========================================================================== */

/////////////////////////////////////////////////////////////////////////////
// class CPrintListCtrl

//##ModelId=3CE242220181
class CPrintListCtrl  
{
  protected :
	//##ModelId=3CE242220191
              HWND        m_hListCtrl;
	//##ModelId=3CE242220192
              HDC         m_hPrintDC;
	//##ModelId=3CE2422201A0
              BOOL        m_bResizeColumnsToFitContent;
	//##ModelId=3CE2422201B0
              BOOL        m_bPrintHead;
	//##ModelId=3CE2422201BF
              BOOL        m_bPrintFoot;
	//##ModelId=3CE2422201CF
              BOOL        m_bTruncateColumn;
	//##ModelId=3CE2422201DF
              BOOL        m_bPrintColumnSeparator;
	//##ModelId=3CE2422201E0
              BOOL        m_bPrintLineSeparator;
	//##ModelId=3CE2422201EE
              BOOL        m_bPrintHeadBkgnd;
	//##ModelId=3CE2422201FE
              BOOL        m_bPrintFootBkgnd;
	//##ModelId=3CE2422201FF
              BOOL        m_bHeaderLine1;
	//##ModelId=3CE24222020E
              BOOL        m_bHeaderLine2;
	//##ModelId=3CE24222026B
              char        m_szFtHead[255];
	//##ModelId=3CE24222028B
              char        m_szFtFoot[255];
	//##ModelId=3CE24222028C
              char        m_szFtList[255];
	//##ModelId=3CE24222029A
              char        m_szHeaderLine1[512];
	//##ModelId=3CE2422202AA
              char        m_szHeaderLine2[512];
	//##ModelId=3CE2422202B9
              int         m_iScreenFtHead;
	//##ModelId=3CE2422202BA
              int         m_iScreenFtFoot;
	//##ModelId=3CE2422202C9
              int         m_iScreenFtList;
	//##ModelId=3CE2422202D9
              UINT        m_uiHeaderLine1Format;
	//##ModelId=3CE2422202E8
              UINT        m_uiHeaderLine2Format;
	//##ModelId=3CE2422202E9
              COLORREF    m_clrHeadBkgnd;
	//##ModelId=3CE2422202F8
              COLORREF    m_clrFootBkgnd;
	//##ModelId=3CE242220309
              SYSTEMTIME  m_stST;
	//##ModelId=3CE242220317
              HGLOBAL     m_hDevMode;

              #ifdef _AFXDLL
	//##ModelId=3CE242220327
              int        *m_piColWidthTable;  // Used for print preview
	//##ModelId=3CE242220336
              int        *m_piColFormatTable; // Used for print preview
	//##ModelId=3CE242220337
              char      **m_pszColTitleTable; // Used for print preview
	//##ModelId=3CE242220346
              CString     m_csDocName;        // Used for print preview

              HFONT       m_hHeadFont, m_hListFont, m_hFootFont; // Used for print preview
              #endif

	//##ModelId=3CE242220356
              void ClearPrintDC();
	//##ModelId=3CE242220365
              void CreateDevModeObject();

	//##ModelId=3CE242220375
              void DeleteDevModeObject();
              
	//##ModelId=3CE242220385
              int GetFootHeight(HFONT hFootFont);
	//##ModelId=3CE2422203A4
              int GetLengthyStringFromColumn(int iCol);
	//##ModelId=3CE2422203B3
              int GetTextSize(LPCTSTR lpszText, SIZE *pstSize = NULL);
              
	//##ModelId=3CE2422203D3
              void PrintFoot(int iOffsetY, HFONT hFootFont, int iPageCounter);
	//##ModelId=3CE24223000A
              int  PrintHead(int iOffsetY, HFONT hHeadFont, LPCTSTR lpszTitle, int iPageCounter);
	//##ModelId=3CE242230029
              int  PrintListCtrlHeader(int iOffsetY, HFONT hListFont, int iColCount, int *piColWidthTable, int *piColFormatTable, char **pszColTitleTable);
	//##ModelId=3CE242230048
              int  PrintListCtrlLine(int iPageWidth, int iPageHeight, int iOffsetY, HFONT hListFont, HWND hListCtrl, int iLine, int *piColWidthTable, int *piColFormatTable, BOOL *pbNewPage);

	//##ModelId=3CE242230078
              BOOL ResizeColumnToFitContent(int iCol);
	//##ModelId=3CE242230088
              BOOL ResizeColumnsToFitContent();

	//##ModelId=3CE242230097
              int ScreenHeightToPrinterHeight(int iScreenHeight);
	//##ModelId=3CE242230099
              int ScreenWidthToPrinterWidth(int iScreenWidth);

  private :
            #ifdef _AFXDLL
	//##ModelId=3CE2422300A7
            int m_iLineCount;    // Used for print preview
	//##ModelId=3CE2422300B6
            int m_iColCount;     // Used for print preview
	//##ModelId=3CE2422300C5
            int m_nLinesPerPage; // Used for print preview
	//##ModelId=3CE2422300D5
            int m_iPageHeight;   // Used for print preview
	//##ModelId=3CE2422300D6
            int m_iPageWidth;    // Used for print preview
            #endif

  public :
	//##ModelId=3CE2422300E5
           CPrintListCtrl();
	//##ModelId=3CE2422300F4
           virtual ~CPrintListCtrl();

	//##ModelId=3CE242230104
           BOOL Print(HWND hParent, LPCTSTR lpszDocName = "PrintListCtrl", BOOL bDefault = FALSE);
	//##ModelId=3CE242230123
           BOOL PrintSetup(HWND hParent, BOOL bDefault = FALSE);

	//##ModelId=3CE242230134
           BOOL SetDefaultPrinterSettings(HWND hParent = NULL);

	//##ModelId=3CE242230142
           void SetHeaderLine1(char *szLine = NULL, UINT uiFormat = DT_RIGHT);
	//##ModelId=3CE242230152
           void SetHeaderLine2(char *szLine = NULL, UINT uiFormat = DT_RIGHT);

	//##ModelId=3CE242230162
           BOOL SetPrinterOrientation(BOOL bLandscape = FALSE);
	//##ModelId=3CE242230171
           BOOL SetPrinterPaperSize(int iValue = DMPAPER_A4); // See DEVMODE structure for possible parameters

           // Functions for print preview
           #ifdef _AFXDLL
	//##ModelId=3CE242230181
           void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
	//##ModelId=3CE242230191
           void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);
	//##ModelId=3CE2422301A0
           BOOL OnPreparePrinting(CPrintInfo *pInfo, CString csDocName);
	//##ModelId=3CE2422301A3
           void OnPrint(CDC *pDC, CPrintInfo *pInfo);
           #endif

           // INLINE
	//##ModelId=3CE2422301B2
           BOOL  GetPrintColumnSeparator()      { return m_bPrintColumnSeparator; }
	//##ModelId=3CE2422301BF
           BOOL  GetPrintFoot()                 { return m_bPrintFoot; }
	//##ModelId=3CE2422301C0
           BOOL  GetPrintHead()                 { return m_bPrintHead; }
	//##ModelId=3CE2422301C1
           BOOL  GetPrintLineSeparator()        { return m_bPrintLineSeparator; }

	//##ModelId=3CE2422301CF
           BOOL  GetResizeColumnsToFitContent() { return m_bResizeColumnsToFitContent; }

	//##ModelId=3CE2422301D0
           HWND  GetListCtrl()                  { return m_hListCtrl; }

	//##ModelId=3CE2422301D1
           BOOL  GetTruncateColumn()            { return m_bTruncateColumn; }

	//##ModelId=3CE2422301DF
           char *GetHeadFontName()              { return m_szFtHead; }
	//##ModelId=3CE2422301E0
           char *GetFootFontName()              { return m_szFtFoot; }
	//##ModelId=3CE2422301E1
           char *GetListFontName()              { return m_szFtList; }

	//##ModelId=3CE2422301EE
           int   GetHeadFontHeight()            { return m_iScreenFtHead; }
	//##ModelId=3CE2422301EF
           int   GetFootFontHeight()            { return m_iScreenFtFoot; }
	//##ModelId=3CE2422301F0
           int   GetListFontHeight()            { return m_iScreenFtList; }

	//##ModelId=3CE2422301FE
           void PrintColumnSeparator(BOOL bValue)      { m_bPrintColumnSeparator = bValue; }
	//##ModelId=3CE242230200
           void PrintFoot(BOOL bValue)                 { m_bPrintFoot = bValue; }
	//##ModelId=3CE24223020F
           void PrintHead(BOOL bValue)                 { m_bPrintHead = bValue; }
	//##ModelId=3CE24223021D
           void PrintLineSeparator(BOOL bValue)        { m_bPrintLineSeparator = bValue; }

	//##ModelId=3CE24223021F
           void ResizeColumnsToFitContent(BOOL bValue) { m_bResizeColumnsToFitContent = bValue; }

	//##ModelId=3CE24223022D
           void SetListCtrl(HWND hListCtrl)            { m_hListCtrl = hListCtrl; }

	//##ModelId=3CE24223022F
           void TruncateColumn(BOOL bValue)            { m_bTruncateColumn = bValue; }

	//##ModelId=3CE24223023E
           void PrintFootBkgnd(BOOL bValue, COLORREF clrValue = RGB(255,255,255))
            {
             m_bPrintFootBkgnd = bValue;
             m_clrFootBkgnd    = clrValue;
            }

	//##ModelId=3CE24223024D
           void PrintHeadBkgnd(BOOL bValue, COLORREF clrValue = RGB(255,255,255))
            {
             m_bPrintHeadBkgnd = bValue;
             m_clrHeadBkgnd    = clrValue;
            }

	//##ModelId=3CE24223025D
           void SetHeadFont(LPCTSTR lpszFontName = "Arial", int iFontHeight = 10)
            {
             strcpy(m_szFtHead, lpszFontName);
             m_iScreenFtHead = iFontHeight;
            }

	//##ModelId=3CE24223026B
           void SetFootFont(LPCTSTR lpszFontName = "Arial", int iFontHeight = 10)
            {
             strcpy(m_szFtFoot, lpszFontName);
             m_iScreenFtFoot = iFontHeight;
            }

	//##ModelId=3CE24223027B
           void SetListFont(LPCTSTR lpszFontName = "Arial", int iFontHeight = 10)
            {
             strcpy(m_szFtList, lpszFontName);
             m_iScreenFtList = iFontHeight;
            }
 };

#endif // !defined(AFX_PRINTLISTCTRL_H__D5C2BEC1_C744_11D3_A0E2_004005555C30__INCLUDED_)
