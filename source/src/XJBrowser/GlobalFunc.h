#ifndef GLOBAL_FUNC_
#define GLOBAL_FUNC_

extern 	CXJBrowserApp theApp;

#define MYASSERT(p) \
if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return;\
}

#define MYASSERT_NULL(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return NULL;\
}

#define MYASSERT_CONTINUE(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	continue;\
}

#define MYASSERT_BOOL(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return FALSE;\
}

#define MYASSERT_STRING(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return "";\
}

#define MYASSERT_INT(p) \
	if (XJASSERT(p, __FILE__, __LINE__))\
{\
	return -1;\
}

#define  MYDELETE(p) \
	if (p != NULL)\
{\
	delete p;\
	 p = NULL;\
}

inline BOOL XJASSERT(void* p, CString strFileName, int iLine)
{
	if (p == NULL)
	{
		CString strLog;
		strLog.Format(strFileName + _T(" ") + ":%d" + _T(" ") + _T("Ö¸ÕëÎª¿Õ"), iLine);
		theApp.WriteLog(strLog, 3);
		return TRUE;
	}
	return FALSE;
}


inline LONGLONG StartCount()
{
//#ifdef _DEBUG	
return GetTickCount();
//#endif
	
}

inline void EndCount(CString strLog, LONGLONG startTime)
{
//#ifdef _DEBUG	
	LONGLONG endTime = GetTickCount();
	strLog.Format(strLog + ":%.3f Ãë", float(endTime-startTime)/(float)1000);
	WriteLog(strLog, 3);
//#endif
}

#endif
