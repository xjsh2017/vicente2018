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

 acrodistx.cpp

*********************************************************************/
// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "acrodistx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _PdfEvents properties

/////////////////////////////////////////////////////////////////////////////
// _PdfEvents operations

void _PdfEvents::OnLogMessage(LPCTSTR strMessage)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strMessage);
}

void _PdfEvents::OnJobStart(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strInputPostScript, strOutputPDF);
}

void _PdfEvents::OnJobDone(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strInputPostScript, strOutputPDF);
}

void _PdfEvents::OnJobFail(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strInputPostScript, strOutputPDF);
}

void _PdfEvents::OnPercentDone(long nPercentDone)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nPercentDone);
}

void _PdfEvents::OnPageNumber(long nPageNumber)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 nPageNumber);
}


/////////////////////////////////////////////////////////////////////////////
// IPdfDistiller6 properties

/////////////////////////////////////////////////////////////////////////////
// IPdfDistiller6 operations

long IPdfDistiller6::GetBShowWindow()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IPdfDistiller6::SetBShowWindow(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IPdfDistiller6::GetBSpoolJobs()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IPdfDistiller6::SetBSpoolJobs(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void IPdfDistiller6::CancelJob()
{
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short IPdfDistiller6::FileToPDF(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF, LPCTSTR strJobOptions)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strInputPostScript, strOutputPDF, strJobOptions);
	return result;
}

short IPdfDistiller6::FileToPDF2(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF, LPCTSTR strJobOptions, long bApplySecurity)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strInputPostScript, strOutputPDF, strJobOptions, bApplySecurity);
	return result;
}

void IPdfDistiller6::Create()
{
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// IPdfDistiller properties

/////////////////////////////////////////////////////////////////////////////
// IPdfDistiller operations

short IPdfDistiller::FileToPDF(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF, LPCTSTR strJobOptions)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strInputPostScript, strOutputPDF, strJobOptions);
	return result;
}

void IPdfDistiller::Create()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long IPdfDistiller::GetBSpoolJobs()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IPdfDistiller::SetBSpoolJobs(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long IPdfDistiller::GetBShowWindow()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IPdfDistiller::SetBShowWindow(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void IPdfDistiller::CancelJob()
{
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
