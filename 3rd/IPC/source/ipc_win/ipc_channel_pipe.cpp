#include "ipc_channel_pipe.h"
#include "ipc_channel_listener.h"
#include "ipc_message.h"
#include "ipc_message_loop.h"

#include <sddl.h>
#include <windows.h>
#include <assert.h>

namespace ipc {

ChannelPipe::State::State(ChannelPipe* channel) : is_pending(false) {
  ::memset(&context, 0, sizeof(context));
}

ChannelPipe::State::~State() {
  static_assert(offsetof(ChannelPipe::State, context) == 0,
                "ChannelWin::State should have context as its first datamember.");
}

ChannelPipe::ChannelPipe(const std::wstring& pipe_name, ChannelMode mode, ChannelListener* listener)
    : input_state_(this),
      output_state_(this),
      waiting_connect_(mode & kChannelMode_Server),
      is_closed_(false), 
      pipe_name_(pipe_name),
      pipe_connected_time_(0),
      pipe_prev_active_time_(0),
      listener_(listener) {
  CreatePipe(pipe_name, mode);
}

ChannelPipe::~ChannelPipe() {
  Close();
}

static BOOL CreateMyDACL(SECURITY_ATTRIBUTES* pSA) {
  if (pSA == NULL) {
    return FALSE;
  }

  TCHAR* szSD =
      // Make it lowest 
      L"S:(ML;;;;;LW)"
      // Discretionary ACL
      L"D:"
      // Allow access to everyone logon
      L"(A;OICI;GA;;;WD)"
      // Allow full control to ALL_APP_PACKAGES
      L"(A;OICI;GA;;;s-1-15-2-1)";

  return ::ConvertStringSecurityDescriptorToSecurityDescriptor(szSD,
                                                               SDDL_REVISION_1,
                                                               &(pSA->lpSecurityDescriptor),
                                                               NULL);
}

bool ChannelPipe::CreatePipe(const std::wstring& name, ChannelMode mode) {
  std::wstring pipe_name(L"\\\\.\\pipe\\");
  pipe_name += name;

  if (mode & kChannelMode_Server) {
    SECURITY_ATTRIBUTES* pAttr = NULL;
    SECURITY_ATTRIBUTES attr;
    if (CreateMyDACL(&attr)) {
      pAttr = &attr;
    }
    const DWORD open_mode = PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED | FILE_FLAG_FIRST_PIPE_INSTANCE;
    pipe_ = ::CreateNamedPipeW(pipe_name.c_str(),
                               open_mode,
                               PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
                               1,
                               ChannelPipe::kReadBufferSize,
                               ChannelPipe::kReadBufferSize,
                               5000,
                               pAttr);
  } else if (mode & kChannelMode_Client) {
    pipe_ = ::CreateFileW(pipe_name.c_str(),
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_FLAG_OVERLAPPED,
                          NULL);
  } else {
    assert(false);
  }

  if (!HandleTraits::IsHandleValid(pipe_)) {
#ifdef _DEBUG
      DWORD lastError = ::GetLastError();
      if (lastError != 0)
      {
          HLOCAL LocalAddress = NULL;
          ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
                          NULL,
                          lastError,
                          0,
                          (LPTSTR)&LocalAddress,
                          0,
                          NULL);
          ::OutputDebugString((LPTSTR)LocalAddress);
          ::LocalFree(LocalAddress);
      }
#endif
    return false;
  }

  auto dur = std::chrono::high_resolution_clock::now().time_since_epoch();
  pipe_prev_active_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
  if (mode & kChannelMode_Client) {
    pipe_connected_time_ = pipe_prev_active_time_;
  }

  // 创建成功之添加一个默认的连接消息
  Message* m = new Message(MSG_ROUTING_NONE, HELLO_MESSAGE_TYPE, Message::PRIORITY_NORMAL);
  if (!m->WriteInt(::GetCurrentProcessId()) ||
      !m->WriteUInt32(Message::kCheckCode)) {
    ::CloseHandle(pipe_);
    return false;
  }

  output_queue_.push(m);
  return true;
}

ChannelPipe* ChannelPipe::CreateClient(const std::wstring& pipe_name, ChannelListener* listener) {
  ChannelPipe* channel = new ChannelPipe(pipe_name, kChannelMode_Client, listener);
  if (!HandleTraits::IsHandleValid(channel->pipe_)) {
    delete channel;
    channel = NULL;
  }

  return channel;
}

ChannelPipe* ChannelPipe::CreateServer(const std::wstring& pipe_name, ChannelListener* listener) {
  ChannelPipe* channel = new ChannelPipe(pipe_name, kChannelMode_Server, listener);
  if (!HandleTraits::IsHandleValid(channel->pipe_)) {
    delete channel;
    channel = NULL;
  }
  return channel;
}

