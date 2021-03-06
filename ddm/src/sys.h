
#ifndef cpu_thread_count_h_
#define cpu_thread_count_h_ 1

#include "g_def.h"
#include <thread>
#ifdef DD_WINDOW
#include <windows.h>
#endif

BEG_NSP_DDM

inline u32 get_thread_count() 
{

    // 通过std提供的方法获取
    u32 count = std::thread::hardware_concurrency();

    if (count != 0) {
        return count;
    }

#ifdef DD_WINDOW
    // 如果获取不到，再调用winapi尝试获取
    ::SYSTEM_INFO info;
    ::GetSystemInfo(&info);
    count = (u32)info.dwNumberOfProcessors;

    // 如果还是获取不到，尝试 
    // WMIC CPU Get NumberOfCores,NumberOfLogicalProcessors /Format:List > ./cpucore.txt 
    // 命令获取，但是这儿因为这个命令可能要初始化WMIC，还要涉及文件读写而引入其他库，所以就不去尝试了

    if (count != 0) {
        return count;
    }
#endif

    // 如果还是获取不到，直接返回1
    return 1;
}

END_NSP_DDM
#endif // cpu_thread_count_h_
