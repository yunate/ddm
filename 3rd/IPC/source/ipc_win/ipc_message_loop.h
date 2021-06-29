#ifndef GAMEHALLMAIN_IPC_IPC_MESSAGE_LOOP_H_
#define GAMEHALLMAIN_IPC_IPC_MESSAGE_LOOP_H_ 1

#include "ipc_callback.h"
#include "ipc_pending_task.h"

#include <mutex>
#include <unordered_set>
#include <windows.h>

namespace ipc {

// 前置声明
class MessageHandler;

  // 线程的消息循环
class MessageLoop {
 public:
  // 构造函数
  MessageLoop() {}
  ~MessageLoop() {}

 public:
  // 消息循环初始化
  virtual void Init() {}

  // 消息循环反初始化
  virtual void Uninit() {}

  // 运行消息循环
  virtual void Run() {}

  // 退出消息循环
  virtual void Quit() {}

  virtual void PostTask(const Closure&) {}
  virtual void ScheduleWork() {}

  // 消息循环是否正在运行
  virtual bool IsRunning() const { return false; }
};

// IPC消息循环
class MessageLoopForIPC : public MessageLoop {
 public:
  MessageLoopForIPC();
  ~MessageLoopForIPC();

 public:
  // 管道句柄绑定到完成端口，并注册一个消息处理接口
  // pipeHandle 要与完成端口绑定的管道句柄
  // messageHandler 消息处理接口
  void RegisterMessageHandler(HANDLE pipe_handle, MessageHandler* message_handler);

  // 获取消息循环实例
  static MessageLoopForIPC* GetInstace();

 public:
  // 运行消息循环
  void Run() override;

  // 退出消息循环
  void Quit() override;

  // 添加工作任务
  virtual void PostTask(const Closure& task) override;

  // 调度任务
  void ScheduleWork() override;

  void InsertValidHandler(MessageHandler*);

  bool IsValidHandler(MessageHandler*);

  void RemoveValidHandler(MessageHandler*);

 protected:
  struct IOItem {
    MessageHandler* handler;
    OVERLAPPED* context;
    DWORD bytes_transfered;
    DWORD error;
    bool has_valid_io_context;
  };  // struct IOItem

  bool WaitForIOCompletion(DWORD timeout, MessageHandler* message_handler);

  // 获取一个消息处理器事件
  bool GetIOItem(DWORD timeout, IOItem* item);

  bool ProcessInternalIOItem(const IOItem& item);

  bool DoWork();

  void AddToIncomingQueue(const PendingTask& task);

  void ReloadWorkQueue();

  // Converts an IOHandler pointer to a completion port key.
  // |has_valid_io_context| specifies whether completion packets posted to
  // |handler| will have valid OVERLAPPED pointers.
  static ULONG_PTR HandlerToKey(MessageHandler* handler, bool has_valid_io_context);

  // Converts a completion port key to an IOHandler pointer.
  static MessageHandler* KeyToHandler(ULONG_PTR key, bool* has_valid_io_context);

 private:
  // 完成端口句柄
  HANDLE iocp_port_;

  long have_work_;

  // 退出
  bool should_quit_;

  // 任务输入队列
  TaskQueue incoming_queue_;

  // 工作任务队列
  TaskQueue work_queue_;

  // 有效的被绑定，有效的指针HandlerToKey
  std::unordered_set<MessageHandler*> valid_handler_;

  std::recursive_mutex incoming_queue_lock_;
  std::recursive_mutex valid_handler_lock_;
};  //  class MessageLoopForIPC

}  //  namespace ipc

#endif  // GAMEHALLMAIN_IPC_IPC_MESSAGE_LOOP_H_
