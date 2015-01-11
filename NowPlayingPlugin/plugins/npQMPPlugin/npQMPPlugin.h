// "Now Playing" Plugin
// QMP plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#ifdef NPQMPPLUGIN_EXPORTS
#define NPQMPPLUGIN_API __declspec(dllexport)
#else
#define NPQMPPLUGIN_API __declspec(dllimport)
#endif

#define QMP_TITLETRACK	(WM_USER+1024)
#define QMP_TITLEALBUM  (WM_USER+1025)
#define QMP_TRACKNUMBER (WM_USER+1026)
#define QMP_ALBUMGENRE  (WM_USER+1027)
#define QMP_ARTISTALBUM (WM_USER+1028)
//#define QMP_TRACKLENGTH (WM_USER+1029)

#include "../../INPPlugin.h"

class QMPPlayer : public INPPlugin
{
public:
	QMPPlayer ();
	virtual ~QMPPlayer ();

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