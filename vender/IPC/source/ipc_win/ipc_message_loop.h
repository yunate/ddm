#ifndef GAMEHALLMAIN_IPC_IPC_MESSAGE_LOOP_H_
#define GAMEHALLMAIN_IPC_IPC_MESSAGE_LOOP_H_ 1

#include "ipc_callback.h"
#include "ipc_pending_task.h"

#include <mutex>
#include <unordered_set>
#include <windows.h>

namespace ipc {

// ǰ������
class MessageHandler;

  // �̵߳���Ϣѭ��
class MessageLoop {
 public:
  // ���캯��
  MessageLoop() {}
  ~MessageLoop() {}

 public:
  // ��Ϣѭ����ʼ��
  virtual void Init() {}

  // ��Ϣѭ������ʼ��
  virtual void Uninit() {}

  // ������Ϣѭ��
  virtual void Run() {}

  // �˳���Ϣѭ��
  virtual void Quit() {}

  virtual void PostTask(const Closure& task) {}
  virtual void ScheduleWork() {}

  // ��Ϣѭ���Ƿ���������
  virtual bool IsRunning() const { return false; }
};

// IPC��Ϣѭ��
class MessageLoopForIPC : public MessageLoop {
 public:
  MessageLoopForIPC();
  ~MessageLoopForIPC();

 public:
  // �ܵ�����󶨵���ɶ˿ڣ���ע��һ����Ϣ����ӿ�
  // pipeHandle Ҫ����ɶ˿ڰ󶨵Ĺܵ����
  // messageHandler ��Ϣ����ӿ�
  void RegisterMessageHandler(HANDLE pipe_handle, MessageHandler* message_handler);

  // ��ȡ��Ϣѭ��ʵ��
  static MessageLoopForIPC* GetInstace();

 public:
  // ������Ϣѭ��
  void Run() override;

  // �˳���Ϣѭ��
  void Quit() override;

  // ��ӹ�������
  virtual void PostTask(const Closure& task) override;

  // ��������
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

  // ��ȡһ����Ϣ�������¼�
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
  // ��ɶ˿ھ��
  HANDLE iocp_port_;

  long have_work_;

  // �˳�
  bool should_quit_;

  // �����������
  TaskQueue incoming_queue_;

  // �����������
  TaskQueue work_queue_;

  // ��Ч�ı��󶨣���Ч��ָ��HandlerToKey
  std::unordered_set<MessageHandler*> valid_handler_;

  std::recursive_mutex incoming_queue_lock_;
  std::recursive_mutex valid_handler_lock_;
};  //  class MessageLoopForIPC

}  //  namespace ipc

#endif  // GAMEHALLMAIN_IPC_IPC_MESSAGE_LOOP_H_
