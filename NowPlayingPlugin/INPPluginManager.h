// "Now Playing" QIP Infium Plugin
// class INPNPPluginManager, np-NPPluginManager interface
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once

class INPPluginManager
{
public:
	virtual ~INPPluginManager () {};

	virtual bool LoadPluginsDirectory ( std::wstring & ) = 0;
	virtual bool LoadPlugin ( std::wstring & ) = 0;
	virtual bool UnloadPlugins () = 0;

	virtual bool SetActivePlugin ( std::wstring & ) = 0;
	virtual INPPlugin* GetActivePlugin () = 0;
	virtual INPPlugin* GetPlugin ( std::wstring & ) = 0;

	virtual const std::vector<INPPlugin*> GetPluginList () = 0;
};