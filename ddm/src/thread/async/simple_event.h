
#ifndef simple_event_h_
#define simple_event_h_ 1

#include "g_def.h"
#include "ddmath.h"
#include "nocopyable.hpp"
#include <chrono>
#include <mutex>
#include <condition_variable>
BEG_NSP_DDM

/** 简单事件
@note 不提供析构函数，如果当该类析构的时候，但是还处于Wait() 状态，很不幸的告诉你发生未定义行为
*/
class simple_event
{
    DD_NO_COPY_MOVE(simple_event);
public:
    simple_event() = default;
    ~simple_event() = default;

    /** 超时等待
    @param [in] timeOut 超时时间
    @return 超时返回false，否者true
    */
    bool wait(u32 timeOut = MAX_U32)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        bool rtn = true;
        while (!m_hasSignal && !m_notyfiall) {
            // 不会死锁，考虑：
            // 1、m_mutex.unlock();
            // 2、wait()
            // 3、m_mutex.lock();
            // wait(lck) 相当于将1、2两步放到一个cpu周期内
            if (timeOut == MAX_U32) {
                m_con.wait(lck);
            } else if (m_con.wait_for(lck, std::chrono::milliseconds(timeOut)) == std::cv_status::timeout) {
               rtn = false;
               break;
            }
        }

        if (rtn && !m_notyfiall) {
            m_hasSignal = false;
        }
        return rtn;
    }

    /** 设置一个信号
    */
    void notify_one()
    {
        {
            std::unique_lock<std::mutex> lck(m_mutex);
            m_hasSignal = true;
        }
        m_con.notify_one();
    }

    // 调用该函数侯，所有的wait 都被通知，未来调用的wait 也不会等待，除非调用reset()函数恢复状态
    void notify_all()
    {
        {
            std::unique_lock<std::mutex> lck(m_mutex);
            m_notyfiall = true;
            m_hasSignal = true;
        }
        m_con.notify_all();
    }

    void reset(bool signal)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_notyfiall = false;
        m_hasSignal = signal;
    }

private:
    /** 互斥量
    */
    std::mutex m_mutex;

    /** condition_variable
    */
    std::condition_variable m_con;

    /** 是否有信号，只是记录是否有信号，但是不记录信号次数
    @note 主要是为了先调用SetEvent()，然后调用wait的情况。考虑在Wait()前加上::Sleep(1000);
    */
    bool m_hasSignal = false;

    // notify all 时候的标记
    bool m_notyfiall = false;
};

END_NSP_DDM
#endif // simple_event_h_

