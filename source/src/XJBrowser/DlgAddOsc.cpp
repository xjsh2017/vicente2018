// DlgAddOsc.cpp : implementation file
//

#include "stdafx.h"
#include "xjbrowser.h"
#include "DlgAddOsc.h"
#include "DlgFaultReport.h"
#include "GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddOsc dialog

extern int g_iStationInfoViewAsc;
CDlgAddOsc::CDlgAddOsc(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddOsc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddOsc)
	m_sPath = _T("");
	m_sDir = _T("");
	m_sOldDir = _T("");
	m_nMS = 0;
	//}}AFX_DATA_INIT
	m_pReport = NULL;
	m_nOldSortCol = -1;
	g_iStationInfoViewAsc = -1;
	m_listOSC.RemoveAll();
}


void CDlgAddOsc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddOsc)
	DDX_Control(pDX, IDC_CMB_ADDOSC_SEC, m_cmbSec);
	DDX_Control(pDX, IDC_CMB_ADDOSC_STATION, m_cmbStation);
	DDX_Control(pDX, IDC_LIST_ADDOSC, m_List);
	DDX_Text(pDX, IDC_EDIT_ADDOSC_PATH, m_sPath);
	DDX_Text(pDX, IDC_EDIT_ADDOSC_MS, m_nMS);
	DDV_MinMaxInt(pDX, m_nMS, 0, 999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddOsc, CDialog)
	//{{AFX_MSG_MAP(CDlgAddOsc)
	ON_BN_CLICKED(IDC_BTN_ADDOSC_SKIN, OnBtnAddoscSkin)
	ON_BN_CLICKED(IDC_BTN_ADDOSC_ADD, OnBtnAddoscAdd)
	ON_BN_CLICKED(IDC_BTN_ADDOSC_REMOVE, OnBtnAddoscRemove)
	ON_BN_CLICKED(IDC_BTN_ADDOSC_MODIFY, OnBtnAddoscModify)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ADDOSC, OnItemchangedListAddosc)
	ON_CBN_SELCHANGE(IDC_CMB_ADDOSC_STATION, OnSelchangeCmbAddoscStation)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_ADDOSC, OnColumnclickListAddosc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddOsc message handlers

