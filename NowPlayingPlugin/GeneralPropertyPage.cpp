// "Now Playing" QIP Infium Plugin
// Settings UI component: General property page
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "GeneralPropertyPage.h"

GeneralPropertyPage::GeneralPropertyPage (  pugi::xml_document* xmlDoc, INPCore* _npCore, _U_STRINGorID title ) :
	CPropertyPageImpl<GeneralPropertyPage> ( title )
{
	npCore = _npCore;
	pluginList = npCore->GetPluginManager ()->GetPluginList ();
	doc = xmlDoc;
	imageList.Create ( 32, 32, ILC_COLOR24 | ILC_MASK, pluginList.size (), 0 );
	m_psp.hInstance = npCore->GetLanguageManager ()->GetResourceInstance ();
}

GeneralPropertyPage::~GeneralPropertyPage ()
{
	imageList.RemoveAll ();
	imageList.Destroy ();
}

int GeneralPropertyPage::OnSetActive()
{
	return TRUE;
}
BOOL GeneralPropertyPage::OnApply ()
{
	int curLangSel = m_languageList.GetCurSel (),
		curLangId = 1033,
		curPlayerSel = m_playerList.GetSelectedIndex ();

	if ( curLangSel != CB_ERR )
	{
		if ( curLangSel < languageList.size () )
		{
			curLangId = languageList[curLangSel].id;
		}
	}

	xpath_node xNode = doc->select_single_node ( "//Settings/General" );

	if ( xNode )
	{
		xNode.node ().attribute ( "UILanguage" ) = curLangId;
		xNode.node ().attribute ( "UpdateInterval" ) = m_updateTime.GetPos ();
		
		if ( curPlayerSel != LB_ERR )
		{
			std::vector<INPPlugin*> pls = npCore->GetPluginManager()->GetPluginList ();
			if ( curPlayerSel >= 0 && curPlayerSel < pls.size () )
				xNode.node ().attribute ( "PL_GUID" ).set_value ( pugi::as_utf8(pls[curPlayerSel]->GetPluginDescription()->guid.c_str ()).c_str () );
		}
	}

	return TRUE;
}

BOOL GeneralPropertyPage::OnKillActive()
{
	return TRUE;
}

LRESULT GeneralPropertyPage::OnInit ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bool langFound = false;
	int itemCount = 0, currentLang = 1033, updateInterval = 10, playerId = 0;
	m_playerList = GetDlgItem ( IDC_LIST1 );
	m_languageList = GetDlgItem ( IDC_COMBOBOXEX1 );
	m_updateTime = GetDlgItem ( IDC_SLIDER1 );

	m_playerList.SetImageList ( imageList, LVSIL_NORMAL );
	m_updateTime.SetRange ( 5, 120 );

	INPLanguageManager* lm = npCore->GetLanguageManager ();
	languageList = lm->GetLanguageList ();

	xml_node Settings = doc->child ( "Settings" ), General;
	xml_attribute attrUILang, attrUpdateTime, attrSelectedPlayer;

	if ( Settings )
	{
		General = Settings.child ( "General" );
		if ( !General )
		{
			General = Settings.append_child ();
			General.set_name ( "General" );
		}

		attrUILang = General.attribute ( "UILanguage" );
		if ( !attrUILang )
		{
			attrUILang = General.append_attribute ( "UILanguage" ) = 1033;
		}

		attrUpdateTime = General.attribute ( "UpdateInterval" );
		if ( !attrUpdateTime )
		{
			attrUpdateTime = General.append_attribute ( "UpdateInterval" ) = 10;
		}

		updateInterval = attrUpdateTime.as_int ();
		updateInterval = updateInterval < 5 ? 5 : updateInterval > 120 ? 120 : updateInterval;
		m_updateTime.SetPos ( updateInterval );

		attrSelectedPlayer = General.attribute ( "PL_GUID" );
		std::vector<INPPlugin*> pls = npCore->GetPluginManager()->GetPluginList();
		unsigned int k = 0;
		playerId = 0;

		if ( !attrSelectedPlayer )
		{
			attrSelectedPlayer = General.append_attribute ( "PL_GUID" ) = "{}";
		} 
		else 
		{
			std::wstring guid = as_utf16(attrSelectedPlayer.value());
			for(std::vector<INPPlugin*>::iterator i = pls.begin(); i != pls.end(); i++)
			{
				if ( (*i)->GetPluginDescription()->guid == guid )
				{
					playerId = k;
					break;
				}
				k++;
			}
		}
		
		currentLang = attrUILang.as_int ();
	}

	for ( std::vector<NPLanguage>::const_iterator i = languageList.begin (); i != languageList.end (); i++ )
	{
		int itemId = m_languageList.AddItem ( CBEIF_TEXT, (*i).name.c_str (), -1, -1, 0, -1, NULL );
		if ( (*i).id == currentLang )
		{
			m_languageList.SetCurSel ( itemId );
			langFound = true;
		}
	}

	if ( !langFound && languageList.size () > 0 )
	{
		m_languageList.SetCurSel ( 0 );
	} else if ( languageList.size () < 1) { 
		m_languageList.EnableWindow ( false );
	}

	for ( std::vector<INPPlugin*>::const_iterator i = pluginList.begin (); i != pluginList.end (); i++ )
	{
		PluginDescription* pInfo = (*i)->GetPluginDescription ();
		int imageIndex = -1;

		if ( pInfo->image )
		{
			HBITMAP bmp = (HBITMAP)CopyImage ( pInfo->image, IMAGE_BITMAP, 32, 32, LR_COPYRETURNORG );
			imageIndex = imageList.Add ( bmp, pInfo->mask );
			DeleteObject ( bmp );
		}

		m_playerList.AddItem ( itemCount++, 0, pInfo->name.c_str (), imageIndex );
	}

	if ( m_playerList.GetItemCount () > 0 && playerId < m_playerList.GetItemCount ()  )
		m_playerList.SelectItem ( playerId );

	return TRUE;
}

LRESULT GeneralPropertyPage::OnPluginListEvent ( int x, int y, HWND hWnd, BOOL& bHandled )
{
	
	return TRUE;
}

LRESULT GeneralPropertyPage::OnLanguageListEvent ( int x, int y, HWND hWnd, BOOL& bHandled )
{
	std::wstring notifytext = npCore->GetLanguageManager ()->GetString ( IDS_STRING146, 100 );
	MessageBox ( notifytext.c_str (), L"Now Playing", MB_OK | MB_ICONINFORMATION );
	return TRUE;
}