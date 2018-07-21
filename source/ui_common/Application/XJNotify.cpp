#include "XJNotify.h"

/****************************************************
Date:2011/10/19  Author:LYH
������:   CXJNotify	
����ֵ:   	
���ܸ�Ҫ: 
����: CDataEngine * pDataEngine	
*****************************************************/
CXJNotify::CXJNotify( CDataEngine* pDataEngine )
{
	m_pDataEngine = pDataEngine;
}

/****************************************************
Date:2011/10/19  Author:LYH
������:   ~CXJNotify	
����ֵ:   	
���ܸ�Ҫ: 
*****************************************************/
CXJNotify::~CXJNotify()
{

}

/*************************************************************
 �� �� ����GetDescribe()
 ���ܸ�Ҫ���õ��¼���ϸ����,������ʵ��
 �� �� ֵ: �¼�����
**************************************************************/
CString CXJNotify::GetDescribe()
{
	CString str("�������¼�����");
	return str;
}

/*************************************************************
 �� �� ����LoadFromDB()
 ���ܸ�Ҫ����ȡ���ݿ��е�һ����¼, ��д�뵽���¼�������,
			���ô˺���֮ǰ�Ĳ�ѯ���ѯ���ֶ�,����select * from table where XXX
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
**************************************************************/
BOOL CXJNotify::LoadFromDB(CMemSet * pMemSet)
{
	//ʹ�ô˺����豣֤����ǰ��
	//1. ��ѯʱ�õ���select *��ѯ�������ֶ�
	//2. Memset���ƶ��ں��������, �����ڲ�����MemSet�Ѿ�����ȷ��λ��
	return TRUE;
}

/*************************************************************
 �� �� ����LoadFromSttp()
 ���ܸ�Ҫ����ȡһ��STTP����, ��д�뵽���¼�������
 �� �� ֵ: ��ȡ�ɹ�����TRUE, ʧ�ܷ���FALSE
 ��    ����param1	STTP�������ݽṹ
		   Param2	ָ���Ǳ��������еĵڼ���������
**************************************************************/
BOOL CXJNotify::LoadFromSttp(STTP_FULL_DATA& sttpData, int nIndex)
{
	//�����и���ʵ��
	return TRUE;
}

/****************************************************
Date:2011/10/19  Author:LYH
������:   Clone	
����ֵ:   CXJNotify*	
���ܸ�Ҫ: 
*****************************************************/
CXJNotify* CXJNotify::Clone()
{
	return NULL;
}

/*************************************************************
 �� �� ����GetFaultType()
 ���ܸ�Ҫ����strFaultType���ͳ�����
 �� �� ֵ: ���ͺ������
**************************************************************/
CString CXJNotify::GetFaultType(CString sFaultType)
{
	CString strTemp;
	sFaultType.MakeUpper();
	if ((sFaultType == "AG") || (sFaultType == "AN") || (sFaultType == "A"))
	{
		strTemp = "A��ӵ�";
	}
	else if ((sFaultType == "BG") || (sFaultType == "BN") || (sFaultType == "B"))
	{
		strTemp = "B��ӵ�";
	}
	else if ((sFaultType == "CG") || (sFaultType == "CN") || (sFaultType == "C"))
	{
		strTemp = "C��ӵ�";
	}
	else if ((sFaultType == "AB") || (sFaultType == "BA"))
	{
		strTemp = "AB�����·";
	}
	else if ((sFaultType == "BC") || (sFaultType == "CB"))
	{
		strTemp = "BC�����·";
	}
	else if ((sFaultType == "CA") || (sFaultType == "AC"))
	{
		strTemp = "CA�����·";
	}
	else if ((sFaultType == "ABG") || (sFaultType == "ABN") || (sFaultType == "BAG") || (sFaultType == "BAN"))
	{
		strTemp = "AB�����·�ӵ�";
	}
	else if ((sFaultType == "BCG") || (sFaultType == "BCN") || (sFaultType == "CBG") || (sFaultType == "CBN"))
	{
		sFaultType = "BC�����·�ӵ�";
	}
	else if ((sFaultType == "CAG") || (sFaultType == "CAN") || (sFaultType == "ACG") || (sFaultType == "ACN"))
	{
		strTemp = "CA�����·�ӵ�";
	}
	else if (sFaultType == "ABC")
	{
		strTemp = "ABC�����·";
	}
	else if ((sFaultType == "ABCG") || (sFaultType == "ABCN"))
	{
		strTemp = "ABC����ӵ�";
	}	
	else
	{
		strTemp = "δ֪����";
		strTemp += sFaultType;
	}

	return strTemp;
}

/*************************************************************
 �� �� ����GetTriggerType()
 ���ܸ�Ҫ���õ����˵��
 �� �� ֵ: ���˵��
 ��    ����param1	����д
		   Param2
**************************************************************/
CString CXJNotify::GetTriggerType( CString sTrigger )
{
	if(sTrigger == "")
		return "";
	CString sReturn;
	sReturn = sTrigger;
	if(sTrigger.GetLength() == 1)
	{
		sReturn += "��";
	}
	else if(sTrigger.GetLength() == 2)
	{
		sReturn += "����";
	}
	else if(sTrigger.GetLength() == 3)
	{
		sReturn += "����";
	}
	return sReturn;
}

/****************************************************
Date:2011/10/19  Author:LYH
������:   BuildReleation	
����ֵ:   BOOL	�ɹ�����TRUE, ʧ�ܷ���FALSE
���ܸ�Ҫ: �������豸�Ĺ���
*****************************************************/
BOOL CXJNotify::BuildReleation()
{
	return FALSE;
}

/****************************************************
Date:2011/10/31  Author:LYH
������:   GetNotifyType	
����ֵ:   int	
���ܸ�Ҫ: 
*****************************************************/
int CXJNotify::GetNotifyType()
{
	return -1;
}

/****************************************************
Date:2011/11/8  Author:LYH
������:   GetSimpleDescribe	
����ֵ:   CString	
���ܸ�Ҫ: 
*****************************************************/
CString CXJNotify::GetSimpleDescribe()
{
	return "��Ҫ����";
}
