/*********************************************************************

 ADOBE SYSTEMS INCORPORATED
 Copyright (C) 1994-2006 Adobe Systems Incorporated
 All rights reserved.

 NOTICE: Adobe permits you to use, modify, and distribute this file
 in accordance with the terms of the Adobe license agreement
 accompanying it. If you have received this file from a source other
 than Adobe, then your use, modification, or distribution of it
 requires the prior written permission of Adobe.

 ---------------------------------------------------------------------

 acrobat.h

*********************************************************************/
// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// CAcroApp wrapper class

class CAcroApp : public COleDispatchDriver
{
public:
	CAcroApp() {}		// Calls COleDispatchDriver default constructor
	CAcroApp(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroApp(const CAcroApp& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long Exit();
	long Hide();
	long Show();
	long CloseAllDocs();
	long MenuItemExecute(LPCTSTR szMenuItemName);
	CString GetActiveTool();
	long SetActiveTool(LPCTSTR szButtonName, long bPersistent);
	long ToolButtonRemove(LPCTSTR szButtonName);
	long ToolButtonIsEnabled(LPCTSTR szButtonName);
	long MenuItemRemove(LPCTSTR szMenuItemName);
	long MenuItemIsEnabled(LPCTSTR szMenuItemName);
	long MenuItemIsMarked(LPCTSTR szMenuItemName);
	long GetNumAVDocs();
	LPDISPATCH GetAVDoc(long nIndex);
	LPDISPATCH GetActiveDoc();
	CString GetLanguage();
	long Lock(LPCTSTR szLockedBy);
	long Unlock();
	long SetPreference(short nType, long nValue);
	long GetPreference(short nType);
	long Maximize(long bMaxSize);
	long SetFrame(LPDISPATCH rect);
	LPDISPATCH GetFrame();
	long Minimize(long bMinimize);
	long Restore(long bRestore);
	long UnlockEx(LPCTSTR szLockedBy);
	LPDISPATCH GetInterface(LPCTSTR szName);
	long SetPreferenceEx(short nType, VARIANT* val);
	VARIANT GetPreferenceEx(short nType);
};
/////////////////////////////////////////////////////////////////////////////
// CAcroPDDoc wrapper class

class CAcroPDDoc : public COleDispatchDriver
{
public:
	CAcroPDDoc() {}		// Calls COleDispatchDriver default constructor
	CAcroPDDoc(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroPDDoc(const CAcroPDDoc& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long Open(LPCTSTR szFullPath);
	long Close();
	long InsertPages(long nInsertPageAfter, LPDISPATCH iPDDocSource, long nStartPage, long nNumPages, long bBookmarks);
	long ReplacePages(long nStartPage, LPDISPATCH iPDDocSource, long nStartSourcePage, long nNumPages, long bMergeTextAnnotations);
	long DeletePages(long nStartPage, long nEndPage);
	long GetNumPages();
	long Create();
	CString GetInfo(LPCTSTR szInfoKey);
	long SetInfo(LPCTSTR szInfoKey, LPCTSTR szBuffer);
	long DeleteThumbs(long nStartPage, long nEndPage);
	long MovePage(long nMoveAfterThisPage, long nPageToMove);
	CString GetFileName();
	long GetPageMode();
	long SetPageMode(long nPageMode);
	long CreateThumbs(long nFirstPage, long nLastPage);
	LPDISPATCH CreateTextSelect(long nPage, LPDISPATCH iAcroRect);
	LPDISPATCH AcquirePage(long nPage);
	CString GetInstanceID();
	CString GetPermanentID();
	long GetFlags();
	long SetFlags(long nFlags);
	LPDISPATCH OpenAVDoc(LPCTSTR szTempTitle);
	long Save(short nType, LPCTSTR szFullPath);
	long ClearFlags(long nFlags);
	long SetOpenInfo(long pgNum, short viewMode, LPCTSTR magnification);
	long CropPages(long nStartPage, long nEndPage, short nOddOrEvenPagesOnly, LPDISPATCH iAcroRect);
	LPDISPATCH GetJSObject();
};
/////////////////////////////////////////////////////////////////////////////
// CAcroAVDoc wrapper class

class CAcroAVDoc : public COleDispatchDriver
{
public:
	CAcroAVDoc() {}		// Calls COleDispatchDriver default constructor
	CAcroAVDoc(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroAVDoc(const CAcroAVDoc& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long Open(LPCTSTR szFullPath, LPCTSTR szTempTitle);
	LPDISPATCH GetPDDoc();
	LPDISPATCH GetAVPageView();
	long SetViewMode(long nType);
	long FindText(LPCTSTR szText, long bCaseSensitive, long bWholeWordsOnly, long bReset);
	long Close(long bNoSave);
	long GetViewMode();
	long PrintPages(long nFirstPage, long nLastPage, long nPSLevel, long bBinaryOk, long bShrinkToFit);
	long ClearSelection();
	long BringToFront();
	CString GetTitle();
	long Maximize(long bMaxSize);
	long SetTitle(LPCTSTR szTitle);
	long OpenInWindow(LPCTSTR szFullPath, short hWnd);
	long SetTextSelection(LPDISPATCH iAcroPDTextSelect);
	long ShowTextSelect();
	long SetFrame(LPDISPATCH rect);
	LPDISPATCH GetFrame();
	long IsValid();
	long OpenInWindowEx(LPCTSTR szFullPath, long hWnd, long openFlgs, long UseOpenParams, long pgNum, short pageMode, short ZoomType, long Zoom, short Top, short Left);
	long PrintPagesSilent(long nFirstPage, long nLastPage, long nPSLevel, long bBinaryOk, long bShrinkToFit);
	long PrintPagesEx(long nFirstPage, long nLastPage, long nPSLevel, long bBinaryOk, long bShrinkToFit, long bReverse, long bFarEastFontOpt, long bEmitHalftones, long iPageOption);
	long PrintPagesSilentEx(long nFirstPage, long nLastPage, long nPSLevel, long bBinaryOk, long bShrinkToFit, long bReverse, long bFarEastFontOpt, long bEmitHalftones, long iPageOption);
	LPDISPATCH GetParentPDDoc();
};
/////////////////////////////////////////////////////////////////////////////
// CAcroAVPageView wrapper class

class CAcroAVPageView : public COleDispatchDriver
{
public:
	CAcroAVPageView() {}		// Calls COleDispatchDriver default constructor
	CAcroAVPageView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroAVPageView(const CAcroAVPageView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GoTo(long nPage);
	long ZoomTo(short nType, short nScale);
	long ScrollTo(short nX, short nY);
	long ReadPageUp();
	long ReadPageDown();
	long DoGoBack();
	long DoGoForward();
	LPDISPATCH GetAVDoc();
	LPDISPATCH GetPage();
	LPDISPATCH GetDoc();
	long GetZoom();
	short GetZoomType();
	long GetPageNum();
	LPDISPATCH PointToDevice(LPDISPATCH point);
	LPDISPATCH DevicePointToPage(LPDISPATCH point);
	LPDISPATCH GetAperture();
};
/////////////////////////////////////////////////////////////////////////////
// CAcroPDPage wrapper class

class CAcroPDPage : public COleDispatchDriver
{
public:
	CAcroPDPage() {}		// Calls COleDispatchDriver default constructor
	CAcroPDPage(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroPDPage(const CAcroPDPage& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetSize();
	LPDISPATCH GetAnnot(long nIndex);
	LPDISPATCH AddNewAnnot(long nIndexAddAfter, LPCTSTR szSubType, LPDISPATCH rect);
	long AddAnnot(long nIndexAddAfter, LPDISPATCH i);
	long RemoveAnnot(long nIndex);
	long GetAnnotIndex(LPDISPATCH i);
	long GetNumAnnots();
	LPDISPATCH CreatePageHilite(LPDISPATCH iAcroHiliteList);
	LPDISPATCH CreateWordHilite(LPDISPATCH iAcroHiliteList);
	LPDISPATCH GetDoc();
	long GetNumber();
	long Draw(short nHWND, short nHDC, short nXOrigin, short nYOrigin, short nZoom);
	short GetRotate();
	long SetRotate(short nRotate);
	long DrawEx(long nHWND, long nHDC, LPDISPATCH updateRect, short nXOrigin, short nYOrigin, short nZoom);
	long CopyToClipboard(LPDISPATCH boundRect, short nXOrigin, short nYOrigin, short nZoom);
	long CropPage(LPDISPATCH rect);
};
/////////////////////////////////////////////////////////////////////////////
// CAcroPDAnnot wrapper class

class CAcroPDAnnot : public COleDispatchDriver
{
public:
	CAcroPDAnnot() {}		// Calls COleDispatchDriver default constructor
	CAcroPDAnnot(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroPDAnnot(const CAcroPDAnnot& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long IsValid();
	CString GetSubtype();
	long IsEqual(LPDISPATCH PDAnnot);
	long GetColor();
	long SetColor(long nRGBColor);
	CString GetTitle();
	long SetTitle(LPCTSTR szTitle);
	CString GetContents();
	long SetContents(LPCTSTR szContents);
	long IsOpen();
	long SetOpen(long bIsOpen);
	LPDISPATCH GetRect();
	long SetRect(LPDISPATCH rect);
	LPDISPATCH GetDate();
	long SetDate(LPDISPATCH iAcroTime);
	long Perform(LPDISPATCH iAcroAVDoc);
};
/////////////////////////////////////////////////////////////////////////////
// CAcroPoint wrapper class

class CAcroPoint : public COleDispatchDriver
{
public:
	CAcroPoint() {}		// Calls COleDispatchDriver default constructor
	CAcroPoint(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroPoint(const CAcroPoint& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetX();
	void SetX(short);
	short GetY();
	void SetY(short);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// CAcroRect wrapper class

class CAcroRect : public COleDispatchDriver
{
public:
	CAcroRect() {}		// Calls COleDispatchDriver default constructor
	CAcroRect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroRect(const CAcroRect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetLeft();
	void SetLeft(short);
	short GetTop();
	void SetTop(short);
	short GetRight();
	void SetRight(short);
	short GetBottom();
	void SetBottom(short);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// CAcroTime wrapper class

class CAcroTime : public COleDispatchDriver
{
public:
	CAcroTime() {}		// Calls COleDispatchDriver default constructor
	CAcroTime(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroTime(const CAcroTime& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetYear();
	void SetYear(short);
	short GetMonth();
	void SetMonth(short);
	short GetDate();
	void SetDate(short);
	short GetHour();
	void SetHour(short);
	short GetMinute();
	void SetMinute(short);
	short GetSecond();
	void SetSecond(short);
	short GetMillisecond();
	void SetMillisecond(short);
	short GetDay();
	void SetDay(short);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// CAcroPDTextSelect wrapper class

class CAcroPDTextSelect : public COleDispatchDriver
{
public:
	CAcroPDTextSelect() {}		// Calls COleDispatchDriver default constructor
	CAcroPDTextSelect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroPDTextSelect(const CAcroPDTextSelect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long Destroy();
	long GetNumText();
	LPDISPATCH GetBoundingRect();
	long GetPage();
	CString GetText(long nTextIndex);
};
/////////////////////////////////////////////////////////////////////////////
// CAcroHiliteList wrapper class

class CAcroHiliteList : public COleDispatchDriver
{
public:
	CAcroHiliteList() {}		// Calls COleDispatchDriver default constructor
	CAcroHiliteList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroHiliteList(const CAcroHiliteList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long Add(short nOffset, short nLength);
};
/////////////////////////////////////////////////////////////////////////////
// CAcroPDBookmark wrapper class

class CAcroPDBookmark : public COleDispatchDriver
{
public:
	CAcroPDBookmark() {}		// Calls COleDispatchDriver default constructor
	CAcroPDBookmark(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcroPDBookmark(const CAcroPDBookmark& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetByTitle(LPDISPATCH iAcroPDDoc, LPCTSTR szBookmarkTitle);
	long Destroy();
	long IsValid();
	CString GetTitle();
	long SetTitle(LPCTSTR szNewTitle);
	long Perform(LPDISPATCH iAcroAVDoc);
};
