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
  // 前置声明
class Message;
struct PendingSyncMsg;
class ChannelPipe;

  // 管道消息响应接口，需要接受IPC消息的类都需要继承该接口
class SyncChannelPipe : public ChannelListener {
 public:
  ~SyncChannelPipe();

  // 向通讯通道中发送一个消息
  bool Send(Message* message);

  void Close();

  bool IsClosed();

  bool IsServer();


  // 是否等待连接中
  bool IsWaitIngConnect();

  /** 获得管道连接的时间
  @return 管道连接的时间
  */
  std::chrono::milliseconds::rep GetPipeConnectedTime();

  /** 获得管道上次活跃的时间
  @return 上次活跃的时间
  */
  std::chrono::milliseconds::rep GetPipePrevActiveTime();

  static SyncChannelPipe* CreateServer(const std::wstring& pipe_name, ChannelListener* listener);
  static SyncChannelPipe* CreateClient(const std::wstring& pipe_name, ChannelListener* listener);

  // 获取回复消息,调用方负责删除
  Message* GetReplayMessage(uint64_t messageid);

  // 通讯通道成功接收一个消息时的回调函数
  // 是否处理了该消息，true表示可以处理；false表示没有处理
  virtual bool OnMessageReceived(const Message& message);

  // 通讯通道发生错误时的回调函数
  // peer_pid节点ID
  virtual void OnChannelError(void* sender);

 private:
  SyncChannelPipe(ChannelListener* listener);

  void SignalAllEvents();

 // 清理没有取回的回复消息
  void ClearRelpays();

  ChannelListener* Listener();

 private:

  std::recursive_mutex mutex_;

  std::shared_ptr<ChannelPipe> channel_pipe_;

  typedef std::set<PendingSyncMsg*> PendingSyncMessages;
  PendingSyncMessages pending_sync_messages_;

  // 获取消息列表
  std::set<Message*> replay_messages_;

  ChannelListener* listener_;

  bool is_server;
};  //  class ChannelListener

}  //  namespace ipc

#endif  // GAMEHALLMAIN_IPC_IPC_SYNC_CHANNEL_PIPE_H_
