// "Now Playing" QIP Infium Plugin
// Settings UI component: Plugins property page
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once
#include "INPCore.h"

class PluginPropertyPage : public CPropertyPageImpl<PluginPropertyPage>
{
public:
	enum { IDD = IDD_PLUGINS };
	PluginPropertyPage ( pugi::xml_document*, INPCore*, _U_STRINGorID title = (LPCTSTR) NULL );
	virtual ~PluginPropertyPage ();
   
	BEGIN_MSG_MAP(PluginPropertyPage)
		MESSAGE_HANDLER( WM_INITDIALOG, OnInit )
		MESSAGE_HANDLER( WM_SHOWWINDOW, OnShowWindow )
		NOTIFY_HANDLER ( IDC_LIST1, LVN_ITEMACTIVATE, OnPluginListSelect )
        CHAIN_MSG_MAP(CPropertyPageImpl<PluginPropertyPage>)
    END_MSG_MAP()
	
    int OnSetActive();
    BOOL OnKillActive();
	BOOL OnApply ();

	LRESULT OnInit ( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnShowWindow ( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnPluginListSelect ( int, NMHDR*, BOOL& );

private:
	CListViewCtrl m_pluginList, m_pluginPropertyList;
	CHorSplitterWindow m_splitter;
	CImageList smImageList;

	INPCore* npCore;

	std::vector<INPPlugin*> pluginList;
	pugi::xml_document *doc;
	HWND m_hWndClient;
};