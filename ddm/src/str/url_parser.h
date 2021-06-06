
#ifndef url_parser_h_
#define url_parser_h_ 1

#include "g_def.h"

#include <regex>
BEG_NSP_DDM
/** url
һ����utf-8����
scheme://[user[:password]@]host[:port] [/path] [?query] [#fragment]
*/
struct dd_url
{
    /** ָ���Ͳ�ʹ�õ�Э��(���磺http, https, ftp)
    */
    ddstra m_scheme;

    /** �û�����
    @note: [��ʡ��]
    */
    ddstra m_user;

    /** ����
    @note: [��ʡ��]
    */
    ddstra m_password;

    /** ��������ͨ��Ϊ��������ʱΪIP��ַ
    */
    ddstra m_host;

    /** �˿ڡ������ַ�ʽ��ʾ,Ĭ��ֵ��:80��
    @note: [��ʡ��]
    */
    unsigned int m_port = 0;

    /** ·�����ԡ�/���ַ�����·���е�ÿһ��Ŀ¼����
    @note: [��ʡ��]
    */
    ddstra m_path;

    /** ��ѯ��GETģʽ�Ĵ���������ԡ�?���ַ�Ϊ��㣬ÿ�������ԡ�&�����������ԡ�=���ֿ��������������ݣ�ͨ����UTF8��URL���룬�ܿ��ַ���ͻ������
    @note: [��ʡ��]
    */
    ddstra m_query;

    /** Ƭ�Ρ��ԡ�#���ַ�Ϊ���
    @note: [��ʡ��]
    */
    ddstra m_fragment;

    /** url �Ƿ���Ч
    @return �Ƿ���Ч
    */
    bool is_valid() {
        return !m_scheme.empty() && m_port != 0 && !m_host.empty();
    }

    /** ��ø�ʽ�����URL
    @note scheme:[//[user[:password]@]host[:port]] [/path] [?query] [#fragment]
    @return ��ʽ�����URL
    */
    ddstra get_formated_url()
    {
        ddstra formatedUrl = "";

        if (!is_valid()) {
            return formatedUrl;
        }

        formatedUrl += m_scheme;
        formatedUrl += "://";

        if (m_user.length() != 0 || m_password.length() != 0) {
            formatedUrl += m_user;

            if (m_password.length() != 0) {
                formatedUrl += ":";
                formatedUrl += m_password;
            }
            
            formatedUrl += "@";
        }

        formatedUrl += m_host;

        if (!( (m_port == 80) &&
               (m_scheme.length() >= 4) &&
               (m_scheme[0] == 'h' || m_scheme[0] == 'H') &&
               (m_scheme[1] == 't' || m_scheme[1] == 'T') &&
               (m_scheme[2] == 't' || m_scheme[2] == 'T') &&
               (m_scheme[3] == 'p' || m_scheme[3] == 'P') )) {
            formatedUrl += ":";
            char portBuff[10] = {0};
            ::_itoa_s(m_port, portBuff, 10);
            formatedUrl += portBuff;
        }

        if (m_path.length() > 0) {
            formatedUrl += m_path;
        }

        if (m_query.length() > 0) {
            formatedUrl += "?";
            formatedUrl += m_query;
        }

        if (m_fragment.length() > 0) {
            formatedUrl += "#";
            formatedUrl += m_fragment;
        }

        return formatedUrl;
    }
};

