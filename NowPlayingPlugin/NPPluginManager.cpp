// "Now Playing" QIP Infium Plugin
// class NPPluginManager
// Provides basic functions to extensions (plugins) like RemoteControls for Media Players etc...
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "NPPluginManager.h"

NPPluginManager::NPPluginManager ()
{
	activePlugin = NULL;
}

NPPluginManager::~NPPluginManager ()
{
}

bool NPPluginManager::LoadPluginsDirectory ( std::wstring & path )
{
	std::wstring search_query = path;
	search_query.append ( L"\\*.dll" );
    struct _wfinddata_t c_file;
    intptr_t hFile;

	if( (hFile = _wfindfirst( search_query.c_str (), &c_file )) != -1L )
	{
      do 
	  {
		std::wstring pluginPath = path;
		pluginPath.append ( L"\\" );
		pluginPath.append ( c_file.name );
		LoadPlugin ( pluginPath );
      } while( _wfindnext( hFile, &c_file ) == 0 );
      _findclose( hFile );
   }
	
	return true;
}

bool NPPluginManager::LoadPlugin ( std::wstring & path )
{
	HMODULE hPluginInstance;

	if ( ( hPluginInstance = LoadLibrary( path.c_str () ) ) != NULL )
	{
			INPPlugin* (__stdcall* _GetInstance)(void) = (INPPlugin* (__stdcall*)(void))GetProcAddress( hPluginInstance, "GetInstance" );
			if(_GetInstance != NULL)
			{
				INPPlugin* plugin = _GetInstance ();
				plugin->wPath = path;
				plugin->hInstance = hPluginInstance;
				if ( plugin->Initialize () )
				{
					pluginList.push_back ( plugin );
				} else
				{
					delete plugin;
					return false;
				}

				return true;
			}
			else
			{
				return false; 
			}
	}
	else
	{
		return false;
	}

	return false;
}

bool NPPluginManager::SetActivePlugin ( std::wstring & guid )
{
	if ( pluginList.size () > 0 )
	{
		INPPlugin *plg = GetPlugin(guid);
		activePlugin = plg;

		return true;
	} 
	else
	{
		return false;
	}
}

INPPlugin* NPPluginManager::GetActivePlugin ()
{
	return activePlugin;
}

INPPlugin* NPPluginManager::GetPlugin ( std::wstring & guid )
{
	for ( std::vector<INPPlugin*>::iterator i = pluginList.begin (); i != pluginList.end (); i++ )
	{
		if ( (*i)->GetPluginDescription()->guid == guid )
		{
			return (*i);
		}
	}

	return NULL;
}

bool NPPluginManager::UnloadPlugins ()
{
	for ( std::vector<INPPlugin*>::iterator i = pluginList.begin (); i != pluginList.end (); i++ )
	{
		if ( (*i) )
		{
			HMODULE hOld = (*i)->GetHandle ();
			(*i)->UnInitialize ();
			delete (*i);
			FreeLibrary ( hOld );
		}
	}
	return true;
}