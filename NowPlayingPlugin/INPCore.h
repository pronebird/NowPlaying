// "Now Playing" QIP Infium Plugin
// class INPCore, np-core interface
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once
#include "INPPluginManager.h"
#include "INPLanguageManager.h"

class INPCore
{
public:
	virtual ~INPCore () {};
	virtual bool SetDefaultSettings () = 0;
	virtual INPPluginManager* GetPluginManager () = 0;
	virtual INPLanguageManager* GetLanguageManager () = 0;
	virtual std::wstring GetPath () = 0;
};