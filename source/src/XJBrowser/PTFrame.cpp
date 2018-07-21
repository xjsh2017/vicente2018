// PTFrame.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "PTFrame.h"

#include "DeviceObj.h"
//������������ͼҳ
#include "PTAction.h"
#include "PTAlarm.h"
#include "PTAnalog.h"
#include "PTDigital.h"
#include "PTGeneral.h"
#include "PTOSC.h"
#include "PTSetting.h"
#include "PTSoftBoard.h"

#include "GlobalFunc.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/

/////////////////////////////////////////////////////////////////////////////
// CPTFrame

IMPLEMENT_DYNCREATE(CPTFrame, CMDIChildWnd)

//##ModelId=49B87B91033E
CPTFrame::CPTFrame()
{
	/** @brief           ������������ҳ��ͼָ��*/
	m_pPTAction = NULL;
	/** @brief           �����澯����ҳ��ͼָ��*/
	m_pPTAlarm = NULL;
	/** @brief           ����ģ��������ҳ��ͼָ��*/
	m_pPTAnalog = NULL;
	/** @brief           ��������������ҳ��ͼָ��*/
	m_pPTDigital = NULL;
	/** @brief           ������������ҳ��ͼָ��*/
	m_pPTGeneral = NULL;
	/** @brief           ����¼������ҳ��ͼָ��*/
	m_pPTOSC = NULL;
	/** @brief           ������ֵ����ҳ��ͼָ��*/
	m_pPTSetting = NULL;
	/** @brief           ������ѹ������ҳ��ͼָ��*/
	m_pPTSoftBoard = NULL;
	m_pPTHistory = NULL;
	m_pPTOSCList = NULL;
	m_pPTNote = NULL;
	m_bSetBar = FALSE;
	m_pCurSec = NULL;
}

//##ModelId=49B87B91039B
CPTFrame::~CPTFrame()
{
	m_mapIndex.RemoveAll();
}


BEGIN_MESSAGE_MAP(CPTFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CPTFrame)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STTP_20008, OnSTTP20008)
	ON_MESSAGE(WM_STTP_20012, OnSTTP20012)
	ON_MESSAGE(WM_STTP_20016, OnSTTP20016)
	ON_MESSAGE(WM_STTP_20048, OnSTTP20048)
	ON_MESSAGE(WM_STTP_20170, OnSTTP20170)
	ON_MESSAGE(WM_STTP_20052, OnSTTP20052)
	ON_MESSAGE(WM_STTP_20054, OnSTTP20054)
	ON_MESSAGE(WM_STTP_20056, OnSTTP20056)
	ON_MESSAGE(WM_STTP_20172, OnSTTP20172)
	ON_MESSAGE(WM_STTP_20058, OnSTTP20058)
	ON_MESSAGE(WM_STTP_20060, OnSTTP20060)
	ON_MESSAGE(WM_STTP_20069, OnSTTP20069)
	ON_MESSAGE(WM_STTP_20107, OnSTTP20107)
	ON_MESSAGE(WM_STTP_20174, OnSTTP20174)
	ON_MESSAGE(WM_STTP_20109, OnSTTP20109)
	ON_MESSAGE(WM_STTP_20111, OnSTTP20111)
	ON_MESSAGE(WM_STTP_20114, OnSTTP20114)
	ON_MESSAGE(WM_STTP_20118, OnSTTP20118)
	ON_MESSAGE(WM_STTP_20125, OnSTTP20125)
	ON_MESSAGE(WM_STTP_20089, OnSTTP20089)
	ON_MESSAGE(WM_STTP_20026, OnSTTP20026)
	ON_MESSAGE(WM_STTP_20043, OnSTTP20043)
	ON_MESSAGE(WM_STTP_20116, OnSTTP20116)
	ON_MESSAGE(WM_STTP_20157, OnSTTP20157)
	ON_MESSAGE(WM_STTP_00901, OnSTTP00901)
	ON_MESSAGE(WM_STTP_20018, OnSTTP20018)
	ON_MESSAGE(WM_STTP_20022, OnSTTP20022)
	ON_MESSAGE(WM_STTP_20004, OnSTTP20004)
	ON_MESSAGE(WM_PTFRAME_OPEN, OnPTFrameOpen)
	ON_MESSAGE(WM_EVENTPROP_ACK_PT, OnEventPropAckPT)
	ON_MESSAGE(WM_EVENTPROP_ACK, OnEventPropAck)
	ON_MESSAGE(WM_EVENTLIST_ACK, OnEventListAck)
	ON_MESSAGE(WM_ALL_CLOSE, OnAllClose)
	ON_MESSAGE(REFRESH_PTSETTING, OnRefreshPTSetting)
	ON_MESSAGE(REFRESH_PTDI, OnRefreshPTDI)
	ON_MESSAGE(REFRESH_PTSOFTBOARD, OnRefreshPTSoft)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTFrame message handlers

