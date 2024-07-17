#include "Shortcut.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ShlObj.h>

int CreateShortcutA(const char* SourceFile, const char* ShortcutFile, const char* Description, const char* WorkingDir, const char* IconPath, int IconId)
{
	int nReturn = 1;
	IShellLinkA* pIShellLink = nullptr;
	IPersistFile* pIPersistFile = nullptr;
	bool COMInit = false;
	try
	{
		HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pIShellLink);
		if (!SUCCEEDED(hr)) {
			//throw "create shell_link object fail.";
			CoInitialize(nullptr);
			COMInit = true;
			hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pIShellLink);
			if (!SUCCEEDED(hr)) throw "create shell_link object and retry fail.";
		}
		hr = pIShellLink->SetPath(SourceFile);
		if (!SUCCEEDED(hr))
			throw "set target fail";
		if (Description)
		{
			hr = pIShellLink->SetDescription(Description);
			if (!SUCCEEDED(hr))
				throw "set description fail.";
		}
		if (WorkingDir)
		{
			hr = pIShellLink->SetWorkingDirectory(WorkingDir);
			if (!SUCCEEDED(hr))
				throw "set working_directory fail.";
		}
		if (IconPath)
		{
			hr = pIShellLink->SetIconLocation(IconPath, IconId);
			if (!SUCCEEDED(hr))
				throw "set icon fail.";
		}
		hr = pIShellLink->QueryInterface(IID_IPersistFile, (void**)&pIPersistFile);
		if (!SUCCEEDED(hr))
			throw "create save object fail.";
		LPWSTR Shortcut = new WCHAR[strlen(ShortcutFile) + 1];
		memset(Shortcut, 0, strlen(ShortcutFile) + 1);
		MultiByteToWideChar(936, 0, ShortcutFile, -1, Shortcut, strlen(ShortcutFile));
		hr = pIPersistFile->Save(Shortcut, TRUE);
		delete[] Shortcut;
		if (!SUCCEEDED(hr))
			throw "save fail.";
	}
	catch (...) { nReturn = 0; }
	if (COMInit) CoUninitialize();
	if (pIPersistFile) pIPersistFile->Release();
	if (pIShellLink) pIShellLink->Release();
	return nReturn;
}

int CreateShortcutW(const wchar_t* SourceFile, const wchar_t* ShortcutFile, const wchar_t* Description, const wchar_t* WorkingDir, const wchar_t* IconPath, int IconId)
{
	int nReturn = 1;
	IShellLinkW* pIShellLink = nullptr;
	IPersistFile* pIPersistFile = nullptr;
	bool COMInit = false;
	try
	{
		HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pIShellLink);
		if (!SUCCEEDED(hr)) {
			//throw "create shell_link object fail.";
			CoInitialize(nullptr);
			COMInit = true;
			hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pIShellLink);
			if (!SUCCEEDED(hr)) throw "create shell_link object and retry fail.";
		}
		hr = pIShellLink->SetPath(SourceFile);
		if (!SUCCEEDED(hr))
			throw "set target fail";
		if (Description)
		{
			hr = pIShellLink->SetDescription(Description);
			if (!SUCCEEDED(hr))
				throw "set description fail.";
		}
		if (WorkingDir)
		{
			hr = pIShellLink->SetWorkingDirectory(WorkingDir);
			if (!SUCCEEDED(hr))
				throw "set working_directory fail.";
		}
		if (IconPath)
		{
			hr = pIShellLink->SetIconLocation(IconPath, IconId);
			if (!SUCCEEDED(hr))
				throw "set icon fail.";
		}
		hr = pIShellLink->QueryInterface(IID_IPersistFile, (void**)&pIPersistFile);
		if (!SUCCEEDED(hr))
			throw "create save object fail.";
		hr = pIPersistFile->Save(ShortcutFile, TRUE);
		if (!SUCCEEDED(hr))
			throw "save fail.";
	}
	catch (...) { nReturn = 0; }
	if (COMInit) CoUninitialize();
	if (pIPersistFile) pIPersistFile->Release();
	if (pIShellLink) pIShellLink->Release();
	return nReturn;
}
