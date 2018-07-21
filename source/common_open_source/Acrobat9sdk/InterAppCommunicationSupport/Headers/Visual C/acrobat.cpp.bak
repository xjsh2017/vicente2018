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

 acrobat.cpp

*********************************************************************/
// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "acrobat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CAcroApp properties

/////////////////////////////////////////////////////////////////////////////
// CAcroApp operations

long CAcroApp::Exit()
{
	long result;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroApp::Hide()
{
	long result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroApp::Show()
{
	long result;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroApp::CloseAllDocs()
{
	long result;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroApp::MenuItemExecute(LPCTSTR szMenuItemName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szMenuItemName);
	return result;
}

CString CAcroApp::GetActiveTool()
{
	CString result;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CAcroApp::SetActiveTool(LPCTSTR szButtonName, long bPersistent)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szButtonName, bPersistent);
	return result;
}

long CAcroApp::ToolButtonRemove(LPCTSTR szButtonName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szButtonName);
	return result;
}

long CAcroApp::ToolButtonIsEnabled(LPCTSTR szButtonName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szButtonName);
	return result;
}

long CAcroApp::MenuItemRemove(LPCTSTR szMenuItemName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szMenuItemName);
	return result;
}

long CAcroApp::MenuItemIsEnabled(LPCTSTR szMenuItemName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szMenuItemName);
	return result;
}

long CAcroApp::MenuItemIsMarked(LPCTSTR szMenuItemName)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szMenuItemName);
	return result;
}

long CAcroApp::GetNumAVDocs()
{
	long result;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH CAcroApp::GetAVDoc(long nIndex)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		nIndex);
	return result;
}

LPDISPATCH CAcroApp::GetActiveDoc()
{
	LPDISPATCH result;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString CAcroApp::GetLanguage()
{
	CString result;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CAcroApp::Lock(LPCTSTR szLockedBy)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szLockedBy);
	return result;
}

long CAcroApp::Unlock()
{
	long result;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroApp::SetPreference(short nType, long nValue)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nType, nValue);
	return result;
}

long CAcroApp::GetPreference(short nType)
{
	long result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nType);
	return result;
}

long CAcroApp::Maximize(long bMaxSize)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		bMaxSize);
	return result;
}

long CAcroApp::SetFrame(LPDISPATCH rect)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		rect);
	return result;
}

LPDISPATCH CAcroApp::GetFrame()
{
	LPDISPATCH result;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long CAcroApp::Minimize(long bMinimize)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		bMinimize);
	return result;
}

long CAcroApp::Restore(long bRestore)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		bRestore);
	return result;
}

long CAcroApp::UnlockEx(LPCTSTR szLockedBy)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szLockedBy);
	return result;
}

LPDISPATCH CAcroApp::GetInterface(LPCTSTR szName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		szName);
	return result;
}

long CAcroApp::SetPreferenceEx(short nType, VARIANT* val)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_PVARIANT;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nType, val);
	return result;
}

VARIANT CAcroApp::GetPreferenceEx(short nType)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		nType);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// CAcroPDDoc properties

/////////////////////////////////////////////////////////////////////////////
// CAcroPDDoc operations

long CAcroPDDoc::Open(LPCTSTR szFullPath)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szFullPath);
	return result;
}

long CAcroPDDoc::Close()
{
	long result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroPDDoc::InsertPages(long nInsertPageAfter, LPDISPATCH iPDDocSource, long nStartPage, long nNumPages, long bBookmarks)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_DISPATCH VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nInsertPageAfter, iPDDocSource, nStartPage, nNumPages, bBookmarks);
	return result;
}

long CAcroPDDoc::ReplacePages(long nStartPage, LPDISPATCH iPDDocSource, long nStartSourcePage, long nNumPages, long bMergeTextAnnotations)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_DISPATCH VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nStartPage, iPDDocSource, nStartSourcePage, nNumPages, bMergeTextAnnotations);
	return result;
}

long CAcroPDDoc::DeletePages(long nStartPage, long nEndPage)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nStartPage, nEndPage);
	return result;
}

