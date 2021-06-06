
#include "simple_thread_manager.h"
#include "thread/task/simple_function_task.h"
BEG_NSP_DDM

simple_thread_manager::simple_thread_manager() {}

simple_thread_manager::~simple_thread_manager() {}

uint32_t simple_thread_manager::GetBlockTaskThreadCount()
{
    return 3;
}

void simple_thread_manager::stop_all()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    if (m_spFastThreadTaskQueue != nullptr) {
        m_spFastThreadTaskQueue->get_task_queue().stop(MAX_U32);
        m_spFastThreadTaskQueue.reset();
    }

    for (auto& it : m_spBlockThreadTaskQueues) {
        it->get_task_queue().stop(MAX_U32);
    }

    m_spBlockThreadTaskQueues.clear();

    if (m_spHeartBeatThreadTaskQueue != nullptr) {
        m_spHeartBeatThreadTaskQueue->get_task_queue().stop(MAX_U32);
        m_spHeartBeatThreadTaskQueue.reset();
    }
}

std::weak_ptr<simple_task_thread> simple_thread_manager::get_fast_thread_taskqueue()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    if (m_spFastThreadTaskQueue == nullptr) {
        m_spFastThreadTaskQueue.reset(new (std::nothrow) simple_task_thread());

        if (m_spFastThreadTaskQueue == nullptr) {
            return m_spFastThreadTaskQueue;
        }

        m_spFastThreadTaskQueue->start();
    }

    return m_spFastThreadTaskQueue;
}

void simple_thread_manager::push_fast_task(const std::shared_ptr<i_ddtask>& task)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    std::weak_ptr<simple_task_thread> spFastTaskQueue = get_fast_thread_taskqueue();
    auto it = spFastTaskQueue.lock();

    if (it == nullptr) {
        return;
    }

    it->get_task_queue().push_task(task);
}

void simple_thread_manager::push_fast_task(const std::function<bool()>& task)
{
    push_fast_task(std::shared_ptr<i_ddtask>(new (std::nothrow) simple_function_task(task)));
}

void simple_thread_manager::push_block_task(const std::shared_ptr<i_ddtask>& task)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    std::shared_ptr<simple_task_thread> tmpQue;

    if (m_spBlockThreadTaskQueues.empty()) {
        tmpQue.reset(new (std::nothrow) simple_task_thread());

        if (tmpQue != nullptr) {
            m_spBlockThreadTaskQueues.push_back(tmpQue);
            tmpQue->start();
        }
    } else {
        tmpQue = m_spBlockThreadTaskQueues[0];

        for (size_t i = 1; i < m_spBlockThreadTaskQueues.size(); ++i) {
            if (tmpQue->get_task_queue().get_task_count() > m_spBlockThreadTaskQueues[i]->get_task_queue().get_task_count()) {
                tmpQue = m_spBlockThreadTaskQueues[i];
            }
        }

        if (tmpQue->get_task_queue().get_task_count() != 0 && m_spBlockThreadTaskQueues.size() < GetBlockTaskThreadCount()) {
            simple_task_thread* pTaskThreadQueue = new (std::nothrow) simple_task_thread();

            if (pTaskThreadQueue != nullptr) {
                tmpQue.reset(pTaskThreadQueue);
                m_spBlockThreadTaskQueues.push_back(tmpQue);
                tmpQue->start();
            }
        }
    }

    if (tmpQue != nullptr) {
        tmpQue->get_task_queue().push_task(task);
    }
}

void simple_thread_manager::push_block_task(const std::function<bool()>& task)
{
    push_block_task(std::shared_ptr<i_ddtask>(new (std::nothrow) simple_function_task(task)));
}

void simple_thread_manager::push_heartbeat_task(const sp_heartbeat_task& task)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    if (m_spHeartBeatThreadTaskQueue == nullptr) {
        m_spHeartBeatThreadTaskQueue.reset(new (std::nothrow) heartbeat_task_thread());

        if (m_spHeartBeatThreadTaskQueue == nullptr) {
            return;
        }

        m_spHeartBeatThreadTaskQueue->start();
    }

    m_spHeartBeatThreadTaskQueue->get_task_queue().push_task(task);
}

void simple_thread_manager::push_heartbeat_task(const std::function<bool()>& task, uint32_t timeOut, int32_t times)
{
    push_heartbeat_task(sp_heartbeat_task(new (std::nothrow) heartbeat_fun_task(task, timeOut, times)));
}

END_NSP_DDM
