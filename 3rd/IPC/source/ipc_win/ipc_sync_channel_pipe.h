#ifndef GAMEHALLMAIN_IPC_IPC_SYNC_CHANNEL_PIPE_H_
#define GAMEHALLMAIN_IPC_IPC_SYNC_CHANNEL_PIPE_H_ 1

#include <chrono>
#include <memory>
#include <set>
#include <stdint.h>

#include "ipc_channel_listener.h"
#include <thread>
#include <mutex>

namespace ipc {
  // ǰ������
class Message;
struct PendingSyncMsg;
class ChannelPipe;

  // �ܵ���Ϣ��Ӧ�ӿڣ���Ҫ����IPC��Ϣ���඼��Ҫ�̳иýӿ�
class SyncChannelPipe : public ChannelListener {
 public:
  ~SyncChannelPipe();

  // ��ͨѶͨ���з���һ����Ϣ
  bool Send(Message* message);

  void Close();

  bool IsClosed();

  bool IsServer();


  // �Ƿ�ȴ�������
  bool IsWaitIngConnect();

  /** ��ùܵ����ӵ�ʱ��
  @return �ܵ����ӵ�ʱ��
  */
  std::chrono::milliseconds::rep GetPipeConnectedTime();

  /** ��ùܵ��ϴλ�Ծ��ʱ��
  @return �ϴλ�Ծ��ʱ��
  */
  std::chrono::milliseconds::rep GetPipePrevActiveTime();

  static SyncChannelPipe* CreateServer(const std::wstring& pipe_name, ChannelListener* listener);
  static SyncChannelPipe* CreateClient(const std::wstring& pipe_name, ChannelListener* listener);

  // ��ȡ�ظ���Ϣ,���÷�����ɾ��
  Message* GetReplayMessage(uint64_t messageid);

  // ͨѶͨ���ɹ�����һ����Ϣʱ�Ļص�����
  // �Ƿ����˸���Ϣ��true��ʾ���Դ���false��ʾû�д���
  virtual bool OnMessageReceived(const Message& message);

  // ͨѶͨ����������ʱ�Ļص�����
  // peer_pid�ڵ�ID
  virtual void OnChannelError(void* sender);

 private:
  SyncChannelPipe(ChannelListener* listener);

  void SignalAllEvents();

 // ����û��ȡ�صĻظ���Ϣ
  void ClearRelpays();

  ChannelListener* Listener();

 private:

  std::recursive_mutex mutex_;

  std::shared_ptr<ChannelPipe> channel_pipe_;

  typedef std::set<PendingSyncMsg*> PendingSyncMessages;
  PendingSyncMessages pending_sync_messages_;

  // ��ȡ��Ϣ�б�
  std::set<Message*> replay_messages_;

  ChannelListener* listener_;

  bool is_server;
};  //  class ChannelListener

}  //  namespace ipc

#endif  // GAMEHALLMAIN_IPC_IPC_SYNC_CHANNEL_PIPE_H_
