// "Now Playing" QIP Infium Plugin
// Just precompiled headers
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once

#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

// #define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <vector>
#include <io.h>
#include <process.h>
#include <commctrl.h>
#include <wchar.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlsplit.h>
#include <atlmisc.h>
#include <htmlhelp.h>

#include "u_plugin_info.h"
#include "u_plugin_msg.h"
#include "INPPlugin.h"
#include "CommonResources.h"
#include "resource.h"

#include "pugixml/pugixml.hpp"
#include "pcre/pcrecpp.h"

using namespace pugi;
using namespace pcrecpp;

#pragma comment ( lib, "comctl32.lib" )

typedef HWND (_stdcall *HTMLHELP)(HWND hwndCaller,LPCSTR  pszFile,UINT uCommand,DWORD dwData);

struct HtmlHlp
{
   BOOL bHelpSystemInit;
   DWORD dwCookie;
   HMODULE hModule;
};

extern CAppModule _Module;
extern HtmlHlp HelpStruct;

/*if defined _M_IX86
pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
elif defined _M_IA64
pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
elif defined _M_X64
pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
else
pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
endif*/