
#ifndef __S2CMsgDef_h_
#define __S2CMsgDef_h_ 1

#include "CommonMsgDef.h"

/** ��������ͻ��˷��͵���Ϣ
*/
enum class IPC_S2C_MSG_DEF : uint32_t
{
    /** ͷ��δʹ��
    */
    IPC_S2C_MSG_DEF_HEAD_NOUSE = (uint32_t)IPC_COMMON_MSG_DEF::IPC_COMMON_MSG_DEF_TAIL_NOUSE,

    /** ������Ϣ
    */
    IPC_S2C_MSG_DEF_TEST,

    /** β��δʹ��
    */
    IPC_S2C_MSG_DEF_TAIL_NOUSE
};

#endif // __S2CMsgDef_h_
