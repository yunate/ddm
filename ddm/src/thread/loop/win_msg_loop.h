
#ifndef win_msg_loop_h_
#define win_msg_loop_h_ 1
#include "i_loop.h"
#include <windows.h>
BEG_NSP_DDM

class win_msg_loop : public i_loop
{
public:
    win_msg_loop() = default;
    virtual ~win_msg_loop() = default;

public:
    virtual bool stop(u32 waitTime);
    virtual void loop();
    virtual bool filter_msg(MSG* msg); // 返回false则该条消息被过滤

protected:
    bool m_stop = false;
    MSG m_msg {0};
    u32 m_loopId = 0;
};

END_NSP_DDM
#endif // win_msg_loop_h_
#pragma once
