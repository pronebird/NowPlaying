// "Now Playing" QIP Infium Plugin
// Settings UI component: Extended property page
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once
#include "INPCore.h"

class ExtendedPropertyPage : public CPropertyPageImpl<ExtendedPropertyPage>
{
public:
	enum { IDD = IDD_EXTENDED };
	ExtendedPropertyPage ( pugi::xml_document*, INPCore*, _U_STRINGorID title = (LPCTSTR) NULL );
	virtual ~ExtendedPropertyPage ();
   
	BEGIN_MSG_MAP(PluginPropertyPage)
		MESSAGE_HANDLER( WM_INITDIALOG, OnInit )
		COMMAND_HANDLER ( IDC_CHECK1, BN_CLICKED, OnQuickSwitchClick )
		COMMAND_HANDLER ( IDC_CHECK3, BN_CLICKED, OnChangeOnlyWhenIconClick )
		MESSAGE_HANDLER ( WM_HELP, OnContextHelp )
        CHAIN_MSG_MAP(CPropertyPageImpl<ExtendedPropertyPage>)
    END_MSG_MAP()

	BOOL OnApply ();
    int OnSetActive();
    BOOL OnKillActive();

	LRESULT OnInit ( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnQuickSwitchClick ( WORD, WORD, HWND, BOOL& );
	LRESULT OnChangeOnlyWhenIconClick ( WORD, WORD, HWND, BOOL& );

	LRESULT OnContextHelp ( UINT, WPARAM, LPARAM, BOOL& );

private:
	pugi::xml_document *doc;

	CEdit m_nameEdit, m_xdescrEdit, m_xaltDescrEdit;
	CButton m_quickSwitchCheck, 
			m_quickSwitchCheckSysTray, 
			m_quickSwitchCheckBelowUserPic, 
			m_ChangeOnlyXStatusCheck, 
			m_ChangeOnlyWhenIconCheck;
	CComboBoxEx m_iconSelect;
	INPCore* npCore;
	CImageList statusImgList;
};