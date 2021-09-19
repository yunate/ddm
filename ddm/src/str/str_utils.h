#ifndef str_utils_h_
#define str_utils_h_ 1

#include "g_def.h"
#include <stdarg.h>
BEG_NSP_DDM
class str_utils
{
public:
    template<class ...Args>
    inline static std::string str_format(const std::string& format, Args... args)
    {
        char buff[10240];
        int cnt = ::sprintf_s(buff, std::size(buff), format.c_str(), args...);
        return buff;
    }

    template<class ...Args>
    static std::wstring str_format(const std::wstring& format, Args...args)
    {
        wchar_t buff[10240];
        int cnt = ::swprintf_s(buff, std::size(buff), format.c_str(), args...);
        return buff;
    }

    ///////////////////////////////编码转换部分///////////////////////////////////////////
public:
    /** 宽字符转多字符,调std库中转换，注意这个转换具有原子性，也就是说多线程加速是不行的
    @param [in] src 源，编码一般为Unicode
    @param [in] des 结果，编码一般为Unicode
    @return 是否成功
    */
    static bool uft16_uft8(const std::wstring& src, std::string& des);

    /** 宽字符转多字符
    @param [in] src 源，编码一般为Unicode
    @param [in] des 结果，编码一般为Unicode
    @return 是否成功
    */
    static bool uft16_uft8_ex(const std::wstring& src, std::string& des);

    /** 多字符转宽字符（编码一般为UNICODE码），注意这个转换具有原子性，也就是说多线程加速是不行的
    @param [in] src 源，编码一般为Unicode
    @param [in] des 结果，编码一般为Unicode
    @return 是否成功
    */
    static bool uft8_uft16(const std::string& src, std::wstring& des);

    /** 宽字符转多字符
    @param [in] src 源，编码一般为Unicode
    @param [in] des 结果，编码一般为Unicode
    @return 是否成功
    */
    static bool uft8_uft16_ex(const std::string& src, std::wstring& des);

    /** 宽字符转ANSI（编码一般为UNICODE码）
    @param [in] src 源，编码一般为Unicode
    @param [in] des 结果，编码为ANSI windows下中国编码一般为GBK
    @return 是否成功
    */
    static bool uft16_ansi(const std::wstring& src, std::string& des);

    /** ANSI转宽字符（编码一般为UNICODE码）
    @param [in] src 源，编码为ANSI windows下中国编码一般为GBK
    @param [in] des 结果，编码一般为Unicode
    @return 是否成功
    */
    static bool ansi_uft16(const std::string& src, std::wstring& des);

    ///////////////////////////////c字符串处理部分///////////////////////////////////////////
public:
    /** 字符串前n个比较
    @param [in] src 源
    @param [in] find 比较
    @param [in] n 个数
    @param [in] icase 是否忽略大小写
    @return 0 相等 > 0 src 大 < 0 find 大
    */
    static int strncmp(const char* src, const char* find, int n, bool icase);

    /** 字符串比较
    @param [in] src 源
    @param [in] find 比较
    @param [in] icase 是否忽略大小写
    @return 0 相等 > 0 src 大 < 0 find 大
    */
    static int strcmp(const char* src, const char* find, bool icase);

    /** 以...结尾
    @param [in] src 源
    @param [in] find
    @param [in] icase 是否忽略大小写
    @return src 是否以find结尾
    */
    static bool strendof(const char* src, const char* find, bool icase);

    /** 以...开始
    @param [in] src 源
    @param [in] find
    @param [in] icase 是否忽略大小写
    @return src 是否以find开始
    */
    static bool strbegof(const char* src, const char* find, bool icase);

    /** 字符串查找
    @param [in] src 源
    @param [in] find
    @param [in] icase 是否忽略大小写
    @return src 找到 find ? 是，返回首次出现的地址；否0
    */
    static char* strstr(const char* src, const char* find, bool icase);

    /** 字符串查找 '*' 代表一个或者多个任意字符；'?' 代表一个字符（UNICODE中文是两个字符）
    @param [in] src 源
    @param [in] find
    @param [in] icase 是否忽略大小写
    @return src 找到 find ? 是，返回首次出现的地址；否0
    */
    static bool strwildcard(const char* src, const char* find, bool icase);

    ////////////////////////////一些技巧，包括字符串分割，获得一行等函数//////////////////////////////////////////////
public:
    /** 遍历字符串直到
    @return 第一次符合条件或者'\0'的地方
    @note : DongStringT<tc>(pBeg, pNex - pBeg)
    */
    template<class tc, class F>
    static inline const tc* str_parse_until(const tc* str, F func)
    {
        assert(str != nullptr);
        for (; *str && !func(*str); ++str);
        return str;
    }

    /** 获得str中的一行（\r\n 或者 \n 分割）包括\r\n
    @note: 使用ddstrt<tc>(pBeg, pNex - pBeg)来取得一行
    @eg: a b c \r\n c e 返回 c 的指针；
    @param [in] str 字符串，以 '\0' 结尾
    @return 下一行的开始
    */
    template<class tc>
    static inline const tc* str_getline(const tc* str)
    {
        assert(str != nullptr);

        const tc* lr = str_parse_until(str, [](tc ch) {
            return (ch == (tc)('\r') || ch == (tc)('\n'));
                                       });

        if (*lr == 0) {
            return lr;
        }

        if (*(lr) == (tc)('\r')) {
            ++lr;
        }

        if (*(lr) == (tc)('\n')) {
            ++lr;
        }

        return lr;
    }

    /** 查找字符串
    @param [in] str 源，是一个字符串，以'\0'结尾
    @param [in] finder 是一个字符串，以'\0'结尾
    @param [in] times 遇到的次数
    @return 遍历字符串直到第times次遇到 finder 或'\0'的地方
    */
    template<class tc>
    static inline const tc* str_findn(const tc* str, const tc* finder, int n)
    {
        assert(str != nullptr);
        assert(finder != nullptr);
        const tc* i = str;
        const tc* ic = finder;
        int t = 0;

        while (1)
        {
            if (*i++ != *ic) {
                if (*i == 0) {
                    break;
                }

                continue;
            }

            const tc* in = i;
            ++ic;

            while (*ic == *i) {
                ++i;
                ++ic;

                if (*ic == 0) {
                    break;
                }
            }

            // 找到了
            if (*ic == 0) {
                if (++t == n) {
                    return in - 1;
                }
            }
            else {
                i = in;
            }

            ic = finder;
        }

        return 0;
    }

    /** 分割字符串
    @param [in] str 源，是一个字符串，以'\0'结尾
    @param [in] spliter 分隔符号，是一个字符串，以'\0'结尾
    @param [out] out 输出
    */
    template<class tc>
    static inline void str_split(const tc* str, const tc* spliter, std::vector<ddstrt<tc> >& out)
    {
        assert(str != nullptr);
        assert(spliter != nullptr);
        const tc* beg = str;
        const tc* i = str;
        const tc* ic = spliter;

        while (1)
        {
            if (*i++ != *ic) {
                if (*i == 0) {
                    out.push_back(ddstrt<tc>(beg, i - beg));
                    break;
                }

                continue;
            }

            const tc* in = i;
            ++ic;

            while (*ic == *i) {
                ++i;
                ++ic;

                if (*ic == 0) {
                    break;
                }
            }

            // 找到了
            if (*ic == 0) {
                out.push_back(ddstrt<tc>(beg, in - 1 - beg));
                beg = i;
            }
            else {
                i = in;
            }

            ic = spliter;
        }
    }
};

END_NSP_DDM
#endif // str_utils_h_
