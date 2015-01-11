// "Now Playing" QIP Infium Plugin
// Settings UI Property sheet
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "AppPropertySheet.h"

AppPropertySheet::AppPropertySheet ( INPCore* _npCore, _U_STRINGorID title, UINT uStartPage, 
                                       HWND hWndParent ) :
    CPropertySheetImpl<AppPropertySheet> ( title, uStartPage, hWndParent )
{
	npCore = _npCore;
	pluginList = npCore->GetPluginManager ()->GetPluginList ();
	settingsPath = npCore->GetPath ();
	bSaveOnQuit = false;

	settingsPath.append ( L"\\Settings.xml" );
	if ( !doc.load_file ( pugi::as_utf8 ( settingsPath.c_str () ).c_str () ) || !doc.child ( "Settings" ) )
	{
		if ( !DefaultXml () )
		{
			::MessageBox ( NULL, L"Error while xml generation. Now dialog will be closed.", L"", MB_OK | MB_ICONERROR );
			return;
		}
	}

	// Setting up language
	int currentLang = doc.child ( "Settings" ).child ( "General" ).attribute ( "UILanguage" ).as_int ();

	npCore->GetLanguageManager ()->SetLanguageById ( currentLang );
	wTitle = npCore->GetLanguageManager()->GetString(IDS_STRING151, 50);

	m_psh.dwFlags |= PSH_NOAPPLYNOW | PSH_USEICONID;
	m_psh.pszIcon = MAKEINTRESOURCE ( IDI_APPICON );
	m_psh.pszCaption = wTitle.c_str();
	m_psh.hInstance = _Module.GetResourceInstance ();

	std::wstring generalTitle = npCore->GetLanguageManager ()->GetString ( IDS_STRING141, 30 ),
				 extendedTitle = npCore->GetLanguageManager ()->GetString ( IDS_STRING142, 30 ),
				 pluginsTitle = npCore->GetLanguageManager ()->GetString ( IDS_STRING143, 30 );
	m_generalPage = new GeneralPropertyPage ( &doc, npCore, generalTitle.c_str () );
	m_extendedPage = new ExtendedPropertyPage ( &doc, npCore, extendedTitle.c_str () );
	m_pluginPage = new PluginPropertyPage ( &doc, npCore, pluginsTitle.c_str () );

	AddPage ( *m_generalPage );
	AddPage ( *m_extendedPage );
	AddPage ( *m_pluginPage );
}

AppPropertySheet::~AppPropertySheet ()
{
	if ( bSaveOnQuit )
		doc.save_file ( pugi::as_utf8 ( settingsPath.c_str () ).c_str () );

	delete m_generalPage;
	delete m_extendedPage;
	delete m_pluginPage;
}

LRESULT AppPropertySheet::OnShowWindow( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	HtmlHelpInit ( true );
	return true;
}

LRESULT AppPropertySheet::OnDestroy ( UINT, WPARAM, LPARAM, BOOL& )
{
	HtmlHelpInit ( false );
	return true;
}

LRESULT AppPropertySheet::OnOkClicked ( WORD, WORD, HWND, BOOL& bHandled )
{
	bSaveOnQuit = true;
	bHandled = false;
	return true;
}

bool AppPropertySheet::DefaultXml ()
{
	npCore->SetDefaultSettings ();
	return true;
}

bool AppPropertySheet::HtmlHelpInit ( bool bInit )
{
    HMODULE  hModule = NULL;
    HTMLHELP HtmlHelp = NULL;
    DWORD dwCookie;

   // if (bInit&&HtmlHlp.bHelpSystemInit) return true;
  //  if (!bInit&&!HtmlHlp.bHelpSystemInit) return true;

    if (bInit&&!HelpStruct.bHelpSystemInit)
    {
        hModule=LoadLibrary ( L"hhctrl.ocx" );
        if (!hModule) return FALSE;
        HtmlHelp = (HTMLHELP)GetProcAddress ( hModule, "HtmlHelpW" );
        if (!HtmlHelp) return FALSE;
        HtmlHelp(NULL,NULL,HH_INITIALIZE,(DWORD)&dwCookie);
        if (!dwCookie)
        {
            FreeLibrary(hModule);
            return FALSE;
        }
        HelpStruct.bHelpSystemInit=TRUE;
        HelpStruct.dwCookie=dwCookie;
        HelpStruct.hModule=hModule;
    }
    else if (!bInit&&HelpStruct.bHelpSystemInit)
    {
        hModule=HelpStruct.hModule;
        HtmlHelp = (HTMLHELP)GetProcAddress(hModule,"HtmlHelpW");
        if (!HtmlHelp) return FALSE;
        HtmlHelp ( NULL, NULL, HH_CLOSE_ALL, NULL );
        HtmlHelp ( NULL,NULL,HH_UNINITIALIZE,(DWORD)HelpStruct.dwCookie );
        FreeLibrary ( HelpStruct.hModule );
        ZeroMemory ( &HelpStruct, sizeof(HtmlHlp) );
    }

    return TRUE;
}