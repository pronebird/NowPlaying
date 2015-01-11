// "Now Playing" QIP Infium Plugin
// Settings UI component: General property page
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once
#include "INPCore.h"

class GeneralPropertyPage : public CPropertyPageImpl<GeneralPropertyPage>
{
public:
	enum { IDD = IDD_GENERAL };
	GeneralPropertyPage (  pugi::xml_document*, INPCore*, _U_STRINGorID title = (LPCTSTR) NULL );
	virtual ~GeneralPropertyPage ();
   
	BEGIN_MSG_MAP(PluginPropertyPage)
		MESSAGE_HANDLER( WM_INITDIALOG, OnInit )
		COMMAND_HANDLER( IDC_LIST1, LBN_SELCHANGE, OnPluginListEvent )
		COMMAND_HANDLER( IDC_COMBOBOXEX1, LBN_SELCHANGE, OnLanguageListEvent )
        CHAIN_MSG_MAP(CPropertyPageImpl<GeneralPropertyPage>)
    END_MSG_MAP()

    int OnSetActive();
    BOOL OnKillActive();
	BOOL OnApply ();

	LRESULT OnInit ( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnPluginListEvent ( int, int, HWND, BOOL& );
	LRESULT OnLanguageListEvent ( int, int, HWND, BOOL& );

private:
	CListViewCtrl m_playerList;
	CImageList imageList;
	CComboBoxEx m_languageList;
	CTrackBarCtrl m_updateTime;

	std::vector<INPPlugin*> pluginList;
	std::vector<NPLanguage> languageList;
	pugi::xml_document *doc;

	INPCore* npCore;
};