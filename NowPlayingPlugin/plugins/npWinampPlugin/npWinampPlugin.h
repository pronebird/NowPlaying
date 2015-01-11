// "Now Playing" Plugin
// WinAMP plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#ifdef NPWINAMPPLUGIN_EXPORTS
#define NPWINAMPPLUGIN_API __declspec(dllexport)
#else
#define NPWINAMPPLUGIN_API __declspec(dllimport)
#endif

#include "../../INPPlugin.h"

class WinAMPPlayer : public INPPlugin
{
public:
	WinAMPPlayer ();
	virtual ~WinAMPPlayer ();

	bool Initialize ();
	bool UnInitialize ();

	PluginDescription* GetPluginDescription ();
	bool OnCheckStatus ( std::wstring & );
private:
	wchar_t* StripText ( const wchar_t*, wchar_t* );
	PluginDescription pInfo;
};