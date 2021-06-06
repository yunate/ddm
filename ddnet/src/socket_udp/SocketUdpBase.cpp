#include "SocketUdpBase.h"

SocketUdpBase::SocketUdpBase()
{
}

SocketUdpBase::~SocketUdpBase()
{
}

int SocketUdpBase::SendMsg(const DogStringA& ip, unsigned short port, const DogStringA& msg)
{
    SocketBean bean;
    bean.SetIpAddress(ip);
    bean.SetPort(port);
    SOCKADDR socketAddr;
    bean.GetSOCKADDR(socketAddr);
    return SendMsg(socketAddr, msg);
}

int SocketUdpBase::SendMsg(const SOCKADDR & sockAddr, const DogStringA& msg)
{
    if (GetSocketBean().IsValidSocket())
    {
        return ::sendto(GetSocketBean().GetSocket(), msg.c_str(), (int)msg.size(), 0, &sockAddr, sizeof(sockAddr));
    }

    return SOCKET_ERROR;
}

int SocketUdpBase::RcvMsg(DogStringA& msg, DogStringA& ip, unsigned short& port)
{
    SOCKADDR sockAddr;
    int rtn = RcvMsg(msg, sockAddr);

    if (SOCKET_ERROR == rtn)
    {
        return rtn;
    }

    SocketBean bean;
    bean.SetSOCKADDR(sockAddr);
    bean.GetIpAddress(ip);
    port = bean.GetPort();
    return rtn;
}

int SocketUdpBase::RcvMsg(DogStringA& msg, SOCKADDR & sockAddr)
{
    unsigned int buffSize = GetBuffSize();
    char* pBuff = new (std::nothrow) char[buffSize];

    if (NULL == pBuff)
    {
        return 0;
    }

    ::memset(pBuff, 0, buffSize);
    int rcvSize = 0;

    if (GetSocketBean().IsValidSocket())
    {
        int size = sizeof(SOCKADDR);
        SOCKADDR socketAddr;
        rcvSize = ::recvfrom(GetSocketBean().GetSocket(), pBuff, buffSize, 0, &socketAddr, &size);

        if (0 != rcvSize)
        {
            sockAddr = socketAddr;
            msg.append(pBuff, rcvSize);
        }
    }

    delete[]pBuff;
    pBuff = NULL;
    return rcvSize;
}

