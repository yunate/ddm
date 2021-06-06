
#ifndef i_heartbeat_task_h_
#define i_heartbeat_task_h_ 1

#include "i_ddtask.h"

#include <functional>
#include <memory>
BEG_NSP_DDM
/** 心跳任务，为心跳任务队列准备
*/
class i_heartbeat_task :
    public i_ddtask
{
public:
    /** 构造函数
    */
    i_heartbeat_task() {}

    /** 析构函数
    */
    virtual ~i_heartbeat_task() {}

    /** 任务执行完成后是否移除该任务
    @return 为true时候，执行完该任务将从任务队列中移除
    */
    virtual bool remove_after_excute()
    {
        return false;
    };

    /** 获得心跳间隔，单位ms
    @return 心跳间隔
    */
    virtual u32 get_timeout() = 0;
};

/** 任务智能指针
*/
using sp_heartbeat_task = std::shared_ptr<i_heartbeat_task>;

/** 回调任务，特别为lambda准备，考虑改为模板
*/
class heartbeat_fun_task :
    public i_heartbeat_task
{
public:
    /** 构造函数
    @param [callback] 可执行回调
    @param [in] timeOut 时间间隔
    @param [in] times 执行次数，为-1代表执行无数次，默认为-1
    */
    heartbeat_fun_task(const std::function<bool()>& callback, u32 timeOut, s32 times = -1) :
        m_callback(callback),
        m_timeOut(timeOut),
        m_times(times),
        m_curTimes(0)
    {}

    /** 析构函数
    */
    virtual ~heartbeat_fun_task() {}

    /** 执行任务
    @return 是否成功
    */
    virtual void do_task()
    {
        if (m_callback != nullptr) {
            if (m_times != -1) {
                ++m_curTimes;
            }

            (void)m_callback();
        }
    }

    /** 停止任务
    */
    virtual void stop() override {}

    /** 任务执行完成后是否移除该任务
    @return 为true时候，执行完该任务将从任务队列中移除
    */
    virtual bool remove_after_excute() override
    {
        if (m_callback == nullptr) {
            return true;
        }

        if (m_times == -1 || m_curTimes != m_times) {
            return false;
        }

        return true;
    };

    /** 获得心跳间隔，单位ms
    @return 心跳间隔
    */
    virtual u32 get_timeout() override
    {
        return m_timeOut;
    }

private:
    /** 回调
    */
    std::function<bool()> m_callback;

    /** 执行间隔
    */
    u32 m_timeOut;

    /** 执行次数
    */
    s32 m_times;

    /** 当前执行次数
    */
    int32_t m_curTimes;
};

END_NSP_DDM
#endif // i_heartbeat_task_h_
