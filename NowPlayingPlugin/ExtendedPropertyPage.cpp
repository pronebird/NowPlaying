// "Now Playing" QIP Infium Plugin
// Settings UI component: Extended property page
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "Utility.h"
#include "ExtendedPropertyPage.h"

ExtendedPropertyPage::ExtendedPropertyPage ( pugi::xml_document *xmlDoc, INPCore* _npCore, _U_STRINGorID title ) :
	CPropertyPageImpl<ExtendedPropertyPage> ( title )
{
	doc = xmlDoc;
	npCore = _npCore;
	m_psp.hInstance = npCore->GetLanguageManager ()->GetResourceInstance ();

	//BOOL t = statusImgList.CreateFromImage ( IDB_BITMAP1, 16, 16, 35, RGB ( 255, 0, 255 ), IMAGE_BITMAP );
	COLORREF clr = RGB(255,0,255);
	statusImgList = ImageList_LoadImage ( m_psp.hInstance, MAKEINTRESOURCE ( IDB_BITMAP1 ), 16, 35, clr, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_COLOR );
}

ExtendedPropertyPage::~ExtendedPropertyPage ()
{
	if ( statusImgList )
	{
		statusImgList.RemoveAll ();
		statusImgList.Destroy ();
	}
}

int ExtendedPropertyPage::OnSetActive()
{
	return TRUE;
}

BOOL ExtendedPropertyPage::OnApply ()
{
	xpath_node xNode = doc->select_single_node ( "//Settings/Extended/Status" ),
			   yNode = doc->select_single_node ( "//Settings/Extended/QuickSwitch" );
	if ( xNode.node () )
	{
		int curStatusIconId = m_iconSelect.GetCurSel ();
		wchar_t statusName[20], xdesrText[512], xaltDescrText[512];
		m_nameEdit.GetWindowText ( statusName, 20 );
		m_xdescrEdit.GetWindowText ( xdesrText, 512 );
		m_xaltDescrEdit.GetWindowText ( xaltDescrText, 512 );

		xNode.node ().attribute ( "StatusName" ) = as_utf8 ( statusName ).c_str ();
		xNode.node ().attribute ( "XStatusText" ) = as_utf8 ( xdesrText ).c_str ();
		xNode.node ().attribute ( "XStatusAltText" ) = as_utf8 ( xaltDescrText ).c_str ();
		xNode.node ().attribute ( "ChangeOnlyXStatus" ) = m_ChangeOnlyXStatusCheck.GetCheck () == BST_CHECKED;
		xNode.node ().attribute ( "ChangeOnlyWhenIcon" ) = m_ChangeOnlyWhenIconCheck.GetCheck () == BST_CHECKED;
		if ( curStatusIconId >= 0 )
			xNode.node ().attribute ( "StatusIconId" ) = curStatusIconId;
	}

	if ( yNode.node () )
	{
		bool what = m_quickSwitchCheck.GetCheck () == BST_CHECKED;
		yNode.node ().attribute ( "Enable" ) = what;
		yNode.node ().attribute ( "SysTray" ) = m_quickSwitchCheckSysTray.GetCheck () == BST_CHECKED;
		yNode.node ().attribute ( "BelowUserPicture" ) = m_quickSwitchCheckBelowUserPic.GetCheck () == BST_CHECKED;
	}

	return TRUE;
}

BOOL ExtendedPropertyPage::OnKillActive()
{
	return TRUE;
}

