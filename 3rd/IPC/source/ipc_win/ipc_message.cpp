#include "ipc_message.h"
#include <windows.h>

namespace ipc {

namespace {

class MessageIDGenerator {
 public:
  uint64_t GetNextId() {
    return message_id_++;
  }

  static MessageIDGenerator* GetInstance() {
    static MessageIDGenerator s_message_id_generator;
    return &s_message_id_generator;
  }

 private:
  MessageIDGenerator() : message_id_(0) {
  }

 private:
  uint64_t message_id_;
};

}

Message::~Message() {
}

Message::Message()
    : Pickle(sizeof(Header)) {
  header()->routing = header()->type = 0;
  header()->flags = 0;
  header()->message_id = 0;
  Init();
}

Message::Message(int32_t routing_id, uint32_t type, PriorityValue priority)
    : Pickle(sizeof(Header)) {
  header()->routing = routing_id;
  header()->type = type;
  header()->flags = priority;
  header()->message_id = 0;
  Init();
}

Message::Message(const char* data, int data_len)
    : Pickle(data, data_len) {
  Init();
}

Message::Message(const Message& other)
    : Pickle(other) {
    dispatch_error_ = other.dispatch_error_;
    sender_pid_ = other.sender_pid_;
    read_index_ = other.read_index_;
    is_sent_ = other.is_sent_;
    sync_send_timeout_ = other.sync_send_timeout_;
    pump_messages_event_ = other.pump_messages_event_;
    sender_name_ = other.sender_name_;
}

void Message::Init() {
  dispatch_error_ = false;
  sender_pid_ = 0;
  read_index_ = 0;
  is_sent_ = false;
  sync_send_timeout_ = INFINITE;
}

Message& Message::operator=(const Message& other) {
  *static_cast<Pickle*>(this) = other;
  dispatch_error_ = other.dispatch_error_;
  sender_pid_ = other.sender_pid_;
  read_index_ = other.read_index_;
  is_sent_ = other.is_sent_;
  sync_send_timeout_ = other.sync_send_timeout_;
  sender_name_ = other.sender_name_;

  // pump_messages_event_ = other.pump_messages_event_;
  return *this;
}

bool Message::ReadBool() const {
  bool result = false;
  ReadBuiltinType(&result);
  return result;
}

int32_t Message::ReadInt() const {
  int32_t result = 0;
  ReadBuiltinType(&result);
  return result;
}

long Message::ReadLong() const {
  long result = 0;
  ReadBuiltinType(&result);
  return result;
}

uint8_t Message::ReadUInt8() const {
  uint8_t result = 0;
  const char* data = NULL;
  int32_t len = sizeof(result);
  PickleIterator it(*this);
  it.SetReadIndex(read_index_);
  bool ok = it.ReadBytes(&data, len);
  read_index_ = it.GetReadIndex();

  if (ok && data != NULL) {
    result = *data;
  }
  return result;
}

uint16_t Message::ReadUInt16() const {
  uint16_t result = 0;
  ReadBuiltinType(&result);
  return result;
}

uint32_t Message::ReadUInt32() const {
  uint32_t result = 0;
  ReadBuiltinType(&result);
  return result;
}

int64_t Message::ReadInt64() const {
  int64_t result = 0;
  ReadBuiltinType(&result);
  return result;
}

uint64_t Message::ReadUInt64() const {
  uint64_t result = 0;
  ReadBuiltinType(&result);
  return result;
}

size_t Message::ReadSizeT() const {
  uint64_t result_uint64 = 0;
  bool success = ReadBuiltinType(&result_uint64);
  result_uint64 = success ? result_uint64 : 0;

  return static_cast<size_t>(result_uint64);
}

float Message::ReadFloat() const {
  float result = 0;
  PickleIterator it(*this);
  it.SetReadIndex(read_index_);
  it.ReadFloat(&result);
  read_index_ = it.GetReadIndex();
  return result;
}

double Message::ReadDouble() const {
  double result = 0;
  PickleIterator it(*this);
  it.SetReadIndex(read_index_);
  it.ReadDouble(&result);
  read_index_ = it.GetReadIndex();
  return result;
}

std::wstring Message::ReadString() const {
  std::wstring result;
  PickleIterator it(*this);
  it.SetReadIndex(read_index_);
  it.ReadString(&result);
  read_index_ = it.GetReadIndex();
  return result;
}

std::string Message::ReadStringA() const {
  std::string result;
  PickleIterator it(*this);
  it.SetReadIndex(read_index_);
  it.ReadString(&result);
  read_index_ = it.GetReadIndex();
  return result;
}

bool Message::ReadData(const char** data, int* length) const {
  PickleIterator it(*this);
  it.SetReadIndex(read_index_);
  bool ok = it.ReadData(data, length);
  read_index_ = it.GetReadIndex();
  return ok;
}

bool Message::ReadBytes(const char** data, int length) const {
  PickleIterator it(*this);
  it.SetReadIndex(read_index_);
  bool ok = it.ReadBytes(data, length);
  read_index_ = it.GetReadIndex();
  return ok;
}

template <typename Type>
inline bool Message::ReadBuiltinType(Type* result) const {
  PickleIterator it(*this);
  it.SetReadIndex(read_index_);
  bool ok = it.ReadBuiltinType(result);
  read_index_ = it.GetReadIndex();
  return ok;
}

uint16_t Message::GetType() const {
  return header()->type;
}

int32_t Message::GetRoutingID() const {
  return header()->routing;
}

uint64_t Message::GetMessageID() const {
  return header()->message_id;
}

bool Message::IsMessageReplyTo(const Message* msg, uint64_t message_id) {
  if (!msg->is_reply()) {
    return false;
  }

  return (msg->GetMessageID() == message_id);
}

void Message::set_sync() {
  header()->flags |= SYNC_BIT;
  header()->message_id = MessageIDGenerator::GetInstance()->GetNextId();
}

void Message::SetMessageId(uint64_t id)
{
    header()->message_id = id;
}

void Message::SetHeaderValues(int32_t routing, uint32_t type, uint32_t flags) {
  // This should only be called when the message is already empty.
  header()->routing = routing;
  header()->type = type;
  header()->flags = flags;
}

void Message::SetPumpMessageEvent(NSP_DDM::simple_event* message_event) {
  pump_messages_event_ = message_event;
}

NSP_DDM::simple_event* Message::GetPumpMessageEvent() {
  return pump_messages_event_;
}

ipc::Message* Message::GenerateReply(const Message* msg) {
  Message* reply = new Message(msg->GetRoutingID(), IPC_REPLY_ID);
  reply->set_reply();
  reply->header()->message_id = msg->GetMessageID();
  return reply;
}

Message::NextMessageInfo::NextMessageInfo()
    : message_size(0),
      message_found(false),
      pickle_end(nullptr),
      message_end(nullptr) {
}

Message::NextMessageInfo::~NextMessageInfo() {
}

// static
void Message::FindNext(const char* range_start,
                       const char* range_end,
                       NextMessageInfo* info) {
  info->message_found = false;
  info->message_size = 0;

  size_t pickle_size = 0;
  if (!Pickle::PeekNext(sizeof(Header), range_start, range_end, &pickle_size)) {
    return;
  }

  bool have_entire_pickle = static_cast<size_t>(range_end - range_start) >= pickle_size;
  info->message_size = pickle_size;
  if (!have_entire_pickle) {
    return;
  }

  const char* pickle_end = range_start + pickle_size;
  info->message_end = pickle_end;
  info->pickle_end = pickle_end;
  info->message_found = true;
}

}  // namespace ipc
