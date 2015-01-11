// "Now Playing" QIP Infium Plugin
// class NPLanguageManager
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once
#include "INPLanguageManager.h"

class NPLanguageManager : public INPLanguageManager
{
public:
	NPLanguageManager();
	virtual ~NPLanguageManager () {};

	bool LoadLanguages ( std::wstring & );
	bool SetLanguageById ( int );
	int GetLanguageId ();
	std::wstring GetLanguageName ();
	std::wstring GetString ( int, int );
	HICON GetIcon ( int );
	HBITMAP GetBitmap ( int );
	HINSTANCE GetResourceInstance ();
	std::vector<NPLanguage> GetLanguageList ();

private:
	std::vector<NPLanguage> languages;
	NPLanguage *currentLanguage;

	void _CleanUp ();
};