long CAcroPDDoc::GetNumPages()
{
	long result;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroPDDoc::Create()
{
	long result;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CAcroPDDoc::GetInfo(LPCTSTR szInfoKey)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		szInfoKey);
	return result;
}

long CAcroPDDoc::SetInfo(LPCTSTR szInfoKey, LPCTSTR szBuffer)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szInfoKey, szBuffer);
	return result;
}

long CAcroPDDoc::DeleteThumbs(long nStartPage, long nEndPage)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nStartPage, nEndPage);
	return result;
}

long CAcroPDDoc::MovePage(long nMoveAfterThisPage, long nPageToMove)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nMoveAfterThisPage, nPageToMove);
	return result;
}

CString CAcroPDDoc::GetFileName()
{
	CString result;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CAcroPDDoc::GetPageMode()
{
	long result;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroPDDoc::SetPageMode(long nPageMode)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nPageMode);
	return result;
}

long CAcroPDDoc::CreateThumbs(long nFirstPage, long nLastPage)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nFirstPage, nLastPage);
	return result;
}

LPDISPATCH CAcroPDDoc::CreateTextSelect(long nPage, LPDISPATCH iAcroRect)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_DISPATCH;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		nPage, iAcroRect);
	return result;
}

LPDISPATCH CAcroPDDoc::AcquirePage(long nPage)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		nPage);
	return result;
}

CString CAcroPDDoc::GetInstanceID()
{
	CString result;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString CAcroPDDoc::GetPermanentID()
{
	CString result;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CAcroPDDoc::GetFlags()
{
	long result;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroPDDoc::SetFlags(long nFlags)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nFlags);
	return result;
}

LPDISPATCH CAcroPDDoc::OpenAVDoc(LPCTSTR szTempTitle)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		szTempTitle);
	return result;
}

long CAcroPDDoc::Save(short nType, LPCTSTR szFullPath)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nType, szFullPath);
	return result;
}

long CAcroPDDoc::ClearFlags(long nFlags)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nFlags);
	return result;
}

long CAcroPDDoc::SetOpenInfo(long pgNum, short viewMode, LPCTSTR magnification)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I2 VTS_BSTR;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		pgNum, viewMode, magnification);
	return result;
}

long CAcroPDDoc::CropPages(long nStartPage, long nEndPage, short nOddOrEvenPagesOnly, LPDISPATCH iAcroRect)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I2 VTS_DISPATCH;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nStartPage, nEndPage, nOddOrEvenPagesOnly, iAcroRect);
	return result;
}

LPDISPATCH CAcroPDDoc::GetJSObject()
{
	LPDISPATCH result;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// CAcroAVDoc properties

/////////////////////////////////////////////////////////////////////////////
// CAcroAVDoc operations

long CAcroAVDoc::Open(LPCTSTR szFullPath, LPCTSTR szTempTitle)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szFullPath, szTempTitle);
	return result;
}

LPDISPATCH CAcroAVDoc::GetPDDoc()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH CAcroAVDoc::GetAVPageView()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long CAcroAVDoc::SetViewMode(long nType)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nType);
	return result;
}

long CAcroAVDoc::FindText(LPCTSTR szText, long bCaseSensitive, long bWholeWordsOnly, long bReset)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szText, bCaseSensitive, bWholeWordsOnly, bReset);
	return result;
}

long CAcroAVDoc::Close(long bNoSave)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		bNoSave);
	return result;
}

long CAcroAVDoc::GetViewMode()
{
	long result;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroAVDoc::PrintPages(long nFirstPage, long nLastPage, long nPSLevel, long bBinaryOk, long bShrinkToFit)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nFirstPage, nLastPage, nPSLevel, bBinaryOk, bShrinkToFit);
	return result;
}

