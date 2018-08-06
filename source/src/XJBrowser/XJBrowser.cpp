// XJBrowser.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "XJBrowser.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "XJBrowserDoc.h"

#include "ViewAction.h"	//�����¼���ͼ
#include "ViewAlarm.h"	//�澯�¼���ͼ
#include "ViewDI.h"		//�������¼���ͼ
#include "ViewFault.h"	//�¹ʱ�����ͼ
#include "ViewOSC.h"	//¼����ͼ


#include "SysAlarmFrame.h"	//ϵͳ�澯�¼����
#include "ViewSysAlarmDetail.h"	//ϵͳ�澯�¼���ϸ��ͼ
#include "ViewSysAlarm.h"	//ϵͳ�澯�¼���ͼ

#include "CommFrame.h"	//����վͨ��״̬�ӿ��
#include "ViewCommMap.h" //����վͨѶ״̬ͼ����ͼ

#include "ActionFrame.h" //�����¼��ӿ��
#include "AlarmFrame.h" //�澯�¼��ӿ��

#include "FaultFrame.h"	 //�¹ʱ����ӿ��
#include "ViewFaultSheet.h" //�¹ʱ���SHEET��ͼ

#include "PTFrame.h"	//�������Դ���
#include "PTSheet.h"	//����ҳ��ͼ
#include "WaveRecFrame.h"//¼�������Դ���

#include "DlgValidateID.h" //�����֤�Ի���

#include "BatchSelectView.h" //�����ٻ�ѡ����ͼ
#include "BatchFrame.h" //�����ٻ����
#include "AutoCallFrame.h"//�Զ����ٿ��
#include "AutoCallResult.h"//�Զ�������ͼ
#include "AutoResultFrame.h"//�Զ����ٽ�����
#include "ACResultSel.h" //�Զ����ٽ����վѡ����ͼ

#include "AboutDlgEx.h"
#include "SysSettingSheet.h"	//ϵͳ���ô���

#include "DownloadFrame.h" //ͨ���ļ����ؿ��
#include "DownLoadView.h"  //ͨ���ļ�������ͼ

#include "CrossFrame.h" //Ԥ�����ڿ��
#include "ViewCross.h" //Ԥ���б���ͼ

#include "SoeFrame.h" //SOE�¼����ڿ��
#include "ViewSOE.h"  //SOE�¼��б���ͼ

#include "DraftFrame.h"
#include "DraftView.h"
#include "DraftDocument.h"
#include "DraftList.h"

#include "ViewDistance.h"

#include "StationInfoFrame.h"
#include <winnetwk.h>

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include "GlobalFunc.h"
#include "CustomStyleSetting.h"
#include "MtVerify.h"
#include "PushMsgProcess.h"

#include "XJRootStore.h"
#include "XJUserStore.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif*/


CPushMsgProcess	g_PushMsgProcess;

#define CLEARCONFIG_SPACE	600000	//��������豸�ֳ����еļ��ʱ��,10����

/*************************************************************
 �� �� ClearSecConfig()
 ���ܸ�Ҫ����������豸�����߳�(��������ֵ����, �����źź�����, �澯�źź�����)
 �� �� ֵ: UINT
 ��    ����param1	
**************************************************************/
UINT ClearSecConfig(LPVOID pParam)
{
	WriteLog("������������豸�����̳߳ɹ�!", XJ_LOG_LV3);
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	while(1)
	{
		if(g_bSuspend)
		{
			Sleep(10);
			continue;
		}
		if(pApp->m_bExitClearConfig)
			break;
		int ntemptime = 0;
		while( ntemptime < CLEARCONFIG_SPACE )
		{
			Sleep(10);
			ntemptime+=10;
			if(pApp->m_bExitClearConfig)
				break;
		}
		if(g_bSuspend)
			continue;
		if(pApp->m_bExitClearConfig)
			break;
		CDataEngine * pData = pApp->GetDataEngine();
		if(pData == NULL)
		{
			continue;
		}
		DEVICE_LIST listDevice;
		listDevice.RemoveAll();
		pData->GetSecList(listDevice, "", TYPE_SEC);
		POSITION pos;
		pos = listDevice.GetHeadPosition();

		WriteLog("��ʼ��������豸����!", XJ_LOG_LV3);
		int ncount = 0;
		int nclearcount = 0;
		while(pos != NULL)
		{
			if(pApp->m_bExitClearConfig || g_bSuspend)
			{
				break;
			}

			CSecObj * pObj = (CSecObj*)listDevice.GetNext(pos);
			if(pObj == NULL)
				continue;
			if(pObj->CanClear())
			{
				pObj->ClearConfig();
				nclearcount++;
				CString str;
				str.Format("�����豸���������̣߳��ɹ�����[%s]������", pObj->m_sID);
				WriteLog(str, XJ_LOG_LV3);
			}

			ncount++;
			if( ncount%50 == 0 )
				Sleep(2);
		}
		CString str;
		str.Format("�����豸���������̣߳��ɹ�����%d��װ�õ�����", nclearcount);
		WriteLog(str, XJ_LOG_LV3);

	}
	WriteLog("�˳���������豸�����̳߳ɹ�!", XJ_LOG_LV3);
	return 0;
}

/*************************************************************
 �� �� ��: PostMessageThread()
 ���ܸ�Ҫ: �Ӷ�����ȡ������ת���������, �������ת�������ӿ��
 �� �� ֵ: UINT
 ��    ��: param1 ������ָ��
		   Param2 
**************************************************************/
UINT PostMessageThread(LPVOID pParam)
{
	WriteLog("�����ַ������̳߳ɹ�!", XJ_LOG_LV3);
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//ѭ���ַ�����
	while(1)
	{
		//WriteLog("PostMessageThread");
		if(pApp->m_bExitReceiveMsg && pApp->m_listFullData.GetCount() == 0)
		{
			//�˳���־ΪTRUE
			return 0;
		}

		while(g_bSuspend)
		{
			//��ͣ�ַ��߳�
			Sleep(10);
		}

		//����
		if(pApp->m_listFullData.GetCount() == 0)
			Sleep(2000);
		else
			Sleep(g_PostTime);

		//ȡ�ö��еĵ�һ������
		STTP_FULL_DATA* sttpData = pApp->GetFirstSTTPData();
		if(sttpData == NULL)
			continue;

		if(pApp->m_bExitReceiveMsg )
		{
			//�˳���־ΪTRUE
			return 0;
		}
		
		//��ȡ���ĳɹ�
		try
		{
			int nMsgID = sttpData->sttp_head.uMsgID;
			if(nMsgID > 20000 && nMsgID < 30000)
			{
				//���ĺϷ�,���㱨�Ķ�Ӧ����Ϣ����ֵ
				int nMessage = (nMsgID - 20000) + 2000 + WM_USER;
				int nNotify = nMsgID - 20000 + 2000;
				CString sNotify;
				sNotify.Format("(WM_USER + %d)", nNotify);
				
				//�õ������ָ��
				CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
				if(pFrame != NULL)
				{
					CString str;
					str.Format("�յ�%d����, ����%s��Ϣ�������", nMsgID, sNotify);
					WriteLog(str, XJ_LOG_LV3);

					if(pApp->m_bExitReceiveMsg )
					{
						//�˳���־ΪTRUE
						return 0;
					}

					pFrame->SendMessage(nMessage, 0, (LPARAM)sttpData);
				}
			}
			else if(901 == nMsgID)
			{
				//�õ������ָ��
				CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
				if(pFrame != NULL)
				{
					CString str;
					str.Format("�յ�00901����, ����WM_STTP_00901��Ϣ�������");
					WriteLog(str, XJ_LOG_LV3);
					if(pApp->m_bExitReceiveMsg )
					{
						//�˳���־ΪTRUE
						return 0;
					}
					pFrame->SendMessage(WM_STTP_00901, 0, (LPARAM)sttpData);
				}
			}
			else if(204 == nMsgID)
			{
				//�õ������ָ��
				CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
				if(pFrame != NULL)
				{
					CString str;
					str.Format("�յ�00204����, ����WM_STTP_00204��Ϣ�������");
					WriteLog(str, XJ_LOG_LV3);
					if(pApp->m_bExitReceiveMsg )
					{
						//�˳���־ΪTRUE
						return 0;
					}
					pFrame->SendMessage(WM_STTP_00204, 0, (LPARAM)sttpData);
				}
			}
			else if(212 == nMsgID)
			{
				//�õ������ָ��
				CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
				if(pFrame != NULL)
				{
					CString str;
					str.Format("�յ�00212����, ����WM_STTP_00212��Ϣ�������");
					WriteLog(str, XJ_LOG_LV3);
					if(pApp->m_bExitReceiveMsg )
					{
						//�˳���־ΪTRUE
						return 0;
					}
					pFrame->SendMessage(WM_STTP_00212, 0, (LPARAM)sttpData);
				}
			}
			else
			{
				CString str;
				str.Format("�յ�δ֪����, ����IDΪ%d", nMsgID);
				WriteLog(str, XJ_LOG_LV3);
			}
		}
		catch (...)
		{
			WriteLog("PostMessageThread, �������쳣", XJ_LOG_LV3);
		}
		
		//zero_sttp_full_data(*sttpData);
		//ɾ������
		pApp->RemoveSTTPData();

		//WaitForSingleObject(theApp.m_hPostEvent, INFINITE);
		//----------------------****************************--------------
		//WriteLog("PostMessageThread.............");
	}
	return 0;
}

/*************************************************************
 �� �� ����ReceiveMessageThread()
 ���ܸ�Ҫ������STTP�����߳�, ���浽����
 �� �� ֵ: UINT
 ��    ����param1	������ָ��
**************************************************************/
UINT ReceiveMessageThread(LPVOID pParam)
{
	WriteLog("������ȡ�����̳߳ɹ�!", XJ_LOG_LV3);
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//ѭ����ȡ����
	while(1)
	{
		//WriteLog("ReceiveMessageThread");
		if(pApp->m_bExitReceiveMsg)
		{
			//�˳���־ΪTRUE
			return 0;
		}
		//�����ű��ĵ�ַ
		STTP_FULL_DATA*  sttpData = new STTP_FULL_DATA();
		if(sttpData == NULL)
			continue;
		zero_sttp_full_data(*sttpData);
		//��ȡ����
		int nResult = 0;
		try
		{
			if( pApp->GetSTTPStatus() )
			{
				nResult = pApp->m_SttpEngine.XJSTTPRead(pApp->m_SttpHandle, *sttpData);
			}		
		}
		catch (...)
		{
			WriteLog("ReceiveMessageThread ���Ķ�ȡʧ��", XJ_LOG_LV3);
		}
		
		if(1 == nResult)
		{
			if(pApp->m_bExitReceiveMsg)
			{
				//�˳���־ΪTRUE
				if(sttpData != NULL)
					delete sttpData;
				return 0;
			}

			//��ȡ���ĳɹ�
			try
			{
				//���浽����
				if(g_PushMsgProcess.IsAutoPushMsg(sttpData->sttp_head.uMsgID))
				{
					g_PushMsgProcess.AddSttpData(sttpData);
					CString str;
					str.Format("ReceiveMessageThread, ���յ��Զ����ͱ���%d,���������ͱ��Ĵ�������", sttpData->sttp_head.uMsgID);
					WriteLog(str, XJ_LOG_LV3);
				}
				else
				{
					if(g_PushMsgProcess.IsAutoNotifyMsg(sttpData->sttp_head.uMsgID))
					{
						CString str;
						str.Format("ReceiveMessageThread, ���յ�����Ҫ����%d,������", sttpData->sttp_head.uMsgID);
						WriteLog(str, XJ_LOG_LV3);
						if(sttpData)
						{
							zero_sttp_full_data(*sttpData);
							delete sttpData;
							sttpData = NULL;
						}
					}
					else
					{
						pApp->AddSTTPData(sttpData);
						CString str;
						str.Format("ReceiveMessageThread, ���յ���Ӧ����%d,����������", sttpData->sttp_head.uMsgID);
						WriteLog(str, XJ_LOG_LV3);
					}
				}
				Sleep(2);
				//WaitForSingleObject(theApp.m_hReceiveEvent, INFINITE);
				continue;
			}
			catch (...)
			{
				WriteLog("ReceiveMessageThread, ���汨������������ʧ��", XJ_LOG_LV3);
			}	
		}
		else if(nResult == -1)
		{
			//�����ж�
			WriteLog("ReceiveMessageThread, ���ձ�������ʧ��,ԭ��Ϊ�����ж�", XJ_LOG_LV3);
			Sleep(500);
		}
		else if(0 == nResult)
		{
			//WriteLog("ReceiveMessageThread, ���յ���������ʧ��,ԭ��Ϊ��ǰ������", XJ_LOG_LV3);
			Sleep(10);
		}
		else
		{
			WriteLog("ReceiveMessageThread, ���ձ�������ʧ��,ԭ��δ֪", XJ_LOG_LV3);
			Sleep(500);
		}
		
		if(sttpData != NULL) //�쳣��������ʱ
		{
			zero_sttp_full_data(*sttpData);
			delete sttpData;
			sttpData = NULL;
		}

		//WaitForSingleObject(theApp.m_hReceiveEvent, INFINITE);
		//-------*********************---------------------
		//WriteLog("ReceiveMessageThread..............");
	}

	return 0;
}

UINT PushMsgThread(LPVOID pParam)
{
	WriteLog("�����Զ����ͱ����̳߳ɹ�!", XJ_LOG_LV3);
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();

	//ѭ���ַ�����
	while(1)
	{
		if(pApp->m_bExitReceiveMsg)
		{
			//�˳���־ΪTRUE
			return 0;
		}

		while(g_bSuspend)
		{
			//��ͣ�ַ��߳�
			Sleep(10);
		}

		//����
		Sleep(g_PostTime);

		if(pApp->m_bExitReceiveMsg )
		{
			//�˳���־ΪTRUE
			return 0;
		}

		g_PushMsgProcess.TranslateSttpData();
	}
	return 0;
}
 
BOOL CheckSystemProcess()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
	DWORD dCurrentProcessId = GetCurrentProcessId();
	
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return FALSE;
	
    // Calculate how many process identifiers were returned.
	
    cProcesses = cbNeeded / sizeof(DWORD);
	
    // Print the name and process identifier for each process.
	
    for ( i = 0; i < cProcesses; i++ )
	{
        if( aProcesses[i] != 0 )
		{
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
			CString strModuleName = _T("");
			DWORD processID = aProcesses[i];
			// Get a handle to the process.
			
			HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, processID );
			if ( processID == dCurrentProcessId)
			{
				continue;
			}
			
			// Get the process name.
			
			if (NULL != hProcess )
			{
				HMODULE hMod;
				DWORD cbNeeded;
				
				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
					&cbNeeded) )
				{
					GetModuleBaseName( hProcess, hMod, szProcessName, 
						sizeof(szProcessName)/sizeof(TCHAR) );
					strModuleName = szProcessName;
					if (strModuleName.Find("XJBrowser") != -1)
					{ 
						if ( AfxMessageBox(StringFromID(IDS_TIP_APP_EXIST), MB_YESNO) == IDYES)
						{
							if ( !::TerminateProcess(hProcess, 0))
							{
								return FALSE;
							}
						}
						else
						{
							return FALSE;
						}
					}
				}
			}	
		}
	}     

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CXJBrowserApp object

CXJBrowserApp theApp;

/*************************************************************
 �� �� ����ComplexExchange()
 ���ܸ�Ҫ������ת��, �жϴ�����ַ����Ƿ��ʾ����,�����,ת���ɸ�����ʽ��ʾ
 �� �� ֵ: void
 ��    ����param1	��ת�����ַ���	
		   Param2
**************************************************************/
CString	ComplexExchange(CString sStr)
{
	CString sReturn = sStr;
	//�������ַ�����ʽΪ: ʵ��&&&�鲿
	int nPos = sStr.Find("&&", 0);
	if(nPos == -1)
	{
		//�Ǹ���
		return sReturn;
	}
	//����
	CString strR = sStr.Left(nPos); //ʵ��
	CString strC = sStr;
	strC.Delete(0, nPos + 2); //�鲿
	sReturn.Format("%s+j%s", strR, strC);
	return sReturn;
}

/*************************************************************
 �� �� ����CompareComplex()
 ���ܸ�Ҫ���Ƚ����������Ĵ�С
 �� �� ֵ: ���1��2��, ����1. 1=2,����0. 1<2,����-1
 ��    ����param1	����1
		   Param2	����2
**************************************************************/
int	CompareComplex(CString str1, CString str2)
{
	//�ж����ݺϷ�,������Ȼ��: ʵ��+j�鲿 ��ʽ
	int nPos1 = str1.Find('+', 0);
	int nPos2 = str2.Find('+', 0);

	if(nPos1 == -1 || nPos2 == -1)
	{
		//���Ϸ�,�˳�
		return 0;
	}
	
	CString strR1 = str1.Left(nPos1); //����1��ʵ��
	CString strC1 = str1;
	strC1.Delete(0, nPos1+1); //����1���鲿
	CString strR2 = str2.Left(nPos2); //����2��ʵ��
	CString strC2 = str2;
	strC2.Delete(0, nPos2+1); //����2���鲿
	//�ȱ�ʵ��
	int R1 = atoi(strR1);
	int R2 = atoi(strR2);
	if(R1 > R2)
	{
		return 1;
	}
	else if(R1 < R2)
	{
		return -1;
	}
	else
	{
		//ʵ�����,���鲿
		int C1 = atoi(strC1);
		int C2 = atoi(strC2);
		if(C1 > C2)
		{
			return 1;
		}
		else if(C1 < C2)
		{
			return -1;
		}
		else 
		{
			return 0;
		}
	}
	return 0;
}

/************************************
Date:2011/7/21  Author:LYH
������:    WriteLogTemp
����ֵ:   void
���ܸ�Ҫ: �����Ӱ�д��־��ֻд�ļ�
����: CString sLog
����: int nLV
************************************/
void WriteLogTemp(CString sLog, int nLV)
{
	//������־�ļ�·��
	CLogMgr logMgr;
	logMgr.SetLogPath(g_LogPath);
	logMgr.WriteLog(sLog, nLV);
}

/*************************************************************
 �� �� ����WriteLog()
 ���ܸ�Ҫ��д��־, ������������������־�ļ�(�������Ƿ������־�ļ�)
 �� �� ֵ: void
 ��    ����param1	��־�ı�����
		   Param2	��־�ȼ�, 1-��Ҫ. 2-һ��. 3-������. Ĭ��Ϊ2
**************************************************************/
void WriteLog(CString sLog, int nLV)
{
	theApp.WriteLog(sLog, nLV);
}

/*************************************************************
 �� �� ����WriteLog()
 ���ܸ�Ҫ��д��־, д����־�ļ�(�������Ƿ������־�ļ�)
 �� �� ֵ: void
 ��    ����param1	��־�ı�����
		   Param2	��־�ȼ�, 1-��Ҫ. 2-һ��. 3-������. Ĭ��Ϊ2
**************************************************************/
void WriteLogEx(CString sLog, int nLV)
{
	theApp.WriteLogEx(sLog, nLV);
}

/*************************************************************
 �� �� ����StringToTimeSttp15()
 ���ܸ�Ҫ����15λ��STTPʱ���ַ���ת��ΪCTime
 �� �� ֵ: ת���ɵ�CTime����
 ��    ����param1 ʱ���ַ���
		   Parsm2 ���غ���
**************************************************************/
CTime StringToTimeSttp15(CString & sTime, int& nms)
{
	//�ַ�����ʽΪYYMMDDhhmmss+3λ������,��080803095654123
	CString strTime = sTime;
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	if(strTime.GetLength() < 12 && strTime.GetLength() > 16)
		return tmTime;
	CString strTemp;

	int nt[6];

	for(int i = 0 ; i < 6; i++)
	{
		//ÿ�ζ�ȡǰ��λ,Ȼ��ɾ��ǰ��λ
		strTemp = strTime.Left(2);
		nt[i] = atoi(strTemp);
		strTime.Delete(0, 2);
	}

	//����
	nms = atoi(strTime);

	//�����2000,���20XX��
	if(nt[0] >= 0 && nt[0] <= 37)
		nt[0] += 2000;
	else
		nt[0] += 1900;

	//�ж����
	int nYear = nt[0];
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	//�ж��·�
	int nMonth = nt[1];
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	//�ж�����
	int nDay = nt[2];
	if(nDay < 1 || nDay > 31)
		return tmTime;
	//�ж�Сʱ
	int nHour = nt[3];
	if(nHour < 0 || nHour > 24)
		return tmTime;
	//�жϷ���
	int nMin = nt[4];
	if(nMin < 0 || nMin > 60)
		return tmTime;
	//�ж���
	int nSec = nt[5];
	if(nSec < 0 || nSec > 60)
		return tmTime;

	CTime tm(nt[0], nt[1], nt[2], nt[3], nt[4], nt[5]);
	return tm;
}

/*************************************************************
 �� �� ����StringToTimeSttp12()
 ���ܸ�Ҫ����12λ��STTPʱ���ַ���ת��ΪCTime,12λ��ʱ�䲻������ֵ
 �� �� ֵ: ת���ɵ�CTime����
 ��    ����param1	ʱ���ַ���
**************************************************************/
CTime StringToTimeSttp12(CString & sTime)
{
	//�ַ�����ʽΪYYMMDDhhmmss,��080803095654
	CString strTime = sTime;
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	if(strTime.GetLength() != 12)
		return tmTime;
	CString strTemp;
	
	int nt[6];
	
	for(int i = 0 ; i < 6; i++)
	{
		//ÿ�ζ�ȡǰ��λ,Ȼ��ɾ��ǰ��λ
		strTemp = strTime.Left(2);
		nt[i] = atoi(strTemp);
		strTime.Delete(0, 2);
	}
	
	//�����2000,���20XX��
	if(nt[0] >= 0 && nt[0] <= 37)
		nt[0] += 2000;
	else
		nt[0] += 1900;

	//�ж����
	int nYear = nt[0];
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	//�ж��·�
	int nMonth = nt[1];
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	int nDay = nt[2];
	if(nDay < 1 || nDay > 31)
		return tmTime;
	//�ж�Сʱ
	int nHour = nt[3];
	if(nHour < 0 || nHour > 24)
		return tmTime;
	//�жϷ���
	int nMin = nt[4];
	if(nMin < 0 || nMin > 60)
		return tmTime;
	//�ж���
	int nSec = nt[5];
	if(nSec < 0 || nSec > 60)
		return tmTime;
	
	CTime tm(nt[0], nt[1], nt[2], nt[3], nt[4], nt[5]);
	return tm;
}

/*************************************************************
 �� �� ����StringToTimeSttp26()
 ���ܸ�Ҫ����26λ��STTPʱ���ַ���ת��ΪCTime,����ֵռ6λ
 �� �� ֵ: CTime����ʱ��
 ��    ����param1	ʱ���ַ���
		   Param2	����ֵ
**************************************************************/
CTime StringToTimeSttp26(CString & sTime, int& nms)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	//�ַ�����ʽΪYYYY-MM-DD HH:MM:SS.MMMMMM ��:2008-05-30 13:44:51.120005
	CString strTime = sTime;
	if(strTime.GetLength() < 23 && strTime.GetLength() >27)
		return tmTime;
	CString strTemp;
	int nYear;
	int pos = -1;
	int pos2 = -1;
	
	//��
	pos = strTime.Find('-', 0);
	if(pos == -1)
	{
		//δ�ҵ�
		return tmTime;
	}
	
	strTemp = strTime.Left(pos);
	nYear = atoi(strTemp);
	//�ж����
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	
	//��
	int nMonth;
	pos2 = pos;
	pos = strTime.Find('-', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 +1, pos - pos2 -1);
	nMonth = atoi(strTemp);
	//�ж��·ݺϷ�
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	
	//��
	int nDay;
	pos2 = pos;
	pos = strTime.Find(' ', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nDay = atoi(strTemp);
	//�ж����ںϷ�
	if(nDay < 1 || nDay > 31)
		return tmTime;
	
	//ʱ
	int nHour;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nHour = atoi(strTemp);
	//�ж�Сʱ�Ϸ�
	if(nHour < 0 || nHour > 24)
		return tmTime;
	
	//��
	int nMin;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nMin = atoi(strTemp);
	//�жϷ��ӺϷ�
	if(nMin < 0 || nMin > 60)
		return tmTime;
	
	//��
	int nSec;
	pos2 = pos;
	pos = strTime.Find('.', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nSec = atoi(strTemp);
	//�ж����ӺϷ�
		if(nSec < 0 || nSec > 60)
		return tmTime;

	//����
	strTemp = strTime;
	strTemp.Delete(0, 20);
	if(strTemp.GetLength() > 3)
	{
		//ȥ������3λ
		strTemp.Delete(strTemp.GetLength() - 3, 3);
	}
	//ʣ��6λ����ֵ
	nms = atoi(strTemp);
	
	CTime tm(nYear, nMonth, nDay, nHour, nMin, nSec);
	return tm;
}

/*************************************************************
 �� �� ����StringToTime14()
 ���ܸ�Ҫ����14����ʱ���ַ���ת����ʱ�����
 �� �� ֵ: CTime����ʱ��
 ��    ����param1 ʱ���ַ���
		   Param2
**************************************************************/
CTime StringToTime14(CString sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 );
	//ʱ���ʽΪ:YYYYMMDDhhmmss, ��:20080808010100
	CString strTime = sTime;
	if(strTime.GetLength() != 14)
		return tmTime;
	CString strTemp;
	
	int nt[6];
	
	for(int i = 0 ; i < 6; i++)
	{
		//ÿ�ζ�ȡǰ��λ,Ȼ��ɾ��ǰ��λ
		int nLength = 2;
		if(i == 0)
			nLength = 4;
		strTemp = strTime.Left(nLength);
		nt[i] = atoi(strTemp);
		strTime.Delete(0, nLength);
	}

	//�ж����
	int nYear = nt[0];
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	//�ж��·�
	int nMonth = nt[1];
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	//�ж�����
	int nDay = nt[2];
	if(nDay < 1 || nDay > 31)
		return tmTime;
	//�ж�Сʱ
	int nHour = nt[3];
	if(nHour < 0 || nHour > 24)
		return tmTime;
	//�жϷ���
	int nMin = nt[4];
	if(nMin < 0 || nMin > 60)
		return tmTime;
	//�ж���
	int nSec = nt[5];
	if(nSec < 0 || nSec > 60)
		return tmTime;

	CTime tm(nt[0], nt[1], nt[2], nt[3], nt[4], nt[5]);
	return tm;
}

/*************************************************************
 �� �� ����XJToMS()
 ���ܸ�Ҫ��CXJString�����ַ���ת��ΪCString�ַ���
 �� �� ֵ: ת����CString����
 ��    ����param1	Ҫת����CXJString����
**************************************************************/
CString XJToMS(string xjStr)
{
	int nLength = xjStr.length();
	char* a = new char[nLength+2];
	memset(a, 0, nLength+2);
	memcpy(a, xjStr.c_str(), nLength);
	CString str = a;
	delete[] a;
	return str;
}

string WChar2Ansi(LPCWSTR pwszSrc) 
{ 
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL); 
	if (nLen <= 0) return std::string(""); 
	char* pszDst = new char[nLen]; 
	if (NULL == pszDst) return std::string(""); 
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL); 
	pszDst[nLen -1] = 0; 
	std::string strTemp(pszDst); 
	delete [] pszDst; 
	return strTemp; 
}	
/*************************************************************
 �� �� ����MSToXJ()
 ���ܸ�Ҫ��CString�����ַ���ת��ΪCXJString�ַ���
 �� �� ֵ: ת����CXJString����
 ��    ����param1	Ҫת����CString����
**************************************************************/
string MSToXJ(CString str)
{
	return (LPCSTR)str;
}

