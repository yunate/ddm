
#include "single_limited.h"

#include "test_case_factory.h"

#include <iostream>

BEG_NSP_DDM

TEST(test_single_limted, auto_limted)
{
    {
        single_limited limted;

        if (limted.try_hold_mutex(L"limted")) {
            std::cout << "holded" << std::endl;
        }

        if (!limted.try_hold_mutex(L"limted")) {
            std::cout << "not holded" << std::endl;
        }
    }

    {
        single_limited limted;

        if (limted.try_hold_mutex(L"limted")) {
            std::cout << "holded" << std::endl;
        }

        if (!limted.try_hold_mutex(L"limted")) {
            std::cout << "not holded" << std::endl;
        }
    }
}
END_NSP_DDM
