// "Now Playing" Plugin
// XMPlay plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "npXMPlayPlugin.h"

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

extern "C" NPXMPLAYPLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new XMPlayPlayer () );
}

HHOOK XMPlayPlayer::hWndHook = NULL;
wchar_t XMPlayPlayer::tmp[100] = L"";

XMPlayPlayer::XMPlayPlayer ()
{
}

XMPlayPlayer::~XMPlayPlayer ()
{
	if (hooked && hWndHook)
	{
		UnhookWindowsHookEx(hWndHook);
	}
}

bool XMPlayPlayer::Initialize ()
{
	pInfo.name = L"XMPlay";
	pInfo.guid = L"{FB7ED8A6-F818-432b-9A26-39521249F1E6}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 255, 0, 255 );
	pInfo.apisupport = false;
	hooked = false;

	return true;
}
bool XMPlayPlayer::UnInitialize ()
{
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return true;
	}
	return false;
}

PluginDescription* XMPlayPlayer::GetPluginDescription ()
{
	return &pInfo;
}

bool XMPlayPlayer::OnCheckStatus ( std::wstring & title )
{
	HWND hXmPlayWnd = FindWindow ( L"XMPLAY-MAIN", NULL );
//	DWORD dwProccessId = 0;

	if ( hXmPlayWnd )
	{
		/*if(!hooked)
		{
			GetWindowThreadProcessId(hXmPlayWnd, &dwProccessId);
			hWndHook = SetWindowsHookEx(WH_CALLWNDPROCRET, HookProc, NULL, dwProccessId);
			if(!hWndHook)
			{
				DWORD err = GetLastError();
			}
			hooked = true;
		}*/

		GetWindowText(hXmPlayWnd, tmp, 100);

		if ( wcscmp ( tmp, L"" ) == 0 || wcscmp ( tmp, L"XMPlay" ) == 0 )
			return false;
		title = tmp;
		return true;
	} /*else if (hooked) {
		hooked = false;
		tmp[0] = '\0';
	}*/
	
	return false;
}

LRESULT CALLBACK XMPlayPlayer::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode < 0)
        return CallNextHookEx(hWndHook, nCode, wParam, lParam);

    switch(nCode)
    {
    case HC_ACTION:
		CWPRETSTRUCT *msg = (CWPRETSTRUCT*)lParam;
		if ( msg->message == WM_SETTEXT )
		{
			wcscpy_s( tmp, 100, (wchar_t*)msg->lParam );
		}
        return 0;

    }

    // Call the next hook, if there is one
    return CallNextHookEx(hWndHook, nCode, wParam, lParam);
}