// MemSet.cpp: implementation of the CMemSet class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#pragma warning(disable:4786)
#include "MemSet.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMemSet::CMemSet():m_lockMemsetAccess("Lock RealMemSetData")
{
	memset(m_chTablename, '\0', sizeof(m_chTablename));//����
	m_pFiledList = NULL;//�ֶ�ָ��
	m_nFieldNum  = 0;//�ֶ���Ŀ
	m_CurmemRow  = NULL;//��ǰ��ָ��
	m_nCurRow    = 0;//��ǰ�ǵڼ���
	m_nRowCount  = 0;//һ������������
    m_nMaxRow    = 1000000;//�������
	m_nExistTime = 0;//����ڵ�ʱ��
	m_pRowList   = NULL;
	m_nRowNum    = 0;
	m_nDirtyType = 0;

	m_listData.clear();
}

//************************************
// ������:    ~CMemSet
// ���ܸ���:  ����
// ����ֵ:   
//************************************
CMemSet::~CMemSet()
{
	//�ͷ�����
	FreeData(true);
}

//************************************
// �� �� ��:  FreeData
// ���ܸ�Ҫ:  �ͷ�����
// ����Ȩ��:  public 
// �� �� ֵ:  int 0:�ɹ�
// ��    ��:  bool bFreeAll �Ƿ����ȫ����if true �ֶζ���Ҳ�����ͷ�
//************************************
int CMemSet::FreeData(bool bFreeAll)
{
	SQL_ROW pTempRowdata = NULL;
    vector<SQL_ROW>::iterator itRowitor = m_listData.begin();
	for (; itRowitor != m_listData.end(); itRowitor++)
	{
		pTempRowdata = *itRowitor;
		if (pTempRowdata == NULL)
		{
			--m_nRowCount;
			continue;
		}

		for(int j = 0; j < m_nFieldNum; j++)
		{
			if(pTempRowdata[j] != NULL)
			{
				delete [] pTempRowdata[j]; //ɾ��ÿ������
				pTempRowdata[j] = NULL;
			}			
		}
		
		if (pTempRowdata != NULL)
		{
			delete[] pTempRowdata;
			pTempRowdata = NULL;
		}
		
		--m_nRowCount;
	}
	m_listData.clear();

	m_nCurRow   = 0;
	m_CurmemRow = NULL;
	
	if(bFreeAll)//�ͷ��ֶζ���
	{
		if(m_pFiledList != NULL)
		{
			delete []m_pFiledList;
			m_pFiledList = NULL;
		}
		m_nFieldNum = 0; 
	}

	if (bFreeAll)
	{
		if (m_pRowList != NULL)
		{
			delete []m_pRowList;
			m_pRowList = NULL;
		}
	}

	return 0;
}

//************************************
// �� �� ��:  MoveFirst
// ���ܸ�Ҫ:  �ƶ�����һ��
// ����Ȩ��:  public 
// �� �� ֵ:  int 0 �ɹ� -1 ʧ��
//************************************
int CMemSet::MoveFirst()
{
	if(m_nRowCount == 0 )
	{
		return -1;
	}

// 	if(m_nCurRow == 0)
// 	{
// 		return 0;
// 	}

	m_CurmemRow = (SQL_ROW) m_listData.at(0);
	m_nCurRow = 0;

	return 0;
}

//************************************
// �� �� ��:  MoveNext
// ���ܸ�Ҫ:  �ƶ�����һ��
// ����Ȩ��:  public 
// �� �� ֵ:  int 
//************************************
int CMemSet::MoveNext()
{
	if(m_nCurRow >= m_nRowCount)
	{
		return -1;
	}

	m_nCurRow++;

	if (m_nCurRow >= m_listData.size())
	{
		MoveFirst();//��ͷ
	}
	else
	{
		m_CurmemRow = (SQL_ROW) m_listData.at(m_nCurRow);
	}
	
	return m_nCurRow;//���ص��ǵ�ǰ������
}

