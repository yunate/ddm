#ifndef GAMEHALLMAIN_IPC_IPC_MESSAGE_HANDLER_H_
#define GAMEHALLMAIN_IPC_IPC_MESSAGE_HANDLER_H_ 1

#include <stdint.h>
#include <Windows.h>

namespace ipc {

  // 消息处理接口定义
class MessageHandler {
 public:
  // 完成端口回调通知接口
  virtual void OnIOCompleted(OVERLAPPED* context, uint32_t bytes_transfered, uint32_t error) = 0;
};  // class MessageHandler

// 句柄操作辅助函数
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
