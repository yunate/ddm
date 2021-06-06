
#ifndef __URL_H_
#define __URL_H_

#include "typedef/DogString.h"

/** url
scheme:[//[user[:password]@]host[:port]] [/path] [?query] [#fragment]
*/
struct DogUrl
{
    /** ָ���Ͳ�ʹ�õ�Э��(���磺http, https, ftp)
    */
    DogStringA m_scheme;

    /** �û�����
    */
    DogStringA m_user;

    /** ����
    */
    DogStringA m_password;

    /** ��������ͨ��Ϊ��������ʱΪIP��ַ��
    */
    DogStringA m_host;

    /** �˿ڣ������ַ�ʽ��ʾ����ΪHTTP��Ĭ��ֵ��:80����ʡ�ԣ�
    */
    unsigned int m_port = 0;

    /** ·�����ԡ�/���ַ�����·���е�ÿһ��Ŀ¼���ƣ�
    */
    DogStringA m_path;

    /** ��ѯ����GETģʽ�Ĵ���������ԡ�?���ַ�Ϊ��㣬ÿ�������ԡ�&�����������ԡ�=���ֿ��������������ݣ�ͨ����UTF8��URL���룬�ܿ��ַ���ͻ�����⣩
    */
    DogStringA m_query;

    /** Ƭ�Ρ��ԡ�#���ַ�Ϊ���
    */
    DogStringA m_fragment;

    /** ���캯��
    */
    DogUrl()
    {

    }

    /** ���캯��
    @param [in] url �����url;"https://www.baidu.com/"
    */
    DogUrl(const DogStringA& constUrl)
    {
        Init(constUrl);
    }

    /** ��ʼ��
    @param [in] url �����url;"https://www.baidu.com/"
    */
    void Init(const DogStringA& constUrl)
    {
        // �� \ ת��Ϊ /
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

        // ��scheme
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

        // �̶� //
        if (index + 1 >= len ||
            url[index++] != '/' ||
            url[index++] != '/')
        {
            return;
        }

        // [user[:password]@]host[:port] ��[/path] [?query] [#fragment]��
        // �ҵ���չ���� ��/ ? #��
        size_t hostPortEndIndex = len - 1;
        for (size_t i = index; i < len; ++i)
        {
            if (url[i] == '/' || url[i] == '?' || url[i] == '#')
            {
                hostPortEndIndex = i - 1;
                break;
            }
        }

        // �ҵ�@���ţ�˵�����û�������
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
                // Ѱ�� ��:��
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

        // host �˿�
        {
            if (index < hostPortEndIndex)
            {
                // Ѱ�� ��:��
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

        // �ҵ���չ���� ��/ ? #��
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

            // �����һ�������� "/" ���ҵ���չ���� ��? #��
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

            // �����һ�������� "?" ��ô���m_query
            else if (url[index - 1] == '?')
            {
                m_query = url.substr(index, extendEndIndex - index + 1);
            }

            // �����һ�������� "#" ��ô���m_fragment
            else if (url[index - 1] == '#')
            {
                m_fragment = url.substr(index, extendEndIndex - index + 1);
            }

            index = extendEndIndex + 2;
        }
    }

    /** url �Ƿ���Ч
    @return �Ƿ���Ч
    */
    bool IsValid()
    {
        return !m_scheme.empty() && m_port != 0 && !m_host.empty();
    }
};

#endif //__URL_H_