//************************************
// �� �� ��:  MoveNextTest
// ���ܸ�Ҫ:  �����ƶ�һ��,��ס��ǰλ��
// ����Ȩ��:  public 
// �� �� ֵ:  int ��ǰ��
//************************************
int CMemSet::MoveNextTest()
{
	if(m_nCurRow >= m_nRowCount)
	{
		return -1;
	}

	m_nCurRow++;

	if (m_nCurRow >= m_listData.size())
	{
		MoveFirst();//��ͷ
	}
	else
	{
		m_CurmemRow = (SQL_ROW) m_listData.at(m_nCurRow);
	}

	return m_nCurRow;
}

//************************************
// �� �� ��:  MoveForward
// ���ܸ�Ҫ:  ��ǰ�ƶ�һ��
// ����Ȩ��:  public 
// �� �� ֵ:  int 0
//************************************
int CMemSet::MoveForward()
{
	if(m_nCurRow == 0 || m_nRowCount == 0)
	{
		return 0;
	}

	m_nCurRow--;

	if (m_nCurRow < 0)
	{
		return -1;
	}

	if (m_nCurRow >= m_listData.size())
	{
		return -1;
	}

	m_CurmemRow = (SQL_ROW) m_listData.at(m_nCurRow);

	return 0;
}

//************************************
// �� �� ��:  MoveToUserRow
// ���ܸ�Ҫ:  �ƶ����û�ָ������
// ����Ȩ��:  public 
// �� �� ֵ:  int 
// ��    ��:  int nRownum ָ������
//************************************
int CMemSet::MoveToUserRow( int nRownum )
{
	if((nRownum >= m_nRowCount) || (nRownum < 0))
	{
		return -1;
	}

	if (nRownum >= m_listData.size())
	{
		return -1;
	}
	
	m_nCurRow = nRownum;//��ǰ�б�־
	
	m_CurmemRow = (SQL_ROW) m_listData.at(m_nCurRow);
	
	return m_nCurRow;
}

//************************************
// �� �� ��:  GetValue
// ���ܸ�Ҫ:  �����ֶ�������ֶε�ֵ�����ִ�Сд��
// ����Ȩ��:  public 
// �� �� ֵ:  char* �ֶε�ֵ��ʧ��ΪNULL
// ��    ��:  const char * pFieldname �ֶ�Ӣ����
//************************************
char* CMemSet::GetValue(const char* pFieldname)
{
	if(pFieldname == NULL || m_CurmemRow == NULL)
	{
		return NULL;
	}

	int nField = -1;

	for(int i = 0; i < m_nFieldNum; i++)//�õ��ֶ����ڱ��
	{
		if (m_pFiledList[i].GetFieldName() != NULL)
		{
			if(strcmp(m_pFiledList[i].GetFieldName(), pFieldname) == 0)
			{
				nField = i;
				break;
			}
		}
	}

	if(nField == -1)
	{
		return NULL;
	}

	return GetValue(nField);
}

//************************************
// �� �� ��:  GetRealDataValue
// ���ܸ�Ҫ:  �����ֶ�������ֶε�ֵ������Сд��
// ����Ȩ��:  public 
// �� �� ֵ:  char* �ֶε�ֵ��ʧ��ΪNULL
// ��    ��:  const char * pLowerFieldname �ֶ�Ӣ����
//************************************
char* CMemSet::GetRealDataValue(const char* pLowerFieldname)
{
	if(pLowerFieldname == NULL || m_CurmemRow == NULL)
	{
		return NULL;
	}

	int nField = -1;
	char* pFieldName = NULL;
	string strMemsetLowerFieldname = "";
	string strLowerFieldname = StringToLower((char*)pLowerFieldname);

	for(int i = 0; i < m_nFieldNum; i++)//�õ��ֶ����ڱ��
	{
		pFieldName = m_pFiledList[i].GetFieldName();
		if (pFieldName == NULL)
		{
			continue;
		}

		strMemsetLowerFieldname = StringToLower(pFieldName);
		if(strcmp(strMemsetLowerFieldname.c_str(), strLowerFieldname.c_str()) == 0)
		{
			nField = i;
			break;
		}
	}

	if(nField == -1)
	{
		return NULL;
	}

	return 	GetValue(nField);
}

