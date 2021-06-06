
#include "shared_memory/shared_memory_manager.h"
#include "test_case_factory.h"
#include <iostream>

BEG_NSP_DDM

TEST(test_shared_memory, shared_memory)
{
    auto spSharedMemory = SHARED_MEMORY_MANAGER.create(L"test_shared_memory", 300);
    char* p = (char*)spSharedMemory->get_buff();
    ::memset(p, 0, 101);
    std::wstring ss = L"this is test";
    ::memcpy(p, ss.c_str(), ss.length() * 2);
}

TEST(test_shared_memory, shared_memory1)
{
    auto spSharedMemory = SHARED_MEMORY_MANAGER.create(L"test_shared_memory", 300);
    char* p = (char*)spSharedMemory->get_buff();
    std::wstring ss = (ddcharw*)p;
    ::wprintf(ss.c_str());
}
END_NSP_DDM
