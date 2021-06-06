#ifndef resource_util_h_
#define resource_util_h_ 1

#include "g_def.h"
#include <windows.h>
BEG_NSP_DDM

class resource_util
{
    // 释放资源文件
    // hModule ： 模块句柄，比如wWinMain( _In_ HINSTANCE hInstance,...) 的hInstance
    // target ： 保存路径
    bool release_res(HMODULE hModule, DWORD resId, const ddstr& resTy, const ddstr& target);
};

END_NSP_DDM
#endif // resource_util_h_

