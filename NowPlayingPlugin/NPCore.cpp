// "Now Playing" QIP Infium Plugin
// class NPCore
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "NPCore.h"
#include "NPPluginManager.h"
#include "NPLanguageManager.h"
#include "AppPropertySheet.h"

PluginInfo NPCore::pluginInfo;

DWORD __stdcall NowPlayingThread ( void* pClass )
{
	NPCore* pCore = static_cast<NPCore*> ( pClass );
	while ( 1 )
	{
		DWORD dwTime = pCore->GetUpdateInterval ();
		pCore->OnUpdate ();
		Sleep ( dwTime*1000 );
	}
	return 0;
}

NPCore::NPCore ( IQIPPluginService* plugServ ) : refCounter ( 0 )
{
	pluginService = plugServ;
	dwThreadId = 0;
	hThread = NULL;
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
// ::DefWindowProc(NULL, 0, 0, 0L);

//	AtlInitCommonControls ( ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES); 

	hRes = _Module.Init(NULL, GetModuleHandle ( NULL ) );
	ATLASSERT(SUCCEEDED(hRes));

	pluginManager = static_cast<INPPluginManager*> ( new NPPluginManager () );
	languageManager = static_cast<INPLanguageManager*> ( new NPLanguageManager () );

	quickSwitchTray = NULL;
	quickSwitchButton = NULL;
	coreSettings.enableQuickSwitchTray = false;
	coreSettings.enableQuickSwitchToolBtn = false;
}

NPCore::~NPCore ()
{
	delete pluginManager;
	delete languageManager;

	_Module.Term();
	::CoUninitialize();
}

bool NPCore::CreateTraySwitcher ()
{
	if ( !quickSwitchTray )
	{
		quickSwitchTray = new AppTrayIcon ();
		quickSwitchTray->Create ( this );

		return true;
	}

	return false;
}

bool NPCore::DestroyTraySwitcher ()
{
	if ( quickSwitchTray )
	{
		delete quickSwitchTray;
		quickSwitchTray = NULL;

		return true;
	}

	return false;
}

bool NPCore::CreateToolbarSwitcher ()
{
	if ( !quickSwitchButton )
	{
		quickSwitchButton = new AppToolbarButton ( this );
		quickSwitchButton->Create ();

		return true;
	}

	return false;
}

bool NPCore::DestroyToolbarSwitcher ()
{
	if ( quickSwitchButton )
	{
		delete quickSwitchButton;
		quickSwitchButton = NULL;

		return true;
	}

	return false;
}

PluginInfo* __stdcall NPCore::GetPluginInfo ()
{
	return &pluginInfo;
}

void __stdcall NPCore::OnQipMessage ( PluginMessage* pMsg )
{
	switch ( pMsg->Msg )
	{
		case PM_PLUGIN_LOAD_SUCCESS:
			OnLoadSuccess();
		break;
		case PM_PLUGIN_RUN:
			OnRun ();
		break;
		case PM_PLUGIN_QUIT:
			OnQuit ();
		break;
		case PM_PLUGIN_ENABLE:
			OnEnable ();
		break;
		case PM_PLUGIN_DISABLE:
			OnDisable ();
		break;
		case PM_PLUGIN_OPTIONS:
			OnOptions();
		break;
		case PM_PLUGIN_WRONG_SDK_VER:
			MessageBox ( NULL, L"Wrong version detected by QIP. Now plugin will be unloaded.", L"\"Now Playing\" incompatibility", MB_OK );
			break;
		case PM_PLUGIN_CAN_ADD_BTNS:
			{
				if ( quickSwitchButton )
				{
					PluginMessage retMsg;
					AddBtnInfo *bi = quickSwitchButton->GetButton ();
					retMsg.Msg = PM_PLUGIN_ADD_BTN;
					retMsg.DllHandle = pluginInfo.DllHandle;
					retMsg.WParam = reinterpret_cast<int>(bi);
					retMsg.LParam = NULL;
					retMsg.NParam = NULL;
					retMsg.Result = 0;
					pluginService->OnPluginMessage ( &retMsg );
				}
			}
		break;
		case PM_PLUGIN_MSG_BTN_CLICK:
			if ( quickSwitchButton )
			{
				quickSwitchButton->OnSync ();
				quickSwitchButton->OnClick ();
			}

			if ( quickSwitchTray )
				quickSwitchTray->OnSync ();
			break;
	};
}