//##ModelId=49B87B910382
void CPTFrame::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CXJBrowserApp* pApp = (CXJBrowserApp *)AfxGetApp();

	nCmdShow =SW_SHOWMAXIMIZED;	
	//֪ͨ����������豸��
	if (!m_bSetBar)
	{
		pApp->XJSetMenuInfo();
		m_bSetBar = TRUE;
	}
// 	pApp->GetMainWnd()->PostMessage(DEVICETREE_HIDE, 0, 0);
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

//##ModelId=49B87B91038A
BOOL CPTFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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

		CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();

		int nIndex = 0;
		//������ͼ

		//������Ϣ
		if(g_PTShowGeneral == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_GENERAL), RUNTIME_CLASS(CPTGeneral), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTGeneral = (CPTGeneral*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_GENERAL, nIndex);
		}

		//��ֵ
		if(g_PTShowSetting == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_SETTING), RUNTIME_CLASS(CPTSetting), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTSetting = (CPTSetting*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_SETTING, nIndex);
			//��¼���ڻص�������ָ��
			pApp->m_pPTSetting = m_pPTSetting;
		}
		
		//������
		if(g_PTShowDI == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_DIGITAL), RUNTIME_CLASS(CPTDigital), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTDigital = (CPTDigital*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_DIGITAL, nIndex);
			//��¼���ڻص�������ָ��
			pApp->m_pPTDigital = m_pPTDigital;
		}
		
		//��ѹ��
		if(g_PTShowSoftBoard == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_SOFT), RUNTIME_CLASS(CPTSoftBoard), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTSoftBoard = (CPTSoftBoard*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_SOFT, nIndex);
			//��¼���ڻص�������ָ��
			pApp->m_pPTSoftBoard = m_pPTSoftBoard;
		}
		
		//ģ����
		if(g_PTShowAI == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_ANALOG), RUNTIME_CLASS(CPTAnalog), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTAnalog = (CPTAnalog*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_ANALOG, nIndex);
			//��¼���ڻص�������ָ��
			pApp->m_pPTAnalog = m_pPTAnalog;
		}
		
		//����
		if(g_PTShowAction == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_ACTION), RUNTIME_CLASS(CPTAction), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTAction = (CPTAction*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_ACTION, nIndex);
			//��¼���ڻص�������ָ��
			pApp->m_pPTAction = m_pPTAction;
		}
		
		//�澯
		if(g_PTShowAlarm == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_ALARM), RUNTIME_CLASS(CPTAlarm), pContext);
			if(nIndex == -1)
				return FALSE;
			m_pPTAlarm = (CPTAlarm*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_ALARM, nIndex);
			//��¼���ڻص�������ָ��
			pApp->m_pPTAlarm = m_pPTAlarm;
		}
		
		//��ʷ�¼�
		if(g_PTShowHistory == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_HISTORYEVENT), RUNTIME_CLASS(CPTHistory), pContext);
			if(nIndex == -1)
				return FALSE;
			//��¼ָ��
			m_pPTHistory = (CPTHistory*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_HISTORY, nIndex);
		}

		//����¼��
		if(g_PTShowOsc == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_LOCALFILE), RUNTIME_CLASS(CPTOSC), pContext);
			if (nIndex == -1)  
				return FALSE;  
			//��¼ָ��
			m_pPTOSC = (CPTOSC*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_OSC, nIndex);
		}
		
		//¼������
		if(g_PTShowOscList == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_REMOTEFILE), RUNTIME_CLASS(CPTOSCList), pContext);
			if(nIndex == -1)
				return FALSE;
			//��¼ָ��
			m_pPTOSCList = (CPTOSCList*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_OSCLIST, nIndex);
		}

		//��עҳ��
		if(g_PTShowNote == 1)
		{
			nIndex = pView->Insert(StringFromID(IDS_SECPROP_NOTE), RUNTIME_CLASS(CPTNote), pContext);
			if(nIndex == -1)
				return FALSE;
			//��¼ָ��
			m_pPTNote = (CPTNote*)pView->GetView(nIndex);
			m_mapIndex.SetAt(PT_PROP_NOTE, nIndex);
		}

		// activate the input view
		SetActiveView(pView);
	}
	return TRUE;
}

