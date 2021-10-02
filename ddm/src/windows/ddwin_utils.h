#ifndef ddwin_utils_h_
#define ddwin_utils_h_
#include "g_def.h"
#include "ddlog.hpp"
#include "str/str_utils.h"
#include <string>
#include <windows.h>

BEG_NSP_DDM
#ifdef DD_WINDOW

// ��� ERROR_CODE ���ַ�������
std::wstring last_error_msgw(DWORD errorCode);
std::string last_error_msga(DWORD errorCode);

// GetLastError ��־��¼
#define DDLOG_LASTERROR() {\
	DWORD errorCode = ::GetLastError();\
	std::string errorMsg = str_utils::str_format("GetLastError:%d %s", errorCode, last_error_msga(errorCode).c_str());\
	DDLOG(ERROR, errorMsg);\
}

// ��������ö�����еĽ���id
// processName ��������, ���� notepad.exe
bool get_process_ids(const std::wstring& processName, std::vector<DWORD>& ids);

// ���� ����id��ý�����
// ������, ���� notepad.exe
std::wstring get_process_name(DWORD processId);

// ���ݽ��� id ��ý���ȫ·��
std::wstring get_process_fullpath(DWORD processId);

#endif
END_NSP_DDM
#endif // ddwin_utils_h_