void NPCore::OnLoadSuccess ()
{
	WCHAR buf[MAX_PATH];
	std::wstring pluginsPath, langPath;

	GetModuleFileName ( pluginInfo.DllHandle, buf, MAX_PATH );
	
	pluginInfo.DllPath = SysAllocString(buf);
	pluginInfo.QipSdkVerMajor   = QIP_SDK_VER_MAJOR;
	pluginInfo.QipSdkVerMinor   = QIP_SDK_VER_MINOR;
	pluginInfo.PlugVerMajor     = PLUGIN_VER_MAJOR;
	pluginInfo.PlugVerMinor     = PLUGIN_VER_MINOR;
	pluginInfo.PluginName       = SysAllocString(PLUGIN_NAME);
	pluginInfo.PluginAuthor     = SysAllocString(PLUGIN_AUTHOR);

	pluginsPath = GetPath ();
	pluginsPath.append ( L"\\plugins" );

	langPath = GetPath ();
	langPath.append ( L"\\languages" );

	languageManager->LoadLanguages ( langPath );
	pluginManager->LoadPluginsDirectory ( pluginsPath );
}

void NPCore::OnRun ()
{
	PluginSpecific* options = pluginService->PluginOptions(pluginInfo.DllHandle);
	LoadCoreSettings ();
	DWORD dwThreadFlags =  options->Bool1 ? CREATE_SUSPENDED : NULL;
	hThread = CreateThread ( NULL, NULL, &NowPlayingThread, this, dwThreadFlags, &dwThreadId );
}

void NPCore::OnQuit ()
{
	DestroyTraySwitcher ();
	DestroyToolbarSwitcher ();

	TerminateThread ( hThread, 0 );
	CloseHandle ( hThread );
	hThread = NULL;

	SysFreeString(pluginInfo.DllPath);
	SysFreeString(pluginInfo.PluginName);
	SysFreeString(pluginInfo.PluginAuthor);
}

void NPCore::OnEnable ()
{
	PluginSpecific* options = pluginService->PluginOptions(pluginInfo.DllHandle);
	options->Bool1 = 0;
	if ( hThread )
	{
		ResumeThread ( hThread );
		CreateTraySwitcher ();
		CreateToolbarSwitcher ();
	}
	else	
	{
		OnRun ();
	}
}

void NPCore::OnDisable ()
{
	PluginSpecific* options = pluginService->PluginOptions(pluginInfo.DllHandle);
	
	DestroyTraySwitcher ();
	DestroyToolbarSwitcher ();

	if ( hThread )
		SuspendThread ( hThread );

	options->Bool1 = 1;
}

void NPCore::OnOptions ()
{
	AppPropertySheet *appSettings = new AppPropertySheet ( this );
	appSettings->DoModal ();
	delete appSettings;
	LoadCoreSettings ();
}

std::wstring NPCore::GetPath ()
{
	std::wstring appPath = pluginInfo.DllPath;
	int npos = appPath.rfind ( L"\\" );
	if ( npos >= 0 )
		appPath = appPath.substr ( 0, npos );
	return appPath;
}

unsigned long NPCore::GetUpdateInterval ()
{
	return coreSettings.updateInterval;
}

INPPluginManager* NPCore::GetPluginManager ()
{
	return pluginManager;
}

INPLanguageManager* NPCore::GetLanguageManager ()
{
	return languageManager;
}

