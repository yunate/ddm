
#ifndef __HTTP_RESPONSE_H_
#define __HTTP_RESPONSE_H_

#include "HttpCommon.hpp"
#include "string_utils/string_utils_s.hpp"

/** ͷ��������ɻص�
@note: �����ṩһ�������������������֮ǰ��head�ļ����ᣬ���������response��������޸�
@functional:
    @param [in] response �ظ����󣬴�ʱ�����з���ͷ���Ѿ��������
    @reutrn ���Ϊfalse����ֹͣ����
*/
class HttpResponse;
using HttpResponseAllHeadCB = std::function<bool(HttpResponse& response)>;

/** �������ظ������ཫ�ظ���ŵ������棬����ֻ��ͷ�����д���
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

    /** ���ûظ�ͷ��
    @param [in] responseHead �ظ�ͷ��
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

    /** ���״̬��
    @return ״̬��
    */
    inline int GetStatusCode()
    {
        return m_statusCode;
    }

    /** ���ͷ��ԭʼ��Ϣ
    @return ͷ��ԭʼ��Ϣ
    */
    inline DogStringA& GetRawHead()
    {
        return m_rawHead;
    }

    /** ������ĳ���
    @reutrn ���ĳ���
    */
    inline s64 GetContentLength()
    {
        return m_contentLength;
    }

    /** ���ã�������ͷ����ȫ���ܺ�Ļص�
    @param [in] allHeadCallBack �ص�����
    */
    inline void SetAllHeadCallBack(const HttpResponseAllHeadCB& allHeadCallBack)
    {
        m_allHeadCallBack = allHeadCallBack;
    }

    /** ����һ���ص���ÿ�ν����·�������
    @note �������false��ֹͣ����,ע�ⲻ��ֹͣ��������
    @param [in] pBuff ���ݻ���
    @param [in] buffSize
    @return �������false��ֹͣ����
    */
    inline bool RcvBuff(u8* pBuff, u32 buffSize)
    {
        u32 rcvSize = buffSize;

        // ����ͷ��,ͷ���� \r\n\r\n ����
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

                        // ��ͷ����飬����response�������һ������
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
                // �����µ����
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
    /** ״̬��
    */
    int m_statusCode;

    /** ԭʼͷ��
    */
    DogStringA m_rawHead;

    /** ͷ�����ܽ�������
    */
    bool m_hasRcvAllHead;

    /** body ����
    */
    s64 m_contentLength;

    /** ������ͷ����ȫ�����˵Ļص����������������������֮ǰ�����һ���޸�
    */
    HttpResponseAllHeadCB m_allHeadCallBack;
};

#endif //__HTTP_RESPONSE_H_
