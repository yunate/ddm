
#include "PipeManager.h"

#include "ipc_win/ipc_message.h"
#include "ipc_win/ipc_message_loop.h"
#include "ipc_win/ipc_thread.h"
#include "msgdef/CommonMsgDef.h"

#include <assert.h>


/** ����Ƶ��
*/
const int32_t HEART_BEAT_TIMER = 1000;

/** ע��ܵ����Ӻ������ʱ�� 10s
*/
const int32_t REGIST_MAX_CONNECTED_TIMER = 10000;

/** ע��ܵ������������ʱ�� 1min
*/
const int32_t REGIST_MAX_ACTIVE_TIMER = 60000;

PipeManager::PipeManager() :
    m_isHeartBeatRunning(false),
    m_isLoopStart(false),
    m_isServer(false)
{
}

PipeManager::~PipeManager()
{
    StopLoop();
}

void PipeManager::SetListen(const std::shared_ptr<ipc::ChannelListener>& spListen)
{
    m_spListener = spListen;
}

void PipeManager::StartLoop()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if (m_isLoopStart)
    {
        return;
    }

    // ������Ϣѭ��
    m_spLoopThread.reset(ipc::Thread::CreateThread(ipc::MessageLoopForIPC::GetInstace(), L"message_loop_thread"));
    m_spLoopThread->Start();

    // ���������߳�
    m_isHeartBeatRunning = true;
    m_spHeartBeatThread.reset(new std::thread(&PipeManager::HeartBeatCallBack, this));
    m_isLoopStart = true;
}

void PipeManager::StopLoop()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if (!m_isLoopStart)
    {
        return;
    }

    m_isLoopStart = false;

    if (m_spLoopThread != nullptr && m_spLoopThread->IsRunning())
    {
        m_spLoopThread->QuitMessageLoop();
        m_spLoopThread->time_wait(PIPE_DEFAULT_TIMEOUT);
        m_spLoopThread.reset(nullptr);
    }

    m_isHeartBeatRunning = false;

    if (m_spHeartBeatThread != nullptr && m_spHeartBeatThread->joinable())
    {
        m_spHeartBeatThread->join();
        m_spHeartBeatThread.reset(nullptr);
    }

    for (auto& it : m_msgPipe)
    {
        SPSyncPipe tmp = it.second;
        if (tmp != nullptr)
        {
            tmp->Close();
        }
    }

    m_msgPipe.clear();
}

void PipeManager::SetIsServer(bool isServer)
{
    m_isServer = isServer;
}

bool PipeManager::IsServer()
{
    return m_isServer;
}

bool PipeManager::CreatePipe(const std::wstring& name, bool isServer, bool sync/* = false*/)
{
    bool result = false;

    if (sync)
    {
        std::shared_ptr<bool> spResult(new (std::nothrow)bool(false));

        if (spResult == nullptr)
        {
            return false;
        }

        std::shared_ptr<NSP_DDM::simple_event> spWaitEvent(new(std::nothrow)NSP_DDM::simple_event());

        if (spWaitEvent == nullptr)
        {
            return false;
        }

        // �ŵ���Ϣ�����У������ڲ���pipe��ʼ�������첽����
        ipc::MessageLoopForIPC::GetInstace()->PostTask(
            ipc::Bind(&PipeManager::OnCreate,
                      this,
                      name,
                      isServer,
                      std::shared_ptr<NSP_DDM::simple_event>(spWaitEvent),
                      std::shared_ptr<bool>(spResult)));

        spWaitEvent->time_wait(PIPE_DEFAULT_TIMEOUT);
        result = *spResult;
    }
    else
    {
        // �ŵ���Ϣ�����У������ڲ���pipe��ʼ�������첽����
        ipc::MessageLoopForIPC::GetInstace()->PostTask(
            ipc::Bind(&PipeManager::OnCreate,
                      this,
                      name,
                      isServer,
                      std::shared_ptr<NSP_DDM::simple_event>(),
                      std::shared_ptr<bool>()));

        result = true;
    }

    return result;
}

