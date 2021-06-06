#pragma once

#include "win_registry_key.h"

namespace utilstest
{
    static void Test_WinRegistryKey0()
    {
        std::string sSubPath = "SOFTWARE\\dd\\test";
        std::string sKeyName = "test";

        WinRegistryKey winRegistryKey(HKEY_CURRENT_USER, sSubPath, true);

        if (!winRegistryKey.exists() || !winRegistryKey.exists(sKeyName))
        {
            return ;
        }

        std::string ss = winRegistryKey.getString(sKeyName);
    }

    static void Test_WinRegistryKey1()
    {
        std::string sSubPath = "SOFTWARE\\dd\\test";
        std::string sKeyName = "test";

        WinRegistryKey winRegistryKey(HKEY_CURRENT_USER, sSubPath, false);
        winRegistryKey.setString(sKeyName, "123");
    }
}