//##ModelId=49B87B91038E
BOOL CPTFrame::PreCreateWindow(CREATESTRUCT& cs) 
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
	
	//cs.style &=~ (LONG) FWS_ADDTOTITLE;
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style |= WS_VISIBLE;
	
	return TRUE;
}

/*************************************************************
 �� �� ����OnSTTP20008()
 ���ܸ�Ҫ��ת������ģ��������ֵ����֪ͨ(20008)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9103C9
void CPTFrame::OnSTTP20008( WPARAM wParam, LPARAM lParam )
{
	//����ģ��������ֵ����֪ͨ,ת������������ҳ--ģ������ͼ
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog->SendMessage(WM_STTP_20008, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20012()
 ���ܸ�Ҫ��ת����������������ֵ����֪ͨ(20012)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9103CD
void CPTFrame::OnSTTP20012( WPARAM wParam, LPARAM lParam )
{
	//����������ֵ����֪ͨ, ת������������ҳ--��������ͼ
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital->SendMessage(WM_STTP_20012, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20016()
 ���ܸ�Ҫ��ת����ֵ����֪ͨ(20016)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9103DB
void CPTFrame::OnSTTP20016( WPARAM wParam, LPARAM lParam )
{
	//������ֵ����֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20016, wParam, lParam);
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
void CPTFrame::OnSTTP20048( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20048, wParam, lParam);
	}
}

void CPTFrame::OnSTTP20170( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20170, wParam, lParam);
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
void CPTFrame::OnSTTP20052( WPARAM wParam, LPARAM lParam )
{
	//������ֵ�޸�֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20052, wParam, lParam);
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
void CPTFrame::OnSTTP20054( WPARAM wParam, LPARAM lParam )
{
	//������ֵ�޸�ִ��֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20054, wParam, lParam);
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
void CPTFrame::OnSTTP20056( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�ִ��֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20056, wParam, lParam);
	}
}

void CPTFrame::OnSTTP20172( WPARAM wParam, LPARAM lParam )
{
	//������ֵ���л�ִ��֪ͨ, ת������������ҳ--��ֵ��ͼ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting->SendMessage(WM_STTP_20172, wParam, lParam);
	}
}


/*************************************************************
 �� �� ����OnSTTP20058()
 ���ܸ�Ҫ��ת�������źŸ���֪ͨ(20058)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B92002F
void CPTFrame::OnSTTP20058( WPARAM wParam, LPARAM lParam )
{
	//�����źŸ���֪ͨ, ת��������������Ϣ��ͼ
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_STTP_20058, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20060()
 ���ܸ�Ҫ��ת��������ʱ֪ͨ(20060)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B92003E
void CPTFrame::OnSTTP20060( WPARAM wParam, LPARAM lParam )
{
	//������ʱ֪ͨ, ת��������������Ϣ��ͼ
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral ->SendMessage(WM_STTP_20060, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20069()
 ���ܸ�Ҫ��ת����������ͨ�÷���֪ͨ(20069)
 �� �� ֵ:  void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920042
void CPTFrame::OnSTTP20069( WPARAM wParam, LPARAM lParam )
{
	//��������ͨ�÷���֪ͨ, ת�������п��ܻᷢ���ٻ���������������ҳ
	//��������ҳ
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//��ֵҳ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//������ҳ
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//��ѹ��
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//ģ����
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//Զ��¼��
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_STTP_20069, wParam, lParam);
	}
	//��ʷ�¼�
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_STTP_20069, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20107()
 ���ܸ�Ҫ��ת���ٻ���ǰ��ֵ��֪ͨ(20107)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920050
void CPTFrame::OnSTTP20107( WPARAM wParam, LPARAM lParam )
{
	//�ٻ�������ǰ��ֵ��֪ͨ, ת������ֵ��ͼ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_STTP_20107, wParam, lParam);
	}
}

void CPTFrame::OnSTTP20174( WPARAM wParam, LPARAM lParam )
{
	//�ٻ�������ǰ��ֵ��֪ͨ, ת������ֵ��ͼ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_STTP_20174, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20109()
 ���ܸ�Ҫ��ת���ٻ�������ѹ��֪ͨ(20109)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B92005D
void CPTFrame::OnSTTP20109( WPARAM wParam, LPARAM lParam )
{
	//�ٻ�������ѹ��֪ͨ, ת������ѹ����ͼ
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard->SendMessage(WM_STTP_20109, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20111()
 ���ܸ�Ҫ��ת���ٻ�����������Ϣ֪ͨ(20111)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920061
void CPTFrame::OnSTTP20111( WPARAM wParam, LPARAM lParam )
{
	//�ٻ�����������Ϣ֪ͨ, ת����������Ϣ��ͼ
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral ->SendMessage(WM_STTP_20111, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20114()
 ���ܸ�Ҫ��ת��������ѹ��Ͷ��ִ��֪ͨ(20114)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B92006E
void CPTFrame::OnSTTP20114( WPARAM wParam, LPARAM lParam )
{
	//������ѹ��Ͷ��ִ��֪ͨ, ת������ѹ����ͼ
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_STTP_20114, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20118()
 ���ܸ�Ҫ��ת��������ѹ��Ͷ��֪ͨ, ת������ѹ����ͼ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920072
void CPTFrame::OnSTTP20118( WPARAM wParam, LPARAM lParam )
{
	//������ѹ��Ͷ��֪ͨ, ת������ѹ����ͼ
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard->SendMessage(WM_STTP_20118, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20125()
 ���ܸ�Ҫ��ת��װ�ò�֧�ֹ���֪ͨ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B92007E
void CPTFrame::OnSTTP20125( WPARAM wParam, LPARAM lParam )
{
	//װ�ò�֧�ֹ���֪ͨ, ת�������п��ܻᷢ���ٻ���������������ҳ
	//��������ҳ
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//��ֵҳ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//������ҳ
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//��ѹ��
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//ģ����
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//Զ��¼��
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_STTP_20125, wParam, lParam);
	}
	//��ʷ�¼�
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_STTP_20125, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20089()
 ���ܸ�Ҫ��ת�������ٻ�ʱ��֪ͨ(20089)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920082
void CPTFrame::OnSTTP20089( WPARAM wParam, LPARAM lParam )
{
	//�����ٻ�ʱ��֪ͨ, ת������������ҳ
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_STTP_20089, wParam, lParam);
	}
}


/*************************************************************
 �� �� ����OnSTTP20004()
 ���ܸ�Ҫ��ת���ٻ�����������Ϣ֪ͨ(20111)
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920061
void CPTFrame::OnSTTP20004( WPARAM wParam, LPARAM lParam )
{
	//�ٻ�����������Ϣ֪ͨ, ת����������Ϣ��ͼ
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral ->SendMessage(WM_STTP_20004, wParam, lParam);
	}
}


//##ModelId=49B87B91039D
void CPTFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//��ת������������
	m_pCurSec = NULL;
	//��������ҳ
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//��ֵҳ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//������ҳ
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//��ѹ��
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//ģ����
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//�����¼�
	if(m_pPTAction != NULL)
	{
		m_pPTAction ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//�澯�¼�
	if(m_pPTAlarm != NULL)
	{
		m_pPTAlarm ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//��ʷ�¼�
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//¼��
	if(m_pPTOSC != NULL)
	{
		m_pPTOSC ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//Զ��¼��
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//��עҳ��
	if(m_pPTNote != NULL )
	{
		m_pPTNote->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//����Ϣ֪ͨ�����
	AfxGetApp() ->m_pMainWnd->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	CMDIChildWnd::OnClose();
}

/*************************************************************
 �� �� ����OnPTFrameOpen()
 ���ܸ�Ҫ����Ӧ��������ҳ�����Ϣ,��ת������������, ֪ͨ���Ǹ�������, �ٰѴ����Ƴ�ǰ��
 �� �� ֵ: void
 ��    ����param1	��Ϣ����
		   Param2	�����豸ָ��
**************************************************************/
//##ModelId=49B87B9200BB
void CPTFrame::OnPTFrameOpen( WPARAM wParam, LPARAM lParam )
{
	//��ת������������
	CSecObj* pObj = (CSecObj*)lParam;
	m_pCurSec = pObj;

	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
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
	
	pApp->GetMainWnd()->PostMessage(WM_MDITAB_REFRESH, 0, 0);
	
	//�����
	ActivateFrame(SW_SHOWMAXIMIZED);
	
	//��������ҳ
	if(m_pPTGeneral != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTGeneral->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("�����������ҳ����ʱ��", startTime);
	}

	//��ֵҳ
	if(m_pPTSetting != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTSetting ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("���붨ֵҳ����ʱ��", startTime);
	}
	
	//������ҳ
	if(m_pPTDigital != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTDigital ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("���뿪��������ʱ��", startTime);
	}
	
	//��ѹ��
	if(m_pPTSoftBoard != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTSoftBoard ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("������ѹ������ʱ��", startTime);
	}
	
	//ģ����
	if(m_pPTAnalog != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTAnalog ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("����ģ��������ʱ��", startTime);
	}
	
	//�����¼�
	if(m_pPTAction != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTAction ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("���붯������ʱ��", startTime);
	}
	
	//�澯�¼�
	if(m_pPTAlarm != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTAlarm ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("����澯����ʱ��", startTime);
	}
	
	//��ʷ�¼�
	if(m_pPTHistory != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTHistory->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("������ʷ�¼�����ʱ��", startTime);
	}
	
	//¼��
	if(m_pPTOSC != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTOSC ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("����¼������ʱ��", startTime);
	}
	
	//Զ��¼��
	if(m_pPTOSCList != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTOSCList ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("����Զ��¼������ʱ��", startTime);
	}

	//��עҳ��
	if(m_pPTNote != NULL)
	{
		LONGLONG startTime = StartCount();
		m_pPTNote ->SendMessage(WM_PTFRAME_OPEN, wParam, lParam);
		EndCount("���뱸ע����ʱ��", startTime);
	}

	//����ҳ��
	int nPage = (int)wParam;
	int nIndex = -1;
	m_mapIndex.Lookup(nPage, nIndex);
	if(nIndex <= PT_PROP_NOTE && nIndex >= PT_PROP_GENERAL)
		m_pTabView->ActivateView(nIndex);
	else
		m_pTabView->ActivateView(0);
}

