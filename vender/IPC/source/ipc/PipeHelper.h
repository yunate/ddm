
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
    /** ���캯��
    */
    PipeHelper();

    /** ��ùܵ�������
    @return �ܵ�������
    */
    std::shared_ptr<PipeManager> GetPipeManager();

    /** ���ͬ����Ϣ�Ļظ�
    @param [in] name �ܵ�����
    @param [in] messageId ��Ϣid
    @return ����message���ⲿ�����ͷ�
    */
    ipc::Message* GetReplyMsg(const std::wstring& name, uint64_t messageId);

protected:
    /** �ܵ�������
    */
    std::shared_ptr<PipeManager> m_spPipeManager;

    /** �Ƿ��Ѿ���ʼ��
    */
    bool m_hasInit;

    /** ͬ����
    */
    std::recursive_mutex m_mutex;
};

/** �ͻ��˹ܵ�������ʼ����
*/
class ClientPipeHelper :
    public PipeHelper
{
public:
    /** ���캯��
    */
    ClientPipeHelper();

    /** ��������
    */
    ~ClientPipeHelper();

public:
    /** ��ʼ���ͻ���
    @param [in] spListener �۲��ߣ�ע��ö������������һ��Ҫ����pipe����Ϣѭ��
    @param [in] spPipeMsgLoop ��Ϣѭ��
    @return �Ƿ�ɹ�
    */
    bool InitPipe(const std::shared_ptr<ipc::ChannelListener>& spListener,
                  std::weak_ptr<NSP_DDM::simple_task_thread> spPipeMsgLoop);

    /** ����ʼ��
    */
    void Uninit();

    /** �ͻ����������ע��ܵ���Ӧ�÷ŵ��߳���ִ��
    @return �Ƿ�ɹ����첽��ע�ᣬ����trueʱ���ܱ�֤�ܵ��Ѿ���������
    */
    bool Regist();

    /** ���ùܵ�����
    @param [in] name ����
    */
    void SetName(const std::wstring& name);

    /** ������Ϣ������ʧ��ʱ���ȥ����Ƿ�Ҫ����ע��
    @param [in] message ��Ҫ���͵���Ϣ���ڲ������ͷ�
    @reutrn �Ƿ�ɹ�
    */
    bool Send(ipc::Message* message);

    /** ��ùܵ�����
    @return name ����
    */
    const std::wstring& GetName();

    /** �ܵ��Ƿ���Ч�����Լ�飬������ǿ��飨����������Ϣ��飩
    @return �Ƿ���Ч
    */
    bool IsValid();

private:
    /** �ܵ�����
    */
    std::wstring m_name;

    /** ��Ϣѭ��
    */
    std::weak_ptr<NSP_DDM::simple_task_thread> m_spPipeMsgLoop;
};

/** �������ܵ�������ʼ����
*/
class ServerPipeHelper :
    public PipeHelper
{
public:
    /** ���캯��
    */
    ServerPipeHelper();

    /** ��������
    */
    ~ServerPipeHelper();

    /** ��ʼ���ͻ���
    @param [in] spListener �۲��ߣ�ע��ö������������һ��Ҫ����pipe����Ϣѭ��
    @return �Ƿ�ɹ�
    */
    bool InitPipe(const std::shared_ptr<ipc::ChannelListener>& spListener);

    /** ����ʼ��
    */
    void Uninit();

    /** �ͻ����������ע��ܵ���Ӧ�÷ŵ��߳���ִ��
    @param [in] message ��Ϣ
    @return �Ƿ�ɹ�
    */
    bool OnRegist(const std::shared_ptr<ipc::Message>& spMsg);

    /** ������Ϣ
    @param [in] pipeName �ܵ�����
    @param [in] message ��Ҫ���͵���Ϣ���ڲ������ͷ�
    @return �Ƿ�ɹ�
    */
    bool Send(const std::wstring& pipeName, ipc::Message* message);
};


#endif // __PIPE_HELPER_H_
