
#include "ipc/PipeListener.h"
#include "ipc/PipeHelper.h"

#include <iostream>

class CLient
{
public:
    DIS_PACHPIPE_MSG_BEGIN
        DIS_PACHPIPE_MSG(6, OnServerBackTest);
    DIS_PACHPIPE_MSG_END

    bool OnServerBackTest(const std::shared_ptr<ipc::Message>& spMsg, int ty)
    {
        ty;
        std::wstring pipeName = spMsg->ReadString();
        std::wstring str = spMsg->ReadString();
        std::wcout << str << std::endl;
        return true;
    }


    void TestSend()
    {
        std::wstring name = m_spPipeHelper->GetName();

        if (name.empty())
        {
            return;
        }

        ipc::Message * message = new ipc::Message();
        message->WriteUInt32(7);
        message->WriteString(L"Client ------> Server");
        m_spPipeHelper->Send(message);
    }

public:
    void Run()
    {
        std::shared_ptr<NSP_DDM::simple_task_thread> taskq(new NSP_DDM::simple_task_thread());
        taskq->start();
        std::shared_ptr<PipeListener<CLient> > spListen(new PipeListener<CLient>(this, taskq));

        // 注册管道
        m_spPipeHelper.reset(new (std::nothrow) ClientPipeHelper());
        m_spPipeHelper->InitPipe(spListen, taskq);

        while (true)
        {
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
            TestSend();
        }
    }

private:
    std::shared_ptr<ClientPipeHelper> m_spPipeHelper;
};

class Server
{
public:
    bool Onreg(const std::shared_ptr<ipc::Message>& spMsg, int ty)
    {
        ty;
        m_spPipeHelper->OnRegist(spMsg);
        return true;
    }

    bool OnClientTest(const std::shared_ptr<ipc::Message>& spMsg, int ty)
    {
        ty;
        std::wcout << spMsg->ReadString() << std::endl;
        return true;
    }

    DIS_PACHPIPE_MSG_BEGIN
            DIS_PACHPIPE_MSG((uint32_t)IPC_C2S_MSG_DEF::IPC_C2S_MSG_DEF_REGIST, Onreg);
             DIS_PACHPIPE_MSG(7, OnClientTest);
    DIS_PACHPIPE_MSG_END

public:
    void Run()
    {
        std::shared_ptr<NSP_DDM::simple_task_thread> taskq(new NSP_DDM::simple_task_thread());
        taskq->start();

        // 设置消息循环线程
        std::shared_ptr<PipeListener<Server> > spListen(new PipeListener<Server>(this, taskq));

        // 注册管道
        m_spPipeHelper.reset(new (std::nothrow) ServerPipeHelper());
        m_spPipeHelper->InitPipe(spListen);

        while (true)
        {
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(100));
        }
    }

private:
    std::shared_ptr<ServerPipeHelper> m_spPipeHelper;
};

int main()
{
    //CLient ddd;
    //ddd.Run();

     Server sss;
     sss.Run();
    return 1;
}
