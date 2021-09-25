
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

// ����

static const auto g_system_steady = std::chrono::system_clock::now().time_since_epoch() - std::chrono::steady_clock::now().time_since_epoch();
class ddtimer
{
public:
    ddtimer()
    {
        reset();
    }

    ~ddtimer() = default;

    // ����
    static inline u64 now()
    {
        return (std::chrono::steady_clock::now().time_since_epoch() + g_system_steady).count();
    }

    // ����
    static inline u64 now_ms()
    {
        auto dur = std::chrono::system_clock::now().time_since_epoch();
        return (u64)std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    }

    struct time_info
    {
        // ������ʱ����
        u32 year;
        u32 mon;
        u32 day;
        u32 hour;
        u32 min;
        u32 sec;
        u32 milli;
        u32 micro;
        u32 nano;

        // �ܼ�
        u32 weekday;

        // һ��ĵڼ���
        u32 yearday;
    };

    static inline void now_fmt(time_info& ti)
    {
        std::chrono::system_clock::time_point n = std::chrono::system_clock::now();
        time_fmt(std::chrono::system_clock::now().time_since_epoch().count(), 10000000, ti);
    }

    static inline void ms_fmt(u64 ms_epoch, time_info& ti)
    {
        time_fmt(ms_epoch, 1000, ti);
    }

    static inline void micro_fmt(u64 micro_epoch, time_info& ti)
    {
        time_fmt(micro_epoch, 1000000, ti);
    }

    static inline void nano_fmt(u64 nano_epoch, time_info& ti)
    {
        time_fmt(nano_epoch, 1000000000, ti);
    }

    static inline void time_fmt(u64 epoch, u64 to_sec, time_info& ti)
    {
        time_t tt = epoch / to_sec;
        static::tm local_tm;
        (void)::localtime_s(&local_tm, &tt);
        ti.year = local_tm.tm_year + 1900;
        ti.mon = local_tm.tm_mon + 1;
        ti.day = local_tm.tm_mday;
        ti.hour = local_tm.tm_hour;
        ti.min = local_tm.tm_min;
        ti.sec = local_tm.tm_sec;
        u64 timeepoch = epoch * (1000000000 / to_sec);
        ti.milli = (timeepoch % 1000000000) / 1000000;
        ti.micro = (timeepoch % 1000000) / 1000;
        ti.nano = (timeepoch % 1000);
        ti.weekday = local_tm.tm_wday + 1;
        ti.yearday = local_tm.tm_yday + 1;
    }

    // sleep_time ��λ����
    // period windows��Ҫ���þ��ȣ�win10 ��Ĭ�Ͼ����� 15ms,Ҳ����˵Sleep(1),Ҳ˯15ms
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
    // ʱ���
    u64 m_epoch = 0;
};

END_NSP_DDM
#endif