bool ChannelPipe::Connect() {
  if (!HandleTraits::IsHandleValid(pipe_)) {
    return false;
  }

  // 向消息循环注册端口
  MessageLoopForIPC::GetInstace()->RegisterMessageHandler(pipe_, this);

  if (waiting_connect_) {
    ProcessConnection();
  }

  if (!input_state_.is_pending) {
    // 客户端连接上之后必须读一次，然后才能够正常处理IOCP等待
    OnIOCompleted(&input_state_.context, 0, 0);
  }

  if (!waiting_connect_) {
    ProcessOutgoingMessages(NULL, 0);
  }
  return true;
}

bool ChannelPipe::ProcessConnection() {
  if (input_state_.is_pending) {
    input_state_.is_pending = false;
  }

  if (!HandleTraits::IsHandleValid(pipe_)) {
    return false;
  }

  BOOL ok = ::ConnectNamedPipe(pipe_, &input_state_.context);
  DWORD err = ::GetLastError();
  if (ok) {
    // Uhm, the API documentation says that this function should never
    // return success when used in overlapped mode.
    return false;
  }

  switch (err) {
  case ERROR_IO_PENDING:
    input_state_.is_pending = true;
    break;
  case ERROR_PIPE_CONNECTED:
    waiting_connect_ = false;
    auto dur = std::chrono::high_resolution_clock::now().time_since_epoch();
    pipe_connected_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    break;
  case ERROR_NO_DATA:
    // The pipe is being closed.
    return false;
  default:
    return false;
  }

  return true;
}

bool ChannelPipe::ProcessOutgoingMessages(OVERLAPPED* context, DWORD bytes_written) {
  if (output_state_.is_pending) {
    output_state_.is_pending = false;
    if (!context || bytes_written == 0) {
      return false;
    }
 
    if (!output_queue_.empty()) {
      Message* element = output_queue_.front();
      output_queue_.pop();
      delete element;
    }
  }

  if (output_queue_.empty()) {
    return true;
  }
  
  if (!HandleTraits::IsHandleValid(pipe_)) {
    return false;
  }
  Message* element = output_queue_.front();
  element->set_sent();
  BOOL ok = ::WriteFile(pipe_,
                        element->data(),
                        static_cast<uint32_t>(element->size()),
                        NULL,
                        &output_state_.context);

  if (!ok) {
    DWORD write_error = ::GetLastError();
    if (write_error == ERROR_IO_PENDING) {
      output_state_.is_pending = true;
      return true;
    }
    return false;
  }

  output_state_.is_pending = true;
  return true;
}

bool ChannelPipe::TranslateInputData(const char* input_data, int input_data_len) {
  const char* p = nullptr;
  const char* end = nullptr;

  // Possibly combine with the overflow buffer to make a larger buffer.
  if (input_overflow_buf_.empty()) {
    p = input_data;
    end = input_data + input_data_len;
  } else {
    if (!CheckMessageSize(input_overflow_buf_.size() + input_data_len))
      return false;
    input_overflow_buf_.append(input_data, input_data_len);
    p = input_overflow_buf_.data();
    end = p + input_overflow_buf_.size();
  }

  size_t next_message_size = 0;

  // Dispatch all complete messages in the data buffer.
  while (p < end) {
    Message::NextMessageInfo info;
    Message::FindNext(p, end, &info);
    if (info.message_found) {
      int pickle_len = static_cast<int>(info.pickle_end - p);
      Message translated_message(p, pickle_len);

      // If there are no queued messages, attempt to immediately dispatch the
      // newly translated message.
      if (queued_messages_.empty()) {
        DispatchIPCMessage(&translated_message);
        p = info.message_end;
        continue;
      }

      // Make a deep copy of |translated_message| to add to the queue.
      Message* m = new Message(translated_message);
      queued_messages_.push_back(m);
      p = info.message_end;
    } else {
      // Last message is partial.
      next_message_size = info.message_size;
      if (!CheckMessageSize(next_message_size)) {
        return false;
      }
      break;
    }
  }

  // Save any partial data in the overflow buffer.
  input_overflow_buf_.assign(p, end - p);

  if (!input_overflow_buf_.empty()) {
    // We have something in the overflow buffer, which means that we will
    // append the next data chunk (instead of parsing it directly). So we
    // resize the buffer to fit the next message, to avoid repeatedly
    // growing the buffer as we receive all message' data chunks.
    next_message_size += ChannelPipe::kReadBufferSize - 1;
    if (next_message_size > input_overflow_buf_.capacity()) {
      input_overflow_buf_.reserve(next_message_size);
    }
  }

  return true;
}

