
#include "SocketHttpClient.h"
#include "socket_common/SocketUtils.h"
#include "socket_tcp/SocketTcpClient.h"

#include <assert.h>

/** 数据超时时间 (ms)
*/
static int DATA_TIME_OUT = 15000;

SocketHttpClient::SocketHttpClient():
    m_gTimeOut(0),
    m_dataTimeOut(DATA_TIME_OUT),
    m_stop(false)
{
}

SocketHttpClient::~SocketHttpClient()
{
}

void SocketHttpClient::SetRequest(const SPRequest & request)
{
    m_spRequest = request;
}

SPRequest & SocketHttpClient::GetRequest()
{
    return m_spRequest;
}

void SocketHttpClient::SetResponse(const SPResponse& response)
{
    m_spReponse = response;
}

SPResponse & SocketHttpClient::GetResponse()
{
    return m_spReponse;
}

bool SocketHttpClient::MakeRequest()
{
    if (m_spRequest == NULL || !m_spRequest->Isvalid())
    {
        return false;
    }

    m_stop = false;

    // 超时记录
    m_gTimer.ReSet();

    // 获得ip
    std::vector<DogStringA> ips;
    SocketUtils::GetIpByHostName(m_spRequest->GetUrl().m_host, ips);

    if (ips.size() == 0)
    {
        return false;
    }

    SocketTcpClient* pClient = new(std::nothrow) SocketTcpClient(ips[0], m_spRequest->GetUrl().m_port);

    if (pClient == NULL || !pClient->Connect())
    {
        return false;
    }

    SPSocketClient spClient(pClient);

    // 解决SSL?
    bool isSafe = m_spRequest->SupportSSL();

    // 发送头部
    DogStringA strHttpHead;
    m_spRequest->Str(strHttpHead);

    if (::send(spClient->GetSocketBean().GetSocket(), strHttpHead.c_str(), (int)strHttpHead.length(), 0)
        != strHttpHead.length())
    {
        return false;
    }

    // 发送body
    if (m_spRequest->GetStream() != NULL)
    {
        if (!SendBody(spClient))
        {
            return false;
        }
    }

    // 接受
    DogStringA responseHead;
    if (!RecvResponse(spClient))
    {
        return false;
    }

    return true;
}

bool SocketHttpClient::RecvResponse(SPSocketClient spClient)
{
    // 接受
    assert(spClient !=NULL && spClient->GetSocketBean().IsValidSocket());
    SOCKET sock = spClient->GetSocketBean().GetSocket();

    // 设置异步接受
    int iMode = 1;
    if (SOCKET_ERROR == ::ioctlsocket(sock, FIONBIO, (u_long FAR*)&iMode))
    {
        return false;
    }

    unsigned int buffSize = spClient->GetBuffSize();
    u8* pBuff = new (std::nothrow) u8[buffSize];

    if (NULL == pBuff)
    {
        return false;
    }

    // 服务器响应了吗？当第一组数据到达的时候认为服务器响应了
    bool hasResponse = false;
    bool success = true;

    // 返回数据头部已经全部接受完毕了吗？
    bool hasGetAllResponseHead = false;

    // 记录上一次数据到来的时间
    xtimer dataTimer;


    while (true)
    {
        if (m_stop)
        {
            success = false;
            break;
        }

        if (m_gTimeOut > 0)
        {
            if (m_gTimer.get_time_pass() >= m_gTimeOut)
            {
                success = false;
                break;
            }
        }

        if (m_dataTimeOut > 0)
        {
            if (dataTimer.get_time_pass() >= m_dataTimeOut)
            {
                success = false;
                break;
            }
        }

        int rcvSize = 0;
        ::memset(pBuff, 0, buffSize);
        rcvSize = ::recv(sock, (char*)pBuff, buffSize, 0);

        if (rcvSize > 0)
        {
            hasResponse = true;

            if (!m_spReponse->RcvBuff(pBuff, rcvSize))
            {
                success = false;
                break;
            }

            dataTimer.ReSet();
        }
        else if (rcvSize == 0)
        {
            // 等于0 说明接受完成了
            break;
        }
        else
        {
            // 服务器还没有响应 ||
            // 缓冲区繁忙 WSAGetLastError 函数是本线程中的所有socket，所以不会有多线程问题
            // 我们先等着
            if (!hasResponse ||
                ::WSAGetLastError() == WSAEWOULDBLOCK)
            {
                ::Sleep(1);
                continue;
            }

            success = false;
            break;
        }
    }

    delete pBuff;
    pBuff = NULL;
    return success;
}

bool SocketHttpClient::SendBody(SPSocketClient spClient)
{
    assert(spClient != NULL && spClient->GetSocketBean().IsValidSocket());
    unsigned int buffSize = spClient->GetBuffSize();
    u8* pBuff = new (std::nothrow) u8[buffSize];
    SOCKET sock = spClient->GetSocketBean().GetSocket();

    if (NULL == pBuff)
    {
        return false;
    }

    DogPercentCallBack& callback = m_spRequest->GetPercentCallBack();
    SPDogStream& stream = m_spRequest->GetStream();

    // 记录上一次数据到来的时间
    xtimer dataTimer;
    bool success = true;
    u64 sendSize = 0;
    u64 allSize = stream->Size();
    
    while (true)
    {
        if (m_stop)
        {
            success = false;
            break;
        }

        if (m_gTimeOut > 0)
        {
            if (m_gTimer.get_time_pass() >= m_gTimeOut)
            {
                success = false;
                break;
            }
        }

        if (m_dataTimeOut > 0)
        {
            if (dataTimer.get_time_pass() >= m_dataTimeOut)
            {
                success = false;
                break;
            }
        }

        s32 readed = stream->Read(pBuff, buffSize);

        if (readed == 0)
        {
            break;
        }

        if (::send(sock, (char*)pBuff, (int)readed, 0) != readed)
        {
            success = false;
            break;
        }

        sendSize += readed;

        if (callback)
        {
            callback(sendSize, allSize);
        }

        dataTimer.ReSet();
    }

    delete pBuff;
    pBuff = NULL;
    return success;
}

void SocketHttpClient::SetGTimeOut(u32 timeOut)
{
    m_gTimeOut = timeOut;
}

void SocketHttpClient::SetDataTimeOut(u32 timeOut)
{
    m_dataTimeOut = timeOut;
}

void SocketHttpClient::Stop()
{
    m_stop = true;
}
