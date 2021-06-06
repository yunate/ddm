#pragma once

#pragma once

#include "service_utils.h"

namespace utilstest
{
    static void Test_WinApiCaller0()
    {
        // 安装服务
        wchar_t * binPath = L"client_svhost.exe --start";
        ServiceInstall::InstallServices(L"test"
            , L"test"
            , binPath
            , SERVICE_WIN32_OWN_PROCESS
            , SERVICE_AUTO_START);
    }
}