

#include "thread/thread_manager/simple_thread_manager.h"
#include "test_case_factory.h"
#include <iostream>
#include <atomic>
BEG_NSP_DDM
TEST(test_thread, simple_task_thread_long_time)
{
    std::atomic_int count = 0;
    simple_task_thread taskThread;
    taskThread.start();

    for (int i = 0; i < 30; ++i) {
        auto& it = taskThread.get_task_queue();
        it.push_task([&count, i]() {
            std::cout << "push_heartbeat_task" << i << std::endl;
            ++count;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            return true;
        });
    }

    while (true)
    {
        if (count == 30) {
            break;
        }

        // ::Sleep(10);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    taskThread.stop();
    taskThread.stop();
    taskThread.stop();
}


TEST(test_thread, simple_task_queue)
{
    simple_task_queue task_que;
    for (int i = 0; i < 30; ++i) {
        task_que.push_task([i]() {
            std::cout << "push_heartbeat_task" << i << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            return true;
        });
    }
    task_que.push_task([&task_que]() {
        // task_que.stop(MAX_U32); //  打开这行试试
        task_que.stop(0);
    });
    task_que.loop();
}

TEST(test_thread, heartbeat_task_thread)
{
    std::atomic_int count = 0;
    heartbeat_task_thread taskThread;
    taskThread.start();

    taskThread.get_task_queue().push_task([&count]() {
        std::cout << "push_heartbeat_task1" << std::endl;
        ++count;
        return true;
    }, 1000, 10);
    taskThread.get_task_queue().push_task([&count]() {
        std::cout << "push_heartbeat_task2" << std::endl;
        ++count;
        return true;
    }, 1000, 10);
    taskThread.get_task_queue().push_task([&count]() {
        std::cout << "push_heartbeat_task3" << std::endl;
        ++count;
        return true;
     }, 1000, 10);
    while (true)
    {
        if (count == 30) {
            break;
        }

        // ::Sleep(10);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    taskThread.stop();
    taskThread.stop();
    taskThread.stop();
}

TEST(test_thread, simple_task_thread)
{
    std::atomic_int count = 0;
    simple_task_thread taskThread;
    taskThread.start();

    for (int i = 0; i < 30; ++i) {
        auto& it = taskThread.get_task_queue();
        it.push_task([&count, i]() {
            std::cout << "push_heartbeat_task" << i << std::endl;
            ++count;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            return true;
        });
    }
    
    while (true)
    {
        if (count == 30) {
            break;
        }

        // ::Sleep(10);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    taskThread.stop();
    taskThread.stop();
    taskThread.stop();
}

TEST(test_thread, simple_thread_manager)
{
    std::atomic_int count = 0;
    simple_thread_manager threadManager;
    threadManager.push_block_task([&count]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        std::cout << "push_block_task" << std::endl;
        ++count;
        return true;
    });

    threadManager.push_fast_task([&count]() {
        std::cout << "push_fast_task" << std::endl;
        ++count;
        return true;
    });

    threadManager.push_heartbeat_task([&count]() {
        std::cout << "push_heartbeat_task" << std::endl;
        ++count;
        return true;
    }, 1000, 3);

    while (true)
    {
        if (count == 5) {
            break;
        }

        // ::Sleep(10);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

END_NSP_DDM

