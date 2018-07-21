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
 函 数 名：RecordRate()
 功能概要：记录控件比例,包括高度,宽度与对话框模板的比例, 高度宽度变化规则,
			位置变化规则. 在子类OnInitialUpdate()方法调用父类OnInitialUpdate()
			之前调用
 返 回 值: void
 参    数：param1   要记录的控件的ID
		   param2 	高度宽度改变规则,可能的值为0~3(见头文件)
		   param3 	左边相对的位置,可能的值为控件ID或者-1~-5外加-99(见头文件),下同
		   param4 	上边相对的位置
		   param5 	右边相对的位置
		   param6 	下边相对的位置
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
		
		
		//计算比例,保存
		ctrlPos cp;
		//控件宽度/客户区宽度
		cp.fWidth = (float)rcCtrl.Width() / (float)rc.Width();
		//控件高度/客户区高度
		cp.fHeight = (float)rcCtrl.Height() / (float)rc.Height();
		
		cp.nID = nID;
		cp.nType = nType;
		//////////////////////////////////////////////////////////////////////////
		cp.nID_Left = nLeft;
		if(nLeft != null_effect)
		{
			//左边有相对位置
			if(nLeft >= 0)
			{
				//相对的是控件
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
				case left_client: //客户区左边
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
			//有相对位置
			if(nRight >= 0)
			{
				//是相对其它控件
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
				 case left_client: //客户区左边
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
			//有相对位置
			if(nTop >= 0)
			{
				//相对的是其它控件
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
			//有相对位置
			if(nBottom >= 0)
			{
				//相对的是其它控件
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
 函 数 名：RegulateControlSize()
 功能概要：调整控件位置, 此函数已在本类在WM_SIZE消息处理中自动调用,子类不需要再调用
 返 回 值: void
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
				//左边有相对位置
				if(cp.nID_Left >= 0)
				{
					//相对的是其它控件
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
					//相对的是客户区
					switch(cp.nID_Left)
					{
					case left_client: //相对客户区左边
						rcCtrl.left = rc.left + cp.Left_Dis;
						break;
					case right_client: //相对客户区右边 
						rcCtrl.left = rc.right + cp.Left_Dis;
						break;
					case mid_client:	//相对客户区中间
						rcCtrl.left = rc.left + rc.Width()/2 + cp.Left_Dis;
						break;
					default:
						break;
					}
				}
				//判断宽度和高度是否有变化
			/*	int const	change_null = 0;	//宽度高度都不变
				int const	change_width = 1;	//宽度改变
				int const	change_height = 2;	//高度改变
				int	const	change_all	= 3;	//都改变*/
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
				//右边有相对位置
				if(cp.nID_Right >= 0)
				{
					//相对的是其它控件
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
					//相对的是客户区
					switch(cp.nID_Right)
					{
					case left_client: //相对客户区左边
						rcCtrl.right = rc.left + cp.Right_Dis;
						break;
					case right_client: //相对客户区右边 
						rcCtrl.right = rc.right + cp.Right_Dis;
						break;
					case mid_client:	//相对客户区中间
						rcCtrl.right = rc.left + rc.Width()/2 + cp.Right_Dis;
						break;
					default:
						break;
					}
				}
				//判断宽度和高度是否有变化
			/*	int const	change_null = 0;	//宽度高度都不变
				int const	change_width = 1;	//宽度改变
				int const	change_height = 2;	//高度改变
				int	const	change_all	= 3;	//都改变*/
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
				//上边有相对位置
				if(cp.nID_Top >= 0)
				{
					//相对的是其它控件
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
					//相对的是客户区
					switch(cp.nID_Top)
					{
					case top_client: //相对客户区上边
						rcCtrl.top = rc.top + cp.Top_Dis;
						break;
					case bottom_client: //相对客户区下边 
						rcCtrl.top = rc.bottom + cp.Top_Dis;
						break;
					default:
						break;
					}
				}
				//判断宽度和高度是否有变化
			/*	int const	change_null = 0;	//宽度高度都不变
				int const	change_width = 1;	//宽度改变
				int const	change_height = 2;	//高度改变
				int	const	change_all	= 3;	//都改变*/
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
				//下边有相对位置
				if(cp.nID_Bottom >= 0)
				{
					//相对的是其它控件
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
					//相对的是客户区
					switch(cp.nID_Bottom)
					{
					case top_client: //相对客户区上边
						rcCtrl.bottom = rc.top + cp.Bottom_Dis;
						break;
					case bottom_client: //相对客户区下边 
						rcCtrl.bottom = rc.bottom + cp.Bottom_Dis;
						break;
					default:
						break;
					}
				}
				//判断宽度和高度是否有变化
			/*	int const	change_null = 0;	//宽度高度都不变
				int const	change_width = 1;	//宽度改变
				int const	change_height = 2;	//高度改变
				int	const	change_all	= 3;	//都改变*/
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
 函 数 名：MoveCtrl()
 功能概要：移动控件位置, 此函数一般在子类的WM_SIZE消息响应中调用,在调用
			此函数前应先调用父类的WM_SIZE消息响应处理	
 返 回 值: void
 参    数：param1   要移动的控件的ID
		   param2 	左方位置,如果输入的是其它控件ID,即是此控件right + 偏移量
		   param3	上方位置,如果输入的是其它控件ID,即是此控件bottom + 偏移量
		   param4 	右方位置,如果输入的是其它控件ID,即是此控件left - 偏移量
		   param5 	下方位置,如果输入的是其它控件ID,即是此控件top - 偏移量
		   param6	偏移量
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
		//左方位置
		if(nLeft >= 0)
		{
			//是相对其它控件位置
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
			//是相对客户区
			//只相对的是左边客户区时才处理
			if(nLeft == left_client)
			{
				rcCtrl.left = rc.left;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//上方位置
		if(nTop >= 0)
		{
			//相对的是其它控件
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
			//相对的是客户区
			//只处理客户区上边
			if(nTop == top_client)
			{
				rcCtrl.top = rc.top;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//右边位置
		if(nRight >= 0)
		{
			//相对的是其它控件
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
			//相对的是客户区
			//只处理客户区右边
			if(nRight == right_client)
			{
				rcCtrl.right = rc.right;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//下边位置
		if(nBottom >= 0)
		{
			//相对的是其它控件
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
			//相对的是客户区
			//只处理客户区下边
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