BOOL CDlgAddOsc::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    MYASSERT_BOOL(m_pReport);
	//�б�
	InitList();
	//��վ
	FillCMBStation();
	//ȡ�ó�վ
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_STATION);
	CStationObj* pStation = NULL;
	if(pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		pStation = (CStationObj*)pCmb->GetItemData(nIndex);
	}

	if ( pStation == NULL || (DWORD)pStation == -1 )
	{
		AfxMessageBox( StringFromID(IDS_COMMON_NEEDSTATION) );
		return FALSE;
	}

	//�����豸
	FillCMBSec(pStation);
	//ʱ��
	CDateTimeCtrl* pTime = (CDateTimeCtrl*)GetDlgItem(IDC_DP_ADDOSC_TIME);
	if(pTime == NULL)
		return FALSE;
	pTime->SetFormat(_T("yyyy-MM-dd HH:mm:ss"));
	//���÷�Χ
	CTime tmMin(1971, 1, 1, 1, 1, 1);
	CTime tmMax(2036, 12, 12, 12, 12, 12);
	pTime->SetRange(&tmMin, &tmMax);
	//����ʱ��
	CTime tm = m_pReport->m_tmFaultTime;
	VERIFY(pTime->SetTime(&tm));
	//����
	m_nMS = m_pReport->m_nmsFaultTime;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 �� �� ��: FillCMBStation()
 ���ܸ�Ҫ: ��䳧վ����������,ֻ����Ӧ���Ϲ鵵�еĳ�վ
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDlgAddOsc::FillCMBStation()
{
	//���������Ч��
	if(m_pReport == NULL)
		return;
	//��վ�����
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_STATION);
	if(pCmb == NULL)
		return;
	pCmb->ResetContent();
	//��䳧վ1
	if(m_pReport->m_pStation1 != NULL)
	{
		int nIndex = pCmb->AddString(m_pReport->m_pStation1->m_sName);
		pCmb->SetItemData(nIndex, (DWORD)m_pReport->m_pStation1);
	}
	//��䳧վ2
	if(m_pReport->m_pStation2 != NULL)
	{
		int nIndex = pCmb->AddString(m_pReport->m_pStation2->m_sName);
		pCmb->SetItemData(nIndex, (DWORD)m_pReport->m_pStation2);
	}
	//Ĭ��ѡ���һ��
	if(pCmb->GetCount() > 0)
		pCmb->SetCurSel(0);
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 �� �� ��: FillCMBSec()
 ���ܸ�Ҫ: ���ָ����վ�Ķ����豸���ݵ�������
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 
**************************************************************/
void CDlgAddOsc::FillCMBSec( CStationObj* pStation )
{
	//���������Ч��
	MYASSERT(pStation);
	//��վ�����
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_SEC);
	MYASSERT(pCmb);
	pCmb->ResetContent();
	//�������
	//ѭ����վ�µı�����¼����
	CXJBrowserApp *pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetSecList(listDevice, "", TYPE_SEC, pStation->m_sID);
	POSITION pos = listDevice.GetHeadPosition();
	while(pos != NULL)
	{
		CSecObj * pObj = (CSecObj*)listDevice.GetNext(pos);
		if(pObj == NULL)
			continue;
		if(pObj->m_nType != TYPE_PROTECT && pObj->m_nType != TYPE_WAVEREC)
			continue;
		//���뵽������
		int nIndex = pCmb->AddString(pObj->m_sName);
		if(nIndex < 0)
			continue;
		//���ù���
		pCmb->SetItemData(nIndex, (DWORD)pObj);
	}
	
	//Ĭ��ѡ���һ��
	if(pCmb->GetCount() > 0)
		pCmb->SetCurSel(0);
	m_cmbSec.OnDataFilled(TRUE, 0);
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 �� �� ��: InitList()
 ���ܸ�Ҫ: ��ʼ���б�
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CDlgAddOsc::InitList()
{
	//��ͼ���б�, ���������и�
	CImageList   m_l;   
	m_l.Create(1,g_ListItemHeight,TRUE|ILC_COLOR32,1,0);   
	m_List.SetImageList(&m_l,LVSIL_SMALL);
	
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_CENTER;
	
	
	//�����п�,��С����
	for (int nCol=0; nCol < 3 ; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0://
			lvCol.cx = 300; // �����豸
			colName = StringFromID(IDS_MODEL_SECONDARY);
			break;
		case 1://
			lvCol.cx = 160; // ʱ��
			colName = StringFromID(IDS_COMMON_FAULTTIME);
			break;
		case 2://
			lvCol.cx = 180; // �ļ���
			colName = StringFromID(IDS_COMMON_FILENAME);
			break;
		default:
			lvCol.cx = 100;
			break;
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_List.InsertColumn(nCol,&lvCol);
	}
	
	//���÷��
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT);
	m_List.ModifyStyle(0, LVS_SHOWSELALWAYS);
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 �� �� ��: AddEventToList()
 ���ܸ�Ҫ: ���ָ���¼����б�
 �� �� ֵ: ��ӳɹ�����TRUE,ʧ�ܷ���FALSE
 ��    ��: param1 ָ���¼�
		   Param2 ��ӵ��б��λ��
**************************************************************/
BOOL CDlgAddOsc::AddEventToList( CFaultEvent* pEvent, int nIndex )
{
	//���������Ч��
	if(pEvent == NULL)
		return FALSE;
	if(nIndex < 0)
		return FALSE;
	//"�����豸","����ʱ��","�ļ�����"
	CString str = "";
	if(pEvent->m_pSec != NULL)
		str.Format("%s", pEvent->m_pSec->m_sName);
	//0:�����豸
	if(m_List.InsertItem(LVIF_TEXT|LVIF_PARAM, nIndex, str, 0, 0, 0, nIndex) == -1)
		return FALSE;
	//1:ʱ��
	str.Format("%s.%03d", pEvent->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent->m_nmsStart);
	m_List.SetItemText(nIndex, 1, str);
	//2:�¼�����
	m_List.SetItemText(nIndex, 2, pEvent->m_sEventDef);
	//�����豸
	m_List.SetItemData(nIndex, (DWORD)pEvent);

	return TRUE;
}

