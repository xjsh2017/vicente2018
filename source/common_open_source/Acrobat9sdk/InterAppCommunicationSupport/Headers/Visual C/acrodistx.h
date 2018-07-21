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

 acrodistx.h

*********************************************************************/
// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _PdfEvents wrapper class

class _PdfEvents : public COleDispatchDriver
{
public:
	_PdfEvents() {}		// Calls COleDispatchDriver default constructor
	_PdfEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_PdfEvents(const _PdfEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void OnLogMessage(LPCTSTR strMessage);
	void OnJobStart(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF);
	void OnJobDone(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF);
	void OnJobFail(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF);
	void OnPercentDone(long nPercentDone);
	void OnPageNumber(long nPageNumber);
};
/////////////////////////////////////////////////////////////////////////////
// IPdfDistiller6 wrapper class

class IPdfDistiller6 : public COleDispatchDriver
{
public:
	IPdfDistiller6() {}		// Calls COleDispatchDriver default constructor
	IPdfDistiller6(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IPdfDistiller6(const IPdfDistiller6& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetBShowWindow();
	void SetBShowWindow(long nNewValue);
	long GetBSpoolJobs();
	void SetBSpoolJobs(long nNewValue);
	void CancelJob();
	short FileToPDF(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF, LPCTSTR strJobOptions);
	short FileToPDF2(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF, LPCTSTR strJobOptions, long bApplySecurity);
	void Create();
};
/////////////////////////////////////////////////////////////////////////////
// IPdfDistiller wrapper class

class IPdfDistiller : public COleDispatchDriver
{
public:
	IPdfDistiller() {}		// Calls COleDispatchDriver default constructor
	IPdfDistiller(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IPdfDistiller(const IPdfDistiller& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	short FileToPDF(LPCTSTR strInputPostScript, LPCTSTR strOutputPDF, LPCTSTR strJobOptions);
	void Create();
	long GetBSpoolJobs();
	void SetBSpoolJobs(long nNewValue);
	long GetBShowWindow();
	void SetBShowWindow(long nNewValue);
	void CancelJob();
};
