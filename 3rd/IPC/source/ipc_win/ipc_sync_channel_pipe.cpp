#include "ipc_sync_channel_pipe.h"

#include "ipc_channel_pipe.h"
#include "ipc_message.h"
#include "thread/async/simple_event.h"


namespace ipc {

SyncChannelPipe::SyncChannelPipe(ChannelListener* listener)
    : listener_(listener), is_server(false){
}

SyncChannelPipe::~SyncChannelPipe() {
  ClearRelpays();
}

bool SyncChannelPipe::Send(Message* message) {
  PendingSyncMsg pending_message(message->GetMessageID(), message->GetPumpMessageEvent());
  bool sync = message->is_sync();
  bool result = false;
  if (sync) {
    uint32_t timeout = message->get_sync_send_timeout();
    {
            std::lock_guard<std::recursive_mutex> lock(mutex_);
      pending_sync_messages_.insert(&pending_message);
    }
    result = channel_pipe_->Send(message);

   // 同步消息处理
   if (result) {
       NSP_DDM::simple_event* pump_message_event = pending_message.done_event;
     if (pump_message_event) {
       pump_message_event->time_wait(timeout);
     }
   }

   {
           std::lock_guard<std::recursive_mutex> lock(mutex_);
     pending_sync_messages_.erase(&pending_message);
   }
  }
  else {
    result = channel_pipe_->Send(message);
  }

  return result;
}

void SyncChannelPipe::Close() {
  channel_pipe_->Close();
}

bool SyncChannelPipe::IsClosed()
{
    return channel_pipe_->IsClosed();
}

bool SyncChannelPipe::IsServer()
{
    return is_server;
}

bool SyncChannelPipe::IsWaitIngConnect()
{
    return channel_pipe_->IsWaitIngConnect();
}

std::chrono::milliseconds::rep SyncChannelPipe::GetPipeConnectedTime()
{
    return channel_pipe_->GetPipeConnectedTime();
}

std::chrono::milliseconds::rep SyncChannelPipe::GetPipePrevActiveTime()
{
    return channel_pipe_->GetPipePrevActiveTime();
}

SyncChannelPipe* SyncChannelPipe::CreateClient(const std::wstring& pipe_name, ChannelListener* listener) {
  std::unique_ptr<SyncChannelPipe> sync_channel(new SyncChannelPipe(listener));
  sync_channel->is_server = false;
  std::shared_ptr<ChannelPipe> channel_pipe;
  channel_pipe.reset(ChannelPipe::CreateClient(pipe_name, sync_channel.get()));

  if ((channel_pipe != nullptr) && (channel_pipe->Connect())) {
    sync_channel->channel_pipe_ = channel_pipe;
    return sync_channel.release();
  }

  return nullptr;
}

SyncChannelPipe* SyncChannelPipe::CreateServer(const std::wstring& pipe_name, ChannelListener* listener) {
  std::unique_ptr<SyncChannelPipe> sync_channel(new SyncChannelPipe(listener));
  sync_channel->is_server = true;
  std::shared_ptr<ChannelPipe> channel_pipe;
  channel_pipe.reset(ChannelPipe::CreateServer(pipe_name, sync_channel.get()));

  if ((channel_pipe != nullptr) && (channel_pipe->Connect())) {
    sync_channel->channel_pipe_ = channel_pipe;
    return sync_channel.release();
  }

  return nullptr;
}
ipc::Message* SyncChannelPipe::GetReplayMessage(uint64_t message_id) {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
  ipc::Message* m = NULL;
  for (auto iter = replay_messages_.begin();
       iter != replay_messages_.end();
       ++iter) {
    if (message_id == (*iter)->GetMessageID()) {
      m = *iter;
      replay_messages_.erase(iter);
      break;
    }
  }

  return m;
}

bool SyncChannelPipe::OnMessageReceived(const Message& message) {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
  for (auto iter = pending_sync_messages_.begin();
       iter != pending_sync_messages_.end();
       ++iter) {
    if (Message::IsMessageReplyTo(&message, (*iter)->id)) {
      Message* m = new Message(message);
      replay_messages_.insert(m);
      
      NSP_DDM::simple_event* wait_event = (*iter)->done_event;
      if ((wait_event != NULL)) {
        wait_event->set_event();
      }

      return true;
      break;
    }
  }

  return Listener()->OnMessageReceived(message);
}

void SyncChannelPipe::OnChannelError(void* sender) {
  ClearRelpays();
  SignalAllEvents();
  Listener()->OnChannelError(this);
}

void SyncChannelPipe::SignalAllEvents() {
        std::lock_guard<std::recursive_mutex> lock(mutex_);
  for (PendingSyncMessages::iterator iter = pending_sync_messages_.begin();
       iter != pending_sync_messages_.end();
       ++iter) {
      NSP_DDM::simple_event* wait_event = (*iter)->done_event;

    if ((wait_event != NULL)) {
      wait_event->set_event();
    }
  }
}

void SyncChannelPipe::ClearRelpays() {
  for (auto item : replay_messages_) {
    delete item;
  }

  replay_messages_.clear();
}

ChannelListener* SyncChannelPipe::Listener() {
  return listener_;
}

}  // namespace ipc
