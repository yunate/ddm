#ifndef ddmath_h_
#define ddmath_h_
#include "g_def.h"
BEG_NSP_DDM

// 32位有符号数最大/小值
#define MAX_S32 0x7fffffff
#define MIN_S32 0x80000000

// 32位无符号数最大值
#define MAX_U32 0xffffffff

// 64位有符号数最大/小值
#define MAX_S64 0x7fffffffffffffff
#define MIN_S64 0x8000000000000000

// 64位无符号数最大值
#define MAX_U64 0xffffffffffffffff

// 取64位高位
#define L64(l) ((__int64)(l) & 0xffffffff)

// 取64位低位d
#define H64(l) (((__int64)(l) >> 32) & 0xffffffff)

// 计算a 按照b 字节对齐后的结果
// 计算b倍数大于等于a的最小上界
template<class T, class U>
inline T align(T a, U b)
{
    return (a + b - 1) & ~(b - 1);
}

END_NSP_DDM
#endif // ddmath_h_