/** ����url
@note: �Ƿ�ɹ���dogUrl �� is_valid �ж�
@param [in] url urlԭʼ�ַ�����һ����UTF-8���룩
@param [out] dogUrl ���
*/
inline void parse_url(const ddstra& url, dd_url& dogUrl)
{
    size_t index = 0;
    size_t len = url.length();

    // ��scheme
    for (size_t i = index; i < len; ++i) {
        if (url[i] == ':') {
            dogUrl.m_scheme = url.substr(index, i - index);
            index = i + 1;
            break;
        }
    }

    if (dogUrl.m_scheme.empty()) {
        return;
    }

    // �̶� // ���� \\ 
    if (index + 1 >= len ||
        (url[index] != '/' && url[index] != '\\') ||
        (url[index + 1] != '/' && url[index + 1] != '\\')) {
        return;
    }

    // ָ�� ��/��
    index += 1;

    // [user[:password]@]host[:port] ��[/path] [?query] [#fragment]��
    // �ҵ���չ���� ��/ \ ? #��
    size_t hostPortEndIndex = len - 1;
    for (size_t i = index + 1; i < len; ++i) {
        if (url[i] == '/' || url[i] == '\\' || url[i] == '?' || url[i] == '#') {
            hostPortEndIndex = i - 1;
            break;
        }
    }

    // ����Ϊ0
    if (hostPortEndIndex <= index) {
        return;
    }

    // �ҵ�@���ţ�˵�����û�������
    {
        size_t userPswEndIndex = 0;
        for (size_t i = index + 1; i <= hostPortEndIndex; ++i) {
            if (url[i] == '@') {
                userPswEndIndex = i - 1;

                // ���Ȳ�Ϊ0
                if (userPswEndIndex > index) {
                    // Ѱ�� ��:��
                    size_t userEndIndex = userPswEndIndex;
                    for (size_t j = index + 1; j <= userPswEndIndex; ++j) {
                        if (url[j] == ':') {
                            userEndIndex = j - 1;
                            break;
                        }
                    }

                    dogUrl.m_user = url.substr(index + 1, userEndIndex - index);

                    if (userEndIndex < userPswEndIndex - 2) {
                        dogUrl.m_password = url.substr(userEndIndex + 2, userPswEndIndex - userEndIndex - 1);
                    }
                }

                // ָ�� ��@��
                index = userPswEndIndex + 1;
                break;
            }
        }
    }

    // host �˿�
    {
        if (index < hostPortEndIndex) {
            // Ѱ�� ��:��
            size_t hostEndIndex = hostPortEndIndex;
            for (size_t j = index + 1; j <= hostPortEndIndex; ++j) {
                if (url[j] == ':') {
                    hostEndIndex = j - 1;
                    break;
                }
            }

            // ����Ϊ0
            if (hostEndIndex < index) {
                return;
            }

            dogUrl.m_host = url.substr(index + 1, hostEndIndex - index);

            if (hostEndIndex < hostPortEndIndex - 2) {
                dogUrl.m_port = ::atoi(url.substr(hostEndIndex + 2, hostPortEndIndex - hostEndIndex - 1).c_str());
            }

            if (dogUrl.m_port == 0) {
                dogUrl.m_port = 80;
            }

            index = hostPortEndIndex + 1;
        } else {
            return;
        }
    }

    while (index < len) {
        // Ѱ����һ����չ����
        size_t extendEndIndex = len - 1;
        for (size_t i = index + 1; i < len; ++i) {
            if (url[i] == '?' || url[i] == '#' || url[i] == '/' || url[i] == '\\') {
                extendEndIndex = i - 1;
                break;
            }
        }

        // �����һ�������� "/" ���� "\\" ��ô������m_path
        if (url[index] == '/' || url[index] == '\\') {
            // ����Ѱ��ֱ�� ? #
            size_t i = extendEndIndex;
            extendEndIndex = len - 1;
            for (; i < len; ++i) {
                if (url[i] == '?' || url[i] == '#') {
                    extendEndIndex = i - 1;
                    break;
                }
            }

            dogUrl.m_path = url.substr(index, extendEndIndex - index + 1);
            size_t pathEndIndex = 0;

            for (size_t i = 0; i < dogUrl.m_path.length(); ++i) {
                if (dogUrl.m_path[i] == '\\' || dogUrl.m_path[i] == '/') {
                    if (pathEndIndex == 0 || dogUrl.m_path[pathEndIndex - 1] != '/') {
                        dogUrl.m_path[pathEndIndex++] = '/';
                    }
                } else {
                    dogUrl.m_path[pathEndIndex++] = dogUrl.m_path[i];
                }
            }

            dogUrl.m_path[pathEndIndex] = 0;
            dogUrl.m_path = dogUrl.m_path.c_str();
        } else if (url[index] == '?') {
            // �����һ�������� "?" ��ô���m_query
            dogUrl.m_query = url.substr(index + 1, extendEndIndex - index);
        } else if (url[index] == '#') {
            // �����һ�������� "#" ��ô���m_fragment
            dogUrl.m_fragment = url.substr(index + 1, extendEndIndex - index);
        }

        index = extendEndIndex + 1;
    }
}

/** ������ʽ����url,��parse_url���в�ͬparse_urlRegex��[/path] [?query] [#fragment]��˳���ϸ�Ҫ��
scheme://[user[:password]@]host[:port] [/path] [?query] [#fragment]
@note: �Ƿ�ɹ���dogUrl �� is_valid �ж�
@param [in] url urlԭʼ�ַ�����һ����UTF-8���룩
@param [out] dogUrl ���
*/
inline void parse_url_regex(const ddstra& url, dd_url& dogUrl)
{
    // note: ������ʽ�� ��������"\"��"]"��Ҫת�壬�����Ĳ���Ҫ
    ddstra parttenStr;

    // scheme://
    parttenStr += R"__(([a-zA-Z]+):[\\/]{2})__";

    // [user[:password]@]
    parttenStr += R"__((?:([^:@]*)(?::([^@]*))?@)?)__";

    // host[:port]
    parttenStr += R"__(([^/\\#?:]*)(?::([0-9]{0,5}))?)__";

    // [/path]
    parttenStr += R"__((?:([/\\][^?#]*))?)__";

    // [?query]
    parttenStr += R"__((?:[?]([^?#/\\]*))?)__";

    // [#fragment]
    parttenStr += R"__((?:[#]([^?#/\\]*))?)__";

    std::regex partten(parttenStr);
    std::smatch results;

    if (!std::regex_search(url, results, partten) || results.size() != 9) {
        return;
    }

    dogUrl.m_scheme = results[1];
    dogUrl.m_user = results[2];
    dogUrl.m_password = results[3];
    dogUrl.m_host = results[4];
    dogUrl.m_port = ::atoi(results[5].str().c_str());
    dogUrl.m_path = results[6];
    dogUrl.m_query = results[7];
    dogUrl.m_fragment = results[8];

    size_t pathEndIndex = 0;

    for (size_t i = 0; i < dogUrl.m_path.length(); ++i) {
        if (dogUrl.m_path[i] == '\\' || dogUrl.m_path[i] == '/') {
            if (pathEndIndex == 0 || dogUrl.m_path[pathEndIndex - 1] != '/') {
                dogUrl.m_path[pathEndIndex++] = '/';
            }
        } else {
            dogUrl.m_path[pathEndIndex++] = dogUrl.m_path[i];
        }
    }

    dogUrl.m_path[pathEndIndex] = 0;
    dogUrl.m_path = dogUrl.m_path.c_str();

    if (dogUrl.m_port == 0) {
        dogUrl.m_port = 80;
    }
}

END_NSP_DDM
#endif // url_parser_h_
