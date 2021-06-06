
#ifndef __SOCKET_NODE_H_
#define __SOCKET_NODE_H_

#include "typedef/DogString.h"
#include <WINSOCK2.H>

#pragma comment(lib, "Ws2_32.lib")

/** socket 数据传输类型
*/
enum class SocketType
{
    /** 字节流，比如TCP
    */
    SOCKET_STREAM = 1,

    /** 离散数据，比如UDP
    */
    SOCKET_DGRAM = 2
};

/** 协议，目前只提供tcp，udp
*/
enum class IpProtocolType
{
    /** TCP
    */
    IPPROTOCOL_TCP = 6,

    /** UDP
    */
    IPPROTOCOL_UDP = 17
};

/** socket 数据对象
*/
class SocketBean
{
public:
    SocketBean() :
        m_socket(INVALID_SOCKET),
        m_ipAddress(""),
        m_port(0)
    {

    }

public:
    /** socket 是否有效
    return 是否有效
    */
    inline bool IsValidSocket() const
    {
        return (INVALID_SOCKET != m_socket);
    }

    /** 获得socket
    return socket
    */
    SOCKET GetSocket() const
    {
        return m_socket;
    }

    /** 设置socket
    @param [in] socket
    */
    inline void SetSocket(SOCKET socket)
    {
        m_socket = socket;
    }

    /** 获得IP地址，对于不同的socket有着不同的意义，
    @param [out] ipAddress ip 地址
    */
    inline void GetIpAddress(DogStringA& ipAddress) const
    {
        ipAddress = m_ipAddress;
    }

    /** 设置IP地址，仅仅是设置而已
    @param [in] ipAddress ip 地址
    */
    inline void SetIpAddress(const DogStringA& ipAddress)
    {
        m_ipAddress = ipAddress;
    }

    /** 获得端口，对于不同的 socket 有着不同的意义，
    @param [out] port 端口
    */
    inline short GetPort() const
    {
         return m_port;
    }

    /** 设置端口，仅仅是设置而已
    @param [in] port 端口
    */
    inline void SetPort(unsigned short port)
    {
        m_port = port;
    }

    /** 获得SOCKADDR，对于不同的socket有着不同的意义，
    @param [out] socketAddr SOCKADDR
    */
    inline void GetSOCKADDR(SOCKADDR& socketAddr) const
    {
        sockaddr_in* pPockAddr = (sockaddr_in*)(&socketAddr);
        memset(pPockAddr, 0, sizeof(sockaddr_in));
        pPockAddr->sin_family = PF_INET;
        pPockAddr->sin_addr.s_addr = inet_addr(m_ipAddress.c_str());
        pPockAddr->sin_port = htons(m_port);
    }

    /** 设置SOCKADDR
    @param [in] socketAddr SOCKADDR
    */
    inline void SetSOCKADDR(const SOCKADDR& socketAddr)
    {
        sockaddr_in* pPockAddr = (sockaddr_in*)(&socketAddr);
        m_ipAddress = inet_ntoa(pPockAddr->sin_addr);
        m_port = pPockAddr->sin_port;
    }

    /** 获得连接流的另一端信息，很显然类似UDP的链接不需要使用这个
    @param [out] fromSocket 另一端信息
    return 是否成功
    */
    bool GetPeerName(SocketBean& fromSocket)
    {
        if (!IsValidSocket())
        {
            return false;
        }

        SOCKADDR addr;
        int size = sizeof(addr);

        if (SOCKET_ERROR == ::getpeername(GetSocket(), &addr, &size))
        {
            return false;
        }

        fromSocket.SetSOCKADDR(addr);
        return true;
    }

private:
    /** 套接字
    */
    SOCKET m_socket;

    /** ip 地址，对于不同的socket有着不同的意义，
        比如服务器的socket这个字段没有实际意义
        比如客户端的socket这个字段表示他链接的服务器地址
    */
    DogStringA m_ipAddress;

    /** 端口
    */
    unsigned short m_port;
};

#endif //__SOCKET_NODE_H_
