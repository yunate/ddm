
#include "test_case_factory.h"
#include "windows/ddwin_utils.h"

BEG_NSP_DDM
TEST(test_win_utils, last_error_msg)
{
	std::wstring name = get_process_name(-1);
	DDLOG_LASTERROR();
}
TEST(test_win_utils, get_process_id)
{
	std::wstring name = get_process_name(5288);
	std::vector<DWORD> ids;
	if (!get_process_ids(L"notepad.exe", ids)) {
		DDLOG(WARNING, "get_process_ids failure");
	}

	std::wstring fullPath = get_process_fullpath(5288);
	int i = 0;
	++i;
}

END_NSP_DDM
