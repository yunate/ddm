
#include "simple_task_queue.h"
#include "thread/async/simple_event.h"
#include "simple_function_task.h"
#include "ddmath.h"

BEG_NSP_DDM
simple_task_queue::simple_task_queue(u32 max_cnt /*= MAX_U32*/) :
    m_max_cnt(max_cnt)
{
}

simple_task_queue::~simple_task_queue()
{
    (void)stop(MAX_U32);
}

u32 simple_task_queue::get_max_cnt()
{
    _lock_guard lock(m_mutex);
    return m_max_cnt;
}

void simple_task_queue::set_max_cnt(u32 max_cnt)
{
    _lock_guard lock(m_mutex);
    m_max_cnt = max_cnt;
}

bool simple_task_queue::stop(u32 waitTime)
{
    {
        _lock_guard lock(m_mutex);
        if (!m_stop) {
            clear_all();
            m_dotask_cv.notify_one();
            m_stop = true;
        }
    }

    return simple_loop::stop(waitTime);
}

void simple_task_queue::stop_current()
{
    _lock_guard lock(m_mutex);

    if (m_currentTask != nullptr) {
        m_currentTask->stop();
    }
}

void simple_task_queue::clear_all()
{
    _lock_guard lock(m_mutex);
    stop_current();
    clear_queue();
}

void simple_task_queue::clear_queue()
{
    task_queue tmp;
    {
        _lock_guard lock(m_mutex);
        m_taskQue.swap(tmp);
    }

    m_push_cv.notify_one();
}

bool simple_task_queue::push_task(const sp_task& task, u32 wait_time /*= MAX_U32*/)
{
    _lock_guard lock(m_mutex);
    if (m_stop) {
        return false;
    }

    if (m_max_cnt != MAX_U32) {
        while (m_taskQue.size() >= m_max_cnt) {
            if (wait_time == MAX_U32) {
                m_push_cv.wait(m_mutex);
            } else if (m_push_cv.wait_for(m_mutex, std::chrono::milliseconds(wait_time)) == std::cv_status::timeout) {
                return false;
            }
        }
    }

    m_taskQue.push(task);
    m_dotask_cv.notify_one();
    return true;
}

bool simple_task_queue::push_task(const std::function<void()>& task, u32 wait_time /*= MAX_U32*/)
{
    return push_task(sp_task(new simple_function_task(task)), wait_time);
}

size_t simple_task_queue::get_task_count()
{
    _lock_guard lock(m_mutex);
    return m_taskQue.size() + (m_currentTask != nullptr ? 1 : 0);
}

void simple_task_queue::loop_core()
{
    while (true)
    {
        {
            _lock_guard lock(m_mutex);
            if (m_stop) {
                break;
            }

            if (m_taskQue.empty()) {
                (void)m_dotask_cv.wait_for(m_mutex, std::chrono::milliseconds(100));
                if (m_taskQue.empty()) {
                    continue;
                }
            }

            // 取出队首
            m_currentTask = m_taskQue.front();
            m_taskQue.pop();

            if (m_currentTask == nullptr) {
                continue;
            }
        }

        // 执行任务
        m_currentTask->do_task();
        m_push_cv.notify_one();

        // 准备销毁
        sp_task tmp = m_currentTask;
        {
            _lock_guard lock(m_mutex);
            m_currentTask = nullptr;
        }
    }
}

END_NSP_DDM

