//////////////////////////////////////////////////////////////////////
// PictureEx.cpp: implementation of the CPictureEx class.
//
// Picture displaying control with support for the following formats:
// GIF (including animated GIF87a and GIF89a), JPEG, BMP, WMF, ICO, CUR
// 
// Written by Oleg Bykov (oleg_bykoff@rsdn.ru)
// Copyright (c) 2001
//
// To use CPictureEx, follow these steps:
//   - place a static control on your dialog (either a text or a bitmap)
//   - change its identifier to something else (e.g. IDC_MYPIC)
//   - associate a CStatic with it using ClassWizard
//   - in your dialog's header file replace CStatic with CPictureEx
//     (don't forget to #include "PictureEx.h" and add 
//     PictureEx.h and PictureEx.cpp to your project)
//   - call one of the overloaded CPictureEx::Load() functions somewhere
//     (OnInitDialog is a good place to start)
//   - if the preceding Load() succeeded call Draw()
//  
// You can also add the control by defining a member variable of type 
// CPictureEx, calling CPictureEx::Create (derived from CStatic), then 
// CPictureEx::Load and CPictureEx::Draw.
//
// By default, the control initializes its background to COLOR_3DFACE
// (see CPictureEx::PrepareDC()). You can change the background by
// calling CPictureEx::SetBkColor(COLORREF) after CPictureEx::Load().
//
// I decided to leave in the class the functions to write separate frames from 
// animated GIF to disk. If you want to use them, uncomment #define GIF_TRACING 
// and an appropriate section in CPictureEx::Load(HGLOBAL, DWORD). These functions 
// won't be compiled and linked to your project unless you uncomment #define GIF_TRACING,
// so you don't have to worry.
// 
// Warning: this code hasn't been subject to a heavy testing, so
// use it on your own risk. The author accepts no liability for the 
// possible damage caused by this code.
//
// Version 1.0  7 Aug 2001
//              Initial release
//
// Version 1.1  6 Sept 2001
//              ATL version of the class
//
// Version 1.2  14 Oct 2001
//              - Fixed a problem with loading GIFs from resources
//                in MFC-version of the class for multi-modules apps.
//                Thanks to Ruben Avila-Carretero for finding this out.
//
//              - Got rid of waitable timer in ThreadAnimation()
//                Now CPictureEx[Wnd] works in Win95 too.
//                Thanks to Alex Egiazarov and Wayne King for the idea.
//
//              - Fixed a visual glitch of using SetBkColor.
//                Thanks to Kwangjin Lee for finding this out.
//
// Version 1.3  10 Nov 2001
//              - Fixed a DC leak. One DC leaked per each UnLoad()
//                (forgot to put a ReleaseDC() in the end of 
//                CPictureExWnd::PrepareDC() function).
//
//              - Now it is possible to set a clipping rectangle using
//                CPictureEx[Wnd]::SetPaintRect(const LPRECT) function.
//                The LPRECT parameter tells the class what portion of
//                a picture should it display. If the clipping rect is 
//                not set, the whole picture is shown.
//                Thanks to Fabrice Rodriguez for the idea.
//
//              - Added support for Stop/Draw. Now you can Stop() an
//                animated GIF, then Draw() it again, it will continue
//                animation from the frame it was stopped on. You can 
//                also know if a GIF is currently playing with the 
//                IsPlaying() function.
//             
//              - Got rid of math.h and made m_bExitThread volatile. 
//                Thanks to Piotr Sawicki for the suggestion.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTUREEX_H__0EFE5DE0_7B68_4DB7_8B34_5DC634948438__INCLUDED_)
#define AFX_PICTUREEX_H__0EFE5DE0_7B68_4DB7_8B34_5DC634948438__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

//#define GIF_TRACING  // uncomment it if you want detailed TRACEs

//##ModelId=49B87B950271
class CPictureEx : public CStatic
{
public:

	//##ModelId=49B87B960004
struct TFrame    // structure that keeps a single frame info
{
		//##ModelId=49B87B960011
	IPicture *m_pPicture;  // pointer to the interface used for drawing
		//##ModelId=49B87B960016
	SIZE     m_frameSize;
		//##ModelId=49B87B96001B
	SIZE     m_frameOffset;
		//##ModelId=49B87B96001F
	UINT     m_nDelay;     // delay (in 1/100s of a second)
		//##ModelId=49B87B960020
	UINT     m_nDisposal;  // disposal method
};

#pragma pack(1)   // turn byte alignment on

