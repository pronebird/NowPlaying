// "Now Playing" Plugin
// AIMP2 plugin
// (c) 2007 Dmitry D. aka Sp1ritus
// ------------------------------------------

#ifdef NPAIMP2PLUGIN_EXPORTS
#define NPAIMP2PLUGIN_API __declspec(dllexport)
#else
#define NPAIMP2PLUGIN_API __declspec(dllimport)
#endif

#include "../../INPPlugin.h"

class AIMP2Player : public INPPlugin
{
public:
	AIMP2Player ();
	virtual ~AIMP2Player ();

	bool Initialize ();
	bool UnInitialize ();

	PluginDescription* GetPluginDescription ();
	bool OnCheckStatus ( PlayerData & );
	int OnCopyData ( HWND, COPYDATASTRUCT* );
private:
	PluginDescription pInfo;
};