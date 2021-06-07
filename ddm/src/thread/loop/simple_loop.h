
#ifndef simple_loop_h_
#define simple_loop_h_ 1
#include "i_loop.h"
#include "ddmath.h"
BEG_NSP_DDM

class simple_loop : public i_loop
{
public:
    simple_loop() = default;
    virtual ~simple_loop();

public:
    virtual bool stop(u32 waitTime);

    /** 等待循环停止
    @param 等待循环结束等待事件，MAX_U32表示永不超时
    @return 如果超时返回false，否则true
    */
    bool wait_loop_end(u32 waitTime = MAX_U32);
    virtual void loop();

protected:
    virtual void loop_core() = 0;

protected:
    std::recursive_mutex m_mutex;
    bool m_stop = false;
    std::thread::id m_loopId;

private:
    simple_event m_loopEndEvent;
    bool m_has_loop_end = false;
    bool m_has_loop_beg = false;
};

END_NSP_DDM
#endif // simple_loop_h_
#pragma once
