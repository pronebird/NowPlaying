// "Now Playing" Plugin
// iTunes plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#ifdef NPITUNESPLUGIN_EXPORTS
#define NPITUNESPLUGIN_API __declspec(dllexport)
#else
#define NPITUNESPLUGIN_API __declspec(dllimport)
#endif

#include "../../INPPlugin.h"
// iTunes interfaces
#include "iTunesCOMInterface.h"
#include "iTunesCOMInterface_i.c"

class iTunesPlayer : public INPPlugin
{
public:
	iTunesPlayer ();
	virtual ~iTunesPlayer ();

	bool Initialize ();
	bool UnInitialize ();

	PluginDescription* GetPluginDescription ();
	bool OnCheckStatus ( PlayerData & );

private:
	PluginDescription pInfo;
};