#ifndef random_h_
#define random_h_ 1

#include "g_def.h"

BEG_NSP_DDM

// ���������
class random
{
public:
    /** ���������
    @param [in] min ����
    @param [in] max ����
    @param [out] out ���
    @return �Ƿ�ɹ�
    */
    static bool get_rand_number(int min, int max, int& out);

    // ����guid
    static bool gen_guida(ddstra& guid);
    static bool gen_guidw(ddstrw& guid);
};

END_NSP_DDM
#endif // random_h_
