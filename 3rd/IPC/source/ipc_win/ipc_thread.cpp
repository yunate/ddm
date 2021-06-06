#include "ipc_thread.h"

#include "ipc_message_loop.h"
#include <assert.h>

namespace ipc {

Thread::Thread(const std::wstring& thread_name) 
    : thread_name_(thread_name),
      message_loop_(NULL) {
}

Thread::~Thread() {
  if (IsRunning()) {
    QuitMessageLoop();
  }
}

Thread* Thread::CreateThread(MessageLoop* message_loop, const std::wstring& thread_name) {
  Thread* thread = new Thread(thread_name);
  thread->message_loop_ = message_loop;
  return thread;
}

bool Thread::Start() {
  assert(message_loop_);
  if (message_loop_ == NULL) {
    return false;
  }

  if (IsRunning()) {
    return false;
  }

  thread_impl_.reset(new std::thread(Thread::ThreadFunc, this));
  return true;
}

bool Thread::QuitMessageLoop() {
  if (!IsRunning()) {
    return true;
  }

  assert(message_loop_);

  if (message_loop_ == NULL) {
    return false;
  }

  message_loop_->Quit();

  if (thread_impl_->joinable())
  {
      thread_impl_->join();
  }

  thread_impl_.reset();
  return true;
}

bool Thread::IsRunning() {
  return thread_impl_ != nullptr;
}

void Thread::time_wait(uint32_t millisecond) {
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
}

uint32_t Thread::GetThreadId() {
  if (thread_impl_ == NULL) {
    return 0;
  }
  return ::GetCurrentThreadId();
}

void Thread::SetMessageLoop(MessageLoop* message_loop) {
  message_loop_ = message_loop;
}

MessageLoop* Thread::GetMessageLoop() {
  return message_loop_;
}

void Thread::ThreadFunc(void* argument) {
  Thread* thread = reinterpret_cast<Thread*>(argument);
  thread->CallThreadProc();
}

void Thread::CallThreadProc() {
  if (message_loop_) {
    message_loop_->Run();
  }
}

}  // namespace ipc
