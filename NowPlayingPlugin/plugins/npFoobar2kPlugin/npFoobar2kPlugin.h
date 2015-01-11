// "Now Playing" Plugin
// Foobar2000 plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#ifdef NPFOOBAR2KPLUGIN_EXPORTS
#define NPFOOBAR2KPLUGIN_API __declspec(dllexport)
#else
#define NPFOOBAR2KPLUGIN_API __declspec(dllimport)
#endif

#include "../../INPPlugin.h"

class Foobar2kPlayer : public INPPlugin
{
public:
	Foobar2kPlayer ();
	virtual ~Foobar2kPlayer ();

	bool Initialize ();
	bool UnInitialize ();

	PluginDescription* GetPluginDescription ();
	bool OnCheckStatus ( std::wstring & );

private:
	wchar_t* StripText ( const wchar_t*, wchar_t* );
	PluginDescription pInfo;
};