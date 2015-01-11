// "Now Playing" Plugin
// XMPlay plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#ifdef NPXMPLAYPLUGIN_EXPORTS
#define NPXMPLAYPLUGIN_API __declspec(dllexport)
#else
#define NPXMPLAYPLUGIN_API __declspec(dllimport)
#endif

#include "../../INPPlugin.h"

class XMPlayPlayer : public INPPlugin
{
public:
	XMPlayPlayer ();
	virtual ~XMPlayPlayer ();

	bool Initialize ();
	bool UnInitialize ();

	PluginDescription* GetPluginDescription ();
	bool OnCheckStatus ( std::wstring & );

	static LRESULT CALLBACK HookProc(int, WPARAM, LPARAM);

private:
	PluginDescription pInfo;
	static HHOOK hWndHook;
	bool hooked;
	static wchar_t tmp[100];
};
