
#include "random.h"
#include "test_case_factory.h"
#include <iostream>

BEG_NSP_DDM

TEST(test_random, random)
{
    std::vector<int> ran(100);
    for (size_t i = 0; i < ran.size(); ++i) {
        random::get_rand_number(0, 10000, ran[i]);
    }

    for (auto it : ran) {
        std::cout << it << std::endl;
    }

    ddstra guida;
    random::gen_guid(guida);
    std::cout << guida << std::endl;

    ddstrw guidw;
    random::gen_guid(guidw);
    std::wcout << guidw << std::endl;
}

END_NSP_DDM
