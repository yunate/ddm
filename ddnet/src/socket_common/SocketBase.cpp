#include "SocketBase.h"

SocketBase::SocketBase() :
    m_buffSize(1024)
{
}

bool SocketBase::Init(SocketType type, IpProtocolType ipProto)
{
    GetSocketBean().SetSocket(::socket(PF_INET, (int)type, (int)ipProto));
    return GetSocketBean().IsValidSocket();
}

bool SocketBase::UnInit()
{
    if (GetSocketBean().IsValidSocket())
    {
        if (SOCKET_ERROR != ::closesocket(GetSocketBean().GetSocket()))
        {
            GetSocketBean().SetSocket(INVALID_SOCKET);
            return true;
        }
    }

    return false;
}
