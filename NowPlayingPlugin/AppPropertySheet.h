// "Now Playing" QIP Infium Plugin
// Settings UI Property sheet
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once
#include "GeneralPropertyPage.h"
#include "ExtendedPropertyPage.h"
#include "PluginPropertyPage.h"

class AppPropertySheet : public CPropertySheetImpl<AppPropertySheet>
{
public:
    AppPropertySheet ( INPCore*, _U_STRINGorID title = (LPCTSTR) NULL, 
                        UINT uStartPage = 0, HWND hWndParent = NULL );
	virtual ~AppPropertySheet ();

    BEGIN_MSG_MAP ( AppPropertySheet )
		COMMAND_HANDLER ( IDOK, BN_CLICKED, OnOkClicked )
		MESSAGE_HANDLER ( WM_DESTROY, OnDestroy )
		MESSAGE_HANDLER( WM_SHOWWINDOW, OnShowWindow )
		CHAIN_MSG_MAP ( CPropertySheetImpl<AppPropertySheet> )
    END_MSG_MAP ()

	LRESULT OnShowWindow ( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnDestroy ( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnOkClicked ( WORD, WORD, HWND, BOOL& );
private:
	bool DefaultXml ();
	bool HtmlHelpInit ( bool bInit );

	bool bSaveOnQuit;
	std::wstring settingsPath;
	std::wstring wTitle;

	GeneralPropertyPage *m_generalPage;
	PluginPropertyPage *m_pluginPage;
	ExtendedPropertyPage* m_extendedPage;

	std::vector<INPPlugin*> pluginList;
	pugi::xml_document doc;
	INPCore* npCore;
};