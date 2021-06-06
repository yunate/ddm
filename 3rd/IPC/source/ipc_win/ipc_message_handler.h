#ifndef GAMEHALLMAIN_IPC_IPC_MESSAGE_HANDLER_H_
#define GAMEHALLMAIN_IPC_IPC_MESSAGE_HANDLER_H_ 1

#include <stdint.h>
#include <Windows.h>

namespace ipc {

  // ��Ϣ����ӿڶ���
class MessageHandler {
 public:
  // ��ɶ˿ڻص�֪ͨ�ӿ�
  virtual void OnIOCompleted(OVERLAPPED* context, uint32_t bytes_transfered, uint32_t error) = 0;
};  // class MessageHandler

// ���������������
class HandleTraits {
 public:
  static bool CloseHandle(HANDLE handle) {
    return (!!::CloseHandle(handle));
  }

  static bool IsHandleValid(HANDLE handle) {
    return handle != NULL && handle != INVALID_HANDLE_VALUE;
  }

  static HANDLE NullHandle() {
    return NULL;
  }
};  //  class HandleTraits

}  //  namespace ipc

#endif  // GAMEHALLMAIN_IPC_IPC_MESSAGE_HANDLER_H_
