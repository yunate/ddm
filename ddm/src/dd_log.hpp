#include "g_def.h"
#include "ddmacro.hpp"
#include "str\str_utils.h"

#include <iostream>
BEG_NSP_DDM
#define DD_LOG_LEVEL_INFO       1
#define DD_LOG_LEVEL_WARING     2
#define DD_LOG_LEVEL_ERROR      3

inline void dd_log(int level, const char* level_str, const std::string& log)
{
    std::cout << "[" << level_str << "]:" << log;
}
inline void dd_log(int level, const wchar_t* level_str, const std::wstring& log)
{
    std::wcout << L"[" << level_str << L"]:" << log;
}
#define DD_LOG(level, str) { dd_log(DD_LOG_LEVEL_ ## level, #level, str); }
#define DD_LOGW(level, str) { dd_log(DD_LOG_LEVEL_ ## level, L#level, str); }

inline void dd_log(int level, const char* level_str, const char* file, const char* line, const std::string& log)
{
    std::cout << file << ":" << line;
    std::cout << "[" << level_str << "]:" << log;
}
inline void dd_log(int level, const wchar_t* level_str, const wchar_t* file, const wchar_t* line, const std::wstring& log)
{
    std::wcout << file << L":" << line;
    std::wcout << L"[" << level_str << L"]:" << log;
}
#define DD_LOG_EX(level, str) { dd_log(DD_LOG_LEVEL_ ## level, #level, __FILE__, DD_TOSTRING(__LINE__), str); }
#define DD_LOGW_EX(level, str) { dd_log(DD_LOG_LEVEL_ ## level, L#level, DD_CONCAT(L, __FILE__), DD_CONCAT(L, DD_TOSTRING(__LINE__)), str); }
END_NSP_DDM
