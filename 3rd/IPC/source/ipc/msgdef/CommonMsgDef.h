#ifndef __CommonMsgDef_h_
#define __CommonMsgDef_h_ 1

#include <cstdint>

/** 服务器、客户端公有消息
*/
enum class IPC_COMMON_MSG_DEF : uint32_t
{
    /** 头部未使用
    */
    IPC_COMMON_MSG_DEF_HEAD_NOUSE = 0,

    /** 尾部未使用
    */
    IPC_COMMON_MSG_DEF_TAIL_NOUSE
};

#endif // __CommonMsgDef_h_
