
#ifndef g_def_h_
#define g_def_h_ 1

// 命名空间宏 DDM for dog_doggy_man
#define NSP_DDM ddm
#define BEG_NSP_DDM namespace NSP_DDM {
#define END_NSP_DDM }

/////////////////////////////////////数字定义/////////////////////////////////////
// 无符号
BEG_NSP_DDM
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned long;
using u64 = unsigned long long;

// 有符号
using s8 = char;
using s16 = short;
using s32 = long;
using s64 = long long;
END_NSP_DDM

/////////////////////////////////////string/////////////////////////////////////
#include <string>
BEG_NSP_DDM
using ddstra = std::string;
using ddstrw = std::wstring;
using ddchara = char;
using ddcharw = wchar_t;

#ifdef _UNICODE
#define _DDT(x) L ## x
using ddstr = ddstrw;
using ddchar = ddcharw;
#else
#define _DDT
using ddstr = ddstra;
using ddchar = ddchara;
#endif // 

// string 对于 tc 的模板
template<class tc>
using ddstrt = std::basic_string<tc, std::char_traits<tc>, std::allocator<tc> >;
END_NSP_DDM

/////////////////////////////////////buff/////////////////////////////////////
#include <vector>
BEG_NSP_DDM
using ddbuff = std::vector<char>;

// ddbuff 对于 tc 的模板
template<class tc>
using ddbufft = std::vector<tc>;
END_NSP_DDM

/////////////////////////////////////development/////////////////////////////////////
#define DD_WINDOW 0

/////////////////////////////////////include/////////////////////////////////////
#include "ddassert.h"

#endif // g_def_h_