CTime StringToTimeWithMillisecond(CString sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 ); 
	//�ַ�����ʽΪYYYY-MM-DD HH:MM:SS.MMMM ��:2008-05-30 13:44:51.0005
	CString strTime = sTime;
	CString strTemp;
	int nYear;
	int pos = -1;
	int pos2 = -1;
	
	//��
	pos = strTime.Find('-', 0);
	if(pos == -1)
	{
		//δ�ҵ�
		return tmTime;
	}
	
	strTemp = strTime.Left(pos);
	nYear = atoi(strTemp);
	//�ж����
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	
	//��
	int nMonth;
	pos2 = pos;
	pos = strTime.Find('-', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 +1, pos - pos2 -1);
	nMonth = atoi(strTemp);
	//�ж��·ݺϷ�
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	
	//��
	int nDay;
	pos2 = pos;
	pos = strTime.Find(' ', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nDay = atoi(strTemp);
	//�ж����ںϷ�
	if(nDay < 1 || nDay > 31)
		return tmTime;
	
	//ʱ
	int nHour;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nHour = atoi(strTemp);
	//�ж�Сʱ�Ϸ�
	if(nHour < 0 || nHour > 24)
		return tmTime;
	
	//��
	int nMin;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nMin = atoi(strTemp);
	//�жϷ��ӺϷ�
	if(nMin < 0 || nMin > 60)
		return tmTime;
	
	//��
	int nSec;
	pos2 = pos;
	pos = strTime.Find('.', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nSec = atoi(strTemp);
	//�ж����ӺϷ�
	if(nSec < 0 || nSec > 60)
		return tmTime;
	
	CTime tm(nYear, nMonth, nDay, nHour, nMin, nSec);
	return tm;
}
CTime StringToTimeWithoutMillisecond(CString sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 ); 
	//19λ�ַ�����ʽΪYYYY-MM-DD HH:MM:SS ��:2008-05-30 13:44:51
	CString strTime = sTime;
	CString strTemp;
	int nYear;
	int pos = -1;
	int pos2 = -1;
	
	//��
	pos = strTime.Find('-', 0);
	if(pos == -1)
	{
		//δ�ҵ�
		return tmTime;
	}
	
	strTemp = strTime.Left(pos);
	nYear = atoi(strTemp);
	//�ж����
	if(nYear< 1971 || nYear >2037)
		return tmTime;
	
	//��
	int nMonth;
	pos2 = pos;
	pos = strTime.Find('-', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 +1, pos - pos2 -1);
	nMonth = atoi(strTemp);
	//�ж��·ݺϷ�
	if(nMonth < 1 || nMonth > 12)
		return tmTime;
	
	//��
	int nDay;
	pos2 = pos;
	pos = strTime.Find(' ', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nDay = atoi(strTemp);
	//�ж����ںϷ�
	if(nDay < 1 || nDay > 31)
		return tmTime;
	
	//ʱ
	int nHour;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nHour = atoi(strTemp);
	//�ж�Сʱ�Ϸ�
	if(nHour < 0 || nHour > 24)
		return tmTime;
	
	//��
	int nMin;
	pos2 = pos;
	pos = strTime.Find(':', pos2 + 1);
	if(pos == -1)
	{
		return tmTime;
	}
	strTemp = strTime.Mid(pos2 + 1, pos - pos2 -1);
	nMin = atoi(strTemp);
	//�жϷ��ӺϷ�
	if(nMin < 0 || nMin > 60)
		return tmTime;
	
	//��
	int nSec;
	strTemp = strTime;
	strTemp.Delete(0, pos+1);
	nSec = atoi(strTemp);
	//�ж����ӺϷ�
	if(nSec < 0 || nSec > 60)
		return tmTime;
	
	CTime tm(nYear, nMonth, nDay, nHour, nMin, nSec);
	return tm;
}

/*************************************************************
 �� �� ����StringToTime()
 ���ܸ�Ҫ�����ݿ��е�ʱ���ַ���ת��Ϊʱ��.�ַ�����ʽΪYYYY-MM-DD HH:MM:SS.MMMM
 �� �� ֵ: ת���ɵ�ʱ��
 ��    ����param1	ʱ���ַ���
**************************************************************/
CTime StringToTime(CString& sTime)
{
	CTime tmTime( 1971, 1, 1, 1, 1, 1 ); 
	//�ַ�����ʽΪYYYY-MM-DD HH:MM:SS.MMMM ��:2008-05-30 13:44:51.0005
	CString strTime = sTime;
	strTime.TrimLeft();
	strTime.TrimRight();
	int nLength = strTime.GetLength();
	if(nLength <= 19)
	{
		//19λ�ַ�����ʽΪYYYY-MM-DD HH:MM:SS
		return StringToTimeWithoutMillisecond(strTime);
	}
	else if(nLength > 19)
	{
		return StringToTimeWithMillisecond(strTime);
	}
	return tmTime;
}

/*************************************************************
�� �� ����IsValidKeyString()
���ܸ�Ҫ���ж�һ�����йؼ��ֵ��ַ����Ƿ���Ч�ַ���
�� �� ֵ: ��Ч����TRUE, ��Ч����FALSE
��    ����param1	�ַ���
		  Param2
**************************************************************/
BOOL IsValidKeyString(CString sKey)
{
	//�ж�������Ч��
	if(sKey == "")
		return FALSE;
	//�ж��Ƿ��йؼ���
	int nFind1 = sKey.Find('$', 0);
	int nFind2 = -1;
	//û�йؼ���, ����sKey
	if(nFind1 == -1)
		return FALSE;
	//���$�ĸ����Ƿ�˫��
	int nCount = 1;
	while(nFind1 != -1)
	{
		nFind1 = sKey.Find('$', nFind1 + 1);
		if(nFind1 == -1)
			break;
		nCount++;
	}
	if(nCount < 2)
		return FALSE;
	int Residue = nCount%2;
	if(Residue != 0)
		return FALSE;
	return TRUE;
}

/*************************************************************
�� �� ����FindOutAllKey()
���ܸ�Ҫ���ҵ���������еĹؼ���,���ѹؼ����滻��%s
�� �� ֵ: ��ʽ�������
��    ����param1	�ؼ����ַ���
	      Param2	�ؼ�������
**************************************************************/
CString FindOutAllKey( CString sKey, CStringArray& arrString )
{
	//�������
	arrString.RemoveAll();
	CString strReturn = "";
	int nFind1 = -1;
	int nFind2 = -1;
	CString sTempKey = sKey;
	nFind1 = sTempKey.Find('$', 0);
	while(nFind1 != -1)
	{
		//���ҵ�һ��$λ��
		nFind1 = sTempKey.Find('$', 0);
		if(nFind1 == -1)
			break;
		//������֮��Ӧ��$��λ��
		nFind2 = sTempKey.Find('$', nFind1 + 1);
		if(nFind2 == -1)
			break;
		//����ؼ��ֵ�����
		CString sTemp = sTempKey.Mid(nFind1, nFind2 - nFind1 + 1);
		arrString.Add(sTemp);
		//�ѹؼ����滻��%s
		sTempKey.Delete(nFind1, nFind2 - nFind1 + 1);
		sTempKey.Insert(nFind1, "%s");
	}
	strReturn = sTempKey;
	return strReturn;
}


/*************************************************************
 �� �� ����GetOscFilePath()
 ���ܸ�Ҫ��ȡ��¼���ļ�·��
 �� �� ֵ: void
 ��    ����param1	¼���ļ�ȫ·��
		   Param2
**************************************************************/
void GetOscFilePath(CString& sFullPath)
{
	CFile file;
	if ( file.Open(sFullPath, CFile::modeRead) == 0 )
	{
		if ( sFullPath.Find("Collection\\", 0) != -1 )
		{
			sFullPath.Replace("Collection\\", "");
		}
		else if ( sFullPath.Find("Collection/", 0) != -1 )
		{
			sFullPath.Replace("Collection/", "");
		}
		else
		{
			CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
			CString strDownDir = pApp->GetDownDir();
			int iIndex = sFullPath.Find(strDownDir, 0);
			if ( iIndex != -1 )
			{
				sFullPath.Insert(iIndex + strDownDir.GetLength(), "Collection\\");
			}
		}

		if ( file.Open(sFullPath, CFile::modeRead) == 0 )
		{
			sFullPath = _T("");
		}
		else
		{
			file.Close();
		}
	}
}

/*************************************************************
 �� �� ����OpenWaveFile()
 ���ܸ�Ҫ����¼���ļ�
 �� �� ֵ: void
 ��    ����param1	¼���ļ�ȫ·��
		   Param2
**************************************************************/
void OpenWaveFile(CString sFullPath, HWND hWnd)
{
	sFullPath.Replace("/", "\\");
	CString strPath;
	strPath.Format("\"%s\"", sFullPath);
	//���ļ������Ƶ�������
	if(OpenClipboard(hWnd))
	{
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GHND, strPath.GetLength()+1 );
		LPTSTR pszDst = (LPTSTR)GlobalLock(hMem);
		lstrcpy(pszDst,strPath);
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();
	}
	else
	{
		WriteLog("�򿪼�����ʧ��", XJ_LOG_LV2);
	}
	
	//�����Ƿ��Ѿ���STWave������,����Ѿ�����������ϢWM_OPEN_WAVEFILE
	if ( !EnumWindows(EnumWindowsProc, EW_STWAVE) )
	{
		WriteLog("�Ѿ���STWave������, ������ϢWM_OPEN_WAVEFILE", XJ_LOG_LV3);
		return;
	}

	/*
	GetOscFilePath(sFullPath);

	if (sFullPath.IsEmpty())
	{
		if ( AfxMessageBox("δ���ҵ�¼���ļ����Ƿ��ֶ�����?", MB_YESNO) == IDYES )
		{
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

			CFileDialog dlgFile(TRUE, "cfg", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);
			dlgFile.m_ofn.lpstrTitle = _T("ѡ��¼���ļ�"); //�Ի������
			dlgFile.m_ofn.lpstrFilter =_T("*.cfg"); //����
			CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
			dlgFile.m_ofn.lpstrInitialDir = pApp->GetDownDir();
			if(dlgFile.DoModal() == IDOK)
			{
				sFullPath = dlgFile.GetPathName(); 
				WriteLog("¼���ļ�·��Ϊ" + sFullPath);

				//���õ�ǰĿ¼
				SetCurrentDirectory(lpszCurDir);
			}
			else
			{
				//���õ�ǰĿ¼
				SetCurrentDirectory(lpszCurDir);
				return;
			}
		}
		else
		{
			return;
		}
	}
	*/
	
	//û��STWave�����У�������֮������Comtrade�ļ�����Ϊ��������
	CString strDir = g_LocalPath;
	if(g_UseXJWave)
		strDir += PATH_XJWAVE;//"\\XJWave\\";
	else
		strDir += PATH_STWAVE;
	CString strLog;
	strLog.Format("¼���������Ŀ¼Ϊ: %s", strDir);
	WriteLog(strLog, XJ_LOG_LV3);
	HINSTANCE iRet = ShellExecute( hWnd, "open", g_strOscName, strPath, strDir, SW_SHOWNORMAL );	
	if ( (int)iRet <= 32 )
	{
		AfxMessageBox(StringFromID(IDS_TIP_FAULTAPP_NOTFOUND));
	}
	else
	{
		WriteLog("��¼���ļ��ɹ�", XJ_LOG_LV2);
	}
}

/*************************************************************
 �� �� ����OpenXJQuery()
 ���ܸ�Ҫ�����¼���ѯ��ͳ��ģ��
 �� �� ֵ: 
 ��    ����param1	��������
		   Param2	�򿪴���
**************************************************************/
void OpenXJQuery(CString sCmd, HWND hWnd)
{
	//���ļ������Ƶ�������
	if(OpenClipboard(hWnd))
	{
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GHND, sCmd.GetLength()+1 );
		LPTSTR pszDst = (LPTSTR)GlobalLock(hMem);
		lstrcpy(pszDst,sCmd);
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();
	}
	
	// ��Ӧ�ó����������ź��� 
	HANDLE hSem = CreateSemaphore(NULL, 1, 1, "XJQuery"); 
	
	// �ź����Ѵ��ڣ� 
	// �ź������ڣ����������һ��ʵ������ 
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		// �ر��ź������ 
		CloseHandle(hSem); 
		
		// Ѱ����ǰʵ���������� 
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD); 
		while (::IsWindow(hWndPrevious)) 
		{ 
			// ��鴰���Ƿ���Ԥ��ı��? 
			// �У���������Ѱ�ҵ����� 
			if (::GetProp(hWndPrevious, "XJQuery")) 
			{ 
				// ����������С������ָ����С 
				if (::IsIconic(hWndPrevious)) 
					::ShowWindow(hWndPrevious,SW_RESTORE); 
				
				// ���������� 
				::SetForegroundWindow(hWndPrevious); 
				
				// �������ĶԻ��򼤻� 
				::SetForegroundWindow(::GetLastActivePopup(hWndPrevious)); 
				
				//֪ͨXJQuery��ָ��װ�ý����¼���ѯ
				::PostMessage(hWndPrevious, WM_QUERY_EVENT, 0, 0);

				// �˳� 
				break; 
			} 
			
			// ����Ѱ����һ������ 
			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT); 
		} 
	}
	else
	{
		// �ر��ź������ 
		CloseHandle(hSem); 

		//�������м����û�������
		CString sUserPass;
		sUserPass.Format("@%s/%s", theApp.m_User.m_strUSER_ID, theApp.m_User.m_strPassword);
		sCmd += sUserPass;
		
		CString strLog;
		strLog.Format("�򿪲�ѯͳ��ģ��,����Ĳ���Ϊ:%s", sCmd);
		WriteLog(strLog, XJ_LOG_LV2);

		//û�������У�������
		CString strDir = g_LocalPath;
		strDir += PATH_XJQUERY;
		HINSTANCE iRet = ShellExecute( hWnd, "open", "XJQuery.exe", sCmd, strDir, SW_SHOWNORMAL );	
		if ( (int)iRet <= 32 )
		AfxMessageBox(StringFromID(IDS_TIP_QUERYAPP_NOFOUND));
	}
}

/*************************************************************
 �� �� ��: OpenXJAlarmQuery()
 ���ܸ�Ҫ: 
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void OpenXJAlarmQuery( CString sCmd, HWND hWnd )
{
	//���ļ������Ƶ�������
	if(OpenClipboard(hWnd))
	{
		EmptyClipboard();
		HGLOBAL hMem = GlobalAlloc(GHND, sCmd.GetLength()+1 );
		LPTSTR pszDst = (LPTSTR)GlobalLock(hMem);
		lstrcpy(pszDst,sCmd);
		GlobalUnlock(hMem);
		SetClipboardData(CF_TEXT,hMem);
		CloseClipboard();
	}
	
	// ��Ӧ�ó����������ź��� 
	HANDLE hSem = CreateSemaphore(NULL, 1, 1, "XJAlarmQuery"); 
	
	// �ź����Ѵ��ڣ� 
	// �ź������ڣ����������һ��ʵ������ 
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		// �ر��ź������ 
		CloseHandle(hSem); 
		
		// Ѱ����ǰʵ���������� 
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD); 
		while (::IsWindow(hWndPrevious)) 
		{ 
			// ��鴰���Ƿ���Ԥ��ı��? 
			// �У���������Ѱ�ҵ����� 
			if (::GetProp(hWndPrevious, "XJAlarmQuery")) 
			{ 
				// ����������С������ָ����С 
				if (::IsIconic(hWndPrevious)) 
					::ShowWindow(hWndPrevious,SW_RESTORE); 
				
				// ���������� 
				::SetForegroundWindow(hWndPrevious); 
				
				// �������ĶԻ��򼤻� 
				::SetForegroundWindow(::GetLastActivePopup(hWndPrevious)); 
				
				//֪ͨXJQuery��ָ��װ�ý����¼���ѯ
				::PostMessage(hWndPrevious, WM_QUERY_ALARM, 0, 0);
				
				// �˳� 
				break; 
			} 
			
			// ����Ѱ����һ������ 
			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT); 
		} 
	}
	else
	{
		// �ر��ź������ 
		CloseHandle(hSem); 
		
		//û�������У�������
		CString strDir = g_LocalPath;
		strDir += PATH_XJALARMQUERY;
		HINSTANCE iRet = ShellExecute( hWnd, "open", "XJAlarmQuery.exe", sCmd, strDir, SW_SHOWNORMAL );	
		if ( (int)iRet <= 32 )
			AfxMessageBox(StringFromID(IDS_TIP_ALARMSTATAPP_NOFOUND));
	}
}

void OpenXJFaultInversion(CString sFullPath, HWND hWnd)
{
	CString sTempPath = sFullPath;
	sTempPath.Replace(" ", "\" \"");
	CString strDir = g_LocalPath;
	strDir += PATH_XJFAULTINVERSION;
	HINSTANCE iRet = ShellExecute( hWnd, "open", "XJFaultInversion.exe", sTempPath, strDir, SW_SHOWNORMAL );	
	if ( (int)iRet <= 32 )
			AfxMessageBox(StringFromID(IDS_TIP_PLAYBACKAPP_NOTFOUND));
}

CString QueryDIName( CString sPTID, int nDIID, int nCPU )
{
	CString sReturn = "";
	//�齨��ѯ����
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	
	//Ҫ��ѯ���ֶ�
	//������Name
	Field Name;
	bzero(&Name, sizeof(Field));
	str = "Name";
	strncpy(Name.FieldName, str, str.GetLength());
	Name.FieldType = EX_STTP_DATA_TYPE_STRING;
	sql.Fieldlist.push_back(Name);
	
	//����
	Condition PT_ID;
	bzero(&PT_ID, sizeof(Condition));
	str.Format("pt_id = '%s'", sPTID);
	strncpy(PT_ID.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(PT_ID);
	
	Condition CPU;
	bzero(&CPU, sizeof(Condition));
	str.Format("cpu_code = %d", nCPU);
	strncpy(CPU.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(CPU);
	
	Condition DI;
	bzero(&DI, sizeof(Condition));
	str.Format("di_id = %d", nDIID);
	strncpy(DI.ConditionContent, str, str.GetLength());
	sql.Conditionlist.push_back(DI);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_PT_DI_CFG, sql, sError, pMemset);
	if(pMemset != NULL && 1 == nResult)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			sReturn = pMemset->GetValue((UINT)0);
		}
	}
	else
	{
		CString str;
		str.Format("QueryDIName,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return sReturn;
}


CString GetSituationDescribe(int nSituationNo)
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if( !pData )
		return "";
	if( nSituationNo < 0 || nSituationNo >= pData->m_arrSituation.GetSize() )
		return "";
	return pData->m_arrSituation.GetAt(nSituationNo);
	return StringFromID(IDS_COMMON_UNKNOWN);
}


CString GetDetailSituation(CString sSituation)
{
	CString sReturn = "";
	if( sSituation.IsEmpty() )
		return "";
	int nfind = sSituation.Find( ',', 0 );
	if( nfind < 0 )
	{
		return GetSituationDescribe( atoi(sSituation) );
	}
	else
	{
		
		while( nfind > 0 )
		{
			CString stemp = sSituation;
			while( nfind > 0 )
			{
				CString str = stemp.Left( nfind );
				stemp.Delete( 0, nfind+1 );
				if( !str.IsEmpty() )
				{
					sReturn += GetSituationDescribe( atoi(str) );
					sReturn += " | ";
				}
				nfind = stemp.Find( ',', 0 );
			}
			if( !stemp.IsEmpty() )
			{
				//���һ��
				CString str = stemp;
				if( !str.IsEmpty() )
					sReturn += GetSituationDescribe( atoi(str) );
			}
		}
	}
	return sReturn;
}


CString GetDisposeOpinion(int nOpinion)
{
	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CDataEngine* pData = pApp->GetDataEngine();
	if( !pData )
		return "";
	if( nOpinion < 0 || nOpinion >= pData->m_arrDispose.GetSize() )
		return "";
	return pData->m_arrDispose.GetAt(nOpinion);
	return "";
}


CString GetDetailDispose(CString sDispose)
{
	CString sReturn = "";
	if( sDispose.IsEmpty() )
		return StringFromID(IDS_COMMON_NONEED_DEAL);
	int nfind = sDispose.Find( ',', 0 );
	if( nfind < 0 )
	{
		return GetDisposeOpinion( atoi(sDispose) );
	}
	else
	{
		CString stemp = sDispose;
		while( nfind > 0 )
		{
			CString str = stemp.Left( nfind );
			stemp.Delete( 0, nfind+1 );
			if( !str.IsEmpty() )
			{
				sReturn += GetDisposeOpinion( atoi(str) );
				sReturn += " | ";
			}
			nfind = stemp.Find( ',', 0 );
		}
		if( !stemp.IsEmpty() )
		{
			//���һ��
			CString str = stemp;
			if( !str.IsEmpty() )
				sReturn += GetDisposeOpinion( atoi(str) );
		}
	}
	return sReturn;
}

CSize OpenReportFile(IXJReport*& pDoc, CString filename)
{
	CSize szDoc;
	szDoc.cx = szDoc.cy = 0;
	if( !pDoc )
		return szDoc;
	CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
	//�õ�ϵͳ����·��
	CString strDir = pApp->GetDownDir();
	//�齨ȫ·��
	strDir += filename;
	
	if(strDir != "")
	{
		//���п��ܵ�/������"\\"
		strDir.Replace("/", "\\");
	}
	
	
	
	
	szDoc = pDoc->OpenReportFile(strDir);
	if(szDoc.cx > 0 || szDoc.cy > 0)
	{		
	}
	else
	{
		strDir = g_LocalPath + filename;
		szDoc = pDoc->OpenReportFile(strDir);
	}
	return szDoc;
}

CString GetDurationFormatStr()
{
	CString sformat;
	CString sday;
	sday.LoadString(IDS_COMMON_DAY);
	CString shour;
	shour.LoadString(IDS_COMMON_HOUR);
	CString smin;
	smin.LoadString(IDS_COMMON_MINUTE);
	sformat.Format("%%D%s %%H%s %%M%s", sday,shour,smin);
	return sformat;
}

CString StringFromID(UINT id)
{
	HINSTANCE old = AfxGetResourceHandle();
	AfxSetResourceHandle(theApp.m_hInstance);
	CString str="";
	str.LoadString(id);
	AfxSetResourceHandle(old);
	return str;
}


/////////////////////////////////////////////////////////////////////////////
// CXJBrowserApp

BEGIN_MESSAGE_MAP(CXJBrowserApp, CWinApp)
	//{{AFX_MSG_MAP(CXJBrowserApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_STTP_CONNECT, OnSttpConnect)
	ON_UPDATE_COMMAND_UI(ID_STTP_CONNECT, OnUpdateSttpConnect)
	ON_COMMAND(ID_STTP_DISCONNECT, OnSttpDisconnect)
	ON_UPDATE_COMMAND_UI(ID_STTP_DISCONNECT, OnUpdateSttpDisconnect)
	ON_COMMAND(ID_SYS_LOGOUT, OnSysLogout)
	ON_UPDATE_COMMAND_UI(ID_SYS_LOGOUT, OnUpdateSysLogout)
	ON_COMMAND(ID_SYS_EXIT, OnSysExit)
	ON_UPDATE_COMMAND_UI(ID_SYS_EXIT, OnUpdateSysExit)
	ON_COMMAND(ID_HELP_CONTENT, OnHelpContent)
	ON_UPDATE_COMMAND_UI(ID_HELP_CONTENT, OnUpdateHelpContent)
	ON_COMMAND(ID_SYS_SETTING, OnSysSetting)
	ON_UPDATE_COMMAND_UI(ID_SYS_SETTING, OnUpdateSysSetting)
//	ON_COMMAND(ID_APP_CALL, OnAppCall)
	ON_COMMAND(ID_CALL_ALL, OnCallAll)
	ON_COMMAND(ID_FILE_DOWNLOAD, OnFileDownload)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserApp construction

//##ModelId=49B87B7D0224
CXJBrowserApp::CXJBrowserApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bExitClearConfig = FALSE;
	m_bExitReceiveMsg = FALSE;
	m_bConnect = TRUE;
	m_sDownDir = "";
	m_pClearConfigThread = NULL;
	m_pReceiveMsgThread = NULL;
	m_pPostMsgThread = NULL;
	m_pPushMsgThread = NULL;
	m_pFaultDetail = NULL;
	m_pMapView = NULL;
	hSem = NULL;
	m_pPTSetting = NULL;
	m_pPTAnalog = NULL;
	m_pPTDigital = NULL;
	m_pPTSoftBoard = NULL;
	m_pWRSetting = NULL;
	m_pPTAction = NULL;
	m_pPTAlarm = NULL;
	m_pAutoCallResult = NULL;
	m_pComm = NULL;
	m_pPTComm = NULL;
	m_pPTRun = NULL;
	m_sSysLogPath = "";
	m_sNetworkUser = "";
	m_sNetworkPassword = "";
	m_pStationInfo = NULL;
	m_bGetSysLogPath = FALSE;
	m_listFullData.RemoveAll();
	m_hPostEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hReceiveEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	InitializeCriticalSection(&m_CriticalSection);

	m_bCanLock = FALSE;
	m_bIsLocked = FALSE;

	m_ConnectStatus[0] = -1;
	m_ConnectStatus[1] = -1;
}

/*************************************************************
 �� �� ��: TranslateKeyInPTRun()
 ���ܸ�Ҫ��Ϊ����վͨ��״̬ҳ����͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInPTRun(CString sKey, CString &sValue)
{
	if(theApp.m_pPTRun == NULL)
	{
		sValue = "";
		return 0;
	}
	CViewPTRun* pView = theApp.m_pPTRun;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� ��: TranslateKeyInStationInfo()
 ���ܸ�Ҫ��Ϊ��վ��Ϣҳ����͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInStationInfo(CString sKey, CString &sValue)
{
	if(theApp.m_pStationInfo == NULL)
	{
		sValue = "";
		return 0;
	}
	CViewStationInfo* pView = theApp.m_pStationInfo;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� ��: TranslateKeyInPTComm()
 ���ܸ�Ҫ��Ϊ����վͨ��״̬ҳ����͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInPTComm(CString sKey, CString &sValue)
{
	if(theApp.m_pPTComm == NULL)
	{
		sValue = "";
		return 0;
	}
	CViewPTComm* pView = theApp.m_pPTComm;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� ����TranslateKeyInStationComm()
 ���ܸ�Ҫ��Ϊ����վͨ��״̬ҳ����͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInStationComm(CString sKey, CString &sValue)
{
	if(theApp.m_pComm == NULL)
	{
		sValue = "";
		return 0;
	}
	CViewComm* pView = theApp.m_pComm;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� ����TranslateKeyInPTSetting()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInPTAnalog(CString sKey, CString &sValue)
{
	if(theApp.m_pPTAnalog == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTAnalog* pView = theApp.m_pPTAnalog;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� ����TranslateKeyInPTSetting()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInPTDigital(CString sKey, CString &sValue)
{
	if(theApp.m_pPTDigital == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTDigital* pView = theApp.m_pPTDigital;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� ����TranslateKeyInPTSetting()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInPTSoftBoard(CString sKey, CString &sValue)
{
	if(theApp.m_pPTSoftBoard == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTSoftBoard* pView = theApp.m_pPTSoftBoard;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� ����TranslateKeyInPTSetting()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInWRSetting(CString sKey, CString &sValue)
{
	if(theApp.m_pWRSetting == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTSetting* pView = theApp.m_pWRSetting;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}
/*************************************************************
 �� �� ����TranslateKeyInWRDigital()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInWRDigital(CString sKey, CString &sValue)
{
	if(theApp.m_pWRDigital == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTDigital* pView = theApp.m_pWRDigital;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}
/*************************************************************
 �� �� ����TranslateKeyInWRAnalog()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInWRAnalog(CString sKey, CString &sValue)
{
	if(theApp.m_pWRAnalog == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTAnalog* pView = theApp.m_pWRAnalog;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}
/*************************************************************
 �� �� TranslateKeyInPTAlarm()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInPTAlarm(CString sKey, CString &sValue)
{
	if(theApp.m_pPTAlarm == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTAlarm* pView = theApp.m_pPTAlarm;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� TranslateKeyInPTAction()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInAutoCallResult(CString sKey, CString &sValue)
{
	if(theApp.m_pAutoCallResult == NULL)
	{
		sValue = "";
		return 0;
	}
	CAutoCallResult* pView = theApp.m_pAutoCallResult;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� TranslateKeyInPTAction()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInPTAction(CString sKey, CString &sValue)
{
	if(theApp.m_pPTAction == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTAction* pView = theApp.m_pPTAction;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� ����TranslateKeyInPTSetting()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInPTSetting(CString sKey, CString & sValue)
{
	if(theApp.m_pPTSetting == NULL)
	{
		sValue = "";
		return 0;
	}
	CPTSetting* pView = theApp.m_pPTSetting;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� ����TranslateKeyInFaultDetail()
 ���ܸ�Ҫ��Ϊ���ϱ�����ϸ��Ϣ���͹ؼ��ֵĻص�����
 �� �� ֵ: 
 ��    ����param1	Ҫ���͵Ĺؼ���
		   Param2	���ͺ�����
**************************************************************/
int TranslateKeyInFaultDetail(CString sKey, CString & sValue)
{
	if(theApp.m_pFaultDetail == NULL)
	{
		sValue = "";
		return 0;
	}
	CViewFaultDetail* pView = theApp.m_pFaultDetail;
	if(pView == NULL)
	{
		sValue = "";
		return 0;
	}
	//�ж�������Ч��
	if(!IsValidKeyString(sKey))
		return 0;
	CString sFormat = "";
	CStringArray arrKey;
	arrKey.RemoveAll();
	//�ҵ����йؼ���
	sFormat = FindOutAllKey(sKey, arrKey);
	if(arrKey.GetSize() < 1)
		return 0;
	//����ÿ���ؼ���
	for(int i = 0; i < arrKey.GetSize(); i++)
	{
		CString sValue = pView->TranslateSingleKey(arrKey.GetAt(i));
		arrKey.SetAt(i, sValue);
	}
	//����ַ���
	int nFind = 0;
	while(nFind != -1)
	{
		//����%s
		nFind = sFormat.Find("%s", 0);
		if(nFind == -1)
			break;
		//ɾ��%s
		sFormat.Delete(nFind, 2);
		//д��ֵ
		if(arrKey.GetSize() > 0)
		{
			sFormat.Insert(nFind, arrKey.GetAt(0));
			arrKey.RemoveAt(0);
		}
	}
	sValue = sFormat;
	return 0;
}

/*************************************************************
 �� �� ����UpdateBlinkRect()
 ���ܸ�Ҫ��ˢ����˸ͼ������
 �� �� ֵ: 
 ��    ����param1	��˸ͼ�η�Χ
		   Param2
**************************************************************/
int UpdateBlinkRect(CRect rcBlink)
{
	//�����ͼ�Ƿ���Ч
	if(theApp.m_pMapView == NULL)
		return 0;
	//ˢ��
	CRect rc = rcBlink;
	theApp.m_pMapView->DocToClient(rc);
	theApp.m_pMapView->InvalidateRect(rc);
	return 1;
}

/*************************************************************
 �� �� ����ReceiveSTTPConnectStatus()
 ���ܸ�Ҫ����ͨѶ����������״̬�仯����
 �� �� ֵ: 
 ��    ����param1	״̬�仯����������
		   Param2	��״̬
		   param3	���
**************************************************************/
int ReceiveSTTPConnectStatus(XJHANDLE pClientHandle , int pStatus , int group)
{
	if(pClientHandle != theApp.m_SttpHandle)
	{
		//���صĲ��Ǳ����ӵ�״̬
		WriteLog("�õ��˴������ӵ�״̬", XJ_LOG_LV2);
	}
	CString str;
	str.Format("��ͨ�ŷ���������״̬�仯,״̬:%d,���:%d", pStatus, group);
	theApp.WriteLog(str, XJ_LOG_LV3);
			
	theApp.SetSTTPStatus( 1==pStatus, group);

	return 1;
}


/*************************************************************
 �� �� ����AckPTAllEvent()
 ���ܸ�Ҫ��ȷ��ָ�������������¼�
 �� �� ֵ: ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	ָ�����ݱ��ʶ
		   Param2	����ID
**************************************************************/
BOOL AckPTAllEvent(int nTable, CString sID)
{
	BOOL bReturn = TRUE;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	try
	{
		//�����ݿ��޸�Is_Ack�ֶ�
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//����IS_ACK
		
		CString str;	
		//�ֶ�
		//is_ack
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");

		//����
		Condition con1;
		str.Format("pt_id='%s'", sID);
		pApp->m_DBEngine.SetCondition(sql, con1, str);
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJUpdateData(nTable, sql, sError);
		}
		catch (...)
		{
			WriteLog("AckPTAllEvent, ����ʧ��", XJ_LOG_LV3);
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
		}
		else
		{
			CString str;
			str.Format("AckPTAllEvent,����ʧ��,ԭ��Ϊ%s", sError);
			WriteLog(str, XJ_LOG_LV3);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
		delete[] sError;
		sError = NULL;
		return FALSE;
	}
	
	return bReturn;
}

/*************************************************************
 �� �� ����AckAllEvent()
 ���ܸ�Ҫ��ȷ�����ݿ��������¼�
 �� �� ֵ: ȷ�ϳɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	���ݿ���ʶ
		   Param2	�¼�����
**************************************************************/
BOOL AckAllEvent( int nTable, int nEventType /*= -1*/ )
{
	BOOL bReturn = TRUE;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	try
	{
		//�����ݿ��޸�Is_Ack�ֶ�
		CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
		
		//�������µ�num����¼
		//�齨��ѯ����
		SQL_DATA sql;
		sql.Conditionlist.clear();
		sql.Fieldlist.clear();
		
		//����IS_ACK
		
		CString str;	
		//�ֶ�
		//is_ack
		Field Field1;
		pApp->m_DBEngine.SetField(sql, Field1, "IS_ACK", EX_STTP_DATA_TYPE_INT, "1");

		//����
		Condition con1;
		if(nEventType >= 0 && nEventType < 3)
		{
			str.Format("property=%d", nEventType);
			pApp->m_DBEngine.SetCondition(sql, con1, str);
		}
		
		int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJUpdateData(nTable, sql, sError);
		}
		catch (...)
		{
			WriteLog("AckAllEvent, ����ʧ��", XJ_LOG_LV3);
			delete[] sError;
			return FALSE;
		}
		if(nResult == 1)
		{
			bReturn = TRUE;
		}
		else
		{
			CString str;
			str.Format("AckAllEvent,����ʧ��,ԭ��Ϊ%s", sError);
			WriteLog(str, XJ_LOG_LV3);
			bReturn = FALSE;
		}
		
		delete[] sError;
		sError = NULL;
	}
	catch(...)
	{
		delete[] sError;
		sError = NULL;
		return FALSE;
	}
	
	//ȷ�ϳɹ�,ֹͣ�豸��˸
	if(bReturn)
	{
		if(g_BlinkDevice)
		{
			CXJBrowserApp* pApp = (CXJBrowserApp*)AfxGetApp();
			CXJBrowserView * pView = pApp->GetSVGView();
			if(pView != NULL)
			{
				if(EX_STTP_INFO_PT_ACTION_DATA == nTable || EX_STTP_INFO_PT_ALARM_DATA == nTable)
				{
					//������澯
					//ֹͣ�豸��˸
					pView->RemoveAllBlinkDevices();
				}
			}
		}
	}
	
	return bReturn;
}

