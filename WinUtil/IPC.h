#pragma once

#ifndef WINUTIL_IPC_H
#define WINUTIL_IPC_H

#include "WinUtilBase.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	typedef Access IPCAccess;
	TYPEHANDLE(HPIPE);

	WINUTIL_EXPORT int PipeExistA(const char* PipeName, uint32_t Milliseconds);
	WINUTIL_EXPORT int PipeExistW(const wchar_t* PipeName, uint32_t Milliseconds);
	WINUTIL_EXPORT HPIPE PipeCreateA(const char* PipeName, int AccessMode);
	WINUTIL_EXPORT HPIPE PipeCreateW(const wchar_t* PipeName, int AccessMode);
	WINUTIL_EXPORT int PipeOpen(HPIPE PipeHandle);
	WINUTIL_EXPORT void PipeClose(HPIPE PipeHandle);
	WINUTIL_EXPORT HPIPE PipeConnectA(const char* PipeName, int AccessMode);
	WINUTIL_EXPORT HPIPE PipeConnectW(const wchar_t* PipeName, int AccessMode);
	WINUTIL_EXPORT void PipeDestroy(HPIPE PipeHandle);
	WINUTIL_EXPORT uint64_t PipeRead(HPIPE PipeHandle, void* Buffer, uint64_t ReadBytes);
	WINUTIL_EXPORT uint64_t PipeWrite(HPIPE PipeHandle, const void* Data, uint64_t WriteBytes);
#ifdef __cplusplus
}
#endif // __cplusplusj

#endif // !WINUTIL_IPC_H
