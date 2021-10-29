
#ifndef simple_event_h_
#define simple_event_h_ 1

#include "g_def.h"
#include "ddmath.h"
#include "nocopyable.hpp"
#include <chrono>
#include <mutex>
#include <condition_variable>
BEG_NSP_DDM

/** ���¼�
@note ���ṩ�������������������������ʱ�򣬵��ǻ�����Wait() ״̬���ܲ��ҵĸ����㷢��δ������Ϊ
*/
class simple_event
{
    DD_NO_COPY_MOVE(simple_event);
public:
    simple_event() = default;
    ~simple_event() = default;

    /** ��ʱ�ȴ�
    @param [in] timeOut ��ʱʱ��
    @return ��ʱ����false������true
    */
    bool wait(u32 timeOut = MAX_U32)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        bool rtn = true;
        while (!m_hasSignal && !m_notyfiall) {
            // �������������ǣ�
            // 1��m_mutex.unlock();
            // 2��wait()
            // 3��m_mutex.lock();
            // wait(lck) �൱�ڽ�1��2�����ŵ�һ��cpu������
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

    /** ����һ���ź�
    */
    void notify_one()
    {
        {
            std::unique_lock<std::mutex> lck(m_mutex);
            m_hasSignal = true;
        }
        m_con.notify_one();
    }

    // ���øú�������е�wait ����֪ͨ��δ�����õ�wait Ҳ����ȴ������ǵ���reset()�����ָ�״̬
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

    // notify all ʱ��ı��
    bool m_notyfiall = false;
};

END_NSP_DDM
#endif // simple_event_h_

