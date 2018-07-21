// SttpDeque.cpp: implementation of the CSttpDeque class.
//
//////////////////////////////////////////////////////////////////////

#include "SttpDeque.h"

//////////////////////////////////////////////////////////////////////////
//                  Class  CSttpDeque   ʵ��                                    
//////////////////////////////////////////////////////////////////////////

/****************************************************************/
/*																*/
/*             ���캯��.									    */
/*																*/
/****************************************************************/
CSttpDeque::CSttpDeque(int m_Size):m_MaxSize(m_Size)
{
	//Ĭ����־·���ͼ���
	m_LogFile.SetLogLevel(CLogFile::trace);
    m_LogFile.SetLogPath("Xj_Log/");

	//��־����
	m_LogFile.Open("sttpdeque.txt");

    
	m_Lock.init("CSttpDeque");
}

/****************************************************************/
/*																*/
/*              ��������.									    */
/*																*/
/****************************************************************/
CSttpDeque::~CSttpDeque()
{
	m_LogFile.Close();

    m_Lock.del();
}

/*************************************************************
 �� �� ����Insert()
 ���ܸ�Ҫ������һ��Ԫ�ص�����β��
 �� �� ֵ��bool �ɹ���ʧ��
 ��    ����
           STTP_DEQUE_MSG &item :Ҫ�����ITEM
***************************************************************/
bool CSttpDeque::Insert(STTP_DEQUE_MSG & item)
{
	char cLog[255]= "";

	if( !m_Lock.lock() ){ 
		sprintf(cLog,"get ownenship failed when add an item to sttp deque");
		m_LogFile.Add(cLog,CLogFile::error);
		return false;
	}

    while(m_mDeque.size() >= m_MaxSize){

		sprintf(cLog,
		        "the total num is more than the MAXSIZE(%d) of sttp deque, \
				del the first item of the sttp deque",
				m_MaxSize);

		m_LogFile.Add(cLog);

		m_mDeque.pop_front();
	}

	m_mDeque.push_back(item);

	m_Lock.unlock();

	m_LogFile.Add("add an item to sttp deque successfuly");
	return true;
}

/*************************************************************
 �� �� ����Extract()
 ���ܸ�Ҫ���Ӷ�����ȡ��ָ��������һ��Ԫ�ز�ɾ��
 �� �� ֵ��bool �ɹ���ʧ��
 ��    ����
           STTP_DEQUE_MSG &item :���ڱ���ȡ��Ԫ�صĻ�����
		   int      nIndex      :������,��0��ʼ(Ĭ��Ϊ0)
***************************************************************/
bool CSttpDeque::Extract(STTP_DEQUE_MSG & item,int nIndex)
{
	char cLog[255]= "";

	if( !m_Lock.lock() ){
		sprintf(cLog,"get ownenship failed when extract an item from sttp deque");
		m_LogFile.Add(cLog,CLogFile::error);
		return false;
	}

	int nsize = m_mDeque.size();

	// �ж����ݸ���
	if(nsize <=0)
	{
		m_Lock.unlock();
		return false;
	}

	// �ж������Ƿ�Ϸ�
	if(nIndex >= nsize){
		m_LogFile.FormatAdd(CLogFile::error,
		                    "the nIndex is greater or equal the total\
			                 size when extract a item from sttp deque!(nIndex=%d \
						     total size =%d)",
						    nIndex,nsize);
		m_Lock.unlock();
		return false;
	}
        
	// del item
	int i = 0;
	list <STTP_DEQUE_MSG> ::iterator ite = m_mDeque.begin();

	try{
		for (ite = m_mDeque.begin(); ite!=m_mDeque.end(); ite++)
		{
			if (i == nIndex)
			{
				item = *ite;
				m_mDeque.erase(ite);
				break ;
			}
			
			i++;
		}
	}
	catch(...)
	{
		m_Lock.unlock();
		return false;
	}
	
	m_Lock.unlock();

	m_LogFile.Add("extract an item from sttp deque successfuly");

	return true;
}

/*************************************************************
 �� �� ����Fetch()
 ���ܸ�Ҫ���Ӷ�����ȡ��ָ��������һ��Ԫ�ز�ɾ��
 �� �� ֵ��bool �ɹ���ʧ��
 ��    ����
           STTP_DEQUE_MSG &item :���ڱ���ȡ����Ԫ��
		   int            nIndex:������,��0��ʼ(Ĭ��Ϊ0)
***************************************************************/
bool CSttpDeque::Fetch(STTP_DEQUE_MSG &item,int nIndex)
{
	char cLog[255]= "";

	if( !m_Lock.lock() ){
		sprintf(cLog,"get ownenship failed when fetch an item from sttp deque");
		m_LogFile.Add(cLog,CLogFile::error);
		return false;
	}

	int nsize = m_mDeque.size();

	// �ж����ݸ���
	if(nsize <=0)
	{
		m_Lock.unlock();
		return false;
	}

	// �ж������Ƿ�Ϸ�
	if(nIndex >= nsize){
		m_LogFile.FormatAdd(CLogFile::error,
		                    "the nIndex is greater or equal the total\
			                 size when fetch a item from sttp deque!(nIndex=%d \
						     total size =%d)",
							nIndex,nsize);
		m_Lock.unlock();
		return false;
	}
        
	// get item
	int i = 0;
	list <STTP_DEQUE_MSG> ::iterator ite = m_mDeque.begin();

    for (ite = m_mDeque.begin(); ite!=m_mDeque.end(); ite++)
	{
		if (i == nIndex)
		{
			item = *ite;
			break ;
		}
		
		i++;
	}

	m_Lock.unlock();

	m_LogFile.Add("fetch an item from sttp deque successfuly");

	return true;
}

