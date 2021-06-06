
#ifndef spin_mutex_h_
#define spin_mutex_h_ 1
#include "g_def.h"
#include <atomic>

// #define USE_ASM_CPUPAUSE
#ifdef USE_ASM_CPUPAUSE
    #ifdef _WIN64 
        extern "C" void cpu_pause_asm();
        #define cpu_pause cpu_pause_asm();
    #else
        #define cpu_pause __asm{pause}
    #endif
#else
    #include <windows.h>
    #define cpu_pause ::Sleep(0);
#endif

BEG_NSP_DDM
/** 自旋锁
*/
class spin_mutex
{
public:
    /** 构造函数
    */
    spin_mutex() {}

    ~spin_mutex() {}

    /** 不允许复制
    */
    spin_mutex(spin_mutex&&) = delete;
    spin_mutex(const spin_mutex&) = delete;
    spin_mutex& operator=(const spin_mutex&&) = delete;

public:
    void lock()
    {
        // test_and_set 返回m_falg的设置前的值
        while (m_flag.test_and_set() != false) {
            cpu_pause
        }
    }

    void unlock() {
        m_flag.clear();
    }

private:
    /** 自旋锁标记
    */
    std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
};

END_NSP_DDM
#endif // spin_mutex_h_