void PipeManager::OnCreate(const std::wstring& name,
                             bool isServer,
                             const std::weak_ptr<NSP_DDM::simple_event>& spEvent,
                             const std::weak_ptr<bool>& spResult)
{
    bool result = [&name, &isServer, this]()
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        // ѭ��û�����������߹ر���
        if (!m_isLoopStart)
        {
            assert(false);
            return false;
        }

        auto it = m_msgPipe.find(name);

        // �Ѿ�����
        if (it != m_msgPipe.end())
        {
            SPSyncPipe tmp = it->second;

            if (!isServer)
            {
                // �����Ѿ����ر��ˣ������̻߳�û���ü�������ǰ����
                if (tmp == nullptr || tmp->IsClosed())
                {
                    m_msgPipe.erase(it);
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        ipc::ChannelListener* listen = m_spListener.get();
        ipc::SyncChannelPipe* pipe = nullptr;

        if (isServer)
        {
            pipe = ipc::SyncChannelPipe::CreateServer(name, listen);
        }
        else
        {
            pipe = ipc::SyncChannelPipe::CreateClient(name, listen);
        }

        if (pipe == nullptr)
        {
            return false;
        }

        m_msgPipe.insert({name, SPSyncPipe(pipe)});

        return true;
    }();

    auto sharedResult = spResult.lock();
    if (sharedResult != nullptr)
    {
        *sharedResult = result;
    }

    auto waitEvent = spEvent.lock();
    if (waitEvent != nullptr)
    {
        waitEvent->set_event();
    }
}

void PipeManager::Close(const std::wstring& name, bool sync/* = false*/)
{
    SPSyncPipe pipe = nullptr;
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        auto it = m_msgPipe.find(name);

        if (it == m_msgPipe.end())
        {
            return;
        }

        pipe = it->second;
        m_msgPipe.erase(it);
    }

    if (sync)
    {
        std::shared_ptr<NSP_DDM::simple_event> spWaitEvent(new (std::nothrow)NSP_DDM::simple_event());

        if (spWaitEvent == nullptr)
        {
            return;
        }

        // �ŵ���Ϣ�����У������ڲ���pipe��ʼ�������첽����
        ipc::MessageLoopForIPC::GetInstace()->PostTask(
            ipc::Bind(&PipeManager::OnClose, this, pipe, std::weak_ptr<NSP_DDM::simple_event>(spWaitEvent)));

        spWaitEvent->time_wait(PIPE_DEFAULT_TIMEOUT);
    }
    else
    {
        // �ŵ���Ϣ�����У������ڲ���pipe��ʼ�������첽����
        ipc::MessageLoopForIPC::GetInstace()->PostTask(
            ipc::Bind(&PipeManager::OnClose, this, pipe, std::weak_ptr<NSP_DDM::simple_event>()));
    }
}

void PipeManager::OnClose(SPSyncPipe spPipe, const std::weak_ptr<NSP_DDM::simple_event>& spEvent)
{
    if (spPipe != nullptr)
    {
        spPipe->Close();
    }

    auto waitEvent = spEvent.lock();
    if (waitEvent != nullptr)
    {
        waitEvent->set_event();
    }
}

bool PipeManager::CloseAndReCreate(const std::wstring& name, bool isServer, bool sync/* = false*/)
{
    SPSyncPipe pipe = nullptr;
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        auto it = m_msgPipe.find(name);

        if (it != m_msgPipe.end())
        {
            pipe = it->second;
            m_msgPipe.erase(it);
        }
    }

    bool result = false;

    if (sync)
    {
        std::shared_ptr<bool> spResult(new (std::nothrow)bool(false));

        if (spResult == nullptr)
        {
            return false;
        }

        std::shared_ptr<NSP_DDM::simple_event> spWaitEvent(new(std::nothrow)NSP_DDM::simple_event());

        if (spWaitEvent == nullptr)
        {
            return false;
        }

        // �ŵ���Ϣ�����У������ڲ���pipe��������첽����
        ipc::MessageLoopForIPC::GetInstace()->PostTask(
            ipc::Bind(&PipeManager::OnCloseAndReCreate,
                      this,
                      name,
                      pipe,
                      isServer,
                      std::weak_ptr<NSP_DDM::simple_event>(spWaitEvent),
                      std::weak_ptr<bool>(spResult)));

        spWaitEvent->time_wait(PIPE_DEFAULT_TIMEOUT);
        result = *spResult;
    }
    else
    {
        // �ŵ���Ϣ�����У������ڲ���pipe��������첽����
        ipc::MessageLoopForIPC::GetInstace()->PostTask(
            ipc::Bind(&PipeManager::OnCloseAndReCreate,
                      this,
                      name,
                      pipe,
                      isServer,
                      std::weak_ptr<NSP_DDM::simple_event>(),
                      std::weak_ptr<bool>()));

        result = true;
    }

    return result;
}

