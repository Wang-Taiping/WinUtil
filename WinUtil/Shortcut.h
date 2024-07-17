#pragma once

#ifndef WINUTIL_SHORTCUT_H
#define WINUTIL_SHORTCUT_H

#include "UtilBase.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	WINUTIL_EXPORT int CreateShortcutA(const char* SourceFile, const char* ShortcutFile, const char* Description, const char* WorkingDir, const char* IconPath, int IconId);
	WINUTIL_EXPORT int CreateShortcutW(const wchar_t* SourceFile, const wchar_t* ShortcutFile, const wchar_t* Description, const wchar_t* WorkingDir, const wchar_t* IconPath, int IconId);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !WINUTIL_SHORTCUT_H
