/********************************************************************
	created:	2009/05/08
	created:	8:5:2009   16:57
	filename: 	\VICENTE\CODE\COMMON\SectionDataWrapper.h
	file path:	\VICENTE\CODE\COMMON
	file base:	SectionDataWrapper
	file ext:	h
	author:		qingch
	
	purpose:	
*********************************************************************/

#pragma   warning   (disable   :   4786) 
#pragma   warning   (disable   :   4275) 

#ifndef SectionDataWrapper_h__
#define SectionDataWrapper_h__

#include "define.h"
#include "xjlib.h"


#ifdef OS_LINUX

/** @brief            �������ݹ�������ƶ���*/
#define LIB_SHARESECTIONDATA_NAME              "libXJShareSectionData.so"

#endif

#ifdef OS_WINDOWS

/** @brief            �������ݹ�������ƶ���*/
#define LIB_SHARESECTIONDATA_NAME              "XJShareSectionData.dll"

#endif

/** @brief            CreateSectionData��������*/
typedef bool (*PFUNCREATESECTIONDATA)(string,vector<string>&,string,int,string&);

/** @brief            OpenSectionData��������*/
typedef bool (*PFUNOPENSECTIONDATA)(string,string,int,string&);

/** @brief            CloseSectionData��������*/
typedef bool (*PFUNCLOSESECTION)(string&);

/** @brief            SetFullSectionData��������*/
typedef bool (*PFUNSETFULLSD)(string,const string&,string&);

/** @brief            GetFullSectionData��������*/
typedef bool (*PFUNGETFULLSD)(string,string&,string&);

/** @brief            SetSectionData��������*/
typedef bool (*PFUNSETSD)(string,int,int,const string&,string&);

/** @brief            GetSectionData��������*/
typedef bool (*PFUNGETSD)(string,int,int,string&,string&);

/** @brief            SetPointData��������*/
typedef bool (*PFUNSETPOINTDATA)(string,int,int,const string&,string&);

/** @brief            GetPointData��������*/
typedef bool (*PFUNGETPOINTDATA)(string,int,int,string&,string&);

class CSectionDataWrapper
{
public:
	CSectionDataWrapper();
	~CSectionDataWrapper();
	bool InitLibrary(char* pErrorMsg);
	/**
	* @brief         �����������ݹ����ڴ�
	* @param[in]     pSHMName:�����ڴ���
	* @param[in]     pStationIDList:��վID�б�	
	* @param[in]     pLogPath:��־·��
	* @param[in]     pLogLevel:��־���� 
	* @param[out]    pErrorMsg:������Ϣ 
	* @return        ��������Ľ�� bool true-�ɹ� false-ʧ��
	*
    */
	bool CreateSectionData(string pSHMName, vector<string>&	pStationIDList, string pLogPath, int pLogLevel, string& pErrorMsg);

	/**
	* @brief         �򿪶������ݹ����ڴ� 
	* @param[in]     pSHMName:�����ڴ���
	* @param[in]     pLogPath:��־·��
	* @param[in]     pLogLevel:��־���� 
	* @param[out]    pErrorMsg:������Ϣ 
	* @return        ��������Ľ�� true-�ɹ� false-ʧ��
	*
    */
	bool OpenSectionData(string pSHMName ,string pLogPath, int pLogLevel, string& pErrorMsg);

	/**
	* @brief         �رն������ݹ����ڴ�
	* @param[out]    pErrorMsg:������Ϣ 
	* @return        ֹͣ����Ľ�� true-�ɹ� false-ʧ��
	*
    */
	bool CloseSectionData(string& pErrorMsg);

	/**
	* @brief         ����ĳվȫ����������
	* @param[in]     pStationId:��վ��� 
	* @param[in]     pSectionData:�������ݿ�
	* @param[out]    pErrorMsg:������Ϣ 
	* @return        ������� true-�ɹ� false-ʧ��
	*
    */
	bool SetFullSectionData(string pStationId, const string& pSectionData, string& pErrorMsg);

