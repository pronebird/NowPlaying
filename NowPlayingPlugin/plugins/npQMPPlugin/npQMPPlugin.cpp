// "Now Playing" Plugin
// QMP plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "npQMPPlugin.h"

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

extern "C" NPQMPPLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new QMPPlayer () );
}

QMPPlayer::QMPPlayer ()
{
}

QMPPlayer::~QMPPlayer ()
{
}

PluginDescription* QMPPlayer::GetPluginDescription ()
{
	return &pInfo;
}

bool QMPPlayer::Initialize ()
{
	pInfo.name = L"QMP";
	pInfo.guid = L"{EB283CAE-9291-4b97-B9A1-C3C3DB8C5B0F}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 255, 255, 255 );
	pInfo.apisupport = true;


	if ( !_RegisterClass () )
	{
		return false;
	}

	return true;
}
bool QMPPlayer::UnInitialize ()
{
	_UnRegisterClass ();
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return true;
	}

	return false;
}

bool QMPPlayer::OnCheckStatus ( PlayerData & pdata )
{
		HWND hQuintessentialWnd = FindWindow ( L"PlayerCanvas", NULL );
		if ( hQuintessentialWnd && dataExchangeWnd )
		{
			SendMessage ( hQuintessentialWnd, WM_QCD_GET_TRACKNUMBER, reinterpret_cast<WPARAM> ( dataExchangeWnd ), QMP_TRACKNUMBER );
			SendMessage ( hQuintessentialWnd, WM_QCD_GET_TITLEALBUM, reinterpret_cast<WPARAM> ( dataExchangeWnd ), QMP_TITLEALBUM );
			SendMessage ( hQuintessentialWnd, WM_QCD_GET_TITLETRACK, reinterpret_cast<WPARAM> ( dataExchangeWnd ), QMP_TITLETRACK );
			SendMessage ( hQuintessentialWnd, WM_QCD_GET_GENREALBUM, reinterpret_cast<WPARAM> ( dataExchangeWnd ), QMP_ALBUMGENRE );
			SendMessage ( hQuintessentialWnd, WM_QCD_GET_ARTISTALBUM, reinterpret_cast<WPARAM> ( dataExchangeWnd ), QMP_ARTISTALBUM );
			//SendMessage ( hQuintessentialWnd, WM_QCD_GETLENGTH, reinterpret_cast<WPARAM> ( dataExchangeWnd ), QMP_TRACKLENGTH );
			pdata = playerData;

			return true;
		}
	
	return false;
}

bool QMPPlayer::_RegisterClass ()
{
	WNDCLASS wc;
	HINSTANCE hInstance = GetHandle ();
	
	wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= (WNDPROC) WindowProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 4;
    wc.hInstance		= hInstance;
    wc.hIcon			= NULL;
    wc.hCursor			= NULL;
    wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName		= 0;
    wc.lpszClassName	= L"NPQMP_DataEx";
    
	if ( !RegisterClass(&wc) )
		return false;

	dataExchangeWnd = CreateWindow (
							L"NPQMP_DataEx",
							L"NPQMP_SECRET_PLACE",
							WS_POPUP,
							0,
							0,
							100,
							70,
							NULL,
							NULL,
							hInstance,
							static_cast<void*>(&playerData) );

	return dataExchangeWnd != NULL;
}

bool QMPPlayer::_UnRegisterClass ()
{
	DestroyWindow ( dataExchangeWnd );
	UnregisterClass ( L"NPQMP_DataEx", GetHandle () );
	return true;
}

LRESULT QMPPlayer::WindowProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static PlayerData* pShareData;

	switch (message)
	{
	case WM_CREATE:
		{
			CREATESTRUCT* cs = (LPCREATESTRUCT) lParam;
			pShareData = static_cast<PlayerData*> ( cs->lpCreateParams );
		}
		break;
	case WM_COPYDATA:
		{
			COPYDATASTRUCT* pCopyData= reinterpret_cast<COPYDATASTRUCT*> ( lParam );
			switch ( pCopyData->dwData )
			{
			case QMP_TITLETRACK:
				pShareData->title = static_cast<wchar_t*> ( pCopyData->lpData );
				break;
			case QMP_TITLEALBUM:
				pShareData->album = static_cast<wchar_t*> ( pCopyData->lpData );
				break;
			case QMP_TRACKNUMBER:
				pShareData->number = static_cast<wchar_t*> ( pCopyData->lpData );
				break;
			case QMP_ALBUMGENRE:
				pShareData->genre = static_cast<wchar_t*> ( pCopyData->lpData );
				break;
			case QMP_ARTISTALBUM:
				pShareData->artist = static_cast<wchar_t*> ( pCopyData->lpData );
				break;
			}

		}

	}
	return true;
}