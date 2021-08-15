
#ifndef simple_task_queue_h_
#define simple_task_queue_h_ 1


#include "i_ddtask.h"
#include "../loop/simple_loop.h"

#include <mutex>
#include <functional>
#include <memory>
#include <queue>
#include <list>
#include <condition_variable>

BEG_NSP_DDM

/** 简单任务队列
*/
class simple_task_queue :
    public simple_loop
{
    using sp_task = std::shared_ptr<i_ddtask>;
    using task_queue = std::queue<sp_task, std::list<sp_task>>;
    using _lock_guard = std::lock_guard<std::recursive_mutex>;
public:
    /** 构造函数
    */
    simple_task_queue(u32 max_cnt = MAX_U32);

    /** 析构函数
    */
    virtual ~simple_task_queue();

public:
    // 获取设置队列最大值，MAX_U32 表示无限
    u32 get_max_cnt();
    void set_max_cnt(u32 max_cnt);

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
    @param [in] wait_time 如果任务队列满了的等待时间 MAX_U32 表示永不超时，单位 ms
    @return 是否成功，如果队列满了并且超时了返回false
    */
    bool push_task(const sp_task& task, u32 wait_time = MAX_U32);

    /** 添加一个函数任务
    @param [in] task 任务
    @param [in] wait_time 如果任务队列满了的等待时间 MAX_U32 表示永不超时，单位 ms
    @return 是否成功，如果队列满了并且超时了，或者停止了返回 false
    */
    bool push_task(const std::function<void()>& task, u32 wait_time = MAX_U32);

    /** 获得当前队列中的数量，包含当前正在执行的，返回0说明队列空闲
    @return 当前队列中的数量
    */
    size_t get_task_count();

protected:
    /** 线程回调
    */
    virtual void loop_core();

protected:
    // 消费循环 do while 中使用 m_dotask_cv 等待直到任务队列不为空
    std::condition_variable_any m_dotask_cv;

    // push 操作用 m_push_cv 来等待任务队列直到有空闲位置
    std::condition_variable_any m_push_cv;

    // 任务队列最大值，为 MAX_U32 表示无限
    u32 m_max_cnt = MAX_U32;

    /** 任务队列
    */
    task_queue m_taskQue;

    /** 当前正在执行的
    */
    sp_task m_currentTask;
};

END_NSP_DDM
#endif // simple_task_queue_h_
