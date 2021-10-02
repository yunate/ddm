#include "ddsyringe.h"
#include "ddlog.hpp"
#include "str/str_utils.h"
#include "windows/ddwin_utils.h"

bool ddclassic_syringe::inject_dll(HANDLE hProcess, const std::wstring& dllFullPath)
{
	return true;
}

bool ddclassic_syringe::inject_dll(DWORD processID, const std::wstring& dllFullPath)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	if (hProcess == NULL) {
		DDLOG_LASTERROR();
		return false;
	}

	bool result = inject_dll(hProcess, dllFullPath);
	::CloseHandle(hProcess);
	return result;
}

bool ddclassic_syringe::inject_dll(const std::wstring& processName, const std::wstring& dllFullPath)
{
	return false;
}
