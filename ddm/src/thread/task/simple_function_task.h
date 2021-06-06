
#ifndef simple_function_task_h_
#define simple_function_task_h_ 1

#include "g_def.h"
#include "i_ddtask.h"
#include <functional>

BEG_NSP_DDM
class simple_function_task :
    public i_ddtask
{
public:
    simple_function_task(const std::function<void()>& callback) :
        m_callback(callback)
    {}

    virtual ~simple_function_task() {}

    /** ִ������
    */
    virtual void do_task()
    {
        if (m_callback != nullptr) {
            (void)m_callback();
        }
    }

    /** ֹͣ����
    */
    virtual void stop() {}

private:
    /** �ص�
    */
    std::function<void()> m_callback;
};

END_NSP_DDM
#endif // simple_function_task_h_

