#ifndef GAMEHALLMAIN_IPC_IPC_MESSAGE_H_
#define GAMEHALLMAIN_IPC_IPC_MESSAGE_H_ 1

#include <stdint.h>
#include <string>

#include "ipc_pickle.h"
#include "thread/async/simple_event.h"

namespace ipc {

namespace internal {
class ChannelReader;
}  // namespace internal

class Message : public Pickle {
 public:
  // 校验码
  static const int32_t kCheckCode = 0xA0DE88CA;

  enum PriorityValue {
    PRIORITY_LOW = 1,
    PRIORITY_NORMAL,
    PRIORITY_HIGH
  };

  // Bit values used in the flags field.
  // Upper 24 bits of flags store a reference number, so this enum is limited to
  // 8 bits.
  enum {
    PRIORITY_MASK     = 0x03,  // Low 2 bits of store the priority value.
    SYNC_BIT          = 0x04,
    REPLY_BIT         = 0x08,
    REPLY_ERROR_BIT   = 0x10,
    UNBLOCK_BIT       = 0x20,
    PUMPING_MSGS_BIT  = 0x40,
    HAS_SENT_TIME_BIT = 0x80,
  };

  ~Message() override;

  Message();

  // Initialize a message with a user-defined type, priority value, and
  // destination WebView ID.
  Message(int32_t routing_id, uint32_t type, PriorityValue priority = PRIORITY_NORMAL);

  // Initializes a message from a const block of data.  The data is not copied;
  // instead the data is merely referenced by this message.  Only const methods
  // should be used on the message when initialized this way.
  Message(const char* data, int data_len);

  Message(const Message& other);
  Message& operator=(const Message& other);

  PriorityValue priority() const {
    return static_cast<PriorityValue>(header()->flags & PRIORITY_MASK);
  }
 public:
  bool ReadBool() const;
  int32_t ReadInt() const;
  long ReadLong() const;
  uint8_t ReadUInt8() const;
  uint16_t ReadUInt16() const;
  uint32_t ReadUInt32() const;
  int64_t ReadInt64() const;
  uint64_t ReadUInt64() const;
  size_t ReadSizeT() const;
  float ReadFloat() const;
  double ReadDouble() const;
  std::wstring ReadString() const;
  std::string ReadStringA() const;
  bool ReadData(const char** data, int* length) const;
  bool ReadBytes(const char** data, int length) const;

  template <typename Type>
  bool ReadBuiltinType(Type* result) const;

 public:
  // 获取消息类型
  uint16_t GetType() const;
  int32_t GetRoutingID() const;

  uint64_t GetMessageID() const;

  void SetPumpMessageEvent(NSP_DDM::simple_event* message_event);
  NSP_DDM::simple_event* GetPumpMessageEvent();

 public:

  // 给同步的消息创建一个返回消息 
  static Message* GenerateReply(const Message* msg);

  // 是否是指定消息的回复消息
  static bool IsMessageReplyTo(const Message* msg, uint64_t message_id);

  // True if this is a synchronous message.
  void set_sync();

  // 设置message的id，一般有回复消息的时候，指定
  void SetMessageId(uint64_t id);

  bool is_sync() const {
    return (header()->flags & SYNC_BIT) != 0;
  }

  // Set this on a reply to a synchronous message.
  void set_reply() {
    header()->flags |= REPLY_BIT;
  }

  bool is_reply() const {
    return (header()->flags & REPLY_BIT) != 0;
  }

  // Set this on a reply to a synchronous message to indicate that no receiver
  // was found.
  void set_reply_error() {
    header()->flags |= REPLY_ERROR_BIT;
  }

  bool is_reply_error() const {
    return (header()->flags & REPLY_ERROR_BIT) != 0;
  }

  // Normally when a receiver gets a message and they're blocked on a
  // synchronous message Send, they buffer a message.  Setting this flag causes
  // the receiver to be unblocked and the message to be dispatched immediately.
  void set_unblock(bool unblock) {
    if (unblock) {
      header()->flags |= UNBLOCK_BIT;
    } else {
      header()->flags &= ~UNBLOCK_BIT;
    }
  }

  bool should_unblock() const {
    return (header()->flags & UNBLOCK_BIT) != 0;
  }

  bool is_sent() const {
    return is_sent_;
  }

  void set_sent() {
    is_sent_ = true;
  }

  uint32_t get_sync_send_timeout() const {
    return sync_send_timeout_;
  }

  void set_sync_send_timeout(uint32_t sync_send_timeout) {
    sync_send_timeout_ = sync_send_timeout;
  }

