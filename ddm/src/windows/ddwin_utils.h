#ifndef ddwin_utils_h_
#define ddwin_utils_h_
#include "g_def.h"
#include "ddlog.hpp"
#include "nocopyable.hpp"
#include "str/str_utils.h"
#include <string>
#include <windows.h>
#include <guiddef.h>
BEG_NSP_DDM
#ifdef DD_WINDOW

// 获得 ERROR_CODE 的字符串描述
std::wstring last_error_msgw(DWORD errorCode);
std::string last_error_msga(DWORD errorCode);

// GetLastError 日志记录
#define DDLOG_LASTERROR() {\
    DWORD errorCode = ::GetLastError();\
    std::string errorMsg = str_utils::str_format("GetLastError:%d %s", errorCode, last_error_msga(errorCode).c_str());\
    DDLOG_EX(ERROR, errorMsg);\
}

//////////////////////////////////////GUID////////////////////////////////////
// eg: guidStr:"{431ECE1A-B4EB-4C15-8899-CAB52D13A06A}"
inline bool ddstr_guid(const std::wstring& guidStr, ::GUID& guid)
{
    short d0, d1, d2, d3, d4, d5, d6, d7;
    int cnt = ::swscanf_s(guidStr.c_str(), L"{%08X-%04hX-%04hX-%02hX%02hX-%02hX%02hX%02hX%02hX%02hX%02hX}",
        &(guid.Data1), &(guid.Data2), &(guid.Data3), &d0, &d1, &d2, &d3, &d4, &d5, &d6, &d7);
    guid.Data4[0] = (unsigned char)d0;
    guid.Data4[1] = (unsigned char)d1;
    guid.Data4[2] = (unsigned char)d2;
    guid.Data4[3] = (unsigned char)d3;
    guid.Data4[4] = (unsigned char)d4;
    guid.Data4[5] = (unsigned char)d5;
    guid.Data4[6] = (unsigned char)d6;
    guid.Data4[7] = (unsigned char)d7;
    return cnt == 11;
}

inline bool ddstr_guid(const std::string& guidStr, ::GUID& guid)
{
    short d0, d1, d2, d3, d4, d5, d6, d7;
    int cnt = ::sscanf_s(guidStr.c_str(), "{%08X-%04hX-%04hX-%02hX%02hX-%02hX%02hX%02hX%02hX%02hX%02hX}",
                          &(guid.Data1), &(guid.Data2), &(guid.Data3), &d0, &d1, &d2, &d3, &d4, &d5, &d6, &d7);
    guid.Data4[0] = (unsigned char)d0;
    guid.Data4[1] = (unsigned char)d1;
    guid.Data4[2] = (unsigned char)d2;
    guid.Data4[3] = (unsigned char)d3;
    guid.Data4[4] = (unsigned char)d4;
    guid.Data4[5] = (unsigned char)d5;
    guid.Data4[6] = (unsigned char)d6;
    guid.Data4[7] = (unsigned char)d7;
    return cnt == 11;
}

inline bool ddguid_str(const ::GUID& guid, std::wstring& guidStr)
{
    guidStr.resize(64);
    int cnt = ::swprintf_s(&(guidStr[0]), guidStr.length(), L"{%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
    return cnt == 38;
}

inline bool ddguid_str(const ::GUID& guid, std::string& guidStr)
{
    guidStr.resize(64);
    int cnt = ::sprintf_s(&(guidStr[0]), guidStr.length(), "{%08X-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
    return cnt == 38;
}


class DDHKEY
{
    DD_NO_COPY_MOVE(DDHKEY);
public:
    DDHKEY() = default;
    DDHKEY& operator=(HKEY key)
    {
        if (m_key != NULL) {
            ::RegCloseKey(m_key);
            m_key = NULL;
        }
        m_key = key;
    }
    operator HKEY()
    {
        return m_key;
    }
    HKEY* operator&()
    {
        return &m_key;
    }
    ~DDHKEY()
    {
        if (m_key != NULL) {
            ::RegCloseKey(m_key);
            m_key = NULL;
        }
    }

    HKEY m_key = NULL;
};
#endif
END_NSP_DDM
#endif // ddwin_utils_h_

