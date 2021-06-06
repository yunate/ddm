
#ifndef __HTTP_REQUEST_HPP_
#define __HTTP_REQUEST_HPP_

#include "HttpCommon.hpp"

#include "Url/DogUrl.h"

#include <map>

/** ���������ַ���
*/
static const char* const MethodStr[] = {"GET", "HEAD", "POST", "DELETE", "TRACE", "CONNECT"};

class HttpRequest:
    public HttpAssister
{
public:
    /** ��������
    */
    enum Method
    {
        GET = 0,
        HEAD,
        POST,
        PUT,
        DEL,
        TRACE,
        CONNECT
    };

    /** ���캯��
    */
    HttpRequest()
    {
    }

    /** ���캯��
    @param [in] url url����
    @param [in] method ��������
    */
    HttpRequest(Method method, const DogUrl& url)
    {
        m_method = method;
        m_url = url;
    }

    /** ���캯��
    @param [in] urlStr url�ַ���
    */
    HttpRequest(Method method, const DogStringA& urlStr)
    {
        m_method = method;
        ParseUrl(urlStr, m_url);
    }

    /** ������󷽷�
    @return ���󷽷�
    */
    inline Method GetMethod() const
    {
        return m_method;
    }

    /** �Ƿ�֧��SSL,�Ƿ���https
    @return �Ƿ�֧��SSL
    */
    inline bool SupportSSL() const
    {
        if (m_url.m_scheme.length() == 0)
        {
            return false;
        }

        DogCharA s = m_url.m_scheme[m_url.m_scheme.length() - 1];
        return (s == 's' || s == 'S');
    }

    /** ƴ������ͷ
    @param [out] headStr ���ؽ��
    */
    inline void Str(DogStringA& headStr) 
    {
        // ƴ��
        headStr = MethodStr[m_method];
        headStr.append(" ");
        headStr.append(m_url.m_path);

        if (m_url.m_query.length() > 0)
        {
            headStr.append("?");
            headStr.append(m_url.m_query);
        }

        if (m_url.m_fragment.length() > 0)
        {
            headStr.append("#");
            headStr.append(m_url.m_fragment);
        }

        headStr.append(" ");
        DogStringA upperScheme = m_url.m_scheme;
        std::transform(upperScheme.begin(), upperScheme.end(), upperScheme.begin(), ::toupper);
        headStr.append(upperScheme);
        headStr.append("/1.1\r\n");
        headStr.append("Host: ");
        headStr.append(m_url.m_host);

        if (m_stream != NULL)
        {
            headStr.append("\r\n");
            Delete("Content-Length");
            char len[64] = {0};
            ::sprintf_s(len, "%lld", m_stream->Size());
            headStr.append("Content-Length: ");
            headStr.append(len);
        }

        for (auto& it : m_keyVal)
        {
            headStr.append("\r\n");
            headStr.append(it.first);
            headStr.append(": ");
            headStr.append(it.second);
        }

        headStr.append("\r\n\r\n");
    }

    /** ���URL
    */
    const DogUrl& GetUrl() const
    {
        return m_url;
    }

    /** �Ƿ���һ����Ч��ͷ
    @return �Ƿ���Ч
    */
    inline bool Isvalid()
    {
        if (!m_url.IsValid())
        {
            return false;
        }

        // ������httpЭ��
        DogStringA upperScheme = m_url.m_scheme;
        std::transform(upperScheme.begin(), upperScheme.end(), upperScheme.begin(), ::toupper);

        if (!(upperScheme == "HTTP" || upperScheme == "HTTPS"))
        {
            return false;
        }

        return true;
    }

protected:
    /** ��������
    */
    Method m_method;

    /** url
    */
    DogUrl m_url;
};


#endif //__HTTP_REQUEST_HPP_