	//##ModelId=49B87B96002E
enum GIFBlockTypes
{
		//##ModelId=49B87B960030
	BLOCK_UNKNOWN,
		//##ModelId=49B87B960031
	BLOCK_APPEXT,
		//##ModelId=49B87B960032
	BLOCK_COMMEXT,
		//##ModelId=49B87B96003E
	BLOCK_CONTROLEXT,
		//##ModelId=49B87B96003F
	BLOCK_PLAINTEXT,
		//##ModelId=49B87B960040
	BLOCK_IMAGE,
		//##ModelId=49B87B960041
	BLOCK_TRAILER
};

	//##ModelId=49B87B96004E
enum ControlExtValues // graphic control extension packed field values
{
		//##ModelId=49B87B960050
	GCX_PACKED_DISPOSAL,  // disposal method
		//##ModelId=49B87B960051
	GCX_PACKED_USERINPUT,
		//##ModelId=49B87B96005D
	GCX_PACKED_TRANSPCOLOR
};

	//##ModelId=49B87B96005E
enum LSDPackedValues  // logical screen descriptor packed field values
{
		//##ModelId=49B87B96006D
	LSD_PACKED_GLOBALCT,
		//##ModelId=49B87B96006E
	LSD_PACKED_CRESOLUTION,
		//##ModelId=49B87B96006F
	LSD_PACKED_SORT,
		//##ModelId=49B87B960070
	LSD_PACKED_GLOBALCTSIZE
};

	//##ModelId=49B87B96007D
enum IDPackedValues   // image descriptor packed field values
{
		//##ModelId=49B87B96007F
	ID_PACKED_LOCALCT,
		//##ModelId=49B87B960080
	ID_PACKED_INTERLACE,
		//##ModelId=49B87B960081
	ID_PACKED_SORT,
		//##ModelId=49B87B96008C
	ID_PACKED_LOCALCTSIZE
};

	//##ModelId=49B87B96008D
struct TGIFHeader       // GIF header  
{
		//##ModelId=49B87B96008F
	char m_cSignature[3]; // Signature - Identifies the GIF Data Stream
						  // This field contains the fixed value 'GIF'
		//##ModelId=49B87B960090
	char m_cVersion[3];	// Version number. May be one of the following:
						// "87a" or "89a"
};

	//##ModelId=49B87B96009C
struct TGIFLSDescriptor // Logical Screen Descriptor
{
		//##ModelId=49B87B96009E
	WORD m_wWidth;	// 2 bytes. Logical screen width
		//##ModelId=49B87B96009F
	WORD m_wHeight; // 2 bytes. Logical screen height

		//##ModelId=49B87B9600AB
	unsigned char m_cPacked;      // packed field	

		//##ModelId=49B87B9600AC
	unsigned char m_cBkIndex;     // 1 byte. Background color index
		//##ModelId=49B87B9600AD
	unsigned char m_cPixelAspect; // 1 byte. Pixel aspect ratio
		//##ModelId=49B87B9600AE
	inline int GetPackedValue(enum LSDPackedValues Value);
};

	//##ModelId=49B87B9600CB
struct TGIFAppExtension // application extension block
{
		//##ModelId=49B87B9600CD
	unsigned char m_cExtIntroducer; // extension introducer (0x21)
		//##ModelId=49B87B9600CE
	unsigned char m_cExtLabel; // app. extension label (0xFF)
		//##ModelId=49B87B9600CF
	unsigned char m_cBlockSize; // fixed value of 11
		//##ModelId=49B87B9600DA
	char m_cAppIdentifier[8];   // application identifier
		//##ModelId=49B87B9600DB
	char m_cAppAuth[3];  // application authentication code
};

