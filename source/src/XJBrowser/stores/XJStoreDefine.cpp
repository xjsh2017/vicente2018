// XJStoreDefine.cpp : implementation file
//

#include "StdAfx.h"
#include "XJStoreDefine.h"

#include <time.h>
#include <stdio.h>

QByteArray GetTime(int nType)
{
	time_t tt = time(NULL);
	struct tm *local;  
	local=localtime(&tt);  //��ȡ��ǰϵͳʱ��  
	
	char szLine[256] = {0};
	char *pxq[]={"��","һ","��","��","��","��","��"};
	
	if (nType == 0){
		sprintf(szLine, "%4d-%02d-%02d %02d:%02d:%02d"
			, local->tm_year + 1900
			, local->tm_mon + 1
			, local->tm_mday
			, local->tm_hour
			, local->tm_min
			, local->tm_sec);
	}else if (1 == nType){
		sprintf(szLine, "%4d-%02d-%02d"
			, local->tm_year + 1900
			, local->tm_mon + 1
			, local->tm_mday);
	}else{
		sprintf(szLine, "%4d-%02d-%02d %02d:%02d:%02d ����%s"
			, local->tm_year + 1900
			, local->tm_mon + 1
			, local->tm_mday
			, local->tm_hour
			, local->tm_min
			, local->tm_sec
			, pxq[local->tm_wday]);
	}
	
	return QByteArray(szLine);
}

QByteArray GetFuncIDName(int nType/* = XJ_FUNC_XJBROWSER_TAGOUT*/)
{
	QByteArray s;
	
	switch (nType){
	case XJ_FUNC_XJBROWSER_TAGOUT:
		return QByteArray("��������");
	case XJ_FUNC_XJBROWSER_CONTROL:
		return QByteArray("���ƹ���");
	case XJ_FUNC_XJBROWSER_CUSTODY:
		return QByteArray("�໤����");
	}
	
	return s;
}

QByteArray GetUserGroupIDName(int nType/* = XJ_USERGROUP_RUNNER*/)
{
	QByteArray s;
	
	switch (nType){
	case XJ_USERGROUP_RUNNER:
		return QByteArray("�����û�");
	case XJ_USERGROUP_OPERATOR:
		return QByteArray("�����û�");
	case XJ_USERGROUP_MONITOR:
		return QByteArray("�໤�û�");
	}
	
	return s;
}

CString pts_SetFloatValue(PT_SETTING *pst, CString strValue)
{
	if (pst==NULL)
	{
		return strValue;
	}
	if (pst->DataType == 0)
	{
		CString sValue = strValue;
		if (sValue != "")
		{
			CString sPrecision = pst->Precision;
			int nDouhao = sPrecision.Find(",");
			sPrecision.Delete(0,nDouhao+1);
			int nAdd = 3;
			sPrecision.TrimLeft();
			sPrecision.TrimRight();
			if(!sPrecision.IsEmpty())
			{
				nAdd = atoi(sPrecision);			//����
			}
			
			float fValue = atof(sValue);
			CString sFormat;
			sFormat.Format("%%.0%df", nAdd);
			sValue.Format(sFormat, fValue);
			return sValue;
		}
	}
	return strValue;
}

BOOL pts_CheckVPicklist(CString strVPickList)
{
	BOOL bRutern = FALSE;
	if (strVPickList == "")
	{
		return bRutern;
	}
	CString sCopy = strVPickList;
	while (!sCopy.IsEmpty())
	{
		int nMaohao = sCopy.Find(':',0);
		int nFenhao = sCopy.Find(';', 0);
		if (nMaohao<0 || nFenhao<0 || nMaohao>nFenhao || nMaohao == nFenhao-1 || nMaohao == 0) //��1.�Ҳ����ֺŻ���ð��2.ð���ڷֺź���3.ð���ڵ�һ��4.����ð�źͷֺ��ǰ��ŵ� ���ش���
		{	
			return bRutern;
		}
		sCopy.Delete(0, nMaohao+1);
		nMaohao = sCopy.Find(':',0);
		nFenhao = sCopy.Find(';', 0);
		if (nMaohao<nFenhao && nMaohao != -1)   //��һ���ֺ�ǰ������ð�� ���ش���
		{
			return bRutern; 
		}
		sCopy.Delete(0,nFenhao+1); //ɾ����ǰ��һ���ֺ�ǰ���ַ�
	}
	return TRUE;
	
}

CString pts_GetTranslatedValue( PT_SETTING *pst,const CString strValue )
{
	//����VPickList���Value����,VPickList����ʱ��Valueֵ�Ľ��͸���VPivkListȷ�� 
	//��VPickList="0:on;1:off" �� Value=1 ʱ��strValueΪ"off"
	//��VPickListΪ�գ���Value=1��Unit="s" ��strValue = "1s"  
	CString strTemp;
	CString strUnit		= pst->Unit;
	CString strVPickList= pst->VPickList;
	CString strReturn	= strValue;//pst->Value;
	
	//ȥ������Ŀո�
	strUnit.TrimLeft();
	strUnit.TrimRight();
	strReturn.TrimLeft();
	strReturn.TrimRight();
	strVPickList.TrimLeft();
	strVPickList.TrimRight();
	if (strVPickList.IsEmpty())
	{
	}
	else
	{
		if (pts_CheckVPicklist(strVPickList))
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
		}
		else
		{
			CString str;
			str.Format("CPTSetting::CheckVPicklist,��ѯʧ��,ԭ��Ϊ���ݿ�vpicklistΪ%s,��ʽ����",strVPickList);
			WriteLog(str, XJ_LOG_LV3);
		}
	}
	return strReturn;
}

QByteArray pts_GetDisplayValue( PT_SETTING* pst, QByteArray &baValue )
{
	CString strReturn = pts_SetFloatValue(pst, baValue.constData());
	strReturn = pts_GetTranslatedValue(pst, strReturn);
	if(pst->ntimeMStoS == 1)
	{
		float fValue = atof(strReturn);
		fValue /= 1000;
		strReturn.Format("%.3f", fValue);
	}
	
	if( pst->DataType == 2 && !strReturn.IsEmpty() ) //16���ƿ�����
	{
		//��16���ƿ�����תΪ0x0000 �ĸ�ʽ������4λ�Ľ���ȫ
		CString str = strReturn;
		str.MakeLower();
		int nfind = str.Find( "0x", 0 );
		if( nfind == 0 )
		{
			//��0x��ͷ
			str.Delete( 0, 2 );
			strReturn.Delete( 0, 2 );
		}
		int noffset = 0;
		if( str.GetLength()%4 != 0 )
			noffset = 4 - str.GetLength() % 4;
		CString strAdd = "0x";
		if( nfind < 0 && g_PTSetting160x == 0 ) //����û��0x�����Ҳ�ǿ�����
			strAdd = "";
		for ( int i = 0; i < noffset; i++ )
		{
			strAdd += "0";
		}
		strReturn = strAdd + strReturn;		
	}
	return QByteArray(strReturn.GetBuffer(0));
}