//************************************
// �� �� ��:  GetValue
// ���ܸ�Ҫ:  �����ֶ�λ�û���ֶε�ֵ
// ����Ȩ��:  public 
// �� �� ֵ:  char* �ֶε�ֵ��ʧ��ΪNULL
// ��    ��:  UINT nField �ֶ�λ�ã��ڼ��У�
//************************************
char* CMemSet::GetValue(UINT nField)
{
	if(nField >= m_nFieldNum || m_CurmemRow == NULL)
	{
		return NULL;
	}

	return m_CurmemRow[nField];//���ظ�ֵ���ַ���ָ��
}

//************************************
// �� �� ��:  GetRealValue
// ���ܸ�Ҫ:  �����ֶ�����ȡĳһ�е�ֵ������Сд��
// ����Ȩ��:  public 
// �� �� ֵ:  char* �ֶε�ֵ��ʧ��ΪNULL
// ��    ��:  const char * pFieldname �ֶ�Ӣ����
// ��    ��:  UINT nRow �к�
//************************************
char* CMemSet::GetRealValue(const char* pFieldname, UINT nRow)
{
	int nField = -1;
	if (pFieldname == NULL)
	{
		return NULL;
	}
	
	CMemField* pTempFieldList = m_pFiledList;
	string strMemsetLowerFieldname = "";

	for(int i = 0; i < m_nFieldNum; i++)//�õ��ֶ����ڱ��
	{
		if (pTempFieldList[i].GetFieldName() == NULL)
		{
			continue;
		}

		strMemsetLowerFieldname = StringToLower(pTempFieldList[i].GetFieldName());//��С��
		if (strMemsetLowerFieldname.size() > 0)
		{
			if(strcmp(strMemsetLowerFieldname.c_str(), pFieldname) == 0)
			{
				nField = i;
				break;
			}
		}
	}

	if(nField == -1)
	{
		return NULL;
	}

	return GetRealNumValue(nRow, nField);
}

//************************************
// �� �� ��:  GetRealNumValue
// ���ܸ�Ҫ:  �����кŻ�ȡĳһ�е�ֵ
// ����Ȩ��:  public 
// �� �� ֵ:  char* �ֶε�ֵ��ʧ��ΪNULL
// ��    ��:  UINT nRow �к�
// ��    ��:  UINT nField �к�
//************************************
char* CMemSet::GetRealNumValue(UINT nRow, UINT nField)
{
	if(nField >= m_nFieldNum)
	{
		return NULL;
	}
	
	CLockUp lockup(&m_lockMemsetAccess);//����


	if (nRow >= m_listData.size())
	{
		return NULL;
	}

	SQL_ROW pTempRow = (SQL_ROW) m_listData.at(nRow);//��ȡֵ
	if (pTempRow != NULL)
	{
		return pTempRow[nField];
	}

	return NULL;
}

//************************************
// ������:    SetValue
// ���ܸ���:  �����ֶ��������õ�ǰ��ĳ���ֶε�ֵ
// ����ֵ:   int 0:�ɹ�
// ����: const char * name �ֶ���
// ����: const char * value ��Ҫ���õ�ֵ
//************************************
int CMemSet::SetValue(const char *name,const char *value)
{
	int nField = -1;
	if(name == NULL || m_CurmemRow == NULL)
	{
		return -1;
	}
	//�õ��ֶ����ڱ��
	char* pFieldName = NULL;
	CMemField * m_temp_field = m_pFiledList;
	string strLowerField = "";

	for(int i = 0; i < m_nFieldNum; i++)
	{
		pFieldName = m_temp_field[i].GetFieldName();
		if (pFieldName != NULL)
		{
			strLowerField = StringToLower(pFieldName);//��С��
			if (strLowerField.size() > 0)
			{
				if(strcmp(strLowerField.c_str(), name) == 0)
				{
					nField = i;
					break;
				}
			}
		}
	}
	if(nField == -1)
	{
		return -1;
	}
	//�����ֶ�������õ�ǰ��ĳ���ֶε�ֵ
	return SetValue(nField,value);
}

