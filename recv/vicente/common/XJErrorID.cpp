/********************************************************************
	created:	2009/02/20
	created:	20:2:2009   10:49
	file base:	XJErrorID
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/

#include "XJErrorID.h"

char* CommErrMsg[] = {
	/* 0 */ "No error",
	/* 1 */ " Memeroy is not enough ",
	/* 2 */ "Failed in Loading lib",
	/* 3 */ " Failed in Getting Function Address",
	/* 4 */ "Runtime Exception",
	/* 5 */ "Invalid Address or Point",
	/* 6 */ "Failed in Creating Thread",
	/* 7 */ " Failed in Ending Thread ",
	/* 8 */ "Verify Failed",
	/* 9 */ "Can't find the function point",
	/* 10 */ "Sttp translat to Data Failed",
	/* 11 */ "Data translat to Sttp Failed",
};


char* BusinessErrMsg[] = {
	/* 0 */ "No error",
	/* 1 */ "��վ�����վͨѶ�ж�",
	/* 2 */ "����վ���ڳ�ʼ������",
	/* 3 */ "�·�������վ��������Ч",
	/* 4 */ "����վ�ظ�����ʧ��",
	/* 5 */ "����վ�Ļظ���Ч",
	/* 6 */ "���ܸ���վ�Ļظ���ʱ",
	/* 7 */ "�Ը���վ�����ݴ���ʧ��",
	/* 8 */ "�����������쳣",
	/* 9 */ "��Ч����������",
	/* 10 */ "Sttp�ͻ��˻�������",
	/* 11 */ "Sttp�ͻ��������������ж�",
	/* 12 */ "�ͻ��˽��ճ�ʱ",
	/* 13 */ "��������ʧ�ܣ���վ��Լ�����δ����",
};

char* OtherErrMsg[] = {
	/* 0 */ "No error",
};

char* getErrorMsg(int pErrorNo)
{
	if ((pErrorNo < MYDEF_ERROR_START_NO) || (pErrorNo > MYDEF_ERROR_END_NO))
	{
		return "No Error";
	}
	else {
		int offSet(0);
		if (pErrorNo <= COM_ERROR_END_NO){ /* ͨ�ô��� */
			offSet = pErrorNo - COM_ERROR_START_NO;
			if (offSet > 11 )
			{
				return "No Error";
			} 
			else
			{
				return CommErrMsg[offSet];
			}
		}
		else if (pErrorNo <= BUSINESS_ERROR_END_NO){ /* ҵ����� */
			offSet = pErrorNo - BUSINESS_ERROR_START_NO;
			if (offSet > 13 )
			{
				return "No Error";
			} 
			else
			{
				return BusinessErrMsg[offSet];
			}
		}
		else { /* �������� */
			offSet = pErrorNo - OTHER_ERROR_START_NO;
			if (offSet > 0 )
			{
				return "No Error";
			} 
			else
			{
				return OtherErrMsg[offSet];
			}
		}
	}
}