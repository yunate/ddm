
#ifndef __PIPE_LISTENER_H_
#define __PIPE_LISTENER_H_ 1

#include "ipc_win/ipc_channel_listener.h"
#include "ipc_win/ipc_message.h"
#include "thread/thread_manager/task_thread.h"
#include <assert.h>


#define DIS_PACHPIPE_MSG_BEGIN \
bool OnMessageReceived(const std::shared_ptr<ipc::Message>& spMsg, uint32_t nType) {\
bool hasHandle = false;\
do {\

#define DIS_PACHPIPE_MSG(msgType, fun)\
if (msgType == nType) {\
    hasHandle = this->fun(spMsg, msgType);\
    break;\
}\

#define DIS_PACHPIPE_RANGE_MSG(msgTypeBeg, msgTypeEnd, fun)\
if (nType >= msgTypeBeg && nType <= msgTypeEnd) {\
    hasHandle = this->fun(spMsg, nType); \
    break;\
}\

#define TRANSPIPE_MSG(pObj) \
if (pObj != nullptr) {\
    if (pObj->OnMessageReceived(spMsg, nType)) {\
        hasHandle = true;\
        break;\
    }\
}

#define DIS_PACHPIPE_MSG_END \
}while(0);\
return hasHandle;}

/** �ܵ���Ϣ�ӿ�
*/
template<class T>
class PipeListener : public ipc::ChannelListener
{
public:
    template<class T>
    PipeListener(T* msgHandlerImpl, const std::weak_ptr<NSP_DDM::simple_task_thread>& spMsgLoop) :
        m_pMsgHandlerImpl(msgHandlerImpl),
        m_spPipeMsgLoop(spMsgLoop)
    {
        // ����ֻ�����ԣ��ڲ������к����õ��ó�Աʱ�򶼲���ȥ�жϿ�
        assert(m_pMsgHandlerImpl != nullptr);
    }

    /** ��������
    */
    virtual ~PipeListener()
    {
        std::shared_ptr<NSP_DDM::simple_task_thread> msgLoop = m_spPipeMsgLoop.lock();

        if (msgLoop != nullptr)
        {
            msgLoop->get_task_queue().stop(MAX_U32);
        }
    }

    /** ͨѶͨ���ɹ�����һ����Ϣʱ�Ļص�����
    �Ƿ����˸���Ϣ��true��ʾ���Դ���false��ʾû�д���
    @note �������
    @param[in] message ��Ϣ
    @return �Ƿ�������Ϣ
    */
    virtual bool OnMessageReceived(const ipc::Message& message)
    {
        std::shared_ptr<ipc::Message> spMsg(new (std::nothrow)ipc::Message(message));
        if (spMsg == nullptr)
        {
            return false;
        }

        std::shared_ptr<NSP_DDM::simple_task_thread> msgLoop = m_spPipeMsgLoop.lock();
        if (msgLoop == nullptr)
        {
            return false;
        }

        T* tmp = m_pMsgHandlerImpl;
        msgLoop->get_task_queue().push_task([tmp, spMsg]()
        {
            return tmp->OnMessageReceived(spMsg, spMsg->ReadUInt32());
        });

        return true;
    }

    /**ͨѶͨ����������ʱ�Ļص�����
    @note �������
    @param [in] sender �������
    */
    virtual void OnChannelError(void* sender)
    {
        sender;
    }

protected:
    /** �ܵ���Ϣѭ��
    */
    std::weak_ptr<NSP_DDM::simple_task_thread> m_spPipeMsgLoop;

    /** �����Ĵ�����Ϣ��ʵ�֣������������ڱ����m_spPipeMsgLoop����Ҳ����˵��this��
    */
    T* m_pMsgHandlerImpl;
};

#endif // __PIPE_LISTENER_H_