	//##ModelId=49B87B9600DC
struct TGIFControlExt // graphic control extension block
{
		//##ModelId=49B87B9600EB
	unsigned char m_cExtIntroducer; // extension introducer (0x21)
		//##ModelId=49B87B9600EC
	unsigned char m_cControlLabel;  // control extension label (0xF9)
		//##ModelId=49B87B9600ED
	unsigned char m_cBlockSize; // fixed value of 4
		//##ModelId=49B87B9600EE
	unsigned char m_cPacked;    // packed field
		//##ModelId=49B87B9600EF
	WORD m_wDelayTime;	// delay time
		//##ModelId=49B87B9600FA
	unsigned char m_cTColorIndex; // transparent color index
		//##ModelId=49B87B9600FB
	unsigned char m_cBlockTerm;   // block terminator (0x00)
public:
		//##ModelId=49B87B9600FC
	inline int GetPackedValue(enum ControlExtValues Value);
};

	//##ModelId=49B87B960119
struct TGIFCommentExt  // comment extension block
{
		//##ModelId=49B87B96011B
	unsigned char m_cExtIntroducer; // extension introducer (0x21)
		//##ModelId=49B87B96011C
	unsigned char m_cCommentLabel;  // comment extension label (0xFE)
};

	//##ModelId=49B87B960128
struct TGIFPlainTextExt // plain text extension block
{
		//##ModelId=49B87B96012A
	unsigned char m_cExtIntroducer;  // extension introducer (0x21)
		//##ModelId=49B87B96012B
	unsigned char m_cPlainTextLabel; // text extension label (0x01)
		//##ModelId=49B87B96012C
	unsigned char m_cBlockSize; // fixed value of 12
		//##ModelId=49B87B960138
	WORD m_wLeftPos;    // text grid left position
		//##ModelId=49B87B960139
	WORD m_wTopPos;     // text grid top position
		//##ModelId=49B87B96013A
	WORD m_wGridWidth;  // text grid width
		//##ModelId=49B87B96013B
	WORD m_wGridHeight; // text grid height
		//##ModelId=49B87B96013C
	unsigned char m_cCellWidth;  // character cell width
		//##ModelId=49B87B960148
	unsigned char m_cCellHeight; // character cell height
		//##ModelId=49B87B960149
	unsigned char m_cFgColor; // text foreground color index
		//##ModelId=49B87B96014A
	unsigned char m_cBkColor; // text background color index
};

	//##ModelId=49B87B96014B
struct TGIFImageDescriptor // image descriptor block
{
		//##ModelId=49B87B960158
	unsigned char m_cImageSeparator; // image separator byte (0x2C)
		//##ModelId=49B87B960159
	WORD m_wLeftPos; // image left position
		//##ModelId=49B87B96015A
	WORD m_wTopPos;  // image top position
		//##ModelId=49B87B96015B
	WORD m_wWidth;   // image width
		//##ModelId=49B87B96015C
	WORD m_wHeight;  // image height
		//##ModelId=49B87B960167
	unsigned char m_cPacked; // packed field
		//##ModelId=49B87B960168
	inline int GetPackedValue(enum IDPackedValues Value);
};

#pragma pack() // turn byte alignment off

public:
	//##ModelId=49B87B950273
	BOOL GetPaintRect(RECT *lpRect);
	//##ModelId=49B87B950275
	BOOL SetPaintRect(const RECT *lpRect);
	//##ModelId=49B87B950277
	CPictureEx();
	//##ModelId=49B87B950280
	virtual ~CPictureEx();
	//##ModelId=49B87B950291
	void Stop();   // stops animation
	//##ModelId=49B87B95029F
	void UnLoad(); // stops animation plus releases all resources

	//##ModelId=49B87B9502A0
	BOOL IsGIF() const;
	//##ModelId=49B87B9502A2
	BOOL IsPlaying() const;
	//##ModelId=49B87B9502B0
	BOOL IsAnimatedGIF() const;
	//##ModelId=49B87B9502B2
	SIZE GetSize() const;
	//##ModelId=49B87B9502B4
	int GetFrameCount() const;
	//##ModelId=49B87B9502BF
	COLORREF GetBkColor() const;
	//##ModelId=49B87B9502C1
	void SetBkColor(COLORREF clr);

	// draws the picture (starts an animation thread if needed)
	// if an animation was previously stopped by Stop(),
	// continues it from the last displayed frame
	//##ModelId=49B87B9502C3
	BOOL Draw();

	// loads a picture from a file
	// i.e. Load(_T("mypic.gif"));
	//##ModelId=49B87B9502C4
	BOOL Load(LPCTSTR szFileName);

