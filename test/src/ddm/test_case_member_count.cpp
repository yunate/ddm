
#include "member_count.hpp"
#include "test_case_factory.h"

#include <iostream>

BEG_NSP_DDM
TEST(test_member_count, member_count1)
{
    struct Test { int a; int b; int c; int d; };
    int x = member_count<Test>::value;
    DD_ASSERT(x == 4);
}
END_NSP_DDM