/*************************************************************
 �� �� ����NeedShow()
 ���ܸ�Ҫ���ж�ָ�����͵��¼��Ƿ���Ҫ��ʾ
 �� �� ֵ: ��Ҫ��ʾ����TRUE, ����Ҫ��ʾ����FALSE
 ��    ����param1	ָ���¼�����. 0-I���¼�, 1-II���¼�, 2-III���¼�
		   Param2
**************************************************************/
BOOL NeedShow(int nEventType)
{
	if(0 == nEventType)
	{
		//I���¼�
		if(1 == g_ShowEventI)
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	else if(1 == nEventType)
	{
		//II���¼�
		if(1 == g_ShowEventII)
			return TRUE;
		else
			return FALSE;
	}
	else if(2 == nEventType)
	{
		//III���¼�
		if(1 == g_ShowEventIII)
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

/*************************************************************
 �� �� ����NeedAlarm()
 ���ܸ�Ҫ���ж�ָ�����͵��¼��Ƿ���Ҫ����
 �� �� ֵ: ��Ҫ��������TRUE, ����Ҫ��������FALSE
 ��    ����param1	ָ���¼�����. 0-I���¼�, 1-II���¼�, 2-III���¼�
		   Param2
**************************************************************/
BOOL NeedAlarm(int nEventType)
{
	if(0 == nEventType)
	{
		//I���¼�
		if(1 == g_AlarmEventI)
		{
			return TRUE;
		}
		else
			return FALSE;
	}
	else if(1 == nEventType)
	{
		//II���¼�
		if(1 == g_AlarmEventII)
			return TRUE;
		else
			return FALSE;
	}
	else if(2 == nEventType)
	{
		//III���¼�
		if(1 == g_AlarmEventIII)
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}
/*************************************************************
 �� �� ��: GetStationStatusString()
 ���ܸ�Ҫ: ����״̬����ȡ��ͨѶ״̬�����ַ���
 �� �� ֵ: ͨѶ״̬�����ַ���
 ��    ��: param1 ״̬����
		   Param2 
**************************************************************/
CString GetStationStatusString( int nStatus )
{
	if(nStatus == 0)
	{
		//�Ͽ�
		return StringFromID(IDS_COMMUSTATUS_OFF);
	}
	else if(nStatus == 1)
	{
		//����
		return StringFromID(IDS_COMMUSTATUS_ON);
	}
	else
	{
		return StringFromID(IDS_COMMON_UNKNOWN);
	}
}

/*************************************************************
 �� �� ��: GetCommStatusString()
 ���ܸ�Ҫ: ����״̬����ȡ��ͨѶ״̬�����ַ���
 �� �� ֵ: ͨѶ״̬�����ַ���
 ��    ��: param1 ״̬����
		   Param2 
**************************************************************/
CString GetCommStatusString( int nStatus )
{
	if(nStatus == 0)
	{
		//����
		return StringFromID(IDS_COMMUSTATUS_ON);
	}
	else if(nStatus == 1)
	{
		//�Ͽ�
		return StringFromID(IDS_COMMUSTATUS_OFF);
	}
	else
	{
		return StringFromID(IDS_COMMON_UNKNOWN);
	}
}

/*************************************************************
 �� �� ��: GetRunStatusString()
 ���ܸ�Ҫ: ����״̬����ȡ������״̬�����ַ���
 �� �� ֵ: ����״̬�����ַ���
 ��    ��: param1 ״̬����
		   Param2 
**************************************************************/
CString GetRunStatusString( int nStatus )
{
	CString sReturn = "";
	switch(nStatus)
	{
	case 0:
		sReturn = StringFromID(IDS_RUNSTATUS_OVERHAULS);
		break;
	case 1:
		sReturn = StringFromID(IDS_RUNSTATUS_OUTAGE);
		break;
	case 2:
		sReturn = StringFromID(IDS_RUNSTATUS_SERVICE);
		break;
	case 3:
		sReturn = StringFromID(IDS_RUNSTATUS_NOACCESS);
		break;
	case 4:
		sReturn = StringFromID(IDS_RUNSTATUS_TESTING);
		break;
	default:
		sReturn = StringFromID(IDS_COMMON_UNKNOWN);
	}
	return sReturn;
}

int GetOperationNum()
{
	int Num = -1;

	CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	
	//�������µ�1����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//TOP num
	Field Field0;
	bzero(&Field0, sizeof(Field));
	CString str;
	str.Format("%d", 1);
	strncpy(Field0.FieldName, str, str.GetLength());
	Field0.FieldType = EX_STTP_DATA_TYPE_TOP;
	sql.Fieldlist.push_back(Field0);
	
	//value
	Field Field1;
	pApp->m_DBEngine.SetField(sql, Field1, "Reserve1", EX_STTP_DATA_TYPE_STRING);
	
	Condition condition2;
	bzero(&condition2, sizeof(Condition));
	//str.Format("order by CAST(Reserve1 AS int) DESC");
	str.Format("order by CAST(Reserve1 AS SIGNED) DESC");
	strncpy(condition2.ConditionContent, str, str.GetLength());
	condition2.ConditionType = EX_STTP_WHERE_ABNORMALITY; //��where����
	sql.Conditionlist.push_back(condition2);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_OPERATION_CFG, sql, sError, pMemset);
	}
	catch (...)
	{
		WriteLogEx("��ѯ������־������, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
		return 0;
	}
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		CString str;
		//ȡ��һ��
		str = pMemset->GetValue((UINT)0); //ID
		if (str == "")
		{
			Num = 0;
		}
		else
		{
			Num = atoi(str) + 1;
		}
	}
	else
	{
		CString str;
		str.Format("��ѯ������־������,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLogEx(str);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return Num;
}

bool IsWin7()  
{  
    OSVERSIONINFOEX osvi;  
    BOOL bOsVersionInfoEx;  
    
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));  
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);  
    bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);  
    
    // win7��ϵͳ�汾ΪNT6.1 
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 6 && osvi.dwMinorVersion >= 0)  
    {  
        return true;      
    }  
    else  
    {  
        return false;  
    }  
}

typedef LANGID (WINAPI *FP_SetThreadUILanguage)(LANGID LangId);
LANGID SetThreadUILanguage(LANGID lcid)
{
    HMODULE hKernel32 = GetModuleHandle(_T("Kernel32.dll"));
    FARPROC pFn = GetProcAddress(hKernel32, "SetThreadUILanguage");
    
    FP_SetThreadUILanguage pSetThreadUILanguage = (FP_SetThreadUILanguage)pFn;
	
    if (pSetThreadUILanguage)
    {
        return pSetThreadUILanguage(lcid);
    }
    return 0;
} 

/////////////////////////////////////////////////////////////////////////////
// CXJBrowserApp initialization

//##ModelId=49B87B7E0262
BOOL CXJBrowserApp::InitInstance()
{
	LONGLONG startTime = StartCount();
/*
	// ��Ӧ�ó����������ź��� 
	hSem = CreateSemaphore(NULL, 1, 1, "XJBrowser"); 
	
	// �ź����Ѵ���
	// �ź������ڣ����������һ��ʵ������ 
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		// �ر��ź������ 
		CloseHandle(hSem); 
		hSem = NULL;
		
		// Ѱ����ǰʵ���������� 
		HWND hWndPrevious = ::GetWindow(::GetDesktopWindow(), GW_CHILD); 
		while (::IsWindow(hWndPrevious)) 
		{ 
			// ��鴰���Ƿ���Ԥ��ı��? 
			// �У���������Ѱ�ҵ����� 
			if (::GetProp(hWndPrevious, "XJBrowser")) 
			{ 
				// ����������С������ָ����С 
				if (::IsIconic(hWndPrevious)) 
					::ShowWindow(hWndPrevious,SW_RESTORE); 
				// ���������� 
				::SetForegroundWindow(hWndPrevious); 
				// �������ĶԻ��򼤻� 
				::SetForegroundWindow(::GetLastActivePopup(hWndPrevious)); 
				
				// �˳���ʵ�� 
				AfxMessageBox("�����Ѿ�������!");
				return FALSE; 
			} 
			// ����Ѱ����һ������ 
			hWndPrevious = ::GetWindow(hWndPrevious, GW_HWNDNEXT); 
		} 
		// ǰһʵ���Ѵ��ڣ����Ҳ��������� 
		//����Ƿ���ڽ�ʬ����
		if ( !CheckSystemProcess())
		{
			return FALSE;
		} 
	}
*/

	
	//�������ݿ�
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	if(m_DBEngine.XJConnect(STTPNET_MODLE_XJBROWSER, sError) == 0)
	{
		//����ʧ��
		CString str;
		str.Format("%s\r\n%s:%s", StringFromID(IDS_DBSERVER_DISCONNECT),StringFromID(IDS_REASON_FOR),sError);
		WriteLog(str);
		AfxMessageBox(str);
		delete[] sError;
		return FALSE;
	}
	memset(sError, '\0', MAXMSGLEN);
	//WriteLog("�������ݿ�ɹ�");

	//����д��Ȼ�޷�ʶ��
	m_brush.CreateSolidBrush(RGB(231,229,236));//�����ɫ
	memset(&lpcmi,0,sizeof(::LPCMENUINFO));
	lpcmi.cbSize=sizeof(MENUINFO);
	lpcmi.fMask=   MIM_BACKGROUND;   
	lpcmi.hbrBack=(HBRUSH)m_brush.operator HBRUSH();


	//����·��
	char   m_szLocalFileName[255]; //�õ���ǰִ���ļ�ȫ����
	for(int i = 0; i < 255; i++)
	{
		m_szLocalFileName[i] = '\0';
	}
	::GetModuleFileName(NULL,m_szLocalFileName,255);

	CString   m_szLocalPath = CString(m_szLocalFileName,255);   
	CString   m_szTemp; 
	while(TRUE) //ȥ���ļ����Ƶõ��ļ�·��
	{   
		m_szTemp=m_szLocalPath.Right(1);   
		if   (m_szTemp != "\\")   
		{   
			m_szLocalPath=m_szLocalPath.Left(m_szLocalPath.GetLength()-1);
			
		}   
		else   
		{   
			break;   
		}   
	}
	g_LocalPath = m_szLocalPath;
	g_UIConfig = g_LocalPath + UI_File;
	g_SysConfig = g_LocalPath + INI_File;
	g_SoundFile = g_LocalPath + SOUND_File;
	g_ImageFile = g_LocalPath + IMAGE_FILE;

	//��־�ļ���·��
	m_sSysLogPath = GetSysLogPath();
	g_LogPath = m_sSysLogPath;
	m_bGetSysLogPath = TRUE;
	char* pPath = (char*)m_sSysLogPath.GetBuffer(0);
	if(m_DBEngine.XJSetLogPath(pPath, sError) == 0)
	{
		//ʧ��
		CString str;
		str.Format("������־·��ʧ��, ·��Ϊ:%s", m_sSysLogPath);
		WriteLog(str);
	}
	
	delete[] sError;

	//����Drag&Drop
	g_dwESDataFormat = ::RegisterClipboardFormat(_T("ESDATAFORMAT"));//ע���Զ������������

	//����ϵͳ����
	LoadConfig();

	//������־�ļ�·��
	m_LogMgr.SetLogPath(g_LogPath);

	m_sSysLogPath.ReleaseBuffer();
	
	m_SttpEngine.LoadSttpLib();

	//�ж���־������
	JudgeOperTableType();

	//���UI�����ļ��Ƿ����
	CFileStatus   status;	   
	if(CFile::GetStatus(g_UIConfig ,status) != TRUE)
	{
		//UI�����ļ�������,����һ�����ļ�
		CFile File;
		if(File.Open(g_UIConfig, CFile::modeCreate | CFile::modeReadWrite))
		{
			CString str;
			str.Format("<?xml version=\"1.0\"?>\r\n<UIConfig>\r\n</UIConfig>");
			char * pBuffer = str.GetBuffer(0);
			File.Write(pBuffer, str.GetLength());
			str.ReleaseBuffer();
		}
		else
		{
			TRACE("open uiconfig faild or wirte faild\n");
		}		
	}



	//��������
	if( IsWin7() )
	{
		if( g_Language.CompareNoCase("en_US") == 0 )
			SetThreadUILanguage(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
	else
	{
		if( g_Language.CompareNoCase("en_US") == 0 )
			SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
	}


	CANCEL_CALL = StringFromID(IDS_BTN_CANCEL_CALL);
	CANCEL_NULL = StringFromID(IDS_CANCEL_NULL);
	STTP_DISCONNECT = StringFromID(IDS_STTP_DISCONNECT);
	


	if (g_bLoginVerify)
	{
		CXJUserStore::GetInstance()->ReLoad();
		//CXJUserStore::GetInstance()->Save("c:/tb_sys_user.txt");

		//�û������֤
		if (!DoLogin())
		{
			SaveSysConfig();
			return FALSE;
		}
		QueryUserStation();
	}

	//�ڴ�������ݶ����еȴ�����
	
	g_wndWait.m_bDrawDetail = TRUE;
	if (g_style == 1)
	{	
		g_wndWait.LoadBitmapEx(/*IDB_BMP_LOGIN*/IDB_BMP_WAIT, RGB(0,255,0), NULL, 0, g_sAppName);
	}
	else if (g_style == 2)
	{
		g_wndWait.LoadBitmapEx(IDB_BMP_WAIT, RGB(0,255,0), NULL, 0, g_sAppName);
	}
	else if (g_style == 3)
	{
		g_wndWait.LoadBitmapEx(g_strStartBmp, RGB(0,255,0), NULL, 0, g_sAppName);
	}
	else
	{
		g_wndWait.LoadBitmapEx(IDB_BMP_WAIT, RGB(0,255,0), NULL, 0, g_sAppName);
	}
	
	g_wndWait.Show(SPLASH_NOTIMER);
	

	//������������
	AfxEnableControlContainer();

	//��ʼ��COM
	if(!AfxOleInit())
		return FALSE;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("XJBrowser"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.


	g_wndWait.AddDetailString(StringFromID(IDS_STARTING_MOUNT));
	//��������洢��
	AddNetworkDisk();
	//���ظ����ĵ�
	InitDocTemplates();	

	//Ϊ������STWave
	strcpy( g_arrStrTable[0], "IPOFAS-���Ϸ�������" );

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;
	
	//����״̬��
	CTime tm;
	CString strTime = tm.GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	CString strTemp;
	strTemp.Format("[%s] %s[%s] %s", strTime, StringFromID(IDS_COMMON_USER),m_User.m_strUSER_ID, StringFromID(IDS_COMMON_LOGIN));
	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_STATUS_INFO, ID_INDICATOR_LOGINTIP, (LPARAM)strTemp.GetBuffer(100));
		strTemp.ReleaseBuffer();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;//�½��ĵ�
	ParseCommandLine(cmdInfo);
	
	if (g_role != MODE_SUB)
	{
		m_pDocTempl[wtCommuView]->OpenDocumentFile(NULL);
	}
	else
	{
		m_pDocTempl[wtMap]->OpenDocumentFile(NULL);
	}

	g_wndWait.AddDetailString(StringFromID(IDS_STARTING_DEVICETREE));
	//�����豸����������
	pMainFrame->m_pDeviceView->FillTree();

	ParseCommandLine(cmdInfo);
	
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	//ȡ������վͨѶ״̬����ָ��

	CFrameWnd * pChild = pMainFrame->GetActiveFrame();
	if (g_role != MODE_SUB)
	{
		if(pChild != NULL)
		{
			if(pChild->IsKindOf(RUNTIME_CLASS(CCommFrame)))
			{
				pMainFrame->pCommFrame = (CCommFrame*)pChild;
			}
		}
	}
	else
	{
		if(pChild != NULL)
		{
			if(pChild->IsKindOf(RUNTIME_CLASS(CChildFrame)))
			{
				pMainFrame->pMapFrame = (CChildFrame*)pChild;
			}
		}
	}



	g_wndWait.AddDetailString(StringFromID(IDS_STARTING_DEFAULTSTATION));
	//��Ĭ�ϳ�վ
	if(g_role != MODE_SUB)
	{
		OpenDefaultStation(g_DefaultStation);
	}
	else
	{
		OpenDefaultStation();
	}
	g_wndWait.AddDetailString(StringFromID(IDS_STARTING_LOADDATA_COMPLETED));
	g_wndWait.Hide();//���ݶ���ȴ����ڹر�
	// The main window has been initialized, so show and update it.
	m_nCmdShow = SW_MAXIMIZE;XJSetMenuInfo();
	pMainFrame->ShowWindow(SW_MAXIMIZE);	
	pMainFrame->SetMenu(NULL);
	int result = ::SetForegroundWindow(GetMainWnd()->m_hWnd);

    if (g_bConnectCommServer)
    {
		//������ͨѶ������������
		ConnectSTTPServer();
	}

	StartClearConfigThread();
	

	EndCount("ϵͳ����ʱ��Ϊ", startTime);

	m_bCanLock = TRUE;
	m_bIsLocked = FALSE;

	if(g_UILockTime > 0)
		m_pMainWnd->PostMessage(STARTLOCK);

	return TRUE;
}

/*************************************************************
 �� �� ����InitDocTemplates()
 ���ܸ�Ҫ����ʼ�����ĵ�����
 �� �� ֵ: 
**************************************************************/
//##ModelId=49B87B7D037B
void CXJBrowserApp::InitDocTemplates()
{
	//������ͼ����
	CMultiDocTemplate *pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWMAP,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CXJBrowserView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtMap]=pDocTemplate;

	if (g_role != MODE_SUB)
	{
		//����վͨѶ״̬����
		pDocTemplate = new CMultiDocTemplate(
			//IDR_XJBROWTYPE,
			1,
			RUNTIME_CLASS(CXJBrowserDoc),
			RUNTIME_CLASS(CCommFrame), // custom MDI child frame
			RUNTIME_CLASS(CViewCommMap));
		AddDocTemplate(pDocTemplate);
		m_pDocTempl[wtCommuView]=pDocTemplate;
	}
	
	//�����¼�����
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CActionFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewAction));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtAction]=pDocTemplate;
	
	 //�澯����
	 pDocTemplate = new CMultiDocTemplate(
	 //IDR_XJBROWTYPE,
	 1,
	 RUNTIME_CLASS(CXJBrowserDoc),
	 RUNTIME_CLASS(CAlarmFrame), // custom MDI child frame
	 RUNTIME_CLASS(CViewAlarm));
	 AddDocTemplate(pDocTemplate);
	 m_pDocTempl[wtAlarm]=pDocTemplate;
	  
	 if (g_role != MODE_SUB)
	 {
		 //�¹ʱ��洰��
		 pDocTemplate = new CMultiDocTemplate(
			 //IDR_XJBROWTYPE,
			 1,
			 RUNTIME_CLASS(CXJBrowserDoc),
			 RUNTIME_CLASS(CFaultFrame), // custom MDI child frame
			 RUNTIME_CLASS(CViewFaultSheet));
		 AddDocTemplate(pDocTemplate);
		 m_pDocTempl[wtFault]=pDocTemplate;
	 }
		
	//¼���¼�����
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewOSC));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtOSC]=pDocTemplate;
		  
	//�������¼�����
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewDI));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtDI]=pDocTemplate;
			
	//����ͨѶ״����
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewPTComm));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtPTCommStatus]=pDocTemplate;
			  
	//��������״̬����
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CChildFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewPTRun));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtPTRunStatus]=pDocTemplate;
				
	//ϵͳ�澯����
	pDocTemplate = new CMultiDocTemplate(
	//IDR_XJBROWTYPE,
	1,
	RUNTIME_CLASS(CXJBrowserDoc),
	RUNTIME_CLASS(CSysAlarmFrame), // custom MDI child frame
	RUNTIME_CLASS(CViewSysAlarm));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtSysAlarm]=pDocTemplate;

	//�������Դ���
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CPTFrame), // custom MDI child frame
		RUNTIME_CLASS(CPTSheet));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtPTSheet]=pDocTemplate;

	//¼�������Դ���
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CWaveRecFrame), // custom MDI child frame
		RUNTIME_CLASS(CPTSheet));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtWaveRecSheet]=pDocTemplate;

	//�����ٻ�����
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CBatchFrame), // custom MDI child frame
		RUNTIME_CLASS(CBatchSelectView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtBatchCall]=pDocTemplate;

	//�Զ��������ô���
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CAutoCallFrame), // custom MDI child frame
		RUNTIME_CLASS(CBatchSelectView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtAutoCall]=pDocTemplate;

	//�Զ����ٽ������
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CAutoResultFrame), // custom MDI child frame
		RUNTIME_CLASS(CACResultSel));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtAutoCallResult]=pDocTemplate;

	//ͨ���ļ����ش���
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CDownloadFrame), // custom MDI child frame
		RUNTIME_CLASS(CDownLoadView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtDownload]=pDocTemplate;

	//Ԥ������
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CCrossFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewCross));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtCross]=pDocTemplate;

	//I���¼�����
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CSoeFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewSOE));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtSOEI]=pDocTemplate;

	//II���¼�����
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CSoeFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewSOE));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtSOEII]=pDocTemplate;

	//III���¼�����
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CSoeFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewSOE));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtSOEIII]=pDocTemplate;

	//������Ϣ����
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CSoeFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewSOE));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtSOEDebug]=pDocTemplate;

	//����ͼ����
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CGeoView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtGeo]=pDocTemplate;

	//��վ��Ϣ����
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CStationInfoFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewStationInfo));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtStationInfo]=pDocTemplate;

	//¼������ҳ��
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CDownOscFrame), // custom MDI child frame
		RUNTIME_CLASS(CDownOscView));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtDownOsc]=pDocTemplate;

	//С�����ҳ��
	pDocTemplate = new CMultiDocTemplate(
		//IDR_XJBROWTYPE,
		1,
		RUNTIME_CLASS(CXJBrowserDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewDistance));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtDistance]=pDocTemplate;

	//ʩ��ͼҳ��
	pDocTemplate = new CMultiDocTemplate(
		IDR_MENU_DRAFT,
		RUNTIME_CLASS(CDraftDocument),
		RUNTIME_CLASS(CDraftFrame), // custom MDI child frame
		RUNTIME_CLASS(CDraftList));
	AddDocTemplate(pDocTemplate);
	m_pDocTempl[wtDraft]=pDocTemplate;
}

/*************************************************************
 �� �� ����GetCurDocument()
 ���ܸ�Ҫ���ҵ���ǰ��ĵ�
 �� �� ֵ: ��ǰ�ĵ�ָ��
**************************************************************/
//##ModelId=49B87B7E000F
CXJBrowserDoc * CXJBrowserApp::GetCurDocument()
{
	//�õ������ָ��
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return NULL;

	return pFrame->m_pDoc;
}

/*************************************************************
 �� �� ����GetCurView()
 ���ܸ�Ҫ���õ���ǰ���ͼ
 �� �� ֵ: ��ǰ���ͼָ��
**************************************************************/
//##ModelId=49B87B7E002E
CView * CXJBrowserApp::GetCurView()
{
	//�õ������ָ��
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return NULL;
	//�õ���ǰ��ӿ��ָ��
	CChildFrame * pChildFrame = (CChildFrame *)pFrame ->GetActiveFrame();
	if(pChildFrame == NULL)
		return NULL;
	//�õ���ǰ��ĵ�ָ��
	CView * pView = pChildFrame->GetActiveView();
	if(pView == NULL)
		return NULL;
	return pView;
}

//##ModelId=49B87B7E005D
void CXJBrowserApp::WriteLog(CString sLog, int nLV)
{
	m_LogMgr.WriteLog(sLog, nLV);
}

