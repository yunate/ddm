
#ifndef simple_event_h_
#define simple_event_h_ 1

#include "g_def.h"
#include "nocopyable.hpp"
#include <chrono>
#include <mutex>
#include <condition_variable>
BEG_NSP_DDM

/** 简单事件
@note 不提供析构函数，如果当该类析构的时候，但是还处于Wait() 状态，很不幸的告诉你发生未定义行为
*/
class simple_event :
    public nocopyable
{
public:
    /** 等待一个信号
    */
    void Wait()
    {
        std::unique_lock<std::mutex> lck(m_mutex);

        if (!m_hasSignal) {
            // 不会死锁，考虑：
            // 1、m_mutex.unlock();
            // 2、wait()
            // 3、m_mutex.lock();
            // wait(lck) 相当于将1、2两步放到一个cpu周期内
            m_con.wait(lck);
        }

        m_hasSignal = false;
    }

    /** 超时等待
    @param [in] timeOut 超时时间
    @return 超时返回false，否者true
    */
    bool time_wait(u32 timeOut)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        std::cv_status statu = std::cv_status::no_timeout;
        if (!m_hasSignal) {
            // 不会死锁，考虑：
            // 1、m_mutex.unlock();
            // 2、wait()
            // 3、m_mutex.lock();
            // wait(lck) 相当于将1、2两步放到一个cpu周期内
            statu = m_con.wait_for(lck, std::chrono::milliseconds(timeOut));
        }

        m_hasSignal = false;
        return statu == std::cv_status::no_timeout;
    }

    /** 设置一个信号
    */
    void set_event()
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_hasSignal = true;
        m_con.notify_all();
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
};

END_NSP_DDM
#endif // simple_event_h_

