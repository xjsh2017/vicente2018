// GifAnimation.cpp : implementation file
//

//#include "stdafx.h"
/*#include "AnimatedGif.h"*/
#include "GifAnimation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGifAnimation

CGifAnimation::CGifAnimation()
{
	pAnimation=NULL;
	CurrentLoop=0;
	CurrentImage=0;
	bAnimationPlaying=false;
}

CGifAnimation::~CGifAnimation()
{
}


BEGIN_MESSAGE_MAP(CGifAnimation, CStatic)
	//{{AFX_MSG_MAP(CGifAnimation)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGifAnimation message handlers
int CGifAnimation::LoadAnimatedGif(LPTSTR FileName)
{
	int Result=AnimGif.LoadGIF(FileName);
	pAnimation=&AnimGif;

	return Result;
}

void CGifAnimation::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	//for (int i=0;i<=CurrentImage;i++)
	{
		C_Image* & current = pAnimation->img[CurrentImage/*i*/];
		current->GDIPaint(dc,current->xPos,current->yPos);
	}	
}

// fnThread: Thread function in charge of looping animation frames.
DWORD WINAPI CGifAnimation::fnThread (LPVOID lpParameter)
{
	CGifAnimation* window;
	C_ImageSet* anim;
	window=(CGifAnimation*) lpParameter;
	anim=window->pAnimation;
	window->bAnimationPlaying=TRUE;
	while (	/*anim->nLoops ? window->CurrentLoop < anim->nLoops : true*/ window->CurrentLoop<1)
	{
		while (1)
		{
			::InvalidateRect(window->m_hWnd,NULL,FALSE);

			C_Image* & image = anim->img[window->CurrentImage];
			Sleep (image->Delay?image->Delay:100);

			if (window->CurrentImage < anim->nImages-1)
				++window->CurrentImage;
			else
				{window->CurrentImage=0; break;}
			// CurrentImage must always be valid!

		} 
		++window->CurrentLoop;
	}
	window->Rewind();
	window->bAnimationPlaying=FALSE;
	return 0;
}

// Play: Start/Resume animation loop
void CGifAnimation::Play ()
{
	if (!bAnimationPlaying)
		if (pAnimation->nImages > 1)
			hThreadAnim = CreateThread(NULL,0,fnThread,this,0,&dwThreadIdAnim);
}

// Stop: Stop animation loop
void CGifAnimation::Stop ()
{
	if (bAnimationPlaying)
	{
		TerminateThread(hThreadAnim,0);
		bAnimationPlaying=FALSE;
	}
}

// Rewind: Reset animation loop to its initial values
void CGifAnimation::Rewind ()
{
	CurrentLoop=0;
	CurrentImage=0;
}

BOOL CGifAnimation::IsPlaying()
{
	return bAnimationPlaying;
}

