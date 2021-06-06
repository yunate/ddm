
#ifndef simple_task_queue_h_
#define simple_task_queue_h_ 1


#include "i_ddtask.h"
#include "../loop/simple_loop.h"

#include <mutex>
#include <functional>
#include <memory>
#include <queue>

BEG_NSP_DDM

/** 简单任务队列
*/
class simple_task_queue :
    public simple_loop
{
public:
    /** 构造函数
    */
    simple_task_queue();

    /** 析构函数
    */
    virtual ~simple_task_queue();

public:
    /** 停止所有任务
    */
    virtual bool stop(u32 waitTime);

    /** 停止当前的任务
    */
    void stop_current();

    /** 清空所有任务包括当前的，但不停止循环
    */
    void clear_all();

    /** 清空所有任务不包括当前的，但不停止循环
    */
    void clear_queue();

    /** 添加一个任务
    @param [in] task 任务
    */
    void push_task(const std::shared_ptr<i_ddtask>& task);

    /** 添加一个函数任务
    @param [in] task 任务
    */
    void push_task(const std::function<void()>& task);

    /** 获得当前队列中的数量，包含当前正在执行的，返回0说明队列空闲
    @return 当前队列中的数量
    */
    size_t get_task_count();

protected:
    /** 线程回调
    */
    virtual void loop_core();

private:
    /** 线程事件
    */
    simple_event m_event;

    /** 任务队列
    */
    std::queue<std::shared_ptr<i_ddtask> > m_taskQue;

    /** 当前正在执行的
    */
    std::shared_ptr<i_ddtask> m_currentTask;
};

END_NSP_DDM
#endif // simple_task_queue_h_
