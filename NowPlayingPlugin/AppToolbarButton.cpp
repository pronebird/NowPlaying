// "Now Playing" QIP Infium Plugin
// Toolbar button UI - Quick Switcher
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "AppToolbarButton.h"

AppToolbarButton::AppToolbarButton( INPCore* _npCore )
{
	prevSelItem = 0;
	bInit = false;
	npCore = _npCore;
	bInfo.BtnPNG = NULL;
	bInfo.BtnHint = SysAllocString(L"");
	bInfo.BtnIcon = NULL;
}

AppToolbarButton::~AppToolbarButton()
{
	if ( bInit )
	{
		if ( appIcon )
			DestroyIcon ( appIcon );

		SysFreeString ( bInfo.BtnHint );
	}
}

AddBtnInfo* AppToolbarButton::GetButton ()
{
	std::wstring str = npCore->GetLanguageManager ()->GetString ( IDS_STRING150, 30 ).c_str();
	SysReAllocString ( &bInfo.BtnHint, str.c_str () );
	return &bInfo;
}

bool AppToolbarButton::Create ()
{
	unsigned int m_id = 0;
	bInit = true;

	pList = npCore->GetPluginManager()->GetPluginList ();
	INPPlugin* pActive = npCore->GetPluginManager()->GetActivePlugin ();

	menu.CreateMenu ();
	subMenu.CreatePopupMenu ();

	for ( unsigned int i = 0; i < pList.size (); i++ )
	{
		m_id = IDM_POPUPMENU + i + 1;
		PluginDescription *d = pList[i]->GetPluginDescription ();
		subMenu.AppendMenu ( MF_STRING, m_id, d->name.c_str () );
		if ( pActive == pList[i] )
		{
			subMenu.CheckMenuItem ( m_id, MF_BYCOMMAND | MF_CHECKED );
			prevSelItem = m_id;
		}
	}

	menu.AppendMenu ( MF_POPUP, subMenu, (HBITMAP) NULL );

	appIcon = static_cast<HICON> ( LoadImage ( _Module.GetResourceInstance (), MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR ) );

	bInfo.BtnPNG = NULL;
	SysReAllocString ( &bInfo.BtnHint, npCore->GetLanguageManager ()->GetString ( IDS_STRING150, 30 ).c_str() );
	bInfo.BtnIcon = appIcon;

	subMenu.SetMenuDefaultItem ( prevSelItem );

	return true;
}

bool AppToolbarButton::OnClick ()
{
	CPoint ptCursor;
    GetCursorPos(&ptCursor);
	HWND hWnd = GetActiveWindow ();
    int newActivePlayer = ::TrackPopupMenuEx ( subMenu, TPM_LEFTBUTTON | TPM_RETURNCMD, ptCursor.x, ptCursor.y, hWnd, NULL );
	
	if ( newActivePlayer > IDM_POPUPMENU )
	{
		if ( prevSelItem > IDM_POPUPMENU )
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
						int curPlayerSel = newActivePlayer - IDM_POPUPMENU - 1;
						std::vector<INPPlugin*> pls = npCore->GetPluginManager()->GetPluginList ();
						if ( curPlayerSel >= 0 && curPlayerSel < pls.size () )
						{
							npCore->GetPluginManager()->SetActivePlugin ( pls[curPlayerSel]->GetPluginDescription()->guid );
							attrSelPlayer.set_value ( as_utf8(pls[curPlayerSel]->GetPluginDescription()->guid.c_str ()).c_str() );
						}
						xml.save_file ( as_utf8 ( path.c_str () ).c_str () );
					}
				}
			}
		}

		subMenu.CheckMenuItem ( newActivePlayer, MF_BYCOMMAND | MF_CHECKED );
		subMenu.SetMenuDefaultItem ( newActivePlayer ); 
		prevSelItem = newActivePlayer;
	}
    ::PostMessage ( hWnd, WM_NULL, 0, 0 );

	return true;
}

bool AppToolbarButton::OnSync ()
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
						prevSelItem = k + IDM_POPUPMENU + 1;
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