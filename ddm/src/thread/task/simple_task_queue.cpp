
#include "simple_task_queue.h"
#include "thread/async/simple_event.h"
#include "simple_function_task.h"
#include "ddmath.h"

BEG_NSP_DDM
simple_task_queue::simple_task_queue()
{
}

simple_task_queue::~simple_task_queue()
{
    (void)stop(MAX_U32);
}

bool simple_task_queue::stop(u32 waitTime)
{
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        if (!m_stop) {
            clear_all();
            m_event.set_event();
        }
    }

    return simple_loop::stop(waitTime);
}

void simple_task_queue::stop_current()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if (m_currentTask != nullptr) {
        m_currentTask->stop();
    }
}

void simple_task_queue::clear_all()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    stop_current();
    clear_queue();
}

void simple_task_queue::clear_queue()
{
    std::queue<std::shared_ptr<i_ddtask> > tmp = std::queue<std::shared_ptr<i_ddtask> >();
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        m_taskQue.swap(tmp);
    }
}

void simple_task_queue::push_task(const std::shared_ptr<i_ddtask>& task)
{
    if (m_stop) {
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_taskQue.push(task);
    m_event.set_event();
}

void simple_task_queue::push_task(const std::function<void()>& task)
{
    push_task(std::shared_ptr<i_ddtask>(new simple_function_task(task)));
}

size_t simple_task_queue::get_task_count()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_taskQue.size() + (m_currentTask != nullptr ? 1 : 0);
}

void simple_task_queue::loop_core()
{
    while (true)
    {
        {
            std::lock_guard<std::recursive_mutex> lock(m_mutex);

            if (m_stop) {
                break;
            }
        }

        m_event.Wait();

        // 将队列里的全部执行掉
        while (true) {
            // 取出队首
            {
                std::lock_guard<std::recursive_mutex> lock(m_mutex);

                if (m_taskQue.empty()) {
                    break;
                }

                m_currentTask = m_taskQue.front();
                m_taskQue.pop();

                if (m_currentTask == nullptr) {
                    continue;
                }
            }

            // 执行任务
            m_currentTask->do_task();

            // 准备销毁
            std::shared_ptr<i_ddtask> tmp = m_currentTask;

            {
                std::lock_guard<std::recursive_mutex> lock(m_mutex);
                m_currentTask = nullptr;
            }
        }
    }
}

END_NSP_DDM

