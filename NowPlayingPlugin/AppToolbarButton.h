// "Now Playing" QIP Infium Plugin
// Toolbar button UI - Quick Switcher
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once

#include "INPCore.h"

#define IDM_POPUPMENU 1024

class AppToolbarButton
{
public:
	AppToolbarButton ( INPCore* );
	~AppToolbarButton ();

	AddBtnInfo* GetButton ();
	bool Create ();
	bool OnClick ();
	bool OnSync ();

private:
	INPCore* npCore;

	HICON appIcon;

	AddBtnInfo bInfo;

	bool bInit;
	unsigned int prevSelItem;

	CMenu menu, subMenu;
	std::vector<INPPlugin*> pList;
};
