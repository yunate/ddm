#ifndef __CommonMsgDef_h_
#define __CommonMsgDef_h_ 1

#include <cstdint>

/** ���������ͻ��˹�����Ϣ
*/
enum class IPC_COMMON_MSG_DEF : uint32_t
{
    /** ͷ��δʹ��
    */
    IPC_COMMON_MSG_DEF_HEAD_NOUSE = 0,

    /** β��δʹ��
    */
    IPC_COMMON_MSG_DEF_TAIL_NOUSE
};

#endif // __CommonMsgDef_h_
