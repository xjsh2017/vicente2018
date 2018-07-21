// btndemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "btndemo.h"
#include "btndemoDlg.h"

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
// CBtndemoDlg dialog

CBtndemoDlg::CBtndemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBtndemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBtndemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBtndemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBtndemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BUTTON1, m_btnStandard);
	DDX_Control(pDX, IDC_BUTTON2, m_btnOption);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBtndemoDlg, CDialog)
	//{{AFX_MSG_MAP(CBtndemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_COMMAND(ID_MENU_1, OnMenu1)
	ON_COMMAND(ID_MENU_2, OnMenu2)
	ON_COMMAND(ID_MENU_3, OnMenu3)
	ON_COMMAND(ID_MENU_4, OnMenu4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBtndemoDlg message handlers

BOOL CBtndemoDlg::OnInitDialog()
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
	//加入标准按钮
	m_btnStandard.SetThemeHelper(&m_ThemeHelper);
	m_btnStandard.SetIcon(IDI_ICON1);
	m_btnStandard.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnStandard.SetTooltipText(_T("我酷吗?"));
	m_btnStandard.SetRounded(TRUE);

	//加入选项按钮
	m_btnOption.SetThemeHelper(&m_ThemeHelper);
	m_btnOption.SetIcon(IDI_ICON4);
	m_btnOption.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, 30);
	m_btnOption.SetTooltipText(_T("选择"));	
#ifdef	BTNST_USE_BCMENU
	m_btnOption.SetMenu(IDR_MENU1, m_hWnd, TRUE, IDR_TOOLBAR1);
#else
	m_btnOption.SetMenu(IDR_MENU1, m_hWnd);
#endif
	m_btnOption.SetMenuCallback(m_hWnd, WM_USER + 100);
	m_btnOption.SetRounded(TRUE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBtndemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBtndemoDlg::OnPaint() 
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
HCURSOR CBtndemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBtndemoDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("按下了按钮1");
}

void CBtndemoDlg::OnMenu1() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("选择了1");
}

void CBtndemoDlg::OnMenu2() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("选择了2");
}

void CBtndemoDlg::OnMenu3() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("选择了3");
}

void CBtndemoDlg::OnMenu4() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("选择了4");
}
