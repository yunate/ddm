#include "SocketUdpClient.h"

SocketUdpClient::SocketUdpClient()
{
    if (!Init(SocketType::SOCKET_DGRAM, IpProtocolType::IPPROTOCOL_UDP))
    {
        return;
    }
}

SocketUdpClient::~SocketUdpClient()
{
}