//##ModelId=49B87B7E004E
CDataEngine* CXJBrowserApp::GetDataEngine()
{
	CXJBrowserDoc * pDoc = GetCurDocument();
	if(pDoc == NULL)
		return NULL;
	return &(pDoc->m_DataEngine);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

//##ModelId=49B87BB603D8
class CAboutDlg : public CDialog
{
public:
	//##ModelId=49B87BB7000F
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	//##ModelId=49B87BB7003E
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//##ModelId=49B87BB7000F
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

//##ModelId=49B87BB7003E
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

// App command to run the dialog
//##ModelId=49B87B7E0280
void CXJBrowserApp::OnAppAbout()
{
// 	CAboutDlg aboutDlg;
// 	aboutDlg.DoModal();
	CAboutDlgEx dlg;
	dlg.DoModal();
}

/*************************************************************
 �� �� ����STTPConnect()
 ���ܸ�Ҫ��������ͨѶ������������
 �� �� ֵ: ������Sttp�ͻ��˶���ľ����ʧ��ʱ����NULL
 ��    ����param1	����ģʽ. 0��ͬ��ģʽ��Ĭ�ϣ�, 1���첽ģʽ
**************************************************************/
//##ModelId=49B87B7E009C
XJHANDLE CXJBrowserApp::STTPConnect(int& pConnected, int pRunModle /*= 0*/ )
{
	//����ͨѶ��־·��
	if (!m_bGetSysLogPath)
	{
		m_sSysLogPath = GetSysLogPath();
		g_LogPath = m_sSysLogPath;
	}
	char * pPath = (char*)g_LogPath.GetBuffer(0);
	
	//�ͻ������Ͳ��ɸ�
	return m_SttpEngine.XJSttpConnectServer(STTPNET_MODLE_XJBROWSER, pConnected, pPath, pRunModle);
}

/*************************************************************
 �� �� ����DoLogin()
 ���ܸ�Ҫ����¼ϵͳ, ��֤�û����
 �� �� ֵ: ��¼�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B7D0399
BOOL CXJBrowserApp::DoLogin()
{
	CDlgValidateID dlg;
	//dlg.m_strUser = m_User.m_strUSER_ID;
	//dlg.m_strPassword = m_User.m_strPassword;
	if (dlg.DoModal() == IDOK)
	{
		m_bIsLocked = FALSE;

		//����m_user����
		m_User.m_strUSER_ID	= dlg.m_strUser;
		m_User.m_strGROUP_ID= dlg.m_strUserGroup;
		m_User.m_strPassword= dlg.m_strPassword;

		
		//����״̬��
		CTime tm;
		CString strTime = tm.GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
		CString strTemp, strMsg;
		strTemp.Format("[%s] %s[%s] %s", strTime, StringFromID(IDS_COMMON_USER),m_User.m_strUSER_ID, StringFromID(IDS_COMMON_LOGIN));
		strMsg = strTemp;
		::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_STATUS_INFO, ID_INDICATOR_LOGINTIP, (LPARAM)strTemp.GetBuffer(100));
		strTemp.ReleaseBuffer();

		//����˹�������־
		AddNewManOperator(FUNC_XJBROWSER_BROWSER, "�û���½", strMsg, -1);

		//SetUserLoginFlag(m_User.m_strUSER_ID, m_User.m_strGROUP_ID, CString("1"));
		CXJUserStore::GetInstance()->SetUserFlags(m_User.m_strUSER_ID.GetBuffer(0)
			, m_User.m_strGROUP_ID.GetBuffer(0), 1);
		CXJUserStore::GetInstance()->Save("c:/tb_sys_user.txt");
		
		return TRUE;
	}
	
	return FALSE;
}

/*************************************************************
 �� �� ����DOLogOut()
 ���ܸ�Ҫ���ǳ�ϵͳ,�����û���¼
 �� �� ֵ: �ٵ�¼�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B7D03B9
BOOL CXJBrowserApp::DoLogOut()
{
	if(m_User.m_strUSER_ID.IsEmpty())
		return TRUE;
	
	//���ԭ�ȵ��û���
	CString strUserID = m_User.m_strUSER_ID;
	CString strUserPWD = m_User.m_strPassword;
	m_User.m_strUSER_ID.Empty();
	m_User.m_strPassword.Empty();
	
	//�����û���¼����Ҫ�����µ�¼
	if ( !DoLogin() )
	{
		if ( AfxMessageBox( StringFromID(IDS_TIP_LOGOUT_FAIL), MB_OKCANCEL ) == IDOK )
		{
			CMainFrame* pFrame = (CMainFrame*)GetMainWnd();
			if(pFrame != NULL)
				pFrame->SendMessage(WM_CLOSE);
		}
		
		m_User.m_strUSER_ID = strUserID;
		m_User.m_strPassword = strUserPWD;	
	}
	return TRUE;
}

/*************************************************************
 �� �� ����TryEnter()
 ���ܸ�Ҫ������ĳ�û��Ƿ���н���ĳ���ܵ�Ȩ��,Ŀǰֻ֧�ֲ�ѯ��ǰ�û�����
 �� �� ֵ: �ɽ���ĳ���ܵ�Ȩ�޷���TRUE,�����÷���FALSE
 ��    ����param1	����ID
		   Param2	�û�ID, NULLʱ�õ�ǰ�û�����
		   param3	�Ƿ���ʾ���洰
**************************************************************/
//##ModelId=49B87B7D03D8
BOOL CXJBrowserApp::TryEnter( LPCTSTR strFuncID, LPCTSTR strUserID /*= NULL*/, BOOL bShowAlarm /*= TRUE*/ )
{
	//Ŀǰֻ֧�ֲ�ѯ��ǰ�û�����
	ASSERT(strFuncID!=NULL);
	BOOL bCan=FALSE;
	if(m_User.m_strUSER_ID.IsEmpty() && strUserID == NULL)
	{
		bCan = FALSE;
	}

	CString strGroup = "";

	if(strUserID != NULL)
	{
		//�����û�����
		strGroup = QueryUserGroup(strUserID);
	}
	else
		strGroup = m_User.m_strGROUP_ID;
	//�齨��ѯ����
	//��֯SQL���
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//����
	Condition con1;
	CString str;
	str.Format("GROUP_ID='%s'", strGroup);
	m_DBEngine.SetCondition(sql, con1, str);
	//������
	Condition con2;
	str.Format("FUNC_ID='%s'", strFuncID);
	m_DBEngine.SetCondition(sql, con2, str);

	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_GROUP_FUNC_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::TryEnter, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//��ѯ�ɹ�
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			bCan = TRUE;
		}
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::TryEnter, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	if( (!bCan) && bShowAlarm)
	{
		CString str;
		str.Format("%s [%s]", StringFromID(IDS_VALIDATE_AUTHORITY),strFuncID);
		AfxMessageBox(str);
	}
	return bCan;
}

/*************************************************************
 �� �� ����ConnectSTTPServer()
 ���ܸ�Ҫ��������ͨѶ������������, ���������ձ����߳�
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B7E00BB
BOOL CXJBrowserApp::ConnectSTTPServer()
{
	//������ͨѶ������������, �õ���������
	int nResult = -1;
	m_SttpHandle = STTPConnect(nResult);

	SetSTTPStatus(1 == nResult, 0);

	if (m_SttpEngine.XJSTTPRegisterHandler(m_SttpHandle, ReceiveSTTPConnectStatus, STTPNET_CALLBACK_ONCONNECTCHANGE))
	{
		WriteLog(_T("CXJBrowserApp::ConnectSTTPServer() ע��ص�����ReceiveSTTPConnectStatus�ɹ�"), 3);
	}
	else
	{
		WriteLog(_T("CXJBrowserApp::ConnectSTTPServer() ע��ص�����ReceiveSTTPConnectStatusʧ��"), 3);
	}
	

	//���������߳�
	if (theApp.m_pReceiveMsgThread == NULL)
	{
		m_bExitReceiveMsg = FALSE;
		m_pReceiveMsgThread = AfxBeginThread(ReceiveMessageThread,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pReceiveMsgThread != NULL)
		{
			m_pReceiveMsgThread->m_bAutoDelete = FALSE;
			m_pReceiveMsgThread->ResumeThread();
		}
	}


	if (m_pPostMsgThread == NULL)
	{
		//�����ַ��߳�
		m_pPostMsgThread = AfxBeginThread(PostMessageThread,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pPostMsgThread != NULL)
		{
			m_pPostMsgThread->m_bAutoDelete = FALSE;
			m_pPostMsgThread->ResumeThread();
		}
	}


	if (m_pPushMsgThread == NULL)
	{
		//�����ַ��߳�
		m_pPushMsgThread = AfxBeginThread(PushMsgThread,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pPushMsgThread != NULL)
		{
			m_pPushMsgThread->m_bAutoDelete = FALSE;
			m_pPushMsgThread->ResumeThread();
		}
	}

	SetEvent(m_hPostEvent);
	SetEvent(m_hReceiveEvent);


	return TRUE;
}

/*************************************************************
 �� �� ����ConnectSTTPServer()
 ���ܸ�Ҫ��������ͨѶ������������, ���������ձ����߳�
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
//##ModelId=49B87B7E00BB
BOOL CXJBrowserApp::ConnectSTTPServerB()
{
	//������ͨѶ������������, �õ���������
	int nResult = -1;
	m_SttpHandle = STTPConnect(nResult);

	SetSTTPStatus(1 == nResult, 0);

	if (m_SttpEngine.XJSTTPRegisterHandler(m_SttpHandle, ReceiveSTTPConnectStatus, STTPNET_CALLBACK_ONCONNECTCHANGE))
	{
		WriteLog(_T("CXJBrowserApp::ConnectSTTPServer() ע��ص�����ReceiveSTTPConnectStatus�ɹ�"), 3);
	}
	else
	{
		WriteLog(_T("CXJBrowserApp::ConnectSTTPServer() ע��ص�����ReceiveSTTPConnectStatusʧ��"), 3);
	}

	SetEvent(m_hPostEvent);
	SetEvent(m_hReceiveEvent);

	return TRUE;
}


/*************************************************************
 �� �� ����DisConnectSTTPServer()
 ���ܸ�Ҫ���Ͽ���ͨѶ������������, ���رս����̱߳���
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
BOOL CXJBrowserApp::DisConnectSTTPServer()
{
	//�˳����ձ����߳�
	if(m_pReceiveMsgThread != NULL)
	{
		ResetEvent(m_hReceiveEvent);
		WriteLog("�жϽ��ձ����̳߳ɹ�");
	}

	//�˳��ַ������߳�
	if(m_pPostMsgThread != NULL)
	{
		ResetEvent(m_hPostEvent);
		WriteLog("�жϷַ������̳߳ɹ�");
	}
	
	//�ر�����
	int nResult = m_SttpEngine.XJSttpCloseConnection(m_SttpHandle);
	if(1 == nResult)
	{
		//�رճɹ�
		m_SttpHandle = NULL;
		WriteLog("�Ͽ�ͨ�ŷ��������ӳɹ�");
	}
	else
	{
		WriteLog("�Ͽ���ͨ�ŷ�����������ʧ��");
		return FALSE;
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CXJBrowserApp message handlers

//##ModelId=49B87B7E029F
void CXJBrowserApp::OnAppExit() 
{
	// TODO: Add your command handler code here
	CMainFrame* pFrame = (CMainFrame*)GetMainWnd();
	if(pFrame != NULL)
		pFrame->SendMessage(WM_CLOSE);
}

/*************************************************************
 �� �� ����SetSTTPStatus()
 ���ܸ�Ҫ��������ͨѶ����������״̬��־, �ı�״̬����Ϣ
 �� �� ֵ: void
 ��    ����param1	����״̬
**************************************************************/
//##ModelId=49B87B7E00EA
void CXJBrowserApp::SetSTTPStatus( BOOL bConnected, int group )
{
	if( group != 1 && group != 2 ){
		group = 1;
	}
	BOOL bRefresh = FALSE;
	if( 1 == group )
	{
		//��������״̬
		if(!bConnected)
		{
			//�����ж�
			m_ConnectStatus[0] = 0;
			theApp.WriteLog(_T("����ͨ�ŷ����������ж�"), XJ_LOG_LV1);
			
			theApp.AddNewManOperator("���ӳ���ͨ�ŷ�����", "���ӳ���ͨ�ŷ�����", "�볣��ͨ�ŷ����������ж�", -1, OPER_FAILD);
		}
		else
		{
			bRefresh = TRUE;
			//��������
			m_ConnectStatus[0] = 1;
			theApp.WriteLog(_T("����ͨ�ŷ��������ӳɹ�"), XJ_LOG_LV1);
			
			theApp.AddNewManOperator("���ӳ���ͨ�ŷ�����", "���ӳ���ͨ�ŷ�����", "�볣��ͨ�ŷ��������ӳɹ�", -1, OPER_SUCCESS);
		}
	}
	else if( 2 == group )
	{
		//��������״̬
		if(!bConnected)
		{
			//�����ж�
			m_ConnectStatus[1] = 0;
			theApp.WriteLog(_T("61850ͨ�ŷ����������ж�"), XJ_LOG_LV1);
			
			theApp.AddNewManOperator("����61850ͨ�ŷ�����", "����61850ͨ�ŷ�����", "��61850ͨ�ŷ����������ж�", -1, OPER_FAILD);
		}
		else
		{
			bRefresh = TRUE;
			//��������
			m_ConnectStatus[1] = 1;
			theApp.WriteLog(_T("61850ͨ�ŷ��������ӳɹ�"), XJ_LOG_LV1);
			
			theApp.AddNewManOperator("����61850ͨ�ŷ�����", "����61850ͨ�ŷ�����", "��61850ͨ�ŷ��������ӳɹ�", -1, OPER_SUCCESS);
		}
	}

	//�ı�״̬����Ϣ
	CString strTemp;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if(bRefresh)
	{
		if(pFrame != NULL)
		{
			pFrame->SendMessage(COMM_STATUS_CHANGE, 0, 0);
			//ȡ��ͨѶ�Ͽ�ʱ��
			int nCommDisconnectTime = pFrame->GetCommDisconnectTime();
			CString str;
			str.Format("��ͨѶ�������Ͽ� %d��", nCommDisconnectTime);
			WriteLog(str, XJ_LOG_LV1);
			if(nCommDisconnectTime >= g_DisconnectTime && g_DisconnectTime > 0)
			{
				//��ȡ����״̬
				if(GetDataEngine() != NULL)
				{
					if( g_role != MODE_SUB )
					{
						GetDataEngine()->RefreshStationStatus();
						WriteLog(_T("ͨ�Żָ�,���¶�ȡ��վ״̬�ɹ�"));
					}
					GetDataEngine()->LoadAllSec();
					GetDataEngine()->RefreshDevStatus();
					WriteLog(_T("ͨ�Żָ�,���¶�ȡ����״̬�ɹ�"));
				}
				
				//ǿ��ˢ��ҳ��
				pFrame->SendMessage(VIEW_REFRESH, 0, 0);
				WriteLog(_T("ǿ��ˢ��ҳ��ɹ�"));
			}else{
				//ֻˢ�³�վͨ��״̬
				//��ȡ����״̬
				if(GetDataEngine() != NULL)
				{
					if( g_role != MODE_SUB )
					{
						GetDataEngine()->RefreshStationStatus();
						WriteLog(_T("ͨ�Żָ�,���¶�ȡ��վ״̬�ɹ�"));
					}
					//ǿ��ˢ��ҳ��
					pFrame->SendMessage(VIEW_REFRESH, 0, 1);
					WriteLog(_T("ǿ��ˢ��ҳ��ɹ�"));
				}
			}
		}
	}
	else
	{
		//�����ж�
		if(pFrame != NULL)
			pFrame->SendMessage(COMM_STATUS_CHANGE, 0, 1);
	}
	if( m_ConnectStatus[0] != -1 )
	{
		strTemp.Format("%s:%s", StringFromID(IDS_COMMUSERVER_CONVENTION),
			0 == m_ConnectStatus[0]?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
	}
	if( m_ConnectStatus[1] != -1 )
	{
		CString str = "";
		str.Format("            %s:%s", StringFromID(IDS_COMMUSERVER_61850),
			0 == m_ConnectStatus[1]?StringFromID(IDS_COMMUSTATUS_OFF):StringFromID(IDS_COMMUSTATUS_ON));
		strTemp += str;
	}
	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_STATUS_INFO, ID_INDICATOR_STTPSTATE, (LPARAM)strTemp.GetBuffer(0));
	strTemp.ReleaseBuffer(0);
}

/*************************************************************
 �� �� ����GetSTTPStatus()
 ���ܸ�Ҫ��ȡ����ͨѶ��������ͨѶ״̬
 �� �� ֵ: ͨѶ��������TRUE, ͨѶ�жϷ���FALSE
**************************************************************/
//##ModelId=49B87B7E0109
BOOL CXJBrowserApp::GetSTTPStatus()
{
	return (1==m_ConnectStatus[0] || 1==m_ConnectStatus[1]);
}


/****************************************************
Date:2011/12/20  Author:LYH
������:   AddNewManOperator	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString FunID	
����: CString Act	
����: CString strMsg	
����: CString strUser	
����: int nOperType	
����: int nOperResult	
*****************************************************/
void CXJBrowserApp::AddNewManOperator( CString FunID, CString Act, CString strMsg, CString strUser, int nOperType, int nOperResult , int num)
{
		/*��û��������Ա�����˹�������־��¼*/
	DWORD nSize = MAX_COMPUTERNAME_LENGTH + 1; 
	char strComputer[MAX_COMPUTERNAME_LENGTH+1]; 
	memset( strComputer, 0, sizeof(strComputer) );
	::GetComputerName( strComputer, &nSize );

	CString sComputer = strComputer;

	//ȡ�õ�ǰʱ��
	CTime tm = CTime::GetCurrentTime();
	CString sTime = tm.Format("%Y-%m-%d %H:%M:%S");
	
	//��֯SQL���
	BLOB_SQL_DATA sql;
	sql.Blob_Conditionlist.clear();
	sql.BlobFieldlist.clear();
	
	//ָ��KeyName,����Value
	
	CString str;	
	//�ֶ�
	//Msg
	BLOB_FIELD Field1;
	m_DBEngine.SetBlobField(sql, Field1, "Msg", EX_STTP_DATA_TYPE_STRING, strMsg);

	//UserName
	BLOB_FIELD Field2;
	CString sUser = strUser;
	if(strUser.IsEmpty())
		sUser = m_User.m_strUSER_ID;
	m_DBEngine.SetBlobField(sql, Field2, "UserName", EX_STTP_DATA_TYPE_STRING, sUser);

	//Computer
	BLOB_FIELD Field3;
	m_DBEngine.SetBlobField(sql, Field3, "Computer", EX_STTP_DATA_TYPE_STRING, sComputer);

	//Func
	BLOB_FIELD Field4;
	m_DBEngine.SetBlobField(sql, Field4, "Func", EX_STTP_DATA_TYPE_STRING, FunID);

	//Act
	BLOB_FIELD Field5;
	m_DBEngine.SetBlobField(sql, Field5, "Act", EX_STTP_DATA_TYPE_STRING, Act);

	//Time
	BLOB_FIELD Field6;
	m_DBEngine.SetBlobField(sql, Field6, "Time", EX_STTP_DATA_TYPE_TIME, sTime);

	if(g_OperTableType == 1)
	{
		//OperResult
		str.Format("%d", nOperResult);
		BLOB_FIELD Field7;
		m_DBEngine.SetBlobField(sql, Field7, "OperResult", EX_STTP_DATA_TYPE_INT, str);

		//OperType
		BLOB_FIELD Field8;
		str.Format("%d", nOperType);
		m_DBEngine.SetBlobField(sql, Field8, "OperType", EX_STTP_DATA_TYPE_INT, str);
		
		//opernum
		BLOB_FIELD Field9;
		str.Format("%d", num);
		m_DBEngine.SetBlobField(sql, Field9, "Reserve1", EX_STTP_DATA_TYPE_STRING, str);
	}

	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJInsertBlobData(EX_STTP_INFO_OPERATION_CFG, sql, sError);
	}
	catch (...)
	{
		WriteLog("���������־�����ݿ�ʧ��");
		return;
	}
	if(nResult == 1)
	{

	}
	else
	{
		CString str;
		str.Format("���������־�����ݿ�ʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
}
void CXJBrowserApp::AddNewManOperator( CString FunID, CString Act, CString strMsg, int nOperType, int nOperResult /*= 0*/, int num /*= -1*/ )
{
	AddNewManOperator(FunID, Act, strMsg, "", nOperType, nOperResult, num);
}

//������ limit ����ָ������ַ����Ĵ���Ŷ.
vector<CString> CXJBrowserApp::SplitCString(CString &str,const CString find, int limit)
{
	vector<CString> ret;

	CString strTmp = str;
	strTmp.TrimLeft();
	strTmp.TrimRight();
	if (strTmp.IsEmpty())
		return ret;

	int start = 0;
	int pos = strTmp.Find(find,start);
	int len = find.GetLength();
	int i = 0;
	while(true){
		if(pos<0 || (limit>0 && i+1==limit)){ //NO FIND   
			ret.push_back(strTmp.Mid(start));
			break;
		}else{
			ret.push_back(strTmp.Mid(start,pos-start));
			start = pos+len;
			pos = strTmp.Find(find,start);
		}
		i++;
	}
	return ret;
}

BOOL CXJBrowserApp::NextPTSetModState(int nNextStateID, const PT_ZONE &data, const CString &sUserID, const CString &sNewRecords)
{
	BOOL bReturn = FALSE;
	
	//CheckKeyNameForPTSet();

	PT_ZONE curZone;
	CString sRecords;
	int nCurStateID = GetPTSetModState(curZone, sRecords);

	CString str;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);

	str.Format(" current PT_SET_MOD_STATE = %d \n next PT_SET_MOD_STATE: %d", nCurStateID, nNextStateID);
	//AfxMessageBox(str);
	
	CString strUserID = sUserID;
	if (sUserID.IsEmpty())
		strUserID = m_User.m_strUSER_ID;

	CTime tm;
	CString strTime = tm.GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");

    // ���¶�ֵ�޸�״̬��
	// �齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	// value
	Field fld0;
	str.Format("%d", nNextStateID);
	m_DBEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_INT, str);
	
	Field fld1; // ����װ��
	str.Format("%s", data.PT_ID);
	m_DBEngine.SetField(sql, fld1, "reverse1", EX_STTP_DATA_TYPE_STRING, str);

	Field fld2; // ��������
	str.Format("%d,%d", data.cpu, data.code);
	m_DBEngine.SetField(sql, fld2, "reverse2", EX_STTP_DATA_TYPE_STRING, str);

	Field fld3; // ������¼
	if (0 != nNextStateID){
		str.Format("%s,%s,%d", strTime, strUserID, nNextStateID);
		
		if (1 != nNextStateID){
			if (!sRecords.IsEmpty())
				str = sRecords + ";" + str;
		}
		if (!sNewRecords.IsEmpty())
			str = sNewRecords;
	}else{
		str = "";
	}
	m_DBEngine.SetField(sql, fld3, "reverse3", EX_STTP_DATA_TYPE_STRING, str);
	
	//����
	//keyname
	Condition cond0;
	str.Format("keyname = 'DZ_MOD_STATE'");
	m_DBEngine.SetCondition(sql, cond0, str);
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJUpdateData(EX_STTP_INFO_TBSYSCONFIG, sql, sError);

		if(1 == nResult){
			bReturn = TRUE;
		}else{
			str.Format("CXJBrowserApp::NextPTSetModState, ����ʧ��, ԭ��Ϊ��%s", sError);
			WriteLog(str);
			AfxMessageBox(str);

			bReturn = FALSE;
		}
	}
	catch (...)
	{
		WriteLog("CXJBrowserApp::NextPTSetModState, ����ʧ��");
		AfxMessageBox(sError);
		delete[] sError;

		return FALSE;
	}
	
	delete[] sError;
	sError = NULL;

	return bReturn;
}

/*BOOL CXJBrowserApp::SaveTempPTSetToDB(CString &sPTID, MODIFY_LIST &arrModifyList)
{
	BOOL bReturn = FALSE;
	
	CString str;
	char * sError = NULL;
	
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	if(arrModifyList.GetSize() < 0)
		return FALSE;
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	int nResult;
	for(int i = 0; i < arrModifyList.GetSize(); i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)arrModifyList.GetAt(i);
		CString strID;
		strID.Format("%d", sttpData->id);
		
		Field fld;
		str.Format("%s", sttpData->str_value.c_str());
		m_DBEngine.SetField(sql, fld, "RESERVE3", EX_STTP_DATA_TYPE_STRING, str);
		
		//����
		//station_id
		Condition condition;
		str.Format("CPU_CODE = '%d'", sttpData->nCpu);
		m_DBEngine.SetCondition(sql, condition, str);
		
		Condition condition1;
		str.Format("SETTING_ID = '%d'", sttpData->id);
		m_DBEngine.SetCondition(sql, condition1, str);
		
		Condition condition2;
		str.Format("pt_id = '%s'", sPTID);
		m_DBEngine.SetCondition(sql, condition2, str);
		
		
		memset(sError, '\0', MAXMSGLEN);
		
		try
		{
			nResult = m_DBEngine.XJUpdateData(EX_STTP_INFO_PT_SETTING_CFG, sql, sError);
		}
		catch (...)
		{
			WriteLog("CXJBrowserApp::SaveTempPTSetToDB, ����ʧ��");
			delete[] sError;
			sError = NULL;
			AfxMessageBox("CXJBrowserApp::SaveTempPTSetToDB, ����ʧ��");
			return FALSE;
		}
		
		if(1 != nResult)
		{
			str.Format("CXJBrowserApp::SaveTempPTSetToDB, ����ʧ��,ԭ��Ϊ%s", sError);
			WriteLog(str);
			AfxMessageBox(str);
			delete[] sError;
			sError = NULL;
			return FALSE;
		}
		
		AfxMessageBox("CXJBrowserApp::SaveTempPTSetToDB excuted success.");
	}
	
	delete[] sError;
	sError = NULL;
	
	return TRUE;
}*/

BOOL CXJBrowserApp::SaveTempPTSetToDB(CString &sPTID, MODIFY_LIST &arrModifyList)
{
	if(arrModifyList.GetSize() < 0)
		return FALSE;

	BOOL bReturn = FALSE;
	
	CString str;
	char * sError = NULL;
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);

	CMemSet* pMemset;
	pMemset = new CMemSet;

	int nResult;
	for(int i = 0; i < arrModifyList.GetSize(); i++)
	{
		STTP_DATA * sttpData = (STTP_DATA*)arrModifyList.GetAt(i);

		CString strSQL;
		strSQL.Format("UPDATE tb_pt_setting_def SET reserve3 = '%s' WHERE pt_id= '%s' AND cpu_code = '%d' AND setting_id = '%d'"
						, sttpData->str_value.c_str()
						, sPTID
						, sttpData->nCpu
						, sttpData->id);
		WriteLog(strSQL);

		//���в�ѯ
		MutiSQL_DATA MutiSql;
		bzero(&MutiSql, sizeof(MutiSQL_DATA));
		MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
		strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	
		memset(sError, '\0', MAXMSGLEN);
		try
		{
			nResult = m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
		}
		catch (...)
		{
			str.Format("CXJBrowserApp::SaveTempPTSetToDB, ��ѯʧ��");
			WriteLog(str);
			delete[] sError;
			delete pMemset;

			return FALSE;
		}

		if(pMemset != NULL && nResult == 1)
		{	
			str.Format("CXJBrowserApp::SaveTempPTSetToDB, ���³ɹ�");
			WriteLog(str);
			//AfxMessageBox(str);
		}
		else
		{
			str.Format("CXJBrowserApp::SaveTempPTSetToDB, ����ʧ��, ԭ��Ϊ%s", sError);
			WriteLog(str);
		}

		pMemset->FreeData(true);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;
}

BOOL CXJBrowserApp::RevertTempPTSetToDB(const PT_ZONE &zone, int nFlag)
{
	if (zone.PT_ID.IsEmpty())
		return FALSE;

	BOOL bReturn = FALSE;
	
	CString str;
	char * sError = NULL;
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;

	CString strSQL;
	if (nFlag != 1){
		strSQL.Format("UPDATE tb_pt_setting_def SET reserve3 = '', reserve2 = '' WHERE pt_id= '%s' AND cpu_code = '%d' AND zone = '%d'"
			, zone.PT_ID
			, zone.cpu
		, zone.code);
	}else if (1 == nFlag){
		strSQL.Format("UPDATE tb_pt_setting_def SET reserve3 = '' WHERE pt_id= '%s' AND cpu_code = '%d' AND zone = '%d'"
			, zone.PT_ID
			, zone.cpu
		, zone.code);
	}
	strSQL.Format("UPDATE tb_pt_setting_def SET reserve3 = '' WHERE pt_id= '%s' AND cpu_code = '%d' AND zone = '%d'"
		, zone.PT_ID
		, zone.cpu
		, zone.code);
	//AfxMessageBox(strSQL);

	WriteLog(strSQL);
	//AfxMessageBox(strSQL);
	
	//���в�ѯ
	MutiSQL_DATA MutiSql;
	bzero(&MutiSql, sizeof(MutiSQL_DATA));
	MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
	strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
	
	memset(sError, '\0', MAXMSGLEN);
	int nResult;
	try
	{
		nResult = m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
	}
	catch (...)
	{
		str.Format("CXJBrowserApp::RevertTempPTSetToDB, ��ѯʧ��");
		WriteLog(str);
		delete[] sError;
		delete pMemset;
		//AfxMessageBox(str);
		
		return FALSE;
	}
	
	if(pMemset != NULL && nResult == 1)
	{	
		str.Format("CXJBrowserApp::RevertTempPTSetToDB, ���³ɹ�");
		WriteLog(str);
		//AfxMessageBox(str);
	}
	else
	{
		str.Format("CXJBrowserApp::RevertTempPTSetToDB, ����ʧ��, ԭ��Ϊ%s", sError);
		WriteLog(str);
		//AfxMessageBox(str);
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;
}

BOOL CXJBrowserApp::SaveNewPTSetToDB(CString &sCPU, CString &sPTID, CTypedPtrArray<CPtrArray, PT_SETTING*> &arrSetting)
{
	if(arrSetting.GetSize() < 0)
		return FALSE;
	
	BOOL bReturn = FALSE;
	
	CString str;
	char * sError = NULL;
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	int nResult;
	for(int i = 0; i < arrSetting.GetSize(); i++)
	{
		PT_SETTING* pts = (PT_SETTING*)arrSetting.GetAt(i);

		CString strSQL;
		strSQL.Format("UPDATE tb_pt_setting_def SET reserve1 = '%s' WHERE pt_id= '%s' AND cpu_code = '%s' AND setting_id = '%s'"
			, pts->Value
			, sPTID
			, sCPU
			, pts->ID);
		WriteLog(strSQL);
		//AfxMessageBox(strSQL);

		//���в�ѯ
		MutiSQL_DATA MutiSql;
		bzero(&MutiSql, sizeof(MutiSQL_DATA));
		MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
		strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
		memset(sError, '\0', MAXMSGLEN);
		
		try
		{
			nResult = m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
		}
		catch (...)
		{
			str.Format("CXJBrowserApp::SaveNewPTSetToDB, ����ʧ��");
			WriteLog(str);
			delete[] sError;
			delete pMemset;
			//AfxMessageBox(str);
			
			return FALSE;
		}
		
		if(pMemset != NULL && nResult == 1)
		{	
			str.Format("CXJBrowserApp::SaveNewPTSetToDB, ���³ɹ�");
			WriteLog(str);
			//AfxMessageBox(str);
		}
		else
		{
			str.Format("CXJBrowserApp::SaveNewPTSetToDB, ����ʧ��, ԭ��Ϊ%s", sError);
			WriteLog(str);
			//AfxMessageBox(str);
		}
		
		pMemset->FreeData(true);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;

}

int CXJBrowserApp::GetPTSetModState(PT_ZONE &data, CString &sRecords, CString &sFlag)
{
	int nReturn = -1;
	//return nReturn;
   
	CheckKeyNameForPTSet();

	CString str;

	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//value
	Field fld0;
	m_DBEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_STRING);
	//reverse1
	Field fld1;
	m_DBEngine.SetField(sql, fld1, "reverse1", EX_STTP_DATA_TYPE_STRING);
	//reverse2
	Field fld2;
	m_DBEngine.SetField(sql, fld2, "reverse2", EX_STTP_DATA_TYPE_STRING);
	//reverse3
	Field fld3;
	m_DBEngine.SetField(sql, fld3, "reverse3", EX_STTP_DATA_TYPE_STRING);
	//note
	Field fld4;
	m_DBEngine.SetField(sql, fld4, "note", EX_STTP_DATA_TYPE_STRING);
	
	//����
	//USER_ID
	Condition cond0;
	str.Format("keyname = 'DZ_MOD_STATE'");
	m_DBEngine.SetCondition(sql, cond0, str);
	
	CMemSet* pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
		
		if (1 != nResult){
			nReturn =  -1;
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::GetPTSetModState, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;

		return -1;
	}

	// ��ѯ�ɹ�
	if(NULL != pMemset)
	{
		int nCount = pMemset->GetMemRowNum();
		
		str.Format("CXJBrowserApp::GetPTSetModState, ��ȡ��%d������", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		if(nCount > 0)
		{
            nReturn = atoi(pMemset->GetValue((UINT)0));
			data.PT_ID = pMemset->GetValue((UINT)1);
			str = pMemset->GetValue((UINT)2);

			str.TrimLeft();
			str.TrimRight();
			if (!str.IsEmpty()){
				//AfxMessageBox(str);
				vector<CString> v = SplitCString(str, ',');
				if (v.size() >= 2){
					
					data.cpu = atoi(v[0]);
					data.code = atoi(v[1]);
				}
			}

			sRecords = pMemset->GetValue((UINT)3);
			sFlag = pMemset->GetValue((UINT)4);
		}
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::GetPTSetModState, ��ѯʧ��, ԭ��Ϊ��%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		nReturn = -1;	
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
    
    return nReturn;
}

CString CXJBrowserApp::GetUserIDByState(int nState, CString &sRecords)
{
	CString sReturn = "";

	PT_ZONE zone;
	int nCurState = -1;

	sRecords.TrimLeft();
	sRecords.TrimRight();
	if (sRecords.IsEmpty()){
		nCurState = GetPTSetModState(zone, sRecords);
	}

	CString sUserID;
	CString sTime;
	vector<CString> v = SplitCString(sRecords, ";");
	if (nState > 0 && v.size() > nState - 1){
		vector<CString> v2 = SplitCString(v[nState - 1], ",");
		sUserID = v2[1];
		sTime = v2[0];

		sReturn = sUserID;
	}

	return sReturn;
}

BOOL CXJBrowserApp::SetUserLoginFlag(const CString &sUserID, const CString &sUserGroupID, CString &sFlag)
{
	if(sUserID.IsEmpty() || sUserGroupID.IsEmpty())
		return FALSE;
	
	BOOL bReturn = FALSE;
	
	CString str;
	char * sError = NULL;
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	int nResult;
	{
		CString strSQL;
		strSQL.Format("UPDATE tb_sys_user SET notes = '%s' WHERE user_id= '%s' AND group_id = '%s'"
			, sFlag
			, sUserID
			, sUserGroupID);
		WriteLog(strSQL);
		
		//���в�ѯ
		MutiSQL_DATA MutiSql;
		bzero(&MutiSql, sizeof(MutiSQL_DATA));
		MutiSql.Funtype = EX_STTP_FUN_TYPE_UPDATE;
		strncpy(MutiSql.SQL_BODY_Content, strSQL, strSQL.GetLength());
		
		memset(sError, '\0', MAXMSGLEN);
		try
		{
			nResult = m_DBEngine.XJExecuteSql(MutiSql, sError, pMemset);
		}
		catch (...)
		{
			str.Format("CXJBrowserApp::SetUserLoginFlag, ��ѯʧ��");
			WriteLog(str);
			delete[] sError;
			delete pMemset;
			
			return FALSE;
		}
		
		if(pMemset != NULL && nResult == 1)
		{	
			str.Format("CXJBrowserApp::SetUserLoginFlag, ���³ɹ�");
			WriteLog(str);
			//AfxMessageBox(str);
		}
		else
		{
			str.Format("CXJBrowserApp::SetUserLoginFlag, ����ʧ��, ԭ��Ϊ%s", sError);
			WriteLog(str);
		}
		
		pMemset->FreeData(true);
	}
	
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	return TRUE;
}

BOOL CXJBrowserApp::RevertPTSetModState(int nRevertStateID, int nFlag)
{
	BOOL bReturn = FALSE;

	PT_ZONE zone;
	CString sRecords;
	int nCurrentState = GetPTSetModState(zone, sRecords);

	if (nCurrentState < nRevertStateID){
		//AfxMessageBox("Cannot Revert PTSet Modification State to this State.");
		return FALSE;
	}else if (nCurrentState == nRevertStateID){
		return TRUE;
	}

	if (1 == nRevertStateID)
		RevertTempPTSetToDB(zone, 1);
	else if (0 == nRevertStateID)
		RevertTempPTSetToDB(zone, 0);

	vector<CString> v = SplitCString(sRecords, ";");
	CString sRevertRecords;
	int nRows = min(v.size(), nRevertStateID);
	for (int i = 0; i < nRows; i++){
		if (i != 0)
			sRecords +=";";
		sRevertRecords += v[i];
	}
	//AfxMessageBox(sRevertRecords);

	SetRevertModifyValueFlag(nFlag);

	return NextPTSetModState(nRevertStateID, zone, CString(), sRevertRecords);
}

BOOL CXJBrowserApp::SetRevertModifyValueFlag(int nFlag)
{
	BOOL bReturn = FALSE;
	
	//CheckKeyNameForPTSet();
	
	PT_ZONE curZone;
	CString sRecords;
	int nCurStateID = GetPTSetModState(curZone, sRecords);
	
	CString str;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	//str.Format(" current PT_SET_MOD_STATE = %d \n next PT_SET_MOD_STATE: %d", nCurStateID, nNextStateID);
	//AfxMessageBox(str);
	
	// �齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	// note
	Field fld0;
	str.Format("%d", nFlag);
	m_DBEngine.SetField(sql, fld0, "note", EX_STTP_DATA_TYPE_STRING, str);
	
	//����
	//keyname
	Condition cond0;
	str.Format("keyname = 'DZ_MOD_STATE'");
	m_DBEngine.SetCondition(sql, cond0, str);
	
	sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJUpdateData(EX_STTP_INFO_TBSYSCONFIG, sql, sError);
		
		if(1 == nResult){
			bReturn = TRUE;
		}else{
			str.Format("CXJBrowserApp::SetRevertModifyValueFlag, ����ʧ��, ԭ��Ϊ��%s", sError);
			WriteLog(str);
			AfxMessageBox(str);
			
			bReturn = FALSE;
		}
	}
	catch (...)
	{
		WriteLog("CXJBrowserApp::SetRevertModifyValueFlag, ����ʧ��");
		AfxMessageBox(sError);
		delete[] sError;
		
		return FALSE;
	}
	
	delete[] sError;
	sError = NULL;
	
	return bReturn;

}

/*************************************************************
 �� �� ����GetSVGView()
 ���ܸ�Ҫ���õ�������ͼָ��
 �� �� ֵ: ������ͼָ��
**************************************************************/
//##ModelId=49B87B7E0128
CXJBrowserView * CXJBrowserApp::GetSVGView()
{
	CXJBrowserDoc * pDoc = GetCurDocument();

	CXJBrowserView * pSvgView = NULL;

	POSITION pos = pDoc->GetFirstViewPosition();
	CView * pView;
	while(pos!=NULL)
	{
		pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CXJBrowserView)))//�����ͨѶ״̬ͼ
		{
			pSvgView = (CXJBrowserView *)pView;
			break;
		}			
	}

	return pSvgView;
}


CViewCommMap * CXJBrowserApp::GetCommView()
{
	CXJBrowserDoc * pDoc = GetCurDocument();
	CViewCommMap * pCommView = NULL;
	
	POSITION pos = pDoc->GetFirstViewPosition();
	CView * pView;
	while(pos!=NULL)
	{
		pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CViewCommMap)))//�����ͨѶ״̬ͼ
		{
			pCommView = (CViewCommMap *)pView;
			break;
		}			
	}
	
	return pCommView;
}


