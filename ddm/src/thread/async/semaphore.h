
#ifndef semaphore_h_
#define semaphore_h_ 1

#include "g_def.h"
#include "nocopyable.hpp"

#include <mutex>
BEG_NSP_DDM

class semaphore :
    public nocopyable
{
public:
    /** 等待一个信号（消费）
    @note 如果在Wait()前已经有信号了，那么wait将不会等待
    */
    void wait() 
    {
        std::unique_lock<std::mutex> lck(m_mutex);

        if (m_count <= 0) {
            // 不会死锁，考虑：
            // 1、m_mutex.unlock();
            // 2、wait()
            // 3、m_mutex.lock();
            // wait(lck) 相当于将1、2两步放到一个cpu周期内
            m_con.wait(lck);
        }

        --m_count;

        // 防止NotifyAll的情况m_count减少为0
        if (m_count < 0) {
            m_count = 0;
        }
    }

    /** 发出一个信号（生产）
    */
    void signal()
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        ++m_count;
        m_con.notify_one();
    }

    /** 唤醒所有（重置）
    */
    void notify_all()
    {
        m_count = 0;
        std::unique_lock<std::mutex> lck(m_mutex);
        m_con.notify_all();
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
};

END_NSP_DDM
#endif // semaphore_h_
