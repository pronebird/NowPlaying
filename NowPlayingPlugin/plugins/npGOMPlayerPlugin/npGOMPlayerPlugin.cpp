// "Now Playing" Plugin
// GOMPlayer plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "npGOMPlayerPlugin.h"

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

extern "C" NPGOMPLAYERPLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new GOMPlayer () );
}

GOMPlayer::GOMPlayer ()
{
}

GOMPlayer::~GOMPlayer ()
{
}

bool GOMPlayer::Initialize ()
{
	pInfo.name = L"GOMPlayer";
	pInfo.guid = L"{B4DB4B67-0A8F-4c70-A26C-40386EDE334B}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 255, 255, 255 );
	pInfo.apisupport = false;

	return true;
}
bool GOMPlayer::UnInitialize ()
{
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return true;
	}
	return false;
}

PluginDescription* GOMPlayer::GetPluginDescription ()
{
	return &pInfo;
}

bool GOMPlayer::OnCheckStatus ( std::wstring & title )
{
	HWND hGOMPlayerWnd = FindWindowEx ( NULL, FindWindow ( L"GXWINDOW", NULL ), L"GomPlayer1.x", NULL );
	wchar_t temp[100] = L"";

	if ( hGOMPlayerWnd )
	{
		GetWindowText ( hGOMPlayerWnd, temp, 100 );

		if ( wcscmp ( temp, L"GOM Player" ) == 0 )
			return false;

		StripText ( L"- GOM Player", temp );
		title = temp;

		return true;
	}

	return false;
}

wchar_t* GOMPlayer::StripText ( const wchar_t* strip, wchar_t* string )
{
	wchar_t *p;
	unsigned int wclen = static_cast<unsigned int>(wcslen ( strip ));
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