/*************************************************************
 �� �� ����LoadDownDir()
 ���ܸ�Ҫ�������ݿ��ж�ȡ�ļ�����·��, ������m_sDownDir
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B7E0148
void CXJBrowserApp::LoadDownDir()
{
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();

	//��ѯcomtrade_path
	Field Field1;
	m_DBEngine.SetField(sql, Field1, "Value", EX_STTP_DATA_TYPE_STRING);

	Condition con1;
	m_DBEngine.SetCondition(sql, con1, "KeyName='DownDir'");
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::LoadDownDir, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//��ѯ�ɹ�
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			m_sDownDir = pMemset->GetValue((UINT)0);
			CString str;
			str.Format("ϵͳ����·��Ϊ:%s", m_sDownDir);
			WriteLog(str, XJ_LOG_LV2);
		}
		else
		{
			WriteLog("�Ҳ���ϵͳ����·��", XJ_LOG_LV2);
		}
		
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::LoadDownDir, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;

	if(m_sDownDir != "")
	{
		//���п��ܵ�/������"\\"
		m_sDownDir.Replace("/", "\\");
		if(m_sDownDir.Right(1)!="\\")
			m_sDownDir += "\\";
	}
}

/*************************************************************
 �� �� ����GetDownDir()
 ���ܸ�Ҫ��ȡ��ϵͳ����·��
 �� �� ֵ: ϵͳ����·��
**************************************************************/
//##ModelId=49B87B7E0157
CString CXJBrowserApp::GetDownDir()
{
	if(m_sDownDir == "")
	{
		if(g_UseLocalIp == 0)
			LoadDownDir();
		else
		{
			m_sDownDir = g_LocalIp;
			if(m_sDownDir != "")
			{
				//���п��ܵ�/������"\\"
				m_sDownDir.Replace("/", "\\");
				if(m_sDownDir.Right(1)!="\\")
					m_sDownDir.Format("%s\\", m_sDownDir);	
			}
		}
	}
	return m_sDownDir;
}

/*************************************************************
 �� �� ����ShowDevice()
 ���ܸ�Ҫ����ʾĳ�豸���ڳ�վ��ͼ�λ���, ��ʹ���豸����
 �� �� ֵ: void
 ��    ����param1 �豸ָ��
**************************************************************/
//##ModelId=49B87B7E0168
void CXJBrowserApp::ShowDevice( CDeviceObj* pObj, BOOL bMoveFirst /*= TRUE*/ )
{
	WriteLog(" CXJBrowserApp::ShowDevice, ��ʼ");
	if(pObj == NULL)
		return;
	//�豸Ϊ����ʱ������
	if(pObj->m_nType == TYPE_NET)
		return;
	//�õ�SVG����ָ��
	CXJBrowserView * pView = GetSVGView();
	if(pView == NULL)
		return;

	CStationObj * pStation = NULL;
	if(pObj->m_nType == TYPE_STATION)
	{
		pStation = (CStationObj*)pObj;
	}
	else
	{
		WriteLog(" CXJBrowserApp::ShowDevice, ���ҳ�վ");
		//���ҳ�վ
		pStation = (CStationObj*)GetDataEngine()->FindDevice(pObj->m_sStationID, TYPE_STATION);
		if(pStation == NULL)
			return;
		WriteLog(" CXJBrowserApp::ShowDevice, ���ҳ�վ����");
	}
	
	
	if(pObj->m_nType == TYPE_BAY)
	{
		WriteLog(" CXJBrowserApp::ShowDevice, �豸����Ϊ���");
		//�رվɵ�,���µ�
		pView->CloseCurrentView();
		pView->OpenBayView((CBayObj*)pObj);
		if(bMoveFirst)
			pView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
	}
	else
	{
		WriteLog(" CXJBrowserApp::ShowDevice, ��ͼ�λ���");
		//��ͼ�λ���
		if(pStation->m_sID == pView->m_sStationID)
		{
			WriteLog(" CXJBrowserApp::ShowDevice, �����Ѵ�,�ѻ����Ƶ�ǰ��");
			//�����Ѵ�,�ѻ����Ƶ�ǰ��
			if(bMoveFirst)
				pView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
		else
		{
			WriteLog(" CXJBrowserApp::ShowDevice, ����ѡ��ĳ�վ�뵱ǰ�򿪵�ͼ�β���ͬһ��վ");
			//����ѡ��ĳ�վ�뵱ǰ�򿪵�ͼ�β���ͬһ��վ
			//�رվɵ�,���µ�
			pView->CloseCurrentView();
			pView->OpenStationView(pStation->m_sID);
			if(bMoveFirst)
				pView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
		}
	}

	//�ǳ�վʱ�豸����
	if(pObj->m_nType != TYPE_STATION && pObj->m_nType != TYPE_BAY)
	{
		WriteLog("CXJBrowserApp::ShowDevice, �ǳ�վʱ�豸����");
		pView->CenterDevice(pObj);
	}
}

/*************************************************************
 �� �� ����LocateInTree()
 ���ܸ�Ҫ�����豸���ж�λ��վ
 �� �� ֵ: void
 ��    ����param1	ָ����վ
		   Param2
**************************************************************/
//##ModelId=49B87B7E0177
void CXJBrowserApp::LocateInTree( CStationObj* pObj )
{
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return;
	pFrame->m_pDeviceView->LocateInTree(pObj);
}

/*************************************************************
 �� �� ����ShowEventProp()
 ���ܸ�Ҫ����������ܴ��ڵķ���, ��ʾ�¼����ԶԻ���
 �� �� ֵ: 
 ��    ����param1	�¼�������ָ��
		   Param2	�Ƿ��Ǳ�������ҳ��򿪵ĶԻ���
**************************************************************/
//##ModelId=49B87B7E0186
void CXJBrowserApp::ShowEventProp( CXJEventManager* pEventManager, BOOL bPT)
{
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return;
	pFrame->ShowEventProp(pEventManager, bPT);
}

/*************************************************************
 �� �� ����EndOwnerThread()
 ���ܸ�Ҫ��ֹͣ�Լ���������߳�
 �� �� ֵ: void
**************************************************************/
//##ModelId=49B87B7E01A5
void CXJBrowserApp::EndOwnerThread()
{
	SetEvent(m_hPostEvent);
	SetEvent(m_hReceiveEvent);
	//�˳���ȡ�����߳�
	if(m_pClearConfigThread != NULL)
	{
		WriteLog("��ʼ�˳���������豸�����߳�", XJ_LOG_LV3);
		//�̻߳�û�˳�ȥ
		m_bExitClearConfig = TRUE;	//�����˳���־
		DWORD dw = WaitForSingleObject(m_pClearConfigThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pClearConfigThread->m_hThread,NULL);
			WriteLog("��������豸�����߳��޷������˳�,ǿ�ƽ���", XJ_LOG_LV3);
			break;
		}
		delete m_pClearConfigThread;
		m_pClearConfigThread = NULL;
		WriteLog("�˳���������豸�����߳����", XJ_LOG_LV3);
	}
	//�˳����ձ����߳�
	if(m_pReceiveMsgThread != NULL)
	{
		WriteLog("��ʼ�˳����ձ����߳�", XJ_LOG_LV3);
		//�̻߳�û�˳�ȥ
		m_bExitReceiveMsg = TRUE; //�����˳���־
		DWORD dw = WaitForSingleObject(m_pReceiveMsgThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pReceiveMsgThread->m_hThread,NULL);
			WriteLog("���ձ����߳��޷������˳�,ǿ�ƽ���", XJ_LOG_LV3);
			break;
		}
		delete m_pReceiveMsgThread;
		m_pReceiveMsgThread = NULL;
		WriteLog("�˳����ձ����߳����", XJ_LOG_LV3);
	}

	//�˳��ַ������߳�
	if(m_pPostMsgThread != NULL)
	{
		WriteLog("��ʼ�˳��ַ������߳�", XJ_LOG_LV3);
		//�̻߳�û�˳�ȥ
		m_bExitReceiveMsg = TRUE; //�����˳���־
		DWORD dw = WaitForSingleObject(m_pPostMsgThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pPostMsgThread->m_hThread,NULL);
			WriteLog("�ַ������߳��޷������˳�,ǿ�ƽ���", XJ_LOG_LV3);
			break;
		}
		delete m_pPostMsgThread;
		m_pPostMsgThread = NULL;
		WriteLog("�˳��ַ������߳����", XJ_LOG_LV3);
	}

	//�˳��ַ������߳�
	if(m_pPushMsgThread != NULL)
	{
		WriteLog("��ʼ�˳��Զ����ͱ����߳�", XJ_LOG_LV3);
		//�̻߳�û�˳�ȥ
		g_PushMsgProcess.Quit();
		m_bExitReceiveMsg = TRUE; //�����˳���־
		DWORD dw = WaitForSingleObject(m_pPushMsgThread->m_hThread, INFINITE);
		switch(dw)
		{
		case WAIT_TIMEOUT:
		case WAIT_FAILED:
			TerminateThread(m_pPushMsgThread->m_hThread,NULL);
			WriteLog("�Զ����ͱ����߳��޷������˳�,ǿ�ƽ���", XJ_LOG_LV3);
			break;
		}
		delete m_pPushMsgThread;
		m_pPushMsgThread = NULL;
		WriteLog("�˳��Զ����ͱ����߳����", XJ_LOG_LV3);
	}

	RemoveAllSTTPData();
}

/*************************************************************
 �� �� ����LoadSysConfig()
 ���ܸ�Ҫ���������ļ�����ϵͳ����
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01B7
void CXJBrowserApp::LoadSysConfig()
{
	CString strIniFile = g_SysConfig;

	g_ConnectServerButton = GetPrivateProfileInt("ToolBarConfig", "ConnectServerButton", 0, strIniFile);
	g_DeviceTreeButton = GetPrivateProfileInt("ToolBarConfig", "DeviceTreeButton", 1, strIniFile);
	g_LogButton = GetPrivateProfileInt("ToolBarConfig", "LogButton", 1, strIniFile);
	g_HistoryQueryButton = GetPrivateProfileInt("ToolBarConfig", "HistoryQueryButton", 1, strIniFile);
	g_AlarmCountButton = GetPrivateProfileInt("ToolBarConfig", "AlarmCountButton", 0, strIniFile);
	g_SystemSettingButton = GetPrivateProfileInt("ToolBarConfig", "SystemSettingButton", 1, strIniFile);
	g_CallAllButton = GetPrivateProfileInt("ToolBarConfig", "CallAllButton", 1, strIniFile);
	g_CommonFileButton = GetPrivateProfileInt("ToolBarConfig", "CommonFileButton", 1, strIniFile);
    g_ExitButton = GetPrivateProfileInt("ToolBarConfig", "ExitButton", 1, strIniFile);

	////��������ӽ�ɫ 0:��վ 1:��վ
	g_role = GetPrivateProfileInt("SysConfig", "role", 0, strIniFile);
	//���SVGͼ�����豸ʱ�����豸��ʽ 0:103��� 1:uri
    g_FindDeviceWay = GetPrivateProfileInt("SysConfig", "FindDeviceWay", 0, strIniFile);; 
	//�Ƿ���ʾ�������
	g_ShowOutputWnd = GetPrivateProfileInt("SysConfig", "ShowOutputWnd", 1, strIniFile);
	//�Ƿ���ʾ�豸������
	g_ShowDeviceTreeWnd = GetPrivateProfileInt("SysConfig", "ShowDeviceTreeWnd", 1, strIniFile);
	//�豸������
	g_DeviceTreeType = GetPrivateProfileInt("SysConfig", "DeviceTreeType", 0, strIniFile);
	//�Ƿ������־����־�ļ�, 0Ϊ�����, >0Ϊ���
	g_PutOutToLogFile = GetPrivateProfileInt("SysConfig", "OutputToLogFile", 0, strIniFile);
	//�Ƿ������־����־�������, 0Ϊ�����, >0Ϊ���
	g_OutPutToWnd = GetPrivateProfileInt("SysConfig", "OutputToWnd", 1, strIniFile);
	////�����־�ĵȼ�, 1Ϊ��Ҫ����־�����; 2Ϊ��ͨ��־���; 3Ϊ������־�����(����);
	g_LogLevel = GetPrivateProfileInt("SysConfig", "LogLevel", 1, strIniFile);
	//��������ʱĬ�ϴ򿪵ĳ�վ
	GetPrivateProfileString("SysConfig", "DefaultStation", "", g_DefaultStation.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	
	//�б���и�
	g_ListItemHeight = GetPrivateProfileInt("SysConfig", "ListItemHeight", 20, strIniFile);
	//����洢���û���
	GetPrivateProfileString("SysConfig", "NetworkUserName", "", m_sNetworkUser.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	//����洢������
	GetPrivateProfileString("SysConfig", "NetworkPassword", "", m_sNetworkPassword.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	//���ߺ��ض����ݿ��ʱ��
	g_DisconnectTime = GetPrivateProfileInt("SysConfig", "DisconnectTime", 20, strIniFile);
    //��վͨѶ״̬����¼��

	g_style  = GetPrivateProfileInt("SysConfig", "Style", 0, strIniFile);
	
	g_SecType = GetPrivateProfileInt("SysConfig", "LoadSecType", -1, strIniFile);
	GetPrivateProfileString("SysConfig", "AppName", "IPOFAS-���������", g_sAppName.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	g_PostTime = GetPrivateProfileInt("SysConfig", "PostTime", 20, strIniFile);

	GetPrivateProfileString("Style", "StyleName", "", g_strStyleName.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);

	g_UseXJWave = GetPrivateProfileInt("SysConfig", "UseXJWave", 1, strIniFile);
	if(g_UseXJWave == 1)
		g_strOscName = "XJWave.exe";
	else
		g_strOscName = "STWave.exe";

	//�Ƿ�ʹ�ñ������õ�IP, 0��1�ǡ�Ϊ0ʱ�����ݿ��ж�ȡ����,Ĭ��Ϊ0 
	g_UseLocalIp = GetPrivateProfileInt("SysConfig", "UseLocalIp", 0, strIniFile);
	//�������õ�IP
	GetPrivateProfileString("SysConfig", "LocalIp", "", g_LocalIp.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	
	//ѡ������ݿ���
	g_LoadSttpDll = GetPrivateProfileInt("SysConfig","LoadSttpDll",0,strIniFile);

	//���ϱ���
	g_UseColorFaultReport = GetPrivateProfileInt("FaultReportConfig", "ColorFault", 0, strIniFile);
	g_FaultReportFilter = GetPrivateProfileInt("FaultReportConfig", "FaultReportFilter", 1, strIniFile);
	g_FaultFilterIII = GetPrivateProfileInt("FaultReportConfig", "FaultFilterIII", 0, strIniFile);
	g_FaultActionTime = GetPrivateProfileInt("FaultReportConfig", "FaultActionTime", 0, strIniFile);
	g_FaultShowWave = GetPrivateProfileInt("FaultReportConfig", "FaultShowWave", 0, strIniFile);

	g_LockTime = GetPrivateProfileInt("SysConfig", "LockTime", 5, strIniFile);
	CString sTime;
	char a[15];
	for(int i = 0; i < 15; i++)
		a[i] = '\0';
	GetPrivateProfileString("SysConfig", "LastFailTime", "19710101010101", a, 15, strIniFile);
	CString str = a;
	g_LastFailTime = StringToTime14(str);
	
	g_LoginFailTimes = GetPrivateProfileInt("SysConfig", "LoginFailTimes", 0, strIniFile);
	
	CTime tmCur = CTime::GetCurrentTime();
	CTimeSpan tsTime = tmCur - g_LastFailTime;
	if(tsTime.GetTotalSeconds() > g_LockTime*60)
	{
		//�������
		g_LoginFailTimes = 0;
	}
	
	g_UILockTime = GetPrivateProfileInt("SysConfig", "UILockTime", 0, strIniFile);

	GetPrivateProfileString("SysConfig", "StationCommSvg", STATIONCOMM_File, g_strStationSvg.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);

	XMLParser xmlParser;
	if (xmlParser.CheckFileExist(g_StyleConfig))
	{
		xmlParser.ParserXmlFile(g_StyleConfig);
		if (xmlParser.m_list.GetCount() == 0)
		{
			xmlParser.SaveNewStyleSetting(g_StyleConfig);
		}
		StyleSetting* pStyle = NULL;
		for(POSITION pos = xmlParser.m_list.GetHeadPosition(); pos != NULL;)
		{
			pStyle = (StyleSetting*)xmlParser.m_list.GetNext(pos);
			if (pStyle != NULL && !pStyle->m_sName.CompareNoCase(g_strStyleName))
			{
				g_strToolBarBK = pStyle->m_sToolBarBK;
				g_strStartBmp = pStyle->m_sStartBmp;
				g_TreeTextColor = atoi(pStyle->m_sDeviceTreeTextColor);
				g_TreeBkColor = atoi(pStyle->m_sDeviceTreeBk);
				g_TitleLightColor = atoi(pStyle->m_sTitleLightColor);
				g_TitleDeepColor = atoi(pStyle->m_sTitleDeepColor);
				g_ListSpaceColor4 = atoi(pStyle->m_sListSpaceColor);
				g_ListHeadColor = atoi(pStyle->m_sListHeadColor);
				break;
			}
		}
	}
	else
	{
		g_style = 0;
	}


	/*
	GetPrivateProfileString("Style", "ToolBarBK", "", g_strToolBarBK.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	GetPrivateProfileString("Style", "StartBmp", "", g_strStartBmp.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	g_ListSpaceColor = GetPrivateProfileInt("Style", "ListSpaceColor", 0, strIniFile);
	g_ListHeadColor = GetPrivateProfileInt("Style", "ListHeadColor", 0, strIniFile);
	g_TreeBkColor = GetPrivateProfileInt("Style", "DeviceTreeBk", 0, strIniFile);
	g_TreeTextColor = GetPrivateProfileInt("Style", "DeviceTreeTextColor", 0, strIniFile);
	g_TitleLightColor = GetPrivateProfileInt("Style", "TitleLightColor", 0, strIniFile);
	g_TitleDeepColor = GetPrivateProfileInt("Style", "TitleDeepColor", 0, strIniFile);
	*/

	g_RemindMaxCount = GetPrivateProfileInt("Remind", "MaxCount", 0, strIniFile);
	g_RemindTime = GetPrivateProfileInt("Remind", "RetainTime", 2, strIniFile);

	g_MarkDownOscPT = GetPrivateProfileInt("SysConfig", "MarkDownOscPT", 0, strIniFile);

	g_AutoStationCommMap = GetPrivateProfileInt("SysConfig", "AutoStationCommMap", 1, strIniFile);

	GetPrivateProfileString("SysConfig", "Language", "zh_CN", g_Language.GetBuffer(MAX_PATH), MAX_PATH, strIniFile);
	
}

/*************************************************************
 �� �� ����SaveSysConfig()
 ���ܸ�Ҫ������ϵͳ���õ������ļ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01C5
void CXJBrowserApp::SaveSysConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	//�Ƿ���ʾ�������
	str.Format("%d", g_ShowOutputWnd);
	WritePrivateProfileString("SysConfig", "ShowOutputWnd", str, strIniFile);
	//�Ƿ���ʾ�豸������
	str.Format("%d", g_ShowDeviceTreeWnd);
	WritePrivateProfileString("SysConfig", "ShowDeviceTreeWnd", str, strIniFile);
	//�豸������
	str.Format("%d", g_DeviceTreeType);
	WritePrivateProfileString("SysConfig", "DeviceTreeType", str, strIniFile);
	//�Ƿ������־����־�ļ�, 0Ϊ�����, >0Ϊ���
	str.Format("%d", g_PutOutToLogFile);
	WritePrivateProfileString("SysConfig", "OutputToLogFile", str, strIniFile);
	//�Ƿ������־����־�������, 0Ϊ�����, >0Ϊ���
	str.Format("%d", g_OutPutToWnd);
	WritePrivateProfileString("SysConfig", "OutputToWnd", str, strIniFile);
	//�����־�ĵȼ�, 1Ϊ��Ҫ����־�����; 2Ϊ��ͨ��־���; 3Ϊ������־�����(����);
	str.Format("%d", g_LogLevel);
	WritePrivateProfileString("SysConfig", "LogLevel", str, strIniFile);
	//��������ʱĬ�ϴ򿪵ĳ�վ
	WritePrivateProfileString("SysConfig", "DefaultStation", g_DefaultStation, strIniFile);
	//�б���и�
	str.Format("%d", g_ListItemHeight);
	WritePrivateProfileString("SysConfig", "ListItemHeight", str, strIniFile);
	//����洢���û���
	WritePrivateProfileString("SysConfig", "NetworkUserName", m_sNetworkUser, strIniFile);
	//����洢������
	WritePrivateProfileString("SysConfig", "NetworkPassword", m_sNetworkPassword, strIniFile);

	//¼���������
	str.Format("%d", g_UseXJWave);
	WritePrivateProfileString("SysConfig", "UseXJWave", str, strIniFile);

	str.Format("%d", g_SecType);
	WritePrivateProfileString("SysConfig", "LoadSecType", str, strIniFile);
	WritePrivateProfileString("SysConfig", "AppName", g_sAppName, strIniFile);
	str.Format("%d", g_PostTime);
	WritePrivateProfileString("SysConfig", "PostTime", str, strIniFile);

	WritePrivateProfileString("SysConfig", "Language", g_Language, strIniFile);

	//�Ƿ�ʹ�ñ������õ�IP, 0��1�ǡ�Ϊ0ʱ�����ݿ��ж�ȡ����,Ĭ��Ϊ0
	str.Format("%d", g_UseLocalIp);
	WritePrivateProfileString("SysConfig", "UseLocalIp", str, strIniFile);
	//�������õ�IP
	WritePrivateProfileString("SysConfig", "LocalIp", g_LocalIp, strIniFile);
	//��̬������
	str.Format("%d", g_LoadSttpDll);
	WritePrivateProfileString("SysConfig", "LoadSttpDll", str, strIniFile);
	//����������
	str.Format("%d", g_ConnectServerButton);
	WritePrivateProfileString("ToolBarConfig", "ConnectServerButton", str, strIniFile);
	str.Format("%d", g_DeviceTreeButton);
	WritePrivateProfileString("ToolBarConfig", "DeviceTreeButton", str, strIniFile);
	str.Format("%d", g_LogButton);
	WritePrivateProfileString("ToolBarConfig", "LogButton", str, strIniFile);
	str.Format("%d", g_HistoryQueryButton);
	WritePrivateProfileString("ToolBarConfig", "HistoryQueryButton", str, strIniFile);
	str.Format("%d", g_AlarmCountButton);
	WritePrivateProfileString("ToolBarConfig", "AlarmCountButton", str, strIniFile);
	str.Format("%d", g_SystemSettingButton);
	WritePrivateProfileString("ToolBarConfig", "SystemSettingButton", str, strIniFile);
	str.Format("%d", g_CallAllButton);
	WritePrivateProfileString("ToolBarConfig", "CallAllButton", str, strIniFile);
	str.Format("%d", g_CommonFileButton);
	WritePrivateProfileString("ToolBarConfig", "CommonFileButton", str, strIniFile);
	str.Format("%d", g_ExitButton);
	WritePrivateProfileString("ToolBarConfig", "ExitButton", str, strIniFile);


	//���ϱ���
	str.Format("%d", g_UseColorFaultReport);
	WritePrivateProfileString("FaultReportConfig", "ColorFault", str, strIniFile);
	
	str.Format("%d", g_FaultReportFilter);
	WritePrivateProfileString("FaultReportConfig", "FaultReportFilter", str, strIniFile);

	str.Format("%d", g_FaultFilterIII);
	WritePrivateProfileString("FaultReportConfig", "FaultFilterIII", str, strIniFile);
	
	str.Format("%d", g_FaultActionTime);
	WritePrivateProfileString("FaultReportConfig", "FaultActionTime", str, strIniFile);

	str.Format("%d",g_FaultShowWave);
	WritePrivateProfileString("FaultReportConfig", "FaultShowWave", str, strIniFile);

	str.Format("%d", g_LockTime);
	WritePrivateProfileString("SysConfig", "LockTime", str, strIniFile);
	
	str = g_LastFailTime.Format("%Y%m%d%H%M%S");
	WritePrivateProfileString("SysConfig", "LastFailTime", str, strIniFile);
	
	str.Format("%d", g_LoginFailTimes);
	WritePrivateProfileString("SysConfig", "LoginFailTimes", str, strIniFile);
	
	str.Format("%d", g_UILockTime);
	WritePrivateProfileString("SysConfig", "UILockTime", str, strIniFile);

	WritePrivateProfileString("SysConfig", "StationCommSvg", g_strStationSvg, strIniFile);

	str.Format("%d", g_MarkDownOscPT);
	WritePrivateProfileString("SysConfig", "MarkDownOscPT", str, strIniFile);

	str.Format("%d", g_RemindMaxCount);
	WritePrivateProfileString("Remind", "MaxCount", str, strIniFile);
	str.Format("%d", g_RemindTime);
	WritePrivateProfileString("Remind", "RetainTime", str, strIniFile);

	str.Format("%d", g_AutoStationCommMap);
	WritePrivateProfileString("SysConfig", "AutoStationCommMap", str, strIniFile);

}

//##ModelId=49B87B7D0271
CXJBrowserApp::~CXJBrowserApp()
{
	DeleteCriticalSection(&m_CriticalSection);
	// �ر��ź������ 
	if(hSem != NULL)
	{
		ReleaseSemaphore(hSem, 1, NULL);
		CloseHandle(hSem);
		hSem = NULL;
	}
	m_listFullData.RemoveAll();
	CloseHandle(m_hPostEvent);
	CloseHandle(m_hReceiveEvent);
}

/*************************************************************
 �� �� ����WriteLogEx()
 ���ܸ�Ҫ��д��־�ļ�����־�ļ�, ��Ϊ�����߳���д��־����ڵȴ��̹߳ر�ʱ���������� 
 �� �� ֵ: 
 ��    ����param1	��־����
		   Param2	��־����ȼ�
**************************************************************/
//##ModelId=49B87B7E008C
void CXJBrowserApp::WriteLogEx( CString sLog, int nLV /*= XJ_LOG_LV2*/ )
{
	m_LogMgr.WriteLogEx(sLog, nLV);
}

//##ModelId=49B87B7E01D4
void CXJBrowserApp::LoadEventConfig()
{
	CString strIniFile = g_SysConfig;

	//I���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
	g_ShowEventI = GetPrivateProfileInt("EventConfig", "EventIShow", 1, strIniFile);
	//I���¼��Ƿ񱨾�. 0-������. 1-����
	g_AlarmEventI = GetPrivateProfileInt("EventConfig", "EventIAlarm", 1, strIniFile);		
	//I���¼���ʾ��ɫ
	g_colEventI = GetPrivateProfileInt("EventConfig", "EventIColor", RGB(255,0,0), strIniFile);

	//II���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
	g_ShowEventII = GetPrivateProfileInt("EventConfig", "EventIIShow", 1, strIniFile);
	//II���¼��Ƿ񱨾�. 0-������. 1-����
	g_AlarmEventII = GetPrivateProfileInt("EventConfig", "EventIIAlarm", 1, strIniFile);
	//II���¼���ʾ��ɫ
	g_colEventII = GetPrivateProfileInt("EventConfig", "EventIIColor", RGB(0, 255, 255), strIniFile);

	//III���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
	g_ShowEventIII = GetPrivateProfileInt("EventConfig", "EventIIIShow", 1, strIniFile);
	//III���¼��Ƿ񱨾�. 0-������. 1-����
	g_AlarmEventIII = GetPrivateProfileInt("EventConfig", "EventIIIAlarm", 1, strIniFile);
	//III���¼���ʾ��ɫ
	g_colEventIII = GetPrivateProfileInt("EventConfig", "EventIIIColor", RGB(214, 223, 247), strIniFile);

	//δȷ���¼���ʾ
	g_colNotAcked = GetPrivateProfileInt("EventConfig", "EventNotAcked", RGB(0, 0, 255), strIniFile);
	//��ȷ���¼���ʾ
	g_colAcked = GetPrivateProfileInt("EventConfig", "EventAcked", RGB(0, 0, 0), strIniFile);

	//�յ�װ�õĶ�����澯ʱ,�Ƿ���˸��װ��ͼ��.0-��,1-��
	g_BlinkDevice = GetPrivateProfileInt("EventConfig", "BlinkDevice", 1, strIniFile);
	//�յ�װ�õĶ�����澯ʱ,�Ƿ��������վ��ͼ�λ����Ƶ���ǰ��.0-��,1-��
	g_MustShowDevice = GetPrivateProfileInt("EventConfig", "MustShowDevice", 1, strIniFile);
	//�����豸��˸ʱ,�Ƿ��������һ���豸һ����˸.0-��,1-��
	g_BlinkOwner = GetPrivateProfileInt("EventConfig", "BlinkOwner", 1, strIniFile);
	//�յ�װ�õĶ�����澯ʱ, �Ƿ񵯳��¼����ԶԻ���. 0-��, 1-��
	g_ShowEventDlg = GetPrivateProfileInt("EventConfig", "ShowEventDlg", 0, strIniFile);
	//�Ƿ�ʹ����������. 0-��,1-��
	g_UseAlarmSound = GetPrivateProfileInt("EventConfig", "UseAlarmSound", 0, strIniFile);	
	//������������ʱ��. >=0����. ��λ.��
	g_SoundTime = GetPrivateProfileInt("EventConfig", "AlarmSoundTime", 60, strIniFile);
	g_FlashTime = GetPrivateProfileInt("EventConfig", "FlashTime", 120, strIniFile);
	//�յ�װ�õĶ�����澯ʱ,�Ƿ���˸��װ��������վͼ��.0-��1-��.����Ϊ1����MustShowDeviceΪ1ʱ����վͼ�λ���ǰ��
	g_BlinkStation = GetPrivateProfileInt("EventConfig", "BlinkStation", 0, strIniFile);
	g_AutoChangeMap = GetPrivateProfileInt("EventConfig", "AutoChangeMap", 0, strIniFile);
}

