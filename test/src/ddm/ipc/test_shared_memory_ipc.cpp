
#include "test_case_factory.h"

#include <iostream>
#include <thread>
#include <windows.h>
#include "ipc/shared_memory_ipc.h"
#include "thread/thread_manager/simple_thread_manager.h"

BEG_NSP_DDM
TEST(test_event, event1)
{
    HANDLE event1 = ::CreateEvent(NULL, false, false, L"test_1");
    if (event1 == NULL) {
        return;
    }

    (void)::SetEvent(event1);
    (void)::SetEvent(event1);

    int count = 0;
    while (WAIT_OBJECT_0 == ::WaitForSingleObject(event1, INFINITE))
    {
        ++count;
    }
}

TEST(simplex_sm_ipc, client)
{
    return;
    simple_thread_manager thread_manager;
    shared_memory_ipc_client client;
    if (!client.create(_DDT("simplex_sm_ipc"), 1024 * 1024)) {
        return;
    }

    u32 send_count = 0;
    u32 count = 0;
    while (count < 100)
    {
        thread_manager.push_fast_task([&client, count, &send_count]() {
            std::string val = std::to_string(count);
            (void)client.send(val.c_str(), (u32)val.length());
            ++send_count;
            return true;
        });
        ++count;
        ::Sleep(10);
    }

    while (send_count != count)
    {
        ::Sleep(10);
    }
}

TEST(simplex_sm_ipc, server)
{
    simple_thread_manager thread_manager;
    shared_memory_ipc_server server;
    if (!server.create(_DDT("simplex_sm_ipc"), 1024 * 1024)) {
        return;
    }

    ddbuff buff;
    while (server.recv(buff))
    {
        std::string str(buff.data(), buff.size());
        std::cout << str << std::endl;
    }
}
END_NSP_DDM
