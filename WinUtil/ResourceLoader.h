#pragma once

#ifndef WINUTIL_RESLOADER_H
#define WINUTIL_RESLOADER_H

#include "WinUtilBase.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	WINUTIL_TYPENAME(HVERSIONINFO);
	typedef void* HANDLE;
	enum VersionInfoItem
	{
		CompanyName,
		FileDescription,
		FileVersion,
		InternalName,
		LegalCopyright,
		OriginalFilename,
		ProductName,
		ProductVersion
	};

	WINUTIL_EXPORT HANDLE WINUTIL_API GetModuleHandleByFunctionAddress(void* ModuleFunctionAddress);
	WINUTIL_EXPORT size_t WINUTIL_API LoadResourceA(void* hModule, const char* Type, int RscId, void** Buffer);
	WINUTIL_EXPORT size_t WINUTIL_API LoadResourceW(void* hModule, const wchar_t* Type, int RscId, void** Buffer);
	WINUTIL_EXPORT HVERSIONINFO WINUTIL_API LoadVersionInfo(void* hModule);
	WINUTIL_EXPORT void WINUTIL_API ReleaseVersionInfo(HVERSIONINFO hVersionInfo);
	WINUTIL_EXPORT uint32_t WINUTIL_API GetVersionInfoItemA(HVERSIONINFO hVersionInfo, VersionInfoItem Item, char* Buffer, uint32_t Buflen);
	WINUTIL_EXPORT uint32_t WINUTIL_API GetVersionInfoItemW(HVERSIONINFO hVersionInfo, VersionInfoItem Item, wchar_t* Buffer, uint32_t Buflen);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !WINUTIL_RESLOADER_H
