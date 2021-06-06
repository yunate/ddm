#ifndef GAMEHALLMAIN_IPC_IPC_CHANNEL_PIPE_H_
#define GAMEHALLMAIN_IPC_IPC_CHANNEL_PIPE_H_ 1

#include <chrono>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "ipc_callback.h"
#include "ipc_message_handler.h"

namespace ipc {
// 前置声明
class Message;
struct PendingSyncMsg;
class ChannelListener;

// IPC底层通讯机制(管道通讯)的实现类
class ChannelPipe : public MessageHandler,
                    public ipc::SupportWeakCallback {
 public:
  // 创建通讯通道的客户端
  static ChannelPipe* CreateClient(const std::wstring& pipe_name, ChannelListener* listener);

  // 创建通讯通道的服务端
  static ChannelPipe* CreateServer(const std::wstring& pipe_name, ChannelListener* listener);

 public:
  ~ChannelPipe();

  // 管道建立连接
  // 返回 true，表示连接成功；false，表示连接失败
  bool Connect();

  // 管道关闭连接
  void Close();

  // 是否关闭了
  bool IsClosed();

  // 是否等待连接中
  bool IsWaitIngConnect();

  // 向通讯通道中发送一个消息
  bool Send(Message* message);

  //  完成端口的通知
  void OnIOCompleted(OVERLAPPED* context, uint32_t transfered_bytes, uint32_t error) override;

  /** 获得管道连接的时间
  @return 管道连接的时间
  */
  std::chrono::milliseconds::rep GetPipeConnectedTime();

  /** 获得管道上次活跃的时间
  @return 上次活跃的时间
  */
  std::chrono::milliseconds::rep GetPipePrevActiveTime();

  // 关闭原因
  enum {
    CLOSE_TYPE_UNKNOWN = 0,
    CLOSE_TYPE_MANUAL,
    CLOSE_TYPE_SWITCH_RENDER_MODE
  };

 protected:
  // 内部使用的消息类型
  enum {
    HELLO_MESSAGE_TYPE = UINT16_MAX,
  };
  // 管道消息最大消息长度
  static const size_t kMaximumMessageSize = 128 * 1024 * 1024;

  // 读写缓存大小.
  static const size_t kReadBufferSize = 4 * 1024;

  // 通讯通道的模式
  enum ChannelMode {
    // 默认值，仅用做初始化
    kChannelMode_None = 0,

    // 服务端模式
    kChannelMode_Server = 1,

    // 客户端模式
    kChannelMode_Client = 2,
  };

  // 构造函数
  ChannelPipe(const std::wstring& pipe_name, ChannelMode mode, ChannelListener* listener);

 private:
  bool CreatePipe(const std::wstring& pipe_name, ChannelMode mode);

  // 读写数据
  bool ReadData(char* buffer, uint32_t buffer_len, uint32_t* bytes_read);

  // 发送消息
  bool SendData(const Message* message);

  void OnSendMessage(const Message* message);

  bool ProcessConnection();
  bool ProcessOutgoingMessages(OVERLAPPED* context, DWORD bytes_written);

  // Takes the given data received from the IPC channel, translates it into
  // Messages, and puts them in queued_messages_.
  // As an optimization, after a message is translated, the message is
  // immediately dispatched if able. This prevents an otherwise unnecessary deep
  // copy of the message which is needed to store the message in the message
  // queue.
  bool TranslateInputData(const char* input_data, int input_data_len);
  void DispatchIPCMessage(Message* m);

 bool AsyncReadComplete(int32_t bytes_read);

 // 检查消息大小是否有效
 bool CheckMessageSize(size_t size);

 // 通知所有挂起的事件退出
 void SignalAllEvents();
 ChannelListener* Listener();

 private:
  struct State {
    explicit State(ChannelPipe* channel);
    ~State();
    OVERLAPPED context;
    bool is_pending;
  };

  State input_state_;
  State output_state_;
  HANDLE pipe_;

  std::wstring pipe_name_;

  bool waiting_connect_;

  std::queue<Message*> output_queue_;

  ChannelListener* listener_;

  // These messages are waiting to be dispatched. If this vector is non-empty,
  // then the front Message must be blocked on receiving an attachment from the
  // AttachmentBroker.
  std::vector<Message*> queued_messages_;

  // 读取缓存
  char input_buffer_[kReadBufferSize];

  // 超出pipe缓存大小数据缓存
  std::string input_overflow_buf_;

  bool is_closed_;

  /** 管道连接时间，对于客户端来说，就是成功创建的时间
  */
  std::chrono::milliseconds::rep pipe_connected_time_;

  /** 上次活跃时间，活跃可以是：创建管道，接收消息，发送消息
  */
  std::chrono::milliseconds::rep pipe_prev_active_time_;
};  // class ChannelPipe

}  //  namespace ipc
#endif  // GAMEHALLMAIN_IPC_IPC_CHANNEL_PIPE_H_
