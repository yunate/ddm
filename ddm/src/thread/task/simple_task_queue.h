
#ifndef simple_task_queue_h_
#define simple_task_queue_h_ 1


#include "i_ddtask.h"
#include "../loop/simple_loop.h"

#include <mutex>
#include <functional>
#include <memory>
#include <queue>

BEG_NSP_DDM

/** ���������
*/
class simple_task_queue :
    public simple_loop
{
public:
    /** ���캯��
    */
    simple_task_queue();

    /** ��������
    */
    virtual ~simple_task_queue();

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
    void push_task(const std::shared_ptr<i_ddtask>& task);

    /** ���һ����������
    @param [in] task ����
    */
    void push_task(const std::function<void()>& task);

    /** ��õ�ǰ�����е�������������ǰ����ִ�еģ�����0˵�����п���
    @return ��ǰ�����е�����
    */
    size_t get_task_count();

protected:
    /** �̻߳ص�
    */
    virtual void loop_core();

private:
    /** �߳��¼�
    */
    simple_event m_event;

    /** �������
    */
    std::queue<std::shared_ptr<i_ddtask> > m_taskQue;

    /** ��ǰ����ִ�е�
    */
    std::shared_ptr<i_ddtask> m_currentTask;
};

END_NSP_DDM
#endif // simple_task_queue_h_
