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

 iac.h

 - Contains enumerated data types and constants necessary for some of
   the OLE method parameters.

*********************************************************************/

typedef long int ASInt32;

/* AV Things */

/* AV Application Preferences data type */
typedef enum _t_APrefsType {
	avpPrefsVersion,			/* Int32 */
	avpOpenDialogAtStartup,		/* boolean */
	avpShowSplashAtStartup,		/* boolean */
	avpShowToolBar,				/* boolean */
	avpRememberDialogs,			/* boolean */
	avpShortMenus,				/* boolean */
	avpDefaultOverviewType,		/* Int32 */
	avpDefaultZoomScale,		/* Fixed */
	avpDefaultZoomType,			/* Int32 */
	avpShowLargeImages,			/* boolean */
	avpGreekText,				/* boolean */
	avpGreekLevel,				/* Int32 */
	avpSubstituteFontType,		/* Int32 */
    avpDoCalibratedColor,		/* boolean */
    avpSkipWarnings,			/* boolean */
	avpPSLevel,					/* Int32 */
	avpShrinkToFit,				/* boolean */
	avpCaseSensitive,			/* boolean */
	avpWholeWords,				/* boolean */
	avpNoteColor,				/* PDColorValue */
	avpNoteLabel,				/* ###cannot use via IAC */
	avpMaxThreadZoom,			/* Fixed */
	avpEnablePageCache,			/* boolean */
	avpFullScreenColor,			/* PDColorValue */
	avpUnused1,					/* obsolete */
	avpMaxPageCacheZoom,		/* Fixed: pages whose zoom > this will not be cached */
	avpMinPageCacheTicks,		/* Int32: pages which take < this to render will not be cached */
	avpMaxPageCacheBytes,		/* Int32: overall constraint on storage used by cache */
	avpUnused2,					/* obsolete */
	avpFullScreenChangeTimeDelay,/* Int32 */
	avpFullScreenLoop,			/* boolean */
	avpThumbViewScale,			/* Fixed */
	avpThumbViewTimeout,		/* Int32 */
	avpDestFitType,				/* ###cannot use via IAC */
	avpDestZoomInherit,			/* boolean */
	avpHighlightMode,			/* Int32 */
	avpDefaultSplitterPos,		/* Int32 */
	avpUnused3,					/* obsolete */
	avpMaxCosDocCache,			/* Int32: max # of bytes of Cos object cache per Cos doc. */
	avpPageUnits,				/* PageUnits */
	avpNoteFontName,			/* ###cannot use via IAC*/
	avpNoteFontSize,			/* Int32 */
	avpRecentFile1,				/* ###cannot use via IAC */
	avpRecentFile2,				/* ###cannot use via IAC */
	avpRecentFile3,				/* ###cannot use via IAC */
	avpRecentFile4,				/* ###cannot use via IAC */
	avpHighlightColor,			/* PDColorValue */
	avpFullScreenUseTimer,		/* boolean */
	avpAntialiasText,			/* boolean */
	avpAntialiasLevel,			/* Int16 */
	avpPersistentCacheSize,		/* Int32 */
	avpPersistentCacheFolder,	/* ASPathName */
	avpPageViewLayoutMode,		/* PDLayoutMode */
	avpSaveAsLinearized,		/* boolean */
	avpMaxOpenDocuments,		/* Int32 */
	avpTextSelectWordOrder,		/* Int32 */
	avpMarkHiddenPages,			/* boolean */
	avpFullScreenTransitionType,/* ###cannot use via IAC* */
	avpFullScreenClick,			/* boolean */
	avpFullScreenEscape,		/* boolean */
	avpFullScreenCursor,		/* Int16 */
	avpOpenInPlace,				/* boolean */
	avpShowHiddenAnnots,		/* boolean */
	avpFullScreenUsePageTiming,	/* boolean */
	avpDownloadEntireFile,		/* boolean */
	avpEmitHalftones,			/* boolean */
	avpShowMenuBar,				/*boolean*/
	avpIgnorePageClip,			/*boolean*/
	avpMinimizeBookmarks,		/* boolean*/
	avpShowAnnotSequence,		/*boolean*/
	avpUseLogicalPageNumbers,	/*boolean*/
	avpASExtensionDigCert,		/*###cannot use via IAC*/
	avpShowLeftToolBar,			/*boolean*/
	avpConfirmOpenFile,			/*boolean*/
	avpNoteLabelEncoding,		/*cannot use via IAC*/
	avpBookmarkShowLocation,	/*boolean*/
	avpUseLocalFonts,			/*boolean*/
	avpCurrCMM,					/*char* */
	avpBrowserIntegration,		/*boolean*/
	avpPrintAnnots,				/*boolean */
	avpSendFarEastFonts,		/*boolean */
	avpSuppressCSA,				/*boolean */
} AVPrefsType;

