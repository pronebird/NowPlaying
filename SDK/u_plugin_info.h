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
#include <unknwn.h>

#include "u_plugin_msg.h"

const int QIP_SDK_VER_MAJOR = 1;
const int QIP_SDK_VER_MINOR = 3;

typedef unsigned char	Boolean;

// Plugin info
struct PluginInfo
{
	HMODULE				DllHandle;			//dll instance/handle will be updated by QIP after successful loading of plugin library
    BSTR				DllPath;			//this should be updated by plugin library after receiving PM_PLUGIN_LOAD_SUCCESS from QIP
    WORD				QipSdkVerMajor;		//major version of sdk for core compatibility check
    WORD				QipSdkVerMinor;		//minor version of sdk for core compatibility check
    WORD				PlugVerMajor;
    WORD				PlugVerMinor;
    BSTR				PluginName;
    BSTR				PluginAuthor;
};

// Plugin specific options which will be saved in profile
struct PluginSpecific 
{
    Boolean		Bool1;
    Boolean		Bool2;
    Boolean		Bool3;
    Boolean		Bool4;
    Boolean		Bool5;
    Boolean		Bool6;
    Boolean		Bool7;
    Boolean		Bool8;
    Boolean		Bool9;
    Boolean		Bool10;

    DWORD		Param1;
    DWORD		Param2;
    DWORD		Param3;
    DWORD		Param4;
    DWORD		Param5;
    DWORD		Param6;
    DWORD		Param7;
    DWORD		Param8;
    DWORD		Param9;
    DWORD		Param10;

    BSTR		Wstr1;
    BSTR		Wstr2;
    BSTR		Wstr3;
    BSTR		Wstr4;
    BSTR		Wstr5;
    BSTR		Wstr6;
    BSTR		Wstr7;
    BSTR		Wstr8;
    BSTR		Wstr9;
    BSTR		Wstr10;
};

// Adding button below avatar in msg window
struct AddBtnInfo
{
    HICON BtnIcon;      //size must be 16x16, if you creating new HICON then after adding button dont forget to destroy it in your plugin.
    long BtnPNG;        //size must be 16x16, LongInt(TPngObject) from PngImage library, delphi only. Else have to be 0. After adding button dont forget to destroy it in your plugin.
    BSTR BtnHint;       //hint of the button
};

// QIP2Plugin instant message record
struct QipMsgPlugin
{
	BYTE		MsgType;	// see below, MSG_TYPE_....
	DWORD		MsgTime;	// unix datetime, local time
	BSTR		ProtoName;
	BSTR		SenderAcc;
	BSTR		SenderNick;
	BSTR		RcvrAcc;
	BSTR		RcvrNick;
	BSTR		MsgText;
	Boolean		Blocked;	// received msg blocked by antispam
};

// QIP gives to plugin this interface
class IQIPPluginService
	: public IUnknown
{
public:
    virtual PluginSpecific*	__stdcall PluginOptions(HMODULE dllHandle) = 0;
    virtual void			__stdcall OnPluginMessage(PluginMessage* plugMsg) = 0;
};

// Plugin gives to QIP this interface
class IQIPPlugin
	: public IUnknown
{
    virtual PluginInfo* __stdcall GetPluginInfo() = 0;
    virtual void __stdcall OnQipMessage(PluginMessage* plugMsg) = 0;
};

