// QIP Infium SDK
// VERSION: 1.3.0
// Copyright(c) Ilham Z.
// ilham@qip.ru
// http://www.qip.im
// ------------------------------------------
// C++ port for 1.3.0 SDK by Mihaylov Andrey
//                        aka AbsoluteZero
// UPD 05 March 2008: fixed a few of string incompatibilities
// based on previous port by Alexander 
//                                 Yastrebov
//                  yastrebov.alex@gmail.com
// ------------------------------------------

#pragma once

#include <windows.h>


struct PluginMessage
{
	DWORD	Msg;
    int		WParam;
    int		LParam;
    int		NParam;
    int		Result;
    HMODULE	DllHandle; // Plugin dll handle, this value have to be always actual if plugin sending msg to qip core!
};