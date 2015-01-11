// "Now Playing" Plugin
// JetAudio plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "npJetAudioPlugin.h"

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

extern "C" NPJETAUDIOPLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new JetAudioPlayer () );
}

unsigned int mbs2ws ( const char* s, wchar_t* ws, int maxlen )
{
    int size = MultiByteToWideChar(CP_UTF7, 0, s, -1, 0, 0);
	if ( size > maxlen && maxlen > 0 ) 
		size = maxlen;
    MultiByteToWideChar(CP_UTF7, 0, s, -1, ws, size);
    return 1;
}

JetAudioPlayer::JetAudioPlayer ()
{
}

JetAudioPlayer::~JetAudioPlayer ()
{
}

bool JetAudioPlayer::Initialize ()
{
	pInfo.name = L"JetAudio";
	pInfo.guid = L"{AC1F6ABE-3B4F-41df-9D4C-86482FA8E646}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 255, 255, 255 );
	pInfo.apisupport = true;

	if ( !_RegisterClass () )
	{
		return false;
	}

	return true;
}
bool JetAudioPlayer::UnInitialize ()
{
	_UnRegisterClass ();
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return true;
	}

	return false;
}

PluginDescription* JetAudioPlayer::GetPluginDescription ()
{
	return &pInfo;
}

LRESULT JetAudioPlayer::WindowProc ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
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
			const char* mbstr = reinterpret_cast<char*> ( pCopyData->lpData );
			wchar_t buffer[260] = L"";
			mbs2ws ( mbstr, buffer, 40 );

			switch ( pCopyData->dwData )
			{
			case JRC_COPYDATA_ID_TRACK_TITLE:
				pShareData->title = buffer;
				break;
			case JRC_COPYDATA_ID_TRACK_ARTIST:
				pShareData->artist = buffer;
				break;
			case JRC_COPYDATA_ID_ALBUMID:
				pShareData->album = buffer;
				break;
			case JRC_COPYDATA_ID_TRACK_FILENAME:
				pShareData->filename = buffer;
				break;
			}

		}

	}

	return true;
}

bool JetAudioPlayer::_RegisterClass ()
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
    wc.lpszClassName	= L"NPJetAudio_DataEx";
    
	if ( !RegisterClass(&wc) )
		return false;

	dataExchangeWnd = CreateWindow (
							L"NPJetAudio_DataEx",
							L"NPJETAUDIO_SECRET_PLACE",
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

bool JetAudioPlayer::_UnRegisterClass ()
{
	DestroyWindow ( dataExchangeWnd );
	UnregisterClass ( L"NPJetAudio_DataEx", GetHandle () );
	return true;
}

bool JetAudioPlayer::OnCheckStatus ( PlayerData & pdata)
{
	HWND hJetAudioWnd = FindWindow ( L"COWON Jet-Audio Remocon Class", L"Jet-Audio Remote Control" );
	if ( hJetAudioWnd && dataExchangeWnd )
	{
		SendMessage ( hJetAudioWnd, WM_REMOCON_GETSTATUS, reinterpret_cast<WPARAM> ( dataExchangeWnd ), GET_STATUS_TRACK_FILENAME );
		SendMessage ( hJetAudioWnd, WM_REMOCON_GETSTATUS, reinterpret_cast<WPARAM> ( dataExchangeWnd ), GET_STATUS_TRACK_TITLE );
		SendMessage ( hJetAudioWnd, WM_REMOCON_GETSTATUS, reinterpret_cast<WPARAM> ( dataExchangeWnd ), GET_STATUS_TRACK_ARTIST );
		SendMessage ( hJetAudioWnd, WM_REMOCON_GETSTATUS, reinterpret_cast<WPARAM> ( dataExchangeWnd ), GET_STATUS_ALBUMID );

		pdata = playerData;
		return true;
	}

	return false;
}