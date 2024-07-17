#include "IPC.h"
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "IPC.h"

#define IPC_PIPE_PREFIX  R"(\\.\pipe\)"
#define IPC_PIPE_PREFIXW  LR"(\\.\pipe\)"

int PipeExistA(const char* PipeName, uint32_t Milliseconds)
{
	std::string name = IPC_PIPE_PREFIX;
	name += PipeName;
	while (Milliseconds > 200)
	{
		if (WaitNamedPipeA(name.c_str(), 1)) return true;
		Sleep(199);
		Milliseconds -= 200;
	}
	if (Milliseconds > 1) Sleep(Milliseconds - 1);
	return WaitNamedPipeA(name.c_str(), 1);
}

int PipeExistW(const wchar_t* PipeName, uint32_t Milliseconds)
{
	std::wstring name = IPC_PIPE_PREFIXW;
	name += PipeName;
	while (Milliseconds > 200)
	{
		if (WaitNamedPipeW(name.c_str(), 1)) return true;
		Sleep(199);
		Milliseconds -= 200;
	}
	if (Milliseconds > 1) Sleep(Milliseconds - 1);
	return WaitNamedPipeW(name.c_str(), 1);
}

HPIPE PipeCreateA(const char* PipeName, int AccessMode)
{
	if (!(AccessMode & AccessAll)) return nullptr;
	std::string name = IPC_PIPE_PREFIX;
	name += PipeName;
	HPIPE hPipe = (HPIPE)CreateNamedPipeA(name.c_str(), DWORD(AccessMode) | FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 4096, 4096, 0, nullptr);
	return (hPipe == INVALID_HANDLE_VALUE) ? nullptr : hPipe;
}

HPIPE PipeCreateW(const wchar_t* PipeName, int AccessMode)
{
	if (!(AccessMode & AccessAll)) return nullptr;
	std::wstring name = IPC_PIPE_PREFIXW;
	name += PipeName;
	HPIPE hPipe = (HPIPE)CreateNamedPipeW(name.c_str(), DWORD(AccessMode) | FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 4096, 4096, 0, nullptr);
	return (hPipe == INVALID_HANDLE_VALUE) ? nullptr : hPipe;
}

int PipeOpen(HPIPE PipeHandle)
{
	if (PipeHandle == nullptr) return 0;
	return ConnectNamedPipe(PipeHandle, nullptr);
}

void PipeClose(HPIPE PipeHandle)
{
	if (PipeHandle == nullptr) return;
	DisconnectNamedPipe(PipeHandle);
}

HPIPE PipeConnectA(const char* PipeName, int AccessMode)
{
	if (!(AccessMode & AccessAll)) return nullptr;
	std::string name = IPC_PIPE_PREFIX;
	name += PipeName;
	DWORD Permission = 0;
	if (AccessMode & AccessRead) Permission |= GENERIC_READ;
	if (AccessMode & AccessWrite) Permission |= GENERIC_WRITE;
	HPIPE hPipe = (HPIPE)CreateFileA(name.c_str(), Permission, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	return (hPipe == INVALID_HANDLE_VALUE) ? nullptr : hPipe;
}

HPIPE PipeConnectW(const wchar_t* PipeName, int AccessMode)
{
	if (!(AccessMode & AccessAll)) return nullptr;
	std::wstring name = IPC_PIPE_PREFIXW;
	name += PipeName;
	DWORD Permission = 0;
	if (AccessMode & AccessRead) Permission |= GENERIC_READ;
	if (AccessMode & AccessWrite) Permission |= GENERIC_WRITE;
	HPIPE hPipe = (HPIPE)CreateFileW(name.c_str(), Permission, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	return (hPipe == INVALID_HANDLE_VALUE) ? nullptr : hPipe;
}

void PipeDestroy(HPIPE PipeHandle)
{
	if (PipeHandle == nullptr) return;
	CloseHandle(PipeHandle);
}

uint64_t PipeRead(HPIPE PipeHandle, void* Buffer, uint64_t ReadBytes)
{
	if (PipeHandle == nullptr || Buffer == nullptr || ReadBytes == 0) return 0;
	DWORD readed = 0;
	if (!ReadFile(PipeHandle, Buffer, ReadBytes, &readed, nullptr)) return 0;
	return readed;
}

uint64_t PipeWrite(HPIPE PipeHandle, const void* Data, uint64_t WriteBytes)
{
	if (PipeHandle == nullptr || Data == nullptr || WriteBytes == 0) return 0;
	DWORD writed = 0;
	if (!WriteFile(PipeHandle, Data, WriteBytes, &writed, nullptr))
		return 0;
	return writed;
}
