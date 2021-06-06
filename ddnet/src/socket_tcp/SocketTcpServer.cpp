
#include "SocketTcpServer.h"

SocketTcpServer::SocketTcpServer(const DogStringA& ip, unsigned short port)
{
    GetSocketBean().SetIpAddress(ip);
    GetSocketBean().SetPort(port);
}

SocketTcpServer::~SocketTcpServer()
{
}

bool SocketTcpServer::Listen(unsigned int backlog)
{
    if (!Init(SocketType::SOCKET_STREAM, IpProtocolType::IPPROTOCOL_TCP))
    {
        return false;
    }

    SOCKADDR socketAddr;
    GetSocketBean().GetSOCKADDR(socketAddr);

    if (0 != ::bind(GetSocketBean().GetSocket(), &socketAddr, sizeof(socketAddr)))
    {
        return false;
    }

    return (0 == ::listen(GetSocketBean().GetSocket(), backlog));
}

std::shared_ptr<SocketTcpBase> SocketTcpServer::Accept()
{
    if (!GetSocketBean().IsValidSocket())
    {
        return NULL;
    }

    SOCKADDR addr;
    int size = sizeof(SOCKADDR);
    SOCKET clientSocket = ::accept(GetSocketBean().GetSocket(), &addr, &size);

    if (INVALID_SOCKET == clientSocket)
    {
        return NULL;
    }

    std::shared_ptr<SocketTcpBase> spClient(new (std::nothrow) SocketTcpBase());

    if (NULL == spClient)
    {
        return NULL;
    }

    spClient->GetSocketBean().SetSOCKADDR(addr);
    spClient->GetSocketBean().SetSocket(clientSocket);
    return spClient;
}
