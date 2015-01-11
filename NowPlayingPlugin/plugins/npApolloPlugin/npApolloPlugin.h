// "Now Playing" Plugin
// Apollo plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#ifdef NPAPOLLOPLUGIN_EXPORTS
#define NPAPOLLOPLUGIN_API __declspec(dllexport)
#else
#define NPAPOLLOPLUGIN_API __declspec(dllimport)
#endif

#include "../../INPPlugin.h"

class ApolloPlayer : public INPPlugin
{
public:
	ApolloPlayer ();
	virtual ~ApolloPlayer ();

	bool Initialize ();
	bool UnInitialize ();

	PluginDescription* GetPluginDescription ();
	bool OnCheckStatus ( PlayerData & );
private:
	PluginDescription pInfo;
};
