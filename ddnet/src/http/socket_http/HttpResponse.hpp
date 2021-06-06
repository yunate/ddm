
#ifndef __HTTP_RESPONSE_H_
#define __HTTP_RESPONSE_H_

#include "HttpCommon.hpp"
#include "string_utils/string_utils_s.hpp"

/** 头部接收完成回调
@note: 我们提供一次你可以在真正的下载之前对head的检查机会，并允许你对response对象进行修改
@functional:
    @param [in] response 回复对象，此时对象中返回头部已经处理完毕
    @reutrn 如果为false，将停止接受
*/
class HttpResponse;
using HttpResponseAllHeadCB = std::function<bool(HttpResponse& response)>;

/** 服务器回复，该类将回复体放到流里面，本身只对头部进行处理
*/
class HttpResponse :
    public HttpAssister
{
public:

    HttpResponse():
        m_statusCode(0),
        m_contentLength(0),
        m_hasRcvAllHead(false)
    {

    }

    /** 设置回复头部
    @param [in] responseHead 回复头部
    */
    void SetHead(const DogStringA& responseHead)
    {
        const DogCharA* beg = responseHead.c_str();

        // HTTP/1.1 200 OK
        const DogCharA* lf = stringutils_s::GetLine(beg);

        if (lf - beg <= 2)
        {
            return;
        }

        DogStringA line(beg, lf - beg - 2);
        std::vector<DogStringA> out;
        stringutils_s::StrSplit(line.c_str(), " ", out);

        if (out.size() < 3)
        {
            return;
        }

        m_statusCode = ::atoi(out[1].c_str());
        if (0 == m_statusCode)
        {
            return;
        }

        beg = lf;

        while (1)
        {
            const DogCharA* lf = stringutils_s::GetLine(beg);

            if (*lf == 0)
            {
                break;
            }

            if (lf - beg <= 2)
            {
                continue;
            }

            DogStringA line(beg, lf - beg - 2);
            beg = lf;
            const DogCharA* sp = stringutils_s::StrFind(line.c_str(), ": ", 1);

            if (*sp == 0)
            {
                continue;
            }

            Set(DogStringA(line.c_str(), sp - line.c_str()), sp + 2);
        }

        auto& it = m_keyVal.find("Content-Length");

        if (it != m_keyVal.end())
        {
            m_contentLength = ::atoll(it->second.c_str());
        }
    }

    /** 获得状态码
    @return 状态码
    */
    inline int GetStatusCode()
    {
        return m_statusCode;
    }

    /** 获得头部原始信息
    @return 头部原始信息
    */
    inline DogStringA& GetRawHead()
    {
        return m_rawHead;
    }

    /** 获得正文长度
    @reutrn 正文长度
    */
    inline s64 GetContentLength()
    {
        return m_contentLength;
    }

    /** 设置，当返回头部完全接受后的回调
    @param [in] allHeadCallBack 回调函数
    */
    inline void SetAllHeadCallBack(const HttpResponseAllHeadCB& allHeadCallBack)
    {
        m_allHeadCallBack = allHeadCallBack;
    }

    /** 这是一个回调，每次接受下发的数据
    @note 如果返回false就停止接受,注意不是停止整个请求
    @param [in] pBuff 数据缓存
    @param [in] buffSize
    @return 如果返回false就停止接受
    */
    inline bool RcvBuff(u8* pBuff, u32 buffSize)
    {
        u32 rcvSize = buffSize;

        // 接受头部,头部以 \r\n\r\n 结束
        if (!m_hasRcvAllHead)
        {
            u32 i = 0;
            for (; i < rcvSize; ++i)
            {
                if (pBuff[i] == '\r')
                {
                    if (i + 3 < rcvSize &&
                        pBuff[i + 1] == '\n' &&
                        pBuff[i + 2] == '\r' &&
                        pBuff[i + 3] == '\n')
                    {
                        i += 4;
                        m_rawHead.append("\r\n\r\n");
                        SetHead(m_rawHead);
                        m_hasRcvAllHead = true;

                        // 对头部检查，并对response对象最后一次设置
                        if (m_allHeadCallBack != NULL)
                        {
                            if (!m_allHeadCallBack(*this))
                            {
                                return false;
                            }
                        }

                        break;
                    }
                }

                m_rawHead.append(1, pBuff[i]);
            }

            if (!m_hasRcvAllHead || i >= rcvSize)
            {
                return true;
            }
            else
            {
                // 将余下的输出
                rcvSize -= i;
                ::memcpy(pBuff, pBuff + i, rcvSize);
                ::memset(pBuff + rcvSize, 0, buffSize - rcvSize);
            }
        }
 
        if (m_stream)
        {
            m_stream->Write(pBuff, rcvSize);
        }

        if (m_percentCallBack)
        {
            m_percentCallBack(rcvSize, m_contentLength);
        }

        return true;
    }

private:
    /** 状态码
    */
    int m_statusCode;

    /** 原始头部
    */
    DogStringA m_rawHead;

    /** 头部接受结束了吗
    */
    bool m_hasRcvAllHead;

    /** body 长度
    */
    s64 m_contentLength;

    /** 当返回头部完全接受了的回调函数，允许你对真正下载之前做最后一次修改
    */
    HttpResponseAllHeadCB m_allHeadCallBack;
};

#endif //__HTTP_RESPONSE_H_
