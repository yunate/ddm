#ifndef __C2SMsgDef_h_
#define __C2SMsgDef_h_ 1

#include "CommonMsgDef.h"

/** 客户端向服务器发送的消息
*/
enum class IPC_C2S_MSG_DEF : uint32_t
{
    /** 头部未使用
    */
    IPC_C2S_MSG_DEF_HEAD_NOUSE = (uint32_t)IPC_COMMON_MSG_DEF::IPC_COMMON_MSG_DEF_TAIL_NOUSE,

    /** 测试消息
    */
    IPC_C2S_MSG_DEF_TEST,

    /** 测试同步消息（有返回）
    */
    IPC_C2S_MSG_DEF_TEST_SYNC,

    /** 注册消息
    */
    IPC_C2S_MSG_DEF_REGIST,

    /** 尾部未使用
    */
    IPC_C2S_MSG_DEF_TAIL_NOUSE
};

#endif // __C2SMsgDef_h_
