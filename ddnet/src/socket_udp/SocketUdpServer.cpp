#include "SocketUdpServer.h"

SocketUdpServer::SocketUdpServer(const DogStringA& ip, unsigned short port)
{
    if (!Init(SocketType::SOCKET_DGRAM, IpProtocolType::IPPROTOCOL_UDP))
    {
        return;
    }

    GetSocketBean().SetIpAddress(ip);
    GetSocketBean().SetPort(port);
    SOCKADDR socketAddr;
    GetSocketBean().GetSOCKADDR(socketAddr);

    if (0 != ::bind(GetSocketBean().GetSocket(), &socketAddr, sizeof(socketAddr)))
    {
        return;
    }
}

SocketUdpServer::~SocketUdpServer()
{
}
