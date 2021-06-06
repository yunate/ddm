
#ifndef __SOCKETUDP_CLIENT_H_
#define __SOCKETUDP_CLIENT_H_

#include "SocketUdpBase.h"

class SocketUdpClient :
    public SocketUdpBase
{
public:
    /** 构造函数
    */
    SocketUdpClient();

    /** 析构函数
    */
    ~SocketUdpClient();
};

#endif // __SOCKETUDP_CLIENT_H_

