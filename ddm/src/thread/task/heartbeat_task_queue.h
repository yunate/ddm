
#ifndef heartbeat_task_queue_h_
#define heartbeat_task_queue_h_ 1

#include "../loop/simple_loop.h"
#include "i_heartbeat_task.h"
#include <list>
#include <mutex>
BEG_NSP_DDM
struct heartbeat_task_wraper;
using sp_heartbeat_taskWraper = std::shared_ptr<heartbeat_task_wraper>;

/** �����߳�����ģ��
*/
class heartbeat_task_queue :
    public simple_loop
{
public:
    /** ���캯��
    */
    heartbeat_task_queue() = default;

    /** ��������
    */
    ~heartbeat_task_queue();

public:
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
    */
    void push_task(const sp_heartbeat_task& task);

    /** ���һ����������
    @param [in] task ����
    @param [in] timeOut ʱ����
    @param [in] times ִ�д�����Ϊ-1����ִ�������Σ�Ĭ��Ϊ-1
    */
    void push_task(const std::function<bool()>& task, uint32_t timeOut, int32_t times = -1);

    /** ��õ�ǰ�����е�������������ǰ����ִ�е�
    @return ��ǰ�����е�����
    */
    size_t get_task_count();

public:
    /** �̻߳ص�
    */
    virtual void loop_core();

private:
    /** �������
    */
    std::list<sp_heartbeat_taskWraper > m_taskList;

    /** ��ǰ����ִ�е�
    */
    sp_heartbeat_taskWraper m_currentTask;
};

END_NSP_DDM
#endif // heartbeat_task_queue_h_
