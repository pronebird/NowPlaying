// "Now Playing" QIP Infium Plugin
// TrayIcon UI - Quick Switcher
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once
#include "INPCore.h"
#include "NTray.h"

#define WM_TRAYNOTIFY WM_USER + 100
#define IDM_TRAYPOPUP 1024

class AppTrayIcon : public CWindowImpl<AppTrayIcon>
{
public:
	DECLARE_WND_CLASS ( L"WTLNTRAY" )

	AppTrayIcon ();
	virtual ~AppTrayIcon ();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_MSG_MAP(AppTrayIcon)
		MESSAGE_HANDLER ( WM_CREATE, OnCreate )
		MESSAGE_HANDLER ( WM_COMMAND, OnCommand )
		MESSAGE_HANDLER ( WM_TRAYNOTIFY, OnTrayNotification )
	END_MSG_MAP()

	LRESULT OnCommand ( UINT, WPARAM, LPARAM, BOOL& );
	HWND Create ( INPCore* );
	LRESULT OnCreate ( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnTrayNotification ( UINT, WPARAM, LPARAM, BOOL& );
	LRESULT OnHide ( WORD, WORD, HWND, BOOL& );
	LRESULT OnShow ( WORD, WORD, HWND, BOOL& );

	bool OnSync ();

protected:
	HICON m_hIcon;
	CTrayNotifyIcon g_TrayIcon;
	INPCore* npCore;
	CMenu menu, subMenu;
	std::vector<INPPlugin*> pList;

	unsigned int prevSelItem;
	bool bInit;
};