/*************************************************************
 �� �� ����SaveEventConfig()
 ���ܸ�Ҫ�������¼����õ������ļ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01D5
void CXJBrowserApp::SaveEventConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;	

	//I���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowEventI);
	WritePrivateProfileString("EventConfig", "EventIShow", str, strIniFile);
	//I���¼��Ƿ񱨾�. 0-������. 1-����
	str.Format("%d", g_AlarmEventI);
	WritePrivateProfileString("EventConfig", "EventIAlarm", str, strIniFile);
	//I���¼���ʾ��ɫ
	str.Format("%d", g_colEventI);
	WritePrivateProfileString("EventConfig", "EventIColor", str, strIniFile);

	//II���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowEventII);
	WritePrivateProfileString("EventConfig", "EventIIShow", str, strIniFile);
	//II���¼��Ƿ񱨾�. 0-������. 1-����
	str.Format("%d", g_AlarmEventII);
	WritePrivateProfileString("EventConfig", "EventIIAlarm", str, strIniFile);
	//II���¼���ʾ��ɫ
	str.Format("%d", g_colEventII);
	WritePrivateProfileString("EventConfig", "EventIIColor", str, strIniFile);

	//III���¼��Ƿ���ʾ. 0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowEventIII);
	WritePrivateProfileString("EventConfig", "EventIIIShow", str, strIniFile);
	//III���¼��Ƿ񱨾�. 0-������. 1-����
	str.Format("%d", g_AlarmEventIII);
	WritePrivateProfileString("EventConfig", "EventIIIAlarm", str, strIniFile);
	//III���¼���ʾ��ɫ
	str.Format("%d", g_colEventIII);
	WritePrivateProfileString("EventConfig", "EventIIIColor", str, strIniFile);

	//δȷ���¼���ʾ
	str.Format("%d", g_colNotAcked);
	WritePrivateProfileString("EventConfig", "EventNotAcked", str, strIniFile);
	//��ȷ���¼���ʾ
	str.Format("%d", g_colAcked);
	WritePrivateProfileString("EventConfig", "EventAcked", str, strIniFile);

	//�յ�װ�õĶ�����澯ʱ,�Ƿ���˸��װ��ͼ��.0-��,1-��
	str.Format("%d", g_BlinkDevice);
	WritePrivateProfileString("EventConfig", "BlinkDevice", str, strIniFile);
	//�յ�װ�õĶ�����澯ʱ,�Ƿ��������վ��ͼ�λ����Ƶ���ǰ��.0-��,1-��
	str.Format("%d", g_MustShowDevice);
	WritePrivateProfileString("EventConfig", "MustShowDevice", str, strIniFile);
	//�����豸��˸ʱ,�Ƿ��������һ���豸һ����˸.0-��,1-��
	str.Format("%d", g_BlinkOwner);
	WritePrivateProfileString("EventConfig", "BlinkOwner", str, strIniFile);
	//�յ�װ�õĶ�����澯ʱ, �Ƿ񵯳��¼����ԶԻ���
	str.Format("%d", g_ShowEventDlg);
	WritePrivateProfileString("EventConfig", "ShowEventDlg", str, strIniFile);

	//�Ƿ�ʹ����������. 0-��,1-��
	str.Format("%d", g_UseAlarmSound);
	WritePrivateProfileString("EventConfig", "UseAlarmSound", str, strIniFile);	
	//������������ʱ��. >=0����. ��λ.��
	str.Format("%d", g_SoundTime);
	WritePrivateProfileString("EventConfig", "AlarmSoundTime", str, strIniFile);
	str.Format("%d", g_FlashTime);
	WritePrivateProfileString("EventConfig", "FlashTime", str, strIniFile);
	str.Format("%d", g_BlinkDevice);
	WritePrivateProfileString("EventConfig", "BlinkOwner", str, strIniFile);
	str.Format("%d", g_BlinkStation);
	WritePrivateProfileString("EventConfig", "BlinkStation", str, strIniFile);
	str.Format("%d", g_AutoChangeMap);
	WritePrivateProfileString("EventConfig", "AutoChangeMap", str, strIniFile);
}

/*************************************************************
 �� �� ����LoadCommStatus()
 ���ܸ�Ҫ���������ļ���������վͨѶ״̬ҳ������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01E4
void CXJBrowserApp::LoadCommStatusConfig()
{
	CString strIniFile = g_SysConfig;
	
	//ˢ�³�վ����ʱ��ļ��ʱ��,��λ(����)
	g_RefreshStationTime = GetPrivateProfileInt("CommStatusConfig", "RefreshStationTime", 10, strIniFile);
	//�б����Ƿ���ʾͨ��״̬�����ĳ�վ��Ϣ.0-����ʾ. 1-��ʾ
	g_ShowCommON = GetPrivateProfileInt("CommStatusConfig", "ShowCommON", 1, strIniFile);
	//�б����Ƿ���ʾͨ��״̬�����ĳ�վ��Ϣ.0-����ʾ. 1-��ʾ
	g_ShowCommOFF = GetPrivateProfileInt("CommStatusConfig", "ShowCommOFF", 1, strIniFile);
	//��վ�Ƿ�ʹ��˫ͨ��ͨ��
	g_DoubleChannel = GetPrivateProfileInt("CommStatusConfig", "DoubleChannel", 0, strIniFile);
	g_ShowDevStatusInStation = GetPrivateProfileInt("CommStatusConfig", "ShowDevStatusInStation", 1, strIniFile);
	//����վͨѶ״̬ͼ�α���ɫ
	g_CommMapBK = GetPrivateProfileInt("CommStatusConfig", "CommMapBk", RGB(0, 79, 79), strIniFile); 
	//����վͨѶ״��ͼ�г�վ������������
	g_CommMapFont = StringFromID(IDS_DEFAULT_FONT);
	GetPrivateProfileString("CommStatusConfig", "CommMapFont", StringFromID(IDS_DEFAULT_FONT), g_CommMapFont.GetBuffer(MAX_PATH), MAX_PATH,strIniFile);
	//����վͨѶ״��ͼ�г�վ���������С
	g_CommMapFontSize = GetPrivateProfileInt("CommStatusConfig", "CommMapFontSize", 18, strIniFile); 
	g_CommMapFontColor = GetPrivateProfileInt("CommStatusConfig", "CommMapFontColor", RGB(0,0,0), strIniFile); 
}

/*************************************************************
 �� �� ����SaveCommStatus()
 ���ܸ�Ҫ����������վͨѶ״̬ҳ�����õ������ļ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01E5
void CXJBrowserApp::SaveCommStatusConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	str.Format("%d", g_RefreshStationTime);
	//ˢ�³�վ����ʱ��ļ��ʱ��,��λ(����)
	WritePrivateProfileString("CommStatusConfig", "RefreshStationTime", str, strIniFile);
	//�б����Ƿ���ʾͨ��״̬�����ĳ�վ��Ϣ.0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowCommON);
	WritePrivateProfileString("CommStatusConfig", "ShowCommON", str, strIniFile);
	//�б����Ƿ���ʾͨ��״̬�����ĳ�վ��Ϣ.0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowCommOFF);
	WritePrivateProfileString("CommStatusConfig", "ShowCommOFF", str, strIniFile);
	//��վ�Ƿ�ʹ��˫ͨ��ͨ��
	str.Format("%d", g_DoubleChannel);
	WritePrivateProfileString("CommStatusConfig", "DoubleChannel", str, strIniFile);
	str.Format("%d", g_ShowDevStatusInStation);
	WritePrivateProfileString("CommStatusConfig", "ShowDevStatusInStation", str, strIniFile);

	//����վͨѶ״̬ͼ�α���ɫ
	str.Format("%d", g_CommMapBK);
	WritePrivateProfileString("CommStatusConfig", "CommMapBk", str, strIniFile); 
	//����վͨѶ״��ͼ�г�վ������������
	WritePrivateProfileString("CommStatusConfig", "CommMapFont", g_CommMapFont, strIniFile); 
	//����վͨѶ״��ͼ�г�վ���������С
	str.Format("%d", g_CommMapFontSize);
	WritePrivateProfileString("CommStatusConfig", "CommMapFontSize", str, strIniFile); 
	str.Format("%d", g_CommMapFontColor);
	WritePrivateProfileString("CommStatusConfig", "CommMapFontColor", str, strIniFile);
}

/*************************************************************
 �� �� ����LoadActionViewConfig()
 ���ܸ�Ҫ���������ļ����붯���¼�ҳ������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01F4
void CXJBrowserApp::LoadActionViewConfig()
{
	CString strIniFile = g_SysConfig;
	
	//�Ƿ���ʾ�����¼���ϸ����
	g_ShowActionDetail = GetPrivateProfileInt("ActionViewConfig", "ShowActionDetail", 1, strIniFile);
}

/*************************************************************
 �� �� ����SaveActionViewConfig()
 ���ܸ�Ҫ�����涯���¼�ҳ�����õ������ļ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01F5
void CXJBrowserApp::SaveActionViewConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	str.Format("%d", g_ShowActionDetail);
	//�Ƿ���ʾ�����¼���ϸ����
	WritePrivateProfileString("ActionViewConfig", "ShowActionDetail", str, strIniFile);
}

/*************************************************************
 �� �� ����LoadAlarmViewConfig()
 ���ܸ�Ҫ���������ļ�����澯�¼�ҳ������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0203
void CXJBrowserApp::LoadAlarmViewConfig()
{
	CString strIniFile = g_SysConfig;
	
	//�Ƿ���ʾ�����¼���ϸ����
	g_ShowAlarmDetail = GetPrivateProfileInt("AlarmViewConfig", "ShowAlarmDetail", 0, strIniFile);
}

/*************************************************************
 �� �� ����SaveAlarmViewConfig()
 ���ܸ�Ҫ��
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0204
void CXJBrowserApp::SaveAlarmViewConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	str.Format("%d", g_ShowAlarmDetail);
	//�Ƿ���ʾ�����¼���ϸ����
	WritePrivateProfileString("AlarmViewConfig", "ShowAlarmDetail", str, strIniFile);
}

/*************************************************************
 �� �� ����LoadSysAlarmViewConfig()
 ���ܸ�Ҫ���������ļ�����ϵͳ�澯ҳ������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0213
void CXJBrowserApp::LoadSysAlarmViewConfig()
{
	CString strIniFile = g_SysConfig;
	
	//�Ƿ���ʾ�����¼���ϸ����
	g_ShowSysAlarmDetail = GetPrivateProfileInt("SysAlarmViewConfig", "ShowSysAlarmDetail", 1, strIniFile);
}

/*************************************************************
 �� �� ����SaveSysAlarmViewConfig()
 ���ܸ�Ҫ������ϵͳ�澯ҳ�����õ������ļ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0214
void CXJBrowserApp::SaveSysAlarmViewConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	str.Format("%d", g_ShowSysAlarmDetail);
	//�Ƿ���ʾ�����¼���ϸ����
	WritePrivateProfileString("SysAlarmViewConfig", "ShowSysAlarmDetail", str, strIniFile);
}

/*************************************************************
 �� �� ����LoadBatchConfig()
 ���ܸ�Ҫ�����������ٻ����õ������ļ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0222
void CXJBrowserApp::LoadBatchConfig()
{
	CString strIniFile = g_SysConfig;
	
	//�����ٻ�ѡ����, "�ٻ�"��������ɫ
	g_BatchCall = GetPrivateProfileInt("BatchConfig", "BatchCall", RGB(0, 0, 0), strIniFile);
	//�����ٻ�ѡ����, "���ٻ�"��������ɫ
	g_BatchNotCall = GetPrivateProfileInt("BatchConfig", "BatchNotCall", RGB(255, 0, 0), strIniFile);
	//�����ٻ�ѡ����, "��֧��"��������ɫ
	g_BatchNotSupport = GetPrivateProfileInt("BatchConfig", "BatchNotSupport", RGB(0, 0, 255), strIniFile);
	//�����ٻ������, "�ɹ�"��������ɫ
	g_BatchSuccess = GetPrivateProfileInt("BatchConfig", "BatchSuccess", RGB(0, 0, 0), strIniFile);
	//�����ٻ������, "ʧ��"��������ɫ
	g_BatchFail = GetPrivateProfileInt("BatchConfig", "BatchFail", RGB(255, 0, 0), strIniFile);
	//�����ٻ������, "����ٻ�"��������ɫ
	g_BatchNULL = GetPrivateProfileInt("BatchConfig", "BatchNULL", RGB(80, 240, 120), strIniFile);
	//�����ٻ������, "��δ�ٻ�"��������ɫ
	g_BatchLater = GetPrivateProfileInt("BatchConfig", "BatchLater", RGB(121, 121, 121), strIniFile);
	//�Զ�����ģ�����ڵ�IP��ַ
	GetPrivateProfileString("BatchConfig", "AutoCall_IP", "10.123.16.133", g_AutoCall_IP.GetBuffer(16), 16,strIniFile);
	//�Զ�����ģ��ļ����˿�
	g_AutoCall_Port  = GetPrivateProfileInt("BatchConfig", "AutoCall_Port", 8888, strIniFile);
}

/*************************************************************
 �� �� ����SaveBatchConfig()
 ���ܸ�Ҫ�����������ٻ����õ������ļ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0223
void CXJBrowserApp::SaveBatchConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//�����ٻ�ѡ����, "�ٻ�"��������ɫ
	str.Format("%d", g_BatchCall);
	WritePrivateProfileString("BatchConfig", "BatchCall", str, strIniFile);
	//�����ٻ�ѡ����, "���ٻ�"��������ɫ
	str.Format("%d", g_BatchNotCall);
	WritePrivateProfileString("BatchConfig", "BatchNotCall", str, strIniFile);
	//�����ٻ�ѡ����, "��֧��"��������ɫ
	str.Format("%d", g_BatchNotSupport);
	WritePrivateProfileString("BatchConfig", "BatchNotSupport", str, strIniFile);
	//�����ٻ������, "�ɹ�"��������ɫ
	str.Format("%d", g_BatchSuccess);
	WritePrivateProfileString("BatchConfig", "BatchSuccess", str, strIniFile);
	//�����ٻ������, "ʧ��"��������ɫ
	str.Format("%d", g_BatchFail);
	WritePrivateProfileString("BatchConfig", "BatchFail", str, strIniFile);
	//�����ٻ������, "����ٻ�"��������ɫ
	str.Format("%d", g_BatchNULL);
	WritePrivateProfileString("BatchConfig", "BatchNULL", str, strIniFile);
	//�����ٻ������, "��δ�ٻ�"��������ɫ
	str.Format("%d", g_BatchLater);
	WritePrivateProfileString("BatchConfig", "BatchLater", str, strIniFile);
	//�Զ�����ģ�����ڵ�IP��ַ
	WritePrivateProfileString("BatchConfig", "AutoCall_IP", g_AutoCall_IP, strIniFile);
	//�Զ�����ģ��ļ����˿�
	str.Format("%d", g_AutoCall_Port);
	WritePrivateProfileString("BatchConfig", "AutoCall_Port", str, strIniFile);
}

/*************************************************************
 �� �� ����LoadPTCommViewConfig()
 ���ܸ�Ҫ�����뱣��ͨѶ״̬ҳ������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0224
void CXJBrowserApp::LoadPTCommViewConfig()
{
	CString strIniFile = g_SysConfig;
	
	//��Ѷ����,��ɫ
	XJ_STATUS_COMM_ON = GetPrivateProfileInt("PTCommViewConfig", "Comm_status_on", RGB(0, 111, 0), strIniFile);
	//ͨѶ�Ͽ�,��ɫ
	XJ_STATUS_COMM_OFF = GetPrivateProfileInt("PTCommViewConfig", "Comm_status_off", RGB(225, 0, 0), strIniFile);
	//�б����Ƿ���ʾͨ��״̬������װ����Ϣ.0-����ʾ. 1-��ʾ
	g_ShowPTCommON = GetPrivateProfileInt("PTCommViewConfig", "ShowPTCommON", 1, strIniFile);
	//�б����Ƿ���ʾͨ��״̬�Ͽ���װ����Ϣ.0-����ʾ. 1-��ʾ
	g_ShowPTCommOFF = GetPrivateProfileInt("PTCommViewConfig", "ShowPTCommOFF", 1, strIniFile);
	//ˢ�±���ͨ��״̬����ʱ��ļ��ʱ��,��λ(����)
	g_RefreshPTCommTime = GetPrivateProfileInt("PTCommViewConfig", "RefreshPTCommTime", 10, strIniFile);
}

/*************************************************************
 �� �� ����SavePTCommViewConfig()
 ���ܸ�Ҫ�����汣��ͨѶ״̬ҳ�����õ������ļ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0232
void CXJBrowserApp::SavePTCommViewConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	str.Format("%d", XJ_STATUS_COMM_ON);
	//��Ѷ����,��ɫ
	WritePrivateProfileString("PTCommViewConfig", "Comm_status_on", str, strIniFile);
	//��Ѷ����,��ɫ
	str.Format("%d", XJ_STATUS_COMM_OFF);
	WritePrivateProfileString("PTCommViewConfig", "Comm_status_off", str, strIniFile);
	//�б����Ƿ���ʾͨ��״̬������װ����Ϣ.0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowPTCommON);
	WritePrivateProfileString("PTCommViewConfig", "ShowPTCommON", str, strIniFile);
	//�б����Ƿ���ʾͨ��״̬�Ͽ���װ����Ϣ.0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowPTCommOFF);
	WritePrivateProfileString("PTCommViewConfig", "ShowPTCommOFF", str, strIniFile);
	//ˢ�±���ͨ��״̬����ʱ��ļ��ʱ��,��λ(����)
	str.Format("%d", g_RefreshPTCommTime);
	WritePrivateProfileString("PTCommViewConfig", "RefreshPTCommTime", str, strIniFile);
}

/*************************************************************
 �� �� ����LoadPTRunViewConfig()
 ���ܸ�Ҫ�����뱣������״̬ҳ������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0233
void CXJBrowserApp::LoadPTRunViewConfig()
{
	CString strIniFile = g_SysConfig;

	//����, ��ɫ
	XJ_STATUS_RUN_ON = GetPrivateProfileInt("PTRunViewConfig", "Run_status_on", RGB(0, 111, 0), strIniFile);
	//ͣ��, ����
	XJ_STATUS_RUN_OFF = GetPrivateProfileInt("PTRunViewConfig", "Run_status_off", RGB(162, 230, 165), strIniFile);
	//����/����, ��ɫ
	XJ_STATUS_RUN_CHECK = GetPrivateProfileInt("PTRunViewConfig", "Run_status_check", RGB(240, 240, 0), strIniFile);
	//��ֵ�޸Ĺ���
	XJ_STATUS_RUN_MARK = GetPrivateProfileInt("PTRunViewConfig", "Run_status_mark", RGB(250, 120, 0), strIniFile);
	//����/�����, ��ɫ
	XJ_STATUS_RUN_DEBUG = GetPrivateProfileInt("PTRunViewConfig", "Run_status_debug", RGB(157, 0, 183), strIniFile);
	//δ���룬��ɫ
	XJ_STATUS_RUN_CODE = GetPrivateProfileInt("PTRunViewConfig", "Run_status_code", RGB(121, 121, 121), strIniFile);
	//�б����Ƿ���ʾ����״̬������װ����Ϣ.0-����ʾ. 1-��ʾ
	g_ShowPTRunON = GetPrivateProfileInt("PTRunViewConfig", "ShowPTRunON", 1, strIniFile);
	//�б����Ƿ���ʾ����״̬���޵�װ����Ϣ.0-����ʾ. 1-��ʾ
	g_ShowPTRunCheck = GetPrivateProfileInt("PTRunViewConfig", "ShowPTRunCheck", 1, strIniFile);
	//�б����Ƿ���ʾ����״̬ͣ�˵�װ����Ϣ.0-����ʾ. 1-��ʾ
	g_ShowPTRunOFF = GetPrivateProfileInt("PTRunViewConfig", "ShowPTRunOFF", 1, strIniFile);
	//�б����Ƿ���ʾ����״̬���Ե�װ����Ϣ.0-����ʾ. 1-��ʾ
	g_ShowPTRunDebug = GetPrivateProfileInt("PTRunViewConfig", "ShowPTRunDebug", 1, strIniFile);
	//�б����Ƿ���ʾ����״̬������װ����Ϣ.0-����ʾ. 1-��ʾ
	g_ShowPTRunCode = GetPrivateProfileInt("PTRunViewConfig", "ShowPTRunCode", 1, strIniFile);
	//ˢ�±�������״̬����ʱ��ļ��ʱ��,��λ(����)
	g_RefreshPTRunTime = GetPrivateProfileInt("PTRunViewConfig", "RefreshPTRunTime", 10, strIniFile);


	//��վ��Ϣҳ��ˢ��ʱ��
	g_RefreshTime = GetPrivateProfileInt("StationInfoViewConfig", "RefreshStationInfoTime", 10, strIniFile);
}

/*************************************************************
 �� �� ����SavePTRunViewConfig()
 ���ܸ�Ҫ�����汣������״̬ҳ������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0234
void CXJBrowserApp::SavePTRunViewConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//����, ��ɫ
	str.Format("%d", XJ_STATUS_RUN_ON);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_on", str, strIniFile);
	//ͣ��, ����
	str.Format("%d", XJ_STATUS_RUN_OFF);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_off", str, strIniFile);
	//����/����, ��ɫ
	str.Format("%d", XJ_STATUS_RUN_CHECK);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_check", str, strIniFile);
	//ͣ��, ����
	str.Format("%d", XJ_STATUS_RUN_DEBUG);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_debug", str, strIniFile);
	//��ֵ�޸Ĺ���
	str.Format("%d", XJ_STATUS_RUN_MARK);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_mark", str, strIniFile);
	//�����, ��ɫ
	str.Format("%d", XJ_STATUS_RUN_CODE);
	WritePrivateProfileString("PTRunViewConfig", "Run_status_code", str, strIniFile);
	//�б����Ƿ���ʾ����״̬������װ����Ϣ.0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowPTRunON);
	WritePrivateProfileString("PTRunViewConfig", "ShowPTRunON", str, strIniFile);
	//�б����Ƿ���ʾ����״̬���޵�װ����Ϣ.0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowPTRunCheck);
	WritePrivateProfileString("PTRunViewConfig", "ShowPTRunCheck", str, strIniFile);
	//�б����Ƿ���ʾ����״̬ͣ�˵�װ����Ϣ.0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowPTRunOFF);
	WritePrivateProfileString("PTRunViewConfig", "ShowPTRunOFF", str, strIniFile);
	//�б����Ƿ���ʾ����״̬���Ե�װ����Ϣ.0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowPTRunDebug);
	WritePrivateProfileString("PTRunViewConfig", "ShowPTRunDebug", str, strIniFile);
	//�б����Ƿ���ʾ����״̬������װ����Ϣ.0-����ʾ. 1-��ʾ
	str.Format("%d", g_ShowPTRunCode);
	WritePrivateProfileString("PTRunViewConfig", "ShowPTRunCode", str, strIniFile);

	//ˢ�±�������״̬����ʱ��ļ��ʱ��,��λ(����)
	str.Format("%d", g_RefreshPTRunTime);
	WritePrivateProfileString("PTRunViewConfig", "RefreshPTRunTime", str, strIniFile);

	//��վ��Ϣҳ��ˢ��ʱ��
	str.Format("%d", g_RefreshTime);
	g_RefreshTime = WritePrivateProfileString("StationInfoViewConfig", "RefreshStationInfoTime", str, strIniFile);
}

/*************************************************************
 �� �� ����LoadConfig()
 ���ܸ�Ҫ���������������ļ�
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01B5
void CXJBrowserApp::LoadConfig()
{
	LoadSysConfig();
	LoadEventConfig();
	LoadActionViewConfig();
	LoadAlarmViewConfig();
	LoadBatchConfig();
	LoadCommStatusConfig();
	LoadPTCommViewConfig();
	LoadPTRunViewConfig();
	LoadSysAlarmViewConfig();
	LoadPTPropConfig();
	LoadWndConfig();
	LoadSplitterWndConfig();
}

/*************************************************************
 �� �� ����SaveConfig()
 ���ܸ�Ҫ��������������
 �� �� ֵ: 
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E01B6
void CXJBrowserApp::SaveConfig()
{
	SaveSysConfig();
	SaveEventConfig();
	SaveActionViewConfig();
	SaveAlarmViewConfig();
	SaveBatchConfig();
	SaveCommStatusConfig();
	SavePTCommViewConfig();
	SavePTRunViewConfig();
	SaveSysAlarmViewConfig();
	SavePTPropConfig();
	SaveWndConfig();
	SaveSplitterWndConfig();
}

//##ModelId=49B87B7E02AF
void CXJBrowserApp::OnSttpConnect() 
{
	// TODO: Add your command handler code here
	if (m_bConnect)
	{
		DisConnectSTTPServer();
	}
	else
	{
		Sleep(1000);
		if(m_SttpHandle != NULL)
		{
			if(AfxMessageBox(StringFromID(IDS_TIP_RECONNECTING), MB_OKCANCEL) == IDOK)
			{
				//�����Ӿ������, �ȹر�
				DisConnectSTTPServer();
				ConnectSTTPServerB();
			}
		}
		else
		{
			ConnectSTTPServerB();
		}
	}
}

//##ModelId=49B87B7E02BF
void CXJBrowserApp::OnUpdateSttpConnect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(!m_bConnect);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (m_bConnect)
	{
		//pCmdUI->SetText("�Ͽ�ͨ��");
		pFrame->m_wndToolBar.SetButtonText(0, StringFromID(IDS_DISCONNECT_COMMUSERVER));
	}
	else
	{
		//pCmdUI->SetText("����ͨ��");
		pFrame->m_wndToolBar.SetButtonText(0, StringFromID(IDS_CONNECT_COMMUSERVER));
	}
}

//##ModelId=49B87B7E02C2
void CXJBrowserApp::OnSttpDisconnect() 
{
	// TODO: Add your command handler code here
	DisConnectSTTPServer();
}

//##ModelId=49B87B7E02C4
void CXJBrowserApp::OnUpdateSttpDisconnect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bConnect);
}

//##ModelId=49B87B7E02D0
void CXJBrowserApp::OnSysLogout() 
{
	// TODO: Add your command handler code here
	DoLogOut();
}

//##ModelId=49B87B7E02D2
void CXJBrowserApp::OnUpdateSysLogout(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_User.m_strUSER_ID.IsEmpty()) //�����û�
	{
	//	pCmdUI->Enable(FALSE);
		pCmdUI->SetText(StringFromID(IDS_COMMON_WRITEOFF)+"[&Z]...");
	}
	else
	{
		CString strTemp;
		strTemp.Format ("%s %s [&Z]...", StringFromID(IDS_COMMON_WRITEOFF),m_User.m_strUSER_ID);
		pCmdUI->Enable(TRUE);
		pCmdUI->SetText (strTemp);
	}
}

/*************************************************************
 �� �� ����OpenDefaultStation()
 ���ܸ�Ҫ����Ĭ�ϳ�վͼ�κ����豸����ѡ��
 �� �� ֵ: void
 ��    ����param1	��վID
		   Param2
**************************************************************/
//##ModelId=49B87B7E0244
void CXJBrowserApp::OpenDefaultStation( CString strID )
{
	CDataEngine* pData = GetDataEngine();
	if(pData == NULL)
		return;
	CStationObj* pStation = NULL;
	if(strID == "")	//IDΪ��,������
	{
		if( g_role == MODE_SUB ) //��վĬ�ϴ�ͼ��
		{
			//���ҳ�վָ�� 
			DEVICE_LIST listDevice;
			listDevice.RemoveAll();
			pData->GetStationList(listDevice);
			POSITION pos = listDevice.GetHeadPosition();
			MYASSERT(pos);
			pStation = (CStationObj*)listDevice.GetAt(pos);
		}
	}
	else
	{
		//���ҳ�վָ��
		pStation = (CStationObj*)pData->FindDevice(strID, TYPE_STATION);
		
	}
	if(pStation == NULL) //���Ҳ�����վ, ������
		return;
	
	//��ͼ��
	if(g_bShowDeviceMapWnd)
		ShowDevice(pStation, FALSE);
	else
		ShowStationInfo(pStation);
	//���豸����ѡ��
	LocateInTree(pStation);
}

void CXJBrowserApp::OpenDefaultStation()
{

	CDataEngine* pData = GetDataEngine();
	MYASSERT(pData);
	//���ҳ�վָ�� 
	DEVICE_LIST listDevice;
	listDevice.RemoveAll();
	pData->GetStationList(listDevice);
	POSITION pos = listDevice.GetHeadPosition();
	MYASSERT(pos);
	CStationObj* pStation = (CStationObj*)listDevice.GetAt(pos);
	MYASSERT(pStation);
	//��ͼ��
	ShowDevice(pStation, FALSE);
	//���豸����ѡ��
	LocateInTree(pStation);
}


//##ModelId=49B87B7E02DF
void CXJBrowserApp::OnSysExit() 
{
	// TODO: Add your command handler code here
	g_bExit = 1;
	CMainFrame* pFrame = (CMainFrame*)GetMainWnd();
	if(pFrame != NULL)
		pFrame->SendMessage(WM_CLOSE);
}

//##ModelId=49B87B7E02E1
void CXJBrowserApp::OnUpdateSysExit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

//##ModelId=49B87B7E02EE
void CXJBrowserApp::OnHelpContent() 
{
	// TODO: Add your command handler code here
	return;
}

//##ModelId=49B87B7E02F0
void CXJBrowserApp::OnUpdateHelpContent(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

/*************************************************************
 �� �� ����QueryUserGroup()
 ���ܸ�Ҫ��ָ���û��������û�����
 �� �� ֵ: �û�����
 ��    ����param1	�û���
		   Param2
**************************************************************/
//##ModelId=49B87B7E0251
CString CXJBrowserApp::QueryUserGroup( CString sUserID )
{
	CString strReturn = "";

	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//group_id
	Field Field1;
	m_DBEngine.SetField(sql, Field1, "group_id", EX_STTP_DATA_TYPE_STRING);
	
	CString str;
	//����
	//USER_ID
	Condition condition2;
	str.Format("USER_ID = '%s'", sUserID);
	m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_USER_CFG, sql, sError, pMemset);
	if(pMemset != NULL && nResult == 1)
	{
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount <= 0)
		{
			strReturn = "";
		}
		else
		{
			//ֻӦ����һ��ֵ
			strReturn = pMemset->GetValue((UINT)0); //�û���ID
		}
	}
	else
	{
		//��ѯ���ݿ�ʧ��
		strReturn = "";
	}
	
	delete[] sError;
	delete pMemset;
	return strReturn;
}

//##ModelId=49B87B7E02FD
void CXJBrowserApp::OnSysSetting() 
{
	// TODO: Add your command handler code here
	if (g_bVerify)
	{
		//��֤Ȩ��
		if(!TryEnter(FUNC_XJBROWSER_SETTING))
		{
			return;
		}
	}
	DoSysSetting(SYS_SETTING_GENERAL);
	//��¼����
	//AddManOperator(FUNC_XJBROWSER_SETTING, FUNC_SYS_SETTING);
}

