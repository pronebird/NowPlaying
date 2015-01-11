// "Now Playing" QIP Infium Plugin
// Settings UI component: Plugins property page
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "Utility.h"
#include "PluginPropertyPage.h"


PluginPropertyPage::PluginPropertyPage ( pugi::xml_document* xmlDoc, INPCore* _npCore, _U_STRINGorID title ) :
	CPropertyPageImpl<PluginPropertyPage> ( title )
{
	npCore = _npCore;
	m_psp.hInstance = npCore->GetLanguageManager ()->GetResourceInstance ();
	pluginList = npCore->GetPluginManager()->GetPluginList ();
	doc = xmlDoc;
	smImageList.Create ( 16, 16, ILC_COLOR32, 1, 0 );
}

PluginPropertyPage::~PluginPropertyPage ()
{
	smImageList.RemoveAll ();
	smImageList.Destroy ();
}

BOOL PluginPropertyPage::OnApply ()
{
	xpath_node xNode = doc->select_single_node ( "//Settings/Plugin/UI/CSplitter" );
	if ( xNode )
		xNode.node ().attribute ( "Position" ) = m_splitter.m_xySplitterPos;

	return TRUE;
}

int PluginPropertyPage::OnSetActive()
{
	return TRUE;
}

BOOL PluginPropertyPage::OnKillActive()
{
	return TRUE;
}

LRESULT PluginPropertyPage::OnInit ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	RECT rectClient;
	xml_node Settings, Plugins, UI, UI_Splitter;
	xml_attribute pos;

	HICON hIconGenericDll = LoadIcon ( m_psp.hInstance, MAKEINTRESOURCE ( IDI_ICON1 ) );
	int iPos = 200, itemCount = 0,  pluginImageIndex = smImageList.AddIcon ( hIconGenericDll );
	DestroyIcon ( hIconGenericDll );

	m_pluginPropertyList = GetDlgItem ( IDC_LIST2 );
	m_pluginList = GetDlgItem ( IDC_LIST1 );
	
	m_pluginList.SetImageList ( smImageList, LVSIL_SMALL );

	std::wstring Strings[] = { 
		npCore->GetLanguageManager ()->GetString ( IDS_STRING124, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING125, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING126, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING149, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING127, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING128, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING129, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING130, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING131, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING132, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING133, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING134, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING135, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING136, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING137, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING138, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING139, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING140, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING144, 30 ),
		npCore->GetLanguageManager ()->GetString ( IDS_STRING145, 30 )
	};
	m_pluginList.AddColumn ( Strings[0].c_str (), itemCount );
	m_pluginList.SetColumnWidth ( itemCount, 120 );
	itemCount++;

	m_pluginList.AddColumn ( Strings[1].c_str (), itemCount );
	m_pluginList.SetColumnWidth ( itemCount, 100 );
	itemCount++;

	m_pluginList.AddColumn ( Strings[2].c_str (), itemCount );
	m_pluginList.SetColumnWidth ( itemCount, 80 );
	itemCount++;

	m_pluginList.AddColumn ( Strings[3].c_str (), itemCount );
	m_pluginList.SetColumnWidth ( itemCount, 250 );

	itemCount = 0;
	m_pluginPropertyList.AddColumn ( Strings[4].c_str (), itemCount );
	m_pluginPropertyList.SetColumnWidth ( itemCount, 100 );

	itemCount++;
	m_pluginPropertyList.AddColumn ( Strings[5].c_str (), itemCount );
	m_pluginPropertyList.SetColumnWidth ( itemCount, 150 );

	itemCount = 0;
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[6].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[7].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[8].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[9].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[10].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[11].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[12].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[13].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[14].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[15].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[16].c_str () );
	m_pluginPropertyList.AddItem ( itemCount++, 0, Strings[17].c_str () );

	m_pluginPropertyList.SetExtendedListViewStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_pluginList.SetExtendedListViewStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_ONECLICKACTIVATE );

	itemCount = 0;
	for ( std::vector<INPPlugin*>::iterator i = pluginList.begin (); i != pluginList.end (); i++ )
	{
		std::wstring moduleName = (*i)->GetPath ();
		PluginDescription* pInfo = (*i)->GetPluginDescription ();

		int npos = moduleName.rfind ( L"\\" );
		if ( npos >= 0 && npos+1 < moduleName.length () )
			moduleName = moduleName.substr ( npos+1, moduleName.length () );

		m_pluginList.AddItem ( itemCount, 0, moduleName.c_str (), pluginImageIndex );
		m_pluginList.SetItem ( itemCount, 1, LVIF_TEXT, pInfo->name.c_str (), -1, 0, 0, NULL );
		m_pluginList.SetItem ( itemCount, 2, LVIF_TEXT, pInfo->apisupport ? Strings[18].c_str () : Strings[19].c_str (), -1, 0, 0, NULL );
		m_pluginList.SetItem ( itemCount, 3, LVIF_TEXT, pInfo->guid.c_str (), -1, 0, 0, NULL );
		itemCount++;
	}

	GetClientRect( &rectClient );

	m_hWndClient = m_splitter.Create( m_hWnd, rectClient, NULL, WS_CHILD | WS_VISIBLE);
	m_splitter.m_bFullDrag = true;
	m_splitter.m_cxyMin = m_splitter.m_cxySplitBar;
	m_splitter.m_xySplitterPos = 200;
	m_splitter.m_cxyMin = 60;

	Settings = doc->child ( "Settings" );
	if ( Settings )
	{
		Plugins = Settings.child ( "Plugin" );
		if ( !Plugins )
		{
			Plugins = Settings.append_child ();
			Plugins.set_name ( "Plugin" );				
		}

		UI = Plugins.child ( "UI" );
		if ( !UI )
		{
			UI = Plugins.append_child ();
			UI.set_name ( "UI" );
		}

		UI_Splitter = UI.child ( "CSplitter" );
		if ( !UI_Splitter )
		{
			UI_Splitter = Plugins.append_child ();
			UI_Splitter.set_name ( "CSplitter" );
		}
				
		pos = UI_Splitter.attribute ( "Position" );
		if ( !pos )
			pos = UI_Splitter.append_attribute ( "Position" ) = "200";

		iPos = pos.as_int ();
		if ( iPos >= m_splitter.m_cxyMin && iPos < rectClient.bottom - rectClient.top )
			m_splitter.m_xySplitterPos = iPos;
	}

	m_splitter.SetSplitterPanes( m_pluginList, m_pluginPropertyList );

	return TRUE;
}

LRESULT PluginPropertyPage::OnShowWindow ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return TRUE;
}

LRESULT PluginPropertyPage::OnPluginListSelect ( int wNotifyCode, NMHDR* hdr, BOOL& bHandled )
{
	FILEVERSION_INFO fvi;
	NMITEMACTIVATE *item = (NMITEMACTIVATE*)  hdr;
	if ( item->iItem >= 0 && item->iItem < pluginList.size () )
	{
		INPPlugin* plugin = pluginList.at ( item->iItem );
		if ( plugin )
		{
			int itemCount = 0;
			GetFileVersion ( plugin->GetPath ().c_str (), &fvi );

			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.Comments.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.InternalName.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.ProductName.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.CompanyName.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.LegalCopyright.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.ProductVersion.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.FileDescription.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.LegalTrademarks.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.PrivateBuild.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.FileVersion.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.OriginalFilename.c_str (), -1, 0, 0, NULL );
			m_pluginPropertyList.SetItem ( itemCount++, 1, LVIF_TEXT, fvi.SpecialBuild.c_str (), -1, 0, 0, NULL );
		}
	}
	return FALSE;
}