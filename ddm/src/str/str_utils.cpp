#define  _CRT_SECURE_NO_WARNINGS

#include "str_utils.h"
#include "g_def.h"
#include <codecvt>

BEG_NSP_DDM

bool str_utils::uft16_uft8(const std::wstring& src, std::string& des)
{
    try {
        std::wstring_convert<std::codecvt_utf8<wchar_t> > wcv;
        des = wcv.to_bytes(src);
    } catch (const std::exception&) {
        return false;
    }

    return true;
}

static bool uft16_uft8_ex_core(char* dest, size_t* destLen, const wchar_t* src, size_t srcLen)
{
    *destLen = 0;
    for (size_t i = 0; i < srcLen; ++i) {
        unsigned long unic = src[i];
        if (unic <= 0x0000007F) {
            // * U-00000000 - U-0000007F:  0xxxxxxx
            if (!dest) {
                (*destLen) += 1;
                continue;
            }

            dest[(*destLen)++] = (unic & 0x7F);
        } else if (unic >= 0x00000080 && unic <= 0x000007FF) {
            // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx
            if (!dest) {
                (*destLen) += 2;
                continue;
            }
            dest[(*destLen)++] = ((unic >> 6) & 0x1F) | 0xC0;
            dest[(*destLen)++] = (unic & 0x3F) | 0x80;
        } else if (unic >= 0x00000800 && unic <= 0x0000FFFF) {
            // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx
            if (!dest)
            {
                (*destLen) += 3;
                continue;
            }

            dest[(*destLen)++] = ((unic >> 12) & 0x0F) | 0xE0;
            dest[(*destLen)++] = ((unic >> 6) & 0x3F) | 0x80;
            dest[(*destLen)++] = (unic & 0x3F) | 0x80;
        } else if (unic >= 0x00010000 && unic <= 0x001FFFFF) {
            // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            if (!dest) {
                (*destLen) += 4;
                continue;
            }

            dest[(*destLen)++] = ((unic >> 18) & 0x07) | 0xF0;
            dest[(*destLen)++] = ((unic >> 12) & 0x3F) | 0x80;
            dest[(*destLen)++] = ((unic >> 6) & 0x3F) | 0x80;
            dest[(*destLen)++] = (unic & 0x3F) | 0x80;
        } else if (unic >= 0x00200000 && unic <= 0x03FFFFFF) {
            // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            if (!dest) {
                (*destLen) += 5;
                continue;
            }

            dest[(*destLen)++] = ((unic >> 24) & 0x03) | 0xF8;
            dest[(*destLen)++] = ((unic >> 18) & 0x3F) | 0x80;
            dest[(*destLen)++] = ((unic >> 12) & 0x3F) | 0x80;
            dest[(*destLen)++] = ((unic >> 6) & 0x3F) | 0x80;
            dest[(*destLen)++] = (unic & 0x3F) | 0x80;
        } else if (unic >= 0x04000000 && unic <= 0x7FFFFFFF) {
            // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
            if (!dest) {
                (*destLen) += 6;
                continue;
            }

            dest[(*destLen)++] = ((unic >> 30) & 0x01) | 0xFC;
            dest[(*destLen)++] = ((unic >> 24) & 0x3F) | 0x80;
            dest[(*destLen)++] = ((unic >> 18) & 0x3F) | 0x80;
            dest[(*destLen)++] = ((unic >> 12) & 0x3F) | 0x80;
            dest[(*destLen)++] = ((unic >> 6) & 0x3F) | 0x80;
            dest[(*destLen)++] = (unic & 0x3F) | 0x80;
        }
    }

    return (*destLen != 0);
}

bool str_utils::uft16_uft8_ex(const std::wstring& src, std::string& des)
{
    size_t destLen = 0;
    uft16_uft8_ex_core(nullptr, &destLen, src.c_str(), src.length());

    if (destLen == 0) {
        return false;
    }

    des.resize(destLen);
    bool res = uft16_uft8_ex_core((char*)des.c_str(), &destLen, src.c_str(), src.length());
    return res;
}

