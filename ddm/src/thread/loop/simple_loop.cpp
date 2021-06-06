#include "simple_loop.h"
BEG_NSP_DDM

simple_loop::~simple_loop()
{
    // ��Ӧ�õ���stop����Ϊloop ��������ʵ�ֵģ�������ȫ���Թ��ߺ����ķ�ʽ�ṩ��stop �� wait_loop_end�����ٴ����ظ�
    // ����stop ���麯�������������д�˸ú�����ô������û�һ���̶���������
    // (void)stop(MAX_U32);
}

bool simple_loop::stop(u32 waitTime)
{
    m_stop = true;
    return wait_loop_end(waitTime);
}

bool simple_loop::wait_loop_end(u32 waitTime /*= MAX_U32*/)
{
    std::thread::id trdId = std::this_thread::get_id();

    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        if (m_hasLoopEnd) {
            return true;
        }

        if (m_loopId == trdId && waitTime != 0) {
            //  ������loop�߳���wait�����ػ�����
            DD_ASSERT_FMT(false, _DDT("wait_loop_end can not be call in his loop thread while waitTime != 0"));
            waitTime = 0;
        }
    }

    bool result = false;
    if (MAX_U32 != waitTime) {
        result = m_loopEndEvent.time_wait(waitTime);
    } else {
        m_loopEndEvent.Wait();
        result = true;
    }

    // ��ֹ�ظ�����wait_loop_end
    if (result) {
        m_loopEndEvent.set_event();
    }

    return result;
}

void NSP_DDM::simple_loop::loop()
{
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        m_loopId = std::this_thread::get_id();
        m_hasLoopEnd = false;
    }
    loop_core();
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        m_hasLoopEnd = true;
    }
    m_loopEndEvent.set_event();
}
END_NSP_DDM


