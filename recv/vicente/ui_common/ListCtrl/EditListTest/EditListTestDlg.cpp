// EditListTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditListTest.h"
#include "EditListTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditListTestDlg dialog

CEditListTestDlg::CEditListTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditListTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditListTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditListTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditListTestDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEditListTestDlg, CDialog)
	//{{AFX_MSG_MAP(CEditListTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, OnCustomDraw)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclk)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CHECK_ALL, OnCheckAll)
	ON_COMMAND(ID_CHECK_NULL, OnCheckNull)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditListTestDlg message handlers

BOOL CEditListTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	char* arColumn[4]={"����","�¼�ID","��������", "װ������ʱ��"};
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	for (int nCol=0; nCol < sizeof(arColumn)/sizeof(arColumn[0]) ; nCol++)
	{
		lvCol.iSubItem=nCol;
		switch(nCol)
		{
		case 0://����
			lvCol.cx = 60; //�п�50����
			break;
		case 1://�¼�ID
			lvCol.cx = 60; //�п�60����
			break;
		case 2://��������
			lvCol.cx = 60; //�п�60����
			break;
		case 3://װ������ʱ��
			lvCol.cx = 100; //�п�160����
			break;
		default:
			lvCol.cx=100;//�п�100����
		}
		lvCol.pszText=arColumn[nCol];
		m_list.InsertColumn(nCol,&lvCol);
	}

	//���÷��
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);

	int nCount = m_list.GetHeaderCtrl()->GetItemCount();

	for(int i = 0; i < 10; i++)
	{
		CString temp;
		temp.Format("%d", i);
		m_list.InsertItem(LVIF_TEXT, i, temp, 0, 0, 0, 0);
		for(int j = 1; j < nCount; j++)
		{
			CString str;
			str.Format("%d%d", i, j);
			m_list.SetItemText(i, j, str);
		}
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEditListTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEditListTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEditListTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEditListTestDlg::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	if(pNMHDR ->code == NM_CUSTOMDRAW)
	{
		LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
		switch(lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			{
				*pResult = CDRF_NOTIFYITEMDRAW;          // ask for item notifications.
				break;
			}
		case CDDS_ITEMPREPAINT:
			{
				*pResult = CDRF_NOTIFYSUBITEMDRAW;
				break;
			}
		case CDDS_ITEMPREPAINT | CDDS_SUBITEM :
			{
				int nItem = static_cast<int> (lplvcd->nmcd.dwItemSpec);
				int nSubItem = lplvcd->iSubItem;
				//�ȸ�Ĭ��ֵ, ���������⴦��
				lplvcd ->clrText = RGB(0, 0, 0);
				lplvcd ->clrTextBk = RGB(255, 255, 255);

				if(nItem == nSubItem) //����=����ʱ����������ɫ
				{
					lplvcd->clrText = RGB(255, 0, 0);
				}
				/*else
				{
					lplvcd->clrText = RGB(0, 0, 0);
					*pResult = CDRF_NEWFONT;
				}*/

				
				if(nSubItem == 2)//��3�����ñ���ɫ
				{
					lplvcd->clrTextBk = RGB(0, 255, 255);
				}
				/*else
				{
					lplvcd->clrTextBk = RGB(255, 255, 255);
					*pResult = CDRF_NEWFONT;
				}*/

				if(nItem == 3)//��4�����ñ���ɫ
				{
					lplvcd ->clrTextBk = RGB(0, 255, 0);
				}
				/*else
				{
					lplvcd ->clrTextBk = RGB(255, 255, 255);
					*pResult = CDRF_NEWFONT;
				}*/
				*pResult = CDRF_NEWFONT;
				break;
			}
		default:
			*pResult = CDRF_DODEFAULT;
		}
	}
}

void CEditListTestDlg::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	//ȡ�ñ�������к�
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	if (nItem == -1)
		return;
	if(nItem == 3)
		AfxMessageBox("˫���˵�4��");
	*pResult = 0;
}

void CEditListTestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	//ȷ�������޸�
	m_list.ClearEdited();
}

void CEditListTestDlg::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->iItem != -1)
    {
		if(pNMListView ->iSubItem == 1)
			m_list.ListSubItem(pNMListView->iItem, pNMListView ->iSubItem, "uuuu;yyyyyyyy;ssss;wwwwwwwww;s");
		if(pNMListView ->iSubItem == 2)
			m_list.EditSubItem(pNMListView ->iItem, pNMListView ->iSubItem);
     }
	*pResult = 0;
}

void CEditListTestDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	//CListCtrlEx�Ƚ�ȡ��WM_CONTEXTMENU��Ϣ, ���������ת��������ߴ���
	//���������ȵõ���Ϣ�����ٴ�����Ϣ
	LPMSG  msg;
	msg = (LPMSG)GetCurrentMessage();
	UINT wParam = (UINT)msg ->wParam;
	if(wParam == LISTCTRLEX_MARK)
	{
		CPoint * ptTemp = (CPoint*)msg->lParam;
		point = *ptTemp;

		//���ɲ˵�
		CMenu Menu, *pMenu;
		if(!Menu.LoadMenu(IDR_MENU1))
			return;
		pMenu = Menu.GetSubMenu(0);

		//�ж����һ�����ͷ�����б�,ֻ�����б�,��ͷ��CListCtrlEx�д���
		CPoint pt = point;
		m_list.ScreenToClient(&pt);

		CWnd* pChildWnd = m_list.ChildWindowFromPoint(pt);
		if(pChildWnd != NULL && pChildWnd->GetSafeHwnd() != m_list.GetSafeHwnd())
		{
			char szClass [50];

			// Verify that this window handle is indeed the header
			// control's by checking its classname.
			GetClassName(pChildWnd->GetSafeHwnd(), szClass, 50);
			if (!lstrcmp (szClass, "SysHeader32"))
			{
			//	AfxMessageBox("���һ�����ͷ");
			}
		}
		else if(pChildWnd != NULL && pChildWnd->GetSafeHwnd() == m_list.GetSafeHwnd())
		{//�������ListCtrl�ķ���ͷ����
			//�����˵�
			pMenu ->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);	
		}
	}
	
}

void CEditListTestDlg::OnCheckAll() 
{
	// TODO: Add your command handler code here
	int nCount = m_list.GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		m_list.SetCheck(i, TRUE);
	}
}

void CEditListTestDlg::OnCheckNull() 
{
	// TODO: Add your command handler code here
	int nCount = m_list.GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		m_list.SetCheck(i, FALSE);
	}
}

void CEditListTestDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	BOOL bReturn = m_list.GetColumnIsHide(2);
	m_list.SetColumnHide(2, !bReturn);
}
