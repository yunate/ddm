#include "g_def.h"
#include "ddmacro.hpp"
#include "str\str_utils.h"
#include "ddtimer.h"

#include <iostream>
BEG_NSP_DDM
#define DD_LOG_LEVEL_INFO       1
#define DD_LOG_LEVEL_WARING     2
#define DD_LOG_LEVEL_ERROR      3

inline void dd_log(int level, const char* level_str, const std::string& log)
{
    ddtimer::time_info ti;
    ddtimer::now_fmt(ti);
    std::string log_fmt = str_utils::str_format("[%d-%d-%d %d-%d-%d %s]:", ti.year, ti.mon, ti.day, ti.hour, ti.min, ti.sec, level_str);
    log_fmt += log;
    std::cout << log_fmt;
}
inline void dd_log(int level, const wchar_t* level_str, const std::wstring& log)
{
    ddtimer::time_info ti;
    ddtimer::now_fmt(ti);
    std::wstring log_fmt = str_utils::str_format(L"[%d-%d-%d %d-%d-%d %s]:", ti.year, ti.mon, ti.day, ti.hour, ti.min, ti.sec, level_str);
    log_fmt += log;
    std::wcout << log_fmt;
}
#define DD_LOG(level, str) { dd_log(DD_LOG_LEVEL_ ## level, #level, str); }
#define DD_LOGW(level, str) { dd_log(DD_LOG_LEVEL_ ## level, L#level, str); }

inline void dd_log(int level, const char* level_str, const char* file, const char* line, const std::string& log)
{
    std::string log_fmt = file;
    log_fmt += ":";
    log_fmt += line;
    log_fmt += " ";
    log_fmt += log;
    dd_log(level, level_str, log_fmt);
}
inline void dd_log(int level, const wchar_t* level_str, const wchar_t* file, const wchar_t* line, const std::wstring& log)
{
    std::wstring log_fmt = file;
    log_fmt += L":";
    log_fmt += line;
    log_fmt += L" ";
    log_fmt += log;
    dd_log(level, level_str, log_fmt);
}
#define DD_LOG_EX(level, str) { dd_log(DD_LOG_LEVEL_ ## level, #level, __FILE__, DD_TOSTRING(__LINE__), str); }
#define DD_LOGW_EX(level, str) { dd_log(DD_LOG_LEVEL_ ## level, L#level, DD_CONCAT(L, __FILE__), DD_CONCAT(L, DD_TOSTRING(__LINE__)), str); }
END_NSP_DDM
