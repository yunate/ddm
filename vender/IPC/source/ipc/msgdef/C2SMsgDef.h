#ifndef __C2SMsgDef_h_
#define __C2SMsgDef_h_ 1

#include "CommonMsgDef.h"

/** �ͻ�������������͵���Ϣ
*/
enum class IPC_C2S_MSG_DEF : uint32_t
{
    /** ͷ��δʹ��
    */
    IPC_C2S_MSG_DEF_HEAD_NOUSE = (uint32_t)IPC_COMMON_MSG_DEF::IPC_COMMON_MSG_DEF_TAIL_NOUSE,

    /** ������Ϣ
    */
    IPC_C2S_MSG_DEF_TEST,

    /** ����ͬ����Ϣ���з��أ�
    */
    IPC_C2S_MSG_DEF_TEST_SYNC,

    /** ע����Ϣ
    */
    IPC_C2S_MSG_DEF_REGIST,

    /** β��δʹ��
    */
    IPC_C2S_MSG_DEF_TAIL_NOUSE
};

#endif // __C2SMsgDef_h_