LRESULT ExtendedPropertyPage::OnInit ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	xml_node Settings, Extended, Status, QuickSwitch;
	xml_attribute StatusName, 
				  XStatusText, 
				  XAltStatusText, 
				  ChangeOnlyXStatus, 
				  ChangeOnlyWhenIcon, 
				  StatusIconId, 
				  QS_Enable, 
				  QS_SysTray, 
				  QS_BelowUserPic;

	INPLanguageManager* lm = npCore->GetLanguageManager ();

	m_nameEdit = GetDlgItem ( IDC_EDIT1 );
	m_xdescrEdit = GetDlgItem ( IDC_EDIT2 );
	m_xaltDescrEdit = GetDlgItem ( IDC_EDIT3 );
	m_quickSwitchCheck = GetDlgItem ( IDC_CHECK1 );
	m_quickSwitchCheckSysTray = GetDlgItem ( IDC_CHECK4 );
	m_quickSwitchCheckBelowUserPic = GetDlgItem ( IDC_CHECK5 );
	m_ChangeOnlyXStatusCheck = GetDlgItem ( IDC_CHECK2 ); 
	m_ChangeOnlyWhenIconCheck = GetDlgItem ( IDC_CHECK3 ); 
	m_iconSelect = GetDlgItem ( IDC_COMBOBOXEX1 ); 

	Settings = doc->child ( "Settings" );
	if ( Settings )
	{
		Extended = Settings.child ( "Extended" );
		if ( !Extended )
		{
			Extended = Settings.append_child ();
			Extended.set_name ( "Extended" );				
		}

		Status = Extended.child ( "Status" );
		if ( !Status )
		{
			Status = Extended.append_child ();
			Status.set_name ( "Status" );				
		}

		StatusName = Status.attribute ( "StatusName" );
		if ( !StatusName )
			StatusName = Status.append_attribute ( "StatusName" ) = "Music";

		XStatusText = Status.attribute ( "XStatusText" );
		if ( !XStatusText )
			XStatusText = Status.append_attribute ( "XStatusText" ) = "Now playing: {%artist%} - {%tracktitle%}";

		XAltStatusText = Status.attribute ( "XStatusAltText" );
		if ( !XAltStatusText )
			XAltStatusText = Status.append_attribute ( "XStatusAltText" ) = "Now playing: {%status%}";

		ChangeOnlyXStatus = Status.attribute ( "ChangeOnlyXStatus" );
		
		if ( !ChangeOnlyXStatus )
		 {
			ChangeOnlyXStatus = Status.append_attribute ( "ChangeOnlyXStatus" ) = true;
		 }

		ChangeOnlyWhenIcon = Status.attribute ( "ChangeOnlyWhenIcon" );
		if ( !ChangeOnlyWhenIcon )
		 {
			ChangeOnlyWhenIcon = Status.append_attribute ( "ChangeOnlyWhenIcon" ) = true;
		 }

		StatusIconId = Status.attribute ( "StatusIconId" ); 
		if ( !StatusIconId )
		{
			StatusIconId = Status.append_attribute ( "StatusIconId" ) = 10;
		}

		QuickSwitch = Extended.child ( "QuickSwitch" );
		if ( !QuickSwitch )
		{
			QuickSwitch = Extended.append_child ();
			QuickSwitch.set_name ( "QuickSwitch" );
		}
		
		QS_Enable = QuickSwitch.attribute ( "Enable" );
		if ( !QS_Enable )
			QS_Enable = QuickSwitch.append_attribute ( "Enable" ) = false;

		QS_SysTray = QuickSwitch.attribute ( "SysTray" );
		if ( !QS_SysTray )
			QS_SysTray = QuickSwitch.append_attribute ( "SysTray" ) = false;

		QS_BelowUserPic = QuickSwitch.attribute ( "BelowUserPicture" );
		if ( !QS_BelowUserPic )
			QS_BelowUserPic = QuickSwitch.append_attribute ( "BelowUserPicture" ) = false;

		m_nameEdit.SetWindowText ( as_utf16 ( StatusName.value () ).substr(0, 20).c_str () );
		m_xdescrEdit.SetWindowText ( as_utf16 ( XStatusText.value () ).substr(0, 512).c_str () );
		m_xaltDescrEdit.SetWindowText ( as_utf16 ( XAltStatusText.value () ).substr(0, 512).c_str () );

		if ( QS_Enable.as_bool () )
		{
			m_quickSwitchCheck.SetCheck ( BST_CHECKED );
			m_quickSwitchCheckSysTray.EnableWindow ( true );
			m_quickSwitchCheckBelowUserPic.EnableWindow ( true );
		} else {
			m_quickSwitchCheck.SetCheck ( BST_UNCHECKED );
			m_quickSwitchCheckSysTray.EnableWindow ( false );
			m_quickSwitchCheckBelowUserPic.EnableWindow ( false );
		}

		if ( ChangeOnlyXStatus.as_bool () )
		{
			m_ChangeOnlyXStatusCheck.SetCheck ( BST_CHECKED );
		}

		if ( ChangeOnlyWhenIcon.as_bool () )
		{
			m_ChangeOnlyWhenIconCheck.SetCheck ( BST_CHECKED );
			m_iconSelect.EnableWindow ();
		} else
		{
			m_iconSelect.EnableWindow ( false );
		}

		m_iconSelect.SetImageList ( statusImgList );
		for ( int i = 0; i < statusImgList.GetImageCount (); i++ )
		{
			m_iconSelect.AddItem ( CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_OVERLAY, NULL, i, i, 0, i, NULL );
		}

		int iStatusIconId = StatusIconId.as_int (),
			iLastIconId = statusImgList.GetImageCount ()-1;

		m_iconSelect.SetCurSel ( iStatusIconId < 0 ? 0 : iStatusIconId > iLastIconId ? iLastIconId : iStatusIconId );
		
		m_quickSwitchCheckSysTray.SetCheck ( QS_SysTray.as_bool () ? BST_CHECKED : BST_UNCHECKED );
		m_quickSwitchCheckBelowUserPic.SetCheck ( QS_BelowUserPic.as_bool () ? BST_CHECKED : BST_UNCHECKED );

	}

	return TRUE;
}

LRESULT ExtendedPropertyPage::OnQuickSwitchClick ( WORD code, WORD id, HWND hWnd, BOOL& bHandled )
{
	m_quickSwitchCheckSysTray.EnableWindow ( m_quickSwitchCheck.GetCheck () == BST_CHECKED );
	m_quickSwitchCheckBelowUserPic.EnableWindow ( m_quickSwitchCheck.GetCheck () == BST_CHECKED );
	return TRUE;
}

LRESULT ExtendedPropertyPage::OnChangeOnlyWhenIconClick ( WORD, WORD, HWND, BOOL& )
{
	m_iconSelect.EnableWindow ( m_ChangeOnlyWhenIconCheck.GetCheck () == BST_CHECKED );
	return TRUE;
}

LRESULT ExtendedPropertyPage::OnContextHelp ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	HELPINFO *lphi = (HELPINFO*) lParam;

	if ( lphi->iContextType != HELPINFO_WINDOW ) 
		return TRUE;

	std::wstring strHelp;
   
   switch ( lphi->iCtrlId )
   {
     case IDC_EDIT2:
		 strHelp = npCore->GetLanguageManager ()->GetString ( IDS_STRING147, 255 );
		 ShowHtmlHelpText( strHelp.c_str (), m_xdescrEdit.m_hWnd, lphi );
        break;   
     case IDC_EDIT3:
		 strHelp = npCore->GetLanguageManager ()->GetString ( IDS_STRING148, 255 );
		 ShowHtmlHelpText( strHelp.c_str (), m_xaltDescrEdit.m_hWnd, lphi );
        break;  
    }
	return TRUE;
}