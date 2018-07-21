#pragma warning(disable : 4275)
#pragma warning(disable : 4786)//屏蔽4786错误
#include "EntityContainer.h"

CEntityContainer::CEntityContainer() 
{
	m_pList = NULL;
	m_listLength = 0;
	m_nMaxLength = 2000;//链表的最大值
}

CEntityContainer::~CEntityContainer() 
{
	  ClearList();
}
/**	\brief	IsEmpty function 判断链表是否为空
	\return	bool             若为空，返回true，否则返回false	
*/
bool CEntityContainer::IsEmpty() 
{
	if (m_pList == NULL) 
	{
		return true;
	}
	return false;
}
/**	\brief	GetLength function 返回链表的中当前节点数
	\return	int
*/
int CEntityContainer::GetLength() 
{
	return m_listLength;
}
/**	\brief	ClearList function 将链表清空，释放当前所有节点
	\return	int                0 表示成功
*/
int CEntityContainer::ClearList() 
{
	try
	{
		while(m_listLength > 0)
		{
			DeleteHeadNode();
		}
	}
	catch(...)
	{
		return -1;
	}
	return 0;
}
/**	\brief	SetNodeData function  将position指定的节点内的数据设置为newData
*	param	position	int       节点索引从0到listLength-1
*	param	pNewObj	    void * 
*	return	int
*/
int CEntityContainer::SetNodeData(int position,   void *  pNewObj) 
{
	LNode *pTemp = GetNode(position);
	if (pTemp == NULL) 
	{
		return -1;
	}
	pTemp->m_pObj = pNewObj;
	return 0;
}
/**	\brief	GetNodeData function  得到指定位置节点的数据
*	param	position	int       节点索引从0到listLength-1
*	return	void*
*/
void* CEntityContainer::GetNodeData(int position)
{
	LNode* pNode = GetNode(position);
	if(pNode != NULL)
	{
		return pNode->m_pObj;
	}
	return NULL;
}
void* CEntityContainer::GetNodeDataTest(int position)
{
	LNode* pNode = GetNodeTest(position);
	if(pNode != NULL)
	{
		return pNode->m_pObj;
	}
	return NULL;
}
void*  CEntityContainer::GetTailNodeData()
{
	if(m_pList == NULL)
		return NULL;
	if(m_pList->forward == NULL)
		return NULL;
	return m_pList->forward->m_pObj;
}
void*  CEntityContainer::GetHeadNodeData()
{
	if(m_pList == NULL)
		return NULL;
	else
		return m_pList->m_pObj;
}
/**	\brief	GetNodeData function  得到节点的数据
	\param	pNode	 LNode *      节点结构指针
	\return	void*	
*/
void* CEntityContainer::GetNodeData(LNode *pNode)
{
	if(pNode != NULL)
	{
		return pNode->m_pObj;
	}
	return NULL;
}
/**	\brief	       AddHead function          添加数据到头
	\param	pObj   void *
	\return	int
*/

