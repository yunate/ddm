
#ifndef __URL_H_
#define __URL_H_

#include "typedef/DogString.h"

/** url
scheme:[//[user[:password]@]host[:port]] [/path] [?query] [#fragment]
*/
struct DogUrl
{
    /** 指定低层使用的协议(例如：http, https, ftp)
    */
    DogStringA m_scheme;

    /** 用户名称
    */
    DogStringA m_user;

    /** 密码
    */
    DogStringA m_password;

    /** 服务器（通常为域名，有时为IP地址）
    */
    DogStringA m_host;

    /** 端口（以数字方式表示，若为HTTP的默认值“:80”可省略）
    */
    unsigned int m_port = 0;

    /** 路径（以“/”字符区别路径中的每一个目录名称）
    */
    DogStringA m_path;

    /** 查询。（GET模式的窗体参数，以“?”字符为起点，每个参数以“&”隔开，再以“=”分开参数名称与数据，通常以UTF8的URL编码，避开字符冲突的问题）
    */
    DogStringA m_query;

    /** 片段。以“#”字符为起点
    */
    DogStringA m_fragment;

    /** 构造函数
    */
    DogUrl()
    {

    }

    /** 构造函数
    @param [in] url 请求的url;"https://www.baidu.com/"
    */
    DogUrl(const DogStringA& constUrl)
    {
        Init(constUrl);
    }

    /** 初始化
    @param [in] url 请求的url;"https://www.baidu.com/"
    */
    void Init(const DogStringA& constUrl)
    {
        // 将 \ 转换为 /
        DogStringA url = constUrl;
        for (size_t i = 0; i < url.length(); ++i)
        {
            if (url[i] == '\\')
            {
                url[i] = '/';
            }
        }

        size_t index = 0;
        size_t len = url.length();

        // 找scheme
        for (size_t i = index; i < len; ++i)
        {
            if (url[i] == ':')
            {
                m_scheme = url.substr(index, i - index);
                index = i + 1;
                break;
            }
        }

        if (m_scheme.empty())
        {
            return;
        }

        // 固定 //
        if (index + 1 >= len ||
            url[index++] != '/' ||
            url[index++] != '/')
        {
            return;
        }

        // [user[:password]@]host[:port] 【[/path] [?query] [#fragment]】
        // 找到扩展符号 “/ ? #”
        size_t hostPortEndIndex = len - 1;
        for (size_t i = index; i < len; ++i)
        {
            if (url[i] == '/' || url[i] == '?' || url[i] == '#')
            {
                hostPortEndIndex = i - 1;
                break;
            }
        }

        // 找到@符号，说明有用户名密码
        {
            size_t userPswEndIndex = index - 1;
            for (size_t i = index; i <= hostPortEndIndex; ++i)
            {
                if (url[i] == '@')
                {
                    userPswEndIndex = i - 1;
                    break;
                }
            }

            if (userPswEndIndex >= index)
            {
                // 寻找 “:”
                size_t userEndIndex = userPswEndIndex;
                for (size_t j = index; j <= userPswEndIndex; ++j)
                {
                    if (url[j] == ':')
                    {
                        userEndIndex = j - 1;
                        break;
                    }
                }

                m_user = url.substr(index, userEndIndex - index + 1);

                if (userEndIndex < userPswEndIndex - 2)
                {
                    m_password = url.substr(userEndIndex + 2, userPswEndIndex - userEndIndex - 1);
                }

                index = userPswEndIndex + 2;
            }
        }

        // host 端口
        {
            if (index < hostPortEndIndex)
            {
                // 寻找 “:”
                size_t hostEndIndex = hostPortEndIndex;
                for (size_t j = index; j <= hostPortEndIndex; ++j)
                {
                    if (url[j] == ':')
                    {
                        hostEndIndex = j - 1;
                        break;
                    }
                }

                m_host = url.substr(index, hostEndIndex - index + 1);

                if (hostEndIndex < hostPortEndIndex - 2)
                {
                    m_port = ::atoi(url.substr(hostEndIndex + 2, hostPortEndIndex - hostEndIndex - 1).c_str());
                }
                else
                {
                    m_port = 80;
                }

                index = hostPortEndIndex + 2;
            }
        }

        // 找到扩展符号 “/ ? #”
        while (index < len)
        {
            size_t extendEndIndex = len - 1;
            for (size_t i = index; i < len; ++i)
            {
                if (url[i] == '/' || url[i] == '?' || url[i] == '#')
                {
                    extendEndIndex = i - 1;
                    break;
                }
            }

            // 如果上一个符号是 "/" ，找到扩展符号 “? #”
            if (url[index - 1] == '/')
            {
                size_t pathEndIndex = len - 1;
                for (size_t i = index; i < len; ++i)
                {
                    if (url[i] == '?' || url[i] == '#')
                    {
                        pathEndIndex = i - 1;
                        break;
                    }
                }

                m_path = url.substr(index, pathEndIndex - index + 1);
            }

            // 如果上一个符号是 "?" 那么填充m_query
            else if (url[index - 1] == '?')
            {
                m_query = url.substr(index, extendEndIndex - index + 1);
            }

            // 如果上一个符号是 "#" 那么填充m_fragment
            else if (url[index - 1] == '#')
            {
                m_fragment = url.substr(index, extendEndIndex - index + 1);
            }

            index = extendEndIndex + 2;
        }
    }

    /** url 是否有效
    @return 是否有效
    */
    bool IsValid()
    {
        return !m_scheme.empty() && m_port != 0 && !m_host.empty();
    }
};

#endif //__URL_H_
