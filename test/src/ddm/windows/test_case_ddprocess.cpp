
#include "test_case_factory.h"
#include "windows/ddprocess.h"


BEG_NSP_DDM

TEST(test_process, get_process_id)
{
    ddprocess p1;
    (void)p1.init(std::wstring(L"notepad.exe"));

    ddprocess p2;
    (void)p2.init(11104);

    ddprocess p3;
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, 11104);
    if (hProcess != NULL) {
        (void)p3.init(hProcess, 0);
    }
}

END_NSP_DDM
