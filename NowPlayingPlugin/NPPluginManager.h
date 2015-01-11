// "Now Playing" QIP Infium Plugin
// class NPPluginManager
// Provides basic functions to extensions (plugins) like RemoteControls for Media Players etc...
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once
#include "INPPluginManager.h"

class NPPluginManager : public INPPluginManager
{
public:
	NPPluginManager ();
	virtual ~NPPluginManager ();

	bool LoadPluginsDirectory ( std::wstring & );
	bool LoadPlugin ( std::wstring & );
	bool UnloadPlugins ();

	bool SetActivePlugin ( std::wstring & );
	INPPlugin* GetActivePlugin ();
	INPPlugin* GetPlugin ( std::wstring & );

	const std::vector<INPPlugin*> GetPluginList () { return pluginList; };

private:
	std::vector<INPPlugin*> pluginList;
	INPPlugin* activePlugin;
};
