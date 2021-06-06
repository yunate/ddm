#ifndef GAMEHALLMAIN_IPC_IPC_THREAD_H_
#define GAMEHALLMAIN_IPC_IPC_THREAD_H_ 1

#include "thread/async/simple_event.h"
#include <string>
#include <thread>

namespace ipc {
// 前置声明
class MessageLoop;

// 线程封装类
class Thread {
 public:
  // 构造函数
  // threadName 线程的名称，如果传空，则会自动生成一个线程名
  Thread(const std::wstring& thread_name);
  ~Thread();

 public:
  // 创建线程
  // messageLoop 消息循环接口
  // threadName 线程的名称，如果传空，则会自动生成一个线程名
  static Thread* CreateThread(MessageLoop* messageLoop, const std::wstring& thread_name);

  // 启动线程
  bool Start();

  // 退出消息循环
  bool QuitMessageLoop();

  // 等待线程结束
  void time_wait(uint32_t millisecond);

  // 线程是否正在运行
  bool IsRunning();

  // 获取线程ID
  uint32_t GetThreadId();

  // 设置线程的消息循环接口
  void SetMessageLoop(MessageLoop* message_loop);

  // 获取线程的消息循环接口
  MessageLoop* GetMessageLoop();

 private:
  // 执行线程函数
  static void ThreadFunc(void* argument);
  void CallThreadProc();

 private:
  std::wstring thread_name_;

  MessageLoop* message_loop_;

  NSP_DDM::simple_event exit_event_;
  std::shared_ptr<std::thread> thread_impl_;
};  //  class Thread

}  //  namespace base

#endif  // GAMEHALLMAIN_IPC_IPC_THREAD_H_
