
#ifndef ddtimer_h
#define ddtimer_h

#include "g_def.h"
#include <chrono>
#include <thread>

#ifdef DD_WINDOW
#pragma comment(lib, "Winmm.lib")
#include <windows.h>
#endif // _WINDOWS


BEG_NSP_DDM

// 纳秒
class ddtimer
{
public:
    ddtimer()
    {
        reset();
    }

    ~ddtimer() = default;

    // 纳秒
    static inline u64 now()
    {
        return std::chrono::steady_clock::now().time_since_epoch().count();
    }

    // 毫秒
    static inline u64 now_ms()
    {
        auto dur = std::chrono::high_resolution_clock::now().time_since_epoch();
        return (u64)std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    }

    // sleep_time 单位毫秒
    // period windows下要设置精度，win10 下默认精度是 15ms,也就是说Sleep(1),也睡15ms
    static inline void sleep(u32 sleep_time, u32 period = 1)
    {
#ifdef DD_WINDOW
        if (::timeBeginPeriod(period) == TIMERR_NOCANDO) {
            period = 1;
            ::timeBeginPeriod(period);
        }
#endif

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));

#ifdef DD_WINDOW
        ::timeEndPeriod(period);
#endif
    }

    void reset()
    {
        m_epoch = now();
    }

    u64 get_time_pass()
    {
        return now() - m_epoch;
    }

private:
    // 时间戳
    u64 m_epoch = 0;
};

END_NSP_DDM
#endif
