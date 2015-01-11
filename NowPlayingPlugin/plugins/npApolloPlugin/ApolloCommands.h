/*
  Updated 13.02.2003

  Here is a set of macros that can be used to interact with Apollo
  from other programs.

  Available macros:

    Apollo_GetFirstApolloWindowHandle()
    Apollo_GetNextApolloWindowHandle(hWnd)

    Apollo_GetVersion(hWnd)
    Apollo_GetStatus(hWnd)
        note: 0 = stopped, 1 = playing, 2 = paused

    Apollo_OpenFile(hWnd,filename)
    Apollo_OpenDirectory(hWnd,dirname)
    Apollo_OpenURL(hWnd,url)

    Apollo_AddFile(hWnd,filename)
    Apollo_AddDirectory(hWnd,dirname)
    Apollo_AddURL(hWnd,url)
    Apollo_AddRandomEntry(hWnd,conditionExpression)

    Apollo_ClearPlaylist(hWnd)

    Apollo_PreviousTrack(hWnd)
    Apollo_Stop(hWnd)
    Apollo_Play(hWnd)
    Apollo_Pause(hWnd)
    Apollo_NextTrack(hWnd)
    Apollo_FadeOut(hWnd)

    Apollo_PreviousTrackWithCrossfade(hWnd)
    Apollo_PlayWithCrossfade(hWnd)
    Apollo_NextTrackWithCrossfade(hWnd)

    Apollo_SetPlaylistPosition(hWnd,position)
        note: position is the index in playlist, not the track number
    Apollo_GetPlaylistPosition(hWnd)
    Apollo_GetCurrentTrackNumber(hWnd)

    Apollo_SetPlaybackPosition(hWnd,position)
    Apollo_GetPlaybackPosition(hWnd)
    Apollo_GetPlaybackPlaylistPosition(hWnd)
    Apollo_GetPlaybackCountdown(hWnd)
    Apollo_GetPlaybackPlaylistCountDown(hWnd)

    Apollo_SetVolume(hWnd,volume)
        note: volume is value between 0x0000 and 0xffff
    Apollo_GetVolume(hWnd)

    Apollo_CreateSampleSpace(hWnd)
        returns 0 if no tracks were found, otherwise 1
    Apollo_SetSampleSpaceSearchPath(hWnd,searchPath)

    Apollo_GetCurrentlyPlayedFilename(hWnd,buffer,bufferSize)
        copies full filename or URL to the buffer
    Apollo_GetCurrentlyPlayedTitle(hWnd,buffer,bufferSize)

    Apollo_GetPlaylistEntry(hWnd,index,buffer,bufferSize)
        copies full filename, URL or the condition expression
        beginning with semicolon to the buffer
    Apollo_GetPlaylistTitle(hWnd,index,buffer,bufferSize)
*/

// Window message definition

#define WM_APOLLO_COMMAND                       WM_USER + 3

// WM_APOLLO_COMMAND command definitions

#define APOLLO_GETVERSION                       0
#define APOLLO_GETSTATUS                        1

#define APOLLO_OPENFILE                         2
#define APOLLO_OPENDIRECTORY                    3
#define APOLLO_OPENURL                          4

#define APOLLO_ADDFILE                          5
#define APOLLO_ADDDIRECTORY                     6
#define APOLLO_ADDURL                           7
#define APOLLO_ADDRANDOMENTRY                   8

#define APOLLO_CLEARPLAYLIST                    9

#define APOLLO_PREVIOUSTRACK                    10
#define APOLLO_STOP                             11
#define APOLLO_PLAY                             12
#define APOLLO_PAUSE                            13
#define APOLLO_NEXTTRACK                        14
#define APOLLO_FADEOUT                          31

#define APOLLO_PREVIOUSTRACKWITHCROSSFADE       29
#define APOLLO_PLAYWITHCROSSFADE                30
#define APOLLO_NEXTTRACKWITHCROSSFADE           28

#define APOLLO_SETPLAYLISTPOSITION              15
#define APOLLO_GETPLAYLISTPOSITION              16
#define APOLLO_GETCURRENTTRACKNUMBER            17

