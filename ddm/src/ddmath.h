#ifndef ddmath_h_
#define ddmath_h_
#include "g_def.h"
BEG_NSP_DDM

// 32λ�з��������ֵ
#define MAX_S32 0x7fffffff

// 32λ�޷��������ֵ
#define MAX_U32 0xffffffff

// 64λ�з��������ֵ
#define MAX_S64 0x7fffffffffffffff

// 64λ�޷��������ֵ
#define MAX_U64 0xffffffffffffffff

// ȡ64λ��λ
#define L64(l) ((__int64)(l) & 0xffffffff)

// ȡ64λ��λd
#define H64(l) (((__int64)(l) >> 32) & 0xffffffff)

// ����a ����b �ֽڶ����Ľ��
// ����b�������ڵ���a����С�Ͻ�
template<class T, class U>
inline T align(T a, U b)
{
    return (a + b - 1) & ~(b - 1);
}

END_NSP_DDM
#endif // ddmath_h_

