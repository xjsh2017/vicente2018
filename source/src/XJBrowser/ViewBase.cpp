// ViewBase.cpp : implementation file
//

#include "stdafx.h"
//#include "XJSubstationTest.h"
#include "ViewBase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewBase

IMPLEMENT_DYNCREATE(CViewBase, CFormView)

//##ModelId=49B87B860244
CViewBase::CViewBase()
	: CFormView(CViewBase::IDD)
{
	//{{AFX_DATA_INIT(CViewBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	posCtrl.RemoveAll();
	szDlg.cx = 718;
	szDlg.cy = 406;
	m_bLoadData = FALSE;
	m_sCyc = _T("");
}

//##ModelId=49B87B860245
CViewBase::CViewBase(UINT nIDTemplate):CFormView(nIDTemplate)
{
	m_nDlgID = nIDTemplate;
	posCtrl.RemoveAll();
	szDlg.cx = 718;
	szDlg.cy = 406;
	m_bLoadData = FALSE;
}

//##ModelId=49B87B860280
CViewBase::~CViewBase()
{
	posCtrl.RemoveAll();
}

//##ModelId=49B87B860277
void CViewBase::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewBase)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewBase, CFormView)
	//{{AFX_MSG_MAP(CViewBase)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewBase diagnostics

#ifdef _DEBUG
//##ModelId=49B87B860282
void CViewBase::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87B860284
void CViewBase::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewBase message handlers

//##ModelId=49B87B860271
void CViewBase::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::OnPrepareDC(pDC, pInfo);
}

//##ModelId=49B87B860275
void CViewBase::OnInitialUpdate() 
{	
	// TODO: Add your specialized code here and/or call the base class
	
	CFormView::OnInitialUpdate();
}

