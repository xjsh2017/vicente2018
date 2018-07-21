#pragma warning(disable : 4275)
#pragma warning(disable : 4786)//����4786����
#include "EntityContainer.h"

CEntityContainer::CEntityContainer() 
{
	m_pList = NULL;
	m_listLength = 0;
	m_nMaxLength = 2000;//��������ֵ
}

CEntityContainer::~CEntityContainer() 
{
	  ClearList();
}
/**	\brief	IsEmpty function �ж������Ƿ�Ϊ��
	\return	bool             ��Ϊ�գ�����true�����򷵻�false	
*/
bool CEntityContainer::IsEmpty() 
{
	if (m_pList == NULL) 
	{
		return true;
	}
	return false;
}
/**	\brief	GetLength function ����������е�ǰ�ڵ���
	\return	int
*/
int CEntityContainer::GetLength() 
{
	return m_listLength;
}
/**	\brief	ClearList function ��������գ��ͷŵ�ǰ���нڵ�
	\return	int                0 ��ʾ�ɹ�
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
/**	\brief	SetNodeData function  ��positionָ���Ľڵ��ڵ���������ΪnewData
*	param	position	int       �ڵ�������0��listLength-1
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
/**	\brief	GetNodeData function  �õ�ָ��λ�ýڵ������
*	param	position	int       �ڵ�������0��listLength-1
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
/**	\brief	GetNodeData function  �õ��ڵ������
	\param	pNode	 LNode *      �ڵ�ṹָ��
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
/**	\brief	       AddHead function          ������ݵ�ͷ
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
		if(m_listLength == 0)//��ǰ�޽ڵ�
		{
			m_pList = newNode;          
			m_pList->next =    m_pList;//�Ի�
			m_pList->forward = m_pList; 
		}
		else
		{
			newNode->next    = m_pList;
//			m_pList->forward = newNode;
			if(m_listLength == 1) //��ǰֻ��һ���ڵ�,��������ڵ�
			{
				m_pList->forward = newNode;
				newNode->forward = m_pList;
				m_pList->next    = newNode;
			}
			else                 //��ǰ��2�������ϵĽڵ�
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
/**	\brief	AddTail function  �����ݷŵ�����β
*   param	pObj	void *    �����е�����
*   return	int	    0         ��ʾ�ɹ�
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
			if(m_listLength == 1)//ֻ��1����ʱ��
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
/**	\brief	The CEntityContainer::InsertNode function         �������в���һ���ڵ�
*   param	beforeWhich	int   beforeWhich <= 0                ʱ��д��������
*							  beforeWhich >=                  ������ʱ��д������β
*                             beforeWhich					  �ڳ���֮��ʱ �½ڵ������beforeWhich֮ǰ
*	param	pObj	    void*								  �����е�����ָ��
	\return	int         0  ��ʾ�ɹ�
*/
int CEntityContainer::InsertNode(int beforeWhich, void* pObj) 
{
	LNode *pTemp = NULL;
	int nResult = -1;
	if(m_listLength == 0)  //��ǰ����Ϊ��,������
	{
		return AddHead(pObj);
	}
	//
	if (beforeWhich <= 0 ) //������
	{
		 nResult = AddHead(pObj);
	}
	else if( beforeWhich >= m_listLength)//����β
	{
		nResult  = AddTail(pObj);
	}
	else   //�����м�
	{
		//�õ�ĳ���ڵ�
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
/**	\brief	DeleteHeadNode function ɾ��ͷ�ڵ�
	\return	int                     0 ��ʾ�ɹ�
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
			if(m_listLength >= 2)  //����2�����Ͻڵ�
			{
				if(m_listLength > 2)//2�����Ͻڵ�
				{
					pNext->forward = m_pList->forward;
				}
				else//ֻ��2�ڵ㣬ɾ��һ��ֻʣһ��
				{
					pNext->forward = pNext;
					pNext->next    = pNext;
				}
				delete m_pList;
				m_pList = pNext;
			}
			else//ֻ��һ���ڵ�
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
/**	\brief	DeleteTailNode function ɾ��β�ڵ�
*	\return	int          0          ��ʾ�ɹ�	
*/
int CEntityContainer::DeleteTailNode()
{
	try
	{
		if(m_listLength > 0)
		{
			if(m_listLength == 1) //ֻ��һ���ڵ�
			{
				delete m_pList;
				m_pList = NULL;
			}
			else
			{
				LNode* pTail = m_pList->forward;
				if(m_listLength > 2) //��ǰΪ2�ڵ�����
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
/**	\brief	DeleteNode function     ɾ��һ��ָ���Ľڵ�
	\param	position   int			positon��ֵ��0��listLength - 1
	\return	int	                    0 ��ʾ�ɹ� ������Ϊ�ջ�ָ���Ľڵ㲻�����򷵻�-1
*/
int CEntityContainer::DeleteNode(int position) 
{
	if(position < 0 || position >= m_listLength) //Խ��
	{
		return -1;
	}
	else if(position == 0)                      //ɾ����
	{
		return DeleteHeadNode();
	}
	else if(position == m_listLength -1)        //ɾ��β��
	{
		return DeleteTailNode();
	}
	else                                        //���䴦��
	{
		LNode *pDel = GetNode(position);
		if (pDel == NULL)      //�õ��ڵ�
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
/**	\brief	DeleteNode function��ɾ��һ��ָ���Ľڵ�
	\param	pnode	LNode * �������ڵ�ָ��
	\return	int	
*/
int CEntityContainer::DeleteNode(LNode *pnode)
{
	if(m_listLength == 0)//������
	{
		return -1;
	}
	if(m_pList == pnode)//ͷ�ڵ�
	{
		DeleteHeadNode();
	}
	else if(pnode == m_pList->forward && m_pList != m_pList->forward )//β�ڵ�
	{
		DeleteTailNode();
	}
	else               //ɾ������ڵ㣬�����У����ڵ�
	{
		LNode* pNodeTemp = m_pList->forward; //�ӵ׽�㿪ʼ
		int nPosition = m_listLength - 2;		
		while(nPosition >= 0)
		{
			if(pNodeTemp == pnode)
			{
				pNodeTemp->forward->next = pNodeTemp->next;
				pNodeTemp->next->forward = pNodeTemp->forward;
				delete pNodeTemp;
				pNodeTemp = NULL;
                m_listLength--; //ɾ���������ȼ�һ by lipeng 
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
/**	\brief	The CEntityContainer::GetNode function�����õ�ָ��λ�ýڵ��ָ�뵱positionλ�ô���ȫ����һ��ʱ��ӵײ���ʼ����
*	param	position   int        0 < position < ������ - 1
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
	if(position <= m_listLength / 2) //С��1��ʱ��ͷ��
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
	else                         //����һ��ʱ��β��ʼ��
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
/**	\brief	GetTailNode function  �õ�β�ͽڵ�
	\return	LNode*	��������������β�ͽڵ�
*/
LNode* CEntityContainer::GetTailNode()
{
	if(m_pList != NULL)
	{
		return m_pList->forward;
	}
	return NULL;
}
/**	\brief	GetNextNode function         �õ���һ�ڵ�Ľڵ�ָ��
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
/**	\brief	GetForwardNode function�����õ���һ�ڵ�Ľڵ�ָ��
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

