// "Now Playing" Plugin
// WinAMP plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "npWinampPlugin.h"

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

extern "C" NPWINAMPPLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new WinAMPPlayer () );
}

WinAMPPlayer::WinAMPPlayer ()
{
}

WinAMPPlayer::~WinAMPPlayer ()
{
}

bool WinAMPPlayer::Initialize ()
{
	pInfo.name = L"WinAMP";
	pInfo.guid = L"{58920897-E21D-4e76-8C08-72CA6C81A4E0}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 255, 0, 255 );
	pInfo.apisupport = false;

	return true;
}
bool WinAMPPlayer::UnInitialize ()
{
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return true;
	}
	return false;
}

PluginDescription* WinAMPPlayer::GetPluginDescription ()
{
	return &pInfo;
}

bool WinAMPPlayer::OnCheckStatus ( std::wstring & title )
{
	HWND hWinampWnd = FindWindow ( L"Winamp v1.x", NULL );
	wchar_t temp[100] = L"";

	if ( hWinampWnd )
	{
		GetWindowText ( hWinampWnd, temp, 100 );
		StripText ( L"- Winamp", temp );	
		title = temp;
		return true;
	}

	return false;
}

wchar_t* WinAMPPlayer::StripText ( const wchar_t* strip, wchar_t* string )
{
	wchar_t *p;
	unsigned int wclen = wcslen ( strip );
	p = string + wcslen ( string ) - wclen;
	while ( p >= string )
	{
		if  ( !_wcsnicmp ( p, strip, wclen ) ) break;
		p--;
	}
	if ( p >= string ) p--;
	while ( p >= string && *p == ' ' ) p--;
	*++p=0;

	return string;
}