// "Now Playing" QIP Infium Plugin
// class NPLanguageManager
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "NPLanguageManager.h"

NPLanguageManager::NPLanguageManager ()
{
	currentLanguage = NULL;
}

bool NPLanguageManager::LoadLanguages ( std::wstring & path )
{
	std::wstring search_query = path;
	search_query.append ( L"\\*.dll" );
	currentLanguage = NULL;
    struct _wfinddata_t c_file;
    intptr_t hFile;

	if( (hFile = _wfindfirst( search_query.c_str (), &c_file )) != -1L )
	{
      do 
	  {
		NPLanguage item;
		item.path = path;
		item.path.append ( L"\\" );
		item.path.append ( c_file.name );
		if ( (item.hInstance = LoadLibrary ( item.path.c_str () )) != NULL )
		{
			wchar_t* buffer = new wchar_t[30];
			buffer[0] = '\0';
			if ( LoadString ( item.hInstance, IDS_STRING122, buffer, 30 ) )
			{
				item.name = buffer;
				if ( LoadString ( item.hInstance, IDS_STRING123, buffer, 30 ) )
				{
					item.id = _wtoi ( buffer );
					languages.push_back ( item );
				}
				else 
				{
					FreeLibrary ( item.hInstance );
				}
			} 
			else 
			{
				FreeLibrary ( item.hInstance );
			}
			delete[] buffer;
		}

      } while( _wfindnext( hFile, &c_file ) == 0 );
      _findclose( hFile );
   }
	return true;
}

bool NPLanguageManager::SetLanguageById ( int langId )
{ 
	for ( std::vector<NPLanguage>::iterator i = languages.begin (); i != languages.end (); i++ )
		if ( (*i).id == langId )
		{
			currentLanguage = &(*i);
			return true;
		}
	return false;
}

int NPLanguageManager::GetLanguageId ()
{
	if ( currentLanguage )
	{
		return currentLanguage->id;
	}
	return 0;
}

std::wstring NPLanguageManager::GetLanguageName ()
{
	if ( currentLanguage )
	{
		return currentLanguage->name;
	}
	return L"";
}

std::wstring NPLanguageManager::GetString ( int strId, int maxLen )
{
	std::wstring strRet;

	if ( currentLanguage )
	{
		wchar_t* buffer = new wchar_t[maxLen];
		buffer[0] = '\0';
		if ( LoadString ( currentLanguage->hInstance, strId, buffer, maxLen ) )
			strRet = buffer;
		delete[] buffer;
	}
	return strRet;
}

HICON NPLanguageManager::GetIcon ( int iconId )
{
	if ( currentLanguage )
	{
		return LoadIcon ( currentLanguage->hInstance, MAKEINTRESOURCE ( iconId ) );
	}
	return NULL;
}

HBITMAP NPLanguageManager::GetBitmap ( int bmpId )
{
	if ( currentLanguage )
	{
		return LoadBitmap ( currentLanguage->hInstance, MAKEINTRESOURCE ( bmpId ) );
	}
	return NULL;
}

HINSTANCE NPLanguageManager::GetResourceInstance ()
{
	if ( currentLanguage )
	{
		return currentLanguage->hInstance;
	}
	return NULL;
}

std::vector<NPLanguage> NPLanguageManager::GetLanguageList ()
{
	return languages;
}

void NPLanguageManager::_CleanUp ()
{
	for ( std::vector<NPLanguage>::iterator i = languages.begin (); i != languages.end (); i++ )
		if ( (*i).hInstance )
			FreeLibrary ( (*i).hInstance );

	languages.clear ();
}