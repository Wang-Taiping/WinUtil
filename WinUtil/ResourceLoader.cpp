#include "ResourceLoader.h"
//#include "Path.hpp"
//#include "String.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <filesystem>

static std::filesystem::path mdlabspath(void* mdl)
{
	std::wstring szpath;
	szpath.resize(4096, 0);
	GetModuleFileNameW(HMODULE(mdl), &szpath[0], 4096);
	return szpath;
}

inline std::wstring cvtstr(std::string string)
{
	int size = MultiByteToWideChar(936, 0, string.c_str(), -1, nullptr, 0);
	wchar_t* temp_str = new wchar_t[size];
	MultiByteToWideChar(936, 0, string.c_str(), -1, temp_str, size);
	std::wstring ret_str = temp_str;
	delete[] temp_str;
	return ret_str;
}

typedef struct NativeVersionInfo
{
	std::string Data;
	std::string Prefix;
} *PNativeVersionInfo;

void* GetModuleHandleByFunctionAddress(void* ModuleFunctionAddress)
{
	MEMORY_BASIC_INFORMATION mbi;
	return ((::VirtualQuery(ModuleFunctionAddress, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : nullptr);
}

size_t LoadResourceA(void* hModule, const char* Type, int RscId, void** Buffer)
{
	HRSRC hRSrc = FindResourceA(HMODULE(hModule), MAKEINTRESOURCEA(RscId), Type);
	if (!hRSrc)
		return 0;
	HGLOBAL hGlobal = LoadResource(HMODULE(hModule), hRSrc);
	if (!hGlobal)
		return 0;
	LPVOID nBuffer = LockResource(hGlobal);
	if (!nBuffer)
		return 0;
	DWORD nBufLen = SizeofResource(HMODULE(hModule), hRSrc);
	*Buffer = nBuffer;
	return nBufLen;
}

size_t LoadResourceW(void* hModule, const wchar_t* Type, int RscId, void** Buffer)
{
	HRSRC hRSrc = FindResourceW(HMODULE(hModule), MAKEINTRESOURCEW(RscId), Type);
	if (!hRSrc)
		return 0;
	HGLOBAL hGlobal = LoadResource(HMODULE(hModule), hRSrc);
	if (!hGlobal)
		return 0;
	LPVOID nBuffer = LockResource(hGlobal);
	if (!nBuffer)
		return 0;
	DWORD nBufLen = SizeofResource(HMODULE(hModule), hRSrc);
	*Buffer = nBuffer;
	return nBufLen;
}

HVERSIONINFO LoadVersionInfo(void* hModule)
{
	std::wstring Path = mdlabspath(hModule).wstring();
	DWORD Size = GetFileVersionInfoSizeW(Path.c_str(), nullptr);
	if (Size > 0) {
		std::string Data;
		Data.resize(Size);
		if (!GetFileVersionInfoW(Path.c_str(), 0, Size, &Data[0])) return nullptr;
		UINT uLen = 0;
		LPDWORD lang = nullptr;
		char strLanguage[10] = { 0 };
		if (VerQueryValueA(Data.c_str(), "\\VarFileInfo\\Translation", (void**)&lang, &uLen)) {
			wsprintfA(strLanguage, "%04x%04x\\", LOWORD(*lang), HIWORD(*lang));
		}
		std::string Prefix = "\\StringFileInfo\\";
		Prefix += strLanguage;
		PNativeVersionInfo HVI = new NativeVersionInfo;
		HVI->Data.swap(Data);
		HVI->Prefix.swap(Prefix);
		return HVERSIONINFO(HVI);
	}
	return nullptr;
}

void ReleaseVersionInfo(HVERSIONINFO hVersionInfo)
{
	if (!hVersionInfo) return;
	PNativeVersionInfo HVI = PNativeVersionInfo(hVersionInfo);
	delete HVI;
}

static std::string GetSymbol(PNativeVersionInfo HVI, VersionInfoItem Item)
{
	std::string Symbol = HVI->Prefix;
	switch (Item)
	{
	case CompanyName:
		Symbol += "CompanyName";
		break;
	case FileDescription:
		Symbol += "FileDescription";
		break;
	case FileVersion:
		Symbol += "FileVersion";
		break;
	case InternalName:
		Symbol += "InternalName";
		break;
	case LegalCopyright:
		Symbol += "LegalCopyright";
		break;
	case OriginalFilename:
		Symbol += "OriginalFilename";
		break;
	case ProductName:
		Symbol += "ProductName";
		break;
	case ProductVersion:
		Symbol += "ProductVersion";
		break;
	default:
		return std::string();
	}
	return Symbol;
}

uint32_t GetVersionInfoItemA(HVERSIONINFO hVersionInfo, VersionInfoItem Item, char* Buffer, uint32_t Buflen)
{
	if (!hVersionInfo) return 0;
	PNativeVersionInfo HVI = PNativeVersionInfo(hVersionInfo);
	std::string Symbol = GetSymbol(HVI, Item);
	if (Symbol.empty()) return 0;
	LPVOID lpBuffer = nullptr;
	UINT uLen = 0;
	if (VerQueryValueA(HVI->Data.c_str(), Symbol.c_str(), (LPVOID*)&lpBuffer, &uLen)) {
		if (uLen > Buflen) return uLen;
		memcpy_s(Buffer, Buflen, lpBuffer, uLen);
	}
	return uLen;
}

uint32_t GetVersionInfoItemW(HVERSIONINFO hVersionInfo, VersionInfoItem Item, wchar_t* Buffer, uint32_t Buflen)
{
	if (!hVersionInfo) return 0;
	PNativeVersionInfo HVI = PNativeVersionInfo(hVersionInfo);
	std::wstring Symbol = cvtstr(GetSymbol(HVI, Item));
	if (Symbol.empty()) return 0;
	LPVOID lpBuffer = nullptr;
	UINT uLen = 0;
	if (VerQueryValueW(HVI->Data.c_str(), Symbol.c_str(), (LPVOID*)&lpBuffer, &uLen)) {
		if (uLen > Buflen) return uLen;
		wmemcpy_s(Buffer, Buflen, LPCWSTR(lpBuffer), uLen);
	}
	return uLen;
}