CDlgAddOsc::~CDlgAddOsc()
{
	for(int i = 0; i < m_listOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
		delete pEvent;
	}
	m_listOSC.RemoveAll();	
}

void CDlgAddOsc::OnBtnAddoscSkin() 
{
	// TODO: Add your control notification handler code here
	//ȡ�õ�ǰѡ��Ķ����豸
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_SEC);
	MYASSERT(pCmb);
	int nIndex = pCmb->GetCurSel();
	CSecObj* pSec = (CSecObj*)pCmb->GetItemData(nIndex);
	MYASSERT(pSec);
	if(pSec->m_nType != TYPE_PROTECT && pSec->m_nType != TYPE_WAVEREC)
		return;
	//ȡ�õ�ǰϵͳ·��
	DWORD cchCurDir = MAX_PATH; 
    LPTSTR lpszCurDir; 
	TCHAR tchBuffer[MAX_PATH];
	for(int i = 0; i < MAX_PATH; i++)
	{
		tchBuffer[i] = '\0';
	}
	lpszCurDir = tchBuffer; 
    GetCurrentDirectory(cchCurDir, lpszCurDir);

	//ָ��·��
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString strSTDown = pApp ->GetDownDir();

	m_sDir = "";
	m_sOldDir = "";

	if(pSec->m_nType == TYPE_PROTECT)
	{
		m_sDir = pSec->m_sStationID + "\\����\\" + pSec->m_sName + "(" + pSec->m_sID + ")" + "\\";
        strSTDown += m_sDir;
	}
	else if(pSec->m_nType == TYPE_WAVEREC)
	{
		m_sDir += pSec->m_sStationID + "\\¼����\\" + pSec->m_sName + "(" + pSec->m_sID + ")" + "\\";
		strSTDown += m_sDir;
	}

	//��ѡ���ļ��Ի���,���û�ѡ���ļ�
	static char szFilter[] = "CFG Files(*.cfg)|*.cfg|All Files(*.*)|*.*||";
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	dlgFile.m_ofn.lpstrTitle = StringFromID(IDS_ADDOSC_SELECTFILE); //�Ի������
	//dlgFile.m_ofn.lpstrFilter =_T("*.cfg"); //����
	dlgFile.m_ofn.lpstrInitialDir = strSTDown;
	if(dlgFile.DoModal() == IDOK)
	{
		//�õ�ȫ·��
		CString strFullPath = dlgFile.GetPathName();
		//������·����ת����Сд���ж�
		CString tempFullPath = strFullPath;
		CString tempSTDown = strSTDown;
		tempFullPath.MakeLower();
		tempSTDown.MakeLower();
	
	
#ifdef  NDEBUG
		if(tempFullPath.Find(tempSTDown, 0) == -1) //�ж�ѡ����ļ��Ƿ���STDownĿ¼��
		{
			
			CString str;
			str.Format("%s:\n%s", StringFromID(IDS_ADDOSC_PATHERROR),strSTDown);
			AfxMessageBox(str);
	
		}
		else //��STDownĿ¼��
		{
#endif
			m_sPath = dlgFile.GetFileName();
			CFile file;
			if ( file.Open(dlgFile.GetPathName(), CFile::modeRead) ) 
			{
				//�õ��ļ�����
				m_nFileSize = file.GetLength();
				CString strLog;
				strLog.Format(_T("CDlgAddOsc::OnBtnAddoscSkin ��ȡ�ļ����ȳɹ�������Ϊ%d"), m_nFileSize);
				WriteLog(strLog, 3);
			}
 
			//ȥ��׺
			int nFind = m_sPath.ReverseFind('.');
			if(nFind >= 0)
			{
				m_sPath = m_sPath.Left(nFind);
			}

			if (g_bSend20162)
			{
				SYSTEMTIME tNow; 
				GetLocalTime(&tNow);
				CString strTime;
				strTime.Format("%d-%02d-%02d %02d.%02d.%02d.%03d", tNow.wYear, tNow.wMonth, tNow.wDay, tNow.wHour, tNow.wMinute, tNow.wSecond, tNow.wMilliseconds);
				//�ɵ����·��
				m_sOldDir = m_sDir + m_sPath;
				//�µ����·��
				m_sDir += m_sPath;
				if (m_sDir.Find("Collection") == -1)
				{
					m_sDir += strTime;
					m_sDir = "Collection\\" + m_sDir;
				}
			}
			else
			{
				m_sPath = m_sDir + m_sPath;
			}
#ifdef NDEBUG
		}
#endif
	}
	
	//���õ�ǰĿ¼
	SetCurrentDirectory(lpszCurDir);
	UpdateData(FALSE);
}