long CAcroAVDoc::ClearSelection()
{
	long result;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroAVDoc::BringToFront()
{
	long result;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CAcroAVDoc::GetTitle()
{
	CString result;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CAcroAVDoc::Maximize(long bMaxSize)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		bMaxSize);
	return result;
}

long CAcroAVDoc::SetTitle(LPCTSTR szTitle)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szTitle);
	return result;
}

long CAcroAVDoc::OpenInWindow(LPCTSTR szFullPath, short hWnd)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I2;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szFullPath, hWnd);
	return result;
}

long CAcroAVDoc::SetTextSelection(LPDISPATCH iAcroPDTextSelect)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		iAcroPDTextSelect);
	return result;
}

long CAcroAVDoc::ShowTextSelect()
{
	long result;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroAVDoc::SetFrame(LPDISPATCH rect)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		rect);
	return result;
}

LPDISPATCH CAcroAVDoc::GetFrame()
{
	LPDISPATCH result;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long CAcroAVDoc::IsValid()
{
	long result;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroAVDoc::OpenInWindowEx(LPCTSTR szFullPath, long hWnd, long openFlgs, long UseOpenParams, long pgNum, short pageMode, short ZoomType, long Zoom, short Top, short Left)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I2 VTS_I2 VTS_I4 VTS_I2 VTS_I2;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szFullPath, hWnd, openFlgs, UseOpenParams, pgNum, pageMode, ZoomType, Zoom, Top, Left);
	return result;
}

long CAcroAVDoc::PrintPagesSilent(long nFirstPage, long nLastPage, long nPSLevel, long bBinaryOk, long bShrinkToFit)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nFirstPage, nLastPage, nPSLevel, bBinaryOk, bShrinkToFit);
	return result;
}

long CAcroAVDoc::PrintPagesEx(long nFirstPage, long nLastPage, long nPSLevel, long bBinaryOk, long bShrinkToFit, long bReverse, long bFarEastFontOpt, long bEmitHalftones, long iPageOption)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nFirstPage, nLastPage, nPSLevel, bBinaryOk, bShrinkToFit, bReverse, bFarEastFontOpt, bEmitHalftones, iPageOption);
	return result;
}

long CAcroAVDoc::PrintPagesSilentEx(long nFirstPage, long nLastPage, long nPSLevel, long bBinaryOk, long bShrinkToFit, long bReverse, long bFarEastFontOpt, long bEmitHalftones, long iPageOption)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nFirstPage, nLastPage, nPSLevel, bBinaryOk, bShrinkToFit, bReverse, bFarEastFontOpt, bEmitHalftones, iPageOption);
	return result;
}

LPDISPATCH CAcroAVDoc::GetParentPDDoc()
{
	LPDISPATCH result;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CAcroAVPageView properties

/////////////////////////////////////////////////////////////////////////////
// CAcroAVPageView operations

long CAcroAVPageView::GoTo(long nPage)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nPage);
	return result;
}

long CAcroAVPageView::ZoomTo(short nType, short nScale)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nType, nScale);
	return result;
}

long CAcroAVPageView::ScrollTo(short nX, short nY)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nX, nY);
	return result;
}

long CAcroAVPageView::ReadPageUp()
{
	long result;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroAVPageView::ReadPageDown()
{
	long result;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroAVPageView::DoGoBack()
{
	long result;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroAVPageView::DoGoForward()
{
	long result;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH CAcroAVPageView::GetAVDoc()
{
	LPDISPATCH result;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH CAcroAVPageView::GetPage()
{
	LPDISPATCH result;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH CAcroAVPageView::GetDoc()
{
	LPDISPATCH result;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long CAcroAVPageView::GetZoom()
{
	long result;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

short CAcroAVPageView::GetZoomType()
{
	short result;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

long CAcroAVPageView::GetPageNum()
{
	long result;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH CAcroAVPageView::PointToDevice(LPDISPATCH point)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		point);
	return result;
}

LPDISPATCH CAcroAVPageView::DevicePointToPage(LPDISPATCH point)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		point);
	return result;
}

LPDISPATCH CAcroAVPageView::GetAperture()
{
	LPDISPATCH result;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// CAcroPDPage properties

/////////////////////////////////////////////////////////////////////////////
// CAcroPDPage operations

LPDISPATCH CAcroPDPage::GetSize()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH CAcroPDPage::GetAnnot(long nIndex)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		nIndex);
	return result;
}

LPDISPATCH CAcroPDPage::AddNewAnnot(long nIndexAddAfter, LPCTSTR szSubType, LPDISPATCH rect)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_DISPATCH;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		nIndexAddAfter, szSubType, rect);
	return result;
}

long CAcroPDPage::AddAnnot(long nIndexAddAfter, LPDISPATCH i)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_DISPATCH;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nIndexAddAfter, i);
	return result;
}

long CAcroPDPage::RemoveAnnot(long nIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nIndex);
	return result;
}

