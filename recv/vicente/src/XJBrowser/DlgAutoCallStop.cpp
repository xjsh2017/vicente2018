// DlgAutoCallStop.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgAutoCallStop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoCallStop dialog


//##ModelId=49B87BA402FF
CDlgAutoCallStop::CDlgAutoCallStop(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoCallStop::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoCallStop)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hRoot = NULL;
	m_nStationNum = 0;
	m_pAutoCall = NULL;
}


//##ModelId=49B87BA4032C
void CDlgAutoCallStop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoCallStop)
	DDX_Control(pDX, IDC_LIST_AUOTCALL_STOP, m_List);
	DDX_Control(pDX, IDC_TREE_AUTOCAL_STOP, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoCallStop, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoCallStop)
	ON_NOTIFY(NM_CLICK, IDC_TREE_AUTOCAL_STOP, OnClickTreeAutocalStop)
	ON_BN_CLICKED(IDC_BTN_AUTOCALL_STOP, OnBtnAutocallStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*************************************************************
 �� �� ����InitTree()
 ���ܸ�Ҫ����ʼ���豸��
 �� �� ֵ: ��ʼ���ɹ�����>=0��ֵ, ʧ�ܷ���<0��ֵ
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA4032F
int CDlgAutoCallStop::InitTree()
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

/*************************************************************
 �� �� ����BuildNet()
 ���ܸ�Ҫ�����豸���д�������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40330
void CDlgAutoCallStop::BuildNet()
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
 �� �� ����InsertNet()
 ���ܸ�Ҫ����������ڵ�
 �� �� ֵ: ����ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	Ҫ����ĵ�������
		   Param2
**************************************************************/
//##ModelId=49B87BA4033C
BOOL CDlgAutoCallStop::InsertNet( CNetObj* pObj )
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
	pObj->m_Tag2 = htNew;
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
//##ModelId=49B87BA4033E
BOOL CDlgAutoCallStop::InsertStation( CStationObj* pObj )
{
	//��վ�ڵ��ڵ����ڵ�����, Ϊ������
	//�Ȳ��ҵ����ڵ�
	HTREEITEM htNet = FindNetPos(pObj->m_sNetID);
	if(htNet == NULL)
		return FALSE;
	
	//����ڵ�
	HTREEITEM htNew = m_Tree.InsertItem(pObj->m_sName, htNet);
	//��¼�ڵ�
	pObj->m_Tag2 = htNew;
	//�豸����
	m_Tree.SetItemData(htNew, (DWORD)pObj);
	//����
	m_Tree.SortChildren(htNet);

	return TRUE;
}

/*************************************************************
 �� �� ����BuildStation()
 ���ܸ�Ҫ�����豸���д�����վ
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40340
void CDlgAutoCallStop::BuildStation()
{
	//ȡ����������
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if(pData == NULL)
		return;
	//���г�վ
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		m_nStationNum++;
		//ȡ�õ���, ������
		CStationObj* pObj = (CStationObj*)listDevice.GetNext(pos);
		if(pObj != NULL && !pObj->m_IsDistanceVirtual && pObj->m_nStationType != 113 && pObj->m_nStationType != 114 )
			InsertStation(pObj);
	}
}

/*************************************************************
 �� �� ����FindNetPos()
 ���ܸ�Ҫ�����ҵ��������е�λ��
 �� �� ֵ: ���ڵ�ṹ
 ��    ����param1	����ID
		   Param2
**************************************************************/
//##ModelId=49B87BA4034B
HTREEITEM CDlgAutoCallStop::FindNetPos( CString sNetID )
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
 �� �� ����CycTreeDown()
 ���ܸ�Ҫ�����±���ָ���ڵ�������ӽڵ�, �������ӽڵ�����ͬ��ѡ�����
 �� �� ֵ: void
 ��    ����param1	ָ���ڵ�
		   Param2	ѡ���־.TRUEΪѡ��, FALSEΪȡ��ѡ��
**************************************************************/
//##ModelId=49B87BA4034D
void CDlgAutoCallStop::CycTreeDown( HTREEITEM hRoot, BOOL bMark )
{
	if(hRoot != NULL)
	{
		//����ѡ��/ȡ��ѡ��
		m_Tree.SetCheck(hRoot, bMark);
		CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hRoot);
		if(pObj != NULL)
		{
			if(bMark)
				CheckDevice(pObj, 1); //ѡ��
			else
				CheckDevice(pObj, 0); //ȡ��ѡ��
		}
		//��������
		CycTreeDown(m_Tree.GetChildItem(hRoot), bMark);
		CycTreeDown(m_Tree.GetNextItem(hRoot, TVGN_NEXT), bMark);
	}
}