bool NPCore::SetDefaultSettings ()
{
	xml_document xml;

	std::wstring xmlPath = GetPath ();
	xmlPath.append ( L"\\Settings.xml" );

	xml.load_file ( as_utf8 ( xmlPath.c_str () ).c_str () );
	xml.append_child(node_comment).set_value ( " THIS IS FILE WAS AUTOMATICALLY GENERATED BY NOWPLAYING PLUGIN " );
	xml.append_child(node_comment).set_value ( " DO NOT CHANGE THIS FILE MANUALLY " );

	xml_node Settings = xml.append_child (),
			 General = Settings.append_child (),
			 Extended = Settings.append_child (),
			 Plugins = Settings.append_child (),
			 StatusRoutine = Extended.append_child (),
			 QuickSwitch = Extended.append_child (),
			 UINPPluginRoutine = Plugins.append_child (),
			 UI_PluginsSplitter = UINPPluginRoutine.append_child ();

	std::vector<INPPlugin*> pls = GetPluginManager()->GetPluginList();
	std::wstring activePlugin = L"{}";

	if ( pls.size() > 0 )
		activePlugin = pls[0]->GetPluginDescription()->guid;

	General.append_attribute ( "PL_GUID" ).set_value( as_utf8(activePlugin.c_str()).c_str() );

	General.append_attribute ( "UILanguage" ) = "1033";
	General.append_attribute ( "UpdateInterval" ) = 10;

	StatusRoutine.append_attribute ( "StatusName" ) = "Music";
	StatusRoutine.append_attribute ( "XStatusText" ) = "Now playing: {%artist%} - {%title%}";
	StatusRoutine.append_attribute ( "XStatusAltText" ) = "Now playing: {%status%}";
	StatusRoutine.append_attribute ( "ChangeOnlyXStatus" ) = true;
	StatusRoutine.append_attribute ( "ChangeOnlyWhenIcon" ) = true;
	StatusRoutine.append_attribute ( "StatusIconId" ) = 10;

	QuickSwitch.append_attribute ( "Enable" ) = false;
	QuickSwitch.append_attribute ( "SysTray" ) = false;
	QuickSwitch.append_attribute ( "BelowUserPicture" ) = false;

	UI_PluginsSplitter.append_attribute ( "Position" ) = "200";

	Settings.set_name ( "Settings" );
	General.set_name ( "General" );
	Extended.set_name ( "Extended" );
	Plugins.set_name ( "Plugin" );
	StatusRoutine.set_name ( "Status" );
	QuickSwitch.set_name ( "QuickSwitch" );
	UINPPluginRoutine.set_name ( "UI" );
	UI_PluginsSplitter.set_name ( "CSplitter" );

	xml.save_file ( pugi::as_utf8 ( xmlPath.c_str () ).c_str () );

	return true;
}