long CAcroPDPage::GetAnnotIndex(LPDISPATCH i)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		i);
	return result;
}

long CAcroPDPage::GetNumAnnots()
{
	long result;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH CAcroPDPage::CreatePageHilite(LPDISPATCH iAcroHiliteList)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		iAcroHiliteList);
	return result;
}

LPDISPATCH CAcroPDPage::CreateWordHilite(LPDISPATCH iAcroHiliteList)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		iAcroHiliteList);
	return result;
}

LPDISPATCH CAcroPDPage::GetDoc()
{
	LPDISPATCH result;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long CAcroPDPage::GetNumber()
{
	long result;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroPDPage::Draw(short nHWND, short nHDC, short nXOrigin, short nYOrigin, short nZoom)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nHWND, nHDC, nXOrigin, nYOrigin, nZoom);
	return result;
}

short CAcroPDPage::GetRotate()
{
	short result;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

long CAcroPDPage::SetRotate(short nRotate)
{
	long result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nRotate);
	return result;
}

long CAcroPDPage::DrawEx(long nHWND, long nHDC, LPDISPATCH updateRect, short nXOrigin, short nYOrigin, short nZoom)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_DISPATCH VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nHWND, nHDC, updateRect, nXOrigin, nYOrigin, nZoom);
	return result;
}

long CAcroPDPage::CopyToClipboard(LPDISPATCH boundRect, short nXOrigin, short nYOrigin, short nZoom)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		boundRect, nXOrigin, nYOrigin, nZoom);
	return result;
}

long CAcroPDPage::CropPage(LPDISPATCH rect)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		rect);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// CAcroPDAnnot properties

/////////////////////////////////////////////////////////////////////////////
// CAcroPDAnnot operations

long CAcroPDAnnot::IsValid()
{
	long result;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CAcroPDAnnot::GetSubtype()
{
	CString result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CAcroPDAnnot::IsEqual(LPDISPATCH PDAnnot)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		PDAnnot);
	return result;
}

long CAcroPDAnnot::GetColor()
{
	long result;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroPDAnnot::SetColor(long nRGBColor)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nRGBColor);
	return result;
}

CString CAcroPDAnnot::GetTitle()
{
	CString result;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CAcroPDAnnot::SetTitle(LPCTSTR szTitle)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szTitle);
	return result;
}

CString CAcroPDAnnot::GetContents()
{
	CString result;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CAcroPDAnnot::SetContents(LPCTSTR szContents)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szContents);
	return result;
}

long CAcroPDAnnot::IsOpen()
{
	long result;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroPDAnnot::SetOpen(long bIsOpen)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		bIsOpen);
	return result;
}

LPDISPATCH CAcroPDAnnot::GetRect()
{
	LPDISPATCH result;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long CAcroPDAnnot::SetRect(LPDISPATCH rect)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		rect);
	return result;
}

LPDISPATCH CAcroPDAnnot::GetDate()
{
	LPDISPATCH result;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long CAcroPDAnnot::SetDate(LPDISPATCH iAcroTime)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		iAcroTime);
	return result;
}

long CAcroPDAnnot::Perform(LPDISPATCH iAcroAVDoc)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		iAcroAVDoc);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// CAcroPoint properties