/*************************************************************
 �� �� ����RemoveHead()
 ���ܸ�Ҫ��ɾ�����еĵ�һ��Ԫ��
 �� �� ֵ��bool �ɹ���ʧ��
 ��    ����void
***************************************************************/
bool CSttpDeque::RemoveHead()
{
	char cLog[255]= "";

	if( !m_Lock.lock() ){
		sprintf(cLog,"get ownenship failed when remove the header of sttp deque");
		m_LogFile.Add(cLog,CLogFile::error);
		return false;
	}

	if(m_mDeque.size() > 0)
		m_mDeque.pop_front();

	m_Lock.unlock();

	m_LogFile.Add("remove the header of sttp deque successfuly");

	return true;
}

/*************************************************************
 �� �� ����Clear()
 ���ܸ�Ҫ��ɾ�����е�����Ԫ��
 �� �� ֵ��bool �ɹ���ʧ��
 ��    ����void
***************************************************************/
bool CSttpDeque::Clear()
{
	char cLog[255]= "";

	if( !m_Lock.lock() ){
		sprintf(cLog,"get ownenship failed when clear the all items of sttp deque");
		m_LogFile.Add(cLog,CLogFile::error);
		return false ;
	}

	while( !m_mDeque.empty() )
		m_mDeque.pop_front();

	m_Lock.unlock();

	m_LogFile.Add("clear the all items of sttp deque successfuly");

	return true;
}

/*************************************************************
 �� �� ����IsEmpty()
 ���ܸ�Ҫ���жϸö����Ƿ���Ԫ��
 �� �� ֵ��void
 ��    ����void
***************************************************************/
bool CSttpDeque::IsEmpty()
{
	return m_mDeque.empty();
}

/*************************************************************
 �� �� ����size()
 ���ܸ�Ҫ�����ظö�����Ԫ�ظ���
 �� �� ֵ��int
 ��    ����void
***************************************************************/
int CSttpDeque::size()
{
	int nsize = 0;

    char cLog[255]= "";

	if( !m_Lock.lock() ){
		sprintf(cLog,"get ownenship failed when get size of sttp deque");
		m_LogFile.Add(cLog,CLogFile::error);
		return -1;
	}

	nsize = m_mDeque.size();

    m_Lock.unlock();

    if(nsize >0)
	{
		sprintf(cLog,"the size of sttp deque is %d",nsize);
		
		m_LogFile.Add(cLog);
	}

	return nsize;
}

/*************************************************************
 �� �� ����SetMaxSize()
 ���ܸ�Ҫ�����ö��������ɵ����Ԫ�ظ���
 �� �� ֵ: bool �ɹ���ʧ��
 ��    ����UINT nNum: Ҫ���õĶ���Ԫ�ص�������
           ע��:��Сһ�����еĻ����ܵ���ԭ�ж����в������ݶ�ʧ
***************************************************************/
bool CSttpDeque::SetMaxSize(UINT nNum)
{
	char cLog[255]="";
	if( nNum <= 0){
		sprintf(cLog,"the param=%d is invalid when set maxsize of sttp deque,it should \
			    >=0",nNum);
				return false;
	}

	if(nNum < m_MaxSize)
	{
		STTP_DEQUE_MSG item;
		// delete the invalid item
		while(size() > nNum)
		{
			Extract(item);
		}
	}

	m_MaxSize = nNum;

	return true;
}

/*************************************************************
 �� �� ����SetLogLevel()
 ���ܸ�Ҫ��������־����
 �� �� ֵ: void
 ��    ����nLevel ��־����
***************************************************************/
void CSttpDeque::SetLogLevel(int nLevel)
{
    m_LogFile.SetLogLevel(nLevel);
}

/*************************************************************
 �� �� ����SetAt()
 ���ܸ�Ҫ���޸�ָ��λ�õ�Ԫ��
 �� �� ֵ: bool
 ��    ����item   ����Ԫ������
           nIndex ����
**************************************************************/
bool CSttpDeque::SetAt(STTP_DEQUE_MSG item,int nIndex)
{
	int nsize = m_mDeque.size();

	if(nIndex >= nsize)
		return false;

	m_Lock.lock();

	// �����ʼλ��
	int i =0;
	list <STTP_DEQUE_MSG> ::iterator ite = m_mDeque.begin();
    for (ite = m_mDeque.begin(); ite!=m_mDeque.end(); ite++)
	{
		if (i == nIndex)
		{
			break ;
		}
		
		i++;
	}

	(*ite).bTreat  = item.bTreat;
	(*ite).nTimes  = item.nTimes;
    memcpy(&(*ite).msg,&item.msg,sizeof(STTPMSG));

	m_Lock.unlock();

	return true;
}

/*************************************************************
 �� �� ����GetLogLevel()
 ���ܸ�Ҫ�������־�����־
 �� �� ֵ��int
 ��    ������  
***************************************************************/
int CSttpDeque::GetLogLevel()
{
	return m_LogFile.GetLogLevel();
}

/*************************************************************
 �� �� ����SetLogPath()
 ���ܸ�Ҫ��������־���·��
 �� �� ֵ��void 
 ��    ����
           pPath :��־·��
***************************************************************/
void CSttpDeque::SetLogPath(char * pPath)
{
	m_LogFile.SetLogPath(pPath);
}

/*************************************************************
 �� �� ����SetLogFileName()
 ���ܸ�Ҫ������־�ļ�
 �� �� ֵ��bool �ɹ���ʧ��
 ��    ����pFileName :�ļ���(����·��)
***************************************************************/
bool CSttpDeque::SetLogFileName(char * pFileName)
{
	m_LogFile.Close();

	return ( m_LogFile.Open(pFileName) );
}