/*************************************************************
 �� �� ����CycTreeUp()
 ���ܸ�Ҫ�����ϱ����ڵ�ĸ��ڵ�, �����ǰ�ڵ��Ǹ��ڵ�����һ����ѡ����ӽڵ�, Ϊ���ڵ�Ҳ����ѡ���־
 �� �� ֵ: void
 ��    ����param1	ָ���ڵ�
		   Param2
**************************************************************/
//##ModelId=49B87BA40350
void CDlgAutoCallStop::CycTreeUp( HTREEITEM hCurrent )
{
	HTREEITEM hParent, hChild;
	if(hCurrent != NULL)
	{
		//ȡ�ø��ڵ�
		hParent = m_Tree.GetParentItem(hCurrent);
		if(hParent != NULL)
		{
			//�������ڵ�������ӽڵ�, ���Ƿ�ȫ��ѡ����
			hChild = m_Tree.GetChildItem(hParent);
			while(hChild != NULL)
			{
				if(hChild != hCurrent)
				{
					if(!m_Tree.GetCheck(hChild) )
						return ;
				}
				hChild = m_Tree.GetNextItem(hChild, TVGN_NEXT);
			}
			
			m_Tree.SetCheck(hParent, TRUE);
		}
		CycTreeUp(m_Tree.GetParentItem(hCurrent));
	}
}

/*************************************************************
 �� �� ����HitItemHelp()
 ���ܸ�Ҫ������ڵ㱻������
 �� �� ֵ: void
 ��    ����param1	������Ľڵ�
		   Param2
**************************************************************/
//##ModelId=49B87BA4035C
void CDlgAutoCallStop::HitItemHelp( HTREEITEM hit )
{
	HTREEITEM hChild, hParent;
	BOOL bMark;
	if(hit != NULL)
	{
		if(hit != NULL)
		{
			hChild = m_Tree.GetChildItem(hit);
			hParent = m_Tree.GetParentItem(hit);
			
		//	UpdateData(TRUE);
			if(!m_Tree.GetCheck(hit))
			{
				//��ѡ��
				CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hit);
				if(pObj != NULL)
					CheckDevice(pObj, 1);
				bMark = TRUE;
				CycTreeDown(hChild, bMark);		//�����ڵ�ѡ��ʱ,�ӽڵ�ȫ��ѡ��
				CycTreeUp(hit);					//���ӽڵ�ȫ��ѡ��ʱ,���ڵ�ѡ��
			}
			else
			{
				//ȡ��ѡ��
				CDeviceObj* pObj = (CDeviceObj*)m_Tree.GetItemData(hit);
				if(pObj != NULL)
					CheckDevice(pObj, 0);
				bMark = FALSE;
				CycTreeDown(hChild, bMark);		//�����ڵ�ȡ��ѡ��ʱ,�ӽڵ�ȫ��ȡ��ѡ��
				
				while(hParent != NULL)			//���ӽڵ���һ��ȡ��ѡ��ʱ,���ڵ�ȡ��ѡ��
				{
					m_Tree.SetCheck(hParent, FALSE);
					hParent = m_Tree.GetParentItem(hParent);
				}
			}
		}		
	}
}
/////////////////////////////////////////////////////////////////////////////
// CDlgAutoCallStop message handlers

//##ModelId=49B87BA4037A
BOOL CDlgAutoCallStop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//��ʼ���豸��
	InitTree();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 �� �� ����CheckDevice()
 ���ܸ�Ҫ����ĳ�豸������Check����
 �� �� ֵ: void
 ��    ����param1	�豸ָ��
		   Param2	1��ʾѡ��, 0��ʾȡ��ѡ��
**************************************************************/
//##ModelId=49B87BA4035E
void CDlgAutoCallStop::CheckDevice( CDeviceObj* pObj, int nSelected )
{
	//�������������
	if(pObj == NULL)
		return;
	if(nSelected != 0 && nSelected != 1) //��־ֵ��Ч
		return;
	if(pObj->m_nType != TYPE_STATION) //������ǳ�վ,�����豸��Ч
		return;
	if(1 == nSelected)
	{
		//ѡ��
		ListAddStation(pObj);
	}
	if(0 == nSelected)
	{
		//ȡ��ѡ��
		ListRemoveStation(pObj);
	}
}

//##ModelId=49B87BA4037C
void CDlgAutoCallStop::OnClickTreeAutocalStop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	UINT uFlags = 0;
	HWND hWnd ;
	
	GetCursorPos(&pt);
	hWnd = ::WindowFromPoint(pt);
	::ScreenToClient(hWnd, &pt);
	
	HTREEITEM  hit = m_Tree.HitTest(pt, &uFlags);
	if(uFlags & TVHT_ONITEMSTATEICON )
	{
		HitItemHelp(hit);
	}

	*pResult = 0;
}

/*************************************************************
 �� �� ����ListAddStation()
 ���ܸ�Ҫ���ѳ�վ���뵽�б�����ʾ
 �� �� ֵ: 
 ��    ����param1	�豸ָ��
		   Param2
**************************************************************/
//##ModelId=49B87BA4036B
void CDlgAutoCallStop::ListAddStation( CDeviceObj* pObj )
{
	//���������Ч��
	if(pObj == NULL)
		return;
	if(m_List.GetSafeHwnd() == NULL)
		return;

	//�������б��е�λ��
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pObj;
	int nIndex = m_List.FindItem(&lvFind);

	if(nIndex > -1)
	{
		//�Ѵ���
		return;
	}

	//����
	nIndex = m_List.InsertItem(0, pObj->m_sName);
	//��������
	m_List.SetItemData(nIndex, (DWORD)pObj);
}