//##ModelId=49B87B7E02FF
void CXJBrowserApp::OnUpdateSysSetting(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

/*************************************************************
 �� �� ����DoSysSetting()
 ���ܸ�Ҫ������ϵͳ����ҳ��
 �� �� ֵ: 
 ��    ����param1	��ʼ���ĸ�SHEETҳ. SYS_SETTING_GENERALΪ��������ҳ��. SYS_SETTING_INFOCASEΪ��Ϣ����ҳ��. const SYS_SETTING_STATUSΪ״̬����ҳ��. SYS_SETTING_PTPTOPΪ������������ҳ��
		   Param2
**************************************************************/
//##ModelId=49B87B7E0253
void CXJBrowserApp::DoSysSetting( int nPage )
{
	CSysSettingSheet dlg(StringFromID(IDS_TOOLBAR_SYSCONF), NULL, nPage);

	BOOL bNeedRefreshActionView = FALSE; //�Ƿ���Ҫˢ�¶����¼���ͼ
	BOOL bNeedRefreshAlarmView = FALSE; //�Ƿ���Ҫˢ�¸澯�¼���ͼ
	BOOL bNeedRefreshPTSetting = FALSE; //�Ƿ���Ҫˢ�±�����ֵҳ��
	BOOL bNeedRefreshPTDI = FALSE; //�Ƿ���Ҫˢ�±���������ҳ��
	BOOL bNeedRefreshPTSoft = FALSE; //�Ƿ���Ҫˢ�±�����ѹ��ҳ��
	
	//ϵͳ����ҳ��
	dlg.m_PageGeneral.m_bDeviceTree = (g_ShowDeviceTreeWnd == 1);
	dlg.m_PageGeneral.m_bOutPut = (g_ShowOutputWnd == 1);
	dlg.m_PageGeneral.m_bActionDetail = (g_ShowActionDetail == 1);
	dlg.m_PageGeneral.m_bSysAlarmDetail = (g_ShowSysAlarmDetail == 1);
	dlg.m_PageGeneral.m_btnBkColor.m_Color = g_CommMapBK;
	dlg.m_PageGeneral.m_btnFontColor.m_Color = g_CommMapFontColor;
	

	//��Ϣ����ҳ��
	//I��
	dlg.m_PageInfoCase.m_bShowViewI = (g_ShowEventI == 1);
	dlg.m_PageInfoCase.m_bAlarmViewI = (g_AlarmEventI == 1 && g_ShowEventI == 1);
	dlg.m_PageInfoCase.m_btnViewIColor.m_Color = g_colEventI;
	//II��
	dlg.m_PageInfoCase.m_bShowViewII = (g_ShowEventII == 1);
	dlg.m_PageInfoCase.m_bAlarmViewII = (g_AlarmEventII == 1 && g_ShowEventII == 1);
	dlg.m_PageInfoCase.m_btnViewIIColor.m_Color = g_colEventII;
	//III��
	dlg.m_PageInfoCase.m_bShowViewIII = (g_ShowEventIII == 1);
	dlg.m_PageInfoCase.m_bAlarmViewIII = (g_AlarmEventIII == 1 && g_ShowEventIII == 1);
	dlg.m_PageInfoCase.m_btnViewIIIColor.m_Color = g_colEventIII;
	//��������
	dlg.m_PageInfoCase.m_bBlinkSec = (g_BlinkDevice == 1);
	dlg.m_PageInfoCase.m_bBlinkDevice = (g_BlinkOwner == 1 && g_BlinkDevice == 1);
	dlg.m_PageInfoCase.m_bPopWnd = (g_ShowEventDlg == 1);
	dlg.m_PageInfoCase.m_bTopSec = (g_MustShowDevice == 1);
	dlg.m_PageInfoCase.m_bUseSound = (g_UseAlarmSound == 1);
	dlg.m_PageInfoCase.m_bAutoChange = (g_AutoChangeMap == 1);
	dlg.m_PageInfoCase.m_nAlarmTime = g_SoundTime;
	dlg.m_PageInfoCase.m_nFlashTime = g_FlashTime;

	//�豸״̬����ҳ��
	//ͨѶ״̬
	dlg.m_PageStatus.m_btnCommON.m_Color = XJ_STATUS_COMM_ON;
	dlg.m_PageStatus.m_btnCommOFF.m_Color = XJ_STATUS_COMM_OFF;
	//����״̬
	dlg.m_PageStatus.m_btnRunON.m_Color = XJ_STATUS_RUN_ON;
	dlg.m_PageStatus.m_btnRunOFF.m_Color = XJ_STATUS_RUN_OFF;
	dlg.m_PageStatus.m_btnRunCheck.m_Color = XJ_STATUS_RUN_CHECK;
	dlg.m_PageStatus.m_btnRunCode.m_Color = XJ_STATUS_RUN_CODE;
	dlg.m_PageStatus.m_btnRunDebug.m_Color = XJ_STATUS_RUN_DEBUG;
	dlg.m_PageStatus.m_btnRunHangout.m_Color = XJ_STATUS_RUN_MARK;

	//��������ҳ��
	dlg.m_PagePTProp.m_nSelSettingValues = g_PTSettingValues;
	dlg.m_PagePTProp.m_bSettingGroup = (g_PTSettingGroup == 1);
	dlg.m_PagePTProp.m_nSelDIValues = g_PTDIValues;
	dlg.m_PagePTProp.m_nSelDIStyle = g_PTDIStyle;
	dlg.m_PagePTProp.m_nSelSoftValues = g_PTSoftValues;
	dlg.m_PagePTProp.m_nSelSoftStyle = g_PTSoftStyle;
	dlg.m_PagePTProp.m_bPTAI = (g_PTAIValues == 1);

	if(dlg.DoModal() == IDOK)
	{
		int nTemp = 0;
		COLORREF colTemp;
		//����ϵͳ����
		g_ShowDeviceTreeWnd = dlg.m_PageGeneral.m_bDeviceTree?1:0;
		g_ShowOutputWnd = dlg.m_PageGeneral.m_bOutPut?1:0;
		g_ShowActionDetail = dlg.m_PageGeneral.m_bActionDetail?1:0;
		g_ShowSysAlarmDetail = dlg.m_PageGeneral.m_bSysAlarmDetail?1:0;
		g_strStyleName = dlg.m_PageGeneral.m_sStyleName;
		CString str;
		if (dlg.m_PageGeneral.m_iSel != -1)
		{
			str.Format("%d", dlg.m_PageGeneral.m_iSel);
			WritePrivateProfileString("SysConfig", "Style", str, g_SysConfig);
		}
		WritePrivateProfileString("Style", "StyleName", g_strStyleName, g_SysConfig);

		g_CommMapBK = dlg.m_PageGeneral.m_btnBkColor.m_Color;
		g_CommMapFontColor = dlg.m_PageGeneral.m_btnFontColor.m_Color;
		g_CommMapFont = dlg.m_PageGeneral.m_sFont;
		g_CommMapFontSize = dlg.m_PageGeneral.m_nFontSize;

		//������Ϣ��������
		//I��
		nTemp = dlg.m_PageInfoCase.m_bShowViewI?1:0;
		if(nTemp != g_ShowEventI)
		{
			//����, ��Ҫˢ��
			g_ShowEventI = nTemp;
			bNeedRefreshActionView = TRUE;
			bNeedRefreshAlarmView = TRUE;
		}
		g_AlarmEventI = dlg.m_PageInfoCase.m_bAlarmViewI?1:0;
		colTemp = dlg.m_PageInfoCase.m_btnViewIColor.m_Color;
		if(colTemp != g_colEventI)
		{
			//����, ��Ҫˢ��
			g_colEventI = colTemp;
		}

		//II��
		nTemp = dlg.m_PageInfoCase.m_bShowViewII?1:0;
		if(nTemp != g_ShowEventII)
		{
			//����,��Ҫˢ��
			g_ShowEventII = nTemp;
			bNeedRefreshActionView = TRUE;
			bNeedRefreshAlarmView = TRUE;
		}
		g_AlarmEventII = dlg.m_PageInfoCase.m_bAlarmViewII?1:0;
		colTemp = dlg.m_PageInfoCase.m_btnViewIIColor.m_Color;
		if(colTemp != g_colEventII)
		{
			//����, ��Ҫˢ��
			g_colEventII = colTemp;
		}

		//III��
		nTemp = dlg.m_PageInfoCase.m_bShowViewIII?1:0;
		if(nTemp != g_ShowEventIII)
		{
			//����, ��Ҫˢ��
			g_ShowEventIII = nTemp;
			bNeedRefreshActionView = TRUE;
			bNeedRefreshAlarmView = TRUE;
		}
		g_AlarmEventIII = dlg.m_PageInfoCase.m_bAlarmViewIII?1:0;
		colTemp = dlg.m_PageInfoCase.m_btnViewIIIColor.m_Color;
		if(colTemp != g_colEventIII)
		{
			//����, ��Ҫˢ��
			g_colEventIII = colTemp;
		}
		//��������
		g_BlinkDevice = dlg.m_PageInfoCase.m_bBlinkSec?1:0;
		g_BlinkOwner = dlg.m_PageInfoCase.m_bBlinkDevice?1:0;
		g_ShowEventDlg = dlg.m_PageInfoCase.m_bPopWnd?1:0;
		g_MustShowDevice = dlg.m_PageInfoCase.m_bTopSec?1:0;
		g_UseAlarmSound = dlg.m_PageInfoCase.m_bUseSound?1:0;
		g_AutoChangeMap = dlg.m_PageInfoCase.m_bAutoChange?1:0;
		g_SoundTime = dlg.m_PageInfoCase.m_nAlarmTime;
		g_FlashTime = dlg.m_PageInfoCase.m_nFlashTime;

		//�����豸״̬����
		//ͨѶ״̬
		XJ_STATUS_COMM_ON = dlg.m_PageStatus.m_btnCommON.m_Color;
		XJ_STATUS_COMM_OFF = dlg.m_PageStatus.m_btnCommOFF.m_Color;
		//����״̬
		XJ_STATUS_RUN_ON = dlg.m_PageStatus.m_btnRunON.m_Color;
		XJ_STATUS_RUN_OFF = dlg.m_PageStatus.m_btnRunOFF.m_Color;
		XJ_STATUS_RUN_CHECK = dlg.m_PageStatus.m_btnRunCheck.m_Color;
		XJ_STATUS_RUN_CODE = dlg.m_PageStatus.m_btnRunCode.m_Color;
		XJ_STATUS_RUN_DEBUG = dlg.m_PageStatus.m_btnRunDebug.m_Color;
		XJ_STATUS_RUN_MARK = dlg.m_PageStatus.m_btnRunHangout.m_Color;

		//���汣����������
		g_PTSettingValues = dlg.m_PagePTProp.m_nSelSettingValues;
		nTemp = dlg.m_PagePTProp.m_bSettingGroup?1:0;
		if(g_PTSettingGroup != nTemp)
		{
			g_PTSettingGroup = nTemp;
			bNeedRefreshPTSetting = TRUE;
		}
		g_PTDIValues = dlg.m_PagePTProp.m_nSelDIValues;
		nTemp = dlg.m_PagePTProp.m_nSelDIStyle;
		if(g_PTDIStyle != nTemp)
		{
			g_PTDIStyle = nTemp;
			bNeedRefreshPTDI = TRUE;
		}
		g_PTSoftValues = dlg.m_PagePTProp.m_nSelSoftValues;
		nTemp = dlg.m_PagePTProp.m_nSelSoftStyle;
		if(g_PTSoftStyle != nTemp)
		{
			g_PTSoftStyle = nTemp;
			bNeedRefreshPTSoft = TRUE;
		}
		g_PTAIValues = dlg.m_PagePTProp.m_bPTAI?1:0;

		if (dlg.m_PageStyle.m_bOper && g_style == 3)
		{
			AfxMessageBox(StringFromID(IDS_TIP_RESTART_APP));
		}

	}

	//ˢ�½�����ʾ
	CMainFrame* pFrame = (CMainFrame*)GetMainWnd();
	if(bNeedRefreshActionView)
	{
		WriteLog("���øı�, ��Ҫˢ�¶����¼���ʾ");		
		if(pFrame != NULL)
			pFrame->PostMessage(REFRESH_ACTIONVIEW);
	}
	if(bNeedRefreshAlarmView)
	{
		WriteLog("���øı�, ��Ҫˢ�¸澯�¼���ʾ");
		if(pFrame != NULL)
			pFrame->PostMessage(REFRESH_ALARMVIEW);
	}
	if(bNeedRefreshPTSetting)
	{
		WriteLog("���øı�, ��Ҫˢ�±�����ֵ��ʾ");
		if(pFrame != NULL)
			pFrame->PostMessage(REFRESH_PTSETTING);
	}
	if(bNeedRefreshPTDI)
	{
		WriteLog("���øı�, ��Ҫˢ�±�����������ʾ");
		if(pFrame != NULL)
			pFrame->PostMessage(REFRESH_PTDI);
	}
	if(bNeedRefreshPTSoft)
	{
		WriteLog("���øı�, ��Ҫˢ�±�����ѹ����ʾ");
		if(pFrame != NULL)
			pFrame->PostMessage(REFRESH_PTSOFTBOARD);
	}
	if(pFrame->pCommFrame)
	{
		pFrame->pCommFrame->PostMessage(REFRESH_COMMMAP);
	}
}

/*************************************************************
 �� �� ����LoadPTPropConfig()
 ���ܸ�Ҫ�����뱣������ҳ������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0242
void CXJBrowserApp::LoadPTPropConfig()
{
	CString strIniFile = g_SysConfig;
	
	//��ֵ��ʷֵ��������. 0-10����
	g_PTSettingValues = GetPrivateProfileInt("PTPropConfig", "SettingValues", 5, strIniFile);
	if(g_PTSettingValues < 0 || g_PTSettingValues > 10)
		g_PTSettingValues = 5;
	//��ֵ�Ƿ�����ʾ. 0-��, 1-��
	g_PTSettingGroup = GetPrivateProfileInt("PTPropConfig", "SettingGroup", 1, strIniFile);
	//��ֵ������Ƚ�, 0-��ǰһ��ֵ�Ƚ�, 1-���׼ֵ�Ƚ�
	g_PTSettingCompare = GetPrivateProfileInt("PTPropConfig", "SettingCompare", 0, strIniFile);
	//��������ʷֵ��������. 0-10����
	g_PTDIValues = GetPrivateProfileInt("PTPropConfig", "DIValues", 5, strIniFile);
	if(g_PTDIValues < 0 || g_PTDIValues > 10)
		g_PTDIValues = 5;
	//������ֵ��ʾ���.0: 1/0, 1: ON/OFF, 2: Ͷ/��, 3: ��/��.
	g_PTDIStyle = GetPrivateProfileInt("PTPropConfig", "DIStyle", 5, strIniFile);
	if(g_PTDIStyle < 0 || g_PTDIStyle > 4)
		g_PTDIStyle = 0;
	//��ѹ����ʷֵ��������. 0-10����
	g_PTSoftValues = GetPrivateProfileInt("PTPropConfig", "SoftValues", 5, strIniFile);
	if(g_PTSoftValues < 0 || g_PTSoftValues > 10)
		g_PTSoftValues = 5;
	//��ѹ��ֵ��ʾ���: 0: 1/0, 1: ON/OFF, 3: Ͷ/��.
	g_PTSoftStyle = GetPrivateProfileInt("PTPropConfig", "SoftStyle", 5, strIniFile);
	if(g_PTSoftStyle < 0 || g_PTSoftStyle > 3)
		g_PTSoftStyle = 0;
	//ģ�����Ƿ���ʾ��ʷֵ 0:����ʾ. 1:��ʾ
	g_PTAIValues = GetPrivateProfileInt("PTPropConfig", "AIValues", 1, strIniFile);
	g_PTAIRealtimeValue = GetPrivateProfileInt("PTPropConfig", "AIRealtimeValue", 0, strIniFile);
	g_PTAIRealtimeRefresh = GetPrivateProfileInt("PTPropConfig", "AIRealtimeRefresh", 60, strIniFile);
	//���ƹ����Ƿ����. 0:������. 1:����
	g_PTControl = GetPrivateProfileInt("PTPropConfig", "ControlFunc", 0, strIniFile);
	//��������ҳ���"��ʾ����"��ť�Ƿ���ʾ.0-��, 1-��
	g_PTGeneralMore = GetPrivateProfileInt("PTPropConfig", "GeneralMore", 0, strIniFile);
	//�����¼���ʾ��
	g_PTActionNum = GetPrivateProfileInt("PTPropConfig", "PTActionNum", 100, strIniFile);
	//�澯�¼���ʾ��
	g_PTAlarmNum = GetPrivateProfileInt("PTPropConfig", "PTAlarmNum", 100, strIniFile);	
	int nDefault = 1;
	if(g_role == MODE_SUB)//��վʱĬ�ϲ���ʾ
		nDefault = 0;
	g_PTShowHistory = GetPrivateProfileInt("PTPropConfig", "PTShowHistory", nDefault, strIniFile);
	g_PTNewZoneTable = GetPrivateProfileInt("PTPropConfig", "PTNewZoneTable", 0, strIniFile);
	g_PTControlModel = GetPrivateProfileInt("PTPropConfig", "PTControlModel", 1, strIniFile);
	g_PTModifyZone = GetPrivateProfileInt("PTPropConfig", "PTModifyzone", 1, strIniFile);
	g_PTShowGeneral = GetPrivateProfileInt("PTPropConfig", "PTShowGeneral", 1, strIniFile);	//������Ϣҳ���Ƿ���ʾ��0-��1-��
	g_PTShowSetting = GetPrivateProfileInt("PTPropConfig", "PTShowSetting", 1, strIniFile);	//��ֵҳ���Ƿ���ʾ��0-��1-��
	g_PTShowDI = GetPrivateProfileInt("PTPropConfig", "PTShowDI", 1, strIniFile);			//������ҳ���Ƿ���ʾ��0-��1-��
	g_PTShowSoftBoard = GetPrivateProfileInt("PTPropConfig", "PTShowSoftBoard", 1, strIniFile);	//��ѹ��ҳ���Ƿ���ʾ��0-��1-��
	g_PTShowAI = GetPrivateProfileInt("PTPropConfig", "PTShowAI", 1, strIniFile);			//ģ����ҳ���Ƿ���ʾ��0-��1-��
	g_PTShowAction = GetPrivateProfileInt("PTPropConfig", "PTShowAction", 1, strIniFile);		//����ҳ���Ƿ���ʾ��0-��1-��
	g_PTShowAlarm = GetPrivateProfileInt("PTPropConfig", "PTShowAlarm", 1, strIniFile);		//�澯ҳ���Ƿ���ʾ��0-��1-��
	g_PTShowOsc = GetPrivateProfileInt("PTPropConfig", "PTShowOSC", 1, strIniFile);		//����¼��ҳ���Ƿ���ʾ��0-��1-��
	g_PTShowOscList = GetPrivateProfileInt("PTPropConfig", "PTShowOscList", 1, strIniFile);	//Զ��¼��ҳ���Ƿ���ʾ��0-��1-��
	g_PTShowNote = GetPrivateProfileInt("PTPropConfig", "PTShowNote", 0, strIniFile);		//��ע���Ƿ���ʾ��0-��1-��
	g_PTSettingJudge = GetPrivateProfileInt("PTPropConfig", "PTSettingJudge", 0, strIniFile);
	g_PTSetting160x = GetPrivateProfileInt("PTPropConfig", "PTSetting160x", 0, strIniFile);

	g_WRShowGeneral = GetPrivateProfileInt("WRPropConfig", "WRShowGeneral", 1, strIniFile);	//������Ϣҳ���Ƿ���ʾ��0-��1-��
	g_WRShowSetting = GetPrivateProfileInt("WRPropConfig", "WRShowSetting", 1, strIniFile);	//��ֵҳ���Ƿ���ʾ��0-��1-��
	g_WRShowDI = GetPrivateProfileInt("WRPropConfig", "WRShowDI", 1, strIniFile);			//������ҳ���Ƿ���ʾ��0-��1-��
	g_WRShowAI = GetPrivateProfileInt("WRPropConfig", "WRShowAI", 1, strIniFile);			//ģ����ҳ���Ƿ���ʾ��0-��1-��
	g_WRShowOsc = GetPrivateProfileInt("WRPropConfig", "WRShowOSC", 1, strIniFile);		//����¼��ҳ���Ƿ���ʾ��0-��1-��
	g_WRShowOscList = GetPrivateProfileInt("WRPropConfig", "WRShowOscList", 1, strIniFile);	//Զ��¼��ҳ���Ƿ���ʾ��0-��1-��
	g_WRShowNote = GetPrivateProfileInt("WRPropConfig", "WRShowNote", 0, strIniFile);		//��ע���Ƿ���ʾ��0-��1-��
	g_PTIndexType = GetPrivateProfileInt("PTPropConfig", "PTIndexType", 0, strIniFile);
	g_PTNotModifyCurrentZone = GetPrivateProfileInt("PTPropConfig", "NotModifyCurrentZone", 0, strIniFile);
	g_PTOscListCompare = GetPrivateProfileInt("PTPropConfig", "OscListCompare", 1, strIniFile);
}

/*************************************************************
 �� �� ����SavePTPropConfig()
 ���ܸ�Ҫ�����汣������ҳ������
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0243
void CXJBrowserApp::SavePTPropConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;

	//��ֵ��ʷֵ��������. 0-10����
	str.Format("%d", g_PTSettingValues);
	WritePrivateProfileString("PTPropConfig", "SettingValues", str, strIniFile);
	//��ֵ�Ƿ�����ʾ. 0-��, 1-��
	str.Format("%d", g_PTSettingGroup);
	WritePrivateProfileString("PTPropConfig", "SettingGroup", str, strIniFile);
	//��ֵ������Ƚ�, 0-��ǰһ��ֵ�Ƚ�, 1-���׼ֵ�Ƚ�
	str.Format("%d", g_PTSettingCompare);
	WritePrivateProfileString("PTPropConfig", "SettingCompare", str, strIniFile);
	//��������ʷֵ��������. 0-10����
	str.Format("%d", g_PTDIValues);
	WritePrivateProfileString("PTPropConfig", "DIValues", str, strIniFile);
	//������ֵ��ʾ���.0: 1/0, 1: ON/OFF, 2: Ͷ/��, 3: ��/��.
	str.Format("%d", g_PTDIStyle);
	WritePrivateProfileString("PTPropConfig", "DIStyle", str, strIniFile);
	//��ѹ����ʷֵ��������. 0-10����
	str.Format("%d", g_PTSoftValues);
	WritePrivateProfileString("PTPropConfig", "SoftValues", str, strIniFile);
	//��ѹ��ֵ��ʾ���: 0: 1/0, 1: ON/OFF, 3: Ͷ/��.
	str.Format("%d", g_PTSoftStyle);
	WritePrivateProfileString("PTPropConfig", "SoftStyle", str, strIniFile);
	//ģ�����Ƿ���ʾ��ʷֵ 0:����ʾ. 1:��ʾ
	str.Format("%d", g_PTAIValues);
	WritePrivateProfileString("PTPropConfig", "AIValues", str, strIniFile);

	str.Format("%d", g_PTAIRealtimeValue);
	WritePrivateProfileString("PTPropConfig", "AIRealtimeValue", str, strIniFile);

	str.Format("%d", g_PTAIRealtimeRefresh);
	WritePrivateProfileString("PTPropConfig", "AIRealtimeRefresh", str, strIniFile);
	//���ƹ����Ƿ����. 0:������. 1:����
	str.Format("%d", g_PTControl);
	WritePrivateProfileString("PTPropConfig", "ControlFunc", str, strIniFile);
	//��������ҳ���"��ʾ����"��ť�Ƿ���ʾ.0-��, 1-��
	str.Format("%d", g_PTGeneralMore);
	WritePrivateProfileString("PTPropConfig", "GeneralMore", str, strIniFile);
	//����������ʾ��
	str.Format("%d", g_PTActionNum);
	WritePrivateProfileString("PTPropConfig", "PTActionNum", str, strIniFile);
	//�����澯��ʾ��
	str.Format("%d", g_PTAlarmNum);
	WritePrivateProfileString("PTPropConfig", "PTAlarmNum", str, strIniFile);
	//��ʷ�¼�ҳ���Ƿ���ʾ
	str.Format("%d", g_PTShowHistory);
	WritePrivateProfileString("PTPropConfig", "PTShowHistory", str, strIniFile);
	str.Format("%d", g_PTNewZoneTable);
	WritePrivateProfileString("PTPropConfig", "PTNewZoneTable", str, strIniFile);
// 	str.Format("%d", g_PTControlModel);
// 	WritePrivateProfileString("PTPropConfig", "PTControlModel", str, strIniFile);
	str.Format("%d", g_PTModifyZone);
	WritePrivateProfileString("PTPropConfig", "PTModifyzone", str, strIniFile);
	str.Format("%d", g_PTShowGeneral);
	WritePrivateProfileString("PTPropConfig", "PTShowGeneral", str, strIniFile);	//������Ϣҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_PTShowSetting);
	WritePrivateProfileString("PTPropConfig", "PTShowSetting", str, strIniFile);	//��ֵҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_PTShowDI);
	WritePrivateProfileString("PTPropConfig", "PTShowDI", str, strIniFile);			//������ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_PTShowSoftBoard);
	WritePrivateProfileString("PTPropConfig", "PTShowSoftBoard", str, strIniFile);	//��ѹ��ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_PTShowAI);
	WritePrivateProfileString("PTPropConfig", "PTShowAI", str, strIniFile);			//ģ����ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_PTShowAction);
	WritePrivateProfileString("PTPropConfig", "PTShowAction", str, strIniFile);		//����ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_PTShowAlarm);
	WritePrivateProfileString("PTPropConfig", "PTShowAlarm", str, strIniFile);		//�澯ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_PTShowOsc);
	WritePrivateProfileString("PTPropConfig", "PTShowOSC", str, strIniFile);		//����¼��ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_PTShowOscList);
	WritePrivateProfileString("PTPropConfig", "PTShowOscList", str, strIniFile);	//Զ��¼��ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_PTSettingJudge);
	WritePrivateProfileString("PTPropConfig", "PTSettingJudge", str, strIniFile);
	str.Format("%d", g_PTIndexType);
	WritePrivateProfileString("PTPropConfig", "PTIndexType", str, strIniFile);
	str.Format("%d", g_PTNotModifyCurrentZone);
	WritePrivateProfileString("PTPropConfig", "NotModifyCurrentZone", str, strIniFile);
	str.Format("%d", g_PTOscListCompare);
	WritePrivateProfileString("PTPropConfig", "OscListCompare", str, strIniFile);
	str.Format("%d", g_PTSetting160x);
	WritePrivateProfileString("PTPropConfig", "PTSetting160x", str, strIniFile);
	str.Format("%d", g_PTShowNote);
	WritePrivateProfileString("PTPropConfig", "PTShowNote", str, strIniFile);	//��עҳ���Ƿ���ʾ��0-��1-��


	str.Format("%d", g_WRShowGeneral);
	WritePrivateProfileString("WRPropConfig", "WRShowGeneral", str, strIniFile);	//������Ϣҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_WRShowSetting);
	WritePrivateProfileString("WRPropConfig", "WRShowSetting", str, strIniFile);	//��ֵҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_WRShowDI);
	WritePrivateProfileString("WRPropConfig", "WRShowDI", str, strIniFile);			//������ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_WRShowAI);
	WritePrivateProfileString("WRPropConfig", "WRShowAI", str, strIniFile);			//ģ����ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_WRShowOsc);
	WritePrivateProfileString("WRPropConfig", "WRShowOSC", str, strIniFile);		//����¼��ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_WRShowOscList);
	WritePrivateProfileString("WRPropConfig", "WRShowOscList", str, strIniFile);	//Զ��¼��ҳ���Ƿ���ʾ��0-��1-��
	str.Format("%d", g_WRShowNote);
	WritePrivateProfileString("WRPropConfig", "WRShowNote", str, strIniFile);	//��עҳ���Ƿ���ʾ��0-��1-��
}

/*************************************************************
 �� �� ����ClearAllTempView()
 ���ܸ�Ҫ������������ʱ��ͼָ��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0261
void CXJBrowserApp::ClearAllTempView()
{
	/** @brief           ���ϱ�����ϸ��Ϣ��ͼָ��,���ڴ�ӡ���ϱ���ʱ�Ļص�����*/
	m_pFaultDetail = NULL;
	
	/** @brief           һ�ν���ͼ��ͼָ��, ������˸ͼ��ʱ�Ļص�����*/
	m_pMapView = NULL;
	
	/** @brief           ������ֵҳ��ָ��, ���ڴ�ӡ��ֵ�Ļص�����*/
	m_pPTSetting = NULL;
	
	/** @brief           ����ģ����ҳ��ָ��, ���ڴ�ӡģ�����Ļص�����*/
	m_pPTAnalog = NULL;
	
	/** @brief           ����������ҳ��ָ��, ���ڴ�ӡ�������Ļص�����*/
	m_pPTDigital = NULL;
	
	/** @brief           ������ѹ��ҳ��ָ��, ���ڴ�ӡ��ѹ��Ļص�����*/
	m_pPTSoftBoard = NULL;
	
	/** @brief           ¼������ֵҳ��ָ��, ���ڴ�ӡ��ֵ�Ļص�����*/
	m_pWRSetting = NULL;

	m_pPTAction = NULL;
	m_pPTAlarm = NULL;
}

/*************************************************************
 �� �� ����LoadSysLogPath()
 ���ܸ�Ҫ�������ݿ��ж�ȡ��־�ļ�·��,������m_sSysLogPath��
 �� �� ֵ: void
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0158
void CXJBrowserApp::LoadSysLogPath()
{
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//��ѯcomtrade_path
	Field Field1;
	m_DBEngine.SetField(sql, Field1, "log_path", EX_STTP_DATA_TYPE_STRING);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_LOCAL_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::LoadSysLogPath, ��ѯʧ��");
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//��ѯ�ɹ�
		pMemset->MoveFirst();
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
			m_sSysLogPath = pMemset->GetValue((UINT)0);
			CString str;
			str.Format("CXJBrowserApp::LoadSysLogPath, ·��Ϊ:%s", m_sSysLogPath);
			WriteLog(str);
		}
		else
		{
			WriteLog("�Ҳ���ϵͳ����·��");
		}
		
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::LoadSysLogPath, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	if(m_sSysLogPath != "")
	{
		//���п��ܵ�"\\"������/
		m_sSysLogPath.Replace("\\", "/");
		if(m_sSysLogPath.Right(1)!="/")
			m_sSysLogPath += "/";
		if (m_sSysLogPath.Find(":") == -1)
		{
			if(m_sSysLogPath.Left(1) == ".")
			{
				CString strLocal = g_LocalPath;
				strLocal.Replace("\\", "/");
				m_sSysLogPath = strLocal + m_sSysLogPath;
			}
			else
			{
				if (m_sSysLogPath.Left(1) != "/")
				{
					m_sSysLogPath = "/" + m_sSysLogPath;
					m_sSysLogPath = "." + m_sSysLogPath;
					CString strLocal = g_LocalPath;
					strLocal.Replace("\\", "/");
					m_sSysLogPath = strLocal + m_sSysLogPath;
				}
				else
				{
					CString strLocal = g_LocalPath;
					int nFind = strLocal.Find(":", 0);
					if(nFind != -1)
					{
						CString sRoot = strLocal.Left(nFind+1);
						m_sSysLogPath = sRoot+m_sSysLogPath;
					}
				}
				
			}
		}
		
		//����ģ����
		m_sSysLogPath += "XJBrowser/";
	}
}

/*************************************************************
 �� �� ����GetSysLogPath()
 ���ܸ�Ҫ��ȡ����־�ļ�·��
 �� �� ֵ: ��־�ļ�·��
 ��    ����param1
		   Param2
**************************************************************/
//##ModelId=49B87B7E0167
CString CXJBrowserApp::GetSysLogPath()
{
	if(m_sSysLogPath == "")
		LoadSysLogPath();
	return m_sSysLogPath;
}

