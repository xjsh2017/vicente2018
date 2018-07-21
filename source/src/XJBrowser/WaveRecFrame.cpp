// WaveRecFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "WaveRecFrame.h"

// #include "WaveRecGeneral.h"
// #include "WaveRecOSC.h"
// #include "WaveRecSetting.h"
// #include "PTOSCList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaveRecFrame

IMPLEMENT_DYNCREATE(CWaveRecFrame, CMDIChildWnd)

//##ModelId=49B87B810002
CWaveRecFrame::CWaveRecFrame()
{
	/** @brief           ¼������������ҳ��ͼָ��*/
	m_pWRGeneral = NULL;
	/** @brief           ¼����¼������ҳ��ͼָ��*/
	m_pWROSC = NULL;
	/** @brief           ¼������ֵ����ҳ��ͼָ��*/
	m_pWRSetting = NULL;
	m_pWROSCList = NULL;
	m_pWRDigital = NULL;
	m_pWRAnalog = NULL;
	m_bSetMenu = FALSE;
	m_pWRNote = NULL;

	m_pCurSec = NULL;
}

//##ModelId=49B87B81003F
CWaveRecFrame::~CWaveRecFrame()
{
}


BEGIN_MESSAGE_MAP(CWaveRecFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CWaveRecFrame)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20008, OnSTTP20008)
	ON_MESSAGE(WM_STTP_20012, OnSTTP20012)
	ON_MESSAGE(WM_STTP_20016, OnSTTP20016)
	ON_MESSAGE(WM_STTP_20060, OnSTTP20060)
	ON_MESSAGE(WM_STTP_20124, OnSTTP20124)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20026, OnSTTP20026)
	ON_MESSAGE(WM_STTP_20043, OnSTTP20043)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_STTP_20086, OnSTTP20086)
	ON_MESSAGE(WM_STTP_20107, OnSTTP20107)
	ON_MESSAGE(WM_STTP_20048, OnSTTP20048)
	ON_MESSAGE(WM_STTP_20052, OnSTTP20052)
	ON_MESSAGE(WM_STTP_20054, OnSTTP20054)
	ON_MESSAGE(WM_STTP_20056, OnSTTP20056)
	ON_MESSAGE(WM_STTP_20170, OnSTTP20170)
	ON_MESSAGE(WM_STTP_20172, OnSTTP20172)
	ON_MESSAGE(WM_STTP_20174, OnSTTP20174)
	ON_MESSAGE(WM_WRFRAME_OPEN, OnOpenWRProp)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(REFRESH_PTSETTING, OnRefreshPTSetting)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveRecFrame message handlers

//##ModelId=49B87B810027
void CWaveRecFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_bSetMenu)
	{
		CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
		pApp->XJSetMenuInfo();
		m_bSetMenu = TRUE;
	}
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

//##ModelId=49B87B810030
BOOL CWaveRecFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	return CMDIChildWnd::PreCreateWindow(cs);
	//��ֹ��˸
	//�õ���Ļ�ֱ���
	int   nSx   =   GetSystemMetrics(SM_CXFULLSCREEN);     //��   
	int   nSy   =   GetSystemMetrics(SM_CYFULLSCREEN);     //�� 
	
	cs.style = WS_CHILD | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		/*| FWS_ADDTOTITLE*/ | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX 
		| WS_MAXIMIZE;  //�Ѵ�����ʽ����Ϊ��󻯣����Ȳ���ʾ
	
	cs.cx = nSx;   //�Ѵ��ڴ�С����Ϊ������Ļ��С
	cs.cy = nSy;
	
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	
	cs.style |= WS_VISIBLE;  //�������ֻ֮������ʾ����
	
	return TRUE;
}

//##ModelId=49B87B810041
void CWaveRecFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

