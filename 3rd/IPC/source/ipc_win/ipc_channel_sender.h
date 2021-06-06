#ifndef GAMEHALLMAIN_IPC_IPC_CHANNEL_SENDER_H_
#define GAMEHALLMAIN_IPC_IPC_CHANNEL_SENDER_H_ 1

namespace ipc {
  // ǰ������
class Message;

class ChannelSender {
 public:
  // ����IPC��Ϣ
  virtual bool Send(Message* message) = 0;
}; //  class ChannelSender

}  //  namespace ipc
#endif  // GAMEHALLMAIN_IPC_IPC_CHANNEL_SENDER_H_
