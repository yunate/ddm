
#ifndef i_heartbeat_task_h_
#define i_heartbeat_task_h_ 1

#include "i_ddtask.h"

#include <functional>
#include <memory>
BEG_NSP_DDM
/** ��������Ϊ�����������׼��
*/
class i_heartbeat_task :
    public i_ddtask
{
public:
    /** ���캯��
    */
    i_heartbeat_task() {}

    /** ��������
    */
    virtual ~i_heartbeat_task() {}

    /** ����ִ����ɺ��Ƿ��Ƴ�������
    @return Ϊtrueʱ��ִ��������񽫴�����������Ƴ�
    */
    virtual bool remove_after_excute()
    {
        return false;
    };

    /** ��������������λms
    @return �������
    */
    virtual u32 get_timeout() = 0;
};

/** ��������ָ��
*/
using sp_heartbeat_task = std::shared_ptr<i_heartbeat_task>;

/** �ص������ر�Ϊlambda׼�������Ǹ�Ϊģ��
*/
class heartbeat_fun_task :
    public i_heartbeat_task
{
public:
    /** ���캯��
    @param [callback] ��ִ�лص�
    @param [in] timeOut ʱ����
    @param [in] times ִ�д�����Ϊ-1����ִ�������Σ�Ĭ��Ϊ-1
    */
    heartbeat_fun_task(const std::function<bool()>& callback, u32 timeOut, s32 times = -1) :
        m_callback(callback),
        m_timeOut(timeOut),
        m_times(times),
        m_curTimes(0)
    {}

    /** ��������
    */
    virtual ~heartbeat_fun_task() {}

    /** ִ������
    @return �Ƿ�ɹ�
    */
    virtual void do_task()
    {
        if (m_callback != nullptr) {
            if (m_times != -1) {
                ++m_curTimes;
            }

            (void)m_callback();
        }
    }

    /** ֹͣ����
    */
    virtual void stop() override {}

    /** ����ִ����ɺ��Ƿ��Ƴ�������
    @return Ϊtrueʱ��ִ��������񽫴�����������Ƴ�
    */
    virtual bool remove_after_excute() override
    {
        if (m_callback == nullptr) {
            return true;
        }

        if (m_times == -1 || m_curTimes != m_times) {
            return false;
        }

        return true;
    };

    /** ��������������λms
    @return �������
    */
    virtual u32 get_timeout() override
    {
        return m_timeOut;
    }

private:
    /** �ص�
    */
    std::function<bool()> m_callback;

    /** ִ�м��
    */
    u32 m_timeOut;

    /** ִ�д���
    */
    s32 m_times;

    /** ��ǰִ�д���
    */
    int32_t m_curTimes;
};

END_NSP_DDM
#endif // i_heartbeat_task_h_
