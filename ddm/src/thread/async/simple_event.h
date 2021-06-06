
#ifndef simple_event_h_
#define simple_event_h_ 1

#include "g_def.h"
#include "nocopyable.hpp"
#include <chrono>
#include <mutex>
#include <condition_variable>
BEG_NSP_DDM

/** ���¼�
@note ���ṩ�������������������������ʱ�򣬵��ǻ�����Wait() ״̬���ܲ��ҵĸ����㷢��δ������Ϊ
*/
class simple_event :
    public nocopyable
{
public:
    /** �ȴ�һ���ź�
    */
    void Wait()
    {
        std::unique_lock<std::mutex> lck(m_mutex);

        if (!m_hasSignal) {
            // �������������ǣ�
            // 1��m_mutex.unlock();
            // 2��wait()
            // 3��m_mutex.lock();
            // wait(lck) �൱�ڽ�1��2�����ŵ�һ��cpu������
            m_con.wait(lck);
        }

        m_hasSignal = false;
    }

    /** ��ʱ�ȴ�
    @param [in] timeOut ��ʱʱ��
    @return ��ʱ����false������true
    */
    bool time_wait(u32 timeOut)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        std::cv_status statu = std::cv_status::no_timeout;
        if (!m_hasSignal) {
            // �������������ǣ�
            // 1��m_mutex.unlock();
            // 2��wait()
            // 3��m_mutex.lock();
            // wait(lck) �൱�ڽ�1��2�����ŵ�һ��cpu������
            statu = m_con.wait_for(lck, std::chrono::milliseconds(timeOut));
        }

        m_hasSignal = false;
        return statu == std::cv_status::no_timeout;
    }

    /** ����һ���ź�
    */
    void set_event()
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_hasSignal = true;
        m_con.notify_all();
    }

private:
    /** ������
    */
    std::mutex m_mutex;

    /** condition_variable
    */
    std::condition_variable m_con;

    /** �Ƿ����źţ�ֻ�Ǽ�¼�Ƿ����źţ����ǲ���¼�źŴ���
    @note ��Ҫ��Ϊ���ȵ���SetEvent()��Ȼ�����wait�������������Wait()ǰ����::Sleep(1000);
    */
    bool m_hasSignal = false;
};

END_NSP_DDM
#endif // simple_event_h_

