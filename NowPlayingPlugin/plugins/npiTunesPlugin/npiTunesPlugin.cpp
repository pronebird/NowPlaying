// "Now Playing" Plugin
// iTunes plugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "npiTunesPlugin.h"

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

extern "C" NPITUNESPLUGIN_API INPPlugin* GetInstance ()
{
	return static_cast<INPPlugin*>( new iTunesPlayer () );
}

iTunesPlayer::iTunesPlayer ()
{
}

iTunesPlayer::~iTunesPlayer ()
{
}

bool iTunesPlayer::Initialize ()
{
	pInfo.name = L"iTunes";
	pInfo.guid = L"{5105A1F0-0EAB-4713-81EF-00A1A94B7744}";
	pInfo.image = LoadBitmap ( GetHandle (), MAKEINTRESOURCE ( IDB_BITMAP1 ) );
	pInfo.mask = RGB ( 255, 255, 255 );
	pInfo.apisupport = true;

	return true;
}
bool iTunesPlayer::UnInitialize ()
{
	if ( pInfo.image )
	{
		DeleteObject ( pInfo.image );
		return true;
	}
	return false;
}

PluginDescription* iTunesPlayer::GetPluginDescription ()
{
	return &pInfo;
}


bool iTunesPlayer::OnCheckStatus ( PlayerData & playerData )
{
	if ( FindWindow ( L"iTunes", L"iTunes" ) == NULL )
		return false;

	CoInitialize ( NULL );

	CComPtr<IiTunes> spITunesApp;
	CComPtr<IITTrack> track;
	CComPtr<IITFileOrCDTrack> fileTrack;

	HRESULT hr = ::CoCreateInstance(CLSID_iTunesApp, 
                                    NULL, CLSCTX_LOCAL_SERVER,
                                    __uuidof(spITunesApp), 
                                    (void **)&spITunesApp);

	if ( SUCCEEDED ( hr ) && spITunesApp )
	{
		if ( SUCCEEDED ( spITunesApp->get_CurrentTrack ( &track ) ) && track )
		{
			wchar_t buffer[11];
			CComBSTR bstrName;
		    CComBSTR bstrArtist;
		    CComBSTR bstrAlbum;
		    CComBSTR bstrTime;
			CComBSTR bstrGenre;
			CComBSTR bstrLocation;
			long trackNumber;

			if ( SUCCEEDED ( track->QueryInterface (__uuidof(fileTrack),(void **)&fileTrack) ) )
			{
				fileTrack->get_Location ( &bstrLocation );
				playerData.filename = bstrLocation;
			}

			if ( SUCCEEDED ( track->get_Name ( &bstrName ) ) )
			{
				playerData.title = bstrName;
			}

			if ( SUCCEEDED ( track->get_Album ( &bstrAlbum ) ) )
			{
				playerData.album = bstrAlbum;
			}

			if ( SUCCEEDED ( track->get_Artist ( &bstrArtist ) ) )
			{
				playerData.artist = bstrArtist;
			}

			if ( SUCCEEDED ( track->get_Time ( &bstrTime ) ) )
			{
				playerData.length = bstrTime;
			}

			if ( SUCCEEDED ( track->get_Genre ( &bstrGenre ) ) )
			{
				playerData.genre = bstrGenre;
			}

			if ( SUCCEEDED ( track->get_TrackNumber ( &trackNumber ) ) )
			{
				_ltow_s ( trackNumber, buffer, 11, 10 );
				playerData.number = buffer;
			}
			track.Release ();
			fileTrack.Release ();
			spITunesApp.Release ();
			CoUninitialize ();

			return true;
		}
	}
	track.Release ();
	fileTrack.Release ();
	spITunesApp.Release ();
	CoUninitialize ();

	return false;
}