void CDlgAddOsc::OnBtnAddoscAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//�ж��Ƿ��Ѵ���
	/*
	if(IsExist(m_sPath))
	{
		//�Ѵ���
		AfxMessageBox("ָ���ļ��Ѵ����ڹ��Ϲ鵵������������!");
		return;
	}
	*/
	//�����豸
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_SEC);
	if(pCmb == NULL)
		return;
	int nIndex = pCmb->GetCurSel();
	CSecObj* pObj = (CSecObj*)pCmb->GetItemData(nIndex);
	if(pObj == NULL)
		return;
	if(m_pReport == NULL)
		return;
	//ʱ��
	CDateTimeCtrl* pTM = (CDateTimeCtrl*)GetDlgItem(IDC_DP_ADDOSC_TIME);
	if(pTM == NULL)
		return;
	CTime tm;
	pTM->GetTime(tm);
	//�½��¼�
	CFaultEvent* pEvent = new CFaultEvent;
	if (g_bSend20162)
	{
		pEvent->m_sEventDef = m_sOldDir;
	}
	else
	{
		CString sEventDef = m_sPath;
		sEventDef.Replace("/", "\\");
		pEvent->m_sEventDef = sEventDef;
	}
	pEvent->m_nMainRecID = m_pReport->m_nID;
	pEvent->m_tmStart = tm;
	pEvent->m_nmsStart = m_nMS;
	pEvent->m_sSecID = pObj->m_sID;
	pEvent->m_pSec = pObj;
	pEvent->m_nType = 4;
	pEvent->m_nmsSign = m_nMS;
	pEvent->m_nmsReceive = m_nMS;
	pEvent->m_tmSign = tm;
	pEvent->m_tmReceiveTime = tm;
	pEvent->m_sDir = m_sDir;
	pEvent->m_sOldDir = m_sOldDir;
	pEvent->m_sEventContent.Format("%d", m_nFileSize);
	//���뵽�б�
	m_listOSC.Add(pEvent);
	AddEventToList(pEvent, 0);
}

/*************************************************************
 CreateDate: 12:1:2010     Author:LYH
 �� �� ��: IsExist()
 ���ܸ�Ҫ: �ж�ָ���ļ��Ƿ����ڴ������б����Ϲ鵵��¼���б���
 �� �� ֵ: �Ѵ��ڷ���TRUE,�����ڷ���FALSE
 ��    ��: param1 ָ���ļ������ƣ�������׺
		   Param2 
**************************************************************/
BOOL CDlgAddOsc::IsExist( CString sName )
{
	//�ж��ļ��Ƿ��Ѽ��뵽�������б�
	for(int i = 0; i < m_listOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		if(pEvent->m_sEventDef == sName)
		{
			//�Ѵ���
			return TRUE;
		}
	}
	if(m_pReport == NULL)
		return FALSE;
	//�ж��Ƿ��ڹ��Ϲ鵵�д���
	//PTOSC
	for(i = 0; i < m_pReport->m_listPTOSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pReport->m_listPTOSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		CString str = pEvent->m_sEventDef;
		if(str.Find(sName, 0) >= 0)
		{
			//�Ѵ���
			return TRUE;
		}
	}
	//WROSC
	for(i = 0; i < m_pReport->m_listWROSC.GetSize(); i++)
	{
		CFaultEvent* pEvent = (CFaultEvent*)m_pReport->m_listWROSC.GetAt(i);
		if(pEvent == NULL)
			continue;
		CString str = pEvent->m_sEventDef;
		if(str.Find(sName, 0) >= 0)
		{
			//�Ѵ���
			return TRUE;
		}
	}
	return FALSE;
}

