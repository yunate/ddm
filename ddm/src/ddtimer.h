
#ifndef ddtimer_h
#define ddtimer_h

#include "g_def.h"
#include <chrono>

BEG_NSP_DDM

// ƒ…√Î
class ddtimer
{
public:
    ddtimer()
    {
        reset();
    }

    ~ddtimer() = default;

    inline u64 now()
    {
        return std::chrono::steady_clock::now().time_since_epoch().count();
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
    //  ±º‰¥¡
    u64 m_epoch = 0;
};

END_NSP_DDM
#endif
