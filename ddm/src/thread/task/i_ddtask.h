
#ifndef i_ddtask_h_
#define i_ddtask_h_ 1

#include "g_def.h"
BEG_NSP_DDM

class i_ddtask
{
public:
    virtual ~i_ddtask() {}

    /** ִ������
    */
    virtual void do_task() = 0;

    /** ֹͣ����
    */
    virtual void stop() = 0;
};

END_NSP_DDM
#endif // i_ddtask.h