/*************************************************************
 �� �� ����AddNetworkDisk()
 ���ܸ�Ҫ����������洢��
 �� �� ֵ: �ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1
		   Param2
**************************************************************/
BOOL CXJBrowserApp::AddNetworkDisk()
{
	BOOL bReturn = FALSE;

	//�õ�����Ŀ¼��
	CString strDir = GetDownDir();
	//ȥ�������\\��
	while(strDir.Right(1) == "\\")
	{
		strDir.Delete(strDir.GetLength() - 1, 1);
	}

	NETRESOURCE nr;
	nr.dwType = RESOURCETYPE_ANY;
	nr.lpLocalName = NULL;
	nr.lpRemoteName = (LPSTR)(LPCTSTR)strDir;
	nr.lpProvider = NULL;

	CString str = "";
	DWORD dwReturn = 1;
	
	if(m_sNetworkPassword == "" && m_sNetworkUser == "")
	{
		dwReturn = WNetAddConnection2(&nr, (LPSTR)NULL, (LPSTR)NULL, FALSE);
	}
	else
	{
		dwReturn = WNetAddConnection2(&nr, m_sNetworkPassword, m_sNetworkUser, FALSE);
	}

	if(dwReturn == NO_ERROR)
	{
		bReturn = TRUE;
		CString strOut="";
		strOut.Format("���������������ɹ�, ��ַ: %s", strDir);
		WriteLog(strOut, XJ_LOG_LV1);
	}
	else
	{
		bReturn = FALSE;
		CString strOut="";
		strOut.Format("��������������ʧ��, ��ַ: %s", strDir);
		WriteLog(strOut, XJ_LOG_LV1);
		if(dwReturn == ERROR_ACCESS_DENIED)
		{
			WriteLog("Access to the network resource was denied", 3);
		}
		if(dwReturn == ERROR_ALREADY_ASSIGNED)
		{
			WriteLog("The local device specified by the lpLocalName member is already connected to a network resource", 3);
		}
		if(dwReturn == ERROR_BAD_DEV_TYPE)
		{
			WriteLog("The type of local device and the type of network resource do not match", 3);
		}
		if(dwReturn == ERROR_BAD_DEVICE)
		{
			WriteLog("The value specified by lpLocalName is invalid", 3);
		}
		if(dwReturn == ERROR_BAD_NET_NAME)
		{
			WriteLog("The value specified by the lpRemoteName member is not acceptable to any network resource provider, either because the resource name is invalid, or because the named resource cannot be located", 3);
		}
		if(dwReturn == ERROR_BAD_PROFILE)
		{
			WriteLog("The user profile is in an incorrect format", 3);
		}
		if(dwReturn == ERROR_BAD_PROVIDER)
		{
			WriteLog("The value specified by the lpProvider member does not match any provider", 3);
		}
		if(dwReturn == ERROR_BUSY)
		{
			WriteLog("The router or provider is busy, possibly initializing. The caller should retry", 3);
		}
		if(dwReturn == ERROR_CANCELLED)
		{
			WriteLog("The attempt to make the connection was cancelled by the user through a dialog box from one of the network resource providers, or by a called resource", 3);
		}
		if(dwReturn == ERROR_CANNOT_OPEN_PROFILE)
		{
			WriteLog("The system is unable to open the user profile to process persistent connections", 3);
		}
		if(dwReturn == ERROR_DEVICE_ALREADY_REMEMBERED)
		{
			WriteLog("An entry for the device specified by lpLocalName is already in the user profile", 3);
		}
		if(dwReturn == ERROR_EXTENDED_ERROR)
		{
			WriteLog("A network-specific error occurred. Call the WNetGetLastError function to obtain a description of the error", 3);
		}
		if(dwReturn == ERROR_INVALID_PASSWORD)
		{
			WriteLog("The specified password is invalid and the CONNECT_INTERACTIVE flag is not set", 3);
		}
		if(dwReturn == ERROR_NO_NET_OR_BAD_PATH)
		{
			WriteLog("The operation cannot be performed because a network component is not started or because a specified name cannot be used", 3);
		}
		if(dwReturn == ERROR_NO_NETWORK)
		{
			WriteLog("The network is unavailable", 3);
		}
	}
	return bReturn;
}

void CXJBrowserApp::LoadWndConfig()
{
	CString strIniFile = g_SysConfig;
	g_bShowActionWnd = GetPrivateProfileInt("WndConfig", "ActionWnd", 1, strIniFile);
	g_bShowAlarmWnd = GetPrivateProfileInt("WndConfig", "AlarmWnd", 1, strIniFile);
	g_bShowDIWnd = GetPrivateProfileInt("WndConfig", "DIWnd", 1, strIniFile);
	g_bShowFlautWnd = GetPrivateProfileInt("WndConfig", "FaultWnd", 1, strIniFile);
	g_bShowOSCWnd = GetPrivateProfileInt("WndConfig", "OSCWnd", 1, strIniFile);
	g_bShowPTCommWnd = GetPrivateProfileInt("WndConfig", "PTCommWnd", 1, strIniFile);
	g_bShowPTRunWnd = GetPrivateProfileInt("WndConfig", "PTRunWnd", 1, strIniFile);
	g_bShowSysAlarmWnd = GetPrivateProfileInt("WndConfig", "SysAlarmWnd", 1, strIniFile);
	g_bShowCrossWnd = GetPrivateProfileInt("WndConfig", "CrossWnd", 0, strIniFile);
	g_bShowSOEIWnd = GetPrivateProfileInt("WndConfig", "SOEIWnd", 0, strIniFile);			//�Ƿ���ʾI���¼�����
	g_bShowSOEIIWnd = GetPrivateProfileInt("WndConfig", "SOEIIWnd", 0, strIniFile);		//�Ƿ���ʾII���¼�����
	g_bShowSOEIIIWnd = GetPrivateProfileInt("WndConfig", "SOEIIIWnd", 0, strIniFile);		//�Ƿ���ʾIII���¼�����
	g_bShowSOEDebugWnd = GetPrivateProfileInt("WndConfig", "SOEDebugWnd", 0, strIniFile);		//�Ƿ���ʾ�����¼�����
	g_bShowGeoWnd = GetPrivateProfileInt("WndConfig", "GeoMapWnd", 0, strIniFile);		//�Ƿ���ʾ����ͼ����
	g_bShowDeviceMapWnd = GetPrivateProfileInt("WndConfig", "DeviceMapWnd", 1, strIniFile);		//�Ƿ���ʾһ�ν���ͼ����
	g_bShowDistanceWnd = GetPrivateProfileInt("WndConfig", "DistanceWnd", 0, strIniFile);		//�Ƿ���ʾС����ര��
	g_bShowStationInfoWnd = GetPrivateProfileInt("WndConfig", "StationInfoWnd", 0, strIniFile);		//�Ƿ���ʾ��վ��Ϣ����
	g_bShowRemindWnd = GetPrivateProfileInt("WndConfig", "RemindWnd", 1, strIniFile);		//�Ƿ���ʾ��վ��Ϣ����	
	g_bShowDraft = GetPrivateProfileInt("WndConfig", "ShowDraft", 0, strIniFile);
}

void CXJBrowserApp::SaveWndConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	str.Format("%d", g_bShowActionWnd);
	WritePrivateProfileString("WndConfig", "ActionWnd", str, strIniFile);
	str.Format("%d", g_bShowAlarmWnd);
	WritePrivateProfileString("WndConfig", "AlarmWnd", str, strIniFile);
	str.Format("%d", g_bShowDIWnd);
	WritePrivateProfileString("WndConfig", "DIWnd", str, strIniFile);
	str.Format("%d", g_bShowFlautWnd);
	WritePrivateProfileString("WndConfig", "FaultWnd", str, strIniFile);
	str.Format("%d", g_bShowOSCWnd);
	WritePrivateProfileString("WndConfig", "OSCWnd", str, strIniFile);
	str.Format("%d", g_bShowPTCommWnd);
	WritePrivateProfileString("WndConfig", "PTCommWnd", str, strIniFile);
	str.Format("%d", g_bShowPTRunWnd);
	WritePrivateProfileString("WndConfig", "PTRunWnd", str, strIniFile);
	str.Format("%d", g_bShowSysAlarmWnd);
	WritePrivateProfileString("WndConfig", "SysAlarmWnd", str, strIniFile);
	str.Format("%d", g_bShowCrossWnd);
	WritePrivateProfileString("WndConfig", "CrossWnd", str, strIniFile);
	str.Format("%d", g_bShowSOEIWnd);
	WritePrivateProfileString("WndConfig", "SOEIWnd", str, strIniFile);
	str.Format("%d", g_bShowSOEIIWnd);
	WritePrivateProfileString("WndConfig", "SOEIIWnd", str, strIniFile);
	str.Format("%d", g_bShowSOEIIIWnd);
	WritePrivateProfileString("WndConfig", "SOEIIIWnd", str, strIniFile);
	str.Format("%d", g_bShowSOEDebugWnd);
	WritePrivateProfileString("WndConfig", "SOEDebugWnd", str, strIniFile);
	str.Format("%d", g_bShowGeoWnd);
	WritePrivateProfileString("WndConfig", "GeoMapWnd", str, strIniFile);
	str.Format("%d", g_bShowDeviceMapWnd);
	WritePrivateProfileString("WndConfig", "DeviceMapWnd", str, strIniFile);
	str.Format("%d", g_bShowDistanceWnd);
	WritePrivateProfileString("WndConfig", "DistanceWnd", str, strIniFile);
	str.Format("%d", g_bShowStationInfoWnd);
	WritePrivateProfileString("WndConfig", "StationInfoWnd", str, strIniFile);
	str.Format("%d", g_bShowRemindWnd);
	WritePrivateProfileString("WndConfig", "RemindWnd", str, strIniFile);
	str.Format("%d", g_bShowDraft);
	WritePrivateProfileString("WndConfig", "ShowDraft", str, strIniFile);
}

int CXJBrowserApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
// 	CString str= "";
// 	m_DBEngine.XJCloseSession(str);
	CXJRootStore::Release();
	return CWinApp::ExitInstance();
}

/*************************************************************
 �� �� ��: ShowStationInfo()
 ���ܸ�Ҫ: ��ָ����վ�ĳ�վ��Ϣ��ͼ
 �� �� ֵ: 
 ��    ��: param1 ָ����վ
		   Param2 
**************************************************************/
void CXJBrowserApp::ShowStationInfo( CStationObj* pStation )
{
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return;
	pFrame->ShowStationInfo(pStation);
}

/*************************************************************
 �� �� ��: ShowDownOsc()
 ���ܸ�Ҫ: ��¼�����ش��ڣ�������ָ��¼��
 �� �� ֵ: 
 ��    ��: param1 ָ�����ص�¼��
		   Param2 
**************************************************************/
void CXJBrowserApp::ShowDownOsc( CXJEventOSC* pOsc )
{
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return;
	pFrame->ShowDownLoadOsc(pOsc);
}

/*************************************************************
 �� �� ��: AddSTTPData()
 ���ܸ�Ҫ: ��STTP���뵽����
 �� �� ֵ: 
 ��    ��: param1 sttp����
		   Param2 
**************************************************************/
void CXJBrowserApp::AddSTTPData( STTP_FULL_DATA* sttpData )
{
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  

		CString strLog;
		strLog.Format("��ʼ���뱨�ĵ����棬��ǰ�����С:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);
		
		m_listFullData.AddTail(sttpData);

		strLog.Format("���뱨�ĵ�����ɹ�����ǰ�����С:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);

		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		CString strLog;
		strLog.Format("���뱨�ĵ�����ʧ�ܣ���ǰ�����С:%d", m_listFullData.GetCount());
		WriteLog(strLog, XJ_LOG_LV3);
		LeaveCriticalSection(&m_CriticalSection);
	}	
}


/****************************************************
Date:2012/3/5  Author:LYH
������:   GetFirstSTTPData	
����ֵ:   STTP_FULL_DATA*	
���ܸ�Ҫ: 
*****************************************************/
STTP_FULL_DATA* CXJBrowserApp::GetFirstSTTPData()
{
	STTP_FULL_DATA* pSttp = NULL;
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  
		
		CString strLog;
		strLog.Format("��ʼ�ӻ���ȡ���ģ���ǰ�����С:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);
		
		POSITION pos = m_listFullData.GetHeadPosition();
		if(pos != NULL)
			pSttp = (STTP_FULL_DATA*)m_listFullData.GetAt(pos);
		
		strLog.Format("�ӻ���ȡ������ϣ���ǰ�����С:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);
		
		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		CString strLog;
		strLog.Format("�ӻ���ȡ����ʧ�ܣ���ǰ�����С:%d", m_listFullData.GetCount());
		WriteLog(strLog, XJ_LOG_LV3);
		LeaveCriticalSection(&m_CriticalSection);
	}
	return pSttp;
}


/*************************************************************
 �� �� ��: RemoveSTTPData()
 ���ܸ�Ҫ: ��STTP���еĵ�һ������ɾ��
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CXJBrowserApp::RemoveSTTPData()
{
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  

		CString strLog;
		strLog.Format("��ʼ�ӻ����Ƴ����ģ���ǰ�����С:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);
		
		POSITION pos = m_listFullData.GetHeadPosition();
		STTP_FULL_DATA* pSttp = NULL;
 		if(pos != NULL)
 		{
			pSttp = (STTP_FULL_DATA*)m_listFullData.GetHead();
			m_listFullData.RemoveHead();
		}
		strLog.Format("�ӻ����Ƴ����ĳɹ�����ǰ�����С:%d", m_listFullData.GetCount());
		//WriteLog(strLog, XJ_LOG_LV3);

		if(pSttp != NULL)
		{
			zero_sttp_full_data(*pSttp);
			delete pSttp;
		}
		strLog.Format("�ӻ����Ƴ����ĳɹ���ɾ�����ݳɹ�");
		//WriteLog(strLog, XJ_LOG_LV3);

		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		CString strLog;
		strLog.Format("�ӻ����Ƴ�����ʧ�ܣ���ǰ�����С:%d", m_listFullData.GetCount());
		WriteLog(strLog, XJ_LOG_LV3);

		LeaveCriticalSection(&m_CriticalSection);
	}	
}

/*************************************************************
 CreateDate: 29:1:2010     Author:LYH
 �� �� ��: LoadSplitterWndConfig()
 ���ܸ�Ҫ: ����ָ����ڴ�С����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CXJBrowserApp::LoadSplitterWndConfig()
{
	CString strIniFile = g_SysConfig;
	//��վͨ�ϴ��ڷָ���С
	g_nCommSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "CommSplitterWnd", 350, strIniFile);
	//�������ڷָ���С
	g_nActionSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "ActionSplitterWnd", 350, strIniFile);
	//�澯���ڷָ���С
	g_nAlarmSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "AlarmSplitterWnd", 350, strIniFile);
	//���Ϲ鵵���ڷָ���С
	g_nFaultSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "FaultSplitterWnd", 200, strIniFile);
	//ϵͳ�澯���ڷָ���С
	g_nSysAlarmSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "SysAlarmSplitterWnd", 350, strIniFile);
	//SOE��Ϣ���ڷָ���С
	g_nSOESplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "SOESplitterWnd", 350, strIniFile);
	//Ԥ�����ڷָ���С
	g_nCrossSplitterWnd = GetPrivateProfileInt("SplitterWndConfig", "CrossSplitterWnd", 350, strIniFile);

	g_nDraftWndWidth = GetPrivateProfileInt("SplitterWndConfig", "DraftWndWidth", 1024, strIniFile);
	g_nDraftWndHeight = GetPrivateProfileInt("SplitterWndConfig", "DraftWndHeight", 600, strIniFile);
}

/*************************************************************
 CreateDate: 29:1:2010     Author:LYH
 �� �� ��: SaveSplitterWndConfig()
 ���ܸ�Ҫ: ����ָ����ڴ�С����
 �� �� ֵ: 
 ��    ��: param1 
		   Param2 
**************************************************************/
void CXJBrowserApp::SaveSplitterWndConfig()
{
	CString strIniFile = g_SysConfig;
	CString str;
	
	//��ֵ��ʷֵ��������. 0-10����
	str.Format("%d", g_PTSettingValues);
	WritePrivateProfileString("PTPropConfig", "SettingValues", str, strIniFile);
	//��վͨ�ϴ��ڷָ���С
	str.Format("%d", g_nCommSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "CommSplitterWnd", str, strIniFile);
	//�������ڷָ���С
	str.Format("%d", g_nActionSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "ActionSplitterWnd", str, strIniFile);
	//�澯���ڷָ���С
	str.Format("%d", g_nAlarmSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "AlarmSplitterWnd", str, strIniFile);
	//���Ϲ鵵���ڷָ���С
	str.Format("%d", g_nFaultSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "FaultSplitterWnd", str, strIniFile);
	//ϵͳ�澯���ڷָ���С
	str.Format("%d", g_nSysAlarmSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "SysAlarmSplitterWnd", str, strIniFile);
	//SOE��Ϣ���ڷָ���С
	str.Format("%d", g_nSOESplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "SOESplitterWnd", str, strIniFile);
	//Ԥ�����ڷָ���С
	str.Format("%d", g_nCrossSplitterWnd);
	WritePrivateProfileString("SplitterWndConfig", "CrossSplitterWnd", str, strIniFile);

	str.Format("%d", g_nDraftWndWidth);
	WritePrivateProfileString("SplitterWndConfig", "DraftWndWidth", str, strIniFile);
	str.Format("%d", g_nDraftWndHeight);
	WritePrivateProfileString("SplitterWndConfig", "DraftWndHeight", str, strIniFile);
}


/*************************************************************
 CreateDate: 18:3:2010     Author:YYH
 �� �� ��: QueryPtRunStatus()
 ���ܸ�Ҫ: ��ѯ�����豸����״̬
 �� �� ֵ: void
 ��    ��: param1 �����豸ID
		   Param2 ����״̬
		   Param3 �豸����ʱ��
**************************************************************/
BOOL CXJBrowserApp::QueryPtRunStatus(CString strPtID, int& nRunStatus, CString& strRunTime)
{
	BOOL nReturn = TRUE;
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	
	//�ֶ�
	//status
	Field Field1;
	m_DBEngine.SetField(sql, Field1, "status", EX_STTP_DATA_TYPE_INT);
	//curtime
	Field Field2;
	m_DBEngine.SetField(sql, Field2, "curtime", EX_STTP_DATA_TYPE_STRING);
	
	CString str;
	//����
	//USER_ID
	Condition condition1;
	str.Format("pt_id = '%s'", strPtID);
	m_DBEngine.SetCondition(sql, condition1, str);

	Condition condition2;
	str = "order by curtime desc";
	m_DBEngine.SetCondition(sql, condition2, str, EX_STTP_WHERE_ABNORMALITY);
		
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_PT_RUN_CFG, sql, sError, pMemset);
		if (nResult)
		{
			nReturn = TRUE;
		}
		else
		{
			nReturn = FALSE;
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CDataEngine::QueryPtRunStatus, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		nReturn = FALSE;
	}
	if(pMemset != NULL && nResult == 1)
	{
		int nCount = pMemset->GetMemRowNum();
		if(nCount > 0)
		{
            nRunStatus = atoi(pMemset->GetValue((UINT)0));
			strRunTime = pMemset->GetValue((UINT)1);
			CString str;
			str.Format("CDataEngine::QueryPtRunStatus, ��ȡ��%d������", nCount);
			WriteLog(str, XJ_LOG_LV3);
			nReturn = TRUE;
		}
	}
	else
	{
		CString str;
		str.Format("CDataEngine::BuildNetList,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		nReturn = FALSE;	
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	return nReturn;
}

/*************************************************************
 CreateDate: 18:3:2010     Author:YYH
 �� �� ��: UpdatePtRunStatus()
 ���ܸ�Ҫ: ��ѯ�����豸����״̬
 �� �� ֵ: void
 ��    ��: param1 �����豸ID
           param2 ״ֵ̬
**************************************************************/
void CXJBrowserApp::UpdatePtRunStatus(CString strPtID, int nRunStatus)
{
	
	//�������µ�num����¼
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	
	CString str;	
	//�ֶ�
	//status
	Field Field1;
	str.Format("%d", nRunStatus);
	m_DBEngine.SetField(sql, Field1, "status", EX_STTP_DATA_TYPE_INT, str);
	
	//����
	//USER_ID
	Condition condition1;
	str.Format("pt_id = '%s'", strPtID);
	m_DBEngine.SetCondition(sql, condition1, str);
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJUpdateData(EX_STTP_INFO_FAULTSHORTEN_DATA, sql, sError);
	}
	catch (...)
	{
		WriteLog("CXJBrowserApp::UpdatePtRunStatus, ����ʧ��", XJ_LOG_LV3);
		delete[] sError;
	}
	if(nResult == 1)
	{
		str.Format("CXJBrowserApp::UpdatePtRunStatus,���³ɹ�");
	}
	else
	{
		str.Format("CXJBrowserApp::UpdatePtRunStatus,����ʧ��,ԭ��Ϊ%s", sError);
	}
	
	WriteLog(str, XJ_LOG_LV3);
	
	delete[] sError;
	sError = NULL;
}


void CXJBrowserApp::XJSetMenuInfo()
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	if (pWnd != NULL && pWnd->GetMenu() != NULL && g_style)
	{
		::SetMenuInfo(AfxGetMainWnd()->GetMenu()->m_hMenu, &lpcmi);
	}
}

/*
void CXJBrowserApp::OnAppCall() 
{
	// TODO: Add your command handler code here
	CMainFrame * pFrame = (CMainFrame*)AfxGetApp() ->m_pMainWnd;
	pFrame->OnToolAutocall();
	pFrame->OnToolAutocallResult();
}
*/

void CXJBrowserApp::OnCallAll() 
{
	// TODO: Add your command handler code here
	CMainFrame * pFrame = (CMainFrame*)AfxGetApp() ->m_pMainWnd;
	pFrame->OnToolAutocall();
}

void CXJBrowserApp::OnFileDownload() 
{
	// TODO: Add your command handler code here
	CMainFrame * pFrame = (CMainFrame*)AfxGetApp() ->m_pMainWnd;
	pFrame->OnToolDownloadFile();
}

/************************************
Date:2011/6/7  Author:LYH
������:    ShowStationInComm
����ֵ:   void
���ܸ�Ҫ: �ڳ�վ״̬ͼ������ʾ��վͼ��
����: CStationObj * pObj ָ����վ
����: BOOL bMoveFirst �Ƿ�ǰ��
************************************/
void CXJBrowserApp::ShowStationInComm( CStationObj* pObj, BOOL bMoveFirst /*= TRUE*/ )
{
	WriteLog(" CXJBrowserApp::ShowStationInComm, ��ʼ", XJ_LOG_LV3);
	if(pObj == NULL)
		return;
	
	//�õ�����ָ��
	CViewCommMap * pView = GetCommView();
	if(pView == NULL)
		return;
	
	//ǰ��
	if(bMoveFirst)
		pView->GetParentFrame()->ActivateFrame(SW_SHOWMAXIMIZED);
	
	//����
	pView->CenterDevice(pObj->m_sID);
}

void CXJBrowserApp::RemoveAllSTTPData()
{
	try 
	{
		EnterCriticalSection(&m_CriticalSection);  
		
		POSITION pos = m_listFullData.GetHeadPosition();
		while(pos != NULL)
		{
			STTP_FULL_DATA* sttpData = (STTP_FULL_DATA*)m_listFullData.GetNext(pos);
			if(sttpData != NULL)
				delete sttpData;	
		}
		m_listFullData.RemoveAll();
		LeaveCriticalSection(&m_CriticalSection);
	}
	catch(...) 
	{
		LeaveCriticalSection(&m_CriticalSection);
	}
}

CGeoView* CXJBrowserApp::GetGeoView()
{
	CXJBrowserDoc * pDoc = GetCurDocument();
	
	CGeoView * pGeoView = NULL;
	
	POSITION pos = pDoc->GetFirstViewPosition();
	CView * pView;
	while(pos!=NULL)
	{
		pView = pDoc->GetNextView(pos);
		if(pView->IsKindOf(RUNTIME_CLASS(CGeoView)))//�����ͨѶ״̬ͼ
		{
			pGeoView = (CGeoView *)pView;
			break;
		}			
	}
	
	return pGeoView;
}

/****************************************************
Date:2011/12/5  Author:LYH
������:   LockUI	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CXJBrowserApp::LockUI()
{
	if(!m_bCanLock)
		return;
	if(m_bIsLocked)
		return;
	m_bIsLocked = TRUE;
	CString str;
	str.Format("ϵͳ����%d����,��������", g_UILockTime);
	WriteLog(str, XJ_LOG_LV1);
	if(!DoLogin())
		m_pMainWnd->PostMessage(WM_CLOSE);
}

/****************************************************
Date:2012/3/13  Author:LYH
������:   JudgeOperTableType	
����ֵ:   void	
���ܸ�Ҫ: �ж���־������
*****************************************************/
void CXJBrowserApp::JudgeOperTableType()
{
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	Field Field2;
	m_DBEngine.SetField(sql, Field2, "1", EX_STTP_DATA_TYPE_TOP);
	
	CMemSet* pMemset;
	pMemset = new CMemSet;
	
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_OPERATION_CFG, sql, sError, pMemset);
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::JudgeOperTableType, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
	}
	if(pMemset != NULL && nResult == 1)
	{
		//��ѯ�ɹ�
		int nFieldCount = pMemset->GetMemFieldNum();
		if(nFieldCount > 7)
			g_OperTableType = 1;
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::JudgeOperTableType, ��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str);
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
	
	CString strLog;
	strLog.Format("ʹ��%s��־��", g_OperTableType==0?"��":"��");
	WriteLog(strLog, XJ_LOG_LV2);
}

/****************************************************
Date:2013/7/9  Author:LYH
������:   SetAppTile	
����ֵ:   void	
���ܸ�Ҫ: 
����: CString sTitle	
*****************************************************/
void CXJBrowserApp::SetAppTile( CString sTitle, CString sDetail /*= ""*/ )
{
	//�õ������ָ��
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	if(pFrame == NULL)
		return;
	CString str;
	if(sDetail != "")
		str.Format("%s - [%s] - [%s]", g_sAppName, sTitle, sDetail);
	else
		str.Format("%s - [%s]", g_sAppName, sTitle);
	pFrame->SetTitle(str);
	pFrame->UpdateFrameTitleForDocument("");
}

/****************************************************
Date:2014/5/22  Author:LYH
������:   	
����ֵ:   void	
���ܸ�Ҫ: 
*****************************************************/
void CXJBrowserApp::StartClearConfigThread()
{
	if (m_pClearConfigThread == NULL)
	{
		//������������豸�����߳�
		m_pClearConfigThread = AfxBeginThread(ClearSecConfig,this,THREAD_PRIORITY_BELOW_NORMAL, 0, CREATE_SUSPENDED);
		if(m_pClearConfigThread != NULL)
		{
			m_pClearConfigThread->m_bAutoDelete = FALSE;
			m_pClearConfigThread->ResumeThread();
		}
	}
}

void CXJBrowserApp::QueryUserStation()
{
	
	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//group_id
	Field Field1;
	m_DBEngine.SetField(sql, Field1, "station_id", EX_STTP_DATA_TYPE_STRING);
	
	CString str;
	//����
	//USER_ID
	Condition condition2;
	str.Format("user_id = '%s'", m_User.m_strUSER_ID);
	m_DBEngine.SetCondition(sql, condition2, str);
	
	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	nResult = m_DBEngine.XJSelectData(157/*tb_user_related*/, sql, sError, &pMemset);
	if(nResult == 1)
	{
		int nCount = pMemset.GetMemRowNum();
		if(nCount > 0)
		{
			m_User.m_bAllStation = FALSE;
			CString strLog;
			strLog.Format("�û�[%s]һ������%d����վ", m_User.m_strUSER_ID, nCount);
			WriteLog(strLog, XJ_LOG_LV2);
		}
	}
}

BOOL CXJBrowserApp::MgrStation(CString stationID)
{
	if(MgrAllStations())
		return TRUE;

	//�齨��ѯ����
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	CString str;
	//����
	//USER_ID
	Condition condition2;
	str.Format("user_id = '%s'", m_User.m_strUSER_ID);
	m_DBEngine.SetCondition(sql, condition2, str);

	Condition condition3;
	str.Format("station_id = '%s'", stationID);
	m_DBEngine.SetCondition(sql, condition3, str);
	
	CMemSet pMemset;
	
	char sError[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	nResult = m_DBEngine.XJSelectData(157/*tb_user_related*/, sql, sError, &pMemset);
	if(nResult == 1)
	{
		int nCount = pMemset.GetMemRowNum();
		if(nCount > 0)
		{
			CString strLog;
			strLog.Format("��վ[%s]���û�[%s]�Ĺ���Χ��", stationID, m_User.m_strUSER_ID);
			WriteLog(strLog, XJ_LOG_LV2);
			return TRUE;
		}
	}
	return FALSE;
}

CString CXJBrowserApp::GetUserGroupNameByID(CString sUserGroupID)
{
	CString sReturn;

	if (sUserGroupID.IsEmpty())
		return sReturn;

	CString str;
	
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//NAME
	Field fld0;
	m_DBEngine.SetField(sql, fld0, "NAME", EX_STTP_DATA_TYPE_STRING);
	
	//����
	//USER_ID
	Condition cond0;
	str.Format("GROUP_ID = '%s'", sUserGroupID);
	m_DBEngine.SetCondition(sql, cond0, str);
	
	CMemSet* pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = m_DBEngine.XJSelectData(EX_STTP_INFO_SYS_GROUP_CFG, sql, sError, pMemset);
		
		if (1 != nResult){
			sReturn = "";	
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::GetUserGroupNameByID, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;

		sReturn = "";
		
		return sReturn;
	}
	
	// ��ѯ�ɹ�
	if(NULL != pMemset)
	{
		int nCount = pMemset->GetMemRowNum();
		
		str.Format("CXJBrowserApp::GetUserGroupNameByID, ��ȡ��%d������", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		if(nCount > 0)
		{
			CString strVal = pMemset->GetValue((UINT)0);
			if (!strVal.IsEmpty()){
				sReturn = strVal;
			}
		}
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::GetUserGroupNameByID, ��ѯʧ��, ԭ��Ϊ��%s", sError);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		sReturn = "";	
	}
	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
    
	return sReturn;
}

int CXJBrowserApp::CheckKeyNameForPTSet()
{
	int nReturn = -1;

    CXJBrowserApp * pApp = (CXJBrowserApp*)AfxGetApp();
	CString str;
    
	SQL_DATA sql;
	sql.Conditionlist.clear();
	sql.Fieldlist.clear();
	
	//�ֶ�
	//value
	Field fld0;
	pApp->m_DBEngine.SetField(sql, fld0, "value", EX_STTP_DATA_TYPE_STRING);
	
	//����
	//keyname
	Condition condition1;
	str.Format("keyname = 'DZ_MOD_STATE'");
	pApp->m_DBEngine.SetCondition(sql, condition1, str);
	
	CMemSet* pMemset = new CMemSet;
	char * sError = new char[MAXMSGLEN];
	memset(sError, '\0', MAXMSGLEN);
	
	int nResult;
	try
	{
		nResult = pApp->m_DBEngine.XJSelectData(EX_STTP_INFO_TBSYSCONFIG, sql, sError, pMemset);
		
		if (1 != nResult)
		{
			nReturn = -1;
			//AfxMessageBox("CXJBrowserApp::CheckKeyNameForPTSet, ��ѯʧ��");
		}
	}
	catch (CException* e)
	{
		e->Delete();
		WriteLog("CXJBrowserApp::CheckKeyNameForPTSet, ��ѯʧ��", XJ_LOG_LV3);
		delete[] sError;
		delete pMemset;
		sError = NULL;

		nReturn = -1;
	}

	if(pMemset != NULL && nResult == 1)
	{
		int nCount = pMemset->GetMemRowNum();
		
		CString str;
		str.Format("CXJBrowserApp::CheckKeyNameForPTSet, ��ȡ��%d������", nCount);
		WriteLog(str, XJ_LOG_LV3);
		//AfxMessageBox(str);
		
		if(nCount > 0)
			return 0;

		// �����ڼ�¼ DZ_MOD_STATE
		//�齨��ѯ����
		SQL_DATA sql1;
		sql1.Conditionlist.clear();
		sql1.Fieldlist.clear();
		
		//�����ֶ�
		//����״̬�ؼ���
		str = "DZ_MOD_STATE";
		Field fld1;
		pApp->m_DBEngine.SetField(sql1, fld1, "keyname", EX_STTP_DATA_TYPE_STRING, str);
		Field fld2;
		pApp->m_DBEngine.SetField(sql1, fld2, "value", EX_STTP_DATA_TYPE_STRING, "0");
		
		memset(sError, '\0', MAXMSGLEN);
		
		//int nResult;
		try
		{
			nResult = pApp->m_DBEngine.XJInsertData(EX_STTP_INFO_TBSYSCONFIG, sql1, sError);

			if(nResult == 1)
			{
				//�ɹ�
				nReturn = 0;
			}
		}
		catch (...)
		{
			WriteLog("CXJBrowserApp::CheckKeyNameForPTSet.XJInsertData, ����ʧ��");
			delete[] sError;
			sError = NULL;
			
			nReturn = -1;
		}
	}
	else
	{
		CString str;
		str.Format("CXJBrowserApp::CheckKeyNameForPTSet,��ѯʧ��,ԭ��Ϊ%s", sError);
		WriteLog(str, XJ_LOG_LV3);

		//AfxMessageBox(str);
		nReturn = -1;	
	}

	delete[] sError;
	delete pMemset;
	sError = NULL;
	pMemset = NULL;
    
    return nReturn;
}
