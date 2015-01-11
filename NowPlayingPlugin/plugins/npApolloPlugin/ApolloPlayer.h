// QIP Infium Plugin
// class NowPlayingPlugin
// (c) 2007 Mihaylov Andrey aka Zero 
//                 & Dmitry D. aka Spiritus
// ------------------------------------------

#include "IPlayerPlugin.h"

class ApolloPlayer : public IPlayerPlugin
{
public:
	ApolloPlayer ();
	virtual ~ApolloPlayer ();

	int OnCheckStatus ();
	int OnCopyData ( HWND, COPYDATASTRUCT* );
};