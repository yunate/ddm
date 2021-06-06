
#ifndef simple_thread_manager_h_
#define simple_thread_manager_h_ 1

#include "g_def.h"
#include "task_thread.h"

BEG_NSP_DDM
/** 简单线程管理类
@note !!!!!!!!!!! 析构函数/uninit 中不应该有东西再线程中执行
*/
class simple_thread_manager
{
public:
    /** 构造函数
    */
    simple_thread_manager();

    /** 析构函数
    */
    virtual ~simple_thread_manager();

public:
    /** 获得阻塞线程数量，如果需要更多数量，继承改类并重载该函数
    @return 返回阻塞任务队列数量
    */
    virtual uint32_t GetBlockTaskThreadCount();

    /** 停止所有，子类重载该函数时候应该显示再次调用该函数
    */
    virtual void stop_all();

public:
    /** 添加一个快速任务
    @param [in] task 任务
    */
    void push_fast_task(const std::shared_ptr<i_ddtask>& task);

    /** 添加一个快速函数任务
    @param [in] task 任务
    */
    void push_fast_task(const std::function<bool()>& task);

    /** 获得快速队列，如果还没初始化的话，该函数会自动初始化
    @return 快速队列
    */
    std::weak_ptr <simple_task_thread> get_fast_thread_taskqueue();

    /** 添加一个阻塞任务
    @param [in] task 任务
    */
    void push_block_task(const std::shared_ptr<i_ddtask>& task);

    /** 添加一个阻塞函数任务
    @param [in] task 任务
    */
    void push_block_task(const std::function<bool()>& task);

    /** 添加一个心跳务
    @param [in] task 任务
    */
    void push_heartbeat_task(const sp_heartbeat_task& task);

    /** 添加一个心跳函数任务
    @param [in] task 任务
    @param [in] timeOut 时间间隔
    @param [in] times 执行次数，为-1代表执行无数次，默认为-1
    */
    void push_heartbeat_task(const std::function<bool()>& task, uint32_t timeOut, int32_t times = -1);

protected:
    /** 快速任务线程一条
    */
    std::shared_ptr<simple_task_thread> m_spFastThreadTaskQueue;

    /** 阻塞任务线程数组
    @note TODO: 暂时用数组存储多个任务队列，以后可以考虑将内部的que封装成线程安全的，然后添加支持多个线程的队列
    */
    std::vector<std::shared_ptr<simple_task_thread>> m_spBlockThreadTaskQueues;

    /** 心跳任务一条
    */
    std::shared_ptr<heartbeat_task_thread> m_spHeartBeatThreadTaskQueue;

    /** 互斥量
    */
    std::recursive_mutex m_mutex;
};

END_NSP_DDM
#endif // simple_thread_manager_h_
