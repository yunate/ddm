#ifndef ddwindows_version_h_
#define ddwindows_version_h_
#include "ddversion.h"
#include <Windows.h>

#pragma comment(lib, "Version.lib")
BEG_NSP_DDM

#define DDVERSION_MIN_WINXP ddversion(5, 1, 0, 0)
#define DDVERSION_MIN_WIN7 ddversion(6, 1, 0, 0)
#define DDVERSION_MIN_WIN8 ddversion(6, 3, 0, 0)
#define DDVERSION_MIN_WIN10 ddversion(10, 0, 0, 0)

inline ddversion DDGetWindowsVersion()
{
    static ddversion windowsVersion = DDEMPTY_VERSION;
    if (!windowsVersion.empty()) {
        return windowsVersion;
    }

    ddversion tmpVersion(DDEMPTY_VERSION);
    VS_FIXEDFILEINFO* pvs;
    u32 infoSize = ::GetFileVersionInfoSize(L"kernel32", NULL);
    std::vector<u8> data(infoSize);
    if (!GetFileVersionInfo(L"kernel32", 0, infoSize, (LPVOID)data.data())) {
        return DDEMPTY_VERSION;
    }

    if (!VerQueryValue(data.data(), L"\\", (LPVOID*)&pvs, &infoSize)) {
        return DDEMPTY_VERSION;
    }

    tmpVersion = ddversion((u32)HIWORD(pvs->dwFileVersionMS), (u32)HIWORD(pvs->dwFileVersionMS), (u32)HIWORD(pvs->dwFileVersionLS), (u32)LOWORD(pvs->dwFileVersionLS));
    if (tmpVersion[0] == 10) {
        HKEY key;
        if (::RegOpenKeyA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", &key) != ERROR_SUCCESS) {
            return DDEMPTY_VERSION;
        }

        DWORD version;
        DWORD size = sizeof(DWORD);
        if (::RegQueryValueExA(key, "UBR", NULL, NULL, (LPBYTE)&version, &size) == ERROR_SUCCESS) {
            if ((u32)version > tmpVersion[3]) {
                tmpVersion[3] = (u32)version;
            }
        }
        ::RegCloseKey(key);
    }

    windowsVersion = tmpVersion;
    return windowsVersion;
}

END_NSP_DDM
#endif

