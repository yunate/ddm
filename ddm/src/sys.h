
#ifndef cpu_thread_count_h_
#define cpu_thread_count_h_ 1

#include "g_def.h"
#include <thread>
#ifdef DD_WINDOW
#include <windows.h>
#endif

BEG_NSP_DDM

u32 get_thread_count() 
{

    // ͨ��std�ṩ�ķ�����ȡ
    u32 count = std::thread::hardware_concurrency();

    if (count != 0) {
        return count;
    }

#ifdef DD_WINDOW
    // �����ȡ�������ٵ���winapi���Ի�ȡ
    ::SYSTEM_INFO info;
    ::GetSystemInfo(&info);
    count = (u32)info.dwNumberOfProcessors;

    // ������ǻ�ȡ���������� 
    // WMIC CPU Get NumberOfCores,NumberOfLogicalProcessors /Format:List > ./cpucore.txt 
    // �����ȡ�����������Ϊ����������Ҫ��ʼ��WMIC����Ҫ�漰�ļ���д�����������⣬���ԾͲ�ȥ������

    if (count != 0) {
        return count;
    }
#endif

    // ������ǻ�ȡ������ֱ�ӷ���1
    return 1;
}

END_NSP_DDM
#endif // cpu_thread_count_h_