short CAcroPoint::GetX()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void CAcroPoint::SetX(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

short CAcroPoint::GetY()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}

void CAcroPoint::SetY(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CAcroPoint operations


/////////////////////////////////////////////////////////////////////////////
// CAcroRect properties

short CAcroRect::GetLeft()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void CAcroRect::SetLeft(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

short CAcroRect::GetTop()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}

void CAcroRect::SetTop(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}

short CAcroRect::GetRight()
{
	short result;
	GetProperty(0x3, VT_I2, (void*)&result);
	return result;
}

void CAcroRect::SetRight(short propVal)
{
	SetProperty(0x3, VT_I2, propVal);
}

short CAcroRect::GetBottom()
{
	short result;
	GetProperty(0x4, VT_I2, (void*)&result);
	return result;
}

void CAcroRect::SetBottom(short propVal)
{
	SetProperty(0x4, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CAcroRect operations


/////////////////////////////////////////////////////////////////////////////
// CAcroTime properties

short CAcroTime::GetYear()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void CAcroTime::SetYear(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

short CAcroTime::GetMonth()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}

void CAcroTime::SetMonth(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}

short CAcroTime::GetDate()
{
	short result;
	GetProperty(0x3, VT_I2, (void*)&result);
	return result;
}

void CAcroTime::SetDate(short propVal)
{
	SetProperty(0x3, VT_I2, propVal);
}

short CAcroTime::GetHour()
{
	short result;
	GetProperty(0x4, VT_I2, (void*)&result);
	return result;
}

void CAcroTime::SetHour(short propVal)
{
	SetProperty(0x4, VT_I2, propVal);
}

short CAcroTime::GetMinute()
{
	short result;
	GetProperty(0x5, VT_I2, (void*)&result);
	return result;
}

void CAcroTime::SetMinute(short propVal)
{
	SetProperty(0x5, VT_I2, propVal);
}

short CAcroTime::GetSecond()
{
	short result;
	GetProperty(0x6, VT_I2, (void*)&result);
	return result;
}

void CAcroTime::SetSecond(short propVal)
{
	SetProperty(0x6, VT_I2, propVal);
}

short CAcroTime::GetMillisecond()
{
	short result;
	GetProperty(0x7, VT_I2, (void*)&result);
	return result;
}

void CAcroTime::SetMillisecond(short propVal)
{
	SetProperty(0x7, VT_I2, propVal);
}

short CAcroTime::GetDay()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}

void CAcroTime::SetDay(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CAcroTime operations


/////////////////////////////////////////////////////////////////////////////
// CAcroPDTextSelect properties

/////////////////////////////////////////////////////////////////////////////
// CAcroPDTextSelect operations

long CAcroPDTextSelect::Destroy()
{
	long result;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroPDTextSelect::GetNumText()
{
	long result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH CAcroPDTextSelect::GetBoundingRect()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long CAcroPDTextSelect::GetPage()
{
	long result;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CAcroPDTextSelect::GetText(long nTextIndex)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		nTextIndex);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// CAcroHiliteList properties

/////////////////////////////////////////////////////////////////////////////
// CAcroHiliteList operations

long CAcroHiliteList::Add(short nOffset, short nLength)
{
	long result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		nOffset, nLength);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// CAcroPDBookmark properties

/////////////////////////////////////////////////////////////////////////////
// CAcroPDBookmark operations

long CAcroPDBookmark::GetByTitle(LPDISPATCH iAcroPDDoc, LPCTSTR szBookmarkTitle)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		iAcroPDDoc, szBookmarkTitle);
	return result;
}

long CAcroPDBookmark::Destroy()
{
	long result;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long CAcroPDBookmark::IsValid()
{
	long result;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CString CAcroPDBookmark::GetTitle()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long CAcroPDBookmark::SetTitle(LPCTSTR szNewTitle)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		szNewTitle);
	return result;
}

long CAcroPDBookmark::Perform(LPDISPATCH iAcroAVDoc)
{
	long result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		iAcroAVDoc);
	return result;
}