/*************************************************************
 �� �� ����ListRemoveStation()
 ���ܸ�Ҫ���ѳ�վ���б����Ƴ�
 �� �� ֵ: 
 ��    ����param1	�豸ָ��
		   Param2
**************************************************************/
//##ModelId=49B87BA4036D
void CDlgAutoCallStop::ListRemoveStation( CDeviceObj* pObj )
{
	//���������Ч��
	if(pObj == NULL)
		return;
	if(m_List.GetSafeHwnd() == NULL)
		return;
	
	//�������б��е�λ��
	LVFINDINFO lvFind;
	lvFind.flags = LVFI_PARAM;
	lvFind.lParam = (LPARAM)pObj;
	int nIndex = m_List.FindItem(&lvFind);
	
	if(nIndex == -1)
	{
		//������
		return;
	}

	//�Ƴ�
	m_List.DeleteItem(nIndex);
}

/*************************************************************
 �� �� ����ConnectAutoCall()
 ���ܸ�Ҫ�����Զ�����ģ�齨������
 �� �� ֵ: ���ӳɹ�����1, ʧ�ܷ���0
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA4036F
int CDlgAutoCallStop::ConnectAutoCall()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//�齨�ͻ��˽ṹ
	STRUCT_NETADDR_INFO addrInfo;
	bzero(&addrInfo, sizeof(STRUCT_NETADDR_INFO));
	strncpy(addrInfo.IpAddr, g_AutoCall_IP, 40);
	addrInfo.nPort = g_AutoCall_Port;
	STRUCT_SERVER_INFO serverInfo;
	bzero(&serverInfo, sizeof(STRUCT_SERVER_INFO));
	serverInfo.bHasReservedAddr = false;
	serverInfo.netAddrA = addrInfo;
	STRUCT_DESTINATION_SERVER destInfo;
	bzero(&destInfo, sizeof(STRUCT_DESTINATION_SERVER));
	destInfo.bHasReservedServer = false;
	destInfo.serverA = serverInfo;
	//����
	int nResult = 0;
	CString sSysLogPath = pApp->GetSysLogPath();
	char * pPath = (char*)sSysLogPath.GetBuffer(0);
	m_pAutoCall = pApp->m_SttpEngine.XJSTTPConnectDestination(destInfo, STTPNET_MODLE_STFRONT, nResult, pPath, STTPNET_RUNMODLE_SYNC);
	sSysLogPath.ReleaseBuffer(0);
	if(nResult == 0)
	{
		if(m_pAutoCall != NULL)
		{
			//�Ͽ�
			pApp->m_SttpEngine.XJSttpCloseConnection(m_pAutoCall);
		}
		m_pAutoCall = NULL;
	}
	return nResult;
}

/*************************************************************
 �� �� ����DisConnectAutoCall()
 ���ܸ�Ҫ���ر����Զ����ٵ�����
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87BA40370
void CDlgAutoCallStop::DisConnectAutoCall()
{
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(m_pAutoCall != NULL)
	{
		pApp->m_SttpEngine.XJSttpCloseConnection(m_pAutoCall);
		m_pAutoCall = NULL;
	}
}

//##ModelId=49B87BA40380
void CDlgAutoCallStop::OnBtnAutocallStop() 
{
	// TODO: Add your control notification handler code here
	int nNum = m_List.GetItemCount();
	if(nNum < 1)
	{
		//û��ѡ��վ
		AfxMessageBox( StringFromID(IDS_STOPAUTOCALL_SELECTSTATION));
		return;
	}
	
	//��֯����
	CStringArray arrStation;
	for(int i = 0; i < m_List.GetItemCount(); i++)
	{
		CDeviceObj* pObj = (CDeviceObj*)m_List.GetItemData(i);
		arrStation.Add(pObj->m_sID);
	}
	STTP_FULL_DATA sttpData;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	if(nNum == m_nStationNum)
	{
		//ȫ����վ
		if(!pApp->m_SttpEngine.BuildDataFor20153Batch(sttpData, 3, NULL))
		{
			AfxMessageBox( StringFromID(IDS_COMMON_BUILDMSG_FAIL));
			return; //�齨����ʧ��
		}
	}
	else
	{
		if(!pApp->m_SttpEngine.BuildDataFor20153Batch(sttpData, 3, &arrStation))
		{
			AfxMessageBox( StringFromID(IDS_COMMON_BUILDMSG_FAIL));
			return; //�齨����ʧ��
		}
	}
	
	//���ͱ���
	int nResult = pApp->m_SttpEngine.XJSTTPWrite(pApp->m_SttpHandle, sttpData);
	if(nResult != 1)
	{
		AfxMessageBox( StringFromID(IDS_STOPAUTOCALL_SENDFAIL));
		return; //����ʧ��
	}
	//
	AfxMessageBox( StringFromID(IDS_STOPAUTOCALL_SENDSUCCESS));
}
