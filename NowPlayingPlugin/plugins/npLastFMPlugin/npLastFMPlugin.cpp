// "Now Playing" Plugin
// LastFM plugin
// (c) 2007 Dmitry D. aka Sp1ritus
// ------------------------------------------

#include "stdafx.h"
#include "npLastFMPlugin.h"

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

extern "C" NPLASTFMPLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new LastFMPlayer () );
}

LastFMPlayer::LastFMPlayer ()
{
}

LastFMPlayer::~LastFMPlayer ()
{
}

bool LastFMPlayer::Initialize ()
{
	pInfo.name = L"Last.fm";
	pInfo.guid = L"{74D8F60D-2F19-41d7-A6C5-ADA8FEE17CF1}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 255, 255, 255 );
	pInfo.apisupport = false;

	return true;
}
bool LastFMPlayer::UnInitialize ()
{
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return true;
	}
	return false;
}

PluginDescription* LastFMPlayer::GetPluginDescription ()
{
	return &pInfo;
}

bool LastFMPlayer::OnCheckStatus ( std::wstring & title )
{
	HWND hLastfmWnd = GetWindow ( FindWindow ( L"QWidget", NULL ), GW_OWNER );
	wchar_t temp[100] = L"";

	if ( hLastfmWnd )
	{
		GetWindowText ( hLastfmWnd, temp, 100 );
		
		if ( wcscmp ( temp, L"Last.fm" ) == 0 )
			return false;

		title = temp;

		return true;
	}

	return false;
}