// "Now Playing" QIP Infium Plugin
// Insignificant Utility functions
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#pragma once
#include <string>
#pragma comment ( lib, "Version.lib" )

struct FILEVERSION_INFO
{
	std::wstring Comments;
	std::wstring InternalName;
	std::wstring ProductName;
	std::wstring CompanyName;
	std::wstring LegalCopyright;
	std::wstring ProductVersion;
	std::wstring FileDescription;
	std::wstring LegalTrademarks;
	std::wstring PrivateBuild;
	std::wstring FileVersion;
	std::wstring OriginalFilename;
	std::wstring SpecialBuild;
};

unsigned int mbs2ws ( const char*, wchar_t*, int );
int ShowHtmlHelpText ( LPCWSTR, HWND, HELPINFO* );
void GetFileVersion ( const wchar_t*, FILEVERSION_INFO* );