enum
{
	// Messages qip <-> plugin
	// All messages "plugin -> qip" have to be with actual PluginMsg.DllHandle parameter}
	// === Plugin main messages =======
	// since 1.0.1
	PM_PLUGIN_LOAD_SUCCESS    =  1, //qip -> plugin
	PM_PLUGIN_RUN             =  2, //qip -> plugin
	PM_PLUGIN_QUIT            =  3, //qip -> plugin
	PM_PLUGIN_ENABLE          =  4, //qip -> plugin
	PM_PLUGIN_DISABLE         =  5, //qip -> plugin
	PM_PLUGIN_OPTIONS         =  6, //qip -> plugin
	// === Plugin specific messages ===
	PM_PLUGIN_SPELL_CHECK     =  7, //qip -> plugin, WParam = PWideChar to checking word, LParam = MissSpelledColor (delphi TColor). Return LParam with own color if needed and Result = True if word misspelled.
	PM_PLUGIN_SPELL_POPUP     =  8, //qip -> plugin, WParam = PWideChar to misspelled word, LParam is PPoint where PopupMenu should be popuped (screen coordinates). Return Result = True to ignore qip default menu popup.
	PM_PLUGIN_SPELL_REPLACE   =  9, //plugin -> qip, WParam = PWideChar to right word which will replace old misspelled word. Qip will return Result = True if misspelled word was successfully replaced.
	PM_PLUGIN_XSTATUS_UPD     = 10, //plugin -> qip, WParam = custom status picture number (from 0 to 35 or more if new custom status pics added), LParam = PWideChar of Status text (max 20 chars), NParam = PWideChar of status description text (max 512 chars). If WParam = 0 then custom status picture will be removed.
	// since 1.1.0
	PM_PLUGIN_XSTATUS_GET     = 11, //plugin -> qip, core will return WParam = custom status picture number (from 0 to 35 or more if new custom status pics added), LParam = PWideChar of Status text (max 20 chars), NParam = PWideChar of status description text (max 512 chars). If WParam = 0 then custom status picture not set by user.
	PM_PLUGIM_XSTATUS_CHANGED = 12, //qip -> plugin, user manually changed custom status picture/text, WParam = custom status picture number (from 0 to 35 or more if new custom status pics added), LParam = PWideChar of Status text (max 20 chars), NParam = PWideChar of status description text (max 512 chars). If WParam = 0 then custom status picture was removed by user.
	PM_PLUGIN_SOUND_GET       = 13, //plugin -> qip, if core returned WParam = True then qip sound enabled else sound disabled.
	PM_PLUGIN_SOUND_SET       = 14, //plugin -> qip, if WParam = True then qip will enable sound else will disable.
	PM_PLUGIN_SOUND_CHANGED   = 15, //qip -> plugin, user manually switched sound On/Off. if WParam = True the sound enabled.
	PM_PLUGIN_MSG_RCVD        = 16, //qip -> plugin, WParam = pQipMsgPlugin, return result = True to allow core accept this msg else message will be IGNORED, CAREFUL here! If you will add to LParam pointer to own widestring then original msg text will be replaced by yours own text.
	PM_PLUGIN_MSG_SEND        = 17, //qip -> plugin, WParam = pQipMsgPlugin, return result = True to allow send this msg else user will not be able to send this message, CAREFUL here! If you will add to LParam pointer to own widestring then original msg text will be replaced by yours own text.
	// since 1.2.1
	PM_PLUGIN_SPELL_RECHECK   = 18, //plugin -> qip, rechecks spelling for all message editing boxes
	PM_PLUGIN_MSG_RCVD_NEW    = 19, //qip -> plugin, notifier, qip received new message and its still not read by user. WParam = PWideChar to accountname of sender, LParam = PWideChar to nickname of sender. Plugin will receive this message periodically (~400 msec) until user will open msg window and read this msg.
	PM_PLUGIN_MSG_RCVD_READ   = 20, //qip -> plugin, notifier, new received message has been read by user and qip core will stop notifing with PM_PLUGIN_MSG_RCVD_NEW message. WParam = PWideChar to accountname of sender, LParam = PWideChar to nickname of sender. Plugin will receive this message only once after message or event will be read by user.
	// since 1.3.0
	PM_PLUGIN_WRONG_SDK_VER   = 21, //qip -> plugin, qip sends this message if plugin sdk version higher than qip's sdk version, after this msg qip will send PM_PLUGIN_QUIT message.
    PM_PLUGIN_CAN_ADD_BTNS    = 22, //qip -> plugin, broadcasted to all plugins, core creates message window and plugin can add buttons to panel below avatars, this message will be sent always on message window creation or tabs changing. WParam is PWideChar of account name of msg tab or wnd, LParam is PWideChar of protocol name of account, NParam is dll handle of protocol(for spec plugin msg sending needs);
    PM_PLUGIN_ADD_BTN         = 23, //plugin -> qip, wParam is pAddBtnInfo, core will return Result = Uniquq Action ID, which plugin will receive on every click on this btn, if Result will be returned  = 0 then btn was not added;
    PM_PLUGIN_MSG_BTN_CLICK   = 24, //qip -> plugin, occurs when user clicked on msg button below avatar. WParam is Unique Action ID given by core on adding this btn, LParam is PWideChar of account name of msg tab or wnd, NParam is PWideChar of protocol name of account, Result is dll handle of protocol(for spec plugin msg sending needs);
    PM_PLUGIN_SPEC_SEND       = 25, //plugin -> qip, WParam is protocol dll handle, LParam is PWideChar of receiver account name, NParam is special msg text/data. if Result returned = True then special message was sent else failed to send.
    PM_PLUGIN_SPEC_RCVD       = 26  //qip -> plugin, broadcasted to all plugins, WParam is protocol dll handle, LParam is PWideChar of sender account name, NParam is special msg text/data, Result is protocol name. 
};

enum
{
	// QIP msg types
	MSG_TYPE_UNK        = 0x00, //unknown msg type
	MSG_TYPE_TEXT       = 0x01,
	MSG_TYPE_CHAT       = 0x02,
	MSG_TYPE_FILE       = 0x03,
	MSG_TYPE_URL        = 0x04,
	MSG_TYPE_AUTH       = 0x05,
	MSG_TYPE_ADDED      = 0x06,
	MSG_TYPE_SERVER     = 0x07,
	MSG_TYPE_WEB        = 0x08,
	MSG_TYPE_CONTACTS   = 0x09,
	MSG_TYPE_AUTO       = 0x0A,
	MSG_TYPE_SERVICE    = 0x0B,
	MSG_TYPE_EMAIL      = 0x0C,
	MSG_TYPE_OFFMSG     = 0x0D,
	MSG_TYPE_AUTHREPLY  = 0x0E,
};