void CDlgAddOsc::OnBtnAddoscRemove() 
{
	// TODO: Add your control notification handler code here
	//�ҵ���LIST�е�ѡ��
	int nCount = m_List.GetSelectedCount();
	if(nCount != 1)
		return;
	int nIndex = -1;
	nIndex = m_List.GetNextItem(nIndex, LVNI_SELECTED);
	if(nIndex == -1)
		return;
	CFaultEvent* pEvent = (CFaultEvent*)m_List.GetItemData(nIndex);
	//��LIST��ɾ��
	m_List.DeleteItem(nIndex);
	if(pEvent == NULL)
		return;
	//��������ɾ��
	for(int i = 0; i < m_listOSC.GetSize(); i++)
	{
		CFaultEvent* pObj = (CFaultEvent*)m_listOSC.GetAt(i);
		if(pObj == NULL)
			continue;
		//�Ƚ�
		if(pObj == pEvent)
		{
			m_listOSC.RemoveAt(i);
			delete pEvent;
			pEvent = NULL;
			return;
		}
	}
}

void CDlgAddOsc::OnBtnAddoscModify() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgAddOsc::OnItemchangedListAddosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if((pNMListView->uChanged   &   LVIF_STATE)&&
		(pNMListView->uNewState   &   LVIS_SELECTED)) 
	{
		//��ʵ���������ѡ����, ���������ITEMCHANGED��Ϣ, 
		//1��A�����ֱ�����ʧ������2��Aȡ��ѡ�д�����3��B��ѡ�д���
		//Ϊ����ˢ��̫Ƶ��, ����ֻ����ѡ����Ϣ, ���������©��ѡ���˿��е���Ϣ
		//���԰�ѡ����з���NM_CLICK�д���, �����ƶ��������¼����µ�ѡ�񵽿��в�������
		
		int nSelected = pNMListView->iItem;//���������ǿհ��������ֵӦ����-1��
		if(nSelected < 0)
			return;
		
		//�õ����ж�Ӧ���¼�
		CFaultEvent * pEvent = (CFaultEvent*)m_List.GetItemData(nSelected);
		if(pEvent == NULL)
			return;
		//����ؼ�ֵ
		//�����豸
		if(pEvent->m_pSec != NULL)
		{
			CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_SEC);
			if(pCmb != NULL)
			{
				pCmb->SelectString(-1, pEvent->m_pSec->m_sName);
			}
			//��վ
			if(pEvent->m_pSec->m_pStation != NULL)
			{
				pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_STATION);
				if(pCmb != NULL)
				{
					pCmb->SelectString(-1, pEvent->m_pSec->m_pStation->m_sName);
				}
			}
		}
		//ʱ��
		CDateTimeCtrl* pTM = (CDateTimeCtrl*)GetDlgItem(IDC_DP_ADDOSC_TIME);
		if(pTM != NULL)
		{
			pTM->SetTime(&pEvent->m_tmStart);
		}
		//����
		m_nMS = pEvent->m_nmsStart;
		//·��
		m_sPath = pEvent->m_sEventDef;
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CDlgAddOsc::OnOK() 
{
	// TODO: Add extra validation here
	try
	{
		//���������Ч��
		CDlgFaultReport* pParent = (CDlgFaultReport*)GetParent();
		if(pParent != NULL)
		{
			//���
			for(int i = 0; i < m_listOSC.GetSize(); i++)
			{
				CFaultEvent* pEvent = (CFaultEvent*)m_listOSC.GetAt(i);
				if(pEvent == NULL)
					continue;
				CFaultEvent* pNew = pEvent->Clone();
				if(pNew->m_pSec == NULL)
					continue;
				if(pNew->m_pSec->m_nType == TYPE_PROTECT)
				{
					//����
					pParent->m_listPTOSC.Add(pNew);
				}
				else if(pNew->m_pSec->m_nType == TYPE_WAVEREC)
				{
					//¼����
					pParent->m_listWROSC.Add(pNew);
				}
				pParent->m_bEventNeedSave = TRUE;
			}
		}
		CDialog::OnOK();
	}
	catch(...)
	{
		WriteLog("CDlgAddOsc::OnOK(), ���¼���ļ�����");
		return;
	}
}

