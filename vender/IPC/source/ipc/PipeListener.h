
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

/** 管道消息接口
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
        // 这里只做断言，内部的所有函数用到该成员时候都不会去判断空
        assert(m_pMsgHandlerImpl != nullptr);
    }

    /** 析构函数
    */
    virtual ~PipeListener()
    {
        std::shared_ptr<NSP_DDM::simple_task_thread> msgLoop = m_spPipeMsgLoop.lock();

        if (msgLoop != nullptr)
        {
            msgLoop->get_task_queue().stop(MAX_U32);
        }
    }

    /** 通讯通道成功接收一个消息时的回调函数
    是否处理了该消息，true表示可以处理；false表示没有处理
    @note 详见基类
    @param[in] message 消息
    @return 是否处理了消息
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

    /**通讯通道发生错误时的回调函数
    @note 详见基类
    @param [in] sender 详见基类
    */
    virtual void OnChannelError(void* sender)
    {
        sender;
    }

protected:
    /** 管道消息循环
    */
    std::weak_ptr<NSP_DDM::simple_task_thread> m_spPipeMsgLoop;

    /** 真正的处理消息的实现，他的生命周期必须比m_spPipeMsgLoop长，也就是说比this长
    */
    T* m_pMsgHandlerImpl;
};

#endif // __PIPE_LISTENER_H_
