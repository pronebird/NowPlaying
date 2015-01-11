// "Now Playing" QIP Infium Plugin
// class INPPlugin, np-plugin interface
// (c) 2007-2008 Mihaylov Andrey
// ------------------------------------------

#include <string>

struct PluginDescription
{
public:
	std::wstring name;
	std::wstring guid;
	HBITMAP image;
	COLORREF mask;
	bool apisupport;
};

struct PlayerData
{
	std::wstring artist;
	std::wstring genre;
	std::wstring album;
	std::wstring number;
	std::wstring title;
	std::wstring length;
	std::wstring filename;
};

class INPPlugin
{
public:
	virtual ~INPPlugin () {};
	virtual PluginDescription* GetPluginDescription () = 0;
	virtual bool Initialize () = 0;
	virtual bool UnInitialize () = 0;
	virtual bool OnCheckStatus ( std::wstring & ) { return false; };
	virtual bool OnCheckStatus ( PlayerData & ) { return false; };
	virtual HMODULE GetHandle () { return hInstance; };
	virtual std::wstring GetPath () { return wPath; };

private:
	HMODULE hInstance;
	std::wstring wPath;
	friend class NPPluginManager;
};