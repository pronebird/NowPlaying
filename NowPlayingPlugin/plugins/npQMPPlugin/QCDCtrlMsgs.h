/*
** QCD IPC Messages
** For retrieving information on QCD or for 
** controlling QCD from other processes
** 
** To get QCD window handle:
** HWND hwndPlayer = FindWindow("PlayerCanvas", NULL);
*/

/*
** These are WM_USER messages that you can use to get
** information on QCD from other processes
** 
** To send these, use:
** SendMessage(hwndPlayer, msg, data, value);
*/

// value = 0 - version, 1 - build
//
// returns version of player (see opGetPlayerVersion in QCDModDefs.h for return values)

#define WM_QCD_GETVERSION			(WM_USER+1000)	// get player version


// Track metadata messages
//
// data = handle to window that will receive the WM_COPYDATA message with 
//			the requested data
// value = application defined value that will be returned with WM_COPYDATA
//			(the strings returned in WM_COPYDATA are always UNICODE)
//
// Example:
// SendMessage(hwndPlayer, WM_QCD_GET_ARTISTALBUM, hwndReturn, userData);
//		(the artist value will be sent to hwndReturn)
// returns TRUE if WM_COPYDATA is sent
//

#define WM_QCD_GET_ARTISTALBUM			(WM_USER+1024)	// get album artist for current track
#define WM_QCD_GET_ARTISTTRACK			(WM_USER+1025)	// get track artist for current track
#define WM_QCD_GET_TITLEALBUM			(WM_USER+1026)	// get album title for current track
#define WM_QCD_GET_TITLETRACK			(WM_USER+1027)	// get track title for current track
#define WM_QCD_GET_TRACKNUMBER			(WM_USER+1028)	// get track number for current track (eg: "1/10")
#define WM_QCD_GET_DISCNUMBER			(WM_USER+1029)	// get disc number for current track (eg: "1/1")	
#define WM_QCD_GET_GENREALBUM			(WM_USER+1030)	// get album genre for current track
#define WM_QCD_GET_GENREALBUM2			(WM_USER+1031)	// get 2ndary album genre for current track
#define WM_QCD_GET_GENRETRACK			(WM_USER+1032)	// get track genre for current track
#define WM_QCD_GET_GENRETRACK2			(WM_USER+1033)	// get 2ndary track genre for current track
#define WM_QCD_GET_YEARALBUM			(WM_USER+1034)	// get album year for current track
#define WM_QCD_GET_YEARTRACK			(WM_USER+1035)	// get track year for current track
#define WM_QCD_GET_PLAYLISTTEXT			(WM_USER+1048)	// get playlist entry (as displayed in playlist)



// Track time messages
//
// data = not used
// value = playlist index (0 based, -1 for current track)
//
// returns value

#define WM_QCD_GETLENGTH			(WM_USER+1064)	// get length of current track (in milliseconds)
#define WM_QCD_GETTIME				(WM_USER+1065)	// get elapsed time of current track (in milliseconds)



/*
** These are WM_COMMAND messages that you can use to control
** specifics of QCD.
** 
** To send these, use:
** SendMessage(hwndPlayer, WM_COMMAND, command_name, 0);
*/

// open window messages
#define QCD_COMMAND_ABOUT			40001	// about
#define QCD_COMMAND_ONTOP			40002	// toggle 'always on top'
#define QCD_COMMAND_PREF			40004	// preferences
#define QCD_COMMAND_PLUGINS			40006	// plugin browser
#define QCD_COMMAND_SKINS			40026	// skin browser
#define QCD_COMMAND_OPENFILE		40007	// open file/stream
#define QCD_COMMAND_OPENFOLDER		40034	// open folder
#define QCD_COMMAND_INFOBROWSER		40017	// browser window
#define QCD_COMMAND_EDITINFO		40025	// edit info
#define QCD_COMMAND_EXTENDEDINFO	40005	// extended info
#define QCD_COMMAND_HELP			42001	// help

// skin control messages
#define QCD_COMMAND_NEXTSKIN		40027	// switch to next skin mode
#define QCD_COMMAND_SKINBASE		40031	// switch to default QCD skin
#define QCD_COMMAND_SKIN1			41050	// switch to skin mode 1 to 9
#define QCD_COMMAND_SKIN2			41051	
#define QCD_COMMAND_SKIN3			41052
#define QCD_COMMAND_SKIN4			41053
#define QCD_COMMAND_SKIN5			41054
#define QCD_COMMAND_SKIN6			41055
#define QCD_COMMAND_SKIN7			41056
#define QCD_COMMAND_SKIN8			41057
#define QCD_COMMAND_SKIN9			41058
#define QCD_COMMAND_SKINREFRESH		41059	// refresh current skin