int CEntityContainer::AddHead(void *pObj)
{
	LNode *newNode = NULL;
	try
	{
		newNode = new LNode;
		newNode->m_nTLife  = 0;
		newNode->m_pObj    = pObj;
		if(m_listLength == 0)//当前无节点
		{
			m_pList = newNode;          
			m_pList->next =    m_pList;//自环
			m_pList->forward = m_pList; 
		}
		else
		{
			newNode->next    = m_pList;
//			m_pList->forward = newNode;
			if(m_listLength == 1) //当前只有一个节点,变成两个节点
			{
				m_pList->forward = newNode;
				newNode->forward = m_pList;
				m_pList->next    = newNode;
			}
			else                 //当前有2个及以上的节点
			{
		   		newNode->forward = m_pList->forward;
				m_pList->forward->next = newNode;
	////////////////////////////by zy////////////////////////////////
				m_pList->forward = newNode;
			}
			m_pList = newNode;
		}
		m_listLength++;
	}
	catch(...)
	{
		if(newNode != NULL)
			delete newNode;
		return -1;
	}
	return 0;
}
/**	\brief	AddTail function  把数据放到链表尾
*   param	pObj	void *    链表中的数据
*   return	int	    0         表示成功
*/
int CEntityContainer::AddTail(void *pObj)
{
	LNode *newNode = NULL;
	try
	{
		if(m_pList != NULL)
		{
			newNode = new LNode;
			newNode->m_nTLife      = 0;
			newNode->m_pObj        = pObj;
			newNode->next          = m_pList;
			if(m_listLength == 1)//只有1个的时候
			{
				m_pList->next    = newNode;
				m_pList->forward = newNode;
				////////////////////////////////////////////by zy/////////////////////////////////////////
				newNode->forward = m_pList;
			}
			else
			{
				newNode->forward       = m_pList->forward;
				m_pList->forward->next = newNode;
				m_pList->forward       = newNode;
			}
			m_listLength++;
		}
		else
		{
			AddHead(pObj);
		}
	}
	catch(...)
	{
		return -1;
	}
	return 0;	
}               
/**	\brief	The CEntityContainer::InsertNode function         在链表中插入一个节点
*   param	beforeWhich	int   beforeWhich <= 0                时候写在链表首
*							  beforeWhich >=                  链表长度时候写在链表尾
*                             beforeWhich					  在长度之间时 新节点插入在beforeWhich之前
*	param	pObj	    void*								  链表中的数据指针
	\return	int         0  表示成功
*/
int CEntityContainer::InsertNode(int beforeWhich, void* pObj) 
{
	LNode *pTemp = NULL;
	int nResult = -1;
	if(m_listLength == 0)  //当前链表为空,插在首
	{
		return AddHead(pObj);
	}
	//
	if (beforeWhich <= 0 ) //插在首
	{
		 nResult = AddHead(pObj);
	}
	else if( beforeWhich >= m_listLength)//插在尾
	{
		nResult  = AddTail(pObj);
	}
	else   //插在中间
	{
		//得到某个节点
		pTemp = GetNode(beforeWhich);
		if(pTemp != NULL)
		{
			LNode *newNode     = new LNode;
			newNode->m_nTLife  = 0;
			newNode->m_pObj    = pObj;
			newNode->next      = pTemp;
			newNode->forward   = pTemp->forward;
			if(pTemp->forward != NULL)
			{
				pTemp->forward->next = newNode;
			}
			pTemp->forward      = newNode;
			m_listLength++;
		}
		nResult = 0;
	}	
	return nResult;
}
//
/**	\brief	DeleteHeadNode function 删除头节点
	\return	int                     0 表示成功
*/
int CEntityContainer::DeleteHeadNode()
{
	LNode* pNext = NULL, *pForWord = NULL;
	try
	{
		if(m_listLength > 0)
		{
			pNext = m_pList->next;
			pForWord = m_pList->forward;
			if(m_listLength >= 2)  //至少2个以上节点
			{
				if(m_listLength > 2)//2个以上节点
				{
					pNext->forward = m_pList->forward;
				}
				else//只有2节点，删除一个只剩一个
				{
					pNext->forward = pNext;
					pNext->next    = pNext;
				}
				delete m_pList;
				m_pList = pNext;
			}
			else//只有一个节点
			{
				delete m_pList;
				m_pList = NULL;
			}
			m_listLength --;
		}
	}
	catch(...)
	{
		return -1;
	}
	return 0;
}
/**	\brief	DeleteTailNode function 删除尾节点
*	\return	int          0          表示成功	
*/
int CEntityContainer::DeleteTailNode()
{
	try
	{
		if(m_listLength > 0)
		{
			if(m_listLength == 1) //只有一个节点
			{
				delete m_pList;
				m_pList = NULL;
			}
			else
			{
				LNode* pTail = m_pList->forward;
				if(m_listLength > 2) //当前为2节点以上
				{
					m_pList->forward = pTail->forward;
					pTail->forward->next = m_pList;
				}
				else
				{
					m_pList->next = m_pList;
					m_pList->forward = m_pList;
				}
				delete pTail;
				pTail = NULL;
			}
			m_listLength--;
		}
	}
	catch (...) 
	{
		return -1;
	}
	return 0;
}
/**	\brief	DeleteNode function     删除一个指定的节点
	\param	position   int			positon的值从0到listLength - 1
	\return	int	                    0 表示成功 若链表为空或指定的节点不存在则返回-1
*/
int CEntityContainer::DeleteNode(int position) 
{
	if(position < 0 || position >= m_listLength) //越限
	{
		return -1;
	}
	else if(position == 0)                      //删除首
	{
		return DeleteHeadNode();
	}
	else if(position == m_listLength -1)        //删除尾巴
	{
		return DeleteTailNode();
	}
	else                                        //区间处理
	{
		LNode *pDel = GetNode(position);
		if (pDel == NULL)      //得到节点
		{
			return -1;
		}
		LNode* pDelNext = NULL, *pDelForward= NULL;
		pDelForward = pDel->forward;
		pDelNext    = pDel->next;
		if(pDelNext!= NULL)
		{
			pDelNext->forward = pDelForward;
		}
		if(pDelForward != NULL)
		{
			pDelForward->next  = pDelNext;
		}
		delete pDel;
	    m_listLength--;
	}
	return 0;
}
/**	\brief	DeleteNode function　删除一个指定的节点
	\param	pnode	LNode * 　　　节点指针
	\return	int	
*/
int CEntityContainer::DeleteNode(LNode *pnode)
{
	if(m_listLength == 0)//无数据
	{
		return -1;
	}
	if(m_pList == pnode)//头节点
	{
		DeleteHeadNode();
	}
	else if(pnode == m_pList->forward && m_pList != m_pList->forward )//尾节点
	{
		DeleteTailNode();
	}
	else               //删除常规节点，至少有３个节点
	{
		LNode* pNodeTemp = m_pList->forward; //从底结点开始
		int nPosition = m_listLength - 2;		
		while(nPosition >= 0)
		{
			if(pNodeTemp == pnode)
			{
				pNodeTemp->forward->next = pNodeTemp->next;
				pNodeTemp->next->forward = pNodeTemp->forward;
				delete pNodeTemp;
				pNodeTemp = NULL;
                m_listLength--; //删除后链表长度减一 by lipeng 
				break;
			}
			else
			{
				pNodeTemp = pNodeTemp->forward;
				nPosition--;
			}
		}
	}
	return 0;
}
LNode* CEntityContainer::FindNode(int position)
{
	int curPos;
	curPos = 0;
	LNode* pTemp = m_pList;
	while (curPos <= m_listLength-1)
	{
		if(curPos == position)
		{
			return pTemp;
		}
		pTemp = pTemp->next;
	    ++curPos;
	}
	return NULL;
}
LNode* CEntityContainer::GetNodeTest(int position)
{
	if(position < 0 || position > m_listLength -1)
	{
		return NULL;
	}
	if (position == 1)
	{
		m_pList = FindNode(position);
		return m_pList;
	}
	else
	{
		m_pList=m_pList->next;
		return m_pList;
	}
	return NULL;
}
/**	\brief	The CEntityContainer::GetNode function遍历得到指定位置节点的指针当position位置大于全长的一半时候从底部开始遍历
*	param	position   int        0 < position < 链表长度 - 1
*	return	LNode*
*/
LNode* CEntityContainer::GetNode(int position) 
{
	if(position < 0 || position > m_listLength -1)
	{
		return NULL;
	}
	int curPos;
	LNode * pTemp = m_pList;
	if(position <= m_listLength / 2) //小于1半时从头找
	{
		curPos = 0;
		while (curPos <= m_listLength - 1)
		{		
			if(curPos == position)
			{
				return pTemp;
			}
			pTemp = pTemp->next;
			++curPos;
		}
	}
	else                         //大于一半时从尾开始找
	{
		curPos = m_listLength;
		while(curPos >= 0)
		{
			if(curPos == position)
			{
				return pTemp;
			}	
			pTemp = pTemp->forward;
			--curPos;
		}		
	}
	return NULL;
}
/**	\brief	GetTailNode function  得到尾巴节点
	\return	LNode*	　　　　　　　尾巴节点
*/
LNode* CEntityContainer::GetTailNode()
{
	if(m_pList != NULL)
	{
		return m_pList->forward;
	}
	return NULL;
}
/**	\brief	GetNextNode function         得到下一节点的节点指针
	\param	pNode	a parameter of type LNode *
	\return	LNode*	
*/
LNode* CEntityContainer::GetNextNode(LNode *pNode)
{
	if(pNode != NULL)
	{
		return pNode->next;
	}
	return NULL;
}
/**	\brief	GetForwardNode function　　得到上一节点的节点指针
	\param	pNode	LNode *
	\return	LNode*	
*/
LNode* CEntityContainer::GetForwardNode(LNode *pNode)
{
	if(pNode != NULL)
	{
		return pNode->forward;
	}
	return NULL;
}