#define APOLLO_SETPLAYBACKPOSITION              18
#define APOLLO_GETPLAYBACKPOSITION              19
#define APOLLO_GETPLAYBACKPLAYLISTPOSITION      32
#define APOLLO_GETPLAYBACKCOUNTDOWN             33
#define APOLLO_GETPLAYBACKPLAYLISTCOUNTDOWN     34

#define APOLLO_SETVOLUME                        20
#define APOLLO_GETVOLUME                        21

#define APOLLO_CREATESAMPLESPACE                22
#define APOLLO_SETSAMPLESPACESEARCHPATH         23

#define APOLLO_GETCURRENTLYPLAYEDFILENAME       24
#define APOLLO_GETCURRENTLYPLAYEDTITLE          25

#define APOLLO_GETPLAYLISTENTRY                 26
#define APOLLO_GETPLAYLISTTITLE                 27

// Macro definitions

#define Apollo_GetFirstApolloWindowHandle() \
    (HWND)FindWindow(L"Apollo - Main Window",NULL)

#define Apollo_GetNextApolloWindowHandle(hWnd) \
    (HWND)FindWindowEx(NULL,hWnd,L"Apollo - Main Window",NULL)


#define Apollo_GetVersion(hWnd) \
    (int)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETVERSION,0),(LPARAM)0)

#define Apollo_GetStatus(hWnd) \
    (int)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETSTATUS,0),(LPARAM)0)


#define Apollo_OpenFile(hWnd,filename) \
    { \
        COPYDATASTRUCT cds; \
        cds.dwData = APOLLO_OPENFILE; \
        cds.lpData = (void *)filename; \
        cds.cbData = strlen((char *)cds.lpData) + 1; \
        SendMessage(hWnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds); \
    }

#define Apollo_OpenDirectory(hWnd,dirname) \
    { \
        COPYDATASTRUCT cds; \
        cds.dwData = APOLLO_OPENDIRECTORY; \
        cds.lpData = (void *)dirname; \
        cds.cbData = strlen((char *)cds.lpData) + 1; \
        SendMessage(hWnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds); \
    }

#define Apollo_OpenURL(hWnd,url) \
    { \
        COPYDATASTRUCT cds; \
        cds.dwData = APOLLO_OPENURL; \
        cds.lpData = (void *)url; \
        cds.cbData = strlen((char *)cds.lpData) + 1; \
        SendMessage(hWnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds); \
    }


#define Apollo_AddFile(hWnd,filename) \
    { \
        COPYDATASTRUCT cds; \
        cds.dwData = APOLLO_ADDFILE; \
        cds.lpData = (void *)filename; \
        cds.cbData = strlen((char *)cds.lpData) + 1; \
        SendMessage(hWnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds); \
    }

#define Apollo_AddDirectory(hWnd,dirname) \
    { \
        COPYDATASTRUCT cds; \
        cds.dwData = APOLLO_ADDDIRECTORY; \
        cds.lpData = (void *)dirname; \
        cds.cbData = strlen((char *)cds.lpData) + 1; \
        SendMessage(hWnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds); \
    }

#define Apollo_AddURL(hWnd,url) \
    { \
        COPYDATASTRUCT cds; \
        cds.dwData = APOLLO_ADDURL; \
        cds.lpData = (void *)url; \
        cds.cbData = strlen((char *)cds.lpData) + 1; \
        SendMessage(hWnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds); \
    }

#define Apollo_AddRandomEntry(hWnd,conditionExpression) \
    { \
        COPYDATASTRUCT cds; \
        cds.dwData = APOLLO_ADDRANDOMENTRY; \
        cds.lpData = (void *)conditionExpression; \
        cds.cbData = strlen((char *)cds.lpData) + 1; \
        SendMessage(hWnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds); \
    }


#define Apollo_ClearPlaylist(hWnd) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_CLEARPLAYLIST,0),(LPARAM)0)


#define Apollo_PreviousTrack(hWnd) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_PREVIOUSTRACK,0),(LPARAM)0)

#define Apollo_Stop(hWnd) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_STOP,0),(LPARAM)0)

#define Apollo_Play(hWnd) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_PLAY,0),(LPARAM)0)

#define Apollo_Pause(hWnd) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_PAUSE,0),(LPARAM)0)

