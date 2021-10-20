
#include "PipeHelper.h"

#include "msgdef/C2SMsgDef.h"
#include "msgdef/S2CMsgDef.h"

#include "random.h"
#include "thread/async/simple_event.h"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
PipeHelper::PipeHelper() :
    m_hasInit(false)
{
}

std::shared_ptr<PipeManager> PipeHelper::GetPipeManager()
{
    return m_spPipeManager;
}

ipc::Message* PipeHelper::GetReplyMsg(const std::wstring& name, uint64_t messageId)
{
    if (m_spPipeManager != nullptr)
    {
        return m_spPipeManager->GetReplyMsg(name, messageId);
    }

    return nullptr;
}

//////////////////////////////////////////////////////////////////////////

ClientPipeHelper::ClientPipeHelper()
{
}

ClientPipeHelper::~ClientPipeHelper()
{
    Uninit();
}

bool ClientPipeHelper::InitPipe(const std::shared_ptr<ipc::ChannelListener>& spListener,
                                  std::weak_ptr<NSP_DDM::simple_task_thread> spPipeMsgLoop)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    assert(spListener != nullptr);

    if (m_hasInit)
    {
        return true;
    }

    m_spPipeMsgLoop = spPipeMsgLoop;
    m_spPipeManager.reset(new (std::nothrow) PipeManager());

    if (m_spPipeManager == nullptr)
    {
        return false;
    }

    std::shared_ptr<PipeManager> spManager = m_spPipeManager;

    // �ܵ�ͨ�ų�ʼ��
    spManager->SetIsServer(false);
    spManager->SetListen(spListener);
    spManager->StartLoop();

    // ע��һ�Σ����ǲ������Ƿ�ɹ�
    Regist();

    m_hasInit = true;
    return true;
}

void ClientPipeHelper::Uninit()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    std::shared_ptr<PipeManager> spManager = m_spPipeManager;

    if (m_hasInit && spManager != nullptr)
    {
        spManager->StopLoop();
        m_spPipeManager = nullptr;
    }

    m_hasInit = false;
}

bool ClientPipeHelper::Regist()
{
    std::shared_ptr<NSP_DDM::simple_task_thread> it = m_spPipeMsgLoop.lock();

    if (it == nullptr)
    {
        return false;
    }

    it->get_task_queue().push_task([this]()
    {
        // TODO ���У��
        std::shared_ptr<PipeManager> spManager = m_spPipeManager;
        if (spManager == nullptr)
        {
            return false;
        }

        bool reuslt = false;
        for (size_t i = 0; i < 2; ++i)
        {
            const std::wstring name = REGIST_PIPE_NAME;

            // ����һ��ע��ܵ�
            if (!spManager->CreatePipe(name, false, true))
            {
                continue;
            }

            // ��һ�����֣�����
            ipc::Message* message1 = new ipc::Message();
            message1->set_sync();
            NSP_DDM::simple_event wait_event;
            message1->SetPumpMessageEvent(&wait_event);
            message1->set_sync_send_timeout(PIPE_DEFAULT_TIMEOUT);
            message1->WriteUInt32((NSP_DDM::u8)IPC_C2S_MSG_DEF::IPC_C2S_MSG_DEF_REGIST);
            uint64_t msgId1 = message1->GetMessageID();
            spManager->Send(name, message1);

            // �ڶ������֣�����
            ipc::Message* message2 = spManager->GetReplyMsg(name, msgId1);

            if (message2 != nullptr)
            {
                // �������
                std::wstring newName = message2->ReadString();

                // �ظ��ڶ������֣����������֣����ͣ�
                // ��֪�������Ѿ��յ��ظ���
                ipc::Message* message3 = ipc::Message::GenerateReply(message2);
                spManager->Send(name, message3);

                // �����µĹܵ�
                if (spManager->CreatePipe(newName, false, true))
                {
                    reuslt = true;
                    this->SetName(newName);
                }

                delete message2;
            }

            if (reuslt)
            {
                break;
            }

            // ���ʧ�ܣ��Ͽ�ע��ܵ���
            // ����ɹ���ȴ������������Ͽ���������������ܻ��ղ������һ��������Ϣ
            spManager->Close(name);
        }

        return reuslt;
    });

    return true;
}

void ClientPipeHelper::SetName(const std::wstring& name)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_name = name;
}

bool ClientPipeHelper::Send(ipc::Message* message)
{
    std::shared_ptr<PipeManager> spManager = m_spPipeManager;
    if (spManager == nullptr)
    {
        delete message;
        return false;
    }

    if (spManager->Send(m_name, message))
    {
        return true;
    }

    // ����ʧ���ˣ�����ܵ������ھ͹ص�����Ȼ������ע��
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    SetName(L"");
    spManager->Close(m_name);
    Regist();
    return false;
}

const std::wstring& ClientPipeHelper::GetName()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_name;
}