void CDlgAddOsc::OnSelchangeCmbAddoscStation() 
{
	// TODO: Add your control notification handler code here
	//ȡ�ó�վ
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_CMB_ADDOSC_STATION);
	CStationObj* pStation = NULL;
	if(pCmb != NULL)
	{
		int nIndex = pCmb->GetCurSel();
		pStation = (CStationObj*)pCmb->GetItemData(nIndex);
	}
	
	if ( pStation == NULL || (DWORD)pStation == -1 )
	{
		AfxMessageBox(StringFromID(IDS_COMMON_NEEDSTATION));
		return;
	}
	
	//�����豸
	FillCMBSec(pStation);
}

void CDlgAddOsc::OnColumnclickListAddosc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//�������������
	const int iCol = pNMListView->iSubItem;
	
	if ( iCol == m_nOldSortCol )
	{
		//�ߵ�˳������
		g_iStationInfoViewAsc = - g_iStationInfoViewAsc;
	}
	else
	{
		g_iStationInfoViewAsc = 1;
		m_nOldSortCol = iCol;
	}
	
	ListView_SortItems(m_List.GetSafeHwnd(), CompareFunction, iCol);
	*pResult = 0;
}
/*************************************************************
 �� �� ��: CompareFunction()
 ���ܸ�Ҫ: �ȽϺ���,����ʱ������
 �� �� ֵ: �ȽϽ��.������ʾ1��2С. 0��ʾ���. ������ʾ1��2��
 ��    ��: param1 �Ƚ϶���1
		   Param2 �Ƚ϶���2
		   Param3 �Ƚϲ���
**************************************************************/
int CALLBACK CDlgAddOsc::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	//ȡ�ñȽ�����
	CFaultEvent* pEvent1 = (CFaultEvent*)lParam1;
	CFaultEvent* pEvent2 = (CFaultEvent*)lParam2;
	if(pEvent1 == NULL || pEvent2 == NULL)
		return 0;
	int nCol = (int)lParamData;
	
	CString str1, str2;
	int iResult = 0;
	//"�����豸","����ʱ��","�ļ�����"
	switch(nCol)
	{
	case 0:	//�����豸

		if(pEvent1->m_pSec != NULL)
			str1.Format("%s", pEvent1->m_pSec->m_sName);
		if(pEvent2->m_pSec != NULL)
			str2.Format("%s", pEvent2->m_pSec->m_sName);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;

	case 1: //����ʱ��
		str1.Format("%s.%03d", pEvent1->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent1->m_nmsStart);
		str2.Format("%s.%03d", pEvent2->m_tmStart.Format("%Y-%m-%d %H:%M:%S"), pEvent2->m_nmsStart);
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);

		break;
	case 2: //�¼�����
		str1.Format("%s", pEvent1->m_sEventDef);
		str2.Format("%s", pEvent2->m_sEventDef);
// 		if(pEvent1->m_nType == 4 && pEvent2->m_nType == 4)
// 		{
// 			str1 = pEvent1->m_sEventDef;
// 			str2 = pEvent2->m_sEventDef;
// 		}
// 		else
// 		{
// 			str1 = pEvent1->m_sEventDefName;
// 			str2 = pEvent2->m_sEventDefName;
// 		}
		iResult = lstrcmpi( str1.GetBuffer(0), str2.GetBuffer(0));
		str1.ReleaseBuffer(0);
		str2.ReleaseBuffer(0);
		break;

	default:
		iResult = 0;
		break;
	}

	iResult *= g_iStationInfoViewAsc;
	return iResult;
}
