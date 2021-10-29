
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
    /** ��ʱ�ȴ�
    @param [in] timeOut ��ʱʱ��
    @return ��ʱ����false������true
    */
    bool wait(u32 timeOut = MAX_U32)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        bool rtn = true;
        while (m_count <= 0 && !m_notyfiall) {
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
            --m_count;
        }

        if (m_count < 0) {
            m_count = 0;
        }
        return rtn;
    }

    /** ����һ���źţ�������
    */
    void notify_one()
    {
        {
            std::unique_lock<std::mutex> lck(m_mutex);
            ++m_count;
        }
        m_con.notify_one();
    }

    // ���øú�������е�wait ����֪ͨ��δ�����õ�wait Ҳ����ȴ������ǵ���reset()�����ָ�״̬
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
    /** ������
    */
    std::mutex m_mutex;

    /** condition_variable
    */
    std::condition_variable m_con;

    /** �Ƿ����ź�
    @note ��Ҫ��Ϊ���ȵ���SetEvent()��Ȼ�����wait() ���ᱻ���ѵ��������ν�ġ���ٻ��ѡ���������Wait()ǰ����::Sleep(1000);
    */
    long m_count = 0;

    // notify all ʱ��ı��
    bool m_notyfiall = false;
};

END_NSP_DDM
#endif // semaphore_h_
