#include "vccommon_define.h"
#include "LogMgr.h"

void WriteLog(CString sLog, int nLogLevel/* = 3*/)
{
	CLogMgr* pMgr = CLogMgr::getInstance();
	if(pMgr != NULL)
		pMgr->WriteLog(sLog, nLogLevel);
}

void WriteLogTemp(CString sLog, int nLogLevel/* = 3*/)
{

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
	if(nDay < 0 || nDay > 31)
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
	if(nDay < 0 || nDay > 31)
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
	if(nDay < 0 || nDay > 31)
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
	delete [] a;
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