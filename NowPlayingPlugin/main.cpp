// "Now Playing" QIP Infium Plugin
// Entry Point
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "NPCore.h"

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

void __stdcall CreateInfiumPLUGIN ( IQIPPlugin** retval, IQIPPluginService* pluginService )
{
	*retval = new NPCore ( pluginService );
}