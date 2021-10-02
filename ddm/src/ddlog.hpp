#include "g_def.h"
#include "ddmacro.hpp"
#include "str\str_utils.h"
#include "ddtimer.h"

#include <iostream>
BEG_NSP_DDM
#define DDLOG_LEVEL_INFO       1
#define DDLOG_LEVEL_WARNING     2
#define DDLOG_LEVEL_ERROR      3

typedef bool (*DDLOG_FILTER)(int level, const std::string& level_str, const std::string& file, const std::string& line, const std::string& log);
static DDLOG_FILTER g_ddlog_filter = nullptr;
inline void set_ddlog_filter(DDLOG_FILTER filter) { g_ddlog_filter = filter; }

inline void ddlog(int level, const char* level_str, const std::string& log)
{
    if (g_ddlog_filter != nullptr) {
        if (g_ddlog_filter(level, level_str, "", "", log)) {
            return;
        }
    }

    ddtimer::time_info ti;
    ddtimer::now_fmt(ti);
    std::string log_fmt = str_utils::str_format("[%d-%d-%d %d-%d-%d %s]:", ti.year, ti.mon, ti.day, ti.hour, ti.min, ti.sec, level_str);
    log_fmt += log;
    std::cout << log_fmt;
}
inline void ddlog(int level, const wchar_t* level_str, const std::wstring& log)
{
    if (g_ddlog_filter != nullptr) {
        std::string levelStra;
        std::string loga;
        str_utils::uft16_uft8(level_str, levelStra);
        str_utils::uft16_uft8(log, loga);
        if (g_ddlog_filter(level, levelStra, "", "", loga)) {
            return;
        }
    }

    ddtimer::time_info ti;
    ddtimer::now_fmt(ti);
    std::wstring log_fmt = str_utils::str_format(L"[%d-%d-%d %d-%d-%d %s]:", ti.year, ti.mon, ti.day, ti.hour, ti.min, ti.sec, level_str);
    log_fmt += log;
    std::wcout << log_fmt;
}
#define DDLOG(level, str) { ddlog(DDLOG_LEVEL_ ## level, #level, str); }
#define DDLOGW(level, str) { ddlog(DDLOG_LEVEL_ ## level, L#level, str); }

inline void ddlog(int level, const char* level_str, const char* file, const char* line, const std::string& log)
{
    if (g_ddlog_filter != nullptr) {
        if (g_ddlog_filter(level, level_str, "", "", log)) {
            return;
        }
    }

    std::string log_fmt = file;
    log_fmt += ":";
    log_fmt += line;
    log_fmt += " ";
    log_fmt += log;
    ddlog(level, level_str, log_fmt);
}
inline void ddlog(int level, const wchar_t* level_str, const wchar_t* file, const wchar_t* line, const std::wstring& log)
{
    if (g_ddlog_filter != nullptr) {
        std::string levelStra;
        std::string loga;
        std::string filea;
        std::string linea;
        str_utils::uft16_uft8(level_str, levelStra);
        str_utils::uft16_uft8(log, loga);
        str_utils::uft16_uft8(file, filea);
        str_utils::uft16_uft8(line, linea);
        if (g_ddlog_filter(level, levelStra, filea, linea, loga)) {
            return;
        }
    }

    std::wstring log_fmt = file;
    log_fmt += L":";
    log_fmt += line;
    log_fmt += L" ";
    log_fmt += log;
    ddlog(level, level_str, log_fmt);
}
#define DDLOG_EX(level, str) { ddlog(DDLOG_LEVEL_ ## level, #level, __FILE__, DD_TOSTRING(__LINE__), str); }
#define DDLOGW_EX(level, str) { ddlog(DDLOG_LEVEL_ ## level, L#level, DD_CONCAT(L, __FILE__), DD_CONCAT(L, DD_TOSTRING(__LINE__)), str); }
END_NSP_DDM
