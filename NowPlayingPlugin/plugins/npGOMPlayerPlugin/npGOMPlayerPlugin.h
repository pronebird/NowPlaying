// "Now Playing" Plugin
// GOMPlayer plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#ifdef NPGOMPLAYERPLUGIN_EXPORTS
#define NPGOMPLAYERPLUGIN_API __declspec(dllexport)
#else
#define NPGOMPLAYERPLUGIN_API __declspec(dllimport)
#endif

#include "../../INPPlugin.h"

class GOMPlayer : public INPPlugin
{
public:
	GOMPlayer ();
	virtual ~GOMPlayer ();

	bool Initialize ();
	bool UnInitialize ();

	PluginDescription* GetPluginDescription ();
	bool OnCheckStatus ( std::wstring & );
private:
	wchar_t* StripText ( const wchar_t*, wchar_t* );
	PluginDescription pInfo;
};