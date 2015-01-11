// "Now Playing" QIP Infium Plugin
// TrayIcon UI - Quick Switcher
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "AppTrayIcon.h"

AppTrayIcon::AppTrayIcon ()
{
	prevSelItem = 0;
	bInit = false;
}

AppTrayIcon::~AppTrayIcon ()
{
	if ( bInit )
		g_TrayIcon.RemoveIcon ();

	if ( m_hWnd )
		DestroyWindow ();
}

BOOL AppTrayIcon::PreTranslateMessage ( MSG* pMsg )
{
	return FALSE;
}

BOOL AppTrayIcon::OnIdle()
{
	return FALSE;
}

bool AppTrayIcon::OnSync ()
{
	xml_document xml;
	std::wstring path = npCore->GetPath ();
	path.append ( L"\\Settings.xml" );

	if ( xml.load_file ( as_utf8 ( path.c_str () ).c_str () ) )
	{
		xpath_node generalSettings = xml.select_single_node ( "//Settings/General" );
		if ( generalSettings.node () )
		{
			xml_attribute attrSelPlayer = generalSettings.node ().attribute ( "PL_GUID" );
			if ( attrSelPlayer )
			{
				std::vector<INPPlugin*> pls = npCore->GetPluginManager()->GetPluginList ();
				std::wstring guid = as_utf16(attrSelPlayer.value());
				unsigned int k = 0;
				
				subMenu.CheckMenuItem ( prevSelItem, MF_BYCOMMAND | MF_UNCHECKED );

				for(std::vector<INPPlugin*>::iterator i = pls.begin(); i != pls.end (); i++)
				{
					if(guid == (*i)->GetPluginDescription()->guid)
					{
						prevSelItem = k + IDM_TRAYPOPUP + 1;
						break;
					}
					k++;
				}

				subMenu.CheckMenuItem ( prevSelItem , MF_BYCOMMAND | MF_CHECKED );
				subMenu.SetMenuDefaultItem ( prevSelItem ); 

				return true;
			}
		}
	}
	return false;
}

LRESULT AppTrayIcon::OnCommand ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int wmId = LOWORD ( wParam ), 
		wmNotifyCode = HIWORD ( wParam );

	if ( wmId > IDM_TRAYPOPUP )
	{
		if ( prevSelItem > IDM_TRAYPOPUP )
		{
			subMenu.CheckMenuItem ( prevSelItem, MF_BYCOMMAND | MF_UNCHECKED );

			xml_document xml;
			std::wstring path = npCore->GetPath ();
			path.append ( L"\\Settings.xml" );
			if ( xml.load_file ( as_utf8 ( path.c_str () ).c_str () ) )
			{
				xpath_node generalSettings = xml.select_single_node ( "//Settings/General" );
				if ( generalSettings.node () )
				{
					xml_attribute attrSelPlayer = generalSettings.node ().attribute ( "PL_GUID" );
					if ( attrSelPlayer )
					{
						std::vector<INPPlugin*> pls = npCore->GetPluginManager()->GetPluginList ();
						int activePlayer = wmId - IDM_TRAYPOPUP - 1;
						if ( activePlayer >= 0 && activePlayer < pls.size() )
							attrSelPlayer.set_value( as_utf8(pls[activePlayer]->GetPluginDescription()->guid.c_str()).c_str() );
						xml.save_file ( as_utf8 ( path.c_str () ).c_str () );
					}
				}
			}
		}

		subMenu.CheckMenuItem ( wmId, MF_BYCOMMAND | MF_CHECKED );
		subMenu.SetMenuDefaultItem ( wmId ); 
		prevSelItem = wmId;

		int curPlayerSel = wmId - IDM_TRAYPOPUP - 1;
		std::vector<INPPlugin*> pls = npCore->GetPluginManager()->GetPluginList ();
			if ( curPlayerSel >= 0 && curPlayerSel < pls.size () )
				npCore->GetPluginManager()->SetActivePlugin ( pls[curPlayerSel]->GetPluginDescription()->guid );
	}

	return TRUE;
}

HWND AppTrayIcon::Create ( INPCore* _npCore )
{
	unsigned int m_id = 0;
	bInit = true;

	npCore = _npCore;
	pList = npCore->GetPluginManager()->GetPluginList ();
	INPPlugin* pActive = npCore->GetPluginManager()->GetActivePlugin ();

	m_hIcon = CTrayNotifyIcon::LoadIconResource ( IDI_APPICON );

	menu.CreateMenu ();
	subMenu.CreatePopupMenu ();

	for ( unsigned int i = 0; i < pList.size (); i++ )
	{
		m_id = IDM_TRAYPOPUP + i + 1;
		PluginDescription *d = pList[i]->GetPluginDescription ();
		subMenu.AppendMenu ( MF_STRING, m_id, d->name.c_str () );
		if ( pActive == pList[i] )
		{
			subMenu.CheckMenuItem ( m_id, MF_BYCOMMAND | MF_CHECKED );
			prevSelItem = m_id;
		}
	}

	menu.AppendMenu ( MF_POPUP, subMenu, (HBITMAP) NULL );

	RECT rc;
	memset(&rc,0,sizeof(RECT));
	return CWindowImpl<AppTrayIcon>::Create(NULL,rc,_T("WTLNTRAY"),WS_POPUPWINDOW,WS_EX_TOOLWINDOW);
}

LRESULT AppTrayIcon::OnCreate ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if ( !g_TrayIcon.Create ( this, IDM_TRAYPOPUP, L"NP Quick Switcher", m_hIcon, WM_TRAYNOTIFY, menu ) )
	{
		MessageBox ( L"Failed to create tray icon", L"Error", MB_OK | MB_ICONSTOP );
		return -1;
	}

	subMenu.SetMenuDefaultItem ( prevSelItem );

	return 0;
}

LRESULT AppTrayIcon::OnTrayNotification ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = TRUE;
	g_TrayIcon.OnTrayNotification(wParam, lParam);
	return 0;
}

LRESULT AppTrayIcon::OnHide(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	g_TrayIcon.HideIcon ();
	return 0;
}
LRESULT AppTrayIcon::OnShow(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	g_TrayIcon.ShowIcon ();
	return 0;
}