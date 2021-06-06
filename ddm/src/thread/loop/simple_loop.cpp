#include "simple_loop.h"
BEG_NSP_DDM

simple_loop::~simple_loop()
{
    // 不应该调用stop，因为loop 是由子类实现的，该类完全是以工具函数的方式提供了stop 和 wait_loop_end来减少代码重复
    // 另外stop 是虚函数，子类如果重写了该函数那么这儿调用会一定程度上造成误解
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
            //  不能在loop线程中wait，否重会死锁
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

    // 防止重复调用wait_loop_end
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


