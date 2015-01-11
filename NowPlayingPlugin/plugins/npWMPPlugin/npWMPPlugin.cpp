// "Now Playing" Plugin
// WMP plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "npWMPPlugin.h"

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

extern "C" NPWMPPLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new WMPPlugin () );
}

WMPPlugin::WMPPlugin ()
{
}

WMPPlugin::~WMPPlugin ()
{
}

bool WMPPlugin::Initialize ()
{
	pInfo.name = L"Windows Media Player";
	pInfo.guid = L"{1697B97B-D715-41dc-B726-AAA74125D249}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 255, 255, 255 );
	pInfo.apisupport = true;

	return true;
}
bool WMPPlugin::UnInitialize ()
{
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return true;
	}
	return false;
}

PluginDescription* WMPPlugin::GetPluginDescription ()
{
	return &pInfo;
}

bool WMPPlugin::OnCheckStatus ( PlayerData & playerData )
{
    CoInitialize(NULL);

    HRESULT hr = S_OK;
    CComBSTR bstrVersionInfo;
    CComPtr<IWMPPlayer> spPlayer;

    hr = spPlayer.CoCreateInstance( __uuidof(WindowsMediaPlayer), 0, CLSCTX_INPROC_SERVER );

    if(SUCCEEDED(hr))
    {
        hr = spPlayer->get_versionInfo(&bstrVersionInfo);
    }

    spPlayer.Release();
    CoUninitialize();

	return false;
}