
#ifndef __PIPE_HELPER_H_
#define __PIPE_HELPER_H_ 1

#include "ipc_win/ipc_message.h"
#include "PipeManager.h"
#include "msgdef/C2SMsgDef.h"
#include "msgdef/S2CMsgDef.h"

#include "thread/thread_manager/task_thread.h"
#include <memory>

class IPipeListener;

class PipeHelper
{
public:
    /** 构造函数
    */
    PipeHelper();

    /** 获得管道管理器
    @return 管道管理器
    */
    std::shared_ptr<PipeManager> GetPipeManager();

    /** 获得同步消息的回复
    @param [in] name 管道名称
    @param [in] messageId 消息id
    @return 返回message，外部负责释放
    */
    ipc::Message* GetReplyMsg(const std::wstring& name, uint64_t messageId);

protected:
    /** 管道管理器
    */
    std::shared_ptr<PipeManager> m_spPipeManager;

    /** 是否已经初始化
    */
    bool m_hasInit;

    /** 同步量
    */
    std::recursive_mutex m_mutex;
};

/** 客户端管道代理（初始化）
*/
class ClientPipeHelper :
    public PipeHelper
{
public:
    /** 构造函数
    */
    ClientPipeHelper();

    /** 析构函数
    */
    ~ClientPipeHelper();

public:
    /** 初始化客户端
    @param [in] spListener 观察者，注意该对象的生命周期一定要长于pipe的消息循环
    @param [in] spPipeMsgLoop 消息循环
    @return 是否成功
    */
    bool InitPipe(const std::shared_ptr<ipc::ChannelListener>& spListener,
                  std::weak_ptr<NSP_DDM::simple_task_thread> spPipeMsgLoop);

    /** 反初始化
    */
    void Uninit();

    /** 客户端向服务器注册管道，应该放到线程中执行
    @return 是否成功，异步的注册，返回true时候不能保证管道已经创建好了
    */
    bool Regist();

    /** 设置管道名称
    @param [in] name 名称
    */
    void SetName(const std::wstring& name);

    /** 发送消息，发送失败时候会去检查是否要重新注册
    @param [in] message 需要发送的消息，内部负责释放
    @reutrn 是否成功
    */
    bool Send(ipc::Message* message);

    /** 获得管道名称
    @return name 名称
    */
    const std::wstring& GetName();

    /** 管道是否有效，弱性检查，区别于强检查（发送心跳消息检查）
    @return 是否有效
    */
    bool IsValid();

private:
    /** 管道名称
    */
    std::wstring m_name;

    /** 消息循环
    */
    std::weak_ptr<NSP_DDM::simple_task_thread> m_spPipeMsgLoop;
};

/** 服务器管道代理（初始化）
*/
class ServerPipeHelper :
    public PipeHelper
{
public:
    /** 构造函数
    */
    ServerPipeHelper();

    /** 析构函数
    */
    ~ServerPipeHelper();

    /** 初始化客户端
    @param [in] spListener 观察者，注意该对象的生命周期一定要长于pipe的消息循环
    @return 是否成功
    */
    bool InitPipe(const std::shared_ptr<ipc::ChannelListener>& spListener);

    /** 反初始化
    */
    void Uninit();

    /** 客户端向服务器注册管道，应该放到线程中执行
    @param [in] message 消息
    @return 是否成功
    */
    bool OnRegist(const std::shared_ptr<ipc::Message>& spMsg);

    /** 发送消息
    @param [in] pipeName 管道名称
    @param [in] message 需要发送的消息，内部负责释放
    @return 是否成功
    */
    bool Send(const std::wstring& pipeName, ipc::Message* message);
};


#endif // __PIPE_HELPER_H_
