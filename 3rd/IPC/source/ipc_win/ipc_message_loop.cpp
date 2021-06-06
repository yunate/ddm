#include "ipc_message_loop.h"

#include <assert.h>
#include "ipc_callback.h"
#include "ipc_message_handler.h"

namespace ipc {

MessageLoopForIPC::MessageLoopForIPC()
    : should_quit_(false),
      have_work_(0) {
  iocp_port_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
}

MessageLoopForIPC::~MessageLoopForIPC() {
  if (HandleTraits::IsHandleValid(iocp_port_)) {
    ::CloseHandle(iocp_port_);
    iocp_port_ = NULL;
  }
}

void MessageLoopForIPC::RegisterMessageHandler(HANDLE pipe_handle, MessageHandler* message_handler) {
  ULONG_PTR key = HandlerToKey(message_handler, true);
  HANDLE iocp_port = ::CreateIoCompletionPort(pipe_handle, iocp_port_, key, 0);
  assert(iocp_port == iocp_port_);
  InsertValidHandler(message_handler);
}

MessageLoopForIPC* MessageLoopForIPC::GetInstace() {
  static MessageLoopForIPC messageLoop;
  return &messageLoop;
}

void MessageLoopForIPC::Run() {
  // ³¬Ê±µÈ´ý1Ãë
  const uint32_t timeout = 1000;
  for (;;) {
    bool more_work_is_plausible = DoWork();
    if (should_quit_) {
      break;
    }

    more_work_is_plausible |= WaitForIOCompletion(0, NULL);
    if (more_work_is_plausible) {
      continue;
    }

    if (should_quit_) {
      break;
    }

    WaitForIOCompletion(timeout, NULL);
    if (should_quit_) {
      break;
    }
  }
}

bool MessageLoopForIPC::DoWork() {
  for (;;) {
    ReloadWorkQueue();
    if (work_queue_.empty()) {
      break;
    }

    do {
      PendingTask task = work_queue_.front();
      work_queue_.pop();
      task.Run();
      return true;
    } while (!work_queue_.empty());
  }

  return false;
}

void MessageLoopForIPC::ReloadWorkQueue() {
  if (!work_queue_.empty()) {
    return;
  }

  {
    std::lock_guard<std::recursive_mutex> lock(incoming_queue_lock_);
    if (incoming_queue_.empty()) {
      return;
    }

    work_queue_.Swap(&incoming_queue_);
  }
}

void MessageLoopForIPC::Quit() {
  should_quit_ = true;
  ::PostQueuedCompletionStatus(iocp_port_, 0, 0, NULL);
}

void MessageLoopForIPC::PostTask(const Closure& task) {
  AddToIncomingQueue(task);
}

void MessageLoopForIPC::AddToIncomingQueue(const PendingTask& task) {
  std::lock_guard<std::recursive_mutex> lock(incoming_queue_lock_);
  bool was_empty = incoming_queue_.empty();
  incoming_queue_.push(task);
  if (!was_empty) {
    return;
  }

  ScheduleWork();
}

bool MessageLoopForIPC::ProcessInternalIOItem(const IOItem& item) {
  if (this == reinterpret_cast<MessageLoopForIPC*>(item.context) &&
      this == reinterpret_cast<MessageLoopForIPC*>(item.handler)) {
    // This is our internal completion.
    assert(!item.bytes_transfered);
    ::InterlockedExchange(&have_work_, 0);
    return true;
  }
  return false;
}

void MessageLoopForIPC::ScheduleWork() {
  if (::InterlockedExchange(&have_work_, 1)) {
    return;  // Someone else continued the pumping.
  }

  // Make sure the MessagePump does some work for us.
  BOOL ret = ::PostQueuedCompletionStatus(iocp_port_,
                                          0,
                                          reinterpret_cast<ULONG_PTR>(this),
                                          reinterpret_cast<OVERLAPPED*>(this));
  assert(ret);
}

void MessageLoopForIPC::InsertValidHandler(MessageHandler* handler)
{
    std::lock_guard<std::recursive_mutex> lock(valid_handler_lock_);
    valid_handler_.insert(handler);
}

void MessageLoopForIPC::RemoveValidHandler(MessageHandler* handler)
{
    std::lock_guard<std::recursive_mutex> lock(valid_handler_lock_);

    if (valid_handler_.find(handler) != valid_handler_.end())
    {
        valid_handler_.erase(handler);
    }
}

bool MessageLoopForIPC::IsValidHandler(MessageHandler* handler)
{
    std::lock_guard<std::recursive_mutex> lock(valid_handler_lock_);
    return valid_handler_.find(handler) != valid_handler_.end();
}

bool MessageLoopForIPC::WaitForIOCompletion(DWORD timeout, MessageHandler* message_handler) {
  MessageHandler* handler = NULL;
  IOItem item = { 0 };
  if (!GetIOItem(timeout, &item)) {
    return false;
  }

  if (ProcessInternalIOItem(item)) {
    return true;
  }

  if (item.handler) {
      std::lock_guard<std::recursive_mutex> lock(valid_handler_lock_);
      if (IsValidHandler(item.handler))
      {
          item.handler->OnIOCompleted(item.context, item.bytes_transfered, item.error);
      }
  }
  return true;
}

ULONG_PTR MessageLoopForIPC::HandlerToKey(MessageHandler* handler, bool has_valid_io_context) {
  ULONG_PTR key = reinterpret_cast<ULONG_PTR>(handler);
  if (!has_valid_io_context) {
    key = key | 1;
  }
  return key;
}

MessageHandler* MessageLoopForIPC::KeyToHandler(ULONG_PTR key, bool* has_valid_io_context) {
  *has_valid_io_context = ((key & 1) == 0);
  return reinterpret_cast<MessageHandler*>(key & ~static_cast<ULONG_PTR>(1));
}

bool MessageLoopForIPC::GetIOItem(DWORD timeout, IOItem* item) {
  ULONG_PTR key = NULL;
  OVERLAPPED* overlapped = NULL;
  if (!::GetQueuedCompletionStatus(iocp_port_, &item->bytes_transfered, &key, &overlapped, timeout)) {
    if (!overlapped) {
      return false;
    }

    item->error = ::GetLastError();
    item->bytes_transfered = 0;
  }

  item->handler = KeyToHandler(key, &item->has_valid_io_context);
  item->context = reinterpret_cast<OVERLAPPED*>(overlapped);
  return true;
}

}  // namespace ipc
