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
	local=localtime(&tt);  //获取当前系统时间  
	
	char szLine[256] = {0};
	char *pxq[]={"日","一","二","三","四","五","六"};
	
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
		sprintf(szLine, "%4d-%02d-%02d %02d:%02d:%02d 星期%s"
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
		return QByteArray("挂牌设置");
	case XJ_FUNC_XJBROWSER_CONTROL:
		return QByteArray("控制功能");
	case XJ_FUNC_XJBROWSER_CUSTODY:
		return QByteArray("监护功能");
	}
	
	return s;
}

QByteArray GetUserGroupIDName(int nType/* = XJ_USERGROUP_RUNNER*/)
{
	QByteArray s;
	
	switch (nType){
	case XJ_USERGROUP_RUNNER:
		return QByteArray("运行用户");
	case XJ_USERGROUP_OPERATOR:
		return QByteArray("操作用户");
	case XJ_USERGROUP_MONITOR:
		return QByteArray("监护用户");
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
				nAdd = atoi(sPrecision);			//精度
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
		if (nMaohao<0 || nFenhao<0 || nMaohao>nFenhao || nMaohao == nFenhao-1 || nMaohao == 0) //若1.找不到分号或者冒号2.冒号在分号后面3.冒号在第一个4.或者冒号和分号是挨着的 返回错误
		{	
			return bRutern;
		}
		sCopy.Delete(0, nMaohao+1);
		nMaohao = sCopy.Find(':',0);
		nFenhao = sCopy.Find(';', 0);
		if (nMaohao<nFenhao && nMaohao != -1)   //第一个分号前有两个冒号 返回错误
		{
			return bRutern; 
		}
		sCopy.Delete(0,nFenhao+1); //删除当前第一个分号前的字符
	}
	return TRUE;
	
}

CString pts_GetTranslatedValue( PT_SETTING *pst,const CString strValue )
{
	//根据VPickList填充Value内容,VPickList不空时，Value值的解释根据VPivkList确定 
	//如VPickList="0:on;1:off" 而 Value=1 时，strValue为"off"
	//如VPickList为空，而Value=1，Unit="s" 则strValue = "1s"  
	CString strTemp;
	CString strUnit		= pst->Unit;
	CString strVPickList= pst->VPickList;
	CString strReturn	= strValue;//pst->Value;
	
	//去掉多余的空格
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
			int nFenhao =0;	//分号的位置
			int nMaohao =0; //冒号的位置
			BOOL bFound = FALSE;
			
			//只要strVPickList不空，则劈开分号之间的字符串，挨个检查冒号左边的值是否与strReturn相同
			//如果相同则stValue用冒号右边的字串取代
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
			str.Format("CPTSetting::CheckVPicklist,查询失败,原因为数据库vpicklist为%s,格式不对",strVPickList);
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
	
	if( pst->DataType == 2 && !strReturn.IsEmpty() ) //16进制控制字
	{
		//将16进制控制字转为0x0000 的格式，不足4位的将补全
		CString str = strReturn;
		str.MakeLower();
		int nfind = str.Find( "0x", 0 );
		if( nfind == 0 )
		{
			//以0x开头
			str.Delete( 0, 2 );
			strReturn.Delete( 0, 2 );
		}
		int noffset = 0;
		if( str.GetLength()%4 != 0 )
			noffset = 4 - str.GetLength() % 4;
		CString strAdd = "0x";
		if( nfind < 0 && g_PTSetting160x == 0 ) //本身没有0x，并且不强制添加
			strAdd = "";
		for ( int i = 0; i < noffset; i++ )
		{
			strAdd += "0";
		}
		strReturn = strAdd + strReturn;		
	}
	return QByteArray(strReturn.GetBuffer(0));
}