void ChannelPipe::DispatchIPCMessage(Message* m) {
  if (m == NULL) {
    return;
  }

  bool handled = false;
  // 连接校验
  if (m->GetType() == HELLO_MESSAGE_TYPE) {
    handled = true;

    PickleIterator it(*m);
    int32_t claimed_pid;
    bool failed = !it.ReadInt(&claimed_pid);

    if (!failed) {
      int32_t secret;
      failed = it.ReadInt(&secret) ? (secret != Message::kCheckCode) : true;
    }

    if (failed) {
      Close();
      return;
    } else {
      // 通知连接成功
        listener_->OnChannelConnected(claimed_pid);
    }
  }
  if (!handled) {
     m->set_sender_name(pipe_name_);
     auto dur = std::chrono::high_resolution_clock::now().time_since_epoch();
     pipe_prev_active_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
     listener_->OnMessageReceived(*m);
  }
}

bool ChannelPipe::AsyncReadComplete(int32_t bytes_read) {
  return TranslateInputData(this->input_buffer_, bytes_read);
}

bool ChannelPipe::ReadData(char* buffer, uint32_t buffer_len, uint32_t* /*bytes_read*/) {
  if (!HandleTraits::IsHandleValid(pipe_)) {
    return false;
  }

  DWORD bytes_read = 0;
  BOOL ok = ::ReadFile(pipe_, buffer, buffer_len, &bytes_read, &input_state_.context);
  if (!ok) {
    DWORD err = ::GetLastError();
    if (err == ERROR_IO_PENDING) {
      input_state_.is_pending = true;
      return true;
    }
    return false;
  }

  input_state_.is_pending = true;
  return true;
}

bool ChannelPipe::SendData(const Message* message) {
  {
    output_queue_.push((Message*)message);
  }
  if (!waiting_connect_) {
    if (!output_state_.is_pending) {
      if (!ProcessOutgoingMessages(NULL, 0)) {
        return false;
      }
    }
  }

  return true;
}

void ChannelPipe::Close() {
    MessageLoopForIPC::GetInstace()->RemoveValidHandler(this);
    HANDLE tmp = pipe_;
    pipe_ = NULL;
    is_closed_ = true;
    if (HandleTraits::IsHandleValid(tmp))
    {
        if (input_state_.is_pending || output_state_.is_pending)
        {
            ::CancelIo(tmp);
        }

        HandleTraits::CloseHandle(tmp);

        while (!output_queue_.empty())
        {
            Message* element = output_queue_.front();
            output_queue_.pop();
            delete element;
        }
    }
}

bool ChannelPipe::IsClosed()
{
    return is_closed_;
}

bool ChannelPipe::IsWaitIngConnect()
{
    return waiting_connect_;
}

bool ChannelPipe::Send(Message* message) {
  if (!HandleTraits::IsHandleValid(pipe_)) {
    return false;
  } else {
    ipc::MessageLoopForIPC::GetInstace()->PostTask(ipc::Bind(&ChannelPipe::OnSendMessage, this, message));
    return true;
  }
}

void ChannelPipe::OnIOCompleted(OVERLAPPED* context, uint32_t transfered_bytes, uint32_t error) {
  bool ok = true;
  // 处理输入
  if (context == &input_state_.context) {
    if (waiting_connect_) {
      if (!ProcessConnection()) {
        return;
      }
      // We may have some messages queued up to send...
      if (!output_queue_.empty() && !output_state_.is_pending) {
        ProcessOutgoingMessages(NULL, 0);
      }
      if (input_state_.is_pending) {
        return;
      }
    }
    // 挂起状态第二次通知时，数据已经到达
    if (input_state_.is_pending) {
      input_state_.is_pending = false;
      if (!transfered_bytes) {
        ok = false;
      } else if (HandleTraits::IsHandleValid(pipe_)) {
        ok = AsyncReadComplete(transfered_bytes);
      }
    }

    if (ok) {
      ::memset(&input_buffer_, 0, ChannelPipe::kReadBufferSize);
      // 读取数据大小
      ok = ReadData(input_buffer_, kReadBufferSize, NULL);
    }
  } else {
    ok = ProcessOutgoingMessages(context, transfered_bytes);
  }
  // 管道读写失败处理
  if (!ok && HandleTraits::IsHandleValid(pipe_)) {
    bool is_closed = is_closed_;
    Close();
    // 手工关闭的不再通知
    if (!is_closed) {
      listener_->OnChannelError(this);
    }
  }
}

std::chrono::milliseconds::rep ChannelPipe::GetPipeConnectedTime()
{
    return pipe_connected_time_;
}

std::chrono::milliseconds::rep ChannelPipe::GetPipePrevActiveTime()
{
    return pipe_prev_active_time_;
}

bool ChannelPipe::CheckMessageSize(size_t size) {
  if (size <= ChannelPipe::kMaximumMessageSize) {
    return true;
  }
  input_overflow_buf_.clear();
  return false;
}

ipc::ChannelListener* ChannelPipe::Listener() {
  return listener_;
}

void ChannelPipe::OnSendMessage(const Message* message) {
  auto dur = std::chrono::high_resolution_clock::now().time_since_epoch();
  pipe_prev_active_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
  SendData(message);
}

} // namespace ipc