	/**
	* @brief         ��ȡĳվȫ����������
	* @param[in]     pStationId:��վ��� 
	* @param[out]    pSectionData:�������ݿ�
	* @param[out]    pErrorMsg:������Ϣ 
	* @return        ������� true-�ɹ� false-ʧ��
	*
    */
	bool GetFullSectionData(string pStationId, string& pSectionData, string& pErrorMsg);

	/**
	* @brief         ����ĳվ���ֶ�������
	* @param[in]     pStationId:��վ��� 
	* @param[in]     pOffSet:�������ݿ���ʼ��ַ
	* @param[in]     pSize:�������ݿ��С
	* @param[in]     pSectionData:�������ݿ�
	* @param[out]    pErrorMsg:������Ϣ 
	* @return        ������� true-�ɹ� false-ʧ��
	*
    */
	bool SetSectionData(string pStationId, int pOffSet, int pSize, const string& pSectionData, string& pErrorMsg);

	/**
	* @brief         ��ȡĳվ���ֶ�������
	* @param[in]     pStationId:��վ��� 
	* @param[in]     pOffSet:�������ݿ���ʼ��ַ
	* @param[in]     pSize:�������ݿ��С
	* @param[out]    pSectionData:�������ݿ�
	* @param[out]    pErrorMsg:������Ϣ 
	* @return        ������� true-�ɹ� false-ʧ��
	*
    */
	bool GetSectionData(string pStationId, int pOffSet, int pSize, string& pSectionData, string& pErrorMsg);

	/**
	* @brief         ����ĳ���������
	* @param[in]     pStationId:��վ��� 
	* @param[in]     pRow:�к� 
	* @param[in]     pColumn:�к�
	* @param[in]     pSectionData:��������
	* @param[out]    pErrorMsg:������Ϣ 
	* @return        ������� true-�ɹ� false-ʧ��
	*
    */
	bool SetPointData(string pStationId, int pRow, int pColumn, const string& pSectionData, string& pErrorMsg);

	/**
	* @brief         ��ȡĳ���������
	* @param[in]     pStationId:��վ��� 
	* @param[in]     pRow:�к� 
	* @param[in]     pColumn:�к�
	* @param[out]     pSectionData:��������
	* @param[out]    pErrorMsg:������Ϣ 
	* @return        ������� true-�ɹ� false-ʧ��
	*
    */
	bool GetPointData(string pStationId, int pRow, int pColumn, string& pSectionData, string& pErrorMsg);
protected:
private:
	/** @brief				�������ݹ����handle*/
	XJHANDLE					m_hLibXJShareSD;
	
	/** @brief				CreateSectionData����ָ��*/
	PFUNCREATESECTIONDATA	m_pFunCreateSectionData;
	
	/** @brief				OpenSectionData����ָ��*/
	PFUNOPENSECTIONDATA		m_pFunOpenSectionData;
	
	/** @brief				CloseSectionData����ָ��*/
	PFUNCLOSESECTION		m_pFunCloseSectionData;
	
	/** @brief				GetFullSectionData����ָ��*/
	PFUNSETFULLSD		    m_pFunSetFullSectionData;
	
	/** @brief				GetFullSectionData����ָ��*/
	PFUNGETFULLSD			m_pFunGetFullSectionData;
	
	/** @brief				SetSectionData����ָ��*/
	PFUNSETSD				m_pFunSetSectionData;
	
	/** @brief				GetSectionData����ָ��*/
	PFUNGETSD				m_pFunGetSectionData;
	
	/** @brief				SetPointData����ָ��*/
	PFUNSETPOINTDATA		m_pFunSetPointData;
	
	/** @brief				GetPointData����ָ��*/
	PFUNGETPOINTDATA		m_pFunGetPointData;
};

#endif // SectionDataWrapper_h__