
#include "windows/ddshared_memory.h"
#include "test_case_factory.h"
#include <iostream>

BEG_NSP_DDM

TEST(test_shared_memory, shared_memory)
{
    ddshared_memory* sm = new(std::nothrow) ddshared_memory();
    if (sm == nullptr || !sm->init(4097, L"test_shared_memory")) {
        return;
    }

    char* p = (char*)sm->get_buff();
    ::memset(p, 0, 101);
    std::wstring ss = L"this is test";
    ::memcpy(p, ss.c_str(), ss.length() * 2);

    while (true)
    {
        ::Sleep(10);
    }
    delete sm;
}

TEST(test_shared_memory, shared_memory1)
{
    return;
    ddshared_memory* sm = new(std::nothrow) ddshared_memory();
    if (sm == nullptr || !sm->init(4097, L"test_shared_memory")) {
        return;
    }
    char* p = (char*)sm->get_buff();
    std::wstring ss = (ddcharw*)p;
    ::wprintf(ss.c_str());
    delete sm;
}
END_NSP_DDM
