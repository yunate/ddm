
#ifndef __S2CMsgDef_h_
#define __S2CMsgDef_h_ 1

#include "CommonMsgDef.h"

/** 服务器向客户端发送的消息
*/
enum class IPC_S2C_MSG_DEF : uint32_t
{
    /** 头部未使用
    */
    IPC_S2C_MSG_DEF_HEAD_NOUSE = (uint32_t)IPC_COMMON_MSG_DEF::IPC_COMMON_MSG_DEF_TAIL_NOUSE,

    /** 测试消息
    */
    IPC_S2C_MSG_DEF_TEST,

    /** 尾部未使用
    */
    IPC_S2C_MSG_DEF_TAIL_NOUSE
};

#endif // __S2CMsgDef_h_
