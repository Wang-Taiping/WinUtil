#pragma once

#ifndef WINUTIL_IPC_H
#define WINUTIL_IPC_H

#include "WinUtilBase.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	typedef Access IPCAccess;
	WINUTIL_TYPENAME(HPIPE);

	WINUTIL_EXPORT int WINUTIL_API PipeExistA(const char* PipeName, uint32_t Milliseconds);
	WINUTIL_EXPORT int WINUTIL_API PipeExistW(const wchar_t* PipeName, uint32_t Milliseconds);
	WINUTIL_EXPORT HPIPE WINUTIL_API PipeCreateA(const char* PipeName, int AccessMode);
	WINUTIL_EXPORT HPIPE WINUTIL_API PipeCreateW(const wchar_t* PipeName, int AccessMode);
	WINUTIL_EXPORT int WINUTIL_API PipeOpen(HPIPE PipeHandle);
	WINUTIL_EXPORT void WINUTIL_API PipeClose(HPIPE PipeHandle);
	WINUTIL_EXPORT HPIPE WINUTIL_API PipeConnectA(const char* PipeName, int AccessMode);
	WINUTIL_EXPORT HPIPE WINUTIL_API PipeConnectW(const wchar_t* PipeName, int AccessMode);
	WINUTIL_EXPORT void WINUTIL_API PipeDestroy(HPIPE PipeHandle);
	WINUTIL_EXPORT uint64_t WINUTIL_API PipeRead(HPIPE PipeHandle, void* Buffer, uint64_t ReadBytes);
	WINUTIL_EXPORT uint64_t WINUTIL_API PipeWrite(HPIPE PipeHandle, const void* Data, uint64_t WriteBytes);
#ifdef __cplusplus
}
#endif // __cplusplusj

#endif // !WINUTIL_IPC_H
