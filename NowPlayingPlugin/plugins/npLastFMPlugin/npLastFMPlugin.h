// "Now Playing" Plugin
// LastFM plugin
// (c) 2007 Dmitry D. aka Sp1ritus
// ------------------------------------------

#ifdef NPLASTFMPLUGIN_EXPORTS
#define NPLASTFMPLUGIN_API __declspec(dllexport)
#else
#define NPLASTFMPLUGIN_API __declspec(dllimport)
#endif

#include "../../INPPlugin.h"

class LastFMPlayer : public INPPlugin
{
public:
	LastFMPlayer ();
	virtual ~LastFMPlayer ();

	bool Initialize ();
	bool UnInitialize ();

	PluginDescription* GetPluginDescription ();
	bool OnCheckStatus ( std::wstring & );
private:
	PluginDescription pInfo;
};