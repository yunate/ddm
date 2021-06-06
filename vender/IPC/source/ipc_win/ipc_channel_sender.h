#ifndef GAMEHALLMAIN_IPC_IPC_CHANNEL_SENDER_H_
#define GAMEHALLMAIN_IPC_IPC_CHANNEL_SENDER_H_ 1

namespace ipc {
  // 前置声明
class Message;

class ChannelSender {
 public:
  // 发送IPC消息
  virtual bool Send(Message* message) = 0;
}; //  class ChannelSender

}  //  namespace ipc
#endif  // GAMEHALLMAIN_IPC_IPC_CHANNEL_SENDER_H_
