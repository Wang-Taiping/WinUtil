#pragma once

#ifndef WINUTIL_RESLOADER_H
#define WINUTIL_RESLOADER_H

#include "UtilBase.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	TYPEHANDLE(HVERSIONINFO);
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

	WINUTIL_EXPORT HANDLE GetModuleHandleByFunctionAddress(void* ModuleFunctionAddress);
	WINUTIL_EXPORT size_t LoadResourceA(void* hModule, const char* Type, int RscId, void** Buffer);
	WINUTIL_EXPORT size_t LoadResourceW(void* hModule, const wchar_t* Type, int RscId, void** Buffer);
	WINUTIL_EXPORT HVERSIONINFO LoadVersionInfo(void* hModule);
	WINUTIL_EXPORT void ReleaseVersionInfo(HVERSIONINFO hVersionInfo);
	WINUTIL_EXPORT uint32_t GetVersionInfoItemA(HVERSIONINFO hVersionInfo, VersionInfoItem Item, char* Buffer, uint32_t Buflen);
	WINUTIL_EXPORT uint32_t GetVersionInfoItemW(HVERSIONINFO hVersionInfo, VersionInfoItem Item, wchar_t* Buffer, uint32_t Buflen);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !WINUTIL_RESLOADER_H
