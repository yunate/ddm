
#ifndef i_loop_h_
#define i_loop_h_ 1
#include "g_def.h"
#include "nocopyable.hpp"
#include "../async/simple_event.h"

BEG_NSP_DDM

class i_loop
{
    DD_NO_COPY_MOVE(i_loop);
public:
    i_loop() = default;
    virtual ~i_loop() = default;
    virtual void loop() = 0;
    virtual bool stop(u32 waitTime) = 0;
};
END_NSP_DDM
#endif // i_loop_h_
#pragma once
