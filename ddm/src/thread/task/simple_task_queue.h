
#ifndef simple_task_queue_h_
#define simple_task_queue_h_ 1


#include "i_ddtask.h"
#include "../loop/simple_loop.h"

#include <mutex>
#include <functional>
#include <memory>
#include <queue>
#include <list>
#include <condition_variable>

BEG_NSP_DDM

/** ���������
*/
class simple_task_queue :
    public simple_loop
{
    using sp_task = std::shared_ptr<i_ddtask>;
    using task_queue = std::queue<sp_task, std::list<sp_task>>;
    using _lock_guard = std::lock_guard<std::recursive_mutex>;
public:
    /** ���캯��
    */
    simple_task_queue(u32 max_cnt = MAX_U32);

    /** ��������
    */
    virtual ~simple_task_queue();

public:
    // ��ȡ���ö������ֵ��MAX_U32 ��ʾ����
    u32 get_max_cnt();
    void set_max_cnt(u32 max_cnt);

    /** ֹͣ��������
    */
    virtual bool stop(u32 waitTime);

    /** ֹͣ��ǰ������
    */
    void stop_current();

    /** ����������������ǰ�ģ�����ֹͣѭ��
    */
    void clear_all();

    /** ����������񲻰�����ǰ�ģ�����ֹͣѭ��
    */
    void clear_queue();

    /** ���һ������
    @param [in] task ����
    @param [in] wait_time �������������˵ĵȴ�ʱ�� MAX_U32 ��ʾ������ʱ����λ ms
    @return �Ƿ�ɹ�������������˲��ҳ�ʱ�˷���false
    */
    bool push_task(const sp_task& task, u32 wait_time = MAX_U32);

    /** ���һ����������
    @param [in] task ����
    @param [in] wait_time �������������˵ĵȴ�ʱ�� MAX_U32 ��ʾ������ʱ����λ ms
    @return �Ƿ�ɹ�������������˲��ҳ�ʱ�ˣ�����ֹͣ�˷��� false
    */
    bool push_task(const std::function<void()>& task, u32 wait_time = MAX_U32);

    /** ��õ�ǰ�����е�������������ǰ����ִ�еģ�����0˵�����п���
    @return ��ǰ�����е�����
    */
    size_t get_task_count();

protected:
    /** �̻߳ص�
    */
    virtual void loop_core();

protected:
    // ����ѭ�� do while ��ʹ�� m_dotask_cv �ȴ�ֱ��������в�Ϊ��
    std::condition_variable_any m_dotask_cv;

    // push ������ m_push_cv ���ȴ��������ֱ���п���λ��
    std::condition_variable_any m_push_cv;

    // ����������ֵ��Ϊ MAX_U32 ��ʾ����
    u32 m_max_cnt = MAX_U32;

    /** �������
    */
    task_queue m_taskQue;

    /** ��ǰ����ִ�е�
    */
    sp_task m_currentTask;
};

END_NSP_DDM
#endif // simple_task_queue_h_
