
#include "heartbeat_task_queue.h"
#include "ddmath.h"
#include "ddtimer.h"

BEG_NSP_DDM

/** ���������һ���װ
*/
struct heartbeat_task_wraper
{
    /** ���캯��
    @param [in] spHeartBeatTask ������������û�Ӧ�ñ�֤��Ϊ�գ����ļ��ڲ�����ȥ�жϿ�
    */
    heartbeat_task_wraper(const sp_heartbeat_task& spHeartBeatTask) :
        m_spHeartBeatTask(spHeartBeatTask) {
        m_preExecuteTime = ddtimer::now_ms();
    }

    /** �Ƿ����ִ���ˣ�����trueʱ������ʱ���
    @return �Ƿ����ִ��
    */
    bool CanExecute()
    {
        u64 nowTime = ddtimer::now_ms();
        if (nowTime - m_preExecuteTime >= m_spHeartBeatTask->get_timeout()) {
            m_preExecuteTime = nowTime;
            return true;
        }

        return false;
    }

    /** ���������ָ��
    */
    sp_heartbeat_task m_spHeartBeatTask;

    /** �ϴ�ִ��ʱ�䣬���һ�ζ�û��ִ�й�Ϊ����ʱ��
    */
    u64 m_preExecuteTime;
};

//////////////////////////////////////////////////////////////////////////
heartbeat_task_queue::~heartbeat_task_queue()
{
    stop(MAX_U32);
}

bool heartbeat_task_queue::stop(u32 waitTime)
{
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        if (!m_stop) {
            clear_all();
            m_stop = true;
        }
    }

    return simple_loop::stop(waitTime);
}

void heartbeat_task_queue::stop_current()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if (m_currentTask != nullptr) {
        m_currentTask->m_spHeartBeatTask->stop();
    }
}

void heartbeat_task_queue::clear_all()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    stop_current();
    clear_queue();
}

void heartbeat_task_queue::clear_queue()
{
    std::list<sp_heartbeat_taskWraper > tmp = std::list<sp_heartbeat_taskWraper >();

    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        m_taskList.swap(tmp);
    }
}

void heartbeat_task_queue::push_task(const sp_heartbeat_task& task)
{
    if (task == nullptr || m_stop) {
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    sp_heartbeat_taskWraper tmp(new(std::nothrow) heartbeat_task_wraper(task));

    if (tmp != nullptr) {
        m_taskList.push_back(tmp);
    }
}

void heartbeat_task_queue::push_task(const std::function<bool()>& task, uint32_t timeOut, int32_t times)
{
    push_task(sp_heartbeat_task(new (std::nothrow) heartbeat_fun_task(task, timeOut, times)));
}

size_t heartbeat_task_queue::get_task_count()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_taskList.size() + (m_currentTask != nullptr ? 1 : 0);
}

void heartbeat_task_queue::loop_core()
{
    while (true) {
        {
            std::lock_guard<std::recursive_mutex> lock(m_mutex);
            if (m_stop) {
                break;
            }
        }

        // ���������ȫ��ִ�е�
        size_t curCount = 0;
        {
            std::lock_guard<std::recursive_mutex> lock(m_mutex);
            curCount = m_taskList.size();
        }
        while (curCount > 0) {
            // ȡ������
            {
                std::lock_guard<std::recursive_mutex> lock(m_mutex);
                if (m_taskList.empty()) {
                    break;
                }
                --curCount;
                m_currentTask = m_taskList.front();
                m_taskList.pop_front();
                if (m_currentTask == nullptr) {
                    continue;
                }
            }

            // ִ������
            if (m_currentTask->CanExecute()) {
                m_currentTask->m_spHeartBeatTask->do_task();
                if (!m_currentTask->m_spHeartBeatTask->remove_after_excute()) {
                    // �������·Żص�β��
                    std::lock_guard<std::recursive_mutex> lock(m_mutex);
                    m_taskList.push_back(m_currentTask);
                }
            } else {
                // �������·Żص�β��
                std::lock_guard<std::recursive_mutex> lock(m_mutex);
                m_taskList.push_back(m_currentTask);
            }

            // �����Ҫ��Ϊ�˷�ֹ���������г�ʱ��ִ��ռ��mutex
            sp_heartbeat_taskWraper tmp = m_currentTask;

            {
                std::lock_guard<std::recursive_mutex> lock(m_mutex);
                m_currentTask = nullptr;
            }
        }

        ddtimer::sleep(1);
    }
}

END_NSP_DDM


