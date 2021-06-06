
#ifndef __SOCKET_TCP_SERVER_H_
#define __SOCKET_TCP_SERVER_H_

#include "SocketTcpBase.h"
#include <memory>

class SocketTcpServer :
    public SocketTcpBase
{
public:
    /** 构造函数
    @param [in] ip 服务器监听ip地址 "0" 表示任意
    @param [in] port 服务器监听端口
    */
    SocketTcpServer(const DogStringA& ip, unsigned short port);

    /** 析构函数
    */
    virtual ~SocketTcpServer();

public:
    /** listen
    初始化socket -> bind -> listen
    param [in] backlog 是在进入队列中允许的连接数目 大多数系统的允许数目是20，你也可以设置为5到10。
    @return 是否成功
    */
    bool Listen(unsigned int backlog);

    /** 接受连入的client，内部创建SocketBase
    @return 成功返回新的socket 的智能指针，失败返回NULL
    */
    std::shared_ptr<SocketTcpBase> Accept();
};

#endif // __SOCKET_TCP_SERVER_H_


