#include "ddwin_utils.h"

#include <tlhelp32.h>
BEG_NSP_DDM
#ifdef DD_WINDOW

std::wstring last_error_msgw(DWORD errorCode)
{
    HLOCAL buff = NULL;
    ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        errorCode,
        0,
        (LPWSTR)&buff,
        0,
        NULL);

    std::wstring msg = (LPWSTR)buff;
    ::LocalFree(buff);
	return msg;
}
std::string last_error_msga(DWORD errorCode)
{
    HLOCAL buff = NULL;
    ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        errorCode,
        0,
        (LPSTR)&buff,
        0,
        NULL);

    std::string msg = (LPSTR)buff;
    ::LocalFree(buff);
    return msg;
}

bool get_process_ids(const std::wstring& processName, std::vector<DWORD>& ids)
{
    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) {
        DDLOG_LASTERROR();
        return false;
    }
    ::PROCESSENTRY32 pe = { sizeof(pe) };
    BOOL flag = ::Process32FirstW(hSnapshot, &pe);
    while (flag) {
        if (processName == pe.szExeFile) {
            ids.push_back(pe.th32ProcessID);
        }
        flag = ::Process32NextW(hSnapshot, &pe);
    }

    ::CloseHandle(hSnapshot);
    return true;
}

std::wstring get_process_name(DWORD processId)
{
    std::wstring processName;
    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) {
        DDLOG_LASTERROR();
        return processName;
    }
    ::PROCESSENTRY32 pe = { sizeof(pe) };
    BOOL flag = ::Process32FirstW(hSnapshot, &pe);
    while (flag) {
        if (processId == pe.th32ProcessID) {
            processName = pe.szExeFile;
            break;
        }
        flag = ::Process32NextW(hSnapshot, &pe);
    }

    ::CloseHandle(hSnapshot);
    return processName;
}

std::wstring get_process_fullpath(DWORD processId)
{
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        DDLOG_LASTERROR();
        return L"";
    }

    WCHAR fullPath[MAX_PATH + 1];
    DWORD buffSize = ARRAYSIZE(fullPath) - 1;
    if (0 == ::QueryFullProcessImageNameW(hProcess, 0, fullPath, &buffSize)) {
        DDLOG_LASTERROR();
        return L"";
    }
    return fullPath;
}

#endif
END_NSP_DDM
