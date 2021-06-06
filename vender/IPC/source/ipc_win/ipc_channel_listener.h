#ifndef GAMEHALLMAIN_IPC_IPC_CHANNEL_LISTENER_H_
#define GAMEHALLMAIN_IPC_IPC_CHANNEL_LISTENER_H_ 1

#include <stdint.h>

namespace ipc {
  // 前置声明
class Message;

  // 管道消息响应接口，需要接受IPC消息的类都需要继承该接口
class ChannelListener {
 public:
  // 通讯通道成功接收一个消息时的回调函数
  // 是否处理了该消息，true表示可以处理；false表示没有处理
  virtual bool OnMessageReceived(const Message& message) = 0;

  // 通讯通道成功建立连接时的回调函数
  // peer_pid节点ID
  virtual void OnChannelConnected(int32_t peer_pid)
  {
      peer_pid;
  }

  // 通讯通道发生错误时的回调函数
  virtual void OnChannelError(void* sender)
  {
      sender;
  }

  // 通讯通道收到"脏消息"时的回调函数
  virtual void OnBadMessageReceived(const Message& message)
  {
      message;
  }

 protected:
  virtual ~ChannelListener() {}
}; //  class ChannelListener

}  //  namespace ipc

#endif  // GAMEHALLMAIN_IPC_IPC_CHANNEL_LISTENER_H_
