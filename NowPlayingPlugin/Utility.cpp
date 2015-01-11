// "Now Playing" QIP Infium Plugin
// Insignificant Utility functions
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include "stdafx.h"
#include "Utility.h"

unsigned int mbs2ws ( const char* s, wchar_t* ws, int maxlen )
{
    int size = MultiByteToWideChar(CP_UTF7, 0, s, -1, 0, 0);
	if ( size > maxlen && maxlen > 0 ) 
		size = maxlen;
    MultiByteToWideChar(CP_UTF7, 0, s, -1, ws, size);
    return 1;
}

int ShowHtmlHelpText ( LPCWSTR HelpText, HWND hwndCTRL, HELPINFO *pHelpInfo )
{
    HMODULE  hModule = NULL;
    HTMLHELP HtmlHelp = NULL;
    HH_POPUP popup;
    RECT rc;
    POINT pt;

    if ( !HelpStruct.bHelpSystemInit ) return 0;
    if ( HelpText==NULL || hwndCTRL == NULL ) return 0;
    if ( wcslen ( HelpText ) > MAX_PATH ) return 0; 

    hModule = HelpStruct.hModule;//Проверяем загружен ли модуль
    if ( !hModule ) return 0;
    HtmlHelp = (HTMLHELP) GetProcAddress ( hModule, "HtmlHelpW" );//Определяем точку входа в процедуру
         if (!HtmlHelp) return 0;

    ZeroMemory ( &popup,sizeof ( HH_POPUP ) );
    
    if (pHelpInfo) popup.pt = pHelpInfo->MousePos;
    else 
    {
       pt.x=pt.y=-1;
            GetCursorPos(&pt);
    }

    //Границы по автомату
    SetRect(&rc,-1,-1,-1,-1);
    
    popup.cbStruct=sizeof(HH_POPUP);//Заполняем структуру для сообщения help'a
         popup.pszText=HelpText;
    popup.clrForeground=-1;//Цвета по умолчанию
         popup.clrBackground=-1; 
         popup.rcMargins = rc; 
         popup.pszFont = L"Arial, 8";//  или "MS Sans Serif, 8, , ";
    
    HtmlHelp ( hwndCTRL, NULL, HH_DISPLAY_TEXT_POPUP, (DWORD)&popup ); //Выводим Help

         return 1;
}

void GetFileVersion ( const wchar_t* pszAppName, FILEVERSION_INFO* verInfo )
{
	DWORD dwScratch;
	DWORD * pdwLangChar;
	DWORD dwInfSize = 255;
	UINT uSize;
	BYTE * pbyInfBuff;
	wchar_t szQuery[255];
	wchar_t szResource [80];
	wchar_t pszLangBuff[32];
	wchar_t* pszQuery = szQuery;

	dwInfSize = GetFileVersionInfoSize ( pszAppName, &dwScratch );

	if (dwInfSize)
	{
		pbyInfBuff = new BYTE [dwInfSize];
		memset (pbyInfBuff, 0, dwInfSize);
		memset (pszQuery, 0, 255);
		if ( pbyInfBuff )
		{
			if ( GetFileVersionInfo ( pszAppName, 0, dwInfSize, pbyInfBuff ) )
			{
				if ( VerQueryValue ( pbyInfBuff,
					 L"\\VarFileInfo\\Translation",
					 (void**)(&pdwLangChar),
					 &uSize))
				{
					if ( VerLanguageName (LOWORD (*pdwLangChar), szResource, sizeof(szResource)) )
					{
						wcsncpy_s ( pszLangBuff, 32, szResource, 32 );
					}

					wsprintf ( szResource, L"\\StringFileInfo\\%04X%04X\\FileVersion", LOWORD (*pdwLangChar), HIWORD (*pdwLangChar) );
					if ( VerQueryValue ( pbyInfBuff, szResource,(void**)(&pszQuery), &uSize ) )
					{
						verInfo->FileVersion = pszQuery;
					}

					wsprintf ( szResource, L"\\StringFileInfo\\%04X%04X\\Comments", LOWORD (*pdwLangChar), HIWORD (*pdwLangChar) );
					if ( VerQueryValue ( pbyInfBuff, szResource,(void**)(&pszQuery), &uSize ) )
					{
						verInfo->Comments = pszQuery;
					}

					wsprintf ( szResource, L"\\StringFileInfo\\%04X%04X\\InternalName", LOWORD (*pdwLangChar), HIWORD (*pdwLangChar) );
					if ( VerQueryValue ( pbyInfBuff, szResource,(void**)(&pszQuery), &uSize ) )
					{
						verInfo->InternalName = pszQuery;
					}

					wsprintf ( szResource, L"\\StringFileInfo\\%04X%04X\\ProductName", LOWORD (*pdwLangChar), HIWORD (*pdwLangChar) );
					if ( VerQueryValue ( pbyInfBuff, szResource,(void**)(&pszQuery), &uSize ) )
					{
						verInfo->ProductName = pszQuery;
					}

					wsprintf ( szResource, L"\\StringFileInfo\\%04X%04X\\FileDescription", LOWORD (*pdwLangChar), HIWORD (*pdwLangChar) );
					if ( VerQueryValue ( pbyInfBuff, szResource,(void**)(&pszQuery), &uSize ) )
					{
						verInfo->FileDescription = pszQuery;
					}

					wsprintf ( szResource, L"\\StringFileInfo\\%04X%04X\\LegalCopyright", LOWORD (*pdwLangChar), HIWORD (*pdwLangChar) );
					if ( VerQueryValue ( pbyInfBuff, szResource,(void**)(&pszQuery), &uSize ) )
					{
						verInfo->LegalCopyright = pszQuery;
					}
					
					wsprintf ( szResource, L"\\StringFileInfo\\%04X%04X\\LegalTrademarks", LOWORD (*pdwLangChar), HIWORD (*pdwLangChar) );
					if ( VerQueryValue ( pbyInfBuff, szResource,(void**)(&pszQuery), &uSize ) )
					{
						verInfo->LegalTrademarks = pszQuery;
					}

					wsprintf ( szResource, L"\\StringFileInfo\\%04X%04X\\PrivateBuild", LOWORD (*pdwLangChar), HIWORD (*pdwLangChar) );
					if ( VerQueryValue ( pbyInfBuff, szResource,(void**)(&pszQuery), &uSize ) )
					{
						verInfo->PrivateBuild = pszQuery;
					}

					wsprintf ( szResource, L"\\StringFileInfo\\%04X%04X\\OriginalFilename", LOWORD (*pdwLangChar), HIWORD (*pdwLangChar) );
					if ( VerQueryValue ( pbyInfBuff, szResource,(void**)(&pszQuery), &uSize ) )
					{
						verInfo->OriginalFilename = pszQuery;
					}

					wsprintf ( szResource, L"\\StringFileInfo\\%04X%04X\\SpecialBuild", LOWORD (*pdwLangChar), HIWORD (*pdwLangChar) );
					if ( VerQueryValue ( pbyInfBuff, szResource,(void**)(&pszQuery), &uSize ) )
					{
						verInfo->SpecialBuild = pszQuery;
					}
				}
			}
			delete [] pbyInfBuff;
		}
	}
} 