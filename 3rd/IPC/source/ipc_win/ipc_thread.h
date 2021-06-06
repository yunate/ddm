#ifndef GAMEHALLMAIN_IPC_IPC_THREAD_H_
#define GAMEHALLMAIN_IPC_IPC_THREAD_H_ 1

#include "thread/async/simple_event.h"
#include <string>
#include <thread>

namespace ipc {
// ǰ������
class MessageLoop;

// �̷߳�װ��
class Thread {
 public:
  // ���캯��
  // threadName �̵߳����ƣ�������գ�����Զ�����һ���߳���
  Thread(const std::wstring& thread_name);
  ~Thread();

 public:
  // �����߳�
  // messageLoop ��Ϣѭ���ӿ�
  // threadName �̵߳����ƣ�������գ�����Զ�����һ���߳���
  static Thread* CreateThread(MessageLoop* messageLoop, const std::wstring& thread_name);

  // �����߳�
  bool Start();

  // �˳���Ϣѭ��
  bool QuitMessageLoop();

  // �ȴ��߳̽���
  void time_wait(uint32_t millisecond);

  // �߳��Ƿ���������
  bool IsRunning();

  // ��ȡ�߳�ID
  uint32_t GetThreadId();

  // �����̵߳���Ϣѭ���ӿ�
  void SetMessageLoop(MessageLoop* message_loop);

  // ��ȡ�̵߳���Ϣѭ���ӿ�
  MessageLoop* GetMessageLoop();

 private:
  // ִ���̺߳���
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
