// SttpDeque.cpp: implementation of the CSttpDeque class.
//
//////////////////////////////////////////////////////////////////////

#include "SttpDeque.h"

//////////////////////////////////////////////////////////////////////////
//                  Class  CSttpDeque   实现                                    
//////////////////////////////////////////////////////////////////////////

/****************************************************************/
/*																*/
/*             构造函数.									    */
/*																*/
/****************************************************************/
CSttpDeque::CSttpDeque(int m_Size):m_MaxSize(m_Size)
{
	//默认日志路径和级别
	m_LogFile.SetLogLevel(CLogFile::trace);
    m_LogFile.SetLogPath("Xj_Log/");

	//日志名称
	m_LogFile.Open("sttpdeque.txt");

    
	m_Lock.init("CSttpDeque");
}

/****************************************************************/
/*																*/
/*              析构函数.									    */
/*																*/
/****************************************************************/
CSttpDeque::~CSttpDeque()
{
	m_LogFile.Close();

    m_Lock.del();
}

/*************************************************************
 函 数 名：Insert()
 功能概要：增加一个元素到队列尾部
 返 回 值：bool 成功或失败
 参    数：
           STTP_DEQUE_MSG &item :要插入的ITEM
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
 函 数 名：Extract()
 功能概要：从队列中取出指定索引的一个元素并删除
 返 回 值：bool 成功或失败
 参    数：
           STTP_DEQUE_MSG &item :用于保存取出元素的缓冲区
		   int      nIndex      :索引号,从0开始(默认为0)
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

	// 判断数据个数
	if(nsize <=0)
	{
		m_Lock.unlock();
		return false;
	}

	// 判断索引是否合法
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
 函 数 名：Fetch()
 功能概要：从队列中取出指定索引的一个元素不删除
 返 回 值：bool 成功或失败
 参    数：
           STTP_DEQUE_MSG &item :用于保存取出的元素
		   int            nIndex:索引号,从0开始(默认为0)
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

	// 判断数据个数
	if(nsize <=0)
	{
		m_Lock.unlock();
		return false;
	}

	// 判断索引是否合法
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
 函 数 名：RemoveHead()
 功能概要：删除队列的第一个元素
 返 回 值：bool 成功或失败
 参    数：void
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
 函 数 名：Clear()
 功能概要：删除队列的所有元素
 返 回 值：bool 成功或失败
 参    数：void
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
 函 数 名：IsEmpty()
 功能概要：判断该队列是否有元素
 返 回 值：void
 参    数：void
***************************************************************/
bool CSttpDeque::IsEmpty()
{
	return m_mDeque.empty();
}

/*************************************************************
 函 数 名：size()
 功能概要：返回该队列中元素个数
 返 回 值：int
 参    数：void
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
 函 数 名：SetMaxSize()
 功能概要：设置队列能容纳的最大元素个数
 返 回 值: bool 成功或失败
 参    数：UINT nNum: 要设置的队列元素的最大个数
           注意:缩小一个队列的话可能导致原有队列中部分数据丢失
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
 函 数 名：SetLogLevel()
 功能概要：设置日志级别
 返 回 值: void
 参    数：nLevel 日志级别
***************************************************************/
void CSttpDeque::SetLogLevel(int nLevel)
{
    m_LogFile.SetLogLevel(nLevel);
}

/*************************************************************
 函 数 名：SetAt()
 功能概要：修改指定位置的元素
 返 回 值: bool
 参    数：item   保存元素内容
           nIndex 索引
**************************************************************/
bool CSttpDeque::SetAt(STTP_DEQUE_MSG item,int nIndex)
{
	int nsize = m_mDeque.size();

	if(nIndex >= nsize)
		return false;

	m_Lock.lock();

	// 获得起始位置
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
 函 数 名：GetLogLevel()
 功能概要：获得日志输出标志
 返 回 值：int
 参    数：无  
***************************************************************/
int CSttpDeque::GetLogLevel()
{
	return m_LogFile.GetLogLevel();
}

/*************************************************************
 函 数 名：SetLogPath()
 功能概要：设置日志输出路径
 返 回 值：void 
 参    数：
           pPath :日志路径
***************************************************************/
void CSttpDeque::SetLogPath(char * pPath)
{
	m_LogFile.SetLogPath(pPath);
}

/*************************************************************
 函 数 名：SetLogFileName()
 功能概要：打开日志文件
 返 回 值：bool 成功或失败
 参    数：pFileName :文件名(不带路径)
***************************************************************/
bool CSttpDeque::SetLogFileName(char * pFileName)
{
	m_LogFile.Close();

	return ( m_LogFile.Open(pFileName) );
}