//##ModelId=49B87B810033
BOOL CWaveRecFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(!CMDIChildWnd::OnCreateClient(lpcs, pContext))
	{
		return FALSE;
	}

	m_mapIndex.RemoveAll();

	//�ҵ�SHEETҳ
	CWnd* pWnd = GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE); 
	if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CPTSheet))) 
	{ 
		CPTSheet* pView = (CPTSheet*)pWnd; 
		m_pTabView = pView; 
		m_pTabView->m_nType = 1;

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
		
		int nIndex = 0;
		//������ͼ
		if(g_WRShowGeneral == 1)
		{
			nIndex = pView->Insert(WR_PROP_GENERAL, StringFromID(IDS_SECPROP_GENERAL), RUNTIME_CLASS(CWaveRecGeneral), pContext); 
			if(nIndex == -1)
			{ 
				return FALSE; 
			}
			//������ͼָ��
			m_pWRGeneral = (CWaveRecGeneral*)pView->GetView(nIndex);
			m_mapIndex.SetAt(WR_PROP_GENERAL, nIndex);
		}
		
		if(g_WRShowSetting == 1)
		{
			nIndex = pView->Insert(WR_PROP_SETTING, StringFromID(IDS_SECPROP_SETTING), RUNTIME_CLASS(CPTSetting), pContext); 
			if(nIndex == -1)
			{ 
				return FALSE; 
			}
			//������ͼָ��
			m_pWRSetting = (CPTSetting*)pView->GetView(nIndex);
			//��¼���ڻص�������ָ��
			pApp->m_pWRSetting = m_pWRSetting;
			m_mapIndex.SetAt(WR_PROP_SETTING, nIndex);
		}

		if(g_WRShowDI == 1)
		{
			nIndex = pView->Insert(WR_PROP_DIGITAL, StringFromID(IDS_SECPROP_DIGITAL), RUNTIME_CLASS(CPTDigital), pContext);
			if(nIndex == -1)
			{ 
				return FALSE; 
			} 
			//������ͼָ��
			m_pWRDigital = (CPTDigital*)pView->GetView(nIndex);
			//��¼���ڻص�������ָ��
			pApp->m_pWRDigital = m_pWRDigital;
			m_mapIndex.SetAt(WR_PROP_DIGITAL,nIndex);
		}

		if(g_WRShowAI == 1)
		{
			nIndex = pView->Insert(WR_PROP_ANALOG, StringFromID(IDS_SECPROP_ANALOG), RUNTIME_CLASS(CPTAnalog), pContext);
			if(nIndex == -1)
			{ 
				return FALSE; 
			} 
			//������ͼָ��
			m_pWRAnalog = (CPTAnalog*)pView->GetView(nIndex);
			//��¼���ڻص�������ָ��
			pApp->m_pWRAnalog = m_pWRAnalog;
			m_mapIndex.SetAt(WR_PROP_ANALOG, nIndex);
		}

		if(g_WRShowOsc == 1)
		{
			nIndex = pView->Insert(WR_PROP_OSC, StringFromID(IDS_SECPROP_LOCALFILE), RUNTIME_CLASS(CPTOSC), pContext);
			if(nIndex == -1)
			{ 
				return FALSE; 
			} 
			//������ͼָ��
			m_pWROSC = (CPTOSC*)pView->GetView(nIndex);
			m_mapIndex.SetAt(WR_PROP_OSC, nIndex);
		}

		if(g_WRShowOscList == 1)
		{
			nIndex = pView->Insert(WR_PROP_OSCLIST, StringFromID(IDS_SECPROP_REMOTEFILE), RUNTIME_CLASS(CPTOSCList), pContext);
			if(nIndex == -1)
			{
				return FALSE;
			}
			//������ͼָ��
			m_pWROSCList = (CPTOSCList*)pView->GetView(nIndex);
			m_mapIndex.SetAt(WR_PROP_OSCLIST, nIndex);
		}

		//��עҳ��
		if(g_WRShowNote == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_NOTE), RUNTIME_CLASS(CPTNote), pContext);
			if(nIndex == -1)
				return FALSE;
			//��¼ָ��
			m_pWRNote = (CPTNote*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_NOTE, nIndex);
		}

		// activate the input view
		SetActiveView(pView);
	}
	return TRUE;
}

/*************************************************************
 �� �� ����OnSTTP20016()
 ���ܸ�Ҫ��ת����ֵ����֪ͨ����(20016)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B81004F
void CWaveRecFrame::OnSTTP20016( WPARAM wParam, LPARAM lParam )
{
	//��ֵ����֪ͨ, ת������ֵ����ҳ
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20016, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20060()
 ���ܸ�Ҫ��ת��������ʱ֪ͨ(20060)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B810052
void CWaveRecFrame::OnSTTP20060( WPARAM wParam, LPARAM lParam )
{
	//��ʱ֪ͨ, ת������������ҳ
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral->SendMessage(WM_STTP_20060, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20124()
 ���ܸ�Ҫ��ת��¼����Զ����������֪ͨ(20124)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B810055
void CWaveRecFrame::OnSTTP20124( WPARAM wParam, LPARAM lParam )
{
	//¼����Զ����������֪ͨ, ת������������ҳ
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral->SendMessage(WM_STTP_20124, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20069()
 ���ܸ�Ҫ��ת����������ͨ�÷���֪ͨ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B81005E
void CWaveRecFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//��������ͨ�÷���֪ͨ, ת���������п��ܻ��ٻ�����Ƶ�ҳ��
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(m_pWRDigital != NULL)
	{
		m_pWRDigital->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	if(m_pWRAnalog != NULL)
	{
		m_pWRAnalog->SendMessage(WM_STTP_20069, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ��ת��װ�ò�֧�ֹ���֪ͨ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B810061
void CWaveRecFrame::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	//װ�ò�֧�ֹ���֪ͨ, ת�������п��ܻ��ٻ�����Ƶ�ҳ��
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(m_pWRDigital != NULL)
	{
		m_pWRDigital->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	if(m_pWRAnalog != NULL)
	{
		m_pWRAnalog->SendMessage(WM_STTP_20125, wParam, lParam);
	}
}

//##ModelId=49B87B810046
void CWaveRecFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
// 	//��������ҳ
// 	if(m_pWRGeneral != NULL)
// 	{
// 		m_pWRGeneral ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
// 	}
// 	
// 	//��ֵ
// 	if(m_pWRSetting != NULL)
// 	{
// 		m_pWRSetting ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
// 	}
// 
// 	if(m_pWRDigital != NULL)
// 	{
// 		m_pWRDigital->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
// 	}
// 	if(m_pWRAnalog != NULL)
// 	{
// 		m_pWRAnalog->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
// 	}
// 	
// 	//¼��
// 	if(m_pWROSC != NULL)
// 	{
// 		m_pWROSC ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
// 	}
	m_pCurSec = NULL;
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}
	if(m_pWRNote != NULL)
	{
		m_pWRNote->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}

	//����Ϣ֪ͨ�����
	AfxGetApp() ->m_pMainWnd->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	CMDIChildWnd::OnClose();
}

/*************************************************************
 �� �� ����OnOpenWRProp()
 ���ܸ�Ҫ����Ӧ��¼����������Ϣ, ���Ĵ��ڱ���, ת����Ϣ������ҳ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B810080
void CWaveRecFrame::OnOpenWRProp( WPARAM wParam, LPARAM lParam )
{
	CXJBrowserApp* pApp = (CXJBrowserApp *)AfxGetApp();

	//��ת������������
	CSecObj* pObj = (CSecObj*)lParam;
	m_pCurSec = pObj;	

	if(m_pCurSec != NULL)
	{
		CString strStation = StringFromID(IDS_COMMON_UNKNOWN_SUBSTATION);
		if(m_pCurSec->m_pStation != NULL)
			strStation = m_pCurSec->m_pStation->m_sName;
		CString str;
		str.Format("[%s][%s]", strStation,m_pCurSec->m_sName);
		pApp->SetAppTile(str);
	}
	else
 		pApp->SetAppTile(StringFromID(IDS_COMMON_UNKNOWN_DEV));

	pApp->GetMainWnd()->SendMessage(WM_MDITAB_REFRESH, 0, 0);
	//��������ҳ
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral ->SendMessage(WM_WRFRAME_OPEN, wParam, lParam);
	}

	//��ֵ
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}

	if(m_pWRDigital != NULL)
	{
		m_pWRDigital->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}
	if(m_pWRAnalog != NULL)
	{
		m_pWRAnalog->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}

	//¼��
	if(m_pWROSC != NULL)
	{
		m_pWROSC ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}

	//¼���б�
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}

	if(m_pWRNote != NULL )
	{
		m_pWRNote->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
	}
	
	//����ҳ��
	int nPage = (int)wParam;
	int nIndex = -1;
	m_mapIndex.Lookup(nPage, nIndex);
	if(nIndex <= WR_PROP_OSCLIST && nIndex >= WR_PROP_GENERAL)
		m_pTabView->ActivateView(nIndex);
	else
		m_pTabView->ActivateView(0);
	
	//�����
	ActivateFrame(SW_SHOWMAXIMIZED);
}

//##ModelId=49B87B810083
CWaveRecFrame::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	//��������ҳ
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}
	
	//��ֵ
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}

	if(m_pWRDigital != NULL)
	{
		m_pWRDigital->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}
	if(m_pWRAnalog != NULL)
	{
		m_pWRAnalog->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}
	
	//¼��
	if(m_pWROSC != NULL)
	{
		m_pWROSC ->SendMessage(WM_WRFRAME_CLOSE, 0, 0);
	}

	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_WRFRAME_CLOSE, wParam, lParam);
	}

	if(m_pWRNote != NULL )
	{
		m_pWRNote->SendMessage(WM_WRFRAME_CLOSE, wParam, lParam);
		m_pWRNote->SendMessage(WM_PTFRAME_CLOSE, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20026()
 ���ܸ�Ҫ��ת��20026���ĸ�Զ��¼����ͼ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B81006E
void CWaveRecFrame::OnSTTP20026( WPARAM wParam, LPARAM lParam )
{
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_STTP_20026, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20043()
 ���ܸ�Ҫ��ת��20043���ĸ�Զ��¼����ͼ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B810071
void CWaveRecFrame::OnSTTP20043( WPARAM wParam, LPARAM lParam )
{
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_STTP_20043, wParam, lParam);
	}
}

//##ModelId=49B87B81008D
CWaveRecFrame::OnRefreshPTSetting( WPARAM wParam, LPARAM lParam )
{
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->PostMessage(REFRESH_PTSETTING, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ��ʱ֪ͨ����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B81007D
void CWaveRecFrame::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	if(m_pWRGeneral != NULL)
	{
		m_pWRGeneral->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(m_pWRDigital != NULL)
	{
		m_pWRDigital->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(m_pWRAnalog != NULL)
	{
		m_pWRAnalog->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	if(m_pWROSCList != NULL)
	{
		m_pWROSCList->SendMessage(WM_STTP_20157, wParam, lParam);
	}
}

/*************************************************************
 �� �� ��: OnSTTP20008()
 ���ܸ�Ҫ: ת��ģ��������֪ͨ(20008)
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CWaveRecFrame::OnSTTP20008( WPARAM wParam, LPARAM lParam )
{
	if(m_pWRAnalog != NULL)
		m_pWRAnalog->SendMessage(WM_STTP_20008, wParam, lParam);
}

/*************************************************************
 �� �� ��: OnSTTP20012()
 ���ܸ�Ҫ: ת��(20012)����������֪ͨ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CWaveRecFrame::OnSTTP20012( WPARAM wParam,LPARAM lParam )
{
	if(m_pWRDigital != NULL)
		m_pWRDigital->SendMessage(WM_STTP_20012, wParam, lParam);
}

/*************************************************************
 �� �� ��: OnSTTP20086()
 ���ܸ�Ҫ: ת��ģ��������֪ͨ(20086)
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CWaveRecFrame::OnSTTP20086( WPARAM wParam, LPARAM lParam )
{
	if(m_pWRGeneral != NULL)
		m_pWRGeneral->SendMessage(WM_STTP_20086, wParam, lParam);
}

/****************************************************
Date:2011/9/27  Author:LYH
������:   OnSTTP20107	
����ֵ:   void	
���ܸ�Ҫ: 
����: WPARAM wParam	
����: LPARAM lParam	
*****************************************************/
void CWaveRecFrame::OnSTTP20107( WPARAM wParam, LPARAM lParam )
{
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20107, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20048()
 ���ܸ�Ҫ��ת����ֵ���л�֪ͨ(20048)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920001
void CWaveRecFrame::OnSTTP20048( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20048, wParam, lParam);
	}
}

void CWaveRecFrame::OnSTTP20170( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20170, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20052()
 ���ܸ�Ҫ��ת��������ֵ�޸�֪ͨ(20052)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B92000F
void CWaveRecFrame::OnSTTP20052( WPARAM wParam, LPARAM lParam )
{
	//������ֵ�޸�֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20052, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20054()
 ���ܸ�Ҫ��ת��������ֵ�޸�ִ��֪ͨ(20054)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920013
void CWaveRecFrame::OnSTTP20054( WPARAM wParam, LPARAM lParam )
{
	//������ֵ�޸�ִ��֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20054, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20056()
 ���ܸ�Ҫ��ת��������ֵ���л�ִ��֪ͨ(20056)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920021
void CWaveRecFrame::OnSTTP20056( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�ִ��֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20056, wParam, lParam);
	}
}

void CWaveRecFrame::OnSTTP20172( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�ִ��֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20172, wParam, lParam);
	}
}

void CWaveRecFrame::OnSTTP20174(WPARAM wParam, LPARAM lParam)
{
	if(m_pWRSetting != NULL)
	{
		m_pWRSetting->SendMessage(WM_STTP_20174, wParam, lParam);
	}
}