//************************************
// ������:    SetValue
// ���ܸ���:  �����ֶ�������õ�ǰ��ĳ���ֶε�ֵ
// ����ֵ:   int 0:�ɹ�
// ����: UINT nField �ֶ����
// ����: const char * value �ֶ�ֵ
//************************************
int CMemSet::SetValue(UINT nField,const char* value)
{
	if(nField >= m_nFieldNum || m_CurmemRow == NULL)
	{
		return NULL;
	}

	CLockUp lockUp(&m_lockMemsetAccess);

	if (m_CurmemRow[nField] != NULL)
	{
		delete []m_CurmemRow[nField];
		m_CurmemRow[nField] = NULL;
	}
	if(value != NULL)//��Ϊ�ǿ�ֵ
	{
		int nLength = strlen(value);
		if(nLength > 0)
		{
			char* newvale = new char[nLength + 1];
			if (newvale != NULL)
			{
				memset(newvale, '\0', (nLength+1) );
				memcpy(newvale, value, nLength);
				m_CurmemRow[nField] = newvale;
			}
		}
	}
	return 0;
}

//************************************
// �� �� ��:  AddRow
// ���ܸ�Ҫ:  ����һ�����ݵ�ĩβ
// ����Ȩ��:  public 
// �� �� ֵ:  int ���Ӻ����������
// ��    ��:  SQL_ROW row ������
//************************************
int CMemSet::AddRow(SQL_ROW row)
{
	CLockUp lockUp(&m_lockMemsetAccess);

	if(m_nRowCount == 0)
	{
		m_CurmemRow = row;
	}
	m_listData.push_back(row);//��ӵ�ĩβ
	m_nRowCount++;
	
	return m_nRowCount;
}

//************************************
// �� �� ��:  AtatchField
// ���ܸ�Ҫ:  �����ֶ�����
// ����Ȩ��:  public 
// �� �� ֵ:  int 0���ɹ� -1��ʧ��
// ��    ��:  CMemField * pMemFildList �ֶ��б�
// ��    ��:  UINT nFieldNum �ֶ���
//************************************
int CMemSet::AtatchField(CMemField *pMemFildList, UINT nFieldNum)
{
	if(pMemFildList == NULL || nFieldNum == 0)
	{
		return -1;
	}

	if(m_pFiledList != NULL)
	{
		delete []m_pFiledList;
		m_pFiledList = NULL;
	}

	m_pFiledList = pMemFildList;
	m_nFieldNum  = nFieldNum;

	return 0;
}

//************************************
// �� �� ��:  SetLifeTime
// ���ܸ�Ҫ:  �����ڴ������ʱ��
// ����Ȩ��:  public 
// �� �� ֵ:  int 0:�ɹ�
// ��    ��:  UINT nLiftTime ����ʱ��
//************************************
int CMemSet::SetLifeTime(UINT nLiftTime)
{
	m_nExistTime = nLiftTime;

	return 0;
}

//************************************
// �� �� ��:  GetLifeTime
// ���ܸ�Ҫ:  �õ���ǰ������ʱ��
// ����Ȩ��:  public 
// �� �� ֵ:  UINT ��ǰ������ʱ��
//************************************
UINT CMemSet::GetLifeTime()
{
	return m_nExistTime;
}

//************************************
// �� �� ��:  GetMemRowNum
// ���ܸ�Ҫ:  �õ���¼������
// ����Ȩ��:  public 
// �� �� ֵ:  UINT ����
//************************************
UINT CMemSet::GetMemRowNum()
{
	return m_nRowCount;
}

//************************************
// �� �� ��:  GetMemFieldNum
// ���ܸ�Ҫ:  �õ�����(�ֶ���)
// ����Ȩ��:  public 
// �� �� ֵ:  UINT ����
//************************************
UINT CMemSet::GetMemFieldNum()
{
	return m_nFieldNum;
}

//************************************
// �� �� ��:  GetFieldType
// ���ܸ�Ҫ:  �õ�ָ���е���������
// ����Ȩ��:  public 
// �� �� ֵ:  int �ֶ���������
// ��    ��:  UINT nField �ֶ����
//************************************
int CMemSet::GetFieldType(UINT nField)
{
	if(nField < 0 || nField > m_nFieldNum)
	{
		return -1;
	}

	return m_pFiledList[nField].GetFieldType();
}

