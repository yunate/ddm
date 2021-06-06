
#include "test_case_factory.h"
#include "thread/async/spin_mutex.h"
#include <thread>
#include <iostream>
BEG_NSP_DDM

TEST(test_spin_mutex, spin_mutex_lock_unlock)
{
    spin_mutex mutex;

    std::thread t1([&mutex] {
        mutex.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        mutex.unlock();
        mutex.lock();
        for (int i = 0; i < 10; ++i) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        mutex.unlock();
    });

    std::thread t2([&mutex] {
        mutex.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        mutex.unlock();
        mutex.lock();
        for (int i = 10; i < 20; ++i) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
        mutex.unlock();
    });

    t1.join();
    t2.join();
}

END_NSP_DDM
