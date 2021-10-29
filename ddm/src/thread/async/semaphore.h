
#ifndef semaphore_h_
#define semaphore_h_ 1

#include "g_def.h"
#include "nocopyable.hpp"
#include "ddmath.h"
#include <mutex>
BEG_NSP_DDM

class semaphore
{
    DD_NO_COPY_MOVE(semaphore);
public:
    semaphore() = default;
    ~semaphore() = default;
    /** 超时等待
    @param [in] timeOut 超时时间
    @return 超时返回false，否者true
    */
    bool wait(u32 timeOut = MAX_U32)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        bool rtn = true;
        while (m_count <= 0 && !m_notyfiall) {
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
            --m_count;
        }

        if (m_count < 0) {
            m_count = 0;
        }
        return rtn;
    }

    /** 发出一个信号（生产）
    */
    void notify_one()
    {
        {
            std::unique_lock<std::mutex> lck(m_mutex);
            ++m_count;
        }
        m_con.notify_one();
    }

    // 调用该函数侯，所有的wait 都被通知，未来调用的wait 也不会等待，除非调用reset()函数恢复状态
    void notify_all()
    {
        {
            std::unique_lock<std::mutex> lck(m_mutex);
            m_notyfiall = true;
            ++m_count;
        }
        m_con.notify_all();
    }

    void reset(u32 cnt)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_notyfiall = false;
        m_count = cnt;
    }

private:
    /** 互斥量
    */
    std::mutex m_mutex;

    /** condition_variable
    */
    std::condition_variable m_con;

    /** 是否有信号
    @note 主要是为了先调用SetEvent()，然后调用wait() 不会被唤醒的情况，所谓的“虚假唤醒”。考虑在Wait()前加上::Sleep(1000);
    */
    long m_count = 0;

    // notify all 时候的标记
    bool m_notyfiall = false;
};

END_NSP_DDM
#endif // semaphore_h_
