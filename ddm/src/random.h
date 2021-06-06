#ifndef random_h_
#define random_h_ 1

#include "g_def.h"

BEG_NSP_DDM

// 随机管理类
class random
{
public:
    /** 产生随机数
    @param [in] min 包含
    @param [in] max 包含
    @param [out] out 结果
    @return 是否成功
    */
    static bool get_rand_number(int min, int max, int& out);

    // 生成guid
    static bool gen_guida(ddstra& guid);
    static bool gen_guidw(ddstrw& guid);
};

END_NSP_DDM
#endif // random_h_