bool ClientPipeHelper::IsValid()
{
    std::shared_ptr<PipeManager> spManager = m_spPipeManager;
    if (spManager == nullptr)
    {
        return false;
    }

    bool isValid = false;
    spManager->DoPipe(GetName(), [&isValid](SPSyncPipe pipe)
    {
        if (!pipe->IsClosed())
        {
            isValid = true;
        }
    });

    return isValid;
}

////////////////////////////////////////////////////////////////////////
// TODO ���У��
void SendTestMsg(std::shared_ptr<PipeManager> spManager, const std::wstring& newName)
{
    // ��һ��������Ϣ
    ipc::Message* message1 = new ipc::Message();
    message1->set_sync();
    NSP_DDM::simple_event waitEvent;
    message1->SetPumpMessageEvent(&waitEvent);
    message1->set_sync_send_timeout(PIPE_DEFAULT_TIMEOUT);
    message1->WriteUInt32((uint32_t)IPC_S2C_MSG_DEF::IPC_S2C_MSG_DEF_TEST);
    std::wstring testMsg = L"server ע��ɹ�";
    testMsg += newName;
    message1->WriteString(testMsg);
    uint64_t msgId1 = message1->GetMessageID();
    spManager->Send(newName, message1);
    ipc::Message* message5 = spManager->GetReplyMsg(newName, msgId1);

    if (message5 != nullptr)
    {
        // �������
        std::wstring testMsgFromClient = message5->ReadString();
        // MessageBox(0, testMsgFromClient.c_str(), 0, 0);
    }
}

//////////////////////////////////////////////////////////////////////////
ServerPipeHelper::ServerPipeHelper()
{

}

ServerPipeHelper::~ServerPipeHelper()
{
    Uninit();
}

bool ServerPipeHelper::InitPipe(const std::shared_ptr<ipc::ChannelListener>& spListener)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    assert(spListener != nullptr);

    if (m_hasInit)
    {
        return true;
    }

    m_spPipeManager.reset(new PipeManager());

    if (m_spPipeManager == nullptr)
    {
        return false;
    }

    std::shared_ptr<PipeManager> spManager = m_spPipeManager;

    // �ܵ�ͨ�ų�ʼ��
    spManager->SetIsServer(true);
    spManager->SetListen(spListener);
    spManager->StartLoop();
    m_hasInit = true;

    // ����һ��ע��ܵ�
    return spManager->CreatePipe(REGIST_PIPE_NAME, true);
}

void ServerPipeHelper::Uninit()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    std::shared_ptr<PipeManager> spManager = m_spPipeManager;
    if (m_hasInit && spManager != nullptr)
    {
        spManager->StopLoop();
        m_spPipeManager = nullptr;
    }

    m_hasInit = false;
}

bool ServerPipeHelper::OnRegist(const std::shared_ptr<ipc::Message>& spMsg)
{
     std::shared_ptr<PipeManager> spManager = m_spPipeManager;
     bool result = [&spManager, &spMsg]()
     {
         if (spManager == nullptr)
         {
             return false;
         }
 
         // ��һ�����֣�����
         std::wstring name = spMsg->get_sender_name();

         if (name != REGIST_PIPE_NAME)
         {
             return false;
         }

         NSP_DDM::ddstrw guid;
         NSP_DDM::random::gen_guid(guid);
         std::wstring newName =
             std::wstring(L"Pipe") + guid + L"_" + L"1.0.0.1";

         // �����µĹܵ�
         if (!spManager->CreatePipe(newName, true, true))
         {
             return false;
         }

         // �ظ���һ�����֣��ڶ������֣����ͣ�
         ipc::Message* message2 = ipc::Message::GenerateReply(spMsg.get());
 
         if (message2 == nullptr)
         {
             return false;
         }
 
         message2->set_sync();
         message2->SetMessageId(spMsg->GetMessageID());
         NSP_DDM::simple_event wait_event;
         message2->SetPumpMessageEvent(&wait_event);
         message2->set_sync_send_timeout(PIPE_DEFAULT_TIMEOUT);
         message2->WriteString(newName);
         uint64_t message2Id = message2->GetMessageID();
         spManager->Send(name, message2);
 
         // ���������֣�����
         ipc::Message* message3 = spManager->GetReplyMsg(name, message2Id);
 
         if (message3 != nullptr)
         {
             delete message3;
 
             #ifdef _DEBUG
             {
                 //SendTestMsg(spManager, newName);
             }
             #endif // _DEBUG
         }
         else
         {
             spManager->Close(newName);
             return false;
         }

         return true;
     }();

     spManager->CloseAndReCreate(REGIST_PIPE_NAME, true);
     return result;
}

bool ServerPipeHelper::Send(const std::wstring& pipeName, ipc::Message* message)
{
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (!m_hasInit)
        {
            delete message;
            return false;
        }
    }

    std::shared_ptr<PipeManager> spManager = m_spPipeManager;
    if (spManager == nullptr)
    {
        delete message;
        return false;
    }

    bool result = spManager->Send(pipeName, message);

    if (!result)
    {
        spManager->Close(pipeName);
    }

    return result;
}

