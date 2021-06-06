#ifndef GAMEHALLMAIN_IPC_IPC_CHANNEL_LISTENER_H_
#define GAMEHALLMAIN_IPC_IPC_CHANNEL_LISTENER_H_ 1

#include <stdint.h>

namespace ipc {
  // ǰ������
class Message;

  // �ܵ���Ϣ��Ӧ�ӿڣ���Ҫ����IPC��Ϣ���඼��Ҫ�̳иýӿ�
class ChannelListener {
 public:
  // ͨѶͨ���ɹ�����һ����Ϣʱ�Ļص�����
  // �Ƿ����˸���Ϣ��true��ʾ���Դ���false��ʾû�д���
  virtual bool OnMessageReceived(const Message& message) = 0;

  // ͨѶͨ���ɹ���������ʱ�Ļص�����
  // peer_pid�ڵ�ID
  virtual void OnChannelConnected(int32_t peer_pid)
  {
      peer_pid;
  }

  // ͨѶͨ����������ʱ�Ļص�����
  virtual void OnChannelError(void* sender)
  {
      sender;
  }

  // ͨѶͨ���յ�"����Ϣ"ʱ�Ļص�����
  virtual void OnBadMessageReceived(const Message& message)
  {
      message;
  }

 protected:
  virtual ~ChannelListener() {}
}; //  class ChannelListener

}  //  namespace ipc

#endif  // GAMEHALLMAIN_IPC_IPC_CHANNEL_LISTENER_H_