/*************************************************************
 �� �� ����RecordRate()
 ���ܸ�Ҫ����¼�ؼ�����,�����߶�,�����Ի���ģ��ı���, �߶ȿ�ȱ仯����,
			λ�ñ仯����. ������OnInitialUpdate()�������ø���OnInitialUpdate()
			֮ǰ����
 �� �� ֵ: void
 ��    ����param1   Ҫ��¼�Ŀؼ���ID
		   param2 	�߶ȿ�ȸı����,���ܵ�ֵΪ0~3(��ͷ�ļ�)
		   param3 	�����Ե�λ��,���ܵ�ֵΪ�ؼ�ID����-1~-5���-99(��ͷ�ļ�),��ͬ
		   param4 	�ϱ���Ե�λ��
		   param5 	�ұ���Ե�λ��
		   param6 	�±���Ե�λ��
**************************************************************/
//##ModelId=49B87B860264
void CViewBase::RecordRate(UINT nID, int nType /* = 0 */, int nLeft /*= null_effect*/, 
		int nTop /*= null_effect*/, int nRight /*= null_effect*/, int nBottom /*= null_effect*/)
{
	CWnd * pWnd = GetDlgItem(nID);
	if(pWnd != NULL)
	{
		CRect rc;
		CDialog dlg;
		dlg.Create(m_nDlgID);
		dlg.GetWindowRect(&rc);

		szDlg.cx = rc.Width();
		szDlg.cy = rc.Height();

		CRect rcCtrl;
		pWnd = dlg.GetDlgItem(nID);
		if(pWnd != NULL)
			pWnd ->GetWindowRect(&rcCtrl);
		
		
		//�������,����
		ctrlPos cp;
		//�ؼ����/�ͻ������
		cp.fWidth = (float)rcCtrl.Width() / (float)rc.Width();
		//�ؼ��߶�/�ͻ����߶�
		cp.fHeight = (float)rcCtrl.Height() / (float)rc.Height();
		
		cp.nID = nID;
		cp.nType = nType;
		//////////////////////////////////////////////////////////////////////////
		cp.nID_Left = nLeft;
		if(nLeft != null_effect)
		{
			//��������λ��
			if(nLeft >= 0)
			{
				//��Ե��ǿؼ�
				CWnd * pOther = dlg.GetDlgItem(nLeft);
				if(pOther != NULL)
				{
					CRect rcTemp;
					pOther->GetWindowRect(&rcTemp);
					cp.Left_Dis = rcCtrl.left - rcTemp.right;
				}
			}
			else
			{
				switch(nLeft)
				{
				case left_client: //�ͻ������
					cp.Left_Dis = rcCtrl.left - rc.left;
					break;
				case right_client:
					cp.Left_Dis = rcCtrl.left - rc.right;
					break;
				case mid_client:
					cp.Left_Dis = rcCtrl.left - (rc.left + rc.Width()/2);
					break;
				default:
					cp.nID_Left  = null_effect;
					break;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		cp.nID_Right = nRight;
		if(nRight != null_effect)
		{
			//�����λ��
			if(nRight >= 0)
			{
				//����������ؼ�
				CWnd * pOther = dlg.GetDlgItem(nRight);
				if(pOther != NULL)
				{
					CRect rcTemp;
					pOther->GetWindowRect(&rcTemp);
					cp.Right_Dis = rcCtrl.right - rcTemp.left;
				}
			}
			else 
			{
				 switch(nRight)
				 {
				 case left_client: //�ͻ������
					 cp.Right_Dis = rcCtrl.right - rc.left;
					 break;
				 case right_client:
					 cp.Right_Dis = rcCtrl.right - rc.right;
					 break;
				 case mid_client:
					 cp.Right_Dis = rcCtrl.right - (rc.left + rc.Width()/2);
					 break;
				 default:
					 cp.nID_Right  = null_effect;
					break;
				 }
			}
		}
		//////////////////////////////////////////////////////////////////////////
		cp.nID_Top = nTop;
		if(nTop != null_effect)
		{
			//�����λ��
			if(nTop >= 0)
			{
				//��Ե��������ؼ�
				CWnd * pOther = dlg.GetDlgItem(nTop);
				if(pOther != NULL)
				{
					CRect rcTemp;
					pOther->GetWindowRect(&rcTemp);
					cp.Top_Dis = rcCtrl.top - rcTemp.bottom;
				}
			}
			else
			{
				switch(nTop)
				{
				case top_client: 
					cp.Top_Dis = rcCtrl.top - rc.top;
					break;
				case bottom_client:
					cp.Top_Dis = rcCtrl.top - rc.bottom;
					break;
				default:
					cp.nID_Top  = null_effect;
					break;
				 }
			}
		}
		//////////////////////////////////////////////////////////////////////////
		cp.nID_Bottom = nBottom;
		if(nBottom != null_effect)
		{
			//�����λ��
			if(nBottom >= 0)
			{
				//��Ե��������ؼ�
				CWnd * pOther = dlg.GetDlgItem(nBottom);
				if(pOther != NULL)
				{
					CRect rcTemp;
					pOther->GetWindowRect(&rcTemp);
					cp.Bottom_Dis = rcCtrl.bottom - rcTemp.top;
				}
			}
			else
			{
				switch(nBottom)
				{
				case top_client: 
					cp.Bottom_Dis = rcCtrl.bottom - rc.top;
					break;
				case bottom_client:
					cp.Bottom_Dis = rcCtrl.bottom - rc.bottom;
					break;
				default:
					cp.nID_Bottom  = null_effect;
					break;
				 }
			}
		}	
		//////////////////////////////////////////////////////////////////////////
		posCtrl.Add(cp);
		dlg.DestroyWindow();
	}
}

/*************************************************************
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ�λ��, �˺������ڱ�����WM_SIZE��Ϣ�������Զ�����,���಻��Ҫ�ٵ���
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B860263
void CViewBase::RegulateControlSize()
{
	CRect rc;
	GetWindowRect(&rc);

	int nScrollX = GetScrollPos(SB_HORZ);
	int nScrollY = GetScrollPos(SB_VERT);
	
	rc.left -= nScrollX;
	rc.right -= nScrollX;
	rc.top -= nScrollY;
	rc.bottom -= nScrollY;

	if(rc.Width() < szDlg.cx)
	{
		rc.right = rc.left + szDlg.cx;
	}
	if(rc.Height() < szDlg.cy)
	{
		rc.bottom = rc.top + szDlg.cy;
	}
	for(int i = 0; i < posCtrl.GetSize(); i++)
	{
		ctrlPos cp = posCtrl.GetAt(i);

		CWnd * pWnd = GetDlgItem(cp.nID);
		if(pWnd != NULL)
		{
			CRect rcCtrl;
			pWnd ->GetWindowRect(&rcCtrl);
			long lw = rcCtrl.Width();
			long lh = rcCtrl.Height();
			long lNew = 0;
			//////////////////////////////////////////////////////////////////////////
			if(cp.nID_Left != null_effect)
			{
				//��������λ��
				if(cp.nID_Left >= 0)
				{
					//��Ե��������ؼ�
					CWnd * pOther = GetDlgItem(cp.nID_Left);
					if(pOther != NULL)
					{
						CRect rcTemp;
						pOther->GetWindowRect(&rcTemp);
						
						rcCtrl.left = rcTemp.right + cp.Left_Dis;
					}
				}
				else
				{
					//��Ե��ǿͻ���
					switch(cp.nID_Left)
					{
					case left_client: //��Կͻ������
						rcCtrl.left = rc.left + cp.Left_Dis;
						break;
					case right_client: //��Կͻ����ұ� 
						rcCtrl.left = rc.right + cp.Left_Dis;
						break;
					case mid_client:	//��Կͻ����м�
						rcCtrl.left = rc.left + rc.Width()/2 + cp.Left_Dis;
						break;
					default:
						break;
					}
				}
				//�жϿ�Ⱥ͸߶��Ƿ��б仯
			/*	int const	change_null = 0;	//��ȸ߶ȶ�����
				int const	change_width = 1;	//��ȸı�
				int const	change_height = 2;	//�߶ȸı�
				int	const	change_all	= 3;	//���ı�*/
				switch(cp.nType)
				{
				case change_null:
					rcCtrl.right = rcCtrl.left + lw;
					break;
				case change_width:
				case change_all:
					lNew = (long)(rc.Width() * cp.fWidth);
					rcCtrl.right = rcCtrl.left + lNew;
					break;
				default:
					rcCtrl.right = rcCtrl.left + lw;
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			if(cp.nID_Right != null_effect)
			{
				//�ұ������λ��
				if(cp.nID_Right >= 0)
				{
					//��Ե��������ؼ�
					CWnd * pOther = GetDlgItem(cp.nID_Right);
					if(pOther != NULL)
					{
						CRect rcTemp;
						pOther->GetWindowRect(&rcTemp);
						
						rcCtrl.right = rcTemp.left + cp.Right_Dis;
					}
				}
				else
				{
					//��Ե��ǿͻ���
					switch(cp.nID_Right)
					{
					case left_client: //��Կͻ������
						rcCtrl.right = rc.left + cp.Right_Dis;
						break;
					case right_client: //��Կͻ����ұ� 
						rcCtrl.right = rc.right + cp.Right_Dis;
						break;
					case mid_client:	//��Կͻ����м�
						rcCtrl.right = rc.left + rc.Width()/2 + cp.Right_Dis;
						break;
					default:
						break;
					}
				}
				//�жϿ�Ⱥ͸߶��Ƿ��б仯
			/*	int const	change_null = 0;	//��ȸ߶ȶ�����
				int const	change_width = 1;	//��ȸı�
				int const	change_height = 2;	//�߶ȸı�
				int	const	change_all	= 3;	//���ı�*/
				switch(cp.nType)
				{
				case change_null:
					rcCtrl.left = rcCtrl.right - lw;
					break;
				case change_width:
				case change_all:
					lNew = (long)(rc.Width() * cp.fWidth);
					rcCtrl.left = rcCtrl.right - lNew;
					break;
				default:
					rcCtrl.left = rcCtrl.right - lw;
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			if(cp.nID_Top != null_effect)
			{
				//�ϱ������λ��
				if(cp.nID_Top >= 0)
				{
					//��Ե��������ؼ�
					CWnd * pOther = GetDlgItem(cp.nID_Top);
					if(pOther != NULL)
					{
						CRect rcTemp;
						pOther->GetWindowRect(&rcTemp);
						
						rcCtrl.top = rcTemp.bottom + cp.Top_Dis;
					}
				}
				else
				{
					//��Ե��ǿͻ���
					switch(cp.nID_Top)
					{
					case top_client: //��Կͻ����ϱ�
						rcCtrl.top = rc.top + cp.Top_Dis;
						break;
					case bottom_client: //��Կͻ����±� 
						rcCtrl.top = rc.bottom + cp.Top_Dis;
						break;
					default:
						break;
					}
				}
				//�жϿ�Ⱥ͸߶��Ƿ��б仯
			/*	int const	change_null = 0;	//��ȸ߶ȶ�����
				int const	change_width = 1;	//��ȸı�
				int const	change_height = 2;	//�߶ȸı�
				int	const	change_all	= 3;	//���ı�*/
				switch(cp.nType)
				{
				case change_null:
					rcCtrl.bottom = rcCtrl.top + lh;
					break;
				case change_height:
				case change_all:
					lNew = (long)(rc.Height() * cp.fHeight);
					rcCtrl.bottom = rcCtrl.top + lNew;
					break;
				default:
					rcCtrl.bottom = rcCtrl.top + lh;
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			if(cp.nID_Bottom != null_effect)
			{
				//�±������λ��
				if(cp.nID_Bottom >= 0)
				{
					//��Ե��������ؼ�
					CWnd * pOther = GetDlgItem(cp.nID_Bottom);
					if(pOther != NULL)
					{
						CRect rcTemp;
						pOther->GetWindowRect(&rcTemp);
						
						rcCtrl.bottom = rcTemp.top + cp.Bottom_Dis;
					}
				}
				else
				{
					//��Ե��ǿͻ���
					switch(cp.nID_Bottom)
					{
					case top_client: //��Կͻ����ϱ�
						rcCtrl.bottom = rc.top + cp.Bottom_Dis;
						break;
					case bottom_client: //��Կͻ����±� 
						rcCtrl.bottom = rc.bottom + cp.Bottom_Dis;
						break;
					default:
						break;
					}
				}
				//�жϿ�Ⱥ͸߶��Ƿ��б仯
			/*	int const	change_null = 0;	//��ȸ߶ȶ�����
				int const	change_width = 1;	//��ȸı�
				int const	change_height = 2;	//�߶ȸı�
				int	const	change_all	= 3;	//���ı�*/
				switch(cp.nType)
				{
				case change_null:
					rcCtrl.top = rcCtrl.bottom - lh;
					break;
				case change_height:
				case change_all:
					lNew = (long)(rc.Height() * cp.fHeight);
					rcCtrl.top = rcCtrl.bottom - lNew;
					break;
				default:
					rcCtrl.top = rcCtrl.bottom - lh;
					break;
				}
			}
			//////////////////////////////////////////////////////////////////////////
			ScreenToClient(&rcCtrl);
			pWnd ->MoveWindow(rcCtrl);
		}
	}
}

/*************************************************************
 �� �� ����MoveCtrl()
 ���ܸ�Ҫ���ƶ��ؼ�λ��, �˺���һ���������WM_SIZE��Ϣ��Ӧ�е���,�ڵ���
			�˺���ǰӦ�ȵ��ø����WM_SIZE��Ϣ��Ӧ����	
 �� �� ֵ: void
 ��    ����param1   Ҫ�ƶ��Ŀؼ���ID
		   param2 	��λ��,���������������ؼ�ID,���Ǵ˿ؼ�right + ƫ����
		   param3	�Ϸ�λ��,���������������ؼ�ID,���Ǵ˿ؼ�bottom + ƫ����
		   param4 	�ҷ�λ��,���������������ؼ�ID,���Ǵ˿ؼ�left - ƫ����
		   param5 	�·�λ��,���������������ؼ�ID,���Ǵ˿ؼ�top - ƫ����
		   param6	ƫ����
**************************************************************/
//##ModelId=49B87B860257
void CViewBase::MoveCtrl(UINT nID, int nLeft, int nTop, int nRight, int nBottom, int offset)
{
	CWnd * pWnd = GetDlgItem(nID);
	if(pWnd != NULL)
	{
		CRect rc;
		GetWindowRect(&rc);

		int nScrollX = GetScrollPos(SB_HORZ);
		int nScrollY = GetScrollPos(SB_VERT);
		
		rc.left -= nScrollX;
		rc.right -= nScrollX;
		rc.top -= nScrollY;
		rc.bottom -= nScrollY;

		if(rc.Width() < szDlg.cx)
		{
			rc.right = rc.left + szDlg.cx;
		}
		if(rc.Height() < szDlg.cy)
		{
			rc.bottom = rc.top + szDlg.cy;
		}

		CRect rcCtrl;
		pWnd->GetWindowRect(&rcCtrl);

		//////////////////////////////////////////////////////////////////////////
		//��λ��
		if(nLeft >= 0)
		{
			//����������ؼ�λ��
			CWnd * pOther = GetDlgItem(nLeft);
			CRect rcTemp;
			if(pOther != NULL)
			{
				pOther->GetWindowRect(&rcTemp);
				rcCtrl.left = rcTemp.right + 20;
			}
		}
		else
		{
			//����Կͻ���
			//ֻ��Ե�����߿ͻ���ʱ�Ŵ���
			if(nLeft == left_client)
			{
				rcCtrl.left = rc.left;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//�Ϸ�λ��
		if(nTop >= 0)
		{
			//��Ե��������ؼ�
			CWnd * pOther = GetDlgItem(nTop);
			CRect rcTemp;
			if(pOther != NULL)
			{
				pOther->GetWindowRect(&rcTemp);
				rcCtrl.top = rcTemp.bottom + offset;
			}
		}
		else
		{
			//��Ե��ǿͻ���
			//ֻ����ͻ����ϱ�
			if(nTop == top_client)
			{
				rcCtrl.top = rc.top;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//�ұ�λ��
		if(nRight >= 0)
		{
			//��Ե��������ؼ�
			CWnd * pOther = GetDlgItem(nRight);
			CRect rcTemp;
			if(pOther != NULL)
			{
				pOther->GetWindowRect(&rcTemp);
				rcCtrl.right = rcTemp.left - offset;
			}
		}
		else
		{
			//��Ե��ǿͻ���
			//ֻ����ͻ����ұ�
			if(nRight == right_client)
			{
				rcCtrl.right = rc.right;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//�±�λ��
		if(nBottom >= 0)
		{
			//��Ե��������ؼ�
			CWnd * pOther = GetDlgItem(nBottom);
			CRect rcTemp;
			if(pOther != NULL)
			{
				pOther->GetWindowRect(&rcTemp);
				rcCtrl.bottom = rcTemp.top - offset;
			}
		}
		else
		{
			//��Ե��ǿͻ���
			//ֻ����ͻ����±�
			if(nBottom == bottom_client)
			{
				rcCtrl.bottom = rc.bottom;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		ScreenToClient(&rcCtrl);
		pWnd ->MoveWindow(rcCtrl);
	}
}

//##ModelId=49B87B860290
void CViewBase::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

//##ModelId=49B87B860295
BOOL CViewBase::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//	return TRUE;
	return CFormView::OnEraseBkgnd(pDC);
}
