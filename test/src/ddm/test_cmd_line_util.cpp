
#include "cmd_line_util.h"

#include "test_case_factory.h"

#include <iostream>

BEG_NSP_DDM
TEST(test_cmd_line_util, get_cmds)
{
    std::vector<ddstr> cmds;
    get_cmds(cmds);
}
END_NSP_DDM
