#include "ddlog.hpp"
BEG_NSP_DDM
DDLOG_FILTER g_ddlog_filter = nullptr;
void set_ddlog_filter(DDLOG_FILTER filter) { g_ddlog_filter = filter; }
void ddlog(int level, const char* level_str, const std::string& log)
{
    if (g_ddlog_filter != nullptr) {
        if (g_ddlog_filter(level, level_str, "", "", log)) {
            return;
        }
    }

    ddtimer::time_info ti;
    ddtimer::now_fmt(ti);
    std::string log_fmt = str_utils::str_format("[%04d-%02d-%02d %02d-%02d-%02d %s]:%s\r\n", ti.year, ti.mon, ti.day, ti.hour, ti.min, ti.sec, level_str, log.c_str());
    std::cout << log_fmt;
}
void ddlog(int level, const wchar_t* level_str, const std::wstring& log)
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
    std::wstring log_fmt = str_utils::str_format(L"[%04d-%02d-%02d %02d-%02d-%02d %s]:%s\r\n", ti.year, ti.mon, ti.day, ti.hour, ti.min, ti.sec, level_str, log.c_str());
    std::wcout << log_fmt;
}
void ddlog(int level, const char* level_str, const char* file, const char* line, const std::string& log)
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
void ddlog(int level, const wchar_t* level_str, const wchar_t* file, const wchar_t* line, const std::wstring& log)
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


END_NSP_DDM