//************************************
// �� �� ��:  GetFieldName
// ���ܸ�Ҫ:  �õ�ָ���е�������
// ����Ȩ��:  public 
// �� �� ֵ:  char* �ֶ���
// ��    ��:  UINT nField �����
//************************************
char* CMemSet::GetFieldName(UINT nField)
{
	if(nField < 0 || nField > m_nFieldNum)
	{
		return NULL;
	}

	return m_pFiledList[nField].GetFieldName();
}

//************************************
// �� �� ��:  AddRows
// ���ܸ�Ҫ:  �ڱ��׷�ӱ�����,�˷���Ϊ������ʽ���
// ����Ȩ��:  public 
// �� �� ֵ:  int 0
// ��    ��:  CMemSet * pMemSet ���ݼ�
//************************************
int CMemSet::AddRows(CMemSet *pMemSet)
{
	if(pMemSet == NULL)
	{
		return -1;
	}

	if(pMemSet->GetMemRowNum() == 0 || pMemSet->GetMemFieldNum() == 0)//������
	{
		return 0;
	}

	SQL_ROW	memRow = NULL;
	for(int i = 0; i < pMemSet->GetMemRowNum(); i++)
	{
		memRow = ColoneRow(pMemSet->GetRow(i));
		if(memRow != NULL)
		{
			AddRow(memRow);
		}
	}

	return 0;
}

//************************************
// �� �� ��:  GetRow
// ���ܸ�Ҫ:  �õ�ָ����������
// ����Ȩ��:  public 
// �� �� ֵ:  SQL_ROW ������ 0 <= nposition < rowcount
// ��    ��:  int nposition λ��
//************************************
SQL_ROW CMemSet::GetRow(int nposition)
{
	if((nposition < 0) || nposition > (m_nRowCount -1))
	{
		return NULL;
	}

	return (SQL_ROW) m_listData.at(nposition);
}

//************************************
// �� �� ��:  ColoneRow
// ���ܸ�Ҫ:  ��¡һ������
// ����Ȩ��:  private 
// �� �� ֵ:  SQL_ROW ������
// ��    ��:  SQL_ROW pSourceRow ���Ƶ�������
//************************************
SQL_ROW CMemSet::ColoneRow(SQL_ROW pSourceRow)
{
	if(pSourceRow == NULL || m_nFieldNum == 0)
	{
		return NULL;
	}

	SQL_ROW	memRow  = new  char* [m_nFieldNum];//new���д������
	if(memRow == NULL)
	{
		return NULL;
	}

	char* pFieldData = NULL;
	UINT nFieldLength = 0;
	for(UINT i = 0; i < m_nFieldNum; i++)//�Ƚ�����ָ�븳�գ�Ȼ���ٸ���ʵ�ʳ���new�ڴ棬new�������ɾ��
	{
		memRow[i] = NULL;              //ָ���е�ֵĬ��Ϊ��
		if(pSourceRow[i] != NULL)
		{
			pFieldData   = NULL;
			nFieldLength = strlen(pSourceRow[i]);
			pFieldData   = new char[nFieldLength + 1];
			if(pFieldData != NULL)
			{
				memset(pFieldData, '\0', (nFieldLength+1));
				memcpy(pFieldData, pSourceRow[i], nFieldLength);//��������
				memRow[i] = pFieldData;                       //��������
			}
		}		
	}

	return memRow;
}

//************************************
// �� �� ��:  AtatchRow
// ���ܸ�Ҫ:  ����������
// ����Ȩ��:  public 
// �� �� ֵ:  int 0���ɹ� -1��ʧ��
// ��    ��:  CMemRow * pMemRowList ������ָ�� 
// ��    ��:  UINT nRowNum ��������
//************************************
int CMemSet::AtatchRow(CMemRow* pMemRowList, UINT nRowNum)
{
	if (pMemRowList == NULL || nRowNum == 0)
	{
		return -1;
	}

	if (m_pRowList != NULL)
	{
		delete []m_pRowList;
		m_pRowList = NULL;
	}

	m_pRowList = pMemRowList;
	m_nRowNum  = nRowNum;
	
	return 0;
}