bool NPCore::LoadCoreSettings ()
{
	std::wstring xmlPath = GetPath ();
	std::wstring selPlayerGUID = L"{}";
	xmlPath.append ( L"\\Settings.xml" );

	xml_document xml;
	if ( !xml.load_file ( as_utf8 ( xmlPath.c_str () ).c_str () ) )
		if ( SetDefaultSettings () )
			return LoadCoreSettings ();
		else
			return false;

		xpath_node retNode = xml.select_single_node ( "//Settings/General" ),
				   stNode = xml.select_single_node ( "//Settings/Extended//Status" ),
				   qsNode = xml.select_single_node ( "//Settings/Extended//QuickSwitch" );

		if ( retNode.node () )
		{
			xml_attribute attrSelectedPlayer = retNode.node ().attribute ( "PL_GUID" ),
						  attrUpdateInterval = retNode.node ().attribute ( "UpdateInterval" ),
						  attrCurLang = retNode.node ().attribute ( "UILanguage" );
			

			if ( attrCurLang )
			{
				GetLanguageManager ()->SetLanguageById ( attrCurLang.as_int () );
			}

			if ( attrUpdateInterval )
			{
				coreSettings.updateInterval = attrUpdateInterval.as_int ();
				coreSettings.updateInterval = coreSettings.updateInterval < 5 ? 5 : coreSettings.updateInterval > 120 ? 120 : coreSettings.updateInterval;
			} else {
				coreSettings.updateInterval = 10;
			}
			
			if ( attrSelectedPlayer )
				selPlayerGUID = as_utf16(attrSelectedPlayer.value());
			
			
		} else {
			coreSettings.updateInterval = 10;
			std::vector<NPLanguage> langs = GetLanguageManager()->GetLanguageList();
			if ( langs.size() > 0 )
				GetLanguageManager ()->SetLanguageById ( langs[0].id );
		}

		GetPluginManager ()->SetActivePlugin ( selPlayerGUID );

		if ( stNode.node () )
		{
			xml_attribute attrStatusName = stNode.node ().attribute ( "StatusName" ),
						  attrStatusX = stNode.node ().attribute ( "XStatusText" ),
						  attrAltStatusX = stNode.node ().attribute ( "XStatusAltText" ), 
						  attrChangeOnlyXStatus = stNode.node ().attribute ( "ChangeOnlyXStatus" ),
						  attrChangeOnlyWhenIcon = stNode.node ().attribute ( "ChangeOnlyWhenIcon" ),
						  attrStatusIconId = stNode.node ().attribute ( "StatusIconId" );
			if ( attrStatusName )
			{
				coreSettings.strStatusName = as_utf16 ( attrStatusName.value () ).c_str ();
			} else {
				coreSettings.strStatusName = L"Music";
			}

			if ( attrStatusX )
			{
				coreSettings.strFormat = as_utf16 ( attrStatusX.value () ).c_str ();
			} else {
				coreSettings.strFormat = L"Now playing: {%artist%} - {%tracktitle%}";
			}

			if ( attrAltStatusX )
			{
				coreSettings.strAltFormat = as_utf16 ( attrAltStatusX.value () ).c_str ();
			} else {
				coreSettings.strAltFormat = L"Now playing: {%status%}";
			}

			if ( attrChangeOnlyXStatus )
			{
				coreSettings.changeOnlyXStatus = attrChangeOnlyXStatus.as_bool ();
			} else {
				coreSettings.changeOnlyXStatus = true;
			}

			if ( attrChangeOnlyWhenIcon )
			{
				coreSettings.changeOnlyWhenIcon = attrChangeOnlyWhenIcon.as_bool ();
			} else {
				coreSettings.changeOnlyWhenIcon = true;
			}

			if ( attrStatusIconId )
			{
				coreSettings.changeOnlyWhenIconId = attrStatusIconId.as_int ();
			} else {
				coreSettings.changeOnlyWhenIconId = 10;
			}
			
			
		} else {
			coreSettings.strStatusName = L"";
			coreSettings.strFormat = L"Now Playing: {%artist%} - {%tracktitle%}";
		}

		if ( qsNode.node () )
		{
			xml_attribute attrQSEnable = qsNode.node ().attribute ( "Enable" ),
						  attrQSSysTray = qsNode.node ().attribute ( "SysTray" ),
						  attrQSBelowUserPic = qsNode.node ().attribute ( "BelowUserPicture" );

			coreSettings.enableQuickSwitchTray = attrQSSysTray.as_bool ();
			coreSettings.enableQuickSwitchToolBtn = attrQSBelowUserPic.as_bool ();

			if ( attrQSEnable )
			{
				bool what = attrQSEnable.as_bool ();
				if ( what )
				{
					if ( coreSettings.enableQuickSwitchTray )
						CreateTraySwitcher (); 
					else
						DestroyTraySwitcher ();

					if ( coreSettings.enableQuickSwitchToolBtn )
						CreateToolbarSwitcher ();
					else
						DestroyToolbarSwitcher ();
				} else {
					DestroyTraySwitcher ();
					DestroyToolbarSwitcher ();
				}
			}
		}

	if ( quickSwitchTray )
		quickSwitchTray->OnSync ();
	if ( quickSwitchButton )
		quickSwitchButton->OnSync ();
	return true;
}

bool NPCore::UpdateStatus ()
{
	PluginMessage pMsg;
	int statusid = 0;
	
	// get current status, just temporary stub
	pMsg.Msg = PM_PLUGIN_XSTATUS_GET;
	pMsg.DllHandle = pluginInfo.DllHandle;
	pMsg.Result = 0;
	pMsg.WParam = 0;
	pMsg.LParam = 0;
	pMsg.NParam = 0;
	pluginService->OnPluginMessage ( &pMsg );

	// (changeOnlyWhenIconId + 1) fix for qip enumeration compatibility
	if ( coreSettings.changeOnlyWhenIcon && pMsg.WParam != coreSettings.changeOnlyWhenIconId+1 )
		return false;

	// flush qip infium users status cache
	statusid = pMsg.WParam;
	pMsg.Msg = PM_PLUGIN_XSTATUS_UPD;
	pMsg.WParam = 1;
	pluginService->OnPluginMessage ( &pMsg );

	BSTR compatibleStrFormat = SysAllocString ( coreSettings.strStatus.c_str () ),
		 compatibleStrName = SysAllocString ( coreSettings.strStatusName.c_str () );

	pMsg.Msg = PM_PLUGIN_XSTATUS_UPD;
	pMsg.DllHandle = pluginInfo.DllHandle;
	pMsg.Result = 1;
	pMsg.WParam = statusid;
	pMsg.NParam = reinterpret_cast<int> ( compatibleStrFormat );
	if ( !coreSettings.changeOnlyXStatus )
		pMsg.LParam  = reinterpret_cast<int> ( compatibleStrName );

	pluginService->OnPluginMessage ( &pMsg );

	SysFreeString ( compatibleStrFormat );
	SysFreeString ( compatibleStrName );

	return true;
}

