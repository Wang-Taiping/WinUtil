#include "WinPECheck.h"
#include <Windows.h>
#include <regex>

ubool WinPECheck()
{
	CHAR szSystemStartOptions[256]{};
	DWORD strBytes = 256;
	if (RegGetValueA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control", "SystemStartOptions", RRF_RT_REG_SZ, nullptr, szSystemStartOptions, &strBytes) != ERROR_SUCCESS) return false;
	std::regex MiniNT("^.*MININT.*$", std::regex_constants::icase);
	return std::regex_match(szSystemStartOptions, MiniNT);
}
