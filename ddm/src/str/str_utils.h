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

    ///////////////////////////////����ת������///////////////////////////////////////////
public:
    /** ���ַ�ת���ַ�,��std����ת����ע�����ת������ԭ���ԣ�Ҳ����˵���̼߳����ǲ��е�
    @param [in] src Դ������һ��ΪUnicode
    @param [in] des ���������һ��ΪUnicode
    @return �Ƿ�ɹ�
    */
    static bool uft16_uft8(const std::wstring& src, std::string& des);

    /** ���ַ�ת���ַ�
    @param [in] src Դ������һ��ΪUnicode
    @param [in] des ���������һ��ΪUnicode
    @return �Ƿ�ɹ�
    */
    static bool uft16_uft8_ex(const std::wstring& src, std::string& des);

    /** ���ַ�ת���ַ�������һ��ΪUNICODE�룩��ע�����ת������ԭ���ԣ�Ҳ����˵���̼߳����ǲ��е�
    @param [in] src Դ������һ��ΪUnicode
    @param [in] des ���������һ��ΪUnicode
    @return �Ƿ�ɹ�
    */
    static bool uft8_uft16(const std::string& src, std::wstring& des);

    /** ���ַ�ת���ַ�
    @param [in] src Դ������һ��ΪUnicode
    @param [in] des ���������һ��ΪUnicode
    @return �Ƿ�ɹ�
    */
    static bool uft8_uft16_ex(const std::string& src, std::wstring& des);

    /** ���ַ�תANSI������һ��ΪUNICODE�룩
    @param [in] src Դ������һ��ΪUnicode
    @param [in] des ���������ΪANSI windows���й�����һ��ΪGBK
    @return �Ƿ�ɹ�
    */
    static bool uft16_ansi(const std::wstring& src, std::string& des);

    /** ANSIת���ַ�������һ��ΪUNICODE�룩
    @param [in] src Դ������ΪANSI windows���й�����һ��ΪGBK
    @param [in] des ���������һ��ΪUnicode
    @return �Ƿ�ɹ�
    */
    static bool ansi_uft16(const std::string& src, std::wstring& des);

    ///////////////////////////////c�ַ���������///////////////////////////////////////////
public:
    /** �ַ���ǰn���Ƚ�
    @param [in] src Դ
    @param [in] find �Ƚ�
    @param [in] n ����
    @param [in] icase �Ƿ���Դ�Сд
    @return 0 ��� > 0 src �� < 0 find ��
    */
    static int strncmp(const char* src, const char* find, int n, bool icase);

    /** �ַ����Ƚ�
    @param [in] src Դ
    @param [in] find �Ƚ�
    @param [in] icase �Ƿ���Դ�Сд
    @return 0 ��� > 0 src �� < 0 find ��
    */
    static int strcmp(const char* src, const char* find, bool icase);

    /** ��...��β
    @param [in] src Դ
    @param [in] find
    @param [in] icase �Ƿ���Դ�Сд
    @return src �Ƿ���find��β
    */
    static bool strendof(const char* src, const char* find, bool icase);

    /** ��...��ʼ
    @param [in] src Դ
    @param [in] find
    @param [in] icase �Ƿ���Դ�Сд
    @return src �Ƿ���find��ʼ
    */
    static bool strbegof(const char* src, const char* find, bool icase);

    /** �ַ�������
    @param [in] src Դ
    @param [in] find
    @param [in] icase �Ƿ���Դ�Сд
    @return src �ҵ� find ? �ǣ������״γ��ֵĵ�ַ����0
    */
    static char* strstr(const char* src, const char* find, bool icase);

    /** �ַ������� '*' ����һ�����߶�������ַ���'?' ����һ���ַ���UNICODE�����������ַ���
    @param [in] src Դ
    @param [in] find
    @param [in] icase �Ƿ���Դ�Сд
    @return src �ҵ� find ? �ǣ������״γ��ֵĵ�ַ����0
    */
    static bool strwildcard(const char* src, const char* find, bool icase);

    ////////////////////////////һЩ���ɣ������ַ����ָ���һ�еȺ���//////////////////////////////////////////////
public:
    /** �����ַ���ֱ��
    @return ��һ�η�����������'\0'�ĵط�
    @note : DongStringT<tc>(pBeg, pNex - pBeg)
    */
    template<class tc, class F>
    static inline const tc* str_parse_until(const tc* str, F func)
    {
        assert(str != nullptr);
        for (; *str && !func(*str); ++str);
        return str;
    }

    /** ���str�е�һ�У�\r\n ���� \n �ָ����\r\n
    @note: ʹ��ddstrt<tc>(pBeg, pNex - pBeg)��ȡ��һ��
    @eg: a b c \r\n c e ���� c ��ָ�룻
    @param [in] str �ַ������� '\0' ��β
    @return ��һ�еĿ�ʼ
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

    /** �����ַ���
    @param [in] str Դ����һ���ַ�������'\0'��β
    @param [in] finder ��һ���ַ�������'\0'��β
    @param [in] times �����Ĵ���
    @return �����ַ���ֱ����times������ finder ��'\0'�ĵط�
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

            // �ҵ���
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

    /** �ָ��ַ���
    @param [in] str Դ����һ���ַ�������'\0'��β
    @param [in] spliter �ָ����ţ���һ���ַ�������'\0'��β
    @param [out] out ���
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

            // �ҵ���
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