#define Apollo_NextTrack(hWnd) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_NEXTTRACK,0),(LPARAM)0)

#define Apollo_FadeOut(hWnd) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_FADEOUT,0),(LPARAM)0)


#define Apollo_PreviousTrackWithCrossfade(hWnd) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_PREVIOUSTRACKWITHCROSSFADE,0),(LPARAM)0)

#define Apollo_PlayWithCrossfade(hWnd) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_PLAYWITHCROSSFADE,0),(LPARAM)0)

#define Apollo_NextTrackWithCrossfade(hWnd) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_NEXTTRACKWITHCROSSFADE,0),(LPARAM)0)


#define Apollo_SetPlaylistPosition(hWnd,position) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_SETPLAYLISTPOSITION,0),(LPARAM)position)

#define Apollo_GetPlaylistPosition(hWnd) \
    (int)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETPLAYLISTPOSITION,0),(LPARAM)0)

#define Apollo_GetCurrentTrackNumber(hWnd) \
    (int)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETCURRENTTRACKNUMBER,0),(LPARAM)0)


#define Apollo_SetPlaybackPosition(hWnd,position) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_SETPLAYBACKPOSITION,0),(LPARAM)position)

#define Apollo_GetPlaybackPosition(hWnd) \
    (int)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETPLAYBACKPOSITION,0),(LPARAM)0)

#define Apollo_GetPlaybackPlaylistPosition(hWnd) \
    (int)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETPLAYBACKPLAYLISTPOSITION,0),(LPARAM)0)

#define Apollo_GetPlaybackCountdown(hWnd) \
    (int)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETPLAYBACKCOUNTDOWN,0),(LPARAM)0)

#define Apollo_GetPlaybackPlaylistCountDown(hWnd) \
    (int)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETPLAYBACKPLAYLISTCOUNTDOWN,0),(LPARAM)0)


#define Apollo_SetVolume(hWnd,volume) \
    SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_SETVOLUME,0),(LPARAM)volume)

#define Apollo_GetVolume(hWnd) \
    (int)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETVOLUME,0),(LPARAM)0)


#define Apollo_CreateSampleSpace(hWnd) \
    (int)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_CREATESAMPLESPACE,0),(LPARAM)0)

#define Apollo_SetSampleSpaceSearchPath(hWnd,searchPath) \
    { \
        COPYDATASTRUCT cds; \
        cds.dwData = APOLLO_SETSAMPLESPACESEARCHPATH; \
        cds.lpData = (void *)searchPath; \
        cds.cbData = strlen((char *)cds.lpData) + 1; \
        SendMessage(hWnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds); \
    }


#define Apollo_GetCurrentlyPlayedFilename(hWnd,buffer,bufferSize) \
    { \
        HWND hTempWnd = (HWND)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETCURRENTLYPLAYEDFILENAME,0),(LPARAM)0); \
        SendMessage(hTempWnd,WM_GETTEXT,(WPARAM)bufferSize,(LPARAM)buffer); \
    }

#define Apollo_GetCurrentlyPlayedTitle(hWnd,buffer,bufferSize) \
    { \
        HWND hTempWnd = (HWND)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETCURRENTLYPLAYEDTITLE,0),(LPARAM)0); \
        SendMessage(hTempWnd,WM_GETTEXT,(WPARAM)bufferSize,(LPARAM)buffer); \
    }

#define Apollo_GetPlaylistEntry(hWnd,index,buffer,bufferSize) \
    { \
        HWND hTempWnd = (HWND)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETPLAYLISTENTRY,0),(LPARAM)index); \
        SendMessage(hTempWnd,WM_GETTEXT,(WPARAM)bufferSize,(LPARAM)buffer); \
    }

#define Apollo_GetPlaylistTitle(hWnd,index,buffer,bufferSize) \
    { \
        HWND hTempWnd = (HWND)SendMessage(hWnd,WM_APOLLO_COMMAND,(WPARAM)MAKELONG(APOLLO_GETPLAYLISTTITLE,0),(LPARAM)index); \
        SendMessage(hTempWnd,WM_GETTEXT,(WPARAM)bufferSize,(LPARAM)buffer); \
    }
