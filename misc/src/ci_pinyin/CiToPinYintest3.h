#pragma once

#include "CiToPinYin.h"
#include "base/singleton.hpp"

namespace utilstest
{
    static void Test_StrToPinYin()
    {
        std::vector<std::wstring> out;
        singleton<CiToPinYin>::get_instance().GetStrPinYin(L"ÐÐºÍËµ", out);
    }
   
}
