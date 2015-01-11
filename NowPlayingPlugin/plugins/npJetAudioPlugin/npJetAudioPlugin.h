// "Now Playing" Plugin
// JetAudio plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#ifdef NPJETAUDIOPLUGIN_EXPORTS
#define NPJETAUDIOPLUGIN_API __declspec(dllexport)
#else
#define NPJETAUDIOPLUGIN_API __declspec(dllimport)
#endif

#include "../../INPPlugin.h"

unsigned int mbs2ws ( const char*, wchar_t*, int );

class JetAudioPlayer : public INPPlugin
{
public:
	JetAudioPlayer ();
	virtual ~JetAudioPlayer ();

	bool Initialize ();
	bool UnInitialize ();

	PluginDescription* GetPluginDescription ();
	bool OnCheckStatus ( PlayerData & );
	
	static LRESULT CALLBACK WindowProc ( HWND, UINT, WPARAM, LPARAM );

private:
	PluginDescription pInfo;
	PlayerData playerData;
	HWND dataExchangeWnd;

	bool _RegisterClass ();
	bool _UnRegisterClass ();
};