
#ifndef __SOCKETUDP_SERVER_H_
#define __SOCKETUDP_SERVER_H_

#include "SocketUdpBase.h"

class SocketUdpServer :
    public SocketUdpBase
{
public:
    /** ���캯��
    @param [in] ip ����������ip��ַ "0" ��ʾ����
    @param [in] port �����������˿�
    */
    SocketUdpServer(const DogStringA& ip, unsigned short port);

    /** ��������
    */
    ~SocketUdpServer();
};

#endif // __SOCKETUDP_SERVER_H_

