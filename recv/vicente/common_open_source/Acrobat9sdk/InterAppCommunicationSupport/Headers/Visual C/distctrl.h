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

 distctrl.h

 - This include file contains enumerated data types and constants
   necessary for using the Windows messaging control interface to
   Acrobat Distiller.

*********************************************************************/

#define FILENAMESIZE	_MAX_PATH
#define MAXSTRINGSIZE	260					/* 256 + extra for command line */
#define MAXFOLDERS  	100
#define ID_TIMER 		1
#define DM_CMDLINE      0x4C646D43
#define DM_DISTILL		0x44696E73
#define DM_DONE			0x64616C65

typedef enum {
	EQ_NO_SAVE_DIALOG,
	EQ_DEFAULT_OLD_DEST,
	EQ_DEFAULT_SOURCE
} EnqueueOptions;

typedef struct _DISTILLRECORD {
	long int param;
	char fileList[260];			/* pathname or list of pathnames (comma delimited) */
	char outputFile[MAX_PATH];	/* output file or output directory for list */
} DISTILLRECORD, *PDISTILLRECORD;
