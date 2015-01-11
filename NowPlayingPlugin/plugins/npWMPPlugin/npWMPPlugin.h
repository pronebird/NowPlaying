// "Now Playing" Plugin
// WMP plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#ifdef NPWMPPLUGIN_EXPORTS
#define NPWMPPLUGIN_API __declspec(dllexport)
#else
#define NPWMPPLUGIN_API __declspec(dllimport)
#endif

#include "../../INPPlugin.h"

class WMPPlugin : public INPPlugin
{
public:
	WMPPlugin ();
	virtual ~WMPPlugin ();

	bool Initialize ();
	bool UnInitialize ();

	PluginDescription* GetPluginDescription ();
	bool OnCheckStatus ( PlayerData & );

private:
	PluginDescription pInfo;
};