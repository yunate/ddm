
#include "SocketTcpClient.h"

#include <assert.h>

SocketTcpClient::SocketTcpClient()
{
}

SocketTcpClient::SocketTcpClient(const DogStringA& ip, unsigned short port)
{
    GetSocketBean().SetIpAddress(ip);
    GetSocketBean().SetPort(port);
}

SocketTcpClient::~SocketTcpClient()
{
}

bool SocketTcpClient::Connect()
{
    // 这里添加断言，防止用户在没有设置ip 端口号时候使用这个
    assert(GetSocketBean().GetPort() != 0);

    if (!Init(SocketType::SOCKET_STREAM, IpProtocolType::IPPROTOCOL_TCP))
    {
        return false;
    }

    SOCKADDR socketAddr;
    GetSocketBean().GetSOCKADDR(socketAddr);
    return (0 == ::connect(GetSocketBean().GetSocket(), &socketAddr, sizeof(socketAddr)));
}

bool SocketTcpClient::Connect(const DogStringA& ip, unsigned short port)
{
    GetSocketBean().SetIpAddress(ip);
    GetSocketBean().SetPort(port);
    return Connect();
}