//************************************
// �� �� ��:  GetRowLength
// ���ܸ�Ҫ:  ��ȡ�е����ݳ���
// ����Ȩ��:  public 
// �� �� ֵ:  int �ܳ���
// ��    ��:  int nRowNum ����
//************************************
int CMemSet::GetRowLength(int nRowNum)
{
	if(nRowNum < 0 || nRowNum > m_nRowNum)
	{
		return -1;
	}
	return m_pRowList[nRowNum].GetRowLength();
}

//************************************
// �� �� ��:  GetFieldLength
// ���ܸ�Ҫ:  ��ȡ�ֶγ��ȣ�������
// ����Ȩ��:  public 
// �� �� ֵ:  int 
// ��    ��:  UINT nField �к�
//************************************
int CMemSet::GetFieldLength(UINT nField)
{
	if(nField < 0 || nField > m_nFieldNum)
	{
		return -1;
	}

	return m_pFiledList[nField].GetFieldLength();
}

//************************************
// �� �� ��:  SetTableName
// ���ܸ�Ҫ:  ���ñ�����
// ����Ȩ��:  public 
// �� �� ֵ:  int 0 �ɹ���1 ������Ч
// ��    ��:  const char * pTbName ������
//************************************
int CMemSet::SetTableName(const char* pTbName)
{
	if (pTbName == NULL)
	{
		return -1;
	}
	
	if(strlen(pTbName) > 199)//���ȳ���199
	{
		return -1;
	}

	memset(m_chTablename, '\0', sizeof(m_chTablename));
	memcpy(m_chTablename, pTbName, strlen(pTbName));
	
	return 0;
}

//************************************
// �� �� ��:  GetTableName
// ���ܸ�Ҫ:  �õ�����
// ����Ȩ��:  public 
// �� �� ֵ:  char * ����
//************************************
char* CMemSet::GetTableName()
{
	return m_chTablename;
}

//************************************
// �� �� ��:  StringToLower
// ���ܸ�Ҫ:  �ַ�����С��
// ����Ȩ��:  private 
// �� �� ֵ:  string ��С������ַ���
// ��    ��:  char * p_val ��������ַ���
//************************************
string CMemSet::StringToLower(char* p_val)
{
	string strLowerStr = "";
	
	if (p_val == NULL)
	{
		return strLowerStr;
	}
	
	char* p = p_val;
	for (p = p_val; *p; p++)
	{
		if ((*p >= ' ') && (*p < 128))
		{
			strLowerStr += tolower(*p);
		}
		else
		{
			strLowerStr += *p;
		}
	}
	return strLowerStr;
}

//************************************
// �� �� ��:  FreeFieldlistonly
// ���ܸ�Ҫ:  ��ɾ���ֶ���Ϣ�б�
// ����Ȩ��:  public 
// �� �� ֵ:  int 
//************************************
int CMemSet::FreeFieldlistonly()
{
	m_nRowCount = 0;
	m_nCurRow   = 0;
	m_CurmemRow = NULL;
	m_listData.clear();
	if(m_pFiledList != NULL)
	{
		delete []m_pFiledList;
		m_pFiledList = NULL;
	}

	m_nFieldNum = 0; 
	if (m_pRowList != NULL)
	{
		delete []m_pRowList;
		m_pRowList = NULL;
	}

	return 0;
}

//************************************
// �� �� ��:  GetCurRow
// ���ܸ�Ҫ:  �õ���ǰ������ָ��
// ����Ȩ��:  public 
// �� �� ֵ:  SQL_ROW ��ǰ������ָ��
//************************************
SQL_ROW CMemSet::GetCurRow()
{
	return m_CurmemRow;
}

//************************************
// �� �� ��:  GetCurRowNum
// ���ܸ�Ҫ:  �õ���ǰ������������
// ����Ȩ��:  public 
// �� �� ֵ:  UINT ��ǰ������������
//************************************
UINT CMemSet::GetCurRowNum()
{
	return m_nCurRow;
}

