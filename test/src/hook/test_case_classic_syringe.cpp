
#include "test_case_factory.h"
#include "ddsyringe.h"

BEG_NSP_DDM

TEST(test_classic_syringe, test_classic_syringe)
{
    auto process = std::make_shared<ddprocess>();
    process->init(L"notepad.exe");
    ddclassic_syringe syringe(process, L"D:\\my\\ddm\\bin\\Debug_x64\\test_syringe.dll");
    syringe.inject_dll();
    syringe.uninject_dll();
}


END_NSP_DDM