void NPCore::OnUpdate ()
{
	PluginMessage pMsg;
	pcrecpp::RE_Options options;
	INPPlugin* p = GetPluginManager ()->GetActivePlugin ();
	std::wstring status;

	if ( p )
	{
		CRITICAL_SECTION cs;
		InitializeCriticalSectionAndSpinCount(&cs, 5000);
		EnterCriticalSection(&cs);

		PluginDescription *d = p->GetPluginDescription ();
		if ( d->apisupport )
		{
			playerInfo.album = L"";
			playerInfo.artist = L"";
			playerInfo.filename = L"";
			playerInfo.genre = L"";
			playerInfo.length = L"";
			playerInfo.title = L"";
			playerInfo.number = L"";

			if ( !p->OnCheckStatus ( playerInfo ) )
				return;

			// format player data into string
			
			//options.set_utf8( true );
			//options.set_multiline ( true );
			
			std::string result = as_utf8 ( coreSettings.strFormat.c_str () );
			std::wstring finRslt = L"";
			pcrecpp::StringPiece pieces[7];
			static const char* patterns[7] = { "{%artist%}", 
										"{%title%}",
										"{%album%}",
										"{%genre%}",
										"{%file%}",
										"{%length%}",
										"{%number%}" };

			std::string utf8_artist = as_utf8 ( playerInfo.artist.c_str () );
			std::string utf8_title = as_utf8 ( playerInfo.title.c_str () ).c_str ();
			std::string utf8_album = as_utf8 ( playerInfo.album.c_str () ).c_str ();
			std::string utf8_genre = as_utf8 ( playerInfo.genre.c_str () ).c_str ();
			std::string utf8_filename = as_utf8 ( playerInfo.filename.c_str () ).c_str ();
			std::string utf8_length = as_utf8 ( playerInfo.length.c_str () ).c_str ();
			std::string utf8_number = as_utf8 ( playerInfo.number.c_str () ).c_str ();

			pieces[0].set ( utf8_artist.c_str () );
			pieces[1].set ( utf8_title.c_str () );
			pieces[2].set ( utf8_album.c_str () );
			pieces[3].set ( utf8_genre.c_str () );
			pieces[4].set ( utf8_filename.c_str () );
			pieces[5].set ( utf8_length.c_str () );
			pieces[6].set ( utf8_number.c_str () );
			
			for ( int i = 0; i < 7; i++ )
				pcrecpp::RE ( patterns[i], options ).Replace ( pieces[i], &result );

			status = as_utf16 ( result.c_str () );
		} 
		else 
		{
			if ( !p->OnCheckStatus ( status ) )
				return;

			std::string result = as_utf8 ( coreSettings.strAltFormat.c_str () );
			std::string utf8_status = as_utf8 ( status.c_str () );
			pcrecpp::RE ( "{%status%}", options ).Replace ( utf8_status, &result );
			status = as_utf16 ( result.c_str () );
		}

		pMsg.Msg = PM_PLUGIN_XSTATUS_GET;
		pMsg.DllHandle = pluginInfo.DllHandle;
		pMsg.Result = 0;
		pMsg.WParam = 0;
		pMsg.LParam = 0;
		pMsg.NParam = 0;
		pluginService->OnPluginMessage ( &pMsg );

		std::wstring userStatus = reinterpret_cast<wchar_t*> ( pMsg.NParam ),
					 userStatusName = reinterpret_cast<wchar_t*> ( pMsg.LParam );

		if ( ( userStatus != status || coreSettings.strStatus != status ) || 
			 ( userStatusName != coreSettings.strStatusName && !coreSettings.changeOnlyXStatus ) )
		{
			coreSettings.strStatus = status;
			UpdateStatus ();
		}
		LeaveCriticalSection(&cs);
		DeleteCriticalSection(&cs);
	}
}

HRESULT __stdcall NPCore::QueryInterface ( REFIID riid, void** ppvObject )
{
	return E_FAIL;
}

ULONG __stdcall NPCore::AddRef ()
{
	return ++refCounter;
}

ULONG __stdcall NPCore::Release ()
{
	return --refCounter;
}