	// loads a picture from a global memory block (allocated by GlobalAlloc)
	// Warning: this function DOES NOT free the global memory, pointed to by hGlobal
	//##ModelId=49B87B9502CF
	BOOL Load(HGLOBAL hGlobal, DWORD dwSize);

	// loads a picture from a program resource
	// i.e. Load(MAKEINTRESOURCE(IDR_MYPIC),_T("GIFTYPE"));
	//##ModelId=49B87B9502DE
	BOOL Load(LPCTSTR szResourceName,LPCTSTR szResourceType);

protected:

#ifdef GIF_TRACING
	//##ModelId=49B87B9502EE
	void EnumGIFBlocks();
	//##ModelId=49B87B9502EF
	void WriteDataOnDisk(CString szFileName, HGLOBAL hData, DWORD dwSize);
#endif // GIF_TRACING

	//##ModelId=49B87B9502F4
	RECT m_PaintRect;
	//##ModelId=49B87B950301
	SIZE m_PictureSize;
	//##ModelId=49B87B95030D
	COLORREF m_clrBackground;
	//##ModelId=49B87B95030E
	UINT m_nCurrFrame;
	//##ModelId=49B87B95030F
	UINT m_nDataSize;
	//##ModelId=49B87B95031C
	UINT m_nCurrOffset;
	//##ModelId=49B87B95031D
	UINT m_nGlobalCTSize;
	//##ModelId=49B87B95031E
	BOOL m_bIsGIF;
	//##ModelId=49B87B95032C
	BOOL m_bIsPlaying;
	//##ModelId=49B87B95032D
	volatile BOOL m_bExitThread;
	//##ModelId=49B87B95032E
	BOOL m_bIsInitialized;
	//##ModelId=49B87B95033C
	HDC m_hMemDC;

	//##ModelId=49B87B95033D
	HDC m_hDispMemDC;
	//##ModelId=49B87B95034B
	HBITMAP m_hDispMemBM;
	//##ModelId=49B87B95034C
	HBITMAP m_hDispOldBM;

	//##ModelId=49B87B95034D
	HBITMAP m_hBitmap;
	//##ModelId=49B87B95035B
	HBITMAP m_hOldBitmap;
	//##ModelId=49B87B95035C
	HANDLE m_hThread;
	//##ModelId=49B87B95036B
	HANDLE m_hExitEvent;
	//##ModelId=49B87B95036D
	IPicture * m_pPicture;
	//##ModelId=49B87B95037B
	TGIFHeader * m_pGIFHeader;
	//##ModelId=49B87B95037F
	unsigned char * m_pRawData;
	//##ModelId=49B87B950381
	TGIFLSDescriptor * m_pGIFLSDescriptor;
	//##ModelId=49B87B950390
	std::vector<TFrame> m_arrFrames;

	//##ModelId=49B87B950399
	void ThreadAnimation();
	//##ModelId=49B87B95039A
	static UINT WINAPI _ThreadAnimation(LPVOID pParam);

	//##ModelId=49B87B95039D
	int GetNextBlockLen() const;
	//##ModelId=49B87B9503AA
	BOOL SkipNextBlock();
	//##ModelId=49B87B9503AB
	BOOL SkipNextGraphicBlock();
	//##ModelId=49B87B9503B9
	BOOL PrepareDC(int nWidth, int nHeight);
	//##ModelId=49B87B9503BC
	void ResetDataPointer();
	//##ModelId=49B87B9503C8
	enum GIFBlockTypes GetNextBlock() const;
	//##ModelId=49B87B9503CA
	UINT GetSubBlocksLen(UINT nStartingOffset) const;
	//##ModelId=49B87B9503D8
	HGLOBAL GetNextGraphicBlock(UINT *pBlockLen, UINT *pDelay, 
		SIZE *pBlockSize, SIZE *pBlockOffset, UINT *pDisposal);

	// Generated message map functions
	//{{AFX_MSG(CPictureEx)
	//##ModelId=49B87B960000
	afx_msg void OnDestroy();
	//##ModelId=49B87B960002
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PICTUREEX_H__0EFE5DE0_7B68_4DB7_8B34_5DC634948438__INCLUDED_)
