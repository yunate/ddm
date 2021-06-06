#include "SocketTcpBase.h"

SocketTcpBase::SocketTcpBase()
{
}

SocketTcpBase::~SocketTcpBase()
{
}

int SocketTcpBase::SendMsg(const DogStringA& msg)
{
    if (GetSocketBean().IsValidSocket())
    {
        return ::send(GetSocketBean().GetSocket(), msg.c_str(), (int)msg.size(), 0);
    }

    return SOCKET_ERROR;
}

int SocketTcpBase::RcvMsg(DogStringA& msg)
{
    unsigned int buffSize = GetBuffSize();
    char* pBuff = new (std::nothrow) char[buffSize];

    if (NULL == pBuff)
    {
        return 0;
    }

    int rcvSize = 0;

    if (GetSocketBean().IsValidSocket())
    {
        ::memset(pBuff, 0, buffSize);
        rcvSize = ::recv(GetSocketBean().GetSocket(), pBuff, buffSize, 0);

        if (rcvSize > 0)
        {
            msg.append(pBuff, rcvSize);
        }
    }
    
    delete[]pBuff;
    pBuff = NULL;
    return rcvSize;
}

void SocketTcpBase::RcvMsg(std::function<bool(char* pBuff, unsigned int buffSize)> callBack)
{
    unsigned int buffSize = GetBuffSize();
    char* pBuff = new (std::nothrow) char[buffSize];

    if (NULL == pBuff)
    {
        return;
    }

    int rcvSize = 0;

    if (GetSocketBean().IsValidSocket())
    {
        do
        {
            ::memset(pBuff, 0, buffSize); 
            rcvSize = ::recv(GetSocketBean().GetSocket(), pBuff, buffSize, 0);

            if (rcvSize <= 0)
            {
                break;
            }

        } while (callBack(pBuff, rcvSize));
    }

    delete[]pBuff;
    pBuff = NULL;
}
