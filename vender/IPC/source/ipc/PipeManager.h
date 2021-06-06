
#ifndef __PipeManager_h_
#define __PipeManager_h_ 1

#include "ipc_win/ipc_callback.h"
#include "ipc_win/ipc_sync_channel_pipe.h"

#include "thread/async/simple_event.h"
#include <memory>
#include <thread>
#include <mutex>
#include <unordered_map>

// 前缀声明
namespace ipc
{
    class Message;
    class SyncChannelPipe;
    class Thread;
    class ChannelListener;
}

/** 管道超时（ms）
*/
const int32_t PIPE_DEFAULT_TIMEOUT = 1000;

/** 注册管道名称
*/
const std::wstring REGIST_PIPE_NAME = std::wstring(L"_regist_pipe_") + L"0.0.0.1";

/** 同步管道智能指针
*/
using SPSyncPipe = std::shared_ptr<ipc::SyncChannelPipe>;

/** 管道管理类
*/
class PipeManager : public ipc::SupportWeakCallback
{
public:
    /** 构造函数
    */
    PipeManager();

    /** 析构函数
    */
    ~PipeManager();

    /** 设置监听器
    @param [in] spListen 监听器
    */
    void SetListen(const std::shared_ptr<ipc::ChannelListener>& spListen);

    /** 开启循环
    */
    void StartLoop();

    /** 关闭循环
    */
    void StopLoop();

    /** 设置是否是服务器
    @param [in] isServer 是否是服务器
    */
    void SetIsServer(bool isServer);

    /** 是否是服务器
    @return 是否是服务器
    */
    bool IsServer();
public:
    /** 创建管道
    @param [in] name 管道名称
    @param [in] isServer 是否是服务器
    @param [in] sync true 同步模式，调用者等待CreatePipe的回调函数执行完成
    @return 是否成功，只有sync为true时有效
    */
    bool CreatePipe(const std::wstring& name, bool isServer, bool sync = false);

    /** 关闭管道
    @param [in] name 管道名称
    @param [in] sync true 同步模式，调用者等待Close的回调函数执行完成
    */
    void Close(const std::wstring& name, bool sync = false);

    /** 关闭并重新创建
    @param [in] name 管道名称
    @param [in] isServer 服务器吗，为false时创建客户端，否则创建服务器
    @param [in] sync true 同步模式，调用者等待CloseAndReCreate的回调函数执行完成
    @return 重新创建管道是否成功，只有sync为true时有效
    */
    bool CloseAndReCreate(const std::wstring& name, bool isServer, bool sync = false);

    /** 对每个管道处理
    @param [in] callable 可调用的，返回false停止迭代
    */
    template <class CALLABLE>
    void DoEachPipe(CALLABLE callable)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        std::unordered_map<std::wstring, SPSyncPipe>::iterator it = m_msgPipe.begin();

        while (it != m_msgPipe.end())
        {
            if (it->first != REGIST_PIPE_NAME)
            {
                if (!callable(it->first, it->second))
                {
                    break;
                }
            }

            ++it;
        }
    }

    /** 对特定管道处理，如果不存在则不调用
    @param [in] pipeName 管道名称
    @param [in] callable 可调用的
    */
    template <class CALLABLE>
    void DoPipe(const std::wstring& pipeName, CALLABLE callable)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        auto& it = m_msgPipe.find(pipeName);

        if (it != m_msgPipe.end())
        {
            callable(it->second);
        }
    }

public:
    /** 心跳检测回调
    */
    void HeartBeatCallBack();

    /** 关闭的管道处理
    */
    void HeartBeatClosedPipe();

    /** 服务器端注册管道处理
    */
    void HeartBeatRegistPipe();

public:
    /** 向通讯通道中发送一个消息
        message 必须是new出来的，内部负责释放，此函数调用过后应该不在使用message，
        比如message->GetMessageID();应该放到send之前调用，否则会有野指针风险
    @param [in] name 管道名称
    @param [in] message chromium 消息，内部赋值释放
    @return 是否成功
    */ 
    bool Send(const std::wstring& name, ipc::Message* message);

    /** 获得同步消息的回复
    @param [in] name 管道名称
    @param [in] messageId 消息id
    @return 返回message，外部负责释放
    */
    ipc::Message* GetReplyMsg(const std::wstring& name, uint64_t messageId);

    /** create 的回调
    @param [in] name 管道名称
    @param [in] isServer 服务器吗，为false时创建客户端，否则创建服务器
    @param [in] spEvent 同步的等待事件
    @param [out] spResult 同步的时候的执行结果
    */
    void OnCreate(const std::wstring& name,
                  bool isServer,
                  const std::weak_ptr<NSP_DDM::simple_event>& spEvent,
                  const std::weak_ptr<bool>& spResult);

    /** close 函数的回调函数
    @param [in] spPipe 管道
    @param [in] spEvent 同步的等待事件
    */
    void OnClose(SPSyncPipe spPipe, const std::weak_ptr<NSP_DDM::simple_event>& spEvent);

    /** 关闭并重新创建的回调函数
    @param [in] name 管道名称
    @param [in] pipe 管道
    @param [in] isServer 服务器吗，为false时创建客户端，否则创建服务器
    @param [in] spEvent 同步的等待事件
    @param [out] spResult 同步的时候的执行结果
    */
    void OnCloseAndReCreate(const std::wstring& name,
                            SPSyncPipe pipe,
                            bool isServer,
                            const std::weak_ptr<NSP_DDM::simple_event>& spEvent,
                            const std::weak_ptr<bool>& spResult);

private:
    /** 管道map表，管道名称-同步管道
    */
    std::unordered_map<std::wstring, SPSyncPipe> m_msgPipe;

    /** 同步量
    */
    std::recursive_mutex m_mutex;

    /** 监听器--观察者
    */
    std::shared_ptr<ipc::ChannelListener> m_spListener;

    /** chromium 专用线程
    */
    std::unique_ptr<ipc::Thread> m_spLoopThread;

    /** 心跳线程
    */
    std::unique_ptr <std::thread> m_spHeartBeatThread;

    /** 心跳线程是否运行中，用于关闭心跳
    */
    bool m_isHeartBeatRunning;

    /** 循环是否开启了
    */
    bool m_isLoopStart;

    /** 是服务器吗
    */
    bool m_isServer;
};

#endif // __PipeManager_h_