bool str_utils::uft8_uft16(const std::string& src, std::wstring& des)
{
    try {
        std::wstring_convert<std::codecvt_utf8<wchar_t> > wcv;
        des = wcv.from_bytes(src);
    } catch (const std::exception&) {
        return false;
    }

    return true;
}

const u8 kUtf8Limits[5] = { 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static bool uft8_uft16_ex_core(wchar_t* dest, size_t* destLen, const char* src, size_t srcLen)
{
    size_t destPos = 0, srcPos = 0;
    for (;;) {
        u8 c;
        unsigned numAdds;
        if (srcPos == srcLen) {
            *destLen = destPos;
            return true;
        }
        c = (u8)src[srcPos++];
        if (c < 0x80) {
            if (dest)
                dest[destPos] = (wchar_t)c;
            destPos++;
            continue;
        }
        if (c < 0xC0) {
            break;
        }
        for (numAdds = 1; numAdds < 5; numAdds++) {
            if (c < kUtf8Limits[numAdds]) {
                break;
            }
        }
        u32 value = (c - kUtf8Limits[numAdds - 1]);
        do {
            u8 c2;
            if (srcPos == srcLen) {
                break;
            }
            c2 = (u8)src[srcPos++];
            if (c2 < 0x80 || c2 >= 0xC0) {
                break;
            }
            value <<= 6;
            value |= (c2 - 0x80);
        } while (--numAdds);

        if (value < 0x10000) {
            if (dest) {
                dest[destPos] = (wchar_t)value;
            }
            destPos++;
        } else {
            value -= 0x10000;
            if (value >= 0x100000) {
                break;
            }
            if (dest) {
                dest[destPos + 0] = (wchar_t)(0xD800 + (value >> 10));
                dest[destPos + 1] = (wchar_t)(0xDC00 + (value & 0x3FF));
            }
            destPos += 2;
        }
    }
    *destLen = destPos;
    return false;
}

bool str_utils::uft8_uft16_ex(const std::string& src, std::wstring& des)
{
    size_t destLen = 0;
    uft8_uft16_ex_core(nullptr, &destLen, src.c_str(), src.length());

    if (destLen == 0) {
        return false;
    }

    des.resize(destLen);
    bool res = uft8_uft16_ex_core((wchar_t*)des.c_str(), &destLen, src.c_str(), src.length());
    return res;
}

bool str_utils::uft16_ansi(const std::wstring& src, std::string& des)
{
    setlocale(LC_CTYPE, "");
    std::mbstate_t state = {};
    const wchar_t* psrc = src.data();
    size_t len = std::wcsrtombs(nullptr, &psrc, 0, &state);

    if (static_cast<size_t>(-1) != len)
    {
        char* buff = new char[len + 1];
        len = std::wcsrtombs(buff, &psrc, len, &state);

        if (static_cast<size_t>(-1) != len)
        {
            des.assign(buff, len);
        } else {
            delete []buff;
            return false;
        }

        delete []buff;
    } else {
        return false;
    }

    return true;
}

bool str_utils::ansi_uft16(const std::string& src, std::wstring& des)
{
    setlocale(LC_CTYPE, "");
    std::mbstate_t state = {};
    const char* psrc = src.data();
    size_t len = std::mbsrtowcs(nullptr, &psrc, 0, &state);

    if (static_cast<size_t>(-1) != len)
    {
        wchar_t* buff = new wchar_t[len + 1];
        len = std::mbsrtowcs(buff, &psrc, len, &state);

        if (static_cast<size_t>(-1) != len) {
            des.assign(buff, len);
        } else {
            delete []buff;
            return false;
        }

        delete []buff;
    } else {
        return false;
    }

    return true;
}

static u8 kAsciiLowerCase[] = {
    0x00, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
    0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40,
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
    0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64,
    0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C,
    0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
    0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
    0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0,
    0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC,
    0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8,
    0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4,
    0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0,
    0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC,
    0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8,
    0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF1, 0xF2, 0xF3, 0xF4,
    0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

// 字符串比较
static int __strcmp(const char* s1, const char* s2)
{
    while (*s1 == *s2++)
    {
        if (*s1++ == 0)
        {
            return (0);
        }
    }

    return (*(u8*)s1 - *(u8*)--s2);
}

// 字符串比较（忽略大小写）
static int __strcmpi(const char* s1, const char* s2)
{
    const u8* us1 = (const u8*)s1;
    const u8* us2 = (const u8*)s2;

    while (kAsciiLowerCase[*us1] == kAsciiLowerCase[*us2++])
    {
        if (*us1++ == '\0')
        {
            return (0);
        }
    }

    return (kAsciiLowerCase[*us1] - kAsciiLowerCase[*--us2]);
}

// 字符串前n个字符比较
static int __strncmp(const char* s1, const char* s2, size_t n)
{
    if (n == 0)
    {
        return (0);
    }

    do
    {
        if (*s1 != *s2++)
        {
            return (*(u8*)s1 - *(u8*)--s2);
        }

        if (*s1++ == 0)
        {
            break;
        }
    } while (--n != 0);

    return (0);
}

// 字符串前n个字符比较（忽略大小写）
static int __strncmpi(const char* s1, const char* s2, size_t n)
{
    if (n != 0)
    {
        const u8* us1 = (const u8*)s1;
        const u8* us2 = (const u8*)s2;

        do
        {
            if (kAsciiLowerCase[*us1] != kAsciiLowerCase[*us2++])
            {
                return (kAsciiLowerCase[*us1] - kAsciiLowerCase[*--us2]);
            }

            if (*us1++ == '\0')
            {
                break;
            }
        } while (--n != 0);
    }

    return (0);
}

// str 是否以 find 结尾
static bool __strendof(const char* str, const char* find)
{
    int lenstr = (int)::strlen(str);
    int lensub = (int)::strlen(find);

    if (lenstr < lensub)
    {
        return false;
    }

    return memcmp(str + lenstr - lensub, find, lensub) == 0;
}

// str 是否以 find 结尾（忽略大小写）
static bool __strendofi(const char* str, const char* find)
{
    const u8* ufind = (const u8*)find;
    int lenstr = (int)::strlen(str);
    int lensub = (int)::strlen(find);

    if (lenstr < lensub)
    {
        return false;
    }

    const u8* pos = (const u8*)(str + lenstr - lensub);

    for (int i = 0; i < lensub; i++)
    {
        if (kAsciiLowerCase[pos[i]] != kAsciiLowerCase[ufind[i]])
        {
            return false;
        }
    }

    return true;
}

// str 是否以 find 开始
static bool __strbegof(const char* str, const char* find)
{
    int lenstr = (int)::strlen(str);
    int lensub = (int)::strlen(find);

    if (lenstr < lensub)
    {
        return false;
    }

    return memcmp(str, find, lensub) == 0;
}

// str 是否以 find 开始（忽略大小写）
static bool __strbegofi(const char* str, const char* find)
{
    const u8* ustr = (const u8*)str;
    const u8* ufind = (const u8*)find;
    int lenstr = (int)::strlen(str);
    int lensub = (int)::strlen(find);

    if (lenstr < lensub)
    {
        return false;
    }

    for (int i = 0; i < lensub; i++)
    {
        if (kAsciiLowerCase[ustr[i]] != kAsciiLowerCase[ufind[i]])
        {
            return false;
        }
    }

    return true;
}

/** find 是否是str的子串。
    是，返回首次出现的地址；否0
    算法：暴力法，除了像（abcabcabc...）这种有很多重复的，这个暴力法的效率已经很高了，所以不要去优化
*/
static char* __strstr(const char* str, const char* find)
{
    char c, sc;
    size_t len;

    if ((c = *find++) != 0)
    {
        len = strlen(find);

        do
        {
            do
            {
                if ((sc = *str++) == 0)
                {
                    return (nullptr);
                }
            } while (sc != c);

        } while (__strncmp(str, find, len) != 0);

        str--;
    }

    return ((char*)str);
}

/** find 是否是str的子串。（忽略大小写）
    是，返回首次出现的地址；否0
    算法：暴力法，除了像（abcabcabc...）这种有很多重复的，这个暴力法的效率已经很高了，所以不要去优化
*/
static char* __strstri(const char* str, const char* find)
{
    u8 c, sc;
    size_t len;

    if ((c = *find++) != 0)
    {
        len = strlen(find);

        do
        {
            do
            {
                if ((sc = *str++) == 0)
                {
                    return (nullptr);
                }

            } while (kAsciiLowerCase[sc] != kAsciiLowerCase[c]);

        } while (__strncmpi(str, find, len) != 0);

        str--;
    }

    return ((char*)str);
}

/** find 和 str 是否匹配（相等）。
    '*' 代表一个或者多个任意字符；'?' 代表一个字符（UNICODE中文是两个字符）
    算法：暴力法
*/
static bool __wildcard(const char* str, const char* find)
{
    char const* back_find = nullptr, * back_str = back_str = nullptr;

    for (;;)
    {
        u8 c = *str++;
        u8 d = *find++;

        switch (d)
        {
        case '?':
            {
                if (c == '\0')
                {
                    return false;
                }

                break;
            }
        case '*':
            {
                if (*find == '\0')
                {
                    return true;
                }

                back_find = find;
                back_str = --str;
                break;
            }
        default:
            {
                if (c == d)
                {
                    if (d == '\0')
                    {
                        return true;
                    }

                    break;
                }

                if (c == '\0' || !back_find)
                {
                    /* No point continuing */
                    return false;
                }

                /* Try again from last *, one character later in str. */
                find = back_find;
                str = ++back_str;
                break;
            }
        }
    }
}

/** find 和 str 是否匹配（相等）。（忽略大小写）
    '*' 代表一个或者多个任意字符；'?' 代表一个字符（UNICODE中文是两个字符）
    算法：暴力法
*/
static bool __wildcardi(const char* str, const char* find)
{
    char const* back_find = nullptr, * back_str = back_str = nullptr;

    for (;;)
    {
        u8 c = *str++;
        u8 d = *find++;

        switch (d)
        {
        case '?':
            {
                if (c == '\0')
                {
                    return false;
                }

                break;
            }
        case '*':
            {
                if (*find == '\0')
                {
                    return true;
                }

                back_find = find;
                back_str = --str;
                break;
            }
        default:
            {
                if (kAsciiLowerCase[c] == kAsciiLowerCase[d])
                {
                    if (d == '\0')
                    {
                        return true;
                    }

                    break;
                }

                if (c == '\0' || !back_find)
                {
                    /* No point continuing */
                    return false;
                }

                /* Try again from last *, one character later in str. */
                find = back_find;
                str = ++back_str;
                break;
            }
        }
    }
}

int str_utils::strcmp(const char* s, const char* find, bool icase)
{
    return icase ? __strcmpi(s, find) : __strcmp(s, find);
}

int str_utils::strncmp(const char* s, const char* find, int n, bool icase)
{
    return icase ? __strncmpi(s, find, n) : __strncmp(s, find, n);
}

bool str_utils::strendof(const char* s, const char* find, bool icase)
{
    return icase ? __strendofi(s, find) : __strendof(s, find);
}

bool str_utils::strbegof(const char* s, const char* find, bool icase)
{
    return icase ? __strbegofi(s, find) : __strbegof(s, find);
}

char* str_utils::strstr(const char* s, const char* find, bool icase)
{
    return icase ? __strstri(s, find) : __strstr(s, find);
}

bool str_utils::strwildcard(const char* s, const char* find, bool icase)
{
    return icase ? __wildcardi(s, find) : __wildcard(s, find);
}

END_NSP_DDM
