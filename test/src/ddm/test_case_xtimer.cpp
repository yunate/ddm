
#include "test_case_factory.h"
#include "ddtimer.h"

#include <iostream>
#include <thread>

BEG_NSP_DDM
TEST(test_xtimer, get_time_pass)
{
    ddtimer timer;

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    timer.get_time_pass();
    std::cout << timer.get_time_pass() / 1000 / 1000 << std::endl;
}

END_NSP_DDM
