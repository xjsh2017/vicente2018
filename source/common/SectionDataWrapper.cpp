/********************************************************************
	created:	2009/05/08
	created:	8:5:2009   16:57
	filename: 	\VICENTE\CODE\COMMON\SectionDataWrapper.cpp
	file path:	\VICENTE\CODE\COMMON
	file base:	SectionDataWrapper
	file ext:	cpp
	author:		qingch
	
	purpose:	
*********************************************************************/
#include "SectionDataWrapper.h"

CSectionDataWrapper::CSectionDataWrapper()
:m_hLibXJShareSD(NULL),m_pFunCloseSectionData(NULL),
 m_pFunCreateSectionData(NULL),m_pFunOpenSectionData(NULL),
 m_pFunSetFullSectionData(NULL),m_pFunGetFullSectionData(NULL),
 m_pFunSetSectionData(NULL),m_pFunGetSectionData(NULL),
 m_pFunSetPointData(NULL),m_pFunGetPointData(NULL)
{
	
}

CSectionDataWrapper::~CSectionDataWrapper()
{
	xj_free_library(m_hLibXJShareSD);		
}

bool CSectionDataWrapper::InitLibrary( char* pErrorMsg )
{
	m_hLibXJShareSD = xj_load_library(LIB_SHARESECTIONDATA_NAME);
	if (m_hLibXJShareSD != NULL)
	{
		m_pFunCreateSectionData = (PFUNCREATESECTIONDATA) xj_get_addr(m_hLibXJShareSD,
			"CreateSectionData");
		if (m_pFunCreateSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function CreateSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunOpenSectionData = (PFUNOPENSECTIONDATA) xj_get_addr(m_hLibXJShareSD,
			"OpenSectionData");
		if (m_pFunOpenSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function OpenSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunCloseSectionData = (PFUNCLOSESECTION) xj_get_addr(m_hLibXJShareSD,
			"CloseSectionData");
		if (m_pFunCloseSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function CloseSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunSetFullSectionData = (PFUNSETFULLSD) xj_get_addr(m_hLibXJShareSD,
			"SetFullSectionData");
		if (m_pFunSetFullSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function SetFullSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunGetFullSectionData = (PFUNGETFULLSD) xj_get_addr(m_hLibXJShareSD,
			"GetFullSectionData");
		if (m_pFunGetFullSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function GetFullSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunSetSectionData = (PFUNSETSD) xj_get_addr(m_hLibXJShareSD,
			"SetSectionData");
		if (m_pFunSetSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function SetSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}

		m_pFunGetSectionData = (PFUNGETSD) xj_get_addr(m_hLibXJShareSD,
			"GetSectionData");
		if (m_pFunGetSectionData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function GetSectionData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	
		
		m_pFunSetPointData = (PFUNSETPOINTDATA) xj_get_addr(m_hLibXJShareSD,
			"SetPointData");
		if (m_pFunSetPointData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function SetPointData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}
		
		m_pFunGetPointData = (PFUNGETPOINTDATA) xj_get_addr(m_hLibXJShareSD,
			"GetPointData");
		if (m_pFunGetPointData == NULL) {
			if (NULL != pErrorMsg)
			{	
				sprintf(pErrorMsg,"(%s)load function GetPointData failed",LIB_SHARESECTIONDATA_NAME);
			}
			return false;
		}	

	} 
	else
	{
		if (NULL != pErrorMsg)
		{	
			sprintf(pErrorMsg,"LoadShareSectionData Lib failed(reason:%s)",strerror(errno));
		}
		return false;
	}
	return true;		
}

/*************************************************************
 �� �� ��:   CreateSectionData
 ���ܸ�Ҫ:   �����������ݹ����ڴ�
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   string pSHMName �����ڴ���
 ��    ��:   vector<string> & pStationIDList ��վID�б�
 ��    ��:   string pLogPath ��־·��
 ��    ��:   int pLogLevel ��־����
 ��    ��:   string & pErrorMsg ������Ϣ
**************************************************************/
bool CSectionDataWrapper::CreateSectionData( string pSHMName, vector<string>& pStationIDList, string pLogPath, int pLogLevel, string& pErrorMsg )
{
	if (m_pFunCreateSectionData != NULL)
	{
		return m_pFunCreateSectionData(pSHMName,pStationIDList,pLogPath,pLogLevel,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "��Ч�ĺ���ָ�루CreateSectionData��";
		return false;
	}
}

/*************************************************************
 �� �� ��:   OpenSectionData
 ���ܸ�Ҫ:   �򿪶������ݹ����ڴ�
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   string pSHMName �����ڴ���
 ��    ��:   string pLogPath ��־·��
 ��    ��:   int pLogLevel ��־����
 ��    ��:   string & pErrorMsg ������Ϣ
**************************************************************/
bool CSectionDataWrapper::OpenSectionData( string pSHMName ,string pLogPath, int pLogLevel, string& pErrorMsg )
{
	if (m_pFunOpenSectionData != NULL)
	{
		return m_pFunOpenSectionData(pSHMName,pLogPath,pLogLevel,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "��Ч�ĺ���ָ�루OpenSectionData��";
		return false;
	}	
}

/*************************************************************
 �� �� ��:   CloseSectionData
 ���ܸ�Ҫ:   �رն������ݹ����ڴ�
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   string & pErrorMsg ������Ϣ
**************************************************************/
bool CSectionDataWrapper::CloseSectionData( string& pErrorMsg )
{
	if (m_pFunCloseSectionData!=NULL)
	{
		return m_pFunCloseSectionData(pErrorMsg);
	} 
	else
	{
		pErrorMsg = "��Ч�ĺ���ָ�루CloseSectionData��";
		return false;
	}		
}

/*************************************************************
 �� �� ��:   SetFullSectionData
 ���ܸ�Ҫ:   ����ĳվȫ����������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   string pStationId ��վ���
 ��    ��:   const string & pSectionData �������ݿ�
 ��    ��:   string & pErrorMsg ������Ϣ
**************************************************************/
bool CSectionDataWrapper::SetFullSectionData( string pStationId, const string& pSectionData, string& pErrorMsg )
{
	if (m_pFunSetFullSectionData!=NULL)
	{
		return m_pFunSetFullSectionData(pStationId,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "��Ч�ĺ���ָ�루SetFullSectionData��";
		return false;
	}	
}

/*************************************************************
 �� �� ��:   GetFullSectionData
 ���ܸ�Ҫ:   ��ȡĳվȫ����������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   string pStationId ��վ���
 ��    ��:   string & pSectionData �������ݿ�
 ��    ��:   string & pErrorMsg ������Ϣ
**************************************************************/
bool CSectionDataWrapper::GetFullSectionData( string pStationId, string& pSectionData, string& pErrorMsg )
{
	if (m_pFunGetFullSectionData!=NULL)
	{
		return m_pFunGetFullSectionData(pStationId,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "��Ч�ĺ���ָ�루GetFullSectionData��";
		return false;
	}		
}

/*************************************************************
 �� �� ��:   SetSectionData
 ���ܸ�Ҫ:   ����ĳվ���ֶ�������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   string pStationId ��վ���
 ��    ��:   int pOffSet �������ݿ���ʼ��ַ
 ��    ��:   int pSize �������ݿ��С
 ��    ��:   const string & pSectionData �������ݿ�
 ��    ��:   string & pErrorMsg ������Ϣ
**************************************************************/
bool CSectionDataWrapper::SetSectionData( string pStationId, int pOffSet, int pSize, const string& pSectionData, string& pErrorMsg )
{
	if (m_pFunSetSectionData!=NULL)
	{
		return m_pFunSetSectionData(pStationId,pOffSet,pSize,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "��Ч�ĺ���ָ�루SetSectionData��";
		return false;
	}
}

/*************************************************************
 �� �� ��:   GetSectionData
 ���ܸ�Ҫ:   ��ȡĳվ���ֶ�������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   string pStationId ��վ���
 ��    ��:   int pOffSet �������ݿ���ʼ��ַ
 ��    ��:   int pSize �������ݿ��С
 ��    ��:   string & pSectionData �������ݿ�
 ��    ��:   string & pErrorMsg ������Ϣ
**************************************************************/
bool CSectionDataWrapper::GetSectionData( string pStationId, int pOffSet, int pSize, string& pSectionData, string& pErrorMsg )
{
	if (m_pFunGetSectionData!=NULL)
	{
		return m_pFunGetSectionData(pStationId,pOffSet,pSize,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "��Ч�ĺ���ָ�루GetSectionData��";
		return false;
	}	
}

/*************************************************************
 �� �� ��:   SetPointData
 ���ܸ�Ҫ:   ����ĳ���������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   string pStationId ��վ���
 ��    ��:   int pRow �к�
 ��    ��:   int pColumn �к�
 ��    ��:   const string & pSectionData ��������
 ��    ��:   string & pErrorMsg ������Ϣ
**************************************************************/
bool CSectionDataWrapper::SetPointData( string pStationId, int pRow, int pColumn, const string& pSectionData, string& pErrorMsg )
{
	if (m_pFunSetPointData!=NULL)
	{
		return m_pFunSetPointData(pStationId,pRow,pColumn,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "��Ч�ĺ���ָ�루SetPointData��";
		return false;
	}		
}

/*************************************************************
 �� �� ��:   GetPointData
 ���ܸ�Ҫ:   ��ȡĳ���������
 �� �� ֵ:   bool true-�ɹ� false-ʧ��
 ��    ��:   string pStationId ��վ���
 ��    ��:   int pRow �к�
 ��    ��:   int pColumn �к�
 ��    ��:   string & pSectionData ��������
 ��    ��:   string & pErrorMsg ������Ϣ
**************************************************************/
bool CSectionDataWrapper::GetPointData( string pStationId, int pRow, int pColumn, string& pSectionData, string& pErrorMsg )
{
	if (m_pFunGetPointData!=NULL)
	{
		return m_pFunGetPointData(pStationId,pRow,pColumn,pSectionData,pErrorMsg);
	} 
	else
	{
		pErrorMsg = "��Ч�ĺ���ָ�루GetPointData��";
		return false;
	}	
}