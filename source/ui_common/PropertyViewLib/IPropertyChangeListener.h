// IPropCtrlParent.h: interface for the IPropCtrlParent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef IPROPERTYCHANGELISTENER_H
#define IPROPERTYCHANGELISTENER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EProperty.h"

class IPropertyChangeListener
{

public:

	//when a property has changed, the property control
	//will notify its parent through this function call
	virtual void PropertyChanged(EProperty* pChanged)
	{
	}
	
	virtual CString GetNoPropertiesMessage()
	{
		return "NO PROPERTIES";
	}

};

#endif
