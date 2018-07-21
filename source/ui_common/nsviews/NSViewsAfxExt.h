// This code is a part of the NanoSoft NSViews C++ Library.
// Copyright (C) 1996 NanoSoft Corporation. All rights reserved.

#ifndef __NSViewsAfxExt_h__
#define __NSViewsAfxExt_h__

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#pragma warning(disable: 4275)
#pragma warning(disable: 4251)

#ifdef BUILD_NSVIEWS
#define AFX_EXT_NSVIEWS AFX_CLASS_EXPORT
#else
#define AFX_EXT_NSVIEWS AFX_CLASS_IMPORT
#endif

#endif

