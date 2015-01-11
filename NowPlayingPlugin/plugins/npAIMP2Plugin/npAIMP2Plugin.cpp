// "Now Playing" Plugin
// AIMP2 plugin
// (c) 2007 Dmitry D. aka Sp1ritus
// ------------------------------------------

#include "stdafx.h"
#include "npAIMP2Plugin.h"
#include "Aimp2.h"

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

extern "C" NPAIMP2PLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new AIMP2Player () );
}

AIMP2Player::AIMP2Player ()
{
}

AIMP2Player::~AIMP2Player ()
{
}

bool AIMP2Player::Initialize ()
{
	pInfo.name = L"AIMP2";
	pInfo.guid = L"{F7E8C40B-1EC6-4457-B96B-BB945D156A8A}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 255, 0, 255 );
	pInfo.apisupport = true;

	return true;
}
bool AIMP2Player::UnInitialize ()
{
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return true;
	}

	return false;
}

PluginDescription* AIMP2Player::GetPluginDescription ()
{
	return &pInfo;
}

int AIMP2Player::OnCopyData ( HWND hWnd, COPYDATASTRUCT* copyData )
{
	return 0;
}

bool AIMP2Player::OnCheckStatus ( PlayerData & playerData )
{
	HWND hAIMP2Wnd = FindWindow ( AIMP2_MainClassName, NULL );
	if ( hAIMP2Wnd )
	{
		HANDLE FFile = NULL;
		PAIMP2FileInfo pFI = NULL;
		ULONG i = 0;
		FFile = OpenFileMapping ( FILE_MAP_READ, false, AIMP2_RemoteClass );
		pFI = ( PAIMP2FileInfo ) MapViewOfFile( FFile, FILE_MAP_READ, 0, 0, AIMP2_RemoteFileSize );	

		if ( pFI )
		{
			wchar_t buffer[11], buffer2[100];
			wchar_t* base = PWCHAR ( ( char* )( pFI ) + sizeof ( TAIMP2FileInfo ) );

			if ( pFI->nAlbumLen > 0 )
			{
				buffer2[0] = '\0';
				lstrcpyn ( buffer2, base, pFI->nAlbumLen + 1 );
				base += pFI->nAlbumLen;
				playerData.album = buffer2;
			}

			if ( pFI->nArtistLen > 0 )
			{
				buffer2[0] = '\0';
				lstrcpyn ( buffer2, base , pFI->nArtistLen + 1 );
				base += pFI->nDateLen + pFI->nArtistLen; // ignoring date
				playerData.artist = buffer2;
			}

			if ( pFI->nFileNameLen > 0 )
			{
				buffer2[0] = '\0';
				lstrcpyn ( buffer2, base, pFI->nFileNameLen + 1 );
				base += pFI->nFileNameLen;
				playerData.filename = buffer2;
			}
			if ( pFI->nGenreLen > 0 )
			{
				buffer2[0] = '\0';
				lstrcpyn ( buffer2, base, pFI->nGenreLen + 1 );
				base += pFI->nGenreLen;
				playerData.genre = buffer2;
			}
			if ( pFI->nTitleLen > 0 )
			{
				buffer2[0] = '\0';
				lstrcpyn ( buffer2, base, pFI->nTitleLen + 1 );
				playerData.title = buffer2;
			}

			_itow_s ( pFI->nTrackID, buffer, 11, 10 );
			playerData.number = buffer;

			return true;
		}
	  }
	
	return false;
}