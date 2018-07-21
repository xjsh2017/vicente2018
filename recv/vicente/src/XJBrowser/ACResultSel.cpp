// ACResultSel.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "ACResultSel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CACResultSel

IMPLEMENT_DYNCREATE(CACResultSel, CFormView)

//##ModelId=49B87BB50205
CACResultSel::CACResultSel()
	: CFormView(CACResultSel::IDD)
{
	//{{AFX_DATA_INIT(CACResultSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hRoot = NULL;
}

//##ModelId=49B87BB50227
CACResultSel::~CACResultSel()
{
}

//##ModelId=49B87BB50224
void CACResultSel::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACResultSel)
	DDX_Control(pDX, IDC_TREE_ACRESULTSEL, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CACResultSel, CFormView)
	//{{AFX_MSG_MAP(CACResultSel)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ACRESULTSEL, OnSelchangedTreeAcresultsel)
	//}}AFX_MSG_MAP
	ON_MESSAGE(AUTOCALL_RESULT_OPEN, OnAutoCallResultFrameOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACResultSel diagnostics

#ifdef _DEBUG
//##ModelId=49B87BB50229
void CACResultSel::AssertValid() const
{
	CFormView::AssertValid();
}

//##ModelId=49B87BB5022B
void CACResultSel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/*************************************************************
 �� �� ����InsertNet()
 ���ܸ�Ҫ����������ڵ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	Ҫ����ĵ�������
		   Param2
**************************************************************/
//##ModelId=49B87BB50213
BOOL CACResultSel::InsertNet( CNetObj* pObj )
{
	//���ڵ�Ϊ"����"
	//�����ڵ��ڵڶ���
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL)
		return FALSE;
	
	//����ڵ�
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, htRoot);
	if(htNew == NULL)
		return FALSE;
	//��¼λ��
	pObj->m_Tag1 = htNew;
	//��������
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//����
	//m_Tree.SortChildren(htRoot);
	return TRUE;
}

/*************************************************************
 �� �� ����InsertStation()
 ���ܸ�Ҫ�����볧վ�ڵ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	Ҫ����ĳ�վ����
		   Param2
**************************************************************/
//##ModelId=49B87BB50215
BOOL CACResultSel::InsertStation( CStationObj* pObj )
{
	//��վ�ڵ��ڵ����ڵ�����, Ϊ������
	//�Ȳ��ҵ����ڵ�
	HTREEITEM htNet = FindNetPos(pObj->m_sNetID);
	if(htNet == NULL)
		return FALSE;
	
	//����ڵ�
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, htNet);
	//��¼�ڵ�
	//	pObj->m_htItem = htNew;
	pObj->m_Tag1 = htNew;
	//�豸����
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//����
	m_Tree.SortChildren(htNet);
	return TRUE;
}

/*************************************************************
 �� �� ����FindNetPos()
 ���ܸ�Ҫ�����ҵ��������е�λ��
 �� �� ֵ: ���ڵ�ṹ
 ��    ����param1	����ID
		   Param2
**************************************************************/
//##ModelId=49B87BB50217
HTREEITEM CACResultSel::FindNetPos( CString sNetID )
{
	HTREEITEM htReturn = NULL;
	//���ڵ�
	HTREEITEM htRoot = m_Tree.GetRootItem();
	if(htRoot == NULL)
		return NULL;
	//��һ�������ڵ�
	HTREEITEM htNetItem = m_Tree.GetChildItem(htRoot);
	
	//ѭ������
	while(htNetItem != NULL)
	{
		CNetObj * pObj = (CNetObj*)m_Tree.GetItemData(htNetItem);
		if(pObj->m_sID == sNetID)
		{
			htReturn = htNetItem;
			break;
		}
		htNetItem = m_Tree.GetNextItem(htNetItem, TVGN_NEXT);
	}
	return htReturn;
}

/*************************************************************
 �� �� ����RegulateControlSize()
 ���ܸ�Ҫ�������ؼ���С,λ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB50219
void CACResultSel::RegulateControlSize()
{
	if(m_Tree.GetSafeHwnd() == NULL)
		return;
	
	CRect rc;
	GetClientRect(&rc);
	CRect rcList = rc;
	//�����豸����С
	CRect rcTree = rc;
	m_Tree.MoveWindow(rcTree);
}


/////////////////////////////////////////////////////////////////////////////
// CACResultSel message handlers

//##ModelId=49B87BB50233
void CACResultSel::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	RegulateControlSize();
}

/*************************************************************
 �� �� ����BuildNet()
 ���ܸ�Ҫ�����豸���д�������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5021A
void CACResultSel::BuildNet()
{
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������е���
	POSITION pos = pData->m_NetList.GetHeadPosition();
	while(pos != NULL)
	{
		//ȡ�õ���, ������
		CNetObj* pObj = (CNetObj*)pData->m_NetList.GetNext(pos);
		InsertNet(pObj);
	}
}

/*************************************************************
 �� �� ����BuildStation()
 ���ܸ�Ҫ�����豸���д�����վ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5021B
void CACResultSel::BuildStation()
{
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//�������г�վ
	DEVICE_LIST listStation;
	listStation.RemoveAll();
	pData->GetStationList(listStation);
	POSITION pos = listStation.GetHeadPosition();
	while(pos != NULL)
	{
		//ȡ�õ���, ������
		CStationObj* pObj = (CStationObj*)listStation.GetNext(pos);
		if(pObj != NULL && !pObj->m_IsDistanceVirtual && pObj->m_nStationType != 113 && pObj->m_nStationType != 114 )
			InsertStation(pObj);
	}
}

/*************************************************************
 �� �� ����InitTree()
 ���ܸ�Ҫ����ʼ���豸��
 �� �� ֵ: ��ʼ���ɹ�����>=0��ֵ, ʧ�ܷ���<0��ֵ
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB5021C
int CACResultSel::InitTree()
{
	//���
	m_Tree.DeleteAllItems();
	//�������ڵ�
	m_hRoot = m_Tree.InsertItem(STRING_NET, NULL);
	//��������
	BuildNet();
	//������վ
	BuildStation();
	return 0;
}

//##ModelId=49B87BB50222
void CACResultSel::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal(10, 10);
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/*************************************************************
 �� �� ����OnAutoCallResultFrameOpen()
 ���ܸ�Ҫ����Ӧ���ڴ���Ϣ, �����ؼ�
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BB50242
void CACResultSel::OnAutoCallResultFrameOpen( WPARAM wParam, LPARAM lParam )
{
	InitTree();
}

//##ModelId=49B87BB50238
void CACResultSel::OnSelchangedTreeAcresultsel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM ht = m_Tree.GetSelectedItem();
	if(ht != NULL)
	{
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(ht);
		//֪ͨ��ʾ����
		CWnd* pWnd = GetParent()->GetParent(); //������
		//������Ϣ
		if(pWnd != NULL)
		{
			pWnd->PostMessage(AUTOCALL_RESULT_CHANGE, 0, (LPARAM)pObj);
		}
	}
	*pResult = 0;
}
