
#include "test_case_factory.h"
#include "ddversion.h"
#include "windows/ddwindows_version.h"
#include <iostream>
BEG_NSP_DDM

void TestVersion(const ddversion& l, const ddversion& r)
{
    bool b1 = (l > r);
    bool b2 = (l < r);
    bool b3 = (l >= r);
    bool b4 = (l <= r);
    std::cout << b1 << std::endl;
    std::cout << b2 << std::endl;
    std::cout << b3 << std::endl;
    std::cout << b4 << std::endl;
    std::cout << std::endl;
}

TEST(testddversion, ddversion1)
{
    {
        ddversion windowsVersion = DDGetWindowsVersion();
        if (windowsVersion.empty()) {
            std::cout << "获取版本失败" << std::endl;
        }
        else {
            std::cout << windowsVersion.str() << std::endl;
        }
    }

    {
        ddversion windowsVersion = DDGetWindowsVersion();
        if (windowsVersion.empty()) {
            std::cout << "获取版本失败" << std::endl;
        }
        else {
            std::cout << windowsVersion.str() << std::endl;
        }
    }

    {
        ddversion v1(1, 2, 3, 4);
        ddversion v2(1, 2, 3, 4);
        TestVersion(v1, v2);
    }

    {
        ddversion v1(1, 3, 3, 4);
        ddversion v2(1, 2, 3, 4);
        TestVersion(v1, v2);
    }

    {
        ddversion v1(1, 2, 3, 4);
        ddversion v2(1, 3, 3, 4);
        TestVersion(v1, v2);
    }

    {
        ddversion v1(1, 2, 3, 4);
        ddversion v2(2, 3, 4, 5);
        TestVersion(v1, v2);
    }
}
END_NSP_DDM
