#ifndef ddwin_utils_h_
#define ddwin_utils_h_
#include "g_def.h"
#include "ddlog.hpp"
#include "str/str_utils.h"
#include <string>
#include <windows.h>

BEG_NSP_DDM
#ifdef DD_WINDOW

// 获得 ERROR_CODE 的字符串描述
std::wstring last_error_msgw(DWORD errorCode);
std::string last_error_msga(DWORD errorCode);

// GetLastError 日志记录
#define DDLOG_LASTERROR() {\
	DWORD errorCode = ::GetLastError();\
	std::string errorMsg = str_utils::str_format("GetLastError:%d %s", errorCode, last_error_msga(errorCode).c_str());\
	DDLOG(ERROR, errorMsg);\
}

// 进程名称枚举所有的进程id
// processName 进程名称, 比如 notepad.exe
bool get_process_ids(const std::wstring& processName, std::vector<DWORD>& ids);

// 根据 进程id获得进程名
// 进程名, 比如 notepad.exe
std::wstring get_process_name(DWORD processId);

// 根据进程 id 获得进程全路径
std::wstring get_process_fullpath(DWORD processId);

#endif
END_NSP_DDM
#endif // ddwin_utils_h_

