#include "g_def.h"
#include "ddmacro.hpp"
#include "str\str_utils.h"
#include "ddtimer.h"

#include <iostream>
BEG_NSP_DDM
#define DDLOG_LEVEL_INFO       1
#define DDLOG_LEVEL_WARNING    2
#define DDLOG_LEVEL_ERROR      3

typedef bool (*DDLOG_FILTER)(int level, const std::string& level_str, const std::string& file, const std::string& line, const std::string& log);
extern DDLOG_FILTER g_ddlog_filter;
void set_ddlog_filter(DDLOG_FILTER filter);

void ddlog(int level, const char* level_str, const std::string& log);
void ddlog(int level, const wchar_t* level_str, const std::wstring& log);
#define DDLOG(level, str) { ddlog(DDLOG_LEVEL_ ## level, #level, str); }
#define DDLOGW(level, str) { ddlog(DDLOG_LEVEL_ ## level, L#level, str); }

void ddlog(int level, const char* level_str, const char* file, const char* line, const std::string& log);
void ddlog(int level, const wchar_t* level_str, const wchar_t* file, const wchar_t* line, const std::wstring& log);
#define DDLOG_EX(level, str) { ddlog(DDLOG_LEVEL_ ## level, #level, __FILE__, DD_TOSTRING(__LINE__), str); }
#define DDLOGW_EX(level, str) { ddlog(DDLOG_LEVEL_ ## level, L#level, DD_CONCAT(L, __FILE__), DD_CONCAT(L, DD_TOSTRING(__LINE__)), str); }
END_NSP_DDM