// playback control messages
#define QCD_COMMAND_REPEATALL		40008	// toggle repeat all
#define QCD_COMMAND_REPEATTRACK		40009	// toggle repeat track
#define QCD_COMMAND_SHUFFLE			40010	// toggle shuffle play
#define QCD_COMMAND_TRKBWD			40012	// previous track
#define QCD_COMMAND_TRKFWD			40013	// next track
#define QCD_COMMAND_STOP			40014	// stop
#define QCD_COMMAND_PAUSE			40015	// pause
#define QCD_COMMAND_PLAY			40016	// play (Presses Play Button - to play index see QCD_COMMAND_PLAYINDEX)
#define QCD_COMMAND_PLAYSEL			40048   // play selected playlist items only
#define QCD_COMMAND_FWD5			41024	// seek forward 5 seconds
#define QCD_COMMAND_FWD10			41040	// seek forward 10 seconds
#define QCD_COMMAND_FWD20			41041	// seek forward 20 seconds
#define QCD_COMMAND_FWD40			41042	// seek forward 40 seconds
#define QCD_COMMAND_BWD5			41025	// seek back 5 seconds
#define QCD_COMMAND_BWD10			41043	// seek back 10 seconds
#define QCD_COMMAND_BWD20			41044	// seek back 20 seconds
#define QCD_COMMAND_BWD40			41045	// seek back 40 seconds

// visual effects control messages
#define QCD_COMMAND_NEXTVISPLUGIN	41022	// switch to next visual plugin
#define QCD_COMMAND_EXTERNALVIS		41035	// open external visual effects window
#define QCD_COMMAND_INTERNALVIS		41036	// set visuals to internal (within skin)
#define QCD_COMMAND_FULLSCREENVIS	41037	// launch fullscreen visual effects window
#define QCD_COMMAND_NOVIS			41038	// turn off visuals

// volume control messages
#define QCD_COMMAND_VOLUP			41026	// Volume goes up 1%
#define QCD_COMMAND_VOLDOWN			41027	// Volume goes down 1%

// playlist control messages
#define QCD_COMMAND_SAVEPLAYLIST	40023	// open 'save playlist as' window
#define QCD_COMMAND_MARKALL			40018	// selects all tracks in playlist
#define QCD_COMMAND_DELETEMARKED	40019	// delete all selected tracks
#define QCD_COMMAND_DELALL			40020	// delete all tracks
#define QCD_COMMAND_CLEARMARKS		40021	// deselect all tracks
#define QCD_COMMAND_DELUNMARKED		42000	// delete all unselected tracks
											// NOTE: to delete individual indeces, see opDeleteIndex in QCDModDefs.h

#define QCD_COMMAND_EJECT			40011	// replace playlist
#define QCD_COMMAND_EJECT_CDROM		12000	// eject cdrom X: usage (QCD_COMMAND_EJECT_CDROM + driveIndex) (eg: drive 'A' = 0, drive 'D' = 4)
#define QCD_COMMAND_EJECT_OPENALL	12060	// eject all cdroms
#define QCD_COMMAND_EJECT_CLOSEALL	12070	// close all cdroms

#define QCD_COMMAND_PLAYINDEX		20000	// play index: usage (QCD_COMMAND_PLAYINDEX + index)
#define QCD_COMMAND_LOADCDTRACKS	10000	// load cd tracks: usage (QCD_COMMAND_LOADCDTRACKS + (drive num * 2) [+ 1 to erase pl])
#define QCD_COMMAND_REQUERYCD		11000	// requery info for cd: usage (QCD_COMMAND_REQUERYCD + drive num)

#define QCD_COMMAND_SORT_NUMBER				40100	// sort by playlist number
#define QCD_COMMAND_SORT_TRACKNAME			40101	// sort by trackname
#define QCD_COMMAND_SORT_FILENAME			40102	// sort by filename
#define QCD_COMMAND_SORT_PATH				40103	// sort by path / filename
#define QCD_COMMAND_SORT_ARTISTTRACKNAME	40106	// sort by artist / track name
#define QCD_COMMAND_SORT_LENGTH				40108	// sort by track length
#define QCD_COMMAND_SORT_TRACKNUMBER		40109	// sort by track number 
#define QCD_COMMAND_SORT_REVERSEPLAYLIST	40104	// reverse playlist
#define QCD_COMMAND_SORT_RENUMBER			40105	// renumber playlist
#define QCD_COMMAND_SORT_RANDOMIZE			40107	// randomize playlist

// timer control messages
#define QCD_COMMAND_TRACKELAPSED	40200	// set timer to elapsed time for current track
#define QCD_COMMAND_TRACKREMAINING	40201	// set timer to remaining time for current track
#define QCD_COMMAND_PLELAPSED		40202	// set timer to elapsed time for playlist
#define QCD_COMMAND_PLREMAINING		40203	// set timer to remaining time for playlist
#define QCD_COMMAND_SHOWHOURS		40204	// toggle whether timer shows time in hours

// EQ control messages
#define QCD_COMMAND_EQ_PRESETS		42002
#define QCD_COMMAND_EQ_NEXTPRESET	42015
#define QCD_COMMAND_EQ_PREVPRESET	42016

