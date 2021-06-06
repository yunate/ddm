
#ifndef __SOCKET_NODE_H_
#define __SOCKET_NODE_H_

#include "typedef/DogString.h"
#include <WINSOCK2.H>

#pragma comment(lib, "Ws2_32.lib")

/** socket ���ݴ�������
*/
enum class SocketType
{
    /** �ֽ���������TCP
    */
    SOCKET_STREAM = 1,

    /** ��ɢ���ݣ�����UDP
    */
    SOCKET_DGRAM = 2
};

/** Э�飬Ŀǰֻ�ṩtcp��udp
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

/** socket ���ݶ���
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
    /** socket �Ƿ���Ч
    return �Ƿ���Ч
    */
    inline bool IsValidSocket() const
    {
        return (INVALID_SOCKET != m_socket);
    }

    /** ���socket
    return socket
    */
    SOCKET GetSocket() const
    {
        return m_socket;
    }

    /** ����socket
    @param [in] socket
    */
    inline void SetSocket(SOCKET socket)
    {
        m_socket = socket;
    }

    /** ���IP��ַ�����ڲ�ͬ��socket���Ų�ͬ�����壬
    @param [out] ipAddress ip ��ַ
    */
    inline void GetIpAddress(DogStringA& ipAddress) const
    {
        ipAddress = m_ipAddress;
    }

    /** ����IP��ַ�����������ö���
    @param [in] ipAddress ip ��ַ
    */
    inline void SetIpAddress(const DogStringA& ipAddress)
    {
        m_ipAddress = ipAddress;
    }

    /** ��ö˿ڣ����ڲ�ͬ�� socket ���Ų�ͬ�����壬
    @param [out] port �˿�
    */
    inline short GetPort() const
    {
         return m_port;
    }

    /** ���ö˿ڣ����������ö���
    @param [in] port �˿�
    */
    inline void SetPort(unsigned short port)
    {
        m_port = port;
    }

    /** ���SOCKADDR�����ڲ�ͬ��socket���Ų�ͬ�����壬
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

    /** ����SOCKADDR
    @param [in] socketAddr SOCKADDR
    */
    inline void SetSOCKADDR(const SOCKADDR& socketAddr)
    {
        sockaddr_in* pPockAddr = (sockaddr_in*)(&socketAddr);
        m_ipAddress = inet_ntoa(pPockAddr->sin_addr);
        m_port = pPockAddr->sin_port;
    }

    /** �������������һ����Ϣ������Ȼ����UDP�����Ӳ���Ҫʹ�����
    @param [out] fromSocket ��һ����Ϣ
    return �Ƿ�ɹ�
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
    /** �׽���
    */
    SOCKET m_socket;

    /** ip ��ַ�����ڲ�ͬ��socket���Ų�ͬ�����壬
        �����������socket����ֶ�û��ʵ������
        ����ͻ��˵�socket����ֶα�ʾ�����ӵķ�������ַ
    */
    DogStringA m_ipAddress;

    /** �˿�
    */
    unsigned short m_port;
};

#endif //__SOCKET_NODE_H_
