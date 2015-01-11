// "Now Playing" Plugin
// Foobar2000 plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "npFoobar2kPlugin.h"

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

extern "C" NPFOOBAR2KPLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new Foobar2kPlayer () );
}

Foobar2kPlayer::Foobar2kPlayer ()
{
}

Foobar2kPlayer::~Foobar2kPlayer ()
{
}

bool Foobar2kPlayer::Initialize ()
{
	pInfo.name = L"Foobar2000";
	pInfo.guid = L"{E60CC2CE-CE31-4cd3-B2C0-A668A8F199B0}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 255, 0, 255 );
	pInfo.apisupport = false;

	return true;
}
bool Foobar2kPlayer::UnInitialize ()
{
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return 1;
	}
	return false;
}

PluginDescription* Foobar2kPlayer::GetPluginDescription ()
{
	return &pInfo;
}

bool Foobar2kPlayer::OnCheckStatus ( std::wstring & title )
{
	// {DA7CD0DE-1602-45e6-89A1-C2CA151E008E}/1
		HWND hFoobar2k = FindWindow ( L"foobar2000", NULL );
		if ( hFoobar2k )
		{
			wchar_t temp[100];
			GetWindowText ( hFoobar2k, temp, 100 );
			StripText ( L"[foobar2000", temp );
			title = temp;
			return true;
		}
	return false;
}

wchar_t* Foobar2kPlayer::StripText ( const wchar_t* strip, wchar_t* string )
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