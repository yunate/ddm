#ifndef GAMEHALLMAIN_IPC_IPC_CHANNEL_PIPE_H_
#define GAMEHALLMAIN_IPC_IPC_CHANNEL_PIPE_H_ 1

#include <chrono>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "ipc_callback.h"
#include "ipc_message_handler.h"

namespace ipc {
// ǰ������
class Message;
struct PendingSyncMsg;
class ChannelListener;

// IPC�ײ�ͨѶ����(�ܵ�ͨѶ)��ʵ����
class ChannelPipe : public MessageHandler,
                    public ipc::SupportWeakCallback {
 public:
  // ����ͨѶͨ���Ŀͻ���
  static ChannelPipe* CreateClient(const std::wstring& pipe_name, ChannelListener* listener);

  // ����ͨѶͨ���ķ����
  static ChannelPipe* CreateServer(const std::wstring& pipe_name, ChannelListener* listener);

 public:
  ~ChannelPipe();

  // �ܵ���������
  // ���� true����ʾ���ӳɹ���false����ʾ����ʧ��
  bool Connect();

  // �ܵ��ر�����
  void Close();

  // �Ƿ�ر���
  bool IsClosed();

  // �Ƿ�ȴ�������
  bool IsWaitIngConnect();

  // ��ͨѶͨ���з���һ����Ϣ
  bool Send(Message* message);

  //  ��ɶ˿ڵ�֪ͨ
  void OnIOCompleted(OVERLAPPED* context, uint32_t transfered_bytes, uint32_t error) override;

  /** ��ùܵ����ӵ�ʱ��
  @return �ܵ����ӵ�ʱ��
  */
  std::chrono::milliseconds::rep GetPipeConnectedTime();

  /** ��ùܵ��ϴλ�Ծ��ʱ��
  @return �ϴλ�Ծ��ʱ��
  */
  std::chrono::milliseconds::rep GetPipePrevActiveTime();

  // �ر�ԭ��
  enum {
    CLOSE_TYPE_UNKNOWN = 0,
    CLOSE_TYPE_MANUAL,
    CLOSE_TYPE_SWITCH_RENDER_MODE
  };

 protected:
  // �ڲ�ʹ�õ���Ϣ����
  enum {
    HELLO_MESSAGE_TYPE = UINT16_MAX,
  };
  // �ܵ���Ϣ�����Ϣ����
  static const size_t kMaximumMessageSize = 128 * 1024 * 1024;

  // ��д�����С.
  static const size_t kReadBufferSize = 4 * 1024;

  // ͨѶͨ����ģʽ
  enum ChannelMode {
    // Ĭ��ֵ����������ʼ��
    kChannelMode_None = 0,

    // �����ģʽ
    kChannelMode_Server = 1,

    // �ͻ���ģʽ
    kChannelMode_Client = 2,
  };

  // ���캯��
  ChannelPipe(const std::wstring& pipe_name, ChannelMode mode, ChannelListener* listener);

 private:
  bool CreatePipe(const std::wstring& pipe_name, ChannelMode mode);

  // ��д����
  bool ReadData(char* buffer, uint32_t buffer_len, uint32_t* bytes_read);

  // ������Ϣ
  bool SendData(const Message* message);

  void OnSendMessage(const Message* message);

  bool ProcessConnection();
  bool ProcessOutgoingMessages(OVERLAPPED* context, DWORD bytes_written);

  // Takes the given data received from the IPC channel, translates it into
  // Messages, and puts them in queued_messages_.
  // As an optimization, after a message is translated, the message is
  // immediately dispatched if able. This prevents an otherwise unnecessary deep
  // copy of the message which is needed to store the message in the message
  // queue.
  bool TranslateInputData(const char* input_data, int input_data_len);
  void DispatchIPCMessage(Message* m);

 bool AsyncReadComplete(int32_t bytes_read);

 // �����Ϣ��С�Ƿ���Ч
 bool CheckMessageSize(size_t size);

 // ֪ͨ���й�����¼��˳�
 void SignalAllEvents();
 ChannelListener* Listener();

 private:
  struct State {
    explicit State(ChannelPipe* channel);
    ~State();
    OVERLAPPED context;
    bool is_pending;
  };

  State input_state_;
  State output_state_;
  HANDLE pipe_;

  std::wstring pipe_name_;

  bool waiting_connect_;

  std::queue<Message*> output_queue_;

  ChannelListener* listener_;

  // These messages are waiting to be dispatched. If this vector is non-empty,
  // then the front Message must be blocked on receiving an attachment from the
  // AttachmentBroker.
  std::vector<Message*> queued_messages_;

  // ��ȡ����
  char input_buffer_[kReadBufferSize];

  // ����pipe�����С���ݻ���
  std::string input_overflow_buf_;

  bool is_closed_;

  /** �ܵ�����ʱ�䣬���ڿͻ�����˵�����ǳɹ�������ʱ��
  */
  std::chrono::milliseconds::rep pipe_connected_time_;

  /** �ϴλ�Ծʱ�䣬��Ծ�����ǣ������ܵ���������Ϣ��������Ϣ
  */
  std::chrono::milliseconds::rep pipe_prev_active_time_;
};  // class ChannelPipe

}  //  namespace ipc
#endif  // GAMEHALLMAIN_IPC_IPC_CHANNEL_PIPE_H_
