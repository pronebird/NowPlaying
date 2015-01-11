// "Now Playing" QIP Infium Plugin
// class NowPlayingPlugin
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once

#include "INPCore.h"
#include "AppTrayIcon.h"
#include "AppToolbarButton.h"

const int PLUGIN_VER_MAJOR = 1;
const int PLUGIN_VER_MINOR = 2;
const PWCHAR PLUGIN_NAME   = L"QIP Now Playing Status Plugin";
const PWCHAR PLUGIN_AUTHOR = L"Mihaylov Andrey aka AbsoluteZero";

struct NPSettings
{
	std::wstring strFormat;
	std::wstring strAltFormat;
	std::wstring strStatusName;
	std::wstring strStatus; // formatted status
	bool enableQuickSwitchTray;
	bool enableQuickSwitchToolBtn;
	bool changeOnlyXStatus;
	bool changeOnlyWhenIcon;
	int changeOnlyWhenIconId;
	unsigned long updateInterval;
};

class NPCore : public IQIPPlugin, public INPCore
{
public:
	NPCore ( IQIPPluginService* );
	virtual ~NPCore ();

	HRESULT __stdcall QueryInterface ( REFIID, void** );
    ULONG __stdcall AddRef ();
    ULONG __stdcall Release ();

	PluginInfo* __stdcall GetPluginInfo ();
    void __stdcall OnQipMessage ( PluginMessage* );

	void OnRun ();
	void OnQuit ();
	void OnEnable ();
	void OnDisable ();
	void OnOptions ();
	void OnUpdate ();
	void OnLoadSuccess ();
	unsigned long GetUpdateInterval ();

	std::wstring GetPath ();
	INPPluginManager* GetPluginManager ();
	INPLanguageManager* GetLanguageManager ();

	bool SetDefaultSettings ();

private:
	static PluginInfo pluginInfo;
	IQIPPluginService* pluginService;
	INPPluginManager* pluginManager;
	INPLanguageManager* languageManager;

	HANDLE hThread;
	DWORD dwThreadId;

	PlayerData playerInfo;

	NPSettings coreSettings;

	unsigned long refCounter;

	bool LoadCoreSettings ();
	bool UpdateStatus ();

	bool CreateTraySwitcher ();
	bool DestroyTraySwitcher ();

	bool CreateToolbarSwitcher ();
	bool DestroyToolbarSwitcher ();

	AppTrayIcon *quickSwitchTray;
	AppToolbarButton *quickSwitchButton;
};