//##ModelId=49B87B9103AA
void CPTFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}

/*************************************************************
 �� �� ����OnEventPropAckPT()
 ���ܸ�Ҫ���ɱ�������ҳ��򿪵��¼����ԶԻ������¼�ȷ����Ϣ, ת��
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B9200BF
void CPTFrame::OnEventPropAckPT( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTAction != NULL)
		m_pPTAction->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
	if(m_pPTAlarm != NULL)
		m_pPTAlarm->PostMessage(WM_EVENTPROP_ACK_PT, wParam, lParam);
}

/*************************************************************
 �� �� ����OnEventPropAck()
 ���ܸ�Ҫ�����¼����򿪵��¼����ԶԻ������¼�ȷ����Ϣ, ת��
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B9200CC
void CPTFrame::OnEventPropAck( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTAction != NULL)
		m_pPTAction->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
	if(m_pPTAlarm != NULL)
		m_pPTAlarm->PostMessage(WM_EVENTPROP_ACK, wParam, lParam);
}

/*************************************************************
 �� �� ����OnEventListAck()
 ���ܸ�Ҫ�����¼��������¼�ȷ����Ϣ, ת��
 �� �� ֵ: void
 ��    ����param1	�¼�����
		   Param2	�¼�ָ��
**************************************************************/
//##ModelId=49B87B9200DA
void CPTFrame::OnEventListAck( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTAction != NULL)
		m_pPTAction->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
	if(m_pPTAlarm != NULL)
		m_pPTAlarm->PostMessage(WM_EVENTLIST_ACK, wParam, lParam);
}

