
#ifndef __SOCKET_TCP_CLIENT_H_
#define __SOCKET_TCP_CLIENT_H_

#include "SocketTcpBase.h"

class SocketTcpClient :
    public SocketTcpBase
{
public:
    /** 构造函数
    */
    SocketTcpClient();

    /** 构造函数
    @param [in] ip 服务器ip地址
    @param [in] port 服务器监听端口
    */
    SocketTcpClient(const DogStringA& ip, unsigned short port);

    /** 析构函数
    */
    virtual ~SocketTcpClient();

public:
    /** 连接到 [服务器等]
        初始化socket -> 连接
    @return 是否成功
    */
    bool Connect();

    /**
    @param [in] ip 服务器ip地址
    @param [in] port 服务器监听端口
    */
    bool Connect(const DogStringA& ip, unsigned short port);
};

#endif // __SOCKET_TCP_CLIENT_H_


