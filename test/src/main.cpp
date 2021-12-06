
#include "test_case_factory.h"

BEG_NSP_DDM

int test_main()
{
    TCF.insert_white_type("ddperfermence");
    TCF.run();
    ::system("pause");
    return 0;
}

END_NSP_DDM

int main()
{
    return ddm::test_main();
}

