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

 srchdde.h

 - This file contains enumerated data types and constants necessary
   for communication with the search plug-in.

*********************************************************************/

#ifndef UIDDE_H
#define UIDDE_H

#define ASInt16 short int
#define ASInt32 long int
#define ASUns16 unsigned short int
#define ASUns32 unsigned long int
#define ASBool ASUns16

typedef enum {
	QLangType_Simple,
	QLangType_CQL,
	QLangType_Passthrough
} eQLangType;

// Word Options.
#define QPON_Case				(1 << 0)	// Option: Case sensitive.
#define QPON_Stemming		(1 << 1)	// Option: Stemming.
#define QPON_SoundsLike		(1 << 2)	// Option: Sounds like.
#define QPON_Thesaurus		(1 << 3)	// Option: Thesaurus.
#define QPON_Proximity		(1 << 4)	// Option: Proximity.
#define QPON_Refine			(1 << 5)	// Option: Refine search.

#define QP_NUM_WORD_OPTIONS	6
#define QP_MAX_SORT_FIELDS		16

typedef struct _QueryData
{
	eQLangType qlt;
	ASBool bOverrideWordOptions;
	ASUns32 nWordOptions;
	ASUns16 nMaxDocs;
	ASUns16 nQueryOffset;
	ASUns16 nNumSorts;
	ASUns16 nSortOffset[QP_MAX_SORT_FIELDS];
	ASBool bSortWays[QP_MAX_SORT_FIELDS];
	unsigned char cData[1];
} QueryData;

typedef enum {
	IndexAction_Add,
	IndexAction_Remove,
	IndexAction_Enable,
	IndexAction_Disable
} IndexActionType;

typedef struct _IndexData
{
	IndexActionType eAction;	// Add, Remove, Enable, Disable.
	ASInt16 nIndexOffset;		// Offset to *.PDX name of index.
	ASInt16 nTempNameOffset;	// Offset to temp name if index down.
	unsigned char cData[1];		// Data.
} IndexData;

#endif
