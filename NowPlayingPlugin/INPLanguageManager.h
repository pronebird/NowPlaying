// "Now Playing" QIP Infium Plugin
// class INPLanguageManager, np-languagemanager interface
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once

struct NPLanguage
{
	int id;
	std::wstring name;
	std::wstring path;
	HMODULE hInstance;
};

class INPLanguageManager
{
public:
	virtual ~INPLanguageManager () {};

	virtual bool LoadLanguages ( std::wstring & ) = 0;
	virtual bool SetLanguageById ( int ) = 0;
	virtual int GetLanguageId () = 0;
	virtual std::wstring GetLanguageName () = 0;
	virtual std::wstring GetString ( int, int ) = 0;
	virtual	HICON GetIcon ( int ) = 0;
	virtual HBITMAP GetBitmap ( int ) = 0;
	virtual HINSTANCE GetResourceInstance () = 0;
	virtual std::vector<NPLanguage> GetLanguageList () = 0;
};