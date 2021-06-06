
#ifndef i_ddtask_h_
#define i_ddtask_h_ 1

#include "g_def.h"
BEG_NSP_DDM

class i_ddtask
{
public:
    virtual ~i_ddtask() {}

    /** 执行任务
    */
    virtual void do_task() = 0;

    /** 停止任务
    */
    virtual void stop() = 0;
};

END_NSP_DDM
#endif // i_ddtask.h

