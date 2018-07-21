// DlgSettingDetail.cpp : implementation file
//

#include "stdafx.h"
#include "XJBrowser.h"
#include "DlgSettingDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSettingDetail dialog


CDlgSettingDetail::CDlgSettingDetail(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettingDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSettingDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pts = NULL;
	m_arrName.RemoveAll();
	m_arrValue.RemoveAll();
}


void CDlgSettingDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSettingDetail)
	DDX_Control(pDX, IDC_LIST_VALUE, m_ListCtrl);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSettingDetail, CDialog)
	//{{AFX_MSG_MAP(CDlgSettingDetail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSettingDetail message handlers

BOOL CDlgSettingDetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_pts == NULL)
		return FALSE;

	DWORD dwStyle;
	//StyleEx
	dwStyle = m_ListCtrl.GetExtendedStyle()|LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES; 
	m_ListCtrl.SetExtendedStyle(dwStyle);

	//��ʼ����ͷ
	InitColumns();

	//�������
	CString strTemp, strName, strGlobalName, strValue, strValue2;
	//int nCount;
	if (m_pts->DataType == 2 || m_pts->DataType == 4)//SETTING_TYPE_CONTROL
	{//���������͵Ķ�ֵ
		strName = m_pts->Name;
		strValue = m_pts->Value;
		/*if (m_pts->DataType == 2)
		{
			strValue = GetTranslatedValue(m_pts, m_pts->Value);
		}
		else if (m_pts->DataType == 4)
		{
			strValue = m_pts->Value;
		}*/

		//strName = "��Ƶ�仯���迹,16,,,C2,,,,,,,,,,,,";
		//strValue = "ABCD";
		if (m_pts->DataType == 2)
			strValue = HexToBin(strValue) ;

		CString strLog;
		strLog.Format("��������ֵ,name=%s, value=%s, ������value=%s", strName, m_pts->Value, strValue);
		WriteLogEx(strLog);

		CString sTitle = TranslateName(strName);
		//��ʾ������
		CWnd *pWnd = GetDlgItem(IDC_EDIT_NAME);
		pWnd->SetWindowText(sTitle);
		TranslateValue(strValue);
		for(int i = 0; i < m_arrName.GetSize(); i++)
		{
			CString str;
			str.Format("%d", i+1);
			m_ListCtrl.InsertItem( LVIF_TEXT|LVIF_PARAM, i, str, 0, 0, 3, i);
			m_ListCtrl.SetItemText(i, 1, m_arrName.GetAt(i));
			if(m_arrValue.GetSize() >= i+1)
				m_ListCtrl.SetItemText(i, 2, m_arrValue.GetAt(m_arrValue.GetUpperBound()-i));
// 			strLog.Format("����ֵ�б�,��%d��,name=%s, value=%s, ������value=%s", i+1, m_arrName.GetAt(i), m_arrValue.GetAt(m_arrValue.GetUpperBound()-i));
// 			WriteLogEx(strLog);
		}

		//����������
		/*int z = strName.Find(",", 0);
		if (z > 0)
		{
			try
			{
				//ȡ��������
				strGlobalName = strName.Left(z);
				strName.Delete(0, z+1);

				//��ʾ������
				CWnd *pWnd = GetDlgItem(IDC_EDIT_NAME);
				pWnd->SetWindowText(strGlobalName);

				//ȡ��������
				z = strName.Find(",", 0);
				strTemp = strName.Left(z);
				strName.Delete(0, z+1);
				nCount = atoi(strTemp);
				char stValue[64];
				strncpy(stValue, strValue, nCount);
				for (int i = 0; i < nCount; i ++)
				{
					z = strName.Find(",", 0);
					if (i != nCount - 1)
					{
						strTemp = strName.Left(z);
						strName.Delete(0, z+1);					
					}
					else
						strTemp = strName;

					m_ListCtrl.InsertItem( LVIF_TEXT|LVIF_PARAM, i, strTemp, 0, 0, 3, i);
					strValue2 = stValue[i];
					if (strValue2.IsEmpty() || strValue2 == "")
						strValue2 = "0";
					m_ListCtrl.SetItemText(i, 1, strValue2);
				}
			}
			catch(...)
			{
				AfxMessageBox("����������ʱ��������!�����ǲ��淶��������ʽ���!");
				WriteLog("����������ʱ��������!�����ǲ��淶��������ʽ���!");
			}
		}*/
	}
	/*else
	{//��������
		m_ListCtrl.InsertItem( LVIF_TEXT|LVIF_PARAM, 0, m_pts->Name, 0, 0, 3, 0);
		strTemp = GetTranslatedValue(m_pts, m_pts->Value);
		m_ListCtrl.SetItemText(0, 1, strTemp);
	}*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSettingDetail::InitColumns()
{
	LV_COLUMN lvCol;
	lvCol.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt=LVCFMT_LEFT;
	for (int nCol=0; nCol < 3 ; nCol++)
	{
		lvCol.iSubItem=nCol;
		CString colName = "";
		switch(nCol)
		{
		case 0:
			lvCol.cx = 40;
			colName = StringFromID(IDS_COMMON_NO);
			break;
		case 1://��ֵ����
			lvCol.cx = 190; //�п�254����
			colName = StringFromID(IDS_SETTING_DESC);
			break;
		case 2://ֵ
			lvCol.cx = 80; //�п�80����
			colName = StringFromID(IDS_COMMON_VALUE);
			break;
		default:
			lvCol.cx = 100;//�п�100����
		}
		lvCol.pszText=colName.GetBuffer(1);
		m_ListCtrl.InsertColumn(nCol,&lvCol);
	}
}

CString CDlgSettingDetail::GetTranslatedValue(PT_SETTING *pst, const CString strValue)
{
	//����VPickList���Value����,VPickList����ʱ��Valueֵ�Ľ��͸���VPivkListȷ�� 
	//��VPickList="0:on;1:off" �� Value=1 ʱ��strValueΪ"off"
	//��VPickListΪ�գ���Value=1��Unit="s" ��strValue = "1s"  
	CString strTemp;
	CString strUnit		= pst->Unit;
	CString strVPickList= pst->VPickList;
 	CString strReturn	= strValue;//pst->Value;
    CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	CString  strLog;
	//ȥ������Ŀո�
	strUnit.TrimLeft();
	strUnit.TrimRight();
	strReturn.TrimLeft();
	strReturn.TrimRight();
	strVPickList.TrimLeft();
	strVPickList.TrimRight();
	if (pst->DataType != 2)
	{//��SETTING_TYPE_CONTROL
		if (strVPickList.IsEmpty())
		{
			//�ӵ�λ
			strReturn = strReturn + strUnit;
		}
		else
		{
			int nFenhao =0;	//�ֺŵ�λ��
			int nMaohao =0; //ð�ŵ�λ��
			BOOL bFound = FALSE;

			//ֻҪstrVPickList���գ��������ֺ�֮����ַ������������ð����ߵ�ֵ�Ƿ���strReturn��ͬ
			//�����ͬ��stValue��ð���ұߵ��ִ�ȡ��
			while (!strVPickList.IsEmpty()) 	 
			{
				nFenhao = strVPickList.Find(';', 0);
				if(nFenhao>0)
					strTemp = strVPickList.Left(nFenhao);
				else
					strTemp = strVPickList;

				nMaohao = strTemp.Find(':', 0);			 
				if (nMaohao>0)
				{
					if (strTemp.Left(nMaohao) == strReturn)
					{
						strTemp.Delete(0, nMaohao+1);
						strReturn = strTemp;
						bFound = TRUE;
						break;
					}
				}

				if (nFenhao>0)
					strVPickList.Delete(0, nFenhao+1);
				else
					break;
			}

			if (bFound == FALSE)
			{//û�ҵ�
				//�ӵ�λ
				strReturn = strReturn + strUnit;
			}
		}
	}
	else
	{

		//SETTING_TYPE_CONTROL
		/*
		if(pApp->m_bDebugMode)
		{
			strLog.Format("�ź�����ֵ%s",strReturn);
			WriteLog(strLog);
		}
		*/
