
#ifndef simple_thread_manager_h_
#define simple_thread_manager_h_ 1

#include "g_def.h"
#include "task_thread.h"

BEG_NSP_DDM
/** ���̹߳�����
@note !!!!!!!!!!! ��������/uninit �в�Ӧ���ж������߳���ִ��
*/
class simple_thread_manager
{
public:
    /** ���캯��
    */
    simple_thread_manager();

    /** ��������
    */
    virtual ~simple_thread_manager();

public:
    /** ��������߳������������Ҫ�����������̳и��ಢ���ظú���
    @return �������������������
    */
    virtual uint32_t GetBlockTaskThreadCount();

    /** ֹͣ���У��������ظú���ʱ��Ӧ����ʾ�ٴε��øú���
    */
    virtual void stop_all();

public:
    /** ���һ����������
    @param [in] task ����
    */
    void push_fast_task(const std::shared_ptr<i_ddtask>& task);

    /** ���һ�����ٺ�������
    @param [in] task ����
    */
    void push_fast_task(const std::function<bool()>& task);

    /** ��ÿ��ٶ��У������û��ʼ���Ļ����ú������Զ���ʼ��
    @return ���ٶ���
    */
    std::weak_ptr <simple_task_thread> get_fast_thread_taskqueue();

    /** ���һ����������
    @param [in] task ����
    */
    void push_block_task(const std::shared_ptr<i_ddtask>& task);

    /** ���һ��������������
    @param [in] task ����
    */
    void push_block_task(const std::function<bool()>& task);

    /** ���һ��������
    @param [in] task ����
    */
    void push_heartbeat_task(const sp_heartbeat_task& task);

    /** ���һ��������������
    @param [in] task ����
    @param [in] timeOut ʱ����
    @param [in] times ִ�д�����Ϊ-1����ִ�������Σ�Ĭ��Ϊ-1
    */
    void push_heartbeat_task(const std::function<bool()>& task, uint32_t timeOut, int32_t times = -1);

protected:
    /** ���������߳�һ��
    */
    std::shared_ptr<simple_task_thread> m_spFastThreadTaskQueue;

    /** ���������߳�����
    @note TODO: ��ʱ������洢���������У��Ժ���Կ��ǽ��ڲ���que��װ���̰߳�ȫ�ģ�Ȼ�����֧�ֶ���̵߳Ķ���
    */
    std::vector<std::shared_ptr<simple_task_thread>> m_spBlockThreadTaskQueues;

    /** ��������һ��
    */
    std::shared_ptr<heartbeat_task_thread> m_spHeartBeatThreadTaskQueue;

    /** ������
    */
    std::recursive_mutex m_mutex;
};

END_NSP_DDM
#endif // simple_thread_manager_h_
