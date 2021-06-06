
#include "SocketUtils.h"

#include <WINSOCK2.H>

bool SocketUtils::GetHostName(std::string& name)
{
    char buff[1024] = {0};
    return (SOCKET_ERROR != ::gethostname(buff, sizeof(buff)));
}

void SocketUtils::GetIpByHostName(const std::string& name, std::vector<DogStringA >& ips)
{
    // 不是字母开头
    if (!::isalpha(name[0]))
    {
        struct in_addr addr;
        addr.s_addr = ::inet_addr(name.c_str());
        if (addr.s_addr != INADDR_NONE)
        {
            ips.push_back(::inet_ntoa(addr));
        }

        return;
    }

    struct hostent* remoteHost = ::gethostbyname(name.c_str());
    if (remoteHost == NULL)
    {
        return;
    }

    if (remoteHost->h_addrtype == AF_INET)
    {
        for (int i = 0; remoteHost->h_addr_list[i] != 0; ++i)
        {
            struct in_addr addr;
            addr.s_addr = *(u_long *)remoteHost->h_addr_list[i];
            ips.push_back(::inet_ntoa(addr));
        }
    }
    else if (remoteHost->h_addrtype == AF_INET6)
    {
        // ？？？
    }
}
