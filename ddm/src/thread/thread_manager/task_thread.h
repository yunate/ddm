
#ifndef task_thread_h_
#define task_thread_h_ 1

#include "../task/simple_task_queue.h"
#include "../task/heartbeat_task_queue.h"
#include <thread>
BEG_NSP_DDM

/** 简单线程任务模型
@note 有点像生产者消费者模型，只是消费者只有一个
*/
template<class TASK_QUEUE>
class task_thread :
    public nocopyable
{
public:
    /** 构造函数
    */
    task_thread();

    /** 析构函数
    */
    ~task_thread();
public:
    TASK_QUEUE& get_task_queue();
    bool start();
    void stop();
private:
    TASK_QUEUE m_task_que;
    std::thread* m_thread = nullptr;
};

template<class TASK_QUEUE>
task_thread<TASK_QUEUE>::task_thread()
{}

template<class TASK_QUEUE>
task_thread<TASK_QUEUE>::~task_thread()
{
    (void)stop();
}

template<class TASK_QUEUE>
TASK_QUEUE& task_thread<TASK_QUEUE>::get_task_queue()
{
    return m_task_que;
}

template<class TASK_QUEUE>
bool task_thread<TASK_QUEUE>::start()
{
    m_thread = new(std::nothrow)std::thread(&TASK_QUEUE::loop, &m_task_que);
    return m_thread != nullptr;
}

template<class TASK_QUEUE>
void task_thread<TASK_QUEUE>::stop()
{
    (void)m_task_que.stop(MAX_U32);
    if (m_thread != nullptr) {
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
    }
}

using simple_task_thread = task_thread<simple_task_queue>;
using heartbeat_task_thread = task_thread<heartbeat_task_queue>;
END_NSP_DDM
#endif // task_thread_h_