/* AVZoomType -- Variable zoom "verbs", corresponding to View menu items */
typedef enum _t_AVZoomType {
	AVZoomNoVary,					/* no variable zoom */
	AVZoomFitPage,					/* fit page to window */
	AVZoomFitWidth,					/* fit page width to window */
	AVZoomFitHeight,				/* fit page height to window */
	AVZoomFitVisibleWidth,			/* fit visible width to window */
	AVZoomPreferred					/* use page's preferred zoom */
} AVZoomType;

typedef enum _t_AVViewType {
	AV_EXTERNAL_VIEW = 1,	/* Open the document with tool bar visible */
	AV_DOC_VIEW = 2,		/* Draw the page pane and scrollbars */
	AV_PAGE_VIEW = 4		/* Draw only the page pane */
} AVViewType;

/* PD Things */

/* PDPageMode -- Variable for how the file opens - bookmarks, thumbnails, full screen, none */
typedef enum {
		PDDontCare,
		PDUseNone,
		PDUseThumbs,
		PDUseBookmarks,
		PDFullScreen
} PDPageMode;

/* PDLayoutMode -- Variable for how the file is opened - single page, one column, two column */
enum {
	PDLayoutDontCare,
	PDLayoutSinglePage,
	PDLayoutOneColumn,
	PDLayoutTwoColumnLeft,
	PDLayoutTwoColumnRight
};

/*
** PDDocFlags -- used by PDDocGetFlags()/SetFlags().
** These flags indicate the state of the document and whether it needs
** to be saved or deleted on close, etc.
** The flags are used as a bit field. More than one value may be set.
** Some flags may be set or get only. Most can be either set or get.
*/
typedef enum {
	PDDocNeedsSave			= 0x0001,	/* document has been modified and needs saving (get/set) */
	PDDocRequiresFullSave	= 0x0002,	/* document cannot be saved incrementally; must
											   be written with PDSaveFull. (get/set)
										*/
	PDDocIsModified			= 0x0004,	/* document has been modified internally, such as
										   opening/closing bookmarks and annotations, but
										   not in such a way as to warrant saving.
										   (get only)
										*/
	PDDocDeleteOnClose		= 0x0008,	/* document is based on a temporary file which must
										   be deleted when the document is closed or saved.
										   (get/set)
										*/
	PDDocWasRepaired		= 0x0010,	/* document was repaired when opened (get only) */
	PDDocNewMajorVersion	= 0x0020,	/* document major version newer than current (get only) */
	PDDocNewMinorVersion	= 0x0040,	/* document minor version newer than current (get only) */
	PDDocOldVersion			= 0x0080,	/* document version older than current (get only) */
	PDDocSuppressErrors		= 0x0100,	/* don't display errors (get/set) */
	PDDocIsEmbedded			= 0x0200,	/* document is embedded in a compound doc (OLE, OpenDoc) */
	PDDocIsLinearized		= 0x0400,	/* document is linearized (get only) */
	PDDocIsOptimized		= 0x0800	/* document is optimized */
} PDDocFlags;

/* PDInsertFlags -- used for inserting pages */
typedef enum {
	PDInsertBookmarks		= 0x0001,	/* insert bookmarks as well as pages */
	PDInsertAll				= 0x1000,	/* insert all Catalog and Info dict values as well as pages */
	PDInsertThreads			= 0x0002	/* insert articles as well */
} PDInsertFlags;

/* Page specification defines -- use where a page number or range or count is required */
#define PDBeforeFirstPage	((ASInt32) -1)
#define PDLastPage			((ASInt32) -2)
#define PDAllPages			((ASInt32) -3)
#define PDOddPagesOnly      ((ASInt32) -4)
#define PDEvenPagesOnly     ((ASInt32) -5)

/* PDSaveFlags -- used for PD-level Save
** All undefined flags should be set to zero.
** If either PDSaveCollectGarbage or PDSaveCopy are used, PDSaveFull must be used.
*/
typedef enum {
	PDSaveIncremental	= 0x0000,	/* write changes only */
	PDSaveFull			= 0x0001,	/* write entire file */
	PDSaveCopy			= 0x0002,	/* write copy w/o affecting current state */
	PDSaveLinearized	= 0x0004,	/* writes the file linearized for
											** page-served remote (net) access.
											*/
	PDSaveBinaryOK		= 0x0010,	/* OK to store binary in file */
	PDSaveCollectGarbage = 0x0020	/* perform garbage collection on unreferenced objects */
} PDSaveFlags;

/* Enum for routines that set/get the value of a page's Rotate key. */
typedef enum {
	pdRotate0	= 0,
	pdRotate90	= 90,
	pdRotate180	= 180,
	pdRotate270	= 270
} PDRotate;