  // Tells the receiver that the caller is pumping messages while waiting
  // for the result.
  bool is_caller_pumping_messages() const {
    return (header()->flags & PUMPING_MSGS_BIT) != 0;
  }

  void set_dispatch_error() const {
    dispatch_error_ = true;
  }

  bool dispatch_error() const {
    return dispatch_error_;
  }

  uint32_t type() const {
    return header()->type;
  }

  int32_t routing_id() const {
    return header()->routing;
  }

  void set_routing_id(int32_t new_id) {
    header()->routing = new_id;
  }

  uint32_t flags() const {
    return header()->flags;
  }

  // Sets all the given header values. The message should be empty at this
  // call.
  void SetHeaderValues(int32_t routing, uint32_t type, uint32_t flags);

  template<class T, class S, class P>
  static bool Dispatch(const Message* msg, T* obj, S* sender, P* parameter,
                       void (T::*func)()) {
    (obj->*func)();
    return true;
  }

  template<class T, class S, class P>
  static bool Dispatch(const Message* msg, T* obj, S* sender, P* parameter,
                       void (T::*func)(P*)) {
    (obj->*func)(parameter);
    return true;
  }

  // The static method FindNext() returns several pieces of information, which
  // are aggregated into an instance of this struct.
  struct NextMessageInfo {
    NextMessageInfo();
    ~NextMessageInfo();

    // Total message size. Always valid if |message_found| is true.
    // If |message_found| is false but we could determine message size
    // from the header, this field is non-zero. Otherwise it's zero.
    size_t message_size;
    // Whether an entire message was found in the given memory range.
    bool message_found;
    // Only filled in if |message_found| is true.
    // The start address is passed into FindNext() by the caller, so isn't
    // repeated in this struct. The end address of the pickle should be used to
    // construct a base::Pickle.
    const char* pickle_end;
    // Only filled in if |message_found| is true.
    // The end address of the message should be used to determine the start
    // address of the next message.
    const char* message_end;
  };

  // |info| is an output parameter and must not be nullptr.
  static void FindNext(const char* range_start,
                       const char* range_end,
                       NextMessageInfo* info);

  void set_sender_pid(uint32_t id) { sender_pid_ = id; }
  uint32_t get_sender_pid() const { return sender_pid_; }

  void set_sender_name(const std::wstring& sendName) { sender_name_ = sendName; }
  const std::wstring& get_sender_name() const{ return sender_name_; }

 protected:
  friend class Channel;
  friend class ChannelMojo;
  friend class ChannelNacl;
  friend class ChannelPosix;
  friend class ChannelWin;
  friend class internal::ChannelReader;
  friend class SyncMessage;

#pragma pack(push, 4)
  struct Header : Pickle::Header {
    int32_t routing;  // ID of the view that this message is destined for
    uint32_t type;    // specifies the user-defined message type
    uint32_t flags;   // specifies control flags for the message
    uint64_t message_id;   // specifies control flags for the message
  };
#pragma pack(pop)

  Header* header() {
    return headerT<Header>();
  }
  const Header* header() const {
    return headerT<Header>();
  }

  void Init();

  inline void set_offset(size_t offset) {
    read_index_ = offset;
  }

  inline size_t get_offset(){
    return read_index_;
  }

 private:
  // 等待事件
     NSP_DDM::simple_event* pump_messages_event_;

  // Used internally to support IPC::Listener::OnBadMessageReceived.
  mutable bool dispatch_error_;

  // The process id of the sender of the message. This member is populated with
  // a valid value for every message dispatched to listeners.
  uint32_t sender_pid_;

  // 发送者名称
  std::wstring sender_name_;

  bool is_sent_;

  // 同步消息发送超时
  uint32_t sync_send_timeout_;

  // 读取偏移
  mutable size_t read_index_;

};  // class Message

// 同步等待数据结构
struct PendingSyncMsg {
  PendingSyncMsg(uint64_t id,
                 NSP_DDM::simple_event* e)
    : id(id), done_event(e), send_result(false) {
  }
  uint64_t id;
  NSP_DDM::simple_event* done_event;
  bool send_result;
};  // struct PendingSyncMsg

}  // namespace IPC

enum SpecialRoutingIDs {
  // indicates that we don't have a routing ID yet.
  MSG_ROUTING_NONE = -2,

  // indicates a general message not sent to a particular tab.
  MSG_ROUTING_CONTROL = INT32_MAX,
};  // enum SpecialRoutingIDs

#define IPC_REPLY_ID 0xFFFFFFF0  // Special message id for replies

#endif  // GAMEHALLMAIN_IPC_IPC_MESSAGE_H_
