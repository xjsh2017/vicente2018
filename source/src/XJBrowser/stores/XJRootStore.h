#ifndef _XJROOTSTORE_H
#define _XJROOTSTORE_H

class CXJRootStore
{
public:
	CXJRootStore(void);
	virtual ~CXJRootStore(void);

	static void Register(CXJRootStore *pInstance);
	static void UnRegister(CXJRootStore *pInstance);
	static void Release();

private:
	static list<CXJRootStore*> m_lstInstantce;
};

#endif