/*************************************************************
 �� �� ����OnAllClose()
 ���ܸ�Ҫ:	��Ӧ����ر���Ϣ, ת����������ҳ��WM_PTFRAME_CLOSE��Ϣ
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9200DE
CPTFrame::OnAllClose( WPARAM wParam, LPARAM lParam )
{
	//��������ҳ
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//��ֵҳ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//������ҳ
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//��ѹ��
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//ģ����
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//�����¼�
	if(m_pPTAction != NULL)
	{
		m_pPTAction ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//�澯�¼�
	if(m_pPTAlarm != NULL)
	{
		m_pPTAlarm ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//��ʷ�¼�
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//¼��
	if(m_pPTOSC != NULL)
	{
		m_pPTOSC ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//Զ��¼��
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
	//��עҳ��
	if(m_pPTNote != NULL )
	{
		m_pPTNote ->SendMessage(WM_PTFRAME_CLOSE, 0, 0);
	}
}

/*************************************************************
 �� �� ����OnSTTP20026()
 ���ܸ�Ҫ��20026(¼���б�����֪ͨ), ת����Զ��¼��ҳ��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B92008F
void CPTFrame::OnSTTP20026( WPARAM wParam, LPARAM lParam )
{
	//Զ��¼��
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_STTP_20026, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20043()
 ���ܸ�Ҫ��20043(¼���ļ����ر���), ת����Զ��¼��ҳ��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B920093
void CPTFrame::OnSTTP20043( WPARAM wParam, LPARAM lparam )
{
	//Զ��¼��
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_STTP_20043, wParam, lparam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20116()
 ���ܸ�Ҫ��20116(������ʷ�¼�����֪ͨ), ת������ʷ�¼�ҳ��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B92009F
void CPTFrame::OnSTTP20116( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_STTP_20116, wParam, lParam);
	}
}

//##ModelId=49B87B9200EA
void CPTFrame::OnRefreshPTSetting( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTSetting != NULL)
		m_pPTSetting->PostMessage(REFRESH_PTSETTING, wParam, lParam);
}

//##ModelId=49B87B9200EE
void CPTFrame::OnRefreshPTDI( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTDigital != NULL)
		m_pPTDigital->PostMessage(REFRESH_PTDI, wParam, lParam);
}

//##ModelId=49B87B9200FB
void CPTFrame::OnRefreshPTSoft( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTSoftBoard != NULL)
		m_pPTSoftBoard->PostMessage(REFRESH_PTSOFTBOARD, wParam, lParam);
}

//##ModelId=49B87B910399
void CPTFrame::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CMDIChildWnd::PostNcDestroy();
//	delete this;
}

//##ModelId=49B87B910385
BOOL CPTFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIChildWnd::DestroyWindow();
}

//##ModelId=49B87B9103AF
void CPTFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIChildWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
}

//##ModelId=49B87B9103BD
void CPTFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIChildWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
}

/*************************************************************
 �� �� ����OnSTTP00901()
 ���ܸ�Ҫ���յ�00901����, ת������ʷ�¼���ͼ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9200AF
void CPTFrame::OnSTTP00901( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_STTP_00901, wParam, lParam);
	}
}

/*************************************************************
 �� �� ����OnSTTP20157()
 ���ܸ�Ҫ����Ӧ20157����, ת�������п��ܻᷢ���ٻ���������������ҳ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B9200AB
void CPTFrame::OnSTTP20157( WPARAM wParam, LPARAM lParam )
{
	//��ʱ����֪ͨ, ת�������п��ܻᷢ���ٻ���������������ҳ
	//��������ҳ
	if(m_pPTGeneral != NULL)
	{
		m_pPTGeneral->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//��ֵҳ
	if(m_pPTSetting != NULL)
	{
		m_pPTSetting ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//������ҳ
	if(m_pPTDigital != NULL)
	{
		m_pPTDigital ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//��ѹ��
	if(m_pPTSoftBoard != NULL)
	{
		m_pPTSoftBoard ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//ģ����
	if(m_pPTAnalog != NULL)
	{
		m_pPTAnalog ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//Զ��¼��
	if(m_pPTOSCList != NULL)
	{
		m_pPTOSCList ->SendMessage(WM_STTP_20157, wParam, lParam);
	}
	//��ʷ�¼�
	if(m_pPTHistory != NULL)
	{
		m_pPTHistory->SendMessage(WM_STTP_20157, wParam, lParam);
	}
}

void CPTFrame::OnSTTP20018( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTAlarm != NULL)
		m_pPTAlarm->SendMessage(WM_STTP_20018, wParam, lParam);
}

void CPTFrame::OnSTTP20022( WPARAM wParam, LPARAM lParam )
{
	if(m_pPTAction != NULL)
		m_pPTAction->SendMessage(WM_STTP_20022, wParam, lParam);
}
