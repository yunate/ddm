
#ifndef __SOCKETUDP_SERVER_H_
#define __SOCKETUDP_SERVER_H_

#include "SocketUdpBase.h"

class SocketUdpServer :
    public SocketUdpBase
{
public:
    /** 构造函数
    @param [in] ip 服务器监听ip地址 "0" 表示任意
    @param [in] port 服务器监听端口
    */
    SocketUdpServer(const DogStringA& ip, unsigned short port);

    /** 析构函数
    */
    ~SocketUdpServer();
};

#endif // __SOCKETUDP_SERVER_H_