//      �ַ���ת
		strReturn = HexToBin(strReturn);

	}
 
	return strReturn;
}

CString CDlgSettingDetail::HexToBin(CString strValue) 
	{
		long l;
		CString strReturnTemp;
		CString strHex;
		CString strReturn = strValue;
		CString strTemp = "";
		int nTemp;
		for(int i = 0; i< strReturn.GetLength(); i++)
		{
			strHex = strReturn[i];
			l = strtol(strHex.GetBuffer(1), NULL, 16);
			_ltoa(l, strTemp.GetBuffer(MAX_PATH), 2);
			strTemp.ReleaseBuffer();
			//����
			nTemp = strTemp.GetLength();
			if(nTemp == 3)
			{
				strTemp = "0" + strTemp;
			}
			else if(nTemp == 2)
			{
				strTemp = "00" + strTemp;
			}
			else if(nTemp == 1)
			{
				strTemp = "000" + strTemp;
			}
			//ƴ��
			if(strReturnTemp.IsEmpty())
			{
				strReturnTemp = strTemp;
			}
			else
			{
				strReturnTemp += strTemp;
			}
		}
		/*
		if(pApp->m_bDebugMode)
		{
		strLog.Format("ת�ɶ����ƺ��ֵ%s",strReturnTemp);
		WriteLog(strLog);
		}
		*/
		strReturn = strReturnTemp;

		return strReturn;
	}

CString CDlgSettingDetail::TranslateName( CString sName )
{
	if(sName.IsEmpty())
		return "";
	CString strName = sName;
	int nFind = strName.Find(",", 0);
	if(nFind == -1)
		return "";
	CString strReturn = strName.Left(nFind);
	strName.Delete(0, nFind+1);
	//��������
	nFind = strName.Find(",", 0);
	if(nFind == -1)
		return strReturn;
	int nCount = atoi(strName.Left(nFind));
	strName.Delete(0, nFind+1);
	for(int i = 0; i < nCount; i++)
	{
		nFind = strName.Find(",", 0);
		if(nFind == -1)
		{
			if(i != nCount-1)
				return strReturn;
			else
				m_arrName.Add(strName);
		}
		else
		{
			m_arrName.Add(strName.Left(nFind));
			strName.Delete(0, nFind+1);
		}
	}
	return strReturn;
}

BOOL CDlgSettingDetail::TranslateValue( CString sValue)
{
	if(sValue.IsEmpty())
		return FALSE;

	CString strValue = sValue;

	for(int i = 0; i < sValue.GetLength(); i++)
	{
		m_arrValue.Add(strValue.Left(1));
		strValue.Delete(0, 1);
	}
	return TRUE;
}