void PipeManager::OnCloseAndReCreate(const std::wstring& name,
                                       SPSyncPipe pipe,
                                       bool isServer,
                                       const std::weak_ptr<NSP_DDM::simple_event>& spEvent,
                                       const std::weak_ptr<bool>& spResult)
{
    if (pipe != nullptr)
    {
        pipe->Close();
    }

    OnCreate(name, isServer, spEvent, spResult);
}

void PipeManager::HeartBeatCallBack()
{
    while (m_isHeartBeatRunning)
    {
        // �رյĹܵ�����
        HeartBeatClosedPipe();

        // �����ע��ܵ�����
        HeartBeatRegistPipe();

        // TODO: ���ڿ��йܵ�����
        // TODO: �ͻ��˷��ͼ����Ϣ�����û�з��أ�������������д���

        // �������
        std::this_thread::sleep_for(std::chrono::microseconds(HEART_BEAT_TIMER));
    }
}

void PipeManager::HeartBeatClosedPipe()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    auto it = m_msgPipe.begin();

    while (it != m_msgPipe.end())
    {
        if (it->second == nullptr || it->second->IsClosed())
        {
            it = m_msgPipe.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void PipeManager::HeartBeatRegistPipe()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    SPSyncPipe registPipe = nullptr;
    std::unordered_map<std::wstring, SPSyncPipe>::iterator it = m_msgPipe.begin();

    while (it != m_msgPipe.end())
    {
        if (it->first == REGIST_PIPE_NAME &&
            it->second != nullptr)
        {
            registPipe = it->second;
            break;
        }

        ++it;
    }

    if (registPipe == nullptr)
    {
        if (IsServer())
        {
            // ע��ܵ������ڣ������������´���
            CreatePipe(REGIST_PIPE_NAME, true, true);
        }
    }
    else if (registPipe->IsClosed())
    {
        if (registPipe->IsServer())
        {
            // �Ѿ��ر��ˣ�����һ�顰�ر�--���´�����
            CloseAndReCreate(REGIST_PIPE_NAME, true);
        }
        else
        {
            Close(REGIST_PIPE_NAME);
        }
    }
    else
    {
        auto dur = std::chrono::high_resolution_clock::now().time_since_epoch();
        auto nowTime = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        std::chrono::milliseconds::rep recordTime = 0;

        if (!registPipe->IsWaitIngConnect())
        {
            // �ܵ��Ѿ����ӵ�ʱ�򣬲鿴���ӵ�ʱ�䲻�ܳ���10s
            recordTime = registPipe->GetPipeConnectedTime() + REGIST_MAX_CONNECTED_TIMER;
        }
        else
        {
            // �ܵ�û�����ӵ�ʱ�򣬴���ʱ�䲻�ܳ���60s
            recordTime = registPipe->GetPipePrevActiveTime() + REGIST_MAX_ACTIVE_TIMER;
        }

        if (recordTime < nowTime)
        {
            if (registPipe->IsServer())
            {
                // �ܵ����ʱ��������ر����´���
                CloseAndReCreate(REGIST_PIPE_NAME, true);
            }
            else
            {
                // �ͻ���ֱ�ӹر�
                Close(REGIST_PIPE_NAME);
            }
        }
    }
}

bool PipeManager::Send(const std::wstring& name, ipc::Message* message)
{
    SPSyncPipe tmp = nullptr;
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        auto it = m_msgPipe.find(name);

        if (it != m_msgPipe.end())
        {
            tmp = it->second;
        }
    }

    if (tmp == nullptr || tmp->IsClosed())
    {
        delete message;
        return false;
    }

    bool result = tmp->Send(message);

    if (!result)
    {
        delete message;
    }

    return result;
}

ipc::Message* PipeManager::GetReplyMsg(const std::wstring& name, uint64_t messageId)
{
    SPSyncPipe tmp = nullptr;
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        auto it = m_msgPipe.find(name);

        if (it == m_msgPipe.end())
        {
            return false;
        }

        tmp = it->second;
    }

    if (tmp == nullptr)
    {
        return false;
    }

    return tmp->GetReplayMessage(messageId);
}

