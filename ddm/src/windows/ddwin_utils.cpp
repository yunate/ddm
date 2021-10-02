#include "ddwin_utils.h"

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

#endif
END_NSP_DDM
