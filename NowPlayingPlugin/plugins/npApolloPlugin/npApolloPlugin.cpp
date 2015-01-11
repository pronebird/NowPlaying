// "Now Playing" Plugin
// Apollo plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "npApolloPlugin.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

extern "C" NPAPOLLOPLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new ApolloPlayer () );
}

ApolloPlayer::ApolloPlayer ()
{
}

ApolloPlayer::~ApolloPlayer ()
{
}

bool ApolloPlayer::Initialize ()
{
	pInfo.name = L"Apollo";
	pInfo.guid = L"{381E75F1-EE6A-4d8b-8531-F7232FEF5656}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 0, 0, 0 );
	pInfo.apisupport = true;

	return true;
}
bool ApolloPlayer::UnInitialize ()
{
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return 1;
	}
	return false;
}

PluginDescription* ApolloPlayer::GetPluginDescription ()
{
	return &pInfo;
}

bool ApolloPlayer::OnCheckStatus ( PlayerData & pdata )
{
	HWND hApolloWnd = Apollo_GetFirstApolloWindowHandle ();
	wchar_t temp[100] = L"";

	if ( hApolloWnd )
	{
		Apollo_GetCurrentlyPlayedFilename ( hApolloWnd, temp, 100 );
		pdata.filename = temp;

		int trackN = Apollo_GetCurrentTrackNumber ( hApolloWnd );
		 _itow_s ( trackN, temp, 100, 10 );
		pdata.number = temp;

		Apollo_GetCurrentlyPlayedTitle ( hApolloWnd, temp, 100 );
		pdata.title = temp;

		int trackLen = Apollo_GetPlaybackCountdown ( hApolloWnd );
		 _itow_s ( trackLen, temp, 100, 10 );
		pdata.length = temp;

		pdata.album = L"";
		pdata.artist = L"";
		pdata.genre = L"";

		return true;
	}
	return false;
}