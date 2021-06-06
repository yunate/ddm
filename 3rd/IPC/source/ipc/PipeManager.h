
#ifndef __PipeManager_h_
#define __PipeManager_h_ 1

#include "ipc_win/ipc_callback.h"
#include "ipc_win/ipc_sync_channel_pipe.h"

#include "thread/async/simple_event.h"
#include <memory>
#include <thread>
#include <mutex>
#include <unordered_map>

// ǰ׺����
namespace ipc
{
    class Message;
    class SyncChannelPipe;
    class Thread;
    class ChannelListener;
}

/** �ܵ���ʱ��ms��
*/
const int32_t PIPE_DEFAULT_TIMEOUT = 1000;

/** ע��ܵ�����
*/
const std::wstring REGIST_PIPE_NAME = std::wstring(L"_regist_pipe_") + L"0.0.0.1";

/** ͬ���ܵ�����ָ��
*/
using SPSyncPipe = std::shared_ptr<ipc::SyncChannelPipe>;

/** �ܵ�������
*/
class PipeManager : public ipc::SupportWeakCallback
{
public:
    /** ���캯��
    */
    PipeManager();

    /** ��������
    */
    ~PipeManager();

    /** ���ü�����
    @param [in] spListen ������
    */
    void SetListen(const std::shared_ptr<ipc::ChannelListener>& spListen);

    /** ����ѭ��
    */
    void StartLoop();

    /** �ر�ѭ��
    */
    void StopLoop();

    /** �����Ƿ��Ƿ�����
    @param [in] isServer �Ƿ��Ƿ�����
    */
    void SetIsServer(bool isServer);

    /** �Ƿ��Ƿ�����
    @return �Ƿ��Ƿ�����
    */
    bool IsServer();
public:
    /** �����ܵ�
    @param [in] name �ܵ�����
    @param [in] isServer �Ƿ��Ƿ�����
    @param [in] sync true ͬ��ģʽ�������ߵȴ�CreatePipe�Ļص�����ִ�����
    @return �Ƿ�ɹ���ֻ��syncΪtrueʱ��Ч
    */
    bool CreatePipe(const std::wstring& name, bool isServer, bool sync = false);

    /** �رչܵ�
    @param [in] name �ܵ�����
    @param [in] sync true ͬ��ģʽ�������ߵȴ�Close�Ļص�����ִ�����
    */
    void Close(const std::wstring& name, bool sync = false);

    /** �رղ����´���
    @param [in] name �ܵ�����
    @param [in] isServer ��������Ϊfalseʱ�����ͻ��ˣ����򴴽�������
    @param [in] sync true ͬ��ģʽ�������ߵȴ�CloseAndReCreate�Ļص�����ִ�����
    @return ���´����ܵ��Ƿ�ɹ���ֻ��syncΪtrueʱ��Ч
    */
    bool CloseAndReCreate(const std::wstring& name, bool isServer, bool sync = false);

    /** ��ÿ���ܵ�����
    @param [in] callable �ɵ��õģ�����falseֹͣ����
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

    /** ���ض��ܵ���������������򲻵���
    @param [in] pipeName �ܵ�����
    @param [in] callable �ɵ��õ�
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
    /** �������ص�
    */
    void HeartBeatCallBack();

    /** �رյĹܵ�����
    */
    void HeartBeatClosedPipe();

    /** ��������ע��ܵ�����
    */
    void HeartBeatRegistPipe();

public:
    /** ��ͨѶͨ���з���һ����Ϣ
        message ������new�����ģ��ڲ������ͷţ��˺������ù���Ӧ�ò���ʹ��message��
        ����message->GetMessageID();Ӧ�÷ŵ�send֮ǰ���ã��������Ұָ�����
    @param [in] name �ܵ�����
    @param [in] message chromium ��Ϣ���ڲ���ֵ�ͷ�
    @return �Ƿ�ɹ�
    */ 
    bool Send(const std::wstring& name, ipc::Message* message);

    /** ���ͬ����Ϣ�Ļظ�
    @param [in] name �ܵ�����
    @param [in] messageId ��Ϣid
    @return ����message���ⲿ�����ͷ�
    */
    ipc::Message* GetReplyMsg(const std::wstring& name, uint64_t messageId);

    /** create �Ļص�
    @param [in] name �ܵ�����
    @param [in] isServer ��������Ϊfalseʱ�����ͻ��ˣ����򴴽�������
    @param [in] spEvent ͬ���ĵȴ��¼�
    @param [out] spResult ͬ����ʱ���ִ�н��
    */
    void OnCreate(const std::wstring& name,
                  bool isServer,
                  const std::weak_ptr<NSP_DDM::simple_event>& spEvent,
                  const std::weak_ptr<bool>& spResult);

    /** close �����Ļص�����
    @param [in] spPipe �ܵ�
    @param [in] spEvent ͬ���ĵȴ��¼�
    */
    void OnClose(SPSyncPipe spPipe, const std::weak_ptr<NSP_DDM::simple_event>& spEvent);

    /** �رղ����´����Ļص�����
    @param [in] name �ܵ�����
    @param [in] pipe �ܵ�
    @param [in] isServer ��������Ϊfalseʱ�����ͻ��ˣ����򴴽�������
    @param [in] spEvent ͬ���ĵȴ��¼�
    @param [out] spResult ͬ����ʱ���ִ�н��
    */
    void OnCloseAndReCreate(const std::wstring& name,
                            SPSyncPipe pipe,
                            bool isServer,
                            const std::weak_ptr<NSP_DDM::simple_event>& spEvent,
                            const std::weak_ptr<bool>& spResult);

private:
    /** �ܵ�map���ܵ�����-ͬ���ܵ�
    */
    std::unordered_map<std::wstring, SPSyncPipe> m_msgPipe;

    /** ͬ����
    */
    std::recursive_mutex m_mutex;

    /** ������--�۲���
    */
    std::shared_ptr<ipc::ChannelListener> m_spListener;

    /** chromium ר���߳�
    */
    std::unique_ptr<ipc::Thread> m_spLoopThread;

    /** �����߳�
    */
    std::unique_ptr <std::thread> m_spHeartBeatThread;

    /** �����߳��Ƿ������У����ڹر�����
    */
    bool m_isHeartBeatRunning;

    /** ѭ���Ƿ�����
    */
    bool m_isLoopStart;

    /** �Ƿ�������
    */
    bool m_isServer;
};

#endif // __PipeManager_h_
