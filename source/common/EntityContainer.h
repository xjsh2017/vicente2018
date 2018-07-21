/**
 * @defgroup 
 * @{
 */
/**
 * @brief     EntityContainer  ͨ��������
 * @author    yinzhehong
 * @version   1.0
 * @date      2006/10/25
 *
 * example
 * @code
 * @endcode
 */
/** @} */ //OVER
#ifndef AFX_ENTITY_H__72A84412_56C5_4A57_A8C7_0AC65895E2B2__INCLUDED_
#define AFX_ENTITY_H__72A84412_56C5_4A57_A8C7_0AC65895E2B2__INCLUDED_
/**	\brief	The LNode struct ����Ľڵ�
*/
#pragma warning(disable : 4786)//����4786����
#include "define.h"
/**	\brief	The LNode struct ָ�������еĽڵ�

*/
typedef struct LNode 
{
	void* m_pObj;           //����а���������ָ��
	struct LNode *next;     //��һ���ڵ��ָ��
	struct LNode *forward;  //��һ���ڵ��ָ��
	UINT m_nTLife;          //�ýڵ�Ĵ���ʱ�䣬��ʼֵΪ0 ��λΪ�� add by yinzhehong 2006
}LNode, *pLinkList;
/**
 * @defgroup 
 * @{
 */
/**
 * @brief     CEntityContainer     ˫��ָ������                    
 * @author    yinzhehong
 * @version   1.0
 * @date      2006/10/18
 *
 * example
 * @code
 * @endcode
 */
/** @} */ //OVER
class CEntityContainer 
{
private:
	pLinkList    m_pList;      //��һ������ڵ�
	int          m_listLength;
	int          m_nMaxLength; //��������ڵ���Ŀ	
public:
	CEntityContainer();
	~CEntityContainer();
public:	
	int    SetNodeData(int position,  void * pNewObj);
	void*  GetNodeData(LNode *pNode);
	int    DeleteNode(LNode* pnode);
	int    InsertNode(int beforeWhich,void * pObj);
	int    DeleteNode(int position);
	int    AddTail(void* pObj);
	int    DeleteTailNode();
	int    AddHead(void * pObj);
	int    DeleteHeadNode();
	void*  GetNodeData(int position);
	void*  GetNodeDataTest(int position);
	
	void*  GetTailNodeData();
	void*  GetHeadNodeData();
	int    ClearList();
	bool   IsEmpty  ();
	int    GetLength();
	LNode* GetNode  (int position);
	LNode* GetForwardNode(LNode *pNode);
	LNode* GetNextNode(LNode* pNode);
	LNode* GetTailNode();
    LNode* FindNode(int position);
	LNode* GetNodeTest (